/**
* @file vs_venc_defines.h
* @brief Declaraction of venc enumeration and struct
* @details Declaraction of venc enumeration and struct
* @author VS software group
* @date 2021-6-15
* @version v0.01
* @copyright (C) 2020, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/

#ifndef __VS_VENC_DEFINES_H__
#define __VS_VENC_DEFINES_H__


#include "vs_type.h"
#include "vs_comm.h"
#include "vs_errno.h"
#include "vs_comm_video.h"
#include "vs_mal_vbm.h"
#include "vs_capability.h"

#ifdef __cplusplus
extern "C"{
#endif

/*****************************************************************************************/

#define VS_ERR_VENC_INVALID_PARAM    VS_ERR_CODE(E_MOD_ID_VENC, E_ERR_INVALID_PARAM)
#define VS_ERR_VENC_INVALID_CHNID    VS_ERR_CODE(E_MOD_ID_VENC, E_ERR_INVALID_CHNID)
#define VS_ERR_VENC_BAD_STATE        VS_ERR_CODE(E_MOD_ID_VENC, E_ERR_BAD_STATE)
#define VS_ERR_VENC_NULL_PTR         VS_ERR_CODE(E_MOD_ID_VENC, E_ERR_NULL_PTR)
#define VS_ERR_VENC_EXIST            VS_ERR_CODE(E_MOD_ID_VENC, E_ERR_EXIST)
#define VS_ERR_VENC_UNEXIST          VS_ERR_CODE(E_MOD_ID_VENC, E_ERR_UNEXIST)
#define VS_ERR_VENC_NOT_SUPPORT      VS_ERR_CODE(E_MOD_ID_VENC, E_ERR_NOT_SUPPORT)
#define VS_ERR_VENC_NOT_INITIALIZED  VS_ERR_CODE(E_MOD_ID_VENC, E_ERR_NOT_INITIALIZED)
#define VS_ERR_VENC_NOT_PERM         VS_ERR_CODE(E_MOD_ID_VENC, E_ERR_NOT_PERM)
#define VS_ERR_VENC_NO_MEMORY        VS_ERR_CODE(E_MOD_ID_VENC, E_ERR_NO_MEMORY)
#define VS_ERR_VENC_BUF_EMPTY        VS_ERR_CODE(E_MOD_ID_VENC, E_ERR_BUF_EMPTY)
#define VS_ERR_VENC_BUF_FULL         VS_ERR_CODE(E_MOD_ID_VENC, E_ERR_BUF_FULL)
#define VS_ERR_VENC_BUSY             VS_ERR_CODE(E_MOD_ID_VENC, E_ERR_BUSY)
#define VS_ERR_VENC_DEVICE_UNEXIST   VS_ERR_CODE(E_MOD_ID_VENC, E_ERR_DEVICE_UNEXIST)
#define VS_ERR_VENC_TIMEOUT   		 VS_ERR_CODE(E_MOD_ID_VENC, E_ERR_TIMEOUT)


/**
* @brief Defines the venc output stream mode.
* @details
*/
typedef enum vs_venc_stream_mode {
    E_VENC_STREAM_MODE_STREAM = 0,    ///< venc output data by stream.
	E_VENC_STREAM_MODE_FRAME,    ///< venc output data by frame.
    E_VENC_STREAM_MODE_NALU,    ///< venc output data by nalu.
    E_VENC_STREAM_MODE_MAX
} vs_venc_stream_mode_e;

/**
* @brief Defines video codec profile.
* @details
*/
typedef enum vs_venc_profile {
    E_VENC_PROFILE_H264_BASELINE = 0,    ///< H.264 baseline profile,
                                         ///< for more information refer to ITU-T doc
    E_VENC_PROFILE_H264_MAIN,            ///< H.264 main profile,
                                         ///< for more information refer to ITU-T doc
    E_VENC_PROFILE_H264_HIGH,            ///< H.264 higt profile,
                                         ///< for more information refer to ITU-T doc
    E_VENC_PROFILE_H264_HIGH_10,         ///< H.264 high10 profile,
                                         ///< for more information refer to ITU-T doc

    E_VENC_PROFILE_H265_MAIN = 10,                   ///< H.265 main profile,
                                                    ///< for more information refer to ITU-T doc
    E_VENC_PROFILE_H265_MAIN_10,                    ///< H.265 main10 profile,
                                                    ///< for more information refer to ITU-T doc
    E_VENC_PROFILE_H265_MAIN_STILL_PICTURE,         ///< H.265 main still profile,
                                                    ///< for more information refer to ITU-T doc

    E_VENC_PROFILE_JPEG_BASELINE = 20,            ///< JPEG baseline profile,
    E_VENC_PROFILE_MAX
} vs_venc_profile_e;


/**
* @brief Defines mpf config of jpeg encoding.
* @details
*/
typedef struct vs_venc_mpf_cfg {
    vs_uint8_t   thumbnail_num;    ///< number of encoded mpf thumbnails.value range: [0, 1]. 0 means do not encode mpf.
    vs_size_s  thumbnail_size[VENC_MAX_THUMBNAIL_NUM];    ///< size of the mpf thumbnail. width & height range:[48, 256].2 alignment
} vs_venc_mpf_cfg_s;

/**
* @brief Defines the pic receive mode.
* @details
*/
typedef enum vs_venc_pic_rcv_mode {
    E_VENC_PIC_RECEIVE_MODE_SINGLE = 0,    ///< current encoding channel allows to receive picture from only one source.
    E_VENC_PIC_RECEIVE_MODE_MULTI,         ///< current encoding channel allows receiving images from multiple sources.
    E_VENC_PIC_RECEIVE_MODE_MAX
} vs_venc_pic_rcv_mode_e;

/**
* @brief Defines the attribute of jpege.
* @details
*/
typedef struct vs_venc_jpeg_attr {
    vs_venc_mpf_cfg_s   mpf_cfg;    ///< encode the JPEG mpf configuration.
    vs_venc_pic_rcv_mode_e rcv_mode;    ///< picture receive mode for the JPEG channel.
    vs_pixel_format_e input_type;    ///< the pixel_format of input frame.
} vs_venc_jpeg_attr_s;

/**
* @brief Defines the attribute of h264e.
* @details
*/
typedef struct vs_venc_h264_attr {
	vs_uint32_t bandwidth_save_strength;    ///< bandwidth save strength. value range: [0, 1, 2], default is 0.
} vs_venc_h264_attr_s;

/**
* @brief Defines the attribute of h265e.
* @details
*/
typedef struct vs_venc_h265_attr {
	vs_uint32_t bandwidth_save_strength;    ///< bandwidth save strength. value range: [0, 1, 2], default is 0
} vs_venc_h265_attr_s;

/**
* @brief Defines the attribute of the Venc.
* @details
*/
typedef struct vs_venc_enc_attr {
    vs_payload_type_e          type;     ///< encoding the protocol type.
    vs_uint32_t                max_frame_width;     ///< encoding picture maximum width.2 alignment
                                                    ///< h264 [VENC_MIN_H264_WIDTH,VENC_MAX_H264_WIDTH]
                                                    ///< h265 [VENC_MIN_H265_WIDTH,VENC_MAX_H265_WIDTH]
                                                    ///< jpeg [VENC_MIN_JPEG_WIDTH,VENC_MAX_JPEG_WIDTH]
                                                    ///< mjpeg [VENC_MIN_JPEG_WIDTH,VENC_MAX_JPEG_WIDTH]
    vs_uint32_t                max_frame_height;    ///< encoding picture maximum height.2 alignment
                                                    ///< h264 [VENC_MIN_H264_WIDTH,VENC_MAX_H264_HEIGHT]
                                                    ///< h265 [VENC_MIN_H265_HEIGHT,VENC_MAX_H265_HEIGHT]
                                                    ///< jpeg [VENC_MIN_JPEG_HEIGHT,VENC_MAX_JPEG_HEIGHT]
                                                    ///< mjpeg [VENC_MIN_JPEG_HEIGHT,VENC_MAX_JPEG_HEIGHT]
    vs_uint32_t                stream_buf_size;     ///< stream buffer size.must not less than 12K bytes.
                                                    ///  must bigger than frame_width *frame_height when stream_buf_mode is E_VENC_STREAM_BUF_MODE_NORMAL
                                                    ///  must be 4096 aligned.
    vs_venc_stream_mode_e      stream_mode;         ///< the venc output stream mode.
    vs_venc_profile_e          profile;             ///< encode video profile
    vs_uint32_t                frame_width;         ///< encoding picture width.2 alignment
													///< h264 [VENC_MIN_H264_WIDTH,max_frame_width]
													///< h265 [VENC_MIN_H265_WIDTH,max_frame_width]
													///< jpeg [VENC_MIN_JPEG_WIDTH,max_frame_width]
													///< mjpeg [VENC_MIN_JPEG_WIDTH,max_frame_width]
    vs_uint32_t                frame_height;        ///< encoding picture height.2 alignment
													///< h264 [VENC_MIN_H264_WIDTH,max_frame_height]
													///< h265 [VENC_MIN_H265_HEIGHT,max_frame_height]
													///< jpeg [VENC_MIN_JPEG_HEIGHT,max_frame_height]
													///< mjpeg [VENC_MIN_JPEG_HEIGHT,max_frame_height]
    union {
        vs_venc_h264_attr_s    h264_attr;           ///< attribute of a H.264 encoder
        vs_venc_h265_attr_s    h265_attr;           ///< attribute of a H.265 encoder
        vs_venc_jpeg_attr_s    jpeg_attr;           ///< attribute of a JPEG encoder
    };
} vs_venc_enc_attr_s;

/**
* @brief Defines the attribute of h264e fixqp.
* @details
*/
typedef struct vs_venc_brc_h264_fixqp {
    vs_uint32_t      gop;    ///< H.264 GOP value, value range: [1, 65536]
    vs_uint32_t      src_framerate;    ///< venc input frame rate (fps), value range:  [1, VENC_MAX_INPUT_FRAME_RATE]
    vs_uint32_t      dst_framerate;    ///< venc output frame rate (fps), value range:  [1/64, VENC_MAX_INPUT_FRAME_RATE] specific reference vs_venc_brc_h264_cbr_s dst_framerate
    vs_uint32_t      fixqp_i;    ///< I frame qp value of all the MBs, value range: [0, 51]
    vs_uint32_t      fixqp_p;    ///< P frame qp value of all the MBs, value range: [0, 51]
    vs_uint32_t      fixqp_b;    ///< B frame qp value of all the MBs, value range: [0, 51]
} vs_venc_brc_h264_fixqp_s;

/**
* @brief Defines the mode of the BRC for controlling the bit rate of one channel of venc
* @details
*/
typedef enum vs_venc_brc_mode {
    E_VENC_BRC_MODE_H264_CBR = 0,    ///< H.264 CBR mode
    E_VENC_BRC_MODE_H264_VBR,    ///< H.264 VBR mode
    E_VENC_BRC_MODE_H264_AVBR,    ///< H.264 AVBR mode
    E_VENC_BRC_MODE_H264_CVBR,    ///< H.264 CVBR mode
    E_VENC_BRC_MODE_H264_FIXQP,    ///< H264 FixQp mode
    E_VENC_BRC_MODE_H264_QPMAP,    ///< H.264 QpMap mode

    E_VENC_BRC_MODE_MJPEG_CBR = 10,///< MJPEG CBR mode
    E_VENC_BRC_MODE_MJPEG_VBR,    ///< MJPEG VBR mode
    E_VENC_BRC_MODE_MJPEG_FIXQP,    ///< MJPEG FixQp mode

    E_VENC_BRC_MODE_H265_CBR = 20, ///< H.265 CBR mode
    E_VENC_BRC_MODE_H265_VBR,    ///< H.265 VBR mode
    E_VENC_BRC_MODE_H265_AVBR,    ///< H.265 AVBR mode
    E_VENC_BRC_MODE_H265_CVBR,    ///< H.265 CVBR mode
    E_VENC_BRC_MODE_H265_FIXQP,    ///< H.265 FixQp mode
    E_VENC_BRC_MODE_H265_QPMAP,    ///< H.265 QpMap mode
    E_VENC_BRC_MODE_MAX
} vs_venc_brc_mode_e;

/**
* @brief Defines the attribute of h264e vbr.
* @details
*/
typedef struct vs_venc_brc_h264_cbr {
    vs_uint32_t      gop;    ///< gop value of h.264, value range: [1, 65536]
    vs_uint32_t      bitrate_window;    ///< CBR statistics window (seconds), value range: [1, 60]
    vs_uint32_t      src_framerate;    ///< venc input frame rate (fps), value range: [1, VENC_MAX_INPUT_FRAME_RATE]
    vs_uint32_t      dst_framerate;    ///< venc output frame rate (fps), value range:  [1/64, VENC_MAX_INPUT_FRAME_RATE]
                                       ///< The denominator shall not be greater than 64 at fractional frame rate
                                       ///< The upper 16 bits represent the denominator and the lower 16 bits represent the numerator.
                                       ///< if the user sets the target frame rate as an integer, set the upper 16 bits of the denominator to 0 to represent an integer.
                                       ///< or example, if srcfrmrate is equal to 25 and dstframerate is equal to 12, it means that 12 frames will be taken out of the 25 input images for coding, and the remaining 13 frames will be lost.
                                       ///< if vifrmrate is set equal to 25 and dstframerate is set equal to 15/2, it means that the encoder is required to encode 15 frames in 2 seconds.
    vs_uint32_t      bitrate;    ///< the average output bitrate of the encoder (kbps). value range: [VENC_MIN_BITRATE, VENC_MAX_BITRATE]
} vs_venc_brc_h264_cbr_s;

/**
* @brief Defines the attribute of h264e vbr.
* @details
*/
typedef struct vs_venc_brc_h264_vbr {
    vs_uint32_t      gop;    ///< gop value of h.264, value range: [1, 65536]
    vs_uint32_t      bitrate_window;    ///< VBR statistics window (seconds), value range: [1, 60]
    vs_uint32_t      src_framerate;    ///< venc input frame rate (fps), value range: [1, VENC_MAX_INPUT_FRAME_RATE]
    vs_uint32_t      dst_framerate;    ///< venc output frame rate (fps), value range:  [1/64, VENC_MAX_INPUT_FRAME_RATE] specific reference vs_venc_brc_h264_cbr_s dst_framerate
    vs_uint32_t      max_bitrate;    ///< the maximum output bitrate of the encoder (kbps). value range: [VENC_MIN_BITRATE, VENC_MAX_BITRATE]
} vs_venc_brc_h264_vbr_s;

/**
* @brief Defines the attribute of h264e cvbr.
* @details
*/
typedef struct vs_venc_brc_h264_cvbr {
    vs_uint32_t      gop;    ///< gop value of h.264, value range: [1, 65536]
    vs_uint32_t      bitrate_window;    ///< CVBR statistics window (seconds), value range: [1, 60]
    vs_uint32_t      src_framerate;    ///< venc input frame rate (fps), value range: [1, VENC_MAX_INPUT_FRAME_RATE]
    vs_uint32_t      dst_framerate;    ///< venc output frame rate (fps), value range: [1/64, VENC_MAX_INPUT_FRAME_RATE] specific reference vs_venc_brc_h264_cbr_s dst_framerate
    vs_uint32_t      max_bitrate;    ///< the maximum output bitrate of the encoder (kbps). value range: [VENC_MIN_BITRATE, VENC_MAX_BITRATE]
    vs_uint32_t      short_term_bitrate_window;    ///< the bitrate of short-term statistical window (seconds), value range: [1, 120] 
    vs_uint32_t      long_term_bitrate_window;    ///< the bitrate of long-term statistical window (seconds), value range: [1, 1440] 
    vs_uint32_t      long_term_max_bitrate;    ///< the maximum long-term output bitrate of the encoder (kbps), value range: [2, max_bitrate]
    vs_uint32_t      long_term_min_bitrate;    ///< the minimum long-term output bitrate of the encoder (kbps), value range: [0, long_term_max_bitrate]
} vs_venc_brc_h264_cvbr_s;

/**
* @brief Defines the attribute of h264e avbr.
* @details
*/
typedef struct vs_venc_brc_h264_avbr {
    vs_uint32_t      gop;    ///< gop value of h.264, value range: [1, 65536]
    vs_uint32_t      bitrate_window;    ///< AVBR statistics window (seconds), value range: [1, 60]
    vs_uint32_t      src_framerate;    ///< venc input frame rate (fps), value range: [1, VENC_MAX_INPUT_FRAME_RATE]
    vs_uint32_t      dst_framerate;    ///< venc output frame rate (fps)value range:  [1/64, VENC_MAX_INPUT_FRAME_RATE] specific reference vs_venc_brc_h264_cbr_s dst_framerate
    vs_uint32_t      max_bitrate;    ///< the maximum output bitrate of the encoder (kbps). value range: [VENC_MIN_BITRATE, VENC_MAX_BITRATE]
} vs_venc_brc_h264_avbr_s;

/**
* @brief Defines the attribute of h264e qpmap.
* @details
*/
typedef struct vs_venc_brc_h264_qpmap {
    vs_uint32_t      gop;    ///< gop value of h.264, value range: [1, 65536]
    vs_uint32_t      bitrate_window;    ///< QPMAP statistics window (seconds), value range: [1, 60]
    vs_uint32_t      src_framerate;    ///< venc input frame rate (fps), value range: [1, VENC_MAX_INPUT_FRAME_RATE]
    vs_uint32_t      dst_framerate;    ///< venc output frame rate (fps), value range:  [1/64, VENC_MAX_INPUT_FRAME_RATE] specific reference vs_venc_brc_h264_cbr_s dst_framerate
} vs_venc_brc_h264_qpmap_s;


typedef vs_venc_brc_h264_cbr_s   vs_venc_brc_h265_cbr_s;
typedef vs_venc_brc_h264_vbr_s   vs_venc_brc_h265_vbr_s;
typedef vs_venc_brc_h264_avbr_s  vs_venc_brc_h265_avbr_s;
typedef vs_venc_brc_h264_cvbr_s  vs_venc_brc_h265_cvbr_s;
typedef vs_venc_brc_h264_fixqp_s vs_venc_brc_h265_fixqp_s;
typedef vs_venc_brc_h264_qpmap_s vs_venc_brc_h265_qpmap_s;

/**
* @brief Defines the attribute of mjpeg qpmap.
* @details
*/
typedef struct vs_venc_brc_mjpeg_fixqp {
    vs_uint32_t      src_framerate;    ///< venc input frame rate (fps), value range: [1, VENC_MAX_INPUT_FRAME_RATE]
    vs_uint32_t      dst_framerate;    ///< venc output frame rate (fps), value range:  [1/64, VENC_MAX_INPUT_FRAME_RATE] specific reference vs_venc_brc_h264_cbr_s dst_framerate
    vs_uint32_t      qfactor;          ///< qfactor used by mjpeg encoding, value range:  [0, 51]
} vs_venc_brc_mjpeg_fixqp_s;

/**
* @brief Defines the attribute of mjpeg cbr.
* @details
*/
typedef struct vs_venc_brc_mjpeg_cbr {
    vs_uint32_t      bitrate_window;    ///< CBR statistics window (seconds), value range: [1, 60]
    vs_uint32_t      src_framerate;     ///< venc input frame rate (fps), value range: [1, VENC_MAX_INPUT_FRAME_RATE]
    vs_uint32_t      dst_framerate;     ///< venc output frame rate (fps), value range:  [1/64, VENC_MAX_INPUT_FRAME_RATE] specific reference vs_venc_brc_h264_cbr_s dst_framerate
    vs_uint32_t      bitrate;           ///< the average output bitrate of the encoder (kbps). value range: [VENC_MIN_BITRATE, VENC_MAX_BITRATE]
} vs_venc_brc_mjpeg_cbr_s;

/**
* @brief Defines the attribute of mjpeg vbr.
* @details
*/
typedef struct vs_venc_brc_mjpeg_vbr {
    vs_uint32_t      bitrate_window;   ///< CBR statistics window (seconds), value range: [1, 60]
    vs_uint32_t      src_framerate;    ///< venc input frame rate (fps), value range: [1, VENC_MAX_INPUT_FRAME_RATE]
    vs_uint32_t      dst_framerate;    ///< venc output frame rate (fps), value range:  [1/64, VENC_MAX_INPUT_FRAME_RATE] specific reference vs_venc_brc_h264_cbr_s dst_framerate
    vs_uint32_t      max_bitrate;      ///< the maximum output bitrate of the encoder (kbps). value range: [VENC_MIN_BITRATE, VENC_MAX_BITRATE]
} vs_venc_brc_mjpeg_vbr_s;

/**
* @brief Defines the attribute of brc.
* @details
*/
typedef struct vs_venc_brc_attr {
    vs_venc_brc_mode_e            brc_mode;    ///< brc mode
    union {
        vs_venc_brc_h264_cbr_s    h264_cbr;    ///< cbr brc attribute for an H.264 channel
        vs_venc_brc_h264_vbr_s    h264_vbr;    ///< vbr brc attribute for an H.264 channel
        vs_venc_brc_h264_avbr_s   h264_avbr;    ///< avbr brc attribute for an H.264 channel
        vs_venc_brc_h264_cvbr_s   h264_cvbr;    ///< cvbr brc attribute for an H.264 channel
        vs_venc_brc_h264_fixqp_s  h264_fixqp;    ///< fixqp brc attribute for an H.264 channel
        vs_venc_brc_h264_qpmap_s  h264_qpmap;    ///< qpmap brc attribute for an H.264 channel

        vs_venc_brc_h265_cbr_s    h265_cbr;    ///< cbr brc attribute for an H.265 channel
        vs_venc_brc_h265_vbr_s    h265_vbr;    ///< vbr brc attribute for an H.265 channel
        vs_venc_brc_h265_avbr_s   h265_avbr;    ///< avbr brc attribute for an H.265 channel
        vs_venc_brc_h265_cvbr_s   h265_cvbr;    ///< cvbr brc attribute for an H.265 channel
        vs_venc_brc_h265_fixqp_s  h265_fixqp;    ///< fixqp brc attribute for an H.265 channel
        vs_venc_brc_h265_qpmap_s  h265_qpmap;    ///< qpmap brc attribute for an H.265 channel

        vs_venc_brc_mjpeg_cbr_s   mjpeg_cbr;    ///< cbr brc attribute for an mjpeg channel
        vs_venc_brc_mjpeg_vbr_s   mjpeg_vbr;    ///< vbr brc attribute for an mjpeg channel
        vs_venc_brc_mjpeg_fixqp_s mjpeg_fixqp;    ///< fixqp brc attribute for an mjpeg channel
    };
} vs_venc_brc_attr_s;

/**
* @brief Defines the gop mode.
* @details
*/
typedef enum vs_venc_gop_mode {
    E_VENC_GOP_MODE_NORMP  = 0,     ///< single reference frame P frame gop type.
    E_VENC_GOP_MODE_LTRP,     ///< long-term P frame: a internal definition special P frame, which only refers to the long-term reference frame
    E_VENC_GOP_MODE_DUALP,     ///< double-reference frame P frame gop type.Reserved.
    E_VENC_GOP_MODE_ADVP,     ///< encode the advanced smart P frame gop type.Reserved.
    E_VENC_GOP_MODE_NORMB,     ///< encodes the B frame gop type. NOT support on VS819L.
    E_VENC_GOP_MODE_LOWDELAYB,     ///< encodes the B frame gop type, where the B frame requires only the forward reference frame. NOT support on VS819L.
    E_VENC_GOP_MODE_MAX
} vs_venc_gop_mode_e;

/**
* @brief Defines the attribute of the normp.
* @details
*/
typedef struct vs_venc_gop_normp {
    vs_int32_t   qpdelta_i_p;     ///< the qp delta of the I frame relative to the P frame. value range[-10, 30].
} vs_venc_gop_normp_s;

/**
* @brief Defines the attribute of the ltrp.
* @details
*/
typedef struct vs_venc_gop_ltrp {
    vs_uint32_t interval_lp;     ///< interval of the long-term P frame. value range(1, gop)
    vs_int32_t  qpdelta_i_p;     ///< the qp delta of the I frame relative to the common P frames. value range[-10, 30].
    vs_int32_t  qpdelta_lp_p;    ///< the qp delta of the ltrp frame relative to the common P frames. value range[-10, 30].
    vs_bool_t   lp_pred_enable;  ///< whether the long-term p frame can be used as reference frame. value range[VS_FALSE, VS_TRUE].
} vs_venc_gop_ltrp_s;

/**
* @brief Defines the attribute of the dualp.
* @details .Reserved.
*/
typedef struct vs_venc_gop_dualp {
    vs_uint32_t interval_sp;     ///< interval of the special P frame. value range(1, MIN(48, gop)].
    vs_int32_t  qpdelta_sp_p;     ///< the qp delta of the special P frame relative to the common P frames.value range[-10, 30].
    vs_int32_t  qpdelta_i_p;     ///< the qp delta of the I frame relative to the common P frame. value range[-10, 30].
} vs_venc_gop_dualp_s;


/**
* @brief Defines the attribute of the advp.
* @details .Reserved.
*/
typedef struct vs_venc_gop_advp {
    vs_uint32_t interval_lp;     ///< interval of the long-term reference frames. value range[1, gop).
    vs_int32_t  qpdelta_i_p;     ///< the qp delta of the I frame relative to the common P frames. value range[-10, 30].
    vs_int32_t  qpdelta_lp_p;    ///< the qp delta of the ltrp frame relative to the common P frames. value range[-10, 30].
} vs_venc_gop_advp_s;

/**
* @brief Defines the attribute of the normb.
* @details.
* NOT support on VS819L.
*/
typedef struct vs_venc_gop_normb {
    vs_uint32_t frame_num_b;     ///< number of the B frame. value range[1, 3].
    vs_int32_t  qpdelta_b_p;     ///< the qp delta of the B frame relative to the common P frames. value range[-10, 30].
    vs_int32_t  qpdelta_i_p;     ///< the qp delta of the I frame relative to the common P frames. value range[-10, 30].
} vs_venc_gop_normb_s;

/**
* @brief Defines the attribute of the lowdylayb.
* @details.
* NOT support on VS819L.
*/
typedef struct vs_venc_gop_lowdelayb {
    vs_int32_t  qpdelta_i_b;     ///< the qp delta of the I frame relative to the B frame. value range[-10, 30].
} vs_venc_gop_lowdelayb_s;

/**
* @brief Defines the attribute of the gop.
* @details
*/
typedef struct vs_venc_gop_attr {
    vs_venc_gop_mode_e            mode;     ///< encoding for the GOP type.
    union {
        vs_venc_gop_normp_s       normp;     ///< the attribute of the normp.
        vs_venc_gop_ltrp_s        ltrp;     ///< the attribute of the ltrp.
        vs_venc_gop_dualp_s       dualp;     ///< the attribute of the dualp.Reserved.
        vs_venc_gop_advp_s        advp;     ///< the attribute of the advp.Reserved.
        vs_venc_gop_normb_s       normb;     ///< the attribute of the normb.NOT support on VS819L.
        vs_venc_gop_lowdelayb_s   lowdelayb;     ///< the attribute of the lowdylayb.NOT support on VS819L.
    };
} vs_venc_gop_attr_s;

/**
* @brief Defines the attribute of the venc channel.
* @details
*/
typedef struct vs_venc_chn_attr {
    vs_venc_enc_attr_s        enc_attr;    ///< attribute of venc channel
    vs_venc_brc_attr_s        brc_attr;     ///< attribute of bitrate control
    vs_venc_gop_attr_s        gop_attr;     ///< attribute of gop
} vs_venc_chn_attr_s;

/**
* @brief Defines the attribute of the receiving frame.
* @details
*/
typedef struct vs_venc_start_param {
    vs_uint32_t    rcvframe_num;    ///< number of frames received and encoded by the encoding channel.
} vs_venc_start_param_s;

/**
* @brief Defines the frame type and reference attributes of the H.264 & h.265 frame ref policy streams.
* @details
*/
typedef enum vs_venc_frame_ref_type {
    E_VENC_REF_TYPE_BASE_IDRSLICE = 0,    ///< IDR frames in the base layer.
    E_VENC_REF_TYPE_BASE_PSLICE_REFTOIDR,    ///< P frames in the base layer, reference by other frames in the base layer and only refer to IDR frames.
    E_VENC_REF_TYPE_BASE_PSLICE_REFBYBASE,    ///< P frames in the base layer, reference by other frames in the base layer
    E_VENC_REF_TYPE_BASE_PSLICE_REFBYENHANCE,    ///< P frames in the base layer,reference by other frames in the enhance layer
    E_VENC_REF_TYPE_ENHANCE_PSLICE_REFBYENHANCE,    ///< P frames in the enhance layer,reference by other frames in the enhance layer
    E_VENC_REF_TYPE_ENHANCE_PSLICE_NOTFORREF,    ///< P frames in the enhance layer,reference by nothing
    E_VENC_REF_TYPE_ENHANCE_PSLICE_MAX
} vs_frame_ref_type_e;

/**
* @brief Defines the venc output stream mode.
* @details
*/
typedef struct vs_venc_stream_info {
    vs_frame_ref_type_e frame_ref_type;    ///< encoding frame type under the advanced frame jump reference mode
    vs_uint32_t  frame_byte_num;    ///< number of bytes encoding the current frame
    vs_uint32_t  pic_cnt;    ///< picture encoded count;(represent frame count when acquiring packet by frame;represent packet count when acquiring packet by stream.)
    vs_uint32_t  pic_init_qp;    ///< initial qp value of the current frame encoding
    vs_uint32_t  mean_qp;    ///< mean qp value of the current frame encoding
    vs_bool_t    pskip_valid;    ///< identifies whether the current frame is a valid pskip frame
    vs_uint32_t  blk_bit_sum;    ///< total mb/lcu bit num of current frame
    vs_uint32_t  blk_num;    ///< mb/lcu num of current frame
} vs_venc_stream_info_s;

/**
* @brief Defines the status of the venc channel.
* @details
*/
typedef struct vs_venc_chn_status {
    vs_uint32_t          left_frame_num;            ///< number of frames waiting to be encoded
    vs_uint32_t          left_stream_byte_num;      ///< number of bytes in output stream buffer
    vs_uint32_t          left_stream_frame_num;     ///< number of frames in output stream buffer
    vs_uint32_t          cur_pack_num;              ///< number of packages in current frame
    vs_uint32_t          left_rcvframe_num;         ///< number of frames to be received
    vs_uint32_t          left_encframe_num;         ///< number of frames to be encoded
    vs_bool_t            jpeg_snap_end_valid;       ///< whether the snap process is finished in jpege snap mode
    vs_venc_stream_info_s stream_info;              ///< stream information
} vs_venc_chn_status_s;

/**
* @brief Defines the venc svct param.
* @details
*/
typedef struct vs_venc_svct {
    vs_bool_t    enable; ///< whether to enable svct encoding, value range : [VS_FALSE, VS_TRUE].default is VS_FALSE
    vs_int32_t   temporal_frame_num[VENC_MAX_TEMPORAL_LAYER_NUM]; ///< number of frames for every svct layer, need to be calculated by the user. must bigger than 0.
                                                                   ///< temporal_frame_num[0] * (temporal_frame_num[1] + 1) should not bigger than 48.
} vs_venc_svct_s;

/**
* @brief Defines the venc nalu type of H264.
* @details
*/
typedef enum vs_venc_nalu_type_h264 {
    E_VENC_NALU_H264_BSLICE = 0,     ///< the BSLICE type
    E_VENC_NALU_H264_PSLICE = 1,     ///< the PSLICE type
    E_VENC_NALU_H264_ISLICE = 2,     ///< the ISLICE type for the P frame
    E_VENC_NALU_H264_IDRSLICE = 5,     ///< the ISLICE type for the I frame
    E_VENC_NALU_H264_SEI    = 6,     ///< SEI
    E_VENC_NALU_H264_SPS    = 7,     ///< SPS
    E_VENC_NALU_H264_PPS    = 8,     ///< PPS
    E_VENC_NALU_H264_MAX
} vs_nalu_type_h264_e;

/**
* @brief Defines the venc nalu type of H265.
* @details
*/
typedef enum vs_venc_nalu_type_h265 {
    E_VENC_NALU_H265_BSLICE = 0,    ///< the BSLICE type
    E_VENC_NALU_H265_PSLICE = 1,    ///< the PSLICE type
    E_VENC_NALU_H265_ISLICE = 2,    ///< the ISLICE type for the P frame
    E_VENC_NALU_H265_IDRSLICE = 19,    ///< the ISLICE type for the I frame
    E_VENC_NALU_H265_VPS    = 32,    ///< VPS
    E_VENC_NALU_H265_SPS    = 33,    ///< SPS
    E_VENC_NALU_H265_PPS    = 34,    ///< PPS
    E_VENC_NALU_H265_SEI    = 39,    ///< SEI
    E_VENC_NALU_H265_MAX
} vs_nalu_type_h265_e;


/**
* @brief Defines the pack type of JPEGE.
* @details
*/
typedef enum vs_venc_pack_type_jpeg{
    E_VENC_PACK_JPEG_ECS = 5,    ///< ECS types.
    E_VENC_PACK_JPEG_APP = 6,    ///< APP types. Reserved.
    E_VENC_PACK_JPEG_VDO = 7,    ///< VDO types. Reserved.
    E_VENC_PACK_JPEG_PIC = 8,    ///< PIC types
    E_VENC_PACK_JPEG_MAX
} vs_pack_type_jpeg_e;


/**
* @brief Defines the data type of VENC.
* @details
*/
typedef union vs_venc_data_type {
    vs_nalu_type_h264_e    h264_type;    ///< the H.264 stream packet type
    vs_pack_type_jpeg_e    jpeg_type;    ///< the JPEG stream packet type
    vs_nalu_type_h265_e    h265_type;    ///< the H.265 stream packet type
} vs_venc_data_type_u;



/**
* @brief Defines the pack info of VENC.
* @details
*/
typedef struct vs_venc_pack_info {
    vs_venc_data_type_u  pack_type;    ///< current stream packet data contains types of other stream packets.
    vs_uint32_t          pack_offset;    ///< current stream packet data contains an offset of other stream packets.
    vs_uint32_t          pack_len;    ///<  current stream packet data contains size of other stream packets.
} vs_venc_pack_info_s;

/**
* @brief Defines a stream packet.
* @details
*/
typedef struct vs_venc_pack {
    vs_int64_t            phys_addr;    ///< stream packet physical address
    vs_uint8_t            *virt_addr;    ///< virtual address of the code stream packet
    vs_uint32_t           length;    ///<  stream packet length.
    vs_int64_t            pts;    ///< time stamp (us)
    vs_bool_t             is_frame_end;    ///< flag of frame end
    vs_venc_data_type_u   nalu_type;    ///< stream type, support H.264/JPEG/H.265.
    vs_uint32_t           offset;    ///< the offset of the effective data relative to the first address p_addr of the stream packet.
    vs_uint32_t           data_num;    ///< the number of other types of current stream packets data. (type of current packet types specified by data_type)
    vs_venc_pack_info_s   pack_info[VENC_MAX_NALU_NUM_IN_ONE_PACK];    ///< current stream packet data contains other types of code stream packet information.
} vs_venc_pack_s;

/**
* @brief Defines the features of an H.264 stream.
* @details
*/
typedef struct vs_venc_stream_h264_info {
    vs_uint32_t frame_byte_num;    ///< number of bytes encoding the current frame
    vs_uint32_t inter_16x16_num;    ///< encodes the number of macroblocks using the inter16x16 prediction mode in the current frame
    vs_uint32_t inter_8x8_num;    ///< encodes the number of macroblocks using the inter8x8 prediction mode in the current frame
    vs_uint32_t intra_16x16_num;    ///< encodes the number of macroblocks using the intra16x16 prediction mode in the current frame
    vs_uint32_t intra_8x8_num;    ///< encodes the number of macroblocks using the intra8x8 prediction mode in the current frame
    vs_uint32_t intra_4x4_num;    ///< encodes the number of macroblocks using the intra4x4 prediction mode in the current frame
    vs_frame_ref_type_e frame_ref_type;    ///< encoding frame type under the advanced frame jump reference mode
    vs_uint32_t attr_set_cnt;     ///< number of times that the channel attribute or parameters were set
    vs_uint32_t pic_init_qp;    ///< initial qp value of the current frame encoding
    vs_uint32_t mean_qp;    ///< mean qp value of the current frame encoding
    vs_bool_t   pskip_valid;    ///< identifies whether the current frame is a valid pskip frame
    vs_uint32_t mb_bit_sum;    ///< total mb bit num of current frame
    vs_uint32_t mb_num;    ///< mb num in the current frame
    vs_uint16_t qp_histogram[VENC_QP_HISTOGRAM_NUM]; ///<the venc qp histogram value
    vs_uint32_t mean_pixel;   ///< the mean value of the corresponding block in the original image
} vs_venc_stream_h264_info_s;

/**
* @brief Defines the features of an H.265 stream.
* @details
*/
typedef struct vs_venc_stream_h265_info {
    vs_uint32_t frame_byte_num;    ///< number of bytes encoding the current frame
    vs_uint32_t inter_64x64_num;    ///< encodes the number of macroblocks using the inter64x64 prediction mode in the current frame
    vs_uint32_t inter_32x32_num;    ///< encodes the number of macroblocks using the inter32x32 prediction mode in the current frame
    vs_uint32_t inter_16x16_num;    ///< encodes the number of macroblocks using the inter16x16 prediction mode in the current frame
    vs_uint32_t inter_8x8_num;    ///< encodes the number of macroblocks using the inter8x8 prediction mode in the current frame
    vs_uint32_t intra_32x32_num;    ///< encodes the number of macroblocks using the intra32x32 prediction mode in the current frame
    vs_uint32_t intra_16x16_num;    ///< encodes the number of macroblocks using the intra16x16 prediction mode in the current frame
    vs_uint32_t intra_8x8_num;    ///< encodes the number of macroblocks using the intra8x8 prediction mode in the current frame
    vs_uint32_t intra_4x4_num;    ///< encodes the number of macroblocks using the intra4x4 prediction mode in the current frame
	vs_frame_ref_type_e frame_ref_type;    ///< encoding frame type under the advanced frame jump reference mode
    vs_uint32_t attr_set_cnt;     ///< number of times that the channel attribute or parameters were set.
    vs_uint32_t pic_init_qp;    ///< initial qp value of the current frame encoding
    vs_uint32_t mean_qp;    ///< mean qp value of the current frame encoding
    vs_bool_t   pskip_valid;    ///< identifies whether the current frame is a valid pskip frame
    vs_uint32_t lcu_bit_sum;    ///< total lcu bit num of current frame
    vs_uint32_t lcu_num;    ///< lcu num of current frame
    vs_uint16_t qp_histogram[VENC_QP_HISTOGRAM_NUM]; ///<the venc qp histogram value
    vs_uint32_t mean_pixel;   ///< the mean value of the corresponding block in the original image
} vs_venc_stream_h265_info_s;


/**
* @brief Defines the features of an jpege stream.
* @details
*/
typedef struct vs_venc_stream_jpeg_info {
    vs_uint32_t frame_byte_num;   ///< number of bytes encoding the current frame
    vs_uint32_t attr_set_cnt;     ///< number of times that the channel attribute or parameters were set.
    vs_uint32_t qfactor;          ///< the qfactor encoding current frame
} vs_venc_stream_jpeg_info_s;

/**
* @brief Defines the output stream information.
* @details
*/
typedef struct vs_venc_stream {
    vs_venc_pack_s    *p_pack;    ///< structure of stream frame .
    vs_uint32_t       pack_num;    ///< number of all stream packages for a frame.value range[1, +∞] as input.
    							   ///< [0, input_pack_num] as output,input_pack_num is caller filled.
    vs_uint32_t      seq;    ///< sequence number of frame.output value, no limit.

    union {
        vs_venc_stream_h264_info_s   h264_info;    ///< H.264 stream feature information.
        vs_venc_stream_h265_info_s   h265_info;    ///< H.265 stream feature information.
        vs_venc_stream_jpeg_info_s   jpeg_info;    ///< JPEG stream feature information.
    };
} vs_venc_stream_s;

/**
* @brief Defines the venc sei userdata info.
* @details
*/
typedef struct vs_venc_sei_userdata {
    vs_uint8_t *p_addr;   ///< virtual address of user data in sei
    vs_int32_t len;           ///< length of user data.Value must be in (0,max_sei_userdata_len - 16].
                              ///< max_sei_userdata_len refers to vs_venc_h264_mod_s or vs_venc_h265_mod_s
} vs_venc_sei_userdata_s;

/**
* @brief Defines the frame type.
* @details
*/
typedef enum vs_venc_qpmap_frame_type {
    E_VENC_QPMAP_FRAME_TYPE_NONE = 0,     ///< current frame encoding type is not specified, automatically determined  by venc
    E_VENC_QPMAP_FRAME_TYPE_IDR,          ///< current frame encoding type is IDR
    E_VENC_QPMAP_FRAME_TYPE_PSKIP,        ///< current frame encoding type is p-skip
    E_VENC_QPMAP_FRAME_TYPE_MAX
} vs_venc_qpmap_frame_type_e;

/**
* @brief Defines the information of the qpmapmap & skipmap.
* @details
*/
typedef enum vs_venc_map_blk_unit {
	E_VENC_MAP_BLK_UNIT_64X64 = 0,    ///< 64x64 block unit size, roimap & skipmap valid for h.265
	E_VENC_MAP_BLK_UNIT_32X32,        ///< 32x32 block unit size, roimap & skipmap valid for h.265
	E_VENC_MAP_BLK_UNIT_16X16,       ///< 16x16 block unit size, roimap & skipmap valid for h.264,roimap for h.265
	E_VENC_MAP_BLK_UNIT_MAX
} vs_venc_map_blk_unit_e;

/**
* @brief Defines the information of the qpmap.
* @details
*/
typedef struct vs_venc_qpmap_info {
    vs_bool_t                   skipmap_enable;   ///< whether to enable skipmap encoding.
    vs_int32_t                  first_blk_qp;     ///< fisrt blk qp value in qpmap mode
    vs_venc_map_blk_unit_e      blk_unit;         ///< enum of block unit size.must the same as rc_blk_size
    vs_uint64_t                 map_phys_addr;    ///< physical address of map table
    vs_uint32_t                 map_size;         ///< map table buf size
    vs_venc_qpmap_frame_type_e  frame_type;       ///< type of current picture
} vs_venc_qpmap_info_s;


/**
* @brief Defines the information of frame with qpmap.
* @details
*/
typedef struct vs_venc_qpmap_frame_info {
    vs_video_frame_info_s    user_frame_info;    ///< frame data
    vs_venc_qpmap_info_s     qpmap_info;    ///< qp map of frame data in encoding
} vs_venc_qpmap_frame_info_s;

/**
* @brief Defines the attribute of the roi.
* @details
*/
typedef struct vs_venc_roi_attr {
    vs_int32_t  index;             ///< index of roi. value range[0, VENC_MAX_ROI_NUM).
    vs_bool_t    enable[VENC_MAX_ROI_FRAME_TYPE_NUM];         ///< whether to enable roi, 0/1/2th element represents I/P/B respectively,
                                    ///< if only set 0th element, which means it valid for all frame type
    vs_bool_t    is_absqp[VENC_MAX_ROI_FRAME_TYPE_NUM];   ///< qp mode of roi, false means relative mode, true means absolute mode
    vs_int32_t   qp[VENC_MAX_ROI_FRAME_TYPE_NUM];             ///< qp value.value range[0, 51] when absolute qp.value range[-30, 0]when relative qp.
    vs_rect_s    rect[VENC_MAX_ROI_FRAME_TYPE_NUM];           ///< rectangle region.x、y、width、height should 16 pixel align for h.264,64 pixel align for h.265.
    														  ///< width、height should not be 0 if roi enabled.
} vs_venc_roi_attr_s;

/**
* @brief Defines the param of the roi background frame rate control.
* @details
*/
typedef struct vs_venc_roi_bg_framerate {
    vs_int32_t   src_framerate;   ///< source frame rate of roi_bg. value range:  -1 or [1, VENC_MAX_INPUT_FRAME_RATE]. should be the same with src_framerate in struct vs_venc_brc_attr_s.
    vs_int32_t   dst_framerate;   ///< target frame rate of roi_bg. value range:  -1 or [1, src_framerate]
} vs_venc_roi_bg_framerate_s;
/**
* @brief Defines the param of the h264e slice split.
* @details
*/
typedef struct vs_venc_h264_slice_split {
    vs_bool_t     enable;       ///< whether to enable slice split encoding.value range : [VS_FALSE, VS_TRUE].default is VS_FALSE.
    vs_uint32_t   mb_line_num;  ///< number of mb lines for each slice.value range[1, max_mbline_num].max_mbline_num means mbline numbers of whole picture.
} vs_venc_h264_slice_split_s;

/**
* @brief Defines the param of the h264e transform.
* @details
*/
typedef struct vs_venc_h264_trans {
    vs_bool_t       scalinglist_valid;          ///< whether to enable scaling list.value range : [VS_FALSE, VS_TRUE].default is VS_FALSE,Reserved.
    vs_int32_t      chroma_qp_index_offset;     ///< for more information refer to ITU-T H.264 value range[-12, 12],default -6
} vs_venc_h264_trans_s;

/**
* @brief Defines the mode of the h264e entropy coding.
* @details
*/
typedef enum vs_venc_entropy_mode {
    E_VENC_ENTROPY_MODE_CAVLC = 0,  ///< entropy coding using CAVLC
    E_VENC_ENTROPY_MODE_CABAC,      ///< entropy coding using CABAC
    E_VENC_ENTROPY_MODE_MAX
} vs_venc_entropy_mode_e;

/**
* @brief Defines the param of the h264e entropy.
* @details only support set i\b\p the same entroy mode now
*/
typedef struct vs_venc_h264_entropy {
    vs_venc_entropy_mode_e mode_i;  ///< entropy coding mode of I frame, baseline does not support CABAC
    vs_venc_entropy_mode_e mode_p;  ///< entropy coding mode of P frame, baseline does not support CABAC
    vs_venc_entropy_mode_e mode_b;  ///< entropy coding mode of B frame, baseline does not support CABAC & B frame
} vs_venc_h264_entropy_s;

/**
* @brief Defines the param of the h264e deblocking.
* @details
*/
typedef struct vs_venc_h264_dblk {
    vs_uint32_t disable_deblocking_filter_idc;  ///< for more information refer to ITU-T H.264 value range [0,1],default is 0
    vs_int32_t slice_alpha_c0_offset_div2;     ///< for more information refer to ITU-T H.264 value range [-6,6],default is 0
    vs_int32_t slice_beta_offset_div2;         ///< for more information refer to ITU-T H.264 value range [-6,6],default is 0
} vs_venc_h264_dblk_s;

/**
* @brief Defines the param of the h264e vui timing info.
* @details
*/
typedef struct vs_venc_vui_time_info_h264 {
    vs_uint8_t     timing_info_present_flag;    ///< for more information refer to ITU-T H.264 value range [0,1],default is 0
    vs_uint8_t     fixed_frame_rate_flag;        ///< for more information refer to ITU-T H.264,value range [0,1],default is 1
    vs_uint32_t    num_units_in_tick;           ///< for more information refer to ITU-T H.264,num_units_in_tick >= 0,default is 1
    vs_uint32_t    time_scale;                  ///< for more information refer to ITU-T H.264,time_scale >= 0,default is 60
} vs_venc_vui_time_info_h264_s;

/**
* @brief Defines the param of the param of the vui aspct ratio.
* @details
*/
typedef struct vs_venc_vui_aspect_ratio {
    vs_uint8_t     aspect_ratio_info_present_flag;  ///< for more information refer to ITU-T H.264/H.265,value range [0,1],default is 0
    vs_uint8_t     aspect_ratio_idc;                ///< for more information refer to ITU-T H.264/H.265,value range [0,255],default is 1
    vs_uint8_t     overscan_info_present_flag;      ///< for more information refer to ITU-T H.264/H.265,value range [0,1],default is 0
    vs_uint8_t     overscan_appropriate_flag;       ///< for more information refer to ITU-T H.264/H.265,value range [0,1],default is 0
    vs_uint16_t    sar_width;                       ///< for more information refer to ITU-T H.264/H.265,value range (0,65535] default is 1  and mutually exclusive with sar_height
    vs_uint16_t    sar_height;                      ///< for more information refer to ITU-T H.264/H.265,value range (0,65535] default is 1  and mutually exclusive with sar_width
} vs_venc_vui_aspect_ratio_s;

/**
* @brief Defines the param of the param of the vui aspct ratio.
* @details
*/
typedef struct vs_venc_vui_video_signal {
    vs_uint8_t  video_signal_type_present_flag;     ///< for more information refer to ITU-T H.264/H.265 value range [0,1],default is 1
    vs_uint8_t  video_format;                       ///< for more information refer to ITU-T H.264 value range [0,7],default is 5
                                                    ///< for more information refer to ITU-T H.265 value range [0,5],default is 5
    vs_uint8_t  video_full_range_flag;              ///< for more information refer to ITU-T H.264/H.265 value range [0,1],default is 1
    vs_uint8_t  colour_description_present_flag;    ///< for more information refer to ITU-T H.264/H.265 value range [0,1],default is 1,read only
    vs_uint8_t  colour_primaries;                   ///< for more information refer to ITU-T H.264/H.265 value range [0,255],default is 1,read only
    vs_uint8_t  transfer_characteristics;           ///< for more information refer to ITU-T H.264/H.265 value range [0,255],default is 1,read only
    vs_uint8_t  matrix_coefficients;                ///< for more information refer to ITU-T H.264/H.265 value range [0,255],default is 1,read only
} vs_venc_vui_video_signal_s;

/**
* @brief Defines the param of the vui video signal.
* @details
*/
typedef struct vs_venc_vui_bitstream_restric {
    vs_uint8_t  bitstream_restriction_flag;     ///< for more information refer to ITU-T H.264/H.265,value range [0,1],default is 0
} vs_venc_vui_bitstream_restric_s;

/**
* @brief Defines the param of the h264e vui.
* @details
*/
typedef struct vs_venc_h264_vui {
    vs_venc_vui_aspect_ratio_s      aspect_ratio;       ///< for more information refer to ITU-T H.264
    vs_venc_vui_time_info_h264_s    time_info;          ///< for more information refer to ITU-T H.264
    vs_venc_vui_video_signal_s      video_signal;       ///< for more information refer to ITU-T H.264
    vs_venc_vui_bitstream_restric_s bitstream_restric;  ///< for more information refer to ITU-T H.264
} vs_venc_h264_vui_s;

/**
* @brief Defines the param of the h265e vui timing info.
* @details
*/
typedef struct vs_venc_vui_time_info_h265 {
    vs_uint32_t timing_info_present_flag;       ///< for more information refer to ITU-T H.265 value range [0,1],default is 0
    vs_uint32_t num_units_in_tick;              ///< for more information refer to ITU-T H.265 num_units_in_tick >= 0,default is 1
    vs_uint32_t time_scale;                     ///< for more information refer to ITU-T H.265 time_scale >= 0,default is 60
    vs_uint32_t num_ticks_poc_diff_one_minus1;  ///< for more information refer to ITU-T H.265 value range [0, 2^32-2],default is 1
} vs_venc_vui_time_info_h265_s;


/**
* @brief Defines the param of the h265e vui.
* @details
*/
typedef struct vs_venc_h265_vui {
    vs_venc_vui_aspect_ratio_s      aspect_ratio;       ///< for more information refer to ITU-T H.265
    vs_venc_vui_time_info_h265_s    time_info;          ///< for more information refer to ITU-T H.265
    vs_venc_vui_video_signal_s      video_signal;       ///< for more information refer to ITU-T H.265
    vs_venc_vui_bitstream_restric_s bitstream_restric;  ///< for more information refer to ITU-T H.265
} vs_venc_h265_vui_s;



/**
* @brief Defines the param of the jpege.
* @details
*/
typedef struct vs_venc_jpeg_param {
    vs_uint32_t qfactor;    ///< quantization factor range[0,52].means user quantification table if this is 52.default is 4 
    vs_uint8_t  y_qt[VENC_SCALINGLIST_BLK_SIZE];   ///< quantization table of y component. enable if qfactor equal to 52
    vs_uint8_t  cb_qt[VENC_SCALINGLIST_BLK_SIZE];  ///< quantization table of cb component. enable if qfactor equal to 52
    vs_uint8_t  cr_qt[VENC_SCALINGLIST_BLK_SIZE];  ///< quantization table of cr component. enable if qfactor equal to 52
    vs_uint32_t mcu_per_ecs;    				   ///< Reserved.
} vs_venc_jpeg_param_s;

/**
* @brief Defines the param of the mjpege.
* @details
*/
typedef struct vs_venc_mjpeg_param {
    vs_uint8_t  y_qt[VENC_SCALINGLIST_BLK_SIZE];       ///< quantization table of y component
    vs_uint8_t  cb_qt[VENC_SCALINGLIST_BLK_SIZE];      ///< quantization table of cb component
    vs_uint8_t  cr_qt[VENC_SCALINGLIST_BLK_SIZE];      ///< quantization table of cr component
    vs_uint32_t mcu_per_ecs;    					   ///< Reserved.
} vs_venc_mjpeg_param_s;

/**
* @brief Defines The param of H264e cbr.
* @details
*/
typedef struct vs_venc_brc_h264_cbr_param {
    vs_uint32_t  min_pct_i;    ///< minimum percentage of I frames bits versus P frames bits , value range: [1, 100]
    vs_uint32_t  max_pct_i;    ///< maximum percentage of I frames bits versus P frames bits , value range: [1, 100], default is 100
    vs_uint32_t  max_qp;    ///< maximum qp of frame, used for controlling image quality, Value range: [0, 51], default is 51
    vs_uint32_t  min_qp;    ///< minimum qp of frame, used for clamp bitrate fluctuations, Value range: [0, max_qp], default is 10
    vs_uint32_t  max_qp_i;    ///< minimum qp of I frame, which is used to control the maximum number of bits of an I frame, value range: [0, 51], default is 45
    vs_uint32_t  min_qp_i;    ///< maximum qp of I frame, which is used to control the minimum number of bits of an I frame, value range: [0, max_qp_i], default is 10
    vs_bool_t    qpmap_enable;        ///< whether to enable qpmap function in cbr mode, value range: [VS_FALSE, VS_TRUE], default is VS_FALSE
} vs_venc_brc_h264_cbr_param_s;
/**
* @brief Defines The param of H264e vbr.
* @details
*/
typedef struct vs_venc_brc_h264_vbr_param {
    vs_int32_t   chg_pos;    ///< when vbr mode starts to adjust qp, the ratio of the current bitrate to the maximum bitrate. value range: [50, 100], default is 90
    vs_uint32_t  min_pct_i;    ///< minimum percentage of I frames bits versus P frames bits , value range: [1, 100], default is 1
    vs_uint32_t  max_pct_i;    ///< maximum percentage of I frames bits versus P frames bits , value range: [1, 100], default is 100
    vs_uint32_t  max_qp;    ///< maximum qp of B frames and P frames, Value range: [0, 51], default is 51
    vs_uint32_t  min_qp;    ///< minimum qp of B frames and P frames, Value range: [0, max_qp], default is 24
    vs_uint32_t  max_qp_i;    ///< maximum qp of the I frames, Value range: [0, 51], default is 51
    vs_uint32_t  min_qp_i;    ///< minimum qp of the I frames, Value range: [0, max_qp_i], default is 24
    vs_bool_t    qpmap_enable;    ///< whether to enable qpmap function in vbr mode, value range: [VS_FALSE, VS_TRUE], default is VS_FALSE
} vs_venc_brc_h264_vbr_param_s;

/**
* @brief Defines The param of H264e avbr.
* @details
*/
typedef struct vs_venc_brc_h264_avbr_param {
    vs_int32_t   chg_pos;    ///< when avbr mode starts to adjust qp, the ratio of the current bitrate to the maximum bitrate. value range: [50, 100], default is 90
    vs_uint32_t  min_pct_i;    ///< minimum percentage of I frames bits versus P frames bits , value range: [1, 100], default is 1
    vs_uint32_t  max_pct_i;    ///< maximum percentage of I frames bits versus P frames bits , value range: [1, 100], default is 100
    vs_bool_t    qpmap_enable;    ///< whether to enable qpmap function in avbr mode, value range: [VS_FALSE, VS_TRUE], default is VS_FALSE
    vs_uint32_t  max_qp;    ///< maximum qp of B frames and P frames, Value range: [0, 51], default is 51
    vs_uint32_t  min_qp;    ///< minimum qp of B frames and P frames, Value range: [0, max_qp], default is 24
    vs_uint32_t  max_qp_i;    ///< maximum qp of the I frames, Value range: [0, 51], default is 51
    vs_uint32_t  min_qp_i;    ///< minimum qp of the I frames, Value range: [0, max_qp_i], default is 24
    vs_int32_t   min_still_pct;    ///< minimum percentage of target bit rate in still scene. if this variable is set to 100, the target bitrate in avbr mode will not actively reduce in static scene. value range: [5, 100], default 25
    vs_uint32_t  max_still_qp;    ///< maximum qp for the static scene. value range: [min_qp_i, max_qp_i], default is 35
    vs_uint32_t  min_still_psnr;    ///< invalid, just set to 0.
    vs_int32_t   min_qpdelta;    ///< the difference between the minimum qp value at the Cu level and the minimum qp value at the frame level. value range: [-10, -1], default is -3
    vs_uint32_t  motion_factor;    ///< motion sensitivity. value range: [0, 100], default is 100
} vs_venc_brc_h264_avbr_param_s;


/**
* @brief Defines The param of H264e cvbr.
* @details
*/
typedef struct vs_venc_brc_h264_cvbr_param {
    vs_uint32_t  min_pct_i;    ///< minimum percentage of I frames bits versus P frames bits , value range: [1, 100], default is 1
    vs_uint32_t  max_pct_i;    ///< maximum percentage of I frames bits versus P frames bits , value range: [1, 100], default is 100
    vs_bool_t    qpmap_enable;    ///< whether to enable qpmap function in avbr mode, value range: [VS_FALSE, VS_TRUE], default is VS_FALSE
    vs_uint32_t  max_qp;    ///< maximum qp of B frames and P frames, Value range: [0, 51], default is 47
    vs_uint32_t  min_qp;    ///< minimum qp of B frames and P frames, Value range: [0, max_qp], default is 22
    vs_uint32_t  max_qp_i;    ///< maximum qp of the I frames, Value range: [0, 51], default is 47
    vs_uint32_t  min_qp_i;    ///< minimum qp of the I frames, Value range: [0, max_qp_i], default is 20
    vs_int32_t   min_qpdelta;    ///< the difference between the minimum qp value at the Cu level and the minimum qp value at the frame level.value range: [-10, -1], default is -3
    vs_int32_t   max_qpdelta;    ///< the difference between the maximum qp value at the Cu level and the maximum qp value at the frame level.value range: [1, 10], default is 3
    vs_uint32_t  overrun_bit_pct;    ///< percentage of the maximum overrun bits for the streams output from the. value range: [0, 1000]. default is 5
    vs_uint32_t  long_term_bitrate_window_unit;    ///< the unit of bit rate long-term statistical time parameter (long_term_bitrate_window). 
    											   ///< The unit of this variable is seconds (s). 
												   ///< e.g if long_term_bitrate_window_unit is set to 60 and long_term_bitrate_window set to 2, 
												   ///< it indicates that the long-term statistical period is 2 minutes. value range: [1, 1800], default is 60
} vs_venc_brc_h264_cvbr_param_s;

/**
* @brief Defines The param of H265e cbr.
* @details
*/
typedef struct vs_venc_brc_h265_cbr_param {
    vs_uint32_t  min_pct_i;    ///< minimum percentage of I frames bits versus P frames bits , value range: [1, 100], default is 1
    vs_uint32_t  max_pct_i;    ///< maximum percentage of I frames bits versus P frames bits , value range: [1, 100], default is 100
    vs_uint32_t  max_qp;       ///< maximum qp of B frames and P frames, Value range: [0, 51], default is 51
    vs_uint32_t  min_qp;       ///< minimum qp of B frames and P frames, Value range: [0, max_qp], default is 10
    vs_uint32_t  max_qp_i;     ///< maximum qp of the I frames, Value range: [0, 51], default is 45
    vs_uint32_t  min_qp_i;     ///< minimum qp of the I frames, Value range: [0, max_qp_i], default is 10
    vs_bool_t    qpmap_enable;        ///< whether to enable qpmap function in cbr mode, value range: [VS_FALSE, VS_TRUE], default is VS_FALSE
} vs_venc_brc_h265_cbr_param_s;

/**
* @brief Defines The param of H265e vbr.
* @details
*/
typedef struct vs_venc_brc_h265_vbr_param {
    vs_int32_t   chg_pos;    ///< when vbr mode starts to adjust qp, the ratio of the current bitrate to the maximum bitrate. value range: [50, 100], default is 90
    vs_uint32_t  min_pct_i;    ///< minimum percentage of I frames bits versus P frames bits , value range: [1, 100], default is 1
    vs_uint32_t  max_pct_i;    ///< maximum percentage of I frames bits versus P frames bits , value range: [1, 100], default is 100
    vs_uint32_t  max_qp;    ///< maximum qp of B frames and P frames, Value range: [0, 51], default is 51
    vs_uint32_t  min_qp;    ///< minimum qp of B frames and P frames, Value range: [0, max_qp], default is 24
    vs_uint32_t  max_qp_i;    ///< maximum qp of the I frames, Value range: [0, 51], default is 51
    vs_uint32_t  min_qp_i;    ///< minimum qp of the I frames, Value range: [0, max_qp_i], default is 24
    vs_bool_t    qpmap_enable;    ///< whether to enable qpmap function in cbr mode, value range: [VS_FALSE, VS_TRUE], default is VS_FALSE
} vs_venc_brc_h265_vbr_param_s;

/**
* @brief Defines The param of H265e avbr.
* @details
*/
typedef struct vs_venc_brc_h265_avbr_param {
    vs_int32_t   chg_pos;    ///< when vbr mode starts to adjust qp, the ratio of the current bitrate to the maximum bitrate. value range: [50, 100], default is 90
    vs_uint32_t  min_pct_i;    ///< minimum percentage of I frames bits versus P frames bits , value range: [1, 100], default is 1
    vs_uint32_t  max_pct_i;    ///< maximum percentage of I frames bits versus P frames bits , value range: [1, 100], default is 100
    vs_int32_t   min_still_pct;   ///< minimum percentage of target bit rate in still scene. if this variable is set to 100, the target bitrate in avbr mode will not actively reduce in static scene. value range: [5, 100], default 25
    vs_uint32_t  max_still_qp;    ///< maximum qp for the static scene. value range: [min_qp_i, max_qp_i], default is 35
    vs_uint32_t  min_still_psnr;    ///< invalid, just set to 0.
    vs_uint32_t  max_qp;    ///< maximum qp of B frames and P frames, Value range: [0, 51], default is 51
    vs_uint32_t  min_qp;    ///< minimum qp of B frames and P frames, Value range: [0, max_qp], default is 16
    vs_uint32_t  max_qp_i;    ///< maximum qp of the I frames, Value range: [0, 51], default is 51
    vs_uint32_t  min_qp_i;    ///< minimum qp of the I frames, Value range: [0, max_qp_i], default is 16
    vs_int32_t   min_qpdelta;    ///< the difference between the minimum qp value at the Cu level and the minimum qp value at the frame level. value range: [-10, -1], default is -3
    vs_uint32_t   motion_factor;    ///< motion sensitivity. value range: [0, 100], default is 100
    vs_bool_t    qpmap_enable;    ///< whether to enable qpmap function in cbr mode, value range: [VS_FALSE, VS_TRUE], default is VS_FALSE
} vs_venc_brc_h265_avbr_param_s;

/**
* @brief Defines The param of H265e cvbr.
* @details
*/
typedef struct vs_venc_brc_h265_cvbr_param {
    vs_uint32_t  min_pct_i;    ///< minimum percentage of I frames bits versus P frames bits , value range: [1, 100], default is 1
    vs_uint32_t  max_pct_i;    ///< maximum percentage of I frames bits versus P frames bits , value range: [1, 100], default is 100
    vs_uint32_t  max_qp;    ///< maximum qp of B frames and P frames, Value range: [0, 51], default is 47
    vs_uint32_t  min_qp;    ///< minimum qp of B frames and P frames, Value range: [0, max_qp], default is 22
    vs_uint32_t  max_qp_i;    ///< maximum qp of the I frames, Value range: [0, 51], default is 47
    vs_uint32_t  min_qp_i;    ///< minimum qp of the I frames, Value range: [0, max_qp_i], default is 20
    vs_int32_t   min_qpdelta;    ///< the difference between the minimum qp value at the Cu level and the minimum qp value at the frame level.
                                 ///< value range: [-10, -1], default is -3
    vs_int32_t   max_qpdelta;    ///< the difference between the maximum qp value at the Cu level and the maximum qp value at the frame level.value range: [1, 10], default is 3
    vs_uint32_t  overrun_bit_pct;    ///< the maximum overrun bits percentage of the streams output from the encoder. value range: [0, 1000]. default is 5
    vs_uint32_t  long_term_bitrate_window_unit;     ///< the unit of bit rate long-term statistical time parameter (long_term_bitrate_window). The unit of this variable is seconds (s). 
    												///< e.g if long_term_bitrate_window_unit is set to 60 and long_term_bitrate_window set to 2, 
    												///< it indicates that the long-term statistical period is 2 minutes. value range: [1, 1800], default is 60
    vs_bool_t    qpmap_enable;     ///< whether to enable qpmap function in cbr mode, value range: [VS_FALSE, VS_TRUE], default is VS_FALSE
} vs_venc_brc_h265_cvbr_param_s;

/**
* @brief Defines The param of mjpege cbr.
* @details
*/
typedef struct vs_venc_brc_mjpeg_cbr_param {
    vs_uint32_t  max_qfactor;    ///< frame maximum qfactor, is used for controlling the pic quality. value range: [1, 51], default is 40
    vs_uint32_t  min_qfactor;    ///< frame minimum qfactor, is used for controlling the pic quality. value range: [0, max_qfactor], default is 1
} vs_venc_brc_mjpeg_cbr_param_s;

/**
* @brief Defines The param of mjpege vbr.
* @details
*/
typedef struct vs_venc_brc_mjpeg_vbr_param {
    vs_int32_t  chg_pos;        ///< when vbr mode starts to adjust qfactor, the ratio of the current bitrate to the maximum bitrate. value range: [50, 100], default is 90
    vs_uint32_t max_qfactor;    ///< maximum quantization factor, Value range: [1, 51], default is 40
    vs_uint32_t min_qfactor;    ///< minimum quantization factor, Value range: [0, max_qfactor], default is 1
} vs_venc_brc_mjpeg_vbr_param_s;


/**
* @brief Defines The scenechange detect.
* @details
*/
typedef struct vs_venc_scene_chg_detect {
    vs_bool_t scene_chg_enable;               ///< whether to enable the scene switch detection. value range: [VS_FALSE, VS_TRUE], default is VS_TRUE
    vs_bool_t adaptive_insert_idr_enable;     ///< whether to enable adaptive insertion of IDR picture. value range: [VS_FALSE, VS_TRUE], default is VS_FALSE
} vs_venc_scene_chg_detect_s;

/**
* @brief Defines The skip level of every frame.
* @details
*/
typedef enum vs_venc_skip_level {
    E_VENC_SKIP_LEVEL_0 = 0, 
    E_VENC_SKIP_LEVEL_1, 
    E_VENC_SKIP_LEVEL_2,
    E_VENC_SKIP_LEVEL_3, 
    E_VENC_SKIP_LEVEL_4, 
    E_VENC_SKIP_LEVEL_5,//default 
    E_VENC_SKIP_LEVEL_6, 
    E_VENC_SKIP_LEVEL_7,
    E_VENC_SKIP_LEVEL_8, 
    E_VENC_SKIP_LEVEL_9, 
    E_VENC_SKIP_LEVEL_10,
    E_VENC_SKIP_LEVEL_MAX,
} vs_venc_skip_level_e;

/**
* @brief Defines The param of brc.
* @details
*/
typedef struct vs_venc_brc_param {
    vs_uint32_t base_cu_complexity;  ///< cu base complexity in macro block level bitrate control, value range [10 ,20]. default is 15. do not change this unless you know it.
    vs_uint32_t qpdelta_range;   ///< qpdelta range in macro block level bitrate control, value range [0 ,15]. default is 10.
    vs_uint32_t row_qp_step;     ///< during macro block level bitrate control, the amplitude of the starting qp of each MB row is relative to the frame start qp.
    							 ///<value range: [0, 51], default is 4 with h264 and 16 wtih h265.
    vs_uint32_t rc_blk_size;     ///< block size in mb/lcu qp adjustment for subjective quality  0=64x64 ,1=32x32, 2=16x16  value range: [0, 2], default is 2.only valid for H.265
    vs_int32_t                 first_pic_init_qp;     ///< set the starting qp value for the first frame, and the CBR/VBR/AVBR/CVBR is valid.
    							 ///<value range [-1 ,51]. -1 means rbc module will decide automatically. default is -1.
    vs_venc_scene_chg_detect_s scene_chg_detect;     ///< scene detection related control parameters.
    vs_bool_t                brc_stat_after_attrset_clear;///< clear brc statistic internal or not when brc attr changed. value range: [VS_FALSE, VS_TRUE], default is VS_FALSE
	vs_venc_skip_level_e skip_level; ///< specific skip level of every frame, the bigger of level the more skipped pixels. default is E_VENC_SKIP_LEVEL_5.
	union {
        vs_venc_brc_h264_cbr_param_s     h264_cbr;     ///< H.264 Channel CBR control mode advanced parameters.
        vs_venc_brc_h264_vbr_param_s     h264_vbr;     ///< H.264 Channel VBR control mode advanced parameters.
        vs_venc_brc_h264_avbr_param_s    h264_avbr;     ///< H.264 Channel AVBR control mode advanced parameters.
        vs_venc_brc_h264_cvbr_param_s    h264_cvbr;     ///< H.264 Channel CVBR control mode advanced parameters.
        vs_venc_brc_h265_cbr_param_s     h265_cbr;     ///< H.265 Channel CBR control mode advanced parameters.
        vs_venc_brc_h265_vbr_param_s     h265_vbr;     ///< H.265 Channel VBR control mode advanced parameters.
        vs_venc_brc_h265_avbr_param_s    h265_avbr;     ///< H.265 Channel AVBR control mode advanced parameters.
        vs_venc_brc_h265_cvbr_param_s    h265_cvbr;     ///< H.265 Channel CVBR control mode advanced parameters.
        vs_venc_brc_mjpeg_cbr_param_s    mjpeg_cbr;     ///< MJPEG Channel CBR control mode advanced parameters.
        vs_venc_brc_mjpeg_vbr_param_s    mjpeg_vbr;     ///< MJPEG Channel VBR control mode advanced parameters.
    };
}vs_venc_brc_param_s;

/**
* @brief Defines the param of the reference frame.
* @details
*/
typedef struct vs_venc_ref_policy {
    vs_bool_t      enable;          ///< whether to enable ref policy value range:[VS_FALSE, VS_TRUE].default is VS_FALSE
    vs_uint32_t    base;            ///< period of the base layer.base >= 1
    vs_uint32_t    enhance;         ///< period of the enhance layer. value range:  (0, 6]
    vs_bool_t      pred_enable;     ///< whether the frame on the base layer is used as a reference
                                    ///< by other frames on the base layer.value range:[VS_FALSE, VS_TRUE]
                                    ///< should be VS_TRUE when set with E_VENC_GOP_MODE_LTRP gop mode
                                    ///< base * (enhance + 1) should not bigger than 48.
} vs_venc_ref_policy_s;

/**
* @brief Defines the Jpeg snap mode.
* @details
*/
typedef enum vs_venc_jpeg_snap_mode {
    E_VENC_JPEG_SNAP_MODE_ALL   = 0,  ///< all frames are encoded as jpeg
    E_VENC_JPEG_SNAP_MODE_FLAG,       ///< only frames marked as snap are encoded as jpeg
    E_VENC_JPEG_SNAP_MODE_MAX
} vs_venc_jpeg_snap_mode_e;

/**
* @brief Defines the information of the stream.
* @details
*/
typedef struct vs_venc_stream_buf {
    vs_uint64_t phys_addr;             ///< physical address of stream buffer
    vs_uint64_t virt_addr;             ///< virtual address of stream buffer
    vs_int32_t buf_size;               ///< size of stream buffer
} vs_venc_stream_buf_s;

/**
* @brief Defines the param of the h265e slice split.
* @details
*/
typedef struct vs_venc_h265_slice_split {
    vs_bool_t    enable;        ///< whether to enable slice split encoding value range:[VS_FALSE, VS_TRUE],default is VS_FALSE.
    vs_uint32_t  lcu_line_num;  ///< number of lcu lines for each slice.value range[1, max_lculine_num].
                                ///<max_lculine_num means lculine numbers of whole picture.
} vs_venc_h265_slice_split_s;

/**
* @brief Defines the param of the h265e intra prediction.
* @details
*/
typedef struct vs_venc_h265_intra_pred {
    vs_uint32_t    strong_intra_smoothing_enabled_flag; ///< for more information refer to ITU-T H.265.value range [0, 1] default is 1
    vs_uint32_t    pcm_loop_filter_disabled_flag;       ///< for more information refer to ITU-T H.265. Used only when IPCM area enabled value range.[0, 1] default is 0
} vs_venc_h265_intra_pred_s;

/**
* @brief Defines the param of the h265e transform.
* @details
*/
typedef struct vs_venc_h265_trans {
    vs_bool_t    scalinglist_valid; ///<  for more information refer to ITU-T H.265, value range:[VS_FALSE, VS_TRUE],default VS_FALSE
    vs_int32_t   cb_qp_offset;  ///<  for more information refer to ITU-T H.265, value range[-12, 12].default 0
    vs_int32_t   cr_qp_offset;  ///<  for more information refer to ITU-T H.265, value range[-12, 12].default 0
} vs_venc_h265_trans_s;

/**
* @brief Defines the param of the h265e deblocking.
* @details
*/
typedef struct vs_venc_h265_dblk {
    vs_uint32_t slice_deblocking_filter_disabled_flag;  ///< for more information refer to ITU-T H.265.value range :[0, 1].default is 0
    vs_int32_t  slice_beta_offset_div2;                 ///< for more information refer to ITU-T H.265.value range :[-6, 6].default is 0
    vs_int32_t  slice_tc_offset_div2;                   ///< for more information refer to ITU-T H.265.value range :[-6, 6].default is 0
} vs_venc_h265_dblk_s;

/**
* @brief Defines the param of the h265e sao.
* @details
*/
typedef struct vs_venc_h265_sao {
    vs_uint32_t  slice_sao_luma_flag;       ///< for more information refer to ITU-T H.265, value range :[0, 1] default is 1,only support the same value with slice_sao_chroma_flag now
    vs_uint32_t  slice_sao_chroma_flag;     ///< for more information refer to ITU-T H.265, value range :[0, 1] default is 1,only support the same value with slice_sao_luma_flag now
} vs_venc_h265_sao_s;

/**
* @brief Defines the frame lost mode.
* @details
*/
typedef enum vs_venc_frame_lost_mode {
    E_VENC_FRAME_LOST_MODE_NORMAL = 0,     ///< the frame will be loss when the instantaneous bitrate above the threshold
    E_VENC_FRAME_LOST_MODE_PSKIP,          ///< the frame will encoded by pskip when the instantaneous bitrate above the threshold.
    E_VENC_FRAME_LOST_MODE_MAX
} vs_venc_frame_lost_mode_e;

/**
* @brief Defines The param of the frame lost mode.
* @details
*/
typedef struct vs_venc_frame_lost {
    vs_bool_t                       enable;     ///< frame loss switch when the instantaneous bitrate above the threshold.value range :[VS_FALSE, VS_TRUE].default is VS_FALSE
    vs_venc_frame_lost_mode_e       mode;     ///< Frame loss mode when the instantaneous bitrate above the threshold.
    vs_uint32_t                     frame_lost_thr;     ///< frame loss threshold >64*1024(bit/s).
    vs_uint32_t                     max_allow_abandon_num;     ///< maximum number allowed to be continuously lost frames. value range: [0, 65535], default is 0.
    vs_uint32_t                     frame_window;  ///< moving bitrate frame number. value range:[2, gop],gop means gop size.
} vs_venc_frame_lost_s;


/**
* @brief Defines the overrun frame mode.
* @details
*/
typedef enum vs_venc_frame_overrun_mode {
    E_VENC_FRAME_OVERRUN_MODE_NONE = 0,    ///<   no special treatment
    E_VENC_FRAME_OVERRUN_MODE_ABANDON,     ///< discards the overrun frame
    E_VENC_FRAME_OVERRUN_MODE_REENCODE,    ///< overrun frame are re-encoded
    E_VENC_FRAME_OVERRUN_MODE_MAX
} vs_venc_frame_overrun_mode_e;

/**
* @brief Defines the config of the overrun frame.
* @details
*/
typedef struct vs_venc_frame_overrun {
    vs_venc_frame_overrun_mode_e    mode;     ///< frame overrun processing mode.
    vs_int32_t                      thr_bits_i;     ///< I frame overrun threshold thr_bits_i > 11*1024 & 512b align,default 500000
    vs_int32_t                      thr_bits_p;     ///< P frame overrun threshold thr_bits_p > 11*1024 & 512b align,default 500000
    vs_int32_t                      thr_bits_b;     ///< B frame overrun threshold thr_bits_b > 11*1024 & 512b align,default 500000
} vs_venc_frame_overrun_s;

/**
* @brief Defines the mode of the gdr(intra refresh).
* @details
*/
typedef enum vs_venc_gdr_mode {
    E_VENC_GDR_ROW = 0, 	///< refresh the intra area by row.
    E_VENC_GDR_COLUMN,		///< refresh the intra area by column.
    E_VENC_GDR_MAX
} vs_venc_gdr_mode_e;

/**
* @brief Defines the parameter of the gdr(intra refresh).
* @details
*/
typedef struct vs_venc_gdr {
    vs_bool_t      enable;      ///< whether to enable gdr encoding value range:[VS_FALSE, VS_TRUE],default VS_FALSE
	vs_venc_gdr_mode_e mode;	///< specific refresh mode of GDR function. default is E_VENC_GDR_ROW.
	vs_uint32_t    refresh_num; ///< number of P frames that I frame should split.value range :0 or [2, gop] default is 4.
    vs_uint32_t    req_qp_i;    ///< qp value of I frame that force requested ,value range :[0, 51].
} vs_venc_gdr_s;

/**
* @brief Defines the param of the crop.
* @details
*/
typedef struct vs_venc_crop_info {
    vs_bool_t enable;      ///< whether to enable crop encoding data. value range:[VS_FALSE, VS_TRUE],default VS_FALSE
    vs_rect_s  rect;       ///< rect area of crop encoding
} vs_venc_crop_info_s;

/**
* @brief Defines the param of rotation.
* @details
*/
typedef struct vs_venc_rotation {
    vs_bool_t       enable;      ///< whether to enable rotation encoding data. value range:[VS_FALSE, VS_TRUE],default VS_FALSE
    vs_fixed_rotation_e   mode;  ///< mode of rotation encoding,default E_FIXED_ROTATION_0
} vs_venc_rotation_s;

/*
* @brief Defines the color to gray mode of the venc encode channel.
* @details
*/
typedef enum vs_venc_color2grey_mode {
    E_VENC_COLOR2GRE_MODE_FIXUV = 0,    ///< fix uv mode, chroma data will filled with indicated uv value
    E_VENC_COLOR2GRE_MODE_MONO,         ///< monochroma mode, stream encoded as yuv400.
    E_VENC_COLOR2GRE_MODE_MAX
} vs_venc_color2grey_mode_e;

/**
* @brief Defines the color to gray param of the venc encode channel.
* @details
*/
typedef struct vs_venc_color2grey {
    vs_bool_t  enable;               ///< whether to enable color to gray data. value range:[VS_FALSE, VS_TRUE],default VS_FALSE
    vs_venc_color2grey_mode_e mode;  ///< mode of color to gray param,default E_VENC_COLOR2GRE_MODE_FIXUV
    vs_uint8_t                uv_value;         ///< uv value in fix uv mode range [0, 255],default 0x00
} vs_venc_color2grey_s;


/**
* @brief Defines the param of the venc encode channel.
* @details
*/
typedef struct vs_venc_chn_param {
    vs_uint32_t  max_streambuf_frame_num;          ///< max frame count in output stream buffer. value range:[1, 20000].default 200
    vs_venc_crop_info_s   crop_cfg;    ///< crop configuration of encoding
    vs_video_frc_s        framerate_cfg;    ///< frame rate control configuration of encoding
    vs_venc_rotation_s    rotation_cfg;     ///< rotation configuration of encoding
    vs_venc_color2grey_s  color2grey_cfg;   ///< color to gray configuration of encoding
} vs_venc_chn_param_s;


/**
* @brief Defines venc mode type.
* @details
*/
typedef enum vs_venc_type {
    E_VENC_MOD_TYPE_H264E = 0,      ///< h264e module parameter type
    E_VENC_MOD_TYPE_H265E,      ///< h265e module parameter type
    E_VENC_MOD_TYPE_JPEGE,      ///< jpege module parameter type
    E_VENC_MOD_TYPE_MAX
} vs_venc_type_e;

/**
* @brief Defines the output package mode of encoding stream.
* @details
*/
typedef enum vs_venc_union_nalu_mode {
    E_VENC_UNION_NALU_MODE_ONE  = 0,     ///< single package mode
    E_VENC_UNION_NALU_MODE_MULTI,   ///< multiple package mode
    E_VENC_UNION_NALU_MODE_MAX
} vs_venc_union_nalu_mode_e;

/**
* @brief Defines the alloc mode of output buffer.
* @details
*/
typedef enum vs_venc_stream_buf_mode {
    E_VENC_STREAM_BUF_MODE_NORMAL  = 0,///< normal mode
    E_VENC_STREAM_BUF_MODE_MINI,         ///< save memory mode
    E_VENC_STREAM_BUF_MODE_MAX
} vs_venc_stream_buf_mode_e;

/**
* @brief Defines the power mode.
* @details
*/
typedef enum vs_venc_power_mode {
    E_VENC_POWER_MODE_NORMAL  = 0,  ///< normal mode
    E_VENC_POWER_MODE_DOWN,    ///< low power mode
    E_VENC_POWER_MODE_MAX
} vs_venc_power_mode_e;

/**
* @brief Defines the param of the h264e module.
* @details
*/
typedef struct vs_venc_h264_mod {
    vs_venc_union_nalu_mode_e         union_nalu_mode;    ///< output package mode of encoding stream, default is E_VENC_UNION_NALU_MODE_ONE
    vs_venc_stream_buf_mode_e         stream_buf_mode; ///< alloc mode of output buffer, default is E_VENC_STREAM_BUF_MODE_NORMAL
    vs_venc_power_mode_e              power_mode;   ///< encoding power mode.Reserved.
    vs_pool_type_e                    vb_source;    ///< source of video buffer for reference frame .Reserved.
    vs_uint32_t                       max_sei_userdata_len; ///< max length of sei user data. range[16, 2048], default is 512
    vs_bool_t                         qp_hstgrm_enable; ///< qp_hstgrm_enable data. value range [VS_FALSE, VS_TRUE] default is VS_FALSE
} vs_venc_h264_mod_s;

/**
* @brief Defines the param of the h265e module.
* @details
*/
typedef struct vs_venc_h264_mod vs_venc_h265_mod_s;

/**
* @brief Defines the param of the jpege module.
* @details
*/
typedef struct vs_venc_jpeg_mod {
    vs_venc_union_nalu_mode_e         union_nalu_mode;  ///< output package mode of encoding stream, default is E_VENC_UNION_NALU_MODE_ONE
    vs_venc_stream_buf_mode_e         stream_buf_mode;    ///< alloc mode of output buffer, default is E_VENC_STREAM_BUF_MODE_NORMAL
    vs_bool_t                         output_buf_after_attrset_clear;   ///< whether to clear output buffer when configuring attributes data. value range [VS_FALSE, VS_TRUE] default is VS_FALSE
} vs_venc_jpeg_mod_s;

/**
* @brief Defines the param of the mod.
* @details
*/
typedef struct vs_venc_mod_param {
    vs_venc_type_e type;    ///< indicate the type of module in the "union"
    vs_bool_t    rcvstop_inputbuf_clear;    ///< whether to clear input buffer after stopping receive.value range [VS_FALSE, VS_TRUE]
    union {
        vs_venc_h264_mod_s h264_param; ///< h.264 parameter of venc module
        vs_venc_h265_mod_s h265_param; ///< h.265 parameter of venc module
        vs_venc_jpeg_mod_s jpeg_param; ///< jpeg/mjpeg parameter of venc module
    };
} vs_venc_mod_param_s ;



/**
* @brief Defines the ground protect of FOREGROUND.
* @details
*/
typedef struct vs_venc_fg_protect {
    vs_bool_t    enable;    ///< whether to enable foreground protect encoding. value range :[VS_FALSE, VS_TRUE],default is VS_FALSE
    vs_uint32_t   direction_thr; ///< in the foreground protect encoding, it is used to control the index of direction threshold. value range :[0, VENC_BRC_COMPLEXITY_THR_SIZE],default is 8
    vs_uint32_t   thr_gain;    ///< threshold used to calculate sad in foreground detection. value range :[0, 15],default is 8
    vs_uint32_t   thr_offset;  ///< threshold used to calculate sad in foreground detection. value range :[0, 255],default is 8
    vs_uint32_t   thr_p[VENC_BRC_COMPLEXITY_THR_SIZE];    ///< mad threshold of P frame in foreground protect encoding. value range :[0, 2550],default is 0
    vs_uint32_t   thr_b[VENC_BRC_COMPLEXITY_THR_SIZE];    ///< mad threshold of B frame in foreground protect encoding. value range :[0, 2550],default is 0
} vs_venc_fg_protect_s;


/**
* @brief Defines the debreath effect of the venc encode channel.
* @details
*/
typedef struct vs_venc_debreath {
    vs_bool_t     enable;       ///< whether to enable debreath encoding, value range :[VS_FALSE, VS_TRUE],default is VS_FALSE
    vs_int32_t    factor0;    ///< parameter 0 of the debreathing effect, when the value is smaller, the I debreath effect is better value range [0,35] default is 6
    vs_int32_t    factor1;    ///< parameter 1 of the debreathing effect, when the value is bigger, the I frame size is bigger value range [0,35] default is 16
} vs_venc_debreath_s;


/**
* @brief Defines the hierarchical qp of the venc encode channel.
* @details
*/
typedef struct vs_venc_hierarchical_qp {
    vs_bool_t      enable;  ///< whether to enable hierarchical qp encoding data. value range :[VS_FALSE, VS_TRUE],default is VS_FALSE
    vs_int32_t     hierarchical_qpdelta[VENC_MAX_HIERARCHICAL_QP_LAYER_NUM];    ///< qp delta between current layer and base layer, index indicated hierarchical layer except base layer data. range [-10, 10] default is 0
    vs_int32_t     hierarchical_frame_num[VENC_MAX_HIERARCHICAL_QP_LAYER_NUM];  ///< frame number for each hierarchical layer except base layer data. range [0, 5] default is 0. total frame number of  VENC_MAX_HIERARCHICAL_QP_LAYER_NUM must less than 15.
} vs_venc_hierarchical_qp_s;


/**
* @brief Defines the slice split mode of the venc encode chn.
* @details
*/
typedef enum vs_venc_slice_split_mode {
    E_VENC_SLICE_SPLIT_MODE_LINE = 0, ///< indicated the line numbers of one slice.
    E_VENC_SLICE_SPLIT_MODE_SLICE,      ///< indicated the slice numbers that each frame split into.
    E_VENC_SLICE_SPLIT_MODE_MAX
} vs_venc_slice_split_mode_e;

/**
* @brief Defines the slice split of the venc encode chn.
* @details
*/
typedef struct vs_venc_slice_split {
    vs_bool_t enable;           ///< whether to enable slice split encoding,value range :[VS_FALSE, VS_TRUE],default is VS_FALSE
    vs_venc_slice_split_mode_e mode;  ///< whether to enable slice split encoding,default is E_VENC_SLICE_SPLIT_MODE_SLICE
    vs_uint32_t  size;    ///< number of macroblock lines or for each slice h264  [1,(frame_height+15)/16] H.265 [1,(frame_height+63)/64],default is 1
} vs_venc_slice_split_s;


/**
* @brief Defines the ipcm area of the venc encode channel.
* @details
*/
typedef struct vs_venc_ipcm_area {
    vs_int32_t index;   ///< index of ipcm area encoding,value range :[0, 1]
    vs_bool_t   enable;   ///< whether to enable intra area encoding,value range :[VS_FALSE, VS_TRUE],default is VS_FALSE
    vs_rect_s  rect;    ///< rect area of ipcm area encoding.x、y、width、height should 16 pixel align for h.264,64 pixel align for h.265
} vs_venc_ipcm_area_s;

/**
* @brief Defines the intra area of the venc encode channel.
* @details
*/
typedef struct vs_venc_intra_area {
    vs_int32_t  index;    ///< index of intra area encoding,must be 0.
	vs_bool_t   enable;   ///< whether to enable intra area encoding,value range :[VS_FALSE, VS_TRUE],default is VS_FALSE
    vs_rect_s   rect;     ///< rect area of intra area encoding.x、y、width、height should 16 pixel align for h.264,64 pixel align for h.265
} vs_venc_intra_area_s;

/**
* @brief Defines the cir param of the venc encode channel.
* @details
*/
typedef struct vs_venc_cir {
    vs_bool_t      enable;    ///< whether to enable cyclic intra refresh encoding,value range :[VS_FALSE, VS_TRUE],default is VS_FALSE
    vs_int32_t     start;     ///< start position of cyclic intra refresh encoding value range [0,total_mb)
                              ///< h264 total_mb = ((frame_width + 15) / 16) * ((frame_height + 15) / 16)
                              ///< h265 total_lcu = ((frame_width + 63) / 64) * ((frame_height + 63) / 64)
    vs_int32_t     interval;  ///< interval of cyclic intra refresh encoding value range (0,total_mb/lcu - start)
} vs_venc_cir_s;


#ifdef __cplusplus
}
#endif

#endif /*!<  __VS_VENC_DEFINES_H__ */

