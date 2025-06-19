/**
* @file
* @brief Declaration of mmz mal api
* @details Declaration of mmz mal api
* @author VS software group
* @date 2021-10-20
* @version v1.0
* @Copyright (C) 2021, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/

#ifndef __VS_MAL_MMZ_H__
#define __VS_MAL_MMZ_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "vs_type.h"
#include "vs_mmz_defines.h"

/*****************************************************************************
 *                    MMZ MAL API Prototypes
 *****************************************************************************/
/**
* @ingroup      vs_mal_mmz_alloc
* @brief        alloc mmb from mmz
* @details
* @param [in]   p_zone_name     mmz zone name
* @param [in]   p_mmb_name      mmb name to be allocated
* @param [in]   len             mmb size to be allocated
* @param [out]  p_phys_addr     mmb physical address
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_mmz_alloc(const vs_char_t *p_zone_name,
		const vs_char_t *p_mmb_name, vs_uint32_t len,
		vs_uint64_t *p_phys_addr);

/**
* @ingroup      vs_mal_mmz_free
* @brief        free the mmb buffer
* @details
* @param [in]   phys_addr       mmb physical address
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_mmz_free(vs_uint64_t phys_addr);

/**
* @ingroup      vs_mal_sys_mmap_cached
* @brief        create the mapping for the specified address range
* @details      it's cached
* @param [in]   phys_addr       physical address
* @param [in]   size            memory size to be used
* @retval       mapped addr     virtual address
* @retval       NULL            error happens.
* @warning
* @see
* @note
* @par
*
*/
vs_void_t *vs_mal_sys_mmap_cached(vs_uint64_t phys_addr, vs_uint32_t size);

/**
* @ingroup      vs_mal_sys_mmap
* @brief        create the mapping for the specified address range
* @details      it's nocache
* @param [in]   phys_addr       physical address
* @param [in]   size            memory size to be used
* @retval       mapped addr     virtual address
* @retval       NULL            error happens.
* @warning
* @see
* @note
* @par
*
*/
vs_void_t *vs_mal_sys_mmap(vs_uint64_t phys_addr, vs_uint32_t size);

/**
* @ingroup      vs_mal_sys_unmap
* @brief        delete the mapping for the specified address range
* @details
* @param [in]   virt_addr       mapped address
* @param [in]   size            memory size to be used
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_sys_unmap(vs_void_t *virt_addr, vs_uint32_t size);

/**
* @ingroup      vs_mal_sys_cache_flush
* @brief        flush cache for the specified address range
* @details
* @param [in]   phys_addr       physical address
* @param [in]   virt_addr       mapped address
* @param [in]   size            memory size to be used
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_sys_cache_flush(
		vs_uint64_t phys_addr, vs_void_t *virt_addr,
		vs_uint32_t size);

/**
* @ingroup      vs_mal_sys_cache_invalidate
* @brief        invalidate cache for the specified address range
* @details
* @param [in]   phys_addr       physical address
* @param [in]   virt_addr       mapped address
* @param [in]   size            memory size to be used
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_sys_cache_invalidate(
		vs_uint64_t phys_addr, vs_void_t *virt_addr,
		vs_uint32_t size);


#ifdef __cplusplus
}
#endif


#endif
