/**
 * @file main.c
 * @author XJT
 * @brief VS839 VENC demo - Optimized for low latency with interactive sensor selection
 * @version 0.4.4
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

// 固定配置参数 - 优化后的低延迟配置
#define FIXED_MODE_INDEX 10
#define DEFAULT_I2C_BUS_ID 5
#define SAMPLE_VENC_INPUT_FORMAT E_PIXEL_FORMAT_YVU_420SP

// 低延迟优化配置
#define LOW_LATENCY_VB_COUNT 6          // 减少VB数量以降低延迟
#define LOW_LATENCY_GOP_SIZE 30         // 降低GOP大小
#define LOW_LATENCY_BITRATE_FACTOR 1.2  // 略微提高码率以保证质量

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

// 低延迟优化配置 - 强制使用在线模式
static vs_vii_vpp_mode_e g_vii_vpp_mode = E_VII_ONLINE_VPP_ONLINE;  // 在线模式降低延迟
static volatile sig_atomic_t g_stop_flag = 0;
static vs_uint32_t g_comm_vb_cnt = LOW_LATENCY_VB_COUNT;  // 减少缓冲区数量
static vs_compress_mode_e g_compress_mode = E_COMPRESS_MODE_NONE;    // 无压缩模式
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
    printf("Low Latency Mode: videostrm (H265-%s@30fps + videostrm with optimizations)\n", MAX_RESOLUTION_RATIO);
    printf("Fixed I2C bus: %d\n", DEFAULT_I2C_BUS_ID);
    printf("Optimizations: Online mode, reduced buffers, low latency encoding\n");
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
            input_buffer[strcspn(input_buffer, "\n")] = 0;

            if (strcmp(input_buffer, "q") == 0 || strcmp(input_buffer, "Q") == 0)
            {
                printf("User cancelled sensor selection.\n");
                return -1;
            }

            selected_sensor = atoi(input_buffer);

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

    if (argc > 1 && strstr(argv[argc - 1], "-h"))
    {
        sample_venc_main_usage(argv[0]);
        sample_venc_sensor_type_usage();
        printf("Example:\n");
        printf("\te.g : %s %d\n", argv[0], g_sensor_type[0]);
        return VS_FAILED;
    }

    if (argc == 1)
    {
        selected_sensor = sample_venc_interactive_sensor_select();
        if (selected_sensor < 0)
        {
            return VS_FAILED;
        }
    }
    else if (argc == 2)
    {
        selected_sensor = atoi(argv[1]);

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
        printf("Error: Too many arguments.\n");
        sample_venc_main_usage(argv[0]);
        return VS_FAILED;
    }

    g_sensor_type[0] = selected_sensor;
    g_bus_id[0] = DEFAULT_I2C_BUS_ID;

    printf("Low Latency Configuration: Sensor Type=%d, I2C Bus=%d, VB Count=%d\n", 
           g_sensor_type[0], g_bus_id[0], g_comm_vb_cnt);

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

    // 放宽性能检查以支持更多配置
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
        printf("Low latency VB config - index %d, blk_size[%llu] blk_cnt[%u]!\n", i, vb_cfg.ast_commpool[i].blk_size, vb_cfg.ast_commpool[i].blk_cnt);
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

    vs_sample_trace("vii_init sensor_id[%d] g_sensor_type[%d] (Low Latency Mode)\n", sensor_id, g_sensor_type[0]);

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
            p_vii_cfg->route_cfg[0].pipe_cfg[i].pipe_attr.compress_mode = E_COMPRESS_MODE_NONE;
            p_vii_cfg->route_cfg[0].pipe_cfg[i].phys_chn_cfg[j].chn_attr.framerate.src_framerate = g_sensor_framerate;
            p_vii_cfg->route_cfg[0].pipe_cfg[i].phys_chn_cfg[j].chn_attr.framerate.dst_framerate = g_sensor_framerate;
            p_vii_cfg->route_cfg[0].pipe_cfg[i].pipe_attr.bypass_mode = g_vii_pipe_bypass_mode;
            p_vii_cfg->route_cfg[0].pipe_cfg[i].phys_chn_cfg[j].chn_attr.compress_mode = g_compress_mode;
            // 低延迟优化：设置更小的depth
            p_vii_cfg->route_cfg[0].pipe_cfg[i].phys_chn_cfg[j].chn_attr.depth = 1;
        }
        for (j = 0; j < VII_MAX_EXT_CHN_NUM; j++)
        {
            p_vii_cfg->route_cfg[0].pipe_cfg[i].ext_chn_cfg[j].chn_attr.framerate.src_framerate = g_sensor_framerate;
            p_vii_cfg->route_cfg[0].pipe_cfg[i].ext_chn_cfg[j].chn_attr.framerate.dst_framerate = g_sensor_framerate;
            p_vii_cfg->route_cfg[0].pipe_cfg[i].ext_chn_cfg[j].chn_attr.compress_mode = g_compress_mode;
            p_vii_cfg->route_cfg[0].pipe_cfg[i].ext_chn_cfg[j].chn_attr.depth = 1;
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

// 完全按照原版代码重构的低延迟VPP初始化函数
static vs_int32_t sample_venc_vpp_lowlatency_init(vs_int32_t vpp_grpid, vs_size_s input_size, vs_size_s* p_output_size, vs_bool_t* p_chn_enable)
{
    vs_int32_t i = 0, ret = 0;
    vs_vpp_grp_attr_s vpp_grp_attr;
    vs_vpp_chn_attr_s vpp_chn_attr[VPP_MAX_PHYCHN_NUM];

    printf("Initializing low latency VPP (group %d)...\n", vpp_grpid);

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
            vpp_chn_attr[i].compress_mode = E_COMPRESS_MODE_NONE;  // 低延迟模式强制无压缩
            vpp_chn_attr[i].framerate.src_framerate = g_sensor_framerate;
            vpp_chn_attr[i].framerate.dst_framerate = g_sensor_framerate;
            vpp_chn_attr[i].mirror_enable = VS_FALSE;
            vpp_chn_attr[i].flip_enable = VS_FALSE;
            vpp_chn_attr[i].depth = 0;  // 参考原版代码设置为0
            vpp_chn_attr[i].aspect_ratio.mode = E_ASPECT_RATIO_MODE_NONE;
            
            printf("VPP chn[%d]: width=%d, height=%d, format=%d\n", 
                   i, vpp_chn_attr[i].width, vpp_chn_attr[i].height, vpp_chn_attr[i].pixel_format);
        }
    }

    // 第1步：使用专门的低延迟VPP启动方式（不会自动启用通道）
    ret = sample_common_vpp_lowlatency_start(vpp_grpid, p_chn_enable, &vpp_grp_attr, vpp_chn_attr);
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("sample_common_vpp_lowlatency_start failed, ret[0x%x]\n", ret);
        return ret;
    }

    printf("VPP lowlatency started successfully\n");
    return VS_SUCCESS;
}

// 修改后的低延迟编码器初始化
vs_int32_t sample_venc_lowlatency_chn_init(vs_int32_t venc_chnid,
                                          vs_payload_type_e type,
                                          vs_venc_profile_e profile,
                                          vs_size_s frame_size,
                                          sample_brc_mode_e brc_mode,
                                          vs_venc_gop_attr_s* p_gop_attr)
{
    vs_int32_t ret = VS_SUCCESS;
    sample_venc_cfg_s sample_venc_cfg;

    printf("Initializing VENC channel %d with size %dx%d\n", venc_chnid, frame_size.width, frame_size.height);

    memset(&sample_venc_cfg, 0, sizeof(sample_venc_cfg_s));
    sample_venc_cfg.format = SAMPLE_VENC_INPUT_FORMAT;
    sample_venc_cfg.compress = VS_FALSE;  // 低延迟模式无压缩
    sample_venc_cfg.type = type;
    sample_venc_cfg.profile = profile;
    sample_venc_cfg.frame_size = frame_size;
    sample_venc_cfg.brc_mode = brc_mode;
    sample_venc_cfg.frc.dst_framerate = g_sensor_framerate;
    sample_venc_cfg.frc.src_framerate = g_sensor_framerate;
    sample_venc_cfg.bandwidth_save_strength = 0;
    
    // 低延迟编码优化 - 通过BRC属性设置GOP大小
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

    // 在编码器启动后设置低延迟相关的BRC参数
    if (type == E_PT_TYPE_H265)
    {
        vs_venc_brc_param_s brc_param;
        memset(&brc_param, 0, sizeof(vs_venc_brc_param_s));
        
        // 获取当前BRC参数
        ret = vs_mal_venc_brc_param_get(venc_chnid, &brc_param);
        if (ret == VS_SUCCESS)
        {
            // 设置低延迟相关参数
            if (brc_mode == E_VENC_BRC_CBR)
            {
                // 为CBR模式优化低延迟参数
                brc_param.h265_cbr.min_qp = 20;      // 提高最小QP以降低延迟
                brc_param.h265_cbr.max_qp = 45;      // 降低最大QP以保证质量
                brc_param.h265_cbr.min_qp_i = 18;    // I帧最小QP
                brc_param.h265_cbr.max_qp_i = 40;    // I帧最大QP
            }
            brc_param.first_pic_init_qp = 25;  // 设置初始QP
            brc_param.skip_level = E_VENC_SKIP_LEVEL_3;  // 较低的跳过级别
            
            ret = vs_mal_venc_brc_param_set(venc_chnid, &brc_param);
            if (ret != VS_SUCCESS)
            {
                vs_sample_trace("vs_mal_venc_brc_param_set failed, ret[0x%x]\n", ret);
                // 继续执行，这不是致命错误
            }
        }
    }

    printf("Low latency encoder initialized for channel %d\n", venc_chnid);
    return VS_SUCCESS;
}

// 修改后的低延迟GOP属性获取函数
static vs_int32_t sample_venc_lowlatency_gop_attr_get(vs_venc_gop_mode_e gop_mode, vs_venc_gop_attr_s* p_gop_attr)
{
    vs_int32_t ret = VS_SUCCESS;
    
    // 先获取默认GOP属性
    ret = sample_common_venc_gop_attr_get(gop_mode, p_gop_attr);
    if (ret != VS_SUCCESS)
    {
        return ret;
    }
    
    // 为低延迟优化GOP参数
    if (gop_mode == E_VENC_GOP_MODE_NORMP)
    {
        // 优化normp模式的参数
        p_gop_attr->normp.qpdelta_i_p = -2;  // I帧相对P帧QP差值，降低以减少I帧大小
    }
    
    return VS_SUCCESS;
}

vs_int32_t sample_venc_lowlatency_videostrm(vs_int32_t argc, char* argv[])
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
    vs_lowlatency_attr_s lowlatency_attr;
    vs_int32_t i = 0;

    printf("\n=====Starting Low Latency Video Stream Mode=====\n");

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
    printf("Sensor size: %dx%d, framerate: %d\n", sensor_size.width, sensor_size.height, g_sensor_framerate);
    
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

    // 使用修改后的GOP属性获取函数
    ret = sample_venc_lowlatency_gop_attr_get(gop_mode, &gop_attr);
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("sample_venc_lowlatency_gop_attr_get failed, ret[0x%x]\n", ret);
        goto exit_vii_stop;
    }

    vpp_output_size[0] = sensor_size;
    // 第1步：初始化VPP（不启用通道）
    ret = sample_venc_vpp_lowlatency_init(vpp_grpid, sensor_size, vpp_output_size, vpp_chn_enable);
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("sample_venc_vpp_lowlatency_init failed, ret[0x%x]\n", ret);
        goto exit_vii_stop;
    }

    // 第2步：初始化VENC
    ret = sample_venc_lowlatency_chn_init(venc_chnid[0], encode_type[0], profile[0], vpp_output_size[0], brc_mode, &gop_attr);
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("sample_venc_lowlatency_chn_init failed, ret[0x%x]\n", ret);
        goto exit_vpp_stop;
    }

    // 第3步：绑定VPP到VENC（关键：在启用VPP通道前绑定）
    printf("Binding VPP to VENC...\n");
    ret = sample_common_vpp_bind_venc(vpp_grpid, vpp_chnid, venc_devid, venc_chnid[0]);
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("sample_common_vpp_bind_venc failed, ret[0x%x]\n", ret);
        goto exit_venc0_stop;
    }

    // 第4步：设置低延迟属性
    printf("Setting low latency attributes...\n");
    lowlatency_attr.enable = VS_TRUE;
    lowlatency_attr.mode = E_LOWLATENCY_MODE_VENC_ONLY;
    if (encode_type[0] == E_PT_TYPE_H264) {
        lowlatency_attr.linebuf_mode = E_LOWLATENCY_LINEBUF_MODE_16_LINE;
    } else {
        lowlatency_attr.linebuf_mode = E_LOWLATENCY_LINEBUF_MODE_64_LINE;
    }
    
    ret = vs_mal_vpp_chn_lowlatency_set(vpp_grpid, vpp_chnid, &lowlatency_attr);
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("vs_mal_vpp_chn_lowlatency_set(grp %d chn %d) failed with 0x%x\n", vpp_grpid, vpp_chnid, ret);
        goto exit_vpp_unbind_venc;
    }

    // 第5步：手动启用VPP通道
    printf("Enabling VPP channels...\n");
    for (i = 0; i < VPP_MAX_PHYCHN_NUM; i++)
    {
        if (vpp_chn_enable[i] == VS_TRUE)
        {
            ret = vs_mal_vpp_chn_enable(vpp_grpid, i);
            if (ret != VS_SUCCESS)
            {
                vs_sample_trace("vs_mal_vpp_chn_enable failed for chn[%d], ret[0x%x]\n", i, ret);
                goto exit_vpp_unbind_venc;
            }
            printf("VPP channel %d enabled successfully\n", i);
        }
    }

    // 第6步：最后绑定VII到VPP以开始数据流
    printf("Binding VII to VPP...\n");
    ret = sample_common_vii_bind_vpp(vii_pipeid, vii_chnid, vpp_grpid);
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("sample_common_vii_bind_vpp failed, ret[0x%x]\n", ret);
        goto exit_vpp_unbind_venc;
    }

    printf("=====Low Latency Pipeline Established=====\n");
    printf("Optimizations applied:\n");
    printf("- Online VII-VPP mode\n");
    printf("- Reduced VB buffers: %d\n", g_comm_vb_cnt);
    printf("- Low latency VPP mode\n");
    printf("- Low latency encoder with optimized BRC\n");
    printf("- Optimized GOP parameters\n");
    printf("- No compression mode\n");
    printf("- Correct binding sequence for low latency\n");
    
    // 等待一会儿让数据流稳定
    printf("Waiting for pipeline to stabilize...\n");
    sleep(3);
    
    sample_venc_pause();

    vs_sample_trace("exit_vii_unbind_vpp\n");
    sample_common_vii_unbind_vpp(vii_pipeid, vii_chnid, vpp_grpid);
exit_vpp_unbind_venc:
    vs_sample_trace("exit_vpp_unbind_venc0\n");
    sample_common_vpp_unbind_venc(vpp_grpid, vpp_chnid, venc_devid, venc_chnid[0]);
exit_venc0_stop:
    vs_sample_trace("exit_venc0_stop\n");
    sample_common_venc_stop(venc_chnid[0]);
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

    vs_sample_trace("sample_venc_lowlatency_videostrm mode.\n");
    ret = sample_venc_lowlatency_videostrm(argc, argv);

    return ret;
}