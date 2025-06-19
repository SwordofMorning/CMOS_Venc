/**
* @file vs_sys_defines.h
* @brief Declaration of sys enumeration, struct and API.
* @details Declaration of sys enumeration, struct and API.
* @author VS software group
* @date 2021-11-02
* @version v0.01
* @copyright (C) 2021, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/
#ifndef __VS_SYS_DEFINES_H__
#define __VS_SYS_DEFINES_H__

/*****************************************************************************
 *                    Includes
 *****************************************************************************/
#include "vs_type.h"
#include "vs_comm.h"
#include "vs_errno.h"
#include "vs_capability.h"

#ifdef __cplusplus
extern "C"{
#endif

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/
///< Invalid parameter
#define VS_ERR_SYS_INVALID_PARAM      VS_ERR_CODE(E_MOD_ID_SYS, E_ERR_INVALID_PARAM)
///< Unexpected Null pointer
#define VS_ERR_SYS_NULL_PTR           VS_ERR_CODE(E_MOD_ID_SYS, E_ERR_NULL_PTR)
///< Resource exists
#define VS_ERR_SYS_EXIST              VS_ERR_CODE(E_MOD_ID_SYS, E_ERR_EXIST)
///< Resource unexists
#define VS_ERR_SYS_UNEXIST            VS_ERR_CODE(E_MOD_ID_SYS, E_ERR_UNEXIST)
///< Operation not supported
#define VS_ERR_SYS_NOT_SUPPORT        VS_ERR_CODE(E_MOD_ID_SYS, E_ERR_NOT_SUPPORT)
///< Target not initialized
#define VS_ERR_SYS_NOT_INITIALIZED    VS_ERR_CODE(E_MOD_ID_SYS, E_ERR_NOT_INITIALIZED)
///< Operation not permitted
#define VS_ERR_SYS_NOT_PERM           VS_ERR_CODE(E_MOD_ID_SYS, E_ERR_NOT_PERM)
///< Out of memory
#define VS_ERR_SYS_NO_MEMORY          VS_ERR_CODE(E_MOD_ID_SYS, E_ERR_NO_MEMORY)
///< Device unexist
#define VS_ERR_SYS_DEVICE_UNEXIST      VS_ERR_CODE(E_MOD_ID_SYS, E_ERR_DEVICE_UNEXIST)

#define VS839T_CHIP_ID	"VS839T"
#define VS839_CHIP_ID	"VS839"
#define VS819_CHIP_ID	"VS819"
#define VS819L_CHIP_ID	"VS819L"
#define VS816_CHIP_ID	"VS816"
#define VS815_CHIP_ID	"VS815"
#define VS835_CHIP_ID	"VS835"

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/
typedef struct vs_sys_bind_chn_list {
    vs_uint32_t num;                         ///< the num in bind list
    vs_chn_s chn[SYS_BIND_MAX_NUM];   ///< the bind attribute
} vs_sys_bind_chn_list_s;

typedef struct vs_uuid {
    vs_uint32_t id[SYS_UUID_NUM];
} vs_uuid_s;


#ifdef __cplusplus
}
#endif

#endif // __VS_SYS_DEFINES_H__
