/**
* @file vs_vdec_defines.h
* @brief This file declare the enumeration and struct of VDEC.
* @details This file declare the enumeration and struct of VDEC.
* @author VS software group
* @date 2021-05-16
* @version v1.01
* @copyright (C) 2022, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/

#ifndef __VS_VDEC_DEFINES_H__
#define __VS_VDEC_DEFINES_H__

#include "vs_comm.h"
#include "vs_comm_video.h"
#include "vs_errno.h"
#include "vs_mal_vbm.h"
#include "vs_capability.h"

#ifdef __cplusplus
extern "C" {
#endif

///< parameter is illegal
#define VS_ERR_VDEC_INVALID_PARAM      VS_ERR_CODE(E_MOD_ID_VDEC, E_ERR_INVALID_PARAM)
///< invalid channel ID */
#define VS_ERR_VDEC_INVALID_CHNID      VS_ERR_CODE(E_MOD_ID_VDEC, E_ERR_INVALID_CHNID)
///< bad state
#define VS_ERR_VDEC_BAD_STATE          VS_ERR_CODE(E_MOD_ID_VDEC, E_ERR_BAD_STATE)
///< using a NULL pointer
#define VS_ERR_VDEC_NULL_PTR           VS_ERR_CODE(E_MOD_ID_VDEC, E_ERR_NULL_PTR)
///< channel has been created
#define VS_ERR_VDEC_EXIST              VS_ERR_CODE(E_MOD_ID_VDEC, E_ERR_EXIST)
///< channel is not created
#define VS_ERR_VDEC_UNEXIST            VS_ERR_CODE(E_MOD_ID_VDEC, E_ERR_UNEXIST)
///< operation is not supported
#define VS_ERR_VDEC_NOT_SUPPORT        VS_ERR_CODE(E_MOD_ID_VDEC, E_ERR_NOT_SUPPORT)
///< system is not ready
#define VS_ERR_VDEC_NOT_INITIALIZED    VS_ERR_CODE(E_MOD_ID_VDEC, E_ERR_NOT_INITIALIZED)
///< operation is not permitted
#define VS_ERR_VDEC_NOT_PERM           VS_ERR_CODE(E_MOD_ID_VDEC, E_ERR_NOT_PERM)
///< no memory for allocated
#define VS_ERR_VDEC_NO_MEMORY          VS_ERR_CODE(E_MOD_ID_VDEC, E_ERR_NO_MEMORY)
///< buffer is empty
#define VS_ERR_VDEC_BUF_EMPTY          VS_ERR_CODE(E_MOD_ID_VDEC, E_ERR_BUF_EMPTY)
///< buffer is full
#define VS_ERR_VDEC_BUF_FULL           VS_ERR_CODE(E_MOD_ID_VDEC, E_ERR_BUF_FULL)
///< system busy
#define VS_ERR_VDEC_BUSY               VS_ERR_CODE(E_MOD_ID_VDEC, E_ERR_BUSY)
///< device unexists
#define VS_ERR_VDEC_DEVICE_UNEXIST     VS_ERR_CODE(E_MOD_ID_VDEC, E_ERR_DEVICE_UNEXIST)

/**
* @brief Define the input stream buffer allocated mode.
* @details
*/
typedef enum vs_vdec_stream_buf_mode {
    E_VDEC_STREAM_BUF_NORMAL = 0,   ///< Normal mode
    E_VDEC_STREAM_BUF_SAVE,         ///< Save memory mode
    E_VDEC_STREAM_BUF_MAX
} vs_vdec_stream_buf_mode_e;

/**
* @brief Define the VDEC module parameters.
* @details
*/
typedef struct vs_vdec_mod_param {
    vs_pool_type_e vb_source;                           ///< RW; video buffer source\n
                                                        ///< default: VB_SOURCE_MODULE
    vs_vdec_stream_buf_mode_e stream_buf_mode; 	        ///< RW; stream buffer allocated mode\n
                                                        ///< default: E_VDEC_STREAM_BUF_NORMAL
    vs_size_s max_video_size;                           ///< R; max video size supported by VDEC
    vs_size_s max_pic_size;                             ///< R; max picture size supported by VDEC
} vs_vdec_mod_param_s;

/**
* @brief Define the stream sending mode.
* @details
*/
typedef enum vs_vdec_input_mode {
	E_VDEC_INPUT_MODE_STREAM = 0,      ///< Sending stream data by stream
	E_VDEC_INPUT_MODE_FRAME,           ///< Sending stream data by frame
	E_VDEC_INPUT_MODE_CHUNK,           ///< Sending stream by frame with multiple packets
	E_VDEC_INPUT_MODE_MAX
} vs_vdec_input_mode_e;

/**
* @brief Defines the scale attributes.
* @details
*/
typedef struct vs_vdec_scale {
    vs_bool_t enable;                           ///< RW; indicate whether to enable scale
    vs_size_s scale_size;                       ///< RW; scale size
} vs_vdec_scale_s;

/**
* @brief Define the sei user data of the decoding channel.
* @details
*/
typedef struct vs_vdec_sei_userdata_attr {
    vs_bool_t enable;                           ///< W; indicate whether to parse user data in SEI
    vs_uint32_t max_len;                        ///< W; max length of user data\n
                                                ///< Range: (0, 65535];
} vs_vdec_sei_userdata_attr_s;

/**
* @brief Define the video(H.264/H.265) attributes of the decoding channel.
* @details
*/
typedef struct vs_vdec_video_attr {
    vs_uint32_t ref_frame_num;                  ///< RW; reference frame number\n
                                                ///< Range: [0, 16];
    vs_vdec_sei_userdata_attr_s userdata_attr;  ///< RW; configuration for obtaining user data of SEI information
} vs_vdec_video_attr_s;

/**
* @brief Define the attributes of the decoding channel.
* @details
*/
typedef struct vs_vdec_chn_attr {
    vs_payload_type_e type;                     ///< RW; stream decoding type
    vs_vdec_input_mode_e input_mode;            ///< RW; stream sending mode
    vs_uint32_t width;                          ///< RW; frame width of decoding channel
    vs_uint32_t height;                         ///< RW; frame height of decoding channel
    vs_uint32_t stream_buf_size;                ///< RW; stream buffer size(Byte)
    vs_uint32_t frame_buf_size;                 ///< RW; frame buffer size(Byte), only used in private VB mode
    vs_uint32_t frame_buf_cnt;                  ///< RW; frame buffer count, only used in private VB mode
    vs_crop_s crop_info;                        ///< RW; crop parameters
    vs_vdec_scale_s scale_info;                 ///< RW; scale parameters
    vs_mirror_mode_e mirror_mode;               ///< RW; mirror mode
    vs_vdec_video_attr_s video_attr;            ///< RW; video attributes of decoding channel
} vs_vdec_chn_attr_s;

/**
* @brief Define the specified output frame type.
* @details
*/
typedef enum vs_vdec_dec_mode {
    E_VDEC_DEC_MODE_IPB = 0,            ///< Output I, P, B frame
    E_VDEC_DEC_MODE_IP,                 ///< Only output I, P frame
    E_VDEC_DEC_MODE_I,                  ///< Only output I frame
    E_VDEC_DEC_MODE_MAX
} vs_vdec_dec_mode_e;

/**
* @brief Define the output order of decoded frames.
* @details
*/
typedef enum vs_vdec_output_order {
    E_VDEC_OUTPUT_ORDER_DISPLAY = 0,        ///< Output by display order
    E_VDEC_OUTPUT_ORDER_DECODE,             ///< Output by decode order
    E_VDEC_OUTPUT_ORDER_MAX
} vs_vdec_output_order_e;

/**
* @brief Define the video(H.264/H.265) parameters of the decoding channel.
* @details
*/
typedef struct vs_vdec_chn_video_param {
    vs_uint32_t err_thr;                            ///< RW; thresholds related to the handling of decoding errors\n
                                                    ///< Range: [0, 100]; default: 30
    vs_vdec_dec_mode_e dec_mode;                    ///< RW; output frame type, only output the specified frame type\n
                                                    ///< default: E_VDEC_DEC_MODE_IP
    vs_vdec_output_order_e output_order;            ///< RW; output order of the decoded frame\n
                                                    ///< default: E_VDEC_OUTPUT_ORDER_DECODE
} vs_vdec_chn_video_param_s;

/**
* @brief Define the parameters of the decoding channel.
* @details
*/
typedef struct vs_vdec_chn_param {
    vs_payload_type_e type;                         ///< RW; stream decoding type
    vs_uint32_t output_frame_num;                   ///< RW; output frame number\n
                                                    ///< Range: [0, 16]; default: 2
    vs_compress_mode_e compress_mode;               ///< RW; output compress mode of the decoded frame\n
                                                    ///< default: E_COMPRESS_MODE_RASTER
    vs_video_format_e video_format;                 ///< RW; output video format of the decoded frame\n
                                                    ///< default: E_VIDEO_FORMAT_LINEAR
    vs_pixel_format_e pixel_format;                 ///< RW; output pixel format of the decoded frame\n
                                                    ///< default: E_PIXEL_FORMAT_YUV_420SP
    vs_vdec_chn_video_param_s video_param;          ///< RW; video parameters of decoding channel
} vs_vdec_chn_param_s;

/**
* @brief Define the information of stream to be decoded.
* @details
*/
typedef struct vs_vdec_stream {
    vs_uint32_t len;                    ///< W; stream buffer length
    vs_uint64_t pts;                    ///< W; presentation time stamp, only valid in frame stream mode
    vs_bool_t is_frame_end;             ///< W; indicate the end of frame, only valid in compatible stream mode
    vs_bool_t is_stream_end;            ///< W; indicate the end of stream
    vs_bool_t is_display;               ///< W; indicate current frame display or not, only valid in frame stream mode
    vs_uint8_t *p_virt_addr;            ///< W; virtual address of stream buffer
} vs_vdec_stream_s;

/**
* @brief Define the user data information in SEI(H.264/H.265).
* @details
*/
typedef struct vs_vdec_sei_userdata {
    vs_uint64_t phys_addr;              ///< R; physical address of user data
    vs_uint32_t len;                    ///< R; length of user data
    vs_uint8_t *p_virt_addr;            ///< R; virtual address of user data
} vs_vdec_sei_userdata_s;

/**
* @brief Define the error information during the decoding process.
* @details
*/
typedef struct vs_vdec_dec_errors {
    vs_uint32_t stream_size_err_cnt;        ///< R; frame width or height over channel size
    vs_uint32_t stream_unsupport_cnt;       ///< R; stream unsupported, e.g. H.265 MAIN10 profile
    vs_uint32_t stream_err_cnt;             ///< R; stream error detected
    vs_uint32_t protocol_param_err_cnt;     ///< R; protocol number is not enough, used only in H.264/H.265 decoding
    vs_uint32_t ref_frame_num_err_cnt;      ///< R; reference number is not enough, used only in H.264/H.265 decoding
    vs_uint32_t frame_buf_size_err_cnt;     ///< R; frame buffer size is not enough
    vs_uint32_t hw_internal_err_cnt;        ///< R; hardware internal error
} vs_vdec_dec_errors_s;

/**
* @brief Define the status information of the decoding channel.
* @details
*/
typedef struct vs_vdec_chn_status {
    vs_payload_type_e type;                 ///< R; stream decoding type
    vs_uint32_t left_stream_byte_num;       ///< R; left stream bytes in stream buffer waiting for decode
    vs_uint32_t left_stream_frame_num;      ///< R; left frames in stream buffer waiting for decode, only valid in frame stream mode
    vs_uint32_t left_output_frame_num;      ///< R; left pictures waiting for output
    vs_bool_t is_chn_started;               ///< R; indicate whether to start channel
    vs_uint32_t rcvstream_frame_num;        ///< R; how many frames has been received, only valid in frame stream mode
    vs_uint32_t decoded_frame_num;          ///< R; how many pictures has been decoded
    vs_vdec_dec_errors_s err_statistic;     ///< R; decoder errors, each error is accumulated until channel reset
    vs_uint32_t width;                      ///< R; decoded frame width
    vs_uint32_t height;                     ///< R; decoded frame height
} vs_vdec_chn_status_s;

/**
* @brief Define the H.264 protocol parameters.
* @details
*/
typedef struct vs_vdec_protocol_h264_param {
    vs_uint32_t max_sps_num;            ///< RW; maximum number of SPS supported
    vs_uint32_t max_pps_num;            ///< RW; maximum number of PPS supported
} vs_vdec_protocol_h264_param_s;

/**
* @brief Define the H.265 protocol parameters.
* @details
*/
typedef struct vs_vdec_protocol_h265_param {
    vs_uint32_t max_vps_num;            ///< RW; maximum number of SPS supported
    vs_uint32_t max_sps_num;            ///< RW; maximum number of PPS supported
    vs_uint32_t max_pps_num;            ///< RW; maximum number of VPS supported
} vs_vdec_protocol_h265_param_s;

/**
* @brief Define the video(H.264/H.265) protocol parameters of the decoding channel.
* @details
*/
typedef struct vs_vdec_protocol_param {
    vs_payload_type_e type;                                 ///< RW; stream decoding type
    union {
        vs_vdec_protocol_h264_param_s protocol_h264_param;  ///< RW; H.264 protocol parameters
        vs_vdec_protocol_h265_param_s protocol_h265_param;  ///< RW; H.265 protocol parameters
    };
} vs_vdec_protocol_param_s;

/**
* @brief Define the output mode of the decoded frame.
* @details
*/
typedef enum vs_vdec_display_mode {
    E_VDEC_DISPLAY_MODE_PLAYBACK = 0,     ///< Repeat sending current frame when the post module cannot receive it
    E_VDEC_DISPLAY_MODE_PREVIEW,          ///< Discard the current frame when the post module cannot receive it
    E_VDEC_DISPLAY_MODE_MAX
} vs_vdec_display_mode_e;

#ifdef __cplusplus
}
#endif

#endif /*!<  __VS_VDEC_DEFINES_H__ */

