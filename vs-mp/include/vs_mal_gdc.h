/**
 * @file vs_mal_gdc.h
 * @brief Declaration of gdc enumeration and struct
 * @details Declaration of gdc enumeration and struct
 * @author VS software group
 * @date 2021-12-6
 * @version v0.01
 * @Copyright (C) 2021, Shanghai Visinex Technologies Co., Ltd. All rights
 * reserved.
 */

#ifndef __VS_MAL_GDC_H__
#define __VS_MAL_GDC_H__

#include "vs_comm_video.h"
#include "vs_type.h"
#include "vs_errno.h"
#include "vs_gdc_defines.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
* @brief  Set the gdc job begin.
* @details Set the gdc job begin.
* @param [out]	p_handle Return the job handle.
* @return	0 call success.
* @return	none 0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_gdc_job_begin(vs_int32_t *p_handle);

/**
* @brief  Add a fisheye task by job handle.
* @details Add a fisheye task by job handle.
* @param [in]  handle Indicate the job handle number to be given.
* @param [in]  p_gdc_task， The pointer of struct gdc, can not be NULL.
* @param [in]  p_fisheye_s The pointer of struct fisheye, can not be NULL.
* @return	0 call success.
* @return	none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_gdc_fisheye_task_add(vs_int32_t handle, const vs_task_attr_s *p_gdc_task,
									const vs_fisheye_attr_s *p_fisheye_s);

/**
* @brief  Add a ldc task by job handle.
* @details Add a ldc task by job handle.
* @param [in]  handle Indicate the job handle number to be given.
* @param [in]  p_gdc_task， The pointer of struct gdc, can not be NULL.
* @param [in]  p_ldc_s The pointer of struct ldc, can not be NULL.
* @return	0 call success.
* @return	none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_gdc_ldc_task_add(vs_int32_t handle, const vs_task_attr_s *p_gdc_task,
								const vs_ldc_attr_s *p_ldc_s);

/**
* @brief Add a rotation task by job handle.
* @details Add a rotation task by job handle.
* @param [in]  job handle .
* @param [in]  p_gdc_task， The pointer of struct gdc, can not be NULL.
* @param [in]  p_rotation_s The pointer of struct ldc, can not be NULL.
* @return	0 call success.
* @return	none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_gdc_rotation_task_add(vs_int32_t handle, const vs_task_attr_s *p_gdc_task,
									const vs_rotation_attr_s *p_rotation_s);

/**
* @brief  Stop add gdc task & start work.
* @details Stop add gdc task & start work.
* @param [in]  job handle .
* @return	0 call success.
* @return	none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_gdc_job_end(vs_int32_t handle);

/**
* @brief  Cancel the gdc job by handle
* @details Cancel the gdc job by handle
* @param [in]  job handle .
* @return	0 call success.
* @return	none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_gdc_job_cancel(vs_int32_t handle);

#ifdef __cplusplus
}
#endif
#endif /*!<  __VS_MAL_GDC_H__ */
