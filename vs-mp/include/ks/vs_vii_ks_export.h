/**
* @file
* @brief vs_vii_ks_export.h
* @details
* @author VS software group
* @date 2023-02-15
* @version v1.0
* @Copyright (C) 2023, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/

#ifndef __VS_VII_KS_EXPORT_H__
#define __VS_VII_KS_EXPORT_H__

#include "vs_type.h"

typedef vs_void_t (*frame_start_callback)(vs_int32_t vii_devid);

vs_void_t vs_vii_frame_start_register(frame_start_callback callback_func);
vs_void_t vs_vii_frame_start_unregister(vs_void_t);

vs_void_t vs_vii_frame_flag_set(vs_int32_t vii_devid, vs_uint32_t flag);

#endif // __VS_VII_KS_EXPORT_H__