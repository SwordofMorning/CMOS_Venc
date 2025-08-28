/*
 * @file vs_dma_defines.h
 * @brief Declaraction of dma enumeration and struct
 * @details Declaration of dma enumeration and struct
 * @author VS platform group
 * @date 2025-05-20
 * @version v0.1
 * @Copyright (C) 2023, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
 */

#ifndef __VS_DMA_DEFINES_H__
#define __VS_DMA_DEFINES_H__

#include "vs_type.h"
#include "vs_errno.h"
#include "vs_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

///< dma device name
#define VS_DMA_DEVICE "/dev/vs_dma"
///< channel is not created
#define VS_ERR_DMA_DEVICE_UNEXIST	VS_ERR_CODE(E_MOD_ID_DMA, E_ERR_DEVICE_UNEXIST)
///< parameter is illagal
#define VS_ERR_DMA_INVALID_PARAM	VS_ERR_CODE(E_MOD_ID_DMA, E_ERR_INVALID_PARAM)
///< using a NULL pointer
#define VS_ERR_DMA_NULL_PTR            VS_ERR_CODE(E_MOD_ID_DMA, E_ERR_NULL_PTR)

#ifdef __cplusplus
}
#endif

#endif
