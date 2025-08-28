
/**
* @file vs_mal_rvs.h
* @brief Declaration of rvs API.
* @details Declaration of rvs API.
* @author VS software group
* @date 2025-01-01
* @version v0.01
* @copyright (C) 2025, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/

#ifndef __VS_MAL_RVS_H__
#define __VS_MAL_RVS_H__

/*****************************************************************************
 *                    Includes
 *****************************************************************************/
#include "vs_type.h"
#include "vs_comm.h"
#include "vs_comm_video.h"
#include "vs_errno.h"
#include "vs_rvs_defines.h"

#ifdef __cplusplus
extern "C"{
#endif


/*****************************************************************************
 *                    RVS API Prototypes
 *****************************************************************************/

/**
* @ingroup      vs_mal_rvs
* @brief        Initial rvs
* @details      Initial rvs
* @param [in]   type            rvs type
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_rvs_init(vs_rvs_type_e type);

/**
* @ingroup      vs_mal_rvs
* @brief        Release rvs
* @details      Release rvs
* @param [in]   type            rvs type
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_rvs_deinit(vs_rvs_type_e type);

/**
* @ingroup      vs_mal_rvs
* @brief        Set eis attribution
* @details      Set eis attribution
* @param [in]   vii_pipeid      vii pipe id
* @param [in]   p_attr          eis attribution
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_rvs_eis_attr_set(vs_int32_t vii_pipeid, vs_rvs_eis_attr_s *p_attr);

/**
* @ingroup      vs_mal_rvs
* @brief        Get eis attribution
* @details      Get eis attribution
* @param [in]   vii_pipeid      vii pipe id
* @param [out]  p_attr          eis attribution
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_rvs_eis_attr_get(vs_int32_t vii_pipeid, vs_rvs_eis_attr_s *p_attr);

/**
* @ingroup      vs_mal_rvs
* @brief        Calculate correction binary by IMU data
* @details      Calculate correction binary by IMU data
* @param [in]   p_imu_data      IMU data
* @param [in]   num             IMU data num
* @param [in]   p_rect          Image ROI
* @param [in]   frame_pts       Frame pts, unit:ns
* @param [out]  p_bin           correction binary
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_rvs_eis_process(vs_rvs_imu_data_s* p_imu_data, vs_int32_t num, vs_rect_s *p_rect, vs_uint64_t frame_pts, vs_correction_algo_cfg_s* p_bin);
#ifdef __cplusplus
}
#endif

#endif

