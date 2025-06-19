/**
* @file vs_capability.h
* @brief capability info
* @details module capability
* @author VS codec group
* @date 2022-06-05
* @version v0.10
* @Copyright (C) 2021, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/

#ifndef __VS_CAPABILITY_H__
#define __VS_CAPABILITY_H__

#include "vs_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

#define VS_ALIGN_DEFAULT 64

/**
* @brief Define the VO capabilities
*/
#define VO_MAX_DEV_NUM          2 ///< maximum number of display device
#define VO_MAX_CURSOR_NUM       2 ///< maximum number of cursor layer, cursor 0, 1 belong to device 0, 1 respectively
#define VO_MAX_WBC_NUM          2 ///< maximum number of write back channel, wbc 0, 1 belong to device 0, 1 respectively

/**
* @details: layer id - functional layer
* layer 0 - VHD0
* layer 1 - VHD2
* layer 2 - graphic layer0
* layer 3 - VHD1
* layer 4 - VHD3
* layer 5 - graphic layer1
*/
#define VO_MAX_LAYER_NUM        6 ///< maximum number of layer
#define VO_MIN_LAYER_WIDTH     64 ///< minimum width of layer
#define VO_MIN_LAYER_HEIGHT    64 ///< minimum width of layer
#define VO_MAX_LAYER_WIDTH   8192 ///< maximum width of layer
#define VO_MAX_LAYER_HEIGHT  8192 ///< maximum height of layer
#define VO_MAX_LAYER_BUF_DEPTH  8 ///< maximum buf depth of layer
#define VO_MIN_LAYER_BUF_DEPTH  2 ///< minimum buf depth of layer

#define VO_MAX_VHD0_CHN_NUM    36 ///< maximum number of video channel on VHD0
#define VO_MAX_VHD1_CHN_NUM    16 ///< maximum number of video channel on VHD1
#define VO_MIN_CHN_WIDTH       32 ///< minimum width of video channel frame
#define VO_MIN_CHN_HEIGHT      32 ///< minimum height of video channel frame
#define VO_MAX_CHN_WIDTH     8192 ///< maximum width of video channel frame
#define VO_MAX_CHN_HEIGHT    8192 ///< maximum height of video channel frame
#define VO_MAX_CHN_BUF_DEPTH    8 ///< maximum buf depth of video channel
#define VO_MIN_CHN_BUF_DEPTH    2 ///< minimum buf depth of video channel
#define VO_MAX_CHN_ZOOM_RAIO 1000 ///< maximum ratio of zoom ratio
#define VO_MAX_CHN_BORDER_WIDTH 16 ///< maximum width of video channel border
#define VO_MIN_CHN_BORDER_WIDTH 0  ///< minimum width of video channel border
#define VO_MAX_FRAME_RATE      64 ///< maximum framerate
#define VO_MIN_FRAME_RATE     -64 ///< minimum framerate

#define VO_MIN_WBC_BUF_DEPTH    0 ///< minimum buf depth of video channel
#define VO_MAX_WBC_BUF_DEPTH    8 ///< maximum buf depth of video channel

/**
* @brief Define the VENC capabilities
*/
#define VENC_MAX_CHN_NUM                         16 ///< maximum number of video channel
#define VENC_MAX_H264_WIDTH                    8192 ///< maximum width of h264 video channel frame
#define VENC_MAX_H264_HEIGHT                   8192 ///< maximum height of h264 video channel frame
#define VENC_MIN_H264_WIDTH                     130 ///< minimum width of h264 video channel frame
#define VENC_MIN_H264_HEIGHT                    130 ///< minimum height of h264 video channel frame
#define VENC_MAX_H265_WIDTH                    8192 ///< maximum width of h265 video channel frame
#define VENC_MAX_H265_HEIGHT                   8192 ///< maximum height of h265 video channel frame
#define VENC_MIN_H265_WIDTH                     130 ///< minimum width of h265 video channel frame
#define VENC_MIN_H265_HEIGHT                    130 ///< minimum height of h265 video channel frame
#define VENC_MAX_JPEG_WIDTH                   32768 ///< maximum width of JPEG video channel frame
#define VENC_MAX_JPEG_HEIGHT                  32768 ///< maximum height of JPEG video channel frame
#define VENC_MIN_JPEG_WIDTH                      48 ///< minimum width of JPEG video channel frame
#define VENC_MIN_JPEG_HEIGHT                     48 ///< minimum height of JPEG video channel frame
#define VENC_MAX_ROI_NUM                          8 ///< maximum number of ROI area for venc support
#define VENC_QP_HISTOGRAM_NUM                    52 ///< number of QP histograms for venc support
#define VENC_MAX_NALU_NUM_IN_ONE_PACK             8 ///< maximum number of nalu in one pack
#define VENC_MAX_THUMBNAIL_NUM                    1 ///< maximum number of thumbnail
#define VENC_MAX_RECEIVE_SOURCE                   1 ///< maximum number of thumbnail receive source
#define VENC_FRAME_RECEIVE_SOURCE0                0 ///< the fisrt thumbnail receive source
#define VENC_MAX_TEMPORAL_LAYER_NUM               3 ///< maximum number of temporal layer for venc support
#define VENC_MAX_HIERARCHICAL_QP_LAYER_NUM        4 ///< maximum number of hierarchical QP layer
#define VENC_MAX_ROI_FRAME_TYPE_NUM               3 ///< maximum number of ROI frame type
#define VENC_SCALINGLIST_BLK_SIZE                64 ///< the block size of scalinglist for jpeg/mjpeg
#define VENC_MIN_INPUT_FRAME_RATE                 1 ///< minimum number of source frame rate
#define VENC_MAX_INPUT_FRAME_RATE               240 ///< maximum number of source frame rate
#define VENC_BRC_COMPLEXITY_THR_SIZE             16 ///< the number of complexity threshold
#define VENC_MIN_BITRATE                         10 ///< minimum bitrate
#define VENC_MAX_BITRATE                         204800 ///< maximum bitrate of H264/H265/MJPEG

/**
* @brief Define the VDEC capabilities
*/
#define VDEC_MAX_CHN_NUM        16


//VII Start
/**
* @brief Define the VII capabilities
*/
#define VII_MAX_DEV_NUM       4 ///< maximum number of vii device
#define VII_MAX_PHYS_PIPE_NUM 8 ///< maximum number of phys pipe
#define VII_MAX_VIRT_PIPE_NUM 4 ///< maximum number of virt pipe
#define VII_MAX_PIPE_NUM      (VII_MAX_PHYS_PIPE_NUM + VII_MAX_VIRT_PIPE_NUM) ///< maximum number of pipe
#define VII_MAX_PHYS_CHN_NUM  1 ///< maximum number of physical channel
#define VII_MAX_EXT_CHN_NUM   4 ///< maximum number of extended channel
#define VII_MAX_CHN_NUM       (VII_MAX_PHYS_CHN_NUM + VII_MAX_EXT_CHN_NUM) ///< maximum number of channel

#define VII_MAX_VC_NUM        4 ///< maximum number of virtual channel
#define VII_MAX_WDR_FRAME_NUM 4 ///< maximum number of wdr frame

#define VII_MAX_INFRARED_REG_NUM 64 ///< maximum number of infrared reg

#define VII_MAX_PIPE_WIDTH   8192 ///< maximum pipe width
#define VII_MAX_PIPE_HEIGHT  8192 ///< maximum pipe height
#define VII_MIN_PIPE_WIDTH   128  ///< minimum pipe width
#define VII_MIN_PIPE_HEIGHT  128  ///< minimum pipe height

#define VII_MAX_CHN_WIDTH   8192 ///< maximum channel width
#define VII_MAX_CHN_HEIGHT  8192 ///< maximum channel height
#define VII_MIN_CHN_WIDTH   128  ///< minimum channel width
#define VII_MIN_CHN_HEIGHT  128  ///< minimum channel height

/**
* @brief Define the ISP capabilities
*/
#define ISP_MAX_PIPE_NUM  VII_MAX_PIPE_NUM ///< maximum number of pipe
#define ISP_MAX_WIDTH     4608 ///< maximum width
#define ISP_MAX_HEIGHT    3456 ///< maximum height
#define ISP_MIN_WIDTH     128 ///< minimum width
#define ISP_MIN_HEIGHT    128 ///< minimum height
//VII End

/**
* @brief Define the sys capabilities
*/
#define SYS_BIND_MAX_NUM  64 ///< maximum bind number

/**
* @brief Define the tdc capabilities
*/
#define TDC_MAX_SLAVE_DEV_NUM        2 ///< maximum number of slave device
#define TDC_MAX_STROBE_FLASH_NUM     8 ///< maximum number of strobe flash
#define TDC_MAX_EXPOSURE_FLASH_NUM   4 ///< maximum number of exposure flash

/**
* @brief Define the vpp capabilities
*/
#define VPP_MAX_GRP_NUM                  128
#define VPP_MAX_PHYCHN_NUM               4
#define VPP_MAX_EXTCHN_NUM               8
#define VPP_MAX_CHN_NUM                  (VPP_MAX_PHYCHN_NUM + VPP_MAX_EXTCHN_NUM)
#define VPP_MAX_IMAGE_WIDTH              8192
#define VPP_MAX_IMAGE_HEIGHT             8192
#define VPP_EXTCHN_MAX_IMAGE_WIDTH       8192
#define VPP_EXTCHN_MAX_IMAGE_HEIGHT      8192
#define VPP_MIN_IMAGE_WIDTH              64
#define VPP_MIN_IMAGE_HEIGHT             64
#define VPP_EXTCHN_MIN_IMAGE_WIDTH       32
#define VPP_EXTCHN_MIN_IMAGE_HEIGHT      32
#define VPP_MAX_STITCH_NUM               2
#define VPP_MAX_STITCH_GRP_NUM           6

/**
* @brief Define the region capabilities
*/
#define RGN_MAX_HANDLE_NUM               1024
#define RGN_MAX_OVERLAY_BUF_NUM          6
#define RGN_MAX_BATCH_GROUP_NUM          8
#define RGN_MAX_BATCH_HANDLE_NUM         24

#define RGN_MAX_VII_COVEREX_NUM          16
#define RGN_MAX_VII_OVERLAYEX_NUM        16

#define RGN_MAX_VENC_OVERLAY_NUM         8
#define RGN_MAX_VENC_MOSAIC_NUM          4

#define RGN_MAX_VPP_COVER_NUM            8
#define RGN_MAX_VPP_COVEREX_NUM          8
#define RGN_MAX_VPP_OVERLAYEX_NUM        8
#define RGN_MAX_VPP_MOSAIC_NUM           4

#define RGN_MAX_VO_COVEREX_NUM           1
#define RGN_MAX_VO_OVERLAYEX_NUM         1

#define RGN_MAX_AVS_OVERLAYEX_NUM        8

#define RGN_MAX_PCIV_OVERLAYEX_NUM       1

#define RGN_COVER_X_MIN                  0
#define RGN_COVER_X_MAX                  8192
#define RGN_COVER_Y_MIN                  0
#define RGN_COVER_Y_MAX                  8192
#define RGN_COVER_WIDTH_MIN              64
#define RGN_COVER_WIDTH_MAX              8192
#define RGN_COVER_HEIGHT_MIN             64
#define RGN_COVER_HEIGHT_MAX             8192
#define RGN_COVER_THICK_MIN              2
#define RGN_COVER_THICK_MAX              8

#define RGN_COVEREX_X_MIN                0
#define RGN_COVEREX_X_MAX                8192
#define RGN_COVEREX_Y_MIN                0
#define RGN_COVEREX_Y_MAX                8192
#define RGN_COVEREX_WIDTH_MIN            64
#define RGN_COVEREX_WIDTH_MAX            8192
#define RGN_COVEREX_HEIGHT_MIN           64
#define RGN_COVEREX_HEIGHT_MAX           8192

#define RGN_MOSAIC_X_MIN                 0
#define RGN_MOSAIC_X_MAX                 8192
#define RGN_MOSAIC_Y_MIN                 0
#define RGN_MOSAIC_Y_MAX                 8192
#define RGN_MOSAIC_WIDTH_MIN             64
#define RGN_MOSAIC_WIDTH_MAX             8192
#define RGN_MOSAIC_HEIGHT_MIN            64
#define RGN_MOSIAC_HEIGHT_MAX            8192

#define RGN_OVERLAY_X_MIN                0
#define RGN_OVERLAY_X_MAX                8192
#define RGN_OVERLAY_Y_MIN                0
#define RGN_OVERLAY_Y_MAX                8192
#define RGN_OVERLAY_WIDTH_MIN            16
#define RGN_OVERLAY_WIDTH_MAX            8192
#define RGN_OVERLAY_HEIGHT_MIN           16
#define RGN_OVERLAY_HEIGHT_MAX           8192

#define RGN_OVERLAYEX_X_MIN              0
#define RGN_OVERLAYEX_X_MAX              8192
#define RGN_OVERLAYEX_Y_MIN              0
#define RGN_OVERLAYEX_Y_MAX              8192
#define RGN_OVERLAYEX_WIDTH_MIN          2
#define RGN_OVERLAYEX_WIDTH_MAX          8192
#define RGN_OVERLAYEX_HEIGHT_MIN         2
#define RGN_OVERLAYEX_HEIGHT_MAX         8192


/**
* @brief Define the gdc capabilities
*/
#define GDC_FRAME_MAX_WIDTH              8192
#define GDC_FRAME_MIN_WIDTH              64

#define GDC_FRAME_MAX_HEIGHT             4096
#define GDC_FRAME_MIN_HEIGHT             64

#define GDC_JOB_MAX_NUM                  255


/**
* @brief Define the gpe capabilities
*/
#define GPE_JOB_MAX_NUM                  128
#define GPE_TASK_MAX_NUM                 200

/**
* @brief Define the tde capabilities
*/
#define TDE_JOB_MAX_NUM                  128
#define TDE_TASK_MAX_NUM                 200

/**
* @brief Define the audio capabilities
*/
#define AIN_DEV_NUM_MAX     2
#define AIN_CHN_NUM_MAX     8
#define AOUT_DEV_NUM_MAX    2
#define AOUT_CHN_NUM_MAX    2
#define AENC_CHN_NUM_MAX    16
#define ADEC_CHN_NUM_MAX    16

/**
* @brief Define the pciv capabilities
*/
#define PCIV_HOST_ID            0       ///< host pcie device id
#define PCIV_SLAVE_ID           1       ///< slave pcie device id
#define PCIV_MAX_CHN_NUM        16      ///< maximum number of pciv channel
#define PCIV_MAX_MSG_LEN        1024    ///< maximum length of message
#define PCIV_MAX_PORT_NUM       128     ///< maximum port of message
#define PCIV_MIN_BUF_NUM        2       ///< minimum buffer number of host pciv channel
#define PCIV_MAX_BUF_NUM        16      ///< maximum buffer number of host pciv channel
#define PCIV_MIN_VB_CNT         1       ///< minimum vb pool number of slave window buffer
#define PCIV_MAX_VB_CNT         8       ///< maximum vb pool number of slave window buffer
#define PCIV_MIN_DMABLK         1       ///< minimum number of dma task
#define PCIV_MAX_DMABLK         128     ///< maximum number of dma task


/**
* @brief Define the avs capabilities
*/

#define AVS_MAX_GRP_NUM                  4
#define AVS_MAX_PIPE_NUM                 4
#define AVS_MAX_PHYCHN_NUM               1
#define AVS_MAX_EXT_CHN_NUM               1
#define AVS_MAX_CHN_NUM                  (AVS_MAX_PHYCHN_NUM + AVS_MAX_EXT_CHN_NUM)
#define AVS_MAX_IMAGE_WIDTH              8192
#define AVS_MAX_IMAGE_HEIGHT             8192
#define AVS_MIN_IN_WIDTH                 1280
#define AVS_MIN_IN_HEIGHT                720
#define AVS_MIN_OUT_WIDTH                256
#define AVS_MIN_OUT_HEIGHT               256

/**
* @brief Define the UUID num
*/
#define SYS_UUID_NUM	5


#ifdef __cplusplus
}
#endif

#endif /*!<  __VS_CAPABILITY_H__ */
