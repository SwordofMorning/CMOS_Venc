/**
* @file sample_common_vo.c
* @brief implement the common functions of vo example.
* @details
* @author Visinex Software Group
* @date 2022-05-25
* @version v1.00
* @Copyright (C) 2022, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

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

#define align_down(x, a)    (((x) / (a)) * (a))
#define LAYER_BUF_DEPTH_DEF 3
#define CHN_BUF_DEPTH_DEF   5
vs_vo_output_type_e g_output_type;
#ifndef VS_ORION
static vs_vo_interface_type_e g_hdmi_common_vo_intf_type;
static vs_vo_timing_s g_hdmi_timing_info = {0};
static vs_vo_clk_info_s g_hdmi_clk_info = {0};
#endif

static vs_int32_t vo_output_type_get(enum sample_vo_output_intf_type type, sample_vo_cfg_s *vo_config)
{
    switch (type) {
    case E_VO_OUTPUT_INTF_TYPE_BT1120_PROGRESSIVE:
        vo_config->vo_intf_type = E_VO_INTERFACE_TYPE_BT1120;
        vo_config->vo_output = E_VO_OUTPUT_TYPE_1080P60;
        break;
    case E_VO_OUTPUT_INTF_TYPE_BT1120_INTERLACE:
        vo_config->vo_intf_type = E_VO_INTERFACE_TYPE_BT1120;
        vo_config->vo_output = E_VO_OUTPUT_TYPE_1080I60;
        break;
    case E_VO_OUTPUT_INTF_TYPE_CVBS:
        vo_config->vo_intf_type = E_VO_INTERFACE_TYPE_CVBS;
        vo_config->vo_output = E_VO_OUTPUT_TYPE_PAL;
        //vo_config->vo_output = E_VO_OUTPUT_TYPE_NTSC;
        break;
    case E_VO_OUTPUT_INTF_TYPE_MIPI_DSI_TX:
        vo_config->vo_intf_type = E_VO_INTERFACE_TYPE_MIPI;
        vo_config->vo_output = E_VO_OUTPUT_TYPE_USER;
        break;
    default:
        vo_config->vo_intf_type = E_VO_INTERFACE_TYPE_BT1120;
        vo_config->vo_output = E_VO_OUTPUT_TYPE_1080P60;
        printf("unexpected type %d\n", type);
        break;
    }

    return 0;
}

vs_int32_t sample_common_vo_disp1_cfg_get(enum sample_vo_output_intf_type type, sample_vo_cfg_s *vo_config)
{
    vs_uint32_t img_width = vo_config->img_width;

    if (type == E_VO_OUTPUT_INTF_TYPE_DISABLE) {
        vo_config->enable = VS_FALSE;
        return 0;
    }

    if (type == E_VO_OUTPUT_INTF_TYPE_MIPI_DSI_TX) {
        img_width = vo_config->img_width;
        vo_config->img_width = vo_config->img_height;
        vo_config->img_height = img_width;
    }
    vo_config->vo_devid = 1;
    vo_config->vo_layerid = 3;
    vo_output_type_get(type, vo_config);
    vo_config->enable = VS_TRUE;

    return 0;
}

static vs_void_t sample_common_vo_dev_output_rect_get(const sample_vo_cfg_s* p_vo_config, vs_rect_s *p_rect)
{
    vs_vo_dev_attr_s dev_attr = {0,};
    vs_uint32_t ret;

    ret = vs_mal_vo_dev_attr_get(p_vo_config->vo_devid, &dev_attr);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("display%d dev attr get failed ret 0x%x\n", p_vo_config->vo_devid, ret);
        return;
    }

    p_rect->width = dev_attr.timing_info.hactive;
    p_rect->height = dev_attr.timing_info.interlaced_output ?
        dev_attr.timing_info.vactive << 1 : dev_attr.timing_info.vactive;
}

vs_void_t sample_common_vo_output_rect_get(const sample_vo_cfg_s* p_vo_config, vs_rect_s *p_rect)
{
    if (!p_vo_config || !p_rect)
        return;

    p_rect->x = 0;
    p_rect->y = 0;
    p_rect->width = 0;
    p_rect->height = 0;

    switch (p_vo_config->vo_output) {
        case E_VO_OUTPUT_TYPE_1080P24:
        case E_VO_OUTPUT_TYPE_1080P25:
        case E_VO_OUTPUT_TYPE_1080P30:
        case E_VO_OUTPUT_TYPE_1080P50:
        case E_VO_OUTPUT_TYPE_1080P60:
        case E_VO_OUTPUT_TYPE_1080I60:
        case E_VO_OUTPUT_TYPE_1080I50:
            p_rect->width = 1920;
            p_rect->height = 1080;
            break;

        case E_VO_OUTPUT_TYPE_PAL:
        case E_VO_OUTPUT_TYPE_576P50:
            p_rect->width = 720;
            p_rect->height = 576;
            break;

        case E_VO_OUTPUT_TYPE_NTSC:
        case E_VO_OUTPUT_TYPE_480P60:
            p_rect->width = 720;
            p_rect->height = 480;
            break;

        case E_VO_OUTPUT_TYPE_720P50:
        case E_VO_OUTPUT_TYPE_720P60:
            p_rect->width = 1280;
            p_rect->height = 720;
            break;

        case E_VO_OUTPUT_TYPE_800x600_60:
        case E_VO_OUTPUT_TYPE_800x600_50:
            p_rect->width = 800;
            p_rect->height = 600;
            break;

        case E_VO_OUTPUT_TYPE_1024x768_60:
            p_rect->width = 1024;
            p_rect->height = 768;
            break;

        case E_VO_OUTPUT_TYPE_1280x1024_60:
            p_rect->width = 1280;
            p_rect->height = 1024;
            break;

        case E_VO_OUTPUT_TYPE_1366x768_60:
            p_rect->width = 1366;
            p_rect->height = 768;
            break;

        case E_VO_OUTPUT_TYPE_1440x900_60:
            p_rect->width = 1440;
            p_rect->height = 900;
            break;

        case E_VO_OUTPUT_TYPE_1280x800_60:
            p_rect->width = 1280;
            p_rect->height = 800;
            break;

        case E_VO_OUTPUT_TYPE_1600x1200_60:
            p_rect->width = 1600;
            p_rect->height = 1200;
            break;

        case E_VO_OUTPUT_TYPE_1680x1050_60:
            p_rect->width = 1680;
            p_rect->height = 1050;
            break;

        case E_VO_OUTPUT_TYPE_1920x1200_60:
            p_rect->width = 1920;
            p_rect->height = 1200;
            break;

        case E_VO_OUTPUT_TYPE_640x480_60:
            p_rect->width = 640;
            p_rect->height = 480;
            break;

        case E_VO_OUTPUT_TYPE_1920x2160_30:
            p_rect->width = 1920;
            p_rect->height = 2160;
            break;

        case E_VO_OUTPUT_TYPE_2560x1440_30:
        case E_VO_OUTPUT_TYPE_2560x1440_60:
            p_rect->width = 2560;
            p_rect->height = 1440;
            break;

        case E_VO_OUTPUT_TYPE_2560x1600_60:
            p_rect->width = 2560;
            p_rect->height = 1600;
            break;

        case E_VO_OUTPUT_TYPE_3840x2160_24:
        case E_VO_OUTPUT_TYPE_3840x2160_25:
        case E_VO_OUTPUT_TYPE_3840x2160_30:
        case E_VO_OUTPUT_TYPE_3840x2160_50:
        case E_VO_OUTPUT_TYPE_3840x2160_60:
            p_rect->width = 3840;
            p_rect->height = 2160;
            break;

        case E_VO_OUTPUT_TYPE_4096x2160_24:
        case E_VO_OUTPUT_TYPE_4096x2160_25:
        case E_VO_OUTPUT_TYPE_4096x2160_30:
        case E_VO_OUTPUT_TYPE_4096x2160_50:
        case E_VO_OUTPUT_TYPE_4096x2160_60:
            p_rect->width = 4096;
            p_rect->height = 2160;
            break;

        case E_VO_OUTPUT_TYPE_320x240_60:
        case E_VO_OUTPUT_TYPE_320x240_50:
            p_rect->width = 320;
            p_rect->height = 240;
            break;

        case E_VO_OUTPUT_TYPE_240x320_50:
        case E_VO_OUTPUT_TYPE_240x320_60:
            p_rect->width = 240;
            p_rect->height = 320;
            break;

        case E_VO_OUTPUT_TYPE_720x1280_60:
            p_rect->width = 720;
            p_rect->height = 1280;
            break;

        case E_VO_OUTPUT_TYPE_1080x1920_60:
            p_rect->width = 1080;
            p_rect->height = 1920;
            break;

        default:
            sample_common_vo_dev_output_rect_get(p_vo_config, p_rect);
            break;
    }
}

vs_void_t sample_common_vo_dest_rect_get(sample_vo_cfg_s* p_vo_config, vs_rect_s *p_rect)
{
    switch (p_vo_config->vo_output) {
        case E_VO_OUTPUT_TYPE_1080P25:
        case E_VO_OUTPUT_TYPE_1080P30:
        case E_VO_OUTPUT_TYPE_1080P50:
        case E_VO_OUTPUT_TYPE_1080P60:
        case E_VO_OUTPUT_TYPE_1080I60:
            p_rect->x = 0;
            p_rect->y = 0;
            p_rect->width = 1920;
            p_rect->height = 1080;
            break;

        case E_VO_OUTPUT_TYPE_3840x2160_25:
        case E_VO_OUTPUT_TYPE_3840x2160_30:
        case E_VO_OUTPUT_TYPE_3840x2160_50:
        case E_VO_OUTPUT_TYPE_3840x2160_60:
            if ((p_vo_config->img_width == 2560 && p_vo_config->img_height == 1440)
                    || (p_vo_config->img_width == 2688 && p_vo_config->img_height == 1520)) {
                p_rect->x = (3840 - p_vo_config->img_width) / 2;
                p_rect->y = (2160 - p_vo_config->img_height) / 2;
                p_rect->width = p_vo_config->img_width;
                p_rect->height = p_vo_config->img_height;
            } else {
                p_rect->x = 0;
                p_rect->y = 0;
                p_rect->width = 3840;
                p_rect->height = 2160;
            }
            break;
        case E_VO_OUTPUT_TYPE_PAL:
            p_rect->x = 0;
            p_rect->y = 0;
            p_rect->width = 720;
            p_rect->height = 576;
            break;
        case E_VO_OUTPUT_TYPE_NTSC:
            p_rect->x = 0;
            p_rect->y = 0;
            p_rect->width = 720;
            p_rect->height = 480;
            break;
        case E_VO_OUTPUT_TYPE_2560x1440_60:
            p_rect->x = 0;
            p_rect->y = 0;
            p_rect->width = 2560;
            p_rect->height = 1440;
            break;
        case E_VO_OUTPUT_TYPE_720P60:
        case E_VO_OUTPUT_TYPE_720P50:
            p_rect->x = 0;
            p_rect->y = 0;
            p_rect->width = 1280;
            p_rect->height = 720;
            break;

        case E_VO_OUTPUT_TYPE_576P50:
            p_rect->x = 0;
            p_rect->y = 0;
            p_rect->width = 720;
            p_rect->height = 576;
            break;

        case E_VO_OUTPUT_TYPE_480P60:
            p_rect->x = 0;
            p_rect->y = 0;
            p_rect->width = 720;
            p_rect->height = 480;
            break;

        default:
            sample_common_vo_output_rect_get(p_vo_config, p_rect);
            break;
    }

    return;
}

#ifndef VS_ORION
vs_void_t sample_common_vo_hdmi_convert(vs_vo_output_type_e vo_video, vs_hdmi_video_format_e *p_hdmi_video)
{
    switch (vo_video) {
        case E_VO_OUTPUT_TYPE_1080P30:
            *p_hdmi_video = E_HDMI_VIDEO_FORMAT_1080P_30;
            break;
        case E_VO_OUTPUT_TYPE_3840x2160_30:
            *p_hdmi_video = E_HDMI_VIDEO_FORMAT_3840x2160P_30;
            break;
        case E_VO_OUTPUT_TYPE_1080P60:
            *p_hdmi_video = E_HDMI_VIDEO_FORMAT_1080P_60;
            break;
        case E_VO_OUTPUT_TYPE_3840x2160_60:
            *p_hdmi_video = E_HDMI_VIDEO_FORMAT_3840x2160P_60;
            break;
        case E_VO_OUTPUT_TYPE_2560x1440_60:
            *p_hdmi_video = E_HDMI_VIDEO_FORMAT_2560x1440_60;
            break;
        case E_VO_OUTPUT_TYPE_USER:
            *p_hdmi_video = E_HDMI_VIDEO_FORMAT_CUSTOMER_DEFINE;
            break;
        default:
            *p_hdmi_video = E_HDMI_VIDEO_FORMAT_1080P_30;
            break;
    }

    return;
}

static vs_void_t sample_common_vo_hdmi_attr_timing_get(const vs_vo_timing_s *timing_info,
    const vs_vo_clk_info_s *clk_info, vs_hdmi_attr_s *p_attr)
{
    if (!timing_info || !p_attr || !clk_info)
        return;

    p_attr->detailed_timing.pixel_clk = (vs_uint32_t) (clk_info->pixel_clk_rate / 1000);
    p_attr->detailed_timing.hactive = timing_info->hactive;
    p_attr->detailed_timing.vactive = timing_info->vactive;
    p_attr->detailed_timing.hbp = timing_info->hbp;
    p_attr->detailed_timing.vbp = timing_info->vbp;
    p_attr->detailed_timing.hfp = timing_info->hfp;
    p_attr->detailed_timing.vfp = timing_info->vfp;
    p_attr->detailed_timing.vpw = timing_info->vpw;
    p_attr->detailed_timing.hpw = timing_info->hpw;
    p_attr->detailed_timing.vsync_polarity = timing_info->vsync_polarity;
    p_attr->detailed_timing.hsync_polarity = timing_info->hsync_polarity;
    p_attr->detailed_timing.is_interlace = timing_info->interlaced_output;
}

static vs_int32_t sample_common_set_hdmi_attr(vs_vo_output_type_e output,
    vs_vo_interface_type_e vo_intf_type, const vs_vo_timing_s *timing,
    const vs_vo_clk_info_s *clk_info)
{
    vs_int32_t ret;
    vs_hdmi_attr_s hdmi_attr = {0};
    vs_hdmi_video_format_e hdmi_video_put = E_HDMI_VIDEO_FORMAT_1080P_60;
    vs_vo_interface_type_e vo_yuv_intf_type = E_VO_INTERFACE_TYPE_HDMI | E_VO_INTERFACE_TYPE_BT1120;

    sample_common_vo_hdmi_convert(output, &hdmi_video_put);

    ret = vs_mal_hdmi_attr_get(0, &hdmi_attr);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_hdmi_attr_get failed with 0x%x\n", ret);
        return VS_FAILED;
    }

    hdmi_attr.hdmi_enable = VS_TRUE;
    hdmi_attr.video_enable = VS_TRUE;
    hdmi_attr.video_format = hdmi_video_put;
    /* hdmi_attr.video_in_mode  : E_HDMI_VIDEO_MODE_RGB444(default). when same source with bt1120 or bt656
     * interface, and bt1120 or bt656 use yuv input format, need set hdmi input format to
     * E_HDMI_VIDEO_MODE_YCBCR444.
     * hdmi_attr.video_out_mode : E_HDMI_VIDEO_MODE_YCBCR444, can support E_HDMI_VIDEO_MODE_RGB444  */
    if ((vo_intf_type & vo_yuv_intf_type) == vo_yuv_intf_type)
        hdmi_attr.video_in_mode = E_HDMI_VIDEO_MODE_YCBCR444;
    else
        hdmi_attr.video_in_mode = E_HDMI_VIDEO_MODE_RGB444;

    hdmi_attr.video_out_mode = E_HDMI_VIDEO_MODE_YCBCR444;
    hdmi_attr.deep_color_mode = E_HDMI_DEEP_COLOR_24BIT;
    hdmi_attr.audio_enable = VS_TRUE;
    hdmi_attr.audio_sample_rate = E_HDMI_SAMPLE_RATE_48K;
    hdmi_attr.audio_bit_depth =  E_HDMI_BIT_DEPTH_16;

    if (hdmi_attr.video_format == E_HDMI_VIDEO_FORMAT_CUSTOMER_DEFINE) {
        sample_common_vo_hdmi_attr_timing_get(timing, clk_info, &hdmi_attr);
    }

    ret = vs_mal_hdmi_attr_set(0, &hdmi_attr);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_hdmi_attr_set failed with 0x%x\n", ret);
        return VS_FAILED;
    }

    return VS_SUCCESS;
}

static vs_int32_t sample_common_hdmi_restart(vs_vo_output_type_e output,
    vs_vo_interface_type_e vo_intf_type, const vs_vo_timing_s *timing,
    const vs_vo_clk_info_s *clk_info)
{
    vs_int32_t ret;

    if (!timing || !clk_info)
        return VS_FAILED;

    ret = sample_common_set_hdmi_attr(output, vo_intf_type, timing, clk_info);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("sample_common_set_hdmi_attr failed with 0x%x\n", ret);
        return VS_FAILED;
    }

    ret = vs_mal_hdmi_start(0);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_hdmi_start failed with 0x%x\n", ret);
        return VS_FAILED;
    }

    return VS_SUCCESS;
}

void sample_hdmi_hotplug_cb(vs_hdmi_event_type_e event, vs_void_t *p_private_date)
{
    vs_int32_t ret = 0;
    if (E_HDMI_EVENT_HOTPLUG_IN == event) {
        ret = sample_common_hdmi_restart(g_output_type, g_hdmi_common_vo_intf_type,
            &g_hdmi_timing_info, &g_hdmi_clk_info);
        if (ret) {
            vs_sample_trace("hdmi_restart fail output_type %d, ret 0x%x\n", g_output_type, ret);
        }
    }
    vs_sample_trace("sample_hdmi_hotplug_cb, with event %d\n", event);
}

vs_int32_t sample_common_hdmi_start(vs_vo_output_type_e output, vs_vo_interface_type_e vo_intf_type)
{
    vs_int32_t ret;
    vs_hdmi_callback_s p_callback = {
        .p_hdmi_event_callback = sample_hdmi_hotplug_cb,
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

    g_hdmi_common_vo_intf_type = vo_intf_type;
    ret = sample_common_set_hdmi_attr(output, vo_intf_type, &g_hdmi_timing_info, &g_hdmi_clk_info);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("sample_common_set_hdmi_attr failed with 0x%x\n", ret);
        return VS_FAILED;
    }

    ret = vs_mal_hdmi_callback_register(0, &p_callback);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_hdmi_callback_register failed with 0x%x\n", ret);
        return VS_FAILED;
    }

    ret = vs_mal_hdmi_start(0);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_hdmi_start failed with 0x%x\n", ret);
        return VS_FAILED;
    }

    return VS_SUCCESS;
}

vs_int32_t sample_common_hdmi_stop()
{
    vs_mal_hdmi_stop(0);
    vs_mal_hdmi_close(0);

    vs_mal_hdmi_deinit();
    return 0;
}
#endif

void sample_common_vo_timing_get(vs_vo_dev_attr_s *attr, vs_vo_clk_info_s *clk)
{
    /* user defined vo timing, especially used when the interface is mipi-tx(1080*1920P60) */
    attr->interface_output = E_VO_OUTPUT_TYPE_USER;
    attr->timing_info.hactive = 1080;
    attr->timing_info.hfp = 118;
    attr->timing_info.hbp = 60;
    attr->timing_info.hpw = 8;
    attr->timing_info.hsync_polarity = 1;
    attr->timing_info.vactive = 1920;
    attr->timing_info.vfp = 15;
    attr->timing_info.vbp = 8;
    attr->timing_info.vpw = 2;
    attr->timing_info.vsync_polarity = 1;
    attr->timing_info.data_polarity = 1;
    attr->timing_info.interlaced_output = 0;
    clk->pixel_clk_rate = 148500000UL;
    clk->clk_div = 8;
}

static void sample_common_vo_hdmi_user_timing_get(vs_vo_dev_attr_s *attr, vs_vo_clk_info_s *clk)
{
    /* user defined vo timing, especially used when the interface is HDMI(1920*1080P59.94) */
    /* hdmi attr should be the same with the vo user timing info, refer to sample_common_hdmi_user_timing_get */
    attr->interface_output = E_VO_OUTPUT_TYPE_USER;
    attr->timing_info.hactive = 1920;
    attr->timing_info.hfp = 88;
    attr->timing_info.hbp = 148;
    attr->timing_info.hpw = 44;
    attr->timing_info.hsync_polarity = 1;
    attr->timing_info.vactive = 1080;
    attr->timing_info.vfp = 4;
    attr->timing_info.vbp = 36;
    attr->timing_info.vpw = 5;
    attr->timing_info.vsync_polarity = 1;
    attr->timing_info.data_polarity = 1;
    attr->timing_info.interlaced_output = 0;
    clk->pixel_clk_rate = 148351500UL;      /* (148500000UL * 59.94) / 60 = 148351500UL */
    clk->clk_div = 8;
}

static vs_void_t sample_common_vo_chn_num_get(sample_vo_mode_e vo_mode, vs_int32_t *row,
    vs_int32_t *col, vs_int32_t *square, vs_int32_t *chn_num)
{
    *row = 1;
    *col = 1;

     switch (vo_mode) {
     case E_VO_MODE_1MUX:
        *chn_num = 1;
        *square = 1;
         break;
     case E_VO_MODE_2MUX:
        *chn_num = 2;
        *square = 2;
         break;
     case E_VO_MODE_4MUX:
        *chn_num = 4;
        *square = 2;
         break;
     case E_VO_MODE_5MUX:
        *chn_num = 5;
        *square = 3;
        *row = 4;
        *col = 2;
         break;
     case E_VO_MODE_8MUX:
        *chn_num = 8;
        *square = 3;
        *col = 2;
        *row = 4;
         break;
     case E_VO_MODE_9MUX:
        *chn_num = 9;
        *square = 3;
         break;
     case E_VO_MODE_12MUX:
        *chn_num = 12;
        *square = 4;
        *col = 4;
        *row = 3;
         break;
     case E_VO_MODE_16MUX:
        *chn_num = 16;
        *square = 4;
         break;
     case E_VO_MODE_25MUX:
        *chn_num = 25;
        *square = 5;
         break;
     case E_VO_MODE_36MUX:
        *chn_num = 36;
        *square = 6;
         break;
     default:
        *chn_num = 1;
        *square = 1;
         break;
    }
}

static vs_void_t sample_common_vo_layer_image_size_get(sample_vo_cfg_s* p_vo_config)
{
    vs_rect_s tmp = {0};

    if (p_vo_config->vo_intf_type == E_VO_INTERFACE_TYPE_MIPI &&
        p_vo_config->vo_output == E_VO_OUTPUT_TYPE_USER) {
        /* for some sample, may have rotated, and changed img width and height */
        if (p_vo_config->img_width > p_vo_config->img_height) {
            vs_sample_trace("display%d layer%d mipi intf need rotation\n", p_vo_config->vo_devid, p_vo_config->vo_layerid);
            p_vo_config->rotation_enable = VS_TRUE;
            tmp.height = p_vo_config->img_height;
            p_vo_config->img_height = p_vo_config->img_width;
            p_vo_config->img_width  = tmp.height;
        }
    }

#ifdef VS_ORION
    /* for orion, layer0 size need equal with display user timing */
    if (p_vo_config->vo_layerid == 0) {
        tmp.height = 0;
        tmp.width  = 0;
        sample_common_vo_dev_output_rect_get(p_vo_config, &tmp);
        if (tmp.height && tmp.width) {
            p_vo_config->img_height = tmp.height;
            p_vo_config->img_width  = tmp.width;
        }
    }
#endif
}

vs_int32_t sample_common_vo_chn_enable(sample_vo_cfg_s* p_vo_config)
{
    vs_int32_t ret;
    vs_int32_t row = 1;
    vs_int32_t col = 1;
    vs_int32_t tmp;
    vs_int32_t square;
    vs_int32_t chn_num;
    vs_int32_t vo_chnid;
    vs_int32_t vo_layerid;
    vs_int32_t layer_width;
    vs_int32_t layer_height;
    sample_vo_mode_e vo_mode;
    vs_vo_chn_attr_s vo_chn_attr = {0};

    sample_common_vo_layer_image_size_get(p_vo_config);

    vo_mode = p_vo_config->vo_mode;
    vo_layerid = p_vo_config->vo_layerid;
    layer_width = p_vo_config->img_width;
    layer_height = p_vo_config->img_height;

    sample_common_vo_chn_num_get(vo_mode, &row, &col, &square, &chn_num);

    /* if mipi, need swap row and col */
    if (p_vo_config->vo_intf_type == E_VO_INTERFACE_TYPE_MIPI) {
        tmp = row;
        row = col;
        col = tmp;
    }

    for (vo_chnid = 0; vo_chnid < chn_num; ++vo_chnid) {
        memset(&vo_chn_attr, 0 , sizeof(vo_chn_attr));
        if (vo_mode == E_VO_MODE_1MUX  ||
            vo_mode == E_VO_MODE_2MUX  ||
            vo_mode == E_VO_MODE_4MUX  ||
            vo_mode == E_VO_MODE_9MUX  ||
            vo_mode == E_VO_MODE_16MUX ||
            vo_mode == E_VO_MODE_25MUX ||
            vo_mode == E_VO_MODE_36MUX) {
            vo_chn_attr.rect.x      = align_down((layer_width / square) * (vo_chnid % square), 2);
            vo_chn_attr.rect.y      = align_down((layer_height / square) * (vo_chnid / square), 2);
            vo_chn_attr.rect.width  = align_down(layer_width / square, 2);
            vo_chn_attr.rect.height = align_down(layer_height / square, 2);
            vo_chn_attr.zorder      = 0;
            vo_chn_attr.deflicker   = VS_FALSE;
        } else if (vo_mode == E_VO_MODE_8MUX || vo_mode == E_VO_MODE_12MUX) {
            vo_chn_attr.rect.x       = align_down((layer_width / col) * (vo_chnid % col), 2);
            vo_chn_attr.rect.y       = align_down((layer_height / row) * (vo_chnid / col), 2);
            vo_chn_attr.rect.width   = align_down(layer_width / col, 2);
            vo_chn_attr.rect.height  = align_down(layer_height / row, 2);
            vo_chn_attr.zorder       = 0;
            vo_chn_attr.deflicker    = VS_FALSE;
        } else if (vo_mode == E_VO_MODE_5MUX) {

            if (vo_chnid == 0) {
                vo_chn_attr.rect.x       = 400;
                vo_chn_attr.rect.y       = 0;
                vo_chn_attr.rect.width   = 1316;
                vo_chn_attr.rect.height  = 740;
                vo_chn_attr.zorder       = 0;
                vo_chn_attr.deflicker    = VS_FALSE;

            }
            if (vo_chnid == 1) {
                vo_chn_attr.rect.x       = 1316+800;
                vo_chn_attr.rect.y       = 0;
                vo_chn_attr.rect.width   = 1316;
                vo_chn_attr.rect.height  = 740;
                vo_chn_attr.zorder       = 0;
                vo_chn_attr.deflicker    = VS_FALSE;

            }
            if (vo_chnid == 2) {
                vo_chn_attr.rect.x       = 400;
                vo_chn_attr.rect.y       = 740;
                vo_chn_attr.rect.width   = 1316;
                vo_chn_attr.rect.height  = 740;
                vo_chn_attr.zorder       = 0;
                vo_chn_attr.deflicker    = VS_FALSE;

            }
            if (vo_chnid == 3) {
                vo_chn_attr.rect.x       = 1316+800;
                vo_chn_attr.rect.y       = 740;
                vo_chn_attr.rect.width   = 1316;
                vo_chn_attr.rect.height  = 740;
                vo_chn_attr.zorder       = 0;
                vo_chn_attr.deflicker    = VS_FALSE;

            }
            if (vo_chnid == 4) {
                vo_chn_attr.rect.x       = 0;
                vo_chn_attr.rect.y       = 1480;
                vo_chn_attr.rect.width   = 3840;
                vo_chn_attr.rect.height  = 680;
                vo_chn_attr.zorder       = 0;
                vo_chn_attr.deflicker    = VS_FALSE;

            }
        }

        ret = vs_mal_vo_chn_attr_set(vo_layerid, vo_chnid, &vo_chn_attr);
        if (ret != VS_SUCCESS) {
            vs_sample_trace("vs_mal_vo_chn_attr_set(vo_layerid %d vo_chnid %d) failed with 0x%x\n", vo_layerid, vo_chnid, ret);
            return VS_FAILED;
        }

        ret = vs_mal_vo_chn_buf_depth_set(vo_layerid, vo_chnid, CHN_BUF_DEPTH_DEF);
        if (ret != VS_SUCCESS) {
            vs_sample_trace("vs_mal_vo_chn_buf_depth_set(vo_layerid %d vo_chnid %d) failed with 0x%x\n", vo_layerid, vo_chnid, ret);
            return VS_FAILED;
        }
    }

    if (p_vo_config->rotation_enable) {
        for (vo_chnid = 0; vo_chnid < chn_num; ++vo_chnid) {
            ret = vs_mal_vo_chn_rotation_set(vo_layerid, vo_chnid, E_FIXED_ROTATION_90);
            if (ret != VS_SUCCESS) {
                vs_sample_trace("vs_mal_vo_chn_rotation_set(vo_layerid %d  vo_chnid %d) failed with 0x%x\n", vo_layerid, vo_chnid, ret);
                return VS_FAILED;
            }
        }
    }

    //todo chn param/zoomin/

    for (vo_chnid = 0; vo_chnid < chn_num; ++vo_chnid) {
        ret = vs_mal_vo_chn_enable(vo_layerid, vo_chnid);
        if (ret != VS_SUCCESS) {
            vs_sample_trace("vs_mal_vo_chn_enable(vo_layerid %d  vo_chnid %d) failed with 0x%x\n", vo_layerid, vo_chnid, ret);
            return VS_FAILED;
        }
    }

    return VS_SUCCESS;
}

vs_int32_t sample_common_vo_chn_disable(sample_vo_cfg_s* p_vo_config)
{
    vs_int32_t ret = 0;
    vs_int32_t row = 1;
    vs_int32_t col = 1;
    vs_int32_t square;
    vs_int32_t chn_num;
    vs_int32_t vo_chnid;
    vs_int32_t vo_layerid;
    sample_vo_mode_e vo_mode;

    vo_mode = p_vo_config->vo_mode;
    vo_layerid = p_vo_config->vo_layerid;

    sample_common_vo_chn_num_get(vo_mode, &row, &col, &square, &chn_num);

    for(vo_chnid = 0; vo_chnid < chn_num; vo_chnid++)
        ret |= vs_mal_vo_chn_disable(vo_layerid, vo_chnid);

    return ret;
}


vs_bool_t is_vo_interlaced_output(sample_vo_cfg_s* p_vo_config)
{
    if (p_vo_config->vo_output == E_VO_OUTPUT_TYPE_PAL
        || p_vo_config->vo_output == E_VO_OUTPUT_TYPE_NTSC
        || p_vo_config->vo_output == E_VO_OUTPUT_TYPE_1080I60
        || p_vo_config->vo_output == E_VO_OUTPUT_TYPE_1080I50)
        return VS_TRUE;

    return VS_FALSE;
}

vs_int32_t sample_common_vo_layer_enable(sample_vo_cfg_s* p_vo_config)
{
    vs_int32_t ret;
    vs_rect_s rect;
    vs_vo_video_layer_attr_s attr = {0};
    vs_crop_s crop;
    p_vo_config->rotation_enable = VS_FALSE;

    sample_common_vo_layer_image_size_get(p_vo_config);

    attr.layer_framerate = 0;
    attr.dynamic_range = p_vo_config->dynamic_range;
    attr.format = p_vo_config->pixel_format;
    attr.image_size.width = p_vo_config->img_width;
    attr.image_size.height = p_vo_config->img_height;

    sample_common_vo_dest_rect_get(p_vo_config, &rect);
    attr.dest_rect.x = rect.x;
    attr.dest_rect.y = rect.y;
    attr.dest_rect.width = rect.width;
    attr.dest_rect.height = rect.height;

    ret = vs_mal_vo_video_layer_bind(p_vo_config->vo_layerid, p_vo_config->vo_devid);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_vo_video_layer_bind(vo_devid %d  vo_layerid %d) failed with 0x%x\n", p_vo_config->vo_devid, p_vo_config->vo_layerid, ret);
        return VS_FAILED;
    }

    ret = vs_mal_vo_video_layer_attr_set(p_vo_config->vo_layerid, &attr);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_vo_video_layer_attr_set(vo_layerid %d) failed with 0x%x\n", p_vo_config->vo_layerid, ret);
        return VS_FAILED;
    }

    ret = vs_mal_vo_video_layer_zorder_set(p_vo_config->vo_layerid, p_vo_config->zorder);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_vo_video_layer_zorder_set(vo_layerid %d) failed with 0x%x\n", p_vo_config->vo_layerid, ret);
        return VS_FAILED;
    }

    if (attr.image_size.width > attr.dest_rect.width) {
        crop.enable = VS_TRUE;
        crop.rect.x = 0;
        crop.rect.y = 0;
        crop.rect.width  = attr.dest_rect.width;
        crop.rect.height = attr.dest_rect.height;

        ret = vs_mal_vo_video_layer_crop_set(p_vo_config->vo_layerid, &crop);
        if (ret != VS_SUCCESS) {
            vs_sample_trace("vs_mal_vo_video_layer_crop_set(vo_layerid %d) failed with 0x%x\n", p_vo_config->vo_layerid, ret);
            return VS_FAILED;
        }
    }

    ret = vs_mal_vo_video_layer_buf_depth_set(p_vo_config->vo_layerid, LAYER_BUF_DEPTH_DEF);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_vo_video_layer_buf_depth_set(vo_layerid %d) failed with 0x%x\n", p_vo_config->vo_layerid, ret);
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

vs_void_t sample_common_wbc_size_get(sample_vo_cfg_s* p_vo_config, vs_size_s* size)
{
    vs_rect_s rect = {0};

    if (p_vo_config->wbc_type == E_VO_SAMPLE_WBC_SOURCE_TYPE_VIDEO ||
        p_vo_config->wbc_type == E_VO_SAMPLE_WBC_SOURCE_TYPE_GRAPHIC) {
        size->width = p_vo_config->img_width;
        size->height = p_vo_config->img_height;
    } else if (p_vo_config->wbc_type == E_VO_SAMPLE_WBC_SOURCE_TYPE_DEVICE) {
        sample_common_vo_output_rect_get(p_vo_config, &rect);
        size->height = rect.height;
        size->width = rect.width;

        if (p_vo_config->vo_output == E_VO_OUTPUT_TYPE_USER
            && p_vo_config->vo_intf_type == E_VO_INTERFACE_TYPE_MIPI) {
            size->width = 1080;
            size->height = 1920;
        }
    }
}

static vs_int32_t sample_common_wbc_source_type_get(sample_vo_cfg_s* p_vo_config, vs_vo_wbc_source_type_e *type)
{
    vs_int32_t ret = VS_SUCCESS;
    switch (p_vo_config->wbc_type) {
        case E_VO_SAMPLE_WBC_SOURCE_TYPE_VIDEO:
            *type = E_VO_WBC_SOURCE_TYPE_VIDEO;
            break;
        case E_VO_SAMPLE_WBC_SOURCE_TYPE_GRAPHIC:
            *type = E_VO_WBC_SOURCE_TYPE_GRAPHIC;
            break;
        case E_VO_SAMPLE_WBC_SOURCE_TYPE_DEVICE:
            *type = E_VO_WBC_SOURCE_TYPE_DEVICE;
            break;
        default:
            ret = VS_FAILED;
            break;
    }

    return ret;
}

vs_int32_t sample_common_vo_wbc_enable(sample_vo_cfg_s* p_vo_config)
{
    vs_int32_t ret;
    vs_vo_wbc_source_type_e type = E_VO_WBC_SOURCE_TYPE_MAX;
    vs_vo_wbc_attr_s attr = {0};

    sample_common_wbc_size_get(p_vo_config, &attr.dest_size);
    attr.format = E_PIXEL_FORMAT_YUV_420SP;
    attr.framerate = 60;

    ret = sample_common_wbc_source_type_get(p_vo_config, &type);

    if (ret != VS_SUCCESS) {
        vs_sample_trace("sample_common_wbc_source_type_get(vo_wbcid %d) failed with 0x%x\n", p_vo_config->vo_devid, ret);
        return VS_FAILED;
    }

    ret = vs_mal_vo_wbc_attr_set(p_vo_config->vo_devid, &attr);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_vo_wbc_attr_set(vo_wbcid %d) failed with 0x%x\n", p_vo_config->vo_devid, ret);
        return VS_FAILED;
    }

    ret = vs_mal_vo_wbc_source_set(p_vo_config->vo_devid, type);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_vo_wbc_source_set(vo_wbcid %d) set %d failed with 0x%x\n", p_vo_config->vo_devid, type, ret);
        return VS_FAILED;
    }

    ret = vs_mal_vo_wbc_enable(p_vo_config->vo_devid);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_vo_wbc_enable(vo_wbcid %d) failed with 0x%x\n", p_vo_config->vo_devid, ret);
        return VS_FAILED;
    }

    return ret;
}

vs_int32_t sample_common_vo_chn_remove(vs_int32_t layerid, vs_int32_t chnid)
{
    return vs_mal_vo_chn_disable(layerid, chnid);
}

vs_int32_t sample_common_vo_layer_remove(vs_int32_t devid, vs_int32_t layerid)
{
    vs_mal_vo_video_layer_unbind(layerid, devid);
    vs_mal_vo_video_layer_disable(layerid);

    return VS_SUCCESS;
}

vs_int32_t sample_common_vo_start(sample_vo_cfg_s* p_vo_config)
{
    vs_int32_t ret;
    vs_vo_clk_info_s clk = {0};
    vs_vo_dev_attr_s dev_attr = {0};

    dev_attr.bg_color = p_vo_config->bg_color;
    dev_attr.interface_output = p_vo_config->vo_output;
    dev_attr.interface_type = p_vo_config->vo_intf_type;

    if (p_vo_config->vo_output == E_VO_OUTPUT_TYPE_USER) {
        if (p_vo_config->vo_intf_type & E_VO_INTERFACE_TYPE_MIPI) {
            sample_common_vo_timing_get(&dev_attr, &clk);
        }

        if (p_vo_config->vo_intf_type & E_VO_INTERFACE_TYPE_HDMI) {
            sample_common_vo_hdmi_user_timing_get(&dev_attr, &clk);
        }

        vs_mal_vo_clk_set(p_vo_config->vo_devid, &clk);
    }

    ret = vs_mal_vo_dev_attr_set(p_vo_config->vo_devid, &dev_attr);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_vo_dev_attr_set(vo_devid %d) failed with 0x%x\n", p_vo_config->vo_devid, ret);
        return VS_FAILED;
    }

    /* Set early_cfg_time when p_vo_config->early_cfg_time isn't 0. */
    if (p_vo_config->early_cfg_time != 0) {
        ret = vs_mal_vo_dev_early_cfg_time_set(p_vo_config->vo_devid, p_vo_config->early_cfg_time);
        if (ret != VS_SUCCESS) {
            vs_sample_trace("vs_mal_vo_dev_early_cfg_time_set(vo_devid %d) failed with 0x%x\n",
                            p_vo_config->vo_devid, ret);
            return VS_FAILED;
        }
    }

    if (p_vo_config->wbc_type > E_VO_SAMPLE_WBC_DISABLE &&
        p_vo_config->wbc_type < E_VO_SAMPLE_WBC_SOURCE_TYPE_MAX) {
        ret = sample_common_vo_wbc_enable(p_vo_config);
        if (ret != VS_SUCCESS) {
            return VS_FAILED;
        }
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

    if (p_vo_config->vo_intf_type & E_VO_INTERFACE_TYPE_HDMI &&
        p_vo_config->wbc_type != E_VO_SAMPLE_WBC_SOURCE_TYPE_DEVICE) {
        g_output_type = p_vo_config->vo_output;
#ifndef VS_ORION

        memcpy(&g_hdmi_timing_info, &dev_attr.timing_info, sizeof(vs_vo_timing_s));
        memcpy(&g_hdmi_clk_info, &clk, sizeof(vs_vo_clk_info_s));
        if (sample_common_hdmi_start(p_vo_config->vo_output, p_vo_config->vo_intf_type) != VS_SUCCESS) {
            vs_sample_trace("hdmi_start failed with 0x%x, registered a callback for hdmi hotplug\n", ret);
        }
#else
        vs_sample_trace("error, hdmi not support for orion\n");
#endif
    }

    if (p_vo_config->vo_intf_type & E_VO_INTERFACE_TYPE_MIPI &&
        p_vo_config->wbc_type != E_VO_SAMPLE_WBC_SOURCE_TYPE_DEVICE)
        sample_common_mipitx_start(p_vo_config->vo_output);

    ret = vs_mal_vo_commit(p_vo_config->vo_devid);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_vo_commit(vo_devid %d) failed with 0x%x\n", p_vo_config->vo_devid, ret);
        return VS_FAILED;
    }


    return ret;
}

vs_int32_t sample_common_vo_stop(sample_vo_cfg_s* p_vo_config)
{
    vs_int32_t vo_devid = p_vo_config->vo_devid;
    vs_int32_t vo_layerid = p_vo_config->vo_layerid;

    if (p_vo_config->wbc_type > E_VO_SAMPLE_WBC_DISABLE &&
        p_vo_config->wbc_type < E_VO_SAMPLE_WBC_SOURCE_TYPE_MAX)
        vs_mal_vo_wbc_disable(p_vo_config->vo_devid);

#ifndef VS_ORION
    if (p_vo_config->vo_intf_type & E_VO_INTERFACE_TYPE_HDMI) {
        sample_common_hdmi_stop();
        memset(&g_hdmi_timing_info, 0, sizeof(vs_vo_timing_s));
        memset(&g_hdmi_clk_info, 0, sizeof(vs_vo_clk_info_s));
    }
#endif

    if (p_vo_config->vo_intf_type & E_VO_INTERFACE_TYPE_MIPI) {
        sample_common_mipitx_stop();
    }

    sample_common_vo_chn_disable(p_vo_config);
    vs_mal_vo_video_layer_unbind(vo_layerid, vo_devid);
    vs_mal_vo_video_layer_disable(vo_layerid);
    vs_mal_vo_disable(vo_devid);
    vs_mal_vo_commit(vo_devid);

    return VS_SUCCESS;
}

vs_int32_t sample_common_vo_chn_add(sample_vo_chn_cfg_s* p_chn_cfg)
{
    vs_int32_t ret;
    vs_vo_chn_attr_s vo_chn_attr = {0};

    memset(&vo_chn_attr, 0 , sizeof(vo_chn_attr));

    vo_chn_attr.rect.x       = p_chn_cfg->rect.x;
    vo_chn_attr.rect.y       = p_chn_cfg->rect.y;
    vo_chn_attr.rect.width   = p_chn_cfg->rect.width;
    vo_chn_attr.rect.height  = p_chn_cfg->rect.height;
    vo_chn_attr.zorder       = p_chn_cfg->zorder;
    vo_chn_attr.deflicker    = p_chn_cfg->deflicker;

    ret = vs_mal_vo_chn_attr_set(p_chn_cfg->layerid, p_chn_cfg->chnid, &vo_chn_attr);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_vo_chn_attr_set(vo_layerid %d vo_chnid %d) failed with 0x%x\n", p_chn_cfg->layerid, p_chn_cfg->chnid, ret);
        return VS_FAILED;
    }

    ret = vs_mal_vo_chn_enable(p_chn_cfg->layerid, p_chn_cfg->chnid);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_vo_chn_enable(vo_layerid %d  vo_chnid %d) failed with 0x%x\n", p_chn_cfg->layerid, p_chn_cfg->chnid, ret);
        return VS_FAILED;
    }

    return VS_SUCCESS;
}

vs_int32_t sample_common_vo_layer_add(sample_vo_layer_cfg_s* p_layer_cfg)
{
    vs_int32_t ret;
    vs_vo_video_layer_attr_s attr;

    attr.layer_framerate = p_layer_cfg->layer_framerate;
    attr.dynamic_range = p_layer_cfg->dynamic_range;
    attr.format = p_layer_cfg->format;
    attr.image_size.width = p_layer_cfg->image_size.width;
    attr.image_size.height = p_layer_cfg->image_size.height;

    attr.dest_rect.x = p_layer_cfg->dest_rect.x;
    attr.dest_rect.y = p_layer_cfg->dest_rect.y;
    attr.dest_rect.width = p_layer_cfg->dest_rect.width;
    attr.dest_rect.height = p_layer_cfg->dest_rect.height;

    ret = vs_mal_vo_video_layer_bind(p_layer_cfg->layerid, p_layer_cfg->devid);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_vo_video_layer_bind(vo_devid %d  vo_layerid %d) failed with 0x%x\n", p_layer_cfg->devid, p_layer_cfg->layerid, ret);
        return VS_FAILED;
    }

    ret = vs_mal_vo_video_layer_attr_set(p_layer_cfg->layerid, &attr);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_vo_video_layer_attr_set(vo_layerid %d) failed with 0x%x\n", p_layer_cfg->layerid, ret);
        return VS_FAILED;
    }
    ret = vs_mal_vo_video_layer_zorder_set(p_layer_cfg->layerid, p_layer_cfg->zorder);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_vo_video_layer_zorder_set(vo_layerid %d) failed with 0x%x\n", p_layer_cfg->layerid, ret);
        return VS_FAILED;
    }

    ret = vs_mal_vo_video_layer_buf_depth_set(p_layer_cfg->layerid, p_layer_cfg->buf_depth);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_vo_video_layer_enable(vo_layerid %d) failed with 0x%x\n", p_layer_cfg->layerid, ret);
        return VS_FAILED;
    }

    ret = vs_mal_vo_video_layer_enable(p_layer_cfg->layerid);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_vo_video_layer_enable(vo_layerid %d) failed with 0x%x\n", p_layer_cfg->layerid, ret);
        return VS_FAILED;
    }

    return ret;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
