/**
* @file
* @brief mal dsp API
* @details mal dsp API
* @author VS software group
* @date 2022-03-29
* @version v1.0
* @Copyright (C) 2022, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/

#ifndef __VS_MAL_DSP_H__
#define __VS_MAL_DSP_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "vs_type.h"
#include "vs_dsp_defines.h"

vs_int32_t vs_mal_dsp_poweron(vs_dsp_id_e dsp_id);
vs_int32_t vs_mal_dsp_poweroff(vs_dsp_id_e dsp_id);
vs_int32_t vs_mal_dsp_load(vs_dsp_id_e dsp_id, const vs_char_t *p_file_name);
vs_int32_t vs_mal_dsp_enable(vs_dsp_id_e dsp_id);
vs_int32_t vs_mal_dsp_disable(vs_dsp_id_e dsp_id);
vs_int32_t vs_mal_dsp_rpc(vs_uint32_t *p_handle, const vs_dsp_message_s *p_msg,
                          vs_dsp_id_e dsp_id, vs_dsp_pri_e pri);
vs_int32_t vs_mal_dsp_query(
		vs_dsp_id_e dsp_id, vs_uint32_t handle,
		vs_bool_t *p_finish, vs_bool_t block);

#ifdef __cplusplus
}
#endif

#endif

