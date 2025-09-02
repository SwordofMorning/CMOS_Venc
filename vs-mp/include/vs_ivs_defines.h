
/**
* @file vs_ivs_defines.h
* @brief Declaration of ivs enumeration, struct and API.
* @details Declaration of ivs enumeration, struct and API.
* @author VS software group
* @date 2024-09-24
* @version v0.01
* @copyright (C) 2024, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/

#ifndef __VS_IVS_DEFINES_H__
#define __VS_IVS_DEFINES_H__

/*****************************************************************************
 *                    Includes
 *****************************************************************************/
#include "vs_type.h"
#include "vs_comm.h"
#include "vs_errno.h"
#include "vs_capability.h"
#include "vs_ive_defines.h"

#ifdef __cplusplus
extern "C"{
#endif

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

#define VS_ERR_IVS_INVALID_PARAM        VS_ERR_CODE(E_MOD_ID_IVS,E_ERR_INVALID_PARAM)           ///< parameter is invalid
#define VS_ERR_IVS_NULL_PTR             VS_ERR_CODE(E_MOD_ID_IVS,E_ERR_NULL_PTR)                ///< pointer val is null
#define VS_ERR_IVS_DEVICE_UNEXIST       VS_ERR_CODE(E_MOD_ID_IVS,E_ERR_DEVICE_UNEXIST)          ///< ivs device does not exist
#define VS_ERR_IVS_NOT_SUPPORT          VS_ERR_CODE(E_MOD_ID_IVS,E_ERR_NOT_SUPPORT)             ///< function unsupported
#define VS_ERR_IVS_NOT_INITIALIZED      VS_ERR_CODE(E_MOD_ID_IVS,E_ERR_NOT_INITIALIZED)         ///< module uninitialized
#define VS_ERR_IVS_ALIGNMENT            VS_ERR_CODE(E_MOD_ID_IVS,E_ERR_IVS_ALIGNMENT)	        ///< alignment error
#define VS_ERR_IVS_IMAGE_SIZE           VS_ERR_CODE(E_MOD_ID_IVS,E_ERR_IVS_IMAGE_SIZE)	        ///< image stride,width or height error
#define VS_ERR_IVS_OUT_OF_RANGE         VS_ERR_CODE(E_MOD_ID_IVS,E_ERR_IVS_OUT_OF_RANGE)        ///< value is out of range
#define VS_ERR_IVS_MODE                 VS_ERR_CODE(E_MOD_ID_IVS,E_ERR_IVS_MODE)                ///< mode error
#define VS_ERR_IVS_IMAGE_TYPE           VS_ERR_CODE(E_MOD_ID_IVS,E_ERR_IVS_IMAGE_TYPE)          ///< image type error
#define VS_ERR_IVS_SCALE_RATIO          VS_ERR_CODE(E_MOD_ID_IVS,E_ERR_IVS_SCALE_RATIO)	        ///< zoom scale error
#define VS_ERR_IVS_HANDLE               VS_ERR_CODE(E_MOD_ID_IVS,E_ERR_IVS_HANDLE)              ///< invalid handle
#define VS_ERR_IVS_TIMEOUT              VS_ERR_CODE(E_MOD_ID_IVS,E_ERR_TIMEOUT)                 ///< ivs ops timeout
#define VS_ERR_IVS_INVALID_CHNID        VS_ERR_CODE(E_MOD_ID_IVS, E_ERR_INVALID_CHNID)          ///< invalid chnid
#define VS_ERR_IVS_BAD_STATE            VS_ERR_CODE(E_MOD_ID_IVS, E_ERR_BAD_STATE)              ///< bad state mathine
#define VS_ERR_IVS_NOT_PERM             VS_ERR_CODE(E_MOD_ID_IVS, E_ERR_NOT_PERM)               ///< Operation not permitted
#define VS_ERR_IVS_NO_MEMORY            VS_ERR_CODE(E_MOD_ID_IVS, E_ERR_NO_MEMORY)              ///< Out of memory


#define IVS_OD_MAX_AREA_NUM        (64)


/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/**
* @brief The IVS error code
*/
typedef enum vs_ivs_error_code
{
    E_ERR_IVS_ALIGNMENT = 0x60,     ///< alignment error
    E_ERR_IVS_IMAGE_SIZE,           ///< image stride,width or height error
    E_ERR_IVS_OUT_OF_RANGE,         ///< value is out of range
    E_ERR_IVS_MODE,                 ///< mode error
    E_ERR_IVS_IMAGE_TYPE,           ///< image type error
    E_ERR_IVS_SCALE_RATIO,          ///< scale ratio error
    E_ERR_IVS_HANDLE,               ///< invalid handle
    E_ERR_IVS_CODE_MAX
}vs_ivs_error_code_e;

/**
 * @brief  the algorithm mode of md
*/
typedef enum vs_ivs_md_algo_mode{
    E_MD_ALGO_MODE_BG = 0x0,  ///< background
    E_MD_ALGO_MODE_REF,       ///< reference
    E_MD_ALGO_MODE_MAX
}vs_ivs_md_algo_mode_e;


/**
 * @brief the attribute of ivs md
*/
typedef struct vs_ivs_md_attr
{
    vs_ivs_md_algo_mode_e algo_mode;    ///< algorithm mode
    vs_ive_sad_split_mode_e sad_mode;   ///< sad mode
    vs_ive_sad_out_mode_e sad_out_ctrl; ///< sad output ctrl
    vs_uint16_t width;                  ///< image width
    vs_uint16_t height;                 ///< image height
    vs_uint16_t sad_thr[3];             ///< sad threshold
    vs_uint32_t global_motion_thr[2];   ///< global motion threshold
    vs_uint32_t local_motion_thr[2];    ///< sub-image motion threshold
}vs_ivs_md_attr_s;

/**
 * @brief the output info of ivs od
*/
typedef struct vs_ivs_od_info{
    vs_uint32_t od_ratio;                        ///< od ratio, [0, 100]
    vs_uint32_t od_num;                          ///< the number of the detected od area
    vs_rect_s   od_area[IVS_OD_MAX_AREA_NUM];    ///< the roi info of the od area
}vs_ivs_od_info_s;

/**
 * @brief the attribute of ivs od
*/
typedef struct vs_ivs_od_attr
{
    vs_uint16_t width;                  ///< image width
    vs_uint16_t height;                 ///< image height
    vs_uint32_t od_sens;                ///< od sensitivity attribute, [0, 100], the bigger value, the more sensitivity
    vs_uint32_t od_area_thr;            ///< the minimum percent of the od area (0, 100]
}vs_ivs_od_attr_s;


#ifdef __cplusplus
}
#endif

#endif
