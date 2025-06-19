/**
* @file vs_vo_defines.h
* @brief vo define
* @details vo data structures and defines
* @author VS vo group
* @date 2022-01-18
* @version v0.10
* @Copyright (C) 2022, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/

#ifndef __VS_VO_DEFINES_H__
#define __VS_VO_DEFINES_H__

#include "vs_type.h"
#include "vs_comm.h"
#include "vs_errno.h"
#include "vs_comm_video.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum vs_vo_errno {
	E_VO_ERR_INVALID_LAYERID = 100, ///< Invalid layer id
	E_VO_ERR_INVALID_CURSORID,      ///< Invalid cursor id
	E_VO_ERR_INVALID_WBCID,         ///< Invalid write back channel id
	E_VO_ERR_CHN_BUF_NOT_ALLOWED,   ///< chn buf is not allowed to receive
	E_VO_ERR_DEV_NOT_DISABLE,       ///< vo device should be disabled first
	E_VO_ERR_LAYER_NOT_DISABLE,     ///< layer should be disabled first
	E_VO_ERR_CHN_NOT_DISABLE,       ///< video channel should be disabled first
	E_VO_ERR_WBC_NOT_DISABLE,       ///< write back channel should be disabled first
	E_VO_ERR_MAX
} vs_vo_errno_e;

#define VS_ERR_VO_INVALID_PARAM                  VS_ERR_CODE(E_MOD_ID_VO, E_ERR_INVALID_PARAM)
#define VS_ERR_VO_INVALID_DEVID                  VS_ERR_CODE(E_MOD_ID_VO, E_ERR_INVALID_DEVID)
#define VS_ERR_VO_INVALID_CHNID                  VS_ERR_CODE(E_MOD_ID_VO, E_ERR_INVALID_CHNID)
#define VS_ERR_VO_BAD_STATE                      VS_ERR_CODE(E_MOD_ID_VO, E_ERR_BAD_STATE)
#define VS_ERR_VO_NULL_PTR                       VS_ERR_CODE(E_MOD_ID_VO, E_ERR_NULL_PTR)
#define VS_ERR_VO_EXIST                          VS_ERR_CODE(E_MOD_ID_VO, E_ERR_EXIST)
#define VS_ERR_VO_UNEXIST                        VS_ERR_CODE(E_MOD_ID_VO, E_ERR_UNEXIST)
#define VS_ERR_VO_NOT_SUPPORT                    VS_ERR_CODE(E_MOD_ID_VO, E_ERR_NOT_SUPPORT)
#define VS_ERR_VO_NOT_IMPLEMENTED                VS_ERR_CODE(E_MOD_ID_VO, E_ERR_NOT_IMPLEMENTED)
#define VS_ERR_VO_NOT_INITIALIZED                VS_ERR_CODE(E_MOD_ID_VO, E_ERR_NOT_INITIALIZED)
#define VS_ERR_VO_NOT_PERM                       VS_ERR_CODE(E_MOD_ID_VO, E_ERR_NOT_PERM)
#define VS_ERR_VO_NO_MEMORY                      VS_ERR_CODE(E_MOD_ID_VO, E_ERR_NO_MEMORY)
#define VS_ERR_VO_BUF_EMPTY                      VS_ERR_CODE(E_MOD_ID_VO, E_ERR_BUF_EMPTY)
#define VS_ERR_VO_BUF_FULL                       VS_ERR_CODE(E_MOD_ID_VO, E_ERR_BUF_FULL)
#define VS_ERR_VO_BAD_ADDR                       VS_ERR_CODE(E_MOD_ID_VO, E_ERR_BAD_ADDR)
#define VS_ERR_VO_BUSY                           VS_ERR_CODE(E_MOD_ID_VO, E_ERR_BUSY)
#define VS_ERR_VO_TIMEOUT                        VS_ERR_CODE(E_MOD_ID_VO, E_ERR_TIMEOUT)
#define VS_ERR_VO_ABORTED                        VS_ERR_CODE(E_MOD_ID_VO, E_ERR_ABORTED)
#define VS_ERR_VO_DEVICE_UNEXIST                 VS_ERR_CODE(E_MOD_ID_VO, E_ERR_DEVICE_UNEXIST)

#define VS_ERR_VO_INVALID_LAYERID                VS_ERR_CODE(E_MOD_ID_VO, E_VO_ERR_INVALID_LAYERID)
#define VS_ERR_VO_INVALID_CURSORID               VS_ERR_CODE(E_MOD_ID_VO, E_VO_ERR_INVALID_CURSORID)
#define VS_ERR_VO_INVALID_WBCID                  VS_ERR_CODE(E_MOD_ID_VO, E_VO_ERR_INVALID_WBCID)
#define VS_ERR_VO_CHN_BUF_NOT_ALLOWED            VS_ERR_CODE(E_MOD_ID_VO, E_VO_ERR_CHN_BUF_NOT_ALLOWED)
#define VS_ERR_VO_DEV_NOT_DISABLE                VS_ERR_CODE(E_MOD_ID_VO, E_VO_ERR_DEV_NOT_DISABLE)
#define VS_ERR_VO_LAYER_NOT_DISABLE              VS_ERR_CODE(E_MOD_ID_VO, E_VO_ERR_LAYER_NOT_DISABLE)
#define VS_ERR_VO_CHN_NOT_DISABLE                VS_ERR_CODE(E_MOD_ID_VO, E_VO_ERR_CHN_NOT_DISABLE)
#define VS_ERR_VO_WBC_NOT_DISABLE                VS_ERR_CODE(E_MOD_ID_VO, E_VO_ERR_WBC_NOT_DISABLE)

/**
* @brief Define the VO interface type
* @details Define the VO output interface type that VO module can support
*/
typedef enum vs_vo_interface_type {
    E_VO_INTERFACE_TYPE_CVBS           = (1UL << 0),      ///< type of CVBS
    E_VO_INTERFACE_TYPE_BT656          = (1UL << 1),      ///< type of BT656
    E_VO_INTERFACE_TYPE_BT1120         = (1UL << 2),      ///< type of BT1120
    E_VO_INTERFACE_TYPE_HDMI           = (1UL << 3),      ///< type of HDMI
    E_VO_INTERFACE_TYPE_LCD_16BIT      = (1UL << 4),      ///< type of LCD_16BIT
    E_VO_INTERFACE_TYPE_LCD_18BIT      = (1UL << 5),      ///< type of LCD_18BIT
    E_VO_INTERFACE_TYPE_LCD_24BIT      = (1UL << 6),      ///< type of LCD_24BIT
    E_VO_INTERFACE_TYPE_MIPI           = (1UL << 7),      ///< type of MIPI TX
} vs_vo_interface_type_e;

/**
* @brief Define the vo output size type
* @details Define the output size and frame rate type that VO module can support
*/
typedef enum vs_vo_output_type {
    E_VO_OUTPUT_TYPE_PAL = 0,         ///< PAL 720x576, 576i@50FPS
    E_VO_OUTPUT_TYPE_NTSC,            ///< NTSC 720x480, 480i
    E_VO_OUTPUT_TYPE_1080P24,         ///< 1920X1080P @ 24 FPS
    E_VO_OUTPUT_TYPE_1080P25,         ///< 1920X1080P @ 25 FPS
    E_VO_OUTPUT_TYPE_1080P30,         ///< 1920X1080P @ 30 FPS
    E_VO_OUTPUT_TYPE_720P50,          ///< 1280X720P  @ 50 FPS
    E_VO_OUTPUT_TYPE_720P60,          ///< 1280X720P  @ 60 FPS
    E_VO_OUTPUT_TYPE_1080I50,         ///< 1920X1080I @ 50 FPS
    E_VO_OUTPUT_TYPE_1080I60,         ///< 1920X1080I @ 60 FPS
    E_VO_OUTPUT_TYPE_1080P50,         ///< 1920X1080P @ 50 FPS
    E_VO_OUTPUT_TYPE_1080P60,         ///< 1920X1080P @ 60 FPS
    E_VO_OUTPUT_TYPE_576P50,          ///< 720X576    @ 50 FPS
    E_VO_OUTPUT_TYPE_480P60,          ///< 720X480    @ 60 FPS
    E_VO_OUTPUT_TYPE_800x600_60,      ///< 800X600    @ 60 FPS
    E_VO_OUTPUT_TYPE_1024x768_60,     ///< 1024X768   @ 60 FPS
    E_VO_OUTPUT_TYPE_1280x1024_60,    ///< 1280X1024  @ 60 FPS
    E_VO_OUTPUT_TYPE_1366x768_60,     ///< 1366X768   @ 60 FPS
    E_VO_OUTPUT_TYPE_1440x900_60,     ///< 1440X900   @ 60 FPS
    E_VO_OUTPUT_TYPE_1280x800_60,     ///< 1280X800   @ 60 FPS
    E_VO_OUTPUT_TYPE_1600x1200_60,    ///< 1600X1200  @ 60 FPS
    E_VO_OUTPUT_TYPE_1680x1050_60,    ///< 1680X1050  @ 60 FPS
    E_VO_OUTPUT_TYPE_1920x1200_60,    ///< 1920X1200  @ 60 FPS
    E_VO_OUTPUT_TYPE_640x480_60,      ///< 640X480    @ 60 FPS
    E_VO_OUTPUT_TYPE_960H_PAL,        ///< TODO
    E_VO_OUTPUT_TYPE_960H_NTSC,       ///<
    E_VO_OUTPUT_TYPE_1920x2160_30,    ///< 1920X2160  @ 30 FPS
    E_VO_OUTPUT_TYPE_2560x1440_30,    ///< 2560X1440  @ 30 FPS
    E_VO_OUTPUT_TYPE_2560x1440_60,    ///< 2560X1440  @ 60 FPS
    E_VO_OUTPUT_TYPE_2560x1600_60,    ///< 2560X1600  @ 60 FPS
    E_VO_OUTPUT_TYPE_3840x2160_24,    ///< 3840X2160  @ 24 FPS
    E_VO_OUTPUT_TYPE_3840x2160_25,    ///< 3840X2160  @ 25 FPS
    E_VO_OUTPUT_TYPE_3840x2160_30,    ///< 3840X2160  @ 30 FPS
    E_VO_OUTPUT_TYPE_3840x2160_50,    ///< 3840X2160  @ 50 FPS
    E_VO_OUTPUT_TYPE_3840x2160_60,    ///< 3840X2160  @ 60 FPS
    E_VO_OUTPUT_TYPE_4096x2160_24,    ///< 4096X2160  @ 24 FPS
    E_VO_OUTPUT_TYPE_4096x2160_25,    ///< 4096X2160  @ 25 FPS
    E_VO_OUTPUT_TYPE_4096x2160_30,    ///< 4096X2160  @ 30 FPS
    E_VO_OUTPUT_TYPE_4096x2160_50,    ///< 4096X2160  @ 50 FPS
    E_VO_OUTPUT_TYPE_4096x2160_60,    ///< 4096X2160  @ 60 FPS
    E_VO_OUTPUT_TYPE_320x240_60,      ///< 320X240    @ 60 FPS
    E_VO_OUTPUT_TYPE_320x240_50,      ///< 320X240    @ 50 FPS
    E_VO_OUTPUT_TYPE_240x320_50,      ///< 240X320    @ 50 FPS
    E_VO_OUTPUT_TYPE_240x320_60,      ///< 240X320    @ 60 FPS
    E_VO_OUTPUT_TYPE_800x600_50,      ///< 800X600    @ 50 FPS
    E_VO_OUTPUT_TYPE_720x1280_60,     ///< 720X1280   @ 60 FPS
    E_VO_OUTPUT_TYPE_1080x1920_60,    ///< 1080X1920  @ 60 FPS
    E_VO_OUTPUT_TYPE_USER,            ///< USER
    E_VO_OUTPUT_TYPE_MAX
} vs_vo_output_type_e;

/**
* @brief Define vo timing info
* @details timing configuration to support user timing
*/
typedef struct vs_vo_timing {
    vs_uint16_t vactive;           ///< vertical active lines
    vs_uint16_t vbp;               ///< vertical back porch
    vs_uint16_t vfp;               ///< vertical front porch
    vs_uint16_t vpw;               ///< vsync pulse width
    vs_uint16_t hactive;           ///< horizontal active pixels
    vs_uint16_t hbp;               ///< horizontal back porch
    vs_uint16_t hfp;               ///< horizontal front porch
    vs_uint16_t hpw;               ///< hsync pulse width
    vs_bool_t   interlaced_output; ///< interlaced output
    vs_uint16_t bvactive;          ///< bottom field's vertical active lines
    vs_uint16_t bvbp;              ///< bottom fields's vertical back porch
    vs_uint16_t bvfp;              ///< bottom fields's vertical front porch
    vs_bool_t   data_polarity;     ///< data polarity, default is positive
    vs_bool_t   hsync_polarity;    ///< hsync polarity, default is positive
    vs_bool_t   vsync_polarity;    ///< vsync polarity, default is positive
} vs_vo_timing_s;

/**
* @brief Define vo device attributes
* @details The vo device attributes contains background color, interface type, output type and timing info
*/
typedef struct vs_vo_dev_attr {
    vs_uint32_t            bg_color;            ///< background color
    vs_vo_interface_type_e interface_type;      ///< output interface type
    vs_vo_output_type_e    interface_output;    ///< output size and frame rate type
    vs_vo_timing_s         timing_info;         ///< vo output timing info
} vs_vo_dev_attr_s;

/**
* @brief Define vo dev policy
* @details The vo dev policy
*/
typedef enum vs_vo_dev_policy_type {
    E_VO_DEV_POLICY_LOWLATENCY = 0,     ///< display frame with low latency
    E_VO_DEV_POLICY_QUICK_DISPLAY,      ///< display background on screen quickly
    E_VO_DEV_POLICY_MAX
} vs_vo_dev_policy_e;

/**
* @brief Define vo device policy
* @details The vo device policy contains dev_policy and tolerant_time
*/
typedef struct vs_vo_dev_policy {
    vs_vo_dev_policy_e dev_policy;
    vs_uint32_t        tolerant_time;  ///< frame tolerant time in microsecond (us), which only used with low latency display mode
} vs_vo_dev_policy_s;

/**
* @brief Define vo clock info
* @details The video pll clk rate = pixel_clk_rate * clk_div,
*          video pll is the same pixel clk source of display0 & 1, so it can be set when both display0 & 1 are disabled.
*/
typedef struct vs_vo_clk_info {
	vs_uint64_t pixel_clk_rate;
	vs_uint32_t clk_div;
} vs_vo_clk_info_s;

/**
* @brief Define vo chn attributes
* @details chn attributes contain zorder , rect and deflicker
*/
typedef struct vs_vo_chn_attr {
    vs_uint32_t zorder;       ///< channel zorder
    vs_rect_s   rect;         ///< channel rectangle
    vs_bool_t   deflicker;    ///< channel deflicker
} vs_vo_chn_attr_s;

/**
* @brief Define vo chn parameter
* @details chn parameter contain the aspect ratio
*/
typedef struct vs_vo_chn_param {
    vs_aspect_ratio_s aspect_ratio;    ///<aspect ratio
} vs_vo_chn_param_s;

/**
* @brief Define the type of vo zoomin type that vo module can support
* @details two types defined rect and ratio
*/
typedef enum vs_vo_zoomin_type {
    E_VO_ZOOMIN_TYPE_RECT = 0,    ///< rect zoomin
    E_VO_ZOOMIN_TYPE_RATIO,       ///< ratio zoomin
    E_VO_ZOOMIN_TYPE_MAX
} vs_vo_zoomin_type_e;

/**
* @brief Define vo zoomin ratio info
* @details x, y, width, height ratio
*/
typedef struct vs_vo_zoomin_ratio {
    vs_uint32_t x_ratio;    ///< the starting point x of the src rect / channel width
    vs_uint32_t y_ratio;    ///< the starting point y of the src rect / channel height
    vs_uint32_t w_ratio;    ///< the width of the src rect / channel width
    vs_uint32_t h_ratio;    ///< the height of the src rect / channel height
 } vs_vo_zoomin_ratio_s;

/**
* @brief Define vo zoomin attributes
* @details zoomin attributes contain zoom type , zoomin_rect or ratio
*/
typedef struct vs_vo_zoomin_attr {
    vs_vo_zoomin_type_e      zoomin_type;    ///< indicate the zoomin type rect or ratio
    union {
        vs_rect_s            zoomin_rect;    ///< zoomin_rect when zoomin type is E_VO_ZOOMIN_TYPE_RECT
        vs_vo_zoomin_ratio_s ratio;          ///< zoomin ratio when zoomin type is E_VO_ZOOMIN_TYPE_RATIO
    };
} vs_vo_zoomin_attr_s;

/**
* @brief Define vo chn border
* @details border enable flag and border info
*/
typedef struct vs_vo_chn_border {
    vs_bool_t   enable;    ///< indicate whether the border is enable
    vs_border_s border;    ///< the top, bottom, left, right width and the color of the border
} vs_vo_chn_border_s;

/**
* @brief Define vo chn status
* @details occupied chn buf number
*/
typedef struct vs_vo_chn_status {
    vs_int32_t occupied_chn_buf_num;    ///< indicate the total number of occupied chn buffers
    vs_int32_t chn_buf_depth;           ///< indicate the max number of chn buffers that haven't been processed
} vs_vo_chn_status_s;

/**
* @brief Define vo chn region info
* @details region info contains region number and region rectangle
*/
typedef struct vs_vo_region_info {
    vs_uint32_t    region_num;    ///< region number
    vs_rect_s      *p_region;     ///< region rect info
} vs_vo_region_info_s;

/**
* @brief Define vo video layer attributes
* @details video layer attributes contain display rectangle, image size, pixel format, dynamic range, layer framerate and buffer depth
*/
typedef struct vs_vo_video_layer_attr {
    vs_rect_s          dest_rect;          ///< rectangle to be displayed
    vs_size_s          image_size;         ///< the size of the layer buffer
    vs_pixel_format_e  format;             ///< pixel format
    vs_dynamic_range_e dynamic_range;      ///< only support SDR8
    vs_uint32_t        layer_framerate;    ///< min(Max(video chn fps), dpu output fps)
} vs_vo_video_layer_attr_s;

/**
* @brief Define vo csc matrix type
* @details vo support the (YUV to YUV), RGB to YUV, RGB to YUV from BT601 to BT709, vice versa
*/
typedef enum vs_vo_csc_mode {
    E_VO_CSC_MODE_IDENTITY = 0,           ///<
    E_VO_CSC_MODE_YUV2RGB_BT601_PC,       ///< YUV to RGB BT601 full range
    E_VO_CSC_MODE_YUV2RGB_BT601_TV,       ///< YUV to RGB BT601 limit range
    E_VO_CSC_MODE_YUV2RGB_BT709_PC,       ///< YUV to RGB BT709 full range
    E_VO_CSC_MODE_YUV2RGB_BT709_TV,       ///< YUV to RGB BT709 limit range
    E_VO_CSC_MODE_RGB2RGB_BT601_TO_BT709, ///< RGB to RGB BT601 to BT709
    E_VO_CSC_MODE_RGB2RGB_BT709_TO_BT601, ///< RGB to RGB BT709 to BT601
    E_VO_CSC_MODE_RGB2YUV_BT601_PC,       ///< RGB to YUV BT601 full range
    E_VO_CSC_MODE_RGB2YUV_BT601_TV,       ///< RGB to YUV BT601 limit range
    E_VO_CSC_MODE_RGB2YUV_BT709_PC,       ///< RGB to YUV BT709 full range
    E_VO_CSC_MODE_RGB2YUV_BT709_TV,       ///< RGB to YUV BT709 limit range
    E_VO_CSC_MODE_YUV2RGB_USER_DEF,       ///< YUV to RGB user define
    E_VO_CSC_MODE_RGB2RGB_USER_DEF,       ///< RGB to RGB user define
    E_VO_CSC_MODE_RGB2YUV_USER_DEF,       ///< RGB to YUV user define
    E_VO_CSC_MODE_MAX
} vs_vo_csc_mode_e;

/**
* @brief Define vo csc matrix info
* @details vo support the YUV to YUV, RGB to YUV, RGB to YUV from BT601 to BT709, vice versa
*/
typedef struct vs_vo_csc {
    vs_vo_csc_mode_e csc_mode;    ///< csc mode
    vs_int32_t matrix[16];        ///< coefficient matrix
    vs_int32_t size;              ///< coefficient num
} vs_vo_csc_s;

/**
* Device based writeback contains device 0 and 1,
* it cannot work at the same time with panel output.
* Layer based writeback contains 6 layer buffers,
* in this case, the wb attr may not be supported to
* change, we just send the layer buffer back to user.
*/

/**
* @brief Define vo write back channel source type
* @details three types , device, video and graphic
*/
typedef enum vs_vo_wbc_source_type {
    E_VO_WBC_SOURCE_TYPE_DEVICE = 0,    ///< write back from device, is used to debug, is not a normal feature
    E_VO_WBC_SOURCE_TYPE_VIDEO,         ///< write back from video layer
    E_VO_WBC_SOURCE_TYPE_GRAPHIC,       ///< write back from graphic layer
    E_VO_WBC_SOURCE_TYPE_MAX
} vs_vo_wbc_source_type_e;

/**
* @brief Define vo write back channel attributes
* @details write back channel attributes contain write back rectangle size, pixel format, framerate and compress mode
*/
typedef struct vs_vo_wbc_attr {
    vs_size_s          dest_size;        ///< write back rectangle size
    vs_pixel_format_e  format;           ///< RGB --> .bmp, YUV --> .yuv
    vs_int32_t        framerate;        ///< write back framerate
} vs_vo_wbc_attr_s;

/**
* @brief Define vo write back channel mode type
* @details three types all. abound repeat frame and the transfer prog to intl
*/
typedef enum vs_vo_wbc_mode {
    E_VO_WBC_MODE_ALL = 0,           ///< write back all frame
    E_VO_WBC_MODE_ABANDON_REPEAT,    ///< abandon the repeat frame
    E_VO_WBC_MODE_PROG_TO_INTL,      ///< TODO
    E_VO_WBC_MODE_MAX,
} vs_vo_wbc_mode_e;

#ifdef __cplusplus
}
#endif

#endif
