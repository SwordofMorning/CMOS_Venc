/**
 * @file    sample_vo_only.c
 * @brief   VO and MIPI-TX initialization only
 * @details Initialize display output without video input pipeline
 * @author  Modified for VO-only usage
 * @date    2025-01-01
 * @version v1.00
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

static vs_void_t sample_vo_only_get_vo_cfg(vs_size_s *img_size, sample_vo_cfg_s *vo_cfg)
{
    vo_cfg->vo_devid = 0;
    vo_cfg->vo_layerid = 0;

    vo_cfg->vo_intf_type = E_VO_INTERFACE_TYPE_MIPI;
    vo_cfg->vo_output = E_VO_OUTPUT_TYPE_USER;
    vo_cfg->bg_color = 0x000000;  // 黑色背景
    vo_cfg->dynamic_range = E_DYNAMIC_RANGE_SDR8;
    vo_cfg->pixel_format = E_PIXEL_FORMAT_YVU_420SP;
    vo_cfg->vo_mode = E_VO_MODE_1MUX;
    
    // 设置显示分辨率
    vo_cfg->img_width = img_size->width;
    vo_cfg->img_height = img_size->height;
    vo_cfg->enable = VS_TRUE;
    vo_cfg->zorder = 0;
    vo_cfg->mipitx_phy_rate = 820;
    vo_cfg->rotation_enable = VS_FALSE;
    
    vs_sample_trace("VO config: %dx%d, format=%d, intf=%d\n", 
                    vo_cfg->img_width, vo_cfg->img_height, 
                    vo_cfg->pixel_format, vo_cfg->vo_intf_type);
}

vs_int32_t sample_vo_only_init_case(vs_size_s *display_size)
{
    vs_int32_t ret;
    vs_vb_cfg_s vb_cfg = {0};
    sample_vo_cfg_s vo_cfg = {0};
    vs_char_t dsp_firmware[100] = "/lib/firmware/vs_dsp0.bin";

    // 配置最小的视频缓冲区（虽然不使用，但系统可能需要）
    vb_cfg.pool_cnt = 1;
    vb_cfg.ast_commpool[0].blk_size = sample_common_buffer_size_get(display_size, 
                                                                    E_PIXEL_FORMAT_YVU_420SP, 
                                                                    E_COMPRESS_MODE_NONE, 1);
    vb_cfg.ast_commpool[0].blk_cnt = 2;  // 最小配置
    vb_cfg.ast_commpool[0].remap_mode = VB_REMAP_MODE_NONE;

    // 初始化系统
    ret = sample_common_sys_init(&vb_cfg);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("sample_common_sys_init failed, ret: 0x%x\n", ret);
        goto exit0;
    }

    // 获取VO配置
    sample_vo_only_get_vo_cfg(display_size, &vo_cfg);

    // 初始化DSP（MIPI-TX需要）
    if (vo_cfg.vo_intf_type == E_VO_INTERFACE_TYPE_MIPI) {
        ret = sample_common_dsp_init(0, dsp_firmware);
        if (ret != VS_SUCCESS) {
            vs_sample_trace("sample_common_dsp_init failed, ret: 0x%x\n", ret);
            goto exit1;
        }
    }

    // 启动VO
    ret = sample_common_vo_start(&vo_cfg);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("sample_common_vo_start failed, ret: 0x%x\n", ret);
        goto exit2;
    }

    vs_sample_trace("VO and MIPI-TX initialization completed successfully!\n");
    vs_sample_trace("Display resolution: %dx%d\n", display_size->width, display_size->height);
    vs_sample_trace("You can now operate framebuffer directly for static display.\n");

    // 等待用户输入或信号退出
    sample_common_pause();

    // 清理部分
    sample_common_vo_stop(&vo_cfg);

exit2:
    if (vo_cfg.vo_intf_type == E_VO_INTERFACE_TYPE_MIPI) {
        sample_common_dsp_exit(0);
    }
exit1:
    sample_common_sys_exit();
exit0:
    return ret;
}

// 预定义一些常用分辨率
static vs_void_t sample_vo_only_get_resolution(vs_int32_t res_idx, vs_size_s *size)
{
    switch (res_idx) {
        case 0:
            size->width = 1280;
            size->height = 960;
            break;
        case 1:
            size->width = 1280;
            size->height = 720;
            break;
        case 2:
            size->width = 1920;
            size->height = 1080;
            break;
        case 3:
            size->width = 1024;
            size->height = 768;
            break;
        case 4:
            size->width = 800;
            size->height = 600;
            break;
        default:
            size->width = 1280;
            size->height = 960;
            break;
    }
}

vs_void_t sample_vo_only_usage(char *prog_name)
{
    printf("Usage : %s <resolution_index>\n", prog_name);
    printf("resolution_index:\n");
    printf("\t 0) 1280x960 (default)\n");
    printf("\t 1) 1280x720\n");
    printf("\t 2) 1920x1080\n");
    printf("\t 3) 1024x768\n");
    printf("\t 4) 800x600\n");
    printf("\nThis program only initializes VO and MIPI-TX for framebuffer operation.\n");
    printf("No video input pipeline is created.\n");
}

static vs_void_t sample_vo_only_register_signal_handler(void (*sig_handler)(int))
{
    struct sigaction sa;
    memset(&sa, 0, sizeof(struct sigaction));
    sa.sa_handler = sig_handler;
    sa.sa_flags = 0;

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
}

vs_void_t vo_only_signal_handle(vs_int32_t s_no)
{
    if (s_no == SIGINT || s_no == SIGTERM) {
        g_stop_flag = 1;
        vs_sample_trace("Received signal %d, preparing to exit...\n", s_no);
    }
}

int main(int argc, char *argv[])
{
    vs_int32_t ret = VS_SUCCESS;
    vs_int32_t res_idx = 0;
    vs_size_s display_size = {0};

    if (argc <= 1) {
        sample_vo_only_usage(argv[0]);
        return VS_SUCCESS;  // 显示帮助后正常退出
    }

    if (!strncmp(argv[1], "-h", 2)) {
        sample_vo_only_usage(argv[0]);
        return VS_SUCCESS;
    }

    // 注册信号处理器
    sample_vo_only_register_signal_handler(vo_only_signal_handle);

    // 获取分辨率参数
    res_idx = atoi(argv[1]);
    sample_vo_only_get_resolution(res_idx, &display_size);

    vs_sample_trace("Starting VO-only initialization...\n");
    vs_sample_trace("Target resolution: %dx%d\n", display_size.width, display_size.height);

    // 执行VO初始化
    ret = sample_vo_only_init_case(&display_size);

    if (ret == VS_SUCCESS) {
        vs_sample_trace("VO-only initialization completed successfully.\n");
    } else {
        vs_sample_trace("VO-only initialization failed with error: 0x%x\n", ret);
    }

    return ret;
}