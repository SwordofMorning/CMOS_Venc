/**
 * @file    main.c
 * @brief   sample vio implementation with MIPI + CVBS support (Fixed Topology)
 * @details Modified to match vio.c topology: Dual VPP Groups
 * @author  Visinex Software Group (Modified)
 * @date    2022-05-25
 * @version v1.02
 * @Copyright (c) 2022 Shanghai Visinex Technologies Co., Ltd. All rights reserved.
 *
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

static sample_fpn_frame_info_s g_fpn_frame_info;
static volatile sig_atomic_t g_stop_flag = 0;

/* --- CVBS 全局控制变量 --- */
static vs_bool_t g_cvbs_enable = VS_FALSE;
static vs_vo_output_type_e g_cvbs_output_type = E_VO_OUTPUT_TYPE_PAL;
/* -------------------------------- */

extern vs_int8_t g_bus_id[VII_MAX_ROUTE_NUM];
extern sample_sensor_type_e g_sensor_type[VII_MAX_ROUTE_NUM];

static vs_void_t sample_vii_get_vpp_grp_attr(vs_size_s *img_size, vs_vpp_grp_attr_s *grp_attr)
{
    grp_attr->max_width = img_size->width;
    grp_attr->max_height = img_size->height;
    grp_attr->dynamic_range = E_DYNAMIC_RANGE_SDR8;
    grp_attr->pixel_format = E_PIXEL_FORMAT_YVU_420SP;
    grp_attr->framerate.dst_framerate = -1;
    grp_attr->framerate.src_framerate = -1;
}

/**
 * @brief 配置 VPP 通道属性
 * @param mode 0: MIPI Path (Original Size), 1: CVBS Path (Scaled)
 */
static vs_void_t sample_vii_get_vpp_chn_attr(vs_size_s *img_size, vs_vpp_chn_attr_s *chn_attr, vs_int32_t mode)
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

    // 修改点：无论 Mode 0 (MIPI) 还是 Mode 1 (CVBS)，都使用 Sensor 原始分辨率
    // 让 VO Layer 去做缩放，就像 vio.c 那样
    chn_attr->width = img_size->width;
    chn_attr->height = img_size->height;

    vs_sample_trace("VPP config [Mode %d]: %dx%d (Keep Original)\n", mode, chn_attr->width, chn_attr->height);
}

static vs_int32_t sample_vpp_crop_config(vs_int32_t vpp_grpid, vs_int32_t vpp_chnid)
{
    vs_int32_t ret;
    vs_vpp_crop_info_s crop_info = {0};
    
    crop_info.enable = VS_TRUE;
    crop_info.coordinate_mode = E_COORDINATE_MODE_ABSOLUTE;
    crop_info.rect.x = 0;
    crop_info.rect.y = 32;          // 上下各裁剪32行
    crop_info.rect.width = 1280;
    crop_info.rect.height = 960;    // 1024-64=960
    
    ret = vs_mal_vpp_chn_crop_set(vpp_grpid, vpp_chnid, &crop_info);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_vpp_chn_crop_set failed, ret: 0x%x\n", ret);
        return VS_FAILED;
    }
    
    return VS_SUCCESS;
}

static vs_void_t sample_vio_get_vo_cfg(vs_size_s *img_size, sample_vo_cfg_s *vo_cfg)
{
    vs_int32_t i;
    
    // 安全检查，防止空指针崩溃
    vs_int32_t src_w = (img_size) ? img_size->width : 1920;
    vs_int32_t src_h = (img_size) ? img_size->height : 1080;

    for (i = 0; i < VO_MAX_DEV_NUM; i++) {
        vo_cfg[i].enable = VS_FALSE;
        vo_cfg[i].bg_color = 0;
        vo_cfg[i].dynamic_range = E_DYNAMIC_RANGE_SDR8;
        vo_cfg[i].pixel_format = E_PIXEL_FORMAT_YVU_420SP;
        vo_cfg[i].vo_mode = E_VO_MODE_1MUX;
        vo_cfg[i].zorder = 0;
        vo_cfg[i].rotation_enable = VS_FALSE;
        
        // --- Display 0: MIPI ---
        if (i == 0) {
            vo_cfg[i].vo_devid = 0;
            vo_cfg[i].vo_layerid = 0;
            vo_cfg[i].vo_intf_type = E_VO_INTERFACE_TYPE_MIPI;
            vo_cfg[i].vo_output = E_VO_OUTPUT_TYPE_USER;
            vo_cfg[i].img_width = 1280; // MIPI 屏如果是固定分辨率，这里可以写死，或者用 src_w
            vo_cfg[i].img_height = 960;
            vo_cfg[i].mipitx_phy_rate = 820;
            vo_cfg[i].enable = VS_TRUE;
        }
        
        // --- Display 1: CVBS ---
        else if (i == 1 && g_cvbs_enable) {
            vo_cfg[i].vo_devid = 1;
            vo_cfg[i].vo_layerid = 3; 
            vo_cfg[i].vo_intf_type = E_VO_INTERFACE_TYPE_CVBS;
            vo_cfg[i].vo_output = g_cvbs_output_type;
            
            // 修改点：VO Layer 的“输入图像尺寸”设置为 Sensor 原始尺寸
            // VO 硬件会自动将其缩放到 PAL/NTSC 的输出尺寸
            vo_cfg[i].img_width = src_w;
            vo_cfg[i].img_height = src_h;

            vo_cfg[i].enable = VS_TRUE;
        }
    }
}

static vs_void_t  sample_vio_get_venc_cfg(vs_int32_t sensor_id, sample_venc_cfg_s *sample_venc_cfg,
    vs_payload_type_e type, vs_venc_profile_e profile, vs_size_s *frame_size,
    sample_brc_mode_e brc_mode, vs_venc_gop_attr_s *p_gop_attr)
{
    vs_int32_t ret = VS_SUCCESS;
    vs_int32_t sensor_framerate = 30;

    if (sensor_id >= 0) {
        ret = sample_common_vii_sensor_framerate_get(sensor_id, &sensor_framerate);
        if (ret != VS_SUCCESS) {
            vs_sample_trace("sample_common_vii_sensor_framerate_get failed, ret[%d]\n", ret);
        }
    }
    sample_venc_cfg->format = E_PIXEL_FORMAT_YUV_420SP;
    sample_venc_cfg->compress = VS_FALSE;
    sample_venc_cfg->type = type;
    sample_venc_cfg->profile = profile;
    sample_venc_cfg->frame_size.width = frame_size->width;
    sample_venc_cfg->frame_size.height = frame_size->height;
    sample_venc_cfg->brc_mode = brc_mode;
    sample_venc_cfg->frc.dst_framerate = sensor_framerate;
    sample_venc_cfg->frc.src_framerate = sensor_framerate;
    sample_venc_cfg->bandwidth_save_strength = 0;
    if (p_gop_attr != NULL) {
        sample_venc_cfg->gop_attr = *p_gop_attr;
    }
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
    
    /* --- 修改：分离 VPP Group --- */
    vs_int32_t vpp_grp_mipi = 0;
    vs_int32_t vpp_grp_cvbs = 1;
    /* -------------------------- */

    vs_int32_t i;
    /* 定义两个 Channel Enable 数组，分别用于两个 Group */
    vs_bool_t chn_enable_mipi[VPP_MAX_PHYCHN_NUM] = {VS_TRUE, VS_FALSE, VS_FALSE, VS_FALSE};
    vs_bool_t chn_enable_cvbs[VPP_MAX_PHYCHN_NUM] = {VS_TRUE, VS_FALSE, VS_FALSE, VS_FALSE}; 
    // 注意：CVBS 在 Group 1 中也是使用 Channel 0

    vs_vpp_grp_attr_s vpp_grp_attr = {0};
    vs_vpp_chn_attr_s vpp_chn_attr[VPP_MAX_PHYCHN_NUM];
    
    sample_vo_cfg_s vo_cfg[VO_MAX_DEV_NUM] = {0};
    vs_char_t name[100] = "/lib/firmware/vs_dsp0.bin";
    vs_int32_t sensor_framerate = 30;

    switch (vii_vpp_mode) {
        case E_VII_ONLINE_VPP_ONLINE: blk_cnt = 7; break;
        case E_VII_ONLINE_VPP_OFFLINE: blk_cnt = 9; break;
        case E_VII_OFFLINE_VPP_ONLINE: blk_cnt = 10; break;
        case E_VII_OFFLINE_VPP_OFFLINE: blk_cnt = 12; break;
        default: blk_cnt = 12; break;
    }

    sample_common_vii_sensor_framerate_get(sensor_id, &sensor_framerate);
    if (sensor_framerate > 60) {
        blk_cnt += 5;
    }
#ifdef VS_ORION
    blk_cnt += 5;
#endif
    if (g_cvbs_enable) {
        blk_cnt += 4;
    }

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

    /* 2. 启动 VPP Group 0 (MIPI Path) */
    vs_sample_trace(">>> Configuring MIPI Path (VPP Grp %d)\n", vpp_grp_mipi);
    
    // 绑定 VII -> VPP 0
    ret = sample_common_vii_bind_vpp(vii_pipeid, vii_chnid, vpp_grp_mipi);
    if (ret != VS_SUCCESS) goto exit2;

    sample_vii_get_vpp_grp_attr(&img_size, &vpp_grp_attr);
    // 配置 Chn 0 属性 (Mode 0: 原始尺寸)
    sample_vii_get_vpp_chn_attr(&img_size, &vpp_chn_attr[0], 0);

    ret = sample_common_vpp_start(vpp_grp_mipi, chn_enable_mipi, &vpp_grp_attr, vpp_chn_attr);
    if (ret != VS_SUCCESS) goto exit3;

    // MIPI Crop
    ret = sample_vpp_crop_config(vpp_grp_mipi, 0);
    if (ret != VS_SUCCESS) goto exit4;


    /* 3. 启动 VPP Group 1 (CVBS Path) - 如果启用 */
    if (g_cvbs_enable) {
        vs_sample_trace(">>> Configuring CVBS Path (VPP Grp %d)\n", vpp_grp_cvbs);

        // 绑定 VII -> VPP 1 (同一个 VII 源分发给第二个 VPP)
        ret = sample_common_vii_bind_vpp(vii_pipeid, vii_chnid, vpp_grp_cvbs);
        if (ret != VS_SUCCESS) goto exit4;

        sample_vii_get_vpp_grp_attr(&img_size, &vpp_grp_attr);
        // 配置 Chn 0 属性 (Mode 1: CVBS 缩放尺寸) 
        // 注意：这里使用的是 Group 1 的 Chn 0
        sample_vii_get_vpp_chn_attr(&img_size, &vpp_chn_attr[0], 1);

        ret = sample_common_vpp_start(vpp_grp_cvbs, chn_enable_cvbs, &vpp_grp_attr, vpp_chn_attr);
        if (ret != VS_SUCCESS) goto exit4;
    }

    usleep(100000); 

    /* 4. 获取 VO 配置并启动 */
    sample_vio_get_vo_cfg(&img_size, vo_cfg);

    // DSP Init for MIPI
    if (vo_cfg[0].enable && vo_cfg[0].vo_intf_type == E_VO_INTERFACE_TYPE_MIPI) {
        ret = sample_common_dsp_init(0, name);
        if (ret != VS_SUCCESS) goto exit4;
    }

    // 启动 VO 设备
    for (i = 0; i < VO_MAX_DEV_NUM; i++) {
        if (vo_cfg[i].enable) {
            ret = sample_common_vo_start(&vo_cfg[i]);
            if (ret != VS_SUCCESS) {
                vs_sample_trace("sample_common_vo_start dev%d failed\n", i);
                goto exit5;
            }
        }
    }

    /* 5. 绑定 VPP -> VO (使用新的拓扑) */
    
    // Path A: VPP 0 (Chn 0) -> VO Dev 0 (Layer 0)
    if (vo_cfg[0].enable) {
        ret = sample_common_vpp_bind_vo(vpp_grp_mipi, 0, vo_cfg[0].vo_layerid, 0);
        if (ret != VS_SUCCESS) {
            vs_sample_trace("VPP(Grp%d)-VO(Layer%d) bind failed\n", vpp_grp_mipi, vo_cfg[0].vo_layerid);
            goto exit6;
        }
        vs_sample_trace("Bind Success: VPP[%d-0] -> VO[%d-0]\n", vpp_grp_mipi, vo_cfg[0].vo_layerid);
    }

    // Path B: VPP 1 (Chn 0) -> VO Dev 1 (Layer 3)
    if (vo_cfg[1].enable) {
        // 注意：这里 VO Dev 1 对应的 layerid 是 3
        ret = sample_common_vpp_bind_vo(vpp_grp_cvbs, 0, vo_cfg[1].vo_layerid, 0);
        if (ret != VS_SUCCESS) {
            vs_sample_trace("VPP(Grp%d)-VO(Layer%d) bind failed\n", vpp_grp_cvbs, vo_cfg[1].vo_layerid);
            goto exit6;
        }
        vs_sample_trace("Bind Success: VPP[%d-0] -> VO[%d-0]\n", vpp_grp_cvbs, vo_cfg[1].vo_layerid);
    }

    sample_common_pause();

    // 清理绑定
    if (vo_cfg[1].enable) sample_common_vpp_unbind_vo(vpp_grp_cvbs, 0, vo_cfg[1].vo_layerid, 0);
    if (vo_cfg[0].enable) sample_common_vpp_unbind_vo(vpp_grp_mipi, 0, vo_cfg[0].vo_layerid, 0);

exit6:
    for (i = 0; i < VO_MAX_DEV_NUM; i++) {
        if (vo_cfg[i].enable) sample_common_vo_stop(&vo_cfg[i]);
    }
exit5:
    if (vo_cfg[0].enable && vo_cfg[0].vo_intf_type == E_VO_INTERFACE_TYPE_MIPI) {
        sample_common_dsp_exit(0);
    }
exit4:
    if (g_cvbs_enable) sample_common_vpp_stop(vpp_grp_cvbs, chn_enable_cvbs);
    sample_common_vpp_stop(vpp_grp_mipi, chn_enable_mipi);
exit3:
    if (g_cvbs_enable) sample_common_vii_unbind_vpp(vii_pipeid, vii_chnid, vpp_grp_cvbs);
    sample_common_vii_unbind_vpp(vii_pipeid, vii_chnid, vpp_grp_mipi);
exit2:
    sample_common_vii_stop(&vii_cfg);
exit1:
    sample_common_sys_exit();
exit0:
    return ret;
}

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

vs_int32_t sample_vio_fpn_case(vs_void_t)
{
    vs_int32_t ret;
    vs_int32_t sensor_id = 0;
    vs_pixel_format_e pixel_format;
    vs_uint32_t frame_num;
    vs_size_s img_size = {0};
    vs_vb_cfg_s vb_cfg = {0};
    sample_vii_cfg_s vii_cfg = {0};
    vs_int32_t vii_pipeid = 0;
    vs_int32_t vii_chnid = 0;
    vs_int32_t vpp_grpid = 0;
    vs_int32_t vpp_chnid = 0;
    vs_int32_t vo_devid = 0;
    vs_int32_t vo_chnid = 0;
    vs_bool_t chn_enable[VPP_MAX_PHYCHN_NUM] = {VS_TRUE, VS_FALSE, VS_FALSE, VS_FALSE};
    vs_vpp_grp_attr_s vpp_grp_attr = {0};
    vs_vpp_chn_attr_s vpp_chn_attr[VPP_MAX_PHYCHN_NUM];
    // [Fix] 适配改为数组
    sample_vo_cfg_s vo_cfg[VO_MAX_DEV_NUM] = {0};

    sample_common_vii_sensor_img_size_get(sensor_id, &img_size);
    sample_common_vii_sensor_pixel_format_get(sensor_id, &pixel_format);
    frame_num = sample_common_vii_wdr_frame_num_get(sensor_id);
    vb_cfg.pool_cnt = 1;
    vb_cfg.ast_commpool[0].blk_size = sample_common_buffer_size_get(&img_size, pixel_format, E_COMPRESS_MODE_NONE, frame_num);
#ifdef VS_ORION
    vb_cfg.ast_commpool[0].blk_cnt = 15;
#else
    vb_cfg.ast_commpool[0].blk_cnt = 10;
#endif
    vb_cfg.ast_commpool[0].remap_mode = VB_REMAP_MODE_NONE;
    ret = sample_common_sys_init(&vb_cfg);
    if (ret != VS_SUCCESS) {
        goto exit0;
    }

    ret = sample_common_dsp_init(0, (vs_char_t*)"/lib/firmware/vs_dsp0.bin");
    if (ret != VS_SUCCESS) {
        goto exit1;
    }

    vii_cfg.vii_vpp_mode = E_VII_OFFLINE_VPP_ONLINE;
    vii_cfg.route_num = 1;
    sample_common_vii_default_cfg_get(sensor_id, &vii_cfg.route_cfg[0]);
    vii_cfg.route_cfg[0].pipe_cfg[0].pipe_attr.compress_mode = E_COMPRESS_MODE_NONE;
    sample_common_get_fpn_cfg(&vii_cfg.route_cfg[0].pipe_cfg[0], &g_fpn_frame_info);
    ret = sample_common_vii_start(&vii_cfg);
    if (ret != VS_SUCCESS) {
        goto exit2;
    }

    ret = sample_common_vii_bind_vpp(vii_pipeid, vii_chnid, vpp_grpid);
    if (ret != VS_SUCCESS) {
        goto exit3;
    }

    sample_vii_get_vpp_grp_attr(&img_size, &vpp_grp_attr);
    // [Fix] 增加参数 vpp_chnid
    sample_vii_get_vpp_chn_attr(&img_size, &vpp_chn_attr[vpp_chnid], vpp_chnid);
    ret = sample_common_vpp_start(vpp_grpid, chn_enable, &vpp_grp_attr, vpp_chn_attr);
    if (ret != VS_SUCCESS) {
        goto exit4;
    }

    ret = sample_common_vpp_bind_vo(vpp_grpid, vpp_chnid, vo_devid, vo_chnid);
    if (ret != VS_SUCCESS) {
        goto exit5;
    }

    // [Fix] 传入数组指针
    sample_vio_get_vo_cfg(&img_size, vo_cfg);
    // [Fix] 仅启动 Dev 0 (FPN case 仅测试主路)
    ret = sample_common_vo_start(&vo_cfg[0]);
    if (ret != VS_SUCCESS) {
        goto exit6;
    }

    sample_common_pause();

    sample_common_vo_stop(&vo_cfg[0]);

exit6:
    sample_common_vpp_unbind_vo(vpp_grpid, vpp_chnid, vo_devid, vo_chnid);
exit5:
    sample_common_vpp_stop(vpp_grpid, chn_enable);
exit4:
    sample_common_vii_unbind_vpp(vii_pipeid, vii_chnid, vpp_grpid);
exit3:
    sample_common_vii_stop(&vii_cfg);
exit2:
    sample_common_vii_pipe_release_fpn_frm_vb_blk(&g_fpn_frame_info);
    sample_common_dsp_exit(0);
exit1:
    sample_common_sys_exit();
exit0:
    return ret;
}

vs_int32_t sample_vio_dual_pipe_case(vs_void_t)
{
    vs_int32_t ret;
    vs_int32_t sensor_id = 0;
    vs_pixel_format_e pixel_format;
    vs_uint32_t frame_num;
    vs_size_s img_size = {0};
    vs_vb_cfg_s vb_cfg = {0};
    sample_vii_cfg_s vii_cfg = {0};
    vs_int32_t vii_pipeid[2] = {0, 1};
    vs_int32_t vii_chnid = 0;
    vs_int32_t vpp_grpid[2] = {0, 1};
    vs_int32_t vpp_chnid = 0;
    vs_int32_t vo_devid = 0;
    vs_int32_t vo_chnid[2] = {0, 1};
    vs_bool_t chn_enable[VPP_MAX_PHYCHN_NUM] = {VS_TRUE, VS_FALSE, VS_FALSE, VS_FALSE};
    vs_vpp_grp_attr_s vpp_grp_attr = {0};
    vs_vpp_chn_attr_s vpp_chn_attr[VPP_MAX_PHYCHN_NUM];
    // [Fix] 适配改为数组
    sample_vo_cfg_s vo_cfg[VO_MAX_DEV_NUM] = {0};
    vs_char_t name[100] = "/lib/firmware/vs_dsp0.bin";
    vs_int32_t sensor_framerate = 30;

    sample_common_vii_sensor_img_size_get(sensor_id, &img_size);
    if (img_size.width >= 3840 || img_size.height >= 2160) {
        printf("Notice: the case don't support this sensor type!\n");
        return VS_FAILED;
    }

    frame_num = sample_common_vii_wdr_frame_num_get(sensor_id);
    if (frame_num >= 3) {
        printf("Notice: the case don't support this sensor type!\n");
        return VS_FAILED;
    }

    sample_common_vii_sensor_framerate_get(sensor_id, &sensor_framerate);
    if (sensor_framerate > 60) {
        printf("Notice: the case don't support this sensor type!\n");
        return VS_FAILED;
    }

    sample_common_vii_sensor_pixel_format_get(sensor_id, &pixel_format);
    if (frame_num > 1) {
        vb_cfg.pool_cnt = 2;
        vb_cfg.ast_commpool[0].blk_size = sample_common_buffer_size_get(&img_size, pixel_format, E_COMPRESS_MODE_NONE, frame_num);
#ifdef VS_ORION
        vb_cfg.ast_commpool[0].blk_cnt = 9;
#else
        vb_cfg.ast_commpool[0].blk_cnt = 4;
#endif
        vb_cfg.ast_commpool[0].remap_mode = VB_REMAP_MODE_NONE;
        vb_cfg.ast_commpool[1].blk_size = sample_common_buffer_size_get(&img_size, pixel_format, E_COMPRESS_MODE_NONE, 1);
        vb_cfg.ast_commpool[1].blk_cnt = 10;
        vb_cfg.ast_commpool[1].remap_mode = VB_REMAP_MODE_NONE;
    } else {
        vb_cfg.pool_cnt = 1;
        vb_cfg.ast_commpool[0].blk_size = sample_common_buffer_size_get(&img_size, pixel_format, E_COMPRESS_MODE_NONE, frame_num);
        vb_cfg.ast_commpool[0].blk_cnt = 18;
        vb_cfg.ast_commpool[0].remap_mode = VB_REMAP_MODE_NONE;
    }

    ret = sample_common_sys_init(&vb_cfg);
    if (ret != VS_SUCCESS) {
        goto exit0;
    }

    vii_cfg.vii_vpp_mode = E_VII_OFFLINE_VPP_ONLINE;
    vii_cfg.route_num = 1;
    sample_common_vii_default_cfg_get(sensor_id, &vii_cfg.route_cfg[0]);
    vii_cfg.route_cfg[0].pipe_num = 2;
    vii_cfg.route_cfg[0].pipe_id[0] = vii_pipeid[0];
    vii_cfg.route_cfg[0].pipe_id[1] = vii_pipeid[1];
    vii_cfg.route_cfg[0].pipe_cfg[0].pipe_param.mode = E_VII_PIPE_ADVANCE_MODE_NONE;
    memcpy(&vii_cfg.route_cfg[0].pipe_cfg[1], &vii_cfg.route_cfg[0].pipe_cfg[0], sizeof(sample_vii_pipe_cfg_s));
    vii_cfg.route_cfg[0].pipe_cfg[0].pipe_id = vii_pipeid[0];
    vii_cfg.route_cfg[0].pipe_cfg[1].pipe_id = vii_pipeid[1];
    vii_cfg.route_cfg[0].pipe_cfg[1].bus_id = -1;
    vii_cfg.route_cfg[0].pipe_cfg[0].pipe_attr.compress_mode = E_COMPRESS_MODE_NONE;
    vii_cfg.route_cfg[0].pipe_cfg[1].pipe_attr.compress_mode = E_COMPRESS_MODE_NONE;
    vii_cfg.route_cfg[0].pipe_cfg[0].phys_chn_cfg[0].chn_attr.compress_mode = E_COMPRESS_MODE_RASTER;
    vii_cfg.route_cfg[0].pipe_cfg[1].phys_chn_cfg[0].chn_attr.compress_mode = E_COMPRESS_MODE_RASTER;

    ret = sample_common_vii_start(&vii_cfg);
    if (ret != VS_SUCCESS) {
        goto exit1;
    }

    ret = sample_common_vii_bind_vpp(vii_pipeid[0], vii_chnid, vpp_grpid[0]);
    ret |= sample_common_vii_bind_vpp(vii_pipeid[1], vii_chnid, vpp_grpid[1]);
    if (ret != VS_SUCCESS) {
        goto exit2;
    }

    sample_vii_get_vpp_grp_attr(&img_size, &vpp_grp_attr);
    // [Fix] 增加参数 vpp_chnid
    sample_vii_get_vpp_chn_attr(&img_size, &vpp_chn_attr[vpp_chnid], vpp_chnid);
    ret = sample_common_vpp_start(vpp_grpid[0], chn_enable, &vpp_grp_attr, vpp_chn_attr);
    ret |= sample_common_vpp_start(vpp_grpid[1], chn_enable, &vpp_grp_attr, vpp_chn_attr);
    if (ret != VS_SUCCESS) {
        goto exit3;
    }

    ret = sample_common_vpp_bind_vo(vpp_grpid[0], vpp_chnid, vo_devid, vo_chnid[0]);
    ret |= sample_common_vpp_bind_vo(vpp_grpid[1], vpp_chnid, vo_devid, vo_chnid[1]);
    if (ret != VS_SUCCESS) {
        goto exit4;
    }

    // [Fix] 传入数组指针
    sample_vio_get_vo_cfg(&img_size, vo_cfg);
    if (vo_cfg[0].vo_intf_type == E_VO_INTERFACE_TYPE_MIPI) {
        ret = sample_common_dsp_init(0, name);
        if (ret != VS_SUCCESS) {
            goto exit5;
        }
    }

    // [Fix] 操作数组索引 0
    vo_cfg[0].vo_mode = E_VO_MODE_4MUX;
    ret = sample_common_vo_start(&vo_cfg[0]);
    if (ret != VS_SUCCESS) {
        goto exit5;
    }

    sample_common_pause();

    sample_common_vo_stop(&vo_cfg[0]);

exit5:
    if (vo_cfg[0].vo_intf_type == E_VO_INTERFACE_TYPE_MIPI) {
        sample_common_dsp_exit(0);
    }
    sample_common_vpp_unbind_vo(vpp_grpid[0], vpp_chnid, vo_devid, vo_chnid[0]);
    sample_common_vpp_unbind_vo(vpp_grpid[1], vpp_chnid, vo_devid, vo_chnid[1]);
exit4:
    sample_common_vpp_stop(vpp_grpid[0], chn_enable);
    sample_common_vpp_stop(vpp_grpid[1], chn_enable);
exit3:
    sample_common_vii_unbind_vpp(vii_pipeid[0], vii_chnid, vpp_grpid[0]);
    sample_common_vii_unbind_vpp(vii_pipeid[1], vii_chnid, vpp_grpid[1]);
exit2:
    sample_common_vii_stop(&vii_cfg);
exit1:
    sample_common_sys_exit();
exit0:
    return ret;
}

static vs_int32_t sample_vio_set_ldc_rotation_cfg(vs_int32_t vii_pipeid, vs_int32_t vii_chnid)
{
    vs_int32_t ret;
    vs_ldc_attr_s ldc_attr = {0};
    vs_rotation_attr_s rotation_attr = {0};

    ldc_attr.enable = VS_TRUE;
    ldc_attr.mode = E_LDC_MODE_PANORAMIC;
    ldc_attr.center_x_offset = 0;
    ldc_attr.center_y_offset = 0;
    ldc_attr.ratio = 1700;
    ldc_attr.h_ratio = 1000;
    ldc_attr.v_ratio = 1000;
    ldc_attr.zoom = 1000;
    ldc_attr.input_fov = 180 * 1000;
    ldc_attr.diameter = 1080;
    ldc_attr.fov_width = 90 * 1000;
    ldc_attr.fov_height = 90 * 1000;
    ldc_attr.ratio_keep = VS_TRUE;
    ret = vs_mal_vii_chn_ldc_set(vii_pipeid, vii_chnid, &ldc_attr);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_vii_chn_ldc_set failed, ret: 0x%x\n", ret);
        return VS_FAILED;
    }

    rotation_attr.enable = VS_TRUE;
    rotation_attr.mode = E_ROTATION_MODE_FIXED;
    rotation_attr.fixed_rotation = E_FIXED_ROTATION_180;
    ret = vs_mal_vii_chn_rotation_set(vii_pipeid, vii_chnid, &rotation_attr);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_vii_chn_rotation_set failed, ret: 0x%x\n", ret);
        return VS_FAILED;
    }

    return VS_SUCCESS;
}

vs_int32_t sample_vio_vii_ldc_rotation_case(vs_void_t)
{
    vs_int32_t ret;
    vs_int32_t sensor_id = 0;
    vs_pixel_format_e pixel_format;
    vs_uint32_t frame_num;
    vs_int32_t sensor_framerate;
    vs_size_s img_size = {0};
    vs_vb_cfg_s vb_cfg = {0};
    sample_vii_cfg_s vii_cfg = {0};
    vs_int32_t vii_pipeid = 0;
    vs_int32_t vii_chnid = 0;
    vs_int32_t vpp_grpid = 0;
    vs_int32_t vpp_chnid = 0;
    vs_int32_t vo_devid = 0;
    vs_int32_t vo_chnid = 0;
    vs_bool_t chn_enable[VPP_MAX_PHYCHN_NUM] = {VS_TRUE, VS_FALSE, VS_FALSE, VS_FALSE};
    vs_vpp_grp_attr_s vpp_grp_attr = {0};
    vs_vpp_chn_attr_s vpp_chn_attr[VPP_MAX_PHYCHN_NUM];
    // [Fix] 适配改为数组
    sample_vo_cfg_s vo_cfg[VO_MAX_DEV_NUM] = {0};
    vs_char_t name[100] = "/lib/firmware/vs_dsp0.bin";

    sample_common_vii_sensor_img_size_get(sensor_id, &img_size);
    sample_common_vii_sensor_framerate_get(sensor_id, &sensor_framerate);
    sample_common_vii_sensor_pixel_format_get(sensor_id, &pixel_format);
    frame_num = sample_common_vii_wdr_frame_num_get(sensor_id);
    if (frame_num >= 2) {
        printf("Notice: the case don't support wdr mode!\n");
        return VS_FAILED;
    }

    if ((img_size.width >= 3840 || img_size.height >= 2160) && (sensor_framerate > 30)) {
        printf("Notice: the case don't support this sensor type!\n");
        return VS_FAILED;
    }

    vb_cfg.pool_cnt = 1;
    vb_cfg.ast_commpool[0].blk_size = sample_common_buffer_size_get(&img_size, pixel_format, E_COMPRESS_MODE_NONE, frame_num);
#ifdef VS_ORION
    vb_cfg.ast_commpool[0].blk_cnt = 15;
#else
    vb_cfg.ast_commpool[0].blk_cnt = 10;
#endif
    if (sensor_framerate > 60) {
        vb_cfg.ast_commpool[0].blk_cnt += 5;
    }
    vb_cfg.ast_commpool[0].remap_mode = VB_REMAP_MODE_NONE;
    ret = sample_common_sys_init(&vb_cfg);
    if (ret != VS_SUCCESS) {
        goto exit0;
    }

    vii_cfg.vii_vpp_mode = E_VII_ONLINE_VPP_OFFLINE;
    vii_cfg.route_num = 1;
    sample_common_vii_default_cfg_get(sensor_id, &vii_cfg.route_cfg[0]);
    vii_cfg.route_cfg[0].pipe_cfg[0].phys_chn_cfg[0].chn_attr.compress_mode = E_COMPRESS_MODE_NONE;
    ret = sample_common_vii_start(&vii_cfg);
    if (ret != VS_SUCCESS) {
        goto exit1;
    }

    ret = sample_vio_set_ldc_rotation_cfg(vii_pipeid, vii_chnid);
    if (ret != VS_SUCCESS) {
        goto exit2;
    }

    ret = sample_common_vii_bind_vpp(vii_pipeid, vii_chnid, vpp_grpid);
    if (ret != VS_SUCCESS) {
        goto exit2;
    }

    sample_vii_get_vpp_grp_attr(&img_size, &vpp_grp_attr);
    // [Fix] 增加参数 vpp_chnid
    sample_vii_get_vpp_chn_attr(&img_size, &vpp_chn_attr[vpp_chnid], vpp_chnid);
    ret = sample_common_vpp_start(vpp_grpid, chn_enable, &vpp_grp_attr, vpp_chn_attr);
    if (ret != VS_SUCCESS) {
        goto exit3;
    }

    ret = sample_common_vpp_bind_vo(vpp_grpid, vpp_chnid, vo_devid, vo_chnid);
    if (ret != VS_SUCCESS) {
        goto exit4;
    }

    // [Fix] 传入数组指针
    sample_vio_get_vo_cfg(&img_size, vo_cfg);
    if (vo_cfg[0].vo_intf_type == E_VO_INTERFACE_TYPE_MIPI) {
        ret = sample_common_dsp_init(0, name);
        if (ret != VS_SUCCESS) {
            goto exit5;
        }
    }

    // [Fix] 仅启动 Dev 0
    ret = sample_common_vo_start(&vo_cfg[0]);
    if (ret != VS_SUCCESS) {
        goto exit5;
    }

    sample_common_pause();

    sample_common_vo_stop(&vo_cfg[0]);

exit5:
    if (vo_cfg[0].vo_intf_type == E_VO_INTERFACE_TYPE_MIPI) {
        sample_common_dsp_exit(0);
    }
    sample_common_vpp_unbind_vo(vpp_grpid, vpp_chnid, vo_devid, vo_chnid);
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

vs_int32_t sample_vio_linear_wdr_switch_case(vs_void_t)
{
    vs_int32_t ret;
    vs_int32_t sensor_id = 0;
    vs_pixel_format_e pixel_format;
    vs_size_s img_size = {0};
    vs_size_s new_img_size = {0};
    vs_vb_cfg_s vb_cfg = {0};
    sample_vii_cfg_s vii_cfg = {0};
    vs_int32_t vii_pipeid = 0;
    vs_int32_t vii_chnid = 0;
    vs_int32_t vpp_grpid = 0;
    vs_int32_t vpp_chnid = 0;
    vs_int32_t vo_devid = 0;
    vs_int32_t vo_chnid = 0;
    vs_bool_t chn_enable[VPP_MAX_PHYCHN_NUM] = {VS_TRUE, VS_FALSE, VS_FALSE, VS_FALSE};
    vs_vpp_grp_attr_s vpp_grp_attr = {0};
    vs_vpp_chn_attr_s vpp_chn_attr[VPP_MAX_PHYCHN_NUM];
    // [Fix] 适配改为数组
    sample_vo_cfg_s vo_cfg[VO_MAX_DEV_NUM] = {0};
    vs_uint32_t switch_cnt = 0;
    vs_char_t name[100] = "/lib/firmware/vs_dsp0.bin";

    sample_common_vii_sensor_img_size_get(sensor_id, &img_size);
    sample_common_vii_sensor_pixel_format_get(sensor_id, &pixel_format);
    vb_cfg.pool_cnt = 2;
    vb_cfg.ast_commpool[0].blk_size = sample_common_buffer_size_get(&img_size, pixel_format, E_COMPRESS_MODE_NONE, 3);
    vb_cfg.ast_commpool[0].blk_cnt = 4;
    vb_cfg.ast_commpool[0].remap_mode = VB_REMAP_MODE_NONE;
    vb_cfg.ast_commpool[1].blk_size = sample_common_buffer_size_get(&img_size, pixel_format, E_COMPRESS_MODE_NONE, 1);
#ifdef VS_ORION
    vb_cfg.ast_commpool[1].blk_cnt = 12;
#else
    vb_cfg.ast_commpool[1].blk_cnt = 7;
#endif
    vb_cfg.ast_commpool[1].remap_mode = VB_REMAP_MODE_NONE;
    ret = sample_common_sys_init(&vb_cfg);
    if (ret != VS_SUCCESS) {
        goto exit0;
    }

    vii_cfg.vii_vpp_mode = E_VII_OFFLINE_VPP_ONLINE;
    vii_cfg.route_num = 1;
    sample_common_vii_default_cfg_get(sensor_id, &vii_cfg.route_cfg[0]);
    vii_cfg.route_cfg[0].pipe_cfg[0].pipe_attr.compress_mode = E_COMPRESS_MODE_NONE;
    vii_cfg.route_cfg[0].pipe_cfg[0].phys_chn_cfg[0].chn_attr.compress_mode = E_COMPRESS_MODE_RASTER;

    char sensor_name[128];
    sample_common_vii_sensor_name_get(g_sensor_type[0], sensor_name);
    vs_uint32_t wdr_frame_num = sample_common_vii_wdr_frame_num_get_by_sensor(g_sensor_type[0]);
    if (((strcmp(sensor_name, "os04a10") == 0 || strcmp(sensor_name, "imx347") == 0)) && (wdr_frame_num > 1)) {
        vii_cfg.route_cfg[0].pipe_cfg[0].pipe_param.mode = E_VII_PIPE_ADVANCE_MODE_SUB_FRAME_FIRST;
        vs_sample_trace("advanced mode wdr_frame_num %d \n", wdr_frame_num);
    } else {
        vii_cfg.route_cfg[0].pipe_cfg[0].pipe_param.mode = E_VII_PIPE_ADVANCE_MODE_NONE;
    }

    ret = sample_common_vii_start(&vii_cfg);
    if (ret != VS_SUCCESS) {
        goto exit1;
    }

    ret = sample_common_vii_bind_vpp(vii_pipeid, vii_chnid, vpp_grpid);
    if (ret != VS_SUCCESS) {
        goto exit2;
    }

    sample_vii_get_vpp_grp_attr(&img_size, &vpp_grp_attr);
    // [Fix] 增加参数 vpp_chnid
    sample_vii_get_vpp_chn_attr(&img_size, &vpp_chn_attr[vpp_chnid], vpp_chnid);
    ret = sample_common_vpp_start(vpp_grpid, chn_enable, &vpp_grp_attr, vpp_chn_attr);
    if (ret != VS_SUCCESS) {
        goto exit3;
    }

    ret = sample_common_vpp_bind_vo(vpp_grpid, vpp_chnid, vo_devid, vo_chnid);
    if (ret != VS_SUCCESS) {
        goto exit4;
    }

    // [Fix] 传入数组指针
    sample_vio_get_vo_cfg(&img_size, vo_cfg);
    if (vo_cfg[0].vo_intf_type == E_VO_INTERFACE_TYPE_MIPI) {
        ret = sample_common_dsp_init(0, name);
        if (ret != VS_SUCCESS) {
            goto exit5;
        }
    }

    // [Fix] 仅启动 Dev 0
    ret = sample_common_vo_start(&vo_cfg[0]);
    if (ret != VS_SUCCESS) {
        goto exit5;
    }

    while(g_stop_flag == 0 && switch_cnt < 3){
        switch_cnt++;
        usleep(5000000);
        vs_sample_trace("printf input enter switch wdr mode: cur wdr mode %d\n", wdr_frame_num);
        while (g_stop_flag == 0) {
            if (getchar() == '\n') {
                break;
            }
            usleep(1000);
        };

        if (g_stop_flag == 1) {
            break;
        }
        vs_uint32_t new_wdr_frame_num = wdr_frame_num;
        char new_sensor_name[128];
        vs_uint32_t i;
        vs_int32_t sensor_framerate;
        for (i = 0; i < sample_common_vii_sensor_type_num_get(); i++) {
            sample_common_vii_sensor_name_get(i, new_sensor_name);
            if (strcmp(sensor_name, new_sensor_name) != 0) {
                continue;
            }
            sample_common_vii_img_size_get(i, &new_img_size);
            if (new_img_size.width != img_size.width || new_img_size.height != img_size.height){
                continue;
            }

            sample_common_vii_framerate_get(i, &sensor_framerate);
            if ((new_img_size.width >= 3840 || new_img_size.height >= 2160) && (sensor_framerate > 30)) {
                continue;
            }

            new_wdr_frame_num = sample_common_vii_wdr_frame_num_get_by_sensor(i);
            if (new_wdr_frame_num != wdr_frame_num) {
                break;
            }
        }

        if (i < sample_common_vii_sensor_type_num_get()){
            vs_sample_trace("wdr mode change %d ----> %d\n", wdr_frame_num, new_wdr_frame_num);
            g_sensor_type[0] = i;
        } else {
            vs_sample_trace("sensor %s only support %d\n", sensor_name, wdr_frame_num);
            break;
        }

        vii_cfg.route_cfg[0].pipe_cfg[0].firmware_bypass = VS_TRUE;
        sample_common_vii_unbind_vpp(vii_pipeid, vii_chnid, vpp_grpid);
        sample_common_vii_stop(&vii_cfg);

        vs_sample_trace("dreset sensor\n");
        vs_mal_mipi_rx_sensor_reset(0);
        // os04a10 needs at least 2ms to reset
        usleep(4 * 1000);
        vs_mal_mipi_rx_sensor_unreset(0);
        vs_sample_trace("reset sensor -done\n");

        sample_common_vii_sensor_name_get(g_sensor_type[0], sensor_name);
        wdr_frame_num = sample_common_vii_wdr_frame_num_get_by_sensor(g_sensor_type[0]);
        sample_common_vii_img_size_get(g_sensor_type[0], &img_size);
        sample_common_vii_default_cfg_get(sensor_id, &vii_cfg.route_cfg[0]);
        vii_cfg.route_cfg[0].pipe_cfg[0].firmware_bypass = VS_TRUE;
        vs_sample_trace("pixel_format %d \n", vii_cfg.route_cfg[0].pipe_cfg->pipe_attr.pixel_format);
        if (((strcmp(sensor_name, "os04a10") == 0 || strcmp(sensor_name, "imx347") == 0)) && (wdr_frame_num > 1)) {
            vii_cfg.route_cfg[0].pipe_cfg[0].pipe_param.mode = E_VII_PIPE_ADVANCE_MODE_SUB_FRAME_FIRST;
        } else {
            vii_cfg.route_cfg[0].pipe_cfg[0].pipe_param.mode = E_VII_PIPE_ADVANCE_MODE_NONE;
        }

        ret = sample_common_vii_start(&vii_cfg);
        ret = sample_common_vii_bind_vpp(vii_pipeid, vii_chnid, vpp_grpid);

        vs_mal_isp_comm_attr_set(vii_pipeid, &vii_cfg.route_cfg[0].pipe_cfg[0].isp_attr);
        vii_cfg.route_cfg[0].pipe_cfg[0].firmware_bypass = VS_FALSE;

        vs_isp_ae_attr_s ae_attr;
        vs_isp_wdr_exp_attr_s wdr_exp_attr;
        vs_mal_isp_ae_attr_get(vii_pipeid, &ae_attr);
        vs_mal_isp_ae_attr_set(vii_pipeid, &ae_attr);
        vs_mal_isp_wdr_exp_attr_get(vii_pipeid, &wdr_exp_attr);
        vs_mal_isp_wdr_exp_attr_set(vii_pipeid, &wdr_exp_attr);
    }

    sample_common_vo_stop(&vo_cfg[0]);

exit5:
    if (vo_cfg[0].vo_intf_type == E_VO_INTERFACE_TYPE_MIPI) {
        sample_common_dsp_exit(0);
    }
    sample_common_vpp_unbind_vo(vpp_grpid, vpp_chnid, vo_devid, vo_chnid);
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

vs_int32_t sample_vio_resolution_switch_case(vs_void_t)
{
    vs_int32_t ret;
    vs_int32_t sensor_id = 0;
    vs_pixel_format_e pixel_format;
    vs_size_s img_size = {0};
    vs_size_s new_img_size = {0};
    vs_vb_cfg_s vb_cfg = {0};
    sample_vii_cfg_s vii_cfg = {0};
    vs_int32_t vii_pipeid = 0;
    vs_int32_t vii_chnid = 0;
    vs_int32_t vpp_grpid = 0;
    vs_int32_t vpp_chnid = 0;
    vs_int32_t vo_devid = 0;
    vs_int32_t vo_chnid = 0;
    vs_bool_t chn_enable[VPP_MAX_PHYCHN_NUM] = {VS_TRUE, VS_FALSE, VS_FALSE, VS_FALSE};
    vs_vpp_grp_attr_s vpp_grp_attr = {0};
    vs_vpp_chn_attr_s vpp_chn_attr[VPP_MAX_PHYCHN_NUM];
    // [Fix] 适配改为数组
    sample_vo_cfg_s vo_cfg[VO_MAX_DEV_NUM] = {0};
    vs_uint32_t switch_cnt = 0;
    vs_char_t name[100] = "/lib/firmware/vs_dsp0.bin";
    vs_uint32_t i;
    vs_uint32_t wdr_frame_num, new_wdr_frame_num;
    char new_sensor_name[128];
    char sensor_name[128];
    vs_uint64_t max_blk_size = 0, blk_size = 0;
    vs_int32_t max_framerate = 0, framerate = 0;

    if (SONY_IMX586_MIPI_48M_1FPS_10BIT == g_sensor_type[0] || SONY_IMX586_MIPI_48M_3FPS_10BIT == g_sensor_type[0]) {
        vs_sample_trace("not support sensor_type %d\n", g_sensor_type[0]);
        return VS_FAILED;
    }

    wdr_frame_num = sample_common_vii_wdr_frame_num_get_by_sensor(g_sensor_type[0]);
    sample_common_vii_sensor_name_get(g_sensor_type[0], sensor_name);
    sample_common_vii_sensor_img_size_get(sensor_id, &img_size);
    sample_common_vii_sensor_pixel_format_get(sensor_id, &pixel_format);
    for (i = 0; i < sample_common_vii_sensor_type_num_get(); i++) {
        if (SONY_IMX586_MIPI_48M_1FPS_10BIT == i || SONY_IMX586_MIPI_48M_3FPS_10BIT == i) {
            continue;
        }

        sample_common_vii_sensor_name_get(i, new_sensor_name);
        if (strcmp(sensor_name, new_sensor_name) != 0) {
            continue;
        }

        new_wdr_frame_num = sample_common_vii_wdr_frame_num_get_by_sensor(i);
        if (new_wdr_frame_num != wdr_frame_num) {
            continue;
        }

        sample_common_vii_img_size_get(i, &new_img_size);
        blk_size = sample_common_buffer_size_get(&new_img_size, pixel_format, E_COMPRESS_MODE_NONE, wdr_frame_num);
        max_blk_size = (blk_size > max_blk_size) ? blk_size : max_blk_size;
        sample_common_vii_framerate_get(i, &framerate);
        max_framerate = (framerate > max_framerate) ? framerate : max_framerate;
    }

    vb_cfg.pool_cnt = 1;
    vb_cfg.ast_commpool[0].blk_size = max_blk_size;
    vb_cfg.ast_commpool[0].blk_cnt = 10;
#ifdef VS_ORION
    vb_cfg.ast_commpool[0].blk_cnt += 5;
#endif
    if (max_framerate > 60) {
        vb_cfg.ast_commpool[0].blk_cnt += 5;
    }
    vb_cfg.ast_commpool[0].remap_mode = VB_REMAP_MODE_NONE;
    ret = sample_common_sys_init(&vb_cfg);
    if (ret != VS_SUCCESS) {
        goto exit0;
    }

    vii_cfg.vii_vpp_mode = E_VII_OFFLINE_VPP_ONLINE;
    vii_cfg.route_num = 1;
    sample_common_vii_default_cfg_get(sensor_id, &vii_cfg.route_cfg[0]);
    vii_cfg.route_cfg[0].pipe_cfg[0].pipe_attr.compress_mode = E_COMPRESS_MODE_NONE;
    vii_cfg.route_cfg[0].pipe_cfg[0].phys_chn_cfg[0].chn_attr.compress_mode = E_COMPRESS_MODE_RASTER;

    if (((strcmp(sensor_name, "os04a10") == 0 || strcmp(sensor_name, "imx347") == 0)) && (wdr_frame_num > 1)) {
        vii_cfg.route_cfg[0].pipe_cfg[0].pipe_param.mode = E_VII_PIPE_ADVANCE_MODE_SUB_FRAME_FIRST;
        vs_sample_trace("advanced mode wdr_frame_num %d \n", wdr_frame_num);
    } else {
        vii_cfg.route_cfg[0].pipe_cfg[0].pipe_param.mode = E_VII_PIPE_ADVANCE_MODE_NONE;
    }

    ret = sample_common_vii_start(&vii_cfg);
    if (ret != VS_SUCCESS) {
        goto exit1;
    }

    ret = sample_common_vii_bind_vpp(vii_pipeid, vii_chnid, vpp_grpid);
    if (ret != VS_SUCCESS) {
        goto exit2;
    }

    sample_vii_get_vpp_grp_attr(&img_size, &vpp_grp_attr);
    // [Fix] 增加参数 vpp_chnid
    sample_vii_get_vpp_chn_attr(&img_size, &vpp_chn_attr[vpp_chnid], vpp_chnid);
    vpp_chn_attr[vpp_chnid].compress_mode = E_COMPRESS_MODE_NONE;
    ret = sample_common_vpp_start(vpp_grpid, chn_enable, &vpp_grp_attr, vpp_chn_attr);
    if (ret != VS_SUCCESS) {
        goto exit3;
    }

    ret = sample_common_vpp_bind_vo(vpp_grpid, vpp_chnid, vo_devid, vo_chnid);
    if (ret != VS_SUCCESS) {
        goto exit4;
    }

    // [Fix] 传入数组指针
    sample_vio_get_vo_cfg(&img_size, vo_cfg);
    if (vo_cfg[0].vo_intf_type == E_VO_INTERFACE_TYPE_MIPI) {
        ret = sample_common_dsp_init(0, name);
        if (ret != VS_SUCCESS) {
            goto exit5;
        }
    }

    // [Fix] 仅启动 Dev 0
    ret = sample_common_vo_start(&vo_cfg[0]);
    if (ret != VS_SUCCESS) {
        goto exit5;
    }

    while(g_stop_flag == 0 && switch_cnt < 3){
        switch_cnt++;
        usleep(2000000);
        vs_sample_trace("please input enter switch resultion\n");
        while (g_stop_flag == 0) {
            if (getchar() == '\n') {
                break;
            }
            usleep(1000);
        };

        if (g_stop_flag == 1) {
            break;
        }
        vs_uint32_t new_wdr_frame_num = wdr_frame_num;
        char new_sensor_name[128];
        vs_uint32_t i;
        vs_int32_t sensor_framerate;
        for (i = 0; i < sample_common_vii_sensor_type_num_get(); i++) {
            if (SONY_IMX586_MIPI_48M_1FPS_10BIT == i || SONY_IMX586_MIPI_48M_3FPS_10BIT == i) {
                continue;
            }

            sample_common_vii_sensor_name_get(i, new_sensor_name);
            if (strcmp(sensor_name, new_sensor_name) != 0) {
                continue;
            }

            new_wdr_frame_num = sample_common_vii_wdr_frame_num_get_by_sensor(i);
            if (new_wdr_frame_num != wdr_frame_num) {
                continue;
            }

            sample_common_vii_img_size_get(i, &new_img_size);
            sample_common_vii_framerate_get(i, &sensor_framerate);
            if ((new_img_size.width >= 3840 || new_img_size.height >= 2160) && (sensor_framerate > 30)) {
                continue;
            }

            if (new_img_size.width != img_size.width || new_img_size.height != img_size.height){
                break;
            }
        }

        if (i < sample_common_vii_sensor_type_num_get()){
            vs_sample_trace("resolution change [%d %d]----> [%d %d]\n", img_size.width, img_size.height,
                new_img_size.width, new_img_size.height);
            g_sensor_type[0] = i;
        } else {
            vs_sample_trace("sensor %s only support %d\n", sensor_name, g_sensor_type[0]);
            break;
        }

        vii_cfg.route_cfg[0].pipe_cfg[0].firmware_bypass = VS_TRUE;
        sample_common_vpp_unbind_vo(vpp_grpid, vpp_chnid, vo_devid, vo_chnid);
        sample_common_vpp_stop(vpp_grpid, chn_enable);
        vs_sample_trace("vpp stop done\n");
        sample_common_vii_unbind_vpp(vii_pipeid, vii_chnid, vpp_grpid);
        sample_common_vii_stop(&vii_cfg);
        sample_common_vo_stop(&vo_cfg[0]);

        vs_sample_trace("reset sensor\n");
        vs_mal_mipi_rx_sensor_reset(0);
        usleep(500);
        vs_mal_mipi_rx_sensor_unreset(0);
        vs_mal_vii_module_reset();
        vs_sample_trace("reset sensor -done\n");

        sample_common_vii_sensor_name_get(g_sensor_type[0], sensor_name);
        wdr_frame_num = sample_common_vii_wdr_frame_num_get_by_sensor(g_sensor_type[0]);
        sample_common_vii_img_size_get(g_sensor_type[0], &img_size);
        sample_common_vii_default_cfg_get(sensor_id, &vii_cfg.route_cfg[0]);

        vii_cfg.route_cfg[0].pipe_cfg[0].firmware_bypass = VS_TRUE;
        if (((strcmp(sensor_name, "os04a10") == 0 || strcmp(sensor_name, "imx347") == 0)) && (wdr_frame_num > 1)) {
            vii_cfg.route_cfg[0].pipe_cfg[0].pipe_param.mode = E_VII_PIPE_ADVANCE_MODE_SUB_FRAME_FIRST;
        } else {
            vii_cfg.route_cfg[0].pipe_cfg[0].pipe_param.mode = E_VII_PIPE_ADVANCE_MODE_NONE;
        }

        ret = sample_common_vii_start(&vii_cfg);
        ret = sample_common_vii_bind_vpp(vii_pipeid, vii_chnid, vpp_grpid);
        sample_vii_get_vpp_grp_attr(&img_size, &vpp_grp_attr);
        // [Fix] 增加参数 vpp_chnid
        sample_vii_get_vpp_chn_attr(&img_size, &vpp_chn_attr[vpp_chnid], vpp_chnid);
        vpp_chn_attr[vpp_chnid].compress_mode = E_COMPRESS_MODE_NONE;
        ret = sample_common_vpp_start(vpp_grpid, chn_enable, &vpp_grp_attr, vpp_chn_attr);
        ret = sample_common_vpp_bind_vo(vpp_grpid, vpp_chnid, vo_devid, vo_chnid);

        // [Fix] 传入数组指针
        sample_vio_get_vo_cfg(&img_size, vo_cfg);
        if (vo_cfg[0].vo_intf_type == E_VO_INTERFACE_TYPE_MIPI) {
            ret = sample_common_dsp_init(0, name);
            if (ret != VS_SUCCESS) {
                goto exit5;
            }
        }

        // [Fix] 仅启动 Dev 0
        ret = sample_common_vo_start(&vo_cfg[0]);
        if (ret != VS_SUCCESS) {
            goto exit5;
        }

        vs_mal_isp_comm_attr_set(vii_pipeid, &vii_cfg.route_cfg[0].pipe_cfg[0].isp_attr);
        vii_cfg.route_cfg[0].pipe_cfg[0].firmware_bypass = VS_FALSE;

        vs_isp_ae_attr_s ae_attr;
        vs_isp_wdr_exp_attr_s wdr_exp_attr;
        vs_mal_isp_ae_attr_get(vii_pipeid, &ae_attr);
        vs_mal_isp_ae_attr_set(vii_pipeid, &ae_attr);
        vs_mal_isp_wdr_exp_attr_get(vii_pipeid, &wdr_exp_attr);
        vs_mal_isp_wdr_exp_attr_set(vii_pipeid, &wdr_exp_attr);
    }

    sample_common_vo_stop(&vo_cfg[0]);

exit5:
    if (vo_cfg[0].vo_intf_type == E_VO_INTERFACE_TYPE_MIPI) {
        sample_common_dsp_exit(0);
    }
    sample_common_vpp_unbind_vo(vpp_grpid, vpp_chnid, vo_devid, vo_chnid);
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

vs_void_t sample_vio_usage(char *prog_name)
{
    vs_uint32_t i;
    vs_int32_t sensor_type_num = sample_common_vii_sensor_type_num_get();
#ifdef VS_ORION
    printf("Usage : %s <index> <sensor_type> [i2c_bus_id] [mipi_tx_board_type] [--pal|--ntsc]\n", prog_name);
#else
    printf("Usage : %s <index> <sensor_type> [i2c_bus_id] [--pal|--ntsc]\n", prog_name);
#endif
    printf("index:\n");
    printf("\t 0) vii(online)  --> vpp(online)  --> vo.\n");
    printf("\t 1) vii(online)  --> vpp(offline) --> vo.\n");
    printf("\t 2) vii(offline) --> vpp(online)  --> vo.\n");
    
    printf("\nCVBS Options:\n");
    printf("\t --pal   : Enable CVBS output with PAL standard.\n");
    printf("\t --ntsc  : Enable CVBS output with NTSC standard.\n");
    
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
    vs_int32_t i;

    if (argc <= 2) {
        sample_vio_usage(argv[0]);
        return VS_FAILED;
    }

    if (!strncmp(argv[1], "-h", 2)) {
        sample_vio_usage(argv[0]);
        return VS_FAILED;
    }
    
    /* 参数解析：查找 CVBS 标志 */
    for (i = 1; i < argc; i++) {
        if (strncmp(argv[i], "--pal", 5) == 0) {
            g_cvbs_enable = VS_TRUE;
            g_cvbs_output_type = E_VO_OUTPUT_TYPE_PAL;
            printf(">>> CVBS Enabled: PAL Mode\n");
        } else if (strncmp(argv[i], "--ntsc", 6) == 0) {
            g_cvbs_enable = VS_TRUE;
            g_cvbs_output_type = E_VO_OUTPUT_TYPE_NTSC;
            printf(">>> CVBS Enabled: NTSC Mode\n");
        }
    }

    sample_vio_register_signal_handler(vii_signal_handle);

    case_idx = atoi(argv[1]);
    g_sensor_type[0] = atoi(argv[2]);

    if (argc > 3 && argv[3][0] != '-') {
        g_bus_id[0] = atoi(argv[3]);
    }

    if (argc > 4 && argv[4][0] != '-') {
        mipitx_board_setting_get(argv[4]);
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