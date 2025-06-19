/**
* @file
* @brief Declaration of mmz mal api
* @details Declaration of mmz mal api
* @author VS software group
* @date 2021-10-20
* @version v1.0
* @Copyright (C) 2021, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/

#ifndef __VS_MAL_LOG_H__
#define __VS_MAL_LOG_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "vs_type.h"
#include "vs_log_defines.h"

/* set log level by specific module ID or all module with E_MOD_ID_MAX*/
vs_int32_t vs_mal_log_level_set(vs_mod_id_e id, vs_uint32_t level);
/* get log level by specific module ID */
vs_int32_t vs_mal_log_level_get(vs_mod_id_e id, vs_uint32_t *p_level);
/* read log form the buffer, block or nonblock read*/
vs_int32_t vs_mal_log_read(vs_char_t *p_buf, vs_uint32_t size, vs_bool_t block);

vs_void_t vs_log(vs_mod_id_e mod_id,
		vs_uint32_t level,
		const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif
