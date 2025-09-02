/**
* @file vs_mipi_tx_defines.h
* @brief data structures for mipi tx
* @details contains data structures for mipi tx
* @author VS platform group
* @date 2022-02-16
* @version v0.10
* @Copyright (C) 2022, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/

#ifndef __VS_MIPI_TX_DEFINES_H__
#define __VS_MIPI_TX_DEFINES_H__

#include "vs_type.h"
#include "vs_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

#define VS_ERR_MIPI_TX_INVALID_PARAM    VS_ERR_CODE(E_MOD_ID_MIPI_TX, E_ERR_INVALID_PARAM)
#define VS_ERR_MIPI_TX_INVALID_DEVID    VS_ERR_CODE(E_MOD_ID_MIPI_TX, E_ERR_INVALID_DEVID)
#define VS_ERR_MIPI_TX_BAD_STATE        VS_ERR_CODE(E_MOD_ID_MIPI_TX, E_ERR_BAD_STATE)
#define VS_ERR_MIPI_TX_NULL_PTR         VS_ERR_CODE(E_MOD_ID_MIPI_TX, E_ERR_NULL_PTR)
#define VS_ERR_MIPI_TX_NO_MEMORY        VS_ERR_CODE(E_MOD_ID_MIPI_TX, E_ERR_NO_MEMORY)
#define VS_ERR_MIPI_TX_BAD_ADDR         VS_ERR_CODE(E_MOD_ID_MIPI_TX, E_ERR_BAD_ADDR)
#define VS_ERR_MIPI_TX_BUSY             VS_ERR_CODE(E_MOD_ID_MIPI_TX, E_ERR_BUSY)
#define VS_ERR_MIPI_TX_READ             VS_ERR_CODE(E_MOD_ID_MIPI_TX, E_ERR_READ)
#define VS_ERR_MIPI_TX_WRITE            VS_ERR_CODE(E_MOD_ID_MIPI_TX, E_ERR_WRITE)
#define VS_ERR_MIPI_TX_DEVICE_UNEXIST   VS_ERR_CODE(E_MOD_ID_MIPI_TX, E_ERR_DEVICE_UNEXIST)

#define MIPI_TX_LANE_MAX_NUM            4

/**
* @brief data structure for write command
*/
typedef struct vs_mipi_tx_cmd_write {
    vs_uint16_t         data_type;
    union {
        vs_uint16_t     data_size;
        vs_uint16_t     data;
    }; ///< re-used as short data when p_data is NULL
    vs_uchar_t          *p_data;
} vs_mipi_tx_cmd_write_s;

/**
* @brief data structure for read command
*/
typedef struct vs_mipi_tx_cmd_read {
    vs_uint16_t         data_type;
    vs_uint16_t         data_param;
    vs_uint16_t         data_size;
    vs_uchar_t          *p_data;
} vs_mipi_tx_cmd_read_s;


/**
* @brief DSI mode definition
*/
typedef enum vs_mipi_tx_mode {
    E_MIPI_TX_MODE_DSI_VIDEO = 0, ///< dsi video mode
    E_MIPI_TX_MODE_DSI_CMD, ///< dsi command mode
    E_MIPI_TX_MODE_MAX
} vs_mipi_tx_mode_e;

/**
* @brief DSI video mode definition
*/
typedef enum vs_mipi_tx_video_mode {
    E_NON_BURST_MODE_SYNC_PULSES = 0,
    E_NON_BURST_MODE_SYNC_EVENTS,
    E_BURST_MODE,
    E_DSI_MODE_MAX
} vs_mipi_tx_video_mode_e;

/*
* @details
* Note:
* 1. No 48-bit support in dsi databook.
* 2. DSC24 not supported in DPI
* 3. YUV is not supported by DPU.
*/
typedef enum vs_mipi_tx_pixel_format {
    E_MIPI_TX_PIXEL_RGB888 = 0,
    E_MIPI_TX_PIXEL_RGB666,
    E_MIPI_TX_PIXEL_RGB565,
    E_MIPI_TX_PIXEL_FORMAT_MAX
} vs_mipi_tx_pixel_format_e;

/**
* @brief DSI sync/timing information
*/
typedef struct vs_mipi_tx_sync_info {
    vs_uint16_t     packet_size; ///< num of pixels in a single video packet, equals to horizontal display size
    vs_uint16_t     hpw; ///< horizontal sync pulse width, in pixels
    vs_uint16_t     hbp; ///< horizontal back porch, in pixels
    vs_uint16_t     htotal; ///< horizontal pixels in total
    vs_uint16_t     vpw; ///< vertical sync pulse width, in lines
    vs_uint16_t     vbp; ///< vertical back porch, in lines
    vs_uint16_t     vfp; ///< vertical front porch, in lines
    vs_uint16_t     vactive; ///< vertical active lines
} vs_mipi_tx_sync_info_s;

/**
* @brief MIPI tx configuration
*/
typedef struct vs_mipi_tx_config {
    vs_uint8_t      lanes; ///< support 1/2/3/4 lanes
    vs_mipi_tx_mode_e           tx_mode; ///< DSI_VIDEO/DSI_CMD
    vs_mipi_tx_video_mode_e     video_mode;
    vs_mipi_tx_pixel_format_e   pixel_format;
    vs_mipi_tx_sync_info_s      sync_info;
    vs_uint32_t     phy_data_rate; ///< unit: mbps
    vs_uint32_t     pixel_clk; ///< unit: khz
} vs_mipi_tx_config_s;

#ifdef __cplusplus
}
#endif

#endif ///< end of __VS_MIPI_TX_DEFINES_H__

