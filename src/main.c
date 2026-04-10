/**
 * @file    main.c
 * @brief   sample vio implementation with LCD_24BIT support (800x600@120Hz)
 * @details VII -> VPP -> VO (LCD 24bit) Single Pipeline
 * @author  Visinex Software Group (Modified)
 * @date    2024
 */
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <dlfcn.h>
#include <pthread.h>
#include <linux/videodev2.h>

#include "sample_common.h"
#include "vs_mal_vii.h"
#include "vs_mal_vpp.h"

static volatile sig_atomic_t g_stop_flag = 0;

extern vs_int8_t g_bus_id[VII_MAX_ROUTE_NUM];
extern sample_sensor_type_e g_sensor_type[VII_MAX_ROUTE_NUM];

static vs_void_t sample_vii_get_vpp_grp_attr(vs_vpp_grp_attr_s *grp_attr)
{
    // 配置 VPP Group 的最大支持能力
    grp_attr->max_width = 800;
    grp_attr->max_height = 600;
    grp_attr->dynamic_range = E_DYNAMIC_RANGE_SDR8;
    grp_attr->pixel_format = E_PIXEL_FORMAT_YVU_420SP;
    grp_attr->framerate.dst_framerate = -1;
    grp_attr->framerate.src_framerate = -1;
}

static vs_void_t sample_vii_get_vpp_chn_attr(vs_vpp_chn_attr_s *chn_attr)
{
    chn_attr->chn_mode = E_VPP_CHN_MODE_USER;
    chn_attr->video_format = E_VIDEO_FORMAT_LINEAR;
    chn_attr->pixel_format = E_PIXEL_FORMAT_YVU_420SP;
    chn_attr->dynamic_range = E_DYNAMIC_RANGE_SDR8;
    chn_attr->compress_mode = E_COMPRESS_MODE_NONE;
    chn_attr->framerate.src_framerate = -1;
    chn_attr->framerate.dst_framerate = -1;
    chn_attr->mirror_enable = VS_FALSE;
    chn_attr->flip_enable = VS_FALSE;
    chn_attr->depth = 0;
    chn_attr->aspect_ratio.mode = E_ASPECT_RATIO_MODE_NONE;

    // 锁定 VPP 输出分辨率为 800x600
    chn_attr->width = 800;
    chn_attr->height = 600;

    vs_sample_trace("VPP Configured to output: %dx%d\n", chn_attr->width, chn_attr->height);
}

/**
 * @brief 设置 800x600@120Hz LCD 自定义时序
 * @note  Pixel Clock: 74.25 MHz (HTotal=990, VTotal=625)
 */
static vs_void_t sample_set_lcd_800x600_120hz_timing(sample_vo_cfg_s *vo_cfg)
{
    vo_cfg->timing.hactive = 800;
    vo_cfg->timing.hfp = 40;
    vo_cfg->timing.hpw = 40;
    vo_cfg->timing.hbp = 110;
    vo_cfg->timing.vactive = 600;
    vo_cfg->timing.vfp = 3;
    vo_cfg->timing.vpw = 4;
    vo_cfg->timing.vbp = 18;
    
    vo_cfg->timing.hsync_polarity = 1; // 1 = Positive
    vo_cfg->timing.vsync_polarity = 1; // 1 = Positive
    vo_cfg->timing.data_polarity = 1;
    vo_cfg->timing.interlaced_output = 0;

    // PLL 基准 1188MHz, 1188 / 16 = 74.25MHz
    vo_cfg->clk_info.pixel_clk_rate = 74250000UL;
    vo_cfg->clk_info.clk_div = 16; 
}

static vs_void_t sample_vio_get_vo_cfg(sample_vo_cfg_s *vo_cfg)
{
    vs_int32_t i;
    for (i = 0; i < VO_MAX_DEV_NUM; i++) {
        vo_cfg[i].enable = VS_FALSE; // 默认全部关闭
    }
        
    // --- 配置 Display 0 (24-bit LCD) ---
    vo_cfg[0].enable = VS_TRUE;
    vo_cfg[0].vo_devid = 0;
    vo_cfg[0].vo_layerid = 0;
    vo_cfg[0].vo_intf_type = E_VO_INTERFACE_TYPE_LCD_24BIT;
    vo_cfg[0].vo_output = E_VO_OUTPUT_TYPE_USER; // 使用自定义时序
    vo_cfg[0].bg_color = 0x000000;               // 纯黑背景
    vo_cfg[0].dynamic_range = E_DYNAMIC_RANGE_SDR8;
    vo_cfg[0].pixel_format = E_PIXEL_FORMAT_YVU_420SP;
    vo_cfg[0].vo_mode = E_VO_MODE_1MUX;
    vo_cfg[0].zorder = 0;
    vo_cfg[0].rotation_enable = VS_FALSE;
    
    vo_cfg[0].img_width = 800;
    vo_cfg[0].img_height = 600;

    // 注入我们计算好的 120Hz 时序参数
    sample_set_lcd_800x600_120hz_timing(&vo_cfg[0]);
}

vs_int32_t sample_vio_vii_vpp_venc_vo_case(vs_vii_vpp_mode_e vii_vpp_mode)
{
    vs_int32_t ret;
    vs_int32_t blk_cnt;
    vs_int32_t wdr_blk_cnt;
    vs_int32_t sensor_id = 0;
    vs_pixel_format_e pixel_format;
    vs_uint32_t frame_num;
    vs_size_s img_size = {0};
    vs_vb_cfg_s vb_cfg = {0};
    sample_vii_cfg_s vii_cfg = {0};
    
    vs_int32_t vii_pipeid = 0;
    vs_int32_t vii_chnid = 0;
    vs_int32_t vpp_grpid = 0; // 只保留一个 VPP Group

    vs_bool_t chn_enable[VPP_MAX_PHYCHN_NUM] = {VS_TRUE, VS_FALSE, VS_FALSE, VS_FALSE};
    vs_vpp_grp_attr_s vpp_grp_attr = {0};
    vs_vpp_chn_attr_s vpp_chn_attr[VPP_MAX_PHYCHN_NUM];
    
    sample_vo_cfg_s vo_cfg[VO_MAX_DEV_NUM] = {0};
    vs_int32_t sensor_framerate = 120; // 假设 Sensor 已经支持 120fps

    // --- VB Buffer 内存池配置 ---
    switch (vii_vpp_mode) {
        case E_VII_ONLINE_VPP_ONLINE: blk_cnt = 7; break;
        case E_VII_ONLINE_VPP_OFFLINE: blk_cnt = 9; break;
        case E_VII_OFFLINE_VPP_ONLINE: blk_cnt = 10; break;
        case E_VII_OFFLINE_VPP_OFFLINE: blk_cnt = 12; break;
        default: blk_cnt = 12; break;
    }

    sample_common_vii_sensor_framerate_get(sensor_id, &sensor_framerate);
    if (sensor_framerate > 60) {
        blk_cnt += 5; // 高帧率增加缓存
    }
#ifdef VS_ORION
    blk_cnt += 5;
#endif

    switch (vii_vpp_mode) {
        case E_VII_ONLINE_VPP_ONLINE:
        case E_VII_ONLINE_VPP_OFFLINE:
            wdr_blk_cnt = 1;
            break;
        default:
            wdr_blk_cnt = 4;
            break;
    }

    sample_common_vii_sensor_img_size_get(sensor_id, &img_size);
    sample_common_vii_sensor_pixel_format_get(sensor_id, &pixel_format);
    frame_num = sample_common_vii_wdr_frame_num_get(sensor_id);
    
    if ((frame_num > 1) && (wdr_blk_cnt != 0)) {
        vb_cfg.pool_cnt = 2;
        vb_cfg.ast_commpool[0].blk_size = sample_common_buffer_size_get(&img_size, pixel_format, E_COMPRESS_MODE_NONE, frame_num);
        vb_cfg.ast_commpool[0].blk_cnt = wdr_blk_cnt;
        vb_cfg.ast_commpool[0].remap_mode = VB_REMAP_MODE_NONE;

        vb_cfg.ast_commpool[1].blk_size = sample_common_buffer_size_get(&img_size, pixel_format, E_COMPRESS_MODE_NONE, 1);
        vb_cfg.ast_commpool[1].blk_cnt = blk_cnt - wdr_blk_cnt;
        vb_cfg.ast_commpool[1].remap_mode = VB_REMAP_MODE_NONE;
    } else {
        vb_cfg.pool_cnt = 1;
        vb_cfg.ast_commpool[0].blk_size = sample_common_buffer_size_get(&img_size, pixel_format, E_COMPRESS_MODE_NONE, 1);
        vb_cfg.ast_commpool[0].blk_cnt = blk_cnt;
        vb_cfg.ast_commpool[0].remap_mode = VB_REMAP_MODE_NONE;
    }

    ret = sample_common_sys_init(&vb_cfg);
    if (ret != VS_SUCCESS) {
        goto exit0;
    }

    /* 1. 启动 VII */
    vii_cfg.vii_vpp_mode = vii_vpp_mode;
    vii_cfg.route_num = 1;
    sample_common_vii_default_cfg_get(sensor_id, &vii_cfg.route_cfg[0]);
    vii_cfg.route_cfg[0].pipe_cfg[0].pipe_attr.compress_mode = E_COMPRESS_MODE_NONE;
    vii_cfg.route_cfg[0].pipe_cfg[0].phys_chn_cfg[0].chn_attr.compress_mode = E_COMPRESS_MODE_RASTER;

    ret = sample_common_vii_start(&vii_cfg);
    if (ret != VS_SUCCESS) {
        goto exit1;
    }

    /* 2. 启动 VPP Group 0 */
    vs_sample_trace(">>> Configuring VPP Grp %d for LCD 800x600\n", vpp_grpid);
    
    ret = sample_common_vii_bind_vpp(vii_pipeid, vii_chnid, vpp_grpid);
    if (ret != VS_SUCCESS) goto exit2;

    sample_vii_get_vpp_grp_attr(&vpp_grp_attr);
    sample_vii_get_vpp_chn_attr(&vpp_chn_attr[0]);

    ret = sample_common_vpp_start(vpp_grpid, chn_enable, &vpp_grp_attr, vpp_chn_attr);
    if (ret != VS_SUCCESS) goto exit3;

    usleep(100000); 

    /* 3. 获取 VO 配置并启动 (LCD_24BIT) */
    sample_vio_get_vo_cfg(vo_cfg);

    // 仅启动 VO Dev 0
    if (vo_cfg[0].enable) {
        ret = sample_common_vo_start(&vo_cfg[0]);
        if (ret != VS_SUCCESS) {
            vs_sample_trace("sample_common_vo_start dev0 failed\n");
            goto exit4;
        }
    }

    /* 4. 绑定 VPP -> VO */
    if (vo_cfg[0].enable) {
        ret = sample_common_vpp_bind_vo(vpp_grpid, 0, vo_cfg[0].vo_layerid, 0);
        if (ret != VS_SUCCESS) {
            vs_sample_trace("VPP(Grp%d)-VO(Layer%d) bind failed\n", vpp_grpid, vo_cfg[0].vo_layerid);
            goto exit5;
        }
        vs_sample_trace("Bind Success: VPP[%d-0] -> VO[%d-0] (LCD_24BIT 800x600@120Hz)\n", vpp_grpid, vo_cfg[0].vo_layerid);
    }

    // 阻塞在这里，等待外部退出信号
    sample_common_pause();

    // 清理绑定
    if (vo_cfg[0].enable) {
        sample_common_vpp_unbind_vo(vpp_grpid, 0, vo_cfg[0].vo_layerid, 0);
    }

exit5:
    if (vo_cfg[0].enable) {
        sample_common_vo_stop(&vo_cfg[0]);
    }
exit4:
    sample_common_vpp_stop(vpp_grpid, chn_enable);
exit3:
    sample_common_vii_unbind_vpp(vii_pipeid, vii_chnid, vpp_grpid);
exit2:
    sample_common_vii_stop(&vii_cfg);
exit1:
    sample_common_sys_exit();
exit0:
    return ret;
}

// 封装接口供 main 函数调用
vs_int32_t sample_vio_vii_online_vpp_online_case(vs_void_t)
{
    return sample_vio_vii_vpp_venc_vo_case(E_VII_ONLINE_VPP_ONLINE);
}

vs_int32_t sample_vio_vii_online_vpp_offline_case(vs_void_t)
{
    return sample_vio_vii_vpp_venc_vo_case(E_VII_ONLINE_VPP_OFFLINE);
}

vs_int32_t sample_vio_vii_offline_vpp_online_case(vs_void_t)
{
    return sample_vio_vii_vpp_venc_vo_case(E_VII_OFFLINE_VPP_ONLINE);
}

vs_void_t sample_vio_usage(char *prog_name)
{
    vs_uint32_t i;
    vs_int32_t sensor_type_num = sample_common_vii_sensor_type_num_get();
    printf("Usage : %s <index> <sensor_type> [i2c_bus_id]\n", prog_name);
    printf("index:\n");
    printf("\t 0) vii(online)  --> vpp(online)  --> vo(LCD).\n");
    printf("\t 1) vii(online)  --> vpp(offline) --> vo(LCD).\n");
    printf("\t 2) vii(offline) --> vpp(online)  --> vo(LCD).\n");
    
    printf("sensor_type:\n");
    for (i = 0; i < sensor_type_num; i++) {
        printf("\t %d) %s.\n", i, sample_common_sensor_type_name_get(i));
    }
}

static vs_void_t sample_vio_register_signal_handler(void (*sig_handler)(int))
{
    struct sigaction sa;
    memset(&sa, 0, sizeof(struct sigaction));
    sa.sa_handler = sig_handler;
    sa.sa_flags = 0;

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
}

vs_void_t vii_signal_handle(vs_int32_t s_no)
{
    if (s_no == SIGINT || s_no == SIGTERM) {
        g_stop_flag = 1;
    }
}

int main(int argc, char *argv[])
{
    vs_int32_t ret = VS_SUCCESS;
    vs_int32_t case_idx;

    if (argc <= 2) {
        sample_vio_usage(argv[0]);
        return VS_FAILED;
    }

    if (!strncmp(argv[1], "-h", 2)) {
        sample_vio_usage(argv[0]);
        return VS_FAILED;
    }

    sample_vio_register_signal_handler(vii_signal_handle);

    case_idx = atoi(argv[1]);
    g_sensor_type[0] = atoi(argv[2]);

    if (argc > 3 && argv[3][0] != '-') {
        g_bus_id[0] = atoi(argv[3]);
    }

    switch (case_idx) {
        case 0:
            ret = sample_vio_vii_online_vpp_online_case();
            break;
        case 1:
            ret = sample_vio_vii_online_vpp_offline_case();
            break;
        case 2:
            ret = sample_vio_vii_offline_vpp_online_case();
            break;
        default:
            sample_vio_usage(argv[0]);
            return VS_FAILED;
    }

    return ret;
}