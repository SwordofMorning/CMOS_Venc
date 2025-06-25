/**
* @file sample_hdmi_path.h
* @brief sample hdmi interface
* @details sample hdmi interface
* @author Visinex Software Group
* @date 2022-05-25
* @version v0.10
* @Copyright (C) 2022, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*
*/

#ifndef SAMPLE_HDMI_CMD_H
#define SAMPLE_HDMI_CMD_H

#include "sample_common.h"

#ifdef __cplusplus
extern "C" {
#endif

vs_int32_t sample_hdmi_hdmi_start(vs_hdmi_video_format_e video_put);
vs_int32_t sample_hdmi_hdmi_stop(vs_void_t);
vs_int32_t sample_hdmi_vdec_vpss_vo_start(vs_hdmi_video_format_e format);
vs_void_t sample_hdmi_vdec_vpss_vo_stop(vs_void_t);
vs_int32_t sample_hdmi_audio_start(vs_int32_t sample_rate);
vs_void_t sample_hdmi_audio_stop(vs_void_t);
vs_int32_t sample_hdmi_vo_fromat_change(vs_hdmi_video_format_e format);


#ifdef __cplusplus
}
#endif

#endif ///< end of _SAMPLE_HDMI_PATH_H_
