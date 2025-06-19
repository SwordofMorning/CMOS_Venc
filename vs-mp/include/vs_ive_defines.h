
/**
* @file vs_ive_defines.h
* @brief Declaration of ive enumeration, struct and API.
* @details Declaration of ive enumeration, struct and API.
* @author VS software group
* @date 2021-11-02
* @version v0.01
* @copyright (C) 2021, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/

#ifndef __VS_IVE_DEFINES_H__
#define __VS_IVE_DEFINES_H__

/*****************************************************************************
 *                    Includes
 *****************************************************************************/
#include "vs_type.h"
#include "vs_comm.h"
#include "vs_comm_video.h"
#include "vs_errno.h"

#ifdef __cplusplus
extern "C"{
#endif

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/
#define IVE_PADDING_NO          (0)
#define IVE_PADDING_3X3         (1)
#define IVE_PADDING_5X5         (2)
#define IVE_PADDING_CANNY       (4)
#define IVE_REGION_NUM_MAX      (255)    ///< CCL max blob num

#define VS_ERR_IVE_INVALID_PARAM        VS_ERR_CODE(E_MOD_ID_IVE,E_ERR_INVALID_PARAM)           ///< parameter is invalid
#define VS_ERR_IVE_NULL_PTR             VS_ERR_CODE(E_MOD_ID_IVE,E_ERR_NULL_PTR)                ///< pointer val is null
#define VS_ERR_IVE_DEVICE_UNEXIST       VS_ERR_CODE(E_MOD_ID_IVE,E_ERR_DEVICE_UNEXIST)          ///< ive device does not exist
#define VS_ERR_IVE_NOT_SUPPORT          VS_ERR_CODE(E_MOD_ID_IVE,E_ERR_NOT_SUPPORT)             ///< function unsupported
#define VS_ERR_IVE_NOT_INITIALIZED      VS_ERR_CODE(E_MOD_ID_IVE,E_ERR_NOT_INITIALIZED)         ///< module uninitialized
#define VS_ERR_IVE_ALIGNMENT            VS_ERR_CODE(E_MOD_ID_IVE,E_ERR_IVE_ALIGNMENT)	        ///< alignment error
#define VS_ERR_IVE_IMAGE_SIZE           VS_ERR_CODE(E_MOD_ID_IVE,E_ERR_IVE_IMAGE_SIZE)	        ///< image stride,width or height error
#define VS_ERR_IVE_OUT_OF_RANGE         VS_ERR_CODE(E_MOD_ID_IVE,E_ERR_IVE_OUT_OF_RANGE)        ///< value is out of range
#define VS_ERR_IVE_MODE                 VS_ERR_CODE(E_MOD_ID_IVE,E_ERR_IVE_MODE)                ///< mode error
#define VS_ERR_IVE_IMAGE_TYPE           VS_ERR_CODE(E_MOD_ID_IVE,E_ERR_IVE_IMAGE_TYPE)          ///< image type error
#define VS_ERR_IVE_SCALE_RATIO          VS_ERR_CODE(E_MOD_ID_IVE,E_ERR_IVE_SCALE_RATIO)	        ///< zoom scale error
#define VS_ERR_IVE_HANDLE               VS_ERR_CODE(E_MOD_ID_IVE,E_ERR_IVE_HANDLE)              ///< invalid handle
#define VS_ERR_IVE_DSP_TIMEOUT          VS_ERR_CODE(E_MOD_ID_IVE,E_ERR_IVE_DSP_TIMEOUT)         ///< dsp ops timeout
#define VS_ERR_IVE_DSP_NOT_INITIALIZED  VS_ERR_CODE(E_MOD_ID_IVE,E_ERR_IVE_DSP_NOT_INITIALIZED) ///< dsp not initialized

/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/

/**
* @brief The IVE error code
*/
typedef enum vs_ive_error_code
{
    E_ERR_IVE_ALIGNMENT = 0x60,     ///< alignment error
    E_ERR_IVE_IMAGE_SIZE,           ///< image stride,width or height error
    E_ERR_IVE_OUT_OF_RANGE,         ///< value is out of range
    E_ERR_IVE_MODE,                 ///< mode error
    E_ERR_IVE_IMAGE_TYPE,           ///< image type error
    E_ERR_IVE_SCALE_RATIO,          ///< scale ratio error
    E_ERR_IVE_HANDLE,               ///< invalid handle
    E_ERR_IVE_DSP_TIMEOUT,          ///< dsp ops timeout
    E_ERR_IVE_DSP_NOT_INITIALIZED,  ///< dsp not initialized
    E_ERR_IVE_CODE_MAX
}vs_ive_error_code_e;

/**
* @brief The IVE tensor type
*/
typedef enum vs_ive_tensor_data_type{
    E_IVE_TENSOR_DATA_FORMAT_FP32 = 0, ///< single-precision floating-point format
    E_IVE_TENSOR_DATA_FORMAT_FP16,     ///< half-precision floating-point format
    E_IVE_TENSOR_DATA_FORMAT_UINT8,    ///< unsigned 8-bit integer format
    E_IVE_TENSOR_DATA_FORMAT_INT8,     ///< signed 8-bit integer format
    E_IVE_TENSOR_DATA_FORMAT_UINT16,   ///< unsigned 16-bit integer format
    E_IVE_TENSOR_DATA_FORMAT_INT16,    ///< signed 16-bit integer format
    E_IVE_TENSOR_DATA_FORMAT_CHAR,     ///< character format
    E_IVE_TENSOR_DATA_FORMAT_BFP16,    ///< bfloat format
    E_IVE_TENSOR_DATA_FORMAT_INT32,    ///< signed 32-bit integer format
    E_IVE_TENSOR_DATA_FORMAT_UINT32,   ///< unsigned 32-bit integer format
    E_IVE_TENSOR_DATA_FORMAT_INT64,    ///< signed 64-bit integer format
    E_IVE_TENSOR_DATA_FORMAT_UINT64,   ///< unsigned 64-bit integer format
    E_IVE_TENSOR_DATA_FORMAT_FP64,     ///< double-precision floating-point format
    E_IVE_TENSOR_DATA_FORMAT_MAX
}vs_ive_tensor_data_type_e;

/**
* @brief The IVE common 2D data type
*/
typedef struct vs_ive_data
{
    vs_uint64_t  phys_addr;		///< physical address of 2D general data
    vs_void_t  *virt_addr; 		///< virtual address of 2D general data
    vs_uint32_t  stride;		///< 2D general data stride,16-byte alignment
    vs_uint32_t  width; 		///< 2D general data width
    vs_uint32_t  height;  		///< 2D general data height
}vs_ive_data_s;

/**
* @brief The IVE image type
*/
typedef enum vs_ive_image_type
{
    E_IVE_IMAGE_TYPE_2D_DATA = 0x0, ///< 2D general data format
    E_IVE_IMAGE_TYPE_1D_DATA,       ///< 1D general data format
    E_IVE_IMAGE_TYPE_U8C1,          ///< single channel image data,unsigned 8-bit per pixel
    E_IVE_IMAGE_TYPE_YUV420SP,      ///< YUV420SP,default NV21
    E_IVE_IMAGE_TYPE_YUV422SP,      ///< YUV422SP,default NV61
    E_IVE_IMAGE_TYPE_U8C3_PACKAGE,  ///< 3-channel package image data,24bit per pixel
    E_IVE_IMAGE_TYPE_U16C1,         ///< single channel image data,unsigned 16-bit per pixel
    E_IVE_IMAGE_TYPE_S16C1,         ///< single channel image data,signed 16-bit per pixel
    E_IVE_IMAGE_TYPE_S8C1,          ///< single channel image data,signed 8-bit per pixel
    E_IVE_IMAGE_TYPE_U32C1,         ///< single channel image data,unsigned 32-bit per pixel
    E_IVE_IMAGE_TYPE_U64C1,         ///< single channel image data,unsigned 64-bit per pixel
    E_IVE_IMAGE_TYPE_U96C1,         ///< single channel image data,unsigned 96-bit per pixel
    E_IVE_IMAGE_TYPE_FP32C1,        ///< single channel image data,float 32-bit per pixel
    E_IVE_IMAGE_TYPE_U8C3_PLANAR,   ///< 3-channel planar image data,24bit per pixel
    E_IVE_IMAGE_TYPE_S8C2_PACKAGE,  ///< 2-channel package image data,16bit per pixel
    E_IVE_IMAGE_TYPE_MAX
}vs_ive_image_type_e;

/**
* @brief The 3-channel image data type
*/
typedef struct vs_ive_image
{
    vs_ive_image_type_e  type;	///< indicating image type
    vs_uint64_t  phys_addr[3];	///< image physical address
    vs_void_t   *virt_addr[3];  ///< image virtual address
    vs_uint32_t  stride[3];     ///< image stride
    vs_uint32_t  width;         ///< image width
    vs_uint32_t  height;        ///< image height
}vs_ive_image_s;

/**
* @brief The IVE common 1D data type
*/
typedef struct vs_ive_buffer
{
    vs_uint64_t  phys_addr;    ///< physical address of 1D general data
    vs_void_t   *virt_addr;    ///< virtual address of 1D general data
    vs_uint32_t  size;         ///< size of 1D general data
}vs_ive_buffer_s;

/**
* @brief The operation mode of DMA
*/
typedef enum vs_ive_dma_mode
{
    E_IVE_DMA_MODE_DIRECT_COPY = 0x0, ///< dma direct copy mode
    E_IVE_DMA_MODE_INTERVAL_COPY,     ///< dma interval copy mode
    E_IVE_DMA_MODE_3BYTE_COPY,        ///< dma 3-byte fill mode
    E_IVE_DMA_MODE_8BYTE_COPY,        ///< dma 8-byte fill mode
    E_IVE_DMA_MODE_MAX
}vs_ive_dma_mode_e;


/**
* @brief The config parameters of DMA
*/
typedef struct vs_ive_dma_cfg
{
    vs_ive_dma_mode_e mode;               ///< dma mode
    vs_uint8_t        sub_image_width;    ///< subimage width only for dma interval copy mode
    vs_uint16_t       sub_image_height;   ///< subimage height only for dma interval copy mode
    vs_uint16_t       sub_image_byte_num; ///< copy byte number only for dma interval copy mode
    vs_uint32_t       byte_copy_val0;     ///< filled value0 for 3-byte or 8-byte fill mode
    vs_uint32_t       byte_copy_val1;     ///< filled value1 for 3-byte or 8-byte fill mode
}vs_ive_dma_cfg_s;


/**
* @brief The config parameters of FILTER
*/
typedef struct vs_ive_filter_cfg
{
    vs_int8_t  mask[25];       ///< 5x5 filter template
    vs_uint8_t normalization;  ///< normalized parameters
    vs_bool_t  abs_enable;     ///< ABS enable
    vs_uint8_t min_value;      ///< min value threshold
    vs_uint8_t max_value;      ///< max value threshold
}vs_ive_filter_cfg_s;

/**
* @brief The config parameters of COLOR
*/
typedef struct vs_ive_color_cfg
{
    vs_uint8_t min_color_y;         ///< minimum y value
    vs_uint8_t max_color_y;         ///< maximum y value
    vs_uint8_t min_color_uv;        ///< minimum uv value
    vs_uint8_t max_color_uv;        ///< maximum uv value
}vs_ive_color_cfg_s;

/**
* @brief The config parameters of FILTER_COLOR
*/
typedef struct vs_ive_filter_color_cfg
{
    vs_ive_filter_cfg_s filter_cfg;  ///< filter config info
    vs_ive_color_cfg_s  color_cfg;   ///< color config info
}vs_ive_filter_color_cfg_s;


/**
* @brief The operation mode of CSC
*/
typedef enum vs_ive_csc_mode
{
    E_IVE_CSC_MODE_VIDEO_BT601_YUV2RGB = 0x0,       ///< YUV2RGB_PACKAGE video conversion of bt601
    E_IVE_CSC_MODE_VIDEO_BT709_YUV2RGB,             ///< YUV2RGB_PACKAGE video conversion of bt709
    E_IVE_CSC_MODE_PICTURE_BT601_YUV2RGB,           ///< YUV2RGB_PACKAGE picture conversion of bt601
    E_IVE_CSC_MODE_PICTURE_BT709_YUV2RGB,           ///< YUV2RGB_PACKAGE picture conversion of bt709
    E_IVE_CSC_MODE_PICTURE_BT601_YUV2HSV,           ///< YUV2HSV picture conversion of bt601
    E_IVE_CSC_MODE_PICTURE_BT709_YUV2HSV,           ///< YUV2HSV picture conversion of bt709
    E_IVE_CSC_MODE_VIDEO_BT601_RGB2YUV,             ///< RGB2YUV video conversion of bt601
    E_IVE_CSC_MODE_VIDEO_BT709_RGB2YUV,             ///< RGB2YUV video conversion of bt709
    E_IVE_CSC_MODE_PICTURE_BT601_RGB2YUV,           ///< RGB2YUV picture conversion of bt601
    E_IVE_CSC_MODE_PICTURE_BT709_RGB2YUV,           ///< RGB2YUV picture conversion of bt709
    E_IVE_CSC_MODE_VIDEO_BT601_YUV2BGR,             ///< YUV2BGR_PACKAGE video conversion of bt601
    E_IVE_CSC_MODE_VIDEO_BT709_YUV2BGR,             ///< YUV2BGR_PACKAGE video conversion of bt709
    E_IVE_CSC_MODE_PICTURE_BT601_YUV2BGR,           ///< YUV2BGR_PACKAGE picture conversion of bt601
    E_IVE_CSC_MODE_PICTURE_BT709_YUV2BGR,           ///< YUV2BGR_PACKAGE picture conversion of bt709
    E_IVE_CSC_MODE_VIDEO_BT601_YUV2BGR_PLANAR,      ///< YUV2BGB_PLANAR video conversion of bt601
    E_IVE_CSC_MODE_VIDEO_BT709_YUV2BGR_PLANAR,      ///< YUV2BGR_PLANAR video conversion of bt709
    E_IVE_CSC_MODE_PICTURE_BT601_YUV2BGR_PLANAR,    ///< YUV2BGR_PLANAR picture conversion of bt601
    E_IVE_CSC_MODE_PICTURE_BT709_YUV2BGR_PLANAR,    ///< YUV2BGR_PLANAR picture conversion of bt709
    E_IVE_CSC_MODE_MAX
}vs_ive_csc_mode_e;

/**
* @brief The operation mode of THRESH
*/
typedef enum vs_ive_thr_mode
{
    E_IVE_THR_DUAL_MODE = 0x0, ///< src<=low_thr,des=min_val;src>high_thr,des=max_val
    E_IVE_THR_TRUNCATION_MODE, ///< src<=low_thr,des=src;src>high_thr,des=max_val
    E_IVE_THR_MIN_MODE,        ///< src<=low_thr,des=min_val;src>high_thr,des=src
    E_IVE_THR_TRIVAL_MODE,     ///< src<=low_thr,des=min_val;low_thr<src<=high_thr,des=mid_val;src>high_thr,des=max_val
    E_IVE_THR_LOW_SRC_MODE,    ///< src<=low_thr,des=src;low_thr<src<=high_thr,des=mid_val;src>high_thr,des=max_val
    E_IVE_THR_HIGH_SRC_MODE,   ///< src<=low_thr,des=min_val;low_thr<src<=high_thr,des=mid_val;src>high_thr,des=src
    E_IVE_THR_MED_SRC_MODE,    ///< src<=low_thr,des=min_val;low_thr<src<=high_thr,des=src;src>high_thr,des=max_val
    E_IVE_THR_DUAL_END_MODE,   ///< src<=low_thr,des=src;low_thr<src<=high_thr,des=mid_val;src>high_thr,des=src
    E_IVE_THR_MODE_MAX
}vs_ive_thr_mode_e;

/**
* @brief The config parameters of THRESH
*/
typedef struct vs_ive_thr_cfg
{
    vs_ive_thr_mode_e mode; ///< threshold mode
    vs_uint8_t low_thr;     ///< low threshold
    vs_uint8_t high_thr;    ///< high threshold
    vs_uint8_t min_val;     ///< minimum value
    vs_uint8_t med_val;     ///< median value
    vs_uint8_t max_val;     ///< maximum value
}vs_ive_thr_cfg_s;

/**
* @brief The config parameters of ADD
*/
typedef struct vs_ive_add_cfg
{
    vs_uint16_t x_weight;  ///< x-axis weight
    vs_uint16_t y_weight;  ///< y-axis weight
}vs_ive_add_cfg_s;

/**
* @brief The operation mode of SUB
*/
typedef enum vs_ive_sub_mode
{
    E_IVE_SUB_MODE_ABS = 0x1,   ///< absolute subtraction
    E_IVE_SUB_MODE_SHIFT = 0x3, ///< shift subtraction
    E_IVE_SUB_MODE_MAX
}vs_ive_sub_mode_e;

/**
* @brief The operation mode of MAP
*/
typedef enum vs_ive_map_mode
{
    E_IVE_MAP_MODE_U8 = 0x0, ///< unsigned 8bit to unsigned 8bit mapping
    E_IVE_MAP_MODE_S16,      ///< unsigned 8bit to signed 16bit mapping
    E_IVE_MAP_MODE_U16,      ///< unsigned 8bit to unsigned 16bit mapping
    E_IVE_MAP_MODE_MAX
}vs_ive_map_mode_e;

/**
* @brief The operation mode of RESIZE
*/
typedef enum vs_ive_resize_mode
{
    E_IVE_RESIZE_MODE_BILINEAR = 0x0, ///< bilinear interpolation
    E_IVE_RESIZE_MODE_AREA,           ///< area interpolation
    E_IVE_RESIZE_MODE_MAX
}vs_ive_resize_mode_e;

/**
* @brief The config parameters of RESIZE
*/
typedef struct vs_ive_resize_cfg
{
    vs_ive_resize_mode_e mode;       ///< resize mode
    vs_uint16_t          image_num;  ///< input image number,up to 10
}vs_ive_resize_cfg_s;

/**
* @brief The operation mode of mag and ang
*/
typedef enum vs_ive_mag_ang_out_mode
{
    E_IVE_MAG_ANG_OUT_MODE_MAG = 0x0, ///< output magnitude only
    E_IVE_MAG_ANG_OUT_MODE_BOTH,      ///< output magnitude and angle
    E_IVE_MAG_ANG_OUT_MODE_MAX
}vs_ive_mag_ang_out_mode_e;

/**
* @brief The config parameters of mag and ang
*/
typedef struct vs_ive_mag_ang_cfg
{
    vs_ive_mag_ang_out_mode_e mode; ///< output mode
    vs_uint16_t        thr;         ///< threshold for amplitude
    vs_int8_t          mask[25];    ///< 5x5 template parameter
}vs_ive_mag_ang_cfg_s;

/**
* @brief The operation mode of 16bit_to_8bit
*/
typedef enum vs_ive_16bit_to_8bit_mode
{
    E_IVE_16BIT_TO_8BIT_MODE_S16_TO_S8 = 0x0, ///< linear transformation from S16 data to S8 data
    E_IVE_16BIT_TO_8BIT_MODE_S16_TO_U8_ABS,   ///< after the linear transformation of S16 data to S8 data,take the absolute value to U8 data
    E_IVE_16BIT_TO_8BIT_MODE_S16_TO_U8_BIAS,  ///< after the linear transformation of S16 data to S8 data,truncated to U8 data
    E_IVE_16BIT_TO_8BIT_MODE_U16_TO_U8,       ///< linear transformation from U16 data to U8 data
    E_IVE_16BIT_TO_8BIT_MODE_MAX
}vs_ive_16bit_to_8bit_mode_e;

/**
* @brief The config parameters of 16bit_to_8bit
*/
typedef struct vs_ive_16bit_to_8bit_cfg
{
    vs_ive_16bit_to_8bit_mode_e mode;         ///< transformation type
    vs_uint16_t                 denominator;  ///< denominator in linear transformation
    vs_uint8_t                  numerator;    ///< numerator in linear transformation
    vs_int8_t                   bias;         ///< bias in linear transformation
}vs_ive_16bit_to_8bit_cfg_s;


/**
* @brief The config parameters of GMM
*/
typedef struct vs_ive_gmm_cfg
{
    vs_uint32_t q10_noise_var;       ///< noise variance value
    vs_uint32_t q10_var_max;         ///< max variance value
    vs_uint32_t q10_var_min;         ///< min variance value
    vs_uint16_t q16_learn_rate;      ///< learning rate
    vs_uint16_t q16_bg_ratio;        ///< background ratio
    vs_uint16_t q8_var_thr;          ///< background variance threshold
    vs_uint16_t q16_init_weight;     ///< initial model weight
    vs_uint16_t nmixture;            ///< model number
}vs_ive_gmm_cfg_s;

/**
* @brief The config parameters of GMM2
*/
typedef struct vs_ive_gmm2_cfg
{
    vs_bool_t    is_first_frame;         ///< check is first frame
    vs_bool_t    update_bg_model;        ///< enable model training
    vs_uint16_t  history;                ///< the frame number work in model train
    vs_uint16_t  q8_var_thr;             ///< variance threshold
    vs_uint16_t  nmixtures;              ///< max model number
    vs_uint16_t  q16_bg_ratio;           ///< background ratio
    vs_uint16_t  q7_var_thr_gen;         ///< variance threshold to generate new model
    vs_uint16_t  q7_var_init;            ///< initial variance
    vs_uint16_t  q7_var_max;             ///< maximum variance
    vs_uint16_t  q7_var_min;             ///< minimal variance
    vs_uint16_t  q16_factor_reduce_thr;  ///< factor reduce threshold
    vs_uint16_t  shadows_value;          ///< shadow value
    vs_uint16_t  q16_shadows_thr;        ///< shadow threshold
}vs_ive_gmm2_cfg_s;


/**
* @brief sobel output type
*/
typedef enum vs_ive_sobel_out_mode
{
    E_IVE_SOBEL_OUT_MODE_HOR = 0x0, ///< output horizontal gradient
    E_IVE_SOBEL_OUT_MODE_VER,	    ///< output vertical gradient
    E_IVE_SOBEL_OUT_MODE_BOTH,	    ///< output horizontal gradient and vertical gradient
    E_IVE_SOBEL_OUT_MODE_MAX
}vs_ive_sobel_out_mode_e;
/**
* @brief sobel config
*/
typedef struct vs_ive_sobel_cfg
{
    vs_ive_sobel_out_mode_e mode;      ///< sobel output mode
    vs_int8_t               mask[25];  ///< 5x5 template parameters
}vs_ive_sobel_cfg_s;

/**
* @brief filter coefficient mask
*/
typedef struct vs_ive_filter_coef
{
    vs_uint8_t mask[25];	///< 5x5 template parameters
}vs_ive_filter_coef_s;



/**
* @brief integ out mode
*/
typedef enum vs_ive_integ_out_mode
{
    E_IVE_INTEG_OUT_MODE_BOTH = 0x0, ///< output sum and square sum integral both
    E_IVE_INTEG_OUT_MODE_SUM,        ///< output sum integral only
    E_IVE_INTEG_OUT_MODE_SQSUM,      ///< output square sum integral only
    E_IVE_INTEG_OUT_MODE_MAX
}vs_ive_integ_out_mode_e;

/**
* @brief region struct
*/
typedef struct vs_ive_region
{
    vs_uint32_t area;	 ///< represented by the pixel number
    vs_uint16_t left;    ///< left border of region
    vs_uint16_t right;   ///< right border of region
    vs_uint16_t top;     ///< top border of region
    vs_uint16_t bottom;  ///< bottom border of region
}vs_ive_region_s;

/**
* @brief CCL blob struct
*/
typedef struct vs_ive_blob
{
    vs_uint16_t area_thr;                         ///< regions area size threshold
    vs_int8_t  label_status;                      ///< 0:success and other value failed
    vs_uint8_t  region_num;                       ///< region number
    vs_ive_region_s regions[IVE_REGION_NUM_MAX];  ///< valid regions information
}vs_ive_blob_s;

/**
* @brief ccl mode
*/
typedef enum vs_ive_ccl_mode
{
    E_IVE_CCL_MODE_4C = 0x0, ///< 4 connectivity
    E_IVE_CCL_MODE_8C,       ///< 8 connectivity
    E_IVE_CCL_MODE_MAX
}vs_ive_ccl_mode_e;

/**
* @brief ccl config
*/
typedef struct vs_ive_ccl_cfg
{
    vs_ive_ccl_mode_e mode;     ///< ccl work mode
    vs_uint16_t init_area_thr;  ///< initial area threshold
    vs_uint16_t step;           ///< area threshold growth step
}vs_ive_ccl_cfg_s;

/**
* @brief canny config
*/
typedef struct vs_ive_canny_cfg
{
    vs_uint16_t low_thr;   ///< low threshold
    vs_uint16_t high_thr;  ///< high threshold
} vs_ive_canny_cfg_s;

/**
* @brief lbp compare mode
*/
typedef enum vs_ive_lbp_cmp_mode
{
    E_IVE_LBP_CMP_MODE_NORMAL = 0x0,  ///< simple comparison mode
    E_IVE_LBP_CMP_MODE_ABS,           ///< absolute comparison mode
    E_IVE_LBP_CMP_MODE_MAX
}vs_ive_lbp_cmp_mode_e;

/**
* @brief lbp config
*/
typedef struct vs_ive_lbp_cfg
{
    vs_ive_lbp_cmp_mode_e mode;	  ///< lbp mode
    union{
        vs_uint8_t        u_val;  ///< unsigned 8bit val
        vs_int8_t         s_val;  ///< signed 8bit val
    } thr;                        ///< threshold
}vs_ive_lbp_cfg_s;

/**
* @brief affine point info
*/
typedef struct vs_ive_affine_point_info
{
    vs_uint16_t     point_pair_num;  ///< point pair number
    vs_ive_buffer_s point_pair;      ///< point pair information
}vs_ive_affine_point_info_s;

/**
* @brief affine roi info
*/
typedef struct vs_ive_affine_roi_info
{
    vs_uint16_t roi_num;  ///< ROI number
    vs_rect_s  *p_roi;    ///< ROI information
}vs_ive_affine_roi_info_s;

/**
* @brief matrix data type
*/
typedef enum vs_ive_matrix_type {
    VS_IVE_MATRIX_TYPE_Q31 = 0,   ///< q31 fixed point
    VS_IVE_MATRIX_TYPE_Q15,       ///< q15 fixed point
    VS_IVE_MATRIX_TYPE_UINT32,    ///< unsigned int
    VS_IVE_MATRIX_TYPE_UINT16,    ///< unsigned short
    VS_IVE_MATRIX_TYPE_FP32,      ///< float point 32bit
    VS_IVE_MATRIX_TYPE_MAX
}vs_ive_matrix_type_e;

/**
* @brief matrix descriptor for cosine distant and euclid distant
*/
typedef struct vs_ive_matrix {
    vs_ive_matrix_type_e type;        ///< matrix data type
    vs_uint32_t          stride;      ///< matrix stride
    vs_void_t           *p_virt_addr; ///< matrix virtual address
    vs_uint64_t          phys_addr;   ///< matrix physical address
    vs_uint32_t          num;         ///< matrix number
    vs_uint32_t          width;       ///< matrix width
    vs_uint32_t          height;      ///< matrix height
    vs_uint32_t          chn;         ///< matrix channel number
} vs_ive_matrix_s;

/**
* @brief double matrix of cosine distant and euclid distant
*/
typedef struct vs_ive_double_matrix {
    vs_ive_matrix_s left_matrix;   ///< left matrix
    vs_ive_matrix_s right_matrix;  ///< right matrix
} vs_ive_double_matrix_s;

/**
* @brief cosine distant output type
*/
typedef enum vs_ive_cos_dist_out_type {
    VS_IVE_COS_DIST_OUT_RESULT = 0x0,   ///< output cosine distant result
    VS_IVE_COS_DIST_OUT_TOP_N,          ///< output top n of cosine distant result
    VS_IVE_COS_DIST_OUT_BOTH,           ///< output cosine distant result and top n
    VS_IVE_COS_DIST_OUT_MAX
}vs_ive_cos_dist_out_type_e;

/**
* @brief cosine distant config
*/
typedef struct vs_ive_cos_dist_cfg {
    vs_ive_cos_dist_out_type_e type;             ///< output type
    vs_bool_t                  fp32_to_fp16_en;  ///< enable to convert q31 fixed point to q15 fixed point
    vs_bool_t                  has_left_index;   ///< whether there is left index
    vs_bool_t                  has_right_index;  ///< whether there is right index
}vs_ive_cos_dist_cfg_s;

/**
* @brief cosine distant output result
*/
typedef struct vs_ive_cos_dist_result {
    vs_ive_matrix_s result;          ///< cosine distant result
    vs_ive_matrix_s top_n;           ///< top n of cosine distant result
    vs_ive_matrix_s top_n_index;     ///< index of top n
}vs_ive_cos_dist_result_s;

/**
* @brief euclid distant output type
*/
typedef enum vs_ive_euclid_dist_out_type {
    VS_IVE_EUCLID_DIST_OUT_RESULT = 0x0,///< output euclid distant result
    VS_IVE_EUCLID_DIST_OUT_TOP_N,       ///< output top n of euclid distant result
    VS_IVE_EUCLID_DIST_OUT_BOTH,        ///< output euclid distant result and top n
    VS_IVE_EUCLID_DIST_OUT_MAX
}vs_ive_euclid_dist_out_type_e;

/**
* @brief euclid distant config
*/
typedef struct vs_ive_euclid_dist_cfg {
    vs_ive_euclid_dist_out_type_e type;             ///< output type
    vs_bool_t                     has_left_index;   ///< whether there is left index
    vs_bool_t                     has_right_index;  ///< whether there is right index
}vs_ive_euclid_dist_cfg_s;

/**
* @brief euclid distant output result
*/
typedef struct vs_ive_euclid_dist_result {
    vs_ive_matrix_s result;             ///< euclid distant result
    vs_ive_matrix_s top_n;              ///< top n of euclid distant result
    vs_ive_matrix_s top_n_index;        ///< index of top n
}vs_ive_euclid_dist_result_s;

/**
* @brief kcf config
*/
typedef struct vs_ive_kcf_cfg {
    vs_bool_t   is_first_frame;         ///< indicate first frame
    vs_bool_t   fhog_enable;            ///< indicate fhog feature
    vs_uint16_t roi_num;                ///< ROI number
    vs_uint16_t q10_padding_rate;       ///< padding rate
    vs_uint16_t q15_lambda;             ///< regularization parameter
    vs_uint16_t q15_output_sigma_factor;///< Gaussian peak parameter
    vs_uint16_t q15_inter_factor;       ///< training factor
    vs_uint16_t q15_sigma;              ///< Gaussian kernel parameter
    vs_uint32_t cell_size;              ///< cell size of fhog
}vs_ive_kcf_cfg_s;

/**
* @brief st corner config
*/
typedef struct vs_ive_st_corner_cfg {
    vs_uint8_t  q8_quality_level;       ///< quality level for ST-Candicorner selection
    vs_uint16_t max_corner_num;         ///< max corner num, should less equal 200
    vs_uint16_t min_distance;           ///< parameter which limits the distance of output corner
}vs_ive_st_corner_cfg_s;

#define IVE_ST_CORNER_MAX_NUM (200)

/**
* @brief the point coordinate of st corner
*/
typedef struct vs_ive_point_u25q7 {
    vs_uint32_t  q7_x;  ///< the x coordinate of the st corner point
    vs_uint32_t  q7_y;  ///< the y coordinate of the st corner point
}vs_ive_point_u25q7_s;

/**
* @brief the st corner information
*/
typedef struct vs_ive_st_corner_info {
    vs_uint16_t          corner_num;                      ///< the number of the st corner point
    vs_ive_point_u25q7_s corner[IVE_ST_CORNER_MAX_NUM];   ///< the st corner points
} vs_ive_st_corner_info_s;

/**
* @brief the output mode fo LK optical flow
*/
typedef enum vs_ive_lk_optical_flow_out_mode
{
    E_IVE_LK_OPTICAL_FLOW_OUT_MODE_NONE = 0,    ///< output only next_points
    E_IVE_LK_OPTICAL_FLOW_OUT_MODE_STATUS,      ///< output next_points + status
    E_IVE_LK_OPTICAL_FLOW_OUT_MODE_BOTH,        ///< output next_points + status + error
    E_IVE_LK_OPTICAL_FLOW_OUT_MODE_MAX
}vs_ive_lk_optical_flow_out_mode_e;

/**
* @brief the LK optical flow config
*/
typedef struct vs_ive_lk_optical_flow_cfg {
    vs_ive_lk_optical_flow_out_mode_e out_mode;     ///< lk optical flow out mode
    vs_uint16_t                       points_num;   ///< lk optical flow points num
    vs_uint8_t                        max_level;    ///< lk optical flow pyramid level 0-3
    vs_uint8_t                        iter_cnt;     ///< lk optical flow max iteration
    vs_uint8_t                        q8_epsilon;   ///< lk optical flow converge condition
} vs_ive_lk_optical_flow_cfg_s;

/**
 * @brief the tracking result of the feature point of LK optical flow
 */
typedef struct vs_ive_lk_optical_flow_status{
    vs_uint8_t  status; ///< tracking status, 1: success, 0: failure
}vs_ive_lk_optical_flow_status_s;

/**
* @brief the tracking error of the feature point of LK optical flow
*/
typedef struct vs_ive_lk_optical_flow_err{
    vs_uint16_t  q7_err;  ///< the tracking error for the feature point tracked successfully
}vs_ive_lk_optical_flow_err_s;

/**
* @brief the split-block mode of SAD
*/
typedef enum vs_ive_sad_split_mode {
    E_IVE_SAD_SPLIT_4X4 = 0x0, ///< sad split 4x4
    E_IVE_SAD_SPLIT_8X8,       ///< sad split 8x8
    E_IVE_SAD_SPLIT_16X16,     ///< sad split 16x16
    E_IVE_SAD_SPLIT_MAX
} vs_ive_sad_split_mode_e;

/**
* @brief the output mode of SAD
*/
typedef enum vs_ive_sad_out_mode {
    E_IVE_SAD_OUT_16BIT_BOTH = 0x0, ///< output both 16bit SAD and 8bit THR
    E_IVE_SAD_OUT_8BIT_BOTH,        ///< output both 8bit SAD and 8bit THR
    E_IVE_SAD_OUT_16BIT_SAD,        ///< output 16bit SAD
    E_IVE_SAD_OUT_8BIT_SAD,         ///< output 8bit SAD
    E_IVE_SAD_OUT_THRESH,           ///< output 8bit THR
    E_IVE_SAD_OUT_MAX
} vs_ive_sad_out_mode_e;

/*
* @brief the SAD config
*/
typedef struct {
    vs_ive_sad_split_mode_e split_mode; ///< sad split mode
    vs_ive_sad_out_mode_e   out_mode;   ///< sad out mode
    vs_uint16_t             thresh;     ///< threshold to generate THR
    vs_uint8_t              min_val;    ///< min value to generate THR
    vs_uint8_t              max_val;    ///< max value to generate THR
} vs_ive_sad_cfg_s;

/*
* @brief the line of the polar coordinate
*/
typedef struct {
    float rho;    ///< line's rho value in polar
    float theta;  ///< line's theta value in polar
}vs_ive_line_polar_s;

/*
* @brief the lines of the houghline
*/
typedef struct {
    vs_int32_t          line_num;       ///< valid num of lines
    vs_ive_line_polar_s p_lines[128];   ///< lines in polar
}vs_ive_lines_s;

/*
* @brief houghline config
*/
typedef struct {
    vs_uint8_t seg_rho;         ///< houghline rho segment size
    vs_uint8_t seg_theta;       ///< houghline theta segment size
    vs_uint8_t threshold;       ///< houghline threshold
    vs_uint8_t min_theta;       ///< houghline min theta
    vs_uint8_t max_theta;       ///< houghline max theta
    vs_uint8_t max_line_num;    ///< houghline max line num
}vs_ive_houghline_cfg_s;

/**
* @brief the sub-block size of the mocaic
*/
typedef enum vs_ive_mosaic_blk_size
{
    E_IVE_MOSAIC_BLK_SIZE_4 = 0x0,  ///< mosaic block size is 4x4 pixels
    E_IVE_MOSAIC_BLK_SIZE_8,        ///< mosaic block size is 8x8 pixels
    E_IVE_MOSAIC_BLK_SIZE_16,       ///< mosaic block size is 16x16 pixels
    E_IVE_MOSAIC_BLK_SIZE_32,       ///< mosaic block size is 32x32 pixels
    E_IVE_MOSAIC_BLK_SIZE_64,       ///< mosaic block size is 64x64 pixels
    E_IVE_MOSAIC_BLK_SIZE_128,      ///< mosaic block size is 128x128 pixels
    E_IVE_MOSAIC_BLK_SIZE_MAX
}vs_ive_mosaic_blk_size_e;

/**
* @brief the mosaic block
*/
typedef struct vs_ive_mosaic
{
    vs_ive_mosaic_blk_size_e blk_size;  ///< mosaic block size
    vs_uint32_t              alpha;     ///< transparency, 0:completely transparent,100:opaque
    vs_rect_s                rect;      ///< mosaic rectangular box
}vs_ive_mosaic_s;

/**
* @brief the ive tensor data type
*/
typedef struct vs_ive_tensor
{
    vs_uint64_t virt_addr;  ///< virtual address of the tensor object
    vs_uint64_t phys_addr;  ///< physical address of the tensor object
    vs_uint32_t mem_size;   ///< the memory size of the data buffer of the tensor
    vs_char_t   data_type;  ///< the data type defined in vs_ive_tensor_data_type_e
    vs_char_t   elem_bits;  ///< the bit number of the data element
    vs_uint16_t zero_point; ///< the quantization zero point
    vs_float_t  scale;      ///< the quantization scale
    vs_int32_t  dim_num;    ///< the number of the data dimension, [1, 6]
    vs_int32_t  shape[6];   ///< the shape of the corresponding dimension
    vs_int32_t  stride[6];  ///< the stride of the corresponding dimension, unit: byte
} vs_ive_tensor_s;

/**
* @brief the glcm compress level
*/
typedef enum vs_ive_glcm_compress {
    E_IVE_GLCM_COMPRESS_4 = 0x0,  ///< glcm src image gray level compress to 4
    E_IVE_GLCM_COMPRESS_8,        ///< glcm src image gray level compress to 8
    E_IVE_GLCM_COMPRESS_16,       ///< glcm src image gray level compress to 16
    E_IVE_GLCM_COMPRESS_MAX
}vs_ive_glcm_compress_e;

/**
 * @brief the glcm scan direction
 */
typedef enum vs_ive_glcm_scan {
    E_IVE_GLCM_SCAN_0   = 1,  ///< glcm scan mode:   0 degree
    E_IVE_GLCM_SCAN_45  = 2,  ///< glcm scan mode:  45 degree
    E_IVE_GLCM_SCAN_90  = 4,  ///< glcm scan mode:  90 degree
    E_IVE_GLCM_SCAN_135 = 8,  ///< glcm scan mode: 135 degree
    E_IVE_GLCM_SCAN_MAX
}vs_ive_glcm_scan_e;

/**
* @brief the glcm scan window size
*/
typedef enum vs_ive_glcm_win_size {
    E_IVE_GLCM_WIN_5X5 = 0x0,  ///< glcm slide window size: 5x5
    E_IVE_GLCM_WIN_7X7,        ///< glcm slide window size: 7x7
    E_IVE_GLCM_WIN_MAX
}vs_ive_glcm_win_size_e;

/**
* @brief the glcm config
*/
typedef struct vs_ive_glcm_cfg {
    vs_ive_glcm_compress_e  compress_mode;   ///< glcm gray compress mode
    vs_ive_glcm_win_size_e  win_size;        ///< glcm slide window size
    vs_int32_t              scan_mode;       ///< glcm gray compress mode
    vs_bool_t               is_symmetric;    ///< glcm matrix is symmetric
    vs_bool_t               out_energy;      ///< glcm output enable energy
    vs_bool_t               out_contrast;    ///< glcm output enable contrast
    vs_bool_t               out_entropy;     ///< glcm output enable entropy
    vs_bool_t               out_correlation; ///< glcm output enable correlation
}vs_ive_glcm_cfg_s;

/**
* @brief the output mode of normgrad
*/
typedef enum vs_ive_normgrad_out_mode{
    E_IVE_NORMGRAD_OUT_MODE_BOTH = 0x0,      ///< output both horizontal and vertical gradients
    E_IVE_NORMGRAD_OUT_MODE_HOR,             ///< output horizontal gradient
    E_IVE_NORMGRAD_OUT_MODE_VER,             ///< output vertical gradient
    E_IVE_NORMGRAD_OUT_MODE_PACKAGE,         ///< output package format of horizontal and vertical gradients
    E_IVE_NORMGRAD_OUT_MODE_MAX
}vs_ive_normgrad_out_mode_e;

/**
* @brief the config of normgrad
*/
typedef struct vs_ive_normgrad_cfg
{
    vs_ive_normgrad_out_mode_e mode;        ///< normgrad output mode
    vs_int8_t mask[25];                     ///< filter kernel
    vs_uint8_t norm;                        ///< normalized coefficient[1,13]
}vs_ive_normgrad_cfg_s;

/**
* @brief the mode of imagematting
*/
typedef enum vs_ive_imagematting_mode {
    E_IVE_IMAGEMATTING_MODE_GREEN = 0x0,  ///< green backgroud mode
    E_IVE_IMAGEMATTING_MODE_CYAN,         ///< cyan backgroud mode
    E_IVE_IMAGEMATTING_MODE_ORANGE,       ///< orange backgroud mode
    E_IVE_IMAGEMATTING_MODE_PURPLE,       ///< purple backgroud mode
    E_IVE_IMAGEMATTING_MODE_MAX
} vs_ive_imagematting_mode_e;

/**
* @brief the config of imagematting
*/
typedef struct vs_ive_imagematting_cfg {
    vs_ive_imagematting_mode_e mode;        ///< image matting mode
    vs_float_t max_bg_chroma_factor;        ///< the maximum chroma factor of the background color space, normalized to [0.0, 1.0]
    vs_float_t min_bg_chroma_factor;        ///< the minimal chroma factor of the background color space, normalized to [0.0, 1.0]
    vs_float_t bg_luma_factor;              ///< the luma factor of the background color space, normalized to [0.0, 1.0]
} vs_ive_imagematting_cfg_s;

/**
* @brief the interpolation mode of perspective
*/
typedef enum vs_ive_interpolate_mode {
    E_IVE_INTERPOLATE_MODE_BILINEAR = 0x0,  ///< bilinear interpolation
    E_IVE_INTERPOLATE_MODE_NEAREST,         ///< nearest neighbor interpolation
    E_IVE_INTERPOLATE_MODE_MAX
} vs_ive_interpolate_mode_e;

/**
* @brief the config of perspective
*/
typedef struct vs_ive_persp_cfg {
    vs_ive_interpolate_mode_e mode;     ///< interpolation mode
    vs_uint8_t fill_value[3];           ///< fill the dst image's invalid area with fill value
} vs_ive_persp_cfg_s;
/**
* @brief the union of char type
*/
typedef union vs_ive_8bit
{
	vs_int8_t   s_val;         ///< char
	vs_uint8_t  u_val;         ///< uchar
}vs_ive_8bit_u;
/**
* @brief the mode of threshold_s16
*/
typedef enum vs_ive_threshold_s16_mode {
	E_IVE_THR_S16_MODE_S16_TO_S8_MIN_MED_MAX = 0x0,       ///< S16_TO_S8,median value mode
	E_IVE_THR_S16_MODE_S16_TO_S8_MIN_ORI_MAX,             ///< S16_TO_S8,original value mode
	E_IVE_THR_S16_MODE_S16_TO_U8_MIN_MED_MAX,             ///< S16_TO_U8,median value mode
	E_IVE_THR_S16_MODE_S16_TO_U8_MIN_ORI_MAX,             ///< S16_TO_U8,original value mode
	E_IVE_THR_S16_MODE_MAX,
}vs_ive_threshold_s16_mode_e;
/**
* @brief the cfg of threshold_s16
*/
typedef struct vs_ive_theshold_s16_cfg{
	vs_ive_threshold_s16_mode_e  mode;       ///< conversion mode
	vs_int16_t low_thr;                      ///< The lower bound of thresholding
	vs_int16_t high_thr;                     ///< The upper limit of the thresholding
	vs_ive_8bit_u min_val;                   ///< The minimum value of thresholding
	vs_ive_8bit_u med_val;                   ///< The middle  value of thresholding
	vs_ive_8bit_u max_val;                   ///< The maximum value of thresholding
}vs_ive_threshold_s16_cfg_s;
/**
* @brief the mode of threshold_s16
*/
typedef enum vs_ive_threshold_u16_mode {
    E_IVE_THR_U16_MODE_U16_TO_U8_MIN_MED_MAX = 0,       ///< U16_TO_U8,median value mode
    E_IVE_THR_U16_MODE_U16_TO_U8_MIN_ORI_MAX,           ///< U16_TO_U8,original value mode
    E_IVE_THR_U16_MODE_MAX,
}vs_ive_threshold_u16_mode_e;
/**
* @brief the cfg of threshold_s16
*/
typedef struct vs_ive_threshold_u16_cfg {
    vs_ive_threshold_u16_mode_e  mode;      ///< conversion mode
    vs_uint16_t low_thr;                    ///< The lower bound of thresholding
    vs_uint16_t high_thr;                   ///< The upper limit of the thresholding
    vs_uint8_t min_val;                     ///< The minimum value of thresholding
    vs_uint8_t med_val;                     ///< The middle  value of thresholding
    vs_uint8_t max_val;                     ///< The maximum value of thresholding
}vs_ive_threshold_u16_cfg_s;

/**
 * @brief the cfg of high_freq_extract
 */
typedef struct vs_ive_high_freq_extract_cfg {
    vs_int16_t coef[25];                    ///< coefficient of the 5x5 filter
} vs_ive_high_freq_extract_cfg_s;

/**
 * @brief the cfg of bilateral_filter
 */
typedef struct vs_ive_bilateral_filter_cfg {
    vs_float_t sigma_space;                 ///< sigma space, only support 1.0f
    vs_float_t sigma_color;                 ///< sigma color, [1.f, 18.f]
} vs_ive_bilateral_filter_cfg_s;

#ifdef __cplusplus
}
#endif

#endif
