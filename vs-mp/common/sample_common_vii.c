/**
* @file sample_common_vii.c
* @brief implement the common functions of vii example.
* @details
* @author Visinex Software Group
* @date 2022-05-20
* @version v0.01
* @Copyright (C) 2022, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <dlfcn.h>
#include <pthread.h>

#include "sample_common.h"
#include "vs_mal_vii.h"
#include "vs_mal_img_sensor.h"
#include "vs_mal_awb.h"
#include "vs_mal_ae.h"

typedef struct sample_sensor_cfg {
    const char *sensor_name;
    sample_sensor_type_e sensor_type;
    const char *sensor_type_name;
    vs_lane_mode_e lane_mode;
    vs_pixel_format_e pixel_format;
    const vs_mipi_rx_dev_attr_s *p_mipi_attr;
    const vs_isp_comm_attr_s *p_isp_attr;
} sample_sensor_cfg_s;


sample_sensor_type_e g_sensor_type[VII_MAX_ROUTE_NUM] =
                    {SENSOR0_TYPE, SENSOR1_TYPE, SENSOR2_TYPE, SENSOR3_TYPE};
vs_int8_t g_bus_id[VII_MAX_ROUTE_NUM] = {1, 1, 1, 1};
vs_bool_t g_nr_3d = VS_TRUE;

#ifndef VS_NO_ISP
static pthread_t s_isp_run_thread[VII_MAX_PIPE_NUM];

static void *s_sensor_lib_handle[VII_MAX_PIPE_NUM]= {VS_NULL};
static void *s_aelib_handle[VII_MAX_PIPE_NUM] = {VS_NULL};
static void *s_awblib_handle[VII_MAX_PIPE_NUM]= {VS_NULL};
#endif

#define ALIGN_UP(x, a)           ( ( ((x) + ((a) - 1) ) / a ) * a )

/*******************************************************
                    mipi attr
*******************************************************/
static vs_mipi_rx_dev_attr_s s_os08a10_mipi_4lane_10bit_8m_linear_attr = {
    .mode = E_MODE_MIPI,
    .mipi_attr.lane_type = E_MIPI_LANE4,
    .mipi_attr.crop_info = {VS_TRUE, {0, 0, 3840, 2160}},
};

static vs_mipi_rx_dev_attr_s s_os08a10_mipi_4lane_10bit_2m_linear_attr = {
    .mode = E_MODE_MIPI,
    .mipi_attr.lane_type = E_MIPI_LANE4,
    .mipi_attr.crop_info = {VS_TRUE, {0, 0, 1920, 1080}},
};

static vs_mipi_rx_dev_attr_s s_os04a10_mipi_4lane_10bit_4m_linear_attr = {
    .mode = E_MODE_MIPI,
    .mipi_attr.lane_type = E_MIPI_LANE4,
    .mipi_attr.crop_info = {VS_TRUE, {0, 0, 2560, 1440}},
};

static vs_mipi_rx_dev_attr_s s_os04a10_mipi_4lane_12bit_4mfull_linear_attr = {
    .mode = E_MODE_MIPI,
    .mipi_attr.lane_type = E_MIPI_LANE4,
    .mipi_attr.crop_info = {VS_TRUE, {0, 0, 2688, 1520}},
};

static vs_mipi_rx_dev_attr_s s_ox03c10_mipi_4lane_12bit_2m_wdr4t1_attr = {
    .mode = E_MODE_MIPI,
    .mipi_attr.lane_type = E_MIPI_LANE4,
    .mipi_attr.crop_info = {VS_TRUE, {0, 2, 1920, 1280}},
};

static vs_mipi_rx_dev_attr_s s_imx347_mipi_4lane_12bit_4mfull_linear_attr = {
    .mode = E_MODE_MIPI,
    .mipi_attr.lane_type = E_MIPI_LANE4,
    .mipi_attr.crop_info = {VS_TRUE, {0, 30, 2688, 1520}},
};

static vs_mipi_rx_dev_attr_s s_imx347_mipi_4lane_12bit_4m_wdr2t1_attr = {
    .mode = E_MODE_MIPI,
    .mipi_attr.lane_type = E_MIPI_LANE4,
    .mipi_attr.crop_info = {VS_TRUE, {0, 20, 2560, 1440}},
};

static vs_mipi_rx_dev_attr_s s_imx347_mipi_4lane_12bit_4mfull_wdr2t1_attr = {
    .mode = E_MODE_MIPI,
    .mipi_attr.lane_type = E_MIPI_LANE4,
    .mipi_attr.crop_info = {VS_TRUE, {0, 20, 2688, 1520}},
};

static vs_mipi_rx_dev_attr_s s_imx334_mipi_4lane_12bit_8m_linear_attr = {
    .mode = E_MODE_MIPI,
    .mipi_attr.lane_type = E_MIPI_LANE4,
    .mipi_attr.crop_info = {VS_TRUE, {0, 32, 3840, 2160}},
};

static vs_mipi_rx_dev_attr_s s_os04a10_mipi_4lane_10bit_2m_linear_attr = {
    .mode = E_MODE_MIPI,
    .mipi_attr.lane_type = E_MIPI_LANE4,
    .mipi_attr.crop_info = {VS_TRUE, {0, 0, 1920, 1080}},
};

static vs_mipi_rx_dev_attr_s s_os04c10_mipi_4lane_12bit_4mfull_linear_attr = {
    .mode = E_MODE_MIPI,
    .mipi_attr.lane_type = E_MIPI_LANE4,
    .mipi_attr.crop_info = {VS_TRUE, {0, 0, 2688, 1520}},
};

static vs_mipi_rx_dev_attr_s s_ov9734_mipi_1lane_10bit_1m_linear_attr = {
    .mode = E_MODE_MIPI,
    .mipi_attr.lane_type = E_MIPI_LANE1,
    .mipi_attr.crop_info = {VS_TRUE, {0, 0, 1280, 720}},
};

static vs_mipi_rx_dev_attr_s s_imx305_slvds_4lane_12bit_8m_linear_attr = {
    .mode = E_MODE_SLVDS,
    .slvds_attr.lane_type = E_SLVDS_LANE4,
    .slvds_attr.endian_type = E_SLVDS_ENDIAN_BIG,
    .slvds_attr.sync_code = 0xB6AB9D80,
    .slvds_attr.crop_info = {VS_TRUE, {0, 19, 3840, 2160}},
};

static vs_mipi_rx_dev_attr_s s_imx305_slvds_8lane_12bit_9m_linear_attr = {
    .mode = E_MODE_SLVDS,
    .slvds_attr.lane_type = E_SLVDS_LANE8,
    .slvds_attr.endian_type = E_SLVDS_ENDIAN_BIG,
    .slvds_attr.sync_code = 0xB6AB9D80,
    .slvds_attr.crop_info = {VS_TRUE, {0, 19, 4096, 2160}},
};

static vs_mipi_rx_dev_attr_s s_imx305_slvds_16lane_12bit_8m_linear_attr = {
    .mode = E_MODE_SLVDS,
    .slvds_attr.lane_type = E_SLVDS_LANE16,
    .slvds_attr.endian_type = E_SLVDS_ENDIAN_BIG,
    .slvds_attr.sync_code = 0xB6AB9D80,
    .slvds_attr.crop_info = {VS_TRUE, {0, 19, 3840, 2160}},
};

static vs_mipi_rx_dev_attr_s s_imx305_slvds_16lane_12bit_9m_linear_attr = {
    .mode = E_MODE_SLVDS,
    .slvds_attr.lane_type = E_SLVDS_LANE16,
    .slvds_attr.endian_type = E_SLVDS_ENDIAN_BIG,
    .slvds_attr.sync_code = 0xB6AB9D80,
    .slvds_attr.crop_info = {VS_TRUE, {0, 19, 4096, 2160}},
};

static vs_mipi_rx_dev_attr_s s_sc450ai_mipi_4lane_10bit_4m_linear_attr = {
    .mode = E_MODE_MIPI,
    .mipi_attr.lane_type = E_MIPI_LANE4,
    .mipi_attr.crop_info = {VS_TRUE, {0, 0, 2560, 1440}},
};

static vs_mipi_rx_dev_attr_s s_sc450ai_mipi_4lane_10bit_2m_linear_attr = {
    .mode = E_MODE_MIPI,
    .mipi_attr.lane_type = E_MIPI_LANE4,
    .mipi_attr.crop_info = {VS_TRUE, {0, 0, 1920, 1080}},
};

static vs_mipi_rx_dev_attr_s s_sc910gs_mipi_4lane_12bit_8m_linear_attr = {
    .mode = E_MODE_MIPI,
    .mipi_attr.lane_type = E_MIPI_LANE4,
    .mipi_attr.crop_info = {VS_TRUE, {0, 0, 3840, 2160}},
};

static vs_mipi_rx_dev_attr_s s_sc200ai_mipi_2lane_10bit_2m_linear_attr = {
    .mode = E_MODE_MIPI,
    .mipi_attr.lane_type = E_MIPI_LANE2,
    .mipi_attr.crop_info = {VS_TRUE, {0, 0, 1920, 1080}},
};

static vs_mipi_rx_dev_attr_s s_sc535hgs_mipi_4lane_12bit_5m_linear_attr = {
    .mode = E_MODE_MIPI,
    .mipi_attr.lane_type = E_MIPI_LANE4,
    .mipi_attr.crop_info = {VS_TRUE, {0, 0, 2432, 2048}},
};

static vs_mipi_rx_dev_attr_s s_sc430ai_mipi_4lane_10bit_4mfull_linear_attr = {
    .mode = E_MODE_MIPI,
    .mipi_attr.lane_type = E_MIPI_LANE4,
    .mipi_attr.crop_info = {VS_TRUE, {0, 0, 2688, 1520}},
};

static vs_mipi_rx_dev_attr_s s_sc430ai_mipi_4lane_10bit_4m_linear_attr = {
    .mode = E_MODE_MIPI,
    .mipi_attr.lane_type = E_MIPI_LANE4,
    .mipi_attr.crop_info = {VS_TRUE, {0, 0, 2560, 1440}},
};

static vs_mipi_rx_dev_attr_s s_sc430ai_mipi_4lane_10bit_2m_linear_attr = {
    .mode = E_MODE_MIPI,
    .mipi_attr.lane_type = E_MIPI_LANE4,
    .mipi_attr.crop_info = {VS_TRUE, {0, 0, 1920, 1080}},
};

static vs_mipi_rx_dev_attr_s s_imx485_mipi_4lane_12bit_8m_linear_attr = {
    .mode = E_MODE_MIPI,
    .mipi_attr.lane_type = E_MIPI_LANE4,
    .mipi_attr.crop_info = {VS_TRUE, {0, 34, 3840, 2160}},
};

static vs_mipi_rx_dev_attr_s s_imx485_mipi_4lane_2ch_12bit_8m_linear_attr = {
    .mode = E_MODE_MIPI,
    .mipi_attr.lane_type = E_MIPI_LANE4_2,
    .mipi_attr.crop_info = {VS_TRUE, {0, 34, 3840, 2160}},
};

static vs_mipi_rx_dev_attr_s s_imx485_mipi_8lane_12bit_8m_linear_attr = {
    .mode = E_MODE_MIPI,
    .mipi_attr.lane_type = E_MIPI_LANE8,
    .mipi_attr.crop_info = {VS_TRUE, {0, 34, 3840, 2160}},
};

static vs_mipi_rx_dev_attr_s s_imx385_mipi_2lane_12bit_2m_linear_attr = {
    .mode = E_MODE_MIPI,
    .mipi_attr.lane_type = E_MIPI_LANE2,
    .mipi_attr.crop_info = {VS_TRUE, {32, 24, 1920, 1080}},
};

static vs_mipi_rx_dev_attr_s s_sc450ai_mipi_4lane_10bit_4mfull_linear_attr = {
    .mode = E_MODE_MIPI,
    .mipi_attr.lane_type = E_MIPI_LANE4,
    .mipi_attr.crop_info = {VS_TRUE, {0, 0, 2688, 1520}},
};

static vs_mipi_rx_dev_attr_s s_imx415_mipi_4lane_12bit_8m_linear_attr = {
    .mode = E_MODE_MIPI,
    .mipi_attr.lane_type = E_MIPI_LANE4,
    .mipi_attr.crop_info = {VS_TRUE, {0, 37, 3840, 2160}},
};

static vs_mipi_rx_dev_attr_s s_imx415_mipi_4lane_12bit_8m_wdr_attr = {
    .mode = E_MODE_MIPI,
    .mipi_attr.lane_type = E_MIPI_LANE4,
    .mipi_attr.crop_info = {VS_TRUE, {0, 38, 3840, 2160}},
};

static vs_mipi_rx_dev_attr_s s_gmax3809_slvds_4lane_12bit_9m_linear_attr = {
    .mode = E_MODE_SLVDS,
    .slvds_attr.lane_type = E_SLVDS_LANE4,
    .slvds_attr.endian_type = E_SLVDS_ENDIAN_BIG,
    .slvds_attr.sync_code = 0x809DB6AB,
    .slvds_attr.crop_info = {VS_TRUE, {128, 1, 4096, 2176}},
};

static vs_mipi_rx_dev_attr_s s_gmax3809_slvds_8lane_12bit_9m_linear_attr = {
    .mode = E_MODE_SLVDS,
    .slvds_attr.lane_type = E_SLVDS_LANE8,
    .slvds_attr.endian_type = E_SLVDS_ENDIAN_BIG,
    .slvds_attr.sync_code = 0x809DB6AB,
    .slvds_attr.crop_info = {VS_TRUE, {128, 1, 4096, 2176}},
};

static vs_mipi_rx_dev_attr_s s_gc4653_mipi_2lane_10bit_4m_linear_attr = {
    .mode = E_MODE_MIPI,
    .mipi_attr.lane_type = E_MIPI_LANE2,
    .mipi_attr.crop_info = {VS_TRUE, {0, 0, 2560, 1440}},
};

static vs_mipi_rx_dev_attr_s s_gc4023_mipi_2lane_10bit_4m_linear_attr = {
    .mode = E_MODE_MIPI,
    .mipi_attr.lane_type = E_MIPI_LANE2,
    .mipi_attr.crop_info = {VS_TRUE, {0, 0, 2560, 1440}},
};

static vs_mipi_rx_dev_attr_s s_imx686_mipi_4lane_14bit_8m_linear_attr = {
    .mode = E_MODE_MIPI,
    .mipi_attr.lane_type = E_MIPI_LANE4,
    .mipi_attr.crop_info = {VS_TRUE, {0, 0, 3840, 2160}},
};

static vs_mipi_rx_dev_attr_s s_imx586_mipi_4lane_10bit_8m_linear_attr = {
    .mode = E_MODE_MIPI,
    .mipi_attr.lane_type = E_MIPI_LANE4,
    .mipi_attr.crop_info = {VS_TRUE, {0, 0, 3840, 2160}},
};

static vs_mipi_rx_dev_attr_s s_imx586_mipi_4lane_10bit_2m_linear_attr = {
    .mode = E_MODE_MIPI,
    .mipi_attr.lane_type = E_MIPI_LANE4,
    .mipi_attr.crop_info = {VS_TRUE, {0, 0, 1920, 1080}},
};

static vs_mipi_rx_dev_attr_s s_imx586_mipi_4lane_10bit_12m_linear_attr = {
    .mode = E_MODE_MIPI,
    .mipi_attr.lane_type = E_MIPI_LANE4,
    .mipi_attr.crop_info = {VS_TRUE, {0, 0, 4000, 3000}},
};

static vs_mipi_rx_dev_attr_s s_imx586_mipi_4lane_10bit_48m_linear_attr = {
    .mode = E_MODE_MIPI,
    .mipi_attr.lane_type = E_MIPI_LANE4,
    .mipi_attr.crop_info = {VS_TRUE, {0, 0, 8000, 6000}},
};

static vs_mipi_rx_dev_attr_s s_bg0651_mipi_4lane_12bit_linear_attr = {
    .mode = E_MODE_MIPI,
    .mipi_attr.lane_type = E_MIPI_LANE4,
    .mipi_attr.crop_info = {VS_TRUE, {0, 0, 800, 600}},
};
/*******************************************************
                    isp attr
*******************************************************/
static vs_isp_comm_attr_s s_os08a10_8m_30fps_10bit_linear_isp_attr = {
    .sensor_size = {3840, 2160},
    .frame_rate = 29.9,
    .bayer_format = E_ISP_BAYER_FORMAT_BGGR,
    .wdr_mode = E_WDR_MODE_NONE,
    .sensor_mode = 8,
};

static vs_isp_comm_attr_s s_os08a10_2m_60fps_10bit_linear_isp_attr = {
    .sensor_size = {1920, 1080},
    .frame_rate = 59.9,
    .bayer_format = E_ISP_BAYER_FORMAT_BGGR,
    .wdr_mode = E_WDR_MODE_NONE,
    .sensor_mode = 0,
};

static vs_isp_comm_attr_s s_os08a10_8m_60fps_10bit_linear_isp_attr = {
    .sensor_size = {3840, 2160},
    .frame_rate = 59.9,
    .bayer_format = E_ISP_BAYER_FORMAT_BGGR,
    .wdr_mode = E_WDR_MODE_NONE,
    .sensor_mode = 9,
};

static vs_isp_comm_attr_s s_os08a20_8m_60fps_10bit_linear_isp_attr = {
    .sensor_size = {3840, 2160},
    .frame_rate = 59.9,
    .bayer_format = E_ISP_BAYER_FORMAT_BGGR,
    .wdr_mode = E_WDR_MODE_NONE,
    .sensor_mode = 0,
};

static vs_isp_comm_attr_s s_os08a20_8m_50fps_10bit_linear_isp_attr = {
    .sensor_size = {3840, 2160},
    .frame_rate = 49.9,
    .bayer_format = E_ISP_BAYER_FORMAT_BGGR,
    .wdr_mode = E_WDR_MODE_NONE,
    .sensor_mode = 3,
};

static vs_isp_comm_attr_s s_os08a20_8m_30fps_10bit_linear_isp_attr = {
    .sensor_size = {3840, 2160},
    .frame_rate = 29.9,
    .bayer_format = E_ISP_BAYER_FORMAT_BGGR,
    .wdr_mode = E_WDR_MODE_NONE,
    .sensor_mode = 1,
};

static vs_isp_comm_attr_s s_os08a20_2m_120fps_10bit_linear_isp_attr = {
    .sensor_size = {1920, 1080},
    .frame_rate = 119.9,
    .bayer_format = E_ISP_BAYER_FORMAT_BGGR,
    .wdr_mode = E_WDR_MODE_NONE,
    .sensor_mode = 2,
};

static vs_isp_comm_attr_s s_imx334_8m_30fps_12bit_linear_isp_attr = {
    .sensor_size = {3840, 2160},
    .frame_rate = 29.9,
    .bayer_format = E_ISP_BAYER_FORMAT_GBRG,
    .wdr_mode = E_WDR_MODE_NONE,
    .sensor_mode = 0,
};

static vs_isp_comm_attr_s s_imx334_8m_45fps_12bit_linear_isp_attr = {
    .sensor_size = {3840, 2160},
    .frame_rate = 44.9,
    .bayer_format = E_ISP_BAYER_FORMAT_GBRG,
    .wdr_mode = E_WDR_MODE_NONE,
    .sensor_mode = 4,
};

static vs_isp_comm_attr_s s_imx334_8m_60fps_12bit_linear_isp_attr = {
    .sensor_size = {3840, 2160},
    .frame_rate = 59.9,
    .bayer_format = E_ISP_BAYER_FORMAT_GBRG,
    .wdr_mode = E_WDR_MODE_NONE,
    .sensor_mode = 3,
};

static vs_isp_comm_attr_s s_os04a10_4m_30fps_10bit_linear_isp_attr = {
    .sensor_size = {2560, 1440},
    .frame_rate = 29.9,
    .bayer_format = E_ISP_BAYER_FORMAT_BGGR,
    .wdr_mode = E_WDR_MODE_NONE,
    .sensor_mode = 1,
};

static vs_isp_comm_attr_s s_os04c10_4mfull_30fps_12bit_linear_isp_attr = {
    .sensor_size = {2688, 1520},
    .frame_rate = 29.9,
    .bayer_format = E_ISP_BAYER_FORMAT_BGGR,
    .wdr_mode = E_WDR_MODE_NONE,
    .sensor_mode = 0,
};

static vs_isp_comm_attr_s s_os04a10_4mfull_25fps_12bit_linear_isp_attr = {
    .sensor_size = {2688, 1520},
    .frame_rate = 24.9,
    .bayer_format = E_ISP_BAYER_FORMAT_BGGR,
    .wdr_mode = E_WDR_MODE_NONE,
    .sensor_mode = 2,
};

static vs_isp_comm_attr_s s_os04a10_4mfull_25fps_10bit_wdr2t1_isp_attr = {
    .sensor_size = {2688, 1520},
    .frame_rate = 24.9,
    .bayer_format = E_ISP_BAYER_FORMAT_BGGR,
    .wdr_mode = E_WDR_MODE_2To1_LINE,
    .sensor_mode = 3,
};

static vs_isp_comm_attr_s s_ox03c10_2m_60fps_12bit_wdr4t1_isp_attr = {
    .sensor_size = {1920, 1280},
    .frame_rate = 59.9,
    .bayer_format = E_ISP_BAYER_FORMAT_BGGR,
    .wdr_mode = E_WDR_MODE_BUILT_IN,
    .sensor_mode = 0,
};

static vs_isp_comm_attr_s s_os04a10_2m_30fps_10bit_linear_isp_attr = {
    .sensor_size = {1920, 1080},
    .frame_rate = 29.9,
    .bayer_format = E_ISP_BAYER_FORMAT_BGGR,
    .wdr_mode = E_WDR_MODE_NONE,
    .sensor_mode = 0,
};

static vs_isp_comm_attr_s s_ov9734_1m_30fps_10bit_linear_isp_attr = {
    .sensor_size = {1280, 720},
    .frame_rate = 29.9,
    .bayer_format = E_ISP_BAYER_FORMAT_BGGR,
    .wdr_mode = E_WDR_MODE_NONE,
    .sensor_mode = 0,
};

static vs_isp_comm_attr_s s_imx305_8m_20fps_12bit_linear_isp_attr = {
    .sensor_size = {3840, 2160},
    .frame_rate = 20.0,
    .bayer_format = E_ISP_BAYER_FORMAT_RGGB,
    .wdr_mode = E_WDR_MODE_NONE,
    .sensor_mode = 0,
};

static vs_isp_comm_attr_s s_imx305_8l_9m_25fps_12bit_linear_isp_attr = {
    .sensor_size = {4096, 2160},
    .frame_rate = 25.0,
    .bayer_format = E_ISP_BAYER_FORMAT_RGGB,
    .wdr_mode = E_WDR_MODE_NONE,
    .sensor_mode = 1,
};

static vs_isp_comm_attr_s s_imx305_8l_9m_30fps_12bit_linear_isp_attr = {
    .sensor_size = {4096, 2160},
    .frame_rate = 30.0,
    .bayer_format = E_ISP_BAYER_FORMAT_RGGB,
    .wdr_mode = E_WDR_MODE_NONE,
    .sensor_mode = 4,
};

static vs_isp_comm_attr_s s_imx305_16l_8m_25fps_12bit_linear_isp_attr = {
    .sensor_size = {3840, 2160},
    .frame_rate = 25.0,
    .bayer_format = E_ISP_BAYER_FORMAT_RGGB,
    .wdr_mode = E_WDR_MODE_NONE,
    .sensor_mode = 2,
};

static vs_isp_comm_attr_s s_imx305_16l_9m_50fps_12bit_linear_isp_attr = {
    .sensor_size = {4096, 2160},
    .frame_rate = 50.0,
    .bayer_format = E_ISP_BAYER_FORMAT_RGGB,
    .wdr_mode = E_WDR_MODE_NONE,
    .sensor_mode = 3,
};

static vs_isp_comm_attr_s s_imx347_4mfull_25fps_12bit_linear_isp_attr = {
    .sensor_size = {2688, 1520},
    .frame_rate = 24.9,
    .bayer_format = E_ISP_BAYER_FORMAT_GBRG,
    .wdr_mode = E_WDR_MODE_NONE,
    .sensor_mode = 0,
};

static vs_isp_comm_attr_s s_imx347_4mfull_30fps_12bit_linear_isp_attr = {
    .sensor_size = {2688, 1520},
    .frame_rate = 29.9,
    .bayer_format = E_ISP_BAYER_FORMAT_GBRG,
    .wdr_mode = E_WDR_MODE_NONE,
    .sensor_mode = 1,
};

static vs_isp_comm_attr_s s_imx347_4m_25fps_12bit_wdr2t1_isp_attr = {
    .sensor_size = {2560, 1440},
    .frame_rate = 24.9,
    .bayer_format = E_ISP_BAYER_FORMAT_RGGB,
    .wdr_mode = E_WDR_MODE_2To1_LINE,
    .sensor_mode = 2,
};

static vs_isp_comm_attr_s s_imx347_4m_30fps_12bit_wdr2t1_isp_attr = {
    .sensor_size = {2560, 1440},
    .frame_rate = 29.9,
    .bayer_format = E_ISP_BAYER_FORMAT_RGGB,
    .wdr_mode = E_WDR_MODE_2To1_LINE,
    .sensor_mode = 3,
};

static vs_isp_comm_attr_s s_imx347_4mfull_25fps_12bit_wdr2t1_isp_attr = {
    .sensor_size = {2688, 1520},
    .frame_rate = 24.9,
    .bayer_format = E_ISP_BAYER_FORMAT_RGGB,
    .wdr_mode = E_WDR_MODE_2To1_LINE,
    .sensor_mode = 4,
};

static vs_isp_comm_attr_s s_sc450ai_4m_30fps_10bit_linear_isp_attr = {
    .sensor_size = {2560, 1440},
    .frame_rate = 29.9,
    .bayer_format = E_ISP_BAYER_FORMAT_BGGR,
    .wdr_mode = E_WDR_MODE_NONE,
    .sensor_mode = 0,
};

static vs_isp_comm_attr_s s_sc450ai_4m_25fps_10bit_linear_isp_attr = {
    .sensor_size = {2560, 1440},
    .frame_rate = 24.9,
    .bayer_format = E_ISP_BAYER_FORMAT_BGGR,
    .wdr_mode = E_WDR_MODE_NONE,
    .sensor_mode = 3,
};

static vs_isp_comm_attr_s s_sc450ai_4m_30fps_10bit_wdr2t1_isp_attr = {
    .sensor_size = {2560, 1440},
    .frame_rate = 29.9,
    .bayer_format = E_ISP_BAYER_FORMAT_BGGR,
    .wdr_mode = E_WDR_MODE_2To1_LINE,
    .sensor_mode = 1,
};

static vs_isp_comm_attr_s s_sc450ai_4m_25fps_10bit_wdr2t1_isp_attr = {
    .sensor_size = {2560, 1440},
    .frame_rate = 24.9,
    .bayer_format = E_ISP_BAYER_FORMAT_BGGR,
    .wdr_mode = E_WDR_MODE_2To1_LINE,
    .sensor_mode = 4,
};

static vs_isp_comm_attr_s s_sc450ai_2m_30fps_10bit_wdr3t1_isp_attr = {
    .sensor_size = {1920, 1080},
    .frame_rate = 29.9,
    .bayer_format = E_ISP_BAYER_FORMAT_BGGR,
    .wdr_mode = E_WDR_MODE_3To1_LINE,
    .sensor_mode = 2,
};

static vs_isp_comm_attr_s s_sc910gs_8m_25fps_12bit_linear_isp_attr = {
    .sensor_size = {3840, 2160},
    .frame_rate = 24.9,
    .bayer_format = E_ISP_BAYER_FORMAT_BGGR,
    .wdr_mode = E_WDR_MODE_NONE,
    .sensor_mode = 0,
};

static vs_isp_comm_attr_s s_sc200ai_2m_30fps_10bit_linear_isp_attr = {
    .sensor_size = {1920, 1080},
    .frame_rate = 29.9,
    .bayer_format = E_ISP_BAYER_FORMAT_BGGR,
    .wdr_mode = E_WDR_MODE_NONE,
    .sensor_mode = 0,
};

static vs_isp_comm_attr_s s_sc200ai_2m_30fps_10bit_wdr2t1_isp_attr = {
    .sensor_size = {1920, 1080},
    .frame_rate = 29.9,
    .bayer_format = E_ISP_BAYER_FORMAT_BGGR,
    .wdr_mode = E_WDR_MODE_2To1_LINE,
    .sensor_mode = 1,
};

static vs_isp_comm_attr_s s_sc200ai_2m_25fps_10bit_linear_isp_attr = {
    .sensor_size = {1920, 1080},
    .frame_rate = 24.9,
    .bayer_format = E_ISP_BAYER_FORMAT_BGGR,
    .wdr_mode = E_WDR_MODE_NONE,
    .sensor_mode = 0,
};

static vs_isp_comm_attr_s s_sc200ai_2m_25fps_10bit_wdr2t1_isp_attr = {
    .sensor_size = {1920, 1080},
    .frame_rate = 24.9,
    .bayer_format = E_ISP_BAYER_FORMAT_BGGR,
    .wdr_mode = E_WDR_MODE_2To1_LINE,
    .sensor_mode = 1,
};

static vs_isp_comm_attr_s s_sc535hgs_5m_60fps_12bit_linear_isp_attr = {
    .sensor_size = {2432, 2048},
    .frame_rate = 59.9,
    .bayer_format = E_ISP_BAYER_FORMAT_BGGR,
    .wdr_mode = E_WDR_MODE_NONE,
    .sensor_mode = 0,
};

static vs_isp_comm_attr_s s_sc430ai_4m_30fps_10bit_linear_isp_attr = {
    .sensor_size = {2560, 1440},
    .frame_rate = 29.9,
    .bayer_format = E_ISP_BAYER_FORMAT_BGGR,
    .wdr_mode = E_WDR_MODE_NONE,
    .sensor_mode = 0,
};

static vs_isp_comm_attr_s s_sc430ai_4mfull_30fps_10bit_linear_isp_attr = {
    .sensor_size = {2688, 1520},
    .frame_rate = 29.9,
    .bayer_format = E_ISP_BAYER_FORMAT_BGGR,
    .wdr_mode = E_WDR_MODE_NONE,
    .sensor_mode = 1,
};

static vs_isp_comm_attr_s s_sc430ai_2m_30fps_10bit_linear_isp_attr = {
    .sensor_size = {1920, 1080},
    .frame_rate = 29.9,
    .bayer_format = E_ISP_BAYER_FORMAT_BGGR,
    .wdr_mode = E_WDR_MODE_NONE,
    .sensor_mode = 2,
};

static vs_isp_comm_attr_s s_sc430ai_4mfull_60fps_10bit_linear_isp_attr = {
    .sensor_size = {2688, 1520},
    .frame_rate = 59.9,
    .bayer_format = E_ISP_BAYER_FORMAT_BGGR,
    .wdr_mode = E_WDR_MODE_NONE,
    .sensor_mode = 3,
};

static vs_isp_comm_attr_s s_sc430ai_4mfull_30fps_10bit_wdr2t1_isp_attr = {
    .sensor_size = {2688, 1520},
    .frame_rate = 29.9,
    .bayer_format = E_ISP_BAYER_FORMAT_BGGR,
    .wdr_mode = E_WDR_MODE_2To1_LINE,
    .sensor_mode = 4,
};

static vs_isp_comm_attr_s s_sc430ai_4m_30fps_10bit_wdr2t1_isp_attr = {
    .sensor_size = {2560, 1440},
    .frame_rate = 29.9,
    .bayer_format = E_ISP_BAYER_FORMAT_BGGR,
    .wdr_mode = E_WDR_MODE_2To1_LINE,
    .sensor_mode = 5,
};

static vs_isp_comm_attr_s s_sc430ai_2m_30fps_10bit_wdr2t1_isp_attr = {
    .sensor_size = {1920, 1080},
    .frame_rate = 29.9,
    .bayer_format = E_ISP_BAYER_FORMAT_BGGR,
    .wdr_mode = E_WDR_MODE_2To1_LINE,
    .sensor_mode = 6,
};

static vs_isp_comm_attr_s s_imx485_8m_15fps_12bit_linear_isp_attr = {
    .sensor_size = {3840, 2160},
    .frame_rate = 14.9,
    .bayer_format = E_ISP_BAYER_FORMAT_GBRG,
    .wdr_mode = E_WDR_MODE_NONE,
    .sensor_mode = 0,
};

static vs_isp_comm_attr_s s_imx485_4l2c_8m_30fps_12bit_linear_isp_attr = {
    .sensor_size = {3840, 2160},
    .frame_rate = 29.9,
    .bayer_format = E_ISP_BAYER_FORMAT_GBRG,
    .wdr_mode = E_WDR_MODE_NONE,
    .sensor_mode = 3,
};

static vs_isp_comm_attr_s s_imx485_8l_8m_30fps_12bit_linear_isp_attr = {
    .sensor_size = {3840, 2160},
    .frame_rate = 29.9,
    .bayer_format = E_ISP_BAYER_FORMAT_GBRG,
    .wdr_mode = E_WDR_MODE_NONE,
    .sensor_mode = 4,
};

static vs_isp_comm_attr_s s_os08a10_8m_30fps_10bit_wdr2to1_isp_attr = {
    .sensor_size = {3840, 2160},
    .frame_rate = 29.9,
    .bayer_format = E_ISP_BAYER_FORMAT_BGGR,
    .wdr_mode = E_WDR_MODE_2To1_LINE,
    .sensor_mode = 10,
};

static vs_isp_comm_attr_s s_imx385_2l_2m_25fps_12bit_linear_isp_attr = {
    .sensor_size = {1920, 1080},
    .frame_rate = 24.9,
    .bayer_format = E_ISP_BAYER_FORMAT_GBRG,
    .wdr_mode = E_WDR_MODE_NONE,
    .sensor_mode = 0,
};

static vs_isp_comm_attr_s s_imx415_8m_60fps_12bit_linear_isp_attr = {
    .sensor_size = {3840, 2160},
    .frame_rate = 59.9,
    .bayer_format = E_ISP_BAYER_FORMAT_GBRG,
    .wdr_mode = E_WDR_MODE_NONE,
    .sensor_mode = 0,
};

static vs_isp_comm_attr_s s_imx415_8m_30fps_12bit_wdr2to1_isp_attr = {
    .sensor_size = {3840, 2160},
    .frame_rate = 29.9,
    .bayer_format = E_ISP_BAYER_FORMAT_GBRG,
    .wdr_mode = E_WDR_MODE_2To1_LINE,
    .sensor_mode = 1,
};

static vs_isp_comm_attr_s s_imx415_8m_25fps_12bit_linear_isp_attr = {
    .sensor_size = {3840, 2160},
    .frame_rate = 24.9,
    .bayer_format = E_ISP_BAYER_FORMAT_GBRG,
    .wdr_mode = E_WDR_MODE_NONE,
    .sensor_mode = 2,
};

static vs_isp_comm_attr_s s_imx415_8m_25fps_12bit_wdr2to1_isp_attr = {
    .sensor_size = {3840, 2160},
    .frame_rate = 24.9,
    .bayer_format = E_ISP_BAYER_FORMAT_GBRG,
    .wdr_mode = E_WDR_MODE_2To1_LINE,
    .sensor_mode = 3,
};

static vs_isp_comm_attr_s s_sc450ai_4mfull_25fps_10bit_linear_isp_attr = {
    .sensor_size = {2688, 1520},
    .frame_rate = 24.9,
    .bayer_format = E_ISP_BAYER_FORMAT_BGGR,
    .wdr_mode = E_WDR_MODE_NONE,
    .sensor_mode = 6,
};

static vs_isp_comm_attr_s s_sc450ai_4mfull_25fps_10bit_wdr2to1_isp_attr = {
    .sensor_size = {2688, 1520},
    .frame_rate = 24.9,
    .bayer_format = E_ISP_BAYER_FORMAT_BGGR,
    .wdr_mode = E_WDR_MODE_2To1_LINE,
    .sensor_mode = 7,
};

static vs_isp_comm_attr_s s_gmax3809_9m_25fps_12bit_linear_isp_attr = {
    .sensor_size = {4096, 2176},
    .frame_rate = 24.9,
    .bayer_format = E_ISP_BAYER_FORMAT_RGGB,
    .wdr_mode = E_WDR_MODE_NONE,
    .sensor_mode = 0,
};

static vs_isp_comm_attr_s s_gmax3809_9m_30fps_12bit_linear_isp_attr = {
    .sensor_size = {4096, 2176},
    .frame_rate = 29.9,
    .bayer_format = E_ISP_BAYER_FORMAT_RGGB,
    .wdr_mode = E_WDR_MODE_NONE,
    .sensor_mode = 1,
};

static vs_isp_comm_attr_s s_gc4653_4m_30fps_10bit_linear_isp_attr = {
    .sensor_size = {2560, 1440},
    .frame_rate = 29.9,
    .bayer_format = E_ISP_BAYER_FORMAT_GRBG,
    .wdr_mode = E_WDR_MODE_NONE,
    .sensor_mode = 0,
};

static vs_isp_comm_attr_s s_gc4023_4m_25fps_10bit_linear_isp_attr = {
    .sensor_size = {2560, 1440},
    .frame_rate = 24.9,
    .bayer_format = E_ISP_BAYER_FORMAT_RGGB,
    .wdr_mode = E_WDR_MODE_NONE,
    .sensor_mode = 0,
};

static vs_isp_comm_attr_s s_imx686_8m_30fps_14bit_linear_isp_attr = {
    .sensor_size = {3840, 2160},
    .frame_rate = 29.9,
    .bayer_format = E_ISP_BAYER_FORMAT_RGGB,
    .wdr_mode = E_WDR_MODE_NONE,
    .sensor_mode = 0,
};

static vs_isp_comm_attr_s s_imx686_8m_30fps_14bit_linear_isp_pd_attr = {
    .sensor_size = {3840, 2160},
    .frame_rate = 29.9,
    .bayer_format = E_ISP_BAYER_FORMAT_RGGB,
    .wdr_mode = E_WDR_MODE_NONE,
    .sensor_mode = 1,
};

static vs_isp_comm_attr_s s_imx686_8m_30fps_14bit_qhdr_isp_attr = {
    .sensor_size = {3840, 2160},
    .frame_rate = 29.9,
    .bayer_format = E_ISP_BAYER_FORMAT_RGGB,
    .wdr_mode = E_WDR_MODE_BUILT_IN,
    .sensor_mode = 2,
};

static vs_isp_comm_attr_s s_imx686_8m_50fps_10bit_linear_isp_attr = {
    .sensor_size = {3840, 2160},
    .frame_rate = 49.9,
    .bayer_format = E_ISP_BAYER_FORMAT_RGGB,
    .wdr_mode = E_WDR_MODE_NONE,
    .sensor_mode = 3,
};

static vs_isp_comm_attr_s s_imx586_8m_30fps_10bit_linear_isp_attr = {
    .sensor_size = {3840, 2160},
    .frame_rate = 29.9,
    .bayer_format = E_ISP_BAYER_FORMAT_RGGB,
    .wdr_mode = E_WDR_MODE_NONE,
    .sensor_mode = 0,
};

static vs_isp_comm_attr_s s_imx586_8m_30fps_10bit_qhdr_isp_attr = {
    .sensor_size = {3840, 2160},
    .frame_rate = 29.9,
    .bayer_format = E_ISP_BAYER_FORMAT_RGGB,
    .wdr_mode = E_WDR_MODE_BUILT_IN,
    .sensor_mode = 1,
};

static vs_isp_comm_attr_s s_imx586_2m_30fps_10bit_qhdr_isp_attr = {
    .sensor_size = {1920, 1080},
    .frame_rate = 29.9,
    .bayer_format = E_ISP_BAYER_FORMAT_RGGB,
    .wdr_mode = E_WDR_MODE_BUILT_IN,
    .sensor_mode = 2,
};

static vs_isp_comm_attr_s s_imx586_12m_30fps_10bit_linear_isp_attr = {
    .sensor_size = {4000, 3000},
    .frame_rate = 29.9,
    .bayer_format = E_ISP_BAYER_FORMAT_RGGB,
    .wdr_mode = E_WDR_MODE_NONE,
    .sensor_mode = 5,
};

static vs_isp_comm_attr_s s_imx586_48m_1fps_10bit_linear_isp_attr = {
    .sensor_size = {8000, 6000},
    .frame_rate = 1.0,
    .bayer_format = E_ISP_BAYER_FORMAT_RGGB,
    .wdr_mode = E_WDR_MODE_NONE,
    .sensor_mode = 6,
};

static vs_isp_comm_attr_s s_imx586_48m_3fps_10bit_linear_isp_attr = {
    .sensor_size = {8000, 6000},
    .frame_rate = 2.9,
    .bayer_format = E_ISP_BAYER_FORMAT_RGGB,
    .wdr_mode = E_WDR_MODE_NONE,
    .sensor_mode = 7,
};

static vs_isp_comm_attr_s s_imx586_2m_60fps_10bit_linear_isp_attr = {
    .sensor_size = {1920, 1080},
    .frame_rate = 59.9,
    .bayer_format = E_ISP_BAYER_FORMAT_RGGB,
    .wdr_mode = E_WDR_MODE_NONE,
    .sensor_mode = 8,
};

static vs_isp_comm_attr_s s_imx586_2m_120fps_10bit_linear_isp_attr = {
    .sensor_size = {1920, 1080},
    .frame_rate = 119.9,
    .bayer_format = E_ISP_BAYER_FORMAT_RGGB,
    .wdr_mode = E_WDR_MODE_NONE,
    .sensor_mode = 9,
};

static vs_isp_comm_attr_s s_bg0651_100fps_12bit_linear_isp_attr = {
    .sensor_size = {800, 600},
    .frame_rate = 100,
    .bayer_format = E_ISP_BAYER_FORMAT_RGGB,
    .wdr_mode = E_WDR_MODE_NONE,
    .sensor_mode = 0,
};
/*******************************************************
                   sensor cfg
*******************************************************/
static sample_sensor_cfg_s s_sensor_cfg[] = {
    // os08a10
    {"os08a10", OV_OS08A10_MIPI_8M_30FPS_10BIT, "OV_OS08A10_MIPI_8M_30FPS_10BIT",
        E_LANE_MODE_4_2_2,
        E_PIXEL_FORMAT_BAYER_10BPP,
        &s_os08a10_mipi_4lane_10bit_8m_linear_attr,
        &s_os08a10_8m_30fps_10bit_linear_isp_attr
    },
    {"os08a10", OV_OS08A10_MIPI_2M_60FPS_10BIT, "OV_OS08A10_MIPI_2M_60FPS_10BIT",
        E_LANE_MODE_4_2_2,
        E_PIXEL_FORMAT_BAYER_10BPP,
        &s_os08a10_mipi_4lane_10bit_2m_linear_attr,
        &s_os08a10_2m_60fps_10bit_linear_isp_attr
    },
    {"os08a10", OV_OS08A10_MIPI_8M_60FPS_10BIT, "OV_OS08A10_MIPI_8M_60FPS_10BIT",
        E_LANE_MODE_4_2_2,
        E_PIXEL_FORMAT_BAYER_10BPP,
        &s_os08a10_mipi_4lane_10bit_8m_linear_attr,
        &s_os08a10_8m_60fps_10bit_linear_isp_attr
    },
    {"os08a10", OV_OS08A10_MIPI_8M_WDR2TO1_30FPS_10BIT, "OV_OS08A10_MIPI_8M_WDR2TO1_30FPS_10BIT",
        E_LANE_MODE_4_2_2,
        E_PIXEL_FORMAT_BAYER_10BPP,
        &s_os08a10_mipi_4lane_10bit_8m_linear_attr,
        &s_os08a10_8m_30fps_10bit_wdr2to1_isp_attr
    },
    // os08a20
    {"os08a20", OV_OS08A20_MIPI_8M_60FPS_10BIT, "OV_OS08A20_MIPI_8M_60FPS_10BIT",
        E_LANE_MODE_4_2_2,
        E_PIXEL_FORMAT_BAYER_10BPP,
        &s_os08a10_mipi_4lane_10bit_8m_linear_attr,
        &s_os08a20_8m_60fps_10bit_linear_isp_attr
    },
    {"os08a20", OV_OS08A20_MIPI_8M_50FPS_10BIT, "OV_OS08A20_MIPI_8M_50FPS_10BIT",
        E_LANE_MODE_4_2_2,
        E_PIXEL_FORMAT_BAYER_10BPP,
        &s_os08a10_mipi_4lane_10bit_8m_linear_attr,
        &s_os08a20_8m_50fps_10bit_linear_isp_attr
    },
    {"os08a20", OV_OS08A20_MIPI_8M_30FPS_10BIT, "OV_OS08A20_MIPI_8M_30FPS_10BIT",
        E_LANE_MODE_4_2_2,
        E_PIXEL_FORMAT_BAYER_10BPP,
        &s_os08a10_mipi_4lane_10bit_8m_linear_attr,
        &s_os08a20_8m_30fps_10bit_linear_isp_attr
    },
    {"os08a20", OV_OS08A20_MIPI_2M_120FPS_10BIT, "OV_OS08A20_MIPI_2M_120FPS_10BIT",
        E_LANE_MODE_4_2_2,
        E_PIXEL_FORMAT_BAYER_10BPP,
        &s_os08a10_mipi_4lane_10bit_2m_linear_attr,
        &s_os08a20_2m_120fps_10bit_linear_isp_attr
    },
    // os04a10
    {"os04a10", OV_OS04A10_MIPI_4M_30FPS_10BIT, "OV_OS04A10_MIPI_4M_30FPS_10BIT",
        E_LANE_MODE_4_2_2,
        E_PIXEL_FORMAT_BAYER_10BPP,
        &s_os04a10_mipi_4lane_10bit_4m_linear_attr,
        &s_os04a10_4m_30fps_10bit_linear_isp_attr
    },
    {"os04a10", OV_OS04A10_MIPI_2M_30FPS_10BIT, "OV_OS04A10_MIPI_2M_30FPS_10BIT",
        E_LANE_MODE_4_2_2,
        E_PIXEL_FORMAT_BAYER_10BPP,
        &s_os04a10_mipi_4lane_10bit_2m_linear_attr,
        &s_os04a10_2m_30fps_10bit_linear_isp_attr
    },
    {"os04a10", OV_OS04A10_MIPI_4MFULL_25FPS_12BIT, "OV_OS04A10_MIPI_4MFULL_25FPS_12BIT",
        E_LANE_MODE_4_2_2,
        E_PIXEL_FORMAT_BAYER_12BPP,
        &s_os04a10_mipi_4lane_12bit_4mfull_linear_attr,
        &s_os04a10_4mfull_25fps_12bit_linear_isp_attr
    },
    {"os04a10", OV_OS04A10_MIPI_4MFULL_WDR2TO1_25FPS_10BIT, "OV_OS04A10_MIPI_4MFULL_WDR2TO1_25FPS_10BIT",
        E_LANE_MODE_4_2_2,
        E_PIXEL_FORMAT_BAYER_10BPP,
        &s_os04a10_mipi_4lane_12bit_4mfull_linear_attr,
        &s_os04a10_4mfull_25fps_10bit_wdr2t1_isp_attr
    },
    {"os04c10", OV_OS04C10_MIPI_4MFULL_30FPS_12BIT, "OV_OS04C10_MIPI_4MFULL_30FPS_12BIT",
        E_LANE_MODE_4_2_2,
        E_PIXEL_FORMAT_BAYER_12BPP,
        &s_os04c10_mipi_4lane_12bit_4mfull_linear_attr,
        &s_os04c10_4mfull_30fps_12bit_linear_isp_attr
    },
    {"ov9734", OV_OV9734_MIPI_1M_30FPS_10BIT, "OV_OV9734_MIPI_1M_30FPS_10BIT",
        E_LANE_MODE_2_2_2_2,
        E_PIXEL_FORMAT_BAYER_10BPP,
        &s_ov9734_mipi_1lane_10bit_1m_linear_attr,
        &s_ov9734_1m_30fps_10bit_linear_isp_attr
    },
    // ox03c10
    {"sg2_ox03c10", SG2_OX03C10_MIPI_2M_WDR4TO1_BUILTIN_60FPS_12BIT, "SG2_OX03C10_MIPI_2M_WDR4TO1_BUILTIN_60FPS_12BIT",
        E_LANE_MODE_4_2_2,
        E_PIXEL_FORMAT_BAYER_12BPP,
        &s_ox03c10_mipi_4lane_12bit_2m_wdr4t1_attr,
        &s_ox03c10_2m_60fps_12bit_wdr4t1_isp_attr
    },
    // sc450ai
    {"sc450ai", SC_450AI_MIPI_4M_30FPS_10BIT, "SC_450AI_MIPI_4M_30FPS_10BIT",
        E_LANE_MODE_4_2_2,
        E_PIXEL_FORMAT_BAYER_10BPP,
        &s_sc450ai_mipi_4lane_10bit_4m_linear_attr,
        &s_sc450ai_4m_30fps_10bit_linear_isp_attr
    },
    {"sc450ai", SC_450AI_MIPI_4M_25FPS_10BIT, "SC_450AI_MIPI_4M_25FPS_10BIT",
        E_LANE_MODE_4_2_2,
        E_PIXEL_FORMAT_BAYER_10BPP,
        &s_sc450ai_mipi_4lane_10bit_4m_linear_attr,
        &s_sc450ai_4m_25fps_10bit_linear_isp_attr
    },
    {"sc450ai", SC_450AI_MIPI_4MFULL_25FPS_10BIT, "SC_450AI_MIPI_4MFULL_25FPS_10BIT",
        E_LANE_MODE_4_2_2,
        E_PIXEL_FORMAT_BAYER_10BPP,
        &s_sc450ai_mipi_4lane_10bit_4mfull_linear_attr,
        &s_sc450ai_4mfull_25fps_10bit_linear_isp_attr
    },
    {"sc450ai", SC_450AI_MIPI_4M_WDR2TO1_30FPS_10BIT, "SC_450AI_MIPI_4M_WDR2TO1_30FPS_10BIT",
        E_LANE_MODE_4_2_2,
        E_PIXEL_FORMAT_BAYER_10BPP,
        &s_sc450ai_mipi_4lane_10bit_4m_linear_attr,
        &s_sc450ai_4m_30fps_10bit_wdr2t1_isp_attr
    },
    {"sc450ai", SC_450AI_MIPI_4M_WDR2TO1_25FPS_10BIT, "SC_450AI_MIPI_4M_WDR2TO1_25FPS_10BIT",
        E_LANE_MODE_4_2_2,
        E_PIXEL_FORMAT_BAYER_10BPP,
        &s_sc450ai_mipi_4lane_10bit_4m_linear_attr,
        &s_sc450ai_4m_25fps_10bit_wdr2t1_isp_attr
    },
    {"sc450ai", SC_450AI_MIPI_2M_WDR3TO1_30FPS_10BIT, "SC_450AI_MIPI_2M_WDR3TO1_30FPS_10BIT",
        E_LANE_MODE_4_2_2,
        E_PIXEL_FORMAT_BAYER_10BPP,
        &s_sc450ai_mipi_4lane_10bit_2m_linear_attr,
        &s_sc450ai_2m_30fps_10bit_wdr3t1_isp_attr
    },
    {"sc450ai", SC_450AI_MIPI_4MFULL_WDR2TO1_25FPS_10BIT, "SC_450AI_MIPI_4MFULL_WDR2TO1_25FPS_10BIT",
        E_LANE_MODE_4_2_2,
        E_PIXEL_FORMAT_BAYER_10BPP,
        &s_sc450ai_mipi_4lane_10bit_4mfull_linear_attr,
        &s_sc450ai_4mfull_25fps_10bit_wdr2to1_isp_attr
    },
    // sc910gs
    {"sc910gs", SC_910GS_MIPI_8M_25FPS_12BIT, "SC_910GS_MIPI_8M_25FPS_12BIT",
        E_LANE_MODE_4_2_2,
        E_PIXEL_FORMAT_BAYER_12BPP,
        &s_sc910gs_mipi_4lane_12bit_8m_linear_attr,
        &s_sc910gs_8m_25fps_12bit_linear_isp_attr
    },
    // sc200ai
    {"sc200ai", SC_200AI_MIPI_2L_2M_30FPS_10BIT, "SC_200AI_MIPI_2L_2M_30FPS_10BIT",
        E_LANE_MODE_2_2_2_2,
        E_PIXEL_FORMAT_BAYER_10BPP,
        &s_sc200ai_mipi_2lane_10bit_2m_linear_attr,
        &s_sc200ai_2m_30fps_10bit_linear_isp_attr
    },
    {"sc200ai", SC_200AI_MIPI_2L_2M_WDR2TO1_30FPS_10BIT, "SC_200AI_MIPI_2L_2M_WDR2TO1_30FPS_10BIT",
        E_LANE_MODE_2_2_2_2,
        E_PIXEL_FORMAT_BAYER_10BPP,
        &s_sc200ai_mipi_2lane_10bit_2m_linear_attr,
        &s_sc200ai_2m_30fps_10bit_wdr2t1_isp_attr
    },
    {"sc200ai", SC_200AI_MIPI_2L_2M_25FPS_10BIT, "SC_200AI_MIPI_2L_2M_25FPS_10BIT",
        E_LANE_MODE_2_2_2_2,
        E_PIXEL_FORMAT_BAYER_10BPP,
        &s_sc200ai_mipi_2lane_10bit_2m_linear_attr,
        &s_sc200ai_2m_25fps_10bit_linear_isp_attr
    },
    {"sc200ai", SC_200AI_MIPI_2L_2M_WDR2TO1_25FPS_10BIT, "SC_200AI_MIPI_2L_2M_WDR2TO1_25FPS_10BIT",
        E_LANE_MODE_2_2_2_2,
        E_PIXEL_FORMAT_BAYER_10BPP,
        &s_sc200ai_mipi_2lane_10bit_2m_linear_attr,
        &s_sc200ai_2m_25fps_10bit_wdr2t1_isp_attr
    },
    // sc535hgs
    {"sc535hgs", SC_535HGS_MIPI_4L_5M_60FPS_12BIT, "SC_535HGS_MIPI_4L_5M_60FPS_12BIT",
        E_LANE_MODE_4_2_2,
        E_PIXEL_FORMAT_BAYER_12BPP,
        &s_sc535hgs_mipi_4lane_12bit_5m_linear_attr,
        &s_sc535hgs_5m_60fps_12bit_linear_isp_attr
    },
    // sc430ai
    {"sc430ai", SC_430AI_MIPI_4M_30FPS_10BIT, "SC_430AI_MIPI_4M_30FPS_10BIT",
        E_LANE_MODE_4_2_2,
        E_PIXEL_FORMAT_BAYER_10BPP,
        &s_sc430ai_mipi_4lane_10bit_4m_linear_attr,
        &s_sc430ai_4m_30fps_10bit_linear_isp_attr
    },
    {"sc430ai", SC_430AI_MIPI_4MFULL_30FPS_10BIT, "SC_430AI_MIPI_4MFULL_30FPS_10BIT",
        E_LANE_MODE_4_2_2,
        E_PIXEL_FORMAT_BAYER_10BPP,
        &s_sc430ai_mipi_4lane_10bit_4mfull_linear_attr,
        &s_sc430ai_4mfull_30fps_10bit_linear_isp_attr
    },
    {"sc430ai", SC_430AI_MIPI_2M_30FPS_10BIT, "SC_430AI_MIPI_2M_30FPS_10BIT",
        E_LANE_MODE_4_2_2,
        E_PIXEL_FORMAT_BAYER_10BPP,
        &s_sc430ai_mipi_4lane_10bit_2m_linear_attr,
        &s_sc430ai_2m_30fps_10bit_linear_isp_attr
    },
    {"sc430ai", SC_430AI_MIPI_4MFULL_60FPS_10BIT, "SC_430AI_MIPI_4MFULL_60FPS_10BIT",
        E_LANE_MODE_4_2_2,
        E_PIXEL_FORMAT_BAYER_10BPP,
        &s_sc430ai_mipi_4lane_10bit_4mfull_linear_attr,
        &s_sc430ai_4mfull_60fps_10bit_linear_isp_attr
    },
    {"sc430ai", SC_430AI_MIPI_4MFULL_WDR2TO1_30FPS_10BIT, "SC_430AI_MIPI_4MFULL_WDR2TO1_30FPS_10BIT",
        E_LANE_MODE_4_2_2,
        E_PIXEL_FORMAT_BAYER_10BPP,
        &s_sc430ai_mipi_4lane_10bit_4mfull_linear_attr,
        &s_sc430ai_4mfull_30fps_10bit_wdr2t1_isp_attr
    },
    {"sc430ai", SC_430AI_MIPI_4M_WDR2TO1_30FPS_10BIT, "SC_430AI_MIPI_4M_WDR2TO1_30FPS_10BIT",
        E_LANE_MODE_4_2_2,
        E_PIXEL_FORMAT_BAYER_10BPP,
        &s_sc430ai_mipi_4lane_10bit_4m_linear_attr,
        &s_sc430ai_4m_30fps_10bit_wdr2t1_isp_attr
    },
    {"sc430ai", SC_430AI_MIPI_2M_WDR2TO1_30FPS_10BIT, "SC_430AI_MIPI_2M_WDR2TO1_30FPS_10BIT",
        E_LANE_MODE_4_2_2,
        E_PIXEL_FORMAT_BAYER_10BPP,
        &s_sc430ai_mipi_4lane_10bit_2m_linear_attr,
        &s_sc430ai_2m_30fps_10bit_wdr2t1_isp_attr
    },
    // imx334
    {"imx334", SONY_IMX334_MIPI_8M_30FPS_12BIT, "SONY_IMX334_MIPI_8M_30FPS_12BIT",
        E_LANE_MODE_4_2_2,
        E_PIXEL_FORMAT_BAYER_12BPP,
        &s_imx334_mipi_4lane_12bit_8m_linear_attr,
        &s_imx334_8m_30fps_12bit_linear_isp_attr
    },
    {"imx334", SONY_IMX334_MIPI_8M_45FPS_12BIT, "SONY_IMX334_MIPI_8M_45FPS_12BIT",
        E_LANE_MODE_4_2_2,
        E_PIXEL_FORMAT_BAYER_12BPP,
        &s_imx334_mipi_4lane_12bit_8m_linear_attr,
        &s_imx334_8m_45fps_12bit_linear_isp_attr
    },
    {"imx334", SONY_IMX334_MIPI_8M_60FPS_12BIT, "SONY_IMX334_MIPI_8M_60FPS_12BIT",
        E_LANE_MODE_4_2_2,
        E_PIXEL_FORMAT_BAYER_12BPP,
        &s_imx334_mipi_4lane_12bit_8m_linear_attr,
        &s_imx334_8m_60fps_12bit_linear_isp_attr
    },
    // imx347
    {"imx347", SONY_IMX347_MIPI_4MFULL_25FPS_12BIT, "SONY_IMX347_MIPI_4MFULL_25FPS_12BIT",
        E_LANE_MODE_4_2_2,
        E_PIXEL_FORMAT_BAYER_12BPP,
        &s_imx347_mipi_4lane_12bit_4mfull_linear_attr,
        &s_imx347_4mfull_25fps_12bit_linear_isp_attr
    },
    {"imx347", SONY_IMX347_MIPI_4MFULL_30FPS_12BIT, "SONY_IMX347_MIPI_4MFULL_30FPS_12BIT",
        E_LANE_MODE_4_2_2,
        E_PIXEL_FORMAT_BAYER_12BPP,
        &s_imx347_mipi_4lane_12bit_4mfull_linear_attr,
        &s_imx347_4mfull_30fps_12bit_linear_isp_attr
    },
    {"imx347", SONY_IMX347_MIPI_4M_WDR2TO1_25FPS_12BIT, "SONY_IMX347_MIPI_4M_WDR2TO1_25FPS_12BIT",
        E_LANE_MODE_4_2_2,
        E_PIXEL_FORMAT_BAYER_12BPP,
        &s_imx347_mipi_4lane_12bit_4m_wdr2t1_attr,
        &s_imx347_4m_25fps_12bit_wdr2t1_isp_attr
    },
    {"imx347", SONY_IMX347_MIPI_4M_WDR2TO1_30FPS_12BIT, "SONY_IMX347_MIPI_4M_WDR2TO1_30FPS_12BIT",
        E_LANE_MODE_4_2_2,
        E_PIXEL_FORMAT_BAYER_12BPP,
        &s_imx347_mipi_4lane_12bit_4m_wdr2t1_attr,
        &s_imx347_4m_30fps_12bit_wdr2t1_isp_attr
    },
    {"imx347", SONY_IMX347_MIPI_4MFULL_WDR2TO1_25FPS_12BIT, "SONY_IMX347_MIPI_4MFULL_WDR2TO1_25FPS_12BIT",
        E_LANE_MODE_4_2_2,
        E_PIXEL_FORMAT_BAYER_12BPP,
        &s_imx347_mipi_4lane_12bit_4mfull_wdr2t1_attr,
        &s_imx347_4mfull_25fps_12bit_wdr2t1_isp_attr
    },
    // imx485
    {"imx485", SONY_IMX485_MIPI_8M_15FPS_12BIT, "SONY_IMX485_MIPI_8M_15FPS_12BIT",
        E_LANE_MODE_4_2_2,
        E_PIXEL_FORMAT_BAYER_12BPP,
        &s_imx485_mipi_4lane_12bit_8m_linear_attr,
        &s_imx485_8m_15fps_12bit_linear_isp_attr
    },
    {"imx485", SONY_IMX485_MIPI_4L2C_8M_30FPS_12BIT, "SONY_IMX485_MIPI_4L2C_8M_30FPS_12BIT",
        E_LANE_MODE_8,
        E_PIXEL_FORMAT_BAYER_12BPP,
        &s_imx485_mipi_4lane_2ch_12bit_8m_linear_attr,
        &s_imx485_4l2c_8m_30fps_12bit_linear_isp_attr
    },
    {"imx485", SONY_IMX485_MIPI_8L_8M_30FPS_12BIT, "SONY_IMX485_MIPI_8L_8M_30FPS_12BIT",
        E_LANE_MODE_8,
        E_PIXEL_FORMAT_BAYER_12BPP,
        &s_imx485_mipi_8lane_12bit_8m_linear_attr,
        &s_imx485_8l_8m_30fps_12bit_linear_isp_attr
    },
    // imx305
    {"imx305", SONY_IMX305_SLVDS_8M_20FPS_12BIT, "SONY_IMX305_SLVDS_8M_20FPS_12BIT",
        E_LANE_MODE_4_2_2,
        E_PIXEL_FORMAT_BAYER_12BPP,
        &s_imx305_slvds_4lane_12bit_8m_linear_attr,
        &s_imx305_8m_20fps_12bit_linear_isp_attr
    },
    {"imx305", SONY_IMX305_SLVDS_8L_9M_25FPS_12BIT, "SONY_IMX305_SLVDS_8L_9M_25FPS_12BIT",
        E_LANE_MODE_8,
        E_PIXEL_FORMAT_BAYER_12BPP,
        &s_imx305_slvds_8lane_12bit_9m_linear_attr,
        &s_imx305_8l_9m_25fps_12bit_linear_isp_attr
    },
    {"imx305", SONY_IMX305_SLVDS_8L_9M_30FPS_12BIT, "SONY_IMX305_SLVDS_8L_9M_30FPS_12BIT",
        E_LANE_MODE_8,
        E_PIXEL_FORMAT_BAYER_12BPP,
        &s_imx305_slvds_8lane_12bit_9m_linear_attr,
        &s_imx305_8l_9m_30fps_12bit_linear_isp_attr
    },
    {"imx305", SONY_IMX305_SLVDS_16L_8M_25FPS_12BIT, "SONY_IMX305_SLVDS_16L_8M_25FPS_12BIT",
        E_LANE_MODE_16,
        E_PIXEL_FORMAT_BAYER_12BPP,
        &s_imx305_slvds_16lane_12bit_8m_linear_attr,
        &s_imx305_16l_8m_25fps_12bit_linear_isp_attr
    },
    {"imx305", SONY_IMX305_SLVDS_16L_9M_50FPS_12BIT, "SONY_IMX305_SLVDS_16L_9M_50FPS_12BIT",
        E_LANE_MODE_16,
        E_PIXEL_FORMAT_BAYER_12BPP,
        &s_imx305_slvds_16lane_12bit_9m_linear_attr,
        &s_imx305_16l_9m_50fps_12bit_linear_isp_attr
    },
    // imx385
    {"imx385", SONY_IMX385_MIPI_2M_25FPS_12BIT, "SONY_IMX385_MIPI_2M_25FPS_12BIT",
        E_LANE_MODE_2_2_2_2,
        E_PIXEL_FORMAT_BAYER_12BPP,
        &s_imx385_mipi_2lane_12bit_2m_linear_attr,
        &s_imx385_2l_2m_25fps_12bit_linear_isp_attr
    },
    // imx415
    {"imx415", SONY_IMX415_MIPI_8M_60FPS_12BIT, "SONY_IMX415_MIPI_8M_60FPS_12BIT",
        E_LANE_MODE_4_2_2,
        E_PIXEL_FORMAT_BAYER_12BPP,
        &s_imx415_mipi_4lane_12bit_8m_linear_attr,
        &s_imx415_8m_60fps_12bit_linear_isp_attr
    },
    {"imx415", SONY_IMX415_MIPI_8M_WDR2TO1_30FPS_12BIT, "SONY_IMX415_MIPI_8M_WDR2TO1_30FPS_12BIT",
        E_LANE_MODE_4_2_2,
        E_PIXEL_FORMAT_BAYER_12BPP,
        &s_imx415_mipi_4lane_12bit_8m_wdr_attr,
        &s_imx415_8m_30fps_12bit_wdr2to1_isp_attr
    },
    {"imx415", SONY_IMX415_MIPI_8M_25FPS_12BIT, "SONY_IMX415_MIPI_8M_25FPS_12BIT",
        E_LANE_MODE_4_2_2,
        E_PIXEL_FORMAT_BAYER_12BPP,
        &s_imx415_mipi_4lane_12bit_8m_linear_attr,
        &s_imx415_8m_25fps_12bit_linear_isp_attr
    },
    {"imx415", SONY_IMX415_MIPI_8M_WDR2TO1_25FPS_12BIT, "SONY_IMX415_MIPI_8M_WDR2TO1_25FPS_12BIT",
        E_LANE_MODE_4_2_2,
        E_PIXEL_FORMAT_BAYER_12BPP,
        &s_imx415_mipi_4lane_12bit_8m_wdr_attr,
        &s_imx415_8m_25fps_12bit_wdr2to1_isp_attr
    },
    // gmax3809
    {"gmax3809", GPIXEL_GMAX3809_SLVDS_9M_LINEAR_25FPS_12BIT, "GPIXEL_GMAX3809_SLVDS_9M_LINEAR_25FPS_12BIT",
        E_LANE_MODE_4_2_2,
        E_PIXEL_FORMAT_BAYER_12BPP,
        &s_gmax3809_slvds_4lane_12bit_9m_linear_attr,
        &s_gmax3809_9m_25fps_12bit_linear_isp_attr
    },
    {"gmax3809", GPIXEL_GMAX3809_SLVDS_8L_9M_LINEAR_30FPS_12BIT, "GPIXEL_GMAX3809_SLVDS_8L_9M_LINEAR_30FPS_12BIT",
        E_LANE_MODE_8,
        E_PIXEL_FORMAT_BAYER_12BPP,
        &s_gmax3809_slvds_8lane_12bit_9m_linear_attr,
        &s_gmax3809_9m_30fps_12bit_linear_isp_attr
    },
    // gc4653
    {"gc4653", GC_GC4653_MIPI_4M_30FPS_10BIT, "GC_GC4653_MIPI_4M_30FPS_10BIT",
        E_LANE_MODE_2_2_2_2,
        E_PIXEL_FORMAT_BAYER_10BPP,
        &s_gc4653_mipi_2lane_10bit_4m_linear_attr,
        &s_gc4653_4m_30fps_10bit_linear_isp_attr
    },
    // gc4023
    {"gc4023", GC_GC4023_MIPI_4M_25FPS_10BIT, "GC_GC4023_MIPI_4M_25FPS_10BIT",
        E_LANE_MODE_2_2_2_2,
        E_PIXEL_FORMAT_BAYER_10BPP,
        &s_gc4023_mipi_2lane_10bit_4m_linear_attr,
        &s_gc4023_4m_25fps_10bit_linear_isp_attr
    },
    // imx686
    {"imx686", SONY_IMX686_MIPI_8M_30FPS_14BIT, "SONY_IMX686_MIPI_8M_30FPS_14BIT",
        E_LANE_MODE_4_2_2,
        E_PIXEL_FORMAT_BAYER_14BPP,
        &s_imx686_mipi_4lane_14bit_8m_linear_attr,
        &s_imx686_8m_30fps_14bit_linear_isp_attr
    },
    {"imx686", SONY_IMX686_MIPI_8M_30FPS_14BIT_PD, "SONY_IMX686_MIPI_8M_30FPS_14BIT_PD",
        E_LANE_MODE_4_2_2,
        E_PIXEL_FORMAT_BAYER_14BPP,
        &s_imx686_mipi_4lane_14bit_8m_linear_attr,
        &s_imx686_8m_30fps_14bit_linear_isp_pd_attr,
    },
    {"imx686", SONY_IMX686_MIPI_8M_QHDR_30FPS_14BIT, "SONY_IMX686_MIPI_8M_QHDR_30FPS_14BIT",
        E_LANE_MODE_4_2_2,
        E_PIXEL_FORMAT_BAYER_14BPP,
        &s_imx686_mipi_4lane_14bit_8m_linear_attr,
        &s_imx686_8m_30fps_14bit_qhdr_isp_attr,
    },
    {"imx686", SONY_IMX686_MIPI_8M_50FPS_10BIT, "SONY_IMX686_MIPI_8M_50FPS_10BIT",
        E_LANE_MODE_4_2_2,
        E_PIXEL_FORMAT_BAYER_10BPP,
        &s_imx686_mipi_4lane_14bit_8m_linear_attr,
        &s_imx686_8m_50fps_10bit_linear_isp_attr
    },
    // imx586
    {"imx586", SONY_IMX586_MIPI_8M_30FPS_10BIT, "SONY_IMX586_MIPI_8M_30FPS_10BIT",
        E_LANE_MODE_4_2_2,
        E_PIXEL_FORMAT_BAYER_10BPP,
        &s_imx586_mipi_4lane_10bit_8m_linear_attr,
        &s_imx586_8m_30fps_10bit_linear_isp_attr
    },
    {"imx586", SONY_IMX586_MIPI_8M_QHDR_30FPS_10BIT, "SONY_IMX586_MIPI_8M_QHDR_30FPS_10BIT",
        E_LANE_MODE_4_2_2,
        E_PIXEL_FORMAT_BAYER_10BPP,
        &s_imx586_mipi_4lane_10bit_8m_linear_attr,
        &s_imx586_8m_30fps_10bit_qhdr_isp_attr,
    },
    {"imx586", SONY_IMX586_MIPI_2M_QHDR_30FPS_10BIT, "SONY_IMX586_MIPI_2M_QHDR_30FPS_10BIT",
        E_LANE_MODE_4_2_2,
        E_PIXEL_FORMAT_BAYER_10BPP,
        &s_imx586_mipi_4lane_10bit_2m_linear_attr,
        &s_imx586_2m_30fps_10bit_qhdr_isp_attr,
    },
    {"imx586", SONY_IMX586_MIPI_12M_30FPS_10BIT, "SONY_IMX586_MIPI_12M_30FPS_10BIT",
        E_LANE_MODE_4_2_2,
        E_PIXEL_FORMAT_BAYER_10BPP,
        &s_imx586_mipi_4lane_10bit_12m_linear_attr,
        &s_imx586_12m_30fps_10bit_linear_isp_attr
    },
    {"imx586", SONY_IMX586_MIPI_48M_1FPS_10BIT, "SONY_IMX586_MIPI_48M_1FPS_10BIT",
        E_LANE_MODE_4_2_2,
        E_PIXEL_FORMAT_BAYER_10BPP,
        &s_imx586_mipi_4lane_10bit_48m_linear_attr,
        &s_imx586_48m_1fps_10bit_linear_isp_attr
    },
    {"imx586", SONY_IMX586_MIPI_48M_3FPS_10BIT, "SONY_IMX586_MIPI_48M_3FPS_10BIT",
        E_LANE_MODE_4_2_2,
        E_PIXEL_FORMAT_BAYER_10BPP,
        &s_imx586_mipi_4lane_10bit_48m_linear_attr,
        &s_imx586_48m_3fps_10bit_linear_isp_attr
    },
    {"imx586", SONY_IMX586_MIPI_2M_60FPS_10BIT, "SONY_IMX586_MIPI_2M_60FPS_10BIT",
        E_LANE_MODE_4_2_2,
        E_PIXEL_FORMAT_BAYER_10BPP,
        &s_imx586_mipi_4lane_10bit_2m_linear_attr,
        &s_imx586_2m_60fps_10bit_linear_isp_attr
    },
    {"imx586", SONY_IMX586_MIPI_2M_120FPS_10BIT, "SONY_IMX586_MIPI_2M_120FPS_10BIT",
        E_LANE_MODE_4_2_2,
        E_PIXEL_FORMAT_BAYER_10BPP,
        &s_imx586_mipi_4lane_10bit_2m_linear_attr,
        &s_imx586_2m_120fps_10bit_linear_isp_attr
    },
    // bg0651
    {"bg0651", BG0651_MIPI_100FPS_12BIT, "BG0651_MIPI_100FPS_12BIT",
        E_LANE_MODE_4_2_2,
        E_PIXEL_FORMAT_BAYER_12BPP,
        &s_bg0651_mipi_4lane_12bit_linear_attr,
        &s_bg0651_100fps_12bit_linear_isp_attr
    },
};

/*******************************************************
                static sensor cfg function
*******************************************************/
static vs_bool_t sample_common_vii_sensor_type_support(
        sample_sensor_type_e sensor_type)
{
#ifdef VS_ORION
    vs_int32_t i;
    for (i = 0; i < sizeof(s_sensor_cfg)/sizeof(sample_sensor_cfg_s); i++) {
        if (s_sensor_cfg[i].sensor_type != sensor_type) {
            continue;
        }
        if (((vs_uint32_t)s_sensor_cfg[i].p_isp_attr->frame_rate) *
                s_sensor_cfg[i].p_isp_attr->sensor_size.width *
                s_sensor_cfg[i].p_isp_attr->sensor_size.height
                > 45 * 3840 * 2160) {
            return VS_FALSE;
        }
        break;
    }
#endif

    return VS_TRUE;
}

static inline sample_sensor_type_e default_sensor_type_get(vs_int32_t sensor_id)
{
    if (sensor_id < 0 || sensor_id > 3) {
        vs_sample_trace("invalid sensor id: %d.\n", sensor_id);
        return SENSOR0_TYPE;
    }

    return g_sensor_type[sensor_id];
}

vs_void_t sample_common_vii_sensor_name_get(
        sample_sensor_type_e sensor_type, char *sensor_name)
{
    vs_int32_t i;

    for (i = 0; i < sizeof(s_sensor_cfg)/sizeof(sample_sensor_cfg_s); i++) {
        if (s_sensor_cfg[i].sensor_type == sensor_type) {
            strcpy(sensor_name, s_sensor_cfg[i].sensor_name);
            return;
        }
    }

    strcpy(sensor_name, "os08a10");
}

vs_uint32_t sample_common_vii_wdr_frame_num_get_by_sensor(
        sample_sensor_type_e sensor_type)
{
    vs_int32_t i;

    for (i = 0; i < sizeof(s_sensor_cfg)/sizeof(sample_sensor_cfg_s); i++) {
        if (s_sensor_cfg[i].sensor_type == sensor_type) {
            switch(s_sensor_cfg[i].p_isp_attr->wdr_mode) {
            case E_WDR_MODE_NONE:
                return 1;
            case E_WDR_MODE_2To1_LINE:
                return 2;
            case E_WDR_MODE_3To1_LINE:
                return 3;
            case E_WDR_MODE_4To1_LINE:
                return 4;
            default:
                return 1;
            }
        }
    }

    return 1;
}

static vs_lane_mode_e sample_common_vii_mipirx_lane_mode_get(
        sample_sensor_type_e sensor_type)
{
    vs_int32_t i;

    for (i = 0; i < sizeof(s_sensor_cfg)/sizeof(sample_sensor_cfg_s); i++) {
        if (s_sensor_cfg[i].sensor_type == sensor_type) {
            return s_sensor_cfg[i].lane_mode;
        }
    }

    return E_LANE_MODE_4_2_2;
}

static vs_void_t sample_common_vii_mipirx_attr_get(
        sample_sensor_type_e sensor_type, vs_mipi_rx_dev_attr_s *mipi_rx_attr)
{
    vs_int32_t i;

    for (i = 0; i < sizeof(s_sensor_cfg)/sizeof(sample_sensor_cfg_s); i++) {
        if (s_sensor_cfg[i].sensor_type == sensor_type) {
            memcpy(mipi_rx_attr, s_sensor_cfg[i].p_mipi_attr,
                    sizeof(vs_mipi_rx_dev_attr_s));
            return;
        }
    }

    memcpy(mipi_rx_attr, &s_os08a10_mipi_4lane_10bit_8m_linear_attr,
            sizeof(vs_mipi_rx_dev_attr_s));
}

static vs_void_t sample_common_vii_dev_attr_get(
        sample_sensor_type_e sensor_type, vs_vii_dev_attr_s *dev_attr)
{
    vs_int32_t i;
    vs_vii_dev_attr_s attr = {
        .intf_type = E_VII_INTF_MIPI,
        .pixel_format = E_PIXEL_FORMAT_BAYER_10BPP
    };

    for (i = 0; i < sizeof(s_sensor_cfg)/sizeof(sample_sensor_cfg_s); i++) {
        if (s_sensor_cfg[i].sensor_type == sensor_type) {
            switch(s_sensor_cfg[i].p_mipi_attr->mode) {
            case E_MODE_MIPI:
                attr.intf_type = E_VII_INTF_MIPI;
                break;
            case E_MODE_SLVDS:
                attr.intf_type = E_VII_INTF_SLVDS;
                break;
            default:
                attr.intf_type = E_VII_INTF_MIPI;
                break;
            }
            attr.pixel_format = s_sensor_cfg[i].pixel_format;
            break;
        }
    }

    memcpy(dev_attr, &attr, sizeof(vs_vii_dev_attr_s));
}

static vs_void_t sample_common_vii_vc_attr_get(
        sample_sensor_type_e sensor_type, vs_vii_vc_attr_s *p_vc_attr)
{
    vs_int32_t i;
    vs_int32_t frame_num;

    frame_num = sample_common_vii_wdr_frame_num_get_by_sensor(sensor_type);

    for (i = 0; i < frame_num; i++) {
        p_vc_attr->vc[i] = E_VII_VC_ID_0 + i;
    }
}

static sample_vii_pipe_cfg_s s_vii_common_pipe_cfg = {
    .pipe_id = 0,
    .pipe_attr = {
        .bypass_mode = E_VII_PIPE_BYPASS_NONE,
        .wdr_mode = E_WDR_MODE_NONE,
        .image_size = {1920, 1080},
        .pixel_format = E_PIXEL_FORMAT_BAYER_10BPP,
        .compress_mode = E_COMPRESS_MODE_NONE,
        .framerate = {-1, -1},
    },

    .pipe_param.mode = E_VII_PIPE_ADVANCE_MODE_NONE,

    .fpn_attr.enable = VS_FALSE,
    .firmware_bypass = VS_FALSE,

    .isp_attr = {
        .sensor_size = {1920, 1080},
        .frame_rate = 1.0,
        .bayer_format = E_ISP_BAYER_FORMAT_RGGB,
        .wdr_mode = E_WDR_MODE_NONE,
        .sensor_mode = 3,
    },

    .phys_chn_num = 1,
    .phys_chn_cfg[0] = {
        .chn_id = 0,
        .chn_attr = {
            .image_size = {1920, 1080},
            .video_format = E_VIDEO_FORMAT_LINEAR,
            .pixel_format = E_PIXEL_FORMAT_YUV_420SP,
            .dynamic_range = E_DYNAMIC_RANGE_SDR8,
            .compress_mode = E_COMPRESS_MODE_NONE,
            .framerate = {30, 30},
            .depth = 1,
        },
    },
    .ext_chn_num = 0,
};

static vs_void_t sample_common_vii_pipe_cfg_size_set(
        sample_vii_pipe_cfg_s *pipe_cfg, vs_size_s *size)
{
    vs_int32_t i;

    pipe_cfg->pipe_attr.image_size.width = size->width;
    pipe_cfg->pipe_attr.image_size.height = size->height;

    for (i = 0; i < pipe_cfg->phys_chn_num; i++) {
        sample_vii_phys_chn_cfg_s *chn_cfg = &pipe_cfg->phys_chn_cfg[i];
        chn_cfg->chn_attr.image_size.width = size->width;
        chn_cfg->chn_attr.image_size.height = size->height;
    }
}

vs_void_t sample_common_vii_img_size_get(
        sample_sensor_type_e sensor_type, vs_size_s *img_size)
{
    vs_int32_t i;

    for (i = 0; i < sizeof(s_sensor_cfg)/sizeof(sample_sensor_cfg_s); i++) {
        if (s_sensor_cfg[i].sensor_type == sensor_type) {
            img_size->width = s_sensor_cfg[i].p_isp_attr->sensor_size.width;
            img_size->height = s_sensor_cfg[i].p_isp_attr->sensor_size.height;
            return;
        }
    }

    img_size->width  = 3840;
    img_size->height = 2160;
}

vs_int32_t sample_common_vii_framerate_get(
        sample_sensor_type_e sensor_type, vs_int32_t *p_framerate)
{
    vs_int32_t i;

    for (i = 0; i < sizeof(s_sensor_cfg)/sizeof(sample_sensor_cfg_s); i++) {
        if (s_sensor_cfg[i].sensor_type == sensor_type) {
            *p_framerate = (vs_int32_t)(s_sensor_cfg[i].p_isp_attr->frame_rate + 0.5);
            return VS_SUCCESS;
        }
    }

    *p_framerate = 30;
    return VS_FAILED;
}

/* Add delay to ensure stable sensor output */
static vs_void_t sample_common_vii_delay_get(
        sample_sensor_type_e sensor_type, vs_uint32_t *p_delay_ms)
{
    switch (sensor_type) {
    case SONY_IMX415_MIPI_8M_WDR2TO1_30FPS_12BIT:
        *p_delay_ms = 500;
        break;

    default:
        *p_delay_ms = 0;
        break;
    }
}

static vs_void_t sample_common_vii_isp_attr_get(
        sample_sensor_type_e sensor_type, vs_isp_comm_attr_s *isp_attr)
{
    vs_int32_t i;

    for (i = 0; i < sizeof(s_sensor_cfg)/sizeof(sample_sensor_cfg_s); i++) {
        if (s_sensor_cfg[i].sensor_type == sensor_type) {
            memcpy(isp_attr, s_sensor_cfg[i].p_isp_attr, sizeof(vs_isp_comm_attr_s));
            return;
        }
    }

    memcpy(isp_attr, &s_os08a10_8m_30fps_10bit_linear_isp_attr,
            sizeof(vs_isp_comm_attr_s));
}

static vs_void_t sample_common_vii_pipe_cfg_get(
        sample_sensor_type_e sensor_type, sample_vii_pipe_cfg_s *pipe_cfg)
{
    vs_int32_t i;
    vs_size_s img_size;

    memcpy(pipe_cfg, &s_vii_common_pipe_cfg, sizeof(sample_vii_pipe_cfg_s));
    sample_common_vii_isp_attr_get(sensor_type, &pipe_cfg->isp_attr);
    sample_common_vii_img_size_get(sensor_type, &img_size);
    sample_common_vii_pipe_cfg_size_set(pipe_cfg, &img_size);
    pipe_cfg->pipe_attr.wdr_mode = pipe_cfg->isp_attr.wdr_mode;

    if (sensor_type == OV_OS04A10_MIPI_4MFULL_WDR2TO1_25FPS_10BIT ||
        sensor_type == SONY_IMX347_MIPI_4M_WDR2TO1_25FPS_12BIT ||
        sensor_type == SONY_IMX347_MIPI_4M_WDR2TO1_30FPS_12BIT ||
        sensor_type == SONY_IMX347_MIPI_4MFULL_WDR2TO1_25FPS_12BIT) {
        pipe_cfg->pipe_param.mode = E_VII_PIPE_ADVANCE_MODE_SUB_FRAME_FIRST;
    }

    for (i = 0; i < sizeof(s_sensor_cfg)/sizeof(sample_sensor_cfg_s); i++) {
        if (s_sensor_cfg[i].sensor_type == sensor_type) {
            pipe_cfg->pipe_attr.pixel_format = s_sensor_cfg[i].pixel_format;
            pipe_cfg->pipe_attr.real_fps =
                (vs_uint32_t)s_sensor_cfg[i].p_isp_attr->frame_rate + 1;

            switch (sensor_type) {
            case SONY_IMX305_SLVDS_16L_9M_50FPS_12BIT:
            case SONY_IMX586_MIPI_8M_30FPS_10BIT:
            case SONY_IMX586_MIPI_12M_30FPS_10BIT:
                pipe_cfg->pipe_attr.real_fps = 48;
                break;
            case SONY_IMX586_MIPI_2M_60FPS_10BIT:
            case SONY_IMX586_MIPI_2M_120FPS_10BIT:
                pipe_cfg->pipe_attr.real_fps = 240;
                break;
            case OV_OS08A20_MIPI_8M_30FPS_10BIT:
                pipe_cfg->pipe_attr.real_fps = 50;
                break;
            default:
                break;
            }

            return;
        }
    }

    pipe_cfg->pipe_attr.pixel_format = E_PIXEL_FORMAT_BAYER_10BPP;
    pipe_cfg->pipe_attr.real_fps = 30;
}


/*******************************************************
                  static vii function
*******************************************************/
static vs_int32_t sample_common_vi_vpp_mode_set(vs_vii_vpp_mode_e vi_vpp_mode)
{
    vs_int32_t i;
    vs_int32_t ret;
    vs_vii_vpp_mode_e other_pipe_mode;
    vs_vii_vpp_online_offline_attr_s online_offline_attr;

    if (vi_vpp_mode == E_VII_OFFLINE_VPP_ONLINE) {
        other_pipe_mode = E_VII_OFFLINE_VPP_ONLINE;
    } else {
        other_pipe_mode = E_VII_OFFLINE_VPP_OFFLINE;
    }

    online_offline_attr.mode[0] = vi_vpp_mode;
    for (i = 1; i < VII_MAX_PIPE_NUM; i++) {
        online_offline_attr.mode[i] = other_pipe_mode;
    }

    ret = vs_mal_vii_vpp_mode_set(&online_offline_attr);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_vii_vpp_mode_set failed, ret: 0x%x.\n", ret);
        return ret;
    }

    return ret;
}

static vs_int32_t sample_common_vii_dev_enable(vs_int32_t dev_id,
        const vs_vii_dev_attr_s *p_dev_attr, const vs_vii_vc_attr_s *p_vc_attr)
{
    vs_int32_t ret;

    ret = vs_mal_vii_dev_attr_set(dev_id, p_dev_attr);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vii set dev %d attr failed, ret: 0x%x.\n", dev_id, ret);
        return ret;
    }

    if (p_vc_attr != VS_NULL) {
        ret = vs_mal_vii_dev_vc_set(dev_id, p_vc_attr);
        if (ret != VS_SUCCESS) {
            vs_sample_trace("vii devid %d set vc attr failed, ret: 0x%x.\n", dev_id, ret);
            return ret;
        }
    }

    ret = vs_mal_vii_dev_enable(dev_id);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vii dev %d enable failed, ret: 0x%x.\n", dev_id, ret);
        return ret;
    }

    return VS_SUCCESS;
}

static vs_void_t sample_common_vii_dev_disable(vs_int32_t dev_id)
{
    vs_int32_t ret;

    ret = vs_mal_vii_dev_disable(dev_id);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vii dev %d disable failed, ret: 0x%x.\n", dev_id, ret);
    }
}

static vs_int32_t sample_common_vii_dev_pipe_bind(vs_int32_t dev_id,
        vs_int32_t pipe_id[], vs_uint32_t pipe_num)
{
    vs_int32_t i;
    vs_uint32_t ret;

    for (i = 0; i < pipe_num; i++) {
        ret = vs_mal_vii_dev_pipe_bind(dev_id, pipe_id[i]);
        if (ret != VS_SUCCESS) {
            vs_sample_trace("vi devid(%d) bind pipeid(%d) failed, ret: 0x%x.\n",
                    dev_id, pipe_id[i], ret);
            goto exit;
        }
    }

    return ret;

exit:
    for (i = i - 1; i < 0; i--) {
        vs_mal_vii_dev_pipe_unbind(dev_id, pipe_id[i]);
    }
    return VS_FAILED;
}

static vs_void_t sample_common_vii_dev_pipe_unbind(vs_int32_t dev_id,
        vs_int32_t pipe_id[], vs_uint32_t pipe_num)
{
    vs_int32_t i;
    vs_uint32_t ret;

    for (i = 0; i < pipe_num; i++) {
        ret = vs_mal_vii_dev_pipe_unbind(dev_id, pipe_id[i]);
        if (ret != VS_SUCCESS) {
            vs_sample_trace("vi devid(%d) unbind pipeid(%d) failed, ret: 0x%x.\n",
                    dev_id, pipe_id[i], ret);
        }
    }
}

static vs_int32_t sample_common_vii_phys_chn_enable(vs_int32_t pipe_id,
        vs_int32_t chn_id, const sample_vii_phys_chn_cfg_s *p_chn_cfg)
{
    vs_uint32_t ret;

    ret = vs_mal_vii_chn_attr_set(pipe_id, chn_id, &p_chn_cfg->chn_attr);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vii pipe %d chn %d set chn attr failed, ret: 0x%x.\n", pipe_id, chn_id, ret);
        return ret;
    }

    if (p_chn_cfg->lowlatency_attr.enable == VS_TRUE) {
        ret = vs_mal_vii_chn_lowlatency_set(pipe_id, chn_id, &p_chn_cfg->lowlatency_attr);
        if (ret != VS_SUCCESS) {
            vs_sample_trace("vii pipe %d chn %d set lowlatency attr failed, ret: 0x%x!\n", pipe_id, chn_id, ret);
            return ret;
        }
    }

    ret = vs_mal_vii_chn_enable(pipe_id, chn_id);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vii pipe %d chn %d enable failed, ret: 0x%x.\n", pipe_id, chn_id, ret);
        return ret;
    }

    return VS_SUCCESS;
}

static vs_int32_t sample_common_vii_phys_chn_disable(
        vs_int32_t pipe_id, vs_int32_t chn_id)
{
    vs_uint32_t ret;

    ret = vs_mal_vii_chn_disable(pipe_id, chn_id);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vii pipe %d chn %d disable failed, ret: 0x%x.\n", pipe_id, chn_id, ret);
        return ret;
    }

    return VS_SUCCESS;
}

static vs_int32_t sample_common_vii_ext_chn_enable(vs_int32_t pipe_id,
        vs_int32_t chn_id, const vs_vii_extchn_attr_s *p_chn_attr)
{
    vs_uint32_t ret;

    ret = vs_mal_vii_extchn_attr_set(pipe_id, chn_id, p_chn_attr);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vii pipe %d chn %d set chn attr failed, ret: 0x%x.\n", pipe_id, chn_id, ret);
        return ret;
    }

    ret = vs_mal_vii_chn_enable(pipe_id, chn_id);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vii pipe %d chn %d enable failed, ret: 0x%x.\n", pipe_id, chn_id, ret);
        return ret;
    }

    return VS_SUCCESS;
}

static vs_int32_t sample_common_vii_ext_chn_disable(
        vs_int32_t pipe_id, vs_int32_t chn_id)
{
    vs_uint32_t ret;

    ret = vs_mal_vii_chn_disable(pipe_id, chn_id);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vii pipe %d chn %d disable failed, ret: 0x%x.\n", pipe_id, chn_id, ret);
        return ret;
    }

    return VS_SUCCESS;
}

#ifndef VS_NO_ISP
typedef vs_int32_t (*img_sensor_isp_ops_get)(vs_isp_img_sensor_ops_s **p_sensor_ops);
typedef vs_int32_t (*img_sensor_user_ops_get)(vs_img_sensor_user_ops_s **p_user_ops);

static vs_int32_t sample_common_isp_sensor_register(vs_int32_t vii_pipe,
        vs_char_t *sensor_name, vs_int8_t bus_id)
{
    char sensor_lib_name[128];
    sprintf(sensor_lib_name, "libsnsr%s.so", sensor_name);
    vs_sample_trace("debug-sensorname: %s\n", sensor_name);

    vs_int32_t rc;
    s_sensor_lib_handle[vii_pipe] = dlopen(sensor_lib_name, RTLD_LAZY);
    if (!s_sensor_lib_handle[vii_pipe]) {
        vs_sample_trace(" open %s library fail.\n", sensor_lib_name);
        return VS_FAILED;
    }
    img_sensor_isp_ops_get sensor_isp_ops_get;
    img_sensor_user_ops_get sensor_user_ops_get;
    vs_isp_img_sensor_ops_s *p_sensor_isp_ops;
    vs_img_sensor_user_ops_s *p_sensor_user_ops;

    sensor_isp_ops_get = dlsym(s_sensor_lib_handle[vii_pipe], "vs_img_sensor_isp_ops_get");
    if (sensor_isp_ops_get == NULL) {
        vs_sample_trace("get vs_img_sensor_isp_ops_get handle error\n");
        goto exit0;
    }
    rc = sensor_isp_ops_get(&p_sensor_isp_ops);
    if (rc != VS_SUCCESS) {
        vs_sample_trace("sensor lib get ops error\n");
        goto exit0;
    }

    rc = vs_mal_isp_img_sensor_register(vii_pipe, p_sensor_isp_ops);
    if (rc != VS_SUCCESS) {
        vs_sample_trace("vs_mal_isp_sensor_register error, ret: 0x%x.\n", rc);
        goto exit0;
    }

    /* set bus id */
    sensor_user_ops_get = dlsym(s_sensor_lib_handle[vii_pipe], "vs_img_sensor_user_ops_get");
    if (sensor_user_ops_get == NULL) {
        vs_sample_trace("get vs_img_sensor_user_ops_get handle error\n");
        goto exit0;
    }
    rc = sensor_user_ops_get(&p_sensor_user_ops);
    if (rc != VS_SUCCESS) {
        vs_sample_trace("sensor lib get ops error\n");
        goto exit0;
    }
    vs_isp_img_sensor_bus_attr_u bus_attr;
    bus_attr.i2c_dev = bus_id;
    if (p_sensor_user_ops == NULL) {
        vs_sample_trace("p_sensor_user_ops is null\n");
        goto exit0;
    }
    rc = p_sensor_user_ops->bus_attr_set(vii_pipe, bus_attr);
    if (rc != VS_SUCCESS) {
        vs_sample_trace("bus_attr_set error\n");
        goto exit0;
    }
    return VS_SUCCESS;
    vs_sample_trace("sensor register success\n");

exit0:
    dlclose(s_sensor_lib_handle[vii_pipe]);
    s_sensor_lib_handle[vii_pipe] = VS_NULL;
    return VS_FAILED;
}

typedef vs_int32_t (*aelib_ops_get)(vs_isp_aelib_ops_s **p_ae_ops);
typedef vs_int32_t (*awblib_ops_get)(vs_isp_awblib_ops_s **p_awb_ops);

static vs_int32_t sample_common_isp_aelib_register(vs_int32_t vii_pipe)
{
    vs_int32_t rc;
    s_aelib_handle[vii_pipe] = dlopen("libae.so", RTLD_LAZY);
    if (!s_aelib_handle[vii_pipe]) {
        vs_sample_trace(" open libae library fail.\n");
        return VS_FAILED;
    }
    aelib_ops_get isp_aelib_ops_get;
    isp_aelib_ops_get = dlsym(s_aelib_handle[vii_pipe], "vs_mal_ae_ops_get");
    if (isp_aelib_ops_get == VS_NULL) {
        vs_sample_trace("get vs_mal_ae_ops_get handle error\n");
        goto exit0;
    }

    vs_isp_alg_lib_s aelib = {0, "vs_aelib"};
    aelib.id = vii_pipe;

    vs_isp_aelib_ops_s *p_vs_ae_ops = VS_NULL;
    rc = isp_aelib_ops_get(&p_vs_ae_ops);
    if (rc != VS_SUCCESS) {
        printf("aelib_ops_get error\n");
        goto exit0;
    }
    rc = vs_mal_isp_aelib_register(vii_pipe, &aelib, p_vs_ae_ops);
    if (rc != VS_SUCCESS) {
        printf("vs_mal_isp_aelib_register error, ret: 0x%x.\n", rc);
        goto exit0;
    }
    return VS_SUCCESS;
exit0:
    dlclose(s_aelib_handle[vii_pipe]);
    s_aelib_handle[vii_pipe] = VS_NULL;
    return VS_FAILED;
}

static vs_int32_t sample_common_isp_awblib_register(vs_int32_t vii_pipe)
{
    vs_int32_t rc;
    s_awblib_handle[vii_pipe] = dlopen("libawb.so", RTLD_LAZY);
    if (!s_awblib_handle[vii_pipe]) {
        vs_sample_trace(" open libawb library fail.\n");
        return VS_FAILED;
    }
    awblib_ops_get isp_awblib_ops_get;
    isp_awblib_ops_get = dlsym(s_awblib_handle[vii_pipe], "vs_mal_awb_ops_get");
    if (isp_awblib_ops_get == VS_NULL) {
        vs_sample_trace("get vs_mal_awb_ops_get handle error\n");
        goto exit0;
    }

    vs_isp_alg_lib_s awblib = {0, "vs_awblib"};
    awblib.id = vii_pipe;

    vs_isp_awblib_ops_s *p_vs_awb_ops = VS_NULL;
    rc = isp_awblib_ops_get(&p_vs_awb_ops);
    if (rc != VS_SUCCESS) {
        printf("awblib_ops_get error\n");
        goto exit0;
    }
    rc = vs_mal_isp_awblib_register(vii_pipe, &awblib, p_vs_awb_ops);
    if (rc != VS_SUCCESS) {
        printf("vs_mal_isp_awblib_register error, ret: 0x%x.\n", rc);
        goto exit0;
    }
    return VS_SUCCESS;
exit0:
    dlclose(s_awblib_handle[vii_pipe]);
    s_awblib_handle[vii_pipe] = VS_NULL;
    return VS_FAILED;
}

static vs_void_t *isp_run_func(vs_void_t* arg)
{
    vs_int32_t pipe_id = (vs_int32_t)(intptr_t)arg;
    vs_sample_trace("isp start to run pipe : %d\n", pipe_id);
    vs_mal_isp_run(pipe_id);
    return 0;
}

static vs_int32_t sample_common_isp_run(vs_int32_t vii_pipe)
{
    vs_int32_t ret = pthread_create(&s_isp_run_thread[vii_pipe], NULL,
                        isp_run_func, (vs_void_t*)(intptr_t)vii_pipe);
    if (ret != 0) {
        vs_sample_trace("create thread err\n");
        return -1;
    }
    vs_sample_trace("pipe %d isp is running.....\n", vii_pipe);
    return 0;
}

static vs_int32_t sample_common_isp_start(vs_int32_t vii_pipe,
        vs_char_t *sensor_name, vs_int8_t bus_id, vs_isp_comm_attr_s *isp_attr)
{
    int rc;
    vs_isp_nr3d_attr_s nr3d_attr;

    rc = vs_mal_isp_mem_init(vii_pipe);
    if (rc != VS_SUCCESS) {
        vs_sample_trace("vs_mal_isp_mem_init error, ret: 0x%x.\n", rc);
        return rc;
    }

    if (strlen((char*)sensor_name) != 0) {
        rc = sample_common_isp_sensor_register(vii_pipe, sensor_name, bus_id);
        if (rc != VS_SUCCESS) {
            vs_sample_trace("sample_common_isp_sensor_register error\n");
            goto exit0;
        }
    }

    rc = vs_mal_isp_comm_attr_set(vii_pipe, isp_attr);
    if (rc != VS_SUCCESS) {
        vs_sample_trace("vs_mal_isp_comm_attr_set error, ret: 0x%x.\n", rc);
        goto exit0;
    }

    if (strlen((char*)sensor_name) != 0) {
        rc = sample_common_isp_aelib_register(vii_pipe);
        if (rc != VS_SUCCESS) {
            printf("ae lib register  error\n");
        }

        rc = sample_common_isp_awblib_register(vii_pipe);
        if (rc != VS_SUCCESS) {
            printf("awb lib register  error\n");
        }

        vs_isp_bind_attr_s bind_attr;
        bind_attr.ae_lib.id = vii_pipe;
        bind_attr.awb_lib.id = vii_pipe;
        rc = vs_mal_isp_bind_attr_set(vii_pipe, &bind_attr);
        if (rc != VS_SUCCESS) {
            printf("vs_mal_isp_bind_attr_set  error, ret: 0x%x.\n", rc);
        }
    }

    rc = vs_mal_isp_init(vii_pipe);
    if (rc != VS_SUCCESS) {
        vs_sample_trace("vs_mal_isp_init error, ret: 0x%x.\n", rc);
        goto exit0;
    }

    if (vs_mal_isp_nr3d_attr_get(vii_pipe, &nr3d_attr) == VS_SUCCESS) {
        nr3d_attr.enable = g_nr_3d;
        vs_sample_trace("nr3d enable(%d) pipe: %d\n", g_nr_3d, vii_pipe);
        vs_mal_isp_nr3d_attr_set(vii_pipe, &nr3d_attr);
    }

    rc = sample_common_isp_run(vii_pipe);
    if (rc != VS_SUCCESS) {
        vs_sample_trace("isp_run error, ret: 0x%x.\n", rc);
        goto exit0;
    }

    return VS_SUCCESS;

exit0:
    vs_mal_isp_deinit(vii_pipe);
    return VS_FAILED;
}

static vs_int32_t sample_common_isp_stop(int vii_pipe)
{
    int rc;
    rc = vs_mal_isp_stop(vii_pipe);
    if (rc != VS_SUCCESS) {
        vs_sample_trace("vs_mal_isp_stop error, ret: 0x%x.\n", rc);
    }

    void* thread_result;
    pthread_join(s_isp_run_thread[vii_pipe], &thread_result);

    rc = vs_mal_isp_deinit(vii_pipe);
    if (rc != VS_SUCCESS) {
        vs_sample_trace("vs_mal_isp_deinit error, ret: 0x%x.\n", rc);
    }

    if (s_sensor_lib_handle[vii_pipe] != VS_NULL) {
        dlclose(s_sensor_lib_handle[vii_pipe]);
        s_sensor_lib_handle[vii_pipe] = VS_NULL;
    }

    if (s_aelib_handle[vii_pipe] != VS_NULL) {
        dlclose(s_aelib_handle[vii_pipe]);
        s_aelib_handle[vii_pipe] = VS_NULL;
    }

    if (s_awblib_handle[vii_pipe] != VS_NULL) {
        dlclose(s_awblib_handle[vii_pipe]);
        s_awblib_handle[vii_pipe] = VS_NULL;
    }

    vs_sample_trace("pipe %d isp exit\n", vii_pipe);
    return 0;
}
#endif
vs_int32_t sample_common_vii_pipe_start(vs_char_t *sensor_name,
        sample_vii_pipe_cfg_s *pipe_cfg)
{
    vs_int32_t i, j;
    vs_uint32_t ret;
    vs_vii_vpp_online_offline_attr_s online_offline_attr;

    ret = vs_mal_vii_pipe_create(pipe_cfg->pipe_id, &pipe_cfg->pipe_attr);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vii pipe %d create failed, ret: 0x%x.\n", pipe_cfg->pipe_id, ret);
        goto exit0;
    }

    ret = vs_mal_vii_vpp_mode_get(&online_offline_attr);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_vii_vpp_mode_get failed, ret: 0x%x.\n", ret);
        goto exit0;
    }

	if ((online_offline_attr.mode[pipe_cfg->pipe_id] == E_VII_ONLINE_VPP_ONLINE ||
        online_offline_attr.mode[pipe_cfg->pipe_id] == E_VII_ONLINE_VPP_OFFLINE) &&
        (pipe_cfg->pipe_param.mode == E_VII_PIPE_ADVANCE_MODE_SUB_FRAME_LAST ||
        pipe_cfg->pipe_param.mode == E_VII_PIPE_ADVANCE_MODE_SUB_FRAME_FIRST)) {
        pipe_cfg->pipe_param.mode = E_VII_PIPE_ADVANCE_MODE_NONE;
    }

    ret = vs_mal_vii_pipe_param_set(pipe_cfg->pipe_id, &pipe_cfg->pipe_param);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("failed to set vii pipe %d param, ret: 0x%x.\n", pipe_cfg->pipe_id, ret);
        goto exit0;
    }

    if (pipe_cfg->fpn_attr.enable == VS_TRUE) {
        ret = vs_mal_vii_pipe_fpn_attr_set(pipe_cfg->pipe_id, &pipe_cfg->fpn_attr);
        if (ret != VS_SUCCESS) {
            vs_sample_trace("pipe %d vs_mal_vii_pipe_fpn_attr_set failed, ret: 0x%x.\n", pipe_cfg->pipe_id, ret);
            goto exit0;
        }
    }

    ret = vs_mal_vii_pipe_start(pipe_cfg->pipe_id);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vii pipe %d start failed, ret: 0x%x.\n", pipe_cfg->pipe_id, ret);
        goto exit1;
    }

    for (i = 0; i < pipe_cfg->phys_chn_num; i++) {
        sample_vii_phys_chn_cfg_s *chn_cfg = &pipe_cfg->phys_chn_cfg[i];
        ret = sample_common_vii_phys_chn_enable(pipe_cfg->pipe_id,
                chn_cfg->chn_id, chn_cfg);
        if (ret != VS_SUCCESS) {
            goto exit2;
        }
    }

    for (j = 0; j < pipe_cfg->ext_chn_num; j++) {
        sample_vii_ext_chn_cfg_s *chn_cfg = &pipe_cfg->ext_chn_cfg[j];
        ret = sample_common_vii_ext_chn_enable(pipe_cfg->pipe_id,
                chn_cfg->chn_id, &chn_cfg->chn_attr);
        if (ret != VS_SUCCESS) {
            goto exit3;
        }
    }
#ifndef VS_NO_ISP
    if (pipe_cfg->pipe_attr.bypass_mode != E_VII_PIPE_BYPASS_ALL &&
        pipe_cfg->pipe_attr.bypass_mode != E_VII_PIPE_BYPASS_FE &&
        pipe_cfg->firmware_bypass != VS_TRUE) {
        ret = sample_common_isp_start(pipe_cfg->pipe_id, sensor_name,
                pipe_cfg->bus_id, &pipe_cfg->isp_attr);
        if (ret != VS_SUCCESS) {
            goto exit3;
        }
    } else if (pipe_cfg->pipe_attr.bypass_mode == E_VII_PIPE_BYPASS_FE) {
        ret = vs_mal_isp_mem_init(pipe_cfg->pipe_id);
        if (ret != VS_SUCCESS) {
            vs_sample_trace("vs_mal_isp_mem_init error, ret: 0x%x.\n", ret);
            goto exit3;
        }
    }
#endif
    return VS_SUCCESS;

exit3:
    for (j = j - 1; j >= 0; j--) {
        sample_vii_ext_chn_cfg_s *chn_cfg = &pipe_cfg->ext_chn_cfg[j];
        sample_common_vii_ext_chn_disable(pipe_cfg->pipe_id, chn_cfg->chn_id);
    }
exit2:
    for (i = i - 1; i >= 0; i--) {
        sample_vii_phys_chn_cfg_s *chn_cfg = &pipe_cfg->phys_chn_cfg[i];
        sample_common_vii_phys_chn_disable(pipe_cfg->pipe_id, chn_cfg->chn_id);
    }
exit1:
    vs_mal_vii_pipe_destroy(pipe_cfg->pipe_id);
exit0:
    return ret;
}

vs_void_t sample_common_vii_pipe_stop(sample_vii_pipe_cfg_s *pipe_cfg)
{
    vs_int32_t i, j;
    vs_uint32_t ret;
#ifndef VS_NO_ISP
    if (pipe_cfg->pipe_attr.bypass_mode != E_VII_PIPE_BYPASS_ALL &&
        pipe_cfg->pipe_attr.bypass_mode != E_VII_PIPE_BYPASS_FE &&
        pipe_cfg->firmware_bypass != VS_TRUE) {
        sample_common_isp_stop(pipe_cfg->pipe_id);
    } else if (pipe_cfg->pipe_attr.bypass_mode == E_VII_PIPE_BYPASS_FE) {
        ret = vs_mal_isp_deinit(pipe_cfg->pipe_id);
        if (ret != VS_SUCCESS) {
            vs_sample_trace("vs_mal_isp_deinit error, ret: 0x%x.\n", ret);
        }
    }
#endif
    for (j = pipe_cfg->ext_chn_num - 1; j >= 0; j--) {
        sample_vii_ext_chn_cfg_s *chn_cfg = &pipe_cfg->ext_chn_cfg[j];
        sample_common_vii_ext_chn_disable(pipe_cfg->pipe_id, chn_cfg->chn_id);
    }

    for (i = pipe_cfg->phys_chn_num - 1; i >= 0; i--) {
        sample_vii_phys_chn_cfg_s *chn_cfg = &pipe_cfg->phys_chn_cfg[i];
        sample_common_vii_phys_chn_disable(pipe_cfg->pipe_id, chn_cfg->chn_id);
    }

    ret = vs_mal_vii_pipe_stop(pipe_cfg->pipe_id);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vii pipe %d stop failed, ret: 0x%x.\n", pipe_cfg->pipe_id, ret);
    }

    ret = vs_mal_vii_pipe_destroy(pipe_cfg->pipe_id);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vii pipe %d destroy failed, ret: 0x%x.\n", pipe_cfg->pipe_id, ret);
    }
}

static vs_int32_t sample_common_vii_route_start(sample_vii_route_cfg_s *route_cfg)
{
    vs_int32_t i;
    vs_int32_t ret;

    ret = vs_mal_mipi_rx_dev_attr_set(route_cfg->dev_id, &(route_cfg->mipi_rx_attr));
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vii set mipi rx %d attr failed, ret: 0x%x.\n", route_cfg->dev_id, ret);
        goto exit0;
    }

    if (route_cfg->pipe_num > 1) {
        ret = sample_common_vii_dev_enable(route_cfg->dev_id,
                &route_cfg->dev_attr, &route_cfg->vc_attr);
    } else {
        ret = sample_common_vii_dev_enable(route_cfg->dev_id,
                &route_cfg->dev_attr, VS_NULL);
    }
    if (ret != VS_SUCCESS) {
        goto exit0;
    }

    if (route_cfg->dev_enable_cb) {
        route_cfg->dev_enable_cb(route_cfg->dev_en_cb_data);
    }

    ret = sample_common_vii_dev_pipe_bind(route_cfg->dev_id,
            route_cfg->pipe_id, route_cfg->pipe_num);
    if (ret != VS_SUCCESS) {
        goto exit1;
    }

    for (i = 0; i < route_cfg->pipe_num; i++) {
        ret = sample_common_vii_pipe_start(route_cfg->sensor_name,
                &route_cfg->pipe_cfg[i]);
        if (ret != VS_SUCCESS) {
            goto exit2;
        }
    }

    return VS_SUCCESS;

exit2:
    for (i = i - 1; i >= 0; i--) {
        sample_common_vii_pipe_stop(&route_cfg->pipe_cfg[i]);
    }
    sample_common_vii_dev_pipe_unbind(route_cfg->dev_id,
            route_cfg->pipe_id, route_cfg->pipe_num);
exit1:
    sample_common_vii_dev_disable(route_cfg->dev_id);
exit0:
    return ret;
}

static vs_void_t sample_common_vii_route_stop(sample_vii_route_cfg_s *route_cfg)
{
    vs_int32_t i;

    for (i = route_cfg->pipe_num - 1; i >= 0; i--) {
        sample_common_vii_pipe_stop(&route_cfg->pipe_cfg[i]);
    }
    sample_common_vii_dev_pipe_unbind(route_cfg->dev_id,
            route_cfg->pipe_id, route_cfg->pipe_num);
    sample_common_vii_dev_disable(route_cfg->dev_id);
}



/*******************************************************
                  vii function definition
*******************************************************/

vs_int32_t sample_common_vii_sensor_type_num_get()
{
    return SAMPLE_SENSOR_TYPE_MAX;
}

const char *sample_common_sensor_type_name_get(sample_sensor_type_e sensor_type)
{
    vs_int32_t i;

    for (i = 0; i < sizeof(s_sensor_cfg)/sizeof(sample_sensor_cfg_s); i++) {
        if (s_sensor_cfg[i].sensor_type == sensor_type) {
            return s_sensor_cfg[i].sensor_type_name;
        }
    }

    return "NA";
}

vs_uint32_t sample_common_vii_wdr_frame_num_get(vs_int32_t sensor_id)
{
    sample_sensor_type_e sensor_type = default_sensor_type_get(sensor_id);

    return sample_common_vii_wdr_frame_num_get_by_sensor(sensor_type);
}

vs_void_t sample_common_vii_sensor_img_size_get(
        vs_int32_t sensor_id, vs_size_s *img_size)
{
    sample_sensor_type_e sensor_type = default_sensor_type_get(sensor_id);
    sample_common_vii_img_size_get(sensor_type, img_size);
}

vs_void_t sample_common_vii_sensor_pixel_format_get(
        vs_int32_t sensor_id, vs_pixel_format_e *format)
{
    sample_sensor_type_e sensor_type = default_sensor_type_get(sensor_id);
    vs_int32_t i;

    for (i = 0; i < sizeof(s_sensor_cfg)/sizeof(sample_sensor_cfg_s); i++) {
        if (s_sensor_cfg[i].sensor_type == sensor_type) {
            *format = s_sensor_cfg[i].pixel_format;
            return;
        }
    }

    *format = E_PIXEL_FORMAT_BAYER_16BPP;
}

vs_int32_t sample_common_vii_sensor_framerate_get(
        vs_int32_t sensor_id, vs_int32_t *p_framerate)
{
    sample_sensor_type_e sensor_type = default_sensor_type_get(sensor_id);
    return sample_common_vii_framerate_get(sensor_type, p_framerate);
}

vs_void_t sample_common_vii_default_cfg_get(
        vs_int32_t sensor_id, sample_vii_route_cfg_s *route_cfg)
{
    vs_uint32_t i;

    sample_sensor_type_e sensor_type = default_sensor_type_get(sensor_id);

    sample_common_vii_sensor_name_get(sensor_type, (char*)route_cfg->sensor_name);
    route_cfg->lane_mode = sample_common_vii_mipirx_lane_mode_get(sensor_type);
    sample_common_vii_mipirx_attr_get(sensor_type, &route_cfg->mipi_rx_attr);

    route_cfg->dev_id = sensor_id;
    sample_common_vii_dev_attr_get(sensor_type, &route_cfg->dev_attr);
    sample_common_vii_vc_attr_get(sensor_type, &route_cfg->vc_attr);

    route_cfg->pipe_num = 1;
    route_cfg->pipe_id[0] = route_cfg->dev_id;

    for (i = 0; i < route_cfg->pipe_num; i++) {
        sample_common_vii_pipe_cfg_get(sensor_type, &route_cfg->pipe_cfg[i]);
        route_cfg->pipe_cfg[i].pipe_id = route_cfg->pipe_id[i];
    }

    route_cfg->dev_enable_cb = VS_NULL;
}

vs_void_t sample_common_vii_virt_pipe_cfg_get(vs_int32_t sensor_id, sample_vii_pipe_cfg_s *pipe_cfg)
{
    sample_sensor_type_e sensor_type = default_sensor_type_get(sensor_id);

    sample_common_vii_pipe_cfg_get(sensor_type, pipe_cfg);
}

vs_int32_t sample_common_vii_virt_pipe_start(vs_int32_t sensor_id, sample_vii_pipe_cfg_s *pipe_cfg)
{
    char sensor_name[SENSOR_MAX_NAME_LEN];
    sample_sensor_type_e sensor_type = default_sensor_type_get(sensor_id);

    sample_common_vii_sensor_name_get(sensor_type, sensor_name);

    return sample_common_vii_pipe_start((vs_char_t *)sensor_name, pipe_cfg);
}

vs_void_t sample_common_vii_virt_pipe_stop(sample_vii_pipe_cfg_s *pipe_cfg)
{
    sample_common_vii_pipe_stop(pipe_cfg);
}

vs_int32_t sample_common_vii_start(sample_vii_cfg_s *vii_cfg)
{
    vs_int32_t i;
    vs_int32_t ret;
    vs_int32_t dev_id;
    sample_sensor_type_e sensor_type;
    vs_uint32_t delay_ms = 0, delay_max_ms = 0;
	vs_vii_vpp_online_offline_attr_s online_offline_attr;

    for (i = 0; i < vii_cfg->route_num; i++) {
        dev_id = vii_cfg->route_cfg[i].dev_id;
        sensor_type = default_sensor_type_get(dev_id);
        ret = sample_common_vii_sensor_type_support(sensor_type);
        if (ret == VS_FALSE) {
            vs_sample_trace("sensor type %d exceeds the capability of platform!\n", sensor_type);
            ret = VS_FAILED;
            goto exit0;
        }

        sample_common_vii_delay_get(sensor_type, &delay_ms);
        delay_max_ms = (delay_ms > delay_max_ms) ? delay_ms : delay_max_ms;
    }
	ret = vs_mal_vii_vpp_mode_get(&online_offline_attr);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_vii_vpp_mode_get failed, ret: 0x%x.\n", ret);
        goto exit0;
    }
	if (online_offline_attr.mode[0] != E_VII_OFFLINE_VPP_OFFLINE) {
        ret = sample_common_vi_vpp_mode_set(vii_cfg->vii_vpp_mode);
        if (ret != VS_SUCCESS) {
            vs_sample_trace("set vii vpp mode (%d) failed, ret: 0x%x.\n", vii_cfg->vii_vpp_mode, ret);
            goto exit0;
        }
    }

    ret = vs_mal_mipi_rx_lane_mode_set(vii_cfg->route_cfg[0].lane_mode);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vii set mipirx lane mode %d failed, ret: 0x%x.\n",
                vii_cfg->route_cfg[0].lane_mode, ret);
        goto exit0;
    }

    for (i = 0; i < vii_cfg->route_num; i++) {
        dev_id = vii_cfg->route_cfg[i].dev_id;
        vii_cfg->route_cfg[i].pipe_cfg[0].bus_id = g_bus_id[dev_id];

        ret = sample_common_vii_route_start(&vii_cfg->route_cfg[i]);
        if (ret != VS_SUCCESS) {
            vs_sample_trace("start route %d failed!\n", i);
            goto exit1;
        }
    }

    /* Add delay to ensure stable sensor output */
    if (delay_max_ms) {
        usleep(delay_max_ms * 1000);
    }

    return VS_SUCCESS;

exit1:
    for (i = i - 1; i >= 0; i--) {
        sample_common_vii_route_stop(&vii_cfg->route_cfg[i]);
    }
exit0:
    return ret;
}

vs_void_t sample_common_vii_stop(sample_vii_cfg_s *vii_cfg)
{
    vs_int32_t i;

    for (i = vii_cfg->route_num - 1; i >= 0; i--) {
        sample_common_vii_route_stop(&vii_cfg->route_cfg[i]);
    }
}

#ifndef VS_NO_ISP
vs_int32_t sample_common_vii_mirror_mode_set(
        vs_int32_t pipe_id, vs_mirror_mode_e mode)
{
    img_sensor_user_ops_get sensor_user_ops_get;
    vs_img_sensor_user_ops_s *p_sensor_user_ops;
    vs_int32_t rc;
    if (mode == E_MIRROR_MODE_NONE) {
        return VS_SUCCESS;
    }
    if (s_sensor_lib_handle[pipe_id] == VS_NULL) {
        vs_sample_trace("failed to get sensor lib handle\n");
        return VS_FAILED;
    }
    sensor_user_ops_get = dlsym(s_sensor_lib_handle[pipe_id], "vs_img_sensor_user_ops_get");
    if (sensor_user_ops_get == NULL) {
        vs_sample_trace("get vs_img_sensor_user_ops_get handle error\n");
        return VS_FAILED;
    }
    rc = sensor_user_ops_get(&p_sensor_user_ops);
    if (rc != VS_SUCCESS || p_sensor_user_ops == NULL) {
        vs_sample_trace("sensor lib get ops error\n");
        return VS_FAILED;
    }
    if (p_sensor_user_ops->mirror_flip(pipe_id, mode) != VS_SUCCESS) {
        vs_sample_trace("sensor mirror mode failed\n");
        return VS_FAILED;
    }
    vs_sample_trace("sensor mirror mode succeed\n");
    return VS_SUCCESS;
}
#endif

vs_uint32_t sample_common_vii_pipe_frame_stride_get(vs_pixel_format_e pixel_format, vs_uint32_t width, vs_compress_mode_e cmp_mode)
{
    vs_uint32_t bit_width;
    vs_uint32_t stride;
    vs_uint32_t line_align;

    if ((pixel_format == E_PIXEL_FORMAT_BAYER_8BPP) ||
        (pixel_format == E_PIXEL_FORMAT_YVU_422SP)  ||
        (pixel_format == E_PIXEL_FORMAT_YUV_422SP)  ||
        (pixel_format == E_PIXEL_FORMAT_YUV_420SP)  ||
        (pixel_format == E_PIXEL_FORMAT_YVU_420SP)) {
        bit_width = 8;
    } else {
        bit_width = 16;
    }

    // compress 128 align
    // non-compress 16 align
    if (cmp_mode == E_COMPRESS_MODE_NONE) {
        line_align = 64;
    } else {
        line_align = 256;
    }

    stride = ALIGN_UP((width * bit_width / 8), line_align);

    return stride;
}

static vs_int32_t sample_common_vii_get_fpn_frm_vb_blk(vs_size_s *img_size, vs_pixel_format_e pixel_format, sample_fpn_frame_info_s *fpn_frame_info)
{
    VB_BLK vb_blk;
    vs_int32_t vb_pool;
    vs_vb_pool_cfg_s pool_cfg = {0};
    vs_uint32_t stride;
    vs_uint32_t cmp_buf_size, img_buf_size;
    vs_uint32_t buf_size;
    vs_uint32_t y_size, uv_size;
    vs_uint32_t y_cmp_size = 0;
    vs_uint64_t phys_addr;
    vs_void_t *virt_addr;
    vs_video_frame_info_s *frame_info;

    stride = sample_common_vii_pipe_frame_stride_get(pixel_format, img_size->width, E_COMPRESS_MODE_NONE);
    y_size  = stride * img_size->height;
    uv_size = 0;

    img_buf_size = y_size + uv_size;
    cmp_buf_size = 0;
    buf_size = cmp_buf_size + img_buf_size;

    pool_cfg.blk_cnt    = 1;
    pool_cfg.blk_size   = buf_size;
    pool_cfg.remap_mode = VB_REMAP_MODE_NONE;
    vb_pool = vs_mal_vb_pool_create(&pool_cfg);
    if (vb_pool == VS_INVALID_POOLID) {
        printf("vii create vb pool failed.\n");
        return VS_FAILED;
    }

    frame_info = &fpn_frame_info->frame_info;
    vb_blk = vs_mal_vb_block_get(vb_pool, buf_size, NULL);
    if (vb_blk == VS_INVALID_VB_HANDLE) {
        printf("vii get vb blk failed.\n");
        return VS_FAILED;
    }

    phys_addr = vs_mal_vb_handle2physaddr(vb_blk);
    virt_addr = vs_mal_sys_mmap(phys_addr, buf_size);

    fpn_frame_info->vb_blk = vb_blk;
    fpn_frame_info->blk_size = buf_size;

    frame_info->modid = E_MOD_ID_VII;
    frame_info->poolid = vb_pool;
    frame_info->frame.width = img_size->width;
    frame_info->frame.height = img_size->height;
    frame_info->frame.pixel_format = pixel_format;
    frame_info->frame.video_format = E_VIDEO_FORMAT_LINEAR;
    frame_info->frame.compress_mode = E_COMPRESS_MODE_NONE;
    frame_info->frame.dynamic_range = E_DYNAMIC_RANGE_SDR8;
    frame_info->frame.color_gamut = E_COLOR_GAMUT_BT709;
    frame_info->frame.compress_header_stride[0] = 0;
    frame_info->frame.compress_header_stride[1] = 0;
    frame_info->frame.compress_header_stride[2] = 0;
    frame_info->frame.stride[0] = stride;
    frame_info->frame.stride[1] = stride;
    frame_info->frame.stride[2] = stride;
    frame_info->frame.compress_header_phys_addr[0] = phys_addr;
    frame_info->frame.compress_header_phys_addr[1] = phys_addr + y_cmp_size;
    frame_info->frame.compress_header_phys_addr[2] = 0;
    frame_info->frame.compress_header_virt_addr[0] = (vs_uint64_t)virt_addr;
    frame_info->frame.compress_header_virt_addr[1] = (vs_uint64_t)(virt_addr + y_cmp_size);
    frame_info->frame.compress_header_virt_addr[2] = 0;
    frame_info->frame.phys_addr[0] = phys_addr + cmp_buf_size;
    frame_info->frame.phys_addr[1] = phys_addr + cmp_buf_size + y_size;
    frame_info->frame.phys_addr[2] = 0;
    frame_info->frame.virt_addr[0] = (vs_uint64_t)(virt_addr + cmp_buf_size);
    frame_info->frame.virt_addr[1] = (vs_uint64_t)(virt_addr + cmp_buf_size + y_size);
    frame_info->frame.virt_addr[2] = 0;

    return VS_SUCCESS;
}

vs_void_t sample_common_vii_pipe_release_fpn_frm_vb_blk(sample_fpn_frame_info_s *fpn_frame_info)
{
    vs_int32_t vb_pool;
    vs_void_t *virt_addr;

    virt_addr = (vs_void_t *)fpn_frame_info->frame_info.frame.compress_header_virt_addr[0];
    vs_mal_sys_unmap(virt_addr, fpn_frame_info->blk_size);
    vs_mal_vb_block_release(fpn_frame_info->vb_blk);

    vb_pool = fpn_frame_info->frame_info.poolid;
    vs_mal_vb_pool_destory(vb_pool);
}

vs_void_t sample_common_vii_read_raw(FILE *pfd, vs_video_frame_s *p_frame)
{
    vs_int32_t i;
    vs_uint8_t *virt_addr;
    vs_uint32_t bit_width;

    if (p_frame->pixel_format == E_PIXEL_FORMAT_BAYER_8BPP) {
        bit_width = 8;
    } else {
        bit_width = 16;
    }

    virt_addr = (vs_uint8_t *)p_frame->compress_header_virt_addr[0];

    for (i = 0; i < p_frame->height; i++) {
        fread(virt_addr, p_frame->width, bit_width / 8, pfd);
        virt_addr += p_frame->stride[0];
    }
}

static vs_void_t vii_pipe_read_fpn_frame_file(const char *file_name, sample_fpn_frame_info_s *fpn_frame)
{
    FILE *pfd = NULL;

    // open file
    pfd = fopen(file_name, "rb");
    if (pfd == NULL) {
        printf("open file %s failed!\n", file_name);
        return;
    }

    sample_common_vii_read_raw(pfd, &fpn_frame->frame_info.frame);

    // close fd
    fclose(pfd);
}

vs_void_t sample_common_get_fpn_cfg(sample_vii_pipe_cfg_s *pipe_cfg, sample_fpn_frame_info_s *fpn_info)
{
    vs_pixel_format_e pixel_format = E_PIXEL_FORMAT_BAYER_8BPP;
    char *file_name = "./vii_pipe_3840x2160_fpn.raw";

    sample_common_vii_get_fpn_frm_vb_blk(&pipe_cfg->pipe_attr.image_size,
        pixel_format, fpn_info);

    vii_pipe_read_fpn_frame_file(file_name, fpn_info);

    pipe_cfg->fpn_attr.enable = VS_TRUE;
    pipe_cfg->fpn_attr.op_type = E_VII_OP_TYPE_MANUAL;
    pipe_cfg->fpn_attr.iso = 2000;
    pipe_cfg->fpn_attr.offset = 0;
    if (pipe_cfg->fpn_attr.op_type == E_VII_OP_TYPE_AUTO) {
        vs_uint32_t i;
        for (i = 0; i < VS_VII_FPN_AUTO_STRENGTH_SIZE; i++) {
            pipe_cfg->fpn_attr.auto_attr.strength[i] = 256 * i + 128;
        }
    } else {
        pipe_cfg->fpn_attr.manual_attr.strength = 0;
    }
    memcpy(&pipe_cfg->fpn_attr.fpn_frame, &fpn_info->frame_info, sizeof(vs_video_frame_info_s));
}
