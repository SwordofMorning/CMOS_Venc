/**
* @file vs_hdmi_defines.h
* @brief hdmi data structures definition
* @details hdmi data structures definition
* @author VS platform group
* @date 2022-02-16
* @version v0.10
* @Copyright (C) 2022, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/

#ifndef __VS_HDMI_DEFINES_H__
#define __VS_HDMI_DEFINES_H__

#include "vs_type.h"
#include "vs_comm.h"
#include "vs_errno.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum vs_hdmi_private_errno {
    E_ERR_HDMI_DEV_NOT_CONNECT = 64,    ///< HDMI device not connected
    E_ERR_HDMI_EDID_READ_FAILED,        ///< EDID read failed
    E_ERR_HDMI_EDID_PARSE_FAILED,       ///< EDID parse failed
    E_ERR_HDMI_BAD_PHY_STATUS,          ///< Bad phy status
    E_ERR_HDMI_MAX
} vs_hdmi_private_errno_e;

#define VS_ERR_HDMI_INVALID_PARAM    VS_ERR_CODE(E_MOD_ID_HDMI, E_ERR_INVALID_PARAM)
#define VS_ERR_HDMI_INVALID_DEVID    VS_ERR_CODE(E_MOD_ID_HDMI, E_ERR_INVALID_DEVID)
#define VS_ERR_HDMI_NULL_PTR         VS_ERR_CODE(E_MOD_ID_HDMI, E_ERR_NULL_PTR)
#define VS_ERR_HDMI_UNEXIST          VS_ERR_CODE(E_MOD_ID_HDMI, E_ERR_UNEXIST)
#define VS_ERR_HDMI_NOT_SUPPORT      VS_ERR_CODE(E_MOD_ID_HDMI, E_ERR_NOT_SUPPORT)
#define VS_ERR_HDMI_NOT_INITIALIZED  VS_ERR_CODE(E_MOD_ID_HDMI, E_ERR_NOT_INITIALIZED)
#define VS_ERR_HDMI_NO_MEMORY        VS_ERR_CODE(E_MOD_ID_HDMI, E_ERR_NO_MEMORY)
#define VS_ERR_HDMI_ABORTED          VS_ERR_CODE(E_MOD_ID_HDMI, E_ERR_ABORTED)
#define VS_ERR_HDMI_DEVICE_UNEXIST   VS_ERR_CODE(E_MOD_ID_HDMI, E_ERR_DEVICE_UNEXIST)

#define VS_ERR_HDMI_DEV_NOT_CONNECT      VS_ERR_CODE(E_MOD_ID_HDMI, E_ERR_HDMI_DEV_NOT_CONNECT)
#define VS_ERR_HDMI_EDID_READ_FAILED     VS_ERR_CODE(E_MOD_ID_HDMI, E_ERR_HDMI_EDID_READ_FAILED)
#define VS_ERR_HDMI_EDID_PARSE_FAILED    VS_ERR_CODE(E_MOD_ID_HDMI, E_ERR_HDMI_EDID_PARSE_FAILED)
#define VS_ERR_HDMI_BAD_PHY_STATUS       VS_ERR_CODE(E_MOD_ID_HDMI, E_ERR_HDMI_BAD_PHY_STATUS)


#define HDMI_MAX_SIMPLE_RATE_NUM        8
#define HDMI_MAX_AUDIO_CAP_COUNT        16
#define HDMI_MAX_BIT_DEPTH_NUM          6
#define HDMI_MAX_EDID_SIZE              512
#define HDMI_MANUFACTURE_ID_LEN         4
#define HDMI_MAX_DETAILED_TIMING_NUM    10

/* signal used by HDMI callback function */
#define HDMI_SIG			46

/**
* @brief Define the HDMI video resolution
* @details Define the video resolution and refresh rate that HDMI module can support
*/
typedef enum vs_hdmi_video_format {
    E_HDMI_VIDEO_FORMAT_1080P_60   = 0,     ///< 1920X1080P @ 60 FPS
    E_HDMI_VIDEO_FORMAT_1080P_50,           ///< 1920X1080P @ 50 FPS
    E_HDMI_VIDEO_FORMAT_1080P_30,           ///< 1920X1080P @ 30 FPS
    E_HDMI_VIDEO_FORMAT_1080P_25,           ///< 1920X1080P @ 25 FPS
    E_HDMI_VIDEO_FORMAT_1080P_24,           ///< 1920X1080P @ 24 FPS
    E_HDMI_VIDEO_FORMAT_1080I_60,           ///< 1920X1080i @ 60 FPS
    E_HDMI_VIDEO_FORMAT_1080I_50,           ///< 1920X1080i @ 50 FPS
    E_HDMI_VIDEO_FORMAT_720P_60,            ///< 1280X720P  @ 60 FPS
    E_HDMI_VIDEO_FORMAT_720P_50,            ///< 1280X720P  @ 50 FPS
    E_HDMI_VIDEO_FORMAT_576P_50,            ///< 720X576    @ 50 FPS
    E_HDMI_VIDEO_FORMAT_480P_60,            ///< 720X480    @ 60 FPS

    E_HDMI_VIDEO_FORMAT_640x480_60,         ///< 640X480    @ 60 FPS
    E_HDMI_VIDEO_FORMAT_720x1280_60,        ///< 720X1280   @ 60 FPS
    E_HDMI_VIDEO_FORMAT_800x600_50,         ///< 800X600    @ 50 FPS
    E_HDMI_VIDEO_FORMAT_800x600_60,         ///< 800X600    @ 60 FPS VESA
    E_HDMI_VIDEO_FORMAT_1024x768_60,        ///< 1024X768   @ 60 FPS VESA
    E_HDMI_VIDEO_FORMAT_1080x1920_60,       ///< 1080X1920  @ 60 FPS
    E_HDMI_VIDEO_FORMAT_1280x800_60,        ///< 1280X800   @ 60 FPS VESA
    E_HDMI_VIDEO_FORMAT_1280x1024_60,       ///< 1280X1024  @ 60 FPS VESA
    E_HDMI_VIDEO_FORMAT_1366x768_60,        ///< 1366X768   @ 60 FPS VESA
    E_HDMI_VIDEO_FORMAT_1440x900_60,        ///< 1440X900   @ 60 FPS VESA
    E_HDMI_VIDEO_FORMAT_1600x1200_60,       ///< 1600X1200  @ 60 FPS VESA
    E_HDMI_VIDEO_FORMAT_1680x1050_60,       ///< 1680X1050  @ 60 FPS VESA
    E_HDMI_VIDEO_FORMAT_1920x1200_60,       ///< 1920X1200  @ 60 FPS VESA

    E_HDMI_VIDEO_FORMAT_2560x1440_30,       ///< 2560X1440  @ 30 FPS
    E_HDMI_VIDEO_FORMAT_2560x1440_60,       ///< 2560X1440  @ 60 FPS
    E_HDMI_VIDEO_FORMAT_2560x1600_60,       ///< 2560X1600  @ 60 FPS VESA
    E_HDMI_VIDEO_FORMAT_1920x2160_30,       ///< 1920X2160  @ 30 FPS

    E_HDMI_VIDEO_FORMAT_3840x2160P_24,      ///< 3840X2160  @ 24 FPS
    E_HDMI_VIDEO_FORMAT_3840x2160P_25,      ///< 3840X2160  @ 25 FPS
    E_HDMI_VIDEO_FORMAT_3840x2160P_30,      ///< 3840X2160  @ 30 FPS
    E_HDMI_VIDEO_FORMAT_3840x2160P_50,      ///< 3840X2160  @ 50 FPS
    E_HDMI_VIDEO_FORMAT_3840x2160P_60,      ///< 3840X2160  @ 60 FPS

    E_HDMI_VIDEO_FORMAT_4096x2160P_24,      ///< 4096X2160  @ 24 FPS
    E_HDMI_VIDEO_FORMAT_4096x2160P_25,      ///< 4096X2160  @ 25 FPS
    E_HDMI_VIDEO_FORMAT_4096x2160P_30,      ///< 4096X2160  @ 30 FPS
    E_HDMI_VIDEO_FORMAT_4096x2160P_50,      ///< 4096X2160  @ 50 FPS
    E_HDMI_VIDEO_FORMAT_4096x2160P_60,      ///< 4096X2160  @ 60 FPS

    E_HDMI_VIDEO_FORMAT_CUSTOMER_DEFINE,    ///< customer define
    E_HDMI_VIDEO_FORMAT_MAX
} vs_hdmi_video_format_e;

/**
* @brief Define the video mode
* @details Define the video mode that HDMI module can support
*/
typedef enum vs_hdmi_video_mode {
    E_HDMI_VIDEO_MODE_RGB444   = 0,     ///< RGB
    E_HDMI_VIDEO_MODE_YCBCR422,         ///< YCbCr 4:2:2
    E_HDMI_VIDEO_MODE_YCBCR444,         ///< YCbCr 4:4:4
    E_HDMI_VIDEO_MODE_YCBCR420,         ///< YCbCr 4:2:0
    E_HDMI_VIDEO_MODE_MAX
} vs_hdmi_video_mode_e;

/**
* @brief Define the quantization range
*/
typedef enum vs_hdmi_quantization {
    E_HDMI_QUANTIZATION_LIMITED_RANGE = 0,  ///< limited quantization range
    E_HDMI_QUANTIZATION_FULL_RANGE,         ///< full quantization range
    E_HDMI_QUANTIZATION_MAX
} vs_hdmi_quantization_e;

/**
* @brief Define the color depth
* @details Define the color depth that HDMI module can support
*/
typedef enum vs_hdmi_deep_color {
    E_HDMI_DEEP_COLOR_24BIT = 0,    ///< 8bit color depth
    E_HDMI_DEEP_COLOR_MAX
} vs_hdmi_deep_color_e;

/**
* @brief Define video colorimitery
*/
typedef enum vs_hdmi_video_colorimetry {
    E_HDMI_VIDEO_COLORIMETRY_ITU601 = 0,   ///< Colorimetry ITU601 option
    E_HDMI_VIDEO_COLORIMETRY_ITU709,       ///< Colorimetry ITU709 option
    E_HDMI_VIDEO_COLORIMETRY_MAX
} vs_hdmi_video_colorimetry_e;

/**
* @brief Define the HDMI audio interface type
*/
typedef enum vs_hdmi_sound_interface {
    E_HDMI_SOUND_INTERFACE_I2S   = 0,   ///< i2s interface
    E_HDMI_SOUND_INTERFACE_MAX
} vs_hdmi_sound_interface_e;

/**
* @brief Define the HDMI audio sample rate
* @details E_HDMI_SAMPLE_RATE_768K is not supported, set this value will return VS_ERR_HDMI_NOT_SUPPORT.
*/
typedef enum vs_hdmi_sample_rate {
    E_HDMI_SAMPLE_RATE_UNKNOWN  = 0,        ///< Reserved, non-configurable
    E_HDMI_SAMPLE_RATE_8K       = 8000,     ///< 8000Hz sample rate
    E_HDMI_SAMPLE_RATE_11K      = 11025,    ///< 11025Hz sample rate
    E_HDMI_SAMPLE_RATE_12K      = 12000,    ///< 12000Hz sample rate
    E_HDMI_SAMPLE_RATE_16K      = 16000,    ///< 16000Hz sample rate
    E_HDMI_SAMPLE_RATE_22K      = 22050,    ///< 22050Hz sample rate
    E_HDMI_SAMPLE_RATE_24K      = 24000,    ///< 24000Hz sample rate
    E_HDMI_SAMPLE_RATE_32K      = 32000,    ///< 32000Hz sample rate
    E_HDMI_SAMPLE_RATE_44K      = 44100,    ///< 44100Hz sample rate
    E_HDMI_SAMPLE_RATE_48K      = 48000,    ///< 48000Hz sample rate
    E_HDMI_SAMPLE_RATE_88K      = 88200,    ///< 88200Hz sample rate
    E_HDMI_SAMPLE_RATE_96K      = 96000,    ///< 96000Hz sample rate
    E_HDMI_SAMPLE_RATE_176K     = 176400,   ///< 176400Hz sample rate
    E_HDMI_SAMPLE_RATE_192K     = 192000,   ///< 192000Hz sample rate
    E_HDMI_SAMPLE_RATE_768K     = 768000,   ///< 768000Hz sample rate
    E_HDMI_SAMPLE_RATE_MAX
} vs_hdmi_sample_rate_e;

/**
* @brief Define the HDMI audio bit depth
* @details E_HDMI_BIT_DEPTH_8, E_HDMI_BIT_DEPTH_18, E_HDMI_BIT_DEPTH_20 and E_HDMI_BIT_DEPTH_32
* are not supported, set these value will return VS_ERR_HDMI_NOT_SUPPORT.
*/
typedef enum vs_hdmi_bit_depth {
    E_HDMI_BIT_DEPTH_UNKNOWN    = 0,    ///< Reserved, non-configurable
    E_HDMI_BIT_DEPTH_8          = 8,    ///< 8 bit depth
    E_HDMI_BIT_DEPTH_16         = 16,   ///< 16 bit depth
    E_HDMI_BIT_DEPTH_18         = 18,   ///< 18 bit depth
    E_HDMI_BIT_DEPTH_20         = 20,   ///< 20 bit depth
    E_HDMI_BIT_DEPTH_24         = 24,   ///< 24 bit depth
    E_HDMI_BIT_DEPTH_32         = 32,   ///< 32 bit depth
    E_HDMI_BIT_DEPTH_MAX
} vs_hdmi_bit_depth_e;

/**
* @brief Define the placement of HDMI audio speakers
*/
typedef enum vs_hdmi_audio_speaker {
    E_HDMI_AUDIO_SPEAKER_FL_FR   = 0,   ///< Front Left and Front Right
    E_HDMI_AUDIO_SPEAKER_LFE,           ///< Low Frequency Effect
    E_HDMI_AUDIO_SPEAKER_FC,            ///< Front Center
    E_HDMI_AUDIO_SPEAKER_RL_RR,         ///< Rear Left
    E_HDMI_AUDIO_SPEAKER_RC,            ///< Rear Center
    E_HDMI_AUDIO_SPEAKER_FLC_FRC,       ///< Front Left Center and Front Right Center
    E_HDMI_AUDIO_SPEAKER_RLC_RRC,       ///< Rear Left Center and Rear Right Center
    E_HDMI_AUDIO_SPEAKER_FLW_FRW,       ///< Front Left Wide and Front Right Wide
    E_HDMI_AUDIO_SPEAKER_FLH_FRH,       ///< Front Left High and Front Right High
    E_HDMI_AUDIO_SPEAKER_TC,            ///< Top Center
    E_HDMI_AUDIO_SPEAKER_FCH,           ///< Front Center High
    E_HDMI_AUDIO_SPEAKER_MAX
} vs_hdmi_audio_speaker_e;

/**
* @brief Define the force output mode when get edid fail
*/
typedef enum vs_hdmi_force_action {
    E_HDMI_FORCE_NULL     = 0,  ///< Invalid parameter
    E_HDMI_FORCE_HDMI,          ///< Forcibly output video in HDMI mode
    E_HDMI_FORCE_DVI,           ///< Forcibly output video in DVI mode
    E_HDMI_FORCE_MAX
} vs_hdmi_force_action_e;


/**
* @brief Define the audio coding type
*/
typedef enum vs_hdmi_audio_format_code {
    E_HDMI_AUDIO_FORMAT_CODE_RESERVED  = 0, ///< refer to stream header
    E_HDMI_AUDIO_FORMAT_CODE_PCM,           ///< L-PCM audio coding type
    E_HDMI_AUDIO_FORMAT_CODE_AC3,           ///< AC-3 audio coding type
    E_HDMI_AUDIO_FORMAT_CODE_MPEG1,         ///< MPEG-1 audio coding type
    E_HDMI_AUDIO_FORMAT_CODE_MP3,           ///< MP3 audio coding type
    E_HDMI_AUDIO_FORMAT_CODE_MPEG2,         ///< MPEG2 audio coding type
    E_HDMI_AUDIO_FORMAT_CODE_AAC,           ///< AAC LC audio coding type
    E_HDMI_AUDIO_FORMAT_CODE_DTS,           ///< DTS audio coding type
    E_HDMI_AUDIO_FORMAT_CODE_ATRAC,         ///< ATRAC audio coding type
    E_HDMI_AUDIO_FORMAT_CODE_ONE_BIT,       ///< One Bit Audio audio coding type
    E_HDMI_AUDIO_FORMAT_CODE_DDP,           ///< Enhanced AC-3 audio coding type
    E_HDMI_AUDIO_FORMAT_CODE_DTS_HD,        ///< DTS-HD audio coding type
    E_HDMI_AUDIO_FORMAT_CODE_MAT,           ///< MAT audio coding type
    E_HDMI_AUDIO_FORMAT_CODE_DST,           ///< DST audio coding type
    E_HDMI_AUDIO_FORMAT_CODE_WMA_PRO,       ///< WMA Pro audio coding type
    E_HDMI_AUDIO_FORMAT_CODE_MAX
} vs_hdmi_audio_format_code_e;


/**
* @brief Define the HDMI infoframe type
*/
typedef enum vs_hdmi_infoframe_type {
    E_HDMI_INFOFRAME_TYPE_AVI         = 0,  ///< AVI infoframe
    E_HDMI_INFOFRAME_TYPE_AUDIO,            ///< Audio infoframe
    E_HDMI_INFOFRAME_TYPE_MAX
} vs_hdmi_infoframe_type_e;


/**
* @brief Define the color space of AVI infoframe
*/
typedef enum vs_hdmi_color_space {
    E_HDMI_COLOR_SPACE_RGB444     = 0,      ///< RGB
    E_HDMI_COLOR_SPACE_YCBCR422,            ///< YCbCr 4:2:2
    E_HDMI_COLOR_SPACE_YCBCR444,            ///< YCbCr 4:4:4
    E_HDMI_COLOR_SPACE_YCBCR420,            ///< YCbCr 4:2:0
    E_HDMI_COLOR_SPACE_MAX
} vs_hdmi_color_space_e;

/**
* @brief Define the Bar Data Present of AVI infoframe
*/
typedef enum vs_hdmi_bar_data {
    E_HDMI_BAR_DATA_NOT_VALID   = 0,    ///< Bar Data not present
    E_HDMI_BAR_DATA_V,                  ///< Vertical Bar Data present
    E_HDMI_BAR_DATA_H,                  ///< Horizontal Bar Data present
    E_HDMI_BAR_DATA_VH,                 ///< Vertical and Horizontal Bar Data present
    E_HDMI_BAR_DATA_MAX
} vs_hdmi_bar_data_e;

/**
* @brief Define the Scan Information of AVI infoframe
*/
typedef enum vs_hdmi_scan_info {
    E_HDMI_SCAN_INFO_NO_DATA    = 0,    ///< No Scan information
    E_HDMI_SCAN_INFO_OVERSCANNED,       ///< Overscan
    E_HDMI_SCAN_INFO_UNDERSCANNED,      ///< Underscan
    E_HDMI_SCAN_INFO_RESERVED,          ///< Reserved
    E_HDMI_SCAN_INFO_MAX
} vs_hdmi_scan_info_e;

/**
* @brief Define the colorimetry of AVI infoframe
*/
typedef enum vs_hdmi_colorimetry {
    E_HDMI_COLORIMETRY_NO_DATA     = 0, ///< Colorimetry No Data option
    E_HDMI_COLORIMETRY_ITU601,          ///< Colorimetry ITU601 option
    E_HDMI_COLORIMETRY_ITU709,          ///< Colorimetry ITU709 option
    E_HDMI_COLORIMETRY_EXTENDED,        ///< Colorimetry extended option
    E_HDMI_COLORIMETRY_MAX
} vs_hdmi_colorimetry_e;

/**
* @brief Define the extend colorimetry of AVI infoframe
*/
typedef enum vs_hdmi_extend_colorimetry {
    E_HDMI_EXTEND_COLORIMETRY_XVYCC_601    = 0,         ///< Colorimetry xvYCC601 extended option
    E_HDMI_EXTEND_COLORIMETRY_XVYCC_709,                ///< Colorimetry xvYCC709 extended option
    E_HDMI_EXTEND_COLORIMETRY_SYCC_601,                 ///< Colorimetry sYCC 601 extended option
    E_HDMI_EXTEND_COLORIMETRY_OPYCC_601,                ///< Colorimetry opYCC 601 extended option
    E_HDMI_EXTEND_COLORIMETRY_OPRGB,                    ///< Colorimetry opRGB extended option
    E_HDMI_EXTEND_COLORIMETRY_BT2020_YCCBCCRC,          ///< Colorimetry BT2020 YcCbcCrc extended option
    E_HDMI_EXTEND_COLORIMETRY_BT2020_RGB_YCC,           ///< Colorimetry BT2020 RGB or Ycc extended option
    E_HDMI_EXTEND_COLORIMETRY_RESERVED,                 ///< Reserved
    E_HDMI_EXTEND_COLORIMETRY_MAX
} vs_hdmi_extend_colorimetry_e;

/**
* @brief Define the picture aspect ratio
*/
typedef enum vs_hdmi_picture_aspect_ratio {
    E_HDMI_PICTURE_ASPECT_RATIO_NO_DATA      = 0,   ///< Unknown picture aspect ratio
    E_HDMI_PICTURE_ASPECT_RATIO_4TO3,               ///< 4:3 picture aspect ratio
    E_HDMI_PICTURE_ASPECT_RATIO_16TO9,              ///< 16:9 picture aspect ratio
    E_HDMI_PICTURE_ASPECT_RATIO_64TO27,             ///< 64:27 picture aspect ratio
    E_HDMI_PICTURE_ASPECT_RATIO_256TO135,           ///< 256:135 picture aspect ratio
    E_HDMI_PICTURE_ASPECT_RATIO_RESERVED,           ///< Reserved
    E_HDMI_PICTURE_ASPECT_RATIO_MAX
} vs_hdmi_picture_aspect_ratio_e;

/**
* @brief Define the Active Portion Aspect Ratio of AVI infoframe
*/
typedef enum vs_hdmi_active_aspect_ratio {
    E_HDMI_ACTIVE_ASPECT_RATIO_RESERVED_0      = 0, ///< Reserved
    E_HDMI_ACTIVE_ASPECT_RATIO_RESERVED_1,          ///< Reserved
    E_HDMI_ACTIVE_ASPECT_RATIO_16TO9_TOP,           ///< 16:9 top
    E_HDMI_ACTIVE_ASPECT_RATIO_14TO9_TOP,           ///< 14:9 top
    E_HDMI_ACTIVE_ASPECT_RATIO_16TO9_BOX_CENTER,    ///< Greater than 16:9 letterbox image
    E_HDMI_ACTIVE_ASPECT_RATIO_RESERVED_5,          ///< Reserved
    E_HDMI_ACTIVE_ASPECT_RATIO_RESERVED_6,          ///< Reserved
    E_HDMI_ACTIVE_ASPECT_RATIO_RESERVED_7,          ///< Reserved
    E_HDMI_ACTIVE_ASPECT_RATIO_SAME_PIC,            ///< 4:3 full frame image
    E_HDMI_ACTIVE_ASPECT_RATIO_4TO3_CENTER,         ///< 4:3 full frame image
    E_HDMI_ACTIVE_ASPECT_RATIO_16TO9_CENTER,        ///< 16:9 letterbox image
    E_HDMI_ACTIVE_ASPECT_RATIO_14TO9_CENTER,        ///< 14:9 letterbox image
    E_HDMI_ACTIVE_ASPECT_RATIO_RESERVED_12,         ///< Reserved
    E_HDMI_ACTIVE_ASPECT_RATIO_4TO3_14_9,           ///< 4:3 letterbox image, alternative 14:9 center
    E_HDMI_ACTIVE_ASPECT_RATIO_16TO9_14_9,          ///< 16:9 letterbox image, alternative 14:9 center
    E_HDMI_ACTIVE_ASPECT_RATIO_16TO9_4_3,           ///< 16:9 letterbox image, alternative 4:3 center
    E_HDMI_ACTIVE_ASPECT_RATIO_MAX
} vs_hdmi_active_aspect_ratio_e;

/**
* @brief Define the Non-Uniform Picture Scaling of AVI infoframe
*/
typedef enum vs_hdmi_picture_scaling {
    E_HDMI_PICTURE_NON_UNIFORM_SCALING  = 0,    ///< No Known, non-uniform Scaling
    E_HDMI_PICTURE_SCALING_H,                   ///< Picture has been scaled horizontally
    E_HDMI_PICTURE_SCALING_V,                   ///< Picture has been scaled Vertically
    E_HDMI_PICTURE_SCALING_HV,                  ///< Picture has been scaled Horizontally and Vertically
    E_HDMI_PICTURE_SCALING_MAX
} vs_hdmi_picture_scaling_e;

/**
* @brief Define the RGB Quantization Range of AVI infoframe
*/
typedef enum vs_hdmi_rgb_quantization {
    E_HDMI_RGB_QUANTIZATION_DEFAULT_RANGE    = 0,   ///< Default, depends on video format
    E_HDMI_RGB_QUANTIZATION_LIMITED_RANGE,          ///< Limited Range of RGB quantization
    E_HDMI_RGB_QUANTIZATION_FULL_RANGE,             ///< Full Range of RGB quantization
    E_HDMI_RGB_QUANTIZATION_RESERVED,               ///< Reserved
    E_HDMI_RGB_QUANTIZATION_MAX
} vs_hdmi_rgb_quantization_e;

/**
* @brief Define the Pixel Repetition Factor of AVI infoframe
*/
typedef enum vs_hdmi_pixel_repetition {
    E_HDMI_PIXEL_REPETITION_NO       = 0,   ///< No Repetition
    E_HDMI_PIXEL_REPETITION_2_TIMES,        ///< Pixel Data sent 2 times
    E_HDMI_PIXEL_REPETITION_3_TIMES,        ///< Pixel Data sent 3 times
    E_HDMI_PIXEL_REPETITION_4_TIMES,        ///< Pixel Data sent 4 times
    E_HDMI_PIXEL_REPETITION_5_TIMES,        ///< Pixel Data sent 5 times
    E_HDMI_PIXEL_REPETITION_6_TIMES,        ///< Pixel Data sent 6 times
    E_HDMI_PIXEL_REPETITION_7_TIMES,        ///< Pixel Data sent 7 times
    E_HDMI_PIXEL_REPETITION_8_TIMES,        ///< Pixel Data sent 8 times
    E_HDMI_PIXEL_REPETITION_9_TIMES,        ///< Pixel Data sent 9 times
    E_HDMI_PIXEL_REPETITION_10_TIMES,       ///< Pixel Data sent 10 times
    E_HDMI_PIXEL_REPETITION_RESERVED_A,     ///< Reserved
    E_HDMI_PIXEL_REPETITION_RESERVED_B,     ///< Reserved
    E_HDMI_PIXEL_REPETITION_RESERVED_C,     ///< Reserved
    E_HDMI_PIXEL_REPETITION_RESERVED_D,     ///< Reserved
    E_HDMI_PIXEL_REPETITION_RESERVED_E,     ///< Reserved
    E_HDMI_PIXEL_REPETITION_RESERVED_F,     ///< Reserved
    E_HDMI_PIXEL_REPETITION_MAX
} vs_hdmi_pixel_repetition_e;

/**
* @brief  Define the YCC Quantization Range of AVI infoframe
*/
typedef enum vs_hdmi_ycc_quantization {
    E_HDMI_YCC_QUANTIZATION_LIMITED_RANGE = 0,  ///< Limited Range of ycc quantization
    E_HDMI_YCC_QUANTIZATION_FULL_RANGE,         ///< Full Range of ycc quantization
    E_HDMI_YCC_QUANTIZATION_RESERVED_2,         ///< Reserved
    E_HDMI_YCC_QUANTIZATION_RESERVED_3,         ///< Reserved
    E_HDMI_YCC_QUANTIZATION_MAX
} vs_hdmi_ycc_quantization_e;

/**
* @brief Define the IT Content Type of AVI infoframe
*/
typedef enum vs_hdmi_content_type {
    E_HDMI_CONTENT_GRAPHICS      = 0,   ///< Graphics type
    E_HDMI_CONTENT_PHOTO,               ///< Photo type
    E_HDMI_CONTENT_CINEMA,              ///< Cinema type
    E_HDMI_CONTENT_GAME,                ///< Game type
    E_HDMI_CONTENT_MAX
} vs_hdmi_content_type_e;

/**
* @brief Define the Audio Channel Count of Audio infoframe
*/
typedef enum vs_hdmi_audio_chn_cnt {
    E_HDMI_AUDIO_CHN_CNT_STREAM    = 0, ///< Refer to Stream Header
    E_HDMI_AUDIO_CHN_CNT_2,             ///< 2 channels
    E_HDMI_AUDIO_CHN_CNT_3,             ///< 3 channels
    E_HDMI_AUDIO_CHN_CNT_4,             ///< 4 channels
    E_HDMI_AUDIO_CHN_CNT_5,             ///< 5 channels
    E_HDMI_AUDIO_CHN_CNT_6,             ///< 6 channels
    E_HDMI_AUDIO_CHN_CNT_7,             ///< 7 channels
    E_HDMI_AUDIO_CHN_CNT_8,             ///< 8 channels
    E_HDMI_AUDIO_CHN_CNT_MAX
} vs_hdmi_audio_chn_cnt_e;

/**
* @brief Define the Audio Coding Type of Audio infoframe
*/
typedef enum vs_hdmi_audio_coding_type {
    E_HDMI_AUDIO_CODING_STREAM = 0,     ///< Refer to Stream Header
    E_HDMI_AUDIO_CODING_PCM,            ///< L-PCM audio coding type
    E_HDMI_AUDIO_CODING_AC3,            ///< AC-3 audio coding type
    E_HDMI_AUDIO_CODING_MPEG1,          ///< MPEG-1 audio coding type
    E_HDMI_AUDIO_CODING_MP3,            ///< MP3 audio coding type
    E_HDMI_AUDIO_CODING_MPEG2,          ///< MPEG2 audio coding type
    E_HDMI_AUDIO_CODING_AACLC,          ///< AAC LC audio coding type
    E_HDMI_AUDIO_CODING_DTS,            ///< DTS audio coding type
    E_HDMI_AUDIO_CODING_ATRAC,          ///< ATRAC audio coding type
    E_HDMI_AUDIO_CODING_ONE_BIT_AUDIO,  ///< One Bit Audio audio coding type
    E_HDMI_AUDIO_CODING_ENHANCED_AC3,   ///< Enhanced AC-3 audio coding type
    E_HDMI_AUDIO_CODING_DTS_HD,         ///< DTS-HD audio coding type
    E_HDMI_AUDIO_CODING_MAT,            ///< MAT audio coding type
    E_HDMI_AUDIO_CODING_DST,            ///< DST audio coding type
    E_HDMI_AUDIO_CODING_WMA_PRO,        ///< WMA Pro audio coding type
    E_HDMI_AUDIO_CODING_MAX
} vs_hdmi_audio_coding_type_e;

/**
* @brief Define the Sample Size of Audio infoframe
*/
typedef enum vs_hdmi_audio_sample_size {
    E_HDMI_AUDIO_SAMPLE_SIZE_STREAM = 0,    ///< default, Refer to Stream header
    E_HDMI_AUDIO_SAMPLE_SIZE_16,            ///< 16 bit width
    E_HDMI_AUDIO_SAMPLE_SIZE_20,            ///< 20 bit width
    E_HDMI_AUDIO_SAMPLE_SIZE_24,            ///< 24 bit width
    E_HDMI_AUDIO_SAMPLE_SIZE_MAX
} vs_hdmi_audio_sample_size_e;

/**
* @brief Define the Sampling Frequency of Audio infoframe
*/
typedef enum vs_hdmi_audio_sample_freq {
    E_HDMI_AUDIO_SAMPLE_FREQ_STREAM = 0,    ///< Refer to Stream Header
    E_HDMI_AUDIO_SAMPLE_FREQ_32000,         ///< 32KHz
    E_HDMI_AUDIO_SAMPLE_FREQ_44100,         ///< 44.1KHz
    E_HDMI_AUDIO_SAMPLE_FREQ_48000,         ///< 48KHz
    E_HDMI_AUDIO_SAMPLE_FREQ_88200,         ///< 88.2KHz
    E_HDMI_AUDIO_SAMPLE_FREQ_96000,         ///< 96KHz
    E_HDMI_AUDIO_SAMPLE_FREQ_176400,        ///< 176.4KHz
    E_HDMI_AUDIO_SAMPLE_FREQ_192000,        ///< 192KHz
    E_HDMI_AUDIO_SAMPLE_FREQ_MAX
} vs_hdmi_audio_sample_freq_e;

/**
* @brief Define the Level Shift Value of Audio infoframe
*/
typedef enum vs_hdmi_level_shift_value {
    E_HDMI_LEVEL_SHIFT_VALUE_0_DB = 0,  ///< 0db
    E_HDMI_LEVEL_SHIFT_VALUE_1_DB,      ///< 1db
    E_HDMI_LEVEL_SHIFT_VALUE_2_DB,      ///< 2db
    E_HDMI_LEVEL_SHIFT_VALUE_3_DB,      ///< 3db
    E_HDMI_LEVEL_SHIFT_VALUE_4_DB,      ///< 4db
    E_HDMI_LEVEL_SHIFT_VALUE_5_DB,      ///< 5db
    E_HDMI_LEVEL_SHIFT_VALUE_6_DB,      ///< 6db
    E_HDMI_LEVEL_SHIFT_VALUE_7_DB,      ///< 7db
    E_HDMI_LEVEL_SHIFT_VALUE_8_DB,      ///< 8db
    E_HDMI_LEVEL_SHIFT_VALUE_9_DB,      ///< 9db
    E_HDMI_LEVEL_SHIFT_VALUE_10_DB,     ///< 10db
    E_HDMI_LEVEL_SHIFT_VALUE_11_DB,     ///< 11db
    E_HDMI_LEVEL_SHIFT_VALUE_12_DB,     ///< 12db
    E_HDMI_LEVEL_SHIFT_VALUE_13_DB,     ///< 13db
    E_HDMI_LEVEL_SHIFT_VALUE_14_DB,     ///< 14db
    E_HDMI_LEVEL_SHIFT_VALUE_15_DB,     ///< 15db
    E_HDMI_LEVEL_SHIFT_VALUE_MAX
} vs_hdmi_level_shift_value_e;

/**
* @brief Define the LFE playback level of Audio infoframe
* @details Describes what value is used for LFE playback level comparing with
* other channel signal.
*/
typedef enum vs_hdmi_lfe_playback_level {
    E_HDMI_LFE_PLAYBACK_LEVEL_NO         = 0,   ///< Unknown or refer to other information
    E_HDMI_LFE_PLAYBACK_LEVEL_0_DB,             ///< 0 dB playback
    E_HDMI_LFE_PLAYBACK_LEVEL_10_DB,            ///< + 10 dB playback
    E_HDMI_LFE_PLAYBACK_LEVEL_RESERVED,          ///< Reserved
    E_HDMI_LFE_PLAYBACK_LEVEL_MAX
} vs_hdmi_lfe_playback_level_e;

/**
* @brief HDMI event type
*/
typedef enum vs_hdmi_event_type {
    E_HDMI_EVENT_HOTPLUG_IN     = 0,    ///< HDMI hot plug in event
    E_HDMI_EVENT_HOTPLUG_OUT,           ///< HDMI hot plug out event
    E_HDMI_EVENT_EDID_FAIL,             ///< HDMI EDID read failure event
    E_HDMI_EVENT_MAX
} vs_hdmi_event_type_e;

/**
* @brief Phy parameters selection
* @details These parameters should be configure when using long cable. And these parameters do not correspond to specific
* cable lengths. But it is confirmed that the longer the cable length, the bigger phy parameter value should be used.
* Normally, parameters 1~2 are used for HDMI1.4(<1.65Gbps), parameters 3~6 are used for HDMI1.4(>1.65Gbps), and parameters
* 7~10 are used for HDMI2.0.
*/
typedef enum vs_hdmi_trace_len {
    E_HDMI_TRACE_LEN_NONE     = 0,  ///< Phy parameter default
    E_HDMI_TRACE_LEN_1,             ///< Phy parameter 1
    E_HDMI_TRACE_LEN_2,             ///< Phy parameter 2
    E_HDMI_TRACE_LEN_3,             ///< Phy parameter 3
    E_HDMI_TRACE_LEN_4,             ///< Phy parameter 4
    E_HDMI_TRACE_LEN_5,             ///< Phy parameter 5
    E_HDMI_TRACE_LEN_6,             ///< Phy parameter 6
    E_HDMI_TRACE_LEN_7,             ///< Phy parameter 7
    E_HDMI_TRACE_LEN_8,             ///< Phy parameter 8
    E_HDMI_TRACE_LEN_9,             ///< Phy parameter 9
    E_HDMI_TRACE_LEN_10,            ///< Phy parameter 10
    E_HDMI_TRACE_LEN_MAX
} vs_hdmi_trace_len_e;

/**
* @brief The detail of Audio infoframe
* @details HDMI Audio infoframe struct, see CEA-861-F for details.
*/
typedef struct vs_hdmi_audio_infoframe_ver1 {
    vs_hdmi_audio_chn_cnt_e        chn_cnt;          ///< RW; Range:[0, 7]; CC2 CC1 CC0, Audio Channel Count
    vs_hdmi_audio_coding_type_e    coding_type;      ///< RW; Range:[0, 14]; CT3 CT2 CT1 CT0, Audio Coding Type
    vs_hdmi_audio_sample_size_e    sample_size;      ///< RW; Range:[0, 3]; SS1 SS0, Sample Size
    vs_hdmi_audio_sample_freq_e    sample_freq;      ///< RW; Range:[0, 7]; SF2 SF1 SF0, Sampling Frequency
    vs_uint8_t                     chn_alloc;        ///< RW; Range:[0, 0x31]; CA7~CA0, channel allocation
    vs_hdmi_level_shift_value_e    level_shift;      ///< RW; Range:[0, 16]; LSV3 LSV2 LSV1 LSV0, Level Shift Value
    vs_hdmi_lfe_playback_level_e   lfe_playback_level;   ///< RW; Range:[0, 2]; LFEPBL1 LFEPBL0, Describes what value is used for LFE playback level comparing with other channel signal
    vs_bool_t                      downmix_inhibit;  ///< RW; Range:[0, 1]; DM_INH, Describes whether the down mixed stereo output is permitted or not
} vs_hdmi_audio_infoframe_ver1_s;

/**
* @brief The detail of AVI infoframe
* @details HDMI AVI infoframe struct, see CEA-861-F for details.
*/
typedef struct vs_hdmi_avi_infoframe_ver3 {
    vs_hdmi_video_format_e              timing_mode;         ///< RW; Range:[0, 12], [14, 38]; VIC, corresponding video resolution
    vs_hdmi_color_space_e               color_space;         ///< RW; Range:[0, 3]; Y2 Y1 0, RGB or YCbCr
    vs_bool_t                           active_info_present; ///< RW; Range:[0, 1]; A0, Active Format Information Present
    vs_hdmi_bar_data_e                  bar_data;            ///< RW; Range:[0, 3]; B1 B0, Bar Data Present
    vs_hdmi_scan_info_e                 scan_info;           ///< RW; Range:[0, 2]; S1 S0, Scan Information
    vs_hdmi_colorimetry_e               colorimetry;         ///< RW; Range:[0, 3]; C1 C0, Colorimetry
    vs_hdmi_extend_colorimetry_e        extend_colorimetry;  ///< RW; Range:[0, 6]; EC2 EC1 EC0, Extend Colorimetry
    vs_hdmi_picture_aspect_ratio_e      aspect_ratio;        ///< RW; Range:[0, 4]; M1 M0, Picture Aspect Ratio
    vs_hdmi_active_aspect_ratio_e       active_aspect_ratio; ///< RW; R3 R2 R1 R0, Active Portion Aspect Ratio. RESERVED members of enum are not supported
    vs_hdmi_picture_scaling_e           picture_scaling;     ///< RW; Range:[0, 3]; SC1 SC0, Non-Uniform Picture Scaling
    vs_hdmi_rgb_quantization_e          rgb_quantization;    ///< RW; Range:[0, 2]; Q1 Q0, RGB Quantization Range
    vs_bool_t                           is_it_content;       ///< RW; Range:[0, 1]; ITC, IT content
    vs_hdmi_pixel_repetition_e          pixel_repetition;    ///< RW; Range:[0, 9]; PR3 PR2 PR1 PR0, Pixel Repetition Factor
    vs_hdmi_content_type_e              content_type;        ///< RW; Range:[0, 3]; CN1 CN0, IT Content Type
    vs_hdmi_ycc_quantization_e          ycc_quantization;    ///< RW; Range:[0, 1]; YQ1 YQ0, YCC Quantization Range
    vs_uint16_t                         line_num_end_of_top_bar;        ///< Line Number of End of Top Bar
    vs_uint16_t                         line_num_start_of_bottom_bar;   ///< Line Number of Start of Bottom Bar
    vs_uint16_t                         line_num_end_of_left_bar;       ///< Pixel Number of End of Left Bar
    vs_uint16_t                         line_num_start_of_right_bar;    ///< Pixel Number of Start of Right Bar
} vs_hdmi_avi_infoframe_ver3_s;

/**
* @brief HDMI infoframe option
*/
typedef union vs_hdmi_infoframe_unit {
    vs_hdmi_avi_infoframe_ver3_s             avi_infoframe;      ///< HDMI AVI infoframe
    vs_hdmi_audio_infoframe_ver1_s           audio_infoframe;    ///< HDMI Audio infoframe
} vs_hdmi_infoframe_unit_u;

/**
* @brief Audio capability of Sink
*/
typedef struct vs_hdmi_audio_info {
    vs_hdmi_audio_format_code_e     audio_format_code;                             ///< audio format code
    vs_hdmi_sample_rate_e           support_sample_rate[HDMI_MAX_SIMPLE_RATE_NUM]; ///< audio sample rate
    vs_uint8_t                      audio_chn;                                     ///< Max number of channels of audio coding type
    vs_hdmi_bit_depth_e             support_bit_depth[HDMI_MAX_BIT_DEPTH_NUM];     ///< sample bit depth
    vs_uint32_t                     support_bit_depth_num;                         ///< sample bit depth number
    vs_uint32_t                     max_bit_rate;                                  ///< max bit rate
} vs_hdmi_audio_info_s;

/**
* @brief Define HDMI timing information
* @details This structure is usually used to store timing read from EDID, but when the value of
* video_format in vs_hdmi_attr_s is E_HDMI_VIDEO_FORMAT_CUSTOMER_DEFINE, this structure can be
* used to configure customer-defined timing. In this case, image_width and image_height do not take
* effect and the value is always 0.
*/
typedef struct vs_hdmi_timing_info {
    vs_uint32_t         vfp;                ///< vertical front porch
    vs_uint32_t         vpw;                ///< vertical pulse width
    vs_uint32_t         vbp;                ///< vertical back porch
    vs_uint32_t         vactive;            ///< vertical active
    vs_uint32_t         hfp;                ///< horizontal front porch
    vs_uint32_t         hpw;                ///< horizontal pulse width
    vs_uint32_t         hbp;                ///< horizontal back porch
    vs_uint32_t         hactive;            ///< horizontal active
    vs_bool_t           hsync_polarity;     ///< RW; Range:[0, 1]; horizontal sync polarity
    vs_bool_t           vsync_polarity;     ///< RW; Range:[0, 1]; vertical sync polarity
    vs_uint32_t         image_width;        ///< image width(mm), invalid for customer-defined timing
    vs_uint32_t         image_height;       ///< image height(mm), invalid for customer-defined timing
    vs_bool_t           is_interlace;       ///< RW; Range:[0, 1]; whether the timing is interlaced
    vs_uint32_t         pixel_clk;          ///< RW; Range:[0, 594000]; pixel clock(KHz)
} vs_hdmi_timing_info_s;

/**
* @brief Detailed timing of EDID
*/
typedef struct vs_hdmi_detailed_timing {
    vs_uint32_t             detailed_timing_num;                            ///< detailed timing num of EDID
    vs_hdmi_timing_info_s   detailed_timing[HDMI_MAX_DETAILED_TIMING_NUM];  ///< detailed timing information
} vs_hdmi_detailed_timing_s;

/**
* @brief Define the capability set of sink
*/
typedef struct vs_hdmi_sink_cap {
    vs_bool_t                   is_connected;                   ///< Whether the devices are connected.
    vs_bool_t                   hdmi_supported;                 ///< Whether the HDMI is supported by device. If HDMI is not supported, the device device is DVI.
    vs_bool_t                   is_sink_poweron;                ///< Whether the device device is power on.
    vs_bool_t                   is_real_edid;                   ///< Whether the obtain the EDID from device. VS_FALSE:the feather below will be invalid

    vs_hdmi_video_format_e      native_video_format;            ///< Physical resolution of the display device.
    vs_uint64_t                 video_format_supported;         ///< Bitmap for video capability set.
    vs_bool_t                   ycbcr_supported;                ///< Whether the YCBCR display is supported. VS_TRUE: The YCbCr mode is supported.

    vs_bool_t                   xvycc601_supported;             ///< Whether the xvYCC601 colorimetry is supported.
    vs_bool_t                   xvycc709_supported;             ///< Whether the xvYCC709 colorimetry is supported.
    vs_uint8_t                  md_bit;                         ///< Colorimetry Metadata Support Flags
    vs_uint32_t                 audio_info_num;                 ///< Number of audio information
    vs_hdmi_audio_info_s        audio_info[HDMI_MAX_AUDIO_CAP_COUNT];           ///< Audio information. For details, see the definition of Audio Data Block in CEA-861-F.

    vs_uint16_t                 speaker_supported;              ///< Bitmap for speaker placement. For details, see the definition of Speaker Date Block in CEA-861-F.
    vs_uint8_t                  id_manufacture_name[HDMI_MANUFACTURE_ID_LEN];   ///< Device manufacture name.
    vs_uint32_t                 id_product_code;                ///< Device product code.
    vs_uint32_t                 id_serial_number;               ///< Device serial number.
    vs_uint32_t                 week_of_manufacture;            ///< Device production data (week).
    vs_uint32_t                 year_of_manufacture;            ///< Device production data (year).
    vs_uint8_t                  version;                        ///< EDID version number.
    vs_uint8_t                  revision;                       ///< EDID sub version number.
    vs_uint8_t                  edid_extern_block_num;          ///< EDID extended block number.

    vs_bool_t                   dvi_dual_supported;             ///< Whether the DVI dual-link operation is supported.
    vs_bool_t                   deep_color_ycc444_supported;    ///< Whether the YCbCr 4:4:4 deep-color mode is supported.
    vs_bool_t                   deep_color_30bit_supported;     ///< Whether the deep-color 30-bit is supported.
    vs_bool_t                   deep_color_36bit_supported;     ///< Whether the deep-color 30-bit is supported.
    vs_bool_t                   deep_color_48bit_supported;     ///< Whether the deep-color 30-bit is supported.
    vs_bool_t                   ai_supported;                   ///< Whether the AI mode is supported.
    vs_uint32_t                 max_tmds_clock;                 ///< Maximum TMDS clock.

    vs_bool_t                   latency_fields_present;         ///< Delay flag bit.
    vs_bool_t                   i_latency_fields_present;       ///< Whether Interlaced_Video_Latency and Interlaced_Audio_Latency fields are present
    vs_bool_t                   hdmi_video_present;             ///< Whether Video_Latency and Audio_Latency fields are present
    vs_uint8_t                  video_latency;                  ///< Indicate the amount of video latency when receiving any video format or only when receiving progressive video formats.
    vs_uint8_t                  audio_latency;                  ///< Indicate the amount of audio latency when receiving any video format or only when receiving progressive video formats.
    vs_uint8_t                  interlaced_video_latency;       ///< Indicate the amount of video latency when receiving an interlaced video format.
    vs_uint8_t                  interlaced_audio_latency;       ///< Indicate the amount of audio latency when receiving an interlaced video format.
    vs_bool_t                   y420_dc_30bit_supported;        ///< Whether YCbCr 4:2:0 deep-color 30-bit mode is supported.
    vs_bool_t                   y420_dc_36bit_supported;        ///< Whether YCbCr 4:2:0 deep-color 36-bit mode is supported.
    vs_bool_t                   y420_dc_48bit_supported;        ///< Whether YCbCr 4:2:0 deep-color 48-bit mode is supported.
    vs_bool_t                   hdmi_2_0_supported;             ///< Whether HDMI 2.0 is supported.
    vs_uint64_t                 y420_format_supported;          ///< Bitmap for the video format of YCbCr 4:2:0 that supported
    vs_uint64_t                 only_y420_format_supported;     ///< Bitmap for the video format of YCbCr 4:2:0 that only supported

    vs_bool_t                   ycc_quantization_range_selectable;   ///< Whether YCC Quantization Range is selectable.
    vs_bool_t                   rgb_quantization_range_selectable;   ///< Whether RGB Quantization Range is selectable.
    vs_hdmi_detailed_timing_s   detailed_timings;               ///< Detailed timing information.
} vs_hdmi_sink_cap_s;

/**
* @brief Define the configuration of HDMI
*/
typedef struct vs_hdmi_attr {
    vs_bool_t                   hdmi_enable;            ///< RW; Range:[0, 1]; VS_TRUE: force HDMI output. VS_FALSE: select output mode according to EDID automatically.
                                                        ///< If get EDID fail, refer default_mode below. Default: VS_FALSE
    vs_bool_t                   video_enable;           ///< RW; Range:[0, 1]; Whether to output video. VS_FALSE: mute video output. Default: VS_TRUE.
    vs_hdmi_video_format_e      video_format;           ///< RW; Range:[0, 40]; Supported video format. Default E_HDMI_VIDEO_FORMAT_1080P_60.
    vs_hdmi_video_mode_e        video_in_mode;          ///< RW; Range:[0, 0] & [2, 2]; HDMI input colorspace. Default: E_HDMI_VIDEO_MODE_RGB444.
    vs_hdmi_video_mode_e        video_out_mode;         ///< RW; Range:[0, 2]; HDMI output colorspace. Default: E_HDMI_VIDEO_MODE_RGB444.
    vs_hdmi_quantization_e      out_csc_quantization;   ///< RW; Range:[0, 1]; HDMI output quantization range. Default: E_HDMI_QUANTIZATION_LIMITED_RANGE
    vs_hdmi_deep_color_e        deep_color_mode;        ///< RW; Range:[0, 0]; Video output color depth. Default: E_HDMI_DEEP_COLOR_24BIT
    vs_hdmi_video_colorimetry_e video_colorimetry;      ///< RW; Range:[0, 1], Video colorimetry. Default: E_HDMI_VIDEO_COLORIMITERY_ITU701

    vs_bool_t                   audio_enable;           ///< RW; Range:[0, 1]; Whether to enable the audio. Default: VS_TRUE.
    vs_hdmi_sound_interface_e   audio_sound_interface;  ///< RW; Range:[0, 0]; HDMI audio interface. Default: E_HDMI_SOUND_INTERFACE_I2S.
    vs_bool_t                   is_audio_multichn;      ///< Rw; Range:[0, 0]; Multi-channel or stereo. VS_FALSE: stereo. VS_TRUE:eight-channel fixed for multi-channel. Default: VS_FALSE.
    vs_hdmi_sample_rate_e       audio_sample_rate;      ///< RW; HDMI audio sample rate. Default: E_HDMI_SAMPLE_RATE_48K.
    vs_hdmi_bit_depth_e         audio_bit_depth;        ///< RW; HDMI audio bit width. Default: E_HDMI_BIT_DEPTH_24.

    vs_bool_t                   avi_infoframe_enable;   ///< RW; Range:[0, 1]; Whether to enable AVI infoframe. Default: VS_TRUE
    vs_bool_t                   audio_infoframe_enable; ///< RW; Range:[0, 1]; Whether to enable Audio infoframe.Default: VS_TRUE

    vs_hdmi_force_action_e      default_mode;           ///< RW; Range:[0, 2]; When the value of hdmi_enable is false and get EDID fail, HDMI forcily select output video mode. Default: E_HDMI_FORCE_HDMI.
    vs_bool_t                   is_auth_mode;           ///< RW; Range:[0, 1]; Whether to enable auth mode. Default: VS_FALSE.
    vs_bool_t                   video_mode_adapt_enable; ///< RW; Range:[0, 1]; Whether to enable video mode adapt. Default: VS_TRUE.
    vs_bool_t                   deep_color_adapt_enable; ///< RW; Range:[0, 1]; Whether to enable color depth adapt. Default: VS_TRUE.
    vs_hdmi_timing_info_s       detailed_timing;        ///< Detail timing for customer-define. This param is valid only when video_format is E_HDMI_VIDEO_FORMAT_CUSTOMER_DEFINE.
} vs_hdmi_attr_s;

/**
* @brief Define EDID row data information
*/
typedef struct vs_hdmi_edid {
    vs_bool_t           is_edid_valid;          ///< R; Whether EDID data is valid.
    vs_uint32_t         edid_len;               ///< R; Data length of EDID.
    vs_uint8_t          edid[HDMI_MAX_EDID_SIZE];   ///< R; EDID row data.
} vs_hdmi_edid_s;

/**
* @brief Define HDMI infoframe information
*/
typedef struct vs_hdmi_infoframe {
    vs_hdmi_infoframe_type_e   infoframe_type;  ///< RW; Range:[0, 1]; HDMI infoframe type
    vs_hdmi_infoframe_unit_u   infoframe_unit;  ///< RW; HDMI infoframe information union
} vs_hdmi_infoframe_s;

/**
* @brief Define HDMI callback function ptr
*/
typedef vs_void_t (*vs_hdmi_callback_func)(vs_hdmi_event_type_e event, vs_void_t *p_private_date);

/**
* @brief Define HDMI callback information
*/
typedef struct vs_hdmi_callback {
    vs_hdmi_callback_func      p_hdmi_event_callback;   ///< Event handling callback function
    vs_void_t                  *p_private_date;         ///< Private data of the HDMI callback functions and parameters
} vs_hdmi_callback_s;

/**
* @brief Define the phy parameters
* @details When using long hdmi cable, this parameter should be used
*/
typedef struct vs_hdmi_param_mod {
    vs_hdmi_trace_len_e         trace_len;      ///< RW; Range:[0, 10]; Phy parameters selection
} vs_hdmi_param_mod_s;

#ifdef __cplusplus
}
#endif

#endif /*!< __VS_HDMI_DEFINES_H__ */
