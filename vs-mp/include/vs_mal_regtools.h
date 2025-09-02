/**
 * @file    vs_mal_regtools.h
 * @brief   regtools api interface
 * @details contain apis
 * @author  Visinex Software Group
 * @date    2022-07-18
 * @version v1.00
 * @Copyright (c) 2021 Shanghai Visinex Technologies Co., Ltd. All rights reserved.
 *
 */

#ifndef __VS_MAL_REGTOOLS_H__
#define __VS_MAL_REGTOOLS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "vs_regtools_defines.h"

/**
 * @brief   Read register
 * @details Read register
 * @param[in]  reg  Register address
 * @param[out] p_val  The pointer of the register value
 * @return  HAL execution result
 * @warning
 * @see enum vs_hal_result_e
 * @note
 * @par
 *
 */
vs_int32_t vs_mal_reg_read(vs_uint32_t reg, vs_uint32_t *p_val);

/**
 * @brief   Write register
 * @details Write register
 * @param[in]  reg  Register address
 * @param[out] val  Register value
 * @return  HAL execution result
 * @warning
 * @see enum vs_hal_result_e
 * @note
 * @par
 *
 */
vs_int32_t vs_mal_reg_write(vs_uint32_t reg, vs_uint32_t val);


/**
 * @brief   Read register with i2c
 * @details Read register with i2c
 * @param[in]  i2c_dev i2c id
 * @param[in]  dev_addr device address
 * @param[in]  reg_addr register address ptr
 * @param[in]  addr_len reg_addr length, range [1, 4]
 * @param[out]  data_buf register value ptr
 * @param[in]  data_len data_buf len
 * @return  0 call success
 * @return  none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_i2c_read(vs_uint8_t i2c_dev, vs_uint8_t dev_addr, vs_int32_t reg_addr,  vs_int32_t addr_len, vs_uint8_t *data_buf, vs_int32_t data_len);

/**
 * @brief   Read register with i2c
 * @details Read register with i2c
 * @param[in]  i2c_dev i2c id
 * @param[in]  dev_addr device address
 * @param[in]  reg_addr register address ptr
 * @param[in]  addr_len reg_addr length, range [1, 4]
 * @param[in]  data_buf register value ptr
 * @param[in]  data_len data_buf length
 * @return  0 call success
 * @return  none0 call failed. see ERROR_CODE for detail.
 *
 */
vs_int32_t vs_mal_i2c_write(vs_uint8_t i2c_dev, vs_uint8_t dev_addr, vs_int32_t reg_addr, vs_int32_t addr_len, vs_uint8_t *data_buf, vs_int32_t data_len);

/**
 * @brief   Read or write with spi
 * @details Read or write with spi
 * @param[in] spi_dev spi dev id
 * @param[in] csn spi csn
 * @param[in] spi_mode spi mode
 * @param[in] tx_buf tx data buffer
 * @param[in] rx_buf rx data buffer
 * @param[in] len tx/rx data_buf length
 * @return  0 call success
 * @return  none0 call failed. see ERROR_CODE for detail.
 *
 */
vs_int32_t vs_mal_spi_transfer(vs_uint8_t spi_dev, vs_uint8_t csn,
		vs_spi_mode_e spi_mode, vs_uint8_t *tx_buf, vs_uint8_t *rx_buf,
		vs_int32_t len);

#ifdef __cplusplus
}
#endif

#endif /*!< __VS_MAL_REGTOOLS_H__ */
