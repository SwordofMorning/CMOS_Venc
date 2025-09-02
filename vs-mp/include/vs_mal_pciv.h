/**
* @file vs_mal_pciv.h
* @brief Declaraction of pciv API
* @details Declaration of pciv API
* @author VS platform group
* @date 2023-02-21
* @version v0.1
* @Copyright (C) 2023, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/
#ifndef __VS_MAL_PCIV_H__
#define __VS_MAL_PCIV_H__

#include "vs_type.h"
#include "vs_errno.h"
#include "vs_pciv_defines.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
* @brief  Create pciv channel by channel id and attributes.
* @details  Create pciv channel by channel id and attributes.
* @param [in]  pciv_chnid Channel id.
* @param [in]  p_attr The Pointer to channel attribute, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_pciv_create(vs_int32_t pciv_chnid, const vs_pciv_attr_s *p_attr);

/**
* @brief  Destroy pciv channel by channel id.
* @details  Destroy pciv channel by channel id.
* @param [in]  pciv_chnid Channel id.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_pciv_destroy(vs_int32_t pciv_chnid);

/**
* @brief  Set pciv channel attributes by channel id and attributes.
* @details  Set pciv channel attributes by channel id and attributes.
* @param [in]  pciv_chnid Channel id.
* @param [in]  p_attr Pointer to channel attributes, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_pciv_attr_set(vs_int32_t pciv_chnid, const vs_pciv_attr_s *p_attr);

/**
* @brief  Get pciv channel attributes by channel id.
* @details  Get pciv channel attributes by channel id.
* @param [in]  pciv_chnid Channel id.
* @param [out]  p_attr Pointer to channel attributes, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_pciv_attr_get(vs_int32_t pciv_chnid, vs_pciv_attr_s *p_attr);

/**
* @brief  Start pciv channel by channel id.
* @details  Start pciv channel attributes by channel id.
* @param [in]  pciv_chnid Channel id.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_pciv_start(vs_int32_t pciv_chnid);

/**
* @brief  Stop pciv channel by channel id.
* @details  Stop pciv channel attributes by channel id.
* @param [in]  pciv_chnid Channel id.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_pciv_stop(vs_int32_t pciv_chnid);

/**
* @brief  Malloc buffer for DMA transmission.
* @details  Malloc buffer for DMA transmission.
* @param [in]  blk_size buffer size.
* @param [in]  blk_cnt buffer count.
* @param [out]  p_phys_addr Pointer to buffer physical address, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_pciv_malloc(vs_uint32_t blk_size, vs_uint32_t blk_cnt, vs_uint64_t *p_phys_addr);

/**
* @brief  Free buffer for DMA transmission.
* @details  Free buffer for DMA transmission.
* @param [in]  blk_cnt buffer count.
* @param [in]  p_phys_addr Pointer to buffer physical address, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_pciv_free(vs_uint32_t blk_cnt, vs_uint64_t *p_phys_addr);

/**
* @brief  Malloc pciv channel video buffer for DMA transmission(only used in host).
* @details  Malloc pciv channel video buffer for DMA transmission(only used in host).
* @param [in]  pciv_chnid Channel id.
* @param [in]  blk_size buffer size.
* @param [in]  blk_size buffer count.
* @param [out]  p_phys_addr Pointer to buffer physical address, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_pciv_chn_buf_malloc(vs_int32_t pciv_chnid, vs_uint32_t blk_size, vs_uint32_t blk_cnt, vs_uint64_t *p_phys_addr);

/**
* @brief  Free pciv channel video buffer for DMA transmission(only used in host).
* @details  Free pciv channel video buffer for DMA transmission(only used in host).
* @param [in]  pciv_chnid Channel id.
* @param [in]  blk_size buffer count.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_pciv_chn_buf_free(vs_int32_t pciv_chnid, vs_uint32_t blk_cnt);


/**
* @brief  Create vb pool for DMA transmission(only used in slave).
* @details  Create vb pool for DMA transmission(only used in slave).
* @param [in]  p_cfg Pointer to vb pool configuration, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_pciv_win_vb_create(vs_pciv_win_cfg_s *p_cfg);

/**
* @brief  Destroy vb pool for DMA transmission(only used in slave).
* @details  Destroy vb pool for DMA transmission(only used in slave).
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_pciv_win_vb_destroy(vs_void_t);

/**
* @brief  Run DMA transmission task.
* @details  Run DMA transmission task.
* @param [in]  p_task Pointer to DMA task configuration, can not be NULL.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_pciv_dma_task_run(vs_pciv_dma_task_s *p_task);

/**
* @brief  Send message to opposite PCIE device.
* @details  Send message to opposite PCIE device.
* @param [in]  target_id Opposite device ID.
* @param [in]  port_id Opposite port ID.
* @param [in]  p_msg Pointer to message data, can not be NULL.
* @param [in]  len message length.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_pciv_msg_send(vs_uint32_t target_id, vs_uint32_t port_id, vs_void_t *p_msg, vs_uint32_t len);

/**
* @brief  Receive message from opposite PCIE device.
* @details  Receive message from opposite PCIE device.
* @param [in]  target_id Opposite device ID.
* @param [in]  port_id Opposite port ID.
* @param [out]  p_msg Pointer to message data, can not be NULL.
* @param [in]  len message length.
* @return   0 call success.
* @warning   none0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_pciv_msg_recv(vs_uint32_t target_id, vs_uint32_t port_id, vs_void_t *p_msg, vs_uint32_t len);

/**
* @brief  Return local PCIE chip ID.
* @details  Return local PCIE chip ID.
* @return   Greater than or equal to 0, local PCIE chip ID.
* @warning   Less than 0 call failed. see ERROR_CODE for detail.
* @par
*
*/
vs_int32_t vs_mal_pciv_local_id_get(vs_void_t);

#ifdef __cplusplus
}
#endif

#endif
