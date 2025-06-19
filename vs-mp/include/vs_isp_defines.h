/**
 * @file vs_isp_defines.h
 * @brief Declaration of isp enumeration and struct
 * @details Declaration of isp enumeration and struct
 * @author VS isp group
 * @date 2022-1-29
 * @version v0.10
 * @Copyright (C) 2022, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
 */


#ifndef __VS_ISP_DEFINES_H__
#define __VS_ISP_DEFINES_H__

#include "vs_type.h"
#include "vs_comm.h"
#include "vs_comm_video.h"
#include "vs_errno.h"
#include "vs_capability.h"

#ifdef __cplusplus
extern "C"{
#endif

#define VS_ERR_ISP_INVALID_PARAM                 VS_ERR_CODE(E_MOD_ID_ISPFE, E_ERR_INVALID_PARAM)
#define VS_ERR_ISP_INVALID_PIPEID                VS_ERR_CODE(E_MOD_ID_ISPFE, E_ERR_INVALID_PIPEID)
#define VS_ERR_ISP_INVALID_LEN                   VS_ERR_CODE(E_MOD_ID_ISPFE, E_ERR_INVALID_LEN)
#define VS_ERR_ISP_BAD_STATE                     VS_ERR_CODE(E_MOD_ID_ISPFE, E_ERR_BAD_STATE)
#define VS_ERR_ISP_NULL_PTR                      VS_ERR_CODE(E_MOD_ID_ISPFE, E_ERR_NULL_PTR)
#define VS_ERR_ISP_EXIST                         VS_ERR_CODE(E_MOD_ID_ISPFE, E_ERR_EXIST)
#define VS_ERR_ISP_UNEXIST                       VS_ERR_CODE(E_MOD_ID_ISPFE, E_ERR_UNEXIST)
#define VS_ERR_ISP_NOT_SUPPORT                   VS_ERR_CODE(E_MOD_ID_ISPFE, E_ERR_NOT_SUPPORT)
#define VS_ERR_ISP_NOT_IMPLEMENTED               VS_ERR_CODE(E_MOD_ID_ISPFE, E_ERR_NOT_IMPLEMENTED)
#define VS_ERR_ISP_NOT_INITIALIZED               VS_ERR_CODE(E_MOD_ID_ISPFE, E_ERR_NOT_INITIALIZED)
#define VS_ERR_ISP_NOT_PERM                      VS_ERR_CODE(E_MOD_ID_ISPFE, E_ERR_NOT_PERM)
#define VS_ERR_ISP_NO_MEMORY                     VS_ERR_CODE(E_MOD_ID_ISPFE, E_ERR_NO_MEMORY)
#define VS_ERR_ISP_BUF_EMPTY                     VS_ERR_CODE(E_MOD_ID_ISPFE, E_ERR_BUF_EMPTY)
#define VS_ERR_ISP_BUF_FULL                      VS_ERR_CODE(E_MOD_ID_ISPFE, E_ERR_BUF_FULL)
#define VS_ERR_ISP_BAD_ADDR                      VS_ERR_CODE(E_MOD_ID_ISPFE, E_ERR_BAD_ADDR)
#define VS_ERR_ISP_BUSY                          VS_ERR_CODE(E_MOD_ID_ISPFE, E_ERR_BUSY)
#define VS_ERR_ISP_TIMEOUT                       VS_ERR_CODE(E_MOD_ID_ISPFE, E_ERR_TIMEOUT)
#define VS_ERR_ISP_ABORTED                       VS_ERR_CODE(E_MOD_ID_ISPFE, E_ERR_ABORTED)
#define VS_ERR_ISP_READ                          VS_ERR_CODE(E_MOD_ID_ISPFE, E_ERR_READ)
#define VS_ERR_ISP_WRITE                         VS_ERR_CODE(E_MOD_ID_ISPFE, E_ERR_WRITE)
#define VS_ERR_ISP_DEVICE_UNEXIST                VS_ERR_CODE(E_MOD_ID_ISPFE, E_ERR_DEVICE_UNEXIST)

#define VS_ISP_STITCH_GRP_NUM_MAX                4
#define VS_ISP_STITCH_PIPE_NUM_MAX               4

#define VS_ALG_LIB_NAME_MAX_SIZE                 256
#define VS_ISP_AUTO_ISO_STRENGTH_SIZE            16
#define VS_ISP_RM_SHADING_LUT_SIZE               33
#define VS_ISP_NP_WEIGHT_LUT_SIZE                128
#define VS_ISP_CAC_MEM_SIZE                      32
#define VS_ISP_CAC_CORRECTION_MEM_SIZE           40
#define VS_ISP_CAC_CORRECTION_SIZE               3
#define VS_ISP_RADIAL_SHADING_LUT_SIZE           129
#define VS_ISP_MESH_SHADING_LUT_SIZE             1024
#define VS_ISP_DRA_ASYMMETRY_SIZE                65
#define VS_ISP_GLOBALTM_LUT_SIZE                 33
#define VS_ISP_DRA_EV_LIM_NO_STR_SIZE            2
#define VS_ISP_DRA8_DARK_ENH_CONTROL_SIZE        15
#define VS_ISP_COLOR_MATRIX_SIZE                 9
#define VS_ISP_AE_HISTOGRAM_SIZE                 1024
#define VS_ISP_DEMOSAIC_LUT_SIZE                 128
#define VS_ISP_PFC_SHADING_LUT_SIZE              33
#define VS_ISP_NONE_EQU_GAMMA_SIZE               65
#define VS_ISP_LUT3D_MEM_SIZE                    1000
#define VS_ISP_GAMMA_DATA_LUT_SIZE               129
#define VS_ISP_GAMMA_THRESH_SIZE                 2
#define VS_ISP_AWB_ZONE_WGHT_SIZE                32
#define VS_ISP_AWB_RG_BG_POS_SIZE                15
#define VS_ISP_AWB_LOWLIGHT_STRENGTH             3
#define VS_ISP_AWB_SKY_STRENGTH                  3
#define VS_ISP_AWB_STATIC_WB_SIZE                4
#define VS_ISP_AWB_WARMING_LS_SIZE               3
#define VS_ISP_AWB_WARMING_CCT_SIZE              4

#define VS_ISP_AWB_COLOR_PREFER_SIZE             4
#define VS_ISP_AWB_SCENE_PRESET_SIZE             7
#define VS_ISP_AWB_EVTOLUX_LUT_SIZE              17
#define VS_ISP_AWB_LUX_LUT                       104
#define VS_ISP_AWB_LIGHT_SRC_LEN                 10
#define VS_ISP_DECOMPANDER0_DATA_SIZE            33
#define VS_ISP_DECOMPANDER1_DATA_SIZE            257
#define VS_ISP_FULL_HISTOGRAM_SIZE               1024
#define VS_ISP_METERING_ZONE_NUM_MAX             1089
#define VS_ISP_LUMA_VAR_SIZE                     512
#define VS_ISP_BLC_SIZE                          4
#define VS_ISP_GAMMA_GAIN_SIZE                   3
#define VS_ISP_CSC_COEFFT_SIZE                   9
#define VS_ISP_CSC_COEFFT_O_SIZE                 3
#define VS_ISP_CSC_DATA_MASK_SIZE                3
#define VS_ISP_AE_CORRECTION_SIZE                12
#define VS_ISP_PFC_COEFF_SIZE                    9
#define VS_ISP_MESH_PAGE_SIZE                    4
#define VS_ISP_MESH_ALPHA_SIZE                   4
#define VS_ISP_RADIAL_CENTER_SIZE                4
#define VS_ISP_NOISE_THRESH_SIZE                 3
#define VS_ISP_NOISE_LEVEL_SIZE                  4
#define VS_ISP_DRA_GAIN_SIZE                     4
#define VS_ISP_AE_EXP_PARTITIONS_SIZE            16

#define VS_ISP_PRESHARPEN_GAUSS_SIZE             5
#define VS_ISP_PRESHARPEN_BILA_SIZE              5
#define VS_ISP_PRESHARPEN_HV_CRTL_SIZE           5
#define VS_ISP_PRESHARPEN_ARRAY_SIZE             32
#define VS_ISP_PRESHARPEN_GAUSSUV_DATA_SIZE0     3
#define VS_ISP_PRESHARPEN_GAUSSUV_DATA_SIZE1     5
#define VS_ISP_PRESHARPEN_UV_SIZE                4
#define VS_ISP_PRESHARPEN_MAX_DIST_INV_SIZE      4
#define VS_ISP_PRESHARPEN_COLOR_STR_GAIN_SIZE    4
#define VS_ISP_NR3D_YLAYER_ENABLE_SIZE           3
#define VS_ISP_NR3D_CLAYER_ENABLE_SIZE           4
#define VS_ISP_NR3D_RADIUS_GAIN_SIZE             6
#define VS_ISP_NR3D_RADIUS_IDX_SIZE              5
#define VS_ISP_NR3D_RADIUS_SLOPE_SIZE            5
#define VS_ISP_NR3D_XYPOS_STRENGTH_SIZE          3
#define VS_ISP_NR3D_XYPOS_SLOPE_SIZE             4
#define VS_ISP_NR3D_WEIGHT_IDX_SIZE              4
#define VS_ISP_NR3D_CORRECT_RATIO_SIZE           4
#define VS_ISP_POSTSHARPEN_GAUSS_SIZE            5
#define VS_ISP_POSTSHARPEN_BILA_SIZE             5
#define VS_ISP_POSTSHARPEN_HV_CTRL_SIZE          5
#define VS_ISP_POSTSHARPEN_ARRAY_SIZE            32
#define VS_ISP_PFC_PLIES_SIZE                    4
#define VS_ISP_SA_LUT_SIZE                       33
#define VS_ISP_COLORING_LUT_SIZE                 256
#define VS_ISP_COLORING_GAUSS_SIZE               5
#define VS_ISP_AF_LDG_X_SIZE                     4
#define VS_ISP_AF_LDG_GAIN_SIZE                  3
#define VS_ISP_AF_CORING_X_SIZE                  2
#define VS_ISP_AF_CORING_MAP_SIZE                2
#define VS_ISP_YUV_AF_FILTER_COEF_SIZE           5
#define VS_ISP_YUV_AF_STATISTICS_ROWS_SIZE       15
#define VS_ISP_YUV_AF_STATISTICS_COLS_SIZE       17
#define VS_ISP_NR3D_SIGMA_SCALE_NOISE_SIZE       256
#define VS_ISP_NR3D_PEAK_VALUE_SIZE              20

#define VS_ISP_AE_EXPOSURE_NUM_MAX               4
#define VS_ISP_SMART_CLASS_MAX                   2

#define VS_ISP_FOCUS_FILTER_COEF_NUM             5
#define VS_ISP_FOCUS_FILTER_SHIFT_NUM            4
#define VS_ISP_FOCUS_VALUE_ROWS_SIZE             15
#define VS_ISP_FOCUS_VALUE_COLS_SIZE             17

#define VS_ISP_COLOR_SHADING_LIGHT_NUM           22

#define VS_ISP_STATIC_DP_LUT_SIZE                4095
/**
 * @brief Defines vs isp operation type
 * @details
 */
typedef enum vs_isp_op_type {
    E_ISP_OP_TYPE_AUTO = 0,
    E_ISP_OP_TYPE_MANUAL,
    E_ISP_OP_TYPE_MAX
} vs_isp_op_type_e;

/**
 * @brief Defines the isp firmware status.
 * @details
 */
typedef enum vs_isp_fw_state {
    E_ISP_FW_STATE_RUN = 0,
    E_ISP_FW_STATE_FREEZE,
    E_ISP_FW_STATE_MAX
} vs_isp_fw_state_e;

/**
 * @brief Defines the isp interrupt wait type.
 * @details
 */
typedef enum vs_isp_wait_type {
    E_ISP_WAIT_TYPE_INTERRUPT_START = 0,
    E_ISP_WAIT_TYPE_INTERRUPT_END,
    E_ISP_WAIT_TYPE_DMA_END,
    E_ISP_WAIT_TYPE_CAPTURE_START,
    E_ISP_WAIT_TYPE_DMA_END_ADV,
    E_ISP_WAIT_TYPE_MAX
} vs_isp_wait_type_e;

/**
 * @brief Defines the format of the input Bayer image.
 * @details
 */
typedef enum vs_isp_bayer_format {
    E_ISP_BAYER_FORMAT_RGGB = 0,
    E_ISP_BAYER_FORMAT_GRBG = 1,
    E_ISP_BAYER_FORMAT_GBRG = 2,
    E_ISP_BAYER_FORMAT_BGGR = 3,
    E_ISP_BAYER_FORMAT_MAX
} vs_isp_bayer_format_e;

/**
 * @brief Defines the mode of shutter.
 * @details
 */
typedef enum vs_isp_ae_shutter_mode {
    E_ISP_SHUTTER_MODE_FIX_FPS = 0,
    E_ISP_SHUTTER_MODE_SLOW_SHUTTER = 1,
    E_ISP_SHUTTER_MODE_MAX
} vs_isp_shutter_mode_e;

/**
 * @brief Defines ISP common attribute, contains the public image attribute.
 * @details
 */
typedef struct vs_isp_comm_attr {
    vs_size_s       sensor_size;          ///<   R; Range:[0, 8192]; Resolution of Sensor
    vs_float_t      frame_rate;           ///<   RW; Range:[1, 240]; Frame rate of Sensor
    vs_isp_bayer_format_e  bayer_format;  ///<   RW; Range:[0x0, 0x3]; Bayer format of Sensor
    vs_wdr_mode_e   wdr_mode;             ///<   R; Range:[0x0, 0x4]; Wdr mode of Sensor
    vs_uint8_t     sensor_mode;           ///<   R; Range:[0x0, 0xff]; Mode of Sensor
} vs_isp_comm_attr_s;

/**
 * @brief Defines ISP ae hist5 statistics  data type.
 * @details
 */
typedef struct vs_isp_ae_hist5_statistics {
    vs_uint16_t hist0;  ///<the 0th bin value   R; Range:[0x0, 0xFFFF];
    vs_uint16_t hist1;  ///<the 1st bin value   R; Range:[0x0, 0xFFFF];
    vs_uint16_t hist2;  ///<the 2nd bin value   R; Range:[0x0, 0xFFFF];
    vs_uint16_t hist3;  ///<the 3rd bin value   R; Range:[0x0, 0xFFFF];
    vs_uint16_t hist4;  ///<the 4th bin value   R; Range:[0x0, 0xFFFF];
} vs_isp_ae_hist5_statistics_s;

/**
 * @brief Defines ISP ae statistics data type.
 * @details
 */
typedef struct vs_isp_ae_statistics {
    vs_uint32_t fullhist[VS_ISP_FULL_HISTOGRAM_SIZE];   ///<   R; Range:[0x0, 0xFFFFFFFF];
    vs_uint32_t fullhist_sum;                           ///<   R; Range:[0x0, 0xFFFFFFFF];
    vs_isp_ae_hist5_statistics_s hist5[VS_ISP_METERING_ZONE_NUM_MAX];
    vs_uint32_t hist5_zones_num;                        ///<   R; Range:[0x0, 0xFFFFFFFF];
    vs_uint32_t fullhist_long_exposure[VS_ISP_FULL_HISTOGRAM_SIZE];   ///<   R; Range:[0x0, 0xFFFFFFFF];
    vs_uint32_t fullhist_sum_long_exposure;             ///<   R; Range:[0x0, 0xFFFFFFFF];
}vs_isp_ae_statistics_s;

/**
 * @brief Defines ISP wb zone file struct.
 * @details
 */
typedef struct vs_isp_awb_zone {
    vs_uint16_t rg;                                   ///<   R; Range:[0x0, 0xFFFF];
    vs_uint16_t bg;                                   ///<   R; Range:[0x0, 0xFFFF];
    vs_uint32_t sum;                                  ///<   R; Range:[0x0, 0xFFFFFFFF];
}vs_isp_awb_zone_s;

/**
 * @brief Defines ISP awb statistics data type.
 * @details
 */
typedef struct vs_isp_awb_statistics {
    vs_isp_awb_zone_s awb_zones[VS_ISP_METERING_ZONE_NUM_MAX];
    vs_uint32_t zones_num;                          ///<   R; Range:[0x0, 1089];
} vs_isp_awb_statistics_s;

/**
 * @brief Defines ISP af statistics data type.
 * @details
 */
typedef struct vs_isp_af_stats_zone {
    vs_uint32_t hist_value;
}vs_isp_af_stats_zone_s;

/**
 * @brief Defines ISP af statistics data type.
 * @details
 */
typedef struct vs_isp_af_statistics {
    vs_isp_af_stats_zone_s zones_stats[VS_ISP_METERING_ZONE_NUM_MAX];
    vs_uint32_t zones_num;       ///<   R; Range:[0x0, 1089];
    vs_isp_af_stats_zone_s zones_stats_adv[VS_ISP_METERING_ZONE_NUM_MAX];
} vs_isp_af_statistics_s;

/**
 * @brief Defines ISP luma statistics data type.
 * @details
 */
typedef struct vs_isp_luma_var_statistics{
    vs_uint16_t mean_info[VS_ISP_LUMA_VAR_SIZE];  ///<   R; Range:[0x0, 0x3FF];
    vs_uint16_t var_info[VS_ISP_LUMA_VAR_SIZE];   ///<   R; Range:[0x0, 0xFFF];
} vs_isp_luma_var_statistics_s;

/**
 * @brief Defines ISP statistics ctrl data type.
 * @details
 */
typedef union vs_isp_statistics_ctrl {
    vs_uint64_t  key;
    struct {
        vs_uint64_t  ae_5bin_hist  : 1 ;   /* [0] */
        vs_uint64_t  aexp_hist     : 1 ;   /* [1] */
        vs_uint64_t  luma_var      : 1 ;   /* [2] */
        vs_uint64_t  af            : 1 ;   /* [3] */
        vs_uint64_t  awb           : 1 ;   /* [4] */
        vs_uint64_t  resv          : 59;   /* [5:63] */
    };
} vs_isp_statistics_ctrl_u;

/**
 * @brief Defines ISP ae histogram statistics position switch type
 * @details
 */
typedef enum vs_isp_ae_hist_switch {
    E_ISP_AE_HIST_AFTER_WB = 0,
    E_ISP_AE_HIST_AFTER_WDR,
    E_ISP_AE_HIST_AFTER_VTPG,
    E_ISP_AE_HIST_SWITCH_MAX,
} vs_isp_ae_hist_switch_e;

/**
 * @brief Defines ISP awb statistics position switch type
 * @details
 */
typedef enum vs_isp_awb_switch {
    E_ISP_AWB_AFTER_DEMOSAICR = 0,
    E_ISP_AWB_AFTER_CNR,
    E_ISP_AWB_SWITCH_MAX,
} vs_isp_awb_switch_e;

/**
 * @brief Defines ISP af statistics position switch type
 * @details
 */
typedef enum vs_isp_af_switch {
    E_ISP_AF_AFTER_NR2D = 0,
    E_ISP_AF_BEFORE_NR2D,
    E_ISP_AF_SWITCH_MAX,
} vs_isp_af_switch_e;

/**
 * @brief Defines ISP ae 5bin statistics configure data type
 * @details
 */
typedef struct vs_isp_ae_bin5_hist_cfg {
    vs_uint16_t hist_thresh01; ///<   RW; Range:[0x0, 0xFFF]; Histogram statistics thresholds for paragraphs 0 and 1.
    vs_uint16_t hist_thresh12; ///<   RW; Range:[0x0, 0xFFF]; Histogram statistics thresholds for paragraphs 1 and 2.
    vs_uint16_t hist_thresh23; ///<   RW; Range:[0x0, 0xFFF]; Histogram statistics thresholds for paragraphs 2 and 3.
    vs_uint16_t hist_thresh34; ///<   RW; Range:[0x0, 0xFFF]; Histogram statistics thresholds for paragraphs 3 and 4.
} vs_isp_ae_bin5_hist_cfg_s;

/**
 * @brief Defines ISP ae 1024 bin statistics configure data type
 * @details
 */
typedef struct vs_isp_ae_bin1024_hist_cfg {
    vs_uint8_t skip_x;            ///<   RW; Range:[0x0, 0x7]; Horizontal sampling points for histogram statistics.
    vs_uint8_t skip_y;            ///<   RW; Range:[0x0, 0x7]; Vertical sampling points for histogram statistics.
    vs_uint8_t offset_x;          ///<   RW; Range:[0x0, 0x1]; Horizontal starting point for histogram statistics.
    vs_uint8_t offset_y;          ///<   RW; Range:[0x0, 0x1]; Vertical starting point for histogram statistics.
    vs_uint8_t scale_bottom;      ///<   RW; Range:[0x0, 0xF]; Range for the lower half scale.
    vs_uint8_t scale_top;         ///<   RW; Range:[0x0, 0xF]; Range for the upper half scale.
    vs_uint8_t plane_mode;        ///<   RW; Range:[0x0, 0x7]; Plane separation mode.
    vs_uint8_t zone_weight[VS_ISP_METERING_ZONE_NUM_MAX]; ///<   RW; Range:[0x0, 0xF]; Block weight table of histogram area.
}vs_isp_ae_bin1024_hist_cfg_s;

/**
 * @brief Defines ISP ae statistics configure data type
 * @details
 */
typedef struct vs_isp_ae_statistics_cfg {
    vs_isp_ae_hist_switch_e  ae_hist_switch;          ///<   RW; Range:[0x0, 0x2]; Position for obtaining ae histogram statistical.
    vs_isp_ae_bin5_hist_cfg_s    bin5_hist;
    vs_isp_ae_bin1024_hist_cfg_s bin1024_hist;
} vs_isp_ae_statistics_cfg_s;

/**
 * @brief Defines ISP awb statistics value is Green/Red and Green/Blue or  Red/Green and Blue/Green
 * @details
 */
typedef enum vs_isp_awb_stats_mode {
    E_AWB_STATS_MODE_GR_GB = 0,
    E_AWB_STATS_MODE_RG_BG,
    E_AWB_STATS_MODE_MAX,
}vs_isp_awb_stats_mode_e;

/**
 * @brief Defines ISP awb statistics configure data type
 * @details
 */
typedef struct vs_isp_awb_statistics_cfg {
    vs_isp_awb_switch_e awb_switch;                    ///<   RW; Range:[0x0, 0x1]; Position for obtaining white balance statistics.
    vs_isp_awb_stats_mode_e awb_stats_mode;            ///<   RW; Range:[0x0, 0x1]; White balance statistical information mode.
    vs_uint8_t  zone_v;      ///<   RW; Range:[0x0, 0x21]; White balance statistics number of vertical blocks.
    vs_uint8_t  zone_h;      ///<   RW; Range:[0x0, 0x21]; White balance statistics number of horizontal blocks.
    vs_uint16_t white_level; ///<   RW; Range:[0x0, 0x3FF]; Find the upper brightness limit of the white spot.
    vs_uint16_t black_level; ///<   RW; Range:[0x0, 0x3FF]; Find the lower brightness limit of the white spot.
    vs_uint16_t cr_min;      ///<   RW; Range:[0x0, 0xFFF]; Minimum color difference R/G when counting white dot information.
    vs_uint16_t cr_max;      ///<   RW; Range:[0x0, 0xFFF]; Maximum color difference R/G when counting white dot information.
    vs_uint16_t cb_min;      ///<   RW; Range:[0x0, 0xFFF]; Minimum color difference B/G when counting white dot information.
    vs_uint16_t cb_max;      ///<   RW; Range:[0x0, 0xFFF]; Maximum color difference B/G when counting white dot information.
    vs_uint16_t cr_low;      ///<   RW; Range:[0x0, 0xFFF]; Cr value Cb value corresponding to CbMin is restricted in the area of hexagonal white dots when counting white dot information.
    vs_uint16_t cr_high;     ///<   RW; Range:[0x0, 0xFFF]; Cr value Cb value corresponding to CbMax is restricted in the area of hexagonal white dots when counting white dot information.
    vs_uint16_t cb_low;      ///<   RW; Range:[0x0, 0xFFF]; Cb value Cb value corresponding to CrMin is restricted in the area of hexagonal white dots when counting white dot information.
    vs_uint16_t cb_high;     ///<   RW; Range:[0x0, 0xFFF]; Cb value Cb value corresponding to CrMax is restricted in the area of hexagonal white dots when counting white dot information.
    vs_uint8_t weight[VS_ISP_METERING_ZONE_NUM_MAX];   ///<   RW; Range:[0x0, 0xF]; Block weight table of white balance statistics.
}vs_isp_awb_statistics_cfg_s;

/**
 * @brief Defines ISP raw domain af statistics configure kernel type
 * @details
 */
typedef enum vs_isp_af_kernel_select {
    E_ISP_AF_KERNEL_0 = 0,
    E_ISP_AF_KERNEL_1,
    E_ISP_AF_KERNEL_2,
    E_ISP_AF_KERNEL_3,
    E_ISP_AF_KERNEL_MAX
} vs_isp_af_kernel_select_e;

/**
 * @brief Defines ISP raw domain af noise reduce manual configure data type
 * @details
 */
typedef struct vs_isp_af_nr_manual_attr {
    vs_uint16_t   thresh_1;                         ///<   RW; Range:[0x0, 0xFF]; High frequency noise threshold.
    vs_uint16_t   strength1;                        ///<   RW; Range:[0x0, 0xFF]; High frequency strength threshold.
}vs_isp_af_nr_manual_attr_s;

/**
 * @brief Defines ISP raw domain af noise reduce auto configure data type
 * @details
 */
typedef struct vs_isp_af_nr_auto_attr {
    vs_uint16_t   thresh_1[VS_ISP_AUTO_ISO_STRENGTH_SIZE];                  ///<   RW; Range:[0x0, 0xFF]; High frequency noise threshold.
    vs_uint16_t   strength1[VS_ISP_AUTO_ISO_STRENGTH_SIZE];                 ///<   RW; Range:[0x0, 0xFF]; High frequency strength threshold.
}vs_isp_af_nr_auto_attr_s;

/**
 * @brief Defines ISP raw domain af noise reduce configure data type
 * @details
 */
typedef struct vs_isp_af_nr_attr {
    vs_bool_t enable;                           ///<   RW; Range:[0, 1]; Format:1.0; AF noise reduction module enable.
    vs_bool_t nlm_en;                           ///<   RW; Range:[0, 1]; Format:1.0; Non-local mean filtering module enable.
    vs_uint16_t  thresh_4;                      ///<   RW; Range:[0x0, 0xFF]; Low frequency noise threshold.
    vs_uint16_t  strength4;                     ///<   RW; Range:[0x0, 0xFF]; Low frequency noise strength.
    vs_uint16_t  global_offset;                 ///<   RW; Range:[0x0, 0xFF]; Global offset.
    vs_isp_op_type_e  nr_optype;                ///<   RW; Range:[0, 1]; Format:1.0; Operation type.
    vs_isp_af_nr_manual_attr_s nr_manual_attr;
    vs_isp_af_nr_auto_attr_s  nr_auto_attr;
} vs_isp_af_nr_attr_s;

/**
 * @brief Defines ISP raw domain af statistics configure data type
 * @details
 */
typedef struct vs_isp_af_raw_statistics_cfg {
    vs_isp_af_switch_e af_switch;               ///<   RW; Range:[0x0, 0x1]; Position for obtaining focus statistics.
    vs_isp_af_kernel_select_e  kernel_sel;      ///<   RW; Range:[0x0, 0x3]; Kernel selection.
    vs_uint8_t zone_v;                          ///<   RW; Range:[0x0, 0x21]; Number of vertical direction blocks.
    vs_uint8_t zone_h;                          ///<   RW; Range:[0x0, 0x21]; Number of horizontal direction blocks.
    vs_isp_af_nr_attr_s nr_attr;
}vs_isp_af_raw_statistics_cfg_s;

/**
 * @brief Defines the isp  statistics configure data type.
 * @details
*/
typedef struct vs_isp_statistics_cfg {
    vs_isp_statistics_ctrl_u       key;
    vs_isp_ae_statistics_cfg_s     ae_cfg;
    vs_isp_awb_statistics_cfg_s    awb_cfg;
    vs_isp_af_raw_statistics_cfg_s  af_cfg;
} vs_isp_statistics_cfg_s;


/**
 * @brief Defines the isp iq information data type.
 * @details
*/
typedef struct vs_isp_dynamic_range_info {
    //dra
    vs_uint16_t   dra_dgain_actual;                        ///<   R; Range:[0x0, 0xFFF]; Dra module digital gain effective value.
    vs_uint16_t   dra_collection_correction_actual;        ///<   R; Range:[0x0, 0xFFF]; Dra module collection factor effective value.
    vs_uint16_t   dra_strength_inroi_actual;               ///<   R; Range:[0x0, 0x3FF]; Dra module strength inroi effective value.
    vs_uint16_t   dra_dark_enh_actual;                     ///<   R; Range:[0x0, 0xFFFF]; Dra module dark enh effective value.

    //GAMMA
    vs_uint16_t   gamma_gain_r_actual;                   ///<   R; Range:[0x0, 0xFFF]; gain of r channel. R channel gamma gain effective value.
    vs_uint16_t   gamma_gain_g_actual;                   ///<   R; Range:[0x0, 0xFFF]; gain of g channel. G channel gamma gain effective value.
    vs_uint16_t   gamma_gain_b_actual;                   ///<   R; Range:[0x0, 0xFFF]; gain of b channel. B channel gamma gain effective value.
    vs_uint16_t   gamma_offset_r_actual;                 ///<   R; Range:[0x0, 0xFFF]; gain of r channel. R channel gamma offset effective value.
    vs_uint16_t   gamma_offset_g_actual;                 ///<   R; Range:[0x0, 0xFFF]; gain of g channel. G channel gamma offset effective value.
    vs_uint16_t   gamma_offset_b_actual;                 ///<   R; Range:[0x0, 0xFFF]; gain of b channel. B channel gamma offset effective value.
    vs_uint32_t   gamma_lut_actual[VS_ISP_GAMMA_DATA_LUT_SIZE]; ///<   R; Range:[0x0, 0xFFFF]; Gamma module gamma lut effective value.
}vs_isp_dynamic_range_info_s;

/**
 * @brief Defines the isp iq information data type.
 * @details
*/
typedef struct vs_isp_iq_info {
    //Black Level
    vs_uint32_t   black_level_r_actual;                  ///<   R; Range:[0x0, 0xFFFFF]; Format:20.0; R channel black level effective value.
    vs_uint32_t   black_level_gr_actual;                 ///<   R; Range:[0x0, 0xFFFFF]; Format:20.0; GR channel black level effective value.
    vs_uint32_t   black_level_gb_actual;                 ///<   R; Range:[0x0, 0xFFFFF]; Format:20.0; GB Channel black level effective value.
    vs_uint32_t   black_level_b_actual;                  ///<   R; Range:[0x0, 0xFFFFF]; Format:20.0; B channelblack level effective value.

    //Dynamic Defect Pixel
    vs_uint16_t   dynamic_dp_slope_actual;               ///<   R; Range:[0x0, 0xFFF]; Dynamic defect pixel module slope effective value.
    vs_uint16_t   dynamic_dp_thresh_actual;              ///<   R; Range:[0x0, 0xFFF]; Dynamic defect pixel module thresh effective value.

    vs_uint8_t    nr2d_int_config_actual;                ///<   R; Range:[0x0, 0x0F]; 2DNR module alpha blend effective value between Raw and intensity image.
    vs_uint8_t    nr2d_sad_filt_thresh_actual;           ///<   R; Range:[0x0, 0xFF]; 2DNR module balanced contrast between edge detail and flat region smoothness effective value .
    vs_uint8_t    nr2d_thresh_1_actual;                  ///<   R; Range:[0x0, 0xFF]; 2DNR module noise threshold effective value  for high frequency.
    vs_uint8_t    nr2d_thresh_4_actual;                  ///<   R; Range:[0x0, 0xFF]; 2DNR module noise threshold effective value  for low frequency.
    vs_uint8_t    nr2d_strength1_actual;                 ///<   R; Range:[0x0, 0xFF]; 2DNR module de-noising strength effective value  for high frequency.
    vs_uint16_t   nr2d_strength4_actual;                 ///<   R; Range:[0x0, 0xFFFF]; 2DNR module de-noising strength effective value  for low frequency.
    vs_uint8_t    nr2d_global_offset_actual;             ///<   R; Range:[0x0, 0xFF]; 2DNR module de-noising global strength effective value.
    vs_uint8_t    nr2d_strength_mc_contrast_actual;      ///<   R; Range:[0x0, 0xFF]; 2DNR module motion compensation strength effective value.
    //Mesh Shading
    vs_uint16_t   mesh_shading_strength_actual;          ///<   R; Range:[0x0, 0xFFFF]; Mesh shading strength effective value.

    //Demosaic
    vs_uint8_t    demosaic_sharp_alt_d_actual;           ///<   R; Range:[0x0, 0xFF]; Demosaic module high frequency strength effective value.
    vs_uint8_t    demosaic_sharp_alt_ud_actual;          ///<   R; Range:[0x0, 0xFF]; Demosaic module medium frequency strength effective value.
    vs_uint8_t    demosaic_sharp_alt_lu_actual;          ///<   R; Range:[0x0, 0xFF]; Demosaic module low frequency strength effective value.
    vs_uint8_t    demosaic_sharp_alt_ldu_actual;         ///<   R; Range:[0x0, 0xFF]; Demosaic module medium frequency strength effective value.
    vs_uint8_t    demosaic_sharp_alt_ld_actual;          ///<   R; Range:[0x0, 0xFF]; Demosaic module high frequency strength effective value.
    vs_uint16_t   demosaic_uu_slope_actual;              ///<   R; Range:[0x0, 0xFFFF]; Demosaic module directionless edge detection slope effective value.
    vs_uint16_t   demosaic_np_offset_actual;             ///<   R; Range:[0x0, 0xFFFF]; Demosaic module iImage noise parameters effective value.

    //CNR
    vs_uint16_t   cnr_uv_delta1_slope_actual;            ///<   R; Range:[0x0, 0xFFFF]; Cnr module color noise removal strength effective value.
    vs_uint16_t   cnr_uv_delta2_slope_actual;            ///<   R; Range:[0x0, 0xFFFF]; Cnr module color noise removal strength effective value.

    //FR Sharpen
    vs_uint16_t   rgb_sharpen_strength_actual;           ///<   R; Range:[0x0, 0x1FF]; RGB sharpen strength effective value.

    //PFC
    vs_uint16_t   pfc_ccm_coeff_actual[VS_ISP_PFC_COEFF_SIZE];                    ///<   RW; Range:[0x0, 0x1FFF]; Ccm coeff rr,rg,rb,gr,gg,gb,br,bg,bb effective value.

    //CSC
    vs_uint16_t   csc_coefft_actual[VS_ISP_CSC_COEFFT_SIZE];                     ///<   RW; Range:[0x0, 0xFFFF]; Csc coefft effective value
    vs_uint16_t   csc_coefft_o_actual[VS_ISP_CSC_COEFFT_O_SIZE];                 ///<   RW; Range:[0x0, 0x7FF]; Csc coefft effective value
} vs_isp_iq_info_s;

/**
 * @brief Defines ISP ae state.
 * @details
 */
typedef enum vs_isp_ae_state {
    E_ISP_AE_INACTIVE = 0,
    E_ISP_AE_SEARCHING,
    E_ISP_AE_CONVERGED,
    E_ISP_AE_STATE_MAX
} vs_isp_ae_state_e;

/**
 * @brief Defines the isp iq exposure information.
 * @details
*/
typedef struct vs_isp_expo_info {
    vs_isp_ae_state_e  ae_state_id;                 ///<   R; Range:[0x0, 0x2]; AE current status.
    vs_uint32_t  system_long_integration_time;      ///<   R; Range:[0x0, 0xFFFFFFFF]; Format:32.0; Sensor long frame exposure time (unit: us ).
    vs_uint32_t  system_median_integration_time;    ///<   R; Range:[0x0, 0xFFFFFFFF]; Format:32.0; Sensor middle frame exposure time (unit: us ).
    vs_uint32_t  system_short_integration_time;     ///<   R; Range:[0x0, 0xFFFFFFFF]; Format:32.0; System short frame exposure time (unit: us ).
    vs_uint32_t  exposure_ratio;                    ///<   R; Range:[0x0, 0xFFFFFFFF]; Format:26.6; Sensor exposure ratio.
    vs_uint32_t  sensor_again;                      ///<   R; Range:[0x100, 0xFFFFFFFF]; Format:22.8; Sensor analog gain (unit: times).
    vs_uint32_t  sensor_dgain;                      ///<   R; Range:[0x100, 0xFFFFFFFF]; Format:22.8; Sensor digital gain (unit: times).
    vs_uint16_t  system_isp_dgain;                  ///<   R; Range:[0x10000, 0xFFFFFFFF]; Format:16.16; Isp digital gain (unit: times).
    vs_uint32_t  ae_hist[VS_ISP_AE_HISTOGRAM_SIZE]; ///<   R; Range:[0x0, 0xFFFFFFFF]; Format:32.0; 1024 bins histogram.
    vs_uint32_t  system_exposure;                   ///<   R; Range:[0x0, 0xFFFFFFFF]; Format:32.0; System exposure.
    vs_uint32_t  status_info_exposure_log2_id;      ///<   R; Range:[0x0, 0xFFFFFFFF]; Format:32.0; System exposure after log2 is taken.
    vs_uint32_t  status_info_iso;                   ///<   R; Range:[0x0, 0xFFFFFFFF]; Format:32.0; System total gain.
    vs_uint32_t  status_info_gaindb;                ///<   R; Range:[0x0, 0xFFFFFFFF]; Format:32.0; System total db.
    vs_uint32_t  lines_per_second;                  ///<   R; Range:[0x0, 0xFFFFFFFF]; Format:32.0; Sensor exposure lines corresponding to 1 second.
    vs_uint16_t  luma_mean;                         ///<   R; Range:[0x0, 0xFFFFFFFF]; Format:32.0; Mean brightness of the current image.
} vs_isp_expo_info_s;

/**
 * @brief Defines vs isp awb algorithm state
 * @details
 */
typedef enum vs_isp_awb_state {
    E_ISP_AWB_INACTIVE = 0,
    E_ISP_AWB_SEARCHING,
    E_ISP_AWB_CONVERGED,
    E_ISP_AWB_STATE_MAX
} vs_isp_awb_state_e;

/**
 * @brief Defines the isp iq wb information.
 * @details
*/
typedef struct vs_isp_wb_info {
    vs_isp_awb_state_e  awb_state;                             ///<   R; Range:[0, 2]; AWB current status.
    vs_uint16_t r_gain_actual;                                 ///<   R; Range:[0x100, 0xF00]; Format:4.8; R channel gain.
    vs_uint16_t gr_gain_actual;                                ///<   R; Range:[0x100, 0xF00]; Format:4.8; GR channel gain.
    vs_uint16_t gb_gain_actual;                                ///<   R; Range:[0x100, 0xF00]; Format:4.8; GB channel gain.
    vs_uint16_t b_gain_actual;                                 ///<   R; Range:[0x100, 0xF00]; Format:4.8; B channel gain.
    vs_uint16_t color_temperature;                             ///<   R; Range:[0x0, 0xFFFF]; Format:16.0; Color temperature calculated by WB.
    vs_uint16_t saturation_actual;                             ///<   R; Range:[0x0, 0x1FF]; Format:1.8; Actual saturation level.
    vs_uint16_t color_matrix_actual[VS_ISP_COLOR_MATRIX_SIZE]; ///<   R; Range:[0x0, 0xFFFF]; Acutal color matrix.
    vs_uint8_t    mesh_alpha_bank_r_actual;                    ///<   R; Range:[0x0, 0x07]; Mesh alpha bank of R channel.
    vs_uint8_t    mesh_alpha_bank_g_actual;                    ///<   R; Range:[0x0, 0x07]; Mesh alpha bank of G channel.
    vs_uint8_t    mesh_alpha_bank_b_actual;                    ///<   R; Range:[0x0, 0x07]; Mesh alpha bank of B channel.
    vs_uint8_t    mesh_alpha_r_actual;                         ///<   R; Range:[0x0, 0x07]; Alpha blend coeff bank of R channel.
    vs_uint8_t    mesh_alpha_g_actual;                         ///<   R; Range:[0x0, 0x07]; Alpha blend coeff bank of G channel.
    vs_uint8_t    mesh_alpha_b_actual;                         ///<   R; Range:[0x0, 0x07]; Alpha blend coeff bank of B channel.
} vs_isp_wb_info_s;

/**
 * @brief Defines the isp wdr stitch information.
 * @details
*/
typedef struct vs_isp_wdr_stitch_info {
    vs_bool_t     mc_off_mode_enable;                    ///<   R; Range:[0, 1]; Mc off mode enable.
    vs_uint16_t   gain_r_actual;                         ///<   R; Range:[0x0, 0xFFF]; Gain of red channel.
    vs_uint16_t   gain_b_actual;                         ///<   R; Range:[0x0, 0xFFF]; Gain of blue channel.
    vs_uint16_t   black_level_long_actual;               ///<   R; Range:[0x0, 0xFFF]; Black level long.
    vs_uint16_t   black_level_medium_actual;             ///<   R; Range:[0x0, 0xFFF]; Black level medium.
    vs_uint16_t   black_level_short_actual;              ///<   R; Range:[0x0, 0xFFF]; Black level short.
    vs_uint16_t   black_level_very_short_actual;         ///<   R; Range:[0x0, 0xFFF]; Black level very short.
    vs_uint32_t   black_level_out_actual;                ///<   R; Range:[0x0, 0xFFFFF]; Black level out.
    vs_uint16_t   lm_np_mult_actual;                     ///<   R; Range:[0x0, 0xFFF]; Long-medium frame exposure noise level factor.
    vs_uint16_t   ms_np_mult_actual;                     ///<   R; Range:[0x0, 0xFFF]; Medium-short frame exposure noise level factor.
    vs_uint16_t   svs_np_mult_actual;                    ///<   R; Range:[0x0, 0xFFF]; Short-very short frame exposure noise level factor.
    vs_uint16_t   lm_alpha_mov_slope_actual;             ///<   R; Range:[0x0, 0xFFF]; Long-medium frame exposure motion detection Slope coefficient.
    vs_uint16_t   ms_alpha_mov_slope_actual;             ///<   R; Range:[0x0, 0xFFF]; Medium-short frame exposure motion detection Slope coefficient.
    vs_uint16_t   svs_alpha_mov_slope_actual;            ///<   R; Range:[0x0, 0xFFF]; Short-very short frame exposure motion detection Slope coefficient.
    vs_uint16_t   lm_med_noise_intensity_thresh_actual;  ///<   R; Range:[0x0, 0xFFF]; Medium-short frame exposure threshold.
} vs_isp_wdr_stitch_info_s;

/*
 * @brief Defines the isp configure information.
 * @details
*/
typedef struct vs_isp_cfg_info {
    vs_uint16_t active_width;                ///<   RW; Range:[0, 65535]; Format 16.0;Input raw image width.
    vs_uint16_t active_height;               ///<   RW; Range:[0, 65535]; Format 16.0;Input raw image height.
} vs_isp_cfg_info_s;

/**
 * @brief Defines the isp iq information attribute.
 * @details
*/
typedef struct vs_isp_info_attr {
    vs_isp_cfg_info_s      cfg_info;
    vs_isp_iq_info_s       iq_info;
    vs_isp_dynamic_range_info_s dynamic_range_info;
    vs_isp_expo_info_s     expo_info;
    vs_isp_wb_info_s       wb_info;
    vs_isp_wdr_stitch_info_s wdr_stitch_info;
} vs_isp_info_attr_s;

/**
 * @brief Defines ISP algorithm bypass setting
 * @details
 */
typedef struct vs_isp_bypass_setting {
    vs_bool_t    bypass_decompander;                ///<   RW; Range:[0, 1]; Format:1.0; Bypass signal of decompander.
    vs_bool_t    bypass_sensor_offset_wdr;          ///<   RW; Range:[0, 1]; Format:1.0; Bypass signal of sensor offset wdr.
    vs_bool_t    bypass_gain_wdr;                   ///<   RW; Range:[0, 1]; Format:1.0; Bypass signal of gain wdr.
    vs_bool_t    bypass_frame_stitch;               ///<   RW; Range:[0, 1]; Format:1.0; Bypass signal of frame stitch.
    vs_bool_t    bypass_digital_gain;               ///<   RW; Range:[0, 1]; Format:1.0; Bypass signal of digital gain.
    vs_bool_t    bypass_frontend_sensor_offset;     ///<   RW; Range:[0, 1]; Format:1.0; Bypass signal of frontend sensor offset.
    vs_bool_t    bypass_sensor_offset_pre_shading;  ///<   RW; Range:[0, 1]; Format:1.0; Bypass signal of sensor offset pre shading.
    vs_bool_t    bypass_fe_sqrt;                    ///<   RW; Range:[0, 1]; Format:1.0; Bypass signal of fe sqrt.
    vs_bool_t    bypass_square_be;                  ///<   RW; Range:[0, 1]; Format:1.0; Bypass signal of square be.
    vs_bool_t    bypass_raw_fronted;                ///<   RW; Range:[0, 1]; Format:1.0; Bypass signal of raw fronted.
    vs_bool_t    bypass_static_defect_pixel;        ///<   RW; Range:[0, 1]; Format:1.0; Bypass signal of static defect pixel.
    vs_bool_t    bypass_nr2d;                       ///<   RW; Range:[0, 1]; Format:1.0; Bypass signal of 2dnr.
    vs_bool_t    bypass_ca_correction;              ///<   RW; Range:[0, 1]; Format:1.0; Bypass signal of cac.
    vs_bool_t    bypass_radial_shading;             ///<   RW; Range:[0, 1]; Format:1.0; Bypass signal of radial shading.
    vs_bool_t    bypass_mesh_shading;               ///<   RW; Range:[0, 1]; Format:1.0; Bypass signal of mesh shading.
    vs_bool_t    bypass_white_balance;              ///<   RW; Range:[0, 1]; Format:1.0; Bypass signal of white balance.
    vs_bool_t    bypass_dra_gain;                   ///<   RW; Range:[0, 1]; Format:1.0; Bypass signal of dra gain.
    vs_bool_t    bypass_dra;                        ///<   RW; Range:[0, 1]; Format:1.0; Bypass signal of dra.
    vs_bool_t    bypass_demosaic;                   ///<   RW; Range:[0, 1]; Format:1.0; Bypass signal of demosaic.
    vs_bool_t    bypass_pf_correction;              ///<   RW; Range:[0, 1]; Format:1.0; Bypass signal of pfc.
    vs_bool_t    bypass_ccm;                        ///<   RW; Range:[0, 1]; Format:1.0; Bypass signal of ccm.
    vs_bool_t    bypass_cnr;                        ///<   RW; Range:[0, 1]; Format:1.0; Bypass signal of cnr.
    vs_bool_t    bypass_3d_lut;                     ///<   RW; Range:[0, 1]; Format:1.0; Bypass signal of 3d_lut.
    vs_bool_t    bypass_nonequ_gamma;               ///<   RW; Range:[0, 1]; Format:1.0; Bypass signal of nonequ gamma.
    vs_bool_t    bypass_gamma;                      ///<   RW; Range:[0, 1]; Format:1.0; Bypass signal of gamma.
    vs_bool_t    bypass_rgb_sharpen;                ///<   RW; Range:[0, 1]; Format:1.0; Bypass signal of rgb sharpen.
    vs_bool_t    bypass_cs_conv;                    ///<   RW; Range:[0, 1]; Format:1.0; Bypass signal of csc.
    vs_bool_t    bypass_pre_sharpen;                ///<   RW; Range:[0, 1]; Format:1.0; Bypass signal of presharpen.
    vs_bool_t    bypass_nr3d;                       ///<   RW; Range:[0, 1]; Format:1.0; Bypass signal of nr3d.
    vs_bool_t    bypass_post_sharpen;               ///<   RW; Range:[0, 1]; Format:1.0; Bypass signal of postsharpen.
    vs_bool_t    bypass_yuv_pfc;                    ///<   RW; Range:[0, 1]; Format:1.0; Bypass signal of yuv pfc.
    vs_bool_t    bypass_coloring;                   ///<   RW; Range:[0, 1]; Format:1.0; Bypass signal of coloring.
    vs_bool_t    bypass_sa;                         ///<   RW; Range:[0, 1]; Format:1.0; Bypass signal of sa.
} vs_isp_bypass_setting_s;

/**
 * @brief Defines vs isp blc manual attribute
 * @details
 */
typedef struct vs_isp_blc_manual_attr {
    vs_uint16_t   black_level[VS_ISP_BLC_SIZE];         ///<   RW; Range: [0x0, 0xFFF];  Manual R Gr Gb B channel black level value.
} vs_isp_blc_manual_attr_s;

/**
 * @brief Defines vs isp blc auto attribute
 * @details
 */
typedef struct vs_isp_blc_auto_attr {
    vs_uint16_t   black_level[VS_ISP_AUTO_ISO_STRENGTH_SIZE][VS_ISP_BLC_SIZE];    ///<   RW; Range: [0x0, 0xFFF];  Auto R Gr Gb B channel black level value.
} vs_isp_blc_auto_attr_s;

/**
 * @brief Defines ISP black level algorithm attribute
 * @details
 */
typedef struct vs_isp_black_level_attr {
    vs_isp_op_type_e  optype;                    ///<   RW; Range:[0, 1]; Format:1.0; Operation type.
    vs_isp_blc_manual_attr_s  manual_attr;
    vs_isp_blc_auto_attr_s    auto_attr;
} vs_isp_black_level_attr_s;

/**
 * @brief Defines ISP green equal algorithm attribute
 * @details
 */
typedef struct vs_isp_green_equal_attr {
    vs_bool_t    ge_enable;     ///<   RW; Range:[0, 1]; Format:1.0;
    vs_uint16_t  debug_sel;     ///<   RW; Range:[0x0, 0xFFFF]; Green equalization blending mask.
    vs_uint8_t   ge_strength;   ///<   RW; Range:[0x0, 0xFF]; Green equalization strength.
    vs_uint16_t  ge_thresh;     ///<   RW; Range:[0x0, 0xFFFF]; Green equalization threshold.
    vs_uint16_t  ge_slope;      ///<   RW; Range:[0x0, 0xFFF]; Green equalization slope.
    vs_uint8_t   ge_sens;       ///<   RW; Range:[0x0, 0xFF]; Green equalization sensitivity.
} vs_isp_green_equal_attr_s;

/**
 * @brief Defines vs isp dynamic dpc manual attribute
 * @details
 */
typedef struct vs_isp_dynamic_dpc_manual_attr {
    vs_uint16_t   dp_slope;                                       ///<   RW; Range:[0x0, 0xFFF]; Alpha blend between original and corrected images.
    vs_uint16_t   dp_thresh;                                      ///<   RW; Range:[0x0, 0xFFF]; Dynamic dpc strength.
    vs_uint16_t   dpdev_thresh;                                   ///<   RW; Range:[0x0, 0xFFFF]; False color strength on edges.
    vs_uint8_t    dp_blend;                                       ///<   RW; Range:[0x0, 0xFF]; Alpha blend between directional and non-directional replacement values.
} vs_isp_dynamic_dpc_manual_attr_s;

/**
 * @brief Defines vs isp dynamic dpc auto attribute
 * @details
 */
typedef struct vs_isp_dynamic_dpc_auto_attr {
    vs_uint16_t   dp_slope[VS_ISP_AUTO_ISO_STRENGTH_SIZE];         ///<   RW; Range:[0x0, 0xFFF]; Alpha blend between original and corrected images.
    vs_uint16_t   dp_thresh[VS_ISP_AUTO_ISO_STRENGTH_SIZE];        ///<   RW; Range:[0x0, 0xFFF]; Dynamic dpc strength.
    vs_uint16_t   dpdev_thresh[VS_ISP_AUTO_ISO_STRENGTH_SIZE];     ///<   RW; Range:[0x0, 0xFFFF]; False color strength on edges.
    vs_uint8_t    dp_blend[VS_ISP_AUTO_ISO_STRENGTH_SIZE];         ///<   RW; Range:[0x0, 0xFF]; Alpha blend between directional and non-directional replacement values.
} vs_isp_dynamic_dpc_auto_attr_s;

/**
 * @brief Defines ISP dynamic defect point algorithm attribute
 * @details
 */
typedef struct vs_isp_dynamic_dp_attr {
    vs_bool_t     dp_enable;                                      ///<   RW; Range:[0, 1]; Format:1.0; Enable signal of dynamic dpc.
    vs_bool_t     show_dynamic_dp;                                ///<   RW; Range:[0, 1]; Format:1.0; Display pixel removed by dynamic dpc.
    vs_bool_t     dark_disable;                                   ///<   RW; Range:[0x0, 0x1]; Dark defect pixel disable.
    vs_bool_t     bright_disable;                                 ///<   RW; Range:[0x0, 0x1]; Bright defect pixel disable.

    vs_uint16_t   line_thresh;                                    ///<   RW; Range:[0x0, 0xFFFF]; Threshold of a defect pixel with direction.
    vs_uint16_t   sigma_in;                                       ///<   RW; Range:[0x0, 0xFFFF]; Manually override noise estimation.
    vs_uint8_t    thresh_short;                                   ///<   RW; Range:[0x0, 0xFF]; Noise threshold for short exposure data.
    vs_uint8_t    thresh_long;                                    ///<   RW; Range:[0x0, 0xFF]; Noise threshold for long exposure data.

    vs_isp_op_type_e  optype;                                     ///<   RW; Range:[0, 1]; Format:1.0; Operation type.
    vs_isp_dynamic_dpc_manual_attr_s  manual_attr;
    vs_isp_dynamic_dpc_auto_attr_s    auto_attr;
} vs_isp_dynamic_dp_attr_s;

/**
 * @brief Defines ISP static defect point algorithm attribute
 * @details
 */
typedef struct vs_isp_static_dp_attr {
    vs_bool_t    point_reset;       ///<   RW; Range:[0, 1]; Format:1.0; Switch of reset static defec-pixel table.
    vs_bool_t    show_reference;    ///<   RW; Range:[0, 1]; Format:1.0; Show reference detect pixel.
    vs_bool_t    correction_enable; ///<   RW; Range:[0, 1]; Format:1.0; Enable of static dpc.
    vs_bool_t    show_static_dp;    ///<   RW; Range:[0, 1]; Format:1.0; Show which pixels have been detected as bad.
    vs_bool_t    detection_enable;  ///<   RW; Range:[0, 1]; Format:1.0; Switch of detection.
    vs_uint16_t      dp_count_in;   ///<   RW; Range:[0x0, 0xFFF]; Number of defect-pixels in the written table.
    vs_uint16_t      dp_count;      ///<   R; Range:[0x0, 0xFFF]; Number of detect-pixels detected.
    vs_uint16_t      table_start;   ///<   R; Range:[0x0, 0xFFF]; Address of first detect-pixels in defec-pixel store.
    vs_uint32_t      table[VS_ISP_STATIC_DP_LUT_SIZE];   ///<   RW; Range:[0x0, 0x1FFFFFF]; defect pixel table.
} vs_isp_static_dp_attr_s;

/**
 * @brief Defines vs isp 2dnr manual attribute
 * @details
 */
typedef struct vs_isp_nr2d_manual_attr {
    vs_uint16_t   int_config;                ///<   RW; Range:[0x0, 0xF]; Alpha blend between RAW and intensity image.
    vs_uint16_t   sad_filt_thresh;           ///<   RW; Range:[0x0, 0xFF]; Balanced contrast between edge detail and flat region smoothness.
    vs_uint16_t   thresh_1;                  ///<   RW; Range:[0x0, 0xFF]; Noise threshold for high frequency.
    vs_uint16_t   thresh_4;                  ///<   RW; Range:[0x0, 0xFF]; Noise threshold for low frequency.
    vs_uint16_t   strength1;                 ///<   RW; Range:[0x0, 0xFF]; De-noising strength for high frequency.
    vs_uint16_t   strength4;                 ///<   RW; Range:[0x0, 0xFF]; De-noising strength for low frequency.
    vs_uint16_t   global_offset;             ///<   RW; Range:[0x0, 0xFF]; De-noising global strength.
    vs_uint16_t   nr2d_strength_mc_contrast; ///<   RW; Range:[0x0, 0xFFFF]; Motion compensation strength.
} vs_isp_nr2d_manual_attr_s;

/**
 * @brief Defines vs isp 2dnr auto attribute
 * @details
 */
typedef struct vs_isp_nr2d_auto_attr {
    vs_uint16_t   int_config[VS_ISP_AUTO_ISO_STRENGTH_SIZE];                ///<   RW; Range:[0x0, 0xF]; Alpha blend between RAW and intensity image.
    vs_uint16_t   sad_filt_thresh[VS_ISP_AUTO_ISO_STRENGTH_SIZE];           ///<   RW; Range:[0x0, 0xFF]; Balanced contrast between edge detail and flat region smoothness.
    vs_uint16_t   thresh_1[VS_ISP_AUTO_ISO_STRENGTH_SIZE];                  ///<   RW; Range:[0x0, 0xFF]; Noise threshold for high frequency.
    vs_uint16_t   thresh_4[VS_ISP_AUTO_ISO_STRENGTH_SIZE];                  ///<   RW; Range:[0x0, 0xFF]; Noise threshold for low frequency.
    vs_uint16_t   strength1[VS_ISP_AUTO_ISO_STRENGTH_SIZE];                 ///<   RW; Range:[0x0, 0xFF]; De-noising strength for high frequency.
    vs_uint16_t   strength4[VS_ISP_AUTO_ISO_STRENGTH_SIZE];                 ///<   RW; Range:[0x0, 0xFF]; De-noising strength for low frequency.
    vs_uint16_t   global_offset[VS_ISP_AUTO_ISO_STRENGTH_SIZE];             ///<   RW; Range:[0x0, 0xFF]; De-noising global strength.
    vs_uint16_t   nr2d_strength_mc_contrast[VS_ISP_AUTO_ISO_STRENGTH_SIZE]; ///<   RW; Range:[0x0, 0xFFFF]; Motion compensation strength.
} vs_isp_nr2d_auto_attr_s;

/**
 * @brief Defines ISP 2dnr algorithm attribute
 * @details
 */
typedef struct vs_isp_nr2d_attr {
    vs_bool_t    enable;                                        ///<   RW; Range:[0, 1]; Format:1.0; Enable singnal of 2dnr.
    vs_uint16_t  fs_mc_off;                                     ///<   RW; Range:[0x0, 0xFFFF]; Motion compensation threshold.
    vs_uint8_t   view_filter;                                   ///<   RW; Range:[0x0, 0x3]; Debug parameter, can view different scales.
    vs_uint8_t   scale_mode;                                    ///<   RW; Range:[0x0, 0x3]; Scale number.
    vs_bool_t    filter_select;                                 ///<   RW; Range:[0, 1]; Format:1.0; Selects different filter modes.
    vs_bool_t    int_select;                                    ///<   RW; Range:[0, 1]; Format:1.0; De-noising on either intensity or RAW image.
    vs_bool_t    nlm_en;                                        ///<   RW; Range:[0, 1]; Format:1.0; Enable signal of non-local mean filtering.
    vs_bool_t    nonlinear_wkgen;                               ///<   RW; Range:[0, 1]; Format:1.0; Enable signal SAD filtering.
    vs_bool_t    rm_enable;                                     ///<   RW; Range:[0, 1]; Format:1.0; Enable signal radial de-noising.
    vs_uint16_t  rm_center_x;                                   ///<   RW; Range:[0x0, 0xFFFF]; Central x-axis coordinate.
    vs_uint16_t  rm_center_y;                                   ///<   RW; Range:[0x0, 0xFFFF]; Central y-axis coordinate.
    vs_uint16_t  rm_off_center_mult;                            ///<   RW; Range:[0x0, 0xFFFF]; Scales radial lut to fit center-to-corner dimension.
    vs_uint8_t   rm_shading_lut[VS_ISP_RM_SHADING_LUT_SIZE];    ///<   RW; Range:[0x0, 0xFF]; LUT of radial correction values.
    vs_uint8_t   thresh_0h;                                     ///<   RW; Range:[0x0, 0xFF]; Noise threshold for high horizontal frequencies.
    vs_uint8_t   thresh_2h;                                     ///<   RW; Range:[0x0, 0xFF]; Noise threshold for low horizontal frequencies.
    vs_uint8_t   thresh_0v;                                     ///<   RW; Range:[0x0, 0xFF]; Noise threshold for high vertical frequencies.
    vs_uint8_t   thresh_2v;                                     ///<   RW; Range:[0x0, 0xFF]; Noise threshold for low vertical frequencies.
    vs_uint8_t   strength0;                                     ///<   RW; Range:[0x0, 0xFF]; Unused - no effect.
    vs_uint8_t   strength2;                                     ///<   RW; Range:[0x0, 0xFF]; Unused - no effect.
    vs_uint8_t   np_weight_lut[VS_ISP_NP_WEIGHT_LUT_SIZE];      ///<   RW; Range:[0x0, 0xFF]; Noise profile controls by 2dnr.
    vs_bool_t    use_lut;                                       ///<   RW; Range:[0, 1]; Format:1.0; Signal of use lookup tables
    vs_bool_t    use_exp_mask;                                  ///<   RW; Range:[0, 1]; Format:1.0; Switch of exposure mask.
    vs_bool_t    black_reflect;                                 ///<   RW; Range:[0, 1]; Format:1.0; Select below black level data processing mode.
    vs_uint16_t  black_level;                                   ///<   Rnage:[0x0, 0xFFFF]; Black level.
    vs_uint16_t  thresh[VS_ISP_NOISE_THRESH_SIZE];              ///<   RW; Range:[0x0, 0xFFFF]; Exposure threshold.
    vs_uint8_t   noise_level[VS_ISP_NOISE_LEVEL_SIZE];          ///<   RW; Range:[0x0, 0xFF]; Noise level of VS/S/M/L frame exposure.
    vs_isp_op_type_e  optype;                                   ///<   RW; Range:[0, 1]; Format:1.0; Operation type.
    vs_isp_nr2d_manual_attr_s manual_attr;
    vs_isp_nr2d_auto_attr_s   auto_attr;
} vs_isp_nr2d_attr_s;

/**
 * @brief Defines ISP color adjust mesh configure attribute
 * @details
 */
typedef struct vs_isp_cac_mesh_cfg {
    vs_uint16_t min_correction; ///<   RW; Range:[0x0, 0xFFFF]; Minimum correction factor.
    vs_uint16_t mesh_width;     ///<   RW; Range:[0x0, 64]; Mesh width.
    vs_uint16_t mesh_height;    ///<   RW; Range:[0x0, 64]; Mesh height.
}vs_isp_cac_mesh_cfg_s;

/**
 * @brief Defines ISP color adjust algorithm attribute
 * @details
 */
typedef struct vs_isp_cac_attr {
    vs_bool_t    enable;                                                ///<   RW; Range:[0, 1]; Format:1.0; Enable signal of cac.
    vs_uint8_t   mesh_scale;                                            ///<   RW; Range:[0x0, 0x3]; Mesh of horizontal and vertical correction values.
    vs_uint32_t  cac_filter_mem[VS_ISP_CAC_MEM_SIZE];                   ///<   RW; Range:[0x0, 0xFFFFFFFF]; FIR filter coefficients used for shift.
    vs_uint32_t  cac_correction_mem[VS_ISP_CAC_CORRECTION_MEM_SIZE];    ///<   RW; Range:[0x0, 0xFFFFFFFF]; Coefficient used for cac_correction_mem product.
    vs_isp_cac_mesh_cfg_s mesh_cfg;
} vs_isp_cac_attr_s;

/**
 * @brief Defines ISP radial shading algorithm attribute
 * @details
 */
typedef struct vs_isp_radial_shading_attr {
    vs_bool_t    enable;                                            ///<   RW; Range:[0, 1]; Format:1.0; Enable signal of radial shading.
    vs_uint16_t  center_x[VS_ISP_RADIAL_CENTER_SIZE];               ///<   RW; Range:[0x0, 0xFFFF]; X-coordinate of R G B IR channel.
    vs_uint16_t  center_y[VS_ISP_RADIAL_CENTER_SIZE];               ///<   RW; Range:[0x0, 0xFFFF]; Y-coordinate of R G B IR channel.
    vs_uint16_t  off_center_x[VS_ISP_RADIAL_CENTER_SIZE];           ///<   RW; Range:[0x0, 0xFFFF]; X-coordinate scales radial of R G B IR channel.
    vs_uint16_t  off_center_y[VS_ISP_RADIAL_CENTER_SIZE];           ///<   RW; Range:[0x0, 0xFFFF]; Y-coordinate scales radial of R G B IR channel.
    vs_uint16_t  shading_radial_r[VS_ISP_RADIAL_SHADING_LUT_SIZE];  ///<   RW; Range:[0x0, 0xFFFF]; Correction value of R channel.
    vs_uint16_t  shading_radial_g[VS_ISP_RADIAL_SHADING_LUT_SIZE];  ///<   RW; Range:[0x0, 0xFFFF]; Correction value of G channel.
    vs_uint16_t  shading_radial_b[VS_ISP_RADIAL_SHADING_LUT_SIZE];  ///<   RW; Range:[0x0, 0xFFFF]; Correction value of B channel.
    vs_uint16_t  shading_radial_ir[VS_ISP_RADIAL_SHADING_LUT_SIZE]; ///<   RW; Range:[0x0, 0xFFFF]; Correction value of IR channel.
} vs_isp_radial_shading_attr_s;

/**
 * @brief Defines vs isp mesh shading algorithm manual attribute
 * @details
 */
typedef struct vs_isp_mesh_shading_manual_attr {
    vs_uint16_t  mesh_strength;                                      ///<   RW; Range:[0x0, 0xFFFF]; Mesh correction strength.
} vs_isp_mesh_shading_manual_attr_s;

/**
 * @brief Defines vs isp mesh shading algorithm auto attribute
 * @details
 */
typedef struct vs_isp_mesh_shading_auto_attr {
    vs_uint16_t  mesh_strength[VS_ISP_AUTO_ISO_STRENGTH_SIZE];       ///<   RW; Range:[0x0, 0xFFFF]; Mesh correction strength.
} vs_isp_mesh_shading_auto_attr_s;

/**
 * @brief Defines vs isp mesh shading algorithm static attribute
 * @details
 */
typedef struct vs_isp_mesh_shading_static_attr {
    vs_bool_t     static_enable;                                       ///<   RW; Range:[0, 1]; Format:1.0; Static signal of mesh shading.
    vs_uint8_t    mesh_alpha_bank[VS_ISP_MESH_ALPHA_SIZE];             ///<   RW; Range:[0x0, 0x7]; Mesh alpha bank of R G B IR chanel.
    vs_uint8_t    mesh_alpha[VS_ISP_MESH_ALPHA_SIZE];                  ///<   RW; Range:[0x0, 0xFF]; Alpha blend coeff bank of R G B IR chanel.
} vs_isp_mesh_shading_static_attr_s;

/**
 * @brief Defines ISP mesh shading algorithm attribute
 * @details
 */
typedef struct vs_isp_mesh_shading_attr {
    vs_bool_t     enable;                                           ///<   RW; Range:[0, 1]; Format:1.0; Enable signal of mesh shading.
    vs_bool_t     mesh_show;                                        ///<   RW; Range:[0, 1]; Show mesh shading.
    vs_uint8_t    mesh_scale;                                       ///<   RW; Range:[0x0, 0x7]; Mesh coefficient.
    vs_uint8_t    mesh_alpha_mode;                                  ///<   RW; Range:[0x0, 0x3]; Mode of alpha blending between mesh shading tables.
    vs_uint8_t    mesh_page[VS_ISP_MESH_PAGE_SIZE];                 ///<   RW; Range:[0x0, 0x3]; Mesh page of R G B IR chanel.
    vs_uint8_t    mesh_width;                                       ///<   RW; Range:[0x0, 0x3F]; Mesh width.
    vs_uint8_t    mesh_height;                                      ///<   RW; Range:[0x0, 0x3F]; Mesh height.
    vs_uint8_t    shading_ls_d65_r[VS_ISP_MESH_SHADING_LUT_SIZE];   ///<   RW; Range:[0, 0xFF]; D65 light shading correction value of G chanel.
    vs_uint8_t    shading_ls_d65_g[VS_ISP_MESH_SHADING_LUT_SIZE];   ///<   RW; Range:[0, 0xFF]; D65 light shading correction value of G chanel.
    vs_uint8_t    shading_ls_d65_b[VS_ISP_MESH_SHADING_LUT_SIZE];   ///<   RW; Range:[0, 0xFF]; D65 light shading correction value of B chanel.
    vs_uint8_t    shading_ls_tl84_r[VS_ISP_MESH_SHADING_LUT_SIZE];  ///<   RW; Range:[0, 0xFF]; TL84 light shading correction value of R chanel.
    vs_uint8_t    shading_ls_tl84_g[VS_ISP_MESH_SHADING_LUT_SIZE];  ///<   RW; Range:[0, 0xFF]; TL84 light shading correction value of G chanel.
    vs_uint8_t    shading_ls_tl84_b[VS_ISP_MESH_SHADING_LUT_SIZE];  ///<   RW; Range:[0, 0xFF]; TL84 light shading correction value of B chanel.
    vs_uint8_t    shading_ls_a_r[VS_ISP_MESH_SHADING_LUT_SIZE];     ///<   RW; Range:[0, 0xFF]; A light shading correction value of R chanel.
    vs_uint8_t    shading_ls_a_g[VS_ISP_MESH_SHADING_LUT_SIZE];     ///<   RW; Range:[0, 0xFF]; A light shading correction value of G chanel.
    vs_uint8_t    shading_ls_a_b[VS_ISP_MESH_SHADING_LUT_SIZE];     ///<   RW; Range:[0, 0xFF]; A light shading correction value of B chanel.
    vs_isp_mesh_shading_static_attr_s  static_attr;
    vs_isp_op_type_e  optype;                                       ///<   RW; Range:[0, 1]; Format:1.0; Operation type.
    vs_isp_mesh_shading_manual_attr_s manual_attr;
    vs_isp_mesh_shading_auto_attr_s   auto_attr;
}vs_isp_mesh_shading_attr_s;

typedef struct vs_isp_dra_static_manual_attr {
    vs_uint16_t       strength_inroi; ///<   RW; Range:[0x0, 0x3FF]; Dynamic range application strength.
    vs_uint16_t       dark_enh;       ///<   RW; Range:[0x0, 0xFFFF]; Dark areas increase strength.
} vs_isp_dra_static_manual_attr_s;

typedef struct vs_isp_dra_static_auto_attr {
    vs_uint16_t       strength_inroi[VS_ISP_AUTO_ISO_STRENGTH_SIZE];        ///<   RW; Range:[0x0, 0x3FF]; Dynamic range application strength.
    vs_uint16_t       dark_enh[VS_ISP_AUTO_ISO_STRENGTH_SIZE];              ///<   RW; Range:[0x0, 0xFFFF]; Dark areas increase strength.
} vs_isp_dra_static_auto_attr_s;

/**
 * @brief Defines vs isp dra algorithm static attribute
 * @details
 */
typedef struct vs_isp_dra_static_attr {
    vs_bool_t     static_enable;                  ///<   RW; Range:[0, 1]; Format:1.0; Static signal of dra.
    vs_isp_op_type_e optype;                      ///<   RW; Range:[0, 1]; Format:1.0; Operation type.
    vs_isp_dra_static_manual_attr_s manual_attr;
    vs_isp_dra_static_auto_attr_s auto_attr;
} vs_isp_dra_static_attr_s;

typedef struct vs_isp_dra_manual_attr {
    vs_uint8_t    svariance; ///<   RW; Range:[0x0, 0xF]; Sensitivity of DRA to regional variations.
    vs_uint32_t   fwd_alpha; ///<   RW; Range:[0x0, 0x3FFFF]; Alpha for gamma_dl.
    vs_uint32_t   rev_alpha; ///<   RW; Range:[0x0, 0x3FFFF]; Alpha for gamma_dl.
} vs_isp_dra_manual_attr_s;

typedef struct vs_isp_dra_auto_attr {
    vs_uint8_t    svariance[VS_ISP_AUTO_ISO_STRENGTH_SIZE]; ///<   RW; Range:[0x0, 0xF]; Ensitivity of DRA to regional variations.
    vs_uint32_t   fwd_alpha[VS_ISP_AUTO_ISO_STRENGTH_SIZE]; ///<   RW; Range:[0x0, 0x3FFFF]; Alpha for gamma_dl.
    vs_uint32_t   rev_alpha[VS_ISP_AUTO_ISO_STRENGTH_SIZE]; ///<   RW; Range:[0x0, 0x3FFFF]; Alpha for gamma_dl.
} vs_isp_dra_auto_attr_s;

typedef struct vs_isp_dra8_control_attr {
    vs_uint32_t   dark_prc;                  ///<   RW; Range:[0x0, 0x64]; Define dark areas as a percentage of the image.
    vs_uint32_t   bright_prc;                ///<   RW; Range:[0x0, 0x64]; Define bright areas as a percentage of the image.
    vs_uint32_t   min_dk;                    ///<   RW; Range:[0x0, 0xFFFF]; Minimum applicable dark_enh.
    vs_uint32_t   max_dk;                    ///<   RW; Range:[0x0, 0xFFFF]; Maximum applicable dark_enh.
    vs_uint32_t   pd_cut_min;                ///<   RW; Range:[0x0, 0x3FF]; Minimum intensity cut for dark regions in which dark_enh will be applied.
    vs_uint32_t   pd_cut_max;                ///<   RW; Range:[0x0, 0x3FF]; Maximum intensity cut for dark regions in which dark_enh will be applied.
    vs_uint32_t   dark_contrast_min;         ///<   RW; Range:[0x1, 0x40000]; Minimum contrast driving dark_enh.
    vs_uint32_t   dark_contrast_max;         ///<   RW; Range:[0x1, 0x40000]; Maximum contrast driving dark_enh.
    vs_uint32_t   min_str;                   ///<   RW; Range:[0x0, 0x400]; Minimum dra8 strength.
    vs_uint32_t   max_str;                   ///<   RW; Range:[0x0, 0x400]; Maximum dra8 strength.
    vs_uint32_t   dark_prc_gain_target;      ///<   RW; Range:[0x0, 0x64]; Target in histogarm(percentage) for dark_prc.
    vs_uint32_t   contrast_min;              ///<   RW; Range:[0x1, 0x40000]; Clip factor of strength for LDR scene.
    vs_uint32_t   contrast_max;              ///<   RW; Range:[0x1, 0x40000]; Clip factor of strength for HDR scene.
    vs_uint32_t   max_dra_gain;              ///<   RW; Range:[0x0, 0xFFF]; Maximum dra gain.
    vs_uint32_t   debug;                     ///<   RW; Range:[0x0, 0x1]; Dra8 debug mode for displays all parameter information.

} vs_isp_dra8_control_attr_s;
/**
 * @brief Defines ISP dra algorithm attribute
 * @details
 */
typedef struct vs_isp_dra_attr {
    vs_uint32_t   offset;                                                           ///<   RW; Range:[0x0, 0xFFFFF]; Data black level..
    vs_bool_t     dra_on;                                                           ///<   RW; Range:[0, 1]; Format:1.0; Enable signal of dra.
    vs_uint32_t   dra_asymmetry_lut[VS_ISP_DRA_ASYMMETRY_SIZE];                     ///<   RW; Range:[0x0, 0xFFFFF]; Dra target curve.
    vs_uint8_t    bright_pr;                                                        ///<   RW; Range:[0x0, 0xFF]; Bright preserve value to control dra core.
    vs_uint8_t    contrast;                                                         ///<   RW; Range:[0x0, 0xFF]; Dra contrast value.
    vs_uint8_t    variance_space;                                                   ///<   RW; Range:[0x0, 0xF]; Degree of spatial sensitivity of the algorithm.
    vs_uint8_t    variance_intensity;                                               ///<   RW; Range:[0x0, 0xF]; Degree of luminance sensitivity of the algorithm.
    vs_uint8_t    slope_max;                                                        ///<   RW; Range:[0x0, 0xFF]; Maximum slope (gain) of adaptive algorithm.
    vs_uint8_t    slope_min;                                                        ///<   RW; Range:[0x0, 0xFF]; Minimum slope (gain) of adaptive algorithm.
    vs_uint32_t   black_level;                                                      ///<   RW; Range:[0x0, 0xFFFFF]; Dra lower limit,values below this will not be affected by dra.
    vs_uint32_t   white_level;                                                      ///<   RW; Range:[0x0, 0xFFFFF]; Dra upper limit,values above this will not be affected by dra.
    vs_uint8_t    fwd_percept_control;                                              ///<   RW; Range:[0x0, 0x3]; Dra gamma processing select: 0=pass, 1=gamma_dl, 2=sqrt, 3=gamma_lut.
    vs_uint8_t    rev_percept_control;                                              ///<   RW; Range:[0x0, 0x3]; Dra gamma processing select: 0=pass, 1=gamma_dl, 2=sqrt, 3=gamma_lut.
    vs_uint16_t   strength_outroi;                                                  ///<   RW; Range:[0x0, 0x3FF]; Strength value for outside of ROI.
    vs_uint16_t   roi_hor_start;                                                    ///<   RW; Range:[0x0, 0xFFFF]; Horizontal starting point of ROI.
    vs_uint16_t   roi_hor_end;                                                      ///<   RW; Range:[0x0, 0xFFFF]; Horizontal ending point of ROI.
    vs_uint16_t   roi_ver_start;                                                    ///<   RW; Range:[0x0, 0xFFFF]; Vertical starting point of ROI.
    vs_uint16_t   roi_ver_end;                                                      ///<   RW; Range:[0x0, 0xFFFF]; Vertical ending point of ROI.
    vs_bool_t     filter_mux;                                                       ///<   RW; Range:[0, 1]; Select between dra8 and dra7, 1=dra8 and 0=dra7.
    vs_uint32_t   wb_offset;                                                        ///<   RW; Range:[0x0, 0xFFFFF]; White balance offset.
    vs_uint16_t   gain[VS_ISP_DRA_GAIN_SIZE];                                       ///<   RW; Range:[0x0, 0xFFF]; White balance gain of rggb chanel.
    vs_bool_t     gtm_select;                                                       ///<   RW; Range:[0, 1]; Global tone map select: 0:Local TM 1:Full Global TM;
    vs_uint32_t   globaltm_x_lut[VS_ISP_GLOBALTM_LUT_SIZE];                         ///<   RW; Range:[0x0, 0xFFFFF]; Tone x direction curve.
    vs_uint32_t   globaltm_y_lut[VS_ISP_GLOBALTM_LUT_SIZE];                         ///<   RW; Range:[0x0, 0xFFFFF]; Tone y direction curve.
    vs_uint8_t    dra_strength_maximum;                                             ///<   RW; Range:[0x0, 0xFF]; Maximum dra strength.
    vs_uint16_t   dra_min_max_str;                                                  ///<   RW; Range:[0x0, 0xFF]; Minimum value of Maximum dra strength.
    vs_uint8_t    dra_avg_coef;                                                     ///<   RW; Range:[0x0, 0xFF]; Dra average coeff.
    vs_uint32_t   dra_ev_lim_no_str[VS_ISP_DRA_EV_LIM_NO_STR_SIZE];                 ///<   RW; Range:[0x0, 0xFFFFFFFF]; Below this EV value, DRA dark enh is set to Min_dk.
    vs_uint32_t   dra_ev_lim_full_str;                                              ///<   RW; Range:[0x0, 0xFFFFFFFF]; Above this EV value, DRA strength is set to the Max_dk.
    vs_isp_dra8_control_attr_s  dra8_strength_dk_enh_control;                       ///<   RW; Range:[0x0, 0xFFFFFFFF];
    vs_isp_dra_static_attr_s  static_attr;
    vs_isp_op_type_e optype;                                                        ///<   RW; Range:[0, 1]; Format:1.0; Operation type.
    vs_isp_dra_manual_attr_s manual_attr;
    vs_isp_dra_auto_attr_s   auto_attr;
} vs_isp_dra_attr_s;

/**
 * @brief Defines vs isp demosaic algorithm manual attribute
 * @details
 */
typedef struct vs_isp_demosaic_manual_attr {
    vs_uint16_t  sharp_alt_lu;                              ///<   RW; Range:[0x0, 0xFF]; Low frequency sharpening strength.
    vs_uint16_t  sharp_alt_ldu;                             ///<   RW; Range:[0x0, 0xFF]; Medium frequency sharpening strength.
    vs_uint16_t  sharp_alt_ld;                              ///<   RW; Range:[0x0, 0xFF]; High frequency sharpening strength.
    vs_uint16_t  uu_slope;                                  ///<   RW; Range:[0x0, 0xFF]; Slope of all edge mixing thresholds.
    vs_uint16_t  np_offset;                                 ///<   RW; Range:[0x0, 0xFF]; Image noise parameters.

    vs_uint16_t   uu_thresh;                                ///<   RW; Range:[0x0, 0xFFF]; Threshold for all edge mixing ranges.
    vs_uint16_t   uu_offset;                                ///<   RW; Range:[0x0, 0xFFF]; Offset for all edge mixing ranges.
    vs_uint8_t    uu_sh_slope;                              ///<   RW; Range:[0x0, 0xFF]; Slope of all edge mixing thresholds.
    vs_uint16_t   uu_sh_thresh;                             ///<   RW; Range:[0x0, 0xFFF]; Threshold for all edge mixing ranges.
    vs_uint16_t   uu_sh_offset;                             ///<   RW; Range:[0x0, 0xFFF]; Offset for all edge mixing ranges.
    vs_uint8_t    vh_slope;                                 ///<   RW; Range:[0x0, 0xFF]; Slope of the vertical and horizontal edge mixture thresholds.
    vs_uint16_t   vh_thresh;                                ///<   RW; Range:[0x0, 0xFFF]; Threshold of the vertical and horizontal edge mixture.
    vs_uint8_t    aa_slope;                                 ///<   RW; Range:[0x0, 0xFF]; Slope of the 45 degree and 135 degree edge mixing threshold.
    vs_uint16_t   aa_thresh;                                ///<   RW; Range:[0x0, 0xFFF]; Threshold for 45 degree and 135 degree edge mixture.
    vs_uint8_t    va_slope;                                 ///<   RW; Range:[0x0, 0xFF]; Slope of vertical, horizontal, 45 degree and 135 degree edge mixture thresholds.
    vs_uint16_t   va_thresh;                                ///<   RW; Range:[0x0, 0xFFF]; Threshold of vertical, horizontal, 45 degree and 135 degree edge mixture.

    vs_uint8_t    fc_alias_slope;                           ///<   RW; Range:[0x0, 0xFF]; Slope(strength) of false colour correction after blending with saturation value.
    vs_uint8_t    fc_alias_thresh;                          ///<   RW; Range:[0x0, 0xFF]; Threshold of false colour correction after blending with saturation value.
    vs_uint8_t    fc_slope;                                 ///<   RW; Range:[0x0, 0xFF]; Slope(strength) of false color correction.
    vs_uint16_t   luma_thresh_low_d;                        ///<   RW; Range:[0x0, 0xFFF]; Threshold of sharpening brightness, intensity values above this value will be sharpen.
    vs_uint8_t    luma_offset_low_d;                        ///<   RW; Range:[0x0, 0xFF]; Linear threshold offset corresponding to luma_thresh_low_d.
    vs_uint32_t   luma_slope_low_d;                         ///<   RW; Range:[0x0, 0xFFFFF]; Linear threshold slope corresponding to luma_thresh_low_d.
    vs_uint16_t   luma_thresh_high_d;                       ///<   RW; Range:[0x0, 0xFFF]; Threshold of sharpening brightness, intensity values below this value will be sharpen.
    vs_uint32_t   luma_slope_high_d;                        ///<   RW; Range:[0x0, 0xFFFFF]; Linear threshold slope corresponding to luma_thresh_high_d.
    vs_uint16_t   luma_thresh_low_ud;                       ///<   RW; Range:[0x0, 0xFFF]; Threshold of no directional sharpening brightness, intensity values above this value will be sharpen.
    vs_uint8_t    luma_offset_low_ud;                       ///<   RW; Range:[0x0, 0xFF]; Linear threshold offset corresponding to luma_thresh_low_ud.
    vs_uint32_t   luma_slope_low_ud;                        ///<   RW; Range:[0x0, 0xFFFFF]; Linear threshold slope corresponding to luma_thresh_low_ud.
    vs_uint16_t   luma_thresh_high_ud;                      ///<   RW; Range:[0x0, 0xFFF]; Threshold of no directional sharpening brightness, intensity values below this value will be sharpen.
    vs_uint32_t   luma_slope_high_ud;                       ///<   RW; Range:[0x0, 0xFFFFF]; Linear threshold slope corresponding to luma_thresh_high_ud.

} vs_isp_demosaic_manual_attr_s;

/**
 * @brief Defines vs isp demosaic algorithm manual attribute
 * @details
 */
typedef struct vs_isp_demosaic_auto_attr {
    vs_uint16_t  sharp_alt_lu[VS_ISP_AUTO_ISO_STRENGTH_SIZE];           ///<   RW; Range:[0x0, 0xFF]; Low frequency sharpening strength.
    vs_uint16_t  sharp_alt_ldu[VS_ISP_AUTO_ISO_STRENGTH_SIZE];          ///<   RW; Range:[0x0, 0xFF]; Medium frequency sharpening strength.
    vs_uint16_t  sharp_alt_ld[VS_ISP_AUTO_ISO_STRENGTH_SIZE];           ///<   RW; Range:[0x0, 0xFF]; High frequency sharpening strength.
    vs_uint16_t  uu_slope[VS_ISP_AUTO_ISO_STRENGTH_SIZE];               ///<   RW; Range:[0x0, 0xFF]; Slope of all edge mixing thresholds.
    vs_uint16_t  np_offset[VS_ISP_AUTO_ISO_STRENGTH_SIZE];              ///<   RW; Range:[0x0, 0xFF]; Image noise parameters.
    vs_uint16_t   uu_thresh[VS_ISP_AUTO_ISO_STRENGTH_SIZE];             ///<   RW; Range:[0x0, 0xFFF]; Threshold of all edge mixing ranges.
    vs_uint16_t   uu_offset[VS_ISP_AUTO_ISO_STRENGTH_SIZE];             ///<   RW; Range:[0x0, 0xFFF]; Offset of all edge mixing ranges.
    vs_uint8_t    uu_sh_slope[VS_ISP_AUTO_ISO_STRENGTH_SIZE];           ///<   RW; Range:[0x0, 0xFF]; Slope of all edge mixing thresholds.
    vs_uint16_t   uu_sh_thresh[VS_ISP_AUTO_ISO_STRENGTH_SIZE];          ///<   RW; Range:[0x0, 0xFFF]; Threshold for all edge mixing ranges.
    vs_uint16_t   uu_sh_offset[VS_ISP_AUTO_ISO_STRENGTH_SIZE];          ///<   RW; Range:[0x0, 0xFFF]; Offset for all edge mixing ranges.
    vs_uint8_t    vh_slope[VS_ISP_AUTO_ISO_STRENGTH_SIZE];              ///<   RW; Range:[0x0, 0xFF]; Slope of vertical and horizontal edge mixture thresholds.
    vs_uint16_t   vh_thresh[VS_ISP_AUTO_ISO_STRENGTH_SIZE];             ///<   RW; Range:[0x0, 0xFFF]; Threshold of vertical and horizontal edge mixture.
    vs_uint8_t    aa_slope[VS_ISP_AUTO_ISO_STRENGTH_SIZE];              ///<   RW; Range:[0x0, 0xFF]; Slope of 45 degree and 135 degree edge mixing threshold.
    vs_uint16_t   aa_thresh[VS_ISP_AUTO_ISO_STRENGTH_SIZE];             ///<   RW; Range:[0x0, 0xFFF]; Threshold of 45 degree and 135 degree edge mixture.
    vs_uint8_t    va_slope[VS_ISP_AUTO_ISO_STRENGTH_SIZE];              ///<   RW; Range:[0x0, 0xFF]; Slope of vertical, horizontal, 45 degree and 135 degree edge mixture thresholds.
    vs_uint16_t   va_thresh[VS_ISP_AUTO_ISO_STRENGTH_SIZE];             ///<   RW; Range:[0x0, 0xFFF]; Threshold of vertical, horizontal, 45 degree and 135 degree edge mixture.
    vs_uint8_t    fc_alias_slope[VS_ISP_AUTO_ISO_STRENGTH_SIZE];        ///<   RW; Range:[0x0, 0xFF]; Slope(strength) of false colour correction after blending with saturation value.
    vs_uint8_t    fc_alias_thresh[VS_ISP_AUTO_ISO_STRENGTH_SIZE];       ///<   RW; Range:[0x0, 0xFF]; Threshold of false colour correction after blending with saturation value.
    vs_uint8_t    fc_slope[VS_ISP_AUTO_ISO_STRENGTH_SIZE];              ///<   RW; Range:[0x0, 0xFF]; Slope(strength) of false color correction.
    vs_uint16_t   luma_thresh_low_d[VS_ISP_AUTO_ISO_STRENGTH_SIZE];     ///<   RW; Range:[0x0, 0xFFF]; Threshold of sharpening brightness, intensity values above this value will be sharpen.
    vs_uint8_t    luma_offset_low_d[VS_ISP_AUTO_ISO_STRENGTH_SIZE];     ///<   RW; Range:[0x0, 0xFF]; Linear threshold offset corresponding to luma_thresh_low_d.
    vs_uint32_t   luma_slope_low_d[VS_ISP_AUTO_ISO_STRENGTH_SIZE];      ///<   RW; Range:[0x0, 0xFFFFF]; Linear threshold slope corresponding to luma_thresh_low_d.
    vs_uint16_t   luma_thresh_high_d[VS_ISP_AUTO_ISO_STRENGTH_SIZE];    ///<   RW; Range:[0x0, 0xFFF]; Threshold of sharpening brightness, intensity values below this value will be sharpen.
    vs_uint32_t   luma_slope_high_d[VS_ISP_AUTO_ISO_STRENGTH_SIZE];     ///<   RW; Range:[0x0, 0xFFFFF]; Linear threshold slope corresponding to luma_thresh_high_d.
    vs_uint16_t   luma_thresh_low_ud[VS_ISP_AUTO_ISO_STRENGTH_SIZE];    ///<   RW; Range:[0x0, 0xFFF]; Threshold of no directional sharpening brightness, intensity values above this value will be sharpen.
    vs_uint8_t    luma_offset_low_ud[VS_ISP_AUTO_ISO_STRENGTH_SIZE];    ///<   RW; Range:[0x0, 0xFF]; Linear threshold offset corresponding to luma_thresh_low_ud.
    vs_uint32_t   luma_slope_low_ud[VS_ISP_AUTO_ISO_STRENGTH_SIZE];     ///<   RW; Range:[0x0, 0xFFFFF]; Linear threshold slope corresponding to luma_thresh_low_ud.
    vs_uint16_t   luma_thresh_high_ud[VS_ISP_AUTO_ISO_STRENGTH_SIZE];   ///<   RW; Range:[0x0, 0xFFF]; Threshold of no directional sharpening brightness, intensity values below this value will be sharpen.
    vs_uint32_t   luma_slope_high_ud[VS_ISP_AUTO_ISO_STRENGTH_SIZE];    ///<   RW; Range:[0x0, 0xFFFFF]; Linear threshold slope corresponding to luma_thresh_high_ud.
} vs_isp_demosaic_auto_attr_s;

/**
 * @brief Defines vs isp demosaic algorithm static attribute
 * @details
 */
typedef struct vs_isp_demosaic_static_attr {
    vs_bool_t     static_enable;    ///<   RW; Range:[0, 1]; Format:1.0; Sharpen algorithm selection.
    vs_uint8_t    sharp_alt_d;      ///<   RW; Range:[0x0, 0xFF]; No directional high frequency sharpening strength.
    vs_uint8_t    sharp_alt_ud;     ///<   RW; Range:[0x0, 0xFF]; No directional low frequency sharpening strength
} vs_isp_demosaic_static_attr_s;

/**
 * @brief Defines ISP demosaic algorithm attribute
 * @details
 */
typedef struct vs_isp_demosaic_attr {
    vs_uint8_t    dmsc_config;                              ///<   RW; Range:[0x0, 0xFF]; Debug mode selection.
    vs_uint16_t   vh_offset;                                ///<   RW; Range:[0x0, 0xFFF]; Offset of vertical and horizontal edge mixture.
    vs_uint16_t   aa_offset;                                ///<   RW; Range:[0x0, 0xFFF]; Offset of 45 degree and 135 degree edge mixture.
    vs_uint16_t   va_offset;                                ///<   RW; Range:[0x0, 0xFFF]; Offset of vertical, horizontal, 45 degree and 135 degree edge mixture.
    vs_uint8_t    grey_det_thresh;                          ///<   RW; Range:[0x0, 0xFF]; Threshold applied to the inter-channel difference for detecting grey region.
    vs_uint16_t   grey_det_slope;                           ///<   RW; Range:[0x0, 0xFFFF]; Slope of the linear thresholding for the grey detector.
    vs_uint8_t    lg_det_thresh;                            ///<   RW; Range:[0x0, 0xFF]; Threshold of the green channel, below this value the gradient is calculated using only B and R channels.
    vs_uint16_t   lg_det_slope;                             ///<   RW; Range:[0x0, 0xFFFF]; Slope of the linear thresholding for the loe green detector.
    vs_uint8_t    sat_slope;                                ///<   RW; Range:[0x0, 0xFF]; Slope of saturation blending threshold.
    vs_uint16_t   sat_thresh;                               ///<   RW; Range:[0x0, 0xFFF]; Threshold for the range of saturation blending.
    vs_uint16_t   sat_offset;                               ///<   RW; Range:[0x0, 0xFFF]; Offset for saturation blending threshold.
    vs_uint16_t   lum_thresh;                               ///<   RW; Range:[0x0, 0xFFF]; Luminance threshold for directional sharpening.
    vs_uint16_t   ac_offset;                                ///<   RW; Range:[0x0, 0xFFF]; Offset for AC blending threshold.
    vs_uint16_t   ac_thresh;                                ///<   RW; Range:[0x0, 0xFFF]; Threshold for AC blending.
    vs_uint8_t    ac_slope;                                 ///<   RW; Range:[0x0, 0xFF]; Slope of AC blending threshold.
    vs_bool_t     np_off_reflect;                           ///<   RW; Range:[0, 1]; Select the value processing mode below the black level, 0:Repeat the first table entry,1:Refiect the noise profile curve below black level.
    vs_uint8_t    np_off;                                   ///<   RW; Range:[0x0, 0x7F]; Noise profile black level offset.
    vs_uint8_t    sad_amp;                                  ///<   RW; Range:[0x0, 0xFF]; Medium and high frequency sharpening mixing degree.
    vs_uint16_t   min_d_strength;                           ///<   RW; Range:[0x1000, 0x1FFF]; Min threshold for the directional.
    vs_uint16_t   min_ud_strength;                          ///<   RW; Range:[0x1000, 0x1FFF]; Min threshold for the un-directional.
    vs_uint16_t   max_d_strength;                           ///<   RW; Range:[0x0, 0xFFF]; Max threshold for the directional.
    vs_uint16_t   max_ud_strength;                          ///<   RW; Range:[0x0, 0xFFF]; Max threshold for the un-directional.
    vs_uint8_t    np_weight_lut[VS_ISP_DEMOSAIC_LUT_SIZE];  ///<   RW; Range:[0x0, 0xFF]; Noise profile LUT.
    vs_isp_demosaic_static_attr_s  static_attr;
    vs_isp_op_type_e optype;                                ///<   RW; Range:[0, 1]; Format:1.0; Operation type.
    vs_isp_demosaic_manual_attr_s manual_attr;
    vs_isp_demosaic_auto_attr_s   auto_attr;
} vs_isp_demosaic_attr_s;

/**
 * @brief Defines ISP purple Purple fringing correction algorithm attribute
 * @details
 */
typedef struct vs_isp_pfc_attr {
    vs_uint8_t    debug_sel;                                           ///<   RW; Range:[0x0, 0xFF]; Debug mode selection.
    vs_bool_t     use_color_corrected_rgb;                             ///<   RW; Range:[0, 1]; Format:1.0; Generate masks using internal CCM.
    vs_uint16_t   hue_low_slope;                                       ///<   RW; Range:[0x0, 0xFFF]; Slope of lower hue limit.
    vs_uint16_t   hue_low_offset;                                      ///<   RW; Range:[0x0, 0xFFF]; Mask value below hue limit.
    vs_uint16_t   hue_low_thresh;                                      ///<   RW; Range:[0x0, 0xFFF]; Threshold of lower hue limit.
    vs_uint16_t   hue_high_slope;                                      ///<   RW; Range:[0x0, 0xFFF]; Slope of upper hue limit.
    vs_uint16_t   hue_high_offset;                                     ///<   RW; Range:[0x0, 0xFFF]; Mask value above hue limit.
    vs_uint16_t   hue_high_thresh;                                     ///<   RW; Range:[0x0, 0xFFF]; Threshold of upper hue limit.
    vs_uint16_t   hue_strength;                                        ///<   RW; Range:[0x0, 0xFFF]; Strength of hue mask.
    vs_uint16_t   sat_low_slope;                                       ///<   RW; Range:[0x0, 0xFFF]; Slope of lower saturation limit.
    vs_uint16_t   sat_low_offset;                                      ///<   RW; Range:[0x0, 0xFFF]; Mask value below saturation limit.
    vs_uint16_t   sat_low_thresh;                                      ///<   RW; Range:[0x0, 0xFFF]; Threshold of lower saturation limit.
    vs_uint16_t   sat_high_slope;                                      ///<   RW; Range:[0x0, 0xFFF]; Slope of upper saturation limit.
    vs_uint16_t   sat_high_offset;                                     ///<   RW; Range:[0x0, 0xFFF]; Mask value above saturation limit.
    vs_uint16_t   sat_high_thresh;                                     ///<   RW; Range:[0x0, 0xFFF]; Threshold of upper saturation limit.
    vs_uint16_t   sat_strength;                                        ///<   RW; Range:[0x0, 0xFFF]; Strength of saturation mask.
    vs_uint16_t   luma1_low_slope;                                     ///<   RW; Range:[0x0, 0xFFF]; Slope of lower luma1 limit.
    vs_uint16_t   luma1_low_offset;                                    ///<   RW; Range:[0x0, 0xFFF]; Mask value below luma1 limit.
    vs_uint16_t   luma1_low_thresh;                                    ///<   RW; Range:[0x0, 0xFFF]; Threshold of lower luma1 limit.
    vs_uint16_t   luma1_high_slope;                                    ///<   RW; Range:[0x0, 0xFFF]; Slope of upper luma1 limit.
    vs_uint16_t   luma1_high_offset;                                   ///<   RW; Range:[0x0, 0xFFF]; Mask value above luma1 limit.
    vs_uint16_t   luma1_high_thresh;                                   ///<   RW; Range:[0x0, 0xFFF]; Threshold of upper luma1 limit.
    vs_uint16_t   luma2_low_slope;                                     ///<   RW; Range:[0x0, 0xFFF]; Slope of lower luma2 limit.
    vs_uint16_t   luma2_low_offset;                                    ///<   RW; Range:[0x0, 0xFFF]; Mask value below luma2 limit.
    vs_uint16_t   luma2_low_thresh;                                    ///<   RW; Range:[0x0, 0xFFF]; Threshold of lower luma2 limit.
    vs_uint16_t   luma2_high_slope;                                    ///<   RW; Range:[0x0, 0xFFF]; Slope of upper luma2 limit.
    vs_uint16_t   luma2_high_offset;                                   ///<   RW; Range:[0x0, 0xFFF]; Mask value above luma2 limit.
    vs_uint16_t   luma2_high_thresh;                                   ///<   RW; Range:[0x0, 0xFFF]; Threshold of upper luma2 limit.
    vs_uint16_t   luma_strength;                                       ///<   RW; Range:[0x0, 0xFFF]; Strength of luma mask.
    vs_uint16_t   sad_slope;                                           ///<   RW; Range:[0x0, 0xFFF]; Slope of SAD limit.
    vs_uint16_t   sad_offset;                                          ///<   RW; Range:[0x0, 0xFFF]; Mask value below SAD lower limit.
    vs_uint16_t   sad_thresh;                                          ///<   RW; Range:[0x0, 0xFFF]; Threshold of lower SAD limit.
    vs_uint16_t   off_center_mult;                                     ///<   RW; Range:[0x0, 0xFFFF]; Radial distance multiplier.
    vs_uint8_t    pf_radial_lut[VS_ISP_PFC_SHADING_LUT_SIZE];          ///<   RW; Range:[0x0, 0xFF]; PF correction radial mask lut.
    vs_uint16_t   hsl_slope;                                           ///<   RW; Range:[0x0, 0xFFF]; Slope of combined mask lower limit.
    vs_uint16_t   hsl_offset;                                          ///<   RW; Range:[0x0, 0xFFF]; Final mask value below combined mask lower limit.
    vs_uint16_t   hsl_thresh;                                          ///<   RW; Range:[0x0, 0xFFF]; Threshold of lower combined mask limit.
    vs_uint16_t   purple_strength;                                     ///<   RW; Range:[0x0, 0xFFF]; Strength OF purple correction.
    vs_uint8_t    saturation_strength;                                 ///<   RW; Range:[0x0, 0xFF]; Saturation strength of fringing after correction.
    vs_uint16_t   center_x;                                            ///<   RW; Range:[0x0, 0xFFFF]; X-coordinate of the center point.
    vs_uint16_t   center_y;                                            ///<   RW; Range:[0x0, 0xFFFF]; Y-coordinate of the center point.
} vs_isp_pfc_attr_s;

/**
 * @brief Defines vs isp color matrix algorithm manual attribute
 * @details
 */
typedef struct vs_isp_color_matrix_manual_attr {
    vs_uint16_t   color_matrix[VS_ISP_COLOR_MATRIX_SIZE];                ///<   RW; Range: [0x0, 0xFFFF]; Format:4.8; Color matrix.
} vs_isp_color_matrix_manual_attr_s;

/**
 * @brief Defines vs isp color matrix algorithm manual attribute
 * @details
 */
typedef struct vs_isp_color_matrix_auto_attr {
    vs_uint16_t   mt_absolute_ls_a_ccm[VS_ISP_COLOR_MATRIX_SIZE];   ///<   RW; Range: [0x0, 0xFFFF]; Format:4.8; A light source color matrix.
    vs_uint16_t   mt_absolute_ls_d40_ccm[VS_ISP_COLOR_MATRIX_SIZE]; ///<   RW; Range: [0x0, 0xFFFF]; Format:4.8; d40 light source color matrix.
    vs_uint16_t   mt_absolute_ls_d50_ccm[VS_ISP_COLOR_MATRIX_SIZE]; ///<   RW; Range: [0x0, 0xFFFF]; Format:4.8; d50 light source color matrix.
} vs_isp_color_matrix_auto_attr_s;

/**
 * @brief Defines vs isp saturation algorithm manual attribute
 * @details
 */
typedef struct vs_isp_saturation_manual_attr {
    vs_uint16_t  saturation_strength;  ///<   RW; Range: [0x0, 0xFF]; Format:4.8; Saturation strength.
} vs_isp_saturation_manual_attr_s;

/**
 * @brief Defines vs isp saturation algorithm auto attribute
 * @details
 */
typedef struct vs_isp_saturation_auto_attr {
    vs_uint16_t   saturation_strength[VS_ISP_AUTO_ISO_STRENGTH_SIZE];       ///<   RW; Range: [0x0, 0xFF]; Format:4.8; Saturation strength.
} vs_isp_saturation_auto_attr_s;

/**
 * @brief Defines ISP color matrix algorithm attribute
 * @details
 */
typedef struct vs_isp_color_matrix_attr {
    vs_bool_t   color_matrix_enable;    ///<   RW; Range:[0, 1]; Format:1.0; Enbale signal of color matrix.
    vs_uint16_t ccm_one_gain_threshold; ///<   RW; Range: [0x0, 0xFFFF]; Format:8.8; Threshold of Identity matrix.
    vs_isp_op_type_e ccm_optype;        ///<   RW; Range:[0, 1]; Format:1.0; Operation type.
    vs_isp_color_matrix_manual_attr_s ccm_manual_attr;
    vs_isp_color_matrix_auto_attr_s ccm_auto_attr;
    vs_isp_op_type_e sat_optype;        ///<   RW; Range:[0, 1]; Format:1.0; Operation type.
    vs_isp_saturation_manual_attr_s sat_manual_attr;
    vs_isp_saturation_auto_attr_s sat_auto_attr;
} vs_isp_color_matrix_attr_s;

/**
 * @brief Defines vs isp cnr manual attribute
 * @details
 */
typedef struct vs_isp_cnr_manual_attr {
    vs_uint16_t   uv_delta1_slope;           ///<   RW; Range:[0x0, 0xFFFF]; Strength of color noise reduction.
    vs_uint16_t   uv_delta2_slope;           ///<   RW; Range:[0x0, 0xFFFF]; Strength of color noise reduction.
} vs_isp_cnr_manual_attr_s;

/**
 * @brief Defines vs isp cnr auto attribute
 * @details
 */
typedef struct vs_isp_cnr_auto_attr {
    vs_uint16_t   uv_delta12_slope[VS_ISP_AUTO_ISO_STRENGTH_SIZE];           ///<   RW; Range:[0x0, 0xFFFF]; Strength of color noise reduction.
} vs_isp_cnr_auto_attr_s;

/**
 * @brief Defines ISP color noise reduction algorithm attribute
 * @details
 */
typedef struct vs_isp_cnr_attr {
    vs_bool_t     square_root_enable;   ///<   RW; Range:[0, 1]; Format:1.0; Enbale signal of pre-CNR square root and the post-CNR square modules.
    vs_bool_t     enable;               ///<   RW; Range:[0, 1]; Format:1.0; Enable signal of CNR module.
    vs_uint8_t    mode;                 ///<   RW; Range:[0x0, 0xFF]; CNR mode.
    vs_uint16_t   debug_reg;            ///<   RW; Range:[0x0, 0xFFFF]; Debug mode selection.
    vs_uint16_t   u_center;             ///<   RW; Range:[0x0, 0xFFF]; Center ppint of U plane.
    vs_uint16_t   v_center;             ///<   RW; Range:[0x0, 0xFFF]; Center ppint of V plane.
    vs_uint16_t   global_offset;        ///<   RW; Range:[0x0, 0xFFF]; Maximum value of uv_seg mask.
    vs_uint16_t   global_slope;         ///<   RW; Range:[0x0, 0xFFFF]; Minimum value of uv_seg mask.
    vs_uint16_t   uv_seg1_slope;        ///<   RW; Range:[0x0, 0xFFFF]; Sensitivity of uv_seg mask.
    vs_uint16_t   uv_seg1_thresh;       ///<   RW; Range:[0x0, 0xFFF]; Threshold of uv_seg mask.
    vs_uint16_t   uv_seg1_offset;       ///<   RW; Range:[0x0, 0xFFF]; Saturation lower limit.
    vs_uint16_t   umean1_slope;         ///<   RW; Range:[0x0, 0xFFFF]; Sensitivity of edge detection for U channel.
    vs_uint16_t   vmean1_slope;         ///<   RW; Range:[0x0, 0xFFFF]; Sensitivity of edge detection for V channel.
    vs_uint16_t   umean2_slope;         ///<   RW; Range:[0x0, 0xFFFF]; Sensitivity of edge detection for U channel.
    vs_uint16_t   vmean2_slope;         ///<   RW; Range:[0x0, 0xFFFF]; Sensitivity of edge detection for V channel.
    vs_uint16_t   umean1_thresh;        ///<   RW; Range:[0x0, 0xFFF]; Threshold of edge detection for U channel.
    vs_uint16_t   vmean1_thresh;        ///<   RW; Range:[0x0, 0xFFF]; Threshold of edge detection for V channel.
    vs_uint16_t   umean2_thresh;        ///<   RW; Range:[0x0, 0xFFF]; Threshold of edge detection for U channel.
    vs_uint16_t   vmean2_thresh;        ///<   RW; Range:[0x0, 0xFFF]; Threshold of edge detection for V channel.
    vs_uint16_t   umean1_offset;        ///<   RW; Range:[0x0, 0xFFF]; Edge detection lower limit of U channel.
    vs_uint16_t   vmean1_offset;        ///<   RW; Range:[0x0, 0xFFF]; Edge detection lower limit of V channel.
    vs_uint16_t   umean2_offset;        ///<   RW; Range:[0x0, 0xFFF]; Edge detection lower limit of U channel.
    vs_uint16_t   vmean2_offset;        ///<   RW; Range:[0x0, 0xFFF]; Edge detection lower limit of V channel.
    vs_uint16_t   uv_var1_scale;        ///<   RW; Range:[0x0, 0x3F]; Slope multiplier for color difference mask.
    vs_uint16_t   uv_var2_scale;        ///<   RW; Range:[0x0, 0x3F]; Slope multiplier for color difference mask.
    vs_uint16_t   uv_var1_slope;        ///<   RW; Range:[0x0, 0xFFFF]; Sensitivity of color difference mask.
    vs_uint16_t   uv_var2_slope;        ///<   RW; Range:[0x0, 0xFFFF]; Sensitivity of color difference mask.
    vs_uint16_t   uv_var1_thresh;       ///<   RW; Range:[0x0, 0xFFF]; Threshold of uv_var1.
    vs_uint16_t   uv_var2_thresh;       ///<   RW; Range:[0x0, 0xFFF]; Threshold of uv_var2.
    vs_uint16_t   uv_var1_offset;       ///<   RW; Range:[0x0, 0xFFF]; Color difference lower limit.
    vs_uint16_t   uv_var2_offset;       ///<   RW; Range:[0x0, 0xFFF]; Color difference lower limit.
    vs_uint16_t   uv_delta1_offset;     ///<   RW; Range:[0x0, 0xFFF]; Color noise lower limit.
    vs_uint16_t   uv_delta2_offset;     ///<   RW; Range:[0x0, 0xFFF]; Color noise lower limit.
    vs_uint16_t   uv_delta1_thresh;     ///<   RW; Range:[0x0, 0xFFF]; Threshold of uv delta1.
    vs_uint16_t   uv_delta2_thresh;     ///<   RW; Range:[0x0, 0xFFF]; Threshold of  uv delta2.
    vs_uint16_t   delta_factor;         ///<   RW; Range:[0x0, 0xFFF]; Color bleeding correction.
    vs_uint16_t   effective_kernel;     ///<   RW; Range:[0x0, 0x7F]; Effective kernel select.
    vs_isp_op_type_e  optype;           ///<   RW; Range:[0, 1]; Format:1.0; Operation type.
    vs_isp_cnr_manual_attr_s manual_attr;
    vs_isp_cnr_auto_attr_s   auto_attr;
} vs_isp_cnr_attr_s;

/**
 * @brief Defines ISP 3d color lut memory attribute.
 * @details
 */
typedef struct vs_isp_lut3d_mem_attr {
    vs_uint16_t   none_equ_gamma[VS_ISP_NONE_EQU_GAMMA_SIZE];   ///<   RW; Range:[0x0, 0x3FFF]; None-equidistant LUT.
    vs_uint32_t   lut3d_mem[VS_ISP_LUT3D_MEM_SIZE];             ///<   RW; Range:[0x0, 0xFFFFFFFF]; 3D LUT.
} vs_isp_lut3d_mem_attr_s;

/**
 * @brief Defines vs isp auto level manual attribute
 * @details
 */
typedef struct vs_isp_auto_level_manual_attr {
    vs_uint16_t   gain[VS_ISP_GAMMA_GAIN_SIZE];         ///<   RW; Range:[0x0, 0xFFF]; Gain of r g b channel.
    vs_uint16_t   offset[VS_ISP_GAMMA_GAIN_SIZE];       ///<   RW; Range:[0x0, 0xFFF]; Offset subtracted of r g b channel.
} vs_isp_auto_level_manual_attr_s;

/**
 * @brief Defines vs isp auto level dynamic attribute
 * @details
 */
typedef struct vs_isp_auto_level_dynamic_attr {
    vs_uint32_t   enable_auto_level;   ///<   RW; Range:[0, 1]; Enable_signal of auto_level module.
    vs_uint32_t   black_percentage;    ///<   RW; Range:[0x0, 0x64]; Percentage of pixel that should be at bin 0 in the histogram after auto levels.
    vs_uint32_t   white_percentage;    ///<   RW; Range:[0x0, 0x64]; Percentage of pixel that should be below the auto_white_prc.
    vs_uint32_t   auto_black_min;      ///<   RW; Range:[0x0, 0xFFF]; The minimum offset to be substracted of the image.
    vs_uint32_t   auto_black_max;      ///<   RW; Range:[0x0, 0xFFF]; The maximum offset to be substracted of the image.
    vs_uint32_t   auto_white_prc;      ///<   RW; Range:[0x0, 0x64]; Highlights target as percentage of histogarm.
    vs_uint32_t   avg_coeff;           ///<   RW; Range:[0x0, 0x1E]; Time filter coefficient for offset and gain applied by auto levels.
} vs_isp_auto_level_dynamic_attr_s;

/**
 * @brief Defines vs isp auto level auto attribute
 * @details
 */
typedef struct vs_isp_auto_level_auto_attr {
    vs_isp_auto_level_dynamic_attr_s auto_level_dynamic;
    vs_uint16_t   gain[VS_ISP_AUTO_ISO_STRENGTH_SIZE][VS_ISP_GAMMA_GAIN_SIZE];         ///<   RW; Range:[0x0, 0xFFF]; Gain of r g b channel.
    vs_uint16_t   offset[VS_ISP_AUTO_ISO_STRENGTH_SIZE][VS_ISP_GAMMA_GAIN_SIZE];       ///<   RW; Range:[0x0, 0xFFF]; Offset subtracted of r g b channel.
} vs_isp_auto_level_auto_attr_s;

/**
 * @brief Defines vs isp gamma static attribute
 * @details
 */
typedef struct vs_isp_gamma_static_attr {
    vs_uint32_t   data[VS_ISP_GAMMA_DATA_LUT_SIZE]; ///<   RW; Range:[0x0, 0xFFF]; Gamma data LUT.
} vs_isp_gamma_static_attr_s;

/**
 * @brief Defines vs isp gamma auto attribute
 * @details
 */
typedef struct vs_isp_gamma_dynamic_attr {
    vs_uint16_t   gamma_ev1[VS_ISP_GAMMA_DATA_LUT_SIZE];                 ///<   RW; Range:[0x0, 0xFFF]; Dynamic gamma LUT1.
    vs_uint16_t   gamma_ev2[VS_ISP_GAMMA_DATA_LUT_SIZE];                 ///<   RW; Range:[0x0, 0xFFF]; Dynamic gamma LUT2.
    vs_uint32_t   gamma_threshold[VS_ISP_GAMMA_THRESH_SIZE];             ///<   RW; Range:[0x0, 0xFFFFFFFF]; Gamma threshold.
} vs_isp_gamma_dynamic_attr_s;

/**
 * @brief Defines vs isp gamma operation type
 * @details
 */
typedef enum vs_isp_gamma_op_type {
    E_ISP_GAMMA_TYPE_STATIC = 0,
    E_ISP_GAMMA_TYPE_DYNAMIC,
    E_ISP_GAMMA_TYPE_MAX
} vs_isp_gamma_op_type_e;

/**
 * @brief Defines ISP gamma attribute.
 * @details
 */
typedef struct vs_isp_gamma_attr {
    vs_bool_t     enable;                           ///<   RW; Range:[0, 1]; Format:1.0; Enable gamma.
    vs_isp_gamma_op_type_e  gamma_optype;           ///<   RW; Range:[0, 1]; Format:1.0; 0:static gamma, 1:dynamic gamma
    vs_isp_gamma_static_attr_s  static_attr;
    vs_isp_gamma_dynamic_attr_s    dynamic_attr;

    vs_isp_op_type_e  op_contrast;                  ///<   RW; Range:[0, 1]; Format:1.0; 0:auto level; 1:manual.
    vs_isp_auto_level_manual_attr_s manual_attr;
    vs_isp_auto_level_auto_attr_s   auto_attr;
} vs_isp_gamma_attr_s;

/**
 * @brief Defines vs isp rgb_sharpen manual attribute
 * @details
 */
typedef struct vs_isp_rgb_sharpen_manual_attr {
    vs_uint16_t   strength;                                ///<   RW; Range:[0x0, 0x1FF]; Strength of RGB sharpen.
    vs_uint8_t    alpha_undershoot;                        ///<   RW; Range:[0x0, 0xFF]; Alpha blend of under/overshoot,0:only undershoot, 255:only overshoot.
    vs_uint16_t   clip_str_max;                            ///<   RW; Range:[0x0, 0x3FFF]; Clips sharpening mask max value, this will control overshoot.
    vs_uint16_t   clip_str_min;                            ///<   RW; Range:[0x0, 0x3FFF]; Clips sharpening mask max value, this will control undershoot.
    vs_uint16_t   luma_thresh_low;                         ///<   RW; Range:[0x0, 0x3FF]; Luma theshold where below this value, no sharpening wiil be applied.
    vs_uint8_t    luma_offset_low;                         ///<   RW; Range:[0x0, 0xFF]; Luma offset(min value) after linear thresholding.
    vs_uint16_t   luma_slope_low;                          ///<   RW; Range:[0x0, 0xFFFF]; Luma linear threshold slope(min value).
    vs_uint16_t   luma_thresh_high;                        ///<   RW; Range:[0x0, 0x3FF]; Luma threshold where above this value, no sharpening will be applied.
    vs_uint8_t    luma_offset_high;                        ///<   RW; Range:[0x0, 0xFF]; Luma offset(max value) after linear thresholding.
    vs_uint16_t   luma_slope_high;                         ///<   RW; Range:[0x0, 0xFFFF]; Luma linear threshold slope(max value).
} vs_isp_rgb_sharpen_manual_attr_s;

/**
 * @brief Defines vs isp rgb_sharpen auto attribute
 * @details
 */
typedef struct vs_isp_rgb_sharpen_auto_attr {
    vs_uint16_t   strength[VS_ISP_AUTO_ISO_STRENGTH_SIZE];          ///<   RW; Range:[0x0, 0x1FF]; Strength of RGB sharpen.
    vs_uint8_t    alpha_undershoot[VS_ISP_AUTO_ISO_STRENGTH_SIZE];  ///<   RW; Range:[0x0, 0xFF]; Alpha blend of under/overshoot,0:only undershoot, 255:only overshoot.
    vs_uint16_t   clip_str_max[VS_ISP_AUTO_ISO_STRENGTH_SIZE];      ///<   RW; Range:[0x0, 0x3FFF]; Clips sharpening mask max value, this will control overshoot.
    vs_uint16_t   clip_str_min[VS_ISP_AUTO_ISO_STRENGTH_SIZE];      ///<   RW; Range:[0x0, 0x3FFF]; Clips sharpening mask max value, this will control undershoot.
    vs_uint16_t   luma_thresh_low[VS_ISP_AUTO_ISO_STRENGTH_SIZE];   ///<   RW; Range:[0x0, 0x3FF]; Luma theshold where below this value, no sharpening wiil be applied.
    vs_uint8_t    luma_offset_low[VS_ISP_AUTO_ISO_STRENGTH_SIZE];   ///<   RW; Range:[0x0, 0xFF]; Luma offset(min value) after linear thresholding.
    vs_uint16_t   luma_slope_low[VS_ISP_AUTO_ISO_STRENGTH_SIZE];    ///<   RW; Range:[0x0, 0xFFFF]; Luma linear threshold slope(min value).
    vs_uint16_t   luma_thresh_high[VS_ISP_AUTO_ISO_STRENGTH_SIZE];  ///<   RW; Range:[0x0, 0x3FF]; Luma threshold where above this value, no sharpening will be applied.
    vs_uint8_t    luma_offset_high[VS_ISP_AUTO_ISO_STRENGTH_SIZE];  ///<   RW; Range:[0x0, 0xFF]; Luma offset(max value) after linear thresholding.
    vs_uint16_t   luma_slope_high[VS_ISP_AUTO_ISO_STRENGTH_SIZE];   ///<   RW; Range:[0x0, 0xFFFF]; Luma linear threshold slope(max value).
} vs_isp_rgb_sharpen_auto_attr_s;

/**
 * @brief Defines ISP sharpen attribute.
 * @details
 */
typedef struct vs_isp_rgb_sharpen_attr {
    vs_bool_t     enable;                                        ///<   RW; Range:[0, 1]; Format:1.0; Enable signal of RGB sharpen.
    vs_uint8_t    control_r;                                     ///<   RW; Range:[0x0, 0xFF]; Luma transform red coefficient.
    vs_uint8_t    control_b;                                     ///<   RW; Range:[0x0, 0xFF]; Luma transform blue coefficient.
    vs_uint8_t    debug;                                         ///<   RW; Range:[0x0, 0xF]; Debug mode select.
    vs_isp_op_type_e  optype;                                    ///<   RW; Range:[0, 1]; Format:1.0; Operation type.
    vs_isp_rgb_sharpen_manual_attr_s manual_attr;
    vs_isp_rgb_sharpen_auto_attr_s   auto_attr;

} vs_isp_rgb_sharpen_attr_s;

/**
 * @brief Defines ISP color mode.
 * @details
 */
typedef enum vs_isp_color_mode{
    E_ISP_COLOR_NORMAL,
    E_ISP_COLOR_BLACK_AND_WHITE,
    E_ISP_COLOR_NEGATIVE,
    E_ISP_COLOR_SEPIA,
    E_ISP_COLOR_VIVID,
    E_ISP_COLOR_MAX
} vs_isp_color_mode_e;


/**
 * @brief Defines ISP color space covert attribute.
 * @details
 */
typedef struct vs_isp_csc_attr {
    vs_bool_t     enable_matrix;                                ///<   RW; Range:[0, 1]; Format:1.0; Enable signal of matrix.
    vs_bool_t     enable_filter;                                ///<   RW; Range:[0, 1]; Format:1.0; Enable signal of filter.
    vs_bool_t     enable_h_downsample;                          ///<   RW; Range:[0, 1]; Format:1.0; Enable signal of horizontal downsampling.
    vs_bool_t     enable_v_downsample;                          ///<   RW; Range:[0, 1]; Format:1.0; Enable signal of vertical downsampling.
    vs_isp_color_mode_e color_mode_id;                          ///<   RW; Range:[0, 4]; Format:2.0; Color space mode selection.
    vs_color_gamut_e   color_space_id;                          ///<   RW; Range:[0, 2]; Format:2.0; Color space curve selection.
    vs_uint16_t   use_coefft[VS_ISP_CSC_COEFFT_SIZE];           ///<   RW; Range:[0x0, 0xFFFF]; Matrix coefficient of user-defined.
    vs_uint16_t   use_coefft_o[VS_ISP_CSC_COEFFT_O_SIZE];       ///<   RW; Range:[0x0, 0x7FF]; Offset of matrix coefficient.
    vs_uint16_t   clip_min_y;                                   ///<   RW; Range:[0x0, 0x3FF]; Minimum value for Y, values below this are clipped.
    vs_uint16_t   clip_max_y;                                   ///<   RW; Range:[0x0, 0x3FF]; Maximum value for Y, values above this are clipped.
    vs_uint16_t   clip_min_uv;                                  ///<   RW; Range:[0x0, 0x3FF]; Minimum value for UV, values below this are clipped.
    vs_uint16_t   clip_max_uv;                                  ///<   RW; Range:[0x0, 0x3FF]; Maximum value for UV, values above this are clipped.
    vs_uint16_t   data_mask[VS_ISP_CSC_DATA_MASK_SIZE];         ///<   RW; Range:[0x0, 0x3FF]; Dta mask for ry\gu\bv.
    vs_bool_t     enable_dither;                                ///<   RW; Range:[0, 1]; Format:1.0; Enable signal of dither module.
    vs_uint8_t    dither_amount;                                ///<   RW; Range:[0x0, 0x3]; Dither amount.
    vs_bool_t     shift_mode;                                   ///<   RW; Range:[0, 1]; Format:1.0; Data alignment mode.
    vs_uint8_t    brightness_strength_id;                       ///<   RW; Range:[0x0, 0xFF]; Brightness strength of target YUV space .
    vs_uint8_t    contrast_strength_id;                         ///<   RW; Range:[0x0, 0xFF]; Contrast strength of target YUV space.
    vs_uint8_t    saturation_strength_id;                       ///<   RW; Range:[0x0, 0xFF]; Saturation strength of target YUV space.
    vs_uint16_t   hue_theta_id;                                 ///<  RW; Range:[0x0, 360]; Hue theta of target YUV space.
} vs_isp_csc_attr_s;

/**
 * @brief Defines vs isp wdr_stitch manual attribute
 * @details
 */
typedef struct vs_isp_wdr_stitch_manual_attr {
    vs_uint16_t   gain_r;                         ///<   RW; Range:[0x0, 0xFFF]; Gain of red channel.
    vs_uint16_t   gain_b;                         ///<   RW; Range:[0x0, 0xFFF]; Gain of blue channel.
    vs_uint16_t   lm_np_mult;                     ///<   RW; Range:[0x0, 0xFFF]; Noise level factor for long and medium exposure.
    vs_uint16_t   ms_np_mult;                     ///<   RW; Range:[0x0, 0xFFF]; Noise level factor for medium and short exposure.
    vs_uint16_t   svs_np_mult;                    ///<   RW; Range:[0x0, 0xFFF]; Noise level factor for short and very short exposure.
    vs_uint16_t   lm_alpha_mov_slope;             ///<   RW; Range:[0x0, 0xFFF]; Motion alpha slope of long and medium exposure.
    vs_uint16_t   ms_alpha_mov_slope;             ///<   RW; Range:[0x0, 0xFFF]; Motion alpha slope of medium and short exposure.
    vs_uint16_t   svs_alpha_mov_slope;            ///<   RW; Range:[0x0, 0xFFF]; Motion alpha slope of short and very short exposure.
    vs_uint16_t   lm_med_noise_intensity_thresh;  ///<   RW; Range:[0x0, 0xFFF]; Noise threshold of long medium exposure.
} vs_isp_wdr_stitch_manual_attr_s;

/**
 * @brief Defines vs isp wdr_stitch auto attribute
 * @details
 */
typedef struct vs_isp_wdr_stitch_auto_attr {
    vs_uint16_t   lm_np_mult[VS_ISP_AUTO_ISO_STRENGTH_SIZE];                    ///<   RW; Range:[0x0, 0xFFF]; Noise level factor for long and medium exposure.
    vs_uint16_t   ms_np_mult[VS_ISP_AUTO_ISO_STRENGTH_SIZE];                    ///<   RW; Range:[0x0, 0xFFF]; Noise level factor for medium and short exposure.
    vs_uint16_t   svs_np_mult[VS_ISP_AUTO_ISO_STRENGTH_SIZE];                   ///<   RW; Range:[0x0, 0xFFF]; Noise level factor for short and very short exposure.
    vs_uint16_t   lm_alpha_mov_slope[VS_ISP_AUTO_ISO_STRENGTH_SIZE];            ///<   RW; Range:[0x0, 0xFFF]; Motion alpha slope of long and medium exposure.
    vs_uint16_t   ms_alpha_mov_slope[VS_ISP_AUTO_ISO_STRENGTH_SIZE];            ///<   RW; Range:[0x0, 0xFFF]; Motion alpha slope of medium and short exposure.
    vs_uint16_t   svs_alpha_mov_slope[VS_ISP_AUTO_ISO_STRENGTH_SIZE];           ///<   RW; Range:[0x0, 0xFFF]; Motion alpha slope of short and very short exposure.
    vs_uint16_t   lm_med_noise_intensity_thresh[VS_ISP_AUTO_ISO_STRENGTH_SIZE]; ///<   RW; Range:[0x0, 0xFFF]; Noise threshold of long and medium exposure.
} vs_isp_wdr_stitch_auto_attr_s;

/**
 * @brief Defines wdr stitch attribute
 * @details
 */
typedef struct vs_isp_wdr_stitch_attr {
    vs_uint8_t   output_select;             ///<   RW; Range:[0x0, 0xFF]; Debug mode selection.
    vs_uint16_t  mc_off_enable_gain;        ///<   RW; Range:[0, 0xFFFF]; Gain theshold of mc_off_mode.
    vs_uint16_t  lm_thresh_high;            ///<   RW; Range:[0x0, 0xFFF]; Theshold of long and medium exposure, Data above this theshold will be taken from short exposure only.
    vs_uint16_t  lm_thresh_low;             ///<   RW; Range:[0x0, 0xFFF]; Theshold of long and medium exposure, Data below this theshold will be taken from short exposure only.
    vs_uint16_t  ms_thresh_high;            ///<   RW; Range:[0x0, 0xFFF]; Theshold of medium and short exposure, Data above this theshold will be taken from short exposure only.
    vs_uint16_t  ms_thresh_low;             ///<   RW; Range:[0x0, 0xFFF]; Theshold of medium and short exposure, Data below this theshold will be taken from short exposure only.
    vs_uint16_t  svs_thresh_high;           ///<   RW; Range:[0x0, 0xFFF]; Theshold of short and very short exposure, Data above this theshold will be taken from short exposure only.
    vs_uint16_t  svs_thresh_low;            ///<   RW; Range:[0x0, 0xFFF]; Theshold of short and very short exposure, Data below this theshold will be taken from short exposure only.
    vs_uint16_t  consistency_thresh_mov;    ///<   RW; Range:[0x0, 0xFFF]; Motion threshold of pixel consistency.
    vs_uint32_t  consistency_thresh_lvl;    ///<   RW; Range:[0x0, 0xFFFFF]; Flicker threshold of pixel consistency.
    vs_uint8_t   lm_noise_thresh;           ///<   RW; Range:[0x0, 0x3F]; Motion correction threshold of long and medium exposure, higher values make it more likely to interpret difference beteween the long exposure as noise.
    vs_uint8_t   lm_pos_weight;             ///<   RW; Range:[0x0, 0x3F]; Motion correction weight of long and medium exposure, lower values make it more likely to interpret difference beteween the long exposure as noise.
    vs_uint8_t   lm_neg_weight;             ///<   RW; Range:[0x0, 0x3F]; Motion correction weight of long and medium exposure, higher values make it more likely to interpret difference beteween the long exposure as noise.
    vs_uint16_t  lm_med_noise_alpha_thresh; ///<   RW; Range:[0x0, 0xFFF]; Noise threshold coefficient of long and medium exposure.
    vs_uint32_t  lm_mc_blend_slope;         ///<   RW; Range:[0x0, 0x3FFFFF]; Long medium exposure motion compensates for blend slope.
    vs_uint8_t   lm_mc_blend_thresh;        ///<   RW; Range:[0x0, 0x3F]; Long medium exposure motion compensates for blend threshold.
    vs_uint16_t  lm_mc_blend_offset;        ///<   RW; Range:[0x0, 0xFFF]; Long medium exposure motion compensates for blend offset.
    vs_uint32_t  lm_mc_thresh_slope;        ///<   RW; Range:[0x0, 0x3FFFFF]; Long medium exposure motion compensation threshold slope.
    vs_uint32_t  lm_mc_thresh_thresh;       ///<   RW; Range:[0x0, 0xFFFFF]; Long medium exposure motion compensation threshold.
    vs_uint16_t  lm_mc_thresh_offset;       ///<   RW; Range:[0x0, 0xFFF]; Long medium exposure motion compensation threshold offset.
    vs_uint32_t  lm_mc_mag_thresh_slope;    ///<   RW; Range:[0x0, 0x3FFFFF]; Long medium exposure motion compensation mag threshold slope.
    vs_uint32_t  lm_mc_mag_thresh_thresh;   ///<   RW; Range:[0x0, 0xFFFFF]; Long medium exposure motion compensation mag threshold.
    vs_uint16_t  lm_mc_mag_thresh_offset;   ///<   RW; Range:[0x0, 0xFFF]; Long medium exposure motion compensation mag threshold offset.
    vs_uint16_t  lm_mc_mag_lblend_thresh;   ///<   RW; Range:[0x0, 0xFFF]; Long medium exposure motion compensation mag blend threshold.
    vs_uint16_t  mc_off_wb_offset;          ///<   RW; Range:[0x0, 0xFFF]; White balance offset value of motion compensation.
    vs_uint16_t  exposure_mask_thresh;      ///<   RW; Range:[0x0, 0xFFF]; Threshold for selection of exposure mask in blending regions.
    vs_bool_t    bwb_select;                ///<   RW; Range:[0, 1]; Selection signal of blend White balance.
    vs_bool_t    use_3x3_max;               ///<   RW; Range:[0, 1]; Enbale signal of 3x3 max filter.
    vs_bool_t    mc_off_nc_enable;          ///<   RW; Range:[0, 1]; Enable signal of noise compensation.
    vs_bool_t    lm_alg_select;             ///<   RW; Range:[0, 1]; Selection signal of L/M stitching algorithm.
    vs_uint16_t  mc_off_l_max;              ///<   RW; Range:[0x0, 0xFFF]; Maximum offset for long frame motion compensation.
    vs_uint16_t  mc_off_m_max;              ///<   RW; Range:[0x0, 0xFFF]; Maximum offset for medium frame motion compensation.
    vs_uint16_t  mc_off_s_max;              ///<   RW; Range:[0x0, 0xFFF]; Maximum offset for short frame motion compensation.
    vs_uint16_t  mc_off_vs_max;             ///<   RW; Range:[0x0, 0xFFF]; Maximum offset for very short frame motion compensation.
    vs_uint16_t  mc_off_nc_thresh_low;      ///<   RW; Range:[0x0, 0xFFF]; Lower limit of noise compensation threshold.
    vs_uint16_t  mc_off_nc_thresh_high;     ///<   RW; Range:[0x0, 0xFFF]; Higher limit of noise compensation threshold.
    vs_uint16_t  mc_off_nc_scale;           ///<   RW; Range:[0x0, 0xFFF]; Noise compensation scale.
    vs_isp_op_type_e  optype;               ///<   RW; Range:[0, 1]; Format:1.0; Operation type.
    vs_isp_wdr_stitch_manual_attr_s manual_attr;
    vs_isp_wdr_stitch_auto_attr_s    auto_attr;
} vs_isp_wdr_stitch_attr_s;

/**
 * @brief Defines ISP decompander attr.
 * @details
 */
typedef struct vs_isp_decompander_attr {
    vs_bool_t lookup0_enable;                                   ///<   RW; Range:[0x0, 0x1]; Enable signal of of decompander0 module.
    vs_bool_t lookup1_enable;                                   ///<   RW; Range:[0x0, 0x1]; Enable signal of of decompander1 module.
    vs_bool_t lookup0_offset_mode;                              ///<   RW; Range:[0x0, 0x1]; Lookup0 reflection mode for black offset region.
    vs_bool_t lookup1_offset_mode;                              ///<   RW; Range:[0x0, 0x1]; Lookup1 reflection mode for black offset region.
    vs_uint32_t  lookup_table0[VS_ISP_DECOMPANDER0_DATA_SIZE];  ///<   RW; Range:[0x0, 0xFFFFFFFF]; Lookup table of decompander0 module.
    vs_uint32_t  lookup_table1[VS_ISP_DECOMPANDER1_DATA_SIZE];  ///<   RW; Range:[0x0, 0xFFFFFFFF]; Lookup table of decompander1 module.
} vs_isp_decompander_attr_s;

/**
 * @brief Defines ISP ae  mode.
 * @details
 */
typedef enum vs_isp_ae_mode {
    E_ISP_AE_AUTO = 0,
    E_ISP_AE_FULL_MANUAL,
    E_ISP_AE_MODE_MAX
} vs_isp_ae_mode_e;

/**
 * @brief Defines ISP ae split preset
 * @details
 */
typedef enum vs_isp_ae_split_preset {
    E_ISP_AE_SPLIT_BALANCED = 0,
    E_ISP_AE_SPLIT_INTEGRATION_PRIORITY,
    E_ISP_AE_SPLIT_MAX
} vs_isp_ae_split_preset_e;

/**
 * @brief Defines vs isp auto exposure ctl attr
 * @details
 */
typedef struct vs_isp_ae_balanced_param {
    vs_uint32_t pi_coeff;              ///<  RW; Range:[0x1, 0x400]; Adjust speed of auto exposure.
    vs_uint32_t target_point;          ///<  RW; Range:[0x0, 0x400]; Target luma of auto exposure.
    vs_uint32_t tail_weight;           ///<  RW; Range:[0x0, 0x400]; Highlight weighting to avoid clipping (overflow).
    vs_uint32_t long_clip;             ///<  RW; Range:[0x0, 0xFF]; Maximum percentage of clipped pixels allowed in long exposure(WDR mode only).
    vs_uint32_t er_avg_coeff;          ///<  RW; Range:[0x0, 0x400]; Exposure ratio adjustment time(WDR mode only).
    vs_uint32_t hi_target_prc;         ///<  RW; Range:[0x0, 0x64]; Highlights percentage target for the tail of the histogarm.
    vs_uint32_t hi_target_p;           ///<  RW; Range:[0x0, 0x64]; Percentage of pixels that should be below hi_target_prc.
    vs_uint32_t enable_dra_gdg;        ///<  RW; Range:[0x0, 0x1]; Switch signal of using dra global gain.
    vs_uint32_t ae_tol;                ///<  RW; Range:[0x0, 0x64]; Tolerance value of auto exposure.
    vs_uint32_t ae_debug;              ///<  RW; Range:[0x0, 0x1]; Debug of auto exposure.
} vs_isp_ae_balanced_param_s;

/**
 * @brief Defines vs isp ae delay triggering
 * @details
 */
typedef struct vs_isp_ae_delay_trigger_attr
{
    vs_uint16_t over_exposure_delay;    ///<  RW; Range:[0x0, 0xFFFF]; Delay the over_exposure_delay frame adjustment exposure, when the image luma is greater than the target luma.
    vs_uint16_t under_exposure_delay;   ///<  RW; Range:[0x0, 0xFFFF]; Delay the under_exposure_delay frame adjustment exposure, when the image luma is less than the target luma.
} vs_isp_ae_delay_trigger_attr_s;

/**
 * @brief Defines vs isp manual exposure attribute
 * @details
 */
typedef struct vs_isp_exposure_manual_attr {
    vs_isp_op_type_e  exp_optype;              ///<  RW; Range:[0, 1]; Format:1.0; Manual or automatic exposure options.
    vs_isp_op_type_e  integration_optype;      ///<  RW; Range:[0, 1]; Format:1.0; Manual or automatic exposure time options.
    vs_isp_op_type_e  again_optype;            ///<  RW; Range:[0, 1]; Format:1.0; Manual or automatic sensor again options.
    vs_isp_op_type_e  dgain_optype;            ///<  RW; Range:[0, 1]; Format:1.0; Manual or automatic sensor dgain options.
    vs_isp_op_type_e  ispdgain_optype;         ///<  RW; Range:[0, 1]; Format:1.0; Manual or automatic isp dgain options.
    vs_uint32_t   exposure;                    ///<  RW; Range:[0x0, 0xFFFFFFFF]; Total system exposure.
    vs_uint32_t   integration_time;            ///<  RW; Range:[0x1, 0xFFFFFF]; Exposure time.
    vs_uint32_t    sensor_again;                ///<  RW; Range:[0x100, 0xFFFFFF]; Sensor analog gain.
    vs_uint32_t    sensor_dgain;                ///<  RW; Range:[0x100, 0xFFFFFF]; Sensor digital gain.
    vs_uint32_t    isp_dgain;                   ///<  RW; Range:[0x100, 0x2000]; Isp digital gain.
} vs_isp_exposure_manual_attr_s;

/**
 * @brief Defines vs isp auto exposure attribute
 * @details
 */
typedef struct vs_isp_exposure_auto_attr {
    vs_uint32_t   max_integration_time;                                 ///<  RW; Range:[0x0, 0xFFFFFF]; System max integration time.
    vs_uint32_t   min_integration_time;                                 ///<  RW; Range:[0x0, 0xFFFFFF]; System min integration time.
    vs_uint32_t    max_sensor_again;                                    ///<  RW; Range:[0x100, 0xFFFFFF]; System max sensor analog gain.
    vs_uint32_t    min_sensor_again;                                    ///<  RW; Range:[0x100, 0xFFFFFF]; System min sensor analog gain.
    vs_uint32_t    max_sensor_dgain;                                    ///<  RW; Range:[0x100, 0xFFFFFF]; System max sensor digital gain.
    vs_uint32_t    min_sensor_dgain;                                    ///<  RW; Range:[0x100, 0xFFFFFF]; System min sensor digital gain.
    vs_uint32_t    max_isp_dgain;                                       ///<  RW; Range:[0x100, 0x2000]; System max isp digital gain.
    vs_uint32_t    min_isp_dgain;                                       ///<  RW; Range:[0x100, 0x2000]; System min isp digital gain.
    vs_bool_t     antiflicker_enable;                                   ///<  RW; Range:[0, 1]; Format:1.0; Enabale of anti flicker.
    vs_uint8_t    antiflicker_frequency;                                ///<  RW; Range:[0x0, 0xFF]; System anti flicker frequency.
    vs_uint32_t   ae_control_hdr_target[VS_ISP_AUTO_ISO_STRENGTH_SIZE]; ///<  RW; Range:[0x0, 0x400]; AE minmum target value for HDR scenes.
    vs_isp_ae_balanced_param_s ae_control;
    vs_uint8_t    ae_correction[VS_ISP_AE_CORRECTION_SIZE];             ///<  RW; Range:[0x0, 0xFF]; LUT defineing AE_comp values that will offset the EV value.
    vs_uint32_t   ae_exposure_correction[VS_ISP_AE_CORRECTION_SIZE];    ///<  RW; Range:[0x0, 0xFFFFFFFF]; LUT defines lux in terms of EV.
    vs_isp_ae_delay_trigger_attr_s ae_delay_trigger;
    vs_isp_shutter_mode_e shutter_mode;                                 ///<  RW; Range:[0x0, 0x1];
    vs_uint32_t gain_threshold;                                         ///<  RW; Range:[0x100, 0xFFFFFF];  Gain threshold of slow shutter.
} vs_isp_exposure_auto_attr_s;

/**
 * @brief Defines vs isp ae exp partitions attribute
 * @details
 */
typedef struct vs_isp_exp_partitions_attr {
    vs_uint32_t  int_time;                    ///<  RW; Range:[0x0, 0xFFFFFF]; Exposure time.
    vs_uint32_t sensor_again;                 ///<  RW; Range:[0x0, 0xFFFFFF]; Sensor analog gain.
    vs_uint32_t sensor_dgain;                 ///<  RW; Range:[0x0, 0xFFFFFF]; Sensor digital gain.
    vs_uint32_t isp_dgain;                    ///<  RW; Range:[0x0, 0x2000]; Isp digital gain.
}vs_isp_exp_partitions_attr_s;

/**
 * @brief Defines vs isp ae wdr base frame enum
 * @details
 */
typedef enum vs_isp_ae_wdr_base_frame {
    E_ISP_AE_WDR_BASE_FRAME_SHORT = 0,
    E_ISP_AE_WDR_BASE_FRAME_LONG,
    E_ISP_AE_WDR_BASE_FRAME_MAX
} vs_isp_ae_wdr_base_frame_e;

/**
 * @brief Defines vs isp ae region exposure attribute
 * @details
 */
typedef struct vs_isp_ae_region_exposure_attr
{
    vs_bool_t enable;                                     ///<  RW; Range:[0, 1]; Enable signal of region exposure.
    vs_rect_s roi;
} vs_isp_ae_region_exposure_attr_s;

/**
 * @brief Defines vs isp ae algorithm attribute
 * @details
 */
typedef struct vs_isp_ae_attr {
    vs_isp_ae_mode_e  ae_mode_id;                   ///<  RW; Range:[0, 1]; Auto exposure mode.
    vs_isp_ae_split_preset_e  ae_split_preset_id;   ///<  RW; Range:[0, 1]; AE automatic exposure strategy mode selection.
    vs_isp_exp_partitions_attr_s exp_partitions[VS_ISP_AE_EXP_PARTITIONS_SIZE];
    vs_uint32_t   ae_compensation_id;               ///<  RW; Range:[0x0, 0xFF]; Auto exposure compensation value.
    vs_isp_exposure_manual_attr_s  manual_attr;
    vs_isp_exposure_auto_attr_s    auto_attr;
    vs_isp_ae_wdr_base_frame_e wdr_base_frame;
    vs_isp_ae_region_exposure_attr_s region_exposure;
} vs_isp_ae_attr_s;

/**
 * @brief Defines vs isp wdr exposure manual attribute
 * @details
 */
typedef struct vs_isp_wdr_exp_manual_attr {
    vs_uint32_t   exp_ratio;                   ///<  RW; Range:[0x0, 0xFFFF]; System exposure ratio.
} vs_isp_wdr_exp_manual_attr_s;

/**
 * @brief Defines vs isp wdr exposure auto attribute
 * @details
 */
typedef struct vs_isp_wdr_exp_auto_attr {
    vs_uint32_t   max_exp_ratio;               ///<  RW; Range:[0x0, 0xFFFF]; System max exposure ratio.
    vs_uint32_t   min_exp_ratio;               ///<  RW; Range:[0x0, 0xFFFF]; System min exposure ratio.
} vs_isp_wdr_exp_auto_attr_s;

/**
 * @brief Defines vs isp wdr exp attribute
 * @details
 */
typedef struct vs_isp_wdr_exp_attr {
    vs_isp_op_type_e  optype;                 ///<  RW; Range:[0x0, 0x1]; System manual exposure ratio.
    vs_isp_wdr_exp_manual_attr_s  manual_attr;
    vs_isp_wdr_exp_auto_attr_s    auto_attr;
}vs_isp_wdr_exp_attr_s;

/**
 * @brief Defines vs isp awb algorithm mode
 * @details
 */
typedef enum vs_isp_awb_mode {
    E_ISP_AWB_AUTO = 0,
    E_ISP_AWB_MANUAL,
    E_ISP_AWB_DAY_LIGHT,
    E_ISP_AWB_CLOUDY,
    E_ISP_AWB_INCANDESCENT,
    E_ISP_AWB_FLOURESCENT,
    E_ISP_AWB_TWILIGHT,
    E_ISP_AWB_SHADE,
    E_ISP_AWB_WARM_FLOURESCENT,
    E_ISP_AWB_COLOR_TEMP,
    E_ISP_AWB_MODE_MAX
} vs_isp_awb_mode_e;

/**
 * @brief Defines vs isp wb manual attribute
 * @details
 */
typedef struct vs_isp_wb_manual_attr {
    vs_uint16_t   awb_r_gain;          ///<  RW; Range:[0x0, 0xFFF]; Awb red gain; level value.
    vs_uint16_t   awb_g_even_gain;     ///<  RW; Range:[0x0, 0xFFF]; Awb green even gain;
    vs_uint16_t   awb_g_odd_gain;      ///<  RW; Range:[0x0, 0xFFF]; Awb green odd gain;
    vs_uint16_t   awb_b_gain;          ///<  RW; Range:[0x0, 0xFFF]; Awb blue gain;
} vs_isp_wb_manual_attr_s;

typedef struct vs_isp_wb_light_src {
    vs_uint16_t light_rg;             ///<  RW; Range:[0x0, 0xFFFF]; Extra light source R/G.
    vs_uint16_t light_bg;             ///<  RW; Range:[0x0, 0xFFFF]; Extra light source B/G.
}vs_isp_wb_light_src_s;

typedef struct vs_isp_wb_mix_light_attr {
    vs_uint32_t enable_awb_mix_light;    ///<  RW; Range:[0x0, 0x1]; Enable Signal of mix light.
    vs_uint32_t lux_low;                 ///<  RW; Range:[0x0, 0xFFFFFFFF]; Minimum illumination of mix light source.
    vs_uint32_t lux_high;                ///<  RW; Range:[0x0, 0xFFFFFFFF]; Maximum illumination of mix light source.
    vs_uint32_t contrast_threshold;      ///<  RW; Range:[0x0, 0xFFFFFFFF]; Contrast threshold of mix light source.
    vs_uint32_t bg_threshold;            ///<  RW; Range:[0x0, 0xFFFFFFFF]; Threshold of B/G.
    vs_uint32_t bg_weight;               ///<  RW; Range:[0x0, 0xFFFFFFFF]; Weight of B/G.
    vs_uint32_t rghigh_lut_max;          ///<  RW; Range:[0x0, 0xFFFFFFFF]; Thereshold of R/G. less that this value will be used max weight.
    vs_uint32_t rghigh_lut_min;          ///<  RW; Range:[0x0, 0xFFFFFFFF]; Thereshold of R/G. greater that this value will be used max weight.
    vs_uint32_t debug;                   ///<  RW; Range:[0x0, 0x1]; Debug mode selection.
} vs_isp_wb_mix_light_attr_s;

typedef struct vs_isp_awb_cbcr_track_attr
{
    vs_bool_t enable;                                     ///<   RW; Range:[0x0, 0x1]; Enable signal of crcb track.
    vs_uint16_t cr_min[VS_ISP_AUTO_ISO_STRENGTH_SIZE];    ///<   RW; Range:[0x0, 0xFFF]; Minimum R/G color ratio.
    vs_uint16_t cr_max[VS_ISP_AUTO_ISO_STRENGTH_SIZE];    ///<   RW; Range:[0x0, 0xFFF]; Maximum R/G color ratio.
    vs_uint16_t cb_min[VS_ISP_AUTO_ISO_STRENGTH_SIZE];    ///<   RW; Range:[0x0, 0xFFF]; Minimum B/G color ratio.
    vs_uint16_t cb_max[VS_ISP_AUTO_ISO_STRENGTH_SIZE];    ///<   RW; Range:[0x0, 0xFFF]; Maximum B/G color ratio.
    vs_uint16_t cr_low[VS_ISP_AUTO_ISO_STRENGTH_SIZE];    ///<   RW; Range:[0x0, 0xFFF]; High R/G color ratio.
    vs_uint16_t cr_high[VS_ISP_AUTO_ISO_STRENGTH_SIZE];   ///<   RW; Range:[0x0, 0xFFF]; Low R/G color ratio.
    vs_uint16_t cb_low[VS_ISP_AUTO_ISO_STRENGTH_SIZE];    ///<   RW; Range:[0x0, 0xFFF]; High B/G color ratio.
    vs_uint16_t cb_high[VS_ISP_AUTO_ISO_STRENGTH_SIZE];   ///<   RW; Range:[0x0, 0xFFF]; Low B/G color ratio.
} vs_isp_awb_cbcr_track_attr_s;

typedef enum vs_isp_awb_multi_light_src_type
{
    E_ISP_AWB_MULTI_LIGHT_SRC_TYPE_SATURATION = 0,
    E_ISP_AWB_MULTI_LIGHT_SRC_TYPE_COLOR_MATRIX,
    E_ISP_AWB_MULTI_LIGHT_SRC_TYPE_MAX
}vs_isp_awb_multi_light_src_type_e;

typedef struct vs_isp_awb_multi_light_src_attr
{
    vs_bool_t enable;                                     ///<   RW; Range:[0x0, 0x1]; Enbale signal of multi light source logic.
    vs_isp_awb_multi_light_src_type_e type;               ///<   RW; Range:[0x0, 0x1]; Saturation reduction mode selection.
    vs_uint16_t lux_thr;                                  ///<   RW; Range:[0x0, 0xFFFF]; Threshold of lux.
    vs_uint8_t  thr1;                                     ///<   RW; Range:[0x0, 0xFF]; Minimum threshold for the conversion of the mixed light index to the desaturation index.
    vs_uint8_t  thr2;                                     ///<   RW; Range:[0x0, 0xFF]; Maximum threshold for the conversion of the mixed light index to the desaturation index.
    vs_uint8_t  slope;                                    ///<   RW; Range:[0x0, 0xFF]; Conversion coefficient from the mixed light index to the saturation reduction index.
} vs_isp_awb_multi_light_src_attr_s;

/**
 * @brief Defines vs isp wb auto attribute
 * @details
 */
typedef struct vs_isp_wb_auto_attr {
    vs_uint16_t   rg_pos[VS_ISP_AWB_RG_BG_POS_SIZE];                                            ///<  RW; Range:[0x0, 0xFFFF]; R/G  position of all illuminants.
    vs_uint16_t   bg_pos[VS_ISP_AWB_RG_BG_POS_SIZE];                                            ///<  RW; Range:[0x0, 0xFFFF]; B/G  position of all illuminants.
    vs_isp_wb_light_src_s   light_src[VS_ISP_AWB_LIGHT_SRC_LEN];                                ///<  RW; Range:[0x0, 0xFFFF]; Extra light source.
    vs_uint16_t   extra_light_src_valid_rows;                                                   ///<  RW; Range: [0, 10]; Format:4.0; Valid number of extra light sources.
    vs_uint16_t   sky_lux_th;                                                                   ///<  RW; Range:[0x0, 0xFFFF]; Lux threshold of sky.
    vs_isp_wb_mix_light_attr_s   mix_light_parameters;
    vs_uint16_t   awb_colour_preference[VS_ISP_AWB_COLOR_PREFER_SIZE];                          ///<  RW; Range:[0x0, 0xFFFF]; AWB colour preference.
    vs_bool_t     evtolux_probability_enable;                                                   ///<  RW; Range:[0, 1]; Enable signal of ev2lux.
    vs_uint32_t   evtolux_ev_lut[VS_ISP_AWB_EVTOLUX_LUT_SIZE];                                  ///<  RW; Range:[0, 0xFFFFFFFF]; EV data for EV to lux calibration.
    vs_uint32_t   evtolux_lux_lut[VS_ISP_AWB_EVTOLUX_LUT_SIZE];                                 ///<  RW; Range:[0, 0xFFFFFFFF]; Lux data for EV to lux calibration.
    vs_uint16_t   awb_bg_max_gain[VS_ISP_AUTO_ISO_STRENGTH_SIZE];                               ///<  RW; Range:[0x0, 0xFFFF]; Maximum gain of AWB B/G.
    vs_uint16_t   static_wb[VS_ISP_AWB_STATIC_WB_SIZE];                                         ///<  RW; Range:[0x0, 0xFFFF]; Coefficient of static white balance .
    vs_uint16_t   wb_strength_sky[VS_ISP_AWB_SKY_STRENGTH];                                     ///<  RW; Range:[0x0, 0xFFFF]; White balance strength of scenes over sky_lux_th.
    vs_uint16_t   awb_warming_cct[VS_ISP_AWB_WARMING_CCT_SIZE];                                 ///<  RW; Range:[0x0, 0xFFFF]; Calibration light color temperature.
    vs_uint16_t   awb_warming_ls_a[VS_ISP_AWB_WARMING_LS_SIZE];                                 ///<  RW; Range:[0x0, 0xFFFF]; White balance offset strength under A light.
    vs_uint16_t   awb_warming_ls_d50[VS_ISP_AWB_WARMING_LS_SIZE];                               ///<  RW; Range:[0x0, 0xFFFF]; White balance offset strength under D50 light.
    vs_uint16_t   awb_warming_ls_d75[VS_ISP_AWB_WARMING_LS_SIZE];                               ///<  RW; Range:[0x0, 0xFFFF]; White balance offset strength under D75 light.
    vs_uint8_t    awb_avg_coef;                                                                 ///<  RW; Range:[0x0, 0xFF]; AWB adjust the speed.
    vs_uint16_t   color_temp[VS_ISP_AWB_LIGHT_SRC_LEN];                                         ///<  RW; Range:[0x0, 0xFFFF]; Range of color temperatures in mired.
    vs_uint16_t   ct_rg_pos_calc[VS_ISP_AWB_LIGHT_SRC_LEN];                                     ///<  RW; Range:[0x0, 0xFFFF]; R/G value of given color temp.
    vs_uint16_t   ct_bg_pos_calc[VS_ISP_AWB_LIGHT_SRC_LEN];                                     ///<  RW; Range:[0x0, 0xFFFF]; B/G value of given color temp.
    vs_uint8_t    ct_rg_bg_valid_rows;                                                          ///<  RW; Range: [2, 10]; Calibration planking light src valid rows.
    vs_uint16_t   ct30pos;                                                                      ///<  RW; Range:[0x0, 0xFFFF]; D30 illuminant position in color_temp.
    vs_uint16_t   ct40pos;                                                                      ///<  RW; Range:[0x0, 0xFFFF]; D40 illuminant position in color_temp.
    vs_uint16_t   ct65pos;                                                                      ///<  RW; Range:[0x0, 0xFFFF]; D65 illuminant position in color_temp.
    vs_uint16_t   mesh_color_temperature[VS_ISP_AWB_RG_BG_POS_SIZE][VS_ISP_AWB_RG_BG_POS_SIZE]; ///<  RW; Range:[0x0, 0xFFFF]; Color temperature mesh.
    vs_uint16_t   mesh_rgbg_weight[VS_ISP_AWB_RG_BG_POS_SIZE][VS_ISP_AWB_RG_BG_POS_SIZE];       ///<  RW; Range:[0x0, 0xFFFF]; R/G and B/G weight mesh.
    vs_uint16_t   mesh_ls_weight[VS_ISP_AWB_RG_BG_POS_SIZE][VS_ISP_AWB_RG_BG_POS_SIZE];         ///<  RW; Range:[0x0, 0xFFFF]; Light source weight mesh.
    vs_uint8_t    tolerance;                                                                    ///<  RW; Range:[0x0, 0xFF]; Tolerance of AWB.
    vs_uint16_t   luxlow_lut[VS_ISP_AWB_LUX_LUT];                                               ///<  RW; Range:[0,0xFFFF]; Low color temperature possibility table corresponding to lux.
    vs_uint16_t   luxhigh_lut[VS_ISP_AWB_LUX_LUT];                                              ///<  RW; Range:[0,0xFFFF]; High color temperature possibility table corresponding to lux.
    vs_uint16_t   lowlight_lux_th;                                                              ///<  RW; Range:[0x0, 0xFFFF]; Threshold of low light level.
    vs_uint16_t   wb_strength_lowlight[VS_ISP_AWB_LOWLIGHT_STRENGTH];                           ///<  RW; Range:[0x0, 0xFFFF]; White balance strength of scenes less than lowlight_lux_th.
    vs_isp_awb_cbcr_track_attr_s cbcr_track;
    vs_isp_awb_multi_light_src_attr_s multi_light_src;
} vs_isp_wb_auto_attr_s;

/**
 * @brief Defines vs isp awb algorithm scene preset struct
 * @details
 */
typedef struct vs_isp_awb_scene_presets {
    vs_uint16_t cr;
    vs_uint16_t cb;
}vs_isp_awb_scene_presets_s;

/**
 * @brief Defines vs isp awb algorithm attribute
 * @details
 */
typedef struct vs_isp_awb_attr {
    vs_isp_awb_mode_e awb_mode_id;                                          ///<  RW; Range:[0, 8]; AWB mode id.
    vs_isp_awb_scene_presets_s scene_presets[VS_ISP_AWB_SCENE_PRESET_SIZE]; ///<  RW; Range:[0x0, 0xFFFF]; AWB scene presets R/G and B/G value.
    vs_uint16_t    awb_temperature_id;                                      ///<  RW; Range:[0x0, 0xFFFF]; AWB temperature id.
    vs_isp_wb_manual_attr_s manual_attr;
    vs_isp_wb_auto_attr_s   auto_attr;
}vs_isp_awb_attr_s;



/**
 * @brief Defines vs isp pre sharpen manual attribute
 * @details
 */
typedef struct vs_isp_presharpen_manual_attr {
    vs_uint8_t  edge_total_gain_pre;    ///< RW; Range:[0x0, 0xFF]; Total edge enhancement strength.
    vs_uint16_t sigma_range_inv_pre;    ///< RW; Range:[0x01, 0xFFF]; The larger, the less will the filter preserve edges. Recommend lower value at high gain.
    vs_uint8_t  bila_alpha_pre;         ///< RW; Range:[0x0, 0xFF]; The larger, the input pixel value blends more over edge-preserving filter result. Recommend lower value at high gain.
    vs_int8_t   bf_dt_weight_pre;       ///< RW; Range:[-128, 127]; Ratio of high frequency detail with edge preserving.
    vs_int8_t   bf_dt_weight2_pre;      ///< RW; Range:[-128, 127]; Ratio of mid frequency detail with edge preserving.
    vs_int8_t   dog_dt_weight_pre;      ///< RW; Range:[-128, 127]; Ratio of mid frequencydetail without edge preserving.
    vs_uint8_t  alpha_freq_pre;         ///< RW; Range:[0x0, 0xFF]; The ratio over high frequency detail without edge preserving over mid frequency detail without edge preserving.
    vs_uint8_t  hedge_scale_pre;        ///< RW; Range:[0x0, 0xFF]; Horizontal edge scaler.
    vs_uint8_t  vedge_scale_pre;        ///< RW; Range:[0x0, 0xFF]; Vertical edge scaler.
    vs_uint8_t  std_total_gain_pre;     ///< RW; Range:[0x0, 0xFF]; Total detail enhancement strength.
    vs_uint8_t  base_alpha1_pre;        ///< RW; Range:[0x0, 0xFF]; Ratio of base layer. Bigger value tends to use the base layer that after the edge preserving filter, otherwise use non-edge preserving filter value.
    vs_uint8_t  base_alpha2_pre;        ///< RW; Range:[0x0, 0xFF]; Ratio of base layer. Smaller value indicates bigger weight of input value, otherwise use base_alpha1_pre result.
    vs_uint8_t  luma_total_gain_pre;    ///< RW; Range:[0x0, 0xFF]; Total luma sharpen strength.
    vs_uint8_t  th_neg_pre;             ///< RW; Range:[0x0, 0xFF]; Undershoot control value.
    vs_uint8_t  th_pos_pre;             ///< RW; Range:[0x0, 0xFF]; Overshoot control value.

    vs_uint8_t gain_usr_pos_pre;                                            ///< RW; Range:[0x0, 0xFF]; Pos-sharpen total strength.
    vs_uint8_t gain_usr_neg_pre;                                            ///< RW; Range:[0x0, 0xFF]; Neg-sharpen total strength.
    vs_uint8_t var_blend_pre;                                               ///< RW; Range:[0x0, 0xFF]; Kernel size for calculating variance.
    vs_uint8_t var_scale_pre;                                               ///< RW; Range:[0x0, 0xFF]; Variance scaler value.
    vs_uint8_t luma_array_pos_pre[VS_ISP_PRESHARPEN_ARRAY_SIZE];            ///< RW; Range:[0x0, 0xFF]; Luma - pos-sharpen strength LUT.
    vs_uint8_t luma_array_neg_pre[VS_ISP_PRESHARPEN_ARRAY_SIZE];            ///< RW; Range:[0x0, 0xFF]; Luma - neg-sharpen strength LUT.
    vs_uint16_t color_str_gain_pre[VS_ISP_PRESHARPEN_COLOR_STR_GAIN_SIZE];  ///< RW; Range:[0x0, 0xFFF]; Corresponds to the sharpen strength of 4 chroma areas. 256->no adjustment, 128->less sharpen, 256->stronger sharpen.
    vs_uint16_t std_array_pre[VS_ISP_PRESHARPEN_ARRAY_SIZE];                ///< RW; Range:[0x0, 0xFFF]; Detail enhancement strength for different textures.
    vs_uint16_t edge_array_pre[VS_ISP_PRESHARPEN_ARRAY_SIZE];               ///< RW; Range:[0x0, 0xFFF]; Edge enhancement strength for different edges.

    vs_uint8_t alpha_detail_pre;                                            ///< RW; Range:[0x0, 0xFF]; The blend ratio of different frequency detail. Smaller value will increase the ratio of BFDTWeightPre, BFDTWeight2Pre and DogDTWeightPr, otherwise AlphaFreqPre increases.
    vs_uint8_t edge_alpha_pre;                                              ///< RW; Range:[0x0, 0xFF]; The blend ratio of variance mixed to the edge detection.
    vs_uint8_t u_center_pre[VS_ISP_PRESHARPEN_UV_SIZE];                     ///< RW; Range:[0x0, 0xFF]; 4 sets params, correspond to the U value of 4 different sets of chroma.
    vs_uint8_t v_center_pre[VS_ISP_PRESHARPEN_UV_SIZE];                     ///< RW; Range:[0x0, 0xFF]; 4 sets params, correspond to the V value of 4 different sets of chroma.
    vs_uint8_t u_radius_pre[VS_ISP_PRESHARPEN_UV_SIZE];                     ///< RW; Range:[0x0, 0x3F]; 4 sets params, correspond to the U channel radius of 4 different sets of chroma.
    vs_uint8_t v_radius_pre[VS_ISP_PRESHARPEN_UV_SIZE];                     ///< RW; Range:[0x0, 0x3F]; 4 sets params, correspond to the V channel radius of 4 different sets of chroma.
} vs_isp_presharpen_manual_attr_s;

/**
 * @brief Defines vs isp pre sharpen auto attribute
 * @details
 */
typedef struct vs_isp_presharpen_auto_attr {
    vs_uint8_t  edge_total_gain_pre[VS_ISP_AUTO_ISO_STRENGTH_SIZE]; ///< RW; Range:[0x0, 0xFF]; Total edge enhancement strength.
    vs_uint16_t sigma_range_inv_pre[VS_ISP_AUTO_ISO_STRENGTH_SIZE]; ///< RW; Range:[0x01, 0xFFF]; The larger, the less will the filter preserve edges. Recommend lower value at high gain.
    vs_uint8_t  bila_alpha_pre[VS_ISP_AUTO_ISO_STRENGTH_SIZE];      ///< RW; Range:[0x0, 0xFF]; The larger, the input pixel value blends more over edge-preserving filter result. Recommend lower value at high gain.
    vs_int8_t   bf_dt_weight_pre[VS_ISP_AUTO_ISO_STRENGTH_SIZE];    ///< RW; Range:[-128, 127]; Ratio of high frequency detail with edge preserving.
    vs_int8_t   bf_dt_weight2_pre[VS_ISP_AUTO_ISO_STRENGTH_SIZE];   ///< RW; Range:[-128, 127]; Ratio of mid frequency detail with edge preserving.
    vs_int8_t   dog_dt_weight_pre[VS_ISP_AUTO_ISO_STRENGTH_SIZE];   ///< RW; Range:[-128, 127]; Ratio of mid frequencydetail without edge preserving.
    vs_uint8_t  alpha_freq_pre[VS_ISP_AUTO_ISO_STRENGTH_SIZE];      ///< RW; Range:[0x0, 0xFF]; The ratio over high frequency detail without edge preserving over mid frequency detail without edge preserving.
    vs_uint8_t  hedge_scale_pre[VS_ISP_AUTO_ISO_STRENGTH_SIZE];     ///< RW; Range:[0x0, 0xFF]; Horizontal edge scaler.
    vs_uint8_t  vedge_scale_pre[VS_ISP_AUTO_ISO_STRENGTH_SIZE];     ///< RW; Range:[0x0, 0xFF]; Vertical edge scaler.
    vs_uint8_t  std_total_gain_pre[VS_ISP_AUTO_ISO_STRENGTH_SIZE];  ///< RW; Range:[0x0, 0xFF]; Total detail enhancement strength.
    vs_uint8_t  base_alpha1_pre[VS_ISP_AUTO_ISO_STRENGTH_SIZE];     ///< RW; Range:[0x0, 0xFF]; Ratio of base layer. Bigger value tends to use the base layer that after the edge preserving filter, otherwise use non-edge preserving filter value.
    vs_uint8_t  base_alpha2_pre[VS_ISP_AUTO_ISO_STRENGTH_SIZE];     ///< RW; Range:[0x0, 0xFF]; Ratio of base layer. Smaller value indicates bigger weight of input value, otherwise use base_alpha1_pre result.
    vs_uint8_t  luma_total_gain_pre[VS_ISP_AUTO_ISO_STRENGTH_SIZE]; ///< RW; Range:[0x0, 0xFF]; Total luma sharpen strength.
    vs_uint8_t  th_neg_pre[VS_ISP_AUTO_ISO_STRENGTH_SIZE];          ///< RW; Range:[0x0, 0xFF]; Undershoot control value.
    vs_uint8_t  th_pos_pre[VS_ISP_AUTO_ISO_STRENGTH_SIZE];          ///< RW; Range:[0x0, 0xFF]; Overshoot control value.

    vs_uint8_t gain_usr_pos_pre[VS_ISP_AUTO_ISO_STRENGTH_SIZE];                                             ///< RW; Range:[0x0, 0xFF]; Pos-sharpen total strength.
    vs_uint8_t gain_usr_neg_pre[VS_ISP_AUTO_ISO_STRENGTH_SIZE];                                             ///< RW; Range:[0x0, 0xFF]; Neg-sharpen total strength.
    vs_uint8_t var_blend_pre[VS_ISP_AUTO_ISO_STRENGTH_SIZE];                                                ///< RW; Range:[0x0, 0xFF]; Kernel size for calculating variance.
    vs_uint8_t var_scale_pre[VS_ISP_AUTO_ISO_STRENGTH_SIZE];                                                ///< RW; Range:[0x0, 0xFF]; Variance scaler value.
    vs_uint8_t luma_array_pos_pre[VS_ISP_AUTO_ISO_STRENGTH_SIZE][VS_ISP_PRESHARPEN_ARRAY_SIZE];             ///< RW; Range:[0x0, 0xFF]; Luma - pos-sharpen strength LUT.
    vs_uint8_t luma_array_neg_pre[VS_ISP_AUTO_ISO_STRENGTH_SIZE][VS_ISP_PRESHARPEN_ARRAY_SIZE];             ///< RW; Range:[0x0, 0xFF]; Luma - neg-sharpen strength LUT.
    vs_uint16_t color_str_gain_pre[VS_ISP_AUTO_ISO_STRENGTH_SIZE][VS_ISP_PRESHARPEN_COLOR_STR_GAIN_SIZE];   ///< RW; Range:[0x0, 0xFFF]; Corresponds to the sharpen strength of 4 chroma areas. 256->no adjustment, 128->less sharpen, 256->stronger sharpen.
    vs_uint16_t std_array_pre[VS_ISP_AUTO_ISO_STRENGTH_SIZE][VS_ISP_PRESHARPEN_ARRAY_SIZE];                 ///< RW; Range:[0x0, 0xFFF]; Detail enhancement strength for different textures.
    vs_uint16_t edge_array_pre[VS_ISP_AUTO_ISO_STRENGTH_SIZE][VS_ISP_PRESHARPEN_ARRAY_SIZE];                ///< RW; Range:[0x0, 0xFFF]; Edge enhancement strength for different edges.

    vs_uint8_t alpha_detail_pre[VS_ISP_AUTO_ISO_STRENGTH_SIZE];                                             ///< RW; Range:[0x0, 0xFF];The blend ratio of different frequency detail. Smaller value will increase the ratio of BFDTWeightPre, BFDTWeight2Pre and DogDTWeightPr, otherwise AlphaFreqPre increases.
    vs_uint8_t edge_alpha_pre[VS_ISP_AUTO_ISO_STRENGTH_SIZE];                                               ///< RW; Range:[0x0, 0xFF]; The blend ratio of variance mixed to the edge detection.
    vs_uint8_t u_center_pre[VS_ISP_AUTO_ISO_STRENGTH_SIZE][VS_ISP_PRESHARPEN_UV_SIZE];                      ///< RW; Range:[0x0, 0xFF]; 4 sets params, correspond to the U value of 4 different sets of chroma.
    vs_uint8_t v_center_pre[VS_ISP_AUTO_ISO_STRENGTH_SIZE][VS_ISP_PRESHARPEN_UV_SIZE];                      ///< RW; Range:[0x0, 0xFF]; 4 sets params, correspond to the V value of 4 different sets of chroma.
    vs_uint8_t u_radius_pre[VS_ISP_AUTO_ISO_STRENGTH_SIZE][VS_ISP_PRESHARPEN_UV_SIZE];                      ///< RW; Range:[0x0, 0x3F]; 4 sets params, correspond to the U channel radius of 4 different sets of chroma.
    vs_uint8_t v_radius_pre[VS_ISP_AUTO_ISO_STRENGTH_SIZE][VS_ISP_PRESHARPEN_UV_SIZE];                      ///< RW; Range:[0x0, 0x3F]; 4 sets params, correspond to the V channel radius of 4 different sets of chroma.
} vs_isp_presharpen_auto_attr_s;

/**
 * @brief Defines vs isp pre_sharpen algorithm attribute
 * @details
 */
typedef struct vs_isp_presharpen_attr {
    vs_bool_t enable;                                                       ///< RW; Range:[0, 1]; Enable signal of preSharpen.
    vs_bool_t presharpen_debug1;                                            ///< RW; Range:[0, 1]; Debug enable. 1 chooses directional edges, 0 chooses undirectional edges.
    vs_bool_t presharpen_debug2;                                            ///< RW; Range:[0, 1]; Debug enable. 1 outputs presharpen_debug1 result.
    vs_uint8_t gauss0_data_pre[VS_ISP_PRESHARPEN_GAUSS_SIZE][VS_ISP_PRESHARPEN_GAUSS_SIZE]; ///< RW; Range:[0x0, 0xFF]; Coefficients of the first Gauss filter.
    vs_uint8_t gauss1_data_pre[VS_ISP_PRESHARPEN_GAUSS_SIZE][VS_ISP_PRESHARPEN_GAUSS_SIZE]; ///< RW; Range:[0x0, 0xFF]; Coefficients of the second Gauss filter.
    vs_uint8_t gauss_in_bila_pre[VS_ISP_PRESHARPEN_BILA_SIZE][VS_ISP_PRESHARPEN_BILA_SIZE]; ///< RW; Range:[0x0, 0xFF]; Coefficients of the third filter.
    vs_uint8_t h1_ctrl_pre[VS_ISP_PRESHARPEN_HV_CRTL_SIZE];                 ///< RW; Range:[0x0, 0xF]; Horizontal filter. Sum should be 16.
    vs_uint8_t v1_ctrl_pre[VS_ISP_PRESHARPEN_HV_CRTL_SIZE];                 ///< RW; Range:[0x0, 0xF]; Vertical filter. Sum should be 16.
    vs_bool_t edge_gain_enable_pre;                                         ///< RW; Range:[0, 1]; Adaptive edge sharpen enable signal. 1 enable edge_array_pre term.
    vs_bool_t std_gain_enable_pre;                                          ///< RW; Range:[0, 1]; Adaptive detail sharpen enable signal. 1 enable std_array_pre term.
    vs_bool_t luma_gain_enable_pre;                                         ///< RW; Range:[0, 1]; Adaptive luma sharpen enable signal. 1 enable luma_array_pos/neg term.
    vs_uint8_t detail_luma_alpha_pos_ctrl_pre;                              ///< RW; Range:[0x0, 0x3F]; Ratio of sharpened luminance of pos-sharpen when use luma-LUT.
    vs_uint8_t detail_luma_alpha_neg_ctrl_pre;                              ///< RW; Range:[0x0, 0x3F]; Ratio of sharpened luminance of neg-sharpen when use luma-LUT.
    vs_uint8_t min_out_ctrl_pre;                                            ///< RW; Range:[0x0, 0x7F]; Min value output of Y channel.
    vs_uint8_t max_out_ctrl_pre;                                            ///< RW; Range:[0x80, 0xFF]; Max value output of Y channel.

    vs_bool_t color_gain_enable_pre;                                        ///< RW; Range:[0, 1]; Color gain enable signal. 1 enable color_str_gain_pre.
    vs_isp_op_type_e optype;                                                ///< RW; Range:[0, 1]; Presharpen operation mode select.
    vs_isp_presharpen_manual_attr_s manual_attr;
    vs_isp_presharpen_auto_attr_s   auto_attr;
} vs_isp_presharpen_attr_s;

/**
 * @brief Defines vs isp nr3d manual attribute
 * @details
 */
typedef struct vs_isp_nr3d_manual_attr {
    vs_uint8_t blur_alpha;              ///< RW; Range:[0x0, 0x3F]; Size of temporal filter.
    vs_uint8_t dif_presion_y;           ///< RW; Range:[0x0, 0xFF]; Y channel noise estimation precision.
    vs_uint8_t dif_presion_uv;          ///< RW; Range:[0x0, 0xFF]; UV channel noise estimation precision.
    vs_uint8_t dark_str;                ///< RW; Range:[0x0, 0xFF]; Dark area Y channal temporal denoise strength.
    vs_uint8_t mid_str;                 ///< RW; Range:[0x0, 0xFF]; Middle-lighted area Y channal temporal denoise strength.
    vs_uint8_t light_str;               ///< RW; Range:[0x0, 0xFF]; Light area Y channal temporal denoise strength.
    vs_uint8_t uv_dark_str;             ///< RW; Range:[0x0, 0xFF]; Dark area UV channal temporal denoise strength.
    vs_uint8_t uv_mid_str;              ///< RW; Range:[0x0, 0xFF]; Middle-lighted area UV channal temporal denoise strength.
    vs_uint8_t uv_light_str;            ///< RW; Range:[0x0, 0xFF]; Light area UV channal temporal denoise strength.
    vs_uint8_t hl_noise_std_y;          ///< RW; Range:[0x01, 0x3F]; High-lighted area Y channal temporal denoise strength.
    vs_uint8_t hl_noise_std_u;          ///< RW; Range:[0x01, 0x3F]; High-lighted area U channal temporal denoise strength.
    vs_uint8_t hl_noise_std_v;          ///< RW; Range:[0x01, 0x3F]; High-lighted area V channal temporal denoise strength.

    vs_uint16_t strength0;              ///< RW; Range:[0x01, 0x0FFF]; Spatial denoise strength of high frequency noise in Y channel.
    vs_uint16_t strength1;              ///< RW; Range:[0x01, 0x0FFF]; Spatial denoise strength of middle frequency noise in Y channel.
    vs_uint16_t strength2;              ///< RW; Range:[0x01, 0x0FFF]; Spatial denoise strength of low frequency noise in Y channel.
    vs_uint8_t strength0_ym;            ///< RW; Range:[0x0, 0xFF]; Spatial high frequency denoise strength of moving area in Y channel.
    vs_uint8_t strength0_ys;            ///< RW; Range:[0x0, 0xFF]; Spatial high frequency denoise strength of static area in Y channel.
    vs_uint8_t strength1_ym;            ///< RW; Range:[0x0, 0xFF]; Spatial mid frequency denoise strength of moving area in Y channel.
    vs_uint8_t strength1_ys;            ///< RW; Range:[0x0, 0xFF]; Spatial mid frequency denoise strength of static area in Y channel.
    vs_uint8_t strength2_ym;            ///< RW; Range:[0x0, 0xFF]; Spatial low frequency denoise strength of moving area in Y channel.
    vs_uint8_t strength2_ys;            ///< RW; Range:[0x0, 0xFF]; Spatial low frequency denoise strength of static area in Y channel.

    vs_uint8_t alpha_static_layer0;     ///< RW; Range:[0x0, 0xFF]; Y channel high frequency blend ratio of static area after spatial denoise.
    vs_uint8_t alpha_motion_layer0;     ///< RW; Range:[0x0, 0xFF]; Y channel high frequency blend ratio of moving area after spatial denoise.
    vs_uint8_t alpha_static_layer1;     ///< RW; Range:[0x0, 0xFF]; Y channel mid frequency blend ratio of static area after spatial denoise.
    vs_uint8_t alpha_motion_layer1;     ///< RW; Range:[0x0, 0xFF]; Y channel mid frequency blend ratio of moving area after spatial denoise.
    vs_uint8_t alpha_static_layer2;     ///< RW; Range:[0x0, 0xFF]; Y channel low frequency blend ratio of static area after spatial denoise.
    vs_uint8_t alpha_motion_layer2;     ///< RW; Range:[0x0, 0xFF]; Y channel low frequency blend ratio of moving area after spatial denoise.

    vs_uint8_t uv_denoise_strength;     ///< RW; Range:[0x0, 0xFF]; Total UV channel spatial denoise strength.
    vs_uint16_t noise_level_scaler_y0;  ///< RW; Range:[0x0, 0xFFF]; High frequency reference denoise strength of Y channel to chroma channel.
    vs_uint16_t noise_level_scaler_u0;  ///< RW; Range:[0x0, 0xFFF]; High frequency u channel denoise strength.
    vs_uint16_t noise_level_scaler_v0;  ///< RW; Range:[0x0, 0xFFF]; High frequency v channel denoise strength.
    vs_uint16_t noise_level_scaler_y1;  ///< RW; Range:[0x0, 0xFFF]; Mid frequency reference denoise strength of Y channel to chroma channel.
    vs_uint16_t noise_level_scaler_u1;  ///< RW; Range:[0x0, 0xFFF]; Mid frequency u channel denoise strength.
    vs_uint16_t noise_level_scaler_v1;  ///< RW; Range:[0x0, 0xFFF]; Mid frequency v channel denoise strength.
    vs_uint16_t noise_level_scaler_y2;  ///< RW; Range:[0x0, 0xFFF]; Mid-low frequency reference denoise strength of Y channel to chroma channel.
    vs_uint16_t noise_level_scaler_u2;  ///< RW; Range:[0x0, 0xFFF]; Mid-low frequency u channel denoise strength.
    vs_uint16_t noise_level_scaler_v2;  ///< RW; Range:[0x0, 0xFFF]; Mid-low frequency v channel denoise strength.
    vs_uint16_t noise_level_scaler_y3;  ///< RW; Range:[0x0, 0xFFF]; Low frequency reference denoise strength of Y channel to chroma channel.
    vs_uint16_t noise_level_scaler_u3;  ///< RW; Range:[0x0, 0xFFF]; Low frequency u channel denoise strength.
    vs_uint16_t noise_level_scaler_v3;  ///< RW; Range:[0x0, 0xFFF]; Low frequency v channel denoise strength.
    vs_uint8_t mini_motion;             ///< RW; Range:[0x0, 0xFF]; The min value of motion.

    vs_uint8_t y_radius_gain[VS_ISP_NR3D_RADIUS_GAIN_SIZE];     ///< RW; Range:[0x0, 0xFF]; Center to corner Y denoise strength gain. LSC related.
    vs_uint8_t u_radius_gain[VS_ISP_NR3D_RADIUS_GAIN_SIZE];     ///< RW; Range:[0x0, 0xFF]; Center to corner U denoise strength gain. LSC related.
    vs_uint8_t v_radius_gain[VS_ISP_NR3D_RADIUS_GAIN_SIZE];     ///< RW; Range:[0x0, 0xFF]; Center to corner V denoise strength gain. LSC related.
    vs_uint8_t x_pos_layer0[VS_ISP_NR3D_XYPOS_STRENGTH_SIZE];   ///< RW; Range:[0x0, 0xFF]; Y channel high frequency area denoising strength.
    vs_uint8_t y_pos_layer0[VS_ISP_NR3D_XYPOS_STRENGTH_SIZE];   ///< RW; Range:[0x0, 0xFF]; Y channel high frequency area denoising strength.
    vs_uint8_t x_pos_layer1[VS_ISP_NR3D_XYPOS_STRENGTH_SIZE];   ///< RW; Range:[0x0, 0xFF]; Y channel middle frequency area denoising strength.
    vs_uint8_t y_pos_layer1[VS_ISP_NR3D_XYPOS_STRENGTH_SIZE];   ///< RW; Range:[0x0, 0xFF]; Y channel middle frequency area denoising strength.
    vs_uint8_t weight_idx_y[VS_ISP_NR3D_WEIGHT_IDX_SIZE];       ///< RW; Range:[0x0, 0xFF]; Monotonically increasing. Y channel motion threshold.
    vs_uint8_t weight_idx_uv[VS_ISP_NR3D_WEIGHT_IDX_SIZE];      ///< RW; Range:[0x0, 0xFF]; Monotonically increasing. UV channel motion threshold.
    vs_uint8_t correct_ratio[VS_ISP_NR3D_CORRECT_RATIO_SIZE];   ///< RW; Range:[0x0, 0xF]; Nr3d algorithm ratio. VS839/VS839T/VS819 not support
    vs_uint8_t sigma_scale_noise_y[VS_ISP_NR3D_SIGMA_SCALE_NOISE_SIZE]; ///< RW; Range:[0x0, 0xFF]; Y channel noiselevel lut.
    vs_uint8_t sigma_scale_noise_u[VS_ISP_NR3D_SIGMA_SCALE_NOISE_SIZE]; ///< RW; Range:[0x0, 0xFF]; U channel noiselevel lut.
    vs_uint8_t sigma_scale_noise_v[VS_ISP_NR3D_SIGMA_SCALE_NOISE_SIZE]; ///< RW; Range:[0x0, 0xFF]; V channel noiselevel lut.

    vs_uint8_t stretch_th0;                                     ///< RW; Range:[0x0, 0xFF]; Stretch mode, pixels that smaller than this value will be output as stretch_out_th0.
    vs_uint8_t stretch_th1;                                     ///< RW; Range:[0x0, 0xFF]; Stretch mode, pixels that bigger than this value will be output as stretch_out_th1.
    vs_uint8_t stretch_out_th0;                                 ///< RW; Range:[0x0, 0xFF]; See stretch_th0.
    vs_uint8_t stretch_out_th1;                                 ///< RW; Range:[0x0, 0xFF]; See stretch_th1.
} vs_isp_nr3d_manual_attr_s;

/**
 * @brief Defines vs isp nr3d auto attribute
 * @details
 */
typedef struct vs_isp_nr3d_auto_attr {
    vs_uint8_t blur_alpha[VS_ISP_AUTO_ISO_STRENGTH_SIZE];               ///< RW; Range:[0x0, 0x3F]; Size of temporal filter.
    vs_uint8_t dif_presion_y[VS_ISP_AUTO_ISO_STRENGTH_SIZE];            ///< RW; Range:[0x0, 0xFF]; Y channel noise estimation precision.
    vs_uint8_t dif_presion_uv[VS_ISP_AUTO_ISO_STRENGTH_SIZE];           ///< RW; Range:[0x0, 0xFF]; UV channel noise estimation precision.
    vs_uint8_t dark_str[VS_ISP_AUTO_ISO_STRENGTH_SIZE];                 ///< RW; Range:[0x0, 0xFF]; Dark area Y channal temporal denoise strength.
    vs_uint8_t mid_str[VS_ISP_AUTO_ISO_STRENGTH_SIZE];                  ///< RW; Range:[0x0, 0xFF]; Middle-lighted area Y channal temporal denoise strength.
    vs_uint8_t light_str[VS_ISP_AUTO_ISO_STRENGTH_SIZE];                ///< RW; Range:[0x0, 0xFF]; Light area Y channal temporal denoise strength.
    vs_uint8_t uv_dark_str[VS_ISP_AUTO_ISO_STRENGTH_SIZE];              ///< RW; Range:[0x0, 0xFF]; Dark area UV channal temporal denoise strength.
    vs_uint8_t uv_mid_str[VS_ISP_AUTO_ISO_STRENGTH_SIZE];               ///< RW; Range:[0x0, 0xFF]; Middle-lighted area UV channal temporal denoise strength.
    vs_uint8_t uv_light_str[VS_ISP_AUTO_ISO_STRENGTH_SIZE];             ///< RW; Range:[0x0, 0xFF]; Light area UV channal temporal denoise strength.
    vs_uint8_t hl_noise_std_y[VS_ISP_AUTO_ISO_STRENGTH_SIZE];           ///< RW; Range:[0x01, 0x3F]; High-lighted area Y channal temporal denoise strength.
    vs_uint8_t hl_noise_std_u[VS_ISP_AUTO_ISO_STRENGTH_SIZE];           ///< RW; Range:[0x01, 0x3F]; High-lighted area U channal temporal denoise strength.
    vs_uint8_t hl_noise_std_v[VS_ISP_AUTO_ISO_STRENGTH_SIZE];           ///< RW; Range:[0x01, 0x3F]; High-lighted area V channal temporal denoise strength.

    vs_uint16_t strength0[VS_ISP_AUTO_ISO_STRENGTH_SIZE];               ///< RW; Range:[0x01, 0x0FFF]; Spatial denoise strength of high frequency noise in Y channel.
    vs_uint16_t strength1[VS_ISP_AUTO_ISO_STRENGTH_SIZE];               ///< RW; Range:[0x01, 0x0FFF]; Spatial denoise strength of middle frequency noise in Y channel.
    vs_uint16_t strength2[VS_ISP_AUTO_ISO_STRENGTH_SIZE];               ///< RW; Range:[0x01, 0x0FFF]; Spatial denoise strength of low frequency noise in Y channel.
    vs_uint8_t strength0_ym[VS_ISP_AUTO_ISO_STRENGTH_SIZE];             ///< RW; Range:[0x0, 0xFF]; Spatial high frequency denoise strength of moving area in Y channel.
    vs_uint8_t strength0_ys[VS_ISP_AUTO_ISO_STRENGTH_SIZE];             ///< RW; Range:[0x0, 0xFF]; Spatial high frequency denoise strength of static area in Y channel.
    vs_uint8_t strength1_ym[VS_ISP_AUTO_ISO_STRENGTH_SIZE];             ///< RW; Range:[0x0, 0xFF]; Spatial mid frequency denoise strength of moving area in Y channel.
    vs_uint8_t strength1_ys[VS_ISP_AUTO_ISO_STRENGTH_SIZE];             ///< RW; Range:[0x0, 0xFF]; Spatial mid frequency denoise strength of static area in Y channel.
    vs_uint8_t strength2_ym[VS_ISP_AUTO_ISO_STRENGTH_SIZE];             ///< RW; Range:[0x0, 0xFF]; Spatial low frequency denoise strength of moving area in Y channel.
    vs_uint8_t strength2_ys[VS_ISP_AUTO_ISO_STRENGTH_SIZE];             ///< RW; Range:[0x0, 0xFF]; Spatial low frequency denoise strength of static area in Y channel.

    vs_uint8_t alpha_static_layer0[VS_ISP_AUTO_ISO_STRENGTH_SIZE];      ///< RW; Range:[0x0, 0xFF]; Y channel high frequency blend ratio of static area after spatial denoise.
    vs_uint8_t alpha_motion_layer0[VS_ISP_AUTO_ISO_STRENGTH_SIZE];      ///< RW; Range:[0x0, 0xFF]; Y channel high frequency blend ratio of moving area after spatial denoise.
    vs_uint8_t alpha_static_layer1[VS_ISP_AUTO_ISO_STRENGTH_SIZE];      ///< RW; Range:[0x0, 0xFF]; Y channel mid frequency blend ratio of static area after spatial denoise.
    vs_uint8_t alpha_motion_layer1[VS_ISP_AUTO_ISO_STRENGTH_SIZE];      ///< RW; Range:[0x0, 0xFF]; Y channel mid frequency blend ratio of moving area after spatial denoise.
    vs_uint8_t alpha_static_layer2[VS_ISP_AUTO_ISO_STRENGTH_SIZE];      ///< RW; Range:[0x0, 0xFF]; Y channel low frequency blend ratio of static area after spatial denoise.
    vs_uint8_t alpha_motion_layer2[VS_ISP_AUTO_ISO_STRENGTH_SIZE];      ///< RW; Range:[0x0, 0xFF]; Y channel low frequency blend ratio of moving area after spatial denoise.

    vs_uint8_t uv_denoise_strength[VS_ISP_AUTO_ISO_STRENGTH_SIZE];      ///< RW; Range:[0x0, 0xFF]; Total UV channel spatial denoise strength.
    vs_uint16_t noise_level_scaler_y0[VS_ISP_AUTO_ISO_STRENGTH_SIZE];   ///< RW; Range:[0x0, 0xFFF]; High frequency reference denoise strength of Y channel to chroma channel.
    vs_uint16_t noise_level_scaler_u0[VS_ISP_AUTO_ISO_STRENGTH_SIZE];   ///< RW; Range:[0x0, 0xFFF]; High frequency u channel denoise strength.
    vs_uint16_t noise_level_scaler_v0[VS_ISP_AUTO_ISO_STRENGTH_SIZE];   ///< RW; Range:[0x0, 0xFFF]; High frequency v channel denoise strength.
    vs_uint16_t noise_level_scaler_y1[VS_ISP_AUTO_ISO_STRENGTH_SIZE];   ///< RW; Range:[0x0, 0xFFF]; Mid frequency reference denoise strength of Y channel to chroma channel.
    vs_uint16_t noise_level_scaler_u1[VS_ISP_AUTO_ISO_STRENGTH_SIZE];   ///< RW; Range:[0x0, 0xFFF]; Mid frequency u channel denoise strength.
    vs_uint16_t noise_level_scaler_v1[VS_ISP_AUTO_ISO_STRENGTH_SIZE];   ///< RW; Range:[0x0, 0xFFF]; Mid frequency v channel denoise strength.
    vs_uint16_t noise_level_scaler_y2[VS_ISP_AUTO_ISO_STRENGTH_SIZE];   ///< RW; Range:[0x0, 0xFFF]; Mid-low frequency reference denoise strength of Y channel to chroma channel.
    vs_uint16_t noise_level_scaler_u2[VS_ISP_AUTO_ISO_STRENGTH_SIZE];   ///< RW; Range:[0x0, 0xFFF]; Mid-low frequency u channel denoise strength.
    vs_uint16_t noise_level_scaler_v2[VS_ISP_AUTO_ISO_STRENGTH_SIZE];   ///< RW; Range:[0x0, 0xFFF]; Mid-low frequency v channel denoise strength.
    vs_uint16_t noise_level_scaler_y3[VS_ISP_AUTO_ISO_STRENGTH_SIZE];   ///< RW; Range:[0x0, 0xFFF]; Low frequency reference denoise strength of Y channel to chroma channel.
    vs_uint16_t noise_level_scaler_u3[VS_ISP_AUTO_ISO_STRENGTH_SIZE];   ///< RW; Range:[0x0, 0xFFF]; Low frequency u channel denoise strength.
    vs_uint16_t noise_level_scaler_v3[VS_ISP_AUTO_ISO_STRENGTH_SIZE];   ///< RW; Range:[0x0, 0xFFF]; Low frequency v channel denoise strength.
    vs_uint8_t mini_motion[VS_ISP_AUTO_ISO_STRENGTH_SIZE];              ///< RW; Range:[0x0, 0xFF]; The min value of motion.

    vs_uint8_t y_radius_gain[VS_ISP_AUTO_ISO_STRENGTH_SIZE][VS_ISP_NR3D_RADIUS_GAIN_SIZE];     ///< RW; Range:[0x0, 0xFF]; Center to corner Y denoise strength gain. LSC related.
    vs_uint8_t u_radius_gain[VS_ISP_AUTO_ISO_STRENGTH_SIZE][VS_ISP_NR3D_RADIUS_GAIN_SIZE];     ///< RW; Range:[0x0, 0xFF]; Center to corner U denoise strength gain. LSC related.
    vs_uint8_t v_radius_gain[VS_ISP_AUTO_ISO_STRENGTH_SIZE][VS_ISP_NR3D_RADIUS_GAIN_SIZE];     ///< RW; Range:[0x0, 0xFF]; Center to corner V denoise strength gain. LSC related.
    vs_uint8_t x_pos_layer0[VS_ISP_AUTO_ISO_STRENGTH_SIZE][VS_ISP_NR3D_XYPOS_STRENGTH_SIZE];   ///< RW; Range:[0x0, 0xFF]; Y channel high frequency area denoising strength.
    vs_uint8_t y_pos_layer0[VS_ISP_AUTO_ISO_STRENGTH_SIZE][VS_ISP_NR3D_XYPOS_STRENGTH_SIZE];   ///< RW; Range:[0x0, 0xFF]; Y channel high frequency area denoising strength.
    vs_uint8_t x_pos_layer1[VS_ISP_AUTO_ISO_STRENGTH_SIZE][VS_ISP_NR3D_XYPOS_STRENGTH_SIZE];   ///< RW; Range:[0x0, 0xFF]; Y channel middle frequency area denoising strength.
    vs_uint8_t y_pos_layer1[VS_ISP_AUTO_ISO_STRENGTH_SIZE][VS_ISP_NR3D_XYPOS_STRENGTH_SIZE];   ///< RW; Range:[0x0, 0xFF]; Y channel middle frequency area denoising strength.
    vs_uint8_t weight_idx_y[VS_ISP_AUTO_ISO_STRENGTH_SIZE][VS_ISP_NR3D_WEIGHT_IDX_SIZE];        ///< RW; Range:[0x0, 0xFF]; Monotonically increasing. Y channel motion threshold.
    vs_uint8_t weight_idx_uv[VS_ISP_AUTO_ISO_STRENGTH_SIZE][VS_ISP_NR3D_WEIGHT_IDX_SIZE];       ///< RW; Range:[0x0, 0xFF]; Monotonically increasing. UV channel motion threshold.
    vs_uint8_t correct_ratio[VS_ISP_AUTO_ISO_STRENGTH_SIZE][VS_ISP_NR3D_CORRECT_RATIO_SIZE];    ///< RW; Range:[0x0, 0xF]; Nr3d algorithm ratio. VS839/VS839T/VS819 not support
    vs_uint8_t sigma_scale_noise_y[VS_ISP_AUTO_ISO_STRENGTH_SIZE][VS_ISP_NR3D_SIGMA_SCALE_NOISE_SIZE]; ///< RW; Range:[0x0, 0xFF]; Y channel noiselevel lut.
    vs_uint8_t sigma_scale_noise_u[VS_ISP_AUTO_ISO_STRENGTH_SIZE][VS_ISP_NR3D_SIGMA_SCALE_NOISE_SIZE]; ///< RW; Range:[0x0, 0xFF]; U channel noiselevel lut.
    vs_uint8_t sigma_scale_noise_v[VS_ISP_AUTO_ISO_STRENGTH_SIZE][VS_ISP_NR3D_SIGMA_SCALE_NOISE_SIZE]; ///< RW; Range:[0x0, 0xFF]; V channel noiselevel lut.
    vs_uint8_t stretch_th0[VS_ISP_AUTO_ISO_STRENGTH_SIZE];              ///< RW; Range:[0x0, 0xFF]; Stretch mode, pixels that smaller than this value will be output as stretch_out_th0.
    vs_uint8_t stretch_th1[VS_ISP_AUTO_ISO_STRENGTH_SIZE];              ///< RW; Range:[0x0, 0xFF]; Stretch mode, pixels that bigger than this value will be output as stretch_out_th1.
    vs_uint8_t stretch_out_th0[VS_ISP_AUTO_ISO_STRENGTH_SIZE];          ///< RW; Range:[0x0, 0xFF]; See stretch_th0.
    vs_uint8_t stretch_out_th1[VS_ISP_AUTO_ISO_STRENGTH_SIZE];          ///< RW; Range:[0x0, 0xFF]; See stretch_th1.
} vs_isp_nr3d_auto_attr_s;

typedef enum vs_isp_nr3d_sigma_mode {
    E_ISP_NR3D_SIGMA_HARDWARE_ORI = 0,
    E_ISP_NR3D_SIGMA_HARDWARE_SCALER,
    E_ISP_NR3D_SIGMA_MANUAL_SET,
    E_ISP_NR3D_SIGMA_SOFT_CALC,
    E_ISP_NR3D_SIGMA_MAX
}vs_isp_nr3d_sigma_mode_e;
/**
 * @brief Defines vs isp nr3d algorithm attribute
 * @details
 */
typedef struct vs_isp_nr3d_attr {
    vs_bool_t enable;                                           ///< RW; Range:[0, 1]; Format:1.0; Enable nr3d
    vs_bool_t tnr_en;                                           ///< RW; Range:[0, 1]; Enable signal of time domain denoise.
    vs_bool_t post_nr_en;                                       ///< RW; Range:[0, 1]; Enable signal of spatial domain denoise.
    vs_bool_t y_layer_en[VS_ISP_NR3D_YLAYER_ENABLE_SIZE];       ///< RW; Range:[0, 1]; Enable signal of Y channel high/mid/low frequency denoise.
    vs_bool_t c_layer_en[VS_ISP_NR3D_CLAYER_ENABLE_SIZE];       ///< RW; Range:[0, 1]; Enable signal of chroma channel high/mid/low frequency denoise.
    vs_uint8_t step_hist;                                       ///< RW; Range:[0x01, 0x0F]; Frame size related. Assign bigger value at larger frame size. Recommend 4@3840*2160, 2@1920*1080.

    vs_uint8_t sigma_time_alpha;                                ///< RW; Range:[0x0, 0xFF]; Time filter strength. Recommend keep as 200.

    vs_uint8_t radius_idx_coef[VS_ISP_NR3D_RADIUS_IDX_SIZE];    ///< RW,Range[0x0,0xFF]; Coefficients of radius from center
    vs_uint8_t center_weight0;                                  ///< RW,Range:[0x0,0xFF]; Center Weight of high frequency filter.
    vs_uint8_t center_weight1;                                  ///< RW,Range:[0x0,0xFF]; Center Weight of mid frequency filter.
    vs_uint8_t center_weight2;                                  ///< RW,Range:[0x0,0xFF]; Center Weight of low frequency filter.

    vs_uint32_t low_thr_num;                                    ///< RW; Range:[0x0, 0xFFFFFF]; Recommend range = (H*W/pow(2, step_hist)/16)*[0, 0.1].
    vs_uint32_t high_thr_num;                                   ///< RW; Range:[0x0, 0xFFFFFF]; Recommend range = (H*W/pow(2, step_hist)/16)*[0.9, 1.0].

    vs_bool_t stretch_nr_en;                                    ///< RW; Range:[0, 1]; Enable signal of Stretch module.

    vs_bool_t pq_debug_motion_y;                                ///< RW; Range:[0, 1]; Y channel motion debug signal. 1 outputs luminance moving info.
    vs_bool_t pq_debug_motion_c;                                ///< RW; Range:[0, 1]; C channel motion debug signal. 1 outputs chroma moving info.

    vs_isp_op_type_e optype;                                    ///< RW; Range:[0, 1]; Nr3d operation mode select.
    vs_isp_nr3d_manual_attr_s manual_attr;
    vs_isp_nr3d_auto_attr_s   auto_attr;
    vs_isp_nr3d_sigma_mode_e ext_sigma_mode;                    ///< RW; Range:[0, 3]; Nr3d sigma mode select.
} vs_isp_nr3d_attr_s;

/**
 * @brief Defines vs isp post sharpen manual attribute
 * @details
 */
typedef struct vs_isp_postsharpen_manual_attr {
    vs_uint16_t sigma_range_inv_post; ///< RW; Range:[0x01, 0xFFF]; The larger, the less will the filter preserve edges. Recommend lower value at high gain.
    vs_uint8_t  edge_total_gain_post; ///< RW; Range:[0x0, 0xFF]; Total edge enhancement strength.
    vs_uint8_t  var_blend_post;       ///< RW; Range:[0x0, 0xFF]; Kernel size for calculating variance.
    vs_uint8_t  std_total_gain_post;  ///< RW; Range:[0x0, 0xFF]; Total detail enhancement strength.
    vs_uint8_t  th_neg_post;          ///< RW; Range:[0x0, 0xFF]; Undershoot control value.
    vs_uint8_t  th_pos_post;          ///< RW; Range:[0x0, 0xFF]; Overshoot control value.

    vs_uint8_t bila_alpha_post;                                     ///< RW; Range:[0x0, 0xFF]; The larger, the input pixel value blends more over edge-preserving filter result. Recommend lower value at high gain.
    vs_uint8_t gain_usr_pos_post;                                   ///< RW; Range:[0x0, 0xFF]; Pos-sharpen total strength.
    vs_uint8_t gain_usr_neg_post;                                   ///< RW; Range:[0x0, 0xFF]; Neg-sharpen total strength.
    vs_uint8_t luma_total_gain_post;                                ///< RW; Range:[0x0, 0xFF]; Total luma sharpen strength.
    vs_uint8_t var_scale_post;                                      ///< RW; Range:[0x0, 0xFF]; Variance scaler value.
    vs_uint8_t luma_array_pos_post[VS_ISP_POSTSHARPEN_ARRAY_SIZE];  ///< RW; Range:[0x0, 0xFF]; Luma - pos-sharpen strength LUT.
    vs_uint8_t luma_array_neg_post[VS_ISP_POSTSHARPEN_ARRAY_SIZE];  ///< RW; Range:[0x0, 0xFF]; Luma - neg-sharpen strength LUT.
    vs_uint16_t edge_array_post[VS_ISP_POSTSHARPEN_ARRAY_SIZE];     ///< RW; Range:[0x0, 0xFFF]; Edge enhancement strength for different edges.
    vs_uint16_t std_array_post[VS_ISP_POSTSHARPEN_ARRAY_SIZE];      ///< RW; Range:[0x0, 0xFFF]; Detail enhancement strength for different textures.
} vs_isp_postsharpen_manual_attr_s;

/**
 * @brief Defines vs isp post sharpen auto attribute
 * @details
 */
typedef struct vs_isp_postsharpen_auto_attr {
    vs_uint16_t sigma_range_inv_post[VS_ISP_AUTO_ISO_STRENGTH_SIZE]; ///< RW; Range:[0x01, 0xFFF]; The larger, the less will the filter preserve edges. Recommend lower value at high gain.
    vs_uint8_t  edge_total_gain_post[VS_ISP_AUTO_ISO_STRENGTH_SIZE]; ///< RW; Range:[0x0, 0xFF]; Total edge enhancement strength.
    vs_uint8_t  var_blend_post[VS_ISP_AUTO_ISO_STRENGTH_SIZE];       ///< RW; Range:[0x0, 0xFF]; Kernel size for calculating variance.
    vs_uint8_t  std_total_gain_post[VS_ISP_AUTO_ISO_STRENGTH_SIZE];  ///< RW; Range:[0x0, 0xFF]; Total detail enhancement strength.
    vs_uint8_t  th_neg_post[VS_ISP_AUTO_ISO_STRENGTH_SIZE];          ///< RW; Range:[0x0, 0xFF]; Undershoot control value.
    vs_uint8_t  th_pos_post[VS_ISP_AUTO_ISO_STRENGTH_SIZE];          ///< RW; Range:[0x0, 0xFF]; Overshoot control value.

    vs_uint8_t bila_alpha_post[VS_ISP_AUTO_ISO_STRENGTH_SIZE];                                      ///< RW; Range:[0x0, 0xFF]; The larger, the input pixel value blends more over edge-preserving filter result. Recommend lower value at high gain.
    vs_uint8_t gain_usr_pos_post[VS_ISP_AUTO_ISO_STRENGTH_SIZE];                                    ///< RW; Range:[0x0, 0xFF]; Pos-sharpen total strength.
    vs_uint8_t gain_usr_neg_post[VS_ISP_AUTO_ISO_STRENGTH_SIZE];                                    ///< RW; Range:[0x0, 0xFF]; Neg-sharpen total strength.
    vs_uint8_t luma_total_gain_post[VS_ISP_AUTO_ISO_STRENGTH_SIZE];                                 ///< RW; Range:[0x0, 0xFF]; Total luma sharpen strength.
    vs_uint8_t var_scale_post[VS_ISP_AUTO_ISO_STRENGTH_SIZE];                                       ///< RW; Range:[0x0, 0xFF]; Variance scaler value.
    vs_uint8_t luma_array_pos_post[VS_ISP_AUTO_ISO_STRENGTH_SIZE][VS_ISP_POSTSHARPEN_ARRAY_SIZE];   ///< RW; Range:[0x0, 0xFF]; Luma - pos-sharpen strength LUT.
    vs_uint8_t luma_array_neg_post[VS_ISP_AUTO_ISO_STRENGTH_SIZE][VS_ISP_POSTSHARPEN_ARRAY_SIZE];   ///< RW; Range:[0x0, 0xFF]; Luma - neg-sharpen strength LUT.
    vs_uint16_t edge_array_post[VS_ISP_AUTO_ISO_STRENGTH_SIZE][VS_ISP_POSTSHARPEN_ARRAY_SIZE];      ///< RW; Range:[0x0, 0xFFF]; Edge enhancement strength for different edges.
    vs_uint16_t std_array_post[VS_ISP_AUTO_ISO_STRENGTH_SIZE][VS_ISP_POSTSHARPEN_ARRAY_SIZE];       ///< RW; Range:[0x0, 0xFFF]; Detail enhancement strength for different textures.
} vs_isp_postsharpen_auto_attr_s;

/**
 * @brief Defines vs isp post_sharpen algorithm attribute
 * @details
 */
typedef struct vs_isp_postsharpen_attr {
    vs_bool_t enable;                                               ///< RW; Range:[0, 1]; Enable signal of postSharpen.
    vs_bool_t edge_gain_enable_post;                                ///< RW; Range:[0, 1]; Adaptive edge sharpen enable signal. 0 disable edge_array_pre term.
    vs_bool_t std_gain_enable_post;                                 ///< RW; Range:[0, 1]; Adaptive detail sharpen enable signal. 0 disable std_array_pre term.
    vs_bool_t luma_gain_enable_post;                                ///< RW; Range:[0, 1]; Adaptive luma sharpen enable signal. 0 disable luma_array_pos/neg term.
    vs_uint8_t gauss0_data_post[VS_ISP_POSTSHARPEN_GAUSS_SIZE][VS_ISP_POSTSHARPEN_GAUSS_SIZE]; ///< RW; Range:[0x0, 0xFF]; Coefficients of the first Gauss filter.
    vs_uint8_t gauss1_data_post[VS_ISP_POSTSHARPEN_GAUSS_SIZE][VS_ISP_POSTSHARPEN_GAUSS_SIZE]; ///< RW; Range:[0x0, 0xFF]; Coefficients of the second Gauss filter.
    vs_uint8_t gauss_in_bila_post[VS_ISP_POSTSHARPEN_BILA_SIZE][VS_ISP_POSTSHARPEN_BILA_SIZE]; ///< RW; Range:[0x0, 0xFF]; Coefficients of the third filter.
    vs_uint8_t h1_ctrl_post[VS_ISP_POSTSHARPEN_HV_CTRL_SIZE];       ///< RW; Range:[0x0, 0xF]; Horizontal filter. Sum should be 16.
    vs_uint8_t v1_ctrl_post[VS_ISP_POSTSHARPEN_HV_CTRL_SIZE];       ///< RW; Range:[0x0, 0xF]; Vertical filter. Sum should be 16.

    vs_int8_t bf_dt_weight_post;                                    ///< RW; Range:[-128, 127]; Ratio of high frequency detail with edge preserving.
    vs_int8_t bf_dt_weight2_post;                                   ///< RW; Range:[-128, 127]; Ratio of mid frequency detail with edge preserving.
    vs_int8_t dog_dt_weight_post;                                   ///< RW; Range:[-128, 127]; Ratio of mid frequencydetail without edge preserving.
    vs_uint8_t alpha_freq_post;                                     ///< RW; Range:[0x0, 0xFF]; The ratio over high frequency detail without edge preserving over mid frequency detail without edge preserving.
    vs_uint8_t alpha_detail_post;                                   ///< RW; Range:[0x0, 0xFF]; The blend ratio of different frequency detail. Smaller value will increase the ratio of BFDTWeightPre, BFDTWeight2Pre and DogDTWeightPr, otherwise AlphaFreqPre increases.
    vs_uint8_t hedge_scale_post;                                    ///< RW; Range:[0x0, 0xFF]; Horizontal edge scaler.
    vs_uint8_t vedge_scale_post;                                    ///< RW; Range:[0x0, 0xFF]; Vertical edge scaler.
    vs_uint8_t edge_alpha_post;                                     ///< RW; Range:[0x0, 0xFF]; The blend ratio of variance mixed to the edge detection.

    vs_uint8_t base_alpha1_post;                                    ///< RW; Range:[0x0, 0xFF]; Ratio of base layer. Bigger value tends to use the base layer that after the edge preserving filter, otherwise use non-edge preserving filter value.
    vs_uint8_t base_alpha2_post;                                    ///< RW; Range:[0x0, 0x3F]; Ratio of base layer. Smaller value indicates bigger weight of input value, otherwise use base_alpha1_pre result.
    vs_uint8_t detail_luma_alpha_pos_ctrl_post;                     ///< RW; Range:[0x0, 0x3F]; Ratio of sharpened luminance of pos-sharpen when use luma-LUT.
    vs_uint8_t detail_luma_alpha_neg_ctrl_post;                     ///< RW; Range:[0x0, 0x3F]; Ratio of sharpened luminance of neg-sharpen when use luma-LUT.

    vs_uint8_t min_out_ctrl_post;                                   ///< RW; Range:[0x0, 0x7F]; Min value output of Y channel.
    vs_uint8_t max_out_ctrl_post;                                   ///< RW; Range:[0x80, 0xFF]; Max value output of Y channel.
    vs_bool_t postsharpen_debug1;                                   ///< RW; Range:[0, 1]; Debug enable. 1 chooses directional edges, 0 chooses undirectional edges.
    vs_bool_t postsharpen_debug2;                                   ///< RW; Range:[0, 1]; Debug enable. 1 outputs presharpen_debug1 result.
    vs_isp_op_type_e  optype;                                       ///< RW; Range:[0, 1]; Postsharpen operation mode select.
    vs_isp_postsharpen_manual_attr_s manual_attr;
    vs_isp_postsharpen_auto_attr_s   auto_attr;
} vs_isp_postsharpen_attr_s;

/**
 * @brief Defines vs isp yuv pfc algorithm manual attribute
 * @details
 */
typedef struct vs_isp_yuv_pfc_manual_attr {
    vs_uint8_t thr_y0[VS_ISP_PFC_PLIES_SIZE];      ///< RW; Range:[0x3, 0xFF]; Highlight threshold at the center of the image.
    vs_uint8_t thr_y1[VS_ISP_PFC_PLIES_SIZE];      ///< RW; Range:[0x2, 0xFE]; Highlight threshold at near the center of the image.
    vs_uint8_t thr_y2[VS_ISP_PFC_PLIES_SIZE];      ///< RW; Range:[0x1, 0xFD]; Highlight threshold at near the edges of the image.
    vs_uint8_t thr_y3[VS_ISP_PFC_PLIES_SIZE];      ///< RW; Range:[0x0, 0xFC]; Highlight threshold at the edges of the image.
    vs_uint8_t cb_gain[VS_ISP_PFC_PLIES_SIZE];     ///< RW; Range:[0x0, 0xFF]; B channel saturation after correction.
    vs_uint8_t cr_gain[VS_ISP_PFC_PLIES_SIZE];     ///< RW; Range:[0x0, 0xFF]; R channel saturation after correction.
    vs_uint8_t pp_cb[VS_ISP_PFC_PLIES_SIZE];       ///< RW; Range:[0x0, 0x7F]; The U value of the center point of the purple fringe color.
    vs_uint8_t pp_cr[VS_ISP_PFC_PLIES_SIZE];       ///< RW; Range:[0x0, 0x7F]; The V value of the center point of the purple fringe color.
    vs_uint8_t cb_range[VS_ISP_PFC_PLIES_SIZE];    ///< RW; Range:[0x2, 0x7F]; U value range of purple fringing color.
    vs_uint8_t cr_range[VS_ISP_PFC_PLIES_SIZE];    ///< RW; Range:[0x2, 0x7F]; V value range of purple fringing color.
    vs_uint8_t cb_strength[VS_ISP_PFC_PLIES_SIZE]; ///< RW; Range:[0x0, 0xFF]; Corrected Intensity of Channel B.
    vs_uint8_t cr_strength[VS_ISP_PFC_PLIES_SIZE]; ///< RW; Range:[0x0, 0xFF]; Corrected Intensity of Channel R.

    vs_uint8_t thr_sat_hight1[VS_ISP_PFC_PLIES_SIZE];   ///< RW; Range:[0x2, 0xFF]; The threshold of high saturation.(thr_sat_low1<thr_sat_low2<thr_sat_hight1<thr_sat_hight2)
    vs_uint8_t thr_sat_hight2[VS_ISP_PFC_PLIES_SIZE];   ///< RW; Range:[0x3, 0xFF]; The threshold of the highest saturation.(thr_sat_low1<thr_sat_low2<thr_sat_hight1<thr_sat_hight2)
    vs_uint8_t thr_sat_low1[VS_ISP_PFC_PLIES_SIZE];     ///< RW; Range:[0x0, 0x3F]; The threshold of the lowest saturation.(thr_sat_low1<thr_sat_low2<thr_sat_hight1<thr_sat_hight2)
    vs_uint8_t thr_sat_low2[VS_ISP_PFC_PLIES_SIZE];     ///< RW; Range:[0x1, 0xFF]; The threshold of low saturation.(thr_sat_low1<thr_sat_low2<thr_sat_hight1<thr_sat_hight2)
    vs_uint32_t thr_gra_high[VS_ISP_PFC_PLIES_SIZE];    ///< RW; Range:[0x1, 0x3FFFF]; The upper threshold of gradient.(thr_gra_low<thr_gra_high)
    vs_uint32_t thr_gra_low[VS_ISP_PFC_PLIES_SIZE];     ///< RW; Range:[0x0, 0x3FFFF]; The lower threshold of gradient.(thr_gra_low<thr_gra_high)

    vs_uint8_t thr_ynum_high[VS_ISP_PFC_PLIES_SIZE];    ///< RW; Range:[0x1, 0x1F]; The upper threshold of the number of highlighted pixels.(thr_ynum_low<thr_ynum_high)
    vs_uint8_t thr_ynum_low[VS_ISP_PFC_PLIES_SIZE];     ///< RW; Range:[0x0, 0x1F]; The lower threshold of the number of highlighted pixels.(thr_ynum_low<thr_ynum_high)
    vs_bool_t gra_enable[VS_ISP_PFC_PLIES_SIZE];        ///< RW; Range:[0x0, 0x1]; Enable signal of edge detection. 1 indicates that edge detection is enabled.

    vs_int8_t thr_uvdiff_high[VS_ISP_PFC_PLIES_SIZE];   ///< RW; Range:[-32, 31]; The upper threshold of the difference between u and v.(thr_uvdiff_low<thr_uvdiff_high)
    vs_int8_t thr_uvdiff_low[VS_ISP_PFC_PLIES_SIZE];    ///< RW; Range:[-32, 31]; The lower threshold of the difference between u and v.(thr_uvdiff_low<thr_uvdiff_high)
    vs_uint8_t thr_u_high[VS_ISP_PFC_PLIES_SIZE];       ///< RW; Range:[0x1, 0xFF]; Upper bound of U that the purple edge being corrected.(thr_u_low<thr_u_high)
    vs_uint8_t thr_u_low[VS_ISP_PFC_PLIES_SIZE];        ///< RW; Range:[0x0, 0xFF]; Lower bound of U that the purple edge being corrected.(thr_u_low<thr_u_high)
    vs_uint8_t thr_v_high[VS_ISP_PFC_PLIES_SIZE];       ///< RW; Range:[0x1, 0xFF]; Upper bound of V that the purple edge being corrected.(thr_v_low<thr_v_high)
    vs_uint8_t thr_v_low[VS_ISP_PFC_PLIES_SIZE];        ///< RW; Range:[0x0, 0xFF]; Lower bound of V that the purple edge being corrected.(thr_v_low<thr_v_high)
    vs_uint8_t thr_u_diff[VS_ISP_PFC_PLIES_SIZE];       ///< RW; Range:[0x0, 0x3F]; The monochromatic saturation threshold of U.
    vs_uint8_t thr_v_diff[VS_ISP_PFC_PLIES_SIZE];       ///< RW; Range:[0x0, 0x3F]; The monochromatic saturation threshold of V.
} vs_isp_yuv_pfc_manual_attr_s;

/**
 * @brief Defines vs isp yuv pfc algorithm auto attribute
 * @details
 */
typedef struct vs_isp_yuv_pfc_auto_attr {
    vs_uint8_t thr_y0[VS_ISP_AUTO_ISO_STRENGTH_SIZE][VS_ISP_PFC_PLIES_SIZE];      ///< RW; Range:[0x3, 0xFF]; Highlight threshold at the center of the image.
    vs_uint8_t thr_y1[VS_ISP_AUTO_ISO_STRENGTH_SIZE][VS_ISP_PFC_PLIES_SIZE];      ///< RW; Range:[0x2, 0xFE]; Highlight threshold at near the center of the image.
    vs_uint8_t thr_y2[VS_ISP_AUTO_ISO_STRENGTH_SIZE][VS_ISP_PFC_PLIES_SIZE];      ///< RW; Range:[0x1, 0xFD]; Highlight threshold at near the edges of the image.
    vs_uint8_t thr_y3[VS_ISP_AUTO_ISO_STRENGTH_SIZE][VS_ISP_PFC_PLIES_SIZE];      ///< RW; Range:[0x0, 0xFC]; Highlight threshold at the edges of the image.
    vs_uint8_t cb_gain[VS_ISP_AUTO_ISO_STRENGTH_SIZE][VS_ISP_PFC_PLIES_SIZE];     ///< RW; Range:[0x0, 0xFF]; B channel saturation after correction.
    vs_uint8_t cr_gain[VS_ISP_AUTO_ISO_STRENGTH_SIZE][VS_ISP_PFC_PLIES_SIZE];     ///< RW; Range:[0x0, 0xFF]; R channel saturation after correction.
    vs_uint8_t pp_cb[VS_ISP_AUTO_ISO_STRENGTH_SIZE][VS_ISP_PFC_PLIES_SIZE];       ///< RW; Range:[0x0, 0x7F]; The U value of the center point of the purple fringe color.
    vs_uint8_t pp_cr[VS_ISP_AUTO_ISO_STRENGTH_SIZE][VS_ISP_PFC_PLIES_SIZE];       ///< RW; Range:[0x0, 0x7F]; The V value of the center point of the purple fringe color.
    vs_uint8_t cb_range[VS_ISP_AUTO_ISO_STRENGTH_SIZE][VS_ISP_PFC_PLIES_SIZE];    ///< RW; Range:[0x2, 0x7F]; U value range of purple fringing color.
    vs_uint8_t cr_range[VS_ISP_AUTO_ISO_STRENGTH_SIZE][VS_ISP_PFC_PLIES_SIZE];    ///< RW; Range:[0x2, 0x7F]; V value range of purple fringing color.
    vs_uint8_t cb_strength[VS_ISP_AUTO_ISO_STRENGTH_SIZE][VS_ISP_PFC_PLIES_SIZE]; ///< RW; Range:[0x0, 0xFF]; Corrected Intensity of Channel B.
    vs_uint8_t cr_strength[VS_ISP_AUTO_ISO_STRENGTH_SIZE][VS_ISP_PFC_PLIES_SIZE]; ///< RW; Range:[0x0, 0xFF]; Corrected Intensity of Channel R.

    vs_uint8_t thr_sat_hight1[VS_ISP_AUTO_ISO_STRENGTH_SIZE][VS_ISP_PFC_PLIES_SIZE];    ///< RW; Range:[0x2, 0xFF]; The threshold of high saturation.(thr_sat_low1<thr_sat_low2<thr_sat_hight1<thr_sat_hight2)
    vs_uint8_t thr_sat_hight2[VS_ISP_AUTO_ISO_STRENGTH_SIZE][VS_ISP_PFC_PLIES_SIZE];    ///< RW; Range:[0x3, 0xFF]; The threshold of the highest saturation.(thr_sat_low1<thr_sat_low2<thr_sat_hight1<thr_sat_hight2)
    vs_uint8_t thr_sat_low1[VS_ISP_AUTO_ISO_STRENGTH_SIZE][VS_ISP_PFC_PLIES_SIZE];      ///< RW; Range:[0x0, 0x3F]; The threshold of the lowest saturation.(thr_sat_low1<thr_sat_low2<thr_sat_hight1<thr_sat_hight2)
    vs_uint8_t thr_sat_low2[VS_ISP_AUTO_ISO_STRENGTH_SIZE][VS_ISP_PFC_PLIES_SIZE];      ///< RW; Range:[0x1, 0xFF]; The threshold of low saturation.(thr_sat_low1<thr_sat_low2<thr_sat_hight1<thr_sat_hight2)
    vs_uint32_t thr_gra_high[VS_ISP_AUTO_ISO_STRENGTH_SIZE][VS_ISP_PFC_PLIES_SIZE];     ///< RW; Range:[0x1, 0x3FFFF]; The upper threshold of gradient.(thr_gra_low<thr_gra_high)
    vs_uint32_t thr_gra_low[VS_ISP_AUTO_ISO_STRENGTH_SIZE][VS_ISP_PFC_PLIES_SIZE];      ///< RW; Range:[0x0, 0x3FFFF]; The lower threshold of gradient.(thr_gra_low<thr_gra_high)

    vs_uint8_t thr_ynum_high[VS_ISP_AUTO_ISO_STRENGTH_SIZE][VS_ISP_PFC_PLIES_SIZE];     ///< RW; Range:[0x1, 0x1F]; The upper threshold of the number of highlighted pixels.(thr_ynum_low<thr_ynum_high)
    vs_uint8_t thr_ynum_low[VS_ISP_AUTO_ISO_STRENGTH_SIZE][VS_ISP_PFC_PLIES_SIZE];      ///< RW; Range:[0x0, 0x1F]; The lower threshold of the number of highlighted pixels.(thr_ynum_low<thr_ynum_high)
    vs_bool_t gra_enable[VS_ISP_AUTO_ISO_STRENGTH_SIZE][VS_ISP_PFC_PLIES_SIZE];         ///< RW; Range:[0x0, 0x1]; Enable signal of edge detection. 1 indicates that edge detection is enabled.

    vs_int8_t thr_uvdiff_high[VS_ISP_AUTO_ISO_STRENGTH_SIZE][VS_ISP_PFC_PLIES_SIZE];    ///< RW; Range:[-32, 31]; The upper threshold of the difference between u and v.(thr_uvdiff_low<thr_uvdiff_high)
    vs_int8_t thr_uvdiff_low[VS_ISP_AUTO_ISO_STRENGTH_SIZE][VS_ISP_PFC_PLIES_SIZE];     ///< RW; Range:[-32, 31]; The lower threshold of the difference between u and v.(thr_uvdiff_low<thr_uvdiff_high)
    vs_uint8_t thr_u_high[VS_ISP_AUTO_ISO_STRENGTH_SIZE][VS_ISP_PFC_PLIES_SIZE];        ///< RW; Range:[0x1, 0xFF]; Upper bound of U that the purple edge being corrected.(thr_u_low<thr_u_high)
    vs_uint8_t thr_u_low[VS_ISP_AUTO_ISO_STRENGTH_SIZE][VS_ISP_PFC_PLIES_SIZE];         ///< RW; Range:[0x0, 0xFF]; Lower bound of U that the purple edge being corrected.(thr_u_low<thr_u_high)
    vs_uint8_t thr_v_high[VS_ISP_AUTO_ISO_STRENGTH_SIZE][VS_ISP_PFC_PLIES_SIZE];        ///< RW; Range:[0x1, 0xFF]; Upper bound of V that the purple edge being corrected.(thr_v_low<thr_v_high)
    vs_uint8_t thr_v_low[VS_ISP_AUTO_ISO_STRENGTH_SIZE][VS_ISP_PFC_PLIES_SIZE];         ///< RW; Range:[0x0, 0xFF]; Lower bound of V that the purple edge being corrected.(thr_v_low<thr_v_high)
    vs_uint8_t thr_u_diff[VS_ISP_AUTO_ISO_STRENGTH_SIZE][VS_ISP_PFC_PLIES_SIZE];        ///< RW; Range:[0x0, 0x3F]; The monochromatic saturation threshold of U.
    vs_uint8_t thr_v_diff[VS_ISP_AUTO_ISO_STRENGTH_SIZE][VS_ISP_PFC_PLIES_SIZE];        ///< RW; Range:[0x0, 0x3F]; The monochromatic saturation threshold of V.
} vs_isp_yuv_pfc_auto_attr_s;

/**
 * @brief Defines vs isp yuv pfc algorithm  attribute, VS819L not support
 * @details
 */
typedef struct vs_isp_yuv_pfc_attr {
    vs_bool_t enable;        ///< RW; Range:[0, 1]; Format:1.0; PFC enable signal.
    vs_isp_op_type_e optype; ///< RW; Range:[0, 1]; Format:1.0; PFC operation mode select.
    vs_isp_yuv_pfc_manual_attr_s manual_attr;
    vs_isp_yuv_pfc_auto_attr_s   auto_attr;
} vs_isp_yuv_pfc_attr_s;

/**
 * @brief Defines vs isp sa algorithm  attribute
 * @details
 */
typedef struct vs_isp_sa_attr {
    vs_bool_t enable;                           ///< RW; Range:[0, 1]; Sa enable signal.
    vs_uint16_t sa_gain[VS_ISP_SA_LUT_SIZE];    ///< RW; Range: [0x0, 0xFFF]; Format:4.8; UV gain Lut.
    vs_uint8_t sa_min;                          ///< RW; Range: [0x0, 0xFF]; Format:8.0; UV Min Value.
    vs_uint8_t sa_max;                          ///< RW; Range: [0x0, 0xFF]; Format:8.0; UV Max Value.
} vs_isp_sa_attr_s;

/**
 * @brief Defines vs isp coloring algorithm  attribute, VS819L not support
 * @details
 */
typedef struct vs_isp_coloring_attr {
    vs_bool_t enable;                                                                   ///< RW; Range:[0, 1]; Coloring enable signal.
    vs_bool_t coloring_gauss_enable;                                                    ///< RW; Range:[0, 1]; Enable signal of gauss filter before coloring.
    vs_uint8_t gauss_filter[VS_ISP_COLORING_GAUSS_SIZE][VS_ISP_COLORING_GAUSS_SIZE];    ///< RW; Range:[0, 0xFF]; Format:0.8; Gauss filter parameter.
    vs_uint32_t coloring_mapping_lut[VS_ISP_COLORING_LUT_SIZE];                         ///< RW; Range: [0x0, 0xFFFFFF]; Format:8.8.8; YUV mapping target.
    vs_uint8_t coloring_y_min;                                                          ///< RW; Range: [0x0, 0xFF]; Format:8.0; Y output min Value.
    vs_uint8_t coloring_y_max;                                                          ///< RW; Range: [0x0, 0xFF]; Format:8.0; Y output max Value.
    vs_uint8_t coloring_uv_min;                                                         ///< RW; Range: [0x0, 0xFF]; Format:8.0; UV output min Value.
    vs_uint8_t coloring_uv_max;                                                         ///< RW; Range: [0x0, 0xFF]; Format:8.0; UV output max Value.
} vs_isp_coloring_attr_s;

/**
 * @brief Defines vs isp yuv af statistic global  attribute
 * @details
 */
typedef struct vs_isp_yuv_af_global_attr {
    vs_bool_t af_enable;            ///< RW; Range:[0, 1]; AF statistic module enable.

    vs_uint16_t crop_x_start;       ///< RW; VS839/VS839T/VS819 Range:[0x2, upper limit],VS819L/VS815/VS835 Range:[0x0, upper limit]; ROI start x coordinate.
    vs_uint16_t crop_y_start;       ///< RW; VS839/VS839T/VS819 Range:[0x2, upper limit],VS819L/VS815/VS835 Range:[0x0, upper limit]; ROI start y coordinate.
    vs_uint16_t crop_width;         ///< RW; ROI width: sum of all grid width. single grid width Range:[10, 270].
    vs_uint16_t crop_height;        ///< RW; ROI height: sum of all grid height. single grid height Range:[10, 230].

    vs_uint8_t  hcnt_luma_thr;      ///< RW; Range:[0x0, 0xFF]; High-light pixel statistical thresh.
    vs_uint8_t  fv_mode;            ///< RW; Range:[0x0, 0x3]; Statistical mode, 0:all pixel fv sum, 1:each pixel fv square sum, 2:each line pixel max fv sum, 3:each line max fv square sum.
    vs_uint8_t  hcnt_shift_value;   ///< RW; Range:[0x0, 0xF]; Right-shift bits to avoid overwhelm after each block counts the number of high-lighted pixels. VS819L not support.
    vs_uint8_t  ysum_shift_value;   ///< RW; Range:[0x0, 0xF]; Right-shift bits to avoid overwhelm after each block counts the sum of all pixels' value. VS819L not support.

    vs_uint8_t row_num;             ///< RW; Range:[0x1, 0xF]; Block number in row direction.VS839/VS839T/VS819 not support (fixed with 0xF)
    vs_uint8_t col_num;             ///< RW; Range:[0x1, 0x11]; Block number in column direction.VS839/VS839T/VS819 not support (fixed with 0x11)
} vs_isp_yuv_af_global_attr_s;

/**
 * @brief Defines vs isp yuv af statistic ldg attribute
 * @details
 */
typedef struct vs_isp_af_ldg_attr {
    vs_bool_t   ldg_enable;                        ///< RW; Range:[0, 1]; LDG module enable signal.
    vs_uint8_t  ldg_x[VS_ISP_AF_LDG_X_SIZE];       ///< RW; Range:[0x0, 0xFF]; LDG x coordinates, in increase order.
    vs_uint8_t  ldg_gain[VS_ISP_AF_LDG_GAIN_SIZE]; ///< RW; Range:[0x0, 0xFF]; LDG gain values, the 2nd gain value should be the largest.
} vs_isp_af_ldg_attr_s;

/**
 * @brief Defines vs isp yuv af statistic coring attribute
 * @details
 */
typedef struct vs_isp_af_coring_attr {
    vs_bool_t   coring_enable;                          ///< RW; Range:[0, 1]; Coring module enable signal.
    vs_uint16_t coring_x[VS_ISP_AF_CORING_X_SIZE];      ///< RW; Range:[0x0, 0x3FF]; Coring x coordinates, p6thr > p5thr.
    vs_uint16_t coring_map[VS_ISP_AF_CORING_MAP_SIZE];  ///< RW; Range:[0x0, 0x3FF]; Coring y coordinates, p6gain > p5gain.
} vs_isp_af_coring_attr_s;

/**
 * @brief Defines vs isp yuv af statistic iir0 attribute
 * @details
 */
typedef struct vs_isp_yuv_af_iir0_attr {
    vs_int8_t  iir0_coef[VS_ISP_YUV_AF_FILTER_COEF_SIZE]; ///< RW; Range:[-127, 127]; Coefs of IIR0 filter.
    vs_isp_af_ldg_attr_s ldg0;
    vs_isp_af_coring_attr_s coring0;
    vs_uint8_t  fv0_shift_value;                          ///< RW; Range:[0x0, 0xF]; Right-shift bits for each block's IIR0 filter to avoid the calculated fv overwhelm. VS819L not support
    vs_bool_t iir0_enable;                                ///< RW; Range:[0, 1]; IIR0 enable signal.VS839/VS839T/VS819 not support
} vs_isp_yuv_af_iir0_attr_s;

/**
 * @brief Defines vs isp yuv af statistic fir0 attribute
 * @details
 */
typedef struct vs_isp_yuv_af_fir0_attr {
    vs_int8_t  fir0_coef[VS_ISP_YUV_AF_FILTER_COEF_SIZE]; ///< RW; Range:[-127, 127]; Coefs of FIR0 filter.
    vs_isp_af_ldg_attr_s ldg2;
    vs_isp_af_coring_attr_s coring2;
    vs_uint8_t  fv2_shift_value;                          ///< RW; Range:[0x0, 0xF]; Right-shift bits for each block's FIR0 filter to avoid the calculated fv overwhelm. VS819L not support
    vs_bool_t fir0_enable;                                ///< RW; Range:[0, 1];  FIR0 enable signal.VS839/VS839T/VS819 not support
} vs_isp_yuv_af_fir0_attr_s;

/**
 * @brief Defines vs isp yuv af statistic fir1 attribute
 * @details
 */
typedef struct vs_isp_yuv_af_fir1_attr {
    vs_int8_t  fir1_coef[VS_ISP_YUV_AF_FILTER_COEF_SIZE]; ///< RW; Range:[-127, 127]; Coefs of FIR1 filter.
    vs_isp_af_ldg_attr_s ldg3;
    vs_isp_af_coring_attr_s coring3;
    vs_uint8_t  fv3_shift_value;                          ///< RW; Range:[0x0, 0xF]; Right-shift bits for each block's FIR1 filter to avoid the calculated fv overwhelm. VS819L not support
    vs_bool_t fir1_enable;                                ///< RW; Range:[0, 1]; FIR1 enable signal.VS839/VS839T/VS819 not support
} vs_isp_yuv_af_fir1_attr_s;

/**
 * @brief Defines vs isp yuv af statistic dog attribute
 * @details
 */
typedef struct vs_isp_yuv_af_tmz_attr {
    vs_int8_t  filter_coef[VS_ISP_YUV_AF_FILTER_COEF_SIZE][VS_ISP_YUV_AF_FILTER_COEF_SIZE]; ///< RW; Range:[-127, 127]; 5*5 DOG filter coefs.
    vs_isp_af_ldg_attr_s ldg4;
    vs_isp_af_coring_attr_s coring4;
    vs_uint8_t  fv4_shift_value;                                                            ///< RW; Range:[0x0, 0xF]; Right-shift bits for each block's DOG filter to avoid the calculated fv overwhelm. VS819L not support
    vs_bool_t tmz_enable;                                                                   ///< RW; Range:[0, 1]; TMZ enable signal.  VS839/VS839T/VS819 not support
} vs_isp_yuv_af_tmz_attr_s;

/**
 * @brief Defines vs isp yuv af statistic attribute
 * @details
 */
typedef struct vs_isp_yuv_af_attr {
    vs_isp_yuv_af_global_attr_s global_attr;
    vs_isp_yuv_af_tmz_attr_s  tmz_attr;
    vs_isp_yuv_af_iir0_attr_s  iir0_attr;
    vs_isp_yuv_af_fir0_attr_s fir0_attr;
    vs_isp_yuv_af_fir1_attr_s fir1_attr;
} vs_isp_yuv_af_attr_s;

typedef struct vs_isp_yuv_af_statistics_attr {
    vs_uint32_t fv_iir0[VS_ISP_YUV_AF_STATISTICS_ROWS_SIZE][VS_ISP_YUV_AF_STATISTICS_COLS_SIZE];  ///< R; Range:[0x0, 0xFFFFFF]; IIR0 filter statistics, fixed 15 rows and 17 cols to detect horizontal gradient.
    vs_uint32_t fv_fir0[VS_ISP_YUV_AF_STATISTICS_ROWS_SIZE][VS_ISP_YUV_AF_STATISTICS_COLS_SIZE];  ///< R; Range:[0x0, 0xFFFFFF]; FIR0 filter statistics, detect vertical gradient.
    vs_uint32_t fv_fir1[VS_ISP_YUV_AF_STATISTICS_ROWS_SIZE][VS_ISP_YUV_AF_STATISTICS_COLS_SIZE];  ///< R; Range:[0x0, 0xFFFFFF]; FIR1 filter statistics, detect vertical gradient.
    vs_uint32_t fv_tmz[VS_ISP_YUV_AF_STATISTICS_ROWS_SIZE][VS_ISP_YUV_AF_STATISTICS_COLS_SIZE];   ///< R; Range:[0x0, 0xFFFFFF]; DOG filter statistics, detect vertical gradient.
    vs_uint16_t hcnt[VS_ISP_YUV_AF_STATISTICS_ROWS_SIZE][VS_ISP_YUV_AF_STATISTICS_COLS_SIZE];     ///< R; Range:[0x0, 0xFFFF]; Over-threshold number of high-lighted pixels.
    vs_uint16_t luma_sum[VS_ISP_YUV_AF_STATISTICS_ROWS_SIZE][VS_ISP_YUV_AF_STATISTICS_COLS_SIZE]; ///< R; Range:[0x0, 0xFFFF]; Sum of luminance.
} vs_isp_yuv_af_statistics_attr_s;


/**
 * @brief Defines the type of the sync edge type.
 * @details
 */
typedef enum vs_isp_sync_edge_type {
    E_ISP_EDGE_TYPE_POSE = 0,    ///< use the rising edge to trigger signal
    E_ISP_EDGE_TYPE_NEGE,        ///< use the falling edge to trigger signal
    E_ISP_EDGE_TYPE_MAX
} vs_isp_sync_edge_type_e;

/**
 * @brief Defines vs isp slave device attribute
 * @details
 */
typedef struct vs_isp_slave_dev_attr {
    vs_bool_t enable;                 ///< RW; Range:[0x0, 0x1]; Enable output xvs/xhs signal
    vs_isp_sync_edge_type_e edge;     ///< RW; Range:[0x0, 0x1]; Edge type
    vs_uint16_t xvs_pulse_width;      ///< RW; Range:[0x0, 0xFFFF]; Xvs signal pulse width   (unit: sensor_clk cycle)
    vs_uint16_t vmax_lines;           ///< RW; Range:[0x0, 0xFFFF]; Ouput xhs signal nums in one xvs period (unit: xhs_period)
    vs_uint16_t xhs_delay;            ///< RW; Range:[0x0, 0xFFFF]; Xhs delay from xvs signal   (unit: sensor_clk cycle)
    vs_uint16_t xhs_pulse_width;      ///< RW; Range:[0x0, 0xFFFF]; Xhs signal pulse width      (unit: sensor_clk cycle)
    vs_uint16_t xhs_period;           ///< RW; Range:[0x0, 0xFFFF]; Clk cycle in one xhs period (unit: sensor_clk cycle)
} vs_isp_slave_dev_attr_s;

typedef enum vs_isp_stitch_mode {
    E_ISP_STITCH_MODE_NORMAL = 0,   ///< use paramters respectively
    E_ISP_STITCH_MODE_SYNC,         ///< use main pipe paramters
    E_ISP_STITCH_MODE_MAX
} vs_isp_stitch_mode_e;

typedef struct vs_isp_stitch_group_attr {
    vs_bool_t enable;                                  ///< RW; Range:[0x0, 0x1]; Enable stitch group.
    vs_isp_stitch_mode_e mode;                         ///< RW; Range:[0x0, 0x1]; Stitch mode.
    vs_uint32_t max_gap_time;                          ///< RW; Range:[0x0, 0xFFFFFFFF]; Reserved.
    vs_uint8_t pipe_num;                               ///< RW; Range:[0x2, 0x4]; Stitch group pipe nums, must be 0x2 or 0x4.
    vs_uint32_t pipe_id[VS_ISP_STITCH_PIPE_NUM_MAX];   ///< RW; Range:[0x0, ISP_MAX_PIPE_NUM); Pipe id in group.
} vs_isp_stitch_group_attr_s;

typedef struct vs_isp_ae_stitch_statistics {
    vs_uint32_t fullhist[VS_ISP_STITCH_PIPE_NUM_MAX][VS_ISP_FULL_HISTOGRAM_SIZE];                     ///< R; Range:[0x0, 0xFFFFFFFF]; All region histogram statistics.
    vs_uint32_t fullhist_sum[VS_ISP_STITCH_PIPE_NUM_MAX];                                             ///< R; Range:[0x0, 0xFFFFFFFF]; The number of partitions in the histogram of the entire region.
    vs_isp_ae_hist5_statistics_s hist5[VS_ISP_STITCH_PIPE_NUM_MAX][VS_ISP_METERING_ZONE_NUM_MAX];
    vs_uint32_t hist5_zones_num[VS_ISP_STITCH_PIPE_NUM_MAX];                                          ///< R; Range:[0x0, 0xFFFFFFFF]; The number of partitions in the histogram of regional part.
} vs_isp_ae_stitch_statistics_s;

typedef struct vs_isp_awb_stitch_statistics {
    vs_isp_awb_zone_s awb_zones[VS_ISP_STITCH_PIPE_NUM_MAX][VS_ISP_METERING_ZONE_NUM_MAX];
    vs_uint32_t zones_num[VS_ISP_STITCH_PIPE_NUM_MAX];                                                ///< R; Range:[0x0, 1089]; The number of white balance statistical regions.
} vs_isp_awb_stitch_statistics_s;

/**
 * @brief Defines vs smart info attr
 * @details
 */
typedef struct vs_isp_smart_roi
{
    vs_bool_t enable;           ///< RW; Range:[0, 1]; Enable smart roi.
    vs_bool_t available;        ///< RW; Range:[0, 1]; Available or not.
    vs_uint8_t luma;            ///< RW; Range:[0, 255]; Overall brightness information.
    vs_rect_s roi;
} vs_isp_smart_roi_s;

typedef struct vs_isp_smart_info
{
    vs_isp_smart_roi_s smart_roi[VS_ISP_SMART_CLASS_MAX];
} vs_isp_smart_info_s;

typedef struct vs_isp_smart_expo_attr
{
    vs_bool_t enable;                    ///< RW; Range: [0, 1]; Enable signal of smart exposure.
    vs_bool_t ir_enable;                 ///< RW; Range: [0, 1]; Switch signal of ir mode.
    vs_isp_op_type_e  optype;            ///< RW; Range: [0, 1]; Switch signal of auto or manunal.
    vs_uint8_t target_point;             ///< RW; Range: [1, 0xFF]; Target of smart region luma
    vs_uint16_t expo_coeff;              ///< RW; Range: [1, 0xFFFF]; Manual exposure coeff.
    vs_uint16_t expo_coeff_max;          ///< RW; Range: [1, 0xFFFF]; Maximum value of  Exposure coeff.
    vs_uint16_t expo_coeff_min;          ///< RW; Range: [1, 0xFFFF]; Minimum value of  Exposure coeff.
    vs_uint8_t smart_interval;           ///< RW; Range: [1, 0xFF]; Smart AE interval.
    vs_uint8_t pi_coeff;                 ///< RW; Range: [1, 0xFF]; Smart AE speed.
} vs_isp_smart_expo_attr_s;

/**
 * @brief Defines the isp algorithm lib attribute.
 * @details
 */
typedef struct vs_isp_alg_lib {
    vs_int32_t id;                                                      ///<  RW; Range:[0x0, 0xF];
    vs_char_t name[VS_ALG_LIB_NAME_MAX_SIZE];
} vs_isp_alg_lib_s;

typedef struct vs_isp_bind_attr {
    vs_isp_alg_lib_s ae_lib;
    vs_isp_alg_lib_s awb_lib;
}vs_isp_bind_attr_s;

typedef struct vs_isp_ae_piris_result {
    vs_bool_t   valid;                                                  ///<  RW; Range:[0x0, 0x1];
    vs_int32_t  position;                                               ///<  RW; Range:[-0x7FFFFFFF, 0x80000000];
    vs_uint32_t gain;                                                   ///<  RW; Range:[0x0, 0xFFFFFFFF];
}vs_isp_ae_piris_result_s;

typedef enum vs_isp_wdr_fs_mode {
    E_ISP_WDR_FS_MODE_NORMAL = 0x0,
    E_ISP_WDR_FS_MODE_LONG_FRAME,
    E_ISP_WDR_FS_MODE_AUTO_LONG_FRAME,
    E_ISP_WDR_FS_MODE_MAX
} vs_isp_wdr_fs_mode_e;

typedef struct vs_isp_ae_result {
    vs_uint32_t shut_time[VS_ISP_AE_EXPOSURE_NUM_MAX]; ///<  RW; Range:[0x0, 0xFFFFFFFF];
    vs_uint32_t isp_dgain;                             ///<  RW; Range:[0x0, 0xFFFFFFFF];
    vs_uint32_t again[VS_ISP_AE_EXPOSURE_NUM_MAX];     ///<  RW; Range:[0x0, 0xFFFFFFFF];
    vs_uint32_t dgain[VS_ISP_AE_EXPOSURE_NUM_MAX];     ///<  RW; Range:[0x0, 0xFFFFFFFF];
    vs_uint32_t total_gain;                            ///<  RW; Range:[0x0, 0xFFFFFFFF];
    vs_uint8_t  run_interval;                          ///<  RW; Range:[0x0, 0xFFFFFFFF];
    vs_isp_ae_piris_result_s piris_result;
    vs_isp_wdr_fs_mode_e wdr_fs_mode;
    vs_uint32_t sensor_h_max;                          ///<  RW; Range:[0x0, 0xFFFFFFFF];
    vs_uint32_t sensor_v_max;                          ///<  RW; Range:[0x0, 0xFFFFFFFF];
    vs_isp_ae_state_e  ae_state;
    vs_bool_t statistics_cfg_update;                   ///<  RW; Range:[0x0, 0x1];
    vs_isp_ae_statistics_cfg_s ae_cfg;
} vs_isp_ae_result_s;

typedef enum vs_isp_awb_light_src {
    E_ISP_AWB_LIGHT_SRC_UNKNOWN = 0,
    E_ISP_AWB_LIGHT_SRC_A,
    E_ISP_AWB_LIGHT_SRC_D40,
    E_ISP_AWB_LIGHT_SRC_D50,
    E_ISP_AWB_LIGHT_SRC_MAX
}vs_isp_awb_light_src_e;

typedef struct vs_isp_awb_result {
    vs_uint32_t awb_g_even_gain;                        ///<  RW; Range:[0x0, 0xFFFFFFFF];
    vs_uint32_t awb_g_odd_gain;                         ///<  RW; Range:[0x0, 0xFFFFFFFF];
    vs_uint32_t awb_b_gain;                             ///<  RW; Range:[0x0, 0xFFFFFFFF];
    vs_uint32_t awb_r_gain;                             ///<  RW; Range:[0x0, 0xFFFFFFFF];
    vs_uint32_t color_temp;                             ///<  RW; Range:[0x0, 0xFFFFFFFF];
    vs_uint16_t color_matrix[VS_ISP_COLOR_MATRIX_SIZE]; ///<  RW; Range:[0x0, 0xFFFF];
    vs_uint8_t  saturation;                             ///<  RW; Range:[0x0, 0xFF];
    vs_uint32_t warming[VS_ISP_AWB_WARMING_LS_SIZE];
    vs_isp_awb_light_src_e light_src;
    vs_isp_awb_state_e state;
    vs_bool_t statistics_cfg_update;                    ///<  RW; Range:[0x0, 0x1];
    vs_isp_awb_statistics_cfg_s  awb_cfg;
 } vs_isp_awb_result_s;

typedef enum vs_isp_aelib_ctrl_cmd {
    E_ISP_AE_CMD_AE_ATTR = 0,
    E_ISP_AE_CMD_STATISTICS_CFG,
    E_ISP_AE_CMD_WDR_EXP_ATTR,
    E_ISP_AE_CMD_SMART_EXPO_ATTR,
    E_ISP_AE_CMD_INIT_EXPOSUER_ATTR,
    E_ISP_AE_CMD_WDR_MODE,
    E_ISP_AE_CMD_SENSOR_INFO,
    E_ISP_AE_CMD_DRA_CONTRAST,
    E_ISP_AE_CMD_BASE_FPS,
    E_ISP_AE_CMD_SENSOR_ACTIVE_ROI,
    E_ISP_AE_CMD_MAX
}vs_isp_aelib_ctrl_cmd_e;

typedef struct vs_isp_stitch_attr {
    vs_bool_t enable;                               ///< RW; Range[0,1]; Enable stitch.
    vs_bool_t is_main_pipe;                         ///< RW; Range[0,1]; Is main pipe or not.
    vs_uint8_t pipe_num;                            ///< RW; Range:[0, VS_ISP_STITCH_PIPE_NUM_MAX); Bind pipe num.
    vs_int32_t pipe_id[VS_ISP_STITCH_PIPE_NUM_MAX]; ///< RW; Range:[0, ISP_MAX_PIPE_NUM); Bind pipe id.
} vs_isp_stitch_attr_s;

typedef struct vs_isp_aelib_attr {
    vs_int32_t pipe_id;      ///<   RW; Range:[0, ISP_MAX_PIPE_NUM); Pipe id.
    vs_wdr_mode_e wdr_mode;  ///<   RW; Range:[0x0, 0x4]; Wdr mode of Sensor.
    vs_uint16_t black_level; ///<   RW; Range:[0, 0xFFFF]; Blcak level value.
    vs_float_t initial_fps;  ///<   RW; Range:[1, 240]; Initial frame rate.
    vs_isp_bayer_format_e bayer_format; ///<   RW; Range:[0x0, 0x3];Bayer format of Sensor.
    vs_isp_stitch_attr_s stitch_attr;
}vs_isp_aelib_attr_s;

typedef struct vs_isp_aelib_param {
    vs_uint32_t frame_cnt;                   ///<   frame number ; Range:[0, 0xFFFFFFFF];
    vs_isp_smart_info_s *p_smart_info;
    vs_isp_ae_statistics_s *p_ae_statistics;
    vs_isp_ae_stitch_statistics_s *p_ae_stitch_statistics;
}vs_isp_aelib_param_s;

typedef struct vs_isp_aelib_ops {
    vs_int32_t (*init)(vs_int32_t lib_id, const vs_isp_aelib_attr_s *p_ae_param);
    vs_int32_t (*proc)(vs_int32_t lib_id, const vs_isp_aelib_param_s *p_ae_params, vs_isp_ae_result_s *p_ae_result,vs_void_t *p_value, vs_uint32_t size);
    vs_int32_t (*ctrl)(vs_int32_t lib_id, vs_uint32_t cmd, vs_void_t *p_value, vs_uint32_t size);
    vs_int32_t (*deinit)(vs_int32_t lib_id);
}vs_isp_aelib_ops_s;

typedef struct vs_isp_awblib_attr {
    vs_int32_t pipe_id;             ///< RW; Range:[0, ISP_MAX_PIPE_NUM); Pipe id.
    vs_wdr_mode_e wdr_mode;         ///< RW; Range:[0x0, 0x4]; Wdr mode of Sensor
    vs_uint8_t statistics_zone_row; ///< RW; Range:[0, 33); Awb statistics zone row number.
    vs_uint8_t statistics_zone_col; ///< RW; Range:[0, 33); Awb statistics zone column number.
    vs_uint16_t img_width;          ///< RW; Range:[64, 8192]; Awb statistic image width.
    vs_uint16_t img_height;         ///< RW; Range:[64, 8192]; Awb statistic image height.
    vs_isp_stitch_attr_s stitch_attr;
} vs_isp_awblib_attr_s;

typedef enum vs_isp_awblib_ctrl_cmd {
    E_ISP_AWB_CMD_AWB_ATTR = 0,
    E_ISP_AWB_CMD_STATISTICS_CFG,
    E_ISP_AWB_CMD_TOTAL_GAIN,
    E_ISP_AWB_CMD_EXPOSURE,
    E_ISP_AWB_CMD_DRA_CONTRAST,
    E_ISP_AWB_CMD_OTP_ATTR,
    E_ISP_AWB_CMD_MAX
}vs_isp_awblib_ctrl_cmd_e;

typedef struct vs_isp_awblib_param {
    vs_uint32_t frame_cnt;                       ///<   RW; Range:[0, 0xFFFFFFFF]; Frame number.
    vs_bool_t is_wb_gain_in_sensor;              ///<   RW; Range:[0, 1]; Indicate wb gain in sensor or not.
    vs_isp_awb_statistics_s *p_awb_statistics;
    vs_isp_awb_stitch_statistics_s *p_awb_stitch_statistics;
    vs_isp_awb_switch_e awb_switch;             ///<   RW; Range:[0x0, 0x1]; Position for obtaining white balance statistics.
}vs_isp_awblib_param_s;

typedef struct vs_isp_awblib_ops {
    vs_int32_t (*init)(vs_int32_t lib_id, const vs_isp_awblib_attr_s *p_awblib_attr, vs_isp_awb_result_s *p_awb_result);
    vs_int32_t (*proc)(vs_int32_t lib_id, const vs_isp_awblib_param_s *p_awb_param, vs_isp_awb_result_s *p_awb_result, vs_void_t *p_value, vs_uint32_t size);
    vs_int32_t (*ctrl)(vs_int32_t lib_id, vs_uint32_t cmd, vs_void_t *p_value, vs_uint32_t size);
    vs_int32_t (*deinit)(vs_int32_t lib_id);
}vs_isp_awblib_ops_s;

typedef struct vs_isp_focus_ldg_attr {
    vs_bool_t  enable;      ///< RW; Range: [0, 1]; Enable ldg.
    vs_uint8_t thr_low1;    ///< RW; Range: [0,255]; The first threshold of LDG abscissa.
    vs_uint8_t thr_low2;    ///< RW; Range: [0,255]; The second threshold of LDG abscissa.
    vs_uint8_t gain_low;    ///< RW; Range: [0,255]; The first threshold corresponds to gain value.
    vs_uint8_t thr_high1;   ///< RW; Range: [0,255]; The third threshold of LDG abscissa.
    vs_uint8_t thr_high2;   ///< RW; Range: [0,255]; The fourth threshold of LDG abscissa.
    vs_uint8_t gain_high;   ///< RW; Range: [0,255]; The fourth threshold corresponds to gain value.
}vs_isp_focus_ldg_attr_s;

typedef struct vs_isp_focus_coring_attr {
    vs_bool_t enable;       ///< RW; Range: [0,1]; Enable focus coring.
    vs_uint16_t thr_low;    ///< RW; Range: [0,32767]; The first threshold of coring abscissa.
    vs_uint16_t thr_high;   ///< RW; Range: [0,32767]; The second threshold of coring abscissa.
    vs_uint16_t limit;      ///< RW; Range: [0,32767]; If the filter result is greater than the second threshold, the filter result will be set to this value.
}vs_isp_focus_coring_attr_s;

typedef enum vs_isp_focus_cell_idx{
    E_CHN_IDX_CELL_LEFT_UP = 0,
    E_CHN_IDX_CELL_RIGHT_UP,
    E_CHN_IDX_CELL_LEFT_DOWN,
    E_CHN_IDX_CELL_RIGHT_DOWN,
    E_CHN_IDX_CELL_MAX,
}vs_isp_focus_cell_idx_e;

typedef enum vs_isp_focus_mode {
    E_FOCUS_MODE_LINEAR = 0,
    E_FOCUS_MODE_QUADRATIC,
    E_FOCUS_MODE_CUBIC,
    E_FOCUS_MODE_QUARTIC,
    E_FOCUS_MODE_MAX,
}vs_isp_focus_mode_e;

typedef struct vs_isp_focus_iir_attr {
    vs_bool_t enable;                                ///< RW; Range: [0, 1]; Enable focus_iir.
    vs_int16_t coef0[VS_ISP_FOCUS_FILTER_COEF_NUM];  ///< RW; Range: [-511,511]; First stage filter coefficients of IIR filters.
    vs_int16_t coef1[VS_ISP_FOCUS_FILTER_COEF_NUM];  ///< RW; Range: [-511,511]; Second stage filter coefficients of IIR filters.
    vs_int16_t coef2[VS_ISP_FOCUS_FILTER_COEF_NUM];  ///< RW; Range: [-511,511]; Third stage filter coefficients of IIR filters.
    vs_uint8_t shift[VS_ISP_FOCUS_FILTER_SHIFT_NUM]; ///< RW; Range: [0,15]; Right shift values.
    vs_uint8_t fv_shift;                             ///< RW; Range: [0,15]; Right shift value of each pixel filtering result.
    vs_isp_focus_ldg_attr_s ldg;
    vs_isp_focus_coring_attr_s coring;
    vs_isp_focus_mode_e fv_mode;                     ///< RW; Range: [0, 1]; Focus mode value.
} vs_isp_focus_iir_attr_s;

typedef struct vs_isp_focus_fir_attr {
    vs_bool_t enable;      ///< RW; Range: [0, 1]; Enable focus_fir.
    vs_uint8_t fv_shift;   ///< RW; Range: [0,15]; Right shift value of each pixel filtering result.
    vs_uint8_t cutoff_thr; ///< RW; Range: [0,255]; The truncation threshold after a single direction is filtered.
    vs_isp_focus_mode_e fv_mode; ///< RW; Range: [0, 3]; Focus mode value.
    vs_int16_t dpc_thr;    ///< RW; Range: [0,255]; Enhancement of effective filter filtering information.
}vs_isp_focus_fir_attr_s;

typedef struct vs_isp_focus_var_attr {
    vs_uint8_t var_thr;   ///< RW; Range: [0,255]; Small value truncation of var statistical information.
    vs_uint8_t var_shift; ///< RW; Range: [0,15]; Single pixel right shift value of variance statistical information.
}vs_isp_focus_var_attr_s;

typedef struct vs_isp_focus_attr {
    vs_bool_t enable;                 ///< RW; Range: [0, 1]; Enable focus.
    vs_isp_focus_cell_idx_e cell_idx; ///< RW; Range: [0, 3]; Focus cell index value.
    vs_uint16_t blc;                  ///< RW; Range: [0, 0xFFF]; Black level correction value.
    vs_isp_focus_iir_attr_s iir0_attr;
    vs_isp_focus_iir_attr_s iir1_attr;
    vs_isp_focus_fir_attr_s fir_attr;
    vs_isp_focus_var_attr_s var_attr;
}vs_isp_focus_attr_s;

typedef struct vs_isp_focus_value {
    vs_uint32_t fv_fir [VS_ISP_FOCUS_VALUE_ROWS_SIZE][VS_ISP_FOCUS_VALUE_COLS_SIZE];    ///< R; Range: [0, 0xFFFFFFFF]; FIR statistics.
    vs_uint32_t fv_var [VS_ISP_FOCUS_VALUE_ROWS_SIZE][VS_ISP_FOCUS_VALUE_COLS_SIZE];    ///< R; Range: [0, 0xFFFFFFFF]; Variance statistics.
    vs_uint32_t fv_iir0[VS_ISP_FOCUS_VALUE_ROWS_SIZE][VS_ISP_FOCUS_VALUE_COLS_SIZE];    ///< R; Range: [0, 0xFFFFFFFF]; IIR0 statistics.
    vs_uint32_t fv_iir1[VS_ISP_FOCUS_VALUE_ROWS_SIZE][VS_ISP_FOCUS_VALUE_COLS_SIZE];    ///< R; Range: [0, 0xFFFFFFFF]; IIR1 statistics.
}vs_isp_focus_value_s;

typedef struct vs_isp_color_shading_cfg {
    vs_bool_t    enable;       /* RW; Range:[0,1];Format:1.0; Color Shading Enable */
    vs_uint16_t  strength;     /* RW; Range:[0,256];Format:9.0; Correction strength for luma shading */
    vs_uint16_t  run_interval; /* RW; Range:[1,255];Format:8.0; Run Interval */
    vs_bool_t    lock_enable;  /* RW; Range:[0,1];Format:1.0; Lock Gain Lut Enable */
} vs_isp_color_shading_cfg_s;

typedef struct vs_isp_color_shading_calib_lut {
    vs_int32_t rg_map_0[VS_ISP_METERING_ZONE_NUM_MAX]; /* R; Range:[0,8388607]; Format:3.20; RG map from calibration */
    vs_int32_t bg_map_0[VS_ISP_METERING_ZONE_NUM_MAX]; /* R; Range:[0,8388607]; Format:3.20; BG map from calibration */
    vs_int32_t rg_map_1[VS_ISP_METERING_ZONE_NUM_MAX]; /* R; Range:[0,8388607]; Format:3.20; RG map from calibration */
    vs_int32_t bg_map_1[VS_ISP_METERING_ZONE_NUM_MAX]; /* R; Range:[0,8388607]; Format:3.20; BG map from calibration */
    vs_uint16_t g_high[VS_ISP_METERING_ZONE_NUM_MAX];  /* R; Range:[0,16383]; Format:4.10; High G map from calibration */
    vs_uint16_t g_low[VS_ISP_METERING_ZONE_NUM_MAX];   /* R; Range:[0,16383]; Format:4.10; Low G map from calibration */
} vs_isp_color_shading_calib_lut_s;

typedef struct vs_isp_color_shading_calib_attr {
    vs_int16_t light_type[VS_ISP_COLOR_SHADING_LIGHT_NUM]; /* R; Range:[-32,31]; Format:6.0; Light type from calibration */
    vs_int32_t g_param_0; /* R; Range:[-2147483648,2147483647]; Format:32.0; G param from calibration */
    vs_int32_t g_param_1; /* R; Range:[-2147483648,2147483647]; Format:32.0; G param from calibration */
    vs_int32_t r_param_0; /* R; Range:[-2147483648,2147483647]; Format:32.0; R param from calibration */
    vs_int32_t r_param_1; /* R; Range:[-2147483648,2147483647]; Format:32.0; R param from calibration */
    vs_int32_t b_param_0; /* R; Range:[-2147483648,2147483647]; Format:32.0; B param from calibration */
    vs_int32_t b_param_1; /* R; Range:[-2147483648,2147483647]; Format:32.0; B param from calibration */
} vs_isp_color_shading_calib_attr_s;

typedef struct vs_isp_color_shading_attr {
    vs_isp_color_shading_cfg_s        cfg;
    vs_isp_color_shading_calib_attr_s calib_attr;
    vs_isp_color_shading_calib_lut_s  calib_lut;
} vs_isp_color_shading_attr_s;

typedef struct vs_isp_otp_awb_rggb_attr {
    vs_uint8_t r;      /* RW; Range:[0,255];Format:8.0; R data */
    vs_uint8_t gr;     /* RW; Range:[0,255];Format:8.0; Gr data */
    vs_uint8_t gb;     /* RW; Range:[0,255];Format:8.0; Gb data */
    vs_uint8_t b;      /* RW; Range:[0,255];Format:8.0; B data */
    vs_uint16_t rg;    /* RW; Range:[0,65535];Format:6.10; R/G data */
    vs_uint16_t bg;    /* RW; Range:[0,65535];Format:6.10; B/G data */
    vs_uint16_t gbgr;  /* RW; Range:[0,65535];Format:6.10; Gb/Gr data */
} vs_isp_otp_awb_rggb_attr_s;

typedef struct vs_isp_otp_awb_attr {
    vs_bool_t    enable;           /* RW; Range:[0,1];Format:1.0; AWB OTP Correction Enable */
    vs_bool_t    stats_enable;     /* RW; Range:[0,1];Format:1.0;Stats Correction Enable */
    vs_uint16_t    r_strength;     /* RW; Range:[0,256];Format:0.8; AWB OTP Correction R Gain Strength */
    vs_uint16_t    b_strength;     /* RW; Range:[0,256];Format:0.8; AWB OTP Correction B Gain Strength */
    vs_isp_otp_awb_rggb_attr_s otp_data;
    vs_isp_otp_awb_rggb_attr_s golden_data;
} vs_isp_otp_awb_attr_s;

typedef struct vs_isp_otp_lsc_lut {
    vs_bool_t enable;                                 /* RW; Range:[0,1];Format:1.0; LSC OTP Enable */
    vs_uint16_t r_gain[VS_ISP_METERING_ZONE_NUM_MAX];  /* RW; Range:[0,65535]; Format:16.0; LSC OTP Rgain */
    vs_uint16_t g_gain[VS_ISP_METERING_ZONE_NUM_MAX];  /* RW; Range:[0,65535]; Format:16.0; LSC OTP Ggain */
    vs_uint16_t b_gain[VS_ISP_METERING_ZONE_NUM_MAX];  /* RW; Range:[0,65535]; Format:16.0; LSC OTP Bgain */
} vs_isp_otp_lsc_lut_s;

typedef struct vs_isp_otp_attr {
    vs_isp_otp_awb_attr_s        otp_awb_attr;
    vs_isp_otp_lsc_lut_s         otp_lsc_lut;
} vs_isp_otp_attr_s;

typedef struct vs_isp_af_raw_statistics_data {
	vs_uint16_t mantissa;          ///<  R; Range:[0x0, 0xFFFF];
	vs_uint8_t exponent;           ///<  R; Range:[0x0, 0xFF];
}vs_isp_af_raw_statistics_data_s;

typedef struct vs_isp_af_raw_statistics_zone {
	vs_isp_af_raw_statistics_data_s i_quadratic;
	vs_isp_af_raw_statistics_data_s i_cubic;
	vs_isp_af_raw_statistics_data_s e_cubic;
}vs_isp_af_raw_statistics_zone_s;

typedef struct vs_isp_af_raw_statistics {
	vs_isp_af_raw_statistics_zone_s statistics_value[VS_ISP_METERING_ZONE_NUM_MAX];
}vs_isp_af_raw_statistics_s;

#ifdef __cplusplus
}
#endif
#endif /*!<  _VS_ISP_DEFINES_H_ */
