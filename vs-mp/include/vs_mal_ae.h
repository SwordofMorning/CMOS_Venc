/**
* @file vs_mal_ae.h
* @brief Declaraction of ae api and struct
* @details
* @author VS software group
* @date 2023-08-30
* @version v0.10
* @Copyright (C) 2023, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/

#ifndef __VS_MAL_AE_H__
#define __VS_MAL_AE_H__

#ifdef __cplusplus
extern "C"{
#endif

#include "vs_type.h"
#include "vs_isp_defines.h"

vs_int32_t vs_mal_ae_ops_get(vs_isp_aelib_ops_s **p_ae_ops);

#ifdef __cplusplus
}
#endif
#endif
