/**
 * @file    sample_hdmi_path.c
 * @brief   sample hdmi implementation
 * @details
 * @author  Visinex Software Group
 * @date    2022-05-25
 * @version v1.00
 * @Copyright (c) 2022 Shanghai Visinex Technologies Co., Ltd. All rights reserved.
 *
 */

#include <string.h>
#include "sample_hdmi_path.h"

#define LAYER_BUF_DEPTH_DEF 3

typedef struct sample_hdmi_pipeline_cfg {
    vs_int32_t                  vdec_chnid;
    vs_int32_t                  vpp_grpid;
    vs_int32_t                  vpp_chnid;
    vs_int32_t                  vo_layerid;
    sample_vdec_cfg_s           vdec_config;
    sample_vdec_thread_param_s  vdec_thread_param;
    vs_bool_t                   vpp_chn_enable[VPP_MAX_PHYCHN_NUM];
    sample_vo_cfg_s             vo_config;
} sample_hdmi_pipeline_cfg_s;

static sample_hdmi_pipeline_cfg_s s_hdmi_pipeline = {0};

static vs_void_t sample_hdmi_vo_timing_compatible_convert(vs_hdmi_video_format_e hdmi_output,
        vs_vo_dev_attr_s *dev_attr)
{
    vs_vo_output_type_e *p_vo_output = &dev_attr->interface_output;

    switch (hdmi_output) {
    case E_HDMI_VIDEO_FORMAT_1080P_60:
        *p_vo_output = E_VO_OUTPUT_TYPE_1080P60;
        dev_attr->timing_info.hactive = 1920;
        dev_attr->timing_info.vactive = 1080;
        break;
    case E_HDMI_VIDEO_FORMAT_1080P_50:
        *p_vo_output = E_VO_OUTPUT_TYPE_1080P50;
        dev_attr->timing_info.hactive = 1920;
        dev_attr->timing_info.vactive = 1080;
        break;
    case E_HDMI_VIDEO_FORMAT_1080P_30:
        *p_vo_output = E_VO_OUTPUT_TYPE_1080P30;
        dev_attr->timing_info.hactive = 1920;
        dev_attr->timing_info.vactive = 1080;
        break;
    case E_HDMI_VIDEO_FORMAT_1080P_25:
        *p_vo_output = E_VO_OUTPUT_TYPE_1080P25;
        dev_attr->timing_info.hactive = 1920;
        dev_attr->timing_info.vactive = 1080;
        break;
    case E_HDMI_VIDEO_FORMAT_1080P_24:
        *p_vo_output = E_VO_OUTPUT_TYPE_1080P24;
        dev_attr->timing_info.hactive = 1920;
        dev_attr->timing_info.vactive = 1080;
        break;
    case E_HDMI_VIDEO_FORMAT_1080I_60:
        *p_vo_output = E_VO_OUTPUT_TYPE_1080I60;
        dev_attr->timing_info.hactive = 1920;
        dev_attr->timing_info.vactive = 1080;
        break;
    case E_HDMI_VIDEO_FORMAT_1080I_50:
        *p_vo_output = E_VO_OUTPUT_TYPE_1080I50;
        dev_attr->timing_info.hactive = 1920;
        dev_attr->timing_info.vactive = 1080;
        break;
    case E_HDMI_VIDEO_FORMAT_720P_60:
        *p_vo_output = E_VO_OUTPUT_TYPE_720P60;
        dev_attr->timing_info.hactive = 1280;
        dev_attr->timing_info.vactive = 720;
        break;
    case E_HDMI_VIDEO_FORMAT_720P_50:
        *p_vo_output = E_VO_OUTPUT_TYPE_720P50;
        dev_attr->timing_info.hactive = 1280;
        dev_attr->timing_info.vactive = 720;
        break;
    case E_HDMI_VIDEO_FORMAT_576P_50:
        *p_vo_output = E_VO_OUTPUT_TYPE_576P50;
        dev_attr->timing_info.hactive = 720;
        dev_attr->timing_info.vactive = 576;
        break;
    case E_HDMI_VIDEO_FORMAT_480P_60:
        *p_vo_output = E_VO_OUTPUT_TYPE_480P60;
        dev_attr->timing_info.hactive = 720;
        dev_attr->timing_info.vactive = 480;
        break;
    case E_HDMI_VIDEO_FORMAT_640x480_60:
        *p_vo_output = E_VO_OUTPUT_TYPE_640x480_60;
        dev_attr->timing_info.hactive = 640;
        dev_attr->timing_info.vactive = 480;
        break;
    case E_HDMI_VIDEO_FORMAT_720x1280_60:
        *p_vo_output = E_VO_OUTPUT_TYPE_720x1280_60;
        dev_attr->timing_info.hactive = 720;
        dev_attr->timing_info.vactive = 1280;
        break;
    case E_HDMI_VIDEO_FORMAT_800x600_50:
        *p_vo_output = E_VO_OUTPUT_TYPE_800x600_50;
        dev_attr->timing_info.hactive = 800;
        dev_attr->timing_info.vactive = 600;
        break;
    case E_HDMI_VIDEO_FORMAT_800x600_60:
        *p_vo_output = E_VO_OUTPUT_TYPE_800x600_60;
        dev_attr->timing_info.hactive = 800;
        dev_attr->timing_info.vactive = 600;
        break;
    case E_HDMI_VIDEO_FORMAT_1024x768_60:
        *p_vo_output = E_VO_OUTPUT_TYPE_1024x768_60;
        dev_attr->timing_info.hactive = 1024;
        dev_attr->timing_info.vactive = 768;
        break;
    case E_HDMI_VIDEO_FORMAT_1080x1920_60:
        *p_vo_output = E_VO_OUTPUT_TYPE_1080x1920_60;
        dev_attr->timing_info.hactive = 1080;
        dev_attr->timing_info.vactive = 1920;
        break;
    case E_HDMI_VIDEO_FORMAT_1280x800_60:
        *p_vo_output = E_VO_OUTPUT_TYPE_1080P30;
        dev_attr->timing_info.hactive = 1280;
        dev_attr->timing_info.vactive = 800;
        break;
    case E_HDMI_VIDEO_FORMAT_1280x1024_60:
        *p_vo_output = E_VO_OUTPUT_TYPE_1280x1024_60;
        dev_attr->timing_info.hactive = 1280;
        dev_attr->timing_info.vactive = 1024;
        break;
    case E_HDMI_VIDEO_FORMAT_1366x768_60:
        *p_vo_output = E_VO_OUTPUT_TYPE_1366x768_60;
        dev_attr->timing_info.hactive = 1366;
        dev_attr->timing_info.vactive = 768;
        break;
    case E_HDMI_VIDEO_FORMAT_1440x900_60:
        *p_vo_output = E_VO_OUTPUT_TYPE_1440x900_60;
        dev_attr->timing_info.hactive = 1440;
        dev_attr->timing_info.vactive = 900;
        break;
    case E_HDMI_VIDEO_FORMAT_1600x1200_60:
        *p_vo_output = E_VO_OUTPUT_TYPE_1600x1200_60;
        dev_attr->timing_info.hactive = 1600;
        dev_attr->timing_info.vactive = 1200;
        break;
    case E_HDMI_VIDEO_FORMAT_1680x1050_60:
        *p_vo_output = E_VO_OUTPUT_TYPE_1680x1050_60;
        dev_attr->timing_info.hactive = 1680;
        dev_attr->timing_info.vactive = 1050;
        break;
    case E_HDMI_VIDEO_FORMAT_1920x1200_60:
        *p_vo_output = E_VO_OUTPUT_TYPE_1920x1200_60;
        dev_attr->timing_info.hactive = 1920;
        dev_attr->timing_info.vactive = 1200;
        break;
    case E_HDMI_VIDEO_FORMAT_2560x1440_30:
        *p_vo_output = E_VO_OUTPUT_TYPE_2560x1440_30;
        dev_attr->timing_info.hactive = 2560;
        dev_attr->timing_info.vactive = 1440;
        break;
    case E_HDMI_VIDEO_FORMAT_2560x1440_60:
        *p_vo_output = E_VO_OUTPUT_TYPE_2560x1440_60;
        dev_attr->timing_info.hactive = 2560;
        dev_attr->timing_info.vactive = 1440;
        break;
    case E_HDMI_VIDEO_FORMAT_2560x1600_60:
        *p_vo_output = E_VO_OUTPUT_TYPE_2560x1600_60;
        dev_attr->timing_info.hactive = 2560;
        dev_attr->timing_info.vactive = 1600;
        break;
    case E_HDMI_VIDEO_FORMAT_1920x2160_30:
        *p_vo_output = E_VO_OUTPUT_TYPE_1920x2160_30;
        break;
    case E_HDMI_VIDEO_FORMAT_3840x2160P_24:
        *p_vo_output = E_VO_OUTPUT_TYPE_3840x2160_24;
        dev_attr->timing_info.hactive = 3840;
        dev_attr->timing_info.vactive = 2160;
        break;
    case E_HDMI_VIDEO_FORMAT_3840x2160P_25:
        *p_vo_output = E_VO_OUTPUT_TYPE_3840x2160_25;
        dev_attr->timing_info.hactive = 3840;
        dev_attr->timing_info.vactive = 2160;
        break;
    case E_HDMI_VIDEO_FORMAT_3840x2160P_30:
        *p_vo_output = E_VO_OUTPUT_TYPE_3840x2160_30;
        dev_attr->timing_info.hactive = 3840;
        dev_attr->timing_info.vactive = 2160;
        break;
    case E_HDMI_VIDEO_FORMAT_3840x2160P_50:
        *p_vo_output = E_VO_OUTPUT_TYPE_3840x2160_50;
        dev_attr->timing_info.hactive = 3840;
        dev_attr->timing_info.vactive = 2160;
        break;
    case E_HDMI_VIDEO_FORMAT_3840x2160P_60:
        *p_vo_output = E_VO_OUTPUT_TYPE_3840x2160_60;
        dev_attr->timing_info.hactive = 3840;
        dev_attr->timing_info.vactive = 2160;
        break;
    case E_HDMI_VIDEO_FORMAT_4096x2160P_24:
        *p_vo_output = E_VO_OUTPUT_TYPE_4096x2160_24;
        dev_attr->timing_info.hactive = 4096;
        dev_attr->timing_info.vactive = 2160;
        break;
    case E_HDMI_VIDEO_FORMAT_4096x2160P_25:
        *p_vo_output = E_VO_OUTPUT_TYPE_4096x2160_25;
        dev_attr->timing_info.hactive = 4096;
        dev_attr->timing_info.vactive = 2160;
        break;
    case E_HDMI_VIDEO_FORMAT_4096x2160P_30:
        *p_vo_output = E_VO_OUTPUT_TYPE_4096x2160_30;
        dev_attr->timing_info.hactive = 4096;
        dev_attr->timing_info.vactive = 2160;
        break;
    case E_HDMI_VIDEO_FORMAT_4096x2160P_50:
        *p_vo_output = E_VO_OUTPUT_TYPE_4096x2160_50;
        dev_attr->timing_info.hactive = 4096;
        dev_attr->timing_info.vactive = 2160;
        break;
    case E_HDMI_VIDEO_FORMAT_4096x2160P_60:
        *p_vo_output = E_VO_OUTPUT_TYPE_4096x2160_60;
        dev_attr->timing_info.hactive = 4096;
        dev_attr->timing_info.vactive = 2160;
        break;
    default:
        *p_vo_output = E_VO_OUTPUT_TYPE_USER;
        break;
	}
}

static vs_int32_t sample_hdmi_plug_in(vs_void_t *p_private_date)
{
    vs_int32_t ret;
    vs_hdmi_attr_s hdmi_attr;
    vs_hdmi_sink_cap_s hdmi_cap;

    ret = vs_mal_hdmi_sink_cap_get(0, &hdmi_cap);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_hdmi_sink_cap_get failed with 0x%x\n", ret);
    }

    ret = vs_mal_hdmi_attr_get(0, &hdmi_attr);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_hdmi_attr_get failed with 0x%x\n", ret);
        return VS_FAILED;
    }

    ret = vs_mal_hdmi_attr_set(0, &hdmi_attr);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_hdmi_attr_set failed with 0x%x\n", ret);
        return VS_FAILED;
    }

    ret = vs_mal_hdmi_start(0);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_hdmi_start failed with 0x%x\n", ret);
        return VS_FAILED;
    }

    return VS_SUCCESS;
}

static vs_int32_t sample_hdmi_plug_out(vs_void_t *p_private_date)
{
    vs_int32_t ret;

    ret = vs_mal_hdmi_stop(0);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_hdmi_stop failed with 0x%x\n", ret);
        return VS_FAILED;
    }

    return VS_SUCCESS;
}

void sample_hdmi_cb_event(vs_hdmi_event_type_e event, vs_void_t *p_private_date)
{
    switch (event) {
        case E_HDMI_EVENT_HOTPLUG_IN:
            sample_hdmi_plug_in(p_private_date);
            break;
        case E_HDMI_EVENT_HOTPLUG_OUT:
            sample_hdmi_plug_out(p_private_date);
            break;
        default:
            break;
    }
    vs_sample_trace("sample_hdmi_hotplug_cb, with event %d\n", event);
}

vs_int32_t sample_hdmi_hdmi_start(vs_hdmi_video_format_e video_put)
{
    vs_hdmi_attr_s hdmi_attr;
    vs_hdmi_sink_cap_s hdmi_cap;
    vs_int32_t ret;
    vs_hdmi_callback_s p_callback = {
        .p_hdmi_event_callback = sample_hdmi_cb_event,
        .p_private_date = NULL,
    };

    ret = vs_mal_hdmi_init();
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_hdmi_init failed with 0x%x\n", ret);
        return VS_FAILED;
    }

    ret = vs_mal_hdmi_open(0);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_hdmi_open failed with 0x%x\n", ret);
        return VS_FAILED;
    }

    ret = vs_mal_hdmi_callback_register(0, &p_callback);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_hdmi_callback_register failed with 0x%x\n", ret);
        return VS_FAILED;
    }

    ret = vs_mal_hdmi_sink_cap_get(0, &hdmi_cap);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_hdmi_sink_cap_get failed with 0x%x\n", ret);
    }

    ret = vs_mal_hdmi_attr_get(0, &hdmi_attr);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_hdmi_attr_get failed with 0x%x\n", ret);
        return VS_FAILED;
    }

    hdmi_attr.hdmi_enable = VS_TRUE;
    hdmi_attr.video_format = video_put;
    hdmi_attr.deep_color_mode = E_HDMI_DEEP_COLOR_24BIT;
    hdmi_attr.audio_enable = VS_TRUE;
    hdmi_attr.audio_sample_rate = E_HDMI_SAMPLE_RATE_48K;
    hdmi_attr.audio_bit_depth =  E_HDMI_BIT_DEPTH_16;

    ret = vs_mal_hdmi_attr_set(0, &hdmi_attr);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_hdmi_attr_set failed with 0x%x\n", ret);
        return VS_FAILED;
    }

    ret = vs_mal_hdmi_start(0);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_hdmi_start failed with 0x%x\n", ret);
        return VS_FAILED;
    }

    vs_sample_trace("hdmi start success\n");
    return VS_SUCCESS;
}

vs_int32_t sample_hdmi_hdmi_stop()
{
    vs_int32_t ret;

    ret = vs_mal_hdmi_stop(0);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_hdmi_stop failed with 0x%x\n", ret);
        return VS_FAILED;
    }

    ret = vs_mal_hdmi_deinit();
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_hdmi_stop failed with 0x%x\n", ret);
        return VS_FAILED;
    }

    return VS_SUCCESS;
}

static vs_int32_t sample_hdmi_sys_init(vs_size_s input_size)
{
    vs_int32_t ret = VS_SUCCESS;
    vs_vb_cfg_s vb_cfg = {0};

    vb_cfg.pool_cnt = 1;
    vb_cfg.ast_commpool[0].blk_size = STRIDE_UP(input_size.width * input_size.height * 2, 4096);
    vb_cfg.ast_commpool[0].blk_cnt = 6;
    vb_cfg.ast_commpool[0].remap_mode = VB_REMAP_MODE_NONE;
    ret = sample_common_sys_init(&vb_cfg);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("sample_common_sys_init failed, ret[%d]\n", ret);
        return ret;
    }
    return VS_SUCCESS;
}

static vs_int32_t sample_hdmi_vpp_init(vs_int32_t vpp_grpid, vs_size_s input_size, vs_bool_t *p_chn_enable)
{
    vs_int32_t i = 0, ret = VS_SUCCESS;
    vs_vpp_grp_attr_s vpp_grp_attr;
    vs_vpp_chn_attr_s vpp_chn_attr[VPP_MAX_PHYCHN_NUM];

    memset(&vpp_grp_attr, 0, sizeof(vpp_grp_attr));
    vpp_grp_attr.max_width = input_size.width;
	vpp_grp_attr.max_height = input_size.height;
	vpp_grp_attr.dynamic_range = E_DYNAMIC_RANGE_SDR8;
	vpp_grp_attr.pixel_format = E_PIXEL_FORMAT_YVU_420SP;
	vpp_grp_attr.framerate.dst_framerate = -1;
	vpp_grp_attr.framerate.src_framerate = -1;

    memset(&vpp_chn_attr, 0, sizeof(vpp_chn_attr));
    for (i = 0; i < VPP_MAX_PHYCHN_NUM; i++) {
        if (p_chn_enable[i] == VS_TRUE) {
            vpp_chn_attr[i].chn_mode = E_VPP_CHN_MODE_AUTO;
            vpp_chn_attr[i].width = input_size.width;
            vpp_chn_attr[i].height = input_size.height;
            vpp_chn_attr[i].video_format = E_VIDEO_FORMAT_LINEAR;
            vpp_chn_attr[i].pixel_format = E_PIXEL_FORMAT_YUV_420SP;
            vpp_chn_attr[i].dynamic_range = E_DYNAMIC_RANGE_SDR8;
            vpp_chn_attr[i].compress_mode = E_COMPRESS_MODE_RASTER;
            vpp_chn_attr[i].framerate.src_framerate = -1;
            vpp_chn_attr[i].framerate.dst_framerate = -1;
            vpp_chn_attr[i].mirror_enable = VS_FALSE;
            vpp_chn_attr[i].flip_enable = VS_FALSE;
            vpp_chn_attr[i].depth = 0;
            vpp_chn_attr[i].aspect_ratio.mode = E_ASPECT_RATIO_MODE_NONE;
        }
    }
    ret = sample_common_vpp_start(vpp_grpid, p_chn_enable, &vpp_grp_attr, vpp_chn_attr);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("sample_common_vpp_start failed, ret[%d]\n", ret);
    }
    return ret;
}

static vs_void_t sample_hdmi_vo_size_convert(vs_vo_output_type_e vo_output, vs_size_s *p_disp_size)
{
    switch (vo_output) {
        case E_VO_OUTPUT_TYPE_3840x2160_25:
        case E_VO_OUTPUT_TYPE_3840x2160_30:
        case E_VO_OUTPUT_TYPE_3840x2160_50:
        case E_VO_OUTPUT_TYPE_3840x2160_60:
            p_disp_size->width = 3840;
            p_disp_size->height = 2160;
            break;

        case E_VO_OUTPUT_TYPE_1080P25:
        case E_VO_OUTPUT_TYPE_1080P30:
        case E_VO_OUTPUT_TYPE_1080P50:
        case E_VO_OUTPUT_TYPE_1080P60:
        case E_VO_OUTPUT_TYPE_1080I60:
        default:
            p_disp_size->width = 1920;
            p_disp_size->height = 1080;
            break;
    }
}

vs_int32_t sample_hdmi_vo_start(sample_vo_cfg_s* p_vo_config)
{
    vs_int32_t ret;
    vs_vo_clk_info_s clk;
    vs_vo_dev_attr_s dev_attr;

    dev_attr.bg_color = p_vo_config->bg_color;
    dev_attr.interface_output = p_vo_config->vo_output;
    dev_attr.interface_type = p_vo_config->vo_intf_type;

    if (p_vo_config->vo_intf_type == E_VO_INTERFACE_TYPE_MIPI) {
        sample_common_vo_timing_get(&dev_attr, &clk);
    }

    if (dev_attr.interface_output == E_VO_OUTPUT_TYPE_USER) {
        vs_mal_vo_clk_set(p_vo_config->vo_devid, &clk);
    }

    ret = vs_mal_vo_dev_attr_set(p_vo_config->vo_devid, &dev_attr);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_vo_dev_attr_set(vo_devid %d) failed with 0x%x\n", p_vo_config->vo_devid, ret);
        return VS_FAILED;
    }

    ret = vs_mal_vo_enable(p_vo_config->vo_devid);
    if (ret) {
        vs_sample_trace("vs_mal_vo_enable(vo_devid %d) failed, 0x%x\n", p_vo_config->vo_devid, ret);
        return VS_FAILED;
    }

    ret = sample_common_vo_layer_enable(p_vo_config);
    if (ret != VS_SUCCESS) {
        return VS_FAILED;
    }

    ret = vs_mal_vo_commit(p_vo_config->vo_devid);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_vo_commit(vo_devid %d) failed with 0x%x\n", p_vo_config->vo_devid, ret);
        return VS_FAILED;
    }

    return ret;
}

static vs_int32_t sample_hdmi_vo_init(sample_vo_cfg_s *p_vo_config, vs_int32_t        vo_layerid,
    vs_vo_output_type_e vo_output)
{
    vs_int32_t ret = VS_SUCCESS;
    vs_size_s disp_size = {0};

    sample_hdmi_vo_size_convert(vo_output, &disp_size);

    p_vo_config->vo_devid = 0;
    p_vo_config->vo_layerid = vo_layerid;
    p_vo_config->vo_output = vo_output;
    p_vo_config->bg_color = 0;
    p_vo_config->dynamic_range = E_DYNAMIC_RANGE_SDR8;
    p_vo_config->pixel_format = E_PIXEL_FORMAT_YUV_420SP;
    p_vo_config->vo_mode = E_VO_MODE_1MUX;
    p_vo_config->img_width = disp_size.width;
    p_vo_config->img_height = disp_size.height;
    p_vo_config->zorder = 0;
    p_vo_config->vo_intf_type = E_VO_INTERFACE_TYPE_HDMI;

    ret =  sample_hdmi_vo_start(p_vo_config);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("sample_common_vo_start failed, ret[%d]\n", ret);
    }

    return ret;
}

vs_int32_t sample_hdmi_vdec_vpss_vo_start(vs_hdmi_video_format_e format)
{
    vs_int32_t ret = VS_SUCCESS;
    vs_size_s vdec_size = {3840, 2160};
    vs_vo_dev_attr_s dev_attr;
    vs_vo_output_type_e vo_output = 0;

    sample_hdmi_vo_timing_compatible_convert(format, &dev_attr);
    if (dev_attr.interface_output == E_VO_OUTPUT_TYPE_USER) {
        vs_sample_trace("unsupported for video format [%d]\n", format);
        return VS_FAILED;
    }
    vo_output = dev_attr.interface_output;

    ret = sample_hdmi_sys_init(vdec_size);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("sample_hdmi_sys_init failed, ret[%d]\n", ret);
        return ret;
    }

    memset(&s_hdmi_pipeline, 0, sizeof(sample_hdmi_pipeline_cfg_s));

    s_hdmi_pipeline.vdec_chnid                   = 0;
    s_hdmi_pipeline.vdec_config.vdec_chnid       = s_hdmi_pipeline.vdec_chnid;
    s_hdmi_pipeline.vdec_config.input_mode       = E_VDEC_INPUT_MODE_FRAME;
    s_hdmi_pipeline.vdec_config.decode_type      = E_PT_TYPE_H265;
    s_hdmi_pipeline.vdec_config.pixel_format     = E_PIXEL_FORMAT_YUV_420SP;
    s_hdmi_pipeline.vdec_config.compress_mode    = E_COMPRESS_MODE_RASTER;
    s_hdmi_pipeline.vdec_config.video_format     = E_VIDEO_FORMAT_LINEAR;
    s_hdmi_pipeline.vdec_config.width            = vdec_size.width;
    s_hdmi_pipeline.vdec_config.height           = vdec_size.height;
    s_hdmi_pipeline.vdec_config.vb_poolid        = VS_INVALID_POOLID;
    s_hdmi_pipeline.vdec_config.frame_buf_size   = 0;
    s_hdmi_pipeline.vdec_config.ref_frame_num    = 2;
    s_hdmi_pipeline.vdec_config.output_frame_num = 2;
    s_hdmi_pipeline.vdec_config.frame_buf_cnt    = s_hdmi_pipeline.vdec_config.ref_frame_num
                                                    + s_hdmi_pipeline.vdec_config.output_frame_num + 1;

    ret = sample_common_vdec_vb_pool_init(1, &s_hdmi_pipeline.vdec_config);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("sample_common_vdec_vb_pool_init failed, ret[%d]\n", ret);
        goto exit_sys_exit;
    }

    ret = sample_common_vdec_start(1, &s_hdmi_pipeline.vdec_config);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("sample_common_vdec_start failed, ret[%d]\n", ret);
        goto exit_vdec_vb_pool_deinit;
    }

    s_hdmi_pipeline.vpp_grpid = 0;
    s_hdmi_pipeline.vpp_chnid = 0;
    s_hdmi_pipeline.vpp_chn_enable[s_hdmi_pipeline.vpp_chnid] = VS_TRUE;
    ret = sample_hdmi_vpp_init(s_hdmi_pipeline.vpp_grpid, vdec_size, s_hdmi_pipeline.vpp_chn_enable);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("sample_common_vpp_start failed, ret[%d]\n", ret);
        goto exit_vdec_stop;
    }

    ret = sample_common_vdec_bind_vpp(0, s_hdmi_pipeline.vdec_chnid, s_hdmi_pipeline.vpp_grpid);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("sample_common_vdec_bind_vpp failed, ret[%d]\n", ret);
        goto exit_vpp_stop;
    }

    s_hdmi_pipeline.vo_layerid = 0;
    ret = sample_hdmi_vo_init(&s_hdmi_pipeline.vo_config, s_hdmi_pipeline.vo_layerid, vo_output);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("sample_hdmi_vo_init failed, ret[%d]\n", ret);
        goto exit_vdec_unbind_vpp;
    }

    ret = sample_common_vpp_bind_vo(s_hdmi_pipeline.vpp_grpid, s_hdmi_pipeline.vpp_chnid,
        s_hdmi_pipeline.vo_layerid, 0);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("sample_common_vpp_bind_vo failed, ret[%d]\n", ret);
        goto exit_vo_stop;
    }

    s_hdmi_pipeline.vdec_thread_param.vdec_chnid = s_hdmi_pipeline.vdec_chnid;
    s_hdmi_pipeline.vdec_thread_param.width = vdec_size.width;
    s_hdmi_pipeline.vdec_thread_param.height = vdec_size.height;
    s_hdmi_pipeline.vdec_thread_param.input_mode = s_hdmi_pipeline.vdec_config.input_mode;
    s_hdmi_pipeline.vdec_thread_param.decode_type = s_hdmi_pipeline.vdec_config.decode_type;
    s_hdmi_pipeline.vdec_thread_param.send_timeout_ms = 200;
    snprintf((char *)s_hdmi_pipeline.vdec_thread_param.input_file, MAX_FILE_PATH_LEN, "hdmi/chn0_3840x2160.h265");
    s_hdmi_pipeline.vdec_thread_param.is_circle_send = VS_TRUE;
    s_hdmi_pipeline.vdec_thread_param.stop_send_task = VS_FALSE;
    s_hdmi_pipeline.vdec_thread_param.tid_send = 0;
    sample_common_vdec_stream_send_start(1, &s_hdmi_pipeline.vdec_thread_param);

    return VS_SUCCESS;

exit_vo_stop:
    vs_sample_trace("exit_vo_stop.\n");
    sample_common_vo_stop(&s_hdmi_pipeline.vo_config);
exit_vdec_unbind_vpp:
    vs_sample_trace("exit_vdec_unbind_vpp.\n");
    sample_common_vdec_unbind_vpp(0, s_hdmi_pipeline.vdec_chnid, s_hdmi_pipeline.vpp_grpid);
exit_vpp_stop:
    vs_sample_trace("exit_vpp_stop.\n");
    sample_common_vpp_stop(s_hdmi_pipeline.vpp_grpid, s_hdmi_pipeline.vpp_chn_enable);
exit_vdec_stop:
    vs_sample_trace("exit_vdec_vdec_stop.\n");
    sample_common_vdec_stop(1, &s_hdmi_pipeline.vdec_config);
exit_vdec_vb_pool_deinit:
    vs_sample_trace("exit_vdec_vb_pool_deinit.\n");
    sample_common_vdec_vb_pool_deinit(1, &s_hdmi_pipeline.vdec_config);
exit_sys_exit:
    vs_sample_trace("exit_sys_exit.\n");
    sample_common_sys_exit();

    return VS_FAILED;
}

vs_void_t sample_hdmi_vdec_vpss_vo_stop(vs_void_t)
{
    vs_sample_trace("exit_vdec_stream_send.\n");
    sample_common_vdec_stream_send_stop(1, &s_hdmi_pipeline.vdec_thread_param);

    vs_sample_trace("exit_vpp_unbind_vo.\n");
    sample_common_vpp_unbind_vo(s_hdmi_pipeline.vpp_grpid, s_hdmi_pipeline.vpp_chnid, s_hdmi_pipeline.vo_layerid, 0);

    vs_sample_trace("exit_vo_stop.\n");
    sample_common_vo_stop(&s_hdmi_pipeline.vo_config);

    vs_sample_trace("exit_vdec_unbind_vpp.\n");
    sample_common_vdec_unbind_vpp(0, s_hdmi_pipeline.vdec_chnid, s_hdmi_pipeline.vpp_grpid);

    vs_sample_trace("exit_vpp_stop.\n");
    sample_common_vpp_stop(s_hdmi_pipeline.vpp_grpid, s_hdmi_pipeline.vpp_chn_enable);

    vs_sample_trace("exit_vdec_vdec_stop.\n");
    sample_common_vdec_stop(1, &s_hdmi_pipeline.vdec_config);

    vs_sample_trace("exit_vdec_vb_pool_deinit.\n");
    sample_common_vdec_vb_pool_deinit(1, &s_hdmi_pipeline.vdec_config);

    vs_sample_trace("exit_sys_exit.\n");
    sample_common_sys_exit();
}

vs_int32_t sample_hdmi_vo_stop(sample_vo_cfg_s* p_vo_config)
{
    vs_int32_t vo_devid = p_vo_config->vo_devid;
    vs_int32_t vo_layerid = p_vo_config->vo_layerid;

    vs_mal_vo_chn_disable(vo_layerid, 0);
    vs_mal_vo_video_layer_unbind(vo_layerid, vo_devid);
    vs_mal_vo_video_layer_disable(vo_layerid);
    vs_mal_vo_disable(vo_devid);
    vs_mal_vo_commit(vo_devid);

    return VS_SUCCESS;
}

vs_int32_t sample_hdmi_vo_layer_enable(sample_vo_cfg_s* p_vo_config, vs_vo_dev_attr_s dev_attr)
{
    vs_int32_t ret;
    vs_vo_video_layer_attr_s attr;

    attr.layer_framerate = 0;
    attr.dynamic_range = p_vo_config->dynamic_range;
    attr.format = p_vo_config->pixel_format;
    p_vo_config->img_width = dev_attr.timing_info.hactive;
    p_vo_config->img_height = dev_attr.timing_info.vactive;
    attr.image_size.width = p_vo_config->img_width;
    attr.image_size.height = p_vo_config->img_height;

    attr.dest_rect.x = 0;
    attr.dest_rect.y = 0;
    attr.dest_rect.width = attr.image_size.width;
    attr.dest_rect.height = attr.image_size.height;

    ret = vs_mal_vo_video_layer_attr_set(p_vo_config->vo_layerid, &attr);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_vo_video_layer_attr_set(vo_layerid %d) failed with 0x%x\n", p_vo_config->vo_layerid, ret);
        return VS_FAILED;
    }
    ret = vs_mal_vo_video_layer_bind(p_vo_config->vo_layerid, p_vo_config->vo_devid);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_vo_video_layer_bind(vo_devid %d  vo_layerid %d) failed with 0x%x\n", p_vo_config->vo_devid, p_vo_config->vo_layerid, ret);
        return VS_FAILED;
    }

    ret = vs_mal_vo_video_layer_buf_depth_set(p_vo_config->vo_layerid, LAYER_BUF_DEPTH_DEF);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_vo_video_layer_enable(vo_layerid %d) failed with 0x%x\n", p_vo_config->vo_layerid, ret);
        return VS_FAILED;
    }

    ret = vs_mal_vo_video_layer_enable(p_vo_config->vo_layerid);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_vo_video_layer_enable(vo_layerid %d) failed with 0x%x\n", p_vo_config->vo_layerid, ret);
        return VS_FAILED;
}

ret = sample_common_vo_chn_enable(p_vo_config);

return ret;
}

vs_int32_t sample_hdmi_vo_fromat_change(vs_hdmi_video_format_e format)
{
    vs_int32_t ret;
    vs_vo_dev_attr_s dev_attr;
    sample_vo_cfg_s *p_vo_config = &s_hdmi_pipeline.vo_config;

    dev_attr.bg_color = p_vo_config->bg_color;
    dev_attr.interface_type = p_vo_config->vo_intf_type;

    sample_hdmi_vo_timing_compatible_convert(format, &dev_attr);
    if (dev_attr.interface_output == E_VO_OUTPUT_TYPE_USER) {
        vs_sample_trace("unsupported for video format [%d]\n", format);
        return VS_FAILED;
    }

    ret = sample_hdmi_vo_stop(p_vo_config);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("sample_hdmi_vo_stop failed with %x\n", ret);
        return VS_FAILED;
    }

    ret = vs_mal_vo_dev_attr_set(p_vo_config->vo_devid, &dev_attr);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_vo_dev_attr_set(vo_devid %d) failed with 0x%x\n", p_vo_config->vo_devid, ret);
        return VS_FAILED;
    }

    ret = vs_mal_vo_enable(p_vo_config->vo_devid);
    if (ret) {
        vs_sample_trace("vs_mal_vo_enable(vo_devid %d) failed with 0x%x\n", p_vo_config->vo_devid, ret);
        return VS_FAILED;
    }

    ret = sample_hdmi_vo_layer_enable(p_vo_config, dev_attr);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("sample_hdmi_vo_layer_enable(vo_devid %d) failed with 0x%x\n", p_vo_config->vo_devid, ret);
        return VS_FAILED;
    }

    ret = vs_mal_vo_commit(p_vo_config->vo_devid);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_vo_commit(vo_devid %d) failed with 0x%x\n", p_vo_config->vo_devid, ret);
        return VS_FAILED;
    }

    return VS_SUCCESS;
}

vs_int32_t sample_hdmi_audio_start(vs_int32_t sample_rate)
{
    vs_int32_t ao_devid = 0;
    vs_aiodev_attr_s  aio_common_attr;
    vs_aiodev_attr_s *p_aio_attr=&aio_common_attr;
    vs_aio_target_e   i2s_target = E_AIO_CONNECT_INNER_CODEC;
    FILE *fp = NULL;

    fp = fopen("./hdmi/song.wav", "rb");
    if (!fp) {
        vs_sample_trace("file<./hdmi/song.wav> does not exist\n");
        return VS_FAILED;
    }

    memset(&aio_common_attr, 0, sizeof(aio_common_attr));
    aio_common_attr.sample_rate      = sample_rate;
    aio_common_attr.chn_num          = 2;
    aio_common_attr.i2s_target       = i2s_target;
    aio_common_attr.work_mode        = E_AIO_MODE_I2S_MASTER_STD;
    aio_common_attr.sample_width     = E_AUDIO_SAMPLE_BITWIDTH_16;
    aio_common_attr.sound_mode       = E_AUDIO_SOUND_MODE_STEREO;
    aio_common_attr.pool_frame_num   = 16;
    aio_common_attr.frame_sample_num = 1024;
    aio_common_attr.clk_source       = 0;

    p_aio_attr->i2s_target = E_AIO_CONNECT_INNER_HDMI;
    sample_common_aout_start(ao_devid, p_aio_attr, sample_rate == 48000 ? -1 : 48000, NULL);
    sample_common_aout_sink_thread_create(ao_devid, 0, p_aio_attr, fp);

    return VS_SUCCESS;
}

vs_void_t sample_hdmi_audio_stop(vs_void_t)
{
    vs_int32_t ao_devid = 0;

    sample_common_aout_sink_thread_destroy(ao_devid, 0);

    sample_common_aout_stop(ao_devid, 2, VS_FALSE);
}