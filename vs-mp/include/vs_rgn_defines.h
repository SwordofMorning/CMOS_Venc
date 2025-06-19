/**
* @file vs_rgn_defines.h
* @brief This file declare enumeration, struct of region.
* @details
* @author visinextek
* @date 2021-12-30
* @version v0.1
* @Copyright (C) 2021, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/

#ifndef __VS_RGN_DEFINES_H__
#define __VS_RGN_DEFINES_H__

#include "vs_type.h"
#include "vs_errno.h"
#include "vs_comm.h"
#include "vs_comm_video.h"
#include "vs_capability.h"

#ifdef __cplusplus
extern "C" {
#endif

/* One or more parameter is invalid, be used for create region or bind to channel */
#define VS_ERR_RGN_INVALID_PARAM                VS_ERR_CODE(E_MOD_ID_REGION, E_ERR_INVALID_PARAM)
/* Invalid device id be used for binding the region */
#define VS_ERR_RGN_INVALID_DEVID                VS_ERR_CODE(E_MOD_ID_REGION, E_ERR_INVALID_DEVID)
/* Invalid channel id be used for binding the region */
#define VS_ERR_RGN_INVALID_CHNID                VS_ERR_CODE(E_MOD_ID_REGION, E_ERR_INVALID_CHNID)
/* Unexpected Null pointer */
#define VS_ERR_RGN_NULL_PTR                     VS_ERR_CODE(E_MOD_ID_REGION, E_ERR_NULL_PTR)
/* Region create error becaue the region of this handle is exist */
#define VS_ERR_RGN_EXIST                        VS_ERR_CODE(E_MOD_ID_REGION, E_ERR_EXIST)
/* Region destroy error becaue the region of this handle is exist */
#define VS_ERR_RGN_UNEXIST                      VS_ERR_CODE(E_MOD_ID_REGION, E_ERR_UNEXIST)
/* Region ko not insmod or inmod not completed */
#define VS_ERR_RGN_NOT_INITIALIZED              VS_ERR_CODE(E_MOD_ID_REGION, E_ERR_NOT_INITIALIZED)
/* operation not permitted */
#define VS_ERR_RGN_NOT_PERM                     VS_ERR_CODE(E_MOD_ID_REGION, E_ERR_NOT_PERM)
/* Not enough memory for malloc */
#define VS_ERR_RGN_NO_MEMORY                    VS_ERR_CODE(E_MOD_ID_REGION, E_ERR_NO_MEMORY)
/* getting region surface buffer timeout */
#define VS_ERR_RGN_BUF_FULL                     VS_ERR_CODE(E_MOD_ID_REGION, E_ERR_BUF_FULL)
/* Invalid addr for system */
#define VS_ERR_RGN_BAD_ADDR                     VS_ERR_CODE(E_MOD_ID_REGION, E_ERR_BAD_ADDR)
/* region is on used by other module */
#define VS_ERR_RGN_BUSY                         VS_ERR_CODE(E_MOD_ID_REGION, E_ERR_BUSY)
/* region model not insert to kernel */
#define VS_ERR_RGN_DEVICE_UNEXIST               VS_ERR_CODE(E_MOD_ID_REGION, E_ERR_DEVICE_UNEXIST)

/* Invalid mod id for module register */
#define VS_ERR_RGN_INVALID_MODID                VS_ERR_CODE(E_MOD_ID_REGION, E_RGN_ERR_INVALID_MODID)
/* Invalid handle be used for the region */
#define VS_ERR_RGN_INVALID_HANDLE               VS_ERR_CODE(E_MOD_ID_REGION, E_RGN_ERR_INVALID_HANDLE)
/* Invalid region type be used for the region */
#define VS_ERR_RGN_INVALID_REGION_TYPE          VS_ERR_CODE(E_MOD_ID_REGION, E_RGN_ERR_INVALID_REGION_TYPE)
/* bind chn num over limit */
#define VS_ERR_RGN_BIND_OVER_LIMIT              VS_ERR_CODE(E_MOD_ID_REGION, E_RGN_ERR_BIND_OVER_LIMIT)
/* the input chn module param is unregisted */
#define VS_ERR_RGN_MODULE_UNREGISTERED            VS_ERR_CODE(E_MOD_ID_REGION, E_RGN_ERR_MODULE_UNREGISTERED)

/**
* @brief The error code that rgn can report
* @details The specific error code rgn module define
*/
typedef enum vs_rgn_errno {
    E_RGN_ERR_INVALID_MODID    = 100,  ///< the input module id is illegal
    E_RGN_ERR_INVALID_HANDLE,          ///< the input region handle is illegal or not created
    E_RGN_ERR_INVALID_REGION_TYPE,     ///< no define for this region type
    E_RGN_ERR_INVALID_REGION_ATTR,     ///< region attribute setting is illegal
    E_RGN_ERR_INVALID_DISPLAY_ATTR,    ///< region display attributes are not correct on the specific channel
    E_RGN_ERR_BIND_OVER_LIMIT,         ///< region number on the channel beyond the maximum
    E_RGN_ERR_MODULE_UNREGISTERED,     ///< module unregistered on region
    E_RGN_ERR_MAX
} vs_rgn_errno_e;

/**
* @brief Define the type of regions those rgn module can support
* @details There define five types, cover, mosaic, overlay, coverex and overlayex
*/
typedef enum vs_rgn_type {
    E_RGN_TYPE_COVER     = 0,   ///< the region type is cover, can be displayed in vpp group
    E_RGN_TYPE_COVEREX,         ///< the region type is coverex, can be displayed in vi/vpp/vo channel
    E_RGN_TYPE_MOSAIC,          ///< the region type is mosaic, can be displayed in vpp group
    E_RGN_TYPE_OVERLAY,         ///< the region type is overlay, can be displayed in venc channel
    E_RGN_TYPE_OVERLAYEX,       ///< the region type is overlayex, can be displayed in vi/vpp/vo/pciv/avs channel
    E_RGN_TYPE_MAX
} vs_rgn_type_e;

/**
* @brief Define the public attributes of overlay region
* @details The public attribute contains size, format, background color, and number of buffers surface used
*/
typedef struct vs_rgn_overlay_attr {
    vs_size_s             size;           ///< overlay region size
    vs_pixel_format_e     format;         ///< image format, only support ARGB8888 on venc channel
    vs_uint32_t           bgcolor;        ///< background color, if the actually image size less than region size, will display the color of this value
    vs_uint32_t           surface_num;    ///< will alloc buffer for this region by using mmz
} vs_rgn_overlay_attr_s;

/**
* @brief Define the public attributes of overlayex region
* @details The public attribute contains size, format, background color, and number of buffers surface used
*/
typedef struct vs_rgn_overlayex_attr {
    vs_size_s             size;           ///< overlayex region size
    vs_pixel_format_e     format;         ///< image format, support ARGB1555/ARGB4444/ARGB888 for vi/vpp/vo and others
    vs_uint32_t           bgcolor;        ///< background color, if the actually image size less than region size, will display the color of this value
    vs_uint32_t           surface_num;    ///< will alloc buffer for this region by using mmz
} vs_rgn_overlayex_attr_s;

/**
* @brief Define the common attributes of overlay and overlayex
* @detail Define union type for compatible both  overlay and overlayex, no attr for cover,coverex and mosaic
*/
typedef union vs_rgn_comm_attr {
    vs_rgn_overlay_attr_s   overlay;      ///< overlay region attributes
    vs_rgn_overlayex_attr_s overlayex;    ///< overlayex region attributes
} vs_rgn_comm_attr_u;

/**
* @brief Define common region public attributes
* @details The common region attribute including region type for cover/mosaic/overlay, but only overlay region support detail attributes
*/
typedef struct vs_rgn_attr {
    vs_rgn_type_e             type;       ///< region type definition
    vs_rgn_comm_attr_u        attr;       ///< regions attribute type for both overlay and overlayex
} vs_rgn_attr_s;

/**
* @brief Define the rect type of cover region
* @details Cover region support two types, rectangle and quad rectangle
*/
typedef enum vs_rgn_area_type {
    E_RGN_COVER_TYPE_RECT     = 0,    ///< the common rectangle of cover
    E_RGN_COVER_TYPE_QUAD,            ///< the irregular rectangle, there means quad rectangle
    E_RGN_COVER_TYPE_MAX
} vs_rgn_area_type_e;

/**
* @brief Define the cover region display attribute just on vpp group
* @details After each cover region applied to channel, will using follow attribute to control the hardware to display it
*/
typedef struct vs_rgn_cover_disp_attr {
    vs_rgn_area_type_e     area;    ///< rect type be used for cover region define
    union {
        vs_rect_s          rect;    ///< definition of rectangle
        vs_quadrangle_s    quad;    ///< definition of quad rectangle
    };
    vs_uint32_t            color;   ///< fill soid cover by using this color value
    vs_uint32_t            zorder;  ///< the z-order of each cover if need compare the display sequence, the same meaninfg to depth
    vs_coordinate_mode_e   coord;   ///< coordinate mode of four points of rect
} vs_rgn_cover_disp_attr_s;

/**
* @brief Define the coverex region display attribute
* @details coverex only support rectangle, and will use GPE hardware fill the cover rectangle
*/
typedef struct vs_rgn_coverex_disp_attr {
    vs_rgn_area_type_e     area;     ///< rect type be used for cover region define
    union {
        vs_rect_s          rect;    ///< definition of rectangle
        vs_quadrangle_s    quad;    ///< definition of quad rectangle
    };
    vs_uint32_t            color;   ///< fill coverex rect by using this colorvalue
    vs_uint32_t            zorder;  ///< the z-order of each cover if need compare the display sequence, the same meaninfg to depth
    vs_coordinate_mode_e   coord;   ///< coordinate mode of four points of rect
} vs_rgn_coverex_disp_attr_s;

/**
* @brief Define the quantizer parameter
* @details This quantizer parameter be used for venc for deal the overlay region, if the region be applied to other modules, ignore it
*/
typedef struct vs_rgn_overlay_qp_info {
    vs_bool_t     enable;           ///< enable qp protect or not in venc module
    vs_bool_t     abs_qp;           ///< is absolute qp or relative qp
    vs_int32_t    qp_value;         ///< value of quantizer parameter
} vs_rgn_overlay_qp_info_s;

/**
* @brief Define the overlay region display attribute on the video channel
* @details The overlay display attribute be used for osd blend in other hardware
*/
typedef struct vs_rgn_overlay_disp_attr {
    vs_point_s                  point;      ///< overlay rect top-left point located on the video frame
    vs_uint32_t                 fgalpha;    ///< reserve
    vs_uint32_t                 bgalpha;    ///< reserve
    vs_uint32_t                 zorder;     ///< the z-order of each overlay region
    vs_coordinate_mode_e        coord;      ///< coordinate mode of four points of rect
    vs_rgn_overlay_qp_info_s    qp;         ///< quantizer parameter information
} vs_rgn_overlay_disp_attr_s;

/**
* @brief Define the overlayex region display attribute on the video channel
* @details The overlay display attribute be used for osd blend in other hardware
*/
typedef struct vs_rgn_overlayex_disp_attr {
    vs_point_s                  point;      ///< overlayex rect top-left point located on the video frame
    vs_uint32_t                 fgalpha;    ///< the fore ground alpha value be usd if the format is ARGB1555
    vs_uint32_t                 bgalpha;    ///< the back ground alpha value be used if the format is ARGB1555
    vs_uint32_t                 zorder;     ///< the z-order of each overlay region
    vs_coordinate_mode_e        coord;      ///< coordinate mode of four points of rect
} vs_rgn_overlayex_disp_attr_s;

/**
* @brief Define the mosaic block size type
* @details The mosaic only be support on vpp module, and the vpp support some block size for mosaic algo using
*/
typedef enum vs_rgn_mosaic_block_size {
    E_RGN_MOSAIC_BLOCK_8     = 0,    ///< the hardware do mosaic algo on 8x8   block size
    E_RGN_MOSAIC_BLOCK_16,           ///< the hardware do mosaic algo on 16x16 block size
    E_RGN_MOSAIC_BLOCK_32,           ///< the hardware do mosaic algo on 32x32 block size
    E_RGN_MOSAIC_BLOCK_MAX
} vs_rgn_mosaic_block_size_e;

/**
* @brief Define the mosaic display attribute on video channel
* @details Display information of mosaic region
*/
typedef struct vs_rgn_mosaic_disp_attr {
    vs_rect_s                      rect;    ///< the region definition of mosaic
    vs_rgn_mosaic_block_size_e     block;   ///< the mosaic block size use
    vs_uint32_t                    zorder;  ///< the z-order of each mosaic region
    vs_coordinate_mode_e           coord;   ///< coordinate mode of four points of rect
} vs_rgn_mosaic_disp_attr_s;

/**
* @brief Define the common region display attribute
* @details including three region type display attributes
*/
typedef union vs_rgn_disp_attr {
    vs_rgn_cover_disp_attr_s         cover_disp;      ///< cover display attribute
    vs_rgn_coverex_disp_attr_s       coverex_disp;    ///< cover display attribute
    vs_rgn_mosaic_disp_attr_s        mosaic_disp;     ///< mosaic display attribute
    vs_rgn_overlay_disp_attr_s       overlay_disp;    ///< overlay display attribute
    vs_rgn_overlayex_disp_attr_s     overlayex_disp;  ///< overlay display attribute
} vs_rgn_disp_attr_u;

/**
* @brief Define the region display control information
* @details show or not, and may get the control information by region type
*/
typedef struct vs_rgn_disp_info {
    vs_bool_t              show;          ///< set the region be show
    vs_rgn_type_e          type;          ///< region type for choose a type from union
    vs_rgn_disp_attr_u     disp_attr;     ///< a region display attribute
} vs_rgn_disp_info_s;

/**
* @brief Define the surface information for store the osd data
* @details The memory information, and stride aligned with hw constraint
*/
typedef struct vs_rgn_surface_info {
    vs_uint64_t       phys_addr;        ///< the physic address of buffer alloc by mmz
    vs_void_t 	      *p_virt_addr;    ///< the virtual address be used in user space
    vs_size_s         size;            ///< the actual size according to region public attribute set by mal
    vs_uint32_t       stride;          ///< the stride according to width be aligned with hw alignment
    vs_pixel_format_e format;          ///< the format of picture, set by mal
} vs_rgn_surface_info_s;


#ifdef __cplusplus
}
#endif

#endif /* __VS_RGN_DEFINES_H__ */

