/**
 * @file    sample_vo.c
 * @brief   sample vo implementation
 * @details
 * @author  Visinex Software Group
 * @date    2022-05-25
 * @version v1.00
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

static vs_uint32_t g_vb_cnt = 12;
static vs_uint32_t g_pool_cnt = 1;
static vs_uint32_t g_buffer_dimension = 1;
static vs_uint32_t g_auto_sleep_time_s = 0;
static vs_bool_t g_dma_pip_en = 0;
static sample_vo_mode_e g_layer0_chn_mode = E_VO_MODE_1MUX;
static sample_vo_mode_e g_layer3_chn_mode = VO_MODE_BUTT;
static vs_vo_interface_type_e g_disp0_intf = 1 << 31;
static vs_vo_interface_type_e g_disp1_intf = 1 << 31;
static vs_vo_output_type_e g_disp0_output = E_VO_OUTPUT_TYPE_1080P60;
static sample_vo_wbc_source_type_e  g_disp0_wbc_type = E_VO_SAMPLE_WBC_DISABLE;
static vs_vo_output_type_e g_disp1_output = E_VO_OUTPUT_TYPE_MAX;
static volatile sig_atomic_t g_stop_flag = 0;

extern vs_bool_t g_nr_3d;
extern vs_int8_t g_bus_id[VII_MAX_ROUTE_NUM];
extern sample_sensor_type_e g_sensor_type[VII_MAX_ROUTE_NUM];

#define align_up(x, alignment) ( (x + alignment - 1) & ~(alignment - 1) )
#define SAMPLE_VO_PLANES_MAX       3
#define SAMPLE_VO_PIP_NUM          2
#define SAMPLE_VO_ACQUIRE_TIMEOUT  1000

typedef struct {
    vs_ive_data_s src;
    vs_ive_data_s dst;
    vs_ive_dma_cfg_s dma_cfg;
    vs_uint32_t handle;
    vs_bool_t block;
    vs_bool_t finish;
} sample_vo_ive_data_info;

typedef struct {
    vs_int32_t vpp_grpid;
    vs_int32_t vpp_main_chnid;
    vs_int32_t vpp_pip_chnid;
    sample_vo_cfg_s *video_cfg;
    pthread_t thread;
    vs_bool_t thread_start;
} sample_vo_dma_pip_thread_info;

static vs_void_t sample_common_vo_pause(vs_void_t)
{
    printf("\n=====Press enter to exit=====\n");
    while (!g_stop_flag) {
        if (getchar() == '\n') {
            break;
        }
        usleep(1000);
    };
    printf("\n=====exit=====\n");
}

static vs_vii_vpp_mode_e sample_vo_get_vii_vpp_mode(vs_bool_t is_llc)
{
    if (is_llc) {
        return E_VII_ONLINE_VPP_ONLINE;
    }

    return E_VII_ONLINE_VPP_OFFLINE;
}

char *sample_vo_display_intf_get(sample_vo_output_intf_type_e vo_intf)
{
    switch (vo_intf) {
        case E_VO_OUTPUT_INTF_TYPE_HDMI:
            return "HDMI";
        case E_VO_OUTPUT_INTF_TYPE_CVBS:
            return "CVBS";
        case E_VO_OUTPUT_INTF_TYPE_BT1120_PROGRESSIVE:
            return "BT1120_P";
        case E_VO_OUTPUT_INTF_TYPE_BT1120_INTERLACE:
            return "BT1120_I";
        case E_VO_OUTPUT_INTF_TYPE_MIPI_DSI_TX:
            return "MIPI";
        default:
            return "DISABLE";
    }
}

char *sample_vo_display_output_get(sample_vo_output_size_type_e vo_timing)
{
    switch (vo_timing) {
        case E_VO_OUTPUT_SIZE_TYPE_PAL:
            return "PAL";
        case E_VO_OUTPUT_SIZE_TYPE_NTSC:
            return "NTSC";
        case E_VO_OUTPUT_SIZE_TYPE_1920X1080P30:
            return "1080P30";
        case E_VO_OUTPUT_SIZE_TYPE_1920X1080I60:
            return "1080I60";
        case E_VO_OUTPUT_SIZE_TYPE_1920X1080P60:
            return "1080P60";
        case E_VO_OUTPUT_SIZE_TYPE_1080X1920_60:
            return "1080X1920";
        case E_VO_OUTPUT_SIZE_TYPE_3840X2160_30:
            return "4K30";
        case E_VO_OUTPUT_SIZE_TYPE_3840X2160_60:
            return "4k60";
        case E_VO_OUTPUT_SIZE_TYPE_USER:
            return "USER";
        default:
            return "DISABLE";
    }
}

char* sample_vo_chn_mode_get(sample_vo_mode_e vo_mode)
{
    switch (vo_mode) {
    case E_VO_MODE_1MUX:
        return "E_VO_MODE_1MUX";
    case E_VO_MODE_2MUX:
        return "E_VO_MODE_2MUX";
    case E_VO_MODE_4MUX:
        return "E_VO_MODE_4MUX";
    case E_VO_MODE_8MUX:
        return "E_VO_MODE_8MUX";
    case E_VO_MODE_9MUX:
        return "E_VO_MODE_9MUX";
    case E_VO_MODE_12MUX:
        return "E_VO_MODE_12MUX";
    case E_VO_MODE_16MUX:
        return "E_VO_MODE_16MUX";
    case E_VO_MODE_25MUX:
        return "E_VO_MODE_25MUX";
    case E_VO_MODE_36MUX:
        return "E_VO_MODE_36MUX";
    default:
        return "NA";
    }
}

static vs_uint32_t sample_vo_chn_num_get(sample_vo_mode_e vo_mode)
{
    vs_uint32_t chn_num;
    switch (vo_mode) {
    case E_VO_MODE_1MUX:
        chn_num = 1;
        break;
    case E_VO_MODE_2MUX:
        chn_num = 2;
        break;
    case E_VO_MODE_4MUX:
        chn_num = 4;
        break;
    case E_VO_MODE_8MUX:
        chn_num = 8;
        break;
    case E_VO_MODE_9MUX:
        chn_num = 9;
        break;
    case E_VO_MODE_12MUX:
        chn_num = 12;
        break;
    case E_VO_MODE_16MUX:
        chn_num = 16;
        break;
    case E_VO_MODE_25MUX:
        chn_num = 25;
        break;
    case E_VO_MODE_36MUX:
        chn_num = 36;
        break;
    default:
        chn_num = 0;
        break;
    }
    return chn_num;
}

static vs_void_t sample_vo_get_vpp_grp_attr(const vs_size_s *img_size, vs_vpp_grp_attr_s *grp_attr)
{
    grp_attr->max_width = img_size->width;
    grp_attr->max_height = img_size->height;
    grp_attr->dynamic_range = E_DYNAMIC_RANGE_SDR8;
    grp_attr->pixel_format = E_PIXEL_FORMAT_YVU_420SP;
    grp_attr->framerate.dst_framerate = -1;
    grp_attr->framerate.src_framerate = -1;
}

static vs_void_t sample_vo_get_vpp_chn_attr(const vs_size_s *img_size, vs_vpp_chn_attr_s *chn_attr,
    const sample_vo_cfg_s *disp_config, vs_bool_t *chn_enable, vs_bool_t pip_chn_en, vs_bool_t is_llc)
{
    vs_int32_t i = 0;
    vs_int32_t scale = 4;

    for (i = 0; i < VPP_MAX_PHYCHN_NUM; i++) {
        if (i > 1)
            continue;

        if (i > 0 && !pip_chn_en && !is_llc)
            continue;

        if (i == 0) {
            if (is_llc)
                continue;

            chn_attr[i].width = img_size->width;
            chn_attr[i].height = img_size->height;

        } else if (i == 1 && pip_chn_en) {
            chn_attr[i].width = align_up((img_size->width / scale), 16);
            chn_attr[i].height = img_size->height / scale;
        } else  {
            chn_attr[i].width  = disp_config->img_width;
            chn_attr[i].height = disp_config->img_height;
        }

        chn_attr[i].chn_mode = E_VPP_CHN_MODE_USER;
        chn_attr[i].video_format = E_VIDEO_FORMAT_LINEAR;
        chn_attr[i].pixel_format = E_PIXEL_FORMAT_YUV_420SP;
        chn_attr[i].dynamic_range = E_DYNAMIC_RANGE_SDR8;
        chn_attr[i].compress_mode = E_COMPRESS_MODE_NONE;
        chn_attr[i].framerate.src_framerate = -1;
        chn_attr[i].framerate.dst_framerate = -1;
        chn_attr[i].mirror_enable = VS_FALSE;
        chn_attr[i].flip_enable = VS_FALSE;
        chn_attr[i].depth = 1;
        chn_attr[i].aspect_ratio.mode = E_ASPECT_RATIO_MODE_NONE;
        chn_enable[i] = VS_TRUE;
    }

}

static vs_void_t sample_vo_get_vo_cfg(vs_size_s *img_size, sample_vo_cfg_s *cfg, vs_int32_t dev_num,
    vs_bool_t dma_pip_en, vs_bool_t is_llc)
{
    vs_uint32_t i = 0;
    if (dev_num > VO_MAX_DEV_NUM)
        return;

    for (i = 0; i < dev_num; i++) {
        if (i == 0) {
            cfg[i].vo_devid = 0;
            cfg[i].vo_layerid = 0;
            cfg[i].vo_intf_type = g_disp0_intf;
            cfg[i].vo_output = g_disp0_output;
            cfg[i].wbc_type = g_disp0_wbc_type;
            cfg[i].vo_mode = g_layer0_chn_mode;
        } else if (i == 1) {
            cfg[i].vo_devid = 1;
            cfg[i].vo_layerid = 3;
            cfg[i].vo_intf_type = g_disp1_intf;
            cfg[i].vo_output = g_disp1_output;
            cfg[i].vo_mode = g_layer3_chn_mode;
        }

        if (cfg[i].vo_output != E_VO_OUTPUT_TYPE_MAX) {
            cfg[i].enable = VS_TRUE;
        } else {
            cfg[i].enable = VS_FALSE;
            continue;
        }

        cfg[i].bg_color = 0;
        cfg[i].zorder  = 0;
        cfg[i].dynamic_range = E_DYNAMIC_RANGE_SDR8;
        cfg[i].pixel_format = E_PIXEL_FORMAT_YUV_420SP;


        if (dma_pip_en) {
            cfg[i].img_width  = 1920;
            cfg[i].img_height = 1080;
        } else {
            cfg[i].img_width  = img_size->width;
            cfg[i].img_height = img_size->height;
        }

        if (is_llc) {
            cfg[i].img_width  = 1920;
            cfg[i].img_height = 1080;
            cfg[i].bypass_layer = VS_TRUE;
            cfg[i].early_cfg_time = 1000;
        }
    }
}

static vs_void_t  sample_vo_get_sample_venc_cfg(sample_venc_cfg_s *sample_venc_cfg, vs_payload_type_e type,
    vs_venc_profile_e profile, vs_size_s frame_size, sample_brc_mode_e brc_mode, vs_venc_gop_attr_s *p_gop_attr)

{
    vs_int32_t output_framerate = 60;

    sample_venc_cfg->format = E_PIXEL_FORMAT_YUV_420SP;
    sample_venc_cfg->compress = VS_FALSE;
    sample_venc_cfg->type = type;
    sample_venc_cfg->profile = profile;
    sample_venc_cfg->frame_size = frame_size;
    sample_venc_cfg->brc_mode = brc_mode;
    sample_venc_cfg->frc.dst_framerate = output_framerate;
    sample_venc_cfg->frc.src_framerate = output_framerate;
    sample_venc_cfg->bandwidth_save_strength = 0;
    if (p_gop_attr != NULL) {
        sample_venc_cfg->gop_attr = *p_gop_attr;
    }
}

static vs_void_t sample_vo_get_stream_threadparam(sample_venc_acquire_stream_param_s *p_acquire_stream_param,
    vs_int32_t *venc_chnid, vs_int32_t chn_num, vs_bool_t store_strm)

{
    vs_int32_t i = VS_SUCCESS;
    memset(p_acquire_stream_param, 0, sizeof(sample_venc_acquire_stream_param_s));
    p_acquire_stream_param->stop_stream_task = VS_FALSE;
    p_acquire_stream_param->chn_num = chn_num;
    p_acquire_stream_param->store_strm = store_strm;
    for (i = 0; i < chn_num; i++) {
        p_acquire_stream_param->venc_chnid[i] = venc_chnid[i];
    }
}
static vs_int32_t sample_vo_get_pixel_bytes(vs_pixel_format_e pixel_format, vs_int32_t plane, vs_int32_t *ext)
{
    vs_int32_t pixel_bytes;

    switch (pixel_format) {
    case E_PIXEL_FORMAT_ARGB8888:
    case E_PIXEL_FORMAT_ABGR8888:
        pixel_bytes = plane == 0 ? 4 : 0;
        break;

    case E_PIXEL_FORMAT_ARGB1555:
    case E_PIXEL_FORMAT_ABGR1555:
    case E_PIXEL_FORMAT_ARGB4444:
    case E_PIXEL_FORMAT_ABGR4444:
    case E_PIXEL_FORMAT_RGB565:
    case E_PIXEL_FORMAT_BGR565:
        pixel_bytes = plane == 0 ? 2 : 0;
        break;

    case E_PIXEL_FORMAT_YUV_420SP:
    case E_PIXEL_FORMAT_YVU_420SP:
    case E_PIXEL_FORMAT_YUV_422SP:
    case E_PIXEL_FORMAT_YVU_422SP:
    case E_PIXEL_FORMAT_YVU_444SP:
        if (plane == 0) {
            pixel_bytes = 1;
        } else if (plane == 1) {
            pixel_bytes = 2;
        } else {
            pixel_bytes = 0;
        }
        break;

    default:
        pixel_bytes = -1;
        break;
    }

    if ((pixel_format == E_PIXEL_FORMAT_YUV_420SP ||
        pixel_format == E_PIXEL_FORMAT_YVU_420SP) &&
        plane == 1) {
        *ext = 2;
    } else {
        *ext = 1;
    }

    return pixel_bytes;
}

static vs_int32_t sample_vo_ive_pip_start(vs_video_frame_info_s *frame, sample_vo_ive_data_info *info,
    vs_int32_t plane_id, vs_int32_t frame_num)
{
    vs_int32_t ret = VS_SUCCESS;
    vs_int32_t ext = 1;

    if (frame_num < 2) {
        vs_sample_trace("frame num must more than 2\n");
        return VS_FAILED;
    }

    if (frame[frame_num - 1].frame.pixel_format !=
        frame[frame_num - 2].frame.pixel_format) {
        vs_sample_trace("frame pixel format not same\n");
        return VS_FAILED;
    }

    sample_vo_get_pixel_bytes(frame[frame_num - 1].frame.pixel_format, plane_id, &ext);

    info->dst.phys_addr = frame[frame_num - 2].frame.phys_addr[plane_id];
    info->dst.virt_addr = (void *)frame[frame_num - 2].frame.virt_addr[plane_id];
    info->dst.stride    = frame[frame_num - 2].frame.stride[plane_id];

    info->src.phys_addr = frame[frame_num - 1].frame.phys_addr[plane_id];
    info->src.virt_addr = (void *)frame[frame_num - 1].frame.virt_addr[plane_id];
    info->src.stride    = frame[frame_num - 1].frame.stride[plane_id];

    info->src.width  = frame[frame_num - 1].frame.width;
    info->src.height = frame[frame_num - 1].frame.height / ext;

    info->dst.width   = info->src.width;
    info->dst.height  = info->src.height;

    info->dma_cfg.mode = E_IVE_DMA_MODE_DIRECT_COPY;
    info->block = VS_TRUE;
    ret = vs_mal_ive_dma(&info->handle, &info->src, &info->dst, &info->dma_cfg, info->block);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_ive_dma set plane[%d] error, failed with ret 0x%x\n", plane_id, ret);
        return VS_FAILED;
    }

    if (info->block == VS_TRUE) {
        ret = vs_mal_ive_query(info->handle, &info->finish, info->block);
        if (ret != VS_SUCCESS || !info->finish) {
            vs_sample_trace("vs_mal_ive_query query palne[%d] failed with ret 0x%x ,finish %d\n", plane_id, ret, info->finish);
        }

        memset(info, 0, sizeof(sample_vo_ive_data_info));

    } else {
        vs_sample_trace("dma pip plane[%d] must use block!\n", plane_id);
        return VS_FAILED;
    }

    return ret;
}

static vs_void_t* sample_vo_dma_pip_thread(vs_void_t *data)
{
    vs_int32_t ret = VS_SUCCESS;
    vs_int32_t i = 0;
    vs_int32_t plane_id = 0;
    vs_int32_t acquire_num = 0;
    sample_vo_dma_pip_thread_info *info = VS_NULL;
    vs_int32_t vpp_grp_id;
    vs_int32_t vpp_chn_id[SAMPLE_VO_PIP_NUM] = {0};
    vs_video_frame_info_s frame[SAMPLE_VO_PIP_NUM] = {0};
    sample_vo_cfg_s *video_config = VS_NULL;
    sample_vo_ive_data_info ive_info[SAMPLE_VO_PLANES_MAX] = {0};

    if (data == VS_NULL)
        return VS_NULL;

    info = (sample_vo_dma_pip_thread_info *)data;
    if (!info->video_cfg)
        return VS_NULL;

    video_config = info->video_cfg;
    vpp_grp_id = info->vpp_grpid;

    for (i = 0; i < SAMPLE_VO_PIP_NUM; i++) {
        if (i == 0)
            vpp_chn_id[i] = info->vpp_main_chnid;
        else if (i == 1)
            vpp_chn_id[i] = info->vpp_pip_chnid;
    }

    printf("vpp to dma pip to vo start.\n");

    while(!g_stop_flag) {
        memset(&frame, 0, sizeof(vs_video_frame_info_s) * SAMPLE_VO_PIP_NUM);
        for (acquire_num = 0; acquire_num < SAMPLE_VO_PIP_NUM; acquire_num++) {
            ret = vs_mal_vpp_chn_frame_acquire(vpp_grp_id, vpp_chn_id[acquire_num],
             &frame[acquire_num], SAMPLE_VO_ACQUIRE_TIMEOUT);
            if (ret != VS_SUCCESS) {
                vs_sample_trace("vs_mal_vpp_chn_frame_acquire grp%d chn%d failed with ret 0x%x\n",
                    vpp_grp_id, vpp_chn_id[acquire_num], ret);
                usleep(20 * 1000);
                goto acquire_free;
            }
        }

        memset(&ive_info, 0, sizeof(sample_vo_ive_data_info) * SAMPLE_VO_PLANES_MAX);

        for (plane_id = 0; plane_id < SAMPLE_VO_PLANES_MAX; plane_id++) {
            if (frame[SAMPLE_VO_PLANES_MAX - 2].frame.stride[plane_id] == 0)
                continue;

            ret = sample_vo_ive_pip_start(frame, &ive_info[plane_id], plane_id, SAMPLE_VO_PIP_NUM);
            if (ret != VS_SUCCESS)
                goto acquire_free;
        }

            ret = vs_mal_vo_chn_frame_send(video_config->vo_layerid, 0, &frame[SAMPLE_VO_PIP_NUM - 2], SAMPLE_VO_ACQUIRE_TIMEOUT);

    acquire_free:
        for (acquire_num--; acquire_num >= 0; acquire_num--) {
            vs_mal_vpp_chn_frame_release(vpp_grp_id, vpp_chn_id[acquire_num], &frame[acquire_num]);
        }
    };

    printf("vpp to ive to vo stop.\n");
    return VS_NULL;
}

static vs_void_t sample_vo_dma_pip_thread_stop(sample_vo_dma_pip_thread_info *info)
{
    if (info->thread_start) {
        pthread_join(info->thread, NULL);
        info->thread_start = VS_FALSE;
    }
}

vs_int32_t sample_vo_dma_pip_thread_start(sample_vo_dma_pip_thread_info *info, sample_vo_cfg_s *cfg,
    vs_int32_t grp_id, vs_bool_t *chn_enable)
{
    vs_int32_t ret = VS_SUCCESS;
    if (!info || !cfg || !chn_enable) {
        vs_sample_trace("info or cfg or chn enable is null!\n");
        return VS_FAILED;
    }

    if (!chn_enable[0] || !chn_enable[1]) {
        vs_sample_trace("vpp grp%d chn0 is %s, vpp grp%d chn1 is %s!\n",
            grp_id, chn_enable[0] ? "enable" : "not enable",
            grp_id, chn_enable[1] ? "enable" : "not enable");
        return VS_FAILED;
    }

    info->video_cfg = cfg;
    info->vpp_grpid = grp_id;
    info->vpp_main_chnid = 0;
    info->vpp_pip_chnid = 1;

    pthread_create(&info->thread, VS_NULL, sample_vo_dma_pip_thread,
        (vs_void_t *)(info));
    info->thread_start = VS_TRUE;

    return ret;
}

#ifndef VS_ORION
static vs_void_t sample_vo_hdmi_none_case_get()
{
    g_disp0_intf = E_VO_INTERFACE_TYPE_HDMI;
    g_disp0_output = E_VO_OUTPUT_TYPE_1080P60;
    g_layer0_chn_mode = E_VO_MODE_1MUX;

    g_disp1_intf = 1 << 31;

    g_vb_cnt = 11;
}

static vs_void_t sample_vo_hdmi_cvbs_case_get()
{
    g_disp0_intf = E_VO_INTERFACE_TYPE_HDMI;
    g_disp0_output = E_VO_OUTPUT_TYPE_1080P60;
    g_layer0_chn_mode = E_VO_MODE_1MUX;

    g_disp1_intf = E_VO_INTERFACE_TYPE_CVBS;
    g_disp1_output =E_VO_OUTPUT_TYPE_PAL;
    g_layer3_chn_mode = E_VO_MODE_1MUX;

    g_vb_cnt = 24;
}
static vs_void_t sample_vo_hdmi_mipi_case_get()
{
    g_disp0_intf = E_VO_INTERFACE_TYPE_HDMI;
    g_disp0_output = E_VO_OUTPUT_TYPE_1080P60;
    g_layer0_chn_mode = E_VO_MODE_1MUX;

    g_disp1_intf = E_VO_INTERFACE_TYPE_MIPI;
    g_disp1_output =E_VO_OUTPUT_TYPE_USER;
    g_layer3_chn_mode = E_VO_MODE_1MUX;

    g_vb_cnt = 35;
}

static vs_void_t sample_vo_hdmi_bt1120_case_get()
{
    g_disp0_intf = E_VO_INTERFACE_TYPE_HDMI;
    g_disp0_output = E_VO_OUTPUT_TYPE_1080P60;
    g_layer0_chn_mode = E_VO_MODE_1MUX;

    g_disp1_intf = E_VO_INTERFACE_TYPE_BT1120;
    g_disp1_output = E_VO_OUTPUT_TYPE_1080P60;
    g_layer3_chn_mode = E_VO_MODE_1MUX;

    g_vb_cnt = 24;
}

static vs_void_t sample_vo_hdmi_bt1120_same_source_case_get()
{
    g_disp0_intf = E_VO_INTERFACE_TYPE_HDMI | E_VO_INTERFACE_TYPE_BT1120;
    g_disp0_output = E_VO_OUTPUT_TYPE_1080P60;
    g_layer0_chn_mode = E_VO_MODE_1MUX;

    g_vb_cnt = 24;
}

static vs_void_t sample_vo_hdmi_user_case_get()
{
    g_disp0_intf = E_VO_INTERFACE_TYPE_HDMI;
    g_disp0_output = E_VO_OUTPUT_TYPE_USER;
    g_layer0_chn_mode = E_VO_MODE_1MUX;

    g_disp1_intf = 1 << 31;

    g_vb_cnt = 11;
}

static vs_void_t sample_vo_dma_pip_case_get()
{
    g_disp0_intf = E_VO_INTERFACE_TYPE_HDMI;
    g_disp0_output = E_VO_OUTPUT_TYPE_1080P60;
    g_layer0_chn_mode = E_VO_MODE_1MUX;
    g_dma_pip_en = VS_TRUE;

    g_disp1_intf = 1 << 31;

    g_vb_cnt = 14;
}

#else
static vs_void_t sample_vo_mipi_case_get()
{
    g_disp0_intf = E_VO_INTERFACE_TYPE_MIPI;
    g_disp0_output = E_VO_OUTPUT_TYPE_USER;
    g_layer0_chn_mode = E_VO_MODE_1MUX;

    g_vb_cnt = 12;
}

static vs_void_t sample_vo_bt1120p_case_get()
{
    g_disp0_intf = E_VO_INTERFACE_TYPE_BT1120;
    g_disp0_output = E_VO_OUTPUT_TYPE_1080P60;
    g_layer0_chn_mode = E_VO_MODE_1MUX;
}

static vs_void_t sample_vo_bt1120i_case_get()
{
    g_disp0_intf = E_VO_INTERFACE_TYPE_BT1120;
    g_disp0_output = E_VO_OUTPUT_TYPE_1080I60;
    g_layer0_chn_mode = E_VO_MODE_1MUX;
}

static vs_void_t sample_vo_bt1120_mipi_same_source_case_get()
{
    g_disp0_intf = E_VO_INTERFACE_TYPE_BT1120 | E_VO_INTERFACE_TYPE_MIPI;
    g_disp0_output = E_VO_OUTPUT_TYPE_1080P60;
    g_layer0_chn_mode = E_VO_MODE_1MUX;

    g_vb_cnt = 24;
}

static vs_void_t sample_vo_dma_pip_case_get()
{
    g_disp0_intf = E_VO_INTERFACE_TYPE_MIPI;
    g_disp0_output = E_VO_OUTPUT_SIZE_TYPE_USER;
    g_layer0_chn_mode = E_VO_MODE_1MUX;
    g_dma_pip_en = VS_TRUE;

    g_disp1_intf = 1 << 31;

    g_vb_cnt = 14;
}
#endif

static vs_void_t sample_vo_vii_vpp_vo_lowlatency_case_get()
{
#ifndef VS_ORION
    g_disp0_intf = E_VO_INTERFACE_TYPE_HDMI;
    g_disp0_output = E_VO_OUTPUT_TYPE_1080P60;
#else
    g_disp0_intf = E_VO_INTERFACE_TYPE_BT1120;
    g_disp0_output = E_VO_OUTPUT_TYPE_1080P30;
#endif

    g_layer0_chn_mode = E_VO_MODE_1MUX;

    g_disp1_intf = 1 << 31;

    g_vb_cnt = 14;
}

static vs_void_t sample_vo_device_wbc_case_get()
{
    g_disp0_intf = E_VO_INTERFACE_TYPE_MIPI;
    g_disp0_output = E_VO_OUTPUT_TYPE_1080P60;
    g_layer0_chn_mode = E_VO_MODE_1MUX;
    g_disp0_wbc_type = E_VO_SAMPLE_WBC_SOURCE_TYPE_DEVICE;
    g_vb_cnt = 24;
}

static vs_void_t sample_vo_mipi_4k_case_get()
{
    g_disp0_intf = E_VO_INTERFACE_TYPE_MIPI;
    g_disp0_output = E_VO_OUTPUT_TYPE_3840x2160_30;
    g_layer0_chn_mode = E_VO_MODE_1MUX;

    g_vb_cnt = 12;
}

static vs_int32_t sammple_vo_vpp_chn_lowlatency_set(vs_int32_t vpp_grpid, vs_bool_t *chn_enable)
{
    vs_int32_t ret = VS_SUCCESS;
    vs_int32_t vpp_chnid = 0;
    vs_lowlatency_attr_s lowlatency_attr = {0};

    lowlatency_attr.enable = VS_TRUE;
    lowlatency_attr.mode = E_LOWLATENCY_MODE_NORMAL;

    for (vpp_chnid = 0; vpp_chnid < VPP_MAX_PHYCHN_NUM; ++vpp_chnid) {
        if (!chn_enable[vpp_chnid])
            continue;

        ret = vs_mal_vpp_chn_lowlatency_set(vpp_grpid, vpp_chnid, &lowlatency_attr);
        if (ret != VS_SUCCESS) {
            vs_sample_trace("vpp grp%d chn%d set lowlatency failed ret is 0x%x\n", vpp_grpid, vpp_chnid, ret);
            return ret;
        }
    }

    return ret;
}

vs_void_t sammple_vo_vpp_grp_unbind_vo(const sample_vo_cfg_s *vo_cfg, vs_int32_t vpp_grpid, vs_bool_t *chn_enable,
    vs_int32_t last_grp_num, vs_bool_t is_llc)
{
    vs_int32_t vpp_chnid = 0;
    vs_int32_t vo_chnid = 0;
    vs_uint32_t vo_chn_num = sample_vo_chn_num_get(vo_cfg->vo_mode);

    for (vpp_chnid = 0; vpp_chnid < VPP_MAX_PHYCHN_NUM; ++vpp_chnid) {
        if (!chn_enable[vpp_chnid])
            continue;

        if (is_llc && vpp_chnid == 1) {
            vo_chnid = 0;
            vo_chn_num = 1;
        } else {
            vo_chnid = (vpp_grpid - last_grp_num) * 4 + vpp_chnid;
            if (vo_chnid >= vo_chn_num)
                continue;
        }

        sample_common_vpp_unbind_vo(vpp_grpid, vpp_chnid, vo_cfg->vo_layerid, vo_chnid);
    }
}

static vs_int32_t sammple_vo_vpp_grp_bind_vo(const sample_vo_cfg_s *vo_cfg, vs_int32_t vpp_grpid, vs_bool_t *chn_enable,
    vs_int32_t last_grp_num, vs_bool_t is_llc)
{
    vs_int32_t vpp_chnid = 0;
    vs_int32_t vo_chnid = 0;
    vs_int32_t ret = VS_SUCCESS;
    vs_uint32_t vo_chn_num = sample_vo_chn_num_get(vo_cfg->vo_mode);

    for (vpp_chnid = 0; vpp_chnid < VPP_MAX_PHYCHN_NUM; ++vpp_chnid) {
        if (!chn_enable[vpp_chnid])
            continue;

        if (is_llc && vpp_chnid == 1) {
            vo_chnid = 0;
            vo_chn_num = 1;
        } else {
            vo_chnid = (vpp_grpid - last_grp_num) * 4 + vpp_chnid;
            if (vo_chnid >= vo_chn_num)
                continue;
        }

        ret = sample_common_vpp_bind_vo(vpp_grpid, vpp_chnid, vo_cfg->vo_layerid, vo_chnid);
        if (ret != VS_SUCCESS) {
            goto failed;
        }
    }

    return ret;

failed:
    for (vpp_chnid--; vpp_chnid >= 0; vpp_chnid--) {
        if (!chn_enable[vpp_chnid])
            continue;

        if (is_llc && vpp_chnid == 1) {
            vo_chnid = 0;
            vo_chn_num = 1;
        } else {
            vo_chnid = (vpp_grpid - last_grp_num) * 4 + vpp_chnid;
            if (vo_chnid >= vo_chn_num)
                continue;
        }

        sample_common_vpp_unbind_vo(vpp_grpid, vpp_chnid, vo_cfg->vo_layerid, vo_chnid);
    }

    return ret;
}

static vs_void_t sample_vo_vpp_unbind_vo(const sample_vo_cfg_s *vo_cfg, vs_int32_t *vo_vpp_grpnum, vs_int32_t dev_num,
    vs_bool_t *chn_enable, vs_int32_t vpp_chn_num, vs_bool_t is_llc)
{
    vs_int32_t vpp_grpid;
    vs_int32_t i;
    vs_int32_t sum_grpnum = 0;

    if (dev_num > VO_MAX_DEV_NUM) {
        vs_sample_trace("vo dev num %d is more than max %d\n", dev_num, VO_MAX_DEV_NUM);
        return;
    }

    if (vpp_chn_num != VPP_MAX_PHYCHN_NUM) {
        vs_sample_trace("vpp chn num must be %d is more than max %d\n", vpp_chn_num, VPP_MAX_PHYCHN_NUM);
    }

    for (i = 0; i < VO_MAX_DEV_NUM; i++) {
        if (!vo_cfg[i].enable)
            continue;

        for (vpp_grpid = sum_grpnum; vpp_grpid < sum_grpnum + vo_vpp_grpnum[i]; ++vpp_grpid) {
            sammple_vo_vpp_grp_unbind_vo(&vo_cfg[i], vpp_grpid, (chn_enable + i * VPP_MAX_PHYCHN_NUM), sum_grpnum, is_llc);
        }

        sum_grpnum += vo_vpp_grpnum[i];
    }

    return;
}

static vs_int32_t sample_vo_vpp_bind_vo(const sample_vo_cfg_s *vo_cfg, vs_int32_t *vo_vpp_grpnum, vs_int32_t dev_num,
    vs_bool_t *chn_enable, vs_int32_t vpp_chn_num, vs_bool_t is_llc)
{
    vs_int32_t vpp_grpid;
    vs_int32_t ret = VS_SUCCESS, i;
    vs_int32_t sum_grpnum = 0;

    if (dev_num > VO_MAX_DEV_NUM) {
        vs_sample_trace("vo dev num %d is more than max %d\n", dev_num, VO_MAX_DEV_NUM);
        return VS_FAILED;
    }

    if (vpp_chn_num != VPP_MAX_PHYCHN_NUM) {
        vs_sample_trace("vpp chn num must be %d is more than max %d\n", vpp_chn_num, VPP_MAX_PHYCHN_NUM);
        return VS_FAILED;
    }

    for (i = 0; i < VO_MAX_DEV_NUM; i++) {
        if (!vo_cfg[i].enable)
            continue;

        for (vpp_grpid = sum_grpnum; vpp_grpid < sum_grpnum + vo_vpp_grpnum[i]; ++vpp_grpid) {
            ret = sammple_vo_vpp_grp_bind_vo(&vo_cfg[i], vpp_grpid, (chn_enable + i * VPP_MAX_PHYCHN_NUM), sum_grpnum, is_llc);
            if (ret != VS_SUCCESS) {
                for (vpp_grpid--; vpp_grpid >= sum_grpnum; vpp_grpid--) {
                    sammple_vo_vpp_grp_unbind_vo(&vo_cfg[i], vpp_grpid, (chn_enable + i * VPP_MAX_PHYCHN_NUM), sum_grpnum, is_llc);
                }
                goto failed;
            }
        }

        sum_grpnum += vo_vpp_grpnum[i];
    }

    return ret;

failed:
    for (i--; i >= 0; i--) {
        if (!vo_cfg[i].enable)
            continue;

        sum_grpnum -= vo_vpp_grpnum[i];
        for (vpp_grpid = sum_grpnum; vpp_grpid < sum_grpnum + vo_vpp_grpnum[i]; ++vpp_grpid) {
            sammple_vo_vpp_grp_unbind_vo(&vo_cfg[i], vpp_grpid, (chn_enable + i * VPP_MAX_PHYCHN_NUM), sum_grpnum, is_llc);
        }
    }

    return ret;
}

static vs_void_t sample_vo_vii_unbind_vpp(sample_vo_cfg_s *vo_cfg, vs_int32_t *disp_vpp_grpnum, vs_int32_t dev_num,
    vs_int32_t vii_pipeid, vs_int32_t vii_chnid)
{
    vs_int32_t vpp_grpid = 0;
    vs_int32_t sum_grpnum = 0;
    vs_int32_t i;

    if (dev_num > VO_MAX_DEV_NUM) {
        vs_sample_trace("vo dev num %d is more than max %d\n", dev_num, VO_MAX_DEV_NUM);
        return;
    }

    for (i = 0; i < VO_MAX_DEV_NUM; i++) {
        if (!vo_cfg[i].enable)
            continue;

        sum_grpnum += disp_vpp_grpnum[i];
    }

    for (vpp_grpid = 0; vpp_grpid < sum_grpnum; ++vpp_grpid) {
        sample_common_vii_unbind_vpp(vii_pipeid, vii_chnid, vpp_grpid);
    }

}

static vs_int32_t sample_vo_vii_bind_vpp(const sample_vo_cfg_s *vo_cfg, const vs_int32_t *vo_vpp_grpnum, vs_int32_t dev_num,
    vs_int32_t vii_pipeid, vs_int32_t vii_chnid)
{
    vs_int32_t vpp_grpid = 0;
    vs_int32_t sum_grpnum = 0;
    vs_int32_t ret = VS_SUCCESS, i;

    if (dev_num > VO_MAX_DEV_NUM) {
        vs_sample_trace("vo dev num %d is more than max %d\n", dev_num, VO_MAX_DEV_NUM);
        return VS_FAILED;
    }

    for (i = 0; i < dev_num; i++) {
        if (!vo_cfg[i].enable)
            continue;

        sum_grpnum += vo_vpp_grpnum[i];
    }

    for (vpp_grpid = 0; vpp_grpid < sum_grpnum; ++vpp_grpid) {
        ret = sample_common_vii_bind_vpp(vii_pipeid, vii_chnid, vpp_grpid);
            if (ret != VS_SUCCESS) {
                goto failed;
        }
    }

    return ret;

failed:
    for (vpp_grpid --; vpp_grpid >= 0; vpp_grpid--) {
        sample_common_vii_unbind_vpp(vii_pipeid, vii_chnid, vpp_grpid);
    }

    return ret;
}

static vs_int32_t sample_vo_vpp_stop(const sample_vo_cfg_s *vo_cfg, const vs_int32_t *vo_vpp_grpnum,
    vs_int32_t dev_num, vs_bool_t *chn_enable, vs_int32_t vpp_chn_num)
{
    vs_int32_t vpp_grpid = 0;
    vs_int32_t sum_grpnum = 0;
    vs_int32_t ret = VS_SUCCESS, i;

    if (dev_num > VO_MAX_DEV_NUM) {
        vs_sample_trace("vo dev num %d is more than max %d\n", dev_num, VO_MAX_DEV_NUM);
        return VS_FAILED;
    }

    if (vpp_chn_num != VPP_MAX_PHYCHN_NUM) {
        vs_sample_trace("vpp chn num must be %d is more than max %d\n", vpp_chn_num, VPP_MAX_PHYCHN_NUM);
        return VS_FAILED;
    }

    for (i = 0; i < dev_num; i++) {
        if (!vo_cfg[i].enable)
            continue;

        for (vpp_grpid = sum_grpnum; vpp_grpid < sum_grpnum + vo_vpp_grpnum[i]; ++vpp_grpid) {
            sample_common_vpp_stop(vpp_grpid, (chn_enable + i * VPP_MAX_PHYCHN_NUM));
        }

        sum_grpnum += vo_vpp_grpnum[i];
    }

    return ret;
}

static vs_int32_t sample_vo_vpp_start(const sample_vo_cfg_s *vo_cfg, const vs_int32_t *vo_vpp_grpnum,
    vs_int32_t dev_num, const vs_size_s *img_size,
    vs_bool_t *chn_enable, vs_int32_t vpp_chn_num, vs_bool_t pip_en, vs_bool_t llc_en)
{
    vs_int32_t vpp_grpid = 0;
    vs_int32_t sum_grpnum = 0;
    vs_int32_t ret = VS_SUCCESS, i;
    vs_vpp_grp_attr_s vpp_grp_attr;
    vs_vpp_chn_attr_s vpp_chn_attr[VPP_MAX_PHYCHN_NUM];

    if (dev_num > VO_MAX_DEV_NUM) {
        vs_sample_trace("vo dev num %d is more than max %d\n", dev_num, VO_MAX_DEV_NUM);
        return VS_FAILED;
    }

    if (vpp_chn_num != VPP_MAX_PHYCHN_NUM) {
        vs_sample_trace("vpp chn num must be %d is more than max %d\n", vpp_chn_num, VPP_MAX_PHYCHN_NUM);
        return VS_FAILED;
    }

    sample_vo_get_vpp_grp_attr(img_size, &vpp_grp_attr);

    for (i = 0; i < dev_num; i++) {
        if (!vo_cfg[i].enable)
            continue;

        sample_vo_get_vpp_chn_attr(img_size, vpp_chn_attr, &vo_cfg[i], (chn_enable + i * VPP_MAX_PHYCHN_NUM),
            i == 0 ? pip_en : VS_FALSE, llc_en);

        for (vpp_grpid = sum_grpnum; vpp_grpid < sum_grpnum + vo_vpp_grpnum[i]; ++vpp_grpid) {
            ret = sample_common_vpp_start(vpp_grpid, (chn_enable + i * VPP_MAX_PHYCHN_NUM), &vpp_grp_attr, vpp_chn_attr);
            if (ret != VS_SUCCESS) {
                for (vpp_grpid--; vpp_grpid >= sum_grpnum; vpp_grpid--) {
                    sample_common_vpp_stop(vpp_grpid, (chn_enable + i * VPP_MAX_PHYCHN_NUM));
                }
                goto failed;
            }

            if (llc_en) {
                ret = sammple_vo_vpp_chn_lowlatency_set(vpp_grpid, (chn_enable + i * VPP_MAX_PHYCHN_NUM));
                if (ret != VS_SUCCESS) {
                    for (vpp_grpid--; vpp_grpid >= sum_grpnum; vpp_grpid--) {
                        sample_common_vpp_stop(vpp_grpid, (chn_enable + i * VPP_MAX_PHYCHN_NUM));
                    }
                    goto failed;
                }
            }
        }

        sum_grpnum += vo_vpp_grpnum[i];
    }

    return ret;

failed:
    for (i--; i >= 0; i--) {
        if (!vo_cfg[i].enable)
            continue;

        sum_grpnum -= vo_vpp_grpnum[i];
        for (vpp_grpid = sum_grpnum; vpp_grpid < sum_grpnum + vo_vpp_grpnum[i]; ++vpp_grpid) {
            sample_common_vpp_stop(vpp_grpid, (chn_enable + i * VPP_MAX_PHYCHN_NUM));
        }
    }
    return ret;
}

static vs_void_t sample_vo_vo_stop(sample_vo_cfg_s *vo_cfg, vs_int32_t dev_num)
{
    vs_int32_t i;

    if (dev_num > VO_MAX_DEV_NUM) {
        vs_sample_trace("vo dev num %d is more than max %d\n", dev_num, VO_MAX_DEV_NUM);
        return;
    }

    for (i = 0; i < VO_MAX_DEV_NUM; i++) {
        if (!vo_cfg[i].enable)
            continue;

        sample_common_vo_stop(&vo_cfg[i]);
    }
}

static vs_int32_t sample_vo_vo_start(sample_vo_cfg_s *vo_cfg, vs_int32_t dev_num, vs_bool_t is_llc)
{
    vs_int32_t ret = VS_SUCCESS, i;

    if (dev_num > VO_MAX_DEV_NUM) {
        vs_sample_trace("vo dev num %d is more than max %d\n", dev_num, VO_MAX_DEV_NUM);
        return VS_FAILED;
    }

    for (i = 0; i < VO_MAX_DEV_NUM; i++) {
        if (!vo_cfg[i].enable)
            continue;

        ret = sample_common_vo_start(&vo_cfg[i]);
        if (ret != VS_SUCCESS) {
            goto failed;
        }

        if (is_llc) {
            ret = vs_mal_vo_chn_buf_depth_set(vo_cfg[i].vo_layerid, 0, 2);
            if (ret != VS_SUCCESS) {
                vs_sample_trace("vs_mal_vo_chn_buf_depth_set(vo_layerid %d vo_chnid %d) failed with 0x%x\n",
                    vo_cfg[i].vo_layerid, 0, ret);
                return VS_FAILED;
            }
        }
    }

    return ret;

failed:
    for (i-- ; i >= 0; i--) {
        if (!vo_cfg[i].enable)
            continue;

        sample_common_vo_stop(&vo_cfg[i]);
    }

    return ret;
}

vs_int32_t sample_vo_vii_vpp_vo_venc_case(vs_bool_t is_llc)
{

    vs_int32_t ret;
    vs_size_s img_size;
    vs_vb_cfg_s vb_cfg;
    vs_uint32_t frame_num;
    sample_vii_cfg_s vii_cfg;
    sample_vo_cfg_s vo_cfg[VO_MAX_DEV_NUM] = {0,};
    sample_vo_dma_pip_thread_info thread_info = {0};
    vs_uint32_t i = 0;
    vs_int32_t sensor_id = 0;
    vs_int32_t vii_chnid = 0;
    vs_int32_t vii_pipeid = 0;
    vs_int32_t vo_vpp_grpnum[VO_MAX_DEV_NUM] = {0};
    vs_int32_t sum_grpnum = 0;
    vs_uint32_t vo_chn_num[VO_MAX_DEV_NUM] = {0};
    vs_bool_t chn_enable[VO_MAX_DEV_NUM][VPP_MAX_PHYCHN_NUM] = {0,};
    vs_pixel_format_e format;
    vs_char_t name[100] = "/lib/firmware/vs_dsp0.bin";

    vs_int32_t venc_chnnum = 0;
    vs_int32_t venc_chnid[VPP_MAX_PHYCHN_NUM] = {0, 1, 2, 3};
    vs_payload_type_e encode_type[VPP_MAX_PHYCHN_NUM] = {E_PT_TYPE_H265, E_PT_TYPE_JPEG,
                                                         E_PT_TYPE_H265, E_PT_TYPE_H265};
    vs_venc_profile_e profile[VPP_MAX_PHYCHN_NUM] = {E_VENC_PROFILE_H265_MAIN, E_VENC_PROFILE_JPEG_BASELINE,
                                                     E_VENC_PROFILE_H265_MAIN, E_VENC_PROFILE_H265_MAIN};
    sample_brc_mode_e brc_mode = E_VENC_BRC_CBR;
    vs_size_s venc_input_img_size = {0};
    vs_venc_gop_attr_s gop_attr = {0};
    sample_venc_cfg_s sample_venc_cfg = {0};
    sample_venc_acquire_stream_param_s acquire_stream_param = {0};

    sample_common_vii_sensor_img_size_get(sensor_id, &img_size);
    sample_common_vii_sensor_pixel_format_get(sensor_id, &format);
    frame_num = sample_common_vii_wdr_frame_num_get(sensor_id);
    vb_cfg.pool_cnt = g_pool_cnt;

    for (i = 0; i < g_pool_cnt; i++) {
        if (g_disp0_wbc_type == E_VO_SAMPLE_WBC_SOURCE_TYPE_DEVICE) {
            vb_cfg.ast_commpool[i].blk_size = g_buffer_dimension *
                sample_common_buffer_size_get(&img_size, E_PIXEL_FORMAT_ARGB8888, E_COMPRESS_MODE_NONE, frame_num);
        } else {
            vb_cfg.ast_commpool[i].blk_size = g_buffer_dimension *
                sample_common_buffer_size_get(&img_size, format, E_COMPRESS_MODE_NONE, frame_num);
        }

        vb_cfg.ast_commpool[i].blk_cnt = g_vb_cnt;
        vb_cfg.ast_commpool[i].remap_mode = VB_REMAP_MODE_NONE;
    }

    sample_vo_get_vo_cfg(&img_size, vo_cfg, VO_MAX_DEV_NUM, g_dma_pip_en, is_llc);

    for (i = 0; i < VO_MAX_DEV_NUM; i++) {
        vo_chn_num[i] = sample_vo_chn_num_get(vo_cfg[i].vo_mode);
        vo_vpp_grpnum[i] = align_up(vo_chn_num[i], 4) / 4;
        sum_grpnum += vo_vpp_grpnum[i];
    }


    if (sum_grpnum > 8) {
        vs_sample_trace("max vpp group num 8\n");
        ret = VS_FAILED;
        goto exit0;
    }

    ret = sample_common_sys_init(&vb_cfg);
    if (ret != VS_SUCCESS) {
        goto exit0;
    }

    /* for rotation need dsp */
    for (i = 0; i < VO_MAX_DEV_NUM; i++) {
        if ((vo_cfg[i].vo_intf_type & E_VO_INTERFACE_TYPE_MIPI) == 0)
            continue;

        if(vo_cfg[i].vo_output != E_VO_OUTPUT_TYPE_USER)
            continue;

        if(vo_cfg[i].wbc_type == E_VO_SAMPLE_WBC_SOURCE_TYPE_DEVICE)
            continue;

        ret = sample_common_dsp_init(0, name);
        if (ret != VS_SUCCESS) {
            vs_sample_trace("sample_common_dsp_init failed, ret[0x%x]\n", ret);
            goto exit1;
        }

        break;
    }

    vii_cfg.vii_vpp_mode = sample_vo_get_vii_vpp_mode(is_llc);
    vii_cfg.route_num = 1;
    sample_common_vii_default_cfg_get(sensor_id, &vii_cfg.route_cfg[0]);
    if (is_llc) {
        vii_cfg.route_cfg[0].pipe_cfg[0].isp_attr.frame_rate += 0.1;
    }

    sample_common_wbc_size_get(&vo_cfg[0], &venc_input_img_size);

    ret = sample_common_vii_start(&vii_cfg);
    if (ret != VS_SUCCESS) {
        goto exit2;
    }

    ret = sample_vo_vii_bind_vpp(vo_cfg, vo_vpp_grpnum, VO_MAX_DEV_NUM, vii_pipeid, vii_chnid);
    if (ret != VS_SUCCESS) {
        goto exit3;
    }

    ret = sample_vo_vpp_start(vo_cfg, vo_vpp_grpnum, VO_MAX_DEV_NUM, &img_size,
        &chn_enable[0][0], VPP_MAX_PHYCHN_NUM, g_dma_pip_en, is_llc);
    if (ret != VS_SUCCESS) {
        goto exit4;
    }

    if (g_dma_pip_en) {
        ret = sample_vo_dma_pip_thread_start(&thread_info, &vo_cfg[0], 0, chn_enable[0]);
        if (ret != VS_SUCCESS) {
            goto exit5;
        }
    } else {
        ret = sample_vo_vpp_bind_vo(vo_cfg, vo_vpp_grpnum, VO_MAX_DEV_NUM, &chn_enable[0][0],
            VPP_MAX_PHYCHN_NUM, is_llc);
        if (ret != VS_SUCCESS) {
            goto exit5;
        }
    }

    ret = sample_vo_vo_start(vo_cfg, VO_MAX_DEV_NUM, is_llc);
    if (ret != VS_SUCCESS) {
        goto exit6;
    }

    //when have wbc, start venc
    if (vo_cfg[0].wbc_type > E_VO_SAMPLE_WBC_DISABLE &&
        vo_cfg[0].wbc_type < E_VO_SAMPLE_WBC_SOURCE_TYPE_MAX) {
        ret = sample_common_venc_gop_attr_get(E_VENC_GOP_MODE_NORMP, &gop_attr);

        if (ret != VS_SUCCESS) {
            vs_sample_trace("venc_gop_attr_get failed, ret[%d]\n", ret);
            goto exit7;
        }

        sample_vo_get_sample_venc_cfg(&sample_venc_cfg, encode_type[0], profile[0], venc_input_img_size, brc_mode, &gop_attr);
        ret = sample_common_venc_start(venc_chnid[0], &sample_venc_cfg);
        if (ret != VS_SUCCESS) {
            vs_sample_trace("sample_common_venc_start failed, ret[%d]\n", ret);
            goto exit7;
        }

        ret = sample_common_vo_bind_venc(vo_cfg[0].vo_devid, 0, venc_chnid[0]);
        if (ret != VS_SUCCESS) {
            vs_sample_trace("sample_common_vo_bind_venc failed, ret[%d]\n", ret);
            sample_common_venc_stop(venc_chnid[0]);
            goto exit7;
        }

        sample_vo_get_stream_threadparam(&acquire_stream_param, venc_chnid, venc_chnnum, VS_TRUE);
        ret = sample_common_venc_acquire_stream_start(&acquire_stream_param);
        if (ret != VS_SUCCESS) {
            vs_sample_trace("sample_common_venc_get_stream_start failed, ret[%d]\n", ret);
            sample_common_vo_unbind_venc(vo_cfg[0].vo_devid, 0, venc_chnid[0]);
            sample_common_venc_stop(venc_chnid[0]);
            goto exit7;
        }
    }

    if (g_auto_sleep_time_s == 0) {
        sample_common_vo_pause();
    } else {
        printf("\n=====SLEEP %u s=====\n", g_auto_sleep_time_s);
        usleep(g_auto_sleep_time_s * 1000 * 1000);
    }

    g_stop_flag = VS_TRUE;

if (vo_cfg[0].wbc_type > E_VO_SAMPLE_WBC_DISABLE &&
    vo_cfg[0].wbc_type < E_VO_SAMPLE_WBC_SOURCE_TYPE_MAX) {
        sample_common_venc_acquire_stream_stop(&acquire_stream_param);
        sample_common_vo_unbind_venc(vo_cfg[0].vo_devid, 0, venc_chnid[0]);
        sample_common_venc_stop(venc_chnid[0]);
    }

exit7:
    sample_vo_vo_stop(vo_cfg, VO_MAX_DEV_NUM);

exit6:
    if (g_dma_pip_en) {
        g_stop_flag = VS_TRUE;
        sample_vo_dma_pip_thread_stop(&thread_info);
    } else {
        sample_vo_vpp_unbind_vo(vo_cfg, vo_vpp_grpnum, VO_MAX_DEV_NUM, &chn_enable[0][0], VPP_MAX_PHYCHN_NUM, is_llc);
    }

exit5:
    sample_vo_vpp_stop(vo_cfg, vo_vpp_grpnum, VO_MAX_DEV_NUM, &chn_enable[0][0], VPP_MAX_PHYCHN_NUM);

exit4:
    sample_vo_vii_unbind_vpp(vo_cfg, vo_vpp_grpnum, VO_MAX_DEV_NUM, vii_pipeid, vii_chnid);

exit3:
    sample_common_vii_stop(&vii_cfg);

exit2:
    for (i = 0; i < VO_MAX_DEV_NUM; i++) {
        if ((vo_cfg[i].vo_intf_type & E_VO_INTERFACE_TYPE_MIPI) == 0)
            continue;

        if(vo_cfg[i].vo_output != E_VO_OUTPUT_TYPE_USER)
            continue;

        if(vo_cfg[i].wbc_type == E_VO_SAMPLE_WBC_SOURCE_TYPE_DEVICE)
            continue;

        sample_common_dsp_exit(0);
        break;
    }

exit1:
    sample_common_sys_exit();
exit0:
    return ret;
}

static vs_void_t sample_vo_index_usage()
{
#ifndef VS_ORION
    printf("\t 0) disp0:hdmi(1080p60)  +  disp1:none.\n");
    printf("\t 1) disp0:hdmi(1080p60)  +  disp1:cvbs(pal).\n");
    printf("\t 2) disp0:hdmi(1080p60)  +  disp1:mipi(user 1080x1920p60).\n");
    printf("\t 3) disp0:hdmi(1080p60)  +  disp1:bt1120(1080p60).\n");
    printf("\t 4) disp0:mipi(4k30 for special demo).\n");
    printf("\t 5) disp0:hdmi(1080p59.94)\n");
    printf("\t 6) disp0:hdmi + bt1120 (1080p60).\n");
	printf("\t 7) disp0:hdmi(1080p60) lowlatency, only for imx 415 60fps\n");
    printf("\t 8) disp0:none + wbc0:device.\n");
    printf("\t 9) disp0:hdmi(1080p60), layer0 dma pip\n");
#else
    printf("\t 0) disp0:mipi(user 1080x1920p60).\n");
    printf("\t 1) disp0:bt1120(1080p60).\n");
    printf("\t 2) disp0:bt1120(1080i60).\n");
    printf("\t 3) disp0:mipi(4k30 for special demo).\n");
    printf("\t 4) disp0:bt1120 + mipi (1080p60).\n");
    printf("\t 5) disp0:none + wbc0:device.\n");
    printf("\t 6) disp0:mipi(user 1080x1920p60), layer0 dma pip\n");
    printf("\t 7) disp0:bt1120(1080p30) lowlatency, only for imx 415 30fps\n");
#endif
}

vs_void_t sample_vo_usage(char *prog_name)
{
    vs_uint32_t i;
    vs_int32_t sensor_type_num = sample_common_vii_sensor_type_num_get();

    printf("Usage : %s <index> <sensor_type> [i2c_bus_id] [mipi_tx_board_type]\n", prog_name);
    printf("index:\n");
    sample_vo_index_usage();

    printf("sensor_type:\n");
    printf("\t vii(online)  --> vpp(offline, llc case is online) --> vo.\n");
    for (i = 0; i < sensor_type_num; i++) {
        printf("\t %d) %s.\n", i, sample_common_sensor_type_name_get(i));
    }

    printf("I2C Bus ID:\n");
    printf("\t 1(default), i2c id based on board, DDR4 bus_id is 1, LPDDR4 bus_id is 5.\n");

    printf("MIPI TX Board Type(0 for default):\n");
    for (i = 0; i < MIPITX_BOARD_TYPE_MAX; i++) {

#ifndef VS_ORION
        if (i == MIPITX_BOARD_VS8X9_CI02 || i == MIPITX_BOARD_VS909)
            continue;
#endif

        printf("\t %d) %s.\n", i, sample_common_mipitx_board_type_get(i));
    }
}

static vs_void_t sample_vo_register_signal_handler(void (*sig_handler)(int))
{
    struct sigaction sa;
    memset(&sa, 0, sizeof(struct sigaction));
    sa.sa_handler = sig_handler;
    sa.sa_flags = 0;

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
}

vs_void_t vo_signal_handle(vs_int32_t s_no)
{
    (vs_void_t)s_no;
    g_stop_flag = 1;
}

int main(int argc, char *argv[])
{
    vs_int32_t ret = VS_SUCCESS;
    vs_long_t case_idx = 0;
    vs_bool_t llc = VS_FALSE;
    vs_int32_t arg_val;

    g_auto_sleep_time_s = 0;
    g_dma_pip_en = VS_FALSE;
    g_layer0_chn_mode = E_VO_MODE_1MUX;
    g_layer3_chn_mode = VO_MODE_BUTT;
    g_disp0_intf = 1 << 31;
    g_disp1_intf = 1 << 31;
    g_disp0_output = E_VO_OUTPUT_TYPE_1080P60;
    g_disp0_wbc_type = E_VO_SAMPLE_WBC_DISABLE;
    g_disp1_output = E_VO_OUTPUT_TYPE_MAX;
    g_vb_cnt = 12;

    if (argc < 2) {
        sample_vo_usage(argv[0]);
        return VS_FAILED;
    }

    if (!strncmp(argv[1], "-h", 2)) {
        sample_vo_usage(argv[0]);
        return VS_FAILED;
    }

    sample_vo_register_signal_handler(vo_signal_handle);

    ret = sample_common_argv_val_get(argv, 1, &case_idx);
    if (ret != VS_SUCCESS) {
        sample_vo_usage(argv[0]);
        return ret;
    }

    if (case_idx == 7) {
        llc = VS_TRUE;
    }

    if (argc > 2) {
        ret = sample_common_get_valid_argv_val(argv, 2, &arg_val, 0, SAMPLE_SENSOR_TYPE_MAX - 1);
        if (ret != VS_SUCCESS) {
            sample_vo_usage(argv[0]);
            return ret;
        }

        if (llc && arg_val != SONY_IMX415_MIPI_8M_60FPS_12BIT && arg_val != SONY_IMX415_MIPI_8M_30FPS_12BIT) {
            vs_sample_trace("for llc case, only support imx 415 4k60 or 4k30\n");
            return VS_FAILED;
        }

        g_sensor_type[0] = arg_val;
    }

    if (argc > 3) {
        ret = sample_common_get_valid_argv_val(argv, 3, &arg_val, 0, INT8_MAX);
        if (ret != VS_SUCCESS) {
            sample_vo_usage(argv[0]);
            return ret;
        }

        g_bus_id[0] = arg_val;
    }

    if (argc > 4) {
        mipitx_board_setting_get(argv[4]);
    }

#ifndef VS_ORION
    switch (case_idx) {
    case 0:
        sample_vo_hdmi_none_case_get();
        break;
    case 1:
        sample_vo_hdmi_cvbs_case_get();
        break;
    case 2:
        sample_vo_hdmi_mipi_case_get();
        break;
    case 3:
        sample_vo_hdmi_bt1120_case_get();
        break;
    case 4:
        sample_vo_mipi_4k_case_get();
        break;
    case 5:
        sample_vo_hdmi_user_case_get();
        break;
    case 6:
        sample_vo_hdmi_bt1120_same_source_case_get();
        break;
    case 7:
        sample_vo_vii_vpp_vo_lowlatency_case_get();
        break;
    case 8:
        sample_vo_device_wbc_case_get();
        break;
    case 9:
        sample_vo_dma_pip_case_get();
        break;
    default:
        sample_vo_usage(argv[0]);
        return VS_FAILED;
    }
#else
    switch (case_idx) {
    case 0:
        sample_vo_mipi_case_get();
        break;
    case 1:
        sample_vo_bt1120p_case_get();
        break;
    case 2:
        sample_vo_bt1120i_case_get();
        break;
    case 3:
        sample_vo_mipi_4k_case_get();
        break;
    case 4:
        sample_vo_bt1120_mipi_same_source_case_get();
        break;
    case 5:
        sample_vo_device_wbc_case_get();
        break;
    case 6:
        sample_vo_dma_pip_case_get();
        break;
    case 7:
        sample_vo_vii_vpp_vo_lowlatency_case_get();
        break;
    default:
        sample_vo_usage(argv[0]);
        return VS_FAILED;
    }
#endif

    ret = sample_vo_vii_vpp_vo_venc_case(llc);

    return ret;
}
