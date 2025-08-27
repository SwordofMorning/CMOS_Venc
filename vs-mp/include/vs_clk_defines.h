/**
* @file vs_clk_defines.h
* @brief common clk data type define
* @details
* @author visinextek
* @date 2024-10-9
* @version v0.1
* @Copyright (C) 2024, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/

#ifndef __VS_CLK_DEFINES_H__
#define __VS_CLK_DEFINES_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "vs_comm.h"
#include "vs_type.h"
#include "vs_errno.h"

#define VS_ERR_CLK_INVALID_PARAM     VS_ERR_CODE(E_MOD_ID_BASE, E_ERR_INVALID_PARAM)
#define VS_ERR_CLK_DEVICE_UNEXIST    VS_ERR_CODE(E_MOD_ID_BASE, E_ERR_DEVICE_UNEXIST)

#ifdef __cplusplus
}
#endif

#endif