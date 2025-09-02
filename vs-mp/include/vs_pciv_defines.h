/**
* @file vs_pciv_defines.h
* @brief Declaraction of pciv enumeration and struct
* @details Declaration of pciv enumeration and struct
* @author VS platform group
* @date 2023-02-21
* @version v0.1
* @Copyright (C) 2023, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/
#ifndef __VS_PCIV_DEFINES_H__
#define __VS_PCIV_DEFINES_H__

#include "vs_type.h"
#include "vs_errno.h"
#include "vs_comm.h"
#include "vs_comm_video.h"
#include "vs_capability.h"

#ifdef __cplusplus
extern "C" {
#endif

///< parameter is illagal
#define VS_ERR_PCIV_INVALID_PARAM       VS_ERR_CODE(E_MOD_ID_PCIV, E_ERR_INVALID_PARAM)
///< invlalid channel ID */
#define VS_ERR_PCIV_INVALID_CHNID       VS_ERR_CODE(E_MOD_ID_PCIV, E_ERR_INVALID_CHNID)
///< using a NULL pointer
#define VS_ERR_PCIV_NULL_PTR            VS_ERR_CODE(E_MOD_ID_PCIV, E_ERR_NULL_PTR)
///< channel has been created
#define VS_ERR_PCIV_EXIST               VS_ERR_CODE(E_MOD_ID_PCIV, E_ERR_EXIST)
///< channel is not created
#define VS_ERR_PCIV_UNEXIST             VS_ERR_CODE(E_MOD_ID_PCIV, E_ERR_UNEXIST)
///< system is not ready
#define VS_ERR_PCIV_NOT_INITIALIZED     VS_ERR_CODE(E_MOD_ID_PCIV, E_ERR_NOT_INITIALIZED)
///< operation is not supported
#define VS_ERR_PCIV_NOT_PERM            VS_ERR_CODE(E_MOD_ID_PCIV, E_ERR_NOT_PERM)
///< no memory for allocated
#define VS_ERR_PCIV_NO_MEMORY           VS_ERR_CODE(E_MOD_ID_PCIV, E_ERR_NO_MEMORY)
///< timeout
#define VS_ERR_PCIV_TIMEOUT             VS_ERR_CODE(E_MOD_ID_PCIV, E_ERR_TIMEOUT)
///< read operation failed
#define VS_ERR_PCIV_READ                VS_ERR_CODE(E_MOD_ID_PCIV, E_ERR_READ)
///< device unexist
#define VS_ERR_PCIV_DEVICE_UNEXIST      VS_ERR_CODE(E_MOD_ID_PCIV, E_ERR_DEVICE_UNEXIST)

/**
* @brief Defines the picture attributes of pciv channel.
* @details
*/
typedef struct vs_pciv_pic_attr {
	vs_uint32_t width;                  ///< picture width
	vs_uint32_t height;                 ///< picture height
	vs_uint32_t stride[3];              ///< picture stride
	vs_pixel_format_e pixel_format;     ///< picture pixel format, only support SP422、SP420
	vs_video_format_e video_format;     ///< picture video format, only support E_VIDEO_FORMAT_LINEAR
	vs_compress_mode_e compress_mode;   ///< picture compress mode
	vs_color_gamut_e color_gamut;       ///< picture color gamut
	vs_dynamic_range_e dynamic_range;   ///< picture dynamic range
} vs_pciv_pic_attr_s;

/**
* @brief Defines the opposite object of pciv channel.
* @details
*/
typedef struct vs_pciv_remote_obj {
	vs_uint32_t chip_id;                ///< opposite pcie device id
	vs_uint32_t pciv_ch;                ///< opposite pciv channel id
} vs_pciv_remote_obj_s;

/**
* @brief Defines the channel attributes of pciv channel.
* @details
*/
typedef struct vs_pciv_attr {
	vs_pciv_pic_attr_s pic_attr;                ///< picture attributes
	vs_uint32_t blk_size;                       ///< video buffer size
	vs_uint32_t blk_cnt;                        ///< video buffer count, value range[PCIV_MIN_BUF_NUM, PCIV_MAX_BUF_NUM].
	vs_uint64_t phys_addr[PCIV_MAX_BUF_NUM];    ///< video buffer physical address
	vs_pciv_remote_obj_s remote_obj;            ///< opposite pcie device object
} vs_pciv_attr_s;

/**
* @brief Defines the vb pool configuration of window buffer.
* @details
*/
typedef struct vs_pciv_win_cfg {
	vs_uint32_t pool_cnt;                   ///< vb pool count, value range[PCIV_MIN_VB_CNT, PCIV_MAX_VB_CNT].
	vs_uint32_t blk_size[PCIV_MAX_VB_CNT];  ///< block size
	vs_uint32_t blk_cnt[PCIV_MAX_VB_CNT];   ///< block count
} vs_pciv_win_cfg_s;

/**
* @brief Defines the block information of DMA transmission.
* @details
*/
typedef struct vs_pciv_dma_block {
	vs_uint64_t src;                    ///< source physical address
	vs_uint64_t dst;                    ///< destination physical address
	vs_uint32_t blk_size;               ///< block size
} vs_pciv_dma_block_s;

/**
* @brief Defines the task information of DMA transmission.
* @details
*/
typedef struct vs_pciv_dma_task {
	vs_uint32_t count;                  ///< task count, value range[PCIV_MIN_DMABLK, PCIV_MAX_DMABLK].
	vs_bool_t is_read;                  ///< whether the transmission direction is read
	vs_pciv_dma_block_s *p_block;       ///< block information
} vs_pciv_dma_task_s;

#ifdef __cplusplus
}
#endif

#endif
