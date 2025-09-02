
/**
* @file vs_rvs_defines.h
* @brief Declaration of rvs enumeration, struct and API.
* @details Declaration of rvs enumeration, struct and API.
* @author VS software group
* @date 2025-01-01
* @version v0.01
* @copyright (C) 2025, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/

#ifndef __VS_RVS_DEFINES_H__
#define __VS_RVS_DEFINES_H__

/*****************************************************************************
 *                    Includes
 *****************************************************************************/
#include "vs_type.h"
#include "vs_comm.h"
#include "vs_comm_video.h"
#include "vs_errno.h"

#ifdef __cplusplus
extern "C"{
#endif

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/


#define VS_ERR_RVS_INVALID_PARAM        VS_ERR_CODE(E_MOD_ID_RVS,E_ERR_INVALID_PARAM)           ///< parameter is invalid
#define VS_ERR_RVS_NULL_PTR             VS_ERR_CODE(E_MOD_ID_RVS,E_ERR_NULL_PTR)                ///< pointer val is null
#define VS_ERR_RVS_DEVICE_UNEXIST       VS_ERR_CODE(E_MOD_ID_RVS,E_ERR_DEVICE_UNEXIST)          ///< rvs device does not exist
#define VS_ERR_RVS_NOT_INITIALIZED      VS_ERR_CODE(E_MOD_ID_RVS,E_ERR_NOT_INITIALIZED)         ///< module uninitialized
#define VS_ERR_RVS_NOT_SUPPORT          VS_ERR_CODE(E_MOD_ID_RVS,E_ERR_NOT_SUPPORT)             ///< operation not supported
#define VS_ERR_RVS_NOT_PERM             VS_ERR_CODE(E_MOD_ID_RVS,E_ERR_NOT_PERM)                ///< operation not permitted
#define VS_ERR_RVS_NO_MEMORY            VS_ERR_CODE(E_MOD_ID_RVS,E_ERR_NO_MEMORY)               ///< no memory
#define VS_ERR_RVS_INVALID_PIPEID       VS_ERR_CODE(E_MOD_ID_RVS,E_ERR_INVALID_PIPEID)          ///< invalid pipe id

#define VS_ERR_RVS_FRAME_LATE           VS_ERR_CODE(E_MOD_ID_RVS,E_ERR_FRAME_LATE)              ///< image frame is later than motion vector
#define VS_ERR_RVS_BINARY_EMPTY         VS_ERR_CODE(E_MOD_ID_RVS,E_ERR_BINARY_EMPTY)            ///< queue of motion vector binary is empty
#define VS_RVS_BIAS_SIZE                (6)                                                     ///< imu bias size
#define VS_RVS_ORIENTATION_SIZE         (4)                                                     ///< imu orientation size
#define VS_RVS_CAMERA_MATRIX_SIZE       (9)                                                     ///< camera matrix size
#define VS_RVS_IMU_RAW_DATA_LEN         (3)                                                     ///< imu raw data length


/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

typedef enum vs_rvs_error_code
{
    E_ERR_FRAME_LATE = 0x60,     ///< alignment error
    E_ERR_BINARY_EMPTY,          ///< image stride,width or height error
    E_ERR_CODE_MAX
}vs_rvs_error_code_e;

/**
* @brief The RVS type
*/
typedef enum vs_rvs_type{
    E_RVS_TYPE_DIS = 0, ///< type is dis
    E_RVS_TYPE_EIS,     ///< type is eis
    E_RVS_TYPE_MAX
} vs_rvs_type_e;


/**
* @brief The eis parameters
*/
typedef struct vs_rvs_eis_attr
{
    vs_int32_t eis_enable;                                      ///<    RW; Range:[0x0, 0x1]; EIS enable flag
    vs_int32_t output_width;                                    ///<    RW; Range:[0, 3840]; EIS output image width
    vs_int32_t output_height;                                   ///<    RW; Range:[0, 2160]; EIS output image height
    vs_double_t bias[VS_RVS_BIAS_SIZE];                         ///<    RW; Range:[-65536.0, 65535.0]; imu bias when motionless
    vs_double_t ascale;                                         ///<    RW; Range:[-1.0, 1.0]; accelerate scale
    vs_double_t gscale;                                         ///<    RW; Range:[-1.0, 1.0]; gyro scale
    vs_double_t tscale;                                         ///<    RW; Range:[-1.0, 1.0]; timestamp scale
    vs_double_t frame_readout_time;                             ///<    RW; Range:[0.0, 64.0]; Sensor frame read out time
    vs_double_t gyro_time_offset;                               ///<    RW; Range:[-64.0, 64.0]; gyro timestamp offset
    vs_char_t gyro_orientation[VS_RVS_ORIENTATION_SIZE];        ///<    RW; gyro 3-axis orientation
    vs_double_t gyro_frequency;                                 ///<    RW; Range:[0.0, 32767.0]; gyro sample frequency
    vs_double_t camera_matrix[VS_RVS_CAMERA_MATRIX_SIZE];       ///<    RW; Range:[0.0, 16777215.0]; camera internal parameter
    vs_int32_t pre_eis_crop_flag;                               ///<    RW; Range:[0x0, 0x1]; pre-crop enable flag
    vs_int32_t pre_eis_width;                                   ///<    RW; Range:[0, 3840]; pre-crop width
    vs_int32_t pre_eis_height;                                  ///<    RW; Range:[0, 2160]; pre-crop height
    vs_int32_t pre_eis_vertical_offset;                         ///<    RW; Range:[0, 2160]; pre-crop vertical offset
    vs_double_t crop_ratio;                                     ///<    RW; Range:[0.2, 0.99]; crop ratio
    vs_double_t padding_ratio;                                  ///<    RW; Range:[0.0, 0.99]; padding ratio
    vs_double_t alpha_min;                                      ///<    RW; Range:[0.0, 0.999]; min smoothing coefficient
    vs_double_t alpha_max;                                      ///<    RW; Range:[0.0, 0.999]; max smoothing coefficient
    vs_double_t alpha_ratio;                                    ///<    RW; Range:[0.0, 1.0]; smoothing ratio
    vs_double_t alpha_damping;                                  ///<    RW; Range:[0.0, 1.0]; fussion ratio
    vs_double_t beta;                                           ///<    RW; Range:[1.0, 5.0]; attenuation coefficient
    vs_int32_t smoothing_method;                                ///<    RW; Range:[0x0, 0x1]; smooth strategy
    vs_int32_t center_enable;                                   ///<    RW; Range:[0x0, 0x1]; center enable flag
    vs_int32_t center_x;                                        ///<    RW; Range:[0, 3840]; horizontal center position
    vs_int32_t center_y;                                        ///<    RW; Range:[0, 2160]; vertical center position
    vs_double_t digital_zoom;                                   ///<    RW; Range:[1.0, 5.0]; digital zoom
} vs_rvs_eis_attr_s;

/**
* @brief The IMU data
*/
typedef struct vs_rvs_imu_data {
    vs_uint64_t ts;                             ///< timestamp,ms as unit
    vs_int16_t gyro[VS_RVS_IMU_RAW_DATA_LEN];   ///< gyro data
    vs_int16_t accl[VS_RVS_IMU_RAW_DATA_LEN];   ///< accelerate data
    vs_int16_t magn[VS_RVS_IMU_RAW_DATA_LEN];   ///< magnetometer data
} vs_rvs_imu_data_s;


#ifdef __cplusplus
}
#endif

#endif
