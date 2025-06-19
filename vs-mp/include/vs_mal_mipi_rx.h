/**
* @file
* @brief Declaration of mipi_rx mal api
* @details Declaration of mipi_rx mal api
* @author VS software group
* @date 2022-01-10
* @version v1.0
* @Copyright (C) 2022, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/

#ifndef __VS_MAL_MIPI_RX_H__
#define __VS_MAL_MIPI_RX_H__

#ifdef __cplusplus
	extern "C" {
#endif


#include "vs_type.h"
#include "vs_mipi_rx_defines.h"

/**
* @ingroup      vs_mal_mipi_rx_dev_attr_set
* @brief        set device attribute
* @details
* @param [in]   devid     the number of mipi/sublvds controller
* @param [in]   p_attr    the device attribute
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_mipi_rx_dev_attr_set(vs_int32_t devid,
		const vs_mipi_rx_dev_attr_s *p_attr);

/**
* @ingroup      vs_mal_mipi_rx_sensor_reset
* @brief        reset sensor
* @details
* @param [in]   reset_source    reset source of sensor
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_mipi_rx_sensor_reset(vs_uint32_t reset_source);

/**
* @ingroup      vs_mal_mipi_rx_sensor_unreset
* @brief        unreset sensor
* @details
* @param [in]   reset_source    reset source of sensor
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_mipi_rx_sensor_unreset(vs_uint32_t reset_source);

/**
* @ingroup      vs_mal_mipi_rx_lane_mode_set
* @brief        set lane mode
* @details
* @param [in]   lane_mode       lane mode
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_mipi_rx_lane_mode_set(vs_lane_mode_e lane_mode);

/**
* @ingroup      vs_mal_mipi_rx_lane_mode_get
* @brief        get lane mode
* @details
* @param [in]   p_lane_mode     the pointer of lane mode
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_mipi_rx_lane_mode_get(vs_lane_mode_e *p_lane_mode);

/**
* @ingroup      vs_mal_mipi_rx_error_stats_get
* @brief        get error stats
* @details
* @param [in]	devid			the number of mipi/sublvds controller
* @param [out]	p_error_stats	the pointer of error status
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_mipi_rx_error_stats_get(vs_int32_t devid,
		vs_mipi_rx_error_stats_s *p_error_stats);


#ifdef __cplusplus
}
#endif

#endif
