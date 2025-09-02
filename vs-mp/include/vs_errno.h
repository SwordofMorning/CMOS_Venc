/**
* @file vs_errno.h
* @brief comm errno info
* @details comm errno info
* @author VS codec group
* @date 2021-06-30
* @version v0.10
* @Copyright (C) 2021, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/

#ifndef __VS_ERRNO_H__
#define __VS_ERRNO_H__

#include "vs_type.h"

#ifdef __cplusplus
extern "C" {
#endif

#define VS_ERR_APPID 0x80000000L

#define VS_ERR_CODE(module, errno) \
	((vs_int32_t)((VS_ERR_APPID) | (((module) & 0xff) << 16) | ((errno) & 0xff)))

typedef enum vs_errno {
	E_ERR_INVALID_PARAM	= 1, /* Invalid parameter */
	E_ERR_INVALID_DEVID	= 2, /* Invalid device ID */
	E_ERR_INVALID_CHNID	= 3, /* Invalid channel ID */
	E_ERR_INVALID_PIPEID	= 4, /* Invalid pipe ID */
	E_ERR_INVALID_GRPID	= 5, /* Invalid group ID */
	E_ERR_INVALID_LEN	= 6, /* Invalid length */
	E_ERR_BAD_STATE		= 7, /* Bad state machine */
	E_ERR_NULL_PTR		= 8, /* Unexpected Null pointer */
	E_ERR_EXIST		= 9, /* Resource already exists */
	E_ERR_UNEXIST		= 10, /* Resource unexists */
	E_ERR_NOT_SUPPORT	= 11, /* Operation not supported */
	E_ERR_NOT_IMPLEMENTED	= 12, /* Request not implemented */
	E_ERR_NOT_INITIALIZED	= 13, /* Target not initialized */
	E_ERR_NOT_PERM		= 14, /* Operation not permitted */
	E_ERR_NO_MEMORY		= 15, /* Out of memory */
	E_ERR_BUF_EMPTY		= 16, /* Buffer is empty */
	E_ERR_BUF_FULL		= 17, /* Buffer is full */
	E_ERR_BAD_ADDR		= 18, /* Bad address */
	E_ERR_BUSY		= 19, /* Device or resource busy */
	E_ERR_TIMEOUT		= 20, /* Operation timed out */
	E_ERR_ABORTED		= 21, /* Operation aborted */
	E_ERR_READ		= 22, /* Read operation failed */
	E_ERR_WRITE		= 23, /* Write operation failed */
	E_ERR_DEVICE_UNEXIST	= 24, /* Device unexists */
	E_ERR_MAX		= 63, /* 0~63 reserved for common error */
} vs_errno_e;


#ifdef __cplusplus
}
#endif

#endif /*!<  __VS_ERRNO_H__ */
