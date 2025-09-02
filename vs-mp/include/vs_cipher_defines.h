/**
* @file vs_cipher_defines.h
* @brief cipher struct\enum\macro definition includes
* @details cipher struct\enum\macro definition includes
* @author VS software group
* @date 2022-08-04
* @version v0.1
* @Copyright (C) 2022, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/
#ifndef __VS_CIPHER_DEFINES_H__
#define __VS_CIPHER_DEFINES_H__

#include "vs_type.h"
#include "vs_errno.h"
#include "vs_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

#define	CIPHER_MAX_BLOCK_SIZE	16
#define	CIPHER_MAX_KEY_SIZE	64
#define	CIPHER_MAX_TAG_SIZE	16
#define	CIPHER_MAX_CHN_NUM	32
#define CIPHER_MAX_HASH_CHN_NUM	32

#define VS_ERR_CIPHER_INVALID_PARAM	VS_ERR_CODE(E_MOD_ID_CIPHER, E_ERR_INVALID_PARAM)
#define VS_ERR_CIPHER_INVALID_DEVID	VS_ERR_CODE(E_MOD_ID_CIPHER, E_ERR_INVALID_DEVID)
#define VS_ERR_CIPHER_NULL_PTR	VS_ERR_CODE(E_MOD_ID_CIPHER, E_ERR_NULL_PTR)
#define VS_ERR_CIPHER_UNEXIST	VS_ERR_CODE(E_MOD_ID_CIPHER, E_ERR_UNEXIST)
#define VS_ERR_CIPHER_NOT_SUPPORT	VS_ERR_CODE(E_MOD_ID_CIPHER, E_ERR_NOT_SUPPORT)
#define VS_ERR_CIPHER_NOT_INITIALIZED VS_ERR_CODE(E_MOD_ID_CIPHER, E_ERR_NOT_INITIALIZED)
#define VS_ERR_CIPHER_NO_MEMORY	VS_ERR_CODE(E_MOD_ID_CIPHER, E_ERR_NO_MEMORY)
#define VS_ERR_CIPHER_ABORTED	VS_ERR_CODE(E_MOD_ID_CIPHER, E_ERR_ABORTED)
#define VS_ERR_CIPHER_DEVICE_UNEXIST	VS_ERR_CODE(E_MOD_ID_CIPHER, E_ERR_DEVICE_UNEXIST)
#define VS_ERR_CIPHER_INVALID_HANDLE	VS_ERR_CODE(E_MOD_ID_CIPHER, E_ERR_CIPHER_INVALID_HANDLE)
#define VS_ERR_CIPHER_GET_HANDLE_FAILED	VS_ERR_CODE(E_MOD_ID_CIPHER, E_ERR_CIPHER_GET_HANDLE_FAILED)
#define VS_ERR_CIPHER_RELEASE_HANDLE_FAILED	VS_ERR_CODE(E_MOD_ID_CIPHER, E_ERR_CIPHER_RELEASE_HANDLE_FAILED)
#define VS_ERR_CIPHER_ENCRYPT_FAILED	VS_ERR_CODE(E_MOD_ID_CIPHER, E_ERR_CIPHER_ENCRYPT_FAILED)
#define VS_ERR_CIPHER_DECRYPT_FAILED	VS_ERR_CODE(E_MOD_ID_CIPHER, E_ERR_CIPHER_DECRYPT_FAILED)
#define VS_ERR_CIPHER_BUSY	VS_ERR_CODE(E_MOD_ID_CIPHER, E_ERR_CIPHER_BUSY)
#define VS_ERR_CIPHER_NO_AVAILABLE_RNG	VS_ERR_CODE(E_MOD_ID_CIPHER, E_ERR_CIPHER_NO_AVAILABLE_RNG)
#define VS_ERR_CIPHER_MEM_ALLOC_FAILED	VS_ERR_CODE(E_MOD_ID_CIPHER, E_ERR_CIPHER_MEM_ALLOC_FAILED)
#define VS_ERR_CIPHER_UNAVAILABLE	VS_ERR_CODE(E_MOD_ID_CIPHER, E_ERR_CIPHER_UNAVAILABLE)
#define VS_ERR_CIPHER_OVERFLOW	VS_ERR_CODE(E_MOD_ID_CIPHER, E_ERR_CIPHER_OVERFLOW)
#define VS_ERR_CIPHER_ILLEGAL_KEY VS_ERR_CODE(E_MOD_ID_CIPHER, E_ERR_CIPHER_ILLEGAL_KEY)
#define VS_ERR_CIPHER_INVALID_ADDR VS_ERR_CODE(E_MOD_ID_CIPHER, E_ERR_CIPHER_INVALID_ADDR)
#define VS_ERR_CIPHER_INVALID_LENGTH VS_ERR_CODE(E_MOD_ID_CIPHER, E_ERR_CIPHER_INVALID_LENGTH)
#define VS_ERR_CIPHER_INVALID_DATA VS_ERR_CODE(E_MOD_ID_CIPHER, E_ERR_CIPHER_INVALID_DATA)
#define VS_ERR_CIPHER_RSA_SIGN_FAILED VS_ERR_CODE(E_MOD_ID_CIPHER, E_ERR_CIPHER_RSA_SIGN_FAILED)
#define VS_ERR_CIPHER_RSA_VERIFY_FAILED VS_ERR_CODE(E_MOD_ID_CIPHER, E_ERR_CIPHER_RSA_VERIFY_FAILED)
#define VS_ERR_CIPHER_RSA_CRYPT_FAILED VS_ERR_CODE(E_MOD_ID_CIPHER, E_ERR_CIPHER_RSA_CRYPT_FAILED)
#define VS_ERR_CIPHER_DEV_MEM_MAP_FAILED VS_ERR_CODE(E_MOD_ID_CIPHER, E_ERR_CIPHER_DEV_MEM_MAP_FAILED)

typedef enum vs_cipher_private_errno {
	E_ERR_CIPHER_INVALID_HANDLE = 64,	///< cipher invalid handle
	E_ERR_CIPHER_GET_HANDLE_FAILED,	///< cipher failed get handle
	E_ERR_CIPHER_RELEASE_HANDLE_FAILED,	///< cipher failed release handle
	E_ERR_CIPHER_ENCRYPT_FAILED,	///< cipher encrypt fail
	E_ERR_CIPHER_DECRYPT_FAILED,	///< cipher decrypt fail
	E_ERR_CIPHER_BUSY,	///< cipher busy
	E_ERR_CIPHER_NO_AVAILABLE_RNG,	///< cipher have no available rng
	E_ERR_CIPHER_MEM_ALLOC_FAILED,	///< cipher memory alloc fail
	E_ERR_CIPHER_UNAVAILABLE,	///< cipher unavailable
	E_ERR_CIPHER_OVERFLOW,	///< cipher overflow
	E_ERR_CIPHER_ILLEGAL_KEY,	///< cipher illegal key
	E_ERR_CIPHER_INVALID_ADDR,	///< cipher invalid addr
	E_ERR_CIPHER_INVALID_LENGTH,	///< cipher invalid length
	E_ERR_CIPHER_INVALID_DATA,	///< cipher invalid data
	E_ERR_CIPHER_RSA_SIGN_FAILED,	///< cipher sign fail
	E_ERR_CIPHER_RSA_VERIFY_FAILED,	///< cipher verify fail
	E_ERR_CIPHER_RSA_CRYPT_FAILED,	///< cipher crypt fail
	E_ERR_CIPHER_DEV_MEM_MAP_FAILED, ///< cipher mem map fail
	E_ERR_CIPHER_MAX
} vs_cipher_private_errno_e;

typedef enum vs_cipher_alg {
	E_CIPHER_ALG_DES = 0x0,
	E_CIPHER_ALG_3DES,
	E_CIPHER_ALG_AES,
	E_CIPHER_ALG_SM4,
	E_CIPHER_ALG_MAX
} vs_cipher_alg_e;

typedef enum vs_cipher_work_mode {
	E_CIPHER_WORK_MODE_ECB = 0x0,
	E_CIPHER_WORK_MODE_CBC,
	E_CIPHER_WORK_MODE_OFB,
	E_CIPHER_WORK_MODE_CTR,
	E_CIPHER_WORK_MODE_CCM,
	E_CIPHER_WORK_MODE_GCM,
	E_CIPHER_WORK_MODE_XTS,
	E_CIPHER_WORK_MODE_MAX
} vs_cipher_work_mode_e;

typedef enum vs_cipher_key_length {
	E_CIPHER_KEY_AES_128BIT = 0x0,
	E_CIPHER_KEY_AES_192BIT,
	E_CIPHER_KEY_AES_256BIT,
	E_CIPHER_KEY_3DES_3KEY,
	E_CIPHER_KEY_DES_KEY,
	E_CIPHER_KEY_SM4_KEY,
	E_CIPHER_KEY_MAX
} vs_cipher_key_length_e;

typedef enum vs_cipher_ca_type {
	E_CIPHER_KEY_SRC_USER = 0x0,
	E_CIPHER_KEY_SRC_MAX
} vs_cipher_ca_type_e;

typedef struct vs_cipher_ctrl {
	vs_uint8_t key[CIPHER_MAX_KEY_SIZE];
	vs_uint8_t iv[CIPHER_MAX_BLOCK_SIZE];
	vs_uint8_t tag[CIPHER_MAX_TAG_SIZE];
	vs_uint64_t assoc_phys_addr;
	vs_cipher_ca_type_e card_type;
	vs_cipher_alg_e alg;
	vs_cipher_work_mode_e work_mode;
	vs_cipher_key_length_e key_len;
	vs_uint32_t iv_len;
	vs_uint32_t assoc_len;
	vs_uint32_t tag_len;
	vs_void_t *p_para;
} vs_cipher_ctrl_s;

typedef struct vs_cipher_attr {
	vs_cipher_ctrl_s ctrl;
} vs_cipher_attr_s;

typedef struct vs_cipher_data {
	vs_uint64_t src_phys_addr;
	vs_uint64_t dst_phys_addr;
	vs_uint32_t len;
} vs_cipher_data_s;

typedef enum vs_cipher_hash_type {
	E_CIPHER_HASH_TYPE_SHA1 = 0,
	E_CIPHER_HASH_TYPE_SHA224,
	E_CIPHER_HASH_TYPE_SHA256,
	E_CIPHER_HASH_TYPE_SHA384,
	E_CIPHER_HASH_TYPE_SHA512,
	E_CIPHER_HASH_TYPE_SM3,
	E_CIPHER_HASH_TYPE_MD5,
	E_CIPHER_HASH_TYPE_HMAC_SHA1,
	E_CIPHER_HASH_TYPE_HMAC_SHA224,
	E_CIPHER_HASH_TYPE_HMAC_SHA256,
	E_CIPHER_HASH_TYPE_HMAC_SHA384,
	E_CIPHER_HASH_TYPE_HMAC_SHA512,
	E_CIPHER_HASH_TYPE_HMAC_SM3,
	E_CIPHER_HASH_TYPE_HMAC_MD5,
	E_CIPHER_HASH_TYPE_MAX
} vs_cipher_hash_type_e;

typedef struct vs_cipher_hash_attr {
	vs_uint8_t *p_key;
	vs_uint32_t key_len;
	vs_uint8_t *p_iv;
	vs_uint32_t iv_len;
	vs_cipher_hash_type_e type;
} vs_cipher_hash_attr_s;

typedef enum vs_cipher_rsa_sign_scheme {
	E_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA1 = 0,
	E_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA224,
	E_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA256,
	E_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA384,
	E_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V15_SHA512,
	E_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V21_SHA1,
	E_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V21_SHA224,
	E_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V21_SHA256,
	E_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V21_SHA384,
	E_CIPHER_RSA_SIGN_SCHEME_RSASSA_PKCS1_V21_SHA512,
	E_CIPHER_RSA_SIGN_SCHEME_MAX
} vs_cipher_rsa_sign_scheme_e;

typedef enum vs_cipher_padding_scheme {
	E_CIPHER_PADDING_SCHEME_NO_PADDING = 0,
	E_CIPHER_PADDING_SCHEME_PKCS1_V15,
	E_CIPHER_PADDING_SCHEME_PKCS1_V21,
	E_CIPHER_PADDING_SCHEME_MAX
} vs_cipher_padding_scheme_e;

typedef struct vs_cipher_rsa_attr {
	vs_uint8_t *p_key;
	vs_uint32_t key_len;
	vs_cipher_padding_scheme_e p_scheme;
} vs_cipher_rsa_attr_s;

typedef struct vs_cipher_rsa_sign_attr {
	vs_uint8_t *p_key;
	vs_uint32_t key_len;
	vs_cipher_rsa_sign_scheme_e scheme;
} vs_cipher_rsa_sign_attr_s;

#ifdef __cplusplus
}
#endif

#endif
