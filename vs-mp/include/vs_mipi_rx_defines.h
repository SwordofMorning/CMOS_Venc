/**
* @file
* @brief mipi_rx defines
* @details mipi_rx defines
* @author VS software group
* @date 2022-05-16
* @version v1.0
* @Copyright (C) 2022, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/

#ifndef __VS_MIPI_RX_DEFINES_H__
#define __VS_MIPI_RX_DEFINES_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "vs_type.h"
#include "vs_errno.h"
#include "vs_comm_video.h"

#define VS_ERR_MIPI_RX_INVALID_PARAM     VS_ERR_CODE(E_MOD_ID_MIPI_RX, E_ERR_INVALID_PARAM)
#define VS_ERR_MIPI_RX_INVALID_DEVID     VS_ERR_CODE(E_MOD_ID_MIPI_RX, E_ERR_INVALID_DEVID)
#define VS_ERR_MIPI_RX_NULL_PTR          VS_ERR_CODE(E_MOD_ID_MIPI_RX, E_ERR_NULL_PTR)
#define VS_ERR_MIPI_RX_UNEXIST           VS_ERR_CODE(E_MOD_ID_MIPI_RX, E_ERR_UNEXIST)
#define VS_ERR_MIPI_RX_DEVICE_UNEXIST    VS_ERR_CODE(E_MOD_ID_MIPI_RX, E_ERR_DEVICE_UNEXIST)
#define VS_ERR_MIPI_RX_NOT_PERM          VS_ERR_CODE(E_MOD_ID_MIPI_RX, E_ERR_NOT_PERM)

#define VS_MIPI_RX_DEVID_MAX	4

typedef enum vs_lane_mode {
	E_LANE_MODE_16 = 0,   ///< 16lane
	E_LANE_MODE_8_8,      ///< 8lane + 8lane
	E_LANE_MODE_8,        ///< 8lane
	E_LANE_MODE_4_4,      ///< 4lane + 4lane
	E_LANE_MODE_4_2_2,    ///< 4lane + 2lane + 2lane
	E_LANE_MODE_2_2_2_2,  ///< 2lane + 2lane + 2lane + 2lane
	E_LANE_MODE_MAX
} vs_lane_mode_e;

typedef enum vs_mipi_lane_type {
	E_MIPI_LANE2 = 0,  ///< mipi 2 lane type
	E_MIPI_LANE4,      ///< mipi 4 lane type
	E_MIPI_LANE8,      ///< mipi 8 lane type
	E_MIPI_LANE4_2,    ///< mipi 4x2 lane type
	E_MIPI_LANE1,      ///< mipi 1 lane type
	E_MIPI_LANE_MAX
} vs_mipi_lane_type_e;

typedef struct vs_mipi_attr {
	vs_mipi_lane_type_e lane_type;  ///< mipi lane type
	vs_crop_s crop_info;                ///< device crop info
} vs_mipi_attr_s;

typedef enum vs_slvds_lane_type {
	E_SLVDS_LANE2 = 0,   ///< lvds 2 lane type
	E_SLVDS_LANE4,       ///< lvds 4 lane type
	E_SLVDS_LANE8,       ///< lvds 8 lane type
	E_SLVDS_LANE16,      ///< lvds 16 lane type
	E_SLVDS_LANE_MAX
} vs_slvds_lane_type_e;

typedef enum vs_slvds_endian_type {
	E_SLVDS_ENDIAN_BIG = 0,   ///<lvds big endian
	E_SLVDS_ENDIAN_LITTLE,    ///<lvds little endian
	E_SLVDS_ENDIAN_MAX
} vs_slvds_endian_type_e;

typedef struct vs_slvds_attr {
	vs_slvds_lane_type_e lane_type;     ///< lvds lane type
	vs_slvds_endian_type_e endian_type; ///< endian type
	vs_uint32_t sync_code;                  ///< lvds sync code
	vs_crop_s crop_info;                    ///< device crop info
} vs_slvds_attr_s;

typedef enum vs_mipi_input_mode {
	E_MODE_MIPI = 0,
	E_MODE_SLVDS,
	E_MODE_BT1120,
	E_MODE_BT656,
	E_MODE_DVP,
	E_MODE_INFRARED,
	E_MODE_MAX
} vs_mipi_input_mode_e;

typedef struct vs_mipi_rx_dev_attr {
	vs_mipi_input_mode_e mode;          ///< mipi or sublvds
	union {
		vs_slvds_attr_s slvds_attr; ///< sublvds attribute
		vs_mipi_attr_s mipi_attr;   ///< mipi attribute
	};
} vs_mipi_rx_dev_attr_s;

typedef struct vs_mipi_rx_error_stats {
	vs_uint64_t phy_errors;
	vs_uint64_t pkt_errors;
	vs_uint64_t bndry_frame_errors;
	vs_uint64_t seq_frame_errors;
	vs_uint64_t crc_frame_errors;
	vs_uint64_t pld_crc_errors;
	vs_uint64_t data_id_errors;
} vs_mipi_rx_error_stats_s;

#ifdef __cplusplus
}
#endif

#endif
