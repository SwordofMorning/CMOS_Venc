
/**
* @file vs_ref_rvs.h
* @brief Declaration of ref API.
* @details Declaration of ref API.
* @author VS software group
* @date 2025-01-01
* @version v0.01
* @copyright (C) 2025, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/

#ifndef __VS_REF_RVS_H__
#define __VS_REF_RVS_H__

/*****************************************************************************
 *                    Includes
 *****************************************************************************/
#include "vs_type.h"
#include "vs_ref_rvs_defines.h"

#ifdef __cplusplus
extern "C"{
#endif


/*****************************************************************************
 *                    REF API Prototypes
 *****************************************************************************/

/**
* @ingroup      vs_ref
* @brief        Start ref related threads
* @details      Start ref related threads
* @param [in]   p_attr          ref attribution
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_ref_rvs_start(vs_ref_rvs_attr_s *p_attr);

/**
* @ingroup      vs_ref
* @brief        Stop ref related threads
* @details      Stop ref related threads
* @param [in]   p_attr          ref attribution
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_ref_rvs_stop(vs_ref_rvs_attr_s *p_attr);


/**
* @ingroup      vs_ref
* @brief        Move imu data into queue
* @details      Move imu data into queue
* @param [in]   buf_size     imu data buffer size
* @param [in]   type         imu data type
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_ref_rvs_imu_queue(vs_int32_t buf_size, vs_ref_rvs_imu_data_type_e type);

/**
* @ingroup      vs_ref
* @brief        Get one buffer from queue
* @details      Get one buffer from queue
* @param [out]  pp_buf       imu data buffer pointer
* @param [in]   buf_size     imu data buffer size
* @param [in]   type         imu data type
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_ref_rvs_imu_dequeue(vs_int8_t **pp_buf, vs_int32_t buf_size, vs_ref_rvs_imu_data_type_e type);


/**
* @ingroup      vs_ref
* @brief        Get correction binary
* @details      Get correction binary
* @param [in]   vii_pipeid      vii pipe id
* @param [in]   frame_id        frame id
* @param [in]   type            rvs type
* @param [out]  p_bin           correction binary
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_ref_rvs_binary_get(vs_int32_t vii_pipeid, vs_int32_t frame_id, vs_rvs_type_e type, vs_correction_algo_cfg_s *p_bin);


#ifdef __cplusplus
}
#endif

#endif

