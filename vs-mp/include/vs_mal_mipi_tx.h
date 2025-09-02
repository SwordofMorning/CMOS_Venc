/**
* @file vs_mal_mipi_tx.h
* @brief mipi tx mal interface
* @details contains data structures and APIs for mipi tx
* @author VS platform group
* @date 2022-02-16
* @version v0.10
* @Copyright (C) 2022, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/

#ifndef __VS_MAL_MIPI_TX_H__
#define __VS_MAL_MIPI_TX_H__

#include "vs_mipi_tx_defines.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
* @brief Open mipi tx device before any other operations.
* @details
* @return VS_SUCCESS on success and error codes on failure.
*/
vs_int32_t vs_mal_mipi_tx_open(vs_void_t);

/**
* @brief Close mipi tx device after use.
* @details
* @return VS_SUCCESS on success and error codes on failure.
*/
vs_int32_t vs_mal_mipi_tx_close(vs_void_t);

/**
* @brief Start mipi tx display.
* @details
* @param [in] mipi_tx_devid  mipi tx device id.
* @return VS_SUCCESS on success and error codes on failure.
*/
vs_int32_t vs_mal_mipi_tx_start(vs_uint32_t mipi_tx_devid);

/**
* @brief Stop mipi tx display.
* @details
* @param [in] mipi_tx_devid  mipi tx device id.
* @return VS_SUCCESS on success and error codes on failure.
*/
vs_int32_t vs_mal_mipi_tx_stop(vs_uint32_t mipi_tx_devid);

/**
* @brief Perform dsi write command.
* @details
* @param [in] mipi_tx_devid  mipi tx device id.
* @param [in] p_cmd  The write command to be sent.
* @return VS_SUCCESS on success and error codes on failure.
*/
vs_int32_t vs_mal_mipi_tx_cmd_write(vs_uint32_t mipi_tx_devid, const vs_mipi_tx_cmd_write_s *p_cmd);

/**
* @brief Perform dsi read command.
* @details
* @param [in] mipi_tx_devid  mipi tx device id.
* @param [in] p_cmd  The read command to be sent.
* @return VS_SUCCESS on success and error codes on failure.
*/
vs_int32_t vs_mal_mipi_tx_cmd_read(vs_uint32_t mipi_tx_devid, const vs_mipi_tx_cmd_read_s *p_cmd);

/**
* @brief Configure mipi tx parameters.
* @details
* @param [in] mipi_tx_devid  mipi tx device id.
* @param [in] p_config  Pointer to the configuration structure.
* @return VS_SUCCESS on success and error codes on failure.
*/
vs_int32_t vs_mal_mipi_tx_config(vs_uint32_t mipi_tx_devid, const vs_mipi_tx_config_s *p_config);

#ifdef __cplusplus
}
#endif

#endif ///< end of __VS_MAL_MIPI_TX_H__

