/**
 * @file main.c
 * @author XJT
 * @brief VS839 VENC demo - Simplified for videostrm mode only with interactive sensor selection
 * @version 0.3
 * @date 2025-06-19
 * 
 * @copyright Copyright (c) 2025
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

// 固定配置参数
#define FIXED_MODE_INDEX 10
#define DEFAULT_I2C_BUS_ID 5
#define SAMPLE_VENC_INPUT_FORMAT E_PIXEL_FORMAT_YVU_420SP

#define SAMPLE_VENC_CHNID_VALID_CHECK(venc_chnid)                                                                                                                                  \
    do                                                                                                                                                                             \
    {                                                                                                                                                                              \
        if (venc_chnid < 0 || venc_chnid >= VENC_MAX_CHN_NUM)                                                                                                                      \
        {                                                                                                                                                                          \
            vs_sample_trace("channel [%d] not exist \n", venc_chnid);                                                                                                              \
            return VS_ERR_VENC_INVALID_CHNID;                                                                                                                                      \
        }                                                                                                                                                                          \
    } while (0)

#define SAMPLE_VENC_CHECK_NULLPTR(ptr)                                                                                                                                             \
    do                                                                                                                                                                             \
    {                                                                                                                                                                              \
        if (ptr == VS_NULL)                                                                                                                                                        \
        {                                                                                                                                                                          \
            vs_sample_trace("check null point!\n");                                                                                                                                \
            return VS_ERR_VENC_NULL_PTR;                                                                                                                                           \
        }                                                                                                                                                                          \
    } while (0);

#if defined(VS_815)
#define MAX_RESOLUTION_RATIO "5M"
#else
#define MAX_RESOLUTION_RATIO "4K"
#endif

extern vs_int8_t g_bus_id[VII_MAX_ROUTE_NUM];
extern sample_sensor_type_e g_sensor_type[VII_MAX_ROUTE_NUM];
extern vs_bool_t g_nr_3d;

static vs_vii_vpp_mode_e g_vii_vpp_mode = E_VII_OFFLINE_VPP_ONLINE;
static volatile sig_atomic_t g_stop_flag = 0;
static vs_uint32_t g_comm_vb_cnt = 30;
static vs_compress_mode_e g_compress_mode = E_COMPRESS_MODE_RASTER;
static vs_int32_t g_sensor_framerate = 30;
static vs_uint32_t g_pool_cnt = 1;
static vs_uint32_t g_buffer_dimension = 1;
static vs_vii_pipe_bypass_mode_e g_vii_pipe_bypass_mode = E_VII_PIPE_BYPASS_NONE;
#ifndef VS_NO_ISP
static vs_mirror_mode_e g_vii_sensor_mirror_mode = E_MIRROR_MODE_NONE;
#endif
static vs_uint32_t g_fpn_enable = 0;
static sample_fpn_frame_info_s g_fpn_frame_info;

static vs_void_t sample_venc_main_usage(char* prog_name)
{
    printf("Usage : %s [sensor_type]\n", prog_name);
    printf("Fixed mode: videostrm (H265-%s@30fps + videostrm)\n", MAX_RESOLUTION_RATIO);
    printf("Fixed I2C bus: %d\n", DEFAULT_I2C_BUS_ID);
    printf("If no sensor_type provided, interactive selection will be available.\n");
}

static vs_void_t sample_venc_sensor_type_usage()
{
    vs_int32_t i;
    vs_int32_t sensor_type_num = sample_common_vii_sensor_type_num_get();

    printf("sensor_type:\n");
    for (i = 0; i < sensor_type_num; i++)
    {
        printf("\t %d) %s.\n", i, sample_common_sensor_type_name_get(i));
    }
}

static vs_int32_t sample_venc_interactive_sensor_select()
{
    vs_int32_t sensor_type_num = sample_common_vii_sensor_type_num_get();
    vs_int32_t selected_sensor = -1;
    char input_buffer[32];

    printf("\n=====Interactive Sensor Selection=====\n");
    sample_venc_sensor_type_usage();

    while (1)
    {
        printf("\nPlease select sensor type (0-%d) or 'q' to quit: ", sensor_type_num - 1);
        fflush(stdout);

        if (fgets(input_buffer, sizeof(input_buffer), stdin) != NULL)
        {
            // 移除换行符
            input_buffer[strcspn(input_buffer, "\n")] = 0;

            // 检查是否要退出
            if (strcmp(input_buffer, "q") == 0 || strcmp(input_buffer, "Q") == 0)
            {
                printf("User cancelled sensor selection.\n");
                return -1;
            }

            // 转换为整数
            selected_sensor = atoi(input_buffer);

            // 验证输入范围
            if (selected_sensor >= 0 && selected_sensor < sensor_type_num)
            {
                printf("Selected sensor: %d) %s\n", selected_sensor, sample_common_sensor_type_name_get(selected_sensor));
                return selected_sensor;
            }
            else
            {
                printf("Invalid input! Please enter a number between 0 and %d.\n", sensor_type_num - 1);
            }
        }
    }
}

vs_void_t sample_venc_pause(vs_void_t)
{
    sleep(1);
    printf("\n=====Press enter to exit sample_venc=====\n");
    while (!g_stop_flag)
    {
        if (getchar() == '\n')
        {
            break;
        }
        usleep(10000);
    };
    printf("\n=====exit sample_venc=====\n");
}

static vs_int32_t sample_venc_param_parse(vs_int32_t argc, char* argv[])
{
    vs_int32_t sensor_type_num = sample_common_vii_sensor_type_num_get();
    vs_int32_t selected_sensor = -1;

    // 检查帮助参数
    if (argc > 1 && strstr(argv[argc - 1], "-h"))
    {
        sample_venc_main_usage(argv[0]);
        sample_venc_sensor_type_usage();
        printf("Example:\n");
        printf("\te.g : %s %d\n", argv[0], g_sensor_type[0]);
        return VS_FAILED;
    }

    // 参数处理
    if (argc == 1)
    {
        // 没有提供参数，使用交互式选择
        selected_sensor = sample_venc_interactive_sensor_select();
        if (selected_sensor < 0)
        {
            return VS_FAILED;
        }
    }
    else if (argc == 2)
    {
        // 提供了sensor_type参数
        selected_sensor = atoi(argv[1]);

        // 验证sensor_type范围
        if (selected_sensor < 0 || selected_sensor >= sensor_type_num)
        {
            printf("Error: Invalid sensor_type %d. Valid range: 0-%d\n", selected_sensor, sensor_type_num - 1);
            sample_venc_sensor_type_usage();
            return VS_FAILED;
        }

        printf("Using sensor: %d) %s\n", selected_sensor, sample_common_sensor_type_name_get(selected_sensor));
    }
    else
    {
        // 参数过多
        printf("Error: Too many arguments.\n");
        sample_venc_main_usage(argv[0]);
        return VS_FAILED;
    }

    // 设置全局变量
    g_sensor_type[0] = selected_sensor;
    g_bus_id[0] = DEFAULT_I2C_BUS_ID; // 固定使用默认I2C总线

    printf("Configuration: Sensor Type=%d, I2C Bus=%d\n", g_sensor_type[0], g_bus_id[0]);

    return VS_SUCCESS;
}

vs_int32_t sample_venc_check_performance(vs_int32_t sensor_id, vs_bool_t* p_vpp_chn_enable)
{
    vs_int32_t ret = VS_SUCCESS, i = 0, vpp_chn_num = 0;
    vs_size_s sensor_size;
    vs_int32_t sensor_framerate = 30;

    sample_common_vii_sensor_img_size_get(sensor_id, &sensor_size);

    ret = sample_common_vii_sensor_framerate_get(sensor_id, &sensor_framerate);
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("sample_common_vii_sensor_framerate_get failed, ret[%d]\n", ret);
        return ret;
    }

    for (i = 0; i < VPP_MAX_PHYCHN_NUM; i++)
    {
        if (p_vpp_chn_enable[i] == VS_TRUE)
        {
            vpp_chn_num++;
        }
    }

    if (sensor_size.width >= PIC_4K_WIDTH && sensor_size.height >= PIC_4K_HEIGHT && sensor_framerate >= 60 && vpp_chn_num > 1)
    {
        vs_sample_trace("check_performance failed,vpp_chn_num[%d] width[%u] height[%u] framerate[%d]\n", vpp_chn_num, sensor_size.width, sensor_size.height, sensor_framerate);
        return VS_FAILED;
    }

    return VS_SUCCESS;
}

static vs_int32_t sample_venc_sys_init(vs_int32_t sensor_id, vs_size_s input_size)
{
    vs_uint32_t i = 0;
    vs_vb_cfg_s vb_cfg;
    vs_uint32_t frame_num;
    vs_int32_t ret = VS_SUCCESS;
    vs_pixel_format_e sensor_format;

    frame_num = sample_common_vii_wdr_frame_num_get(sensor_id);
    sample_common_vii_sensor_pixel_format_get(sensor_id, &sensor_format);

    memset(&vb_cfg, 0, sizeof(vs_vb_cfg_s));
    vb_cfg.pool_cnt = g_pool_cnt;
    for (i = 0; i < g_pool_cnt; i++)
    {
        vb_cfg.ast_commpool[i].blk_size = sample_common_buffer_size_get(&input_size, sensor_format, g_compress_mode, frame_num) * g_buffer_dimension;
        vb_cfg.ast_commpool[i].blk_cnt = g_comm_vb_cnt;
        vb_cfg.ast_commpool[i].remap_mode = VB_REMAP_MODE_NONE;
        printf("index %d, blk_size[%llu] blk_cnt[%u]!\n", i, vb_cfg.ast_commpool[i].blk_size, vb_cfg.ast_commpool[i].blk_cnt);
    }

    ret = sample_common_sys_init(&vb_cfg);
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("sample_common_sys_init failed, ret[0x%x]\n", ret);
        return ret;
    }

    return VS_SUCCESS;
}

vs_int32_t sample_venc_vii_init(vs_int32_t sensor_id, sample_vii_cfg_s* p_vii_cfg)
{
    vs_int32_t ret = VS_SUCCESS;
    vs_int32_t i = 0, j = 0;

    vs_sample_trace("vii_init sensor_id[%d] g_sensor_type[%d]\n", sensor_id, g_sensor_type[0]);

    p_vii_cfg->vii_vpp_mode = g_vii_vpp_mode;
    p_vii_cfg->route_num = 1;
    sample_common_vii_default_cfg_get(sensor_id, &p_vii_cfg->route_cfg[0]);

    ret = sample_common_vii_sensor_framerate_get(sensor_id, &g_sensor_framerate);
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("sample_common_vii_sensor_framerate_get failed, ret[%d]\n", ret);
        return ret;
    }
    for (i = 0; i < DEV_BIND_MAX_PIPE_NUM; i++)
    {
        for (j = 0; j < VII_MAX_PHYS_CHN_NUM; j++)
        {
#ifdef VS_ORION
            p_vii_cfg->route_cfg[0].pipe_cfg[i].pipe_attr.compress_mode = E_COMPRESS_MODE_NONE;
#else
            p_vii_cfg->route_cfg[0].pipe_cfg[i].pipe_attr.compress_mode = E_COMPRESS_MODE_NONE;
#endif
            p_vii_cfg->route_cfg[0].pipe_cfg[i].phys_chn_cfg[j].chn_attr.framerate.src_framerate = g_sensor_framerate;
            p_vii_cfg->route_cfg[0].pipe_cfg[i].phys_chn_cfg[j].chn_attr.framerate.dst_framerate = g_sensor_framerate;
            p_vii_cfg->route_cfg[0].pipe_cfg[i].pipe_attr.bypass_mode = g_vii_pipe_bypass_mode;
            p_vii_cfg->route_cfg[0].pipe_cfg[i].phys_chn_cfg[j].chn_attr.compress_mode = g_compress_mode;
        }
        for (j = 0; j < VII_MAX_EXT_CHN_NUM; j++)
        {
            p_vii_cfg->route_cfg[0].pipe_cfg[i].ext_chn_cfg[j].chn_attr.framerate.src_framerate = g_sensor_framerate;
            p_vii_cfg->route_cfg[0].pipe_cfg[i].ext_chn_cfg[j].chn_attr.framerate.dst_framerate = g_sensor_framerate;
            p_vii_cfg->route_cfg[0].pipe_cfg[i].ext_chn_cfg[j].chn_attr.compress_mode = g_compress_mode;
        }
    }

    if (1 == g_fpn_enable)
    {
        sample_common_get_fpn_cfg(&p_vii_cfg->route_cfg[0].pipe_cfg[0], &g_fpn_frame_info);
    }

    ret = sample_common_vii_start(p_vii_cfg);
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("sample_common_vii_start failed, ret[0x%x]\n", ret);
        return ret;
    }

#ifndef VS_NO_ISP
    sample_common_vii_mirror_mode_set(sensor_id, g_vii_sensor_mirror_mode);
#endif
    return VS_SUCCESS;
}

static vs_int32_t sample_venc_vpp_init(vs_int32_t vpp_grpid, vs_size_s input_size, vs_size_s* p_output_size, vs_bool_t* p_chn_enable, vs_bool_t lowlatency_enable)
{
    vs_int32_t i = 0, ret = 0;
    vs_vpp_grp_attr_s vpp_grp_attr;
    vs_vpp_chn_attr_s vpp_chn_attr[VPP_MAX_PHYCHN_NUM];

    memset(&vpp_grp_attr, 0, sizeof(vpp_grp_attr));
    vpp_grp_attr.max_width = input_size.width;
    vpp_grp_attr.max_height = input_size.height;
    vpp_grp_attr.dynamic_range = E_DYNAMIC_RANGE_SDR8;
    vpp_grp_attr.pixel_format = E_PIXEL_FORMAT_YVU_420SP;
    vpp_grp_attr.framerate.dst_framerate = g_sensor_framerate;
    vpp_grp_attr.framerate.src_framerate = g_sensor_framerate;

    memset(&vpp_chn_attr, 0, sizeof(vpp_chn_attr));
    for (i = 0; i < VPP_MAX_PHYCHN_NUM; i++)
    {
        if (p_chn_enable[i] == VS_TRUE)
        {
            vpp_chn_attr[i].chn_mode = E_VPP_CHN_MODE_USER;
            vpp_chn_attr[i].width = p_output_size[i].width;
            vpp_chn_attr[i].height = p_output_size[i].height;
            vpp_chn_attr[i].video_format = E_VIDEO_FORMAT_LINEAR;
            vpp_chn_attr[i].pixel_format = SAMPLE_VENC_INPUT_FORMAT;
            vpp_chn_attr[i].dynamic_range = E_DYNAMIC_RANGE_SDR8;
            if (lowlatency_enable == VS_TRUE)
            {
                vpp_chn_attr[i].compress_mode = E_COMPRESS_MODE_NONE;
            }
            else
            {
                vpp_chn_attr[i].compress_mode = g_compress_mode;
            }
            vpp_chn_attr[i].framerate.src_framerate = g_sensor_framerate;
            vpp_chn_attr[i].framerate.dst_framerate = g_sensor_framerate;
            vpp_chn_attr[i].mirror_enable = VS_FALSE;
            vpp_chn_attr[i].flip_enable = VS_FALSE;
            vpp_chn_attr[i].depth = 0;
            vpp_chn_attr[i].aspect_ratio.mode = E_ASPECT_RATIO_MODE_NONE;
        }
    }
    if (lowlatency_enable == VS_TRUE)
    {
        ret = sample_common_vpp_lowlatency_start(vpp_grpid, p_chn_enable, &vpp_grp_attr, vpp_chn_attr);
    }
    else
    {
        ret = sample_common_vpp_start(vpp_grpid, p_chn_enable, &vpp_grp_attr, vpp_chn_attr);
    }
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("sample_common_vpp_start failed, ret[0x%x]\n", ret);
        return ret;
    }
    return ret;
}

vs_int32_t sample_venc_chn_init(vs_int32_t venc_chnid,
                                vs_payload_type_e type,
                                vs_venc_profile_e profile,
                                vs_size_s frame_size,
                                sample_brc_mode_e brc_mode,
                                vs_venc_gop_attr_s* p_gop_attr)

{
    vs_int32_t ret = VS_SUCCESS;
    sample_venc_cfg_s sample_venc_cfg;

    memset(&sample_venc_cfg, 0, sizeof(sample_venc_cfg_s));
    sample_venc_cfg.format = SAMPLE_VENC_INPUT_FORMAT;
    sample_venc_cfg.compress = (g_compress_mode == E_COMPRESS_MODE_NONE) ? VS_FALSE : VS_TRUE;
    sample_venc_cfg.type = type;
    sample_venc_cfg.profile = profile;
    sample_venc_cfg.frame_size = frame_size;
    sample_venc_cfg.brc_mode = brc_mode;
    sample_venc_cfg.frc.dst_framerate = g_sensor_framerate;
    sample_venc_cfg.frc.src_framerate = g_sensor_framerate;
    sample_venc_cfg.bandwidth_save_strength = 0;
    if (p_gop_attr != NULL)
    {
        sample_venc_cfg.gop_attr = *p_gop_attr;
    }
    ret = sample_common_venc_start(venc_chnid, &sample_venc_cfg);
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("sample_common_venc_start failed, ret[0x%x]\n", ret);
        return ret;
    }

    return ret;
}

vs_int32_t sample_venc_videostrm(vs_int32_t argc, char* argv[])
{
    vs_int32_t ret = VS_SUCCESS;
    vs_int32_t sensor_id = 0;
    vs_size_s sensor_size;
    vs_int32_t vii_pipeid = 0;
    vs_int32_t vii_chnid = 0;
    sample_vii_cfg_s vii_cfg;
    vs_int32_t vpp_grpid = 0;
    vs_int32_t vpp_chnid = 0;
    vs_size_s vpp_output_size[VPP_MAX_PHYCHN_NUM] = {0};
    vs_bool_t vpp_chn_enable[VPP_MAX_PHYCHN_NUM] = {VS_TRUE, VS_FALSE, VS_FALSE, VS_FALSE};
    vs_int32_t venc_devid = 0;
    vs_int32_t venc_chnid[1] = {0};
    vs_payload_type_e encode_type[1] = {E_PT_TYPE_H265};
    vs_venc_profile_e profile[1] = {E_VENC_PROFILE_H265_MAIN};
    sample_brc_mode_e brc_mode = E_VENC_BRC_CBR;
    vs_venc_gop_mode_e gop_mode = E_VENC_GOP_MODE_NORMP;
    vs_venc_gop_attr_s gop_attr;

    ret = sample_venc_param_parse(argc, argv);
    if (ret != VS_SUCCESS)
    {
        return ret;
    }

    ret = sample_venc_check_performance(sensor_id, vpp_chn_enable);
    if (ret != VS_SUCCESS)
    {
        return ret;
    }

    sample_common_vii_sensor_img_size_get(sensor_id, &sensor_size);
    ret = sample_venc_sys_init(sensor_id, sensor_size);
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("sample_venc_sys_init failed, ret[0x%x]\n", ret);
        return ret;
    }

    ret = sample_venc_vii_init(sensor_id, &vii_cfg);
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("sample_venc_vii_init failed, ret[0x%x]\n", ret);
        goto exit_sys_exit;
    }

    vpp_output_size[0] = sensor_size;
    ret = sample_venc_vpp_init(vpp_grpid, sensor_size, vpp_output_size, vpp_chn_enable, VS_FALSE);
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("sample_common_vpp_start failed, ret[0x%x]\n", ret);
        goto exit_vii_stop;
    }

    ret = sample_common_vii_bind_vpp(vii_pipeid, vii_chnid, vpp_grpid);
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("sample_common_vii_bind_vpp failed, ret[0x%x]\n", ret);
        goto exit_vpp_stop;
    }

    ret = sample_common_venc_gop_attr_get(gop_mode, &gop_attr);
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("gop_attr_get failed, ret[0x%x]\n", ret);
        goto exit_vii_unbind_vpp;
    }
    ret = sample_venc_chn_init(venc_chnid[0], encode_type[0], profile[0], vpp_output_size[0], brc_mode, &gop_attr);
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("sample_venc_chn_init failed, ret[0x%x]\n", ret);
        goto exit_vii_unbind_vpp;
    }

    ret = sample_common_vpp_bind_venc(vpp_grpid, vpp_chnid, venc_devid, venc_chnid[0]);
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("sample_common_vpp_bind_venc failed, ret[0x%x]\n", ret);
        goto exit_venc0_stop;
    }
    sample_venc_pause();

    vs_sample_trace("exit_venc_acquire_stream_stop \n");

    vs_sample_trace("exit_vpp_unbind_venc0\n");
    sample_common_vpp_unbind_venc(vpp_grpid, vpp_chnid, venc_devid, venc_chnid[0]);
exit_venc0_stop:
    vs_sample_trace("exit_venc0_stop\n");
    sample_common_venc_stop(venc_chnid[0]);
exit_vii_unbind_vpp:
    vs_sample_trace("exit_vii_unbind_vpp\n");
    sample_common_vii_unbind_vpp(vii_pipeid, vii_chnid, vpp_grpid);
exit_vpp_stop:
    vs_sample_trace("exit_vpp_stop\n");
    sample_common_vpp_stop(vpp_grpid, vpp_chn_enable);
exit_vii_stop:
    vs_sample_trace("exit_vii_stop\n");
    sample_common_vii_stop(&vii_cfg);
exit_sys_exit:
    vs_sample_trace("exit_sys_exit\n");
    sample_common_sys_exit();
    return ret;
}

static vs_void_t sample_venc_register_signal_handler(void (*sig_handler)(int))
{
    struct sigaction sa;
    memset(&sa, 0, sizeof(struct sigaction));
    sa.sa_handler = sig_handler;
    sa.sa_flags = 0;

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
}

vs_void_t venc_signal_handle(vs_int32_t s_no)
{
    printf("received signal %d\n", s_no);
    g_stop_flag = 1;
}

vs_int32_t main(vs_int32_t argc, char* argv[])
{
    vs_int32_t ret = VS_SUCCESS;

    if (argc > 1 && strstr(argv[argc - 1], "-h"))
    {
        sample_venc_main_usage(argv[0]);
        sample_venc_sensor_type_usage();
        return VS_FAILED;
    }

    sample_venc_register_signal_handler(venc_signal_handle);

    vs_sample_trace("sample_venc_videostrm mode (fixed).\n");
    ret = sample_venc_videostrm(argc, argv);

    return ret;
}