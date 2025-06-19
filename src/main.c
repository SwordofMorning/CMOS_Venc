/**
 * @file    sample_venc.c
 * @brief   sample venc api implementation
 * @details
 * @author  Visinex Software Group
 * @date    2022-05-22
 * @version v1.00
 * @Copyright (c) 2021 Shanghai Visinex Technologies Co., Ltd. All rights reserved.
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
#include <sys/socket.h>
#include <arpa/inet.h>

#include "sample_common.h"

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

#define SAMPLE_VENC_INPUT_FORMAT E_PIXEL_FORMAT_YVU_420SP
#define SAMPLE_MAX_UDP_SEND_LEN 0xf000
#define SAMPLE_UDP_MAGIC_CODE 0xaabbcc

typedef enum lowlatency_aquire_strm_type
{
    LOWLATENCY_AQUIRE_STRM_TYPE_FILE = 0,
    LOWLATENCY_AQUIRE_STRM_TYPE_UDP,
    LOWLATENCY_AQUIRE_STRM_TYPE_MAX
} lowlatency_aquire_strm_type_e;

typedef struct lowlatency_udp_info
{
    vs_int32_t socket_fd;
    struct sockaddr_in socket_addr;
} lowlatency_udp_info_s;

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
#if defined(VS_815)
#define MAX_RESOLUTION_RATIO "5M"
#else
#define MAX_RESOLUTION_RATIO "4K"
#endif

static vs_void_t sample_venc_main_index_usage(vs_int32_t index)
{
    switch (index)
    {
    case 1:
        printf("\t 1) typical: H265-%s@30fps + H264-%s@30fps.\n", MAX_RESOLUTION_RATIO, MAX_RESOLUTION_RATIO);
        break;
    case 2:
        printf("\t 2) gdr: H265-%s@30fps + H264-%s@30fps.\n", MAX_RESOLUTION_RATIO, MAX_RESOLUTION_RATIO);
        break;
    case 3:
        printf("\t 3) roibgframerate: H265-%s@30fps + H264-%s@30fps.\n", MAX_RESOLUTION_RATIO, MAX_RESOLUTION_RATIO);
        break;
    case 4:
        printf("\t 4) debreath: H265-%s@30fps + H264-%s@30fps.\n", MAX_RESOLUTION_RATIO, MAX_RESOLUTION_RATIO);
        break;
    case 5:
        printf("\t 5) svct: H265-%s@30fps + H264-%s@30fps.\n", MAX_RESOLUTION_RATIO, MAX_RESOLUTION_RATIO);
        break;
    case 6:
        printf("\t 6) qpmap: H265-%s@30fps + H264-%s@30fps.\n", MAX_RESOLUTION_RATIO, MAX_RESOLUTION_RATIO);
        break;
    case 7:
        printf("\t 7) mjpeg: mjpeg-%s@30fps.\n", MAX_RESOLUTION_RATIO);
        break;
    case 8:
        printf("\t 8) jpeg: jpeg mpf.\n");
        break;
    case 9:
        printf("\t 9) vpp lowlatency: 1 chn lowlatency(H264|H265-%s@30fps) + 0-2 normal chn(H264|H265 720p@30fps).\n", MAX_RESOLUTION_RATIO);
        break;
    case 10:
        printf("\t 10) videostrm: H265-%s@30fps +  videostrm.\n", MAX_RESOLUTION_RATIO);
        break;
    default:
        break;
    }
}

static vs_void_t sample_venc_main_usage(char* prog_name)
{
    vs_int32_t i;
    printf("Usage : %s <index>\n", prog_name);
    printf("index:\n");
    for (i = 0; i < 11; i++)
    {
        sample_venc_main_index_usage(i);
    }
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

static vs_void_t sample_venc_isp_i2c_bus_id_usage()
{
    printf("bus_id:(default is %d)\n", g_bus_id[0]);
    printf("\t xxx) isp i2c bus_id is a inter, range[-1-0xffffffff].\n");
}

static vs_void_t sample_venc_lowlatency_udp_usage()
{
    printf("acquire_strm_type:(processing method of encoding code stream)\n");
    printf("\t 0) save as file.\n");
    printf("\t 1) packaging through udp.\n");
    printf("udp_host:\n");
    printf("\t ip address of udp server.\n");
    printf("udp_port:\n");
    printf("\t port of udp server.\n");
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
    if (strstr(argv[argc - 1], "-h") || argc != 4)
    {
        printf("Usage : %s <index> <sensor_type> <i2c_bus_id> \n", argv[0]);
        printf("index:\n");
        sample_venc_main_index_usage(atoi(argv[1]));
        ;
        sample_venc_sensor_type_usage();
        sample_venc_isp_i2c_bus_id_usage();
        printf("Example:\n");
        printf("\te.g : %s %s %d %d\n", argv[0], argv[1], g_sensor_type[0], g_bus_id[0]);
        return VS_FAILED;
    }
    else
    {
        if (argc > 3)
        {
            g_sensor_type[0] = atoi(argv[2]);
            g_bus_id[0] = atoi(argv[3]);
        }
    }
    return VS_SUCCESS;
}

static vs_int32_t sample_venc_lowlatency_param_parse(vs_int32_t argc, char* argv[], lowlatency_aquire_strm_type_e* p_aquire_strm_type, char** p_udp_host, vs_int32_t* p_udp_port)
{
    if (strstr(argv[argc - 1], "-h") || argc < 4)
    {
        printf("Usage : %s <index> <sensor_type> <mode> <i2c_bus_id> [acquire_strm_type] [udp_host] [udp_port]\n", argv[0]);
        printf("index:\n");
        sample_venc_main_index_usage(atoi(argv[1]));
        sample_venc_sensor_type_usage();
        sample_venc_isp_i2c_bus_id_usage();
        sample_venc_lowlatency_udp_usage();
        printf("Example:\n");
        printf("\te.g : %s %s %d %d \n", argv[0], argv[1], g_sensor_type[0], g_bus_id[0]);
        return VS_FAILED;
    }
    else
    {
        g_sensor_type[0] = atoi(argv[2]);
        g_bus_id[0] = atoi(argv[3]);
        if (argc > 4)
        {
            *p_aquire_strm_type = atoi(argv[4]);
            if (*p_aquire_strm_type == LOWLATENCY_AQUIRE_STRM_TYPE_UDP)
            {
                if (argc > 5)
                {
                    *p_udp_host = argv[5];
                }
                else
                {
                    printf("udp host addr cannot be empty, please input udp host addr. \n");
                    return VS_FAILED;
                }
                if (argc > 6)
                {
                    *p_udp_port = atoi(argv[6]);
                }
                else
                {
                    printf("udp port cannot be empty, please input udp port. \n");
                    return VS_FAILED;
                }
                g_vii_vpp_mode = E_VII_ONLINE_VPP_ONLINE;
            }
        }
    }
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
            //p_vii_cfg->route_cfg[0].pipe_cfg[i].pipe_attr.compress_mode = E_COMPRESS_MODE_NONE;
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

static vs_void_t sample_venc_stream_threadparam_get(sample_venc_acquire_stream_param_s* p_acquire_stream_param, vs_int32_t* venc_chnid, vs_int32_t chn_num, vs_bool_t store_strm)
{
    vs_int32_t i = VS_SUCCESS;

    memset(p_acquire_stream_param, 0, sizeof(sample_venc_acquire_stream_param_s));
    p_acquire_stream_param->stop_stream_task = VS_FALSE;
    p_acquire_stream_param->chn_num = chn_num;
    p_acquire_stream_param->store_strm = store_strm;
    for (i = 0; i < chn_num; i++)
    {
        p_acquire_stream_param->venc_chnid[i] = venc_chnid[i];
    }
}

vs_int32_t sample_venc_typical(vs_int32_t argc, char* argv[])
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
#ifdef VS_ORION
    vs_int32_t venc_chnnum = 1;
#else
    vs_int32_t venc_chnnum = 2;
#endif
    vs_int32_t venc_chnid[2] = {0, 1};
    vs_payload_type_e encode_type[2] = {E_PT_TYPE_H265, E_PT_TYPE_H264};
    vs_venc_profile_e profile[2] = {E_VENC_PROFILE_H265_MAIN, E_VENC_PROFILE_H264_MAIN};
    sample_brc_mode_e brc_mode = E_VENC_BRC_CBR;
    vs_venc_gop_mode_e gop_mode = E_VENC_GOP_MODE_NORMP;
    vs_venc_gop_attr_s gop_attr;
    vs_bool_t store_strm = VS_TRUE;
    sample_venc_acquire_stream_param_s acquire_stream_param;

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
#ifndef VS_ORION
    ret = sample_venc_chn_init(venc_chnid[1], encode_type[1], profile[1], vpp_output_size[0], brc_mode, &gop_attr);
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("sample_venc_chn_init failed, ret[0x%x]\n", ret);
        goto exit_vpp_unbind_venc0;
    }

    ret = sample_common_vpp_bind_venc(vpp_grpid, vpp_chnid, venc_devid, venc_chnid[1]);
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("sample_common_vpp_bind_venc failed, ret[0x%x]\n", ret);
        goto exit_venc1_stop;
    }
#endif
    sample_venc_stream_threadparam_get(&acquire_stream_param, venc_chnid, venc_chnnum, store_strm);
    ret = sample_common_venc_acquire_stream_start(&acquire_stream_param);
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("sample_common_venc_acquire_stream_start failed, ret[%d]\n", ret);
        goto exit_vpp_unbind_venc1;
    }

    sample_venc_pause();

    vs_sample_trace("exit_venc_acquire_stream_stop \n");
    sample_common_venc_acquire_stream_stop(&acquire_stream_param);
exit_vpp_unbind_venc1:
#ifndef VS_ORION
    vs_sample_trace("exit_vpp_unbind_venc1\n");
    sample_common_vpp_unbind_venc(vpp_grpid, vpp_chnid, venc_devid, venc_chnid[1]);
exit_venc1_stop:
    vs_sample_trace("exit_venc1_stop\n");
    sample_common_venc_stop(venc_chnid[1]);
exit_vpp_unbind_venc0:
#endif
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

vs_int32_t sample_venc_gdr(vs_int32_t argc, char* argv[])
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
#ifdef VS_ORION
    vs_int32_t venc_chnnum = 1;
#else
    vs_int32_t venc_chnnum = 2;
#endif
    vs_int32_t venc_chnid[2] = {0, 1};
    vs_payload_type_e encode_type[2] = {E_PT_TYPE_H265, E_PT_TYPE_H264};
    vs_venc_profile_e profile[2] = {E_VENC_PROFILE_H265_MAIN, E_VENC_PROFILE_H264_MAIN};
    sample_brc_mode_e brc_mode = E_VENC_BRC_CBR;
    vs_venc_gop_mode_e gop_mode = E_VENC_GOP_MODE_NORMP;
    vs_venc_gop_attr_s gop_attr;
    vs_venc_gdr_s gdr_info;
    vs_bool_t store_strm = VS_TRUE;
    sample_venc_acquire_stream_param_s acquire_stream_param;

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

    /*vii and vpp init*/
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

    /*venc h265 chn set*/
    ret = sample_venc_chn_init(venc_chnid[0], encode_type[0], profile[0], vpp_output_size[0], brc_mode, &gop_attr);
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("sample_venc_chn_init failed, ret[0x%x]\n", ret);
        goto exit_vii_unbind_vpp;
    }

    gdr_info.enable = VS_TRUE;
    gdr_info.mode = E_VENC_GDR_ROW;
    gdr_info.refresh_num = 2;
    gdr_info.req_qp_i = 30;
    ret = vs_mal_venc_gdr_set(venc_chnid[0], &gdr_info);
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("vs_mal_venc_gdr_set failed, ret[0x%x]\n", ret);
        goto exit_venc0_stop;
    }

    ret = sample_common_vpp_bind_venc(vpp_grpid, vpp_chnid, venc_devid, venc_chnid[0]);
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("sample_common_vpp_bind_venc failed, ret[0x%x]\n", ret);
        goto exit_venc0_stop;
    }
#ifndef VS_ORION
    /*venc h264 chn set*/
    ret = sample_venc_chn_init(venc_chnid[1], encode_type[1], profile[1], vpp_output_size[0], brc_mode, &gop_attr);
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("sample_venc_chn_init failed, ret[0x%x]\n", ret);
        goto exit_vpp_unbind_venc0;
    }

    gdr_info.enable = VS_TRUE;
    gdr_info.mode = E_VENC_GDR_ROW;
    gdr_info.refresh_num = 8;
    gdr_info.req_qp_i = 28;
    ret = vs_mal_venc_gdr_set(venc_chnid[1], &gdr_info);
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("vs_mal_venc_gdr_set failed, ret[0x%x]\n", ret);
        goto exit_venc1_stop;
    }

    ret = sample_common_vpp_bind_venc(vpp_grpid, vpp_chnid, venc_devid, venc_chnid[1]);
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("sample_common_vpp_bind_venc failed, ret[0x%x]\n", ret);
        goto exit_venc1_stop;
    }
#endif
    /*stream get thread*/
    sample_venc_stream_threadparam_get(&acquire_stream_param, venc_chnid, venc_chnnum, store_strm);
    ret = sample_common_venc_acquire_stream_start(&acquire_stream_param);
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("sample_common_venc_acquire_stream_start failed, ret[%d]\n", ret);
        goto exit_vpp_unbind_venc1;
    }

    sample_venc_pause();

    vs_sample_trace("exit_venc_acquire_stream_stop \n");
    sample_common_venc_acquire_stream_stop(&acquire_stream_param);
exit_vpp_unbind_venc1:
#ifndef VS_ORION
    vs_sample_trace("exit_vpp_unbind_venc1\n");
    sample_common_vpp_unbind_venc(vpp_grpid, vpp_chnid, venc_devid, venc_chnid[1]);
exit_venc1_stop:
    vs_sample_trace("exit_venc1_stop\n");
    sample_common_venc_stop(venc_chnid[1]);
exit_vpp_unbind_venc0:
#endif
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

vs_int32_t sample_venc_roi_bg(vs_int32_t argc, char* argv[])
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
#ifdef VS_ORION
    vs_int32_t venc_chnnum = 1;
#else
    vs_int32_t venc_chnnum = 2;
#endif
    vs_int32_t venc_chnid[2] = {0, 1};
    vs_payload_type_e encode_type[2] = {E_PT_TYPE_H265, E_PT_TYPE_H264};
    vs_venc_profile_e profile[2] = {E_VENC_PROFILE_H265_MAIN, E_VENC_PROFILE_H264_MAIN};
    sample_brc_mode_e brc_mode = E_VENC_BRC_CBR;
    vs_venc_gop_mode_e gop_mode = E_VENC_GOP_MODE_NORMP;
    vs_venc_gop_attr_s gop_attr;
    vs_venc_roi_attr_s roi_attr;
    vs_venc_roi_bg_framerate_s roi_bg_framerate;
    vs_bool_t store_strm = VS_TRUE;
    sample_venc_acquire_stream_param_s acquire_stream_param;

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

    /*vii and vpp init*/
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

    /*venc h265 chn set*/
    ret = sample_venc_chn_init(venc_chnid[0], encode_type[0], profile[0], vpp_output_size[0], brc_mode, &gop_attr);
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("sample_venc_chn_init failed, ret[0x%x]\n", ret);
        goto exit_vii_unbind_vpp;
    }

    memset(&roi_attr, 0, sizeof(vs_venc_roi_attr_s));
    roi_attr.index = 0;
    roi_attr.enable[0] = VS_TRUE;
    roi_attr.enable[1] = VS_TRUE;
    roi_attr.enable[2] = VS_TRUE;
    roi_attr.is_absqp[0] = VS_TRUE;
    roi_attr.is_absqp[1] = VS_TRUE;
    roi_attr.is_absqp[2] = VS_TRUE;
    roi_attr.qp[0] = 28;
    roi_attr.qp[1] = 30;
    roi_attr.qp[2] = 32;
    roi_attr.rect[0].x = 0;
    roi_attr.rect[0].y = 0;
    roi_attr.rect[0].width = 64;
    roi_attr.rect[0].height = 64;
    roi_attr.rect[1].x = 64;
    roi_attr.rect[1].y = 64;
    roi_attr.rect[1].width = 64;
    roi_attr.rect[1].height = 64;
    roi_attr.rect[2].x = 128;
    roi_attr.rect[2].y = 128;
    roi_attr.rect[2].width = 64;
    roi_attr.rect[2].height = 64;
    ret = vs_mal_venc_roi_attr_set(venc_chnid[0], &roi_attr);
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("vs_mal_venc_roi_attr_set failed, ret[0x%x]\n", ret);
        goto exit_venc0_stop;
    }

    roi_bg_framerate.src_framerate = 30;
    roi_bg_framerate.dst_framerate = 15;
    ret = vs_mal_venc_roi_bg_framerate_set(venc_chnid[0], &roi_bg_framerate);
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("vs_mal_venc_roi_bg_framerate_set failed, ret[0x%x]\n", ret);
        goto exit_venc0_stop;
    }

    ret = sample_common_vpp_bind_venc(vpp_grpid, vpp_chnid, venc_devid, venc_chnid[0]);
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("sample_common_vpp_bind_venc failed, ret[0x%x]\n", ret);
        goto exit_venc0_stop;
    }
#ifndef VS_ORION
    /*venc h264 chn set*/
    ret = sample_venc_chn_init(venc_chnid[1], encode_type[1], profile[1], vpp_output_size[0], brc_mode, &gop_attr);
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("sample_venc_chn_init failed, ret[0x%x]\n", ret);
        goto exit_vpp_unbind_venc0;
    }

    memset(&roi_attr, 0, sizeof(vs_venc_roi_attr_s));
    roi_attr.index = 0;
    roi_attr.enable[0] = VS_TRUE;
    roi_attr.enable[1] = VS_TRUE;
    roi_attr.enable[2] = VS_TRUE;
    roi_attr.is_absqp[0] = VS_TRUE;
    roi_attr.is_absqp[1] = VS_TRUE;
    roi_attr.is_absqp[2] = VS_TRUE;
    roi_attr.qp[0] = 28;
    roi_attr.qp[1] = 30;
    roi_attr.qp[2] = 32;
    roi_attr.rect[0].x = 0;
    roi_attr.rect[0].y = 0;
    roi_attr.rect[0].width = 32;
    roi_attr.rect[0].height = 32;
    roi_attr.rect[1].x = 32;
    roi_attr.rect[1].y = 32;
    roi_attr.rect[1].width = 32;
    roi_attr.rect[1].height = 32;
    roi_attr.rect[2].x = 64;
    roi_attr.rect[2].y = 64;
    roi_attr.rect[2].width = 32;
    roi_attr.rect[2].height = 32;
    ret = vs_mal_venc_roi_attr_set(venc_chnid[1], &roi_attr);
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("vs_mal_venc_roi_attr_set failed, ret[0x%x]\n", ret);
        goto exit_venc1_stop;
    }

    roi_bg_framerate.src_framerate = 30;
    roi_bg_framerate.dst_framerate = 15;
    ret = vs_mal_venc_roi_bg_framerate_set(venc_chnid[1], &roi_bg_framerate);
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("vs_mal_venc_roi_bg_framerate_set failed, ret[0x%x]\n", ret);
        goto exit_venc1_stop;
    }

    ret = sample_common_vpp_bind_venc(vpp_grpid, vpp_chnid, venc_devid, venc_chnid[1]);
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("sample_common_vpp_bind_venc failed, ret[0x%x]\n", ret);
        goto exit_venc1_stop;
    }
#endif
    /*stream get thread*/
    sample_venc_stream_threadparam_get(&acquire_stream_param, venc_chnid, venc_chnnum, store_strm);
    ret = sample_common_venc_acquire_stream_start(&acquire_stream_param);
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("sample_common_venc_acquire_stream_start failed, ret[%d]\n", ret);
        goto exit_vpp_unbind_venc1;
    }

    sample_venc_pause();

    vs_sample_trace("exit_venc_acquire_stream_stop \n");
    sample_common_venc_acquire_stream_stop(&acquire_stream_param);
exit_vpp_unbind_venc1:
#ifndef VS_ORION
    vs_sample_trace("exit_vpp_unbind_venc1\n");
    sample_common_vpp_unbind_venc(vpp_grpid, vpp_chnid, venc_devid, venc_chnid[1]);
exit_venc1_stop:
    vs_sample_trace("exit_venc1_stop\n");
    sample_common_venc_stop(venc_chnid[1]);
exit_vpp_unbind_venc0:
#endif
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

vs_int32_t sample_venc_debreath(vs_int32_t argc, char* argv[])
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
#ifdef VS_ORION
    vs_int32_t venc_chnnum = 1;
#else
    vs_int32_t venc_chnnum = 2;
#endif
    vs_int32_t venc_chnid[2] = {0, 1};
    vs_payload_type_e encode_type[2] = {E_PT_TYPE_H265, E_PT_TYPE_H264};
    vs_venc_profile_e profile[2] = {E_VENC_PROFILE_H265_MAIN, E_VENC_PROFILE_H264_MAIN};
    sample_brc_mode_e brc_mode = E_VENC_BRC_CBR;
    vs_venc_gop_mode_e gop_mode = E_VENC_GOP_MODE_NORMP;
    vs_venc_gop_attr_s gop_attr;
    vs_venc_debreath_s debreath_info;
    vs_bool_t store_strm = VS_TRUE;
    vs_char_t name[100] = "/lib/firmware/vs_dsp0.bin";
    g_comm_vb_cnt = 15;

    sample_venc_acquire_stream_param_s acquire_stream_param;

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

    ret = sample_common_dsp_init(0, name);
    vs_sample_trace("sample_common_dsp_init ret[0x%x]\n", ret);
    if (ret != VS_SUCCESS)
    {
        return ret;
    }

    /*vii and vpp init*/
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

    /*venc h265 chn set*/
    ret = sample_venc_chn_init(venc_chnid[0], encode_type[0], profile[0], vpp_output_size[0], brc_mode, &gop_attr);
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("sample_venc_chn_init failed, ret[0x%x]\n", ret);
        goto exit_vii_unbind_vpp;
    }

    ret = vs_mal_venc_debreath_get(venc_chnid[0], &debreath_info);
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("vs_mal_venc_debreath_get failed, ret[0x%x]\n", ret);
        goto exit_venc0_stop;
    }
    debreath_info.enable = VS_TRUE;
    ret = vs_mal_venc_debreath_set(venc_chnid[0], &debreath_info);
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("vs_mal_venc_debreath_set failed, ret[0x%x]\n", ret);
        goto exit_venc0_stop;
    }

    ret = sample_common_vpp_bind_venc(vpp_grpid, vpp_chnid, venc_devid, venc_chnid[0]);
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("sample_common_vpp_bind_venc failed, ret[0x%x]\n", ret);
        goto exit_venc0_stop;
    }
#ifndef VS_ORION
    /*venc h264 chn set*/
    ret = sample_venc_chn_init(venc_chnid[1], encode_type[1], profile[1], vpp_output_size[0], brc_mode, &gop_attr);
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("sample_venc_chn_init failed, ret[0x%x]\n", ret);
        goto exit_vpp_unbind_venc0;
    }

    ret = vs_mal_venc_debreath_get(venc_chnid[1], &debreath_info);
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("vs_mal_venc_debreath_get failed, ret[0x%x]\n", ret);
        goto exit_venc1_stop;
    }
    debreath_info.enable = VS_TRUE;
    ret = vs_mal_venc_debreath_set(venc_chnid[1], &debreath_info);
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("vs_mal_venc_debreath_set failed, ret[0x%x]\n", ret);
        goto exit_venc1_stop;
    }

    ret = sample_common_vpp_bind_venc(vpp_grpid, vpp_chnid, venc_devid, venc_chnid[1]);
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("sample_common_vpp_bind_venc failed, ret[0x%x]\n", ret);
        goto exit_venc1_stop;
    }
#endif
    /*stream get thread*/
    sample_venc_stream_threadparam_get(&acquire_stream_param, venc_chnid, venc_chnnum, store_strm);
    ret = sample_common_venc_acquire_stream_start(&acquire_stream_param);
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("sample_common_venc_acquire_stream_start failed, ret[%d]\n", ret);
        goto exit_vpp_unbind_venc1;
    }

    sample_venc_pause();

    vs_sample_trace("exit_venc_acquire_stream_stop \n");
    sample_common_venc_acquire_stream_stop(&acquire_stream_param);
exit_vpp_unbind_venc1:
#ifndef VS_ORION
    vs_sample_trace("exit_vpp_unbind_venc1\n");
    sample_common_vpp_unbind_venc(vpp_grpid, vpp_chnid, venc_devid, venc_chnid[1]);
exit_venc1_stop:
    vs_sample_trace("exit_venc1_stop\n");
    sample_common_venc_stop(venc_chnid[1]);
exit_vpp_unbind_venc0:
#endif
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
    sample_common_dsp_exit(0);
    sample_common_sys_exit();
    return ret;
}

vs_int32_t sample_venc_svct(vs_int32_t argc, char* argv[])
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
#ifdef VS_ORION
    vs_int32_t venc_chnnum = 1;
#else
    vs_int32_t venc_chnnum = 2;
#endif
    vs_int32_t venc_chnid[2] = {0, 1};
    vs_payload_type_e encode_type[2] = {E_PT_TYPE_H265, E_PT_TYPE_H264};
    vs_venc_profile_e profile[2] = {E_VENC_PROFILE_H265_MAIN, E_VENC_PROFILE_H264_MAIN};
    sample_brc_mode_e brc_mode = E_VENC_BRC_CBR;
    vs_venc_gop_mode_e gop_mode = E_VENC_GOP_MODE_NORMP;
    vs_venc_gop_attr_s gop_attr;
    vs_venc_svct_s svct_info;
    vs_bool_t store_strm = VS_TRUE;
    sample_venc_acquire_stream_param_s acquire_stream_param;

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

    /*vii and vpp init*/
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

    /*venc h265 chn set*/
    ret = sample_venc_chn_init(venc_chnid[0], encode_type[0], profile[0], vpp_output_size[0], brc_mode, &gop_attr);
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("sample_venc_chn_init failed, ret[0x%x]\n", ret);
        goto exit_vii_unbind_vpp;
    }
    memset(&svct_info, 0, sizeof(vs_venc_svct_s));
    svct_info.enable = VS_TRUE;
    svct_info.temporal_frame_num[0] = 2;
    svct_info.temporal_frame_num[1] = 3;
    ret = vs_mal_venc_svct_set(venc_chnid[0], &svct_info);
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("vs_mal_venc_svct_set failed, ret[0x%x]\n", ret);
        goto exit_venc0_stop;
    }

    ret = sample_common_vpp_bind_venc(vpp_grpid, vpp_chnid, venc_devid, venc_chnid[0]);
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("sample_common_vpp_bind_venc failed, ret[0x%x]\n", ret);
        goto exit_venc0_stop;
    }
#ifndef VS_ORION
    /*venc h264 chn set*/
    ret = sample_venc_chn_init(venc_chnid[1], encode_type[1], profile[1], vpp_output_size[0], brc_mode, &gop_attr);
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("sample_venc_chn_init failed, ret[0x%x]\n", ret);
        goto exit_vpp_unbind_venc0;
    }
    memset(&svct_info, 0, sizeof(vs_venc_svct_s));
    svct_info.enable = VS_TRUE;
    svct_info.temporal_frame_num[0] = 2;
    svct_info.temporal_frame_num[1] = 3;
    ret = vs_mal_venc_svct_set(venc_chnid[1], &svct_info);
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("vs_mal_venc_svct_set failed, ret[0x%x]\n", ret);
        goto exit_venc1_stop;
    }

    ret = sample_common_vpp_bind_venc(vpp_grpid, vpp_chnid, venc_devid, venc_chnid[1]);
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("sample_common_vpp_bind_venc failed, ret[0x%x]\n", ret);
        goto exit_venc1_stop;
    }
#endif
    /*stream get thread*/
    sample_venc_stream_threadparam_get(&acquire_stream_param, venc_chnid, venc_chnnum, store_strm);
    ret = sample_common_venc_acquire_stream_start(&acquire_stream_param);
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("sample_common_venc_acquire_stream_start failed, ret[%d]\n", ret);
        goto exit_vpp_unbind_venc1;
    }

    sample_venc_pause();

    vs_sample_trace("exit_venc_acquire_stream_stop \n");
    sample_common_venc_acquire_stream_stop(&acquire_stream_param);
exit_vpp_unbind_venc1:
#ifndef VS_ORION
    vs_sample_trace("exit_vpp_unbind_venc1\n");
    sample_common_vpp_unbind_venc(vpp_grpid, vpp_chnid, venc_devid, venc_chnid[1]);
exit_venc1_stop:
    vs_sample_trace("exit_venc1_stop\n");
    sample_common_venc_stop(venc_chnid[1]);
exit_vpp_unbind_venc0:
#endif
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

vs_int32_t sample_venc_qpmap(vs_int32_t argc, char* argv[])
{
    vs_int32_t ret = VS_SUCCESS, i = 0;
    vs_int32_t sensor_id = 0;
    vs_size_s sensor_size;
    vs_int32_t vii_pipeid = 0;
    vs_int32_t vii_chnid = 0;
    sample_vii_cfg_s vii_cfg;
    vs_int32_t vpp_grpid = 0;
    vs_int32_t vpp_chnid = 0;
    vs_size_s vpp_output_size[VPP_MAX_PHYCHN_NUM] = {0};
    vs_bool_t vpp_chn_enable[VPP_MAX_PHYCHN_NUM] = {VS_TRUE, VS_FALSE, VS_FALSE, VS_FALSE};
#ifdef VS_ORION
    vs_int32_t venc_chnnum = 1;
#else
    vs_int32_t venc_chnnum = 2;
#endif
    vs_int32_t venc_chnid[2] = {0, 1};
    vs_payload_type_e encode_type[2] = {E_PT_TYPE_H265, E_PT_TYPE_H264};
    vs_venc_profile_e profile[2] = {E_VENC_PROFILE_H265_MAIN, E_VENC_PROFILE_H264_MAIN};
    sample_brc_mode_e brc_mode = E_VENC_BRC_QPMAP;
    vs_venc_gop_mode_e gop_mode = E_VENC_GOP_MODE_NORMP;
    vs_venc_gop_attr_s gop_attr;
    sample_venc_send_qpmap_frame_param_s send_qpmap_frame_param;
    vs_bool_t store_strm = VS_TRUE;
    sample_venc_acquire_stream_param_s acquire_stream_param;

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
#ifndef VS_ORION
    ret = sample_venc_chn_init(venc_chnid[1], encode_type[1], profile[1], vpp_output_size[0], brc_mode, &gop_attr);
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("sample_venc_chn_init failed, ret[0x%x]\n", ret);
        goto exit_venc0_stop;
    }
#endif
    /*stream get thread*/
    sample_venc_stream_threadparam_get(&acquire_stream_param, venc_chnid, venc_chnnum, store_strm);
    ret = sample_common_venc_acquire_stream_start(&acquire_stream_param);
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("sample_common_venc_acquire_stream_start failed, ret[%d]\n", ret);
        goto exit_venc1_stop;
    }

    send_qpmap_frame_param.vpp_grpid = vpp_grpid;
    send_qpmap_frame_param.vpp_chnid = vpp_chnid;
    send_qpmap_frame_param.width = vpp_output_size[0].width;
    send_qpmap_frame_param.height = vpp_output_size[0].height;
    send_qpmap_frame_param.pixel_format = E_PIXEL_FORMAT_YVU_420SP;
    send_qpmap_frame_param.stop_send_task = VS_FALSE;
    send_qpmap_frame_param.private_data = VS_NULL;
    send_qpmap_frame_param.venc_chnnum = venc_chnnum;
    for (i = 0; i < venc_chnnum; i++)
    {
        send_qpmap_frame_param.venc_chnid[i] = venc_chnid[i];
        send_qpmap_frame_param.venc_type[i] = encode_type[i];
    }
    ret = sample_common_venc_send_qpmap_frame_start(&send_qpmap_frame_param);
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("sample_common_venc_send_qpmap_frame_start failed, ret[%d]\n", ret);
        goto exit_venc_acquire_stream_stop;
    }

    sample_venc_pause();

    vs_sample_trace("exit_venc_send_qpmap_frame_stop\n");
    sample_common_venc_send_qpmap_frame_stop(&send_qpmap_frame_param);
exit_venc_acquire_stream_stop:
    vs_sample_trace("exit_venc_acquire_stream_stop\n");
    sample_common_venc_acquire_stream_stop(&acquire_stream_param);
exit_venc1_stop:
#ifndef VS_ORION
    vs_sample_trace("exit_venc1_stop\n");
    sample_common_venc_stop(venc_chnid[1]);
exit_venc0_stop:
#endif
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

vs_int32_t sample_venc_mjpeg(vs_int32_t argc, char* argv[])
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
    vs_int32_t venc_chnnum = 1;
    vs_int32_t venc_chnid[2] = {0, 1};
    vs_payload_type_e encode_type[2] = {E_PT_TYPE_MJPEG, E_PT_TYPE_MJPEG};
    vs_venc_profile_e profile[2] = {E_VENC_PROFILE_JPEG_BASELINE, E_VENC_PROFILE_JPEG_BASELINE};
    sample_brc_mode_e brc_mode = E_VENC_BRC_CBR;
    vs_bool_t store_strm = VS_TRUE;
    sample_venc_acquire_stream_param_s acquire_stream_param;

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

    ret = sample_venc_chn_init(venc_chnid[0], encode_type[0], profile[0], vpp_output_size[0], brc_mode, VS_NULL);
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

    /*stream get thread*/
    sample_venc_stream_threadparam_get(&acquire_stream_param, venc_chnid, venc_chnnum, store_strm);
    ret = sample_common_venc_acquire_stream_start(&acquire_stream_param);
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("sample_common_venc_acquire_stream_start failed, ret[%d]\n", ret);
        goto exit_vpp_unbind_venc0;
    }

    sample_venc_pause();

    vs_sample_trace("exit_venc_acquire_stream_stop \n");
    sample_common_venc_acquire_stream_stop(&acquire_stream_param);
exit_vpp_unbind_venc0:
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

vs_int32_t sample_venc_jpeg_mpf(vs_int32_t argc, char* argv[])
{
    vs_int32_t ret = VS_SUCCESS;
    vs_int32_t sensor_id = 0;
    vs_size_s sensor_size;
    vs_int32_t vii_pipeid = 0;
    vs_int32_t vii_chnid = 0;
    sample_vii_cfg_s vii_cfg;
    vs_int32_t vpp_grpid = 0;
    vs_int32_t vpp_chnid[VPP_MAX_PHYCHN_NUM] = {0, 1, 2, 3};
    vs_size_s vpp_output_size[VPP_MAX_PHYCHN_NUM] = {0};
    vs_bool_t vpp_chn_enable[VPP_MAX_PHYCHN_NUM] = {VS_TRUE, VS_FALSE, VS_FALSE, VS_FALSE};
    vs_int32_t venc_devid[2] = {0, 1};
    vs_int32_t venc_chnnum = 1;
    vs_int32_t venc_chnid[2] = {0, 1};
    vs_payload_type_e encode_type[2] = {E_PT_TYPE_JPEG, E_PT_TYPE_JPEG};
    vs_venc_profile_e profile[2] = {E_VENC_PROFILE_JPEG_BASELINE, E_VENC_PROFILE_JPEG_BASELINE};
    sample_brc_mode_e brc_mode = E_VENC_BRC_CBR;
    vs_bool_t store_strm = VS_TRUE;
    sample_venc_acquire_stream_param_s acquire_stream_param;
    /*NOTE:thumbnail size depends on the zoom ability of vpp channel and constraints of venc.*/
    vs_size_s thumbnail_size = {256, 256};
    vs_venc_jpeg_attr_s jpeg_attr;
    sample_venc_cfg_s sample_venc_cfg;
    vs_venc_pic_rcv_mode_e rcv_mode = E_VENC_PIC_RECEIVE_MODE_SINGLE;
    vs_int32_t rcvframe_num = 1;

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
    if (rcv_mode == E_VENC_PIC_RECEIVE_MODE_MULTI)
    {
        vpp_chn_enable[1] = VS_TRUE;
        vpp_output_size[1] = thumbnail_size;
    }
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

    memset(&jpeg_attr, 0, sizeof(vs_venc_jpeg_attr_s));
    jpeg_attr.input_type = SAMPLE_VENC_INPUT_FORMAT;
    jpeg_attr.rcv_mode = rcv_mode;
    jpeg_attr.mpf_cfg.thumbnail_num = 1;
    jpeg_attr.mpf_cfg.thumbnail_size[0] = thumbnail_size;
    memset(&sample_venc_cfg, 0, sizeof(sample_venc_cfg_s));
    sample_venc_cfg.format = SAMPLE_VENC_INPUT_FORMAT;
    sample_venc_cfg.compress = (g_compress_mode == E_COMPRESS_MODE_NONE) ? VS_FALSE : VS_TRUE;
    sample_venc_cfg.type = encode_type[0];
    sample_venc_cfg.profile = profile[0];
    sample_venc_cfg.frame_size = vpp_output_size[0];
    sample_venc_cfg.brc_mode = brc_mode;
    sample_venc_cfg.frc.dst_framerate = g_sensor_framerate;
    sample_venc_cfg.frc.src_framerate = g_sensor_framerate;
    sample_venc_cfg.bandwidth_save_strength = 0;
    ret = sample_common_venc_mpf_start(venc_chnid[0], &sample_venc_cfg, &jpeg_attr);
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("sample_common_venc_start failed, ret[0x%x]\n", ret);
        goto exit_vii_unbind_vpp;
    }

    ret = sample_common_vpp_bind_venc(vpp_grpid, vpp_chnid[0], venc_devid[0], venc_chnid[0]);
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("sample_common_vpp_bind_venc failed, ret[0x%x]\n", ret);
        goto exit_venc_stop;
    }

    if (rcv_mode == E_VENC_PIC_RECEIVE_MODE_MULTI)
    {
        ret = sample_common_vpp_bind_venc(vpp_grpid, vpp_chnid[1], venc_devid[1], venc_chnid[0]);
        if (ret != VS_SUCCESS)
        {
            vs_sample_trace("sample_common_vpp_bind_venc failed chn1, ret[0x%x]\n", ret);
            goto exit_vpp_unbind_venc;
        }
    }

    sample_venc_stream_threadparam_get(&acquire_stream_param, venc_chnid, venc_chnnum, store_strm);
    while (!g_stop_flag)
    {
        vs_sample_trace("\n=====Press enter to get next jpeg(mpf), Press 'e' to exit=====\n");
        vs_char_t input_key;
        input_key = getchar();
        if (input_key == '\n')
        {
            ret = sample_common_venc_acquire_jpeg_mpf_process(&acquire_stream_param, rcvframe_num);
            if (ret != VS_SUCCESS)
            {
                vs_sample_trace("sample_common_venc_acquire_jpeg_mpf_process failed, ret[0x%x]\n", ret);
                break;
            }
        }
        else if (input_key == 'e')
        {
            break;
        }
        usleep(10000);
    };

    printf("\n=====exit sample_venc_jpeg_mpf=====\n");
exit_vpp_unbind_venc:
    vs_sample_trace("exit_vpp_unbind_venc\n");
    sample_common_vpp_unbind_venc(vpp_grpid, vpp_chnid[0], venc_devid[0], venc_chnid[0]);
    if (rcv_mode == E_VENC_PIC_RECEIVE_MODE_MULTI)
    {
        vs_sample_trace("exit_vpp_unbind_venc\n");
        sample_common_vpp_unbind_venc(vpp_grpid, vpp_chnid[1], venc_devid[1], venc_chnid[0]);
    }
exit_venc_stop:
    vs_sample_trace("exit_venc_stop\n");
    sample_common_venc_mpf_stop(venc_chnid[0]);
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

static vs_int32_t sample_venc_udp_socket_create(char* p_udp_host, vs_int32_t udp_port, lowlatency_udp_info_s* p_udp_info)
{
    p_udp_info->socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (p_udp_info->socket_fd < 0)
    {
        vs_sample_trace("socket_fd create failed for %s:%d\n", p_udp_host, udp_port);
        return VS_FAILED;
    }

    bzero(&p_udp_info->socket_addr, sizeof(struct sockaddr_in));
    p_udp_info->socket_addr.sin_family = AF_INET;
    inet_pton(AF_INET, p_udp_host, &p_udp_info->socket_addr.sin_addr.s_addr);
    p_udp_info->socket_addr.sin_port = htons(udp_port);

    return VS_SUCCESS;
}

static vs_void_t sample_venc_udp_socket_close(lowlatency_udp_info_s* p_udp_info)
{
    close(p_udp_info->socket_fd);
}

static vs_void_t sample_venc_udp_send(vs_int32_t socket_fd, struct sockaddr_in* socket_addr, vs_uint8_t* p_virt_addr, vs_uint32_t len)
{
    vs_uint32_t send_len = len;
    vs_uint32_t send_offset = 0;
    vs_uint32_t send_times = 0;

    while (send_len > SAMPLE_MAX_UDP_SEND_LEN)
    {
        send_times++;
        send_len -= SAMPLE_MAX_UDP_SEND_LEN;
    }

    send_times++;
    send_len = len;

    *((vs_uint32_t*)p_virt_addr) = send_times | (SAMPLE_UDP_MAGIC_CODE << 8);

    while (send_len > SAMPLE_MAX_UDP_SEND_LEN)
    {
        sendto(socket_fd, p_virt_addr + send_offset, SAMPLE_MAX_UDP_SEND_LEN, 0, (struct sockaddr*)socket_addr, sizeof(struct sockaddr_in));
        send_len -= SAMPLE_MAX_UDP_SEND_LEN;
        send_offset += SAMPLE_MAX_UDP_SEND_LEN;
    }

    sendto(socket_fd, p_virt_addr + send_offset, send_len, 0, (struct sockaddr*)socket_addr, sizeof(struct sockaddr_in));
}

static vs_void_t sample_venc_stream_callback(vs_int32_t chnid, vs_void_t* p_acquire_param, vs_venc_stream_s* p_out_stream)
{
    sample_venc_acquire_stream_param_s* p_stream_param = (sample_venc_acquire_stream_param_s*)p_acquire_param;
    lowlatency_udp_info_s* p_udp_info = (lowlatency_udp_info_s*)p_stream_param->private_data;
    vs_uint32_t i = 0, j = 0, wr_len = 0;

    for (i = 0; i < p_out_stream->pack_num; i++)
    {
        if (0 == p_out_stream->p_pack[i].length)
        {
            continue;
        }
        wr_len = 0;
        for (j = 0; j < p_out_stream->p_pack[i].data_num; j++)
        {
            sample_venc_udp_send(p_udp_info->socket_fd, &p_udp_info->socket_addr, p_out_stream->p_pack[i].virt_addr + p_out_stream->p_pack[i].pack_info[j].pack_offset,
                                 p_out_stream->p_pack[i].pack_info[j].pack_len);
            wr_len += p_out_stream->p_pack[i].pack_info[j].pack_len;
        }
        sample_venc_udp_send(p_udp_info->socket_fd, &p_udp_info->socket_addr, p_out_stream->p_pack[i].virt_addr + p_out_stream->p_pack[i].offset,
                             p_out_stream->p_pack[i].length - wr_len);
    }
}

vs_int32_t sample_venc_lowlatency_vpp(vs_int32_t argc, char* argv[])
{
    vs_int32_t ret = VS_SUCCESS;
    vs_int32_t sensor_id = 0;
    vs_size_s sensor_size;
    vs_int32_t vii_pipeid = 0;
    vs_int32_t vii_chnid = 0;
    sample_vii_cfg_s vii_cfg;
    vs_int32_t vpp_grpid = 0;
    vs_int32_t vpp_chnid[VPP_MAX_PHYCHN_NUM] = {0, 1, 2, 3};
    ;
    vs_size_s vpp_output_size[VPP_MAX_PHYCHN_NUM] = {{3840, 2160}, {1280, 720}, {1280, 720}, {352, 288}};
    vs_bool_t vpp_chn_enable[VPP_MAX_PHYCHN_NUM] = {VS_FALSE, VS_FALSE, VS_FALSE, VS_FALSE};
    vs_int32_t venc_chnnum = 1;
    vs_int32_t venc_devid = 0;
    vs_int32_t venc_chnid[4] = {0, 1, 2, 3};
    vs_payload_type_e encode_type[4] = {E_PT_TYPE_H265, E_PT_TYPE_H265, E_PT_TYPE_H265, E_PT_TYPE_H265};
    vs_venc_profile_e profile[4] = {E_VENC_PROFILE_H265_MAIN, E_VENC_PROFILE_H265_MAIN, E_VENC_PROFILE_H265_MAIN, E_VENC_PROFILE_H265_MAIN};
    sample_brc_mode_e brc_mode = E_VENC_BRC_CBR;
    vs_venc_gop_mode_e gop_mode = E_VENC_GOP_MODE_NORMP;
    vs_venc_gop_attr_s gop_attr;
    vs_lowlatency_attr_s lowlatency_attr;
    vs_int32_t i = 0;
    sample_venc_acquire_stream_param_s acquire_stream_param;
    vs_bool_t store_strm = VS_FALSE;
    lowlatency_aquire_strm_type_e acquire_strm_type = LOWLATENCY_AQUIRE_STRM_TYPE_FILE;
    char* udp_host = VS_NULL;
    vs_int32_t udp_port = 0;
    lowlatency_udp_info_s udp_info = {0};

    g_vii_vpp_mode = E_VII_ONLINE_VPP_ONLINE;
    g_sensor_type[0] = SONY_IMX334_MIPI_8M_30FPS_12BIT;

    ret = sample_venc_lowlatency_param_parse(argc, argv, &acquire_strm_type, &udp_host, &udp_port);
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
    for (i = 0; i < venc_chnnum; i++)
    {
        vpp_chn_enable[i] = VS_TRUE;
    }
    ret = sample_venc_vpp_init(vpp_grpid, sensor_size, vpp_output_size, vpp_chn_enable, VS_TRUE);
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("sample_common_vpp_start failed, ret[0x%x]\n", ret);
        goto exit_vii_stop;
    }

    ret = sample_common_venc_gop_attr_get(gop_mode, &gop_attr);
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("gop_attr_get failed, ret[0x%x]\n", ret);
        goto exit_vpp_stop;
    }

    for (i = 0; i < venc_chnnum; i++)
    {
        ret = sample_venc_chn_init(venc_chnid[i], encode_type[i], profile[i], vpp_output_size[i], brc_mode, &gop_attr);
        if (ret != VS_SUCCESS)
        {
            vs_sample_trace("sample_venc_chn_init failed, ret[0x%x]\n", ret);
            goto exit_venc_stop_unbind_vpp;
        }

        ret = sample_common_vpp_bind_venc(vpp_grpid, vpp_chnid[i], venc_devid, venc_chnid[i]);
        if (ret != VS_SUCCESS)
        {
            vs_sample_trace("sample_common_vpp_bind_venc failed, ret[0x%x]\n", ret);
            goto exit_venc_stop_unbind_vpp;
        }
    }

    lowlatency_attr.enable = VS_TRUE;
    lowlatency_attr.mode = E_LOWLATENCY_MODE_VENC_ONLY;
    if (encode_type[0] == E_PT_TYPE_H264)
    {
        lowlatency_attr.linebuf_mode = E_LOWLATENCY_LINEBUF_MODE_16_LINE;
    }
    else
    {
        lowlatency_attr.linebuf_mode = E_LOWLATENCY_LINEBUF_MODE_64_LINE;
    }
    ret = vs_mal_vpp_chn_lowlatency_set(vpp_grpid, vpp_chnid[0], &lowlatency_attr);
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("vs_mal_vpp_chn_lowlatency_set(grp %d chn %d) failed with 0x%x\n", vpp_grpid, vpp_chnid[0], ret);
        goto exit_venc_stop_unbind_vpp;
    }

    for (i = 0; i < VPP_MAX_PHYCHN_NUM; i++)
    {
        if (vpp_chn_enable[i] == VS_TRUE)
        {
            ret = vs_mal_vpp_chn_enable(vpp_grpid, vpp_chnid[i]);
            if (ret != VS_SUCCESS)
            {
                vs_sample_trace("vs_mal_vpp_chn_enable failed, ret[0x%x]\n", ret);
                goto exit_venc_stop_unbind_vpp;
            }
        }
    }

    ret = sample_common_vii_bind_vpp(vii_pipeid, vii_chnid, vpp_grpid);
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("sample_common_vii_bind_vpp failed, ret[0x%x]\n", ret);
        goto exit_vii_unbind_vpp;
    }
    if (acquire_strm_type == LOWLATENCY_AQUIRE_STRM_TYPE_FILE)
    {
        store_strm = VS_TRUE;
        sample_venc_stream_threadparam_get(&acquire_stream_param, venc_chnid, venc_chnnum, store_strm);
        ret = sample_common_venc_acquire_stream_start(&acquire_stream_param);
        if (ret != VS_SUCCESS)
        {
            vs_sample_trace("sample_common_venc_acquire_stream_start failed, ret[%d]\n", ret);
        }
        else
        {
            sample_venc_pause();
            vs_sample_trace("exit_venc_acquire_stream_stop\n");
            sample_common_venc_acquire_stream_stop(&acquire_stream_param);
        }
    }
    else
    {
        ret = sample_venc_udp_socket_create(udp_host, udp_port, &udp_info);
        if (ret != VS_SUCCESS)
        {
            vs_sample_trace("sample_venc_udp_socket_create failed, ret[%d]\n", ret);
            goto exit_vii_unbind_vpp;
        }

        sample_venc_stream_threadparam_get(&acquire_stream_param, venc_chnid, venc_chnnum, VS_FALSE);
        acquire_stream_param.stream_acquire_func = sample_venc_stream_callback;
        acquire_stream_param.private_data = &udp_info;
        ret = sample_common_venc_acquire_stream_start(&acquire_stream_param);
        if (ret != VS_SUCCESS)
        {
            vs_sample_trace("sample_common_venc_acquire_stream_start failed, ret[%d]\n", ret);
        }
        else
        {
            sample_venc_pause();
            vs_sample_trace("exit_venc_acquire_stream_stop\n");
            sample_common_venc_acquire_stream_stop(&acquire_stream_param);
        }
        sample_venc_udp_socket_close(&udp_info);
    }

exit_vii_unbind_vpp:
    vs_sample_trace("exit_vii_unbind_vpp\n");
    sample_common_vii_unbind_vpp(vii_pipeid, vii_chnid, vpp_grpid);
exit_venc_stop_unbind_vpp:
    for (i = 0; i < venc_chnnum; i++)
    {
        vs_sample_trace("exit_venc_stop_unbind_vpp, unbind_vpp.\n");
        sample_common_vpp_unbind_venc(vpp_grpid, vpp_chnid[i], venc_devid, venc_chnid[i]);
        vs_sample_trace("exit_venc_stop_unbind_vpp, venc_stop.\n");
        sample_common_venc_stop(venc_chnid[i]);
    }
exit_vpp_stop:
    vs_sample_trace("exit_vpp_stop.\n");
    sample_common_vpp_stop(vpp_grpid, vpp_chn_enable);
exit_vii_stop:
    vs_sample_trace("exit_vii_stop.\n");
    sample_common_vii_stop(&vii_cfg);
exit_sys_exit:
    vs_sample_trace("exit_sys_exit.\n");
    sample_common_sys_exit();
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
    vs_int32_t idx;

    if (argc < 2 || (strstr(argv[argc - 1], "-h") && argc == 2))
    {
        sample_venc_main_usage(argv[0]);
        return VS_FAILED;
    }

    sample_venc_register_signal_handler(venc_signal_handle);

    idx = atoi(argv[1]);
    switch (idx)
    {
    case 1:
        vs_sample_trace("sample_venc_typical.\n");
        sample_venc_typical(argc, argv);
        break;
    case 2:
        vs_sample_trace("sample_venc_gdr.\n");
        sample_venc_gdr(argc, argv);
        break;
    case 3:
        vs_sample_trace("sample_venc_roi_bg.\n");
        sample_venc_roi_bg(argc, argv);
        break;
    case 4:
        vs_sample_trace("sample_venc_debreath.\n");
        sample_venc_debreath(argc, argv);
        break;
    case 5:
        vs_sample_trace("sample_venc_svct.\n");
        sample_venc_svct(argc, argv);
        break;
    case 6:
        vs_sample_trace("sample_venc_qpmap.\n");
        sample_venc_qpmap(argc, argv);
        break;
    case 7:
        vs_sample_trace("sample_venc_mjpeg.\n");
        sample_venc_mjpeg(argc, argv);
        break;
    case 8:
        vs_sample_trace("sample_venc_jpeg_mpf.\n");
        sample_venc_jpeg_mpf(argc, argv);
        break;
    case 9:
        vs_sample_trace("sample_venc_lowlatency_vpp.\n");
        sample_venc_lowlatency_vpp(argc, argv);
        break;
    case 10:
        vs_sample_trace("sample_venc_typical_videostrm.\n");
        sample_venc_videostrm(argc, argv);
        break;
    default:
        vs_sample_trace("%s %d not support!\n", argv[0], idx);
        return VS_FAILED;
    }

    return ret;
}
