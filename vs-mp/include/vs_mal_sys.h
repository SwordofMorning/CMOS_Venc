/**
* @file vs_mal_sys.h
* @brief Declaration of sys enumeration, struct and API.
* @details Declaration of sys enumeration, struct and API.
* @author VS software group
* @date 2021-11-02
* @version v0.01
* @copyright (C) 2021, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/
#ifndef __VS_MAL_SYS_H__
#define __VS_MAL_SYS_H__

#include "vs_type.h"
#include "vs_comm.h"
#include "vs_errno.h"
#include "vs_sys_defines.h"

#ifdef __cplusplus
extern "C"{
#endif

/*****************************************************************************
 *                    SYS MAL API Prototypes
 *****************************************************************************/

/**
* @ingroup      vs_mal_sys_init
* @brief        sys init
* @details      sys init
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_sys_init(vs_void_t);

/**
* @ingroup      vs_mal_sys_exit
* @brief        sys exit
* @details      sys exit
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_sys_exit(vs_void_t);

/**
* @ingroup      vs_mal_bind
* @brief        bind the producer with the consumer
* @details
* @param [in]   p_producer      the producer
* @param [in]   p_consumer      the consumer
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_sys_bind(const vs_chn_s *p_producer, const vs_chn_s *p_consumer);

/**
* @ingroup      vs_mal_bind
* @brief        unbind the producer with the consumer
* @details
* @param [in]   p_producer      the producer
* @param [in]   p_consumer      the consumer
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_sys_unbind(const vs_chn_s *p_producer, const vs_chn_s *p_consumer);

/**
* @ingroup      vs_mal_bind
* @brief        get the producer by the binded consumer
* @details
* @param [in]   p_consumer      the consumer
* @param [out]  p_producer      the producer
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_sys_producer_get(const vs_chn_s *p_consumer, vs_chn_s *p_producer);

/**
* @ingroup      vs_mal_bind
* @brief        get the consumer list by the binded producer
* @details
* @param [in]   p_producer      the producer
* @param [out]  p_consumer_list the list of consumer
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_sys_consumer_get(const vs_chn_s *p_producer,
        vs_sys_bind_chn_list_s *p_consumer_list);

/**
* @ingroup      vs_mal_sys_pts_get
* @brief        get mp current timestamp
* @details
* @param [out]  p_pts the current timestamp
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_sys_pts_get(vs_uint64_t *p_pts);

/**
* @ingroup      vs_mal_sys_chipid_get
* @brief        get CHIP ID
* @details
* @retval       chipid string
* @warning
* @see
* @note
* @par
*
*/
const vs_char_t *vs_mal_sys_chipid_get(vs_void_t);

/**
* @ingroup      vs_mal_sys_memconfig_set
* @brief        set mmz name for module/device
* @details
* @param [in]   p_chn      	module/device/channel
* @param [in]   p_mmz_name      mmz name
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_sys_memconfig_set(const vs_chn_s *p_chn, const vs_char_t *p_mmz_name);

/**
* @ingroup      vs_mal_sys_memconfig_get
* @brief        get mmz name for module/device
* @details
* @param [in]   p_chn      	module/device/channel
* @param [out]  p_mmz_name      mmz name
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_sys_memconfig_get(const vs_chn_s *p_chn, vs_char_t *p_mmz_name);

/**
* @ingroup	vs_mal_sys_uuid_get
* @brief	get unique ID
* @details
* @param [out]  p_uuid      	uuid
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_sys_uuid_get(vs_uuid_s *p_uuid);


#ifdef __cplusplus
}
#endif

#endif // __VS_MAL_SYS_H__
