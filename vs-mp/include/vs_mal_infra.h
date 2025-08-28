/**
* @file vs_mal_infra.h
* @brief Declaration of INFRA API.
* @details Declaration of INFRA API.
* @author Visinex software group
* @date 2024-07-16
* @version v0.01
* @copyright (C) 2024, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/
#ifndef __VS_MAL_INFRA_H__
#define __VS_MAL_INFRA_H__

#include "vs_type.h"
#include "vs_infra_defines.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
* @brief  Set infra attr by device id.
* @details  Set infra attr by device id.
* @param [in]  devid Indicate the device id to be given.
* @param [in]  p_attr The pointer of struct device attributes, can not be NULL.
* @return  0 call success.
* @warning  none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_infra_attr_set(vs_int32_t devid, const vs_infra_attr_s *p_attr);

/**
* @brief  Get infra attr by device id.
* @details  Get infra attr by device id.
* @param [in]  devid Indicate the device id to be given.
* @param [out]  p_attr The pointer of struct device attributes, can not be NULL.
* @return  0 call success.
* @warning  none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_infra_attr_get(vs_int32_t devid, vs_infra_attr_s *p_attr);

/**
* @brief  Set iframe attr by device id.
* @details  Set iframe attr by device id.
* @param [in]  devid Indicate the device id to be given.
* @param [in]  p_iframe_cfg The pointer of struct infra attributes, can not be NULL.
* @return  0 call success.
* @warning  none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_infra_iframe_config_set(vs_int32_t devid, const vs_infra_iframe_cfg_s *p_iframe_cfg);

/**
* @brief  Get iframe attr by device id.
* @details  Get iframe attr by device id.
* @param [in]  devid Indicate the device id to be given.
* @param [out]  p_iframe_cfg The pointer of struct infra attributes, can not be NULL.
* @return  0 call success.
* @warning  none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_infra_iframe_config_get(vs_int32_t devid, vs_infra_iframe_cfg_s *p_iframe_cfg);

/**
* @brief  Set ooc attr by device id.
* @details  Set ooc attr by device id.
* @param [in]  devid Indicate the device id to be given.
* @param [in]  p_ooc_cfg The pointer of struct ooc attributes, can not be NULL.
* @return  0 call success.
* @warning  none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_infra_ooc_config_set(vs_int32_t devid, const vs_infra_ooc_cfg_s *p_ooc_cfg);

/**
* @brief  Get ooc attr by device id.
* @details  Get ooc attr by device id.
* @param [in]  devid Indicate the device id to be given.
* @param [out]  p_ooc_cfg The pointer of struct ooc attributes, can not be NULL.
* @return  0 call success.
* @warning  none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_infra_ooc_config_get(vs_int32_t devid, vs_infra_ooc_cfg_s *p_ooc_cfg);

#ifdef __cplusplus
}
#endif

#endif // __VS_MAL_INFRA_H__
