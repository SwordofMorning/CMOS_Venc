/**
* @file
* @brief dsp defines
* @details dsp defines
* @author VS software group
* @date 2022-05-16
* @version v1.0
* @Copyright (C) 2022, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/

#ifndef __VS_DSP_DEFINES_H__
#define __VS_DSP_DEFINES_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "vs_type.h"
#include "vs_comm.h"
#include "vs_errno.h"

#define VS_ERR_DSP_INVALID_PARAM     VS_ERR_CODE(E_MOD_ID_DSP, E_ERR_INVALID_PARAM)
#define VS_ERR_DSP_NULL_PTR          VS_ERR_CODE(E_MOD_ID_DSP, E_ERR_NULL_PTR)
#define VS_ERR_DSP_NOT_INITIALIZED   VS_ERR_CODE(E_MOD_ID_DSP, E_ERR_NOT_INITIALIZED)
#define VS_ERR_DSP_NO_MEMORY         VS_ERR_CODE(E_MOD_ID_DSP, E_ERR_NO_MEMORY)
#define VS_ERR_DSP_DEVICE_EXIST      VS_ERR_CODE(E_MOD_ID_DSP, E_ERR_EXIST)
#define VS_ERR_DSP_DEVICE_UNEXIST    VS_ERR_CODE(E_MOD_ID_DSP, E_ERR_DEVICE_UNEXIST)
#define VS_ERR_DSP_TIMEOUT           VS_ERR_CODE(E_MOD_ID_DSP, E_ERR_TIMEOUT)
#define VS_ERR_DSP_ABORTED           VS_ERR_CODE(E_MOD_ID_DSP, E_ERR_ABORTED)
#define VS_ERR_DSP_PANIC             VS_ERR_CODE(E_MOD_ID_DSP, E_ERR_BAD_STATE)

typedef enum vs_dsp_id {
	E_DSP_ID_0 = 0,
	E_DSP_ID_1,
	E_DSP_ID_MAX
} vs_dsp_id_e;

typedef enum vs_dsp_pri {
	E_DSP_PRI_0 = 0,
	E_DSP_PRI_1,
	E_DSP_PRI_2,
	E_DSP_PRI_MAX
} vs_dsp_pri_e;

typedef struct vs_dsp_message {
	vs_uint32_t cmd;
	vs_uint32_t msgid;
	vs_uint64_t body;
	vs_uint32_t len;
} vs_dsp_message_s;

#ifdef __cplusplus
}
#endif

#endif

