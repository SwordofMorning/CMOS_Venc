
/**
* @file vs_mal_ivs.h
* @brief Declaration of ive enumeration, struct and API.
* @details Declaration of ive enumeration, struct and API.
* @author VS software group
* @date 2024-09-24
* @version v0.01
* @copyright (C) 2024, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/

#ifndef __VS_MAL_IVS_H__
#define __VS_MAL_IVS_H__

/*****************************************************************************
 *                    Includes
 *****************************************************************************/
#include "vs_type.h"
#include "vs_errno.h"
#include "vs_ivs_defines.h"

#ifdef __cplusplus
extern "C"{
#endif


/*****************************************************************************
 *                    IVS API Prototypes
 *****************************************************************************/

/**
* @ingroup      vs_mal_ivs
* @brief        initialize md system
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ivs_md_init(vs_void_t);

/**
* @ingroup      vs_mal_ivs
* @brief        destroy all the resource allocated to md
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ivs_md_deinit(vs_void_t);

/**
* @ingroup      vs_mal_ivs
* @brief        create the specified channel of md
* @param [in]   md_chnid    the channel id of md
* @param [in]   p_attr      the attribute of md channel
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ivs_md_chn_create(vs_int32_t md_chnid, vs_ivs_md_attr_s *p_attr);

/**
* @ingroup      vs_mal_ivs
* @brief        destroy the specified channel of md
* @param [in]   md_chnid    the channel id of md
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ivs_md_chn_destroy(vs_int32_t md_chnid);

/**
* @ingroup      vs_mal_ivs
* @brief        set the attribute of the md
* @param [in]   md_chnid    the channel id of md
* @param [in]   p_attr      the attribute of md channel
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note         channel attribute(algo_mode, sad_mode, width and height) can not be modified after creating
* @par
*
*/
vs_int32_t vs_mal_ivs_md_chn_attr_set(vs_int32_t md_chnid, vs_ivs_md_attr_s *p_attr);

/**
* @ingroup      vs_mal_ivs
* @brief        query the attribute of the md
* @param [in]   md_chnid    the channel id of md
* @param [out]  p_attr      the attribute of md channel
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ivs_md_chn_attr_get(vs_int32_t md_chnid, vs_ivs_md_attr_s *p_attr);

/**
* @ingroup      vs_mal_ivs
* @brief        process the motion detection
* @details      Input image type              U8C1
*               Input image resolution range  64x64 - 3840x2160
* @param [in]   md_chnid    the channel id of md
* @param [in]   p_src       the input current frame
* @param [in]   p_ref       the input reference frame
* @param [out]  p_sad       the output sad image(optional), no sad output when NULL
* @param [out]  p_result    the result of the md(vs_ive_blob_s)
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ivs_md_process(vs_int32_t md_chnid, vs_ive_image_s *p_src, vs_ive_image_s *p_ref, vs_ive_image_s *p_sad, vs_ive_buffer_s *p_dst);


/**
* @ingroup      vs_mal_ivs
* @brief        initialize od system
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ivs_od_init(vs_void_t);

/**
* @ingroup      vs_mal_ivs
* @brief        destroy all the resource allocated to od
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ivs_od_deinit(vs_void_t);

/**
* @ingroup      vs_mal_ivs
* @brief        create the specified channel of od
* @param [in]   od_chnid    the channel id of od
* @param [in]   p_attr      the attribute of od channel
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ivs_od_chn_create(vs_int32_t od_chnid, vs_ivs_od_attr_s *p_attr);

/**
* @ingroup      vs_mal_ivs
* @brief        destroy the specified channel of od
* @param [in]   od_chnid    the channel id of od
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ivs_od_chn_destroy(vs_int32_t od_chnid);

/**
* @ingroup      vs_mal_ivs
* @brief        set the attribute of the od
* @param [in]   od_chnid    the channel id of od
* @param [in]   p_attr      the attribute of od channel
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note         channel attribute(width and height) can not be modified after creating
* @par
*
*/
vs_int32_t vs_mal_ivs_od_chn_attr_set(vs_int32_t od_chnid, vs_ivs_od_attr_s *p_attr);

/**
* @ingroup      vs_mal_ivs
* @brief        query the attribute of the od
* @param [in]   od_chnid    the channel id of od
* @param [out]  p_attr      the attribute of od channel
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ivs_od_chn_attr_get(vs_int32_t od_chnid, vs_ivs_od_attr_s *p_attr);

/**
* @ingroup      vs_mal_ivs
* @brief        process the occlusion detection
* @details      Input image type              U8C1
*               Input image resolution range  64x64 - 3840x2160
* @param [in]   od_chnid    the channel id of od
* @param [in]   p_src       the input current frame
* @param [in]   p_ref       the input reference frame(optional, can be NULL)
* @param [out]  p_dst       the result of the od (vs_ivs_od_info_s)
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ivs_od_process(vs_int32_t od_chnid, vs_ive_image_s *p_src, vs_ive_image_s *p_ref, vs_ive_buffer_s *p_dst);



#ifdef __cplusplus
}
#endif

#endif

