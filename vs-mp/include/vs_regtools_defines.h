/**
 * @file    vs_regtools_defines.h
 * @brief   regtools struct and macro defines
 * @details regtools struct and macro defines
 * @author  Visinex Software Group
 * @date    2022-07-18
 * @version v1.00
 * @Copyright (c) 2021 Shanghai Visinex Technologies Co., Ltd. All rights reserved.
 *
 */

#ifndef __VS_REGTOOLS_DEFINES_H__
#define __VS_REGTOOLS_DEFINES_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "vs_type.h"
#include "vs_comm.h"
#include "vs_errno.h"

#define VS_ERR_REGTOOLS_INVALID_PARAM    VS_ERR_CODE(E_MOD_ID_REGTOOLS, E_ERR_INVALID_PARAM)
#define VS_ERR_REGTOOLS_NULL_PTR         VS_ERR_CODE(E_MOD_ID_REGTOOLS, E_ERR_NULL_PTR)
#define VS_ERR_REGTOOLS_NOT_SUPPORT      VS_ERR_CODE(E_MOD_ID_REGTOOLS, E_ERR_NOT_SUPPORT)
#define VS_ERR_REGTOOLS_READ             VS_ERR_CODE(E_MOD_ID_REGTOOLS, E_ERR_READ)
#define VS_ERR_REGTOOLS_WRITE            VS_ERR_CODE(E_MOD_ID_REGTOOLS, E_ERR_WRITE)
#define VS_ERR_REGTOOLS_DEVICE_UNEXIST   VS_ERR_CODE(E_MOD_ID_REGTOOLS, E_ERR_DEVICE_UNEXIST)

#define VS_REGTOOLS_DEV		"vs_regtools"

typedef enum vs_spi_mode {
    E_SPI_MODE0 = 0,	///< CPOL:0, CPHA:0
    E_SPI_MODE1,		///< CPOL:0, CPHA:1
    E_SPI_MODE2,		///< CPOL:1, CPHA:0
    E_SPI_MODE3,		///< CPOL:1, CPHA:1
    E_SPI_MODE_MAX
}vs_spi_mode_e;



#ifdef __cplusplus
}
#endif

#endif /*!< __VS_REGTOOLS_DEFINES_H__ */
