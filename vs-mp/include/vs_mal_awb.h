/**
* @file vs_mal_awb.h
* @brief Declaraction of awb api and struct
* @details
* @author VS software group
* @date 2023-08-30
* @version v0.10
* @Copyright (C) 2023, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/

#ifndef __VS_MAL_AWB_H__
#define __VS_MAL_AWB_H__

#include "vs_type.h"
#include "vs_isp_defines.h"

#ifdef __cplusplus
extern "C"{
#endif

vs_int32_t vs_mal_awb_ops_get(vs_isp_awblib_ops_s **p_awb_ops);

#ifdef __cplusplus
}
#endif
#endif
