/**
* @file
* @brief Declaraction of mmz mal api and struct
* @details Declaraction of mmz mal api and struct
* @author VS software group
* @date 2022-05-16
* @version v1.0
* @Copyright (C) 2022, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/

#ifndef __VS_LOG_DEFINES_H__
#define __VS_LOG_DEFINES_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "vs_type.h"
#include "vs_errno.h"
#include "vs_comm.h"

#define VS_LOG_NOPRINT	0
#define VS_LOG_ERROR	1
#define VS_LOG_WARN	2
#define VS_LOG_INFO	3
#define VS_LOG_DEBUG	4
#define VS_LOG_DEFAULT	VS_LOG_ERROR
#define VS_LOG_LEVEL_MAX	VS_LOG_DEBUG

#define VS_ERR_LOG_NULL_PTR          VS_ERR_CODE(E_MOD_ID_LOG, E_ERR_NULL_PTR)
#define VS_ERR_LOG_INVALID_PARAM     VS_ERR_CODE(E_MOD_ID_LOG, E_ERR_INVALID_PARAM)
#define VS_ERR_LOG_DEVICE_UNEXIST    VS_ERR_CODE(E_MOD_ID_LOG, E_ERR_DEVICE_UNEXIST)

#ifdef __cplusplus
}
#endif

#endif


