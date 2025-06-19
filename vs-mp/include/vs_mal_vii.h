/**
* @file vs_mal_vii.h
* @brief Declaration of vii API.
* @details Declaration of vii API.
* @author Visinex software group
* @date 2022-01-27
* @version v0.01
* @copyright (C) 2022, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/

#ifndef __VS_MAL_VII_H__
#define __VS_MAL_VII_H__

/*****************************************************************************
 *                    Includes
 *****************************************************************************/
#include "vs_type.h"
#include "vs_comm.h"
#include "vs_comm_video.h"
#include "vs_errno.h"
#include "vs_vii_defines.h"

#ifdef __cplusplus
extern "C"{
#endif

/*****************************************************************************
 *                    Vii API Prototypes
 *****************************************************************************/

/**
* @brief  Set device attr by device id.
* @details Set device attr by device id.
* @param [in]  vii_devid Indicate the device id to be given.
* @param [in]  p_dev_attr The pointer of struct device attributes, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vii_dev_attr_set(vs_int32_t vii_devid, const vs_vii_dev_attr_s *p_dev_attr);

/**
* @brief  Get device attr by device id.
* @details Get device attr by device id.
* @param [in]  vii_devid Indicate the device id to be given.
* @param [out] p_dev_attr The pointer of struct device attributes, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vii_dev_attr_get(vs_int32_t vii_devid, vs_vii_dev_attr_s *p_dev_attr);

/**
* @brief  Enable the device by device id.
* @details Enable the device by device id.
* @param [in]  vii_devid Indicate the device id to be given.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vii_dev_enable(vs_int32_t vii_devid);

/**
* @brief  Disable the device by device id.
* @details Disable the device by device id.
* @param [in]  vii_devid Indicate the device id to be given.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vii_dev_disable(vs_int32_t vii_devid);

/**
* @brief  Bind dev and pipe by device id and pipe id.
* @details Bind dev and pipe by device id and pipe id.
* @param [in]  vii_devid Indicate the device id to be given.
* @param [in]  vii_pipeid Indicate the pipe id to be given.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vii_dev_pipe_bind(vs_int32_t vii_devid, vs_int32_t vii_pipeid);

/**
* @brief  Unbind dev and pipe by device id and pipe id.
* @details Unbind dev and pipe by device id and pipe id.
* @param [in]  vii_devid Indicate the device id to be given.
* @param [in]  vii_pipeid Indicate the pipe id to be given.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vii_dev_pipe_unbind(vs_int32_t vii_devid, vs_int32_t vii_pipeid);

/**
* @brief  Get pip bind info by device id.
* @details Get pip bind info by device id.
* @param [in]  vii_devid Indicate the device id to be given.
* @param [out] p_pipe_bind The pointer of struct pipe bind info, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vii_dev_pipe_bind_get_by_dev(vs_int32_t vii_devid, vs_vii_dev_pipe_bind_s *p_pipe_bind);

/**
* @brief  Get device bind id by pipe id.
* @details Get device bind id by pipe id.
* @param [in]  vii_pipeid Indicate the device id to be given.
* @param [out] vii_devid The pointer of device id, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vii_dev_pipe_bind_get_by_pipe(vs_int32_t vii_pipeid, vs_int32_t *vii_devid);

/**
* @brief  Set device vc id by dev id.
* @details Set device vc id by dev id.
* @param [in]  vii_devid Indicate the device id to be given.
* @param [in]  p_attr The pointer of vc attribution, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vii_dev_vc_set(vs_int32_t vii_devid, const vs_vii_vc_attr_s *p_attr);

/**
* @brief  Get device vc id by dev id.
* @details Get device vc id by dev id.
* @param [in]  vii_pipeid Indicate the device id to be given.
* @param [out]  p_attr The pointer of vc attribution, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vii_dev_vc_get(vs_int32_t vii_devid, vs_vii_vc_attr_s *p_attr);

/**
* @brief  Set vi and vpp online or offline attributes for all pipe.
* @details Set vi and vpp online or offline attributes for all pipe.
* @param [in]  p_online_offline_attr The pointer of each pipe about vi vpp online or offline attributes, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vii_vpp_mode_set(const vs_vii_vpp_online_offline_attr_s *p_online_offline_attr);

/**
* @brief  Get vi and vpp online or offline attributes for all pipe.
* @details Get vi and vpp online or offline attributes for all pipe.
* @param [out]  p_online_offline_attr The pointer of each pipe about vi vpp online or offline attributes, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vii_vpp_mode_get(vs_vii_vpp_online_offline_attr_s *p_online_offline_attr);

/**
* @brief  Create a pipe by pipe id.
* @details Create a pipe by pipe id.
* @param [in] vii_pipeid Indicate the pipe id to be given.
* @param [in] p_pipe_attr The pointer of struct pipe attributes, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vii_pipe_create(vs_int32_t vii_pipeid, const vs_vii_pipe_attr_s *p_pipe_attr);

/**
* @brief  Destroy a pipe by pipe id.
* @details Destroy a pipe by pipe id.
* @param [in] vii_pipeid Indicate the pipe id to be given.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vii_pipe_destroy(vs_int32_t vii_pipeid);

/**
* @brief  Set pipe attributes by pipe id.
* @details Set pipe attributes by pipe id.
* @param [in]  vii_pipeid Indicate the pipe id to be given.
* @param [in]  p_pipe_attr The pointer of struct pipe attributes, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vii_pipe_attr_set(vs_int32_t vii_pipeid, const vs_vii_pipe_attr_s *p_pipe_attr);

/**
* @brief  Get pipe attributes by pipe id.
* @details Get pipe attributes by pipe id.
* @param [in]  vii_pipeid Indicate the pipe id to be given.
* @param [out] p_pipe_attr The pointer of struct pipe attributes, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vii_pipe_attr_get(vs_int32_t vii_pipeid, vs_vii_pipe_attr_s *p_pipe_attr);

/**
* @brief  Set pipe optional parameter by pipe id.
* @details Set pipe optional parameter by pipe id.
* @param [in]  vii_pipeid Indicate the pipe id to be given.
* @param [in]  p_pipe_param The pointer of struct pipe parameter, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vii_pipe_param_set(vs_int32_t vii_pipeid, const vs_vii_pipe_param_s *p_pipe_param);

/**
* @brief  Get pipe optional parameter by pipe id.
* @details Get pipe optional parameter by pipe id.
* @param [in]  vii_pipeid Indicate the pipe id to be given.
* @param [out] p_pipe_param The pointer of struct pipe parameter, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vii_pipe_param_get(vs_int32_t vii_pipeid, vs_vii_pipe_param_s *p_pipe_param);

/**
* @brief  Set pipe fpn attributes by pipe id.
* @details Set pipe fpn attributes by pipe id.
* @param [in]  vii_pipeid Indicate the pipe id to be given.
* @param [in]  p_attr The pointer of struct pipe fpn attributes, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vii_pipe_fpn_attr_set(vs_int32_t vii_pipeid, const vs_vii_pipe_fpn_attr_s *p_attr);

/**
* @brief  Get pipe fpn attributes by pipe id.
* @details Get pipe fpn attributes by pipe id.
* @param [in]  vii_pipeid Indicate the pipe id to be given.
* @param [out]  p_attr The pointer of struct pipe fpn attributes, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vii_pipe_fpn_attr_get(vs_int32_t vii_pipeid, vs_vii_pipe_fpn_attr_s *p_attr);

/**
* @brief  Start the pipe by pipe id.
* @details Start the pipe by pipe id.
* @param [in]  vii_pipeid Indicate the pipe id to be given.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vii_pipe_start(vs_int32_t vii_pipeid);

/**
* @brief  Stop the pipe by pipe id.
* @details Stop the pipe by pipe id.
* @param [in]  vii_pipeid Indicate the pipe id to be given.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vii_pipe_stop(vs_int32_t vii_pipeid);

/**
* @brief  Set pipe input crop by pipe id.
* @details Set pipe input crop by pipe id.
* @param [in]  vii_pipeid Indicate the pipe id to be given.
* @param [in]  p_crop_info The pointer of struct pipe crop info, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vii_pipe_precrop_set(vs_int32_t vii_pipeid, const vs_crop_s *p_crop_info);

/**
* @brief  Get pipe input crop by pipe id.
* @details Get pipe input crop by pipe id.
* @param [in]  vii_pipeid Indicate the pipe id to be given.
* @param [out] p_crop_info The pointer of struct pipe crop info, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vii_pipe_precrop_get(vs_int32_t vii_pipeid, vs_crop_s *p_crop_info);

/**
* @brief  Set pipe vc attr by pipe id.
* @details Set pipe vc attr by pipe id.
* @param [in]  vii_pipeid Indicate the pipe id to be given.
* @param [in]  p_vc_attr The pointer of struct vc attr, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vii_pipe_vc_set(vs_int32_t vii_pipeid, const vs_vii_vc_attr_s *p_vc_attr);

/**
* @brief  Get pipe vc attr by pipe id.
* @details Get pipe vc attr by pipe id.
* @param [in]  vii_pipeid Indicate the pipe id to be given.
* @param [out] p_vc_attr The pointer of struct vc attr, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vii_pipe_vc_get(vs_int32_t vii_pipeid, vs_vii_vc_attr_s *p_vc_attr);

/**
* @brief  Set pipe dump frame attr by pipe id.
* @details Set pipe dump frame attr by pipe id.
* @param [in]  vii_pipeid Indicate the pipe id to be given.
* @param [in]  p_dump_frame_attr The pointer of struct pipe dump frame attr, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vii_pipe_dump_frame_attr_set(vs_int32_t vii_pipeid, const vs_vii_pipe_dump_frame_attr_s *p_dump_frame_attr);

/**
* @brief  Get pipe dump frame attr by pipe id.
* @details Get pipe dump frame attr by pipe id.
* @param [in]  vii_pipeid Indicate the pipe id to be given.
* @param [out] p_dump_frame_attr The pointer of struct pipe dump frame attr, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vii_pipe_dump_frame_attr_get(vs_int32_t vii_pipeid, vs_vii_pipe_dump_frame_attr_s *p_dump_frame_attr);

/**
* @brief  Acquire pipe frame by pipe id.
* @details Acquire pipe frame by pipe id.
* @param [in]  vii_pipeid Indicate the pipe id to be given.
* @param [out] p_frame The pointer of struct pipe frame info, can not be NULL.
* @param [out] p_frame_num Indicate the number to acquired frame.
* @param [in]  timeout_ms The timeout value unit of ms.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vii_pipe_frame_acquire(vs_int32_t vii_pipeid, vs_video_frame_info_s p_frame[VII_MAX_WDR_FRAME_NUM],
                                         vs_uint32_t *p_frame_num, vs_int32_t timeout_ms);

/**
* @brief  Release pipe frame by pipe id.
* @details Release pipe frame by pipe id.
* @param [in] vii_pipeid Indicate the pipe id to be given.
* @param [in] p_frame The pointer of struct pipe frame info, can not be NULL.
* @param [in] frame_num Indicate the number to release frame.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vii_pipe_frame_release(vs_int32_t vii_pipeid, const vs_video_frame_info_s p_frame[], vs_uint32_t frame_num);

/**
* @brief  Set pipe input source by pipe id.
* @details Set pipe input source by pipe id.
* @param [in] vii_pipeid Indicate the pipe id to be given.
* @param [in] src Indicate the pipe input frame source.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vii_pipe_src_set(vs_int32_t vii_pipeid, vs_vii_pipe_source_e src);

/**
* @brief  Get pipe input source by pipe id.
* @details Get pipe input source by pipe id.
* @param [in] vii_pipeid Indicate the pipe id to be given.
* @param [out] p_src The pointer of struct pipe frame source, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vii_pipe_src_get(vs_int32_t vii_pipeid, vs_vii_pipe_source_e *p_src);

/**
* @brief  Send user frame to pipe by pipe id.
* @details Send user frame to pipe by pipe id.
* @param [in] vii_pipeid Indicate the pipe id to be given.
* @param [in] p_frame The pointer of struct pipe frame info array, can not be NULL.
* @param [in] frame_num Indicate pipe frame num to be given.
* @param [in]  timeout_ms The timeout value unit of ms.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vii_pipe_frame_send(vs_int32_t vii_pipeid,
                                      const vs_video_frame_info_s *p_frame[], vs_uint32_t frame_num,
                                      vs_int32_t timeout_ms);

/**
* @brief  Get pipe status by pipe id.
* @details Get pipe status by pipe id.
* @param [in] vii_pipeid Indicate the pipe id to be given.
* @param [out] p_status The pointer of struct pipe status, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vii_pipe_status_query(vs_int32_t vii_pipeid, vs_vii_pipe_status_s *p_status);

/**
* @brief  Attach pipe to vbpool by pipe id and vbpool.
* @details Attach pipe to vbpool by pipe id and vbpool.
* @param [in]  vii_pipeid Indicate the pipe id to be given.
* @param [in]  vbpool Indicate the video buffer pool to be given.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vii_pipe_pool_attach(vs_int32_t vii_pipeid, vs_int32_t vbpool);

/**
* @brief  Detach pipe to vbpool by pipe id and vbpool.
* @details Detach pipe to vbpool by pipe id and vbpool.
* @param [in]  vii_pipeid Indicate the pipe id to be given.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vii_pipe_pool_detach(vs_int32_t vii_pipeid);

/**
* @brief  Get pipe file descriptor by pipe id.
* @details Get pipe file descriptor by pipe id.
* @param [in]  vii_pipeid Indicate the pipe id to be given.
* @return   positive call success indicate the file descriptor.
* @warning   negative or 0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vii_pipe_fd_get(vs_int32_t vii_pipeid);

/**
* @brief  Set a channel attributes by pipe id, channel id and channel attributes.
* @details Set a channel attributes by pipe id, channel id and channel attributes.
* @param [in]  vii_pipeid Indicate the pipe id to be given.
* @param [in]  vii_chnid Indicate the channel id to be given.
* @param [in]  p_chn_attr The pointer of struct channel attributes, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vii_chn_attr_set(vs_int32_t vii_pipeid, vs_int32_t vii_chnid, const vs_vii_chn_attr_s *p_chn_attr);

/**
* @brief  Get a channel attributes by pipe id, channel id and channel attributes.
* @details Get a channel attributes by pipe id, channel id and channel attributes.
* @param [in]  vii_pipeid Indicate the pipe id to be given.
* @param [in]  vii_chnid Indicate the channel id to be given.
* @param [out] p_chn_attr The pointer of struct channel attributes, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vii_chn_attr_get(vs_int32_t vii_pipeid, vs_int32_t vii_chnid, vs_vii_chn_attr_s *p_chn_attr);

/**
* @brief  Enable a channel by pipe id and channel id.
* @details Enable a channel by pipe id and channel id.
* @param [in]  vii_pipeid Indicate the pipe id to be given.
* @param [in]  vii_chnid Indicate the channel id to be given.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vii_chn_enable(vs_int32_t vii_pipeid, vs_int32_t vii_chnid);

/**
* @brief  Disable a channel by pipe id and channel id.
* @details Disable a channel by pipe id and channel id.
* @param [in]  vii_pipeid Indicate the pipe id to be given.
* @param [in]  vii_chnid Indicate the channel id to be given.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vii_chn_disable(vs_int32_t vii_pipeid, vs_int32_t vii_chnid);

/**
* @brief  Acquire a frame from channel by pipe id and channel id.
* @details Acquire a frame from channel by pipe id and channel id.
* @param [in]  vii_pipeid Indicate the pipe id to be given.
* @param [in]  vii_chnid Indicate the channel id to be given.
* @param [out]  p_frame_info The pointer of struct video frame information, can not be NULL.
* @param [in]  timeout_ms The timeout value unit of ms.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vii_chn_frame_acquire(vs_int32_t vii_pipeid,
                                        vs_int32_t vii_chnid,
                                        vs_video_frame_info_s *p_frame_info,
                                        vs_int32_t timeout_ms);

/**
* @brief  Release a channel frame by pipe id and channel id.
* @details Release a channel frame by pipe id and channel id.
* @param [in]  vii_pipeid Indicate the pipe id to be given.
* @param [in]  vii_chnid Indicate the channel id to be given.
* @param [in]  p_frame_info The pointer of struct video frame information, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vii_chn_frame_release(vs_int32_t vii_pipeid, vs_int32_t vii_chnid, const vs_video_frame_info_s *p_frame_info);


/**
* @brief  Set a extended channel attributes by pipe id, channel id and extended channel attributes.
* @details Set a extended channel attributes by pipe id, channel id and extended channel attributes.
* @param [in]  vii_pipeid Indicate the pipe id to be given.
* @param [in]  vii_chnid Indicate the channel id to be given.
* @param [in]  p_extchn_attr The pointer of struct extended channel attributes, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vii_extchn_attr_set(vs_int32_t vii_pipeid,
                                      vs_int32_t vii_chnid,
                                      const vs_vii_extchn_attr_s *p_extchn_attr);

/**
* @brief  Get a extended channel attributes by pipe id, channel id and extended channel attributes.
* @details Get a extended channel attributes by pipe id, channel id and extended channel attributes.
* @param [in]  vii_pipeid Indicate the pipe id to be given.
* @param [in]  vii_chnid Indicate the channel id to be given.
* @param [out] p_extchn_attr The pointer of struct extended channel attributes, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vii_extchn_attr_get(vs_int32_t vii_pipeid,
                                      vs_int32_t vii_chnid,
                                      vs_vii_extchn_attr_s *p_extchn_attr);

/**
* @brief  Set a extended channel fisheye attributes by pipe id, channel id and fisheye attributes.
* @details Set a extended channel fisheye attributes by pipe id, channel id and fisheye attributes.
* @param [in]  vii_pipeid Indicate the pipe id to be given.
* @param [in]  vii_chnid Indicate the channel id to be given.
* @param [in]  p_fisheye_attr The pointer of struct fisheye attributes, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vii_extchn_fisheye_set(vs_int32_t vii_pipeid,
                                         vs_int32_t vii_chnid,
                                         const vs_fisheye_attr_s *p_fisheye_attr);

/**
* @brief  Get a extended channel fisheye attributes by pipe id, channel id and fisheye attributes.
* @details Get a extended channel fisheye attributes by pipe id, channel id and fisheye attributes.
* @param [in]  vii_pipeid Indicate the pipe id to be given.
* @param [in]  vii_chnid Indicate the channel id to be given.
* @param [out] p_fisheye_attr The pointer of struct fisheye attributes, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vii_extchn_fisheye_get(vs_int32_t vii_pipeid,
                                         vs_int32_t vii_chnid,
                                         vs_fisheye_attr_s *p_fisheye_attr);

/**
* @brief  Get a luma data by pipe id, channel id, region information and timeout value.
* @details Get a luma data by pipe id, channel id, region information and timeout value.
* @param [in]  vii_pipeid Indicate the pipe id to be given.
* @param [in]  vii_chnid Indicate the channel id to be given.
* @param [in]  p_region_info The pointer of struct region information, can not be NULL.
* @param [out]  luma_data The pointer of struct luma data, can not be NULL.
* @param [in]  timeout_ms The timeout value unit of ms.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vii_chn_luma_get(vs_int32_t vii_pipeid,
                                   vs_int32_t vii_chnid,
                                   const vs_region_info_s *p_region_info,
                                   vs_uint64_t *luma_data,
                                   vs_int32_t timeout_ms);

/**
* @brief  Set channel crop information by pipe id, channel id and crop information.
* @details Set channel crop information by pipe id, channel id and crop information.
* @param [in]  vii_pipeid Indicate the pipe id to be given.
* @param [in]  vii_chnid Indicate the channel id to be given.
* @param [in]  p_crop_info The pointer of struct crop information, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vii_chn_crop_set(vs_int32_t vii_pipeid, vs_int32_t vii_chnid, const vs_vii_crop_info_s *p_crop_info);

/**
* @brief  Get channel crop information by pipe id and channel id.
* @details Get channel crop information by pipe id and channel id.
* @param [in]  vii_pipeid Indicate the pipe id to be given.
* @param [in]  vii_chnid Indicate the channel id to be given.
* @param [out]  p_crop_info The pointer of struct crop information, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vii_chn_crop_get(vs_int32_t vii_pipeid, vs_int32_t vii_chnid, vs_vii_crop_info_s *p_crop_info);

/**
* @brief  Set channel rotation attributes by pipe id, channel id and rotation attributes.
* @details Set channel rotation attributes by pipe id, channel id and rotation attributes.
* @param [in]  vii_pipeid Indicate the pipe id to be given.
* @param [in]  vii_chnid Indicate the channel id to be given.
* @param [in]  p_rotation_attr The pointer of struct rotation attributes, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vii_chn_rotation_set(vs_int32_t vii_pipeid, vs_int32_t vii_chnid, const vs_rotation_attr_s *p_rotation_attr);

/**
* @brief  Get channel rotation attributes by pipe id and channel id.
* @details Get channel rotation attributes by pipe id and channel id.
* @param [in]  vii_pipeid Indicate the pipe id to be given.
* @param [in]  vii_chnid Indicate the channel id to be given.
* @param [out]  p_rotation_attr The pointer of struct rotation attributes, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vii_chn_rotation_get(vs_int32_t vii_pipeid, vs_int32_t vii_chnid, vs_rotation_attr_s *p_rotation_attr);

/**
* @brief  Set channel ldc attributes by pipe id, channel id and ldc attributes.
* @details Set channel ldc attributes by pipe id, channel id and ldc attributes.
* @param [in]  vii_pipeid Indicate the pipe id to be given.
* @param [in]  vii_chnid Indicate the channel id to be given.
* @param [in]  p_ldc_attr The pointer of struct ldc attributes, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vii_chn_ldc_set(vs_int32_t vii_pipeid, vs_int32_t vii_chnid, const vs_ldc_attr_s *p_ldc_attr);

/**
* @brief  Get channel ldc attributes by pipe id and channel id.
* @details Get channel ldc attributes by pipe id and channel id.
* @param [in]  vii_pipeid Indicate the pipe id to be given.
* @param [in]  vii_chnid Indicate the channel id to be given.
* @param [out]  p_ldc_attr The pointer of struct ldc attributes, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vii_chn_ldc_get(vs_int32_t vii_pipeid, vs_int32_t vii_chnid, vs_ldc_attr_s *p_ldc_attr);

/**
* @brief  Set lowlatency information by pipe id and channel id.
* @details Set lowlatency information by pipe id and channel id.
* @param [in]  vii_pipeid Indicate the pipe id to be given.
* @param [in]  vii_chnid Indicate the channel id to be given.
* @param [in]  p_lowlatency_attr The pointer of struct lowlatency information, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vii_chn_lowlatency_set(vs_int32_t vii_pipeid,
                                         vs_int32_t vii_chnid,
                                         const vs_lowlatency_attr_s *p_lowlatency_attr);

/**
* @brief  Set lowlatency information by pipe id and channel id.
* @details Set lowlatency information by pipe id and channel id.
* @param [in]  vii_pipeid Indicate the pipe id to be given.
* @param [in]  vii_chnid Indicate the channel id to be given.
* @param [out]  p_lowlatency_attr The pointer of struct lowlatency information, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vii_chn_lowlatency_get(vs_int32_t vii_pipeid,
                                         vs_int32_t vii_chnid,
                                         vs_lowlatency_attr_s *p_lowlatency_attr);

/**
* @brief  Attach channel to vbpool by pipe id, channel id and vbpool.
* @details Attach channel to vbpool by pipe id, channel id and vbpool.
* @param [in]  vii_pipeid Indicate the pipe id to be given.
* @param [in]  vii_chnid Indicate the channel id to be given.
* @param [in]  vbpool Indicate the video buffer pool to be given.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vii_chn_pool_attach(vs_int32_t vii_pipeid, vs_int32_t vii_chnid, vs_int32_t vbpool);

/**
* @brief  Detach channel from vbpool by pipe id and channel id.
* @details Detach channel from vbpool by pipe id and channel id.
* @param [in]  vii_pipeid Indicate the pipe id to be given.
* @param [in]  vii_chnid Indicate the channel id to be given.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vii_chn_pool_detach(vs_int32_t vii_pipeid, vs_int32_t vii_chnid);

/**
* @brief  Get pipe status by pipe id and channel id.
* @details Get pipe status by pipe id and channel id.
* @param [in] vii_pipeid Indicate the pipe id to be given.
* @param [in]  vii_chnid Indicate the channel id to be given.
* @param [out] p_status The pointer of struct pipe status, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vii_chn_status_query(vs_int32_t vii_pipeid, vs_int32_t vii_chnid, vs_vii_chn_status_s *p_status);

/**
* @brief  Set channel align by pipe id and channel id.
* @details Set channel align by pipe id and channel id.
* @param [in]  vii_pipeid Indicate the pipe id to be given.
* @param [in]  vii_chnid Indicate the channel id to be given.
* @param [in]  align Indicate the channel align value to be given.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vii_chn_align_set(vs_int32_t vii_pipeid, vs_int32_t vii_chnid, vs_uint32_t align);

/**
* @brief  Get channel align by pipe id and channel id.
* @details Get channel align by pipe id and channel id.
* @param [in]  vii_pipeid Indicate the pipe id to be given.
* @param [in]  vii_chnid Indicate the channel id to be given.
* @param [out]  p_align The pointer of channel align, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vii_chn_align_get(vs_int32_t vii_pipeid, vs_int32_t vii_chnid, vs_uint32_t *p_align);

/**
* @brief  Get channel file descriptor by pipe id and channel id.
* @details Get channel file descriptor by pipe id and channel id.
* @param [in]  vii_pipeid Indicate the pipe id to be given.
* @param [in]  vii_chnid Indicate the channel id to be given.
* @return   positive call success indicate the file descriptor.
* @warning   negative or 0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vii_chn_fd_get(vs_int32_t vii_pipeid, vs_int32_t vii_chnid);

/**
* @brief  Close all file descriptor.
* @details Close all file descriptor.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_vii_fd_close(vs_void_t);

#ifdef __cplusplus
}
#endif

#endif
