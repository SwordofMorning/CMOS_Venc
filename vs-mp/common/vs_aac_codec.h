
/**
* @file vs_aac_codec.h
* @brief Declaration of audio enumeration, struct and API.
* @details Declaration of audio enumeration, struct and API.
* @author VS software group
* @date 2021-11-02
* @version v0.01
* @copyright (C) 2021, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/

#ifndef __VS_AAC_CODEC_H__
#define __VS_AAC_CODEC_H__

/*****************************************************************************
 *                    Includes
 *****************************************************************************/
#include "vs_audio_defines.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/
/** Samples per frame for different AAC type */
#define AACLD_FRAME_SAMPLE_NUM    (512)
#define AACELD_FRAME_SAMPLE_NUM   (512)
#define AACLC_FRAME_SAMPLE_NUM    (1024)
#define EAAC_FRAME_SAMPLE_NUM     (2048)
#define EAACPLUS_FRAME_SAMPLE_NUM (2048)

/**
 * @brief AAC codec profile type
 */
typedef enum vs_aac_type {
    E_AUDIO_AAC_TYPE_UNKNOWN = -1,
    E_AUDIO_AAC_TYPE_AACLC,
    E_AUDIO_AAC_TYPE_EAAC,
    E_AUDIO_AAC_TYPE_EAACPLUS,
    E_AUDIO_AAC_TYPE_AACLD,
    E_AUDIO_AAC_TYPE_AACELD,
    E_AUDIO_AAC_TYPE_MAX
} vs_aac_type_e;

/**
 * @brief AAC codec trans type
 */
typedef enum vs_aac_trans_type {
    E_AUDIO_AAC_TRANS_TYPE_ADTS     = 0,
    E_AUDIO_AAC_TRANS_TYPE_LOAS,
    E_AUDIO_AAC_TRANS_TYPE_LATM_MCP1,
    E_AUDIO_AAC_TRANS_TYPE_MAX
} vs_aac_trans_type_e;

/**
* @brief the bitrate of the audio encoded stream
*/
typedef enum vs_audio_bitrate {
    E_AUDIO_BPS_8K    = 8000,
    E_AUDIO_BPS_16K   = 16000,
    E_AUDIO_BPS_22K   = 22000,
    E_AUDIO_BPS_24K   = 24000,
    E_AUDIO_BPS_32K   = 32000,
    E_AUDIO_BPS_44K   = 44000,
    E_AUDIO_BPS_48K   = 48000,
    E_AUDIO_BPS_64K   = 64000,
    E_AUDIO_BPS_96K   = 96000,
    E_AUDIO_BPS_128K  = 128000,
    E_AUDIO_BPS_132K  = 132000,
    E_AUDIO_BPS_144K  = 144000,
    E_AUDIO_BPS_192K  = 192000,
    E_AUDIO_BPS_256K  = 256000,
    E_AUDIO_BPS_265K  = 265000,
    E_AUDIO_BPS_288K  = 288000,
    E_AUDIO_BPS_320K  = 320000,
    E_AUDIO_BPS_MAX
} vs_audio_bitrate_e;

/**
 * @brief AAC encoder attribute
 */
typedef struct vs_aenc_aac_attr {
    vs_uint32_t                 sample_rate;
    vs_aac_type_e               aac_type;
    vs_audio_sample_bitwidth_e  bit_width;       ///< AAC bit width (only support 16bit)
    vs_audio_bitrate_e          bitrate;
    vs_aac_trans_type_e         trans_type;
    vs_audio_sound_mode_e       sound_mode;
    vs_uint16_t                 channel_num;
    vs_int16_t                  band_width;      ///< targeted audio bandwidth in Hz (0 or 1000~sample_rate/2), the default is 0
} vs_aenc_aac_attr_s;

/**
 * @brief AAC decoder attribute
 */
typedef struct vs_adec_aac_attr {
    vs_aac_trans_type_e  trans_type;
    vs_aac_type_e        aac_type;
} vs_adec_aac_attr_s;

/*****************************************************************************
 *                    Audio API Prototypes
 *****************************************************************************/

/**
* @ingroup      vs_aenc_aac
* @brief        get aac encoder object
* @details
* @param        void
* @retval       pointer of vs_aenc_encoder_s
* @retval       NULL          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_aenc_encoder_s *vs_drv_aenc_aac_object_get(void);

/**
* @ingroup      vs_adec_aac
* @brief        get aac decoder object
* @details
* @param        void
* @retval       pointer of vs_adec_decoder_s
* @retval       NULL          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_adec_decoder_s *vs_drv_adec_aac_object_get(void);


#ifdef __cplusplus
}
#endif

#endif

