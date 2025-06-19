/**
* @file vs_mal_venc.h
* @brief Declaration of venc api
* @details Declaration of venc api
* @author VS software group
* @date 2021-6-15
* @version v0.01
* @copyright (C) 2020, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/

#ifndef __VS_MAL_VENC_H__
#define __VS_MAL_VENC_H__


#include "vs_type.h"
#include "vs_comm.h"
#include "vs_errno.h"
#include "vs_comm_video.h"
#include "vs_venc_defines.h"
#include "vs_mal_vbm.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
* @brief
* @details 	create an encode channel with the input param.
* @param [in]  venc_chnid Indicate the channel number to be be created. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [in]  p_chn_attr The pointer of struct vs_venc_chn_attr_s. Indicate channel attribute, valued by caller,
*			   can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note  	- The attribute of channel is made up of three parts:encoder attribute,rate-controller attribute,
*			gop attribute.
* - The encoder attribute must select an protocol and assign values to its properties.
* - The MAX width and height must be legal.
*
*/
vs_int32_t vs_mal_venc_chn_create(vs_int32_t venc_chnid,
    const vs_venc_chn_attr_s *p_chn_attr);

/**
* @brief
* @details 	Destroy an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be destroyed. Value must be in [0,VENC_MAX_CHN_NUM).
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not destroy the channel that not be created.
* - The channel must be stopped before destroy.
* @par
*
*/
vs_int32_t vs_mal_venc_chn_destroy(vs_int32_t venc_chnid);

/**
* @brief
* @details 	Reset an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be reset. Value must be in [0,VENC_MAX_CHN_NUM).
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not reset the channel that not be created.
* - The channel must be stopped before reset.
* @par
*
*/
vs_int32_t vs_mal_venc_chn_reset(vs_int32_t venc_chnid);

/**
* @brief
* @details 	Start receive frames of an encode channel. It will stop receive frames automatically when received exceeds
*			specified number of frames.
* @param [in]  venc_chnid Indicate the channel number to be started. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [in]  p_start_param The pointer of struct vs_venc_start_param_s, Indicate frame numbers to be received,
*			   valued by caller, can not be NULL.See details in vs_venc_chn_status_s.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not start the channel that not be created.
* - The channel will continuous encoding if number of frames to be received is -1.
* - Can not call this if not stop the channel that already started with specified number of frames
*			to be received.
* - Can re-call this if the channel that already started with continuous encoding.
* - The channel starts to encode only by call this.
* @par
*
*/
vs_int32_t vs_mal_venc_chn_start(vs_int32_t venc_chnid,
    const vs_venc_start_param_s *p_start_param);

/**
* @brief
* @details 	Stop receive frames of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be stopped. Value must be in [0,VENC_MAX_CHN_NUM).
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not stop the channel that not be created.
* - Can re-call this with no limits.
* - You should call this before reset or destroy the channel.
* - It will only stop receive frames from input, the output stream buffer will not be reset,
*			the frames in the input buffer will still be encoded.
* @par
*
*/
vs_int32_t vs_mal_venc_chn_stop(vs_int32_t venc_chnid);

/**
* @brief
* @details 	Get the status of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be got status from. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [out] p_chn_status The pointer of struct vs_venc_chn_status_s, should not be NULL.
*			   See details in vs_venc_chn_status_s.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not get statue from the channel that not be created.
* - It gives the channel status of the moment that when you call.
* @par
*
*/
vs_int32_t vs_mal_venc_chn_status_get(vs_int32_t venc_chnid,
    vs_venc_chn_status_s *p_chn_status);

/**
* @brief
* @details 	Set the attribute of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be set the attribute with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [in]  p_chn_attr The pointer of struct vs_venc_chn_attr_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not set the attribute of channel that not be created.
* - Can not set dynamic attribute, see details in struct vs_venc_chn_attr_s.
* @par
*
*/
vs_int32_t vs_mal_venc_chn_attr_set(vs_int32_t venc_chnid,
    const vs_venc_chn_attr_s *p_chn_attr);

/**
* @brief
* @details 	Get the attribute of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be got the attribute with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [out]  p_chn_attr The pointer of struct vs_venc_chn_attr_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not get the attribute of channel that not be created.
* @par
*
*/
vs_int32_t vs_mal_venc_chn_attr_get(vs_int32_t venc_chnid,
    vs_venc_chn_attr_s *p_chn_attr);

/**
* @brief
* @details 	Acquire the output stream of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be acquired the output stream with.
*			   Value must be in [0,VENC_MAX_CHN_NUM).
* @param [in]  timeout_ms Indicate the timeout in milliseconds. 0 means return immediately;
*			   -1 means will not return until get valid data.
*			   other value means will not return if can not get valid data in indicated milliseconds.
* @param [out] p_stream The pointer of struct vs_venc_stream_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not acquire the output stream of channel that not be created.
* - timeout_ms can not smaller than -1.
* - The pointer of struct vs_venc_pack_s in p_stream can not be NULL, it should be allocated by caller,
*			the length of the pointer must be equal to pack_count in p_stream and the pack_count must greater than 1.
* - The output buffer will full if you do not acquire the stream for a long time, encoder will stop encoding
*			until the output buffer has enough space for encode by you acquire the stream.
* - Recommend acquire & release stream appear in pairs, and release the stream as soon as possible when you
*			acquire the stream to avoid encoder stop encode caused by output buffer full.
* - Recommend acquire stream by using select option. Follow the following procedure:
*			Step 1: Call vs_mal_venc_chn_status_get to get the status of target channel.
*			Step 2: Make sure there is data in output buffer.
*			Step 3: Alloc enough memory for the pointer of struct vs_venc_pack_s in p_stream.
*			Step 4: Call vs_mal_venc_stream_acquire to acquire the stream.
*			Step 5: Call vs_mal_venc_stream_release to release the stream.
* @par
*
*/
vs_int32_t vs_mal_venc_stream_acquire(vs_int32_t venc_chnid,
    vs_venc_stream_s *p_stream,vs_int32_t timeout_ms);

/**
* @brief
* @details 	Release the output stream of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be released the output stream with.
*			   Value must be in [0,VENC_MAX_CHN_NUM).
* @param [in]  p_stream The pointer of struct vs_venc_stream_s, should be the same one in vs_mal_venc_stream_acquire.
*			   Can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not release the output stream of channel that not be created.
* - Can not release the output stream that you did not acquired.
* - Can not release the output stream that you acquired before reset.
* - You should use this pair with vs_mal_venc_stream_acquire.
* @par
*
*/
vs_int32_t vs_mal_venc_stream_release(vs_int32_t venc_chnid,
    vs_venc_stream_s *p_stream);

/**
* @brief
* @details 	Set userdata to be inserted in SEI of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be inserted the userdata with.
*			   Value must be in [0,VENC_MAX_CHN_NUM).
* @param [in]  p_userdata The pointer of struct vs_venc_sei_userdata_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not set if the channel is not create.
* - The pointer p_addr in struct vs_venc_sei_userdata_s can not be NULL and
*			the len in struct vs_venc_sei_userdata_s must be greater than 0.
* @par
*
*/
vs_int32_t vs_mal_venc_sei_userdata_set(vs_int32_t venc_chnid,
    const vs_venc_sei_userdata_s *p_userdata);

/**
* @brief
* @details 	Get userdata to be inserted in SEI of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be got the userdata with.
*			   Value must be in [0,VENC_MAX_CHN_NUM).
* @param [in]  p_userdata The pointer of struct vs_venc_sei_userdata_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not got if the channel is not create.
* - The pointer p_addr in struct vs_venc_sei_userdata_s can not be NULL and
*			the len in struct vs_venc_sei_userdata_s must be greater than the value in vs_mal_venc_sei_userdata_set.
* @par
*
*/
vs_int32_t vs_mal_venc_sei_userdata_get(vs_int32_t venc_chnid,
    vs_venc_sei_userdata_s *p_userdata);

/**
* @brief
* @details 	Send YUV frame to an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be sended frame with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [in]  p_frame The pointer of struct vs_video_frame_info_s, can not be NULL.
* @param [in]  timeout_ms Indicate the timeout in milliseconds. 0 means return immediately whether success or not;
*			   -1 means will not return until send frame success. \
*			   other positive value means will not return if send frame not success in indicated milliseconds.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - timeout_ms can not smaller than -1.
* - You can send frame to an encode channel that not bind to up module.
* - The YUV frame size must not smaller than channel size.
* - Can not send frame to the channel not created or started.
* @par
*
*/
vs_int32_t vs_mal_venc_frame_send(vs_int32_t venc_chnid,
    const vs_video_frame_info_s *p_frame,vs_int32_t timeout_ms);

/**
* @brief
* @details 	Send YUV frame with QpMap to an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be sended frame with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [in]  p_frame The pointer of struct vs_venc_qpmap_frame_info_s, can not be NULL.
* @param [in]  timeout_ms Indicate the timeout in milliseconds. 0 means return immediately whether success or not;
*			   -1 means will not return until send frame success.
*			   other positive value means will not return if send frame not success in indicated milliseconds.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - timeout_ms can not smaller than -1.
* - You can send frame to an encode channel that not bind to up module.
* - The YUV frame size must not smaller than channel size.
* - Can not send frame to the channel not created or started.
* @par
*
*/
vs_int32_t vs_mal_venc_qpmap_frame_send(vs_int32_t venc_chnid,
    const vs_venc_qpmap_frame_info_s *p_frame,vs_int32_t timeout_ms);

/**
* @brief
* @details 	Request an IDR frame in an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be requested IDR frame with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [in]  force Encode an IDR frame immediately or not. Value must be in [0,1].
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not request if the channel is not create.
* - Only supported by H.264/H.265.
* - Encode an IDR frame immediately if force is 1, otherwise will Encode an IDR frame with frame control.
* - Multiple calls between to frame encoding only cause one IDR frame. It will affect frame-rate and bitrate
*			if you call this frequently, use it prudently.
* @par
*
*/
vs_int32_t vs_mal_venc_idr_request(vs_int32_t venc_chnid, vs_int32_t force);

/**
* @brief
* @details 	Enable IDR frame encoding of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be enabled of IDR frame encoding with. Value must be in [0,VENC_MAX_CHN_NUM).
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not enable if the channel is not create.
* - Only supported by H.264/H.265.
* - Enable from disable will cause next frame to be encode as IDR frame.
* @par
*
*/
vs_int32_t vs_mal_venc_idr_enable(vs_int32_t venc_chnid);

/**
* @brief
* @details 	Disable IDR frame encoding of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be disabled of IDR frame encoding with. Value must be in [0,VENC_MAX_CHN_NUM).
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not disable if the channel is not create.
* - Only supported by H.264/H.265.
* - The encoder will not encode any frame as IDR after call this until vs_mal_venc_idr_enable is called.
* - Can not disable when gop mode is E_VENC_GOP_MODE_LTRP.
* @par
*
*/
vs_int32_t vs_mal_venc_idr_disable(vs_int32_t venc_chnid);

/**
* @brief
* @details 	Get device fd of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be got device fd with. Value must be in [0,VENC_MAX_CHN_NUM).
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* @par
*
*/
vs_int32_t vs_mal_venc_chn_fd_get(vs_int32_t venc_chnid);

/**
* @brief
* @details 	Close device fd of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be closed device fd with. Value must be in [0,VENC_MAX_CHN_NUM).
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not be called with other MAL interface in the same time, ensure that when using this.
* - You must not close fd when monitor stream with select mode.
* - Recommend close fd after the encode channel is destroyed.
* @par
*
*/
vs_int32_t vs_mal_venc_chn_fd_close(vs_int32_t venc_chnid);

/**
* @brief
* @details 	Set roi area of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be set with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [in]  p_roi The pointer of struct vs_venc_roi_attr_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not set if the channel is not create.
* - Must be called between channel created and destroyed, will affect from next frame when called during encoding.
* - System disabled all roi area by default, you must call this to enable them one at a time.
* - Recommend call this before channel start, avoid call this during encoding if you can.
* - Recommend call this after vs_mal_venc_roi_attr_get and only modify the properties you need.
* - pskip has higher priority with roi.
* @par
*
*/
vs_int32_t vs_mal_venc_roi_attr_set(vs_int32_t venc_chnid,
    const vs_venc_roi_attr_s *p_roi_attr);

/**
* @brief
* @details 	Get roi area of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be got with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [in]  index Indicate the roi index to be got with. Value must be in [0,VENC_MAX_ROI_NUM).
* @param [out]  p_roi The pointer of struct vs_venc_roi_attr_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not get if the channel is not create.
* - Recommend call this before channel start, avoid call this during encoding if you can.
* @par
*
*/
vs_int32_t vs_mal_venc_roi_attr_get(vs_int32_t venc_chnid,vs_int32_t index,
    vs_venc_roi_attr_s *p_roi_attr);

/**
* @brief
* @details 	Set roi background area frame rate of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be set with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [in]  p_roi_bg_framerate The pointer of struct vs_venc_roi_bg_framerate_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not set if the channel is not create.
* - Can be called if none roi area is enabled.
* - Work for E_VENC_GOP_MODE_NORMP of vs_venc_gop_attr_s only.
* - Must be called between channel created and destroyed, will affect from next frame when called during encoding.
* - System disabled by default, you must call this to enable it.
* - Source frame rate must greater than target frame rate and target frame rate greater than 0
*			or all frame rate equal to -1.
* - Recommend call this before channel start, avoid call this during encoding if you can.
* - Recommend call this after vs_mal_venc_roi_bg_framerate_get and only modify the properties you need.
* @par
*
*/
vs_int32_t vs_mal_venc_roi_bg_framerate_set(vs_int32_t venc_chnid, const vs_venc_roi_bg_framerate_s *p_roi_bg_framerate);

/**
* @brief
* @details 	Get roi background area frame rate of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be got with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [out]  p_roi_bg_framerate The pointer of struct vs_venc_roi_bg_framerate_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not get if the channel is not create.
* - Must be called between channel created and destroyed.
* - Recommend call this before channel start, avoid call this during encoding if you can.
* @par
*
*/
vs_int32_t vs_mal_venc_roi_bg_framerate_get(vs_int32_t venc_chnid, vs_venc_roi_bg_framerate_s *p_roi_bg_framerate);

/**
* @brief
* @details 	Set slice split attribute of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be set with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [in]  p_slice_split The pointer of struct vs_venc_h264_slice_split_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not set if the channel is not create.
* - Only supported by H.265.
* - Must be called between channel created and destroyed, will affect from next frame when called during encoding.
* - System disabled by default, you must call this to enable it.
* - Recommend call this before channel start, avoid call this during encoding if you can.
* - Recommend call this after vs_mal_venc_h264_slice_split_get and only modify the properties you need.
* @par
*
*/
vs_int32_t vs_mal_venc_h264_slice_split_set(vs_int32_t venc_chnid,
    const vs_venc_h264_slice_split_s *p_slice_split);

/**
* @brief
* @details 	Get slice split attribute of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be got with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [out]  p_slice_split The pointer of struct vs_venc_h264_slice_split_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not get if the channel is not create.
* - Only supported by H.264.
* - Must be called between channel created and destroyed.
* - Recommend call this before channel start, avoid call this during encoding if you can.
* @par
*
*/
vs_int32_t vs_mal_venc_h264_slice_split_get(vs_int32_t venc_chnid,
    vs_venc_h264_slice_split_s *p_slice_split);

/**
* @brief
* @details 	Set transform and quant param of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be set with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [in]  p_trans The pointer of struct vs_venc_h264_trans_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not set if the channel is not create.
* - Only supported by H.264.
* - Must be called between channel created and destroyed, will affect from next I frame when called during encoding.
* - System has default values. Do not recommend to call.
* - Recommend call this before channel start, avoid call this during encoding if you can.
* - Recommend call this after vs_mal_venc_h264_trans_get and only modify the properties you need.
* @par
*
*/
vs_int32_t vs_mal_venc_h264_trans_set(vs_int32_t venc_chnid,
    const vs_venc_h264_trans_s *p_trans);

/**
* @brief
* @details 	Get transform and quant param of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be got with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [out]  p_trans The pointer of struct vs_venc_h264_trans_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not get if the channel is not create.
* - Only supported by H.264.
* - Must be called between channel created and destroyed.
* - Recommend call this before channel start, avoid call this during encoding if you can.
* @par
*
*/
vs_int32_t vs_mal_venc_h264_trans_get(vs_int32_t venc_chnid,
    vs_venc_h264_trans_s *p_trans);

/**
* @brief
* @details 	Set entropy mode of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be set with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [in]  p_entropy The pointer of struct vs_venc_h264_entropy_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not set if the channel is not create.
* - Only supported by H.264.
* - Must be called between channel created and destroyed, will affect from next I frame when called during encoding.
* - You can set I/P/B frame entropy mode separated.
* - You can only set CAVLC mode in baseline profile.
* - System has default values. Do not recommend to call.
* - Recommend call this before channel start, avoid call this during encoding if you can.
* - Recommend call this after vs_mal_venc_h264_entropy_get and only modify the properties you need.
* @par
*
*/
vs_int32_t vs_mal_venc_h264_entropy_set(vs_int32_t venc_chnid,
    const vs_venc_h264_entropy_s *p_entropy);

/**
* @brief
* @details 	Get entropy mode of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be got with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [out]  p_entropy The pointer of struct vs_venc_h264_entropy_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not get if the channel is not create.
* - Only supported by H.264.
* - Must be called between channel created and destroyed.
* - Recommend call this before channel start, avoid call this during encoding if you can.
* @par
*
*/
vs_int32_t vs_mal_venc_h264_entropy_get(vs_int32_t venc_chnid,
    vs_venc_h264_entropy_s *p_entropy);

/**
* @brief
* @details 	Set deblocking attribute of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be set with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [in]  p_dblk The pointer of struct vs_venc_h264_dblk_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not set if the channel is not create.
* - Only supported by H.264.
* - Must be called between channel created and destroyed, will affect from next frame when called during encoding.
* - System enable deblocking by default, you can call this to disable it.
* - Recommend call this before channel start, avoid call this during encoding if you can.
* - Recommend call this after vs_mal_venc_h264_dblk_get and only modify the properties you need.
* @par
*
*/
vs_int32_t vs_mal_venc_h264_dblk_set(vs_int32_t venc_chnid,
    const vs_venc_h264_dblk_s *p_dblk);

/**
* @brief
* @details 	Get deblocking attribute of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be got with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [out]  p_dblk The pointer of struct vs_venc_h264_dblk_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not get if the channel is not create.
* - Only supported by H.264.
* - Must be called between channel created and destroyed.
* - Recommend call this before channel start, avoid call this during encoding if you can.
* @par
*
*/
vs_int32_t vs_mal_venc_h264_dblk_get(vs_int32_t venc_chnid,
    vs_venc_h264_dblk_s *p_dblk);

/**
* @brief
* @details 	Set vui attribute of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be set with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [in]  p_vui The pointer of struct vs_venc_h264_vui_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not set if the channel is not create.
* - Only supported by H.264.
* - Must be called between channel created and destroyed, will affect from next I frame when called during encoding.
* - Recommend call this before channel start, avoid call this during encoding if you can.
* - Recommend call this after vs_mal_venc_h264_vui_get and only modify the properties you need.
* @par
*
*/
vs_int32_t vs_mal_venc_h264_vui_set(vs_int32_t venc_chnid,
    const vs_venc_h264_vui_s *p_vui);

/**
* @brief
* @details 	Get vui attribute of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be got with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [out]  p_vui The pointer of struct vs_venc_h264_vui_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not get if the channel is not create.
* - Only supported by H.264.
* - Must be called between channel created and destroyed.
* - Recommend call this before channel start, avoid call this during encoding if you can.
* @par
*
*/
vs_int32_t vs_mal_venc_h264_vui_get(vs_int32_t venc_chnid,
    vs_venc_h264_vui_s *p_vui);

/**
* @brief
* @details 	Set vui attribute of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be set with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [in]  p_vui The pointer of struct vs_venc_h265_vui_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not set if the channel is not create.
* - Only supported by H.265.
* - Must be called between channel created and destroyed, will affect from next I frame when called during encoding.
* - Recommend call this before channel start, avoid call this during encoding if you can.
* - Recommend call this after vs_mal_venc_h265_vui_get and only modify the properties you need.
* @par
*
*/
vs_int32_t vs_mal_venc_h265_vui_set(vs_int32_t venc_chnid,
    const vs_venc_h265_vui_s *p_vui);

/**
* @brief
* @details 	Get vui attribute of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be got with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [out]  p_vui The pointer of struct vs_venc_h265_vui_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not get if the channel is not create.
* - Only supported by H.265.
* - Must be called between channel created and destroyed.
* - Recommend call this before channel start, avoid call this during encoding if you can.
* @par
*
*/
vs_int32_t vs_mal_venc_h265_vui_get(vs_int32_t venc_chnid,
    vs_venc_h265_vui_s *p_vui);

/**
* @brief
* @details 	Set jpeg advance param of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be set with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [in]  p_jpeg_param The pointer of struct vs_venc_jpeg_param_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not set if the channel is not create.
* - Must be called between channel created and destroyed, will affect from next frame when called during encoding.
* - Recommend call this before channel start, avoid call this during encoding if you can.
* - Recommend call this after vs_mal_venc_jpeg_param_get and only modify the properties you need.
* @par
*
*/
vs_int32_t vs_mal_venc_jpeg_param_set(vs_int32_t venc_chnid,
    const vs_venc_jpeg_param_s *p_jpeg_param);

/**
* @brief
* @details 	Get jpeg advance param of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be got with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [out]  p_jpeg_param The pointer of struct vs_venc_jpeg_param_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not get if the channel is not create.
* - Must be called between channel created and destroyed.
* - Recommend call this before channel start, avoid call this during encoding if you can.
* @par
*
*/
vs_int32_t vs_mal_venc_jpeg_param_get(vs_int32_t venc_chnid,
    vs_venc_jpeg_param_s *p_jpeg_param);

/**
* @brief
* @details 	Set mjpeg advance param of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be set with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [in]  p_mjpeg_param The pointer of struct vs_venc_mjpeg_param_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not set if the channel is not create.
* - Must be called between channel created and destroyed, will affect from next frame when called during encoding.
* - Recommend call this before channel start, avoid call this during encoding if you can.
* - Recommend call this after vs_mal_venc_mjpeg_param_get and only modify the properties you need.
* @par
*
*/
vs_int32_t vs_mal_venc_mjpeg_param_set(vs_int32_t venc_chnid,
    const vs_venc_mjpeg_param_s *p_mjpeg_param);

/**
* @brief
* @details 	Get mjpeg advance param of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be got with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [out]  p_mjpeg_param The pointer of struct vs_venc_mjpeg_param_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not get if the channel is not create.
* - Must be called between channel created and destroyed.
* - Recommend call this before channel start, avoid call this during encoding if you can.
* @par
*
*/
vs_int32_t vs_mal_venc_mjpeg_param_get(vs_int32_t venc_chnid,
    vs_venc_mjpeg_param_s *p_mjpeg_param);

/**
* @brief
* @details 	Set bitrate control advance param of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be set with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [in]  p_brc_param The pointer of struct vs_venc_brc_param_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not set if the channel is not create.
* - Must be called between channel created and destroyed, will affect from next frame when called during encoding.
* - System has default values and use them by default.
* - Recommend call this before channel start, avoid call this during encoding if you can.
* - Recommend call this after vs_mal_venc_brc_param_get and only modify the properties you need.
* @par
*
*/
vs_int32_t vs_mal_venc_brc_param_set(vs_int32_t venc_chnid,
    const vs_venc_brc_param_s *p_brc_param);

/**
* @brief
* @details 	Get bitrate control advance param of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be got with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [out]  p_brc_param The pointer of struct vs_venc_brc_param_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not get if the channel is not create.
* @par
*
*/
vs_int32_t vs_mal_venc_brc_param_get(vs_int32_t venc_chnid,
    vs_venc_brc_param_s *p_brc_param);

/**
* @brief
* @details 	Set gop reference param of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be set with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [in]  p_ref_param The pointer of struct vs_venc_ref_policy_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not set if the channel is not create.
* - Must be called between channel created and destroyed, will affect from next I frame when called during encoding.
* - Recommend call this before channel start, avoid call this during encoding if you can.
* - if enable with E_VENC_GOP_MODE_LTRP gop mode, ensure hits: interval_lp = base * (enhance + 1).
* @par
*
*/
vs_int32_t vs_mal_venc_ref_policy_set(vs_int32_t venc_chnid,
    const vs_venc_ref_policy_s *p_ref_param);

/**
* @brief
* @details 	Get gop reference param of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be got with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [out]  p_ref_param The pointer of struct vs_venc_ref_policy_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not get if the channel is not create.
* @par
*
*/
vs_int32_t vs_mal_venc_ref_policy_get(vs_int32_t venc_chnid,
    vs_venc_ref_policy_s *p_ref_param);

/**
* @brief
* @details 	Set svct param param of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be set with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [in]  p_svct The pointer of struct vs_venc_svct_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not set if the channel is not create.
* - Should not be create when vs_mal_venc_ref_policy_set is created.
* - Must be called between channel created and destroyed, will affect from next I frame when called during encoding.
* - Recommend call this before channel start, avoid call this during encoding if you can.
* @par
*
*/
vs_int32_t vs_mal_venc_svct_set(vs_int32_t venc_chnid,
    const vs_venc_svct_s *p_svct);

/**
* @brief
* @details 	Get svct param of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be got with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [out]  p_svct The pointer of struct vs_venc_svct_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not get if the channel is not create.
* @par
*
*/
vs_int32_t vs_mal_venc_svct_get(vs_int32_t venc_chnid,
    vs_venc_svct_s *p_svct);


/**
* @brief
* @details 	Set jpeg snap mode of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be set with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [in]  enJpegEncodeMode Indicate the snap mode. Value must be in vs_venc_jpeg_snap_mode_e.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not set if the channel is not create.
* - Only supported by JPEG.
* - Must be called between channel created and destroyed.
* - Encode channel use E_VENC_JPEG_ENCMODE_ALL by default after channel created, change it when you need.
* @par
*
*/
vs_int32_t vs_mal_venc_jpeg_snap_mode_set(vs_int32_t venc_chnid,
    const vs_venc_jpeg_snap_mode_e snap_mode);

/**
* @brief
* @details 	Get jpeg snap mode of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be got with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [out]  p_snap_mode The pointer of enum vs_venc_jpeg_snap_mode_e, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not get if the channel is not create.
* - Only supported by JPEG.
* - Must be called between channel created and destroyed.
* @par
*
*/
vs_int32_t vs_mal_venc_jpeg_snap_mode_get(vs_int32_t venc_chnid,
    vs_venc_jpeg_snap_mode_e *p_snap_mode);

/**
* @brief
* @details 	Get output stream buffer information of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be got with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [out]  p_stream_buf The pointer of enum vs_venc_stream_buf_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not get if the channel is not create.
* - Must be called between channel start and stop.
* @par
*
*/
vs_int32_t vs_mal_venc_stream_buf_get(vs_int32_t venc_chnid,
    vs_venc_stream_buf_s *p_stream_buf);

/**
* @brief
* @details 	Set slice split attribute of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be set with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [in]  p_slice_split The pointer of struct vs_venc_h265_slice_split_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not set if the channel is not create.
* - Only supported by H.265.
* - Must be called between channel created and destroyed, will affect from next frame when called during encoding.
* - System disabled by default, you must call this to enable it.
* - Recommend call this before channel start, avoid call this during encoding if you can.
* - Recommend call this after vs_mal_venc_h265_slice_split_get and only modify the properties you need.
* @par
*
*/
vs_int32_t vs_mal_venc_h265_slice_split_set(vs_int32_t venc_chnid,
    const vs_venc_h265_slice_split_s *p_slice_split);

/**
* @brief
* @details 	Get slice split attribute of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be got with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [out]  p_slice_split The pointer of struct vs_venc_h265_slice_split_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not get if the channel is not create.
* - Only supported by H.265.
* - Must be called between channel created and destroyed.
* - Recommend call this before channel start, avoid call this during encoding if you can.
* @par
*
*/
vs_int32_t vs_mal_venc_h265_slice_split_get(vs_int32_t venc_chnid,
    vs_venc_h265_slice_split_s *p_slice_split);

/**
* @brief
* @details 	Set intra prediction flag of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be set with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [in]  p_intra_pred The pointer of struct vs_venc_h265_intra_pred_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not set if the channel is not create.
* - Only supported by H.265.
* - Must be called between channel created and destroyed, will affect from next I frame when called during encoding.
* - System has default value. Do not recommend to call.
* - Recommend call this before channel start, avoid call this during encoding if you can.
* - Recommend call this after vs_mal_venc_h265_intra_pred_get and only modify the properties you need.
* @par
*
*/
vs_int32_t vs_mal_venc_h265_intra_pred_set(vs_int32_t venc_chnid,
    const vs_venc_h265_intra_pred_s *p_intra_pred);

/**
* @brief
* @details 	Get intra prediction flag of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be got with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [out]  p_intra_pred The pointer of struct vs_venc_h265_intra_pred_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not get if the channel is not create.
* - Only supported by H.265.
* - Must be called between channel created and destroyed.
* - Recommend call this before channel start, avoid call this during encoding if you can.
* @par
*
*/
vs_int32_t vs_mal_venc_h265_intra_pred_get(vs_int32_t venc_chnid,
    vs_venc_h265_intra_pred_s *p_intra_pred);

/**
* @brief
* @details 	Set transform and quant param of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be set with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [in]  p_trans The pointer of struct vs_venc_h265_trans_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not set if the channel is not create.
* - Only supported by H.265.
* - Must be called between channel created and destroyed, will affect from next I frame when called during encoding.
* - System has default values. Do not recommend to call.
* - Recommend call this before channel start, avoid call this during encoding if you can.
* - Recommend call this after vs_mal_venc_h265_trans_get and only modify the properties you need.
* @par
*
*/
vs_int32_t vs_mal_venc_h265_trans_set(vs_int32_t venc_chnid,
    const vs_venc_h265_trans_s *p_trans);

/**
* @brief
* @details 	Get transform and quant param of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be got with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [out]  p_trans The pointer of struct vs_venc_h265_trans_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not get if the channel is not create.
* - Only supported by H.265.
* - Must be called between channel created and destroyed.
* - Recommend call this before channel start, avoid call this during encoding if you can.
* @par
*
*/
vs_int32_t vs_mal_venc_h265_trans_get(vs_int32_t venc_chnid,
    vs_venc_h265_trans_s *p_trans);

/**
* @brief
* @details 	Set deblocking attribute of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be set with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [in]  p_dblk The pointer of struct vs_venc_h265_dblk_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not set if the channel is not create.
* - Only supported by H.265.
* - Must be called between channel created and destroyed, will affect from next frame when called during encoding.
* - System enable deblocking by default, you can call this to disable it.
* - Recommend call this before channel start, avoid call this during encoding if you can.
* - Recommend call this after vs_mal_venc_h265_dblk_get and only modify the properties you need.
* @par
*
*/
vs_int32_t vs_mal_venc_h265_dblk_set(vs_int32_t venc_chnid,
    const vs_venc_h265_dblk_s *p_dblk);

/**
* @brief
* @details 	Get deblocking attribute of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be got with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [out]  p_dblk The pointer of struct vs_venc_h265_dblk_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not get if the channel is not create.
* - Only supported by H.265.
* - Must be called between channel created and destroyed.
* - Recommend call this before channel start, avoid call this during encoding if you can.
* @par
*
*/
vs_int32_t vs_mal_venc_h265_dblk_get(vs_int32_t venc_chnid,
    vs_venc_h265_dblk_s *p_dblk);

/**
* @brief
* @details 	Set sao attribute of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be set with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [in]  p_dblk The pointer of struct vs_venc_h265_dblk_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not set if the channel is not create.
* - Only supported by H.265.
* - Must be called between channel created and destroyed, will affect from next frame when called during encoding.
* - System enable sao by default, you can call this to disable it.
* - Recommend call this before channel start, avoid call this during encoding if you can.
* - Recommend call this after vs_mal_venc_h265_sao_get and only modify the properties you need.
* @par
*
*/
vs_int32_t vs_mal_venc_h265_sao_set(vs_int32_t venc_chnid,
    const vs_venc_h265_sao_s *p_sao);

/**
* @brief
* @details 	Get sao attribute of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be got with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [out]  p_dblk The pointer of struct vs_venc_h265_dblk_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not get if the channel is not create.
* - Only supported by H.265.
* - Must be called between channel created and destroyed.
* - Recommend call this before channel start, avoid call this during encoding if you can.
* @par
*
*/
vs_int32_t vs_mal_venc_h265_sao_get(vs_int32_t venc_chnid,
    vs_venc_h265_sao_s *p_sao);

/**
* @brief
* @details 	Set frame lost policy in moving bitrate overrun case of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be set with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [in]  p_frame_lost The pointer of struct vs_venc_frame_lost_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not set if the channel is not create.
* - Must be called between channel created and destroyed, will affect from next frame when called during encoding.
* - System has default values, you can call this if necessary.
* @par
*
*/
vs_int32_t vs_mal_venc_frame_lost_policy_set(vs_int32_t venc_chnid,
    const vs_venc_frame_lost_s *p_frame_lost_policy);

/**
* @brief
* @details 	Get frame lost policy in moving bitrate overrun case of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be got with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [out]  p_frame_lost The pointer of struct vs_venc_frame_lost_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not get if the channel is not create.
* @par
*
*/
vs_int32_t vs_mal_venc_frame_lost_policy_get(vs_int32_t venc_chnid,
    vs_venc_frame_lost_s *p_frame_lost_policy);

/**
* @brief
* @details 	Set the policy of frame overrun case of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be set with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [in]  p_frame_overrun The pointer of struct vs_venc_frame_overrun_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not set if the channel is not create.
* - Must be called between channel created and destroyed, will affect from next frame when called during encoding.
* - System has default values, you can call this if necessary.
* @par
*
*/
vs_int32_t vs_mal_venc_frame_overrun_policy_set(vs_int32_t venc_chnid,
    const vs_venc_frame_overrun_s *p_frame_overrun_policy);

/**
* @brief
* @details 	Get the policy of frame overrun case of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be got with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [out]  p_frame_overrun The pointer of struct vs_venc_frame_overrun_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not get if the channel is not create.
* @par
*
*/
vs_int32_t vs_mal_venc_frame_overrun_policy_get(vs_int32_t venc_chnid,
    vs_venc_frame_overrun_s *p_frame_overrun_policy);

/**
* @brief
* @details 	Set gradual decoding refresh param of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be set with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [in]  p_gdr The pointer of struct vs_venc_gdr_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not set if the channel is not create.
* - Can not enable when debreath is enabled.
* - Must be called between channel created and destroyed, will affect from the IDR in next gop when enable during encoding,
*			but affect immediately when enable during encoding.
* - Modify param dynamically will affect from the IDR in next gop.
* @par
*
*/
vs_int32_t vs_mal_venc_gdr_set(vs_int32_t venc_chnid,
    const vs_venc_gdr_s *p_gdr);

/**
* @brief
* @details 	Get gradual decoding refresh param of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be got with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [out]  p_gdr The pointer of struct vs_venc_gdr_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not get if the channel is not create.
* - Must be called between channel created and destroyed.
* @par
*
*/
vs_int32_t vs_mal_venc_gdr_get(vs_int32_t venc_chnid,
    vs_venc_gdr_s *p_gdr);

/**
* @brief
* @details 	Set advance channel param of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be set with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [in]  p_chn_param The pointer of struct vs_venc_chn_param_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not set if the channel is not create.
* - Must be called between channel created and destroyed.
* - Recommend call this after vs_mal_venc_chn_param_get and only modify the properties you need.
* @par
*
*/
vs_int32_t vs_mal_venc_chn_param_set(vs_int32_t venc_chnid,
    const vs_venc_chn_param_s *p_chn_param);

/**
* @brief
* @details 	Get advance channel param of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be got with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [out]  p_chn_param The pointer of struct vs_venc_chn_param_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not get if the channel is not create.
* @par
*
*/
vs_int32_t vs_mal_venc_chn_param_get(vs_int32_t venc_chnid,
    vs_venc_chn_param_s *p_chn_param);

/**
* @brief
* @details 	Set venc module param.
* @param [in]  venc_chnid Indicate the channel number to be set with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [in]  p_mod_param The pointer of struct vs_venc_mod_param_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Must be called before any channel created.
* - Different protocol param should be set separately.
* - Recommend call this after vs_mal_venc_mod_param_get and only modify the properties you need.
* @par
*
*/
vs_int32_t vs_mal_venc_mod_param_set(const vs_venc_mod_param_s *p_mod_param);

/**
* @brief
* @details 	Get venc module param.
* @param [in]  venc_chnid Indicate the channel number to be got with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [out]  p_mod_param The pointer of struct vs_venc_mod_param_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Different protocol param should be got separately.
* @par
*
*/
vs_int32_t vs_mal_venc_mod_param_get(vs_venc_mod_param_s *p_mod_param);

/**
* @brief
* @details 	Set foreground protect param attribute of an encode channel by channel number. reserved
* @param [in]  venc_chnid Indicate the channel number to be set with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [in]  p_protect The pointer of struct vs_venc_fg_protect_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not set if the channel is not create.
* - Must be called between channel created and destroyed, will affect from next frame when called during encoding.
* - Recommend call this before channel start, avoid call this during encoding if you can.
* - Recommend call this after vs_mal_venc_fg_protect_get and only modify the properties you need.
* @par
*
*/
vs_int32_t vs_mal_venc_fg_protect_set(vs_int32_t venc_chnid,
    const vs_venc_fg_protect_s *p_protect);

/**
* @brief
* @details 	Get foreground protect param attribute of an encode channel by channel number. reserved
* @param [in]  venc_chnid Indicate the channel number to be got with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [out]  p_protect The pointer of struct vs_venc_fg_protect_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not set if the channel is not create.
* - Must be called between channel created and destroyed.
* - Recommend call this before channel start, avoid call this during encoding if you can.
* @par
*
*/
vs_int32_t vs_mal_venc_fg_protect_get(vs_int32_t venc_chnid,
    vs_venc_fg_protect_s *p_protect);

/**
* @brief
* @details 	Set debreath param of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be set with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [in]  p_debreath The pointer of struct vs_venc_debreath_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not set if the channel is not create.
* - Can not enable when gdr is enabled.
* - Only supported by H.264/H.265.
* - Must be called between channel created and destroyed, will affect from next frame when called during encoding.
* - Recommend call this before channel start, avoid call this during encoding if you can.
* - Recommend call this after vs_mal_venc_debreath_get and only modify the properties you need.
* @par
*
*/
vs_int32_t vs_mal_venc_debreath_set(vs_int32_t venc_chnid,
    const vs_venc_debreath_s *p_debreath);

/**
* @brief
* @details 	Get debreath param of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be got with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [out]  p_debreath The pointer of struct vs_venc_debreath_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not set if the channel is not create.
* - Only supported by H.264/H.265.
* - Must be called between channel created and destroyed.
* @par
*
*/
vs_int32_t vs_mal_venc_debreath_get(vs_int32_t venc_chnid,
    vs_venc_debreath_s *p_debreath);

/**
* @brief
* @details 	Set hierarchical QP param of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be set with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [in]  p_hierarchical The pointer of struct vs_venc_hierarchical_qp_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not set if the channel is not create.
* - Only supported by H.264/H.265.
* - Must be called between channel created and destroyed, will affect from next I frame when called during encoding.
* - Recommend call this before channel start, avoid call this during encoding if you can.
* - Recommend call this after vs_mal_venc_hierarchical_qp_get and only modify the properties you need.
* @par
*
*/
vs_int32_t vs_mal_venc_hierarchical_qp_set(vs_int32_t venc_chnid,
    const vs_venc_hierarchical_qp_s *p_hierarchical_qp);

/**
* @brief
* @details 	Get hierarchical QP param of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be got with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [out]  p_hierarchical The pointer of struct vs_venc_hierarchical_qp_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not set if the channel is not create.
* - Only supported by H.264/H.265.
* - Must be called between channel created and destroyed.
* @par
*
*/
vs_int32_t vs_mal_venc_hierarchical_qp_get(vs_int32_t venc_chnid,
    vs_venc_hierarchical_qp_s *p_hierarchical_qp);

/**
* @brief
* @details 	Set slice split attribute of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be set with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [in]  p_slice_split The pointer of struct vs_venc_slice_split_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not set if the channel is not create.
* - Only supported by H.264/H.265.
* - Must be called between channel created and destroyed, will affect from next frame when called during encoding.
* - System disabled by default, you must call this to enable it.
* - Recommend call this before channel start, avoid call this during encoding if you can.
* - Recommend call this after vs_mal_venc_slice_split_get and only modify the properties you need.
* @par
*
*/
vs_int32_t vs_mal_venc_slice_split_set(vs_int32_t venc_chnid,
    const vs_venc_slice_split_s *p_slice_split);

/**
* @brief
* @details 	Get slice split attribute of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be got with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [out]  p_slice_split The pointer of struct vs_venc_slice_split_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not get if the channel is not create.
* - Only supported by H.264/H.265.
* - Must be called between channel created and destroyed.
* @par
*
*/
vs_int32_t vs_mal_venc_slice_split_get(vs_int32_t venc_chnid,
    vs_venc_slice_split_s *p_slice_split);

/**
* @brief
* @details 	Set ipcm area attribute of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be set with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [in]  p_ipcm The pointer of struct vs_venc_ipcm_area_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not set if the channel is not create.
* - Must be called between channel created and destroyed, will affect from next frame when called during encoding.
* - Recommend call this before channel start, avoid call this during encoding if you can.
* - Recommend call this after vs_mal_venc_ipcm_area_get and only modify the properties you need.
* @par
*
*/
vs_int32_t vs_mal_venc_ipcm_area_set(vs_int32_t venc_chnid,
    const vs_venc_ipcm_area_s *p_ipcm);

/**
* @brief
* @details 	Get ipcm area attribute of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be got with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [in]  index Indicate ipcm area index to be got with. Value must be in [0,MAX_IPCM_NUM).
* @param [out]  p_ipcm The pointer of struct vs_venc_ipcm_area_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not get if the channel is not create.
* - Must be called between channel created and destroyed.
* @par
*
*/
vs_int32_t vs_mal_venc_ipcm_area_get(vs_int32_t venc_chnid,vs_int32_t index,
    vs_venc_ipcm_area_s *p_ipcm);

/**
* @brief
* @details 	Set intra area attribute of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be set with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [in]  p_intra The pointer of struct vs_venc_intra_area_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not set if the channel is not create.
* - Must be called between channel created and destroyed, will affect from next frame when called during encoding.
* - Recommend call this before channel start, avoid call this during encoding if you can.
* - Recommend call this after vs_mal_venc_intra_area_get and only modify the properties you need.
* @par
*
*/
vs_int32_t vs_mal_venc_intra_area_set(vs_int32_t venc_chnid,
    const vs_venc_intra_area_s *p_intra);

/**
* @brief
* @details 	Get intra area attribute of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be got with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [in]  index Indicate ipcm area index to be got with. Value must be in [0,MAX_INTRA_NUM).
* @param [out]  p_intra The pointer of struct vs_venc_intra_area_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not get if the channel is not create.
* - Must be called between channel created and destroyed.
* @par
*
*/
vs_int32_t vs_mal_venc_intra_area_get(vs_int32_t venc_chnid,vs_int32_t index,
    vs_venc_intra_area_s *p_intra);

/**
* @brief
* @details 	Set cyclic intra refresh attribute of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be set with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [in]  p_cir The pointer of struct vs_venc_cir_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not set if the channel is not create.
* - Must be called between channel created and destroyed, will affect from next frame when called during encoding.
* - Recommend call this before channel start, avoid call this during encoding if you can.
* - Recommend call this after vs_mal_venc_cir_get and only modify the properties you need.
* @par
*
*/
vs_int32_t vs_mal_venc_cir_set(vs_int32_t venc_chnid,
    const vs_venc_cir_s *p_cir);

/**
* @brief
* @details 	Get cyclic intra refresh attribute of an encode channel by channel number.
* @param [in]  venc_chnid Indicate the channel number to be got with. Value must be in [0,VENC_MAX_CHN_NUM).
* @param [out]  p_cir The pointer of struct vs_venc_cir_s, can not be NULL.
* @retval   0 call success.
* @retval   none0 call failed. see ERROR_CODE for detail.
*
*
* @note
* - Can not get if the channel is not create.
* - Must be called between channel created and destroyed.
* @par
*
*/
vs_int32_t vs_mal_venc_cir_get(vs_int32_t venc_chnid,
    vs_venc_cir_s *p_cir);


#ifdef __cplusplus
}
#endif
#endif /*!<  __VS_MAL_VENC_H__ */

