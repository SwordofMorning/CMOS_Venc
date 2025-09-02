/**
* @file vs_mal_vo.h
* @brief vo mal interface
* @details vo APIs
* @author VS vo group
* @date 2022-01-18
* @version v0.10
* @Copyright (C) 2022, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/

#ifndef __VS_MAL_VO_H__
#define __VS_MAL_VO_H__

#include "vs_vo_defines.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
* [API constraints 1]: (1) This API's param will not be taken effect until vs_mal_commit(),
*                          those param can be modified before vs_mal_commit();
*                      (2) vs_mal_commit() is used for ensuring all the parameters belong to different APIs to take effect at the same frame;
*                          the calling sequence may be api1(); api2(); ... apin(); vs_mal_commit();
*
*
* [API constraints 2]:     The API will take effect immediately, without calling any other API.
*
*
* [API constraints 3]: (1) This API will take effect immediately.
*                      (2) This API can also support batch processing by calling batch_begin() and batch_end(),
*                          the param will take effect after batch_end(),
*                          the calling sequence may be batch_begin();api1(); api2(); ... apin(); batch_end();
**/

/**
* @brief       enable vo device
* @details     the device should be enabled after device attributes set
* @param [in]  vo_devid, vo device: display0/display1 with value 0/1
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 1] for calling
* @par
*/
vs_int32_t vs_mal_vo_enable(vs_uint32_t vo_devid);

/**
* @brief       disable vo device
* @details     disable the device and init
* @param [in]  vo_devid, 0 or 1
* @return      0 indicates success
* @return      nonzero indicates failure, see ERROR_CODE for details
* @note        see [API constraints 1] for calling
* @par
*/
vs_int32_t vs_mal_vo_disable(vs_uint32_t vo_devid);

/**
* @brief       set the device attributes
* @details     set the device attributes before the device enable
* @param [in]  vo_devid, 0 or 1
* @param [in]  p_attr, device attributes to be set
* @return      0 indicates success
* @return      nonzero indicates failure, see ERROR_CODE for details
* @note        see [API constraints 1] for calling
* @par
*/
vs_int32_t vs_mal_vo_dev_attr_set(vs_int32_t vo_devid, const vs_vo_dev_attr_s *p_attr);

/**
* @brief       get the device attributes
* @details     get the device attributes
* @param [in]  vo_devid, 0 or 1
* @param [out] p_attr, device attributes
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 2] for calling
* @par
*/
vs_int32_t vs_mal_vo_dev_attr_get(vs_int32_t vo_devid, vs_vo_dev_attr_s *p_attr);

/**
* @brief       set the device early config time
* @details     set the device early config time after device attributes setting and before the device enable
* @param [in]  vo_devid, 0 or 1
* @param [in]  early_cfg_time, device early config time to be set which is in microsecond (us)
* @return      0 indicates success
* @return      nonzero indicates failure, see ERROR_CODE for details
* @note        see [API constraints 1] for calling
* @par
*/
vs_int32_t vs_mal_vo_dev_early_cfg_time_set(vs_int32_t vo_devid, vs_uint32_t early_cfg_time);

/**
* @brief       get the device early config time
* @details     get the device early config time
* @param [in]  vo_devid, 0 or 1
* @param [out] p_early_cfg_time, device early config time which is in microsecond (us)
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 2] for calling
* @par
*/
vs_int32_t vs_mal_vo_dev_early_cfg_time_get(vs_int32_t vo_devid, vs_uint32_t *p_early_cfg_time);

/**
* @brief       set the device policy
* @details     set the device policy after device attributes setting and before the device enable
* @param [in]  vo_devid, 0 or 1
* @param [in]  p_dev_policy, device policy to be set
* @return      0 indicates success
* @return      nonzero indicates failure, see ERROR_CODE for details
* @note        see [API constraints 1] for calling
* @par
*/
vs_int32_t vs_mal_vo_dev_policy_set(vs_int32_t vo_devid, const vs_vo_dev_policy_s *p_dev_policy);

/**
* @brief       get the device policy
* @details     get the device policy
* @param [in]  vo_devid, 0 or 1
* @param [out] p_dev_policy, device policy
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 2] for calling
* @par
*/
vs_int32_t vs_mal_vo_dev_policy_get(vs_int32_t vo_devid, vs_vo_dev_policy_s *p_dev_policy);

/**
* @brief       set the vo clk
* @details     TODO
* @param [in]  vo_devid, 0 or 1
* @param [in]  p_clk, TODO
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 2] for calling, this api should be called after dev_attr_set and before the dev_enable if needed.
* @par
*/
vs_int32_t vs_mal_vo_clk_set(vs_int32_t vo_devid, vs_vo_clk_info_s *p_clk);

/**
* @brief       set the output csc
* @details     set the output csc before the device enable
* @param [in]  vo_devid, 0 or 1
* @param [in]  output csc to be set
* @return      0 indicates success
* @return      nonzero indicates failure, see ERROR_CODE for details
* @note        see [API constraints 1] for calling
* @par
*/
vs_int32_t vs_mal_vo_output_csc_set(vs_int32_t vo_devid, const vs_vo_csc_s *p_output_csc);

/**
* @brief       get the output csc
* @details     get the output csc
* @param [in]  vo_devid, 0 or 1
* @param [in]  p_output_csc, output csc
* @return      0 indicates success
* @return      nonzero indicates failure, see ERROR_CODE for details
* @note        see [API constraints 1] for calling
* @par
*/
vs_int32_t vs_mal_vo_output_csc_get(vs_int32_t vo_devid, vs_vo_csc_s *p_output_csc);

/**
* @brief       enable the channel
* @details     channel attributes has been enabled after channel attributes set
* @param [in]  vo_layerid, indicate the layer which the channel belongs to
* @param [in]  vo_chnid, video channel id
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 1] for calling
* @par
*/
vs_int32_t vs_mal_vo_chn_enable(vs_int32_t vo_layerid, vs_int32_t vo_chnid);

/**
* @brief       disable the channel
* @details     disablt the channel
* @param [in]  vo_layerid, indicate the layer which the channel belongs to
* @param [in]  vo_chnid
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 1] for calling
* @par
*/
vs_int32_t vs_mal_vo_chn_disable(vs_int32_t vo_layerid, vs_int32_t vo_chnid);

/**
* @brief       set the channel attributes
* @details     channel attributes should be set after layer has been bind to the display
* @param [in]  vo_layerid, indicate the layer which the channel belongs to
* @param [in]  vo_chnid
* @param [in]  p_chn_attr, the channel attributes
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 1] for calling
* @par
*/
vs_int32_t vs_mal_vo_chn_attr_set(vs_int32_t vo_layerid, vs_int32_t vo_chnid, const vs_vo_chn_attr_s *p_chn_attr);

/**
* @brief       get the channel attributes
* @details     get the channel attributes
* @param [in]  vo_layerid, indicate the layer which the channel belongs to
* @param [in]  vo_chnid
* @param [out] p_chn_attr, the channel attributes
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 2] for calling
* @par
*/
vs_int32_t vs_mal_vo_chn_attr_get(vs_int32_t vo_layerid, vs_int32_t vo_chnid, vs_vo_chn_attr_s *p_chn_attr);

/**
* @brief       set the channel parameters
* @details     set the channel parameters
* @param [in]  vo_layerid, indicate the layer which the channel belongs to
* @param [in]  vo_chnid
* @param [in]  p_chn_param, the channel parameters
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 2] for calling
* @par
*/
vs_int32_t vs_mal_vo_chn_param_set(vs_int32_t vo_layerid, vs_int32_t vo_chnid, const vs_vo_chn_param_s *p_chn_param);

/**
* @brief       get the channel parameters
* @details     get the channel parameters
* @param [in]  vo_layerid, indicate the layer which the channel belongs to
* @param [in]  vo_chnid
* @param [out] p_chn_param, the channel parameters
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 2] for calling
* @par
*/
vs_int32_t vs_mal_vo_chn_param_get(vs_int32_t vo_layerid, vs_int32_t vo_chnid, vs_vo_chn_param_s *p_chn_param);

/**
* @brief       set the channel framerate
* @details     set the channel framerate range?
* @param [in]  vo_layerid, indicate the layer which the channel belongs to
* @param [in]  vo_chnid
* @param [in]  chn_framerate, the channel framerate
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 2] for calling
* @par
*/
vs_int32_t vs_mal_vo_chn_framerate_set(vs_int32_t vo_layerid, vs_int32_t vo_chnid, vs_int32_t chn_framerate);

/**
* @brief       get the channel parameters
* @details     get the channel parameters
* @param [in]  vo_layerid, indicate the layer which the channel belongs to
* @param [in]  vo_chnid
* @param [out] p_chn_param, the channel parameters
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 2] for calling
* @par
*/
vs_int32_t vs_mal_vo_chn_framerate_get(vs_int32_t vo_layerid, vs_int32_t vo_chnid, vs_int32_t *p_chn_framerate);

/**
* @brief       acquire the channel frame
* @details     acquire the channel frame to the file
* @param [in]  vo_layerid, indicate the layer which the channel belongs to
* @param [in]  vo_chnid
* @param [out] p_frame_info, acquire the channel frame info
* @param [in]  timeout_ms, indicate the time to acquire the frame, -1 means never timeout
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 2] for calling
* @par
*/
vs_int32_t vs_mal_vo_chn_frame_acquire(vs_int32_t vo_layerid, vs_int32_t vo_chnid, vs_video_frame_info_s *p_frame_info, vs_int32_t timeout_ms);

/**
* @brief       release the channel frame
* @details     release the channel frame
* @param [in]  vo_layerid, indicate the layer which the channel belongs to
* @param [in]  vo_chnid
* @param [in]  p_frame_info, the channel frame info to be release
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 2] for calling
* @par
*/
vs_int32_t vs_mal_vo_chn_frame_release(vs_int32_t vo_layerid, vs_int32_t vo_chnid, const vs_video_frame_info_s *p_frame_info);

/**
* @brief       pause the channel
* @details     the channel stops to receive chn buffer
* @param [in]  vo_layerid, indicate the layer which the channel belongs to
* @param [in]  vo_chnid
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 3] for calling
* @par
*/
vs_int32_t vs_mal_vo_chn_pause(vs_int32_t vo_layerid, vs_int32_t vo_chnid);

/**
* @brief       resume the channel
* @details     the channel starts to receive buffer
* @param [in]  vo_layerid, indicate the layer which the channel belongs to
* @param [in]  vo_chnid
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 2] for calling
* @par
*/
vs_int32_t vs_mal_vo_chn_resume(vs_int32_t vo_layerid, vs_int32_t vo_chnid);

/**
* @brief       step the channel
* @details     the channel stop to receive buffer and hold until the external cmd
* @param [in]  vo_layerid, indicate the layer which the channel belongs to
* @param [in]  vo_chnid
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 2] for calling
* @par
*/
vs_int32_t vs_mal_vo_chn_step(vs_int32_t vo_layerid, vs_int32_t vo_chnid);

/**
* @brief       refresh the channel
* @details     channel buffer is going to be refresh
* @param [in]  vo_layerid, indicate the layer which the channel belongs to
* @param [in]  vo_chnid
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 2] for calling
* @par
*/
vs_int32_t vs_mal_vo_chn_refresh(vs_int32_t vo_layerid, vs_int32_t vo_chnid);

/**
* @brief       show the channel
* @details     the channel start to show
* @param [in]  vo_layerid, indicate the layer which the channel belongs to
* @param [in]  vo_chnid
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 3] for calling
* @par
*/
vs_int32_t vs_mal_vo_chn_show(vs_int32_t vo_layerid, vs_int32_t vo_chnid);

/**
* @brief       show the channel
* @details     the channel start to hide, display background color
* @param [in]  vo_layerid, indicate the layer which the channel belongs to
* @param [in]  vo_chnid
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 3] for calling
* @par
*/
vs_int32_t vs_mal_vo_chn_hide(vs_int32_t vo_layerid, vs_int32_t vo_chnid);

/**
* @brief       set the channel zoomin attributes
* @details     channel zoomin attributes should be set after device and layer are both enable
* @param [in]  vo_layerid, indicate the layer which the channel belongs to
* @param [in]  vo_chnid
* @param [in]  p_zoom_attr, the zoomin attributes.
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 1] for calling
* @par
*/
vs_int32_t vs_mal_vo_chn_zoomin_set(vs_int32_t vo_layerid, vs_int32_t vo_chnid, const vs_vo_zoomin_attr_s * p_zoom_attr);

/**
* @brief       get the channel zoomin attributes
* @details     get the channel zoomin attributes
* @param [in]  vo_layerid, indicate the layer which the channel belongs to
* @param [in]  vo_chnid
* @param [out] p_zoom_attr, the zoomin attributes.
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 2] for calling

* @par
*/
vs_int32_t vs_mal_vo_chn_zoomin_get(vs_int32_t vo_layerid, vs_int32_t vo_chnid, vs_vo_zoomin_attr_s * p_zoom_attr);

/**
* @brief       get the channel pts
* @details     get the channel pts
* @param [in]  vo_layerid, indicate the layer which the channel belongs to
* @param [in]  vo_chnid
* @param [out] p_chn_pts
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 2] for calling
* @par
*/
vs_int32_t vs_mal_vo_chn_pts_get(vs_int32_t vo_layerid, vs_int32_t vo_chnid, vs_uint64_t *p_chn_pts);

/**
* @brief       get the channel status
* @details     get the channel status, including buf depth, unprocessed buf number
* @param [in]  vo_layerid, indicate the layer which the channel belongs to
* @param [in]  vo_chnid
* @param [out] p_chn_status, the status of channel
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 2] for calling
* @par
*/
vs_int32_t vs_mal_vo_chn_status_get(vs_int32_t vo_layerid, vs_int32_t vo_chnid, vs_vo_chn_status_s *p_chn_status);

/**
* @brief       set the channel buf depth
* @details     set the channel buf depth
* @param [in]  vo_layerid, indicate the layer which the channel belongs to
* @param [in]  vo_chnid
* @param [out] p_buf_depth, the num of channel buffer
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 2] for calling
* @par
*/
vs_int32_t vs_mal_vo_chn_buf_depth_set(vs_int32_t vo_layerid, vs_int32_t vo_chnid, vs_int32_t buf_depth);

/**
* @brief       get the channel buf depth
* @details     get the channel buf depth
* @param [in]  vo_layerid, indicate the layer which the channel belongs to
* @param [in]  vo_chnid
* @param [out] p_buf_depth, the num of channel buffer
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 2] for calling
* @par
*/
vs_int32_t vs_mal_vo_chn_buf_depth_get(vs_int32_t vo_layerid, vs_int32_t vo_chnid, vs_int32_t *p_buf_depth);

/**
* @brief       clear the channel buffer
* @details     clear the data in the channel buffer
* @param [in]  vo_layerid, indicate the layer which the channel belongs to
* @param [in]  vo_chnid
* @param [in]  clear_all, indicate whether to clear all the channel buffer
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 2] for calling
* @par
*/
vs_int32_t vs_mal_vo_chn_buf_clear(vs_int32_t vo_layerid, vs_int32_t vo_chnid, vs_bool_t clear_all);

/**
* @brief       send the frame to channel buffer
* @details     send the user frame to chanel buffer
* @param [in]  vo_layerid, indicate the layer which the channel belongs to
* @param [in]  vo_chnid
* @param [in]  p_frame_info, the frame info which should be sent to the channel
* @param [in]  timeout_ms, indicate the time to send the frame, -1 means never timeout
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 2] for calling
* @par
*/
vs_int32_t vs_mal_vo_chn_frame_send(vs_int32_t vo_layerid, vs_int32_t vo_chnid, vs_video_frame_info_s *p_frame_info, vs_int32_t timeout_ms);

/**
* @brief       set the channel border info
* @details     set the channel border info
* @param [in]  vo_layerid, indicate the layer which the channel belongs to
* @param [in]  vo_chnid
* @param [in]  p_chn_border, the channel border info
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 1] for calling
* @par
*/
vs_int32_t vs_mal_vo_chn_border_set(vs_int32_t vo_layerid, vs_int32_t vo_chnid, const vs_vo_chn_border_s *p_chn_border);

/**
* @brief       get the channel border info
* @details     get the channel border info
* @param [in]  vo_layerid, indicate the layer which the channel belongs to
* @param [in]  vo_chnid
* @param [out] p_chn_border, the channel border info
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 2] for calling
* @par
*/
vs_int32_t vs_mal_vo_chn_border_get(vs_int32_t vo_layerid, vs_int32_t vo_chnid, vs_vo_chn_border_s *p_chn_border);

/**
* @brief       set the channel rotation type
* @details     set the channel rotation type
* @param [in]  vo_layerid, indicate the layer which the channel belongs to
* @param [in]  vo_chnid
* @param [in]  rotation_type, the channel rotation type
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 1] for calling
* @par
*/
vs_int32_t vs_mal_vo_chn_rotation_set(vs_int32_t vo_layerid, vs_int32_t vo_chnid, vs_fixed_rotation_e rotation_type);

/**
* @brief       get the channel rotation type
* @details     get the channel rotation type
* @param [in]  vo_layerid, indicate the layer which the channel belongs to
* @param [in]  vo_chnid
* @param [out] rotation_type, the channel rotation type
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 2] for calling
* @par
*/
vs_int32_t vs_mal_vo_chn_rotation_get(vs_int32_t vo_layerid, vs_int32_t vo_chnid, vs_fixed_rotation_e *p_rotation_type);

/**
* @brief       set the channel mirror type
* @details     set the channel mirror type
* @param [in]  vo_layerid, indicate the layer which the channel belongs to
* @param [in]  vo_chnid
* @param [in]  mirror_type, the channel mirror type
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 1] for calling
* @par
*/
vs_int32_t vs_mal_vo_chn_mirror_set(vs_int32_t vo_layerid, vs_int32_t vo_chnid, vs_mirror_mode_e mirror_type);

/**
* @brief       get the channel mirror type
* @details     get the channel mirror type
* @param [in]  vo_layerid, indicate the layer which the channel belongs to
* @param [in]  vo_chnid
* @param [out] mirror_type, the channel mirror type
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 2] for calling
* @par
*/
vs_int32_t vs_mal_vo_chn_mirror_get(vs_int32_t vo_layerid, vs_int32_t vo_chnid, vs_mirror_mode_e *p_mirror_type);

/**
* @brief       get the channel luma TODO
* @details     get the channel luma
* @param [in]  vo_layerid, indicate the layer which the channel belongs to
* @param [in]  vo_chnid
* @param [in]  p_region_info, the channel region info
* @param [out] p_luma_data, the channel luma data
* @param [in]  timeout_ms, indicate the time to get the luma data, -1 means never timeout
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 2] for calling
* @par
*/
vs_int32_t vs_mal_vo_chn_luma_get(vs_int32_t vo_layerid, vs_int32_t vo_chnid, vs_vo_region_info_s *p_region_info, vs_uint64_t *p_luma_data, vs_int32_t timeout_ms);

/**
* @brief       set the layer attributes
* @details     set the layer attributes before the layer enable
* @param [in]  vo_layerid, [0,5]
* @param [in]  p_layer_attr, layer attributes to be set
* @return      0 indicates success
* @return      nonzero indicates failure, see ERROR_CODE for details
* @note        see [API constraints 1] for calling
* @par
*/
vs_int32_t vs_mal_vo_video_layer_attr_set(vs_int32_t vo_layerid, const vs_vo_video_layer_attr_s *p_layer_attr);

/**
* @brief       get the layer attributes
* @details     get the layer attributes before the layer enable
* @param [in]  vo_layerid, [0,5]
* @param [out] p_layer_attr, layer attributes to be get
* @return      0 indicates success
* @return      nonzero indicates failure, see ERROR_CODE for details
* @note        see [API constraints 2] for calling
* @par
*/
vs_int32_t vs_mal_vo_video_layer_attr_get(vs_int32_t vo_layerid, vs_vo_video_layer_attr_s *p_layer_attr);

/**
* @brief       enable the layer attributes
* @details     layer should be enabled after the layer attributes set
* @param [in]  vo_layerid, [0,5]
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 1] for calling
* @par
*/
vs_int32_t vs_mal_vo_video_layer_enable(vs_int32_t vo_layerid);

/**
* @brief       disable the layer
* @details     disable the layer
* @param [in]  vo_layerid, [0,5]
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 1] for calling
* @par
*/
vs_int32_t vs_mal_vo_video_layer_disable(vs_int32_t vo_layerid);

/**
* @brief       bind the layer to device
* @details     layer should be binded to the device when the layer is disable
* @param [in]  vo_layerid, [0,5]
* @param [in]  vo_devid, [0,1]
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 1] for calling
* @par
*/
vs_int32_t vs_mal_vo_video_layer_bind(vs_int32_t vo_layerid, vs_uint32_t vo_devid);

/**
* @brief       unbind the layer to device
* @details     layer should be unbinded to the device when the layer is disable
* @param [in]  vo_layerid, [0,5]
* @param [in]  vo_devid, [0,1]
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 1] for calling
* @par
*/
vs_int32_t vs_mal_vo_video_layer_unbind(vs_int32_t vo_layerid, vs_uint32_t vo_devid);

/**
* @brief       set layer zorder
* @details     set layer zorder
* @param [in]  vo_layerid, [0,5]
* @param [in]  zorder, [0,5]
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 1] for calling
* @par
*/
vs_int32_t vs_mal_vo_video_layer_zorder_set(vs_int32_t vo_layerid, vs_uint32_t zorder);

/**
* @brief       get layer zorder
* @details     get layer zorder
* @param [in]  vo_layerid, [0,5]
* @param [out] p_zorder, point of the layer zorder
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 2] for calling
* @par
*/
vs_int32_t vs_mal_vo_video_layer_zorder_get(vs_int32_t vo_layerid, vs_uint32_t *p_zorder);

/**
* @brief       set layer csc
* @details     set layer csc
* @param [in]  vo_layerid, [0,5]
* @param [in]  p_video_csc,the video layer csc info
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 1] for calling
* @par
*/
vs_int32_t vs_mal_vo_video_layer_csc_set(vs_int32_t vo_layerid, const vs_vo_csc_s *p_video_csc);

/**
* @brief       get video layer csc
* @details     get video layer csc
* @param [in]  vo_layerid, [0,5]
* @param [oun] p_csc,the video layer csc info
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 2] for calling
* @par
*/
vs_int32_t vs_mal_vo_video_layer_csc_get(vs_uint32_t vo_layerid, vs_vo_csc_s *p_csc);

/**
* @brief       batch begin
* @details     start to set batch , wait the end
* @param [in]  vo_layerid, [0,5]
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 2] for calling
* @par
*/
vs_int32_t vs_mal_vo_batch_begin(vs_int32_t vo_layerid);

/**
* @brief       batch end
* @details     end to set batch , and all the configuration take effect
* @param [in]  vo_layerid, [0,5]
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 2] for calling
* @par
*/
vs_int32_t vs_mal_vo_batch_end(vs_int32_t vo_layerid);

/**
* @brief       set layer aspect ratio
* @details     set layer aspect ratio
* @param [in]  vo_layerid, [0,5]
* @param [in]  p_aspect_ratio, the layer aspect ratio info,
*              layer.bg_color is not supported, will be substituted by dev.bg_color.
* @return      0 indicates success
* @return      none zero indicates failure, see ERROR_CODE for details
* @note        see [API constraints 1] for calling
* @par
*/
vs_int32_t vs_mal_vo_video_layer_aspect_ratio_set(vs_int32_t vo_layerid, const vs_aspect_ratio_s *p_aspect_ratio);

/**
* @brief       get layer aspect ratio
* @details     get layer aspect ratio
* @param [in]  vo_layerid, [0,5]
* @param [out] p_aspect_ratio, the layer aspect ratio info
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 2] for calling
* @par
*/
vs_int32_t vs_mal_vo_video_layer_aspect_ratio_get(vs_int32_t vo_layerid, vs_aspect_ratio_s *p_aspect_ratio);

/**
* @brief       set layer crop
* @details     set layer crop
* @param [in]  vo_layerid, [0,5]
* @param [in]  p_crop_info, the layer crop info
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 1] for calling
* @par
*/
vs_int32_t vs_mal_vo_video_layer_crop_set(vs_int32_t vo_layerid, const vs_crop_s *p_crop_info);

/**
* @brief       get layer crop
* @details     get layer crop
* @param [in]  vo_layerid, [0,5]
* @param [out] p_crop_info, the layer crop info
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 2] for calling
* @par
*/
vs_int32_t vs_mal_vo_video_layer_crop_get(vs_int32_t vo_layerid, vs_crop_s *p_crop_info);

/**
* @brief       set the layer buffer depth
* @details     set the layer buffer depth before layer enabled
* @param [in]  vo_layerid, [0,5]
* @param [in]  buf_depth, layer buf depth to be set
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 2] for calling
* @par
*/
vs_int32_t vs_mal_vo_video_layer_buf_depth_set(vs_int32_t vo_layerid, vs_uint32_t buf_depth);

/**
* @brief       get the layer buffer depth
* @details     get the layer buffer depth after device open
* @param [in]  vo_layerid, [0,5]
* @param [out] *buf_depth, layer buf depth to be get
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 2] for calling
* @par
*/
vs_int32_t vs_mal_vo_video_layer_buf_depth_get(vs_int32_t vo_layerid, vs_uint32_t *p_buf_depth);

/**
* @brief       acquire the layer frame
* @details     acquire the layer frame to the file
* @param [in]  vo_layerid, indicate the layer which the channel belongs to
* @param [out] p_layer_frame, acquire the layer frame info
* @param [in]  timeout_ms, indicate the time to acquire the frame, -1 means never timeout
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 2] for calling
* @par
*/
vs_int32_t vs_mal_vo_layer_frame_acquire(vs_int32_t vo_layerid, vs_video_frame_info_s *p_layer_frame, vs_int32_t timeout_ms);

/**
* @brief       release the layer frame
* @details     release the layer frame to the file
* @param [in]  vo_layerid, indicate the layer which the channel belongs to
* @param [in]  p_layer_frame, acquire the layer frame info
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 2] for calling
* @par
*/
vs_int32_t vs_mal_vo_layer_frame_release(vs_int32_t vo_layerid, const vs_video_frame_info_s *p_layer_frame);

/**
* @brief The video layer APIs can be reused for graphic layer
*/

/**
* @brief       bind the graphic layer to device
* @details     graphic layer should be binded to the device when the layer is disable
* @param [in]  vo_layerid, [0,5]
* @param [in]  vo_devid, [0,1]
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 1] for calling
* @par
*/
vs_int32_t vs_mal_vo_graphic_layer_bind(vs_uint32_t vo_layerid, vs_uint32_t vo_devid);

/**
* @brief       unbind the graphic layer to device
* @details     graphic layer should be unbinded to the device when the layer is disable
* @param [in]  vo_layerid, [0,5]
* @param [in]  vo_devid, [0,1]
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 1] for calling
* @par
*/
vs_int32_t vs_mal_vo_graphic_layer_unbind(vs_uint32_t vo_layerid, vs_uint32_t vo_devid);

/**
* @brief       set graphic layer csc
* @details     set graphic layer csc
* @param [in]  vo_layerid, [0,5]
* @param [in]  p_csc,the graphic layer csc info
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 1] for calling
* @par
*/
vs_int32_t vs_mal_vo_graphic_layer_csc_set(vs_uint32_t vo_layerid, const vs_vo_csc_s *p_csc);

/**
* @brief       get graphic layer csc
* @details     get graphic layer csc
* @param [in]  vo_layerid, [0,5]
* @param [oun] p_csc,the graphic layer csc info
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 2] for calling
* @par
*/
vs_int32_t vs_mal_vo_graphic_layer_csc_get(vs_uint32_t vo_layerid, vs_vo_csc_s *p_csc);

/**
* @brief       set write back channel source
* @details     set write back channel source
* @param [in]  vo_wbcid, [0,1]
* @param [in]  wbc_source,the write back channel source
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 1] for calling
* @par
*/
vs_int32_t vs_mal_vo_wbc_source_set(vs_int32_t vo_wbcid, vs_vo_wbc_source_type_e wbc_source);

/**
* @brief       get write back channel source
* @details     get write back channel source
* @param [in]  vo_wbcid, [0,1]
* @param [out] p_wbc_source,the write back channel source
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 2] for calling
* @par
*/
vs_int32_t vs_mal_vo_wbc_source_get(vs_int32_t vo_wbcid, vs_vo_wbc_source_type_e *p_wbc_source);

/**
* @brief       set write back attr
* @details     set write back attr
* @param [in]  vo_wbcid, [0,1]
* @param [in]  p_wb_attr,the write back channel attr
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 1] for calling
* @par
*/
vs_int32_t vs_mal_vo_wbc_attr_set(vs_int32_t vo_wbcid, const vs_vo_wbc_attr_s *p_wb_attr);

/**
* @brief       get write back attr
* @details     get write back attr
* @param [in]  vo_wbcid, [0,1]
* @param [out] p_wb_attr,the write back channel attr
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 2] for calling
* @par
*/
vs_int32_t vs_mal_vo_wbc_attr_get(vs_int32_t vo_wbcid, vs_vo_wbc_attr_s *p_wbc_attr);

/**
* @brief       enable write back channel
* @details     write back channel should be enabled after attributes set
* @param [in]  vo_wbcid, [0,1]
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 1] for calling
* @par
*/
vs_int32_t vs_mal_vo_wbc_enable(vs_int32_t vo_wbcid);

/**
* @brief       disable write back channel
* @details     disable write back channel
* @param [in]  vo_wbcid, [0,1]
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 1] for calling
* @par
*/
vs_int32_t vs_mal_vo_wbc_disable(vs_int32_t vo_wbcid);

/**
* @brief       set write back attr mode
* @details     set write back attr mode
* @param [in]  vo_wbcid, [0,1]
* @param [in]  mode,the write back channel mode
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 1] for calling
* @par
*/
vs_int32_t vs_mal_vo_wbc_mode_set(vs_int32_t vo_wbcid, vs_vo_wbc_mode_e mode);

/**
* @brief       get write back attr mode
* @details     get write back attr source
* @param [in]  vo_wbcid, [0,1]
* @param [out] p_wb_attr,the write back channel attr
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 2] for calling
* @par
*/
vs_int32_t vs_mal_vo_wbc_mode_get(vs_int32_t vo_wbcid, vs_vo_wbc_mode_e *p_mode);

/**
* @brief       set write back channel buffer depth
* @details     set write back channel depth
* @param [in]  vo_wbcid, [0,1]
* @param [in]  buf_depth,the write back channel depth
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 2] for calling
* @par
*/
vs_int32_t vs_mal_vo_wbc_buf_depth_set(vs_int32_t vo_wbcid, vs_uint32_t buf_depth);

/**
* @brief       get write back channel buffer depth
* @details     get write back channel buffer depth
* @param [in]  vo_wbcid, [0,1]
* @param [out] p_buf_depth,the write back channel depth
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 2] for calling
* @par
*/
vs_int32_t vs_mal_vo_wbc_buf_depth_get(vs_int32_t vo_wbcid, vs_uint32_t *p_buf_depth);

/**
* @brief       acquire the write back channel frame
* @details     acquire the write back channel frame
* @param [in]  vo_wbcid, [0,1]
* @param [out] p_frame_info, acquire the write back channel frame info
* @param [in]  timeout_ms, indicate the time to acquire the frame, -1 means never timeout
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 2] for calling
* @par
*/
vs_int32_t vs_mal_vo_wbc_frame_acquire(vs_int32_t vo_wbcid, vs_video_frame_info_s *p_frame_info, vs_int32_t timeout_ms);

/**
* @brief       release the write back channel frame
* @details     release the write back channel frame
* @param [in]  vo_wbcid, [0,1]
* @param [in]  p_frame_info, indicate the write back channel frame to be released
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @note        see [API constraints 2] for calling
* @par
*/
vs_int32_t vs_mal_vo_wbc_frame_release(vs_int32_t vo_wbcid, const vs_video_frame_info_s *p_frame_info);

/**
* @brief this api can ensure all configurations take effect in the same frame, use vo_commit after all the operations for each frame.
*/

/**
* @brief       commit all
* @details     commit the layer and the channel config which bind to the device
* @param [in]  vo_devid, [0,1]
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @par
*/
vs_int32_t vs_mal_vo_commit(vs_uint32_t vo_devid);

/**
* @brief       Close the device fd of the VO module
* @details     Close the device fd of the VO module
* @param [in]  void
* @return      0 call success
* @return      none 0 call failed, see ERROR_CODE for detail
* @par
*/
vs_int32_t vs_mal_vo_fd_close(vs_void_t);

#ifdef __cplusplus
}
#endif

#endif ///< end of __VS_MAL_VO_H__

