
/**
* @file vs_mal_ive.h
* @brief Declaration of ive enumeration, struct and API.
* @details Declaration of ive enumeration, struct and API.
* @author VS software group
* @date 2021-11-02
* @version v0.01
* @copyright (C) 2021, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/

#ifndef __VS_MAL_IVE_H__
#define __VS_MAL_IVE_H__

/*****************************************************************************
 *                    Includes
 *****************************************************************************/
#include "vs_type.h"
#include "vs_comm.h"
#include "vs_comm_video.h"
#include "vs_errno.h"
#include "vs_ive_defines.h"

#ifdef __cplusplus
extern "C"{
#endif


/*****************************************************************************
 *                    IVE API Prototypes
 *****************************************************************************/
/**
* @ingroup      vs_mal_ive
* @brief        Direct memory access (DMA)
* @details      Input image type              IVE_2D_DATA
*               Input image resolution range  32x1-8192x8192
*               Output image type             IVE_2D_DATA
*               Output image resolution range 32x1-8192x8192
* @param [out]  p_handle    the returned task ID used for task status query
* @param [in]   p_src       the input source data address,1bytes alignment
* @param [out]  p_dst       the output destined data address,1bytes alignment
* @param [in]   p_cfg       the config parameters of DMA
* @param [in]   block       the flag indicating whether to block the task
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ive_dma(vs_uint32_t *p_handle, vs_ive_data_s  *p_src, vs_ive_data_s  *p_dst,
                            vs_ive_dma_cfg_s *p_cfg, vs_bool_t block);


/**
* @ingroup      vs_mal_ive
* @brief        5x5 matrix filter
* @details      Input image type              U8C1,YUV420SP,yuv422SP
*               Input image resolution range  64x64-1920x1080
*               Output image type             U8C1,YUV420SP,yuv422SP
*               Output image resolution range 64x64-1920x1080
* @param [out]  p_handle    the returned task ID used for task status query
* @param [in]   p_src       the input source data address,16bytes alignment
* @param [out]  p_dst       the output destined data address,16bytes alignment
* @param [in]   p_cfg       the config parameters of FILTER
* @param [in]   block       the flag indicating whether to block the task
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ive_filter(vs_uint32_t *p_handle, vs_ive_image_s  *p_src, vs_ive_image_s  *p_dst,
                                vs_ive_filter_cfg_s *p_cfg, vs_bool_t block);


/**
* @ingroup      vs_mal_ive
* @brief        coloring the image
* @details      Input image type              U8C1,YUV420SP,yuv422SP
*               Input image resolution range  64x64-1920x1080
*               Output image type             U8C1,YUV420SP,yuv422SP
*               Output image resolution range 64x64-1920x1080
* @param [out]  p_handle        the returned task ID used for task status query
* @param [in]   p_src           the input source data address,16bytes alignment
* @param [out]  p_dst           the output destined data address,16bytes alignment
* @param [in]   p_cfg           the config parameters of COLOR
* @param [in]   p_color_table   the color table of COLOR
* @param [in]   block           the flag indicating whether to block the task
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ive_color(vs_uint32_t *p_handle, vs_ive_image_s  *p_src, vs_ive_image_s  *p_dst,
	vs_ive_color_cfg_s *p_cfg, vs_ive_buffer_s *p_color_table, vs_bool_t block);

/**
* @ingroup      vs_mal_ive
* @brief        5x5 matrix filter and color
* @details      Input image type              U8C1,YUV420SP,yuv422SP
*               Input image resolution range  64x64-1920x1080
*               Output image type             U8C1,YUV420SP,yuv422SP
*               Output image resolution range 64x64-1920x1080
* @param [out]  p_handle        the returned task ID used for task status query
* @param [in]   p_src           the input source data address,16bytes alignment
* @param [out]  p_dst           the output destined data address,16bytes alignment
* @param [in]   p_cfg           the config parameters of FILTER
* @param [in]   p_color_table   the color table of COLOR
* @param [in]   block           the flag indicating whether to block the task
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ive_filter_color(vs_uint32_t *p_handle, vs_ive_image_s  *p_src, vs_ive_image_s  *p_dst,
	vs_ive_filter_color_cfg_s *p_cfg, vs_ive_buffer_s *p_color_table, vs_bool_t block);

/**
* @ingroup      vs_mal_ive
* @brief        RGB to YUV420sp/YUV422sp conversion,YUV420sp/YUV422sp to RGB/HSV conversion
* @details      Input image type              U8C3_PACKAGE,YUV420SP,yuv422SP
*               Input image resolution range  64x64-4096x4096
*               Output image type             U8C3_PACKAGE,YUV420SP,yuv422SP
*               Output image resolution range 64x64-4096x4096
* @param [out]  p_handle    the returned task ID used for task status query
* @param [in]   p_src       the input source data address,16bytes alignment
* @param [out]  p_dst       the output destined data address,16bytes alignment
* @param [in]   mode        the operation mode of CSC
* @param [in]   block       the flag indicating whether to block the task
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ive_csc(vs_uint32_t *p_handle, vs_ive_image_s  *p_src, vs_ive_image_s  *p_dst,
                             vs_ive_csc_mode_e mode, vs_bool_t block);



/**
* @ingroup      vs_mal_ive
* @brief        Image threshold operation
* @details      Input image type              U8C1
*               Input image resolution range  64x64-4096x4096
*               Output image type             U8C1
*               Output image resolution range 64x64-4096x4096
* @param [out]  p_handle    the returned task ID used for task status query
* @param [in]   p_src       the input source data address,16bytes alignment
* @param [out]  p_dst       the output destined data address,16bytes alignment
* @param [in]   p_cfg       the config parameters of THRESH
* @param [in]   block       the flag indicating whether to block the task
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ive_threshold(vs_uint32_t *p_handle, vs_ive_image_s  *p_src, vs_ive_image_s  *p_dst,
                                vs_ive_thr_cfg_s *p_cfg, vs_bool_t block);


/**
* @ingroup      vs_mal_ive
* @brief        Add operation of two images with x weight and y weight
* @details      Input image type              U8C1
*               Input image resolution range  64x64-4096x4096
*               Output image type:             U8C1
*               Output image resolution range 64x64-4096x4096
* @param [out]  p_handle    the returned task ID used for task status query
* @param [in]   p_src1      the input source1 data address,16bytes alignment
* @param [in]   p_src2      the input source2 data address,16bytes alignment
* @param [out]  p_dst       the output destined data address,16bytes alignment
* @param [in]   p_cfg       the config parameters of ADD
* @param [in]   block       the flag indicating whether to block the task
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ive_add(vs_uint32_t *p_handle, vs_ive_image_s  *p_src1, vs_ive_image_s  *p_src2,
                             vs_ive_image_s  *p_dst, vs_ive_add_cfg_s *p_cfg, vs_bool_t block);


/**
* @ingroup      vs_mal_ive
* @brief        Source1 image minus source2 image for each pixel
* @details      Input image type              U8C1
*               Input image resolution range  64x64-4096x4096
*               Output image type:             U8C1,S8C1
*               Output image resolution range 64x64-4096x4096
* @param [out]  p_handle    the returned task ID used for task status query
* @param [in]   p_src1      the input source1 data address,16bytes alignment
* @param [in]   p_src2      the input source2 data address,16bytes alignment
* @param [out]  p_dst       the output destined data address,16bytes alignment
* @param [in]   mode        the operation mode of SUB
* @param [in]   block       the flag indicating whether to block the task
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ive_sub(vs_uint32_t *p_handle, vs_ive_image_s  *p_src1, vs_ive_image_s  *p_src2,
                             vs_ive_image_s  *p_dst, vs_ive_sub_mode_e mode, vs_bool_t block);

/**
* @ingroup      vs_mal_ive
* @brief        Edit image according to map table.Map table has 256 levels, 16bits per level.Size of map table is 512bytes
* @details      Input image type:              U8C1
*               Input image resolution range:  64x64-4096x4096
*               Output image type              U8C1,U16C1,S16C1
*               Output image resolution range: 64x64-4096x4096
* @param [out]  p_handle    the returned task ID used for task status query
* @param [in]   p_src       the input source data address,16bytes alignment
* @param [in]   p_map_table the map table indicating how to map from src to dst,16bytes alignment
* @param [out]  p_dst       the output destined data address,16bytes alignment
* @param [in]   block       the flag indicating whether to block the task
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ive_map(vs_uint32_t *p_handle, vs_ive_image_s  *p_src, vs_ive_buffer_s  *p_map_table,
                            vs_ive_image_s  *p_dst, vs_bool_t block);

/**
* @ingroup      vs_mal_ive
* @brief        Scale the image and Supports zooming in and out of multiple images
* @details      Input image type              U8C1,YUV420SP,yuv422SP
*               Input image resolution range  32x12-1920x1080
*               Output image type             U8C1,YUV420SP,yuv422SP
*               Output image resolution range 32x12-1920x1080
* @param [out]  p_handle    the returned task ID used for task status query
* @param [in]   src         the input source data address,16bytes alignment
* @param [out]  dst         the output destined data address,16bytes alignment
* @param [in]   p_cfg       the config parameters of RESIZE
* @param [in]   block       the flag indicating whether to block the task
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ive_resize(vs_uint32_t *p_handle, vs_ive_image_s  src[], vs_ive_image_s  dst[],
                                vs_ive_resize_cfg_s *p_cfg, vs_bool_t block);


/**
* @ingroup      vs_mal_ive
* @brief        Histogram statistics
* @details      Input image type              U8C1
*               Input image resolution range  64x64-4096x4096
*               Output data type              IVE_1D_DATA
*               Output data size              256x4=1024bytes
* @param [out]  p_handle    the returned task ID used for task status query
* @param [in]   p_src       the input source data address,16bytes alignment
* @param [out]  p_dst       the output destined data address,16bytes alignment
* @param [in]   block       the flag indicating whether to block the task
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ive_hist(vs_uint32_t *p_handle, vs_ive_image_s  *p_src,
                                vs_ive_buffer_s  *p_dst, vs_bool_t block);

/**
* @ingroup      vs_mal_ive
* @brief        And operation of two binary images for each pixel
* @details      Input image type              U8C1
*               Input image resolution range  64x64-4096x4096
*               Output image type             U8C1
*               Output image resolution range 64x64-4096x4096
* @param [out]  p_handle    the returned task ID used for task status query
* @param [in]   p_src1      the input source data address,16bytes alignment
* @param [in]   p_src2      the input source data address,16bytes alignment
* @param [out]  p_dst       the output destined data address,16bytes alignment
* @param [in]   block       the flag indicating whether to block the task
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ive_and(vs_uint32_t *p_handle, vs_ive_image_s  *p_src1, vs_ive_image_s  *p_src2,
                             vs_ive_image_s  *p_dst, vs_bool_t block);


/**
* @ingroup      vs_mal_ive
* @brief        Or operation of two binary images for each pixel
* @details      Input image type              U8C1
*               Input image resolution range  64x64-4096x4096
*               Output image type             U8C1
*               Output image resolution range 64x64-4096x4096
* @param [out]  p_handle    the returned task ID used for task status query
* @param [in]   p_src1      the input source data address,16bytes alignment
* @param [in]   p_src2      the input source data address,16bytes alignment
* @param [out]  p_dst       the output destined data address,16bytes alignment
* @param [in]   block       the flag indicating whether to block the task
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ive_or(vs_uint32_t *p_handle, vs_ive_image_s  *p_src1, vs_ive_image_s  *p_src2,
                         vs_ive_image_s  *p_dst, vs_bool_t block);

/**
* @ingroup      vs_mal_ive
* @brief        Xor operation of two binary images for each pixel
* @details      Input image type              U8C1
*               Input image resolution range  64x64-4096x4096
*               Output image type             U8C1
*               Output image resolution range 64x64-4096x4096
* @param [out]  p_handle    the returned task ID used for task status query
* @param [in]   p_src1      the input source data address,16bytes alignment
* @param [in]   p_src2      the input source data address,16bytes alignment
* @param [out]  p_dst       the output destined data address,16bytes alignment
* @param [in]   block       the flag indicating whether to block the task
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ive_xor(vs_uint32_t *p_handle, vs_ive_image_s  *p_src1, vs_ive_image_s  *p_src2,
                            vs_ive_image_s  *p_dst, vs_bool_t block);


/**
* @ingroup      vs_mal_ive
* @brief        Calculate gradient amplitude using 5x5 template and angle
* @details      p_src image type              U8C1
*               p_src image resolution range  64x64-1920x1080
*               p_mag image type              U16C1
*               p_mag image resolution range  64x64-1920x1080
*               p_ang image type              U8C1
*               p_ang image resolution range  64x64-1920x1080
* @param [out]  p_handle    the returned task ID used for task status query
* @param [in]   p_src       the input source data address,16bytes alignment
* @param [out]  p_mag       the output mag data address,16bytes alignment
* @param [out]  p_ang       the output ang data address,16bytes alignment
* @param [in]   p_cfg       the config parameters of MAG_ANG
* @param [in]   block       the flag indicating whether to block the task
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ive_mag_ang(vs_uint32_t *p_handle, vs_ive_image_s  *p_src, vs_ive_image_s  *p_mag,
                             vs_ive_image_s  *p_ang, vs_ive_mag_ang_cfg_s *p_cfg, vs_bool_t block);

/**
* @ingroup      vs_mal_ive
* @brief        A linear conversion from 16bit image data to 8bit image data
* @details      Input image type              U16C1,S16C1
*               Input image resolution range  16X16-1920x1080
*               Output image type             U8C1,S8C1
*               Output image resolution range 16x16-1920x1080
* @param [out]  p_handle    the returned task ID used for task status query
* @param [in]   p_src       the input source data address,16bytes alignment
* @param [out]  p_dst       the output destined data address,16bytes alignment
* @param [in]   p_cfg       the config parameters of 16bit_to_8bit
* @param [in]   block       the flag indicating whether to block the task
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ive_16bit_to_8bit(vs_uint32_t *p_handle, vs_ive_image_s  *p_src, vs_ive_image_s  *p_dst,
                                    vs_ive_16bit_to_8bit_cfg_s  *p_cfg, vs_bool_t block);

/**
* @ingroup      vs_mal_ive
* @brief        Calculate histogram equalization of gray image.
* @details      Input image type              U8C1
*               Input image resolution range  64x64-1920x1080
*               Output image type             U8C1
*               Output image resolution range 64x64-1920x1080
* @param [out]  p_handle    the returned task ID used for task status query
* @param [in]   p_src       the input source data address,16bytes alignment
* @param [out]  p_dst       the output destined data address,16bytes alignment
* @param [in]   block       the flag indicating whether to block the task
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ive_equalize_hist(vs_uint32_t *p_handle, vs_ive_image_s  *p_src, vs_ive_image_s  *p_dst, vs_bool_t block);

/**
* @ingroup      vs_mal_ive
* @brief        Separate foreground from background using GMM, and support grayscale image and RGB package image
* @details      p_src image type              U8C1,U8C3_PACKAGE
*               p_src image resolution range  64x64-640x480
*               p_fg image type               U8C1
*               p_fg image resolution range   64x64-640x480
*               p_bg image type               U8C1,U8C3_PACKAGE
*               p_bg image resolution range   64x64-640x480
* @param [out]  p_handle   the returned task ID used for task status query
* @param [in]   p_src      the input source data address,16bytes alignment
* @param [out]  p_fg       the output data address,16bytes alignment
* @param [out]  p_bg       the output data address,16bytes alignment
* @param [out]  p_model    the GMM reference model
* @param [in]   p_cfg      the config parameters of GMM
* @param [in]   block      the flag indicating whether to block the task
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ive_gmm(vs_uint32_t *p_handle, vs_ive_image_s  *p_src, vs_ive_image_s  *p_fg, vs_ive_image_s  *p_bg,
                          vs_ive_buffer_s  *p_model, vs_ive_gmm_cfg_s *p_cfg, vs_bool_t block);

/**
* @ingroup      vs_mal_ive
* @brief        Separate foreground from background using GMM2, and support RGB package image
* @details      p_src image type              U8C3_PACKAGE
*               p_src image resolution range  64x64-1280x720
*               p_fg image type               U8C1
*               p_fg image resolution range   64x64-1280x720
*               p_bg image type               U8C3_PACKAGE
*               p_bg image resolution range   64x64-1280x720
*               p_match_model_info image type               U8C1
*               p_match_model_info image resolution range   64x64-1280x720
* @param [out]  p_handle             the returned task ID used for task status query
* @param [in]   p_src                the input source data address,16bytes alignment
* @param [out]  p_fg                 the output data address,16bytes alignment
* @param [out]  p_bg                 the output data address,16bytes alignment
* @param [out]  p_match_model_info   the output data address,16bytes alignment
* @param [out]  p_model              the GMM2 reference model
* @param [in]   p_cfg                the config parameters of GMM2
* @param [in]   block                the flag indicating whether to block the task
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ive_gmm2(vs_uint32_t *p_handle, vs_ive_image_s  *p_src, vs_ive_image_s  *p_fg, vs_ive_image_s  *p_bg,
                          vs_ive_image_s  *p_match_model_info, vs_ive_buffer_s  *p_model, vs_ive_gmm2_cfg_s *p_cfg, vs_bool_t block);



/**
* @ingroup      vs_mal_ive
* @brief        Calculate Sobel-like gradient using 5x5 template
* @details      p_src image type              U8C1
*               p_src image resolution range  8x8-1920x1080
*               p_dst_h image type             S16C1
*               p_dst_h image resolution range 8x8-1920x1080
*               p_dst_v image type             S16C1
*               p_dst_v image resolution range 8x8-1920x1080
* @param [out]  p_handle    the returned task ID used for task status query
* @param [in]   p_src       input image,16bytes alignment
* @param [out]  p_dst_h     Hor-gradient image,16bytes alignment
* @param [out]  p_dst_v     Ver-gradient image,16bytes alignment
* @param [in]   p_cfg       the config parameters of SOBEL
* @param [in]   block       the flag indicating whether to block the task
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ive_sobel(vs_uint32_t *p_handle, vs_ive_image_s *p_src,
        vs_ive_image_s *p_dst_h, vs_ive_image_s *p_dst_v,
        vs_ive_sobel_cfg_s *p_cfg, vs_bool_t block);

/**
* @ingroup      vs_mal_ive
* @brief        Dilate binary image using 5x5 template
* @details      Input image type              U8C1
*               Input image resolution range  64x64-1920x1080
*               Output image type             U8C1
*               Output image resolution range 64x64-1920x1080
* @param [out]  p_handle    the returned task ID used for task status query
* @param [in]   p_src       input binary image,16bytes alignment
* @param [out]  p_dst       output dilate image,16bytes alignment
* @param [in]   p_cfg       the config parameters of DILATE
* @param [in]   block       the flag indicating whether to block the task
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ive_dilate(vs_uint32_t *p_handle, vs_ive_image_s *p_src,
    vs_ive_image_s *p_dst, vs_ive_filter_coef_s *p_cfg,vs_bool_t block);


/**
* @ingroup      vs_mal_ive
* @brief        Erode binary image using 5x5 template
* @details      Input image type              U8C1
*               Input image resolution range  64x64-1920x1080
*               Output image type             U8C1
*               Output image resolution range 64x64-1920x1080
* @param [out]  p_handle    the returned task ID used for task status query
* @param [in]   p_src       input image,16bytes alignment
* @param [out]  p_dst       output erode image,16bytes alignment
* @param [in]   p_cfg       the config parameters of ERODE
* @param [in]   block       the flag indicating whether to block the task
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ive_erode(vs_uint32_t *p_handle, vs_ive_image_s *p_src,
    vs_ive_image_s *p_dst, vs_ive_filter_coef_s *p_cfg,vs_bool_t block);


/**
* @ingroup      vs_mal_ive
* @brief        Calculate integral of gray image
* @details      Input image type              U8C1
*               Input image resolution range  32x16-1920x1080
*               Output image type             U32C1,U64C1,U96C1
*               Output image resolution range 32x16-1920x1080
* @param [out]  p_handle    the returned task ID used for task status query
* @param [in]   p_src       input image,16bytes alignment
* @param [out]  p_dst       output integ image,16bytes alignment
* @param [in]   mode        integ mode
* @param [in]   block       the flag indicating whether to block the task
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ive_integ(vs_uint32_t *p_handle, vs_ive_image_s *p_src,
	vs_ive_image_s *p_dst, vs_ive_integ_out_mode_e mode, vs_bool_t block);

/**
* @ingroup      vs_mal_ive
* @brief        Connected area statistics for binary image
* @details      p_src image type              U8C1
*               p_src image resolution range  16x16-1280x720
* @param [out]  p_handle    the returned task ID used for task status query
* @param [in]   p_src       input image,16bytes alignment
* @param [out]  p_blob      connected area information,255 blobs at most
* @param [in]   p_cfg       the config parameters of CCL
* @param [in]   block       the flag indicating whether to block the task
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ive_ccl(vs_uint32_t *p_handle, vs_ive_image_s *p_src,
	 vs_ive_buffer_s *p_blob, vs_ive_ccl_cfg_s *p_cfg, vs_bool_t block);


/**
* @ingroup      vs_mal_ive
* @brief        Calculate canny edge of grey image
* @details      p_src image type              U8C1
*               p_src image resolution range  64x64-1920x1080
*               p_edge image type             U8C1
*               p_edge image resolution range 64x64-1920x1080
* @param [out]  p_handle    the returned task ID used for task status query
* @param [in]   p_src       input image,16bytes alignment
* @param [out]  p_edge      edge binary image,16bytes alignment
* @param [in]   p_cfg       the config parameters of CANNY
* @param [in]   block       the flag indicating whether to block the task
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ive_canny(vs_uint32_t *p_handle, vs_ive_image_s *p_src, vs_ive_image_s *p_edge,
                            vs_ive_canny_cfg_s *p_cfg, vs_bool_t block);




/**
* @ingroup      vs_mal_ive
* @brief        Calculate LBP of grey image
* @details      p_src image type              U8C1
*               p_src image resolution range  64x64-1920x1080
*               p_dst image type              U8C1
*               p_dst image resolution range  64x64-1920x1080
* @param [out]  p_handle    the returned task ID used for task status query
* @param [in]   p_src       input image,16bytes alignment
* @param [out]  p_dst       output image,16bytes alignment
* @param [in]   p_cfg       the config parameters of LBP
* @param [in]   block       the flag indicating whether to block the task
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ive_lbp(vs_uint32_t *p_handle, vs_ive_image_s *p_src,
	vs_ive_image_s *p_dst, vs_ive_lbp_cfg_s *p_cfg, vs_bool_t block);



/**
* @ingroup      vs_mal_ive
* @brief        Compute affine transformation matrix
* @details
* @param [out]  p_handle     the returned task ID used for task status query
* @param [in]   p_point_info input point pair information
* @param [out]  p_matrix     output affine matrix
* @param [in]   block        the flag indicating whether to block the task
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ive_affine_transform_get(vs_uint32_t *p_handle, vs_ive_affine_point_info_s *p_point_info,
    vs_ive_buffer_s *p_matrix, vs_bool_t block);

/**
* @ingroup      vs_mal_ive
* @brief        Affine transformation of each ROI in source image
* @details      p_src image type              U8C1
*               p_src image resolution range  20x20-1920x1080
*               p_dst image type              U8C1
*               p_dst image resolution range  20x20-256x256
* @param [out]  p_handle    the returned task ID used for task status query
* @param [in]   p_src       input image,16bytes alignment
* @param [in]   p_roi_info  input region,region size is [20-1024]
* @param [in]   matrix      input matrix
* @param [out]  p_dst       output image,16bytes alignment
* @param [in]   block       the flag indicating whether to block the task
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ive_affine(vs_uint32_t *p_handle, vs_ive_image_s  *p_src, vs_ive_affine_roi_info_s *p_roi_info,
    vs_float_t matrix[], vs_ive_image_s dst[], vs_bool_t block);


/**
* @ingroup      vs_mal_ive
* @brief        cosine distant of two vector
* @details
* @param [out]  p_handle    the returned task ID used for task status query
* @param [in]   p_src       input left and right matrix
* @param [in]   p_index     vector index of left matrix and right matrix
* @param [in]   p_cfg       the config parameters of cos_dist
* @param [out]  p_dst       cosine distant result and top n result
* @param [in]   block       the flag indicating whether to block the task
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ive_cos_dist(vs_uint32_t *p_handle,vs_ive_double_matrix_s* p_src,vs_ive_double_matrix_s* p_index,
    vs_ive_cos_dist_cfg_s* p_cfg,vs_ive_cos_dist_result_s* p_dst,vs_bool_t block);

/**
* @ingroup      vs_mal_ive
* @brief        euclid distant of two matrix
* @details
* @param [out]  p_handle    the returned task ID used for task status query
* @param [in]   p_src       input left and right matrix
* @param [in]   p_index     vector index of left matrix and right matrix
* @param [in]   p_cfg       the config parameters of euclid_dist
* @param [out]  p_dst       euclid distant result and top n result
* @param [in]   block       the flag indicating whether to block the task
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ive_euclid_dist(vs_uint32_t *p_handle, vs_ive_double_matrix_s *p_src, vs_ive_double_matrix_s *p_index,
    vs_ive_euclid_dist_cfg_s *p_cfg, vs_ive_euclid_dist_result_s *p_dst, vs_bool_t block);

/**
* @ingroup      vs_mal_ive
* @brief        kernel correlation filter
* @details
* @param [out]  p_handle    the returned task ID used for task status query
* @param [in]   p_src       input image, 176x144 - 1920x1080
* @param [in]   roi[]       input region
* @param [in]   p_training_buf  buffer for training
* @param [out]  target_roi[]    output target region
* @param [in]   p_cfg           the config parameters of kcf
* @param [in]   block           the flag indicating whether to block the task
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ive_kcf(vs_uint32_t *p_handle,vs_ive_image_s* p_src,vs_rect_s roi[],vs_ive_buffer_s *p_training_buf,
    vs_rect_s target_roi[],vs_ive_kcf_cfg_s* p_cfg,vs_bool_t block);

/**
* @ingroup      vs_mal_ive
* @brief        st corner detect
* @details
* @param [out]  p_handle    the returned task ID used for task status query
* @param [in]   p_src       input image, 64x64 - 1920x1080
* @param [out]  p_corner    output corner
* @param [in]   p_cfg       the config parameters of st_corner
* @param [in]   block       the flag indicating whether to block the task
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ive_st_corner(vs_uint32_t *p_handle, vs_ive_image_s *p_src, vs_ive_buffer_s *p_corner,
    vs_ive_st_corner_cfg_s *p_cfg, vs_bool_t block);

/**
* @ingroup      vs_mal_ive
* @brief        lk optical flow
* @details
* @param [out]  p_handle       the returned task ID used for task status query
* @param [in]   src_curr_pyr   input curr image pyramid, base level: 64x64 - 1920x1080
* @param [in]   src_next_pyr   input next image pyramid, base level: 64x64 - 1920x1080
* @param [in]   p_curr_points  input curr points
* @param [out]  p_next_points  output next points
* @param [out]  p_status       output status
* @param [out]  p_err          output error
* @param [in]   p_cfg          the config parameters of lk_optical_flow
* @param [in]   block          the flag indicating whether to block the task
* @retval       VS_SUCCESS     no error
* @retval       others         error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ive_lk_optical_flow(vs_uint32_t* p_handle, vs_ive_image_s src_curr_pyr[], vs_ive_image_s src_next_pyr[],
    vs_ive_buffer_s* p_curr_points, vs_ive_buffer_s* p_next_points, vs_ive_buffer_s* p_status,
    vs_ive_buffer_s* p_err, vs_ive_lk_optical_flow_cfg_s* p_cfg, vs_bool_t block);

/**
* @ingroup      vs_mal_ive
* @brief        sad
* @details
* @param [out]  p_handle        the returned task ID used for task status query
* @param [in]   p_src1          input src image 1 U8C1: 64x64 - 1920x1080
* @param [in]   p_src2          input src image 2 U8C1: 64x64 - 1920x1080
* @param [out]  p_sad           output sad U16C1/U8C1
* @param [out]  p_thr           output thr U8C1
* @param [in]   p_cfg           the config parameters of sad
* @param [in]   block           the flag indicating whether to block the task
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ive_sad(vs_uint32_t* p_handle, vs_ive_image_s* p_src1, vs_ive_image_s* p_src2,
    vs_ive_image_s* p_sad, vs_ive_image_s* p_thr, vs_ive_sad_cfg_s* p_cfg, vs_bool_t block);

/**
* @ingroup      vs_mal_ive
* @brief        Normalized Cross-Correlation
* @details
* @param [out]  p_handle        the returned task ID used for task status query
* @param [in]   p_src1          input src image 1 U8C1: 32x32 - 1920x1080
* @param [in]   p_src2          input src image 2 U8C1: 32x32 - 1920x1080
* @param [out]  p_dst           output: 3 * uint64
* @param [in]   block           the flag indicating whether to block the task
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ive_ncc(vs_uint32_t *p_handle, vs_ive_image_s *p_src1, vs_ive_image_s *p_src2,
    vs_ive_buffer_s *p_dst, vs_bool_t block);

/**
* @ingroup      vs_mal_ive
* @brief        hough line
* @details
* @param [out]  p_handle    the returned task ID used for task status query
* @param [in]   p_src       input image, 64x64 - 1280x720
* @param [out]  p_line_info output lines
* @param [in]   p_cfg       the config parameters of houghline
* @param [in]   block       the flag indicating whether to block the task
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ive_houghline(vs_uint32_t *p_handle, vs_ive_image_s *p_src, vs_ive_buffer_s *p_line_info,
    vs_ive_houghline_cfg_s *p_cfg, vs_bool_t block);

/**
* @ingroup      vs_mal_ive
* @brief        add mosaic to a image
* @details
* @param [out]  p_handle    the returned task ID used for task status query
* @param [in/out]   p_img   input image, 64x64 - 8192x8192
* @param [in]   mosaic[]    input region
* @param [in]   num         the num of mosaic
* @param [in]   block       the flag indicating whether to block the task
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ive_mosaic(vs_uint32_t *p_handle, vs_ive_image_s *p_img, vs_ive_mosaic_s mosaic[], vs_uint32_t num, vs_bool_t block);

/**
* @ingroup      vs_mal_ive
* @brief        Argument-Maximum
* @details
* @param [out]  p_handle    the returned task ID used for task status query
* @param [in]   p_src       input src tensor, data type: int8 | uint8 | fp32, dimension = 1 | 2 | 3
* @param [out]  p_dst       output dst tensor, data type: uint16, dimension = 0 | 1 | 2
* @param [in]   axis        the axis of argmax calculation
* @param [in]   block       the flag indicating whether to block the task
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ive_argmax(vs_uint32_t *p_handle, vs_ive_tensor_s  *p_src, vs_ive_tensor_s  *p_dst,
                            vs_int32_t axis, vs_bool_t block);

/**
* @ingroup      vs_mal_ive
* @brief        gray level co-occurrence matrix
* @details
* @param [out]  p_handle        the returned task ID used for task status query
* @param [in]   p_src           input image, 64x64 - 1920x1080
* @param [out]  p_energy        output energy
* @param [out]  p_contrast      output contrast
* @param [out]  p_entropy       output entropy
* @param [out]  p_correlation   output correlation
* @param [in]   p_cfg           the config parameters of glcm
* @param [in]   block           the flag indicating whether to block the task
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ive_glcm(vs_uint32_t *p_handle, vs_ive_image_s *p_src, vs_ive_image_s *p_energy, vs_ive_image_s *p_contrast,
						vs_ive_image_s *p_entropy, vs_ive_image_s *p_correlation, vs_ive_glcm_cfg_s *p_cfg, vs_bool_t block);

/**
* @ingroup      vs_mal_ive
* @brief        normgrad (normalized gradient)
* @details		gradient calculation and the output is normalized to S8.
* @param [out]  p_handle		the returned task ID used for task status query
* @param [in]   p_src			input src image U8C1: 64x64 - 4096x4096
* @param [out]  p_dst_h			output the horizontal result of input image filtered by the input mask; S8C1
* @param [out]  p_dst_v			output the vertical result of input image filtered by the transposed mask; S8C1
* @param [out]  p_dst_hv		output the horizontal and vertical component in single image in package format. S8C2_PACKAGE
* @param [in]   p_cfg			the config parameters of normgrad
* @param [in]   block			the flag indicating whether to block the task
* @retval       VS_SUCCESS		no error
* @retval       others			error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ive_normgrad(vs_uint32_t *p_handle, vs_ive_image_s *p_src, vs_ive_image_s *p_dst_h, vs_ive_image_s *p_dst_v,
                                vs_ive_image_s *p_dst_hv, vs_ive_normgrad_cfg_s *p_cfg, vs_bool_t block);

/**
* @ingroup      vs_mal_ive
* @brief        matting foreground image from green screen video and merge background image
* @details
* @param [out]  p_handle            the returned task ID used for task status query
* @param [in]   p_src               input frame, 64x64 - 3840x2160
* @param [in]   p_bg                input background image
* @param [out]  p_dst               output frame, support to multiplex input frame to store
* @param [in]   p_cfg               the config parameters of imagematting
* @param [in]   block               the flag indicating whether to block the task
* @retval       VS_SUCCESS          no error
* @retval       others              error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ive_imagematting(vs_uint32_t* p_handle, vs_ive_image_s* p_src, vs_ive_image_s* p_bg,
                                vs_ive_image_s* p_dst, vs_ive_imagematting_cfg_s* p_cfg, vs_bool_t block);

/**
* @ingroup      vs_mal_ive
* @brief        perspective warp, transform src image to dst image, based on 2d points' coordinate in src & dst
* @details
* @param [out]  p_handle            the returned task ID used for task status query
* @param [in]   p_src               input src image, 64x64 - 3840x2160
* @param [in]   src_point           four 2d points' coordinate in src, represents the corner of interested region
* @param [in]   dst_point           four 2d points' coordinate in dst, represents the corner of interested region
* @param [out]  p_dst               output dst image, 64x64 - 3840x2160
* @param [in]   p_cfg               the config parameters of perspective
* @param [in]   block               the flag indicating whether to block the task
* @retval       VS_SUCCESS          no error
* @retval       others              error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ive_perspective(vs_uint32_t* p_handle, vs_ive_image_s* p_src, vs_point_s src_point[], vs_point_s dst_point[],
                                vs_ive_image_s* p_dst, vs_ive_persp_cfg_s* p_cfg, vs_bool_t block);

/**
* @ingroup      vs_mal_ive
* @brief        Threshold processing of S16 data to 8bit data
* @details
* @param [out]  p_handle            the returned task ID used for task status query
* @param [in]   p_src               input src image, 1x1 - 3840x2160
* @param [out]  p_dst               output dst image, 1x1 - 3840x2160
* @param [in]   p_cfg               the config parameters of threshold_s16
* @param [in]   block               the flag indicating whether to block the task
* @retval       VS_SUCCESS          no error
* @retval       others              error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ive_threshold_s16(vs_uint32_t *p_handle, vs_ive_image_s  *p_src, vs_ive_image_s  *p_dst,
                                vs_ive_threshold_s16_cfg_s *p_cfg, vs_bool_t block);

/**
* @ingroup      vs_mal_ive
* @brief        Threshold processing of U16 data to 8bit data
* @details
* @param [out]  p_handle            the returned task ID used for task status query
* @param [in]   p_src               input src image, 1x1 - 3840x2160
* @param [out]  p_dst               output dst image, 1x1 - 3840x2160
* @param [in]   p_cfg               the config parameters of threshold_u16
* @param [in]   block               the flag indicating whether to block the task
* @retval       VS_SUCCESS          no error
* @retval       others              error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ive_threshold_u16(vs_uint32_t *p_handle, vs_ive_image_s  *p_src, vs_ive_image_s  *p_dst,
                                vs_ive_threshold_u16_cfg_s *p_cfg, vs_bool_t block);

/**
* @ingroup      vs_mal_ive
* @brief        extract high frequency component of the s16 src image and cast to s8 dst image
* @details
* @param [out]  p_handle            the returned task ID used for task status query
* @param [in]   p_src               input image, 16x16 - 4096x4096
* @param [out]  p_dst               output image, resolution is the same as p_src
* @param [in]   p_cfg               the config parameters of high_freq_extract
* @param [in]   block               the flag indicating whether to block the task
* @retval       VS_SUCCESS          no error
* @retval       others              error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ive_high_freq_extract(vs_uint32_t* p_handle, vs_ive_image_s* p_src, vs_ive_image_s* p_dst,
                                        vs_ive_high_freq_extract_cfg_s* p_cfg, vs_bool_t block);

/**
* @ingroup      vs_mal_ive
* @brief        bilateral filter
* @details
* @param [out]  p_handle            the returned task ID used for task status query
* @param [in]   p_src               input image, 16x16 - 4096x4096
* @param [out]  p_dst               output image, resolution is the same as p_src
* @param [in]   p_cfg               the config parameters of bilateral filter
* @param [in]   block               the flag indicating whether to block the task
* @retval       VS_SUCCESS          no error
* @retval       others              error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ive_bilateral_filter(vs_uint32_t* p_handle, vs_ive_image_s* p_src, vs_ive_image_s* p_dst,
                                        vs_ive_bilateral_filter_cfg_s* p_cfg, vs_bool_t block);

/**
* @ingroup      vs_mal_ive
* @brief        query task status
* @details      query whether the task is finish or not
* @param [in]   handle    the returned task ID used for task status query
* @param [out]  p_finish  indicating whether the task is finished
* @param [in]   block     the flag indicating whether to block the task
* @retval       VS_SUCCESS      no error
* @retval       others          error happens. see ERROR_CODE for detail.
* @warning
* @see
* @note
* @par
*
*/
vs_int32_t vs_mal_ive_query(vs_uint32_t handle, vs_bool_t *p_finish, vs_bool_t block);

#ifdef __cplusplus
}
#endif

#endif

