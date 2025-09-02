/**
* @file vs_mal_rgn.h
* @brief This file implements rgn API.
* @details
* @author visinextek
* @date 2021-12-30
* @version v0.1
* @Copyright (C) 2021, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/

#ifndef __VS_MAL_RGN_H__
#define __VS_MAL_RGN_H__

#include "vs_type.h"
#include "vs_errno.h"
#include "vs_comm.h"
#include "vs_comm_video.h"
#include "vs_rgn_defines.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
* @brief       Create a region.
* @details     Create region by using a handle id, and set the region type.
* @param [in]  handle, must set a handle which is available.
* @param [in]  p_attr, must set a region type, such as cover, overlay or mosaic.
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @par
*/
vs_int32_t vs_mal_rgn_create(vs_uint32_t handle, const vs_rgn_attr_s *p_attr);

/**
* @brief       Destroy a region.
* @details     Create region by using a handle id, and free the memory or resource it used.
* @param [in]  handle, must set a handle of region which had been created
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @par
*/
vs_int32_t vs_mal_rgn_destroy(vs_uint32_t handle);

/**
* @brief       Get the region attributes.
* @details     Get the region attribute, including size or format of overlay region.
* @param [in]  handle, must set a handle of region which had been created
* @param [out] p_attr, The pointer of overlay attribute
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @par
*/
vs_int32_t vs_mal_rgn_attr_get(vs_uint32_t handle, vs_rgn_attr_s *p_attr);

/**
* @brief       Set the region public attributes.
* @details     Set the region public attributes when the region type is overlay.
* @param [in]  handle, must set a handle of region which had been created
* @param [in]  p_attr, The pointer of overlay attribute
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @par
*/
vs_int32_t vs_mal_rgn_attr_set(vs_uint32_t handle, const vs_rgn_attr_s *p_attr);

/**
* @brief       Set a bitmap for a overlay region.
* @details     Set a bitmap for a overlay region, can't set to cover or mosaic.
* @param [in]  handle, must set a handle of region which had been created.
* @param [in]  p_bitmap, bitmap object that store the picture content.
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @par
*/
vs_int32_t vs_mal_rgn_bitmap_set(vs_uint32_t handle, const vs_bitmap_s *p_bitmap);

/**
* @brief       Bind the region to a video channel.
* @details     Bind the region to a video channel, prepare to display the region.
* @param [in]  handle, must set a handle of region which had been created.
* @param [in]  p_chn, the video channel information
* @param [in]  p_disp_info, the region display information on video channel
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @par
*/
vs_int32_t vs_mal_rgn_chn_bind(vs_uint32_t handle, const vs_chn_s *p_chn, const vs_rgn_disp_info_s *p_disp_info);

/**
* @brief       Unbind the region from a video channel.
* @details     Bind the region from a video channel, then will be set bitmap again or update, or destroy.
* @param [in]  handle, must set a handle of region which had been created.
* @param [in]  p_chn, the video channel information
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @par
*/
vs_int32_t vs_mal_rgn_chn_unbind(vs_uint32_t handle, const vs_chn_s *p_chn);

/**
* @brief       Set the region display info
* @details     Set the region display information on the video channel which the region be bound to
* @param [in]  handle, must set a handle which is available.
* @param [in]  p_disp_info, the relative display information
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @par
*/
vs_int32_t vs_mal_rgn_chn_disp_set(vs_uint32_t handle, const vs_chn_s *p_chn, const vs_rgn_disp_info_s *p_disp_info);

/**
* @brief       Get the region display information
* @details     Get the region display information on the video channel which the region be bound to
* @param [in]  handle, must set a handle which is available.
* @param [out] p_disp_info, the relative display information
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @par
*/
vs_int32_t vs_mal_rgn_chn_disp_get(vs_uint32_t handle, const vs_chn_s *p_chn, vs_rgn_disp_info_s *p_disp_info);

/**
* @brief       Get the surface of overlay region.
* @details     Can only get the surface information for overlay region.
* @param [in]  handle, must set a handle which is available.
* @param [out] p_surface, the surface object of overlay region.
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @par
*/
vs_int32_t vs_mal_rgn_surface_get(vs_uint32_t handle, vs_rgn_surface_info_s *p_surface);

/**
* @brief       Update the surface of overlay region.
* @details     Action of update the overlay region surface will swap the buffer
* @param [in]  handle, must set a handle which is available.
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @par
*/
vs_int32_t vs_mal_rgn_surface_update(vs_uint32_t handle);

/**
* @brief       Set a batch flag.
* @details     Set a batch, the following bitmap set or surface update will be commit to all region after batch end
* @param [out] p_batchid, a batch group id.
* @param [in]  handle_num, the number for handles in group
* @param [in]  p_handle_array, handles array
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @par
*/
vs_int32_t vs_mal_rgn_batch_begin(vs_uint32_t *p_batchid, vs_uint32_t handle_num, const vs_uint32_t *p_handle_array);

/**
* @brief       End batch and commit.
* @details     End batch and commit all bitmap or surface update actions.
* @param [in]  p_batchid, a batch group id.
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @par
*/
vs_int32_t vs_mal_rgn_batch_end(vs_uint32_t batchid);

#ifdef __cplusplus
}
#endif

#endif /* __VS_MAL_RGN_H__ */
