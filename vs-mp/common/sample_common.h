/**
* @file sample_common.h
* @brief sample common interface
* @details contains APIs
* @author Visinex Software Group
* @date 2022-05-25
* @version v0.10
* @Copyright (C) 2022, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/

#ifndef _SAMPLE_COMMON_H_
#define _SAMPLE_COMMON_H_

#include <stdio.h>
#include <pthread.h>

#include "vs_comm.h"
#include "vs_buffer.h"

#include "vs_mal_vpp.h"
#include "vs_mal_vo.h"
//VII Start
#include "vs_vii_defines.h"
#include "vs_mal_mipi_rx.h"
#include "vs_mal_isp.h"
#include "vs_mal_vii.h"
//VII End
#include "vs_mal_venc.h"
#include "vs_mal_sys.h"
#include "vs_mal_audio.h"
#include "vs_mal_mmz.h"
#include "vs_mal_vdec.h"
#include "vs_mal_vbm.h"
#include "vs_mal_gpe.h"
#include "vs_mal_nn.h"
#include "vs_mal_rgn.h"
#include "vs_mal_ive.h"
#ifndef VS_ORION
#include "vs_mal_hdmi.h"
#include "vs_mal_pciv.h"
#endif
#if !defined(VS_815) && !defined(VS_816)
#include "vs_mal_dsp.h"
#endif
#include "sample_common_mipitx.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LOG_TAG "vs_sample"
#define vs_sample_trace(fmt, ...) \
    printf("%s: %s[%d]: " fmt, LOG_TAG, __func__, __LINE__, ##__VA_ARGS__);

#define VPP_MAX_PHYCHN_NUM          4
#define VPP_MAX_EXTCHN_NUM          8
#define SENSOR_MAX_NAME_LEN         20
#define DEV_BIND_MAX_PIPE_NUM       4
#define VII_MAX_ROUTE_NUM           4
#define MAX_FILE_PATH_LEN           256
#define PIC_4K_WIDTH                3840
#define PIC_4K_HEIGHT               2160
#define PIC_2K_WIDTH                2560
#define PIC_2K_HEIGHT               1440
#define PIC_2K_2688_WIDTH           2688
#define PIC_2K_1520_HEIGHT          1520
#define PIC_1080P_WIDTH             1920
#define PIC_1080P_HEIGHT            1080
#define PIC_720P_WIDTH              1280
#define PIC_720P_HEIGHT             720
#define PIC_D1_WIDTH                704
#define PIC_D1_HEIGHT               576
#define PIC_VGA_WIDTH               640
#define PIC_VGA_HEIGHT              480
#define PIC_CIF_WIDTH               352
#define PIC_CIF_HEIGHT              288
#define COMPRESSION_HEADLINE        128
#define WRITE_FILE_LEN_MAX          0x1000
#define NN_MAX_DETECT_OBJ_NUM       30
#define NN_MAX_INPUT_NUM            16
#define NN_VPP_ACQUIRE_TIMEOUT      100
#define NN_MAX_MODEL_NUM            32
#define NN_MAX_NB_FILE_PATH         64

//VII Start
typedef enum sample_sensor_type {
    OV_OS08A10_MIPI_8M_30FPS_10BIT = 0,
    OV_OS08A10_MIPI_2M_60FPS_10BIT,
    OV_OS08A10_MIPI_8M_60FPS_10BIT,
    OV_OS08A10_MIPI_8M_WDR2TO1_30FPS_10BIT,

    OV_OS08A20_MIPI_8M_60FPS_10BIT,
    OV_OS08A20_MIPI_8M_50FPS_10BIT,
    OV_OS08A20_MIPI_8M_30FPS_10BIT,
    OV_OS08A20_MIPI_2M_120FPS_10BIT,

    OV_OS04A10_MIPI_4M_30FPS_10BIT,
    OV_OS04A10_MIPI_2M_30FPS_10BIT,
    OV_OS04A10_MIPI_4MFULL_25FPS_12BIT,
    OV_OS04A10_MIPI_4MFULL_WDR2TO1_25FPS_10BIT,

    OV_OS04C10_MIPI_4MFULL_30FPS_12BIT,
    OV_OV9734_MIPI_1M_30FPS_10BIT,

    SC_450AI_MIPI_4M_30FPS_10BIT,
    SC_450AI_MIPI_4M_25FPS_10BIT,
    SC_450AI_MIPI_4MFULL_25FPS_10BIT,
    SC_450AI_MIPI_4M_WDR2TO1_30FPS_10BIT,
    SC_450AI_MIPI_4M_WDR2TO1_25FPS_10BIT,
    SC_450AI_MIPI_2M_WDR3TO1_30FPS_10BIT,
    SC_450AI_MIPI_4MFULL_WDR2TO1_25FPS_10BIT,

    SC_910GS_MIPI_8M_25FPS_12BIT,

    SC_200AI_MIPI_2L_2M_30FPS_10BIT,
    SC_200AI_MIPI_2L_2M_WDR2TO1_30FPS_10BIT,
    SC_200AI_MIPI_2L_2M_25FPS_10BIT,
    SC_200AI_MIPI_2L_2M_WDR2TO1_25FPS_10BIT,

    SC_535HGS_MIPI_4L_5M_60FPS_12BIT,

    SC_430AI_MIPI_4M_30FPS_10BIT,
    SC_430AI_MIPI_4MFULL_30FPS_10BIT,
    SC_430AI_MIPI_2M_30FPS_10BIT,
    SC_430AI_MIPI_4MFULL_60FPS_10BIT,
    SC_430AI_MIPI_4MFULL_WDR2TO1_30FPS_10BIT,
    SC_430AI_MIPI_4M_WDR2TO1_30FPS_10BIT,
    SC_430AI_MIPI_2M_WDR2TO1_30FPS_10BIT,

    SONY_IMX334_MIPI_8M_30FPS_12BIT,
    SONY_IMX334_MIPI_8M_45FPS_12BIT,
    SONY_IMX334_MIPI_8M_60FPS_12BIT,

    SONY_IMX347_MIPI_4MFULL_25FPS_12BIT,
    SONY_IMX347_MIPI_4MFULL_30FPS_12BIT,
    SONY_IMX347_MIPI_4M_WDR2TO1_25FPS_12BIT,
    SONY_IMX347_MIPI_4M_WDR2TO1_30FPS_12BIT,
    SONY_IMX347_MIPI_4MFULL_WDR2TO1_25FPS_12BIT,

    SONY_IMX485_MIPI_8M_15FPS_12BIT,
    SONY_IMX485_MIPI_4L2C_8M_30FPS_12BIT,
    SONY_IMX485_MIPI_8L_8M_30FPS_12BIT,

    SONY_IMX305_SLVDS_8M_20FPS_12BIT,
    SONY_IMX305_SLVDS_8L_9M_25FPS_12BIT,
    SONY_IMX305_SLVDS_8L_9M_30FPS_12BIT,
    SONY_IMX305_SLVDS_16L_8M_25FPS_12BIT,
    SONY_IMX305_SLVDS_16L_9M_50FPS_12BIT,

    SONY_IMX385_MIPI_2M_25FPS_12BIT,

    SONY_IMX415_MIPI_8M_60FPS_12BIT,
    SONY_IMX415_MIPI_8M_WDR2TO1_30FPS_12BIT,
    SONY_IMX415_MIPI_8M_25FPS_12BIT,
    SONY_IMX415_MIPI_8M_WDR2TO1_25FPS_12BIT,

    GPIXEL_GMAX3809_SLVDS_9M_LINEAR_25FPS_12BIT,
    GPIXEL_GMAX3809_SLVDS_8L_9M_LINEAR_30FPS_12BIT,

    GC_GC4653_MIPI_4M_30FPS_10BIT,
    GC_GC4023_MIPI_4M_25FPS_10BIT,

    SONY_IMX686_MIPI_8M_30FPS_14BIT,
    SONY_IMX686_MIPI_8M_30FPS_14BIT_PD,
    SONY_IMX686_MIPI_8M_QHDR_30FPS_14BIT,
    SONY_IMX686_MIPI_8M_50FPS_10BIT,

    SONY_IMX586_MIPI_8M_30FPS_10BIT,
    SONY_IMX586_MIPI_8M_QHDR_30FPS_10BIT,
    SONY_IMX586_MIPI_2M_QHDR_30FPS_10BIT,
    SONY_IMX586_MIPI_12M_30FPS_10BIT,
    SONY_IMX586_MIPI_48M_1FPS_10BIT,
    SONY_IMX586_MIPI_48M_3FPS_10BIT,
    SONY_IMX586_MIPI_2M_60FPS_10BIT,
    SONY_IMX586_MIPI_2M_120FPS_10BIT,

    SG2_OX03C10_MIPI_2M_WDR4TO1_BUILTIN_60FPS_12BIT,

    BG0651_MIPI_100FPS_12BIT,

    SAMPLE_SENSOR_TYPE_MAX,
} sample_sensor_type_e;

typedef struct sample_vii_physchn_cfg {
    vs_int32_t chn_id;
    vs_vii_chn_attr_s chn_attr;
    vs_lowlatency_attr_s lowlatency_attr;
} sample_vii_phys_chn_cfg_s;

typedef struct sample_vii_extchn_cfg {
    vs_int32_t chn_id;
    vs_vii_extchn_attr_s chn_attr;
} sample_vii_ext_chn_cfg_s;

typedef struct sample_vii_pipe_cfg {
    vs_uint32_t pipe_id;
    vs_vii_pipe_attr_s pipe_attr;
    vs_vii_pipe_param_s pipe_param;
    vs_vii_pipe_fpn_attr_s fpn_attr;
    vs_int8_t bus_id;
    vs_isp_comm_attr_s isp_attr;
    vs_bool_t firmware_bypass;

    vs_uint32_t phys_chn_num;
    sample_vii_phys_chn_cfg_s phys_chn_cfg[VII_MAX_PHYS_CHN_NUM];

    vs_uint32_t ext_chn_num;
    sample_vii_ext_chn_cfg_s ext_chn_cfg[VII_MAX_EXT_CHN_NUM];
} sample_vii_pipe_cfg_s;

typedef struct sample_vii_route_cfg {
    // sensor cfg
    vs_char_t sensor_name[SENSOR_MAX_NAME_LEN];

    // mipi cfg
    vs_lane_mode_e lane_mode;
    vs_mipi_rx_dev_attr_s mipi_rx_attr;

    // dev cfg
    vs_int32_t dev_id;
    vs_vii_dev_attr_s dev_attr;
    vs_vii_vc_attr_s vc_attr;

    // dev en cb
    vs_void_t *dev_en_cb_data;
    vs_int32_t (*dev_enable_cb)(vs_void_t *data);

    // bind cfg
    vs_uint32_t pipe_num;
    vs_int32_t pipe_id[DEV_BIND_MAX_PIPE_NUM];

    // pipe_cfg
    sample_vii_pipe_cfg_s pipe_cfg[DEV_BIND_MAX_PIPE_NUM];
} sample_vii_route_cfg_s;

typedef struct sample_vii_cfg {
    vs_vii_vpp_mode_e vii_vpp_mode;
    vs_uint32_t route_num;
    sample_vii_route_cfg_s route_cfg[VII_MAX_ROUTE_NUM];
} sample_vii_cfg_s;
//VII End

typedef struct {
    vs_video_frame_info_s frame_info;
    VB_BLK vb_blk;
    vs_uint32_t blk_size;
} sample_fpn_frame_info_s;
//VII End

typedef enum sample_vo_mode {
    E_VO_MODE_1MUX  ,
    E_VO_MODE_2MUX  ,
    E_VO_MODE_4MUX  ,
    E_VO_MODE_5MUX  ,
    E_VO_MODE_8MUX  ,
    E_VO_MODE_9MUX  ,
    E_VO_MODE_12MUX ,
    E_VO_MODE_16MUX ,
    E_VO_MODE_25MUX ,
    E_VO_MODE_36MUX ,
    E_VO_MODE_49MUX ,
    E_VO_MODE_64MUX ,
    VO_MODE_BUTT
} sample_vo_mode_e;

typedef enum sample_brc_mode {
    E_VENC_BRC_CBR = 0,       ///< CBR mode
    E_VENC_BRC_VBR,           ///< VBR mode
    E_VENC_BRC_AVBR,          ///< AVBR mode
    E_VENC_BRC_CVBR,          ///< CVBR mode
    E_VENC_BRC_FIXQP,         ///< FixQp mode
    E_VENC_BRC_QPMAP,         ///< QpMap mode
} sample_brc_mode_e;

typedef struct sample_venc_cfg {
    vs_pixel_format_e       format;
    vs_bool_t               compress;
    vs_payload_type_e       type;
    vs_venc_profile_e       profile;
    vs_size_s               frame_size;
    sample_brc_mode_e       brc_mode;
    vs_venc_gop_attr_s      gop_attr;
    vs_video_frc_s          frc;
    vs_uint32_t             bandwidth_save_strength;
} sample_venc_cfg_s;

typedef vs_void_t (*stream_acquire_callback)(vs_int32_t chnid, vs_void_t * p_acquire_param, vs_venc_stream_s *p_out_stream);
typedef struct sample_venc_acquire_stream_param {
    vs_int32_t              chn_num;
    vs_int32_t              venc_chnid[VENC_MAX_CHN_NUM];
    vs_bool_t               stop_stream_task;
    vs_char_t               *p_out_prefix[VENC_MAX_CHN_NUM];
    FILE                    *p_out_file[VENC_MAX_CHN_NUM];
    vs_venc_chn_attr_s      chn_attr[VENC_MAX_CHN_NUM];
    vs_int32_t              received_frame_num[VENC_MAX_CHN_NUM];
    pthread_t               venc_pid;
    vs_bool_t               store_strm;
    stream_acquire_callback stream_acquire_func;
    vs_void_t               *private_data;
} sample_venc_acquire_stream_param_s;

typedef vs_void_t (*frame_send_callback)(vs_void_t *p_send_param, vs_video_frame_info_s *p_frame_info);
typedef struct sample_venc_send_frame_param {
    vs_int32_t              chnid;         /* send moudle chnid */
    vs_uint32_t             width;         /* send moudle frame width.*/
    vs_uint32_t             height;        /* send moudle frame height.*/
    vs_pixel_format_e       pixel_format;
    vs_bool_t               compress;
    vs_uint32_t             input_alignment;
    vs_bool_t               stop_send_task;     /* stop the default send task of sample. */
    frame_send_callback     frame_send_func;
    vs_bool_t               circle_send;
    vs_void_t               *private_data;
    pthread_t               thread_pid;
    char                    input_table_path[MAX_FILE_PATH_LEN];
    char                    input_yuv_path[MAX_FILE_PATH_LEN];
    FILE                    *p_input_table;
    FILE                    *p_input_yuv;
    VB_POOL                 vb_poolid;
} sample_venc_send_frame_param_s;

typedef struct sample_venc_get_vpp_frame_param {
    vs_int32_t              vpp_grpid;
    vs_int32_t              vpp_chnid;
    vs_int32_t              venc_devid;
    vs_int32_t              venc_chnid;
    vs_bool_t               stop_send_task;
    vs_int32_t              encode_start;//start encode frame index
    vs_int32_t              encode_total;//total encode frame num
    pthread_t               thread_pid;
    vs_void_t               *private_data;
    vs_int32_t              encode_bitrate;//venc chn encode bitrate param
} sample_venc_get_vpp_frame_param_s;

typedef struct sample_venc_send_qpmap_frame_param {
    vs_int32_t              vpp_grpid;
    vs_int32_t              vpp_chnid;
    vs_uint32_t             width;
    vs_uint32_t             height;
    vs_pixel_format_e       pixel_format;
    vs_bool_t               stop_send_task;     /* stop the default send task of sample. */
    vs_void_t               *private_data;
    pthread_t               thread_pid;
    vs_int32_t              venc_chnid[VENC_MAX_CHN_NUM];
    vs_payload_type_e       venc_type[VENC_MAX_CHN_NUM];
    vs_int32_t              venc_chnnum;
} sample_venc_send_qpmap_frame_param_s;

typedef struct sample_vdec_cfg {
    vs_int32_t              vdec_chnid;
    vs_vdec_input_mode_e    input_mode;
    vs_payload_type_e       decode_type;
    vs_pixel_format_e       pixel_format;
    vs_video_format_e       video_format;
    vs_compress_mode_e      compress_mode;
    vs_uint32_t             width;
    vs_uint32_t             height;
    VB_POOL                 vb_poolid;
    vs_uint32_t             frame_buf_size;
    vs_uint32_t             frame_buf_cnt;
    vs_uint32_t             ref_frame_num;
    vs_uint32_t             output_frame_num;
} sample_vdec_cfg_s;

typedef struct sample_vdec_ltrp_info {
    vs_bool_t               ltrp_dec_enable;
    vs_uint32_t             interval_lp;
    vs_uint32_t             frame_cnt;
} sample_vdec_ltrp_info_s;

typedef struct sample_vdec_thread_param {
    vs_int32_t              vdec_chnid;
    vs_uint32_t             width;                          ///< RW; frame width of decoding channel
    vs_uint32_t             height;                         ///< RW; frame height of decoding channel
    vs_vdec_input_mode_e    input_mode;
    vs_payload_type_e       decode_type;
    vs_int32_t              send_timeout_ms;
    vs_char_t               input_file[MAX_FILE_PATH_LEN];
    vs_bool_t               is_circle_send;
    vs_bool_t               stop_send_task;
    pthread_t               tid_send;

    // ltrp stream decode
    sample_vdec_ltrp_info_s ltrp_info;
} sample_vdec_thread_param_s;

typedef struct sample_vpp_thread_param {
    vs_int32_t              vpp_grpid;
    vs_int32_t              vpp_chnid;    /* sample channel no */
    vs_bool_t               stop_task;    /* stop the default stream task of sample. */
    pthread_t               vpp_pid;
} sample_vpp_thread_param_s;

typedef enum sample_vo_output_intf_type {
    E_VO_OUTPUT_INTF_TYPE_DISABLE = 0,
    E_VO_OUTPUT_INTF_TYPE_BT1120_PROGRESSIVE,
    E_VO_OUTPUT_INTF_TYPE_BT1120_INTERLACE,
    E_VO_OUTPUT_INTF_TYPE_CVBS,
    E_VO_OUTPUT_INTF_TYPE_MIPI_DSI_TX,
    E_VO_OUTPUT_INTF_TYPE_HDMI,
    E_VO_OUTPUT_INTF_TYPE_HDMI_BT1120,
    E_VO_OUTPUT_INTF_TYPE_HDMI_MIPI,
    E_VO_OUTPUT_INTF_TYPE_BT1120_MIPI,
} sample_vo_output_intf_type_e;

typedef enum sample_vo_output_size_type {
    E_VO_OUTPUT_SIZE_TYPE_DISABLE = 0,
    E_VO_OUTPUT_SIZE_TYPE_PAL,
    E_VO_OUTPUT_SIZE_TYPE_NTSC,
    E_VO_OUTPUT_SIZE_TYPE_1920X1080P30,
    E_VO_OUTPUT_SIZE_TYPE_1920X1080I60,
    E_VO_OUTPUT_SIZE_TYPE_1920X1080P60,
    E_VO_OUTPUT_SIZE_TYPE_1080X1920_60,
    E_VO_OUTPUT_SIZE_TYPE_3840X2160_30,
    E_VO_OUTPUT_SIZE_TYPE_3840X2160_60,
    E_VO_OUTPUT_SIZE_TYPE_USER
} sample_vo_output_size_type_e;

typedef enum sample_vo_wbc_source_type {
    E_VO_SAMPLE_WBC_DISABLE = 0,
    E_VO_SAMPLE_WBC_SOURCE_TYPE_VIDEO,
    E_VO_SAMPLE_WBC_SOURCE_TYPE_GRAPHIC,
    E_VO_SAMPLE_WBC_SOURCE_TYPE_DEVICE,
    E_VO_SAMPLE_WBC_SOURCE_TYPE_MAX,
} sample_vo_wbc_source_type_e;

typedef struct sample_vo_cfg {
    vs_bool_t enable;
    vs_uint32_t vo_devid;
    vs_uint32_t vo_layerid;
    sample_vo_wbc_source_type_e wbc_type;
    vs_vo_interface_type_e vo_intf_type;
    vs_vo_output_type_e vo_output;
    vs_uint32_t bg_color;
    vs_dynamic_range_e dynamic_range;
    vs_pixel_format_e pixel_format;
    sample_vo_mode_e vo_mode;
    vs_uint32_t img_width;
    vs_uint32_t img_height;
    vs_uint32_t zorder;
    vs_bool_t rotation_enable;
    vs_uint32_t early_cfg_time;
} sample_vo_cfg_s;

typedef struct sample_vo_layer_cfg {
    vs_uint32_t        layerid;
    vs_uint32_t        devid;
    vs_uint32_t        zorder;
    vs_uint32_t        buf_depth;
    vs_rect_s          dest_rect;
    vs_size_s          image_size;
    vs_pixel_format_e  format;
    vs_dynamic_range_e dynamic_range;
    vs_uint32_t        layer_framerate;
} sample_vo_layer_cfg_s;

typedef struct sample_vo_chn_cfg {
    vs_uint32_t chnid;
    vs_uint32_t layerid;
    vs_uint32_t zorder;
    vs_rect_s   rect;
    vs_bool_t   deflicker;
} sample_vo_chn_cfg_s;

typedef enum sample_nn_model_id{
    E_NN_MODEL_ID_YOLOV5LITE = 0,
    E_NN_MODEL_ID_CENTERFACE,
    E_NN_MODEL_ID_YOLOV3,
    E_NN_MODEL_ID_MOBILENET,
    E_NN_MODEL_ID_RESNET50,
    E_NN_MODEL_ID_YOLOV5S,
    E_NN_MODEL_ID_VGG16,
    E_NN_MODEL_ID_YOLOV6S,
    E_NN_MODEL_ID_DENSENET121,

    E_NN_MODEL_ID_MAX
} sample_nn_model_id_e;

typedef enum sample_nn_model_type {
    E_NN_MODE_DETECTION,
    E_NN_MODE_CLASSIFFICATION,
    E_NN_MODE_SEGMENTATION,
    E_NN_MODE_STYLE_TRANSFER,

    E_NN_MODE_MAX
} sample_nn_model_type_e;

typedef enum sample_nn_input_mode {
    E_NN_INPUT_MODE_PRODUCER = 0,  //input data from producer_chn
    E_NN_INPUT_MODE_TENSOR_DATA,   //input data from user input
    E_NN_INPUT_MODE_MAX
} sample_nn_input_mode_e;

typedef struct sample_nn_det_obj {
    vs_uint32_t             x;
    vs_uint32_t             y;
    vs_uint32_t             w;
    vs_uint32_t             h;
    vs_uint32_t             cid;
    vs_float_t              cprob;
    vs_uint32_t             color;
} sample_nn_det_obj_s;

typedef struct sample_nn_detection_result{
    vs_uint32_t             obj_num;
    sample_nn_det_obj_s     objs[NN_MAX_DETECT_OBJ_NUM];
} sample_nn_detection_result_s;

typedef struct sample_nn_classification_result{
    vs_uint32_t             cid;
    vs_float_t              max_prob;
} sample_nn_classification_result_s;

typedef struct sample_nn_segmentation_result{
    vs_uint32_t             disp_rgn_handle;
} sample_nn_segmentation_result_s;

typedef struct sample_nn_style_transfer_result{
    vs_uint32_t             disp_rgn_handle;
} sample_nn_style_transfer_result_s;

typedef struct sample_nn_post_result{
    vs_bool_t               is_updated;
    sample_nn_model_type_e  model_type;
    union {
        sample_nn_detection_result_s        detection;
        sample_nn_classification_result_s   classification;
        sample_nn_segmentation_result_s     segmentation;
        sample_nn_style_transfer_result_s   style_transfer;
    };
} sample_nn_post_result_s;

typedef vs_int32_t (*nn_post_func)(const vs_nn_data_s *out_datas, const vs_int32_t out_num, sample_nn_post_result_s *p_post_ret);
typedef struct sample_nn_nb_info{
    vs_char_t               nb_file[NN_MAX_NB_FILE_PATH];
    vs_char_t               model_name[NN_MAX_NB_FILE_PATH];
    vs_int32_t              coast;//ms
    nn_post_func            post_proc;
    sample_nn_model_type_e  model_type;
}sample_nn_nb_info_s;

typedef struct sample_nn_thread_info{
    pthread_t               thread_id;
    pthread_mutex_t         mutex;
    vs_bool_t               stop_flag;
    sample_nn_post_result_s result;
} sample_nn_thread_info_s;

typedef struct sample_nn_tensor_data{
    //nchw
    vs_uint32_t             batch_size;
    vs_uint32_t             chn_num;
    vs_uint32_t             width;
    vs_uint32_t             height;
    vs_uint64_t             phys_addr[NN_MAX_INPUT_NODES];
    vs_void_t               *virt_addr[NN_MAX_INPUT_NODES];
} sample_nn_tensor_data_s;

typedef struct sample_nn_input_info {
    vs_uint32_t             src_num;
    sample_nn_input_mode_e  mode;
    union {
        vs_chn_s            producer_chn;                   //only vavlid for PRODUVER
        vs_nn_src_data_s    src_data[NN_MAX_INPUT_NODES];   //only vavlid for SRCDATA
    };
} sample_nn_input_info_s;

typedef struct sample_nn_output_info {
    vs_uint32_t             dst_num;
    vs_nn_dst_data_s        dst_data[NN_MAX_OUTPUT_NODES];
} sample_nn_output_info_s;

typedef struct sample_nn_info{
    vs_int32_t              cluster_id;
    sample_nn_nb_info_s     nb_info;
    vs_nn_model_s           model;
    sample_nn_thread_info_s thead_info;
    vs_uint32_t             disp_rgn_handle; //Only valid for model_type is segmentation&style_transfer, should big than 0.
    sample_nn_input_info_s  input_info;
    sample_nn_output_info_s output_info;
} sample_nn_info_s;

//define box type
typedef struct vs_box_info {
	vs_float_t x;
	vs_float_t y;
	vs_float_t w;
	vs_float_t h;
	vs_float_t score;
	vs_int32_t label;
} vs_box_info_s;

/*******************************************************
        function declaration
*******************************************************/
vs_int32_t sample_common_sys_init(vs_vb_cfg_s *vb_cfg);
vs_void_t sample_common_sys_exit(vs_void_t);
vs_int32_t sample_common_dsp_init(vs_uint32_t dsp_id, vs_char_t* name);
vs_void_t sample_common_dsp_exit(vs_uint32_t dsp_id);
vs_int32_t sample_common_dsp_support(vs_int32_t dsp_id, vs_bool_t *p_support);
vs_int32_t sample_common_vii_bind_vpp(vs_int32_t vii_pipeid, vs_int32_t vii_chnid, vs_int32_t vpp_grpid);
vs_int32_t sample_common_vii_unbind_vpp(vs_int32_t vii_pipeid, vs_int32_t vii_chnid, vs_int32_t vpp_grpid);
vs_int32_t sample_common_vii_bind_venc(vs_int32_t vii_pipeid, vs_int32_t vii_chnid, vs_int32_t venc_devid, vs_int32_t venc_chnid);
vs_int32_t sample_common_vii_unbind_venc(vs_int32_t vii_pipeid, vs_int32_t vii_chnid, vs_int32_t venc_devid, vs_int32_t venc_chnid);
vs_int32_t sample_common_vpp_bind_vo(vs_int32_t vpp_grpid, vs_int32_t vpp_chnid, vs_int32_t vo_devid, vs_int32_t vo_chnid);
vs_int32_t sample_common_vpp_unbind_vo(vs_int32_t vpp_grpid, vs_int32_t vpp_chnid, vs_int32_t vo_devid, vs_int32_t vo_chnid);
vs_int32_t sample_common_vpp_bind_venc(vs_int32_t vpp_grpid, vs_int32_t vpp_chnid, vs_int32_t venc_devid, vs_int32_t venc_chnid);
vs_int32_t sample_common_vpp_unbind_venc(vs_int32_t vpp_grpid, vs_int32_t vpp_chnid, vs_int32_t venc_devid, vs_int32_t venc_chnid);
vs_int32_t sample_common_vdec_bind_vpp(vs_int32_t vdec_devid, vs_int32_t vdec_chnid, vs_int32_t vpp_grpid);
vs_int32_t sample_common_vdec_unbind_vpp(vs_int32_t vdec_devid, vs_int32_t vdec_chnid, vs_int32_t vpp_grpid);
vs_int32_t sample_common_vdec_bind_venc(vs_int32_t vdec_devid, vs_int32_t vdec_chnid, vs_int32_t venc_devid, vs_int32_t venc_chnid);
vs_int32_t sample_common_vdec_unbind_venc(vs_int32_t vdec_devid, vs_int32_t vdec_chnid, vs_int32_t venc_devid, vs_int32_t venc_chnid);
vs_int32_t sample_common_vpp_bind_pciv(vs_int32_t vpp_grpid, vs_int32_t vpp_chnid, vs_int32_t pciv_chnid);
vs_int32_t sample_common_vpp_unbind_pciv(vs_int32_t vpp_grpid, vs_int32_t vpp_chnid, vs_int32_t pciv_chnid);
vs_int32_t sample_common_pciv_bind_vo(vs_int32_t pciv_chnid, vs_int32_t vo_devid, vs_int32_t vo_chnid);
vs_int32_t sample_common_pciv_unbind_vo(vs_int32_t pciv_chnid, vs_int32_t vo_devid, vs_int32_t vo_chnid);

vs_uint32_t sample_common_buffer_size_get(vs_size_s *img_size, vs_pixel_format_e pixel_format, vs_compress_mode_e compress_mode,
        vs_uint32_t wdr_frame_num);
// avs bind modules
vs_int32_t sample_common_vii_bind_vo(vs_int32_t vii_pipeid, vs_int32_t vii_chnid,  vs_int32_t vo_devid, vs_int32_t vo_chnid);
vs_int32_t sample_common_vii_unbind_vo(vs_int32_t vii_pipeid, vs_int32_t vii_chnid,  vs_int32_t vo_devid, vs_int32_t vo_chnid);
vs_int32_t sample_common_avs_bind_vo(vs_int32_t avs_devid, vs_int32_t avs_chnid,vs_int32_t vo_devid, vs_int32_t vo_chnid);
vs_int32_t sample_common_avs_unbind_vo(vs_int32_t avs_devid, vs_int32_t avs_chnid,vs_int32_t vo_devid, vs_int32_t vo_chnid);
vs_int32_t sample_common_vpp_bind_avs(vs_int32_t vpp_grpid, vs_int32_t vpp_chnid, vs_int32_t avs_devid, vs_int32_t avs_pipeid);
vs_int32_t sample_common_vii_bind_avs(vs_int32_t vii_pipeid, vs_int32_t vii_chnid, vs_int32_t avs_devid, vs_int32_t avs_pipeid);
vs_int32_t sample_common_vii_unbind_avs(vs_int32_t vii_pipeid, vs_int32_t vii_chnid, vs_int32_t avs_devid, vs_int32_t avs_pipeid);
vs_int32_t sample_common_vpp_unbind_avs(vs_int32_t vpp_grpid, vs_int32_t vpp_chnid, vs_int32_t avs_devid, vs_int32_t avs_pipeid);
vs_int32_t sample_common_avs_bind_venc(vs_int32_t avs_grpid, vs_int32_t avs_chnid, vs_int32_t venc_devid, vs_int32_t venc_chnid);
vs_int32_t sample_common_avs_unbind_venc(vs_int32_t avs_grpid, vs_int32_t avs_chnid, vs_int32_t venc_devid, vs_int32_t venc_chnid);
vs_int32_t sample_common_vo_bind_venc(vs_int32_t wbc_id, vs_int32_t venc_devid, vs_int32_t venc_chnid);
vs_int32_t sample_common_vo_unbind_venc(vs_int32_t wbc_id, vs_int32_t venc_devid, vs_int32_t venc_chnid);

vs_void_t sample_common_pause(vs_void_t);

//VII Start
vs_int32_t sample_common_vii_sensor_type_num_get();
const char *sample_common_sensor_type_name_get(sample_sensor_type_e sensor_type);
vs_void_t sample_common_vii_sensor_name_get(sample_sensor_type_e sensor_type, char *sensor_name);
vs_uint32_t sample_common_vii_wdr_frame_num_get(vs_int32_t sensor_id);
vs_uint32_t sample_common_vii_wdr_frame_num_get_by_sensor(sample_sensor_type_e sensor_type);
vs_void_t sample_common_vii_sensor_img_size_get(vs_int32_t sensor_id, vs_size_s *img_size);
vs_void_t sample_common_vii_img_size_get(sample_sensor_type_e sensor_type, vs_size_s *img_size);
vs_int32_t sample_common_vii_framerate_get(sample_sensor_type_e sensor_type, vs_int32_t *p_framerate);
vs_void_t sample_common_vii_sensor_pixel_format_get(vs_int32_t sensor_id, vs_pixel_format_e *format);
vs_int32_t sample_common_vii_sensor_framerate_get(vs_int32_t sensor_id, vs_int32_t *p_framerate);
vs_void_t sample_common_vii_default_cfg_get(vs_int32_t sensor_id, sample_vii_route_cfg_s *route_cfg);
vs_int32_t sample_common_vii_start(sample_vii_cfg_s *vii_cfg);
vs_void_t sample_common_vii_stop(sample_vii_cfg_s *vii_cfg);
#ifndef VS_NO_ISP
vs_int32_t sample_common_vii_mirror_mode_set(vs_int32_t pipe_id, vs_mirror_mode_e mode);
#endif
vs_uint32_t sample_common_vii_pipe_frame_stride_get(vs_pixel_format_e pixel_format, vs_uint32_t width, vs_compress_mode_e cmp_mode);
vs_void_t sample_common_vii_pipe_release_fpn_frm_vb_blk(sample_fpn_frame_info_s *fpn_frame_info);
vs_void_t sample_common_vii_read_raw(FILE *pfd, vs_video_frame_s *p_frame);
vs_void_t sample_common_get_fpn_cfg(sample_vii_pipe_cfg_s *pipe_cfg, sample_fpn_frame_info_s *fpn_info);
vs_void_t sample_common_vii_virt_pipe_cfg_get(vs_int32_t sensor_id, sample_vii_pipe_cfg_s *pipe_cfg);
vs_int32_t sample_common_vii_virt_pipe_start(vs_int32_t sensor_id, sample_vii_pipe_cfg_s *pipe_cfg);
vs_void_t sample_common_vii_virt_pipe_stop(sample_vii_pipe_cfg_s *pipe_cfg);
//VII End

vs_int32_t sample_common_vpp_rotation_enable(vs_int32_t chnid);
vs_int32_t sample_common_vpp_rotation_disable(vs_int32_t vpp_grpid, vs_int32_t chnid);
vs_int32_t sample_common_vpp_start(vs_int32_t vpp_grpid, vs_bool_t* p_chn_enable,
    vs_vpp_grp_attr_s* p_vpp_grp_attr, vs_vpp_chn_attr_s* p_vpp_chn_attr);
vs_int32_t sample_common_vpp_lowlatency_start(vs_int32_t vpp_grpid, vs_bool_t* p_chn_enable,
    vs_vpp_grp_attr_s* p_vpp_grp_attr, vs_vpp_chn_attr_s* p_vpp_chn_attr);
vs_int32_t sample_common_vpp_stop(vs_int32_t vpp_grpid, vs_bool_t* p_chn_enable);
vs_int32_t sample_comm_vpp_extchn_attr_set(vs_int32_t vpp_grpid, vs_bool_t* p_chn_enable, vs_vpp_extchn_attr_s* p_vpp_chn_attr);
vs_int32_t sample_common_vpp_chn_acquire_start(vs_int32_t vpp_grpid, vs_int32_t vpp_chnid);
vs_int32_t sample_common_vpp_chn_acquire_stop();

vs_int32_t sample_common_venc_start(vs_int32_t venc_chnid, sample_venc_cfg_s *p_sample_venc_cfg);
vs_int32_t sample_common_venc_stop(vs_int32_t venc_chnid);
vs_int32_t sample_common_venc_bitrate_set(vs_int32_t venc_chnid, vs_uint32_t bitrate);
vs_int32_t sample_common_venc_mpf_start(vs_int32_t venc_chnid, sample_venc_cfg_s *p_sample_venc_cfg, vs_venc_jpeg_attr_s *p_jpeg_attr);
vs_int32_t sample_common_venc_mpf_stop(vs_int32_t venc_chnid);
vs_int32_t sample_common_venc_acquire_jpeg_mpf_process(sample_venc_acquire_stream_param_s *p_acquire_stream_param, vs_int32_t rcvframe_num);
vs_int32_t sample_common_venc_gop_attr_get(vs_venc_gop_mode_e gop_mode, vs_venc_gop_attr_s *p_gop_attr);
vs_int32_t sample_common_venc_acquire_stream_start(sample_venc_acquire_stream_param_s *p_acquire_stream_param);
vs_int32_t sample_common_venc_acquire_stream_stop(sample_venc_acquire_stream_param_s *p_acquire_stream_param);
vs_int32_t sample_common_venc_send_frame_start(sample_venc_send_frame_param_s *p_send_frame_param);
vs_int32_t sample_common_venc_send_frame_stop(sample_venc_send_frame_param_s *p_send_frame_param);
vs_int32_t sample_common_venc_user_vb_pool_init(vs_size_s frame_size, vs_pixel_format_e format, vs_bool_t compress, VB_POOL *p_vb_poolid);
vs_int32_t sample_common_venc_user_vb_pool_deinit(VB_POOL vb_poolid);
vs_int32_t sample_common_venc_send_qpmap_frame_start(sample_venc_send_qpmap_frame_param_s *p_send_qpmap_param);
vs_int32_t sample_common_venc_send_qpmap_frame_stop(sample_venc_send_qpmap_frame_param_s *p_send_qpmap_param);
vs_int32_t sample_common_venc_framerate_set(vs_int32_t venc_chnid, vs_video_frc_s framerate);
vs_int32_t sample_common_venc_video_vui_enable(vs_int32_t venc_chnid);

vs_int32_t sample_common_vdec_vb_pool_init(vs_int32_t vdec_chnnum, sample_vdec_cfg_s *p_sample_vdec_cfg);
vs_int32_t sample_common_vdec_vb_pool_deinit(vs_int32_t vdec_chnnum, sample_vdec_cfg_s *p_sample_vdec_cfg);
vs_int32_t sample_common_vdec_start(vs_int32_t vdec_chnnum, sample_vdec_cfg_s *p_sample_vdec_cfg);
vs_int32_t sample_common_vdec_stop(vs_int32_t vdec_chnnum, sample_vdec_cfg_s *p_sample_vdec_cfg);
vs_int32_t sample_common_vdec_stream_send_start(vs_int32_t vdec_chnnum, sample_vdec_thread_param_s *p_vdec_thread_param);
vs_int32_t sample_common_vdec_stream_send_stop(vs_int32_t vdec_chnnum, sample_vdec_thread_param_s *p_vdec_thread_param);
vs_void_t sample_common_vdec_cmdrecv(vs_void_t);

void sample_common_vo_timing_get(vs_vo_dev_attr_s *attr, vs_vo_clk_info_s *clk);
vs_int32_t sample_common_vo_start(sample_vo_cfg_s* p_vo_config);
vs_int32_t sample_common_vo_stop(sample_vo_cfg_s* p_vo_config);
vs_int32_t sample_common_vo_disp1_cfg_get(enum sample_vo_output_intf_type type, sample_vo_cfg_s *vo_config);
vs_int32_t sample_common_vo_layer_enable(sample_vo_cfg_s* p_vo_config);
vs_int32_t sample_common_vo_chn_enable(sample_vo_cfg_s* p_vo_config);
vs_int32_t sample_common_vo_chn_disable(sample_vo_cfg_s* p_vo_config);
vs_void_t sample_common_vo_output_rect_get(const sample_vo_cfg_s* p_vo_config, vs_rect_s *p_rect);
vs_void_t sample_common_wbc_size_get(sample_vo_cfg_s* p_vo_config, vs_size_s* size);
vs_int32_t sample_common_vo_wbc_enable(sample_vo_cfg_s* p_vo_config);
vs_void_t sample_common_vo_dest_rect_get(sample_vo_cfg_s* p_vo_config, vs_rect_s *p_rect);
vs_int32_t sample_common_vo_chn_add(sample_vo_chn_cfg_s* p_chn_cfg);
vs_int32_t sample_common_vo_chn_remove(vs_int32_t layerid, vs_int32_t chnid);
vs_int32_t sample_common_vo_layer_add(sample_vo_layer_cfg_s* p_layer_cfg);
vs_int32_t sample_common_vo_layer_remove(vs_int32_t devid, vs_int32_t layerid);

#ifndef VS_ORION
vs_int32_t sample_common_hdmi_start(vs_vo_output_type_e output, vs_vo_interface_type_e vo_intf_type);
vs_int32_t sample_common_hdmi_stop();
#endif

/*******************************************************
        Audio function declaration
*******************************************************/
vs_int32_t sample_common_adec_aout_bind(vs_int32_t adec_chnid, vs_int32_t ao_devid, vs_int32_t ao_chnid);
vs_int32_t sample_common_adec_aout_unbind(vs_int32_t adec_chnid, vs_int32_t ao_devid, vs_int32_t ao_chnid);
vs_int32_t sample_common_ain_aout_bind(vs_int32_t ai_devid, vs_int32_t ai_chnid,
                                     vs_int32_t ao_devid, vs_int32_t ao_chnid);
vs_int32_t sample_common_ain_aout_unbind(vs_int32_t ai_devid, vs_int32_t ai_chnid,
                                       vs_int32_t ao_devid, vs_int32_t ao_chnid);
vs_int32_t sample_common_ain_aenc_bind(vs_int32_t ai_devid, vs_int32_t ai_chnid, vs_int32_t aenc_chnid);
vs_int32_t sample_common_ain_aenc_unbind(vs_int32_t ai_devid, vs_int32_t ai_chnid, vs_int32_t aenc_chnid);
vs_int32_t sample_common_aenc_adec_bind(vs_int32_t aenc_chnid, vs_int32_t adec_chnid);
vs_int32_t sample_common_aenc_adec_unbind(vs_int32_t aenc_chnid, vs_int32_t adec_chnid);
vs_int32_t sample_common_adec_aenc_bind(vs_int32_t adec_chnid, vs_int32_t aenc_chnid);
vs_int32_t sample_common_adec_aenc_unbind(vs_int32_t adec_chnid, vs_int32_t aenc_chnid);
vs_int32_t vs_acodec_dac_stop(vs_int32_t ao_devid);
vs_int32_t sample_common_ain_source_thread_create(vs_int32_t ai_devid, vs_int32_t ai_chnid, vs_int32_t aenc_chnid,
                                                vs_int32_t ao_devid, vs_int32_t ao_chnid, vs_bool_t send_to_file);
vs_int32_t sample_common_ain_source_thread_destroy(vs_int32_t ai_devid, vs_int32_t ai_chnid);
vs_int32_t sample_common_ain_multi_channel_thread_create(vs_int32_t ai_devid, vs_int32_t ai_chn_num, vs_bool_t send_to_file);
vs_int32_t sample_common_ain_multi_channel_thread_destroy(vs_int32_t ai_devid);
vs_int32_t sample_common_ain_start(vs_int32_t ai_devid, vs_aiodev_attr_s *p_aio_attr, vs_int32_t out_samplerate, void *p_aqe_attr, vs_int32_t aqe_type);
vs_int32_t sample_common_ain_stop(vs_int32_t ai_devid, vs_int32_t chn_num, vs_bool_t aqe_en);
vs_int32_t sample_common_aout_sink_thread_create(vs_int32_t ao_devid, vs_int32_t ao_chnid,
                                               vs_aiodev_attr_s *p_aio_common_attr, FILE *fp);
vs_int32_t sample_common_aout_sink_thread_destroy(vs_int32_t ao_devid, vs_int32_t ao_chnid);
vs_int32_t sample_common_hdmi_aout_start(vs_int32_t ao_devid, vs_aiodev_attr_s *p_aio_attr);
vs_int32_t sample_common_aout_start(vs_int32_t ao_devid, vs_aiodev_attr_s *p_aio_attr, vs_int32_t in_samplerate, vs_void_t *p_aqe_attr);
vs_int32_t sample_common_aout_stop(vs_int32_t ao_devid, vs_int32_t chn_num, vs_bool_t aqe_en);
vs_int32_t sample_common_aenc_sink_thread_create(vs_int32_t aenc_chnid, vs_aiodev_attr_s *p_aio_common_attr,vs_payload_type_e encoder_type, FILE *fp);
vs_int32_t sample_common_aenc_sink_thread_destroy(vs_int32_t aenc_chnid);
vs_int32_t sample_common_aenc_source_thread_create(vs_int32_t aenc_chnid, vs_int32_t adec_chnid, vs_payload_type_e encoder_type, vs_bool_t send_to_file);
vs_int32_t sample_common_aenc_source_thread_destroy(vs_int32_t aenc_chnid);
vs_int32_t sample_common_aenc_start(vs_int32_t aenc_chnid, vs_int32_t paired_chn_num,
                                  vs_aiodev_attr_s *p_aio_attr, vs_payload_type_e encoder_type);
vs_int32_t sample_common_aenc_stop(vs_int32_t aenc_chnid, vs_int32_t paired_chn_num);
vs_int32_t sample_common_adec_source_thread_create(vs_int32_t adec_chnid, vs_int32_t ao_devid, vs_int32_t ao_chnid,
                                                 vs_int32_t aenc_chnid, vs_bool_t send_to_file);
vs_int32_t sample_common_adec_source_thread_destroy(vs_int32_t adec_chnid);
vs_int32_t sample_common_adec_sink_thread_create(vs_int32_t adec_chnid,vs_payload_type_e decoder_type, FILE *fp);
vs_int32_t sample_common_adec_sink_thread_destroy(vs_int32_t adec_chnid);
vs_int32_t sample_common_adec_start(vs_int32_t adec_chnid, vs_payload_type_e decoder_type);
vs_int32_t sample_common_adec_stop(vs_int32_t adec_chnid);
vs_int32_t sample_common_acodec_set(vs_int32_t input_gain, vs_int32_t output_gain, vs_int32_t mic_boost, vs_int32_t mic_gain, vs_int32_t adc_gain,
                                    vs_int32_t dac_gain, vs_int32_t hp_gain);
vs_int32_t sample_common_acodec_gain_get(void);
vs_int32_t sample_common_rtc_speech_jb_init();
vs_int32_t sample_common_rtc_speech_jb_exit();
vs_int32_t sample_common_rtc_prepare(vs_int32_t local_port, const char *remote_ip, vs_int32_t remote_port, vs_int32_t is_speech);
vs_int32_t sample_common_rtc_recv_thread_create();
vs_int32_t sample_common_rtc_send_thread_create();
vs_int32_t sample_common_rtc_recv_thread_destroy();
vs_int32_t sample_common_rtc_send_thread_destroy();
vs_int32_t sample_common_rtc_aout_thread_destroy();
vs_int32_t sample_common_rtc_aout_thread_create(vs_int32_t ao_devid, vs_int32_t ao_chnid, vs_aiodev_attr_s *p_aio_common_attr);
vs_int32_t sample_common_rtc_ain_thread_destroy();
vs_int32_t sample_common_rtc_ain_thread_create(vs_int32_t ai_devid, vs_int32_t ai_chnid);
vs_int32_t sample_common_acodec_alc_set(void *attr);

/*******************************************************
        NN function declaration
*******************************************************/
vs_int32_t sample_common_nn_nb_info_get(sample_nn_model_id_e model_id, vs_bool_t is_single_model, sample_nn_nb_info_s *p_nb_info);
vs_int32_t sample_common_nn_io_data_size_get(const vs_nn_node_s *p_node);
vs_int32_t sample_common_nn_init();
vs_int32_t sample_common_nn_create(sample_nn_info_s *p_nn_info);
vs_int32_t sample_common_nn_run(sample_nn_info_s *p_nn_info);
vs_int32_t sample_common_nn_result_get(sample_nn_info_s *p_nn_info, sample_nn_post_result_s *p_results);
vs_int32_t sample_common_nn_stop(sample_nn_info_s *p_nn_info);
vs_int32_t sample_common_nn_destroy(sample_nn_info_s *p_nn_info);
vs_int32_t sample_common_nn_exit();

vs_int32_t sample_common_nn_input_buffer_create_by_file(const vs_nn_model_s *p_model, vs_nn_src_data_s *p_src_data, const vs_char_t *input_file[]);
vs_int32_t sample_common_nn_input_buffer_destroy(const vs_nn_model_s *p_model, vs_nn_src_data_s *p_src_data);
/*******************************************************
        RGN function declaration
*******************************************************/
vs_uint32_t sample_common_rgn_get_min_handle(vs_rgn_type_e rgn_type);
vs_uint32_t sample_common_rgn_get_update_surface(vs_uint32_t handle);
vs_uint32_t sample_common_region_create(vs_uint32_t handle_num, vs_rgn_type_e rgn_type);
vs_uint32_t sample_common_rgn_destroy(vs_uint32_t handle_num, vs_rgn_type_e rgn_type);
vs_uint32_t sample_common_rgn_unbind(vs_uint32_t handle_num, vs_rgn_type_e rgn_type, const vs_chn_s *p_chn);
vs_uint32_t sample_common_rgn_chn_bind(vs_uint32_t handle_num, vs_rgn_type_e rgn_type, const vs_chn_s *p_chn);

#ifdef __cplusplus
}
#endif

#endif ///< end of _SAMPLE_COMMON_H_
