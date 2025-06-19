/**
* @file vs_mal_avs.h
* @brief Declaraction of avs mal interface
* @details  Declaraction of avs mal interface
* @author visinextek
* @date 2022-08-27
* @version v0.1
* @Copyright (C) 2021, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/

#ifndef __VS_MAL_AVS_H__
#define __VS_MAL_AVS_H__

#include "vs_errno.h"
#include "vs_comm_video.h"
#include "vs_avs_defines.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
* @brief       init avs mouduls work mode.
* @details     init avs mouduls work mode.
* @param [in]  void.
* @return      0 call success.
* @return      none 0 call failed. see ERROR_CODE for detail.
* @par
*/
vs_int32_t vs_mal_avs_work_mode_init(vs_void_t);

/**
* @brief       Create a group by group id.
* @details     Create a group by group id.
* @param [in]  avs_grpid Indicate the group id to be given.
* @return      0 call success.
* @return      none 0 call failed. see ERROR_CODE for detail.
* @par
*/
vs_int32_t vs_mal_avs_grp_create(vs_int32_t avs_grpid, const vs_avs_grp_attr_s *p_grp_attr);

/**
* @brief       Start a group by group id.
* @details     Start a group by group id.
* @param [in]  avs_grpid Indicate the group id to be given.
* @return      0 call success.
* @return      none 0 call failed. see ERROR_CODE for detail.
* @par
*/
vs_int32_t vs_mal_avs_grp_start(vs_int32_t avs_grpid);

/**
* @brief       Get a group attributes by group id.
* @details     Get a group attributes by group id.
* @param [in]  avs_grpid Indicate the group id to be given.
* @param [out] p_grp_attr is the pointer of struct group attributes can not be NULL.
* @return      0 call success.
* @return      none 0 call failed. see ERROR_CODE for detail.
* @par
*/
vs_int32_t vs_mal_avs_grp_attr_get(vs_int32_t avs_grpid, vs_avs_grp_attr_s *p_grp_attr);

/**
* @brief       Set group attributes by group id and group attributes.
* @details     Set group attributes by group id and group attributes.
* @param [in]  avs_grpid Indicate the group id to be given.
* @param [in]  p_grp_attr the pointer of struct group attributes can not be NULL.
* @return      0 call success.
* @return      none 0 call failed. see ERROR_CODE for detail.
* @par
*/
vs_int32_t vs_mal_avs_grp_attr_set(vs_int32_t avs_grpid, const vs_avs_grp_attr_s *p_grp_attr);


/**
* @brief       Stop a group by group id.
* @details     Stop a group by group id.
* @param [in]  avs_grpid Indicate the group id to be given.
* @return      0 call success.
* @return      none 0 call failed. see ERROR_CODE for detail.
* @par
*/
vs_int32_t vs_mal_avs_grp_stop(vs_int32_t avs_grpid);

/**
* @brief       Destroy a group by group id.
* @details     Destroy a group by group id.
* @param [in]  avs_grpid Indicate the group id to be given.
* @return      0 call success.
* @return      none 0 call failed. see ERROR_CODE for detail.
* @par
*/
vs_int32_t vs_mal_avs_grp_destroy(vs_int32_t avs_grpid);

/**
* @brief       Send a frame to group by group id.
* @details     Send a frame to group by group id.
* @param [in]  avs_grpid and pipe_id Indicate the group id to be given.
* @param [in]  pipe_id Indicate the pipe id to be given.
* @param [in]  p_frame_info the pointer of struct video frame information can not be NULL.
* @param [in]  timeout_ms The timeout value unit of ms.
* @return      0 call success.
* @return      none 0 call failed. see ERROR_CODE for detail.
* @par
*/
vs_int32_t vs_mal_avs_grp_frame_send(vs_int32_t avs_grpid, vs_int32_t avs_pipeid,
                                               const vs_video_frame_info_s *p_frame_info,
                                               vs_int32_t timeout_ms);

/**
* @brief       Acquire a frame form group by group id.
* @details     Acquire a frame form group by group id.
* @param [in]  avs_grpid and pipe_id Indicate the group id to be given.
* @param [in]  avs_pipeid Indicate the pipe id to be given.
* @param [in]  p_frame_info the pointer of struct video frame information can not be NULL.
* @param [in]  timeout_ms The timeout value unit of ms.
* @return      0 call success.
* @return      none 0 call failed. see ERROR_CODE for detail.
* @par
*/
vs_int32_t vs_mal_avs_grp_frame_acquire(vs_int32_t avs_grpid, vs_int32_t avs_pipeid,
												vs_video_frame_info_s *p_frame_info,
												vs_int32_t timeout_ms);

/**
* @brief       Release a group frame by group id.
* @details     Release a group frame by group id.
* @param [in]  avs_grpid and pipe_id Indicate the group id to be given.
* @param [in]  avs_pipeid Indicate the pipe id to be given.
* @param [in]  p_frame_info the pointer of struct video frame information can not be NULL.
* @return      0 call success.
* @return      none 0 call failed. see ERROR_CODE for detail.
* @par
*/
vs_int32_t vs_mal_avs_grp_frame_release(vs_int32_t avs_grpid, vs_int32_t avs_pipeid,
												const vs_video_frame_info_s *p_frame_info);

/**
* @brief   	   Get a channel attributes by group id and channel id.
* @details 	   Get a channel attributes by group id and channel id.
* @param [in]  avs_grpid Indicate the group id to be given.
* @param [in]  avs_chnid Indicate the channel id to be given.
* @param [out] p_chn_attr the pointer of struct channel attributes can not be NULL.
* @return      0 call success.
* @warning     none 0 call failed. see ERROR_CODE for detail.
* @par
*/
vs_int32_t vs_mal_avs_chn_attr_get(vs_int32_t avs_grpid, vs_int32_t avs_chnid, vs_avs_chn_attr_s *p_chn_attr);

/**
* @brief       Set a channel attributes by group id channel id and channel attributes.
* @details     Set a channel attributes by group id channel id and channel attributes.
* @param [in]  avs_grpid Indicate the group id to be given.
* @param [in]  avs_chnid Indicate the channel id to be given.
* @param [in]  p_chn_attr the pointer of struct channel attributes can not be NULL.
* @return      0 call success.
* @return      none 0 call failed. see ERROR_CODE for detail.
* @par
*/
vs_int32_t vs_mal_avs_chn_attr_set(vs_int32_t avs_grpid, vs_int32_t avs_chnid, const vs_avs_chn_attr_s *p_chn_attr);

/**
* @brief      Enable a channel by group id and channel id.
* @details    Enable a channel by group id and channel id.
* @param [in] avs_grpid Indicate the group id to be given.
* @param [in] avs_chnid Indicate the channel id to be given.
* @return     0 call success.
* @warning    none 0 call failed. see ERROR_CODE for detail.
* @par
*/
vs_int32_t vs_mal_avs_chn_enable(vs_int32_t avs_grpid, vs_int32_t avs_chnid);

/**
* @brief      Disable a channel by group id and channel id.
* @details    Disable a channel by group id and channel id.
* @param [in] avs_grpid Indicate the group id to be given.
* @param [in] avs_chnid Indicate the channel id to be given.
* @return     0 call success.
* @warning    none 0 call failed. see ERROR_CODE for detail.
* @par
*/
vs_int32_t vs_mal_avs_chn_disable(vs_int32_t avs_grpid, vs_int32_t avs_chnid);

/**
* @brief       Acquire a frame from channel by group id and channel id.
* @details     Acquire a frame from channel by group id and channel id.
* @param [in]  avs_grpid Indicate the group id to be given.
* @param [in]  avs_chnid Indicate the channel id to be given.
* @param [out] p_frame_info the pointer of struct video frame information can not be NULL.
* @param [in]  timeout_ms The timeout value unit of ms.
* @return      0 call success.
* @warning     none 0 call failed. see ERROR_CODE for detail.
* @par
*/
vs_int32_t vs_mal_avs_chn_frame_acquire(vs_int32_t avs_grpid,
                                        vs_int32_t avs_chnid,
                                        vs_video_frame_info_s *p_frame_info,
                                        vs_int32_t timeout_ms);

/**
* @brief       Release a channel frame by group id and channel id.
* @details     Release a channel frame by group id and channel id.
* @param [in]  avs_grpid Indicate the group id to be given.
* @param [in]  avs_chnid Indicate the channel id to be given.
* @param [in]  p_frame_info  the pointer of struct video frame information can not be NULL.
* @return      0 call success.
* @warning     none 0 call failed. see ERROR_CODE for detail.
* @par
*/
vs_int32_t vs_mal_avs_chn_frame_release(vs_int32_t avs_grpid,
										vs_int32_t avs_chnid,
										const vs_video_frame_info_s *p_frame_info);

/**
* @brief	   Close channel file descriptor by group id and channel id.
* @details	   Close channel file descriptor by group id and channel id.
* @param [in]  avs_grpid Indicate the group id to be given.
* @param [in]  avs_chnid Indicate the channel id to be given.
* @return	   0 call success.
* @warning	   none 0 call failed. see ERROR_CODE for detail.
* @par
*/
vs_int32_t vs_mal_avs_chn_fd_close(vs_int32_t avs_grpid, vs_int32_t avs_chnid);

#ifdef __cplusplus
}
#endif

#endif /* __VS_MAL_GPE_H__ */
