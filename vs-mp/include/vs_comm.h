/**
* @file vs_comm.h
* @brief comm  info
* @details comm  info
* @author VS codec group
* @date 2021-06-30
* @version v0.10
* @Copyright (C) 2021, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/

#ifndef __VS_COMM_H__
#define __VS_COMM_H__

#include "vs_type.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
* @brief Defines common module id enum.
* @details
*/
typedef enum vs_mod_id {
    E_MOD_ID_BASE = 0,          ///<  base module ID
    E_MOD_ID_VB,                ///<  video buffer module ID
    E_MOD_ID_SYS,               ///<  system module ID
    E_MOD_ID_DMA,               ///<  dma module ID
    E_MOD_ID_MIPI_RX,           ///<  mipi rx module ID
    E_MOD_ID_VII,               ///<  video input interface module ID
    E_MOD_ID_ISPFE,             ///<  image signal process front-end module ID
    E_MOD_ID_ISPBE,             ///<  image signal process back-end module ID
    E_MOD_ID_VPP,               ///<  video pre-process module ID
    E_MOD_ID_GPE,               ///<  Graphic process engine module ID
    E_MOD_ID_GDC,               ///<  Graphic distortion correction module  ID
    E_MOD_ID_AVS,               ///<  Any view stitch module ID
    E_MOD_ID_PCIV,              ///<  Video translate by pcie, PCIV is the module ID
    E_MOD_ID_VO,                ///<  video output/display module ID
    E_MOD_ID_MIPI_TX,           ///<  MIPI TX module ID
    E_MOD_ID_HDMI,              ///<  HDMI module ID
    E_MOD_ID_REGION,            ///<  Region module ID
    E_MOD_ID_FB,                ///<  linux frame buffer module ID
    E_MOD_ID_IVE,               ///<  ive module ID
    E_MOD_ID_NPU,               ///<  Neural Network process module ID
    E_MOD_ID_DSP,               ///<  dsp host module ID
    E_MOD_ID_VENC,              ///<  video encoder module ID
    E_MOD_ID_VDEC,              ///<  video decoder module ID
    E_MOD_ID_YSUM,              ///<  ysum module ID
    E_MOD_ID_ACODEC,            ///<  audio codec module ID
    E_MOD_ID_AUDIO,             ///<  audio module ID
    E_MOD_ID_AIN,               ///<  audio input module ID
    E_MOD_ID_AOUT,              ///<  audio output module ID
    E_MOD_ID_AENC,              ///<  audio encoder module ID
    E_MOD_ID_ADEC,              ///<  audio decoder module ID
    E_MOD_ID_CMP_ENGINE,        ///<  compress engine module ID
    E_MOD_ID_TDC,               ///<  traffic signal module ID
    E_MOD_ID_USER,              ///<  user application module ID
    E_MOD_ID_LOG,               ///<  log module ID
    E_MOD_ID_REGTOOLS,          ///<  reg-tools ID
    E_MOD_ID_TDE,               ///<  Two dimension engine module ID
    E_MOD_ID_CIPHER,            ///<  cipher module ID
    E_MOD_ID_INIT,              ///<  init module ID
    E_MOD_ID_ISPAE,		///<  ispae module ID
    E_MOD_ID_ISPAWB,		///<  ispawb module ID
    E_MOD_ID_ISPAF,		///<  ispaf module ID
    E_MOD_ID_MAX
} vs_mod_id_e;

/**
* @brief Comm video channel define for all modules.
* @details including module id, device id and channel id.
*/
typedef struct vs_chn {
   vs_mod_id_e modid;     ///< module id.
   vs_uint32_t devid;     ///< device id, some module using pipe or group id.
   vs_uint32_t chnid;     ///< video channel id, include module internal channel or extern channel.
} vs_chn_s;

/**
* @brief Defines the video/audio payload type.
* @details define payload type depend RTP/RTSP definition.
*/
typedef enum vs_payload_type {
    E_PT_TYPE_PCMU         = 0,
    E_PT_TYPE_1016         = 1,
    E_PT_TYPE_G721         = 2,
    E_PT_TYPE_GSM          = 3,
    E_PT_TYPE_G723         = 4,
    E_PT_TYPE_LPC          = 7,
    E_PT_TYPE_PCMA         = 8,
    E_PT_TYPE_G722         = 9,
    E_PT_TYPE_S16BE_STEREO  = 10,
    E_PT_TYPE_S16BE_MONO    = 11,
    E_PT_TYPE_QCELP       = 12,
    E_PT_TYPE_CN          = 13,
    E_PT_TYPE_MPEGAUDIO   = 14,
    E_PT_TYPE_G728       = 15,
    E_PT_TYPE_G729       = 18,
    E_PT_TYPE_G711A      = 19,
    E_PT_TYPE_G711U      = 20,
    E_PT_TYPE_G726       = 21,
    E_PT_TYPE_G729A      = 22,
    E_PT_TYPE_LPCM       = 23,
    E_PT_TYPE_CelB       = 25,
    E_PT_TYPE_JPEG       = 26,
    E_PT_TYPE_CUSM       = 27,
    E_PT_TYPE_NV         = 28,
    E_PT_TYPE_PICW       = 29,
    E_PT_TYPE_CPV        = 30,
    E_PT_TYPE_H261       = 31,
    E_PT_TYPE_MPEGVIDEO  = 32,
    E_PT_TYPE_MPEG2TS    = 33,
    E_PT_TYPE_H263       = 34,
    E_PT_TYPE_SPEG       = 35,
    E_PT_TYPE_MPEG2VIDEO = 36,
    E_PT_TYPE_AAC        = 37,
    E_PT_TYPE_WMA9STD    = 38,
    E_PT_TYPE_HEAAC      = 39,
    E_PT_TYPE_PCM_VOICE  = 40,
    E_PT_TYPE_PCM_AUDIO  = 41,
    E_PT_TYPE_MP3        = 43,
    E_PT_TYPE_ADPCMA     = 49,
    E_PT_TYPE_AEC        = 50,
    E_PT_TYPE_X_LD       = 95,
    E_PT_TYPE_H264       = 96,
    E_PT_TYPE_MP4VIDEO   = 230,
    E_PT_TYPE_MP4AUDIO   = 237,
    E_PT_TYPE_VC1        = 238,
    E_PT_TYPE_AVS        = 258,
    E_PT_TYPE_REAL8      = 259,
    E_PT_TYPE_REAL9      = 260,
    E_PT_TYPE_H265       = 265,
    E_PT_TYPE_VP8        = 266,
    E_PT_TYPE_MVC        = 267,
    E_PT_TYPE_PNG        = 268,
    E_PT_TYPE_MJPEG      = 1001, ///<  for Visinex
    E_PT_TYPE_MAX
} vs_payload_type_e;

#ifndef MAJOR_VER
#define MAJOR_VER	1
#endif

#ifndef MINOR_VER
#define MINOR_VER	0
#endif

#ifndef PATCH_VER
#define PATCH_VER	0
#endif

#ifndef BUILD_ID
#define BUILD_ID	0
#endif

#define __MK_VERSION(a, b, c, d) #a "." #b "." #c "." #d
#define VS_VERSION(a, b, c, d)  __MK_VERSION(a, b, c, d)
#define VS_REL "Rel R"
#define VS_REL_VERSION VS_REL VS_VERSION(MAJOR_VER, MINOR_VER, PATCH_VER, BUILD_ID)


#ifdef __cplusplus
}
#endif

#endif /*!<  __VS_COMM_H__ */
