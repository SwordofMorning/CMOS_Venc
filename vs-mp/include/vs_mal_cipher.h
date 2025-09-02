/**
* @file vs_mal_cipher.h
* @brief cipher mal includes
* @details cipher mal includes
* @author VS software group
* @date 2022-08-04
* @version v0.1
* @Copyright (C) 2022, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/
#ifndef __VS_MAL_CIPHER_H__
#define __VS_MAL_CIPHER_H__

#include "vs_cipher_defines.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
* @ingroup      vs_mal_cipher_init
* @brief        cipher device init
* @details      This api is used to cipher init
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_cipher_init(vs_void_t);

/**
* @ingroup      vs_mal_cipher_deinit
* @brief        cipher device remove
* @details      This api is used to cipher deinit
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_cipher_deinit(vs_void_t);

/**
* @ingroup      vs_mal_cipher_handle_create
* @brief        symc cipher handle create
* @details      This api is used to cipher create
* @param [out]  p_handle        cipher handle
* @param [in]   p_attr          cipher attribute
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_cipher_handle_create(vs_uint32_t *p_handle,
									   const vs_cipher_attr_s *p_attr);

/**
* @ingroup      vs_mal_cipher_handle_destroy
* @brief        symc cipher handle destroy
* @details      This api is used to cipher destroy
* @param [in]   handle          cipher handle
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_cipher_handle_destroy(vs_uint32_t handle);

/**
* @ingroup      vs_mal_cipher_handle_config_get
* @brief        get cipher attribute
* @details      This api is used to get cipher attribute
* @param [in]   handle          cipher handle
* @param [out]  p_attr          cipher attribute
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_cipher_handle_config_get(vs_uint32_t handle,
										   vs_cipher_attr_s *p_attr);

/**
* @ingroup      vs_mal_cipher_encrypt
* @brief        cipher encrypt
* @details      This api is used to encrypt plaintext
* @param [in]   handle          cipher handle
* @param [in]   src_phys_addr   source phys addr for plaintext
* @param [out]  dst_phys_addr   destination phys addr for ciphertext
* @param [in]   size            cipher encrypt data size
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_cipher_encrypt(vs_uint32_t handle, vs_uint64_t src_phys_addr,
								 vs_uint64_t dst_phys_addr, vs_uint32_t size);

/**
* @ingroup      vs_mal_cipher_decrypt
* @brief        cipher decrypt
* @details      This api is used to decrypt ciphertext
* @param [in]   handle          cipher handle
* @param [in]   src_phys_addr   source phys addr for ciphertext
* @param [out]  dst_phys_addr   destination phys addr for plaintext
* @param [in]   size            cipher encrypt data size
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_cipher_decrypt(vs_uint32_t handle, vs_uint64_t src_phys_addr,
								 vs_uint64_t dst_phys_addr, vs_uint32_t size);

/**
* @ingroup      vs_mal_cipher_virt_encrypt
* @brief        cipher encrypt
* @details      This api is used to encrypt plaintext
* @param [in]   handle          cipher handle
* @param [in]   p_src_virt_addr source virt addr for plaintext
* @param [out]  p_dst_virt_addr destination virt addr for ciphertext
* @param [in]   size            cipher encrypt data size
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_cipher_virt_encrypt(vs_uint32_t handle,
									  const vs_uint8_t *p_src_virt_addr,
									  vs_uint8_t *p_dst_virt_addr,
									  vs_uint32_t size);

/**
* @ingroup      vs_mal_cipher_decrypt
* @brief        cipher decrypt
* @details      This api is used to decrypt ciphertext
* @param [in]   handle          cipher handle
* @param [in]   p_src_virt_addr source virt addr for ciphertext
* @param [out]  p_dst_virt_addr destination virt addr for plaintext
* @param [in]   size            cipher encrypt data size
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_cipher_virt_decrypt(vs_uint32_t handle,
									  const vs_uint8_t *p_src_virt_addr,
									  vs_uint8_t *p_dst_virt_addr,
									  vs_uint32_t size);

/**
* @ingroup      vs_mal_cipher_tag_get
* @brief        get cipher tag data
* @details      This api is used to get cipher tag data,only for ccm gcm
* @param [in]   handle          cipher handle
* @param [out]  p_tag           tag data for aead crypt
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_cipher_tag_get(vs_uint32_t handle, vs_uint8_t *p_tag);

/**
* @ingroup      vs_mal_cipher_multi_encrypt
* @brief        cipher encrypt for muti package
* @details
* @param [in]   handle          cipher handle
* @param [in]   p_pkg           cipher package information
* @param [in]   pkg_num         package number
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_cipher_multi_encrypt(vs_uint32_t handle,
									   const vs_cipher_data_s *p_pkg,
									   vs_uint32_t pkg_num);

/**
* @ingroup      vs_mal_cipher_multi_decrypt
* @brief        cipher decrypt for muti package
* @details
* @param [in]   handle          cipher handle
* @param [in]   p_pkg           cipher package information
* @param [in]   pkg_num         package number
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_cipher_multi_decrypt(vs_uint32_t handle,
									   const vs_cipher_data_s *p_pkg,
									   vs_uint32_t pkg_num);

/**
* @ingroup      vs_mal_hash_init
* @brief        cipher hash init
* @details      this api is used to init hash attribute
* @param [out]  handle          hash handle
* @param [in]   p_attr          hash attribute
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_hash_init(vs_uint32_t *p_handle,
							const vs_cipher_hash_attr_s *p_attr);

/**
* @ingroup      vs_mal_hash_update
* @brief        cipher hash update
* @details      this api is used to update input data to te-600
* @param [in]   handle          cipher handle
* @param [in]   p_input         input data
* @param [in]   input_len       input data len
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_hash_update(vs_uint32_t handle, const vs_uint8_t *p_input,
							  vs_uint32_t input_len);

/**
* @ingroup      vs_mal_cipher_finish
* @brief        cipher hash finish
* @details      this api is used to get hash data
* @param [in]   handle          cipher handle
* @param [out]  p_output_hash   output hash data
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_hash_finish(vs_uint32_t handle, vs_uint8_t *p_output_hash);

/**
* @ingroup      vs_mal_hash_digest
* @brief        cipher hash digest
* @details      this api is used to calc hash data from input data
* @param [in]   p_attr          hash attribute
* @param [in]   p_input         input data
* @param [in]   input_len       input data len
* @param [out]  p_output_hash   output hash data
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_hash_digest(const vs_cipher_hash_attr_s *p_attr,
							  const vs_uint8_t *p_input, vs_uint32_t input_len,
							  vs_uint8_t *p_output_hash);

/**
* @ingroup      vs_mal_rsa_pub_encrypt
* @brief        rsa public key encrypt
* @details
* @param [in]   p_attr          rsa attribute
* @param [in]   p_input         input data
* @param [in]   input_len       input data len
* @param [out]  p_output        output data
* @param [out]  output_len      output data len
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_rsa_pub_encrypt(const vs_cipher_rsa_attr_s *p_attr,
								  const vs_uint8_t *p_input, vs_uint32_t input_len,
								  vs_uint8_t *p_output,
								  vs_uint32_t *output_len);

/**
* @ingroup      vs_mal_rsa_priv_decrypt
* @brief        rsa private decrypt
* @details
* @param [in]   p_attr          rsa attribute
* @param [in]   p_input         input data
* @param [in]   input_len       input data len
* @param [out]  p_output        output data
* @param [out]  output_len      output data len
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_rsa_priv_decrypt(const vs_cipher_rsa_attr_s *p_attr,
								   const vs_uint8_t *p_input, vs_uint32_t input_len,
								   vs_uint8_t *p_output,
								   vs_uint32_t *output_len);

/**
* @ingroup      vs_mal_rsa_priv_encrypt
* @brief        rsa private key encrypt
* @details
* @param [in]   p_attr          rsa attribute
* @param [in]   p_input         input data
* @param [in]   input_len       input data len
* @param [out]  p_output        output data
* @param [out]  output_len      output data len
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_rsa_priv_encrypt(const vs_cipher_rsa_attr_s *p_attr,
								   const vs_uint8_t *p_input, vs_uint32_t input_len,
								   vs_uint8_t *p_output,
								   vs_uint32_t *output_len);

/**
* @ingroup      vs_mal_rsa_pub_encrypt
* @brief        rsa public key encrypt
* @details
* @param [in]   p_attr          rsa attribute
* @param [in]   p_input         input data
* @param [in]   input_len       input data len
* @param [out]  p_output        output data
* @param [out]  output_len      output data len
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_rsa_pub_decrypt(const vs_cipher_rsa_attr_s *p_attr,
								  const vs_uint8_t *p_input, vs_uint32_t input_len,
								  vs_uint8_t *p_output,
								  vs_uint32_t *output_len);

/**
* @ingroup      vs_mal_rsa_sign
* @brief        cipher rsa sign
* @details
* @param [in]   p_attr          rsa attribute
* @param [in]   p_in_data       input plaintext data
* @param [in]   in_data_len     input data len
* @param [in]   p_hash_data     input hash data
* @param [out]  p_out_sign      output sign data
* @param [out]  p_out_sign_len  output sign data len
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_rsa_sign(const vs_cipher_rsa_sign_attr_s *p_attr,
						   const vs_uint8_t *p_in_data, vs_uint32_t in_data_len,
						   const vs_uint8_t *p_hash_data, vs_uint8_t *p_out_sign,
						   vs_uint32_t *p_out_sign_len);

/**
* @ingroup      vs_mal_rsa_verify
* @brief        cipher rsa verify sign data
* @details
* @param [in]   p_attr          rsa attribute
* @param [in]   p_in_data       input plaintext data
* @param [in]   in_data_len     input data len
* @param [in]   p_hash_data     input hash data
* @param [in]   p_in_sign       input sign data
* @param [in]   p_out_sign_len  input sign data len
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_rsa_verify(const vs_cipher_rsa_sign_attr_s *p_attr,
							 const vs_uint8_t *p_in_data, vs_uint32_t in_data_len,
							 const vs_uint8_t *p_hash_data, const vs_uint8_t *p_in_sign,
							 vs_uint32_t in_sign_len);

/**
* @ingroup      vs_mal_cipher_random_num_get
* @brief        get hardware random number
* @details
* @param [out]  p_random_num    pointer to 4 byte random number
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_cipher_random_num_get(vs_uint32_t *p_random_num);

#ifdef __cplusplus
}
#endif

#endif
