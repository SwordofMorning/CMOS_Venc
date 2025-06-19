/**
* @file vs_mal_vdec.h
* @brief This file declare the API of VDEC.
* @details This file declare the API of VDEC.
* @author VS software group
* @date 2021-4-14
* @version v1.01
* @copyright (C) 2021, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/

#ifndef __VS_MAL_VDEC_H__
#define __VS_MAL_VDEC_H__

#include "vs_comm.h"
#include "vs_comm_video.h"
#include "vs_vdec_defines.h"
#include "vs_errno.h"
#include "vs_mal_vbm.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
* @brief Create a decoding channel by channel number and attributes.
* @details
* @param [in] vdec_chnid Channel number. Range: [0, MAX_VDEC_CHN_NUM).
* @param [in] p_chn_attr Pointer to the decoding channel attribute structure.
* @retval VS_SUCCESS Success.
* @retval VS_ERR_VDEC_INVALID_CHNID Channel number exceeds the maximum supported.
* @retval VS_ERR_VDEC_NULL_PTR Channel attribute pointer is NULL.
* @retval VS_ERR_VDEC_EXIST Channel has been created.
* @retval VS_ERR_VDEC_INVALID_PARAM --- dec_type is not H.264/H.265/JPEG/MJPEG.\n
* --- dec_type is JPEG/MJPEG but stream mode is E_VDEC_STREAM_MODE_STREAM.\n
* --- frame_width or frame_height exceeded the size of the module parameter.\n
* --- ref_frame_num or max_userdata_len out of range.\n
* --- crop_rect is illegal when crop is enabled.
* @note
* - JPEG/MJPEG decoding does not support stream sending mode.
* - frame_buf_size and frame_buf_cnt only valid when the VB source is private mode.
* - ref_frame_num setting wrong will cause decoding failure, please set it carefully.
*/
vs_int32_t vs_mal_vdec_chn_create(vs_int32_t vdec_chnid, const vs_vdec_chn_attr_s *p_chn_attr);

/**
* @brief Destroy a decoding channel by channel number.
* @details
* @param [in] vdec_chnid Channel number. Range: [0, MAX_VDEC_CHN_NUM).
* @retval VS_SUCCESS Success.
* @retval VS_ERR_VDEC_INVALID_CHNID Channel number exceeds the maximum supported.
* @retval VS_ERR_VDEC_UNEXIST Channel is not created.
* @retval VS_ERR_VDEC_NOT_PERM Channel is not stopped.
* @note
* - You must stopping the channel before destroy the channel.
*/
vs_int32_t vs_mal_vdec_chn_destroy(vs_int32_t vdec_chnid);

/**
* @brief Set channel attributes by channel number.
* @details
* @param [in] vdec_chnid Channel number. Range: [0, MAX_VDEC_CHN_NUM).
* @param [in] p_chn_attr Pointer to the decoding channel attribute structure.
* @retval VS_SUCCESS Success.
* @retval VS_ERR_VDEC_INVALID_CHNID Channel number exceeds the maximum supported.
* @retval VS_ERR_VDEC_NULL_PTR Channel attribute pointer is NULL.
* @retval VS_ERR_VDEC_UNEXIST Channel is not created.
* @retval VS_ERR_VDEC_NOT_PERM Channel is not stopped.
* @retval VS_ERR_VDEC_INVALID_PARAM --- In addition to dec_type/stream_mode/ref_frame_num,
* other different attributes are setting.\n
* --- dec_type is JPEG/MJPEG but stream mode is E_VDEC_STREAM_MODE_STREAM.\n
* --- ref_frame_num out of range.
* @note
* - Only dec_type, stream_mode, ref_frame_num can be changed.
* - Changing above three attributes will cause the channel to reset, and must be calling after channel stopped.
* - Changing dec_type will cause the channel parameters to restore the default values.
*/
vs_int32_t vs_mal_vdec_chn_attr_set(vs_int32_t vdec_chnid, const vs_vdec_chn_attr_s *p_chn_attr);

/**
* @brief Get channel attributes by channel number.
* @details
* @param [in] vdec_chnid Channel number. Range: [0, MAX_VDEC_CHN_NUM).
* @param [out] p_chn_attr Pointer to the decoding channel attribute structure.
* @retval VS_SUCCESS Success.
* @retval VS_ERR_VDEC_INVALID_CHNID Channel number exceeds the maximum supported.
* @retval VS_ERR_VDEC_NULL_PTR Channel attribute pointer is NULL.
* @retval VS_ERR_VDEC_UNEXIST Channel is not created.
* @note
*/
vs_int32_t vs_mal_vdec_chn_attr_get(vs_int32_t vdec_chnid, vs_vdec_chn_attr_s *p_chn_attr);

/**
* @brief Notify the decoding channel to start receiving stream.
* @details
* @param [in] vdec_chnid Channel number. Range: [0, MAX_VDEC_CHN_NUM).
* @retval VS_SUCCESS Success.
* @retval VS_ERR_VDEC_INVALID_CHNID Channel number exceeds the maximum supported.
* @retval VS_ERR_VDEC_UNEXIST Channel is not created.
* @note
* - vs_mal_vdec_stream_send will be sent successfully(if stream buffer has enough space) after calling this function.
*/
vs_int32_t vs_mal_vdec_chn_start(vs_int32_t vdec_chnid);

/**
* @brief Notify the decoding channel to stop receiving stream.
* @details
* @param [in] vdec_chnid Channel number. Range: [0, MAX_VDEC_CHN_NUM).
* @retval VS_SUCCESS Success.
* @retval VS_ERR_VDEC_INVALID_CHNID Channel number exceeds the maximum supported.
* @retval VS_ERR_VDEC_UNEXIST Channel is not created.
* @note
* - vs_mal_vdec_stream_send will be sent failed after calling this function.
*/
vs_int32_t vs_mal_vdec_chn_stop(vs_int32_t vdec_chnid);

/**
* @brief Get channel status by channel number.
* @details
* @param [in] vdec_chnid Channel number. Range: [0, MAX_VDEC_CHN_NUM).
* @param [out] Pointer to the decoding channel status structure.
* @retval VS_SUCCESS Success.
* @retval VS_ERR_VDEC_INVALID_CHNID Channel number exceeds the maximum supported.
* @retval VS_ERR_VDEC_NULL_PTR Channel status pointer is NULL.
* @retval VS_ERR_VDEC_UNEXIST Channel is not created.
* @note
* - frame_width and frame_height will be assigned after the start of decoding.
* - Each error in dec_errors is cumulative, resetting the channel will cause them to reset.
*/
vs_int32_t vs_mal_vdec_chn_status_get(vs_int32_t vdec_chnid, vs_vdec_chn_status_s *p_chn_status);

/**
* @brief Reset the channel by channel number.
* @details This function will clear the stream buffer and output buffer, but will not reset the channel parameters.
* @param [in] vdec_chnid Channel number. Range: [0, MAX_VDEC_CHN_NUM).
* @retval VS_SUCCESS Success.
* @retval VS_ERR_VDEC_INVALID_CHNID Channel number exceeds the maximum supported.
* @retval VS_ERR_VDEC_UNEXIST Channel is not created.
* @retval VS_ERR_VDEC_NOT_PERM Channel is not stopped.
* @note
* - You must stopping the channel before resetting the channel.
*/
vs_int32_t vs_mal_vdec_chn_reset(vs_int32_t vdec_chnid);

/**
* @brief Get the device fd of the VDEC channel.
* @details
* @param [in] vdec_chnid Channel number. Range: [0, MAX_VDEC_CHN_NUM).
* @retval VS_ERR_VDEC_INVALID_CHNID Channel number exceeds the maximum supported.
* @retval <=0 Invalid return value.
* @retval other Valid return value.
* @note
*/
vs_int32_t vs_mal_vdec_chn_fd_get(vs_int32_t vdec_chnid);

/**
* @brief Close the device fd of the VDEC channel.
* @details
* @param [in] vdec_chnid Channel number. Range: [0, MAX_VDEC_CHN_NUM).
* @retval VS_SUCCESS Closed successful.
* @retval VS_ERR_VDEC_INVALID_CHNID Channel number exceeds the maximum supported.
* @note
*/
vs_int32_t vs_mal_vdec_chn_fd_close(vs_int32_t vdec_chnid);

/**
* @brief Set channel parameters by channel number.
* @details
* @param [in] vdec_chnid Channel number. Range: [0, MAX_VDEC_CHN_NUM).
* @param [in] p_chn_param Pointer to the decoding channel parameters structure.
* @retval VS_SUCCESS Success.
* @retval VS_ERR_VDEC_INVALID_CHNID Channel number exceeds the maximum supported.
* @retval VS_ERR_VDEC_NULL_PTR Channel parameters pointer is NULL.
* @retval VS_ERR_VDEC_UNEXIST Channel is not created.
* @retval VS_ERR_VDEC_NOT_PERM Channel is not stopped.
* @retval VS_ERR_VDEC_INVALID_PARAM --- dec_type is not equal to channel attributes.\n
* --- dec_type is JPEG/MJPEG but stream mode is E_VDEC_STREAM_MODE_STREAM.\n
* --- output_frame_num or err_handling_threshold out of range.\n
* --- format_mode is not E_VIDEO_FORMAT_LINEAR and E_VIDEO_FORMAT_TILE_4x4.\n
* --- pixel_mode is not E_PIXEL_FORMAT_YUV_420SP, E_PIXEL_FORMAT_YVU_420SP and E_PIXEL_FORMAT_YUV_400.
* @note
* - VDEC will set the default channel parameters after channel created.
* - The parameters passed in when calling this function should be obtained by calling vs_mal_vdec_chn_param_get.
* - Changing output_frame_num, video_format, pixel_format(NV12/NV21->YUV400 or YUV400->NV12/NV21), output_order\n
* or compress_mode will cause the channel to reset, and must be calling after channel stopped.
* - Enable compression will force to use 256-byte alignment, user need to ensure that the size of the
* VB pool meets the needs of the frame buffer.
*/
vs_int32_t vs_mal_vdec_chn_param_set(vs_int32_t vdec_chnid, const vs_vdec_chn_param_s *p_chn_param);

/**
* @brief Get channel parameters by channel number.
* @details
* @param [in] vdec_chnid Channel number. Range: [0, MAX_VDEC_CHN_NUM).
* @param [out] p_chn_param Pointer to the decoding channel parameters structure.
* @retval VS_SUCCESS Success.
* @retval VS_ERR_VDEC_INVALID_CHNID Channel number exceeds the maximum supported.
* @retval VS_ERR_VDEC_NULL_PTR Channel parameters pointer is NULL.
* @retval VS_ERR_VDEC_UNEXIST Channel is not created.
* @note
*/
vs_int32_t vs_mal_vdec_chn_param_get(vs_int32_t vdec_chnid, vs_vdec_chn_param_s *p_chn_param);

/**
* @brief Set H.264/H.265 protocol parameters by channel number.
* @details
* @param [in] vdec_chnid Channel number. Range: [0, MAX_VDEC_CHN_NUM).
* @param [in] p_chn_param Pointer to the H.264/H.265 protocol parameters structure.
* @retval VS_SUCCESS Success.
* @retval VS_ERR_VDEC_INVALID_CHNID Channel number exceeds the maximum supported.
* @retval VS_ERR_VDEC_NULL_PTR Channel parameters pointer is NULL.
* @retval VS_ERR_VDEC_UNEXIST Channel is not created.
* @retval VS_ERR_VDEC_NOT_PERM Channel is not stopped.
* @retval VS_ERR_VDEC_INVALID_PARAM --- dec_type is not equal to channel attributes.\n
* --- max_sps_num/max_pps_num/max_vps_num out of range.
* @note
* - VDEC will set the default H.264/H.265 protocol parameters after channel created.
* - The parameters passed in when calling this function should be obtained by
* calling vs_mal_vdec_chn_protocol_param_get.
* - Changing H.264/H.265 protocol parameters will cause the channel to reset,
* and must be calling after channel stopped.
* - Wrong parameter setting will cause decoding failure, please set it carefully.
*/
vs_int32_t vs_mal_vdec_chn_protocol_param_set(vs_int32_t vdec_chnid,
    const vs_vdec_protocol_param_s *p_protocol_param);

/**
* @brief Get H.264/H.265 protocol parameters by channel number.
* @details
* @param [in] vdec_chnid Channel number. Range: [0, MAX_VDEC_CHN_NUM).
* @param [out] p_chn_param Pointer to the H.264/H.265 protocol parameters structure.
* @retval VS_SUCCESS Success.
* @retval VS_ERR_VDEC_INVALID_CHNID Channel number exceeds the maximum supported.
* @retval VS_ERR_VDEC_NULL_PTR Channel parameters pointer is NULL.
* @retval VS_ERR_VDEC_UNEXIST Channel is not created.
* @note
*/
vs_int32_t vs_mal_vdec_chn_protocol_param_get(vs_int32_t vdec_chnid,
    vs_vdec_protocol_param_s *p_protocol_param);

/**
* @brief Send the stream data to be decoded.
* @details
* @param [in] vdec_chnid Channel number. Range: [0, MAX_VDEC_CHN_NUM).
* @param [in] p_vdec_stream Pointer to stream information structure.
* @param [in] timeout_ms Timeout for sending stream data. Range: [-1, 0x7FFFFFFF].
* - -1: Wait indefinitely until sending successful.
* - 0: Success or failure return immediately.
* - >0: Timeout in milliseconds.
* @retval VS_SUCCESS Success.
* @retval VS_ERR_VDEC_INVALID_CHNID Channel number exceeds the maximum supported.
* @retval VS_ERR_VDEC_NULL_PTR Stream information pointer is NULL.
* @retval VS_ERR_VDEC_UNEXIST Channel is not created or destroying the channel when this function is waiting.
* @retval VS_ERR_VDEC_NOT_PERM Channel is not started or stopping channel when this function is waiting.
* @retval VS_ERR_VDEC_BUF_FULL Stream has not enough space to send when the timeout ends or immediately return mode.
* @retval VS_ERR_VDEC_INVALID_PARAM --- p_virt_addr is NULL or len is 0, but is_stream_end is not true.\n
* --- timeout_ms out of range(< -1).
* @note
* - Only when all the stream are sent, is_stream_end can be set to true.
* it means the decoder will decode and output all the left data in the stream buffer.
* - The stream information that is_stream_end is true can carry empty packets(p_virt_addr is NULL or len is 0).
* - pts and is_display only valid when marking stream is end of frame(is_frame_end is true)
* in frame split stream sending mode.
* - pts only valid in frame or frame split stream sending mode, it will be passed to the post module.
* - The timeout mode takes 10ms as the waiting unit, so the actual maximum waiting time is a multiple of 10ms.
*/
vs_int32_t vs_mal_vdec_stream_send(vs_int32_t vdec_chnid, const vs_vdec_stream_s *p_stream,
    vs_int32_t timeout_ms);

/**
* @brief Acquire the decoded frame of the decoding channel.
* @details
* @param [in] vdec_chnid Channel number. Range: [0, MAX_VDEC_CHN_NUM).
* @param [out] p_frame_info Pointer to the decoded frame structure.
* @param [in] timeout_ms Timeout for acquire frame. Range: [-1, 0x7FFFFFFF].
* - -1: Wait indefinitely until acquiring successful.
* - 0: Success or failure return immediately.
* - >0: Timeout in milliseconds.
* @retval VS_SUCCESS Success.
* @retval VS_ERR_VDEC_INVALID_CHNID Channel number exceeds the maximum supported.
* @retval VS_ERR_VDEC_NULL_PTR Frame information pointer is NULL.
* @retval VS_ERR_VDEC_UNEXIST Channel is not created, destroying or resetting the channel when this function is waiting.
* @retval VS_ERR_VDEC_INVALID_PARAM --- timeout_ms out of range(<-1).
* @retval VS_ERR_VDEC_BUF_EMPTY There has no decoded frame when the timeout ends or immediately return mode.
* @note
* - The timeout mode takes 10ms as the waiting unit, so the actual maximum waiting time is a multiple of 10ms.
* - This interface supports the select operations.
*/
vs_int32_t vs_mal_vdec_frame_acquire(vs_int32_t vdec_chnid, vs_video_frame_info_s *p_frame_info,
    vs_int32_t timeout_ms);

/**
* @brief Release the decoding frame of the vdec channel.
* @details
* @param [in] vdec_chnid Channel number. Range: [0, MAX_VDEC_CHN_NUM).
* @param [in] p_frame_info Pointer to the decoded frame structure.
* @retval VS_SUCCESS Success.
* @retval VS_ERR_VDEC_INVALID_CHNID Channel number exceeds the maximum supported.
* @retval VS_ERR_VDEC_NULL_PTR Frame information pointer is NULL.
* @retval VS_ERR_VDEC_UNEXIST Channel is not created.
* @retval VS_ERR_VDEC_INVALID_PARAM The contents of p_frame_info is illegal.
* @note
* - The released frame must be obtained by vs_mal_vdec_frame_acquire, it is not allowed to
* modify the contents of p_frame_info.
* - Frame should be released immediately after use. If not released in time, the decoding
* process will block for available buffers.
* - Allows the user to release frames not in the order in which the frames were acquired.
*/
vs_int32_t vs_mal_vdec_frame_release(vs_int32_t vdec_chnid, const vs_video_frame_info_s *p_frame_info);

/**
* @brief Acquire the user data of the vdec channel.
* @details
* @param [in] vdec_chnid Channel number. Range: [0, MAX_VDEC_CHN_NUM).
* @param [out] p_sei_userdata The pointer to the user data structure.
* @param [in] timeout_ms Timeout for acquire frame. Range: [-1, 0x7FFFFFFF].
* - -1: Wait indefinitely until acquiring successful.
* - 0: Success or failure return immediately.
* - >0: Timeout in milliseconds.
* @retval VS_SUCCESS Success.
* @retval VS_ERR_VDEC_INVALID_CHNID Channel number exceeds the maximum supported.
* @retval VS_ERR_VDEC_NULL_PTR Frame information pointer is NULL.
* @retval VS_ERR_VDEC_UNEXIST Channel is not created, destroying or resetting the channel when this function is waiting.
* @retval VS_ERR_VDEC_BUF_EMPTY There has no user data when the timeout ends or immediately return mode.
* @note
* - The JPEG/MJPEG decoding channel does not support obtaining user data.
* - If the user data is not obtained in time, it will cause the user data buffer to be full,
* and the next user data in stream will be discarded.
* - The timeout mode takes 10ms as the waiting unit, so the actual maximum waiting time is a multiple of 10ms.
*/
vs_int32_t vs_mal_vdec_sei_userdata_acquire(vs_int32_t vdec_chnid, vs_vdec_sei_userdata_s *p_sei_userdata, vs_int32_t timeout_ms);

/**
* @brief Release the user data of the vdec channel.
* @details
* @param [in] vdec_chnid Channel number. Range: [0, MAX_VDEC_CHN_NUM).
* @param [in] p_sei_userdata The pointer to the user data structure.
* @retval VS_SUCCESS Success.
* @retval VS_ERR_VDEC_INVALID_CHNID Channel number exceeds the maximum supported.
* @retval VS_ERR_VDEC_NULL_PTR user data information pointer is NULL.
* @retval VS_ERR_VDEC_UNEXIST Channel is not created.
* @retval VS_ERR_VDEC_INVALID_PARAM The contents of p_sei_userdata is illegal.
* @note
* - User data should be released immediately after use. If not released in time, the user data will be discarded when the buffer is full.
*/
vs_int32_t vs_mal_vdec_sei_userdata_release(vs_int32_t vdec_chnid, const vs_vdec_sei_userdata_s *p_sei_userdata);

/**
* @brief Set the user picture attribute.
* @details
* @param [in] vdec_chnid Channel number. Range: [0, MAX_VDEC_CHN_NUM).
* @param [in] p_frame_info The pointer of user frame structure.
* @retval VS_SUCCESS Success.
* @retval VS_ERR_VDEC_INVALID_CHNID Channel number exceeds the maximum supported.
* @retval VS_ERR_VDEC_NULL_PTR user frame pointer is NULL.
* @retval VS_ERR_VDEC_UNEXIST Channel is not created.
* @retval VS_ERR_VDEC_NOT_PERM Before setting the user picture, the interface
* vs_mal_vdec_user_frame_disable must be called for disable the user frame, otherwise
* this error code will be returned.
* @note
* - After the user picture attribute is successfully set, the VDEC will always occupy the user picture VB,
* and it will not be released until the decoding channel is destroyed.
* - The user picture attribute supports repeated setting. When the user picture property is set repeatedly,
* the decoder will replace the previous user picture VB block, and occupy the current user picture VB block.
* The decoder will not release the previous user picture VB block.
* - The format of the user picture must be consistent with current decoding channel.
* - The PTS of user picture should be set to 0 uniformly, otherwise it will affect the processing of post module.
*/
vs_int32_t vs_mal_vdec_user_frame_set(vs_int32_t vdec_chnid, const vs_video_frame_info_s *p_frame_info);

/**
* @brief Enable to insert user picture.
* @details
* @param [in] vdec_chnid Channel number. Range: [0, MAX_VDEC_CHN_NUM).
* @param [in] is_instant Enable user image mode.
* - 1 : Enables the user picture to be inserted immediately. In this mode, vdec will first
* reset the decoding channel and then insert the user picture.
* - 0 : Enables the user picture to be inserted delay. In this mode, vdec will wait for
* the decoder to decode all streams and output all pictures before inserting user picture.
* @retval VS_SUCCESS Success.
* @retval VS_ERR_VDEC_INVALID_CHNID Channel number exceeds the maximum supported.
* @retval VS_ERR_VDEC_UNEXIST Channel is not created.
* @retval VS_ERR_VDEC_NOT_PERM Channel is not stopped or no user picture has been set.
* @note
* - In inserted immediately mode, when the user picture is disable and continue send stream,
* the decoder will wait until the next I frame arrives before start decoding correctly.
* - In inserted delay mode, if the user picture is disabled before stream decoded,
* the user picture will not be inserted after stream decoded.
* - Repeat enables insert user picture returned VS_SUCCESS, but no user picture is inserted again.
*/
vs_int32_t vs_mal_vdec_user_frame_enable(vs_int32_t vdec_chnid, vs_bool_t is_instant);

/**
* @brief Disable to insert user picture.
* @details
* @param [in] vdec_chnid Channel number. Range: [0, MAX_VDEC_CHN_NUM).
* @retval VS_SUCCESS Success.
* @retval VS_ERR_VDEC_INVALID_CHNID Channel number exceeds the maximum supported.
* @retval VS_ERR_VDEC_UNEXIST Channel is not created.
* @note
* - In inserted immediately mode, when the user picture is disable and continue send stream,
* the decoder will wait until the next I frame arrives before start decoding correctly.
* - In inserted delay mode, if the user picture is disabled before stream decoded,
* the user picture will not be inserted after stream decoded.
* - Repeat disable insert user picture returned VS_SUCCESS.
*/
vs_int32_t vs_mal_vdec_user_frame_disable(vs_int32_t vdec_chnid);

/**
* @brief Set the mode for passing the decoded picture to the post module.
* @details
* @param [in] vdec_chnid Channel number. Range: [0, MAX_VDEC_CHN_NUM).
* @param [in] output_mode Output mode enumeration.
* @retval VS_SUCCESS Success.
* @retval VS_ERR_VDEC_INVALID_CHNID Channel number exceeds the maximum supported.
* @retval VS_ERR_VDEC_UNEXIST Channel is not created.
* @note
* - Output mode only valid when the post module is bound.
* - Preview mode means discarding the current frame when the post module cannot receive it.
* - Playback mode means repeat sending current frame when the post module cannot receive it.
*/
vs_int32_t vs_mal_vdec_display_mode_set(vs_int32_t vdec_chnid, vs_vdec_display_mode_e display_mode);

/**
* @brief Set the mode for passing the decoded picture to the post module.
* @details
* @param [in] vdec_chnid Channel number. Range: [0, MAX_VDEC_CHN_NUM).
* @param [out] p_output_mode Pointer to output mode enumeration.
* @retval VS_SUCCESS Success.
* @retval VS_ERR_VDEC_INVALID_CHNID Channel number exceeds the maximum supported.
* @retval VS_ERR_VDEC_UNEXIST Channel is not created.
* @retval VS_ERR_VDEC_NULL_PTR Output mode enumeration pointer is NULL.
* @note
*/
vs_int32_t vs_mal_vdec_display_mode_get(vs_int32_t vdec_chnid, vs_vdec_display_mode_e *p_display_mode);

/**
* @brief Set the rotation parameters by channel number.
* @details
* @param [in] vdec_chnid Channel number. Range: [0, MAX_VDEC_CHN_NUM).
* @param [in] rotation Rotation parameters enumeration.
* @retval VS_SUCCESS Success.
* @retval VS_ERR_VDEC_INVALID_CHNID Channel number exceeds the maximum supported.
* @retval VS_ERR_VDEC_UNEXIST Channel is not created.
* @retval
* @note
*/
vs_int32_t vs_mal_vdec_rotation_set(vs_int32_t vdec_chnid, vs_fixed_rotation_e rotation);

/**
* @brief Get the rotation parameters by channel number.
* @details
* @param [in] vdec_chnid Channel number. Range: [0, MAX_VDEC_CHN_NUM).
* @param [out] rotation Pointer to rotation parameters enumeration.
* @retval VS_SUCCESS Success.
* @retval VS_ERR_VDEC_INVALID_CHNID Channel number exceeds the maximum supported.
* @retval VS_ERR_VDEC_NULL_PTR VDEC rotation parameters pointer is NULL.
* @retval VS_ERR_VDEC_UNEXIST Channel is not created.
* @retval
* @note
*/
vs_int32_t vs_mal_vdec_rotation_get(vs_int32_t vdec_chnid, vs_fixed_rotation_e *p_rotation);

/**
* @brief Attach a VB pool which is allocated by user to decoding channel.
* @details This function will make the frame buffer used for decoding use the VB pool provided by the user,
* user needs to ensure that the size and count of the VB pool meet the decoding needs, otherwise it will
* cause the decoding to fail.
* @param [in] vdec_chnid Channel number. Range: [0, MAX_VDEC_CHN_NUM).
* @param [in] p_vb_pool Pointer to VB pool structure, the VB pool id is returned when allocating VB pool.
* @retval VS_SUCCESS Success.
* @retval VS_ERR_VDEC_INVALID_CHNID Channel number exceeds the maximum supported.
* @retval VS_ERR_VDEC_UNEXIST Channel is not created.
* @retval VS_ERR_VDEC_NOT_PERM Channel is not stopped.
* @retval VS_ERR_VDEC_NOT_SUPPORT VB source is not user mode.
* @retval VS_ERR_VDEC_INVALID_PARAM VB pool given by user is not available.
* @note
* - VDEC is not responsible for the allocate and release of the VB pool, it should be done by user.
* - A VB pool can be bound to multiple decoding channels.
* - Changing VB pool attaching will cause the channel to reset, and must be calling after channel stopped.
*/
vs_int32_t vs_mal_vdec_vbpool_attach(vs_int32_t vdec_chnid, vs_uint32_t vbpool_id);

/**
* @brief Detach a VB pool which is allocated by user to decoding channel.
* @details
* @param [in] vdec_chnid Channel number. Range: [0, MAX_VDEC_CHN_NUM).
* @retval VS_ERR_VDEC_INVALID_CHNID Channel number exceeds the maximum supported.
* @retval VS_ERR_VDEC_UNEXIST Channel is not created.
* @retval VS_ERR_VDEC_NOT_PERM Channel is not stopped.
* @retval VS_ERR_VDEC_NOT_SUPPORT VB source is not user mode.
* @note
* - Detaching VB pool will cause the channel to reset, and must be calling after channel stopped.
*/
vs_int32_t vs_mal_vdec_vbpool_detach(vs_int32_t vdec_chnid);

/**
* @brief Set VDEC module parameters.
* @details
* @param [in] p_mod_param Pointer to the VDEC module parameters structure.
* @retval VS_SUCCESS Success.
* @retval VS_ERR_VDEC_NULL_PTR VDEC module parameters pointer is NULL.
* @retval VS_ERR_VDEC_NOT_PERM There are some created decoding channels.
* @retval VS_ERR_VDEC_INVALID_PARAM --- vb_source is VB_SOURCE_COMMON.
* @note
* - VDEC module parameters must be set when all channels are not created.
* - max_frame_width or max_frame_height in max_video_size/max_pic_size are read-only.
*/
vs_int32_t vs_mal_vdec_mod_param_set(const vs_vdec_mod_param_s *p_mod_param);

/**
* @brief Get VDEC module parameters.
* @details
* @param [out] p_mod_param Pointer to the VDEC module parameters structure.
* @retval VS_SUCCESS Success.
* @retval VS_ERR_VDEC_NULL_PTR VDEC module parameters pointer is NULL.
* @note
*/
vs_int32_t vs_mal_vdec_mod_param_get(vs_vdec_mod_param_s *p_mod_param);

#ifdef __cplusplus
}
#endif

#endif /*!<  __VS_MAL_VDEC_H__ */

