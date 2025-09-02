/**
* @file vs_ref_infra_sensor_defines.h
* @brief Declaraction of rtd2121w sensor ctrl function
* @details
* @author VS software group
* @date 2024-10-10
* @version v0.10
* @Copyright (C) 2024, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/
#ifndef __VS_REF_INFRA_SENSOR_DEFINES_H__
#define __VS_REF_INFRA_SENSOR_DEFINES_H__

#include "vs_infra_defines.h"
#include "vs_comm_video.h"

#ifdef __cplusplus
extern "C"{
#endif

#define VS_REF_INFRA_SENSOR_MAX_ITEM_NUM 8

typedef struct vs_ref_infra_iframe_item {
    vs_infra_iframe_cfg_s cfg;
    vs_int32_t delay_ms;
} vs_ref_infra_iframe_item_s;

typedef struct vs_ref_infra_sensor_attr {
    vs_infra_attr_s  infra_attr;
    vs_ref_infra_iframe_item_s array[VS_REF_INFRA_SENSOR_MAX_ITEM_NUM];
    vs_infra_ooc_cfg_s ooc_cfg;
} vs_ref_infra_sensor_attr_s;

#ifdef __cplusplus
}
#endif

#endif // __VS_REF_INFRA_SENSOR_DEFINES_H__