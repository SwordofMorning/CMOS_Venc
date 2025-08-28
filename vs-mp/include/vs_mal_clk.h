/**
* @file vs_mal_clk.h
* @brief clk gate on and off, clk rate set api
* @details
* @author visinextek
* @date 2024-10-9
* @version v0.1
* @Copyright (C) 2024, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/

#ifndef __VS_MAL_CLK_H__
#define __VS_MAL_CLK_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "vs_type.h"
#include "vs_clk_defines.h"

/**
* @ingroup      vs_mal_sys_clk_rate_set
* @brief        set clock rate
* @details      This api is used to set rate to clock
* @param [in]   name            name of clock
* @param [in]   rate            rate of clock
* @retval       VS_SUCCESS      no error
* @retval       others          error happens.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_sys_clk_rate_set(vs_char_t *name, vs_uint32_t rate);


/**
* @ingroup      vs_mal_sys_clk_enable
* @brief        clock enable
* @details      This api is used to open clock
* @param [in]   name            name of clock
* @retval       VS_SUCCESS      no error
* @retval       others          error happens.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_sys_clk_enable(vs_char_t *name);

/**
* @ingroup      vs_mal_sys_clk_disable
* @brief        clock disable
* @details      This api is used to close clock
* @param [in]   name            name of clock
* @retval       VS_SUCCESS      no error
* @retval       others          error happens.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_sys_clk_disable(vs_char_t *name);

#ifdef __cplusplus
}
#endif

#endif