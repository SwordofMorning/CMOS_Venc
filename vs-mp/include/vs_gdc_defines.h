/**
 * @file vs_gdc_defines.h
 * @brief Declaration of gdc define and struct
 * @details Declaration of gdc define and struct
 * @author VS software group
 * @date 2022-5-18
 * @version v0.01
 * @Copyright (C) 2021, Shanghai Visinex Technologies Co., Ltd. All rights
 * reserved.
 */

#ifndef __VS_GDC_DEFINES_H__
#define __VS_GDC_DEFINES_H__

#include "vs_type.h"
#include "vs_errno.h"
#include "vs_capability.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************************/
///< parameter is illegal
#define VS_ERR_GDC_INVALID_PARAM	VS_ERR_CODE(E_MOD_ID_GDC, E_ERR_INVALID_PARAM)
///< invalid ID
#define VS_ERR_GDC_INVALID_GRPID	VS_ERR_CODE(E_MOD_ID_GDC, E_ERR_INVALID_GRPID)
///< using a NULL pointer
#define VS_ERR_GDC_NULL_PTR		VS_ERR_CODE(E_MOD_ID_GDC, E_ERR_NULL_PTR)
///< operation is not supported
#define VS_ERR_GDC_NOT_SUPPORT		VS_ERR_CODE(E_MOD_ID_GDC, E_ERR_NOT_SUPPORT)
///< system is not ready
#define VS_ERR_GDC_NOT_INITIALIZED	VS_ERR_CODE(E_MOD_ID_GDC, E_ERR_NOT_INITIALIZED)
///< operation is not permitted
#define VS_ERR_GDC_NOT_PERM		VS_ERR_CODE(E_MOD_ID_GDC, E_ERR_NOT_PERM)
///< no memory for allocated
#define VS_ERR_GDC_NO_MEMORY		VS_ERR_CODE(E_MOD_ID_GDC, E_ERR_NO_MEMORY)
///< buffer is empty
#define VS_ERR_GDC_BUF_EMPTY		VS_ERR_CODE(E_MOD_ID_GDC, E_ERR_BUF_EMPTY)
///< buffer is full
#define VS_ERR_GDC_BUF_FULL		VS_ERR_CODE(E_MOD_ID_GDC, E_ERR_BUF_FULL)
///< address is bad
#define VS_ERR_GDC_BAD_ADDR		VS_ERR_CODE(E_MOD_ID_GDC, E_ERR_BAD_ADDR)
///< system busy
#define VS_ERR_GDC_BUSY			VS_ERR_CODE(E_MOD_ID_GDC, E_ERR_BUSY)
///< task timeout
#define VS_ERR_GDC_TIMEOUT		VS_ERR_CODE(E_MOD_ID_GDC, E_ERR_TIMEOUT)
///< Device unexists
#define VS_ERR_GDC_DEVICE_UNEXIST	VS_ERR_CODE(E_MOD_ID_GDC, E_ERR_DEVICE_UNEXIST)

#ifdef __cplusplus
}
#endif
#endif /*!<  __VS_GDC_DEFINES_H__ */

