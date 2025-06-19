/**
* @file
* @brief vbm defines
* @details vbm defines
* @author VS software group
* @date 2022-05-16
* @version v1.0
* @Copyright (C) 2022, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/

#ifndef __VS_VBM_DEFINES_H__
#define __VS_VBM_DEFINES_H__

#ifdef __cplusplus
	extern "C" {
#endif

#include "vs_type.h"
#include "vs_errno.h"
#include "vs_comm.h"
#include "vs_comm_video.h"

#define VS_ERR_VBM_INVALID_PARAM     VS_ERR_CODE(E_MOD_ID_VB, E_ERR_INVALID_PARAM)
#define VS_ERR_VBM_NULL_PTR          VS_ERR_CODE(E_MOD_ID_VB, E_ERR_NULL_PTR)
#define VS_ERR_VBM_EXIST             VS_ERR_CODE(E_MOD_ID_VB, E_ERR_EXIST)
#define VS_ERR_VBM_NOT_SUPPORT       VS_ERR_CODE(E_MOD_ID_VB, E_ERR_NOT_SUPPORT)
#define VS_ERR_VBM_NO_MEMORY	     VS_ERR_CODE(E_MOD_ID_VB, E_ERR_NO_MEMORY)
#define VS_ERR_VBM_BUSY              VS_ERR_CODE(E_MOD_ID_VB, E_ERR_BUSY)
#define VS_ERR_VBM_DEVICE_UNEXIST    VS_ERR_CODE(E_MOD_ID_VB, E_ERR_DEVICE_UNEXIST)

typedef vs_uint32_t VB_POOL;
typedef vs_uint32_t VB_BLK;

#define VS_INVALID_VB_HANDLE	(-1U)
#define VS_INVALID_POOLID	(-1U)
#define VS_INVALID_VB_PHYSADDR (-1UL)

typedef enum vs_vb_remap_mode {
	VB_REMAP_MODE_NONE = 0,    ///< no remap
	VB_REMAP_MODE_NOCACHE = 1, ///< noncache remap
	VB_REMAP_MODE_CACHED = 2,  ///< cache remap
	VB_REMAP_MODE_MAX
} vs_vb_remap_mode_e;

#define VS_VBM_MMZ_MAX_NAME_LEN	32
typedef struct vs_vb_pool_cfg {
	vs_uint64_t blk_size;     ///< block size
	vs_uint32_t blk_cnt;      ///< block count
	vs_vb_remap_mode_e remap_mode;  ///< remap mode
	vs_char_t zone_name[VS_VBM_MMZ_MAX_NAME_LEN]; ///< zone name
} vs_vb_pool_cfg_s;

#define VS_VB_MAX_POOL_NUM		768
#define VS_VB_MAX_MODULE_POOL_NUM	16
#define VS_VB_MAX_COMM_POOL_NUM		16

typedef struct vs_vb_cfg {
    	vs_uint32_t pool_cnt;
    	vs_vb_pool_cfg_s ast_commpool[VS_VB_MAX_COMM_POOL_NUM];
} vs_vb_cfg_s;

#define VS_VB_SUPPLEMENT_ISPINFO_MASK	0x1
typedef struct vs_vb_supplement_cfg {
	vs_uint32_t supplement_cfg;
} vs_vb_supplement_cfg_s;

typedef enum vs_vb_uid {
	VS_VB_UID_VII = 0,
	VS_VB_UID_ISPFE,
	VS_VB_UID_ISPBE,
	VS_VB_UID_VPP,
	VS_VB_UID_VO,
	VS_VB_UID_VENC,
	VS_VB_UID_VDEC,
	VS_VB_UID_GDC,
	VS_VB_UID_GPE,
	VS_VB_UID_AUDIO,
	VS_VB_UID_AVS,
	VS_VB_UID_PCIV,
	VS_VB_UID_USER,
	VS_VB_UID_FB,
	VS_VB_UID_IVE,
	VS_VB_UID_MAX,
} vs_vb_uid_e;

typedef enum vs_pool_type {
	VB_SOURCE_COMMON  = 0,
	VB_SOURCE_MODULE  = 1,
	VB_SOURCE_PRIVATE = 2,
	VB_SOURCE_USER = 3,
	VB_SOURCE_MAX
} vs_pool_type_e;

typedef struct vs_frame_supplement_addr {
    vs_uint64_t   isp_info_phys_addr;
} vs_frame_supplement_addr_s;

typedef struct vs_vb_pool_info {
	vs_uint32_t blk_cnt;
	vs_uint64_t blk_size;
	vs_uint64_t pool_size;
	vs_uint64_t pool_phys_addr;
} vs_vb_pool_info_s;

#ifdef __cplusplus
}
#endif


#endif
