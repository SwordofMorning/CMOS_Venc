
/**
* @file vs_mal_audio.h
* @brief Declaration of audio enumeration, struct and API.
* @details Declaration of audio enumeration, struct and API.
* @author VS software group
* @date 2021-11-02
* @version v0.01
* @copyright (C) 2021, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/

#ifndef __VS_MAL_AUDIO_H__
#define __VS_MAL_AUDIO_H__

/*****************************************************************************
 *                    Includes
 *****************************************************************************/
#include "vs_audio_defines.h"

#ifdef __cplusplus
extern "C" {
#endif


/*****************************************************************************
 *                    Audio API Prototypes
 *****************************************************************************/

/**
* @ingroup      vs_mal_audio
* @brief        set the parameters of the audio module
* @details
* @param [in]   p_setting       the parameters of the audio module
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_audio_mod_param_set( const vs_audio_mod_param_s *p_setting );

/**
* @ingroup      vs_mal_audio
* @brief        get the parameters of the audio module
* @details
* @param [out]  p_setting       the pointer of the parameters of the audio module
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_audio_mod_param_get(vs_audio_mod_param_s *p_setting);


/* AIN API Prototype */

/**
* @ingroup      vs_mal_audio
* @brief        set the attribute of the specified audio input device
* @details
* @param [in]   ai_devid        audio input device id
* @param [in]   p_attr          the pointer of the ain attribute to set
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ain_attr_set( vs_int32_t ai_devid, const vs_aiodev_attr_s *p_attr);

/**
* @ingroup      vs_mal_audio
* @brief        get the attribute of the specified audio input device
* @details
* @param [in]   ai_devid        audio input device id
* @param [out]  p_attr          the pointer of the ain attribute to save
* @retval       VS_SUCCESS      no error
* @retval       VS_FAILED       the attribute has never been set
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ain_attr_get( vs_int32_t ai_devid, vs_aiodev_attr_s *p_attr);

/**
* @ingroup      vs_mal_audio
* @brief        clear the attribute of the specified audio input device
* @details
* @param [in]   ai_devid        audio input device id
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ain_attr_clr( vs_int32_t ai_devid);

/**
* @ingroup      vs_mal_audio
* @brief        enable the specified audio input device
* @details
* @param [in]   ai_devid        audio input device id
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note         1. the device attribute must be set before calling this function
*               2. return success directly if it has been enabled
* @par
*
*/
vs_int32_t vs_mal_ain_enable( vs_int32_t ai_devid);

/**
* @ingroup      vs_mal_audio
* @brief        disable the specified audio input device
* @details
* @param [in]   ai_devid        audio input device id
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note         1. all the channels of this device must be destroyed before calling it, meanwhile all related
*                  AENC and AO who uses the this device's data should be closed too.
*               2. return success directly if it has been disabled
* @par
*
*/
vs_int32_t vs_mal_ain_disable( vs_int32_t ai_devid);

/**
* @ingroup      vs_mal_audio
* @brief        create the audio input channel
* @details
* @param [in]   ai_devid        audio input device id
* @param [in]   ai_chnid        audio input channel id
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note         1. make sure the input device has been enabled before calling it, or error happens.
* @par
*
*/
vs_int32_t vs_mal_ain_chn_enable( vs_int32_t ai_devid, vs_int32_t ai_chnid );

/**
* @ingroup      vs_mal_audio
* @brief        destroy the audio input channel
* @details
* @param [in]   ai_devid        audio input device id
* @param [in]   ai_chnid        audio input channel id
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ain_chn_disable( vs_int32_t ai_devid, vs_int32_t ai_chnid );

/**
* @ingroup      vs_mal_audio
* @brief        set the parameters of the audio input channel
* @details
* @param [in]   ai_devid        audio input device id
* @param [in]   ai_chnid        audio input channel id
* @param [in]   p_attr          point to the attribute to set to audio input channel
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ain_chn_param_set(vs_int32_t ai_devid, vs_int32_t ai_chnid, const vs_ain_chn_param_s *p_attr);

/**
* @ingroup      vs_mal_audio
* @brief        get the parameters of the audio input channel
* @details
* @param [in]   ai_devid        audio input device id
* @param [in]   ai_chnid        audio input channel id
* @param [out]  p_attr          point to the attribute reading from audio input channel
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ain_chn_param_get(vs_int32_t ai_devid, vs_int32_t ai_chnid, vs_ain_chn_param_s *p_attr);

/**
* @ingroup      vs_mal_audio
* @brief        get the audio frames including reference aec frame if needed
* @details
* @param [in]   ai_devid        audio input device id
* @param [in]   ai_chnid        audio input channel id
* @param [in]   timeout_ms      timeout to acquire ain output frame
*                               >0: error returns if the frame can't been acquired within timeout_ms
*                               =0: return directly, error if no data, or success
*                               -1: block mode until the frame is available
* @param [out]  p_main_frame    audio input frame pointer
* @param [out]  p_aec_frame     audio aec frame pointer
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ain_frame_acquire(vs_int32_t ai_devid, vs_int32_t ai_chnid, vs_audio_frame_s *p_main_frame, vs_audio_aecframe_s *p_aec_frame, vs_int32_t timeout_ms);

/**
* @ingroup      vs_mal_audio
* @brief        release the audio frame resource
* @details
* @param [in]   ai_devid        audio input device id
* @param [in]   ai_chnid        audio input channel id
* @param [in]   p_main_frame    audio input frame pointer
* @param [in]   p_aec_frame     audio aec frame pointer
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ain_frame_release(vs_int32_t ai_devid, vs_int32_t ai_chnid, const vs_audio_frame_s *p_main_frame, const vs_audio_aecframe_s *p_aec_frame);

/**
* @ingroup      vs_mal_audio
* @brief        enable the audio resample feature
* @details
* @param [in]   ai_devid        audio input device id
* @param [in]   ai_chnid        audio input channel id
* @param [in]   out_sample_rate the output sample rate
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ain_resample_enable(vs_int32_t ai_devid, vs_int32_t ai_chnid, vs_audio_sample_rate_e out_sample_rate);

/**
* @ingroup      vs_mal_audio
* @brief        disable the audio resample feature
* @details
* @param [in]   ai_devid        audio input device id
* @param [in]   ai_chnid        audio input channel id
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ain_resample_disable(vs_int32_t ai_devid, vs_int32_t ai_chnid);

/**
* @ingroup      vs_mal_audio
* @brief        set the audio input dualmono mode
* @details
* @param [in]   ai_devid        audio input device id
* @param [in]   dualmode        the dualmono mode
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ain_dualmode_set(vs_int32_t ai_devid, vs_audio_dualmode_e dualmode);

/**
* @ingroup      vs_mal_audio
* @brief        get the audio input dualmono mode
* @details
* @param [in]   ai_devid        audio input device id
* @param [out]  p_dualmode      the dualmono mode pointer to save the getten value
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ain_dualmode_get(vs_int32_t ai_devid, vs_audio_dualmode_e *p_dualmode);

/**
* @ingroup      vs_mal_audio
* @brief        save the audio output frames from the audio channel of the audio input device
* @details
* @param [in]   ai_devid        audio input device id
* @param [in]   ai_chnid        audio input channel id
* @param [in]   p_file          the pointer of the audio dumpfile handler
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ain_dumpfile_save(vs_int32_t ai_devid, vs_int32_t ai_chnid, const vs_audio_dumpfile_s *p_file);

/**
* @ingroup      vs_mal_audio
* @brief        get the file dump status from the audio channel of the audio input device
* @details
* @param [in]   ai_devid        audio input device id
* @param [in]   ai_chnid        audio input channel id
* @param [out]  p_status        the pointer to store the dumpfile status
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ain_dumpfile_status_get(vs_int32_t ai_devid, vs_int32_t ai_chnid, vs_audio_dumpfile_status_s *p_status);

/**
* @ingroup      vs_mal_audio
* @brief        get the file handle of the audio channel of the audio input device
* @details
* @param [in]   ai_devid        audio input device id
* @param [in]   ai_chnid        audio input channel id
* @retval       fd              the file handle of the audio channel of the audio input device
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ain_fd_get(vs_int32_t ai_devid, vs_int32_t ai_chnid );

/**
* @ingroup      vs_mal_audio
* @brief        enable the acquisition of the aec frame
*               even the AEC feature is not enabled
* @details
* @param [in]   ai_devid        audio input device id
* @param [in]   ai_chnid        audio input channel id
* @param [in]   ao_devid        audio output device id
* @param [in]   ao_chnid        audio output channel id
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note         1. This API is used by the user to apply AEC on his/her application
*               2. This API can only be used when AEC feature is disable, or error returns.
* @par
*
*/
vs_int32_t vs_mal_ain_ref_aecframe_enable(vs_int32_t ai_devid, vs_int32_t ai_chnid, vs_int32_t ao_devid, vs_int32_t ao_chnid);

/**
* @ingroup      vs_mal_audio
* @brief        disable the acquisition of the aec frame when
*               the AEC feature is not enabled
* @details
* @param [in]   ai_devid        audio input device id
* @param [in]   ai_chnid        audio input channel id
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ain_ref_aecframe_disable(vs_int32_t ai_devid, vs_int32_t ai_chnid);

/**
* @ingroup      vs_mal_audio
* @brief        enable the aqe pipe in the audio input channel
* @details
* @param [in]   ai_devid        audio input device id
* @param [in]   ai_chnid        audio input channel id
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note         return error if the aqe pipe is not hooked in the audio channel
* @par
*
*/
vs_int32_t vs_mal_ain_aqe_enable(vs_int32_t ai_devid, vs_int32_t ai_chnid);

/**
* @ingroup      vs_mal_audio
* @brief        disable the aqe pipe in the audio input channel
* @details
* @param [in]   ai_devid        audio input device id
* @param [in]   ai_chnid        audio input channel id
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note         return error if the aqe pipe is not hooked in the audio channel
* @par
*
*/
vs_int32_t vs_mal_ain_aqe_disable(vs_int32_t ai_devid, vs_int32_t ai_chnid);

/**
* @ingroup      vs_mal_audio
* @brief        set the attribute of the record aqe pipe in the audio input channel
* @details
* @param [in]   ai_devid        audio input device id
* @param [in]   ai_chnid        audio input channel id
* @param [in]   p_setting       the attribute of the record aqe pipe
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note         return error if the aqe pipeline is not hooked in the audio channel
* @par
*
*/
vs_int32_t vs_mal_ain_record_aqe_attr_set(vs_int32_t ai_devid, vs_int32_t ai_chnid, const vs_ain_record_aqe_cfg_s *p_setting);

/**
* @ingroup      vs_mal_audio
* @brief        get the attribute of the record aqe pipe in the audio input channel
* @details
* @param [in]   ai_devid        audio input device id
* @param [in]   ai_chnid        audio input channel id
* @param [out]  p_setting       the attribute of the record aqe pipe
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note         return error if the aqe pipeline is not hooked in the audio channel
* @par
*
*/
vs_int32_t vs_mal_ain_record_aqe_attr_get(vs_int32_t ai_devid, vs_int32_t ai_chnid, vs_ain_record_aqe_cfg_s *p_setting);

/**
* @ingroup      vs_mal_audio
* @brief        set the attribute of the talk aqe pipe in the audio input channel
* @details
* @param [in]   ai_devid        audio input device id
* @param [in]   ai_chnid        audio input channel id
* @param [in]   p_setting       the attribute of the talk aqe pipe
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note         return error if the aqe pipeline is not hooked in the audio channel
* @par
*
*/

vs_int32_t vs_mal_ain_talk_aqe_attr_set(vs_int32_t ai_devid, vs_int32_t ai_chnid, vs_int32_t ao_devid, vs_int32_t ao_chnid, const vs_ain_talk_aqe_cfg_s *p_setting);

/**
* @ingroup      vs_mal_audio
* @brief        get the attribute of the talk aqe pipe in the audio input channel
* @details
* @param [in]   ai_devid        audio input device id
* @param [in]   ai_chnid        audio input channel id
* @param [out]  p_setting       the attribute of the talk aqe pipe
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note         return error if the aqe pipeline is not hooked in the audio channel
* @par
*
*/
vs_int32_t vs_mal_ain_talk_aqe_attr_get(vs_int32_t ai_devid, vs_int32_t ai_chnid, vs_ain_talk_aqe_cfg_s *p_setting);




/* AOUT API Prototype */

/**
* @ingroup      vs_mal_audio
* @brief        set the attribute of the specified audio output device
* @details
* @param [in]   ao_devid        audio output device id
* @param [in]   p_attr          the pointer of the aout attribute to set
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_aout_attr_set(vs_int32_t ao_devid, const vs_aiodev_attr_s *p_attr);

/**
* @ingroup      vs_mal_audio
* @brief        get the attribute of the specified audio output device
* @details
* @param [in]   ao_devid        audio output device id
* @param [out]  p_attr          the pointer of the aout attribute to save
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_aout_attr_get(vs_int32_t ao_devid, vs_aiodev_attr_s *p_attr);

/**
* @ingroup      vs_mal_audio
* @brief        clear the attribute of the specified audio output device
* @details
* @param [in]   ao_devid        audio output device id
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_aout_attr_clr(vs_int32_t ao_devid);

/**
* @ingroup      vs_mal_audio
* @brief        enable the specified audio output device
* @details
* @param [in]   ao_devid        audio output device id
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note         1. the device attribute must be set before calling this function
*               2. return success directly if it has been enabled
* @par
*
*/
vs_int32_t vs_mal_aout_enable(vs_int32_t ao_devid);

/**
* @ingroup      vs_mal_audio
* @brief        disable the specified audio output device
* @details
* @param [in]   ao_devid        audio output device id
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note         1. all the channels of this device must be destroyed before calling it
*               2. return success directly if it has been disabled
* @par
*
*/
vs_int32_t vs_mal_aout_disable(vs_int32_t ao_devid);

/**
* @ingroup      vs_mal_audio
* @brief        enable the audio output channel
* @details
* @param [in]   ao_devid        audio output device id
* @param [in]   ao_chnid        audio output channel id
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note         1. make sure the output device has been enabled before calling it, or error happens.
* @par
*
*/
vs_int32_t vs_mal_aout_chn_enable(vs_int32_t ao_devid, vs_int32_t ao_chnid);

/**
* @ingroup      vs_mal_audio
* @brief        disable the audio output channel
* @details
* @param [in]   ao_devid        audio output device id
* @param [in]   ao_chnid        audio output channel id
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note         1. make sure the output device has been enabled before calling it, or error happens.
* @par
*
*/
vs_int32_t vs_mal_aout_chn_disable(vs_int32_t ao_devid, vs_int32_t ao_chnid);

/**
* @ingroup      vs_mal_audio
* @brief        send audio output PCM frame to the audio output channel for playback
* @details
* @param [in]   ao_devid        audio output device id
* @param [in]   ao_chnid        audio output channel id
* @param [in]   p_frame         audio output frame pointer
* @param [in]   timeout_ms      timeout to send the audio output frame
*                               >0: error returns if the frame can't been submitted within timeout_ms
*                               =0: return directly, error if can't be submitted, or success
*                               -1: block mode until the frame has been submitted
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_aout_frame_send ( vs_int32_t ao_devid, vs_int32_t ao_chnid, const vs_audio_frame_s *p_frame,
                                    vs_int32_t timeout_ms );

/**
* @ingroup      vs_mal_audio
* @brief        enable the audio resample feature in the audio output channel
* @details
* @param [in]   ao_devid        audio output device id
* @param [in]   ao_chnid        audio output channel id
* @param [in]   in_sample_rate  the input sample rate
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_aout_resample_enable ( vs_int32_t ao_devid, vs_int32_t ao_chnid,
        vs_audio_sample_rate_e in_sample_rate );

/**
* @ingroup      vs_mal_audio
* @brief        disable the audio resample feature in the audio output channel
* @details
* @param [in]   ao_devid        audio output device id
* @param [in]   ao_chnid        audio output channel id
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_aout_resample_disable(vs_int32_t ao_devid, vs_int32_t ao_chnid);

/**
* @ingroup      vs_mal_audio
* @brief        clear the data in the audio buffer of the audio output channel
* @details
* @param [in]   ao_devid        audio output device id
* @param [in]   ao_chnid        audio output channel id
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_aout_chn_buf_clear(vs_int32_t ao_devid, vs_int32_t ao_chnid);

/**
* @ingroup      vs_mal_audio
* @brief        get the data status in the audio buffer of the audio output channel
* @details
* @param [in]   ao_devid        audio output device id
* @param [in]   ao_chnid        audio output channel id
* @param [out]  p_status        the data status pointer
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_aout_chn_status_get(vs_int32_t ao_devid, vs_int32_t ao_chnid, vs_aout_chn_status_s *p_status);

/**
* @ingroup      vs_mal_audio
* @brief        pause the audio output channel
* @details
* @param [in]   ao_devid        audio output device id
* @param [in]   ao_chnid        audio output channel id
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note         1. if the aout channel has been paused, the data from ADEC will be blocked, and the data from AI will
*                  be filled to the AO buffer until it is full, or dropped if it is full.
* @par
*
*/
vs_int32_t vs_mal_aout_chn_pause(vs_int32_t ao_devid, vs_int32_t ao_chnid);

/**
* @ingroup      vs_mal_audio
* @brief        resume the audio output channel
* @details
* @param [in]   ao_devid        audio output device id
* @param [in]   ao_chnid        audio output channel id
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_aout_chn_resume(vs_int32_t ao_devid, vs_int32_t ao_chnid);


/**
* @ingroup      vs_mal_audio
* @brief        set the volume of the audio output channel
* @details
* @param [in]   ao_devid        audio output device id
* @param [in]   volume_db       the output volume
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_aout_volume_set(vs_int32_t ao_devid, vs_int32_t volume_db);

/**
* @ingroup      vs_mal_audio
* @brief        get the volume of the audio output channel
* @details
* @param [in]   ao_devid        audio output device id
* @param [out]  p_volume_db     the output volume pointer
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_aout_volume_get(vs_int32_t ao_devid, vs_int32_t *p_volume_db);

/**
* @ingroup      vs_mal_audio
* @brief        mute the output of the audio output channel
* @details
* @param [in]   ao_devid        audio output device id
* @param [in]   enable          VS_FALSE: mute disable, VS_TRUE mute enable
* @param [in]   p_fade          fade-in and fade-out pointer
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_aout_mute_set(vs_int32_t ao_devid, vs_bool_t enable, const vs_audio_fade_s *p_fade);

/**
* @ingroup      vs_mal_audio
* @brief        mute the output of the audio output channel
* @details
* @param [in]   ao_devid        audio output device id
* @param [out]  p_enable        mute status pointer
* @param [out]  p_fade          fade-in and fade-out pointer
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_aout_mute_get(vs_int32_t ao_devid, vs_bool_t *p_enable, vs_audio_fade_s *p_fade);

/**
* @ingroup      vs_mal_audio
* @brief        set the audio output dualmono mode
* @details
* @param [in]   ao_devid        audio output device id
* @param [in]   dualmode        the dualmono mode
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_aout_dualmode_set(vs_int32_t ao_devid, vs_audio_dualmode_e dualmode);

/**
* @ingroup      vs_mal_audio
* @brief        get the audio output dualmono mode
* @details
* @param [in]   ao_devid        audio output device id
* @param [out]  p_dualmode      the dualmono mode pointer
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_aout_dualmode_get(vs_int32_t ao_devid, vs_audio_dualmode_e *p_dualmode);

/**
* @ingroup      vs_mal_audio
* @brief        get the file handle of the audio channel of the audio output device
* @details
* @param [in]   ao_devid        audio output device id
* @param [in]   ao_chnid        audio output channel id
* @retval       file_handle
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_aout_fd_get(vs_int32_t ao_devid, vs_int32_t ao_chnid);


/**
* @ingroup      vs_mal_audio
* @brief        enable the aqe pipe in the audio output channel
* @details
* @param [in]   ao_devid        audio output device id
* @param [in]   ao_chnid        audio output channel id
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note         return error if the aqe pipe is not hooked in the audio channel
* @par
*
*/
vs_int32_t vs_mal_aout_aqe_enable(vs_int32_t ao_devid, vs_int32_t ao_chnid);

/**
* @ingroup      vs_mal_audio
* @brief        disable the aqe pipe in the audio output channel
* @details
* @param [in]   ao_devid        audio output device id
* @param [in]   ao_chnid        audio output channel id
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note         return error if the aqe pipe is not hooked in the audio channel
* @par
*
*/
vs_int32_t vs_mal_aout_aqe_disable(vs_int32_t ao_devid, vs_int32_t ao_chnid);

/**
* @ingroup      vs_mal_audio
* @brief        set the attribute of the aqe pipe in the audio output channel
* @details
* @param [in]   ao_devid        audio output device id
* @param [in]   ao_chnid        audio output channel id
* @param [in]   p_cfg           the attribute of the output aqe pipe
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note         return error if the aqe pipeline is not hooked in the audio channel
* @par
*
*/
vs_int32_t vs_mal_aout_aqe_attr_set(vs_int32_t ao_devid, vs_int32_t ao_chnid, const vs_aout_aqe_cfg_s *p_cfg);

/**
* @ingroup      vs_mal_audio
* @brief        get the attribute of the aqe pipe in the audio output channel
* @details
* @param [in]   ao_devid        audio output device id
* @param [in]   ao_chnid        audio output channel id
* @param [out]  p_cfg           the attribute of the output aqe pipe
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note         return error if the aqe pipeline is not hooked in the audio channel
* @par
*
*/
vs_int32_t vs_mal_aout_aqe_attr_get(vs_int32_t ao_devid, vs_int32_t ao_chnid, vs_aout_aqe_cfg_s *p_cfg);



/* AENC API Prototype */

/**
* @ingroup      vs_mal_audio
* @brief        create audio encoder channel
* @details
* @param [in]   aenc_chnid      audio encoder channel id
* @param [in]   p_attr          the parameter of the audio encoder
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_aenc_create(vs_int32_t aenc_chnid, const vs_aenc_attr_s *p_attr);

/**
* @ingroup      vs_mal_audio
* @brief        destroy audio encoder channel
* @details
* @param [in]   aenc_chnid      audio encoder channel id
* @param [in]   p_setting       the parameter of the audio encoder
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_aenc_destroy(vs_int32_t aenc_chnid);

/**
* @ingroup      vs_mal_audio
* @brief        send pcm frame to encode
* @details
* @param [in]   aenc_chnid      audio encoder channel id
* @param [in]   p_main_frame    audio frame pointer
* @param [in]   p_aec_frame     audio aec frame pointer
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note         1. p_aec_frame is set to NULL if no AEC is required
*               2. the api is non-block, return error if the audio pool is full
*               3. the api is not needed if the sys_bind has binded encoder with AI
* @par
*
*/
vs_int32_t vs_mal_aenc_frame_send(vs_int32_t aenc_chnid, const vs_audio_frame_s *p_main_frame, const vs_audio_aecframe_s *p_aec_frame);

/**
* @ingroup      vs_mal_audio
* @brief        get the encoded compressed stream
* @details
* @param [in]   aenc_chnid      audio encoder channel id
* @param [in]   timeout_ms      timeout to acquire audio output encoded frame
*                               >0: error returns if the frame can't been acquired within timeout_ms
*                               =0: return directly, error if no data, or success
*                               -1: block mode until the frame is available
* @param [out]  p_stream        audio encoded frame pointer
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_aenc_stream_acquire(vs_int32_t aenc_chnid, vs_audio_stream_s *p_stream, vs_int32_t timeout_ms);

/**
* @ingroup      vs_mal_audio
* @brief        release the encoded compressed stream resource
* @details
* @param [in]   aenc_chnid      audio encoder channel id
* @param [in]   p_stream        audio encoded frame pointer
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_aenc_stream_release(vs_int32_t aenc_chnid, const vs_audio_stream_s *p_stream);

/**
* @ingroup      vs_mal_audio
* @brief        get the file handle of the audio encoder channel
* @details
* @param [in]   aenc_chnid      audio encoder channel id
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_aenc_fd_get(vs_int32_t aenc_chnid);

/**
* @ingroup      vs_mal_audio
* @brief        get the encoded stream information
* @details
* @param [in]   aenc_chnid      audio encoder channel id
* @param [out]  p_buffer_addr   the base address of the stream buffer
* @param [out]  p_buffer_size   the size of the stream in the buffer
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_aenc_stream_buf_info_get(vs_int32_t aenc_chnid, vs_uint64_t *p_buffer_addr, vs_uint32_t *p_buffer_size);

/**
* @ingroup      vs_mal_audio
* @brief        set the mute state of the encoder channel
* @details
* @param [in]   aenc_chnid      audio encoder channel id
* @param [in]   enable          VS_FALSE: un-mute, VS_TRUE: mute
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_aenc_mute_set(vs_int32_t aenc_chnid, vs_bool_t enable);

/**
* @ingroup      vs_mal_audio
* @brief        get the mute state of the encoder channel
* @details
* @param [in]   aenc_chnid      audio encoder channel id
* @param [out]  p_enable        mute state pointer
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_aenc_mute_get(vs_int32_t aenc_chnid, vs_bool_t *p_enable);


/**
* @ingroup      vs_mal_audio
* @brief        register the specified encoder to the encoder pool
* @details
* @param [in]   p_encoder       the encoder object
* @param [out]  p_handle        the registered encoder handler
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_aenc_encoder_register(vs_int32_t *p_handle, const vs_aenc_encoder_s *p_encoder);

/**
* @ingroup      vs_mal_audio
* @brief        un-register the specified encoder from the encoder pool
* @details
* @param [in]   handle          the registered encoder handler
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_aenc_encoder_unregister(vs_int32_t handle);


/* ADEC API Prototype */

/**
* @ingroup      vs_mal_audio
* @brief        create audio decoder channel
* @details
* @param [in]   adec_chnid      audio decoder channel id
* @param [in]   p_attr          the parameter of the audio decoder
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_adec_create(vs_int32_t adec_chnid, const vs_adec_attr_s *p_attr);

/**
* @ingroup      vs_mal_audio
* @brief        destroy audio decoder channel
* @details
* @param [in]   adec_chnid      audio decoder channel id
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @ingroup      vs_mal_audio
* @brief        send pcm frame to encode
* @details
* @param [in]   aenc_chnid      audio encoder channel id
* @note
* @par
*
*/
vs_int32_t vs_mal_adec_destroy(vs_int32_t adec_chnid);

/**
* @param [in]   p_stream        audio encoded stream
* @param [in]   is_block        VS_TRUE: block, VS_FALSE: non-block
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note         1. LPCM decoder only support FRAME mode
*               2. make sure the bit-stream sent to the decoder is correct, or error happens
* @par
*
*/
vs_int32_t vs_mal_adec_stream_send(vs_int32_t adec_chnid, const vs_audio_stream_s *p_stream, vs_bool_t is_block);

/**
* @ingroup      vs_mal_audio
* @brief        clear the buffered data on the audio decoder channel
* @details
* @param [in]   adec_chnid      audio decoder channel id
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_adec_buf_clear(vs_int32_t adec_chnid);

/**
* @ingroup      vs_mal_audio
* @brief        get the decoded PCM frame
* @details
* @param [in]   adec_chnid      audio decoder channel id
* @param [in]   is_block        VS_TRUE: block, VS_FALSE: non-block
* @param [out]  p_frame         the decoded PCM frame pointer
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_adec_frame_acquire(vs_int32_t adec_chnid, vs_audio_frame_s *p_frame, vs_bool_t is_block);

/**
* @ingroup      vs_mal_audio
* @brief        release the decoded PCM frame
* @details
* @param [in]   adec_chnid      audio decoder channel id
* @param [in]   p_frame         the decoded PCM frame pointer
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_adec_frame_release(vs_int32_t adec_chnid, const vs_audio_frame_s * p_frame);

/**
* @ingroup      vs_mal_audio
* @brief        send end of stream notification to the decoder, and clear the buffered stream
* @details
* @param [in]   adec_chnid      audio decoder channel id
* @param [in]   is_instant      VS_TRUE: clear immediately the buffered stream, VS_FALSE: delay to clear
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_adec_eos_set(vs_int32_t adec_chnid, vs_bool_t is_instant);

/**
* @ingroup      vs_mal_audio
* @brief        get the audio buffer status in the decoder
* @details
* @param [in]   adec_chnid      audio decoder channel id
* @param [out]  p_status        the audio buffer status pointer
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_adec_status_get(vs_int32_t adec_chnid, vs_adec_status_s *p_status);

/**
* @brief        get the file handle of the audio decoder channel
* @details
* @param [in]   adec_chnid      audio decoder channel id
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_adec_fd_get(vs_int32_t adec_chnid);

/**
* @ingroup      vs_mal_audio
* @brief        register the specified decoder to the encoder pool
* @details
* @param [in]   p_decoder       the decoder object
* @param [out]  p_handle        the registered decoder handler
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_adec_decoder_register(vs_int32_t *p_handle, const vs_adec_decoder_s *p_decoder);

/**
* @ingroup      vs_mal_audio
* @brief        un-register the specified decoder from the encoder pool
* @details
* @param [in]   p_handle        the registered decoder handler
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_adec_decoder_unregister(vs_int32_t handle);


#ifdef __cplusplus
}
#endif

#endif

