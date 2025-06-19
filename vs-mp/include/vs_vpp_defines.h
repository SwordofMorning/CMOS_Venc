/**
* @file vs_vpp_defines.h
* @brief Declaration of vpp enumeration and struct
* @details Declaration of vpp enumeration and struct
* @author VS vpp group
* @date 2021-6-30
* @version v0.10
* @Copyright (C) 2021, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/


#ifndef __VS_VPP_DEFINES_H__
#define __VS_VPP_DEFINES_H__


#include "vs_comm.h"
#include "vs_comm_video.h"
#include "vs_vbm_defines.h"
#include "vs_errno.h"
#include "vs_capability.h"

#ifdef __cplusplus
extern "C"{
#endif


/*****************************************************************************************/

/**
* @brief Defines the vpp channel mode.
* @details
*/
typedef enum vs_vpp_chn_mode {
    E_VPP_CHN_MODE_USER = 0,        ///< channel user mode
    E_VPP_CHN_MODE_AUTO,            ///< channel auto mode
    E_VPP_CHN_MODE_MAX
}vs_vpp_chn_mode_e;

/**
* @brief Defines the vpp channel process mode.
* @details
*/
typedef enum vs_vpp_chn_proc_mode {
    E_VPP_CHN_PROC_MODE_VIDEO = 0,      ///< channel process video mode
    E_VPP_CHN_PROC_MODE_SNAP,           ///< channel process snap mode
    E_VPP_CHN_PROC_MODE_MAX
} vs_vpp_chn_proc_mode_e;

/**
* @brief Defines the modules register vpp.
* @details
*/
typedef enum vs_vpp_mod_id {
    E_VPP_MOD_ID_VO = 0,        ///< vo module ID
    E_VPP_MOD_ID_PCIV,          ///< pciv module ID
    E_VPP_MOD_ID_VPP,           ///< vpp module ID
    E_VPP_MOD_ID_AVS,           ///< avs module ID
    E_VPP_MOD_ID_VENC,          ///< venc module ID
    E_VPP_MOD_ID_MAX
} vs_vpp_mod_id_e;

/**
* @brief Defines NVR arbitration return values.
* @details
*/
typedef enum vs_vpp_register_ret {
    E_VPP_REGISTER_RET_PROC = 0,
    E_VPP_REGISTER_RET_DISCARD,
    E_VPP_REGISTER_RET_BACKPRESS,
    E_VPP_REGISTER_RET_MAX
} vs_vpp_register_ret_e;

/**
 * @brief Defines vpp stitch atrributes
 * @details
 */
typedef struct vs_vpp_grp_stitch_attr {
    vs_bool_t enable;
    vs_uint32_t grp_num;
    vs_uint32_t grpid[VPP_MAX_STITCH_GRP_NUM];
    vs_uint32_t hor_blk_num;
    vs_uint32_t ver_blk_num;
    vs_uint32_t hor_overlap;
    vs_uint32_t ver_overlap;
} vs_vpp_grp_stitch_attr_s;

/**
* @brief Define vpp group attributes.
* @details
*/
typedef struct vs_vpp_grp_attr {
    vs_uint32_t         max_width;      ///< group support max width，static parameter
    vs_uint32_t         max_height;     ///< group support max height，static parameter
    vs_pixel_format_e   pixel_format;   ///< pixel format, only support SP422、SP420、400，static parameter
    vs_dynamic_range_e  dynamic_range;  ///< dynamic range of the frame
    vs_video_frc_s      framerate;      ///< frame rate control information
} vs_vpp_grp_attr_s;

/**
* @brief Define vpp channel attributes.
* @details
*/
typedef struct vs_vpp_chn_attr {
    vs_vpp_chn_mode_e   chn_mode;       ///< channel mode
    vs_uint32_t         width;          ///< channel width
    vs_uint32_t         height;         ///< channel height
    vs_video_format_e   video_format;   ///< video_format, only support E_VIDEO_FORMAT_LINEAR
    vs_pixel_format_e   pixel_format;   ///< pixel format, only support SP422、SP420、SP400
    vs_dynamic_range_e  dynamic_range;  ///< dynamic range of the frame
    vs_compress_mode_e  compress_mode;  ///< video compression format, only support E_COMPRESS_MODE_NONE
    vs_video_frc_s      framerate;      ///< frame rate control information
    vs_bool_t           mirror_enable;  ///< enable mirror or not
    vs_bool_t           flip_enable;    ///< enable flip or not
    vs_uint32_t         depth;          ///< channel frame queue depth
    vs_aspect_ratio_s   aspect_ratio;   ///< aspect ratio information
} vs_vpp_chn_attr_s;

/**
* @brief Define vpp extended channel attributes.
* @details
*/
typedef struct vs_vpp_extchn_attr {
    vs_int32_t          bind_chnid;         ///< extended channel bind channel id
    vs_uint32_t         width;              ///< extended channel width
    vs_uint32_t         height;             ///< extended channel height
    vs_video_format_e   video_format;       ///< video_format, only support E_VIDEO_FORMAT_LINEAR
    vs_pixel_format_e   pixel_format;       ///< pixel format, only support SP422、SP420、SP400
    vs_dynamic_range_e  dynamic_range;      ///< dynamic range of the frame
    vs_compress_mode_e  compress_mode;      ///< video compression format, only support E_COMPRESS_MODE_NONE
    vs_uint32_t         depth;              ///< extended channel frame queue depth
    vs_video_frc_s      framerate;          ///< frame rate control information
} vs_vpp_extchn_attr_s;

/**
* @brief Define vpp crop information.
* @details
*/
typedef struct vs_vpp_crop_info {
    vs_bool_t               enable;             ///< enable crop or not
    vs_coordinate_mode_e    coordinate_mode;    ///< coordinate  mode
    vs_rect_s               rect;               ///< crop rect information
} vs_vpp_crop_info_s;


///< parameter is illegal
#define VS_ERR_VPP_INVALID_PARAM      VS_ERR_CODE(E_MOD_ID_VPP, E_ERR_INVALID_PARAM)
///< invalid channel ID */
#define VS_ERR_VPP_INVALID_CHNID      VS_ERR_CODE(E_MOD_ID_VPP, E_ERR_INVALID_CHNID)
///< invalid group ID */
#define VS_ERR_VPP_INVALID_GRPID      VS_ERR_CODE(E_MOD_ID_VPP, E_ERR_INVALID_GRPID)
///< using a NULL pointer
#define VS_ERR_VPP_NULL_PTR           VS_ERR_CODE(E_MOD_ID_VPP, E_ERR_NULL_PTR)
///< channel has been created
#define VS_ERR_VPP_GRP_EXIST          VS_ERR_CODE(E_MOD_ID_VPP, E_ERR_EXIST)
///< channel is not created
#define VS_ERR_VPP_GRP_UNEXIST        VS_ERR_CODE(E_MOD_ID_VPP, E_ERR_UNEXIST)
///< operation is not supported
#define VS_ERR_VPP_NOT_SUPPORT        VS_ERR_CODE(E_MOD_ID_VPP, E_ERR_NOT_SUPPORT)
///< operation is not implemented
#define VS_ERR_VPP_NOT_IMPLEMENTED    VS_ERR_CODE(E_MOD_ID_VPP, E_ERR_NOT_IMPLEMENTED)
///< system is not ready
#define VS_ERR_VPP_NOT_INITIALIZED    VS_ERR_CODE(E_MOD_ID_VPP, E_ERR_NOT_INITIALIZED)
///< operation is not permitted
#define VS_ERR_VPP_NOT_PERM           VS_ERR_CODE(E_MOD_ID_VPP, E_ERR_NOT_PERM)
///< no memory for allocated
#define VS_ERR_VPP_NO_MEMORY          VS_ERR_CODE(E_MOD_ID_VPP, E_ERR_NO_MEMORY)
///< buffer is empty
#define VS_ERR_VPP_BUF_EMPTY          VS_ERR_CODE(E_MOD_ID_VPP, E_ERR_BUF_EMPTY)
///< buffer is full
#define VS_ERR_VPP_BUF_FULL           VS_ERR_CODE(E_MOD_ID_VPP, E_ERR_BUF_FULL)
///< address is bad
#define VS_ERR_VPP_BAD_ADDR           VS_ERR_CODE(E_MOD_ID_VPP, E_ERR_BAD_ADDR)
///< system busy
#define VS_ERR_VPP_BUSY               VS_ERR_CODE(E_MOD_ID_VPP, E_ERR_BUSY)
///< task timeout
#define VS_ERR_VPP_TIMEOUT            VS_ERR_CODE(E_MOD_ID_VPP, E_ERR_TIMEOUT)
///< device unexist
#define VS_ERR_VPP_DEVICE_UNEXIST     VS_ERR_CODE(E_MOD_ID_VPP, E_ERR_DEVICE_UNEXIST)

#ifdef __cplusplus
}
#endif
#endif /*!<  __VS_VPP_DEFINES_H__ */
