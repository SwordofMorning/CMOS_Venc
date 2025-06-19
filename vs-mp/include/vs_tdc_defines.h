/**
* @file vs_tdc_defines.h
* @brief Declaration of traffic digital control enumeration and struct.
* @details Declaration of traffic digital control enumeration and struct.
* @author Visinex software group
* @date 2022-01-27
* @version v0.01
* @copyright (C) 2022, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/
#ifndef __VS_TDC_DEFINES_H__
#define __VS_TDC_DEFINES_H__

#include "vs_type.h"
#include "vs_errno.h"
#include "vs_capability.h"
#include "vs_comm.h"

#ifdef __cplusplus
extern "C"{
#endif

///< parameter is invalid
#define VS_ERR_TDC_INVALID_PARAM      VS_ERR_CODE(E_MOD_ID_TDC, E_ERR_INVALID_PARAM)
///< invalid device ID */
#define VS_ERR_TDC_INVALID_ID         VS_ERR_CODE(E_MOD_ID_TDC, E_ERR_INVALID_DEVID)
///< using a NULL pointer
#define VS_ERR_TDC_NULL_PTR           VS_ERR_CODE(E_MOD_ID_TDC, E_ERR_NULL_PTR)
///< operation is not supported
#define VS_ERR_TDC_NOT_SUPPORT        VS_ERR_CODE(E_MOD_ID_TDC, E_ERR_NOT_SUPPORT)
///< system is not ready
#define VS_ERR_TDC_NOT_INITIALIZED    VS_ERR_CODE(E_MOD_ID_TDC, E_ERR_NOT_INITIALIZED)
///< operation is not permitted
#define VS_ERR_TDC_NOT_PERM           VS_ERR_CODE(E_MOD_ID_TDC, E_ERR_NOT_PERM)
///< no memory for allocated
#define VS_ERR_TDC_NO_MEMORY          VS_ERR_CODE(E_MOD_ID_TDC, E_ERR_NO_MEMORY)
///< device unexist
#define VS_ERR_TDC_DEVICE_UNEXIST     VS_ERR_CODE(E_MOD_ID_TDC, E_ERR_DEVICE_UNEXIST)

/**
* @brief Defines signal edge type.
* @details
*/
typedef enum vs_tdc_edge_type {
    E_TDC_EDGE_TYPE_POSE = 0,    ///< use the rising edge to trigger signal
    E_TDC_EDGE_TYPE_NEGE,        ///< use the falling edge to trigger signal

    E_TDC_EDGE_TYPE_MAX
} vs_tdc_edge_type_e;

/**
* @brief Defines xvs/xhs signal run mode.
* @details
*/
typedef enum vs_tdc_slave_dev_run_mode {
    E_TDC_SLAVE_DEV_RUN_MODE_FREE = 0,   ///< free run
    E_TDC_SLAVE_DEV_RUN_MODE_SYNC,       ///< trace sync signal
    E_TDC_SLAVE_DEV_RUN_MODE_MAX,
} vs_tdc_slave_dev_run_mode_e;

/**
* @brief Defines sync signal attr.
* @details
*/
typedef struct vs_tdc_sync_attr {
    vs_uint32_t in_freq;             ///< sync signal input frequency
    vs_tdc_edge_type_e edge;         ///< sync signal edge type
    vs_uint32_t sensor_fps;          ///< sensor real frame rate
    vs_uint32_t min_cycle;           ///< sync signal cycle minimum (unit: sensor_clk cycle)
    vs_uint32_t max_cycle;           ///< sync signal cycle maximum (unit: sensor_clk cycle)
} vs_tdc_sync_attr_s;

/**
* @brief Defines slave device output attr.
* @details
*/
typedef struct vs_tdc_slave_dev_attr {
    vs_bool_t enable;                 ///< enable output xvs/xhs signal
    vs_tdc_slave_dev_run_mode_e mode; ///< run mode
    vs_tdc_edge_type_e edge;          ///< edge type

    vs_uint32_t xvs_delay;            ///< xvs delay from sync signal              (unit: sensor_clk cycle)
    vs_uint16_t xvs_pulse_width;      ///< xvs signal pulse width                  (unit: sensor_clk cycle)
    vs_uint16_t vmax_lines;           ///< ouput xhs signal nums in one xvs period (unit: xhs_period)

    vs_uint16_t xhs_delay;            ///< xhs delay from xvs signal   (unit: sensor_clk cycle)
    vs_uint16_t xhs_pulse_width;      ///< xhs signal pulse width      (unit: sensor_clk cycle)
    vs_uint16_t xhs_period;           ///< clk cycle in one xhs period (unit: sensor_clk cycle)
} vs_tdc_slave_dev_attr_s;

/**
* @brief Defines strobe flash signal attr.
* @details
*/
typedef struct vs_tdc_strobe_flash_attr {
    vs_bool_t enable;              ///< enable output exposure_flash signal
    vs_uint16_t bind_slave_dev_id; ///< bind slave device id
    vs_tdc_edge_type_e edge;       ///< edge type
    vs_uint16_t delay;             ///< delay from xvs signal                 (unit: xhs_period)
    vs_uint16_t period;            ///< pwm period                            (unit: xhs_period)
    vs_uint16_t width;             ///< pwm width                             (unit: xhs_period)
} vs_tdc_strobe_flash_attr_s;

/**
* @brief Defines exposure flash signal attr.
* @details
*/
typedef struct vs_tdc_exposure_flash_attr {
    vs_bool_t enable;              ///< enable output exposure_flash signal
    vs_uint16_t bind_slave_dev_id; ///< bind slave device id
    vs_tdc_edge_type_e edge;       ///< edge type
    vs_uint16_t delay;             ///< delay from xvs signal                 (unit: xhs_period)
    vs_uint16_t pulse_width;       ///< the exposure_flash signal pulse width (unit: xhs_period)
} vs_tdc_exposure_flash_attr_s;

/**
* @brief Defines snap mode.
* @details
*/
typedef enum vs_tdc_snap_mode {
    E_TDC_SNAP_MODE_ONCE = 0,   ///< snap one time when trigger
    E_TDC_SNAP_MODE_PERIOD,     ///< snap period time

    E_TDC_SNAP_MODE_MAX,
} vs_tdc_snap_mode_e;

/**
* @brief Defines snap attr.
* @details
*/
typedef struct vs_tdc_snap_attr {
   vs_int32_t vii_dev_id;  ///< bind vii dev id.
   vs_int32_t exposure_flash_id[TDC_MAX_EXPOSURE_FLASH_NUM];  ///< select sync exposure flash id.
   vs_uint32_t exposure_flash_num;                            ///< exposure flash num.

   vs_int32_t strobe_flash_id[TDC_MAX_STROBE_FLASH_NUM];     ///< select sync strobe flash id.
   vs_int32_t strobe_flash_width[TDC_MAX_STROBE_FLASH_NUM]; ///< strobe flash extra width set when snap.
   vs_uint32_t strobe_flash_num;                             ///< strobe flash num.

   vs_tdc_snap_mode_e snap_mode;  ///< snap mode select.
   vs_uint32_t frame_num;         ///< snap frame num, available when snap mode is E_TDC_SNAP_MODE_PERIOD.
   vs_uint32_t interval_num;      ///< snap interval num, available when snap mode is E_TDC_SNAP_MODE_PERIOD.

   vs_uint64_t private_data;     ///< usr data pass through to video frame info.
} vs_tdc_snap_attr_s;

#ifdef __cplusplus
}
#endif

#endif // __VS_TDC_DEFINES_H__