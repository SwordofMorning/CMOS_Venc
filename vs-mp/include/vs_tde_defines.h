/**
* @file vs_tde_defines.h
* @brief common two-dimensional engine data type define
* @details
* @author visinextek
* @date 2022-07-02
* @version v0.1
* @Copyright (C) 2021, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/

#ifndef __VS_TDE_DEFINES_H__
#define __VS_TDE_DEFINES_H__

#include "vs_errno.h"
#include "vs_comm_video.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
* @brief Define the bitmap information
* @details
*/
typedef struct vs_tde_surface {
    vs_uint64_t        phy_addr;         ///< bitmap data's phy addr or Y data's phy addr
    vs_pixel_format_e  pixel_format;     ///< pixel format
    vs_uint32_t        height;           ///< surface height, value range (0, 8192]
    vs_uint32_t        width;            ///< surface width, value range (0, 8192]
    vs_uint32_t        stride;           ///< bitmap data's stride or Y data's phy addr
    vs_bool_t          alpha_extend;     ///< enable alpha extend when ARGB1555
    vs_uint8_t         ext_alpha0;       ///< one extend alpha value
    vs_uint8_t         ext_alpha1;       ///< another extend alpha value
    vs_uint64_t        CbCr_phy_addr;    ///< CbCr data's phy addr if any
    vs_uint32_t        CbCr_stride;      ///< CbCr data's stride if any
} vs_tde_surface_s;

/**
* @brief Define the blend mode
* @details
*/
typedef enum vs_tde_blend_mode {
    E_TDE_BLEND_MODE_ZERO = 0x0,
    E_TDE_BLEND_MODE_ONE,
    E_TDE_BLEND_MODE_SRC2COLOR,
    E_TDE_BLEND_MODE_INVSRC2COLOR,
    E_TDE_BLEND_MODE_SRC2ALPHA,
    E_TDE_BLEND_MODE_INVSRC2ALPHA,
    E_TDE_BLEND_MODE_SRC1COLOR,
    E_TDE_BLEND_MODE_INVSRC1COLOR,
    E_TDE_BLEND_MODE_SRC1ALPHA,
    E_TDE_BLEND_MODE_INVSRC1ALPHA,
    E_TDE_BLEND_MODE_SRC2ALPHASAT,
    E_TDE_BLEND_MODE_MAX
}vs_tde_blend_mode_e;

/**
* @brief Define the blend command
* @details
*/
typedef enum vs_tde_blend_cmd {
    E_TDE_BLEND_CMD_CLEAR = 0,
    E_TDE_BLEND_CMD_SRC,
    E_TDE_BLEND_CMD_SRC_OVER,
    E_TDE_BLEND_CMD_DST_OVER,
    E_TDE_BLEND_CMD_SRC_IN,
    E_TDE_BLEND_CMD_DST_IN,
    E_TDE_BLEND_CMD_SRC_OUT,
    E_TDE_BLEND_CMD_DST_OUT,
    E_TDE_BLEND_CMD_SRC_ATOP,
    E_TDE_BLEND_CMD_DST_ATOP,
    E_TDE_BLEND_CMD_ADD,
    E_TDE_BLEND_CMD_XOR,
    E_TDE_BLEND_CMD_DST,
    E_TDE_BLEND_CMD_CONFIG,
    E_TDE_BLEND_CMD_MAX
}vs_tde_blend_cmd_e;

/**
* @brief Define the blend operation
* @details
*/
typedef struct vs_tde_blend_opt {
    vs_bool_t global_alpha_enable;           ///< global alpha enable or not
    vs_bool_t pixel_alpha_enable;            ///< pixel alpha enable or not
    vs_bool_t src_alpha_premulti;            ///< src premultiply enable or not
    vs_bool_t dst_alpha_premulti;            ///< dst premultiply enable or not
    vs_tde_blend_cmd_e blend_cmd;            ///< porter or duff mode
    vs_tde_blend_mode_e src1_blend_mode;     ///< src1 blend mode
    vs_tde_blend_mode_e src2_blend_mode;     ///< src2 blend mode
} vs_tde_blend_opt_s;

/**
* @brief Define the filter type that TDE support
* @details Support two type of filter in horizontal and vertical direction filter
*/
typedef enum vs_tde_filter_type {
    E_TDE_FILTER_TYPE_3_TAP = 0,    ///< 3x3 filter matrix that TDE support for scale or deflicker
    E_TDE_FILTER_TYPE_5_TAP,        ///< 5x5 filter matrix that TDE support for scale or deflicker
    E_TDE_FILTER_TYPE_MAX
} vs_tde_filter_type_e;

/**
* @brief Define the clip mode
* @details
*/
typedef enum vs_tde_clip_mode {
    E_TDE_CLIP_MODE_NONE = 0,   ///< no clip
    E_TDE_CLIP_MODE_INSIDE,     ///< clip the inside rect
    E_TDE_CLIP_MODE_OUTSIDE,    ///< clip the outside rect
    E_TDE_CLIP_MODE_MAX
} vs_tde_clip_mode_e;

/**
* @brief Define the operation mode
* @details
*/
typedef enum vs_tde_opt_mode {
    E_TDE_BIT_BLIT_NONE = 0x0,   ///< no any operation
    E_TDE_BIT_BLIT_BLEND = 0x1,  ///< alpha blend operation
    E_TDE_BIT_BLIT_MAX = 0x2,
} vs_tde_opt_mode_e;

/**
* @brief Define the fill color
* @details
*/
typedef struct vs_tde_fill_color {
    vs_pixel_format_e pixel_format;    ///< pixel format
    vs_uint32_t       color;           ///< the color value be filled
} vs_tde_fill_color_s;

/**
* @brief Definition of the semi-planar YUV data
* @details
*/
typedef struct vs_tde_mb {
    vs_pixel_format_e pixel_format;   ///< pixel format
    vs_uint64_t       Y_phy_addr;     ///< Y data's phy addr
    vs_uint32_t       width;          ///< mb width, value range (0, 8192]
    vs_uint32_t       height;         ///< mb height, value range (0, 8192]
    vs_uint32_t       stride;         ///< Y data's stride
    vs_uint64_t       CbCr_phy_addr;  ///< CbCr data's phy addr
    vs_uint32_t       CbCr_stride;    ///< CbCr stride's phy addr
} vs_tde_mb_s;

/**
* @brief Definition of macroblock operation options
* @details
*/
typedef struct vs_tde_mb_opt {
    vs_tde_clip_mode_e    clip_mode;     ///< clip mode inside or outside
    vs_rect_s             clip_rect;     ///< clip rect in mb
    vs_bool_t             deflicker;     ///< deflicker mb or not
    vs_bool_t             scale;         ///< scale fg rect to bg or not
    vs_tde_filter_type_e  filter;        ///< scale or deflicker filter type
    vs_bool_t             alpha_enable;  ///< enable specific alpha or not
    vs_uint8_t            alpha;         ///< specific alpha value
} vs_tde_mb_opt_s;

/**
* @brief Definition of blit operation options
* @details
*/
typedef struct vs_tde_opt {
    vs_tde_opt_mode_e         mode;          ///< the bit blit option mode
    vs_tde_clip_mode_e        clip_mode;     ///< clip mode inside or outside
    vs_rect_s                 clip_rect;     ///< clip rect in blend surface
    vs_bool_t                 deflicker;     ///< deflicker or not
    vs_bool_t                 scale;         ///< scale fg rect to bg or not
    vs_tde_filter_type_e      filter;        ///< scale or deflicker filter type
    vs_mirror_mode_e          mirror;        ///< mirror blend surface
    vs_uint8_t                global_alpha;  ///< blend global alpha
    vs_tde_blend_opt_s        blend_opt;     ///< alpha blend operation
} vs_tde_opt_s;

/**
* @brief Definition of composed surface info
* @details
*/
typedef struct vs_tde_compose {
    vs_tde_surface_s  src_surf;    ///< blending src surface
    vs_rect_s         src_rect;    ///< blending src rect in src surface
    vs_rect_s         dst_rect;    ///< blending dst rect in dst surface
    vs_tde_opt_s      opt;         ///< blending operation
} vs_tde_compose_s;

/**
* @brief Definition of composed surface list
* @details
*/
typedef struct vs_tde_surface_list {
    vs_uint32_t       surface_num;     ///< multi blending surface num
    vs_tde_surface_s  *p_dst_surf;     ///< blending dst surface
    vs_tde_compose_s  *p_compose;      ///< compose src surface attr
} vs_tde_surface_list_s;

///< parameter is illegal */
#define VS_ERR_TDE_INVALID_PARAM    VS_ERR_CODE(E_MOD_ID_TDE, E_ERR_INVALID_PARAM)
///< using a NULL pointer
#define VS_ERR_TDE_NULL_PTR        VS_ERR_CODE(E_MOD_ID_TDE, E_ERR_NULL_PTR)
///< using a unexist handle
#define VS_ERR_TDE_UNEXIST        VS_ERR_CODE(E_MOD_ID_TDE, E_ERR_UNEXIST)
///< not support
#define VS_ERR_TDE_NOT_SUPPORT        VS_ERR_CODE(E_MOD_ID_TDE, E_ERR_NOT_SUPPORT)
///< system is not ready
#define VS_ERR_TDE_NOT_INITIALIZED    VS_ERR_CODE(E_MOD_ID_TDE, E_ERR_NOT_INITIALIZED)
///< This operation is not allowed
#define VS_ERR_TDE_NOT_PERM        VS_ERR_CODE(E_MOD_ID_TDE, E_ERR_NOT_PERM)
///< There is no memory
#define VS_ERR_TDE_NO_MEMORY        VS_ERR_CODE(E_MOD_ID_TDE, E_ERR_NO_MEMORY)
///< Internal nodes(job or task) are not enough */
#define VS_ERR_TDE_BUF_EMPTY        VS_ERR_CODE(E_MOD_ID_TDE, E_ERR_BUF_EMPTY)
///< tde module unexist */
#define VS_ERR_TDE_DEVICE_UNEXIST        VS_ERR_CODE(E_MOD_ID_TDE, E_ERR_DEVICE_UNEXIST)

#ifdef __cplusplus
}
#endif

#endif  /* __VS_TDE_DEFINES_H__ */
