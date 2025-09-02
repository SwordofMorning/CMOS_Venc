/**
* @file vs_mal_hdmi.h
* @brief hdmi mal interface
* @details contains hdmi mal APIs
* @author VS platform group
* @date 2022-02-16
* @version v0.10
* @Copyright (C) 2022, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/

#ifndef __VS_MAL_HDMI_H__
#define __VS_MAL_HDMI_H__

#include "vs_hdmi_defines.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
* @brief Init HDMI device
* @details Init HDMI device
* @return   0 call success.
* @return   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_hdmi_init(vs_void_t);

/**
* @brief Deinit HDMI device
* @details Deinit HDMI device
* @return   0 call success.
* @return   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_hdmi_deinit(vs_void_t);

/**
* @brief Open HDMI device
* @details Open HDMI device.
* @param [in]  hdmi_devid
* @return   0 call success.
* @return   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_hdmi_open(vs_uint32_t hdmi_devid);

/**
* @brief Close HDMI device
* @details Close HDMI device
* @param [in]  hdmi_devid
* @return   0 call success.
* @return   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_hdmi_close(vs_uint32_t hdmi_devid);

/**
* @brief Set HDMI attribution
* @details This api is used to configure HDMI device. It is recommended to call the vs_mal_hdmi_attr_get()
* before the first call to vs_mal_hdmi_attr_set() to get default attribution.
* @param [in]  hdmi_devid
* @param [in]  p_attr
* @return   0 call success.
* @return   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_hdmi_attr_set(vs_uint32_t hdmi_devid, const vs_hdmi_attr_s *p_attr);

/**
* @brief Get HDMI attribution
* @details This api is used to get the configuration of HDMI device. It can also get default configuration
* of HDMI before calling vs_mal_hdmi_attr_set(). Therefore, it is recommended to call the vs_mal_hdmi_attr_get()
* before the first call to vs_mal_hdmi_attr_set()
* @param [in]  hdmi_devid
* @param [out]  p_attr
* @return   0 call success.
* @return   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_hdmi_attr_get(vs_uint32_t hdmi_devid, vs_hdmi_attr_s *p_attr);

/**
* @brief Start HDMI device
* @details Start HDMI device
* @param [in]  hdmi_devid
* @return   0 call success.
* @return   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_hdmi_start(vs_uint32_t hdmi_devid);

/**
* @brief Stop HDMI device
* @details Stop HDMI device
* @param [in]  hdmi_devid
* @return   0 call success.
* @return   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_hdmi_stop(vs_uint32_t hdmi_devid);

/**
* @brief Get sink capability
* @details Get sink capability
* @param [in]  hdmi_devid
* @param [out]  p_sink_cap
* @return   0 call success.
* @return   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_hdmi_sink_cap_get(vs_uint32_t hdmi_devid, vs_hdmi_sink_cap_s *p_sink_cap);

/**
* @brief Get EDID row data forcibly
* @details Get EDID row data forcibly
* @param [in]  hdmi_devid
* @param [out]  p_edid_data
* @return   0 call success.
* @return   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_hdmi_edid_force_get(vs_uint32_t hdmi_devid, vs_hdmi_edid_s *p_edid_data);

/**
* @brief Register HDMI callback function
* @details Register HDMI callback function
* @param [in]  hdmi_devid
* @param [in]  p_callback
* @return   0 call success.
* @return   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_hdmi_callback_register(vs_uint32_t hdmi_devid, const vs_hdmi_callback_s *p_callback);

/**
* @brief Unregister HDMI callback function
* @details Unregister HDMI callback function
* @param [in]  hdmi_devid
* @param [in]  p_callback
* @return   0 call success.
* @return   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_hdmi_callback_unregister(vs_uint32_t hdmi_devid, const vs_hdmi_callback_s *p_callback);

/**
* @brief Set HDMI infoframe
* @details This api could set HDMI infoframe manually.
* @param [in]  hdmi_devid
* @param [in]  p_infoframe
* @return   0 call success.
* @return   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_hdmi_infoframe_set(vs_uint32_t hdmi_devid, const vs_hdmi_infoframe_s *p_infoframe);

/**
* @brief Get HDMI infoframe
* @details This api could get HDMI infoframe information.
* @param [in]  hdmi_devid
* @param [in]  infoframe_type
* @param [out]  p_infoframe
* @return   0 call success.
* @return   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_hdmi_infoframe_get(vs_uint32_t hdmi_devid, vs_hdmi_infoframe_type_e infoframe_type,
                             vs_hdmi_infoframe_s *p_infoframe);

/**
* @brief Set HDMI phy parameters
* @details This api is used to set phy parameters. These phy parameters adjust
* the HDMI voltage to accommodate long cable scenarios
* @param [in]  hdmi_devid
* @param [in]  p_param_mod
* @return   0 call success.
* @return   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_hdmi_param_mod_set(vs_uint32_t hdmi_devid, const vs_hdmi_param_mod_s *p_param_mod);

/**
* @brief Get HDMI phy parameters
* @details This api is used to get phy parameters
* @param [in]  hdmi_devid
* @param [out]  p_param_mod
* @return   0 call success.
* @return   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_hdmi_param_mod_get(vs_uint32_t hdmi_devid, vs_hdmi_param_mod_s *p_param_mod);

#ifdef __cplusplus
}
#endif

#endif /*!< __VS_MAL_HDMI_H__ */
