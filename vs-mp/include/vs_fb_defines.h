/**
* @file vs_fb_defines.h
* @brief ioctl cmd of vs fb
* @author VS fb group
* @date 2023-05-22
* @version v0.10
* @Copyright (C) 2023, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/

#ifndef __VS_FB_DEFINES_H__
#define __VS_FB_DEFINES_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "vs_type.h"
#include "vs_comm.h"
#include "vs_errno.h"
#include "vs_comm_video.h"

#define VS_ERR_FB_INVALID_PARAM                  VS_ERR_CODE(E_MOD_ID_FB, E_ERR_INVALID_PARAM)
#define VS_ERR_FB_BAD_STATE                      VS_ERR_CODE(E_MOD_ID_FB, E_ERR_BAD_STATE)
#define VS_ERR_FB_NULL_PTR                       VS_ERR_CODE(E_MOD_ID_FB, E_ERR_NULL_PTR)
#define VS_ERR_FB_NOT_SUPPORT                    VS_ERR_CODE(E_MOD_ID_FB, E_ERR_NOT_SUPPORT)
#define VS_ERR_FB_NO_MEMORY                      VS_ERR_CODE(E_MOD_ID_FB, E_ERR_NO_MEMORY)
#define VS_ERR_FB_TIMEOUT                        VS_ERR_CODE(E_MOD_ID_FB, E_ERR_TIMEOUT)

#define IOC_TYPE_VS_FB       'F'

/* frame and layer iocmd number range is from 90 to 129 */
/** To get the origin of an overlay layer on the screen */
#define FBIOGET_SCREEN_ORIGIN_VS_FB  _IOR(IOC_TYPE_VS_FB, 90, vs_fb_point_s)
/** To set the origin of an overlay layer on the screen */
#define FBIOPUT_SCREEN_ORIGIN_VS_FB  _IOW(IOC_TYPE_VS_FB, 91, vs_fb_point_s)
/** To obtain the display state of an overlay layer */
#define FBIOGET_SHOW_VS_FB           _IOR(IOC_TYPE_VS_FB, 92, vs_bool_t)
/** To set the display state of an overlay layer */
#define FBIOPUT_SHOW_VS_FB           _IOW(IOC_TYPE_VS_FB, 93, vs_bool_t)
/** To obtain the colorkey of an overlay layer */
#define FBIOGET_COLORKEY_VS_FB       _IOR(IOC_TYPE_VS_FB, 94, vs_fb_colorkey_s)
/** To set the colorkey of an overlay layer */
#define FBIOPUT_COLORKEY_VS_FB       _IOW(IOC_TYPE_VS_FB, 95, vs_fb_colorkey_s)
/** To get the alpha of an overlay layer */
#define FBIOGET_ALPHA_VS_FB          _IOR(IOC_TYPE_VS_FB, 96, vs_fb_alpha_s)
/** To set the alpha of an overlay layer */
#define FBIOPUT_ALPHA_VS_FB          _IOW(IOC_TYPE_VS_FB, 97, vs_fb_alpha_s)

/** To get the layer information */
#define FBIOGET_LAYER_INFO           _IOR(IOC_TYPE_VS_FB, 98, vs_fb_layer_info_s)
/** To set the layer information */
#define FBIOPUT_LAYER_INFO           _IOW(IOC_TYPE_VS_FB, 99, vs_fb_layer_info_s)
/** To get canvas buf */
#define FBIOGET_CANVAS_BUFFER        _IOR(IOC_TYPE_VS_FB, 100, vs_fb_buffer_s)
/** To refresh the displayed contents in extended mode */
#define FBIO_REFRESH                 _IOW(IOC_TYPE_VS_FB, 101, vs_fb_buffer_s)

/** To wait for the vertical blanking region of an overlay layer */
#define FBIOGET_VBLANK_VS_FB         _IO(IOC_TYPE_VS_FB, 102)

/* To get the rotate mode */
#define FBIOGET_ROTATE_MODE          _IOW(IOC_TYPE_VS_FB, 103, vs_fixed_rotation_e)
/* To set the rotate mode */
#define FBIOPUT_ROTATE_MODE          _IOW(IOC_TYPE_VS_FB, 104, vs_fixed_rotation_e)
/* To get the mirror mode */
#define FBIOGET_MIRROR_MODE          _IOW(IOC_TYPE_VS_FB, 105, vs_mirror_mode_e)
/* To set the mirror mode */
#define FBIOPUT_MIRROR_MODE          _IOW(IOC_TYPE_VS_FB, 106, vs_mirror_mode_e)

typedef struct vs_fb_point {
	vs_int32_t x; /* horizontal position */
	vs_int32_t y; /* vertical position */
} vs_fb_point_s;

typedef struct vs_fb_colorkey {
	vs_bool_t key_enable; /* colorkey enable flag */
	vs_uint32_t key;      /* colorkey value */
} vs_fb_colorkey_s;

typedef struct vs_fb_alpha {
	vs_bool_t alpha_enable;         /**<  alpha enable flag */
	vs_bool_t global_alpha_enable;  /**<  global alpha enable flag */
	vs_uint8_t global_alpha;        /**<  global alpha value */
	vs_uint8_t reserved[3];
} vs_fb_alpha_s;

/*refresh mode*/
typedef enum vs_fb_layer_buf_mode {
	VS_FB_LAYER_BUF_DOUBLE           = 0x0,       /**<  2 display buf in fb */
	VS_FB_LAYER_BUF_ONE              = 0x1,       /**<  1 display buf in fb */
	VS_FB_LAYER_BUF_NONE             = 0x2,       /**<  no display buf in fb,the buf user refreshed will be directly set to VO*/
	VS_FB_LAYER_BUF_DOUBLE_IMMEDIATE = 0x3,       /**< 2 display buf in fb, each refresh will be displayed*/
	VS_FB_LAYER_BUF_FENCE            = 0x4,       /**<  2 display buf in fb with fence */
	VS_FB_LAYER_BUF_BUTT
} vs_fb_layer_buf_mode_e;

/* layer info maskbit */
typedef enum vs_fb_layer_info_maskbit {
	VS_FB_LAYER_MASK_BUFMODE    = 0x1,      /**< BUFMODE bitmask */
	VS_FB_LAYER_MASK_POS        = 0x2,      /**< the position bitmask */
	VS_FB_LAYER_MASK_CANVASSIZE = 0x4,      /**< canvassize bitmask */
	VS_FB_LAYER_MASK_DISPSIZE   = 0x8,      /**< displaysize bitmask */
	VS_FB_LAYER_MASK_SCREENSIZE = 0x10,     /**< screensize bitmask */
	VS_FB_LAYER_MASK_BMUL       = 0x20,     /**< pre-mult bitmask */
	VS_FB_LAYER_MASK_BUTT
} vs_fb_layer_info_maskbit_e;

/* layer info */
typedef struct vs_fb_layer_info {
	vs_fb_layer_buf_mode_e buf_mode;
	vs_int32_t x;                /**<  the x pos of origion point in screen */
	vs_int32_t y;                /**<  the y pos of origion point in screen */
	vs_uint32_t canvas_width;    /**<  the width of canvas buffer */
	vs_uint32_t canvas_height;   /**<  the height of canvas buffer */
	vs_uint32_t display_width;   /**<  the width of display buf in fb.for 0 buf ,there is no display buf in fb, so it's effectless*/
	vs_uint32_t display_height;  /**<  the height of display buf in fb. */
	vs_uint32_t screen_width;    /**<  the width of screen */
	vs_uint32_t screen_height;   /**<  the height of screen */
	vs_bool_t b_premul;          /**<  The data drawed in buf is premul data or not*/
	vs_uint32_t mask;            /**<  param modify mask bit*/
} vs_fb_layer_info_s;

typedef struct vs_fb_surface {
	vs_uint64_t  phy_addr;       /**<  start physical address */
	vs_uint32_t  width;          /**<  width pixels */
	vs_uint32_t  height;         /**<  height pixels */
	vs_uint32_t  pitch;          /**<  line pixels */
	vs_pixel_format_e fmt;       /**<  color format */
} vs_fb_surface_s;

typedef struct vs_fb_rect {
	vs_int32_t x;
	vs_int32_t y;
	vs_int32_t w;
	vs_int32_t h;
} vs_fb_rect_s;

/* refresh surface info */
typedef struct vs_fb_buffer {
	vs_fb_surface_s canvas;
	vs_fb_rect_s update_rect;    /* refresh region*/
} vs_fb_buffer_s;

#ifdef __cplusplus
}
#endif

#endif //end of __VS_FB_DEFINES_H__
