/**
* @file vs_mal_tdc.h
* @brief Declaration of signal API.
* @details Declaration of signal API.
* @author Visinex software group
* @date 2022-01-27
* @version v0.01
* @copyright (C) 2022, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/
#ifndef __VS_MAL_TDC_H__
#define __VS_MAL_TDC_H__

#include "vs_tdc_defines.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
* @brief  set sync attr
* @details
* @param [in]  p_attr The pointer of struct sync attributes, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_tdc_sync_attr_set(const vs_tdc_sync_attr_s *p_attr);

/**
* @brief  get sync attr
* @details
* @param [out]  p_attr The pointer of struct sync attributes, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_tdc_sync_attr_get(vs_tdc_sync_attr_s *p_attr);

/**
* @brief  Set slave device attr by slave device id.
* @details Set slave device attr by slave device id.
* @param [in]  slave_dev_id indicate the slave device id to be given.
* @param [in]  p_attr The pointer of struct slave device attributes, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_tdc_slave_dev_attr_set(vs_int32_t slave_dev_id, const vs_tdc_slave_dev_attr_s *p_attr);

/**
* @brief  Get slave device attr by slave device id.
* @details Get slave device attr by slave device id.
* @param [in]  slave_dev_id indicate the slave device id to be given.
* @param [out]  p_attr The pointer of struct slave device attributes, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_tdc_slave_dev_attr_get(vs_int32_t slave_dev_id, vs_tdc_slave_dev_attr_s *p_attr);

/**
* @brief  Set strobe flash attr by strobe flash id.
* @details Set strobe flash attr by strobe flash id.
* @param [in]  strobe_flash_id  indicate the strobe flash id to be given.
* @param [in]  p_attr The pointer of struct strobe flash attributes, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_tdc_strobe_flash_attr_set(vs_int32_t strobe_flash_id,
    const vs_tdc_strobe_flash_attr_s *p_attr);

/**
* @brief  Get strobe flash attr by strobe flash id.
* @details Get strobe flash attr by strobe flash id.
* @param [in]  stro_flash_id  indicate the strobe flash id to be given.
* @param [out]  p_attr The pointer of struct strobe flash attributes, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_tdc_strobe_flash_attr_get(vs_int32_t strobe_flash_id,
    vs_tdc_strobe_flash_attr_s *p_attr);

/**
* @brief  Set exposure flash attr by exposure flash id.
* @details Set exposure flash attr by exposure flash id.
* @param [in]  exposure_flash_id indicate the exposure flash id to be given.
* @param [in]  p_attr The pointer of struct exposure flash attributes, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_tdc_exposure_flash_attr_set(vs_int32_t exposure_flash_id,
    const vs_tdc_exposure_flash_attr_s *p_attr);

/**
* @brief  Get exposure flash attr by exposure flash id.
* @details Set exposure flash attr by exposure flash id.
* @param [in]  exposure_flash_id indicate the exposure flash id to be given.
* @param [out]  p_attr The pointer of struct exposure flash attributes, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_tdc_exposure_flash_attr_get(vs_int32_t exposure_flash_id,
    vs_tdc_exposure_flash_attr_s *p_attr);

/**
* @brief  Trigger snap by slave device id.
* @details Trigger snap by slave device id.
* @param [in]  slave_dev_id indicate the slave device id to be given.
* @param [in]  p_attr indicate the snap attributes to be given.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_tdc_snap_trigger(vs_int32_t slave_dev_id, const vs_tdc_snap_attr_s *p_attr);

/**
* @brief  Set the time of wait frame num by slave device id.
* @details Set the time of wait frame num by slave device id.
* @param [in]  slave_dev_id indicate the slave device id to be given.
* @param [in]  frame_num indicate the number of frame period time.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_tdc_wait_frame_time_set(vs_int32_t vii_devid,  vs_uint32_t frame_num);

/**
* @brief  Get sync signal count statistics within input time.
* @details Get sync signal count statistics within input time.
* @param [in]  second indicate the time of sync signal count statistics.
* @return   positive call success, indicate sync count.
* @warning   0 call failed. see mal reference for detail.
* @par
*
*/
vs_uint32_t vs_mal_tdc_sync_count_get(vs_uint32_t second);

#ifdef __cplusplus
}
#endif

#endif // __VS_MAL_TDC_H__