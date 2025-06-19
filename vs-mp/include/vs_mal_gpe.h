/**
* @file vs_mal_gpe.h
* @brief common graphic process engine function define
* @details 
* @author visinextek
* @date 2021-11-30
* @version v0.1
* @Copyright (C) 2021, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/

#ifndef __VS_MAL_GPE_H__
#define __VS_MAL_GPE_H__

#include "vs_errno.h"
#include "vs_comm_video.h"
#include "vs_gpe_defines.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
* @brief       Start a new job
* @details     Create a new job, and some task in next
* @param [in]  p_job_handle, a job handle to control this job and the task in this job
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @par
*/
vs_int32_t vs_mal_gpe_job_start(vs_int32_t *p_job_handle);

/**
* @brief       Finish a job
* @details     Commit all the tasks of this job to hardware one by one, and wait process done
* @param [in]  job_handle, handle of this job
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @par
*/
vs_int32_t vs_mal_gpe_job_finish(vs_int32_t job_handle);

/**
* @brief       Cancel a started job
* @details     Cancel a job, direct cancel this job if it's not end, or wait current task done and stop
* @param [in]  job_handle, the handle of job which need cancel
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @par
*/
vs_int32_t vs_mal_gpe_job_cancel(vs_int32_t job_handle);

/**
* @brief       Add a scale task to this job's task list
* @details     Add a task which could finish scale src image to dst buffer
* @param [in]  job_handle, the handle of this job
* @param [in]  p_task_attr, the source and dest information of this task
* @param [in]  filter_type, which type of filter be used for the next scale operation
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @par
*/
vs_int32_t vs_mal_gpe_scale_task_add(vs_int32_t job_handle, const vs_task_attr_s *p_task_attr, vs_gpe_filter_type_e filter_type);

/**
* @brief       Add a rotation task to this job's task list
* @details     The rotation task could impl 90/180/270 rotation
* @param [in]  job_handle, the handle of this job
* @param [in]  p_task_attr, the source and dest information of this task
* @param [in]  p_rotation_attr, means fixed rotation attribute need be set
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @par
*/
vs_int32_t vs_mal_gpe_rotation_task_add(vs_int32_t job_handle, const vs_task_attr_s *p_task_attr, const vs_rotation_attr_s *p_rotation_attr);

/**
* @brief       Add a task to implement single line drawing
* @details     GPE will draw a line from start point to end point by process this task
* @param [in]  job_handle, the handle of this job
* @param [in]  p_task_attr, the source and dest information of this task
* @param [in]  p_line_attr, contains start and end point, and other draw line parameters
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @par
*/
vs_int32_t vs_mal_gpe_line_task_add(vs_int32_t job_handle, const vs_task_attr_s *p_task_attr, const vs_gpe_line_attr_s *p_line_attr);

/**
* @brief       Add a task to draw lines
* @details     GPE will draw multi lines by processing this task
* @param [in]  job_handle, the handle of this job
* @param [in]  p_task_attr, the source and dest information of this task
* @param [in]  lines_attr[], the attribute array contains many draw line parameter
* @param [in]  line_num, the number of line attributes
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @par
*/
vs_int32_t vs_mal_gpe_multiline_task_add(vs_int32_t job_handle, const vs_task_attr_s *p_task_attr, const vs_gpe_line_attr_s lines_attr[], vs_uint32_t line_num);

/**
* @brief       Add a draw cover task
* @details     Add a new cover task into the job task list to draw a cover on source video image
* @param [in]  job_handle, the handle of this job
* @param [in]  p_task_attr, the source and dest information of this task
* @param [in]  p_cover_attr, the attributes for GPE used to draw a cover
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @par
*/
vs_int32_t vs_mal_gpe_cover_task_add(vs_int32_t job_handle, const vs_task_attr_s *p_task_attr, const vs_gpe_cover_attr_s *p_cover_attr);

/**
* @brief       
* @details     GPE will draw a set of covers in this task
* @param [in]  job_handle, the handle of this job
* @param [in]  p_task_attr, the source and dest information of this task
* @param [in]  covers_attr[], cover attributes array to support draw many covers
* @param [in]  cover_num, the number of cover attributes
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @par
*/
vs_int32_t vs_mal_gpe_multicover_task_add(vs_int32_t job_handle, const vs_task_attr_s *p_task_attr, const vs_gpe_cover_attr_s covers_attr[], vs_uint32_t cover_num);

/**
* @brief       Add a draw osd task 
* @details     Add a osd task for deal with overlay region for other modules
* @param [in]  job_handle, the handle of this job
* @param [in]  p_task_attr, the source and dest information of this task
* @param [in]  p_osd_attr, the attributes for gpe draw a osd
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @par
*/
vs_int32_t vs_mal_gpe_osd_task_add(vs_int32_t job_handle, const vs_task_attr_s *p_task_attr, const vs_gpe_osd_attr_s *p_osd_attr);

/**
* @brief       Add a task to draw many osds
* @details     Add a multi osd draw task for deal with many overlay region in a single task
* @param [in]  job_handle, the handle of this job
* @param [in]  p_task_attr, the source and dest information of this task
* @param [in]  osds_attr, the attributes for gpe draw a osd
* @param [in]  osd_num, the number of osd attributes
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @par
*/
vs_int32_t vs_mal_gpe_multiosd_task_add(vs_int32_t job_handle, const vs_task_attr_s *p_task_attr, const vs_gpe_osd_attr_s osds_attr[], vs_uint32_t osd_num);

/**
* @brief       
* @details     Add a task for getting luma for many regions
* @param [in]  job_handle, the handle of this job
* @param [in]  p_task_attr, the source and dest information of this task
* @param [in]  luma_rects, regions rectangle define
* @param [in]  luma_num, the number of regions
* @param [in]  luma_results, array for store those regions luma result
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @par
*/
vs_int32_t vs_mal_gpe_multiluma_task_add(vs_int32_t job_handle, const vs_task_attr_s *p_task_attr, const vs_rect_s luma_rects[], vs_uint32_t luma_num, vs_uint64_t luma_results[]);

/**
* @brief
* @details     Add a convert format task
* @param [in]  job_handle, the handle of this job
* @param [in]  p_task_attr, the source and dest information of this task
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @par
*/
vs_int32_t vs_mal_gpe_pixel_convert_task_add(vs_int32_t job_handle, const vs_task_attr_s *p_task_attr);


#ifdef __cplusplus
}
#endif

#endif /* __VS_MAL_GPE_H__ */
