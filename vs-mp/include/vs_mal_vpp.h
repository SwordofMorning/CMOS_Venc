/**
* @file vs_mal_vpp.h
* @brief Declaraction of vpp enumeration and struct
* @details Declaration of vpp enumeration and struct
* @author VS vpp group
* @date 2021-6-30
* @version v0.10
* @Copyright (C) 2021, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/


#ifndef __VS_MAL_VPP_H__
#define __VS_MAL_VPP_H__


#include "vs_comm.h"
#include "vs_comm_video.h"
#include "vs_mal_vbm.h"
#include "vs_errno.h"
#include "vs_vpp_defines.h"
#ifdef VPP_BE_ALWAYS_ONLINE
#include "vs_comm_algo.h"
#endif

#ifdef __cplusplus
extern "C"{
#endif


/*****************************************************************************************/

/**
* @brief  Create a group by group id.
* @details Create a group by group id.
* @param [in]  vpp_grpid Indicate the group id to be given.
* @param [in]  p_grp_attr The pointer of struct group attributes, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vpp_grp_create(vs_int32_t vpp_grpid, const vs_vpp_grp_attr_s *p_grp_attr);

/**
* @brief  Destroy a group by group id.
* @details Destroy a group by group id.
* @param [in]  vpp_grpid Indicate the group id to be given.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vpp_grp_destroy(vs_int32_t vpp_grpid);

/**
* @brief  Set group attributes by group id and group attributes.
* @details Set group attributes by group id and group attributes.
* @param [in]  vpp_grpid Indicate the group id to be given.
* @param [in]  p_grp_attr The pointer of struct group attributes, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vpp_grp_attr_set(vs_int32_t vpp_grpid, const vs_vpp_grp_attr_s *p_grp_attr);

/**
* @brief  Get a group attributes by group id.
* @details Get a group attributes by group id.
* @param [in]  vpp_grpid Indicate the group id to be given.
* @param [out]  p_grp_attr The pointer of struct group attributes, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vpp_grp_attr_get(vs_int32_t vpp_grpid, vs_vpp_grp_attr_s *p_grp_attr);

/**
* @brief  Start a group by group id.
* @details Start a group by group id.
* @param [in]  vpp_grpid Indicate the group id to be given.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vpp_grp_start(vs_int32_t vpp_grpid);

/**
* @brief  Stop a group by group id.
* @details Stop a group by group id.
* @param [in]  vpp_grpid Indicate the group id to be given.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vpp_grp_stop(vs_int32_t vpp_grpid);

/**
* @brief  Reset a group by group id.
* @details Reset a group by group id.
* @param [in]  vpp_grpid Indicate the group id to be given.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vpp_grp_reset(vs_int32_t vpp_grpid);

/**
* @brief  Set a channel attributes by group id, channel id and channel attributes.
* @details Set a channel attributes by group id, channel id and channel attributes.
* @param [in]  vpp_grpid Indicate the group id to be given.
* @param [in]  vpp_chnid Indicate the channel id to be given.
* @param [in]  p_chn_attr The pointer of struct channel attributes, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vpp_chn_attr_set(vs_int32_t vpp_grpid, vs_int32_t vpp_chnid, const vs_vpp_chn_attr_s *p_chn_attr);

/**
* @brief  Get a channel attributes by group id and channel id.
* @details Get a channel attributes by group id and channel id.
* @param [in]  vpp_grpid Indicate the group id to be given.
* @param [in]  vpp_chnid Indicate the channel id to be given.
* @param [out]  p_chn_attr The pointer of struct channel attributes, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vpp_chn_attr_get(vs_int32_t vpp_grpid, vs_int32_t vpp_chnid, vs_vpp_chn_attr_s *p_chn_attr);

/**
* @brief  Enable a channel by group id and channel id.
* @details Enable a channel by group id and channel id.
* @param [in]  vpp_grpid Indicate the group id to be given.
* @param [in]  vpp_chnid Indicate the channel id to be given.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vpp_chn_enable(vs_int32_t vpp_grpid, vs_int32_t vpp_chnid);

/**
* @brief  Disable a channel by group id and channel id.
* @details Disable a channel by group id and channel id.
* @param [in]  vpp_grpid Indicate the group id to be given.
* @param [in]  vpp_chnid Indicate the channel id to be given.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vpp_chn_disable(vs_int32_t vpp_grpid, vs_int32_t vpp_chnid);

/**
* @brief  Set group crop information by group id and crop information.
* @details Set group crop information by group id and crop information.
* @param [in]  vpp_grpid Indicate the group id to be given.
* @param [in]  p_crop_info The pointer of struct crop information, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vpp_grp_crop_set(vs_int32_t vpp_grpid, const vs_vpp_crop_info_s *p_crop_info);

/**
* @brief  Get group crop information by group id.
* @details Get group crop information by group id.
* @param [in]  vpp_grpid Indicate the group id to be given.
* @param [out]  p_crop_info The pointer of struct crop information, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vpp_grp_crop_get(vs_int32_t vpp_grpid, vs_vpp_crop_info_s *p_crop_info);

/**
* @brief  Send a frame to group by group id.
* @details Send a frame to group by group id.
* @param [in]  vpp_grpid Indicate the group id to be given.
* @param [in]  p_frame_info The pointer of struct video frame information, can not be NULL.
* @param [in]  timeout_ms The timeout value unit of ms.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vpp_grp_frame_send(vs_int32_t vpp_grpid,
                                               const vs_video_frame_info_s *p_frame_info,
                                               vs_int32_t timeout_ms);

/**
* @brief  Acquire a frame from channel by group id and channel id.
* @details Acquire a frame from channel by group id and channel id.
* @param [in]  vpp_grpid Indicate the group id to be given.
* @param [in]  vpp_chnid Indicate the channel id to be given.
* @param [out]  p_frame_info The pointer of struct video frame information, can not be NULL.
* @param [in]  timeout_ms The timeout value unit of ms.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vpp_chn_frame_acquire(vs_int32_t vpp_grpid,
                                                    vs_int32_t vpp_chnid,
                                                    vs_video_frame_info_s *p_frame_info,
                                                    vs_int32_t timeout_ms);

/**
* @brief  Release a channel frame by group id and channel id.
* @details Release a channel frame by group id and channel id.
* @param [in]  vpp_grpid Indicate the group id to be given.
* @param [in]  vpp_chnid Indicate the channel id to be given.
* @param [in]  p_frame_info The pointer of struct video frame information, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vpp_chn_frame_release(vs_int32_t vpp_grpid, vs_int32_t vpp_chnid, const vs_video_frame_info_s *p_frame_info);

/**
* @brief  Acquire a frame form group by group id.
* @details Acquire a frame form group by group i.
* @param [in]  vpp_grpid Indicate the group id to be given.
* @param [out]  p_frame_info The pointer of struct video frame information, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vpp_grp_frame_acquire(vs_int32_t vpp_grpid, vs_video_frame_info_s *p_frame_info);

/**
* @brief  Release a group frame by group id.
* @details Release a group frame by group id.
* @param [in]  vpp_grpid Indicate the group id to be given.
* @param [in]  p_frame_info The pointer of struct video frame information, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vpp_grp_frame_release(vs_int32_t vpp_grpid, const vs_video_frame_info_s *p_frame_info);

/**
* @brief  Set a extended channel attributes by group id, channel id and extended channel attributes.
* @details Set a extended channel attributes by group id, channel id and extended channel attributes.
* @param [in]  vpp_grpid Indicate the group id to be given.
* @param [in]  vpp_chnid Indicate the channel id to be given.
* @param [in]  p_extchn_attr The pointer of struct extended channel attributes, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vpp_extchn_attr_set(vs_int32_t vpp_grpid,
                                                 vs_int32_t vpp_chnid,
                                                 const vs_vpp_extchn_attr_s *p_extchn_attr);

/**
* @brief  Get a extended channel attributes by group id and channel id.
* @details Get a extended channel attributes by group id and channel id.
* @param [in]  vpp_grpid Indicate the group id to be given.
* @param [in]  vpp_chnid Indicate the channel id to be given.
* @param [out]  p_extchn_attr The pointer of struct extended channel attributes, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vpp_extchn_attr_get(vs_int32_t vpp_grpid,
                                                 vs_int32_t vpp_chnid,
                                                 vs_vpp_extchn_attr_s *p_extchn_attr);

/**
* @brief  Set a extended channel fisheye attributes by group id, channel id and fisheye attributes.
* @details Set a extended channel fisheye attributes by group id, channel id and fisheye attributes.
* @param [in]  vpp_grpid Indicate the group id to be given.
* @param [in]  vpp_chnid Indicate the channel id to be given.
* @param [in]  p_fisheye_attr The pointer of struct fisheye attributes, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vpp_extchn_fisheye_set(vs_int32_t vpp_grpid,
                                                     vs_int32_t vpp_chnid,
                                                     const vs_fisheye_attr_s *p_fisheye_attr);

/**
* @brief  Get a extended channel fisheye attributes by group id and channel id.
* @details Get a extended channel fisheye attributes by group id and channel id.
* @param [in]  vpp_grpid Indicate the group id to be given.
* @param [in]  vpp_chnid Indicate the channel id to be given.
* @param [out]  p_fisheye_attr The pointer of struct fisheye attributes, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vpp_extchn_fisheye_get(vs_int32_t vpp_grpid,
                                                     vs_int32_t vpp_chnid,
                                                     vs_fisheye_attr_s *p_fisheye_attr);

/**
* @brief  Get a luma data by group id, channel id, region information and timeout value.
* @details Get a luma data by group id, channel id, region information and timeout value.
* @param [in]  vpp_grpid Indicate the group id to be given.
* @param [in]  vpp_chnid Indicate the channel id to be given.
* @param [in]  p_region_info The pointer of struct region information, can not be NULL.
* @param [out]  luma_data The pointer of struct luma data, can not be NULL.
* @param [in]  timeout_ms The timeout value unit of ms.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vpp_chn_luma_get(vs_int32_t vpp_grpid,
                                            vs_int32_t vpp_chnid,
                                            const vs_region_info_s *p_region_info,
                                            vs_uint64_t *luma_data,
                                            vs_int32_t timeout_ms);

/**
* @brief  Set channel crop information by group id, channel id and crop information.
* @details Set channel crop information by group id, channel id and crop information.
* @param [in]  vpp_grpid Indicate the group id to be given.
* @param [in]  vpp_chnid Indicate the channel id to be given.
* @param [in]  p_crop_info The pointer of struct crop information, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vpp_chn_crop_set(vs_int32_t vpp_grpid, vs_int32_t vpp_chnid, const vs_vpp_crop_info_s *p_crop_info);

/**
* @brief  Get channel crop information by group id and channel id.
* @details Get channel crop information by group id and channel id.
* @param [in]  vpp_grpid Indicate the group id to be given.
* @param [in]  vpp_chnid Indicate the channel id to be given.
* @param [out]  p_crop_info The pointer of struct crop information, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vpp_chn_crop_get(vs_int32_t vpp_grpid, vs_int32_t vpp_chnid, vs_vpp_crop_info_s *p_crop_info);

/**
* @brief  Set channel rotation attributes by group id, channel id and rotation attributes.
* @details Set channel rotation attributes by group id, channel id and rotation attributes.
* @param [in]  vpp_grpid Indicate the group id to be given.
* @param [in]  vpp_chnid Indicate the channel id to be given.
* @param [in]  p_rotation_attr The pointer of struct rotation attributes, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vpp_chn_rotation_set(vs_int32_t vpp_grpid, vs_int32_t vpp_chnid, const vs_rotation_attr_s *p_rotation_attr);

/**
* @brief  Get channel rotation attributes by group id and channel id.
* @details Get channel rotation attributes by group id and channel id.
* @param [in]  vpp_grpid Indicate the group id to be given.
* @param [in]  vpp_chnid Indicate the channel id to be given.
* @param [out]  p_rotation_attr The pointer of struct rotation attributes, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vpp_chn_rotation_get(vs_int32_t vpp_grpid, vs_int32_t vpp_chnid, vs_rotation_attr_s *p_rotation_attr);

/**
* @brief  Set channel ldc attributes by group id, channel id and ldc attributes.
* @details Set channel ldc attributes by group id, channel id and ldc attributes.
* @param [in]  vpp_grpid Indicate the group id to be given.
* @param [in]  vpp_chnid Indicate the channel id to be given.
* @param [in]  p_ldc_attr The pointer of struct ldc attributes, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vpp_chn_ldc_set(vs_int32_t vpp_grpid, vs_int32_t vpp_chnid, const vs_ldc_attr_s *p_ldc_attr);

/**
* @brief  Get channel ldc attributes by group id and channel id.
* @details Get channel ldc attributes by group id and channel id.
* @param [in]  vpp_grpid Indicate the group id to be given.
* @param [in]  vpp_chnid Indicate the channel id to be given.
* @param [out]  p_ldc_attr The pointer of struct ldc attributes, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vpp_chn_ldc_get(vs_int32_t vpp_grpid, vs_int32_t vpp_chnid, vs_ldc_attr_s *p_ldc_attr);

/**
* @brief  Set lowlatency information by group id and channel id.
* @details Set lowlatency information by group id and channel id.
* @param [in]  vpp_grpid Indicate the group id to be given.
* @param [in]  vpp_chnid Only support channel 0.
* @param [in]  p_lowlatency_info The pointer of struct lowlatency information, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vpp_chn_lowlatency_set(vs_int32_t vpp_grpid,
                                             vs_int32_t vpp_chnid,
                                             const vs_lowlatency_attr_s *p_lowlatency_attr);

/**
* @brief  Set lowlatency information by group id and channel id.
* @details Set lowlatency information by group id and channel id.
* @param [in]  vpp_grpid Indicate the group id to be given.
* @param [in]  vpp_chnid Only support channel 0.
* @param [out]  p_lowlatency_info The pointer of struct lowlatency information, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vpp_chn_lowlatency_get(vs_int32_t vpp_grpid,
                                             vs_int32_t vpp_chnid,
                                             vs_lowlatency_attr_s *p_lowlatency_attr);

/**
* @brief  Set length of group delay queue by group id.
* @details Set length of group delay queue by group id.
* @param [in]  vpp_grpid Indicate the group id to be given.
* @param [in]  delay Indicate the length of lowdelay queue to be given.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vpp_grp_delay_set(vs_int32_t vpp_grpid, vs_uint32_t delay);

/**
* @brief  Get length of group delay queue by group id.
* @details Get length of group delay queue by group id.
* @param [in]  vpp_grpid Indicate the group id to be given.
* @param [out]  p_delay The pointer of queue length, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vpp_grp_delay_get(vs_int32_t vpp_grpid, vs_uint32_t *p_delay);

/**
* @brief  Enable user framerate control by group id.
* @details Enable user framerate control by group id.
* @param [in]  vpp_grpid Indicate the group id to be given.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vpp_user_frc_enable(vs_int32_t vpp_grpid);

/**
* @brief  Disable user framerate control by group id.
* @details Disable user framerate control by group id.
* @param [in]  vpp_grpid Indicate the group id to be given.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vpp_user_frc_disable(vs_int32_t vpp_grpid);

/**
* @brief  Set channel align by group id and channel id.
* @details Set channel align by group id and channel id.
* @param [in]  vpp_grpid Indicate the group id to be given.
* @param [in]  vpp_chnid Indicate the channel id to be given.
* @param [in]  align Indicate the channel align value to be given.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vpp_chn_align_set(vs_int32_t vpp_grpid, vs_int32_t vpp_chnid, vs_uint32_t align);

/**
* @brief  Get channel align by group id and channel id.
* @details Get channel align by group id and channel id.
* @param [in]  vpp_grpid Indicate the group id to be given.
* @param [in]  vpp_chnid Indicate the channel id to be given.
* @param [out]  p_align The pointer of channel align, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vpp_chn_align_get(vs_int32_t vpp_grpid, vs_int32_t vpp_chnid, vs_uint32_t *p_align);

/**
* @brief  Get channel file descriptor by group id and channel id.
* @details Get channel file descriptor by group id and channel id.
* @param [in]  vpp_grpid Indicate the group id to be given.
* @param [in]  vpp_chnid Indicate the channel id to be given.
* @return   positive call success indicate the file descriptor.
* @warning   negative or 0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vpp_chn_fd_get(vs_int32_t vpp_grpid, vs_int32_t vpp_chnid);

/**
* @brief  Close channel file descriptor by group id and channel id.
* @details Close channel file descriptor by group id and channel id.
* @param [in]  vpp_grpid Indicate the group id to be given.
* @param [in]  vpp_chnid Indicate the channel id to be given.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vpp_chn_fd_close(vs_int32_t vpp_grpid, vs_int32_t vpp_chnid);

/**
* @brief  Attach channel to vbpool by group id, channel id and vbpool.
* @details Attach channel to vbpool by group id, channel id and vbpool.
* @param [in]  vpp_grpid Indicate the group id to be given.
* @param [in]  vpp_chnid Indicate the channel id to be given.
* @param [in]  vbpool Indicate the video buffer pool to be given.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vpp_chn_pool_attach(vs_int32_t vpp_grpid, vs_int32_t vpp_chnid, VB_POOL vbpool);

/**
* @brief  Detach channel from vbpool by group id and channel id.
* @details Detach channel from vbpool by group id and channel id.
* @param [in]  vpp_grpid Indicate the group id to be given.
* @param [in]  vpp_chnid Indicate the channel id to be given.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vpp_chn_pool_detach(vs_int32_t vpp_grpid, vs_int32_t vpp_chnid);

/**
* @brief  Set a stitch attributes by stitch id and stitch attributes.
* @details Set a stitch attributes by stitch id and stitch attributes.
* @param [in]  vpp_stitchid Indicate the stitch id to be given.
* @param [in]  p_stitch_attr The pointer of struct stitch attributes, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vpp_grp_stitch_set(vs_int32_t vpp_stitchid, const vs_vpp_grp_stitch_attr_s* p_stitch_attr);

/**
* @brief  Get a stitch attributes by stitch id.
* @details Get a stitch attributes by stitch id.
* @param [in]  vpp_stitchid Indicate the stitch id to be given.
* @param [out]  p_stitch_attr The pointer of struct stitch attributes, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vpp_grp_stitch_get(vs_int32_t vpp_stitchid, vs_vpp_grp_stitch_attr_s* p_stitch_attr);

#ifdef __cplusplus
}
#endif
#endif /*!<  __VS_MAL_VPP_H__ */
