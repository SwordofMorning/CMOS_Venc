/**
* @file vs_gpe_defines.h
* @brief common graphic process engine  data type define
* @details 
* @author visinextek
* @date 2021-11-30
* @version v0.1
* @Copyright (C) 2021, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/

#ifndef __VS_GPE_DEFINES_H__
#define __VS_GPE_DEFINES_H__

#include "vs_errno.h"
#include "vs_comm_video.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
* @brief Define the filter type that GPE support
* @details Support two type of filter in horizontal and vertical direction filter
*/
typedef enum vs_gpe_filter_type {
    E_GPE_FILTER_TYPE_3_TAP = 0,        ///< 3x3 filter matrix that GPE support for scale or deflicker
    E_GPE_FILTER_TYPE_5_TAP,            ///< 5x5 filter matrix that GPE support for scale or deflicker
    E_GPE_FILTER_TYPE_MAX
} vs_gpe_filter_type_e;

/**
* @brief Define line attributes those could be set for draw line
* @details The GPE using the following attributes to draw a line by using Bresenham algorithm
*/
typedef struct vs_gpe_line_attr {
    vs_point_s     start;               ///< the start point of a line
    vs_point_s     end;                 ///< the end point of a line
    vs_uint32_t    width;               ///< the line color width, means how many pixels of the line body width
    vs_uint32_t    color;               ///< the value of color for the line
} vs_gpe_line_attr_s;

/**
* @brief Define the cover area type
* @details Cover has two geometry type, rectangle and quadrangle
*/
typedef enum vs_gpe_cover_type {
    E_GPE_COVER_TYPE_RECTANGLE = 0,     ///< the cover area is the rectangle geometry
    E_GPE_COVER_TYPE_QUADRANGLE,        ///< the cover area is the quadrangle, concave quadrangle is draw as triangle
    E_GPE_COVER_TYPE_MAX
} vs_gpe_cover_type_e;

/**
* @brief Define the attribute for GPE draw a specific cover on video frame
* @details
*/
typedef struct vs_gpe_cover_attr {
    vs_gpe_cover_type_e     type;       ///< setting the cover type, rectangle or quadrangle
    vs_uint32_t             color;      ///< the color value be filled into rectangle for solid quadrangle cover
    vs_rect_s               rect;       ///< definition for rectangle cover

    /** following member be used defined for quadrangle only,
     * 1. please ignore if it is a rectangle cover 
     * 2. must give the value of following attr if there use quadrangle cover
    */
    vs_bool_t               solid;      ///< indicate the quadrangle cover is solid or hollow
    vs_uint32_t             width;      ///< hollow quadrangle cover border width
    vs_point_s              points[4];  ///< the four points define for quadrangle cover
} vs_gpe_cover_attr_s;

/**
* @brief Define the color revert mode for ARGB format data
* @details There support two mode of revert, one is revert RGB value, another is revert Alpha value
*/
typedef enum vs_gpe_revert_mode {
    E_GPE_REVERT_MODE_ONLY_RGB = 0,     ///< just revert rgb channel value
    E_GPE_REVERT_MODE_ONLY_ALPHA,       ///< just revert alpga channel value
    E_GPE_REVERT_MODE_ALL,              ///< revert all color channel value
    E_GPE_REVERT_MODE_MAX
} vs_gpe_revert_mode_e;

/**
* @brief Define the color revert control struct
* @details The rect of the video and the revert mode
*/
typedef struct vs_gpe_revert {
    vs_rect_s              rect;        ///< It is a sub rect of entire osd rect
    vs_gpe_revert_mode_e   mode;        ///< which color channel will be revert
} vs_gpe_revert_s;

/**
* @brief The osd display attributes, be used for GPE draw a osd on video frame
* @details OSD color format inf
*/
typedef struct vs_gpe_osd_attr {
    vs_bool_t                  enable;       ///< enable or not
    vs_rect_s                  rect;         ///< rect of osd area
    vs_uint8_t                 global_alpha; ///< the global alpha could instead of per pixel alpha
    vs_uint32_t                bg_color;     ///< the background color for clear the osd area
    vs_pixel_format_e          format;       ///< format of osd color data
    vs_uint64_t                phys_addr;    ///< physic address for buffer of osd date store
    vs_uint32_t                stride;       ///< stride for the hardware which will read osd data
    /**
     * following be used for osd with ARGB1555 format 
     */
    vs_bool_t                  alpha_extend; ///< enable alpha extend function when covert ARGB1555 to ARGB8888
    vs_uint32_t                ext_alpha0;   ///< one extend alpha value
    vs_uint32_t                ext_alpha1;   ///< another extend alpha value
    vs_gpe_revert_s            revert_info;  ///< the color revert control information
} vs_gpe_osd_attr_s;

///< parameter is illegal */
#define VS_ERR_GPE_INVALID_PARAM	VS_ERR_CODE(E_MOD_ID_GPE, E_ERR_INVALID_PARAM)
///< using a NULL pointer
#define VS_ERR_GPE_NULL_PTR		VS_ERR_CODE(E_MOD_ID_GPE, E_ERR_NULL_PTR)
///< using a unexist handle
#define VS_ERR_GPE_UNEXIST		VS_ERR_CODE(E_MOD_ID_GPE, E_ERR_UNEXIST)
///< not support
#define VS_ERR_GPE_NOT_SUPPORT		VS_ERR_CODE(E_MOD_ID_GPE, E_ERR_NOT_SUPPORT)
///< system is not ready
#define VS_ERR_GPE_NOT_INITIALIZED	VS_ERR_CODE(E_MOD_ID_GPE, E_ERR_NOT_INITIALIZED)
///< This operation is not allowed
#define VS_ERR_GPE_NOT_PERM		VS_ERR_CODE(E_MOD_ID_GPE, E_ERR_NOT_PERM)
///< There is no memory
#define VS_ERR_GPE_NO_MEMORY		VS_ERR_CODE(E_MOD_ID_GPE, E_ERR_NO_MEMORY)
///< Internal nodes(job or task) are not enough */
#define VS_ERR_GPE_BUF_EMPTY		VS_ERR_CODE(E_MOD_ID_GPE, E_ERR_BUF_EMPTY)
///< operation timed out */
#define VS_ERR_GPE_TIMEOUT		VS_ERR_CODE(E_MOD_ID_GPE, E_ERR_TIMEOUT)
///< gpe module unexist */
#define VS_ERR_GPE_DEVICE_UNEXIST		VS_ERR_CODE(E_MOD_ID_GPE, E_ERR_DEVICE_UNEXIST)

#ifdef __cplusplus
}
#endif

#endif /* __VS_GPE_DEFINES_H__ */
