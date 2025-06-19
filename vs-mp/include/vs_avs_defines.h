/**
* @file vs_avs_defines.h
* @brief Declaraction of avs enumeration and struct
* @details Declaraction of avs enumeration and struct
* @author visinextek
* @date 2022-08-27
* @version v0.1
* @Copyright (C) 2021, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/

#ifndef __VS_AVS_DEINES_H__
#define __VS_AVS_DEINES_H__

#include "vs_errno.h"
#include "vs_comm_video.h"

#ifdef __cplusplus
extern "C" {
#endif

#define AVS_PIPE_NUM 4

/**
* @brief Define the stitching mode.
* @details Supports two modes of stitching: blend and non blend
*/
typedef enum vs_avs_mode {
    E_AVS_MODE_BLEND = 0,		 ///< blend stitching mode
    E_AVS_MODE_NOBLEND_VER,		 ///< vertical non blend stitching mode
    E_AVS_MODE_NOBLEND_HOR,		 ///< horizontal non blend stitching mode
    E_AVS_MODE_NOBLEND_QR,		 ///< grid non blend stitching mode
    E_AVS_MODE_MAX
} vs_avs_mode_e;

/**
* @brief Define lookup table data structure
* @details Define lookup table data structure
*/
typedef struct vs_avs_lut {
    vs_uint64_t phys_addr;		///< Physica address of lookup table for each pipe
} vs_avs_lut_s;


/**
* @brief Define mode and coefficient of brightness gain.
* @details Manual mode and automatic mode.
*/
typedef enum vs_avs_gain_mode {
    E_AVS_GAIN_MODE_MANUAL = 0,		///< Manual mode
    E_AVS_GAIN_MODE_AUTO,			///< Automatic mode。
    E_AVS_GAIN_MODE_MAX
} vs_avs_gain_mode_e;

/**
* @brief Brightness gain control attribute
* @details Mode and coefficient of brightness gain.
*/
typedef struct vs_avs_gain_attr {
    vs_avs_gain_mode_e mode;		///< Brightness gain mode
    vs_int32_t coef[AVS_PIPE_NUM];	///< Coefficient of manual mode of each pipe
} vs_avs_gain_attr_s;

/**
* @brief Define the attitude of the stitching group.
* @details stitching mode, number of stitching pipes, synchronization, look-up table, 
		   gain control, stitching output attribute, stitching frame rate control.
*/
typedef struct vs_avs_grp_attr {
    vs_avs_mode_e mode; 		///< The stitching mode.
    vs_uint32_t pipe_num;		///< Number of stitching pipes.
    vs_bool_t sync_enable; 		///< Whether synchronization is required between each pipe.
    vs_avs_lut_s lut; 			///< Lookup table for each pipe.
    vs_avs_gain_attr_s gain_attr;	///< stitching gain control attribute.
    vs_video_frc_s  framerate; 			///< Frame rate control of stitching image.
} vs_avs_grp_attr_s;

/**
* @brief Defines the attitude of the channel output.
* @details size of the channel,depth, ouput mode, frame rate control.
*/
typedef struct vs_avs_phys_chn_width_attr {
    vs_int32_t output_width;	///< Width of channel output.
    vs_int32_t width_gap;		///< width gap of channel output.
} vs_avs_phys_chn_width_attr_s;


typedef struct vs_avs_chn_attr {
    union {
        vs_avs_phys_chn_width_attr_s width_attr;   /* phys chn ouput width info */
        vs_int32_t width;  						/* ext chn output width */
    } width_info;
    vs_int32_t height;		///< Height of channel output.
    vs_int32_t depth;		///< The depth of the channel image.
    vs_video_frc_s  framerate;	///< Frame rate control of channel output image.
} vs_avs_chn_attr_s;




///< parameter is illagal */
#define VS_ERR_AVS_INVALID_PARAM	VS_ERR_CODE(E_MOD_ID_AVS, E_ERR_INVALID_PARAM)
///< using a NULL pointer
#define VS_ERR_AVS_NULL_PTR			VS_ERR_CODE(E_MOD_ID_AVS, E_ERR_NULL_PTR)
///< using a exist handle
#define VS_ERR_AVS_GRP_EXIST		VS_ERR_CODE(E_MOD_ID_AVS, E_ERR_EXIST)
///< using a unexist handle
#define VS_ERR_AVS_UNEXIST			VS_ERR_CODE(E_MOD_ID_AVS, E_ERR_UNEXIST)
///< not support
#define VS_ERR_AVS_NOT_SUPPORT		VS_ERR_CODE(E_MOD_ID_AVS, E_ERR_NOT_SUPPORT)
///< system is not ready
#define VS_ERR_AVS_NOT_INITIALIZED	VS_ERR_CODE(E_MOD_ID_AVS, E_ERR_NOT_INITIALIZED)
///< This operation is not allowed
#define VS_ERR_AVS_NOT_PERM			VS_ERR_CODE(E_MOD_ID_AVS, E_ERR_NOT_PERM)
///< There is no memory
#define VS_ERR_AVS_NO_MEMORY		VS_ERR_CODE(E_MOD_ID_AVS, E_ERR_NO_MEMORY)
///< Internal nodes(task) are not enough */
#define VS_ERR_AVS_BUF_EMPTY		VS_ERR_CODE(E_MOD_ID_AVS, E_ERR_BUF_EMPTY)
#define VS_ERR_AVS_BUF_FULL			VS_ERR_CODE(E_MOD_ID_AVS, E_ERR_BUF_FULL)
///< system busy
#define VS_ERR_AVS_BUSY				VS_ERR_CODE(E_MOD_ID_AVS, E_ERR_BUSY)


///< avs module unexist */
#define VS_ERR_AVS_DEVICE_UNEXIST	VS_ERR_CODE(E_MOD_ID_AVS, E_ERR_DEVICE_UNEXIST)

#ifdef __cplusplus
}
#endif

#endif /* __VS_AVS_DEINES_H__ */
