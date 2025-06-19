/**
* @file sample_pciv_common.h
* @brief sample pciv common interface
* @details contains APIs
* @author Visinex Software Group
* @date 2023-03-04
* @version v0.10
* @Copyright (C) 2022, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/


#ifndef _SAMPLE_PCIV_COMMON_H_
#define _SAMPLE_PCIV_COMMON_H_

#include "sample_common.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SAMPLE_PCIV_MSG_MAX_LEN          1024
#define SAMPLE_PCIV_MSG_DATA_MAX_LEN     1008
#define SAMPLE_PCIV_MSG_PORT        0
#define SAMPLE_PCIV_SEND_LEN   512*1024
#define SAMPLE_PCIV_STREAM_BUF_LEN (SAMPLE_PCIV_SEND_LEN*6)

typedef enum sample_pciv_msg_type {
    SAMPLE_PCIV_MSG_START_VII = 0,
    SAMPLE_PCIV_MSG_STOP_VII,
    SAMPLE_PCIV_MSG_START_VPP,
    SAMPLE_PCIV_MSG_STOP_VPP,
    SAMPLE_PCIV_MSG_VII_BIND_VPP,
    SAMPLE_PCIV_MSG_VII_UNBIND_VPP,
    SAMPLE_PCIV_MSG_START_PCIV,
    SAMPLE_PCIV_MSG_STOP_PCIV,
    SAMPLE_PCIV_MSG_VPP_BIND_PCIV,
    SAMPLE_PCIV_MSG_VPP_UNBIND_PCIV,
    SAMPLE_PCIV_MSG_WIN_BUFF_INIT,
    SAMPLE_PCIV_MSG_WIN_BUFF_EXIT,
    SAMPLE_PCIV_MSG_RECEIVER_INIT,
    SAMPLE_PCIV_MSG_RECEIVER_EXIT,
    SAMPLE_PCIV_MSG_STREAM_STOP,
    SAMPLE_PCIV_MSG_RESULT,
    SAMPLE_PCIV_MSG_WRITE_DONE,
    SAMPLE_PCIV_MSG_READ_DONE,
    SAMPLE_PCIV_MSG_NN_INIT,
    SAMPLE_PCIV_MSG_NN_EXIT,
    SAMPLE_PCIV_MSG_NN_RUN,
    SAMPLE_PCIV_MSG_NN_STOP,
    SAMPLE_PCIV_MSG_SYS_EXIT,
    SAMPLE_PCIV_MSG_NUM
} sample_pciv_msg_type_e;

typedef struct sample_pciv_msg_head
{
    vs_uint32_t                     target_id;
    sample_pciv_msg_type_e          msg_type;
    vs_uint32_t                     data_len;
    vs_int32_t                      result;
} sample_pciv_msg_head_s;

typedef struct sample_pciv_msg
{
    sample_pciv_msg_head_s          head;
    vs_uint8_t                      data[SAMPLE_PCIV_MSG_DATA_MAX_LEN];
} sample_pciv_msg_s;

/* nn */
typedef struct sample_pciv_nn_cfg {
    sample_nn_model_id_e    model_id;
    vs_uint32_t             cluster_id;
    pthread_t               pid;
    vs_size_s               nn_frame_size;
    vs_uint32_t             nn_rate;
} sample_pciv_nn_cfg_s;

typedef struct sample_pciv_frame {
    vs_uint32_t             blk_size;
    vs_uint32_t             width;
    vs_uint32_t             height;
    vs_uint64_t             phys_addr;
} sample_pciv_frame_s;
typedef struct sample_pciv_nn_ctx {
    sample_pciv_nn_cfg_s    nn_cfg;
    sample_pciv_frame_s     nn_frame;
    sample_nn_post_result_s nn_results;
} sample_pciv_nn_ctx_s;

typedef struct sample_pciv_nn_send_param {
    vs_int32_t              pciv_id;
    vs_int32_t              vpp_grpid;
    vs_int32_t              vpp_chnid;
    pthread_t               pid;
    vs_void_t               *p_local_buf;
    vs_bool_t               start;
    sample_pciv_nn_ctx_s    nn_ctx;
} sample_pciv_nn_send_param_s;

typedef struct sample_pciv_win_buff_cfg {
    vs_uint32_t              blk_size;
    vs_uint32_t              blk_cnt;
    vs_pciv_win_cfg_s        win_cfg;
    vs_uint64_t              phys_addr[PCIV_MAX_BUF_NUM];
} sample_pciv_win_buff_cfg_s;

typedef struct sample_pciv_stream_ctx {
    vs_uint32_t     chn;
    pthread_t       pid;
    vs_bool_t       start;
    char            file_name[64];
    vs_void_t       *p_trans_handle;
    vs_uint32_t     msg_port_write;
    vs_uint32_t     msg_port_read;
} sample_pciv_stream_ctx_s;

#define PCIV_STREAM_MAGIC 0x55555555

typedef struct sample_pciv_stream_head {
    vs_uint32_t     magic;
    vs_uint32_t     pay_load;
    vs_uint32_t     dma_data_len;
    vs_uint32_t     stream_len;
    vs_uint32_t     seq;
    vs_uint64_t     pts;
    vs_uint32_t     chn_id;
    vs_bool_t       field_end;
    vs_bool_t       frame_end;
    vs_uint32_t     venc_data_type;
} sample_pciv_stream_head_s;

/*========================================TRANS DEFINE========================================*/

typedef struct sample_pciv_trans_attr {
    vs_uint32_t              chip_id;
    vs_bool_t                bReciver;           /* receiver or sender ? */
    vs_uint32_t              buf_size;
    vs_uint64_t              phys_addr;
    vs_uint32_t              msg_port_write;
    vs_uint32_t              msg_port_read;
    vs_uint32_t              chn_Id;
} sample_pciv_trans_attr_s;

typedef struct sample_pciv_rmt_buf {
    vs_uint64_t              base_addr;
    vs_uint32_t              length;
    vs_uint32_t              read_pos;
    vs_uint32_t              write_pos;
} sample_pciv_rmt_buf_s;

typedef struct sample_pciv_local_buf {
    vs_uint32_t              vb_blk;
    vs_uint8_t               *p_base_addr;
    vs_uint64_t              phys_addr;
    vs_uint32_t              buf_len;
    vs_uint32_t              cur_len;
} sample_pciv_local_buf_s;

typedef struct sample_pciv_sender {
    vs_bool_t                init;
    vs_uint32_t              chip_id;
    sample_pciv_rmt_buf_s    rmt_buf;
    sample_pciv_local_buf_s  local_buf;
    vs_uint32_t              msg_port_write;
    vs_uint32_t              msg_port_read;
    pthread_t                pid;
    vs_bool_t                start;
    vs_uint32_t              msg_seq_send;
    vs_uint32_t              msg_seq_free;
} sample_pciv_sender_s;

typedef struct sample_pciv_receiver {
    vs_bool_t                init;
    vs_uint32_t              chip_id;
    vs_uint8_t               *p_base_addr;
    vs_uint32_t              buf_len;
    vs_uint32_t              msg_port_write;
    vs_uint32_t              msg_port_read;
    vs_uint32_t              msg_seq_send;
    vs_uint32_t              msg_seq_free;
} sample_pciv_receiver_s;

typedef struct sample_pciv_trans_notify {
    vs_uint32_t              start;
    vs_uint32_t              end;
    vs_uint32_t              seq;
} sample_pciv_trans_notify_s;

typedef struct sample_pciv_buf_stat {
    vs_uint32_t              free_len;
} sample_pciv_buf_stat_s;

#ifdef __cplusplus
}
#endif

#endif //_SAMPLE_PCIV_COMMON_H_