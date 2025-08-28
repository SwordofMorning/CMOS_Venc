/**
* @file vs_ref_infra_sensor_ctrl.h
* @brief Declaraction of infra sensor ctrl function
* @details
* @author VS software group
* @date 2024-10-10
* @version v0.10
* @Copyright (C) 2024, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/
#ifndef __VS_REF_INFRA_SENSOR_CTRL_H__
#define __VS_REF_INFRA_SENSOR_CTRL_H__

#include "vs_ref_infra_sensor_defines.h"

#ifdef __cplusplus
extern "C"{
#endif

vs_int32_t vs_ref_infra_sensor_attr_get(vs_int32_t mode_id, vs_ref_infra_sensor_attr_s *p_attr);

vs_int32_t vs_ref_infra_sensor_init(vs_int32_t mode_id);

vs_int32_t vs_ref_infra_sensor_deinit(vs_int32_t mode_id);

vs_int32_t vs_ref_infra_sensor_stream_on(vs_int32_t mode_id);

vs_int32_t vs_ref_infra_sensor_stream_off(vs_int32_t mode_id);

vs_int32_t vs_ref_infra_sensor_ooc_update(vs_int32_t mode_id,
        vs_infra_ooc_cfg_s *p_ooc_cfg, vs_bool_t *is_inbound);

vs_int32_t vs_ref_infra_sensor_frame_process(vs_int32_t mode_id,
        vs_video_frame_info_s *p_src_frm, vs_video_frame_info_s *p_dst_frm);

#ifdef __cplusplus
}
#endif

#endif // __VS_REF_INFRA_SENSOR_CTRL_H__