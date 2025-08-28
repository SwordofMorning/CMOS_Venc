
/**
* @file vs_ref_defines.h
* @brief Declaration of ref enumeration, struct and API.
* @details Declaration of ref enumeration, struct and API.
* @author VS software group
* @date 2025-01-01
* @version v0.01
* @copyright (C) 2025, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/

#ifndef __VS_REF_DEFINES_H__
#define __VS_REF_DEFINES_H__

/*****************************************************************************
 *                    Includes
 *****************************************************************************/
#include "vs_type.h"
#include "vs_rvs_defines.h"


#ifdef __cplusplus
extern "C"{
#endif

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/



/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/**
* @brief The imu raw data type
*/
typedef enum vs_ref_rvs_imu_data_type{
    E_REF_TYPE_GYRO = 0, ///< type is gyro
    E_REF_TYPE_ACCEL,    ///< type is accelerator
    E_REF_TYPE_MAG,      ///< type is magnetic
    E_REF_TYPE_MAX
} vs_ref_rvs_imu_data_type_e;

/**
* @brief imu raw data format
*/
typedef struct vs_ref_rvs_imu_raw_data {
    vs_int16_t value[VS_RVS_IMU_RAW_DATA_LEN];  ///< imu raw data
    vs_int16_t pedding;                         ///< padding data
    vs_uint64_t soc_ts;                          ///< soc timestamp
    vs_uint64_t imu_ts;                          ///< imu timestamp
} vs_ref_rvs_imu_raw_data_s;

/**
* @brief The ref attribution
*/
typedef struct vs_ref_rvs_attr {
    vs_int32_t vii_pipeid;      ///< vii pipe id; Range:[0x0, 0x7]
    vs_rvs_type_e type;         ///< rvs type
    vs_int32_t imu_buffer_size; ///< imu buffer size
    vs_int32_t imu_buffer_num;  ///< imu buffer number
} vs_ref_rvs_attr_s;

#ifdef __cplusplus
}
#endif

#endif
