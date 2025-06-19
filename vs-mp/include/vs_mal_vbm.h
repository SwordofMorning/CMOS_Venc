/**
* @file
* @brief Declaration of vbm mal api
* @details Declaration of vbm mal api
* @author VS software group
* @date 2022-01-23
* @version v1.0
* @Copyright (C) 2022, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/

#ifndef __VS_MAL_VBM_H__
#define __VS_MAL_VBM_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "vs_type.h"
#include "vs_comm_video.h"
#include "vs_vbm_defines.h"

/**
* @ingroup      vs_mal_vb_cfg_set
* @brief        set common pool attribute
* @details
* @param [in]   p_vb_cfg       common pool configuration
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_vb_cfg_set(const vs_vb_cfg_s *p_vb_cfg);

/**
* @ingroup      vs_mal_vb_cfg_get
* @brief        get common pool configuration
* @details
* @param [out]  p_vb_cfg       common pool configuration
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_vb_cfg_get(vs_vb_cfg_s *p_vb_cfg);

/**
* @ingroup      vs_mal_vb_init
* @brief        common pool init
* @details
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_vb_init(vs_void_t);

/**
* @ingroup      vs_mal_vb_exit
* @brief        common pool exit
* @details
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_vb_exit(vs_void_t);

/**
* @ingroup      vs_mal_vb_pool_create
* @brief        create user pool
* @details
* @param [in]   p_cfg           user pool configuration
* @retval       VB_POOL         pool id
* @warning
* @see
* @note
* @par
*
*/
VB_POOL vs_mal_vb_pool_create(vs_vb_pool_cfg_s *p_cfg);

/**
* @ingroup      vs_mal_vb_pool_destory
* @brief        destroy the specified pool
* @details
* @param [in]   pool        	pool ID
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_vb_pool_destory(VB_POOL pool);

/**
* @ingroup      vs_mal_vb_block_get
* @brief        get a free vb by pool or blk size
* @details
* @param [in]   pool        	pool ID
* @param [in]   blk_size        block size
* @param [in]   p_mmz_name      mmz name
* @retval       VB_BLK          vb block handle.
* @warning
* @see
* @note
* @par
*
*/
VB_BLK vs_mal_vb_block_get(VB_POOL pool, vs_uint64_t blk_size,
		const vs_char_t *p_mmz_name);

/**
* @ingroup      vs_mal_vb_block_release
* @brief        release vb block
* @details
* @param [in]   block        	vb block handle
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_vb_block_release(VB_BLK block);

/**
* @ingroup      vs_mal_modpool_cfg_set
* @brief        set module common pool attribute
* @details
* @param [in]   uid		vb user ID
* @param [in]   p_vb_cfg     	module common pool configuration
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_modpool_cfg_set(
		vs_vb_uid_e uid,
		const vs_vb_cfg_s *p_vb_cfg);

/**
* @ingroup      vs_mal_modpool_cfg_get
* @brief        get module common pool attribute
* @details
* @param [in]   uid		vb user ID
* @param [out]  p_vb_cfg        module common pool configuration
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_modpool_cfg_get(
		vs_vb_uid_e uid,
		vs_vb_cfg_s *p_vb_cfg);

/**
* @ingroup      vs_mal_vb_modpool_init
* @brief        module common pool init
* @details
* @param [in]   uid		vb user ID
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_vb_modpool_init(vs_vb_uid_e uid);

/**
* @ingroup      vs_mal_vb_modpool_exit
* @brief        module common pool exit
* @details
* @param [in]   uid		vb user ID
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_vb_modpool_exit(vs_vb_uid_e uid);

/**
* @ingroup      vs_mal_vb_supplement_cfg_set
* @brief        set suppliment configuration
* @details
* @param [in]   p_cfg		supplement configuration
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_vb_supplement_cfg_set(
	const vs_vb_supplement_cfg_s *p_cfg);

/**
* @ingroup      vs_mal_vb_supplement_cfg_get
* @brief        get suppliment configuration
* @details
* @param [out]  p_cfg		supplement configuration
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_vb_supplement_cfg_get(
		vs_vb_supplement_cfg_s *p_cfg);

/**
* @ingroup      vs_mal_vb_supplementaddr_get
* @brief        get suppliment address
* @details
* @param [in]   block		vb block handle
* @param [out]  p_suppaddr	supplement address
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_vb_supplementaddr_get(
		VB_BLK block,
		vs_frame_supplement_addr_s *p_suppaddr);

/**
* @ingroup      vs_mal_vb_physaddr2handle
* @brief        get vb block handle by phys addr
* @details
* @param [in]   phys_addr	vb block phys address
* @retval       VB_BLK      	vb block handle
* @warning
* @see
* @note
* @par
*
*/
VB_BLK vs_mal_vb_physaddr2handle(vs_uint64_t phys_addr);

/**
* @ingroup      vs_mal_vb_handle2physaddr
* @brief        get phys addr for vb block
* @details
* @param [in]   block		vb block handle
* @retval       addr		vb block phys address
* @warning
* @see
* @note
* @par
*
*/
vs_uint64_t vs_mal_vb_handle2physaddr(VB_BLK block);

/**
* @ingroup      vs_mal_vb_handle2poolid
* @brief        get pool ID for the vb block
* @details
* @param [in]   block		vb block handle
* @retval       VB_POOL		pool ID
* @warning
* @see
* @note
* @par
*
*/
VB_POOL vs_mal_vb_handle2poolid (VB_BLK block);

/**
* @ingroup      vs_mal_vb_usercnt_get
* @brief        get user count for the vb block
* @details
* @param [in]   block		vb block handle
* @retval       cnt		user count
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_vb_usercnt_get(VB_BLK block);

/**
* @ingroup      vs_mal_vb_pool_info_get
* @brief        get pool info
* @details
* @param [in]   pool        	pool id
* @param [out]  p_info        	pool info
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_vb_pool_info_get(
		VB_POOL pool, vs_vb_pool_info_s *p_info);


#ifdef __cplusplus
}
#endif


#endif
