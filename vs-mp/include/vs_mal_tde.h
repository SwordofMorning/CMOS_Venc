/**
* @file vs_mal_tde.h
* @brief common two-dimensional engine function define
* @details
* @author visinextek
* @date 2022-06-15
* @version v0.1
* @Copyright (C) 2021, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/

#ifndef __VS_MAL_TDE_H__
#define __VS_MAL_TDE_H__

#include "vs_errno.h"
#include "vs_comm_video.h"
#include "vs_tde_defines.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
* @brief       Start a two-dimensional engine device
* @details     Create a two-dimensional engine device
* @param [in]  vs_void_t
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @par
*/
vs_int32_t vs_mal_tde_us_init(vs_void_t);

/**
* @brief       Stop a two-dimensional engine device
* @details     Close a two-dimensional engine device
* @param [in]  vs_void_t
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @par
*/
vs_int32_t vs_mal_tde_us_exit(vs_void_t);

/**
* @brief       Reset a two-dimensional engine device
* @details     Reset a two-dimensional engine device
* @param [in]  vs_void_t
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @par
*/
vs_int32_t vs_mal_tde_us_reset(vs_void_t);

/**
* @brief       Start a new job
* @details     Create a new job, and some task in next
* @param [in]  p_job_handle, a job handle to control this job and the task in this job
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @par
*/
vs_int32_t vs_mal_tde_job_start(vs_int32_t *p_job_handle);

/**
* @brief       Finish a job
* @details     Commit all the tasks of this job to hardware one by one, and wait process done
* @param [in]  job_handle, handle of this job
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @par
*/
vs_int32_t vs_mal_tde_job_finish(vs_int32_t job_handle, vs_bool_t block, vs_uint32_t timeout);

/**
* @brief       Cancel a started job
* @details     Cancel a job, direct cancel this job if it's not end, or wait current task done and stop
* @param [in]  job_handle, the handle of job which need cancel
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @par
*/
vs_int32_t vs_mal_tde_job_cancel(vs_int32_t job_handle);

/**
* @brief       Wait for the completion of a specified TDE task.
* @details     As a block interface, this API is blocked until the specified task is complete.
* @param [in]  job_handle, the handle of job which need to be waited
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @par
*/
vs_int32_t vs_mal_tde_wait_for_done(vs_int32_t job_handle);

/**
* @brief       Wait for the completion of all TDE tasks.
* @details     As a block interface, this API is blocked until all TDE tasks are complete.
* @param [in]  vs_void_t
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @par
*/
vs_int32_t vs_mal_tde_wait_all_done(vs_void_t);

/**
* @brief       Add a quick copy task
* @details     Add a task for fast copy operation.
* @param [in]  job_handle, the handle of this job
* @param [in]  p_src_surf, source bitmap
* @param [in]  p_src_rect, source rect
* @param [in]  p_dst_surf, destination bitmap
* @param [in]  p_dst_rect, destination rect
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @par
*/
vs_int32_t vs_mal_tde_quick_copy(vs_int32_t job_handle, const vs_tde_surface_s *p_src_surf,
                                                        const vs_rect_s *p_src_rect,
                                                        const vs_tde_surface_s *p_dst_surf,
                                                        const vs_rect_s *p_dst_rect);

/**
* @brief       Add a quick fill task
* @details     Add a task to filling in target bitmap with a fixed value.
* @param [in]  p_dst_surf, destination bitmap
* @param [in]  p_dst_rect, destination rect
* @param [in]  fill_color, fill color ARGB8888 value
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @par
*/
vs_int32_t vs_mal_tde_quick_fill(vs_int32_t job_handle, const vs_tde_surface_s *p_dst_surf,
                                                        const vs_rect_s *p_dst_rect,
                                                        vs_uint32_t fill_color);

/**
* @brief       Add a quick scale task
* @details     Add a task to scale the source bitmap based on a specified size of the target bitmap.
* @param [in]  job_handle, the handle of this job
* @param [in]  p_src_surf, source bitmap
* @param [in]  p_src_rect, source rect
* @param [in]  p_dst_surf, destination bitmap
* @param [in]  p_dst_rect, destination rect
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @par
*/
vs_int32_t vs_mal_tde_quick_scale(vs_int32_t job_handle, const vs_tde_surface_s *p_src_surf,
                                                         const vs_rect_s *p_src_rect,
                                                         const vs_tde_surface_s *p_dst_surf,
                                                         const vs_rect_s *p_dst_rect);

/**
* @brief       Add a quick de-flicker task
* @details     Add an de-flicker operation task.
* @param [in]  job_handle, the handle of this job
* @param [in]  p_src_surf, source bitmap
* @param [in]  p_src_rect, source rect
* @param [in]  p_dst_surf, destination bitmap
* @param [in]  p_dst_rect, destination rect
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @par
*/
vs_int32_t vs_mal_tde_quick_deflicker(vs_int32_t job_handle, const vs_tde_surface_s *p_src_surf,
                                                             const vs_rect_s *p_src_rect,
                                                             const vs_tde_surface_s *p_dst_surf,
                                                             const vs_rect_s *p_dst_rect);

/**
* @brief       Add a bit blit task
* @details     Adds the transfer operation (with additional functions available for the raster or macroblock bitmap) 
               to a TDE task
* @param [in]  job_handle, the handle of this job
* @param [in]  p_fg_surf, foreground bitmap
* @param [in]  p_fg_rect, foreground rect
* @param [in]  p_bg_surf, background bitmap
* @param [in]  p_bg_rect, background rect
* @param [in]  p_dst_surf, destination bitmap
* @param [in]  p_dst_rect, destination rect
* @param [in]  p_opt, operation
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @par
*/
vs_int32_t vs_mal_tde_bit_blit(vs_int32_t job_handle, const vs_tde_surface_s *p_fg_surf,
                                                      const vs_rect_s *p_fg_rect,
                                                      const vs_tde_surface_s *p_bg_surf,
                                                      const vs_rect_s *p_bg_rect,
                                                      const vs_tde_surface_s *p_dst_surf,
                                                      const vs_rect_s *p_dst_rect,
                                                      const vs_tde_opt_s *p_opt);

/**
* @brief       Add a solid draw task
* @details     Adds the fill and transfer operations (with additional functions available for the raster bitmap)
               to a TDE task
* @param [in]  job_handle, the handle of this job
* @param [in]  p_fg_surf, foreground bitmap
* @param [in]  p_fg_rect, foreground rect
* @param [in]  p_dst_surf, destination bitmap
* @param [in]  p_dst_rect, destination rect
* @param [in]  p_fill_color, fill color
* @param [in]  p_opt, operation
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @par
*/
vs_int32_t vs_mal_tde_solid_draw(vs_int32_t job_handle, const vs_tde_surface_s *p_fg_surf,
                                                        const vs_rect_s *p_fg_rect,
                                                        const vs_tde_surface_s *p_dst_surf,
                                                        const vs_rect_s *p_dst_rect,
                                                        const vs_tde_fill_color_s *p_fill_color,
                                                        const vs_tde_opt_s *p_opt);

/**
* @brief       Add a mb blit task
* @details     the luminance macroblock data and the chrominance macroblock data are combined into raster data.
               During the combination, the scaling, anti-flicker, and clip operations can be performed concurrently.
* @param [in]  job_handle, the handle of this job
* @param [in]  p_mb,
* @param [in]  p_mb_rect, mb rect
* @param [in]  p_dst_surf, destination bitmap
* @param [in]  p_dst_rect, destination rect
* @param [in]  p_mb_opt, mb operation
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @par
*/
vs_int32_t vs_mal_tde_mb_blit(vs_int32_t job_handle, const vs_tde_mb_s *p_mb,
                                                     const vs_rect_s *p_mb_rect,
                                                     const vs_tde_surface_s *p_dst_surf,
                                                     const vs_rect_s *p_dst_rect,
                                                     const vs_tde_mb_opt_s *p_mb_opt);

/**
* @brief       Add a rotation task
* @details     Add a rotation operation task
* @param [in]  job_handle, the handle of this job
* @param [in]  p_src_surf, source bitmap
* @param [in]  p_src_rect, source rect
* @param [in]  p_dst_surf, destination bitmap
* @param [in]  p_dst_rect, destination rect
* @param [in]  p_rotation, means fixed rotation attribute need be set
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @par
*/
vs_int32_t vs_mal_tde_rotate(vs_int32_t job_handle, const vs_tde_surface_s *p_src_surf,
                                                    const vs_rect_s *p_src_rect,
                                                    const vs_tde_surface_s *p_dst_surf,
                                                    const vs_rect_s *p_dst_rect,
                                                    const vs_fixed_rotation_e *p_rotation);

/**
* @brief       Add a multi-blending task
* @details     Add a compose surface operation task
* @param [in]  job_handle, the handle of this job
* @param [in]  p_surf_list, compose surface list
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @par
*/
vs_int32_t vs_mal_tde_multi_blending(vs_int32_t job_handle, const vs_tde_surface_list_s *p_surf_list);

#ifdef __cplusplus
}
#endif

#endif /* __VS_MAL_TDE_H__ */
