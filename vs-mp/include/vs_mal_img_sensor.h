/**
* @file vs_mal_img_sensor.h
* @brief Declaraction of sensor api and struct
* @details
* @author VS software group
* @date 2022-01-01
* @version v0.10
* @Copyright (C) 2022, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/


#ifndef __VS_MAL_IMG_SENSOR_H__
#define __VS_MAL_IMG_SENSOR_H__

/*****************************************************************************
 *                    Includes
 *****************************************************************************/
#include "vs_comm.h"
#include "vs_comm_video.h"
#include "vs_mal_isp_img_sensor.h"
#ifdef __cplusplus
extern "C"{
#endif

/*****************************************************************************
 *                    Image Sensor API Prototypes
 *****************************************************************************/
typedef struct vs_img_sensor_user_ops {
    /**
    * @brief  Set sensor bus attribute by pipe id and bus attribute.
    * @details Set sensor bus attribute by pipe id and bus attribute.
    * @param [in]  vii_pipe Indicate the pipe id to be given.
    * @param [in]  bus_attr Indicate the bus attribute to be given.
    * @return   VS_SUCCESS call success.
    * @warning   VS_FAILED call failed.
    * @par
    *
    */
    vs_int32_t (*bus_attr_set)(vs_int32_t vii_pipe, vs_isp_img_sensor_bus_attr_u bus_attr);

    /**
    * @brief  Set sensor standby mode by pipe id.
    * @details Set sensor standby mode by pipe id.
    * @param [in]  vii_pipe Indicate the pipe id to be given.
    * @return   VS_SUCCESS call success.
    * @warning   VS_FAILED call failed.
    * @par
    *
    */
    vs_int32_t  (*standby)(vs_int32_t vii_pipe);

    /**
    * @brief  Restart sensor by pipe id.
    * @details Restart sensor by pipe id.
    * @param [in]  vii_pipe Indicate the pipe id to be given.
    * @return   VS_SUCCESS call success.
    * @warning   VS_FAILED call failed.
    * @par
    *
    */
    vs_int32_t  (*restart)(vs_int32_t vii_pipe);

    /**
    * @brief  Set sensor mirror or flip type by pipe id and mirror flip type.
    * @details Set sensor mirror or flip type by pipe id and mirror flip type.
    * @param [in]  vii_pipe Indicate the pipe id to be given.
    * @param [in]  mirror_flip Indicate the mirror or flip type to be given.
    * @return   VS_SUCCESS call success.
    * @warning   VS_FAILED call failed.
    * @par
    *
    */
    vs_int32_t  (*mirror_flip)(vs_int32_t vii_pipe, vs_mirror_mode_e mirror_flip);
} vs_img_sensor_user_ops_s;


/**
 * @brief  Get the functions sensorlib provide to user.
 * @details Get the functions sensorlib provide to user.
 * @param [out]  p_user_ops The pointer of functions provided to user.
 * @return   VS_SUCCESS call success.
 * @warning   VS_FAILED call failed.
 * @par
 *
 */
vs_int32_t vs_img_sensor_user_ops_get(vs_void_t **p_user_ops);

#ifdef __cplusplus
}
#endif
#endif /*!<  _VS_MAL_IMG_SENSOR_H_ */
