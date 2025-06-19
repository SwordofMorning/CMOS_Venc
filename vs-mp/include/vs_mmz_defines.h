/**
* @file
* @brief Declaration of mmz mal api and struct
* @details Declaration of mmz mal api and struct
* @author VS software group
* @date 2021-10-20
* @version v1.0
* @Copyright (C) 2021, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/

#ifndef __VS_MMZ_DEFINES_H__
#define __VS_MMZ_DEFINES_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "vs_type.h"
#include "vs_errno.h"
#include "vs_comm.h"

#define VS_ERR_MMZ_NULL_PTR          VS_ERR_CODE(E_MOD_ID_BASE, E_ERR_NULL_PTR)
#define VS_ERR_MMZ_INVALID_PARAM     VS_ERR_CODE(E_MOD_ID_BASE, E_ERR_INVALID_PARAM)
#define VS_ERR_MMZ_DEVICE_UNEXIST    VS_ERR_CODE(E_MOD_ID_BASE, E_ERR_DEVICE_UNEXIST)

#define VS_INVALID_MMZ_PHYSADDR (-1UL)

#ifdef __cplusplus
}
#endif


#endif
