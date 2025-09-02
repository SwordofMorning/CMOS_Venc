/*
 * @file vs_mal_dma.h
 * @brief Declaraction of dma API
 * @details Declaration of dma API
 * @author VS platform group
 * @date 2025-05-20
 * @version v0.1
 * @Copyright (C) 2023, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
 */

#ifndef __VS_MAL_DMA_H__
#define __VS_MAL_DMA_H__

#include "vs_dma_defines.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief  Open dma device and get dma channel
 * @details  Open dma device and get dma channel
 * @param [out]  p_handle the pointer to dma channel handle, can not be NULL.
 * @return   VS_SUCCESS on success and error code on failure.
 */
vs_int32_t vs_mal_dma_chn_alloc(vs_uint32_t *p_handle);

/**
 * @brief  Start a dma transfer.
 * @details  Start a dma transfer.
 * @param [in]  p_handle dma channel handle.
 * @param [in]  dst_phys_addr Dma destination physical buffer.
 * @param [in]  src_phys_addr Dma source physical buffer.
 * @param [in]  len Dma buffer size.
 * @return   VS_SUCCESS on success and error code on failure.
 */
vs_int32_t vs_mal_dma_copy(vs_uint32_t p_handle, vs_uint64_t dst_phys_addr, vs_uint64_t src_phys_addr, vs_uint32_t len);

/**
 * @brief  Free dma channel and device.
 * @details  Free dma channel and device.
 * @param [in]  p_handle dma channel handle.
 * @return   VS_SUCCESS on success and error code on failure.
 */
vs_int32_t vs_mal_dma_chn_free(vs_uint32_t p_handle);

#ifdef __cplusplus
}
#endif

#endif
