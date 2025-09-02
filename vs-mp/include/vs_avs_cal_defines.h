/**
* @file vs_avs_defines.h
* @brief Declaraction of avs enumeration and struct
* @details Declaraction of avs enumeration and struct
* @author visinextek
* @date 2023-09-25
* @version v0.1
* @Copyright (C) 2021, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/

#ifndef __VS_AVS_CAL_DEINES_H__
#define __VS_AVS_CAL_DEINES_H__

#include "vs_errno.h"
#include "vs_comm_video.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum vs_avs_projection_mode {
    E_AVS_PROJECTION_EQUIRECTANGULAR = 0,
    E_AVS_PROJECTION_RECTILINEAR,
    E_AVS_PROJECTION_CYLINDRICAL,
    E_AVS_PROJECTION_MAX
} vs_avs_projection_mode_e;

///< Defines the fov angle of the stitching.
typedef struct vs_avs_fov {
    vs_uint32_t fovx;	///< horizontal fov angle of the stitching.
    vs_uint32_t fovy;	///< vertical fov angle of the stitching.
} vs_avs_fov_s;

/**
* @brief Defines the attitude angle of the Stitching output.
* @details These three dimensions: yaw, pitch, roll.
*/
typedef struct vs_avs_rotation {
    vs_int32_t yaw;		///< yaw angle of the stitching output.
    vs_int32_t pitch;	///< pitch angle of the stitching output.
    vs_int32_t roll;	///< roll angle of the stitching output.
} vs_avs_rotation_s;

/**
* @brief Stitching output attribute definition
* @details The center point, fov angle, attitude angle, etc. of the stitching output.
*/
typedef struct vs_avs_output_attr {
    vs_avs_projection_mode_e mode;		///< stitching project mode
    vs_point_s  center;					///< Center point of stitching output.
    vs_avs_fov_s  fov;					///< Fov angle of stitching output.
    vs_avs_rotation_s  ori_rotation;	///< Original attitude angle of stitching output.
    vs_avs_rotation_s  rotation;		///< Attitude angle of stitching output.
} vs_avs_output_attr_s;

typedef struct vs_avs_calibration_attr {

    vs_avs_output_attr_s output_attr;  /// projection mode, rotation, fov.
    vs_int32_t input_height;		///< Height of input picture.
    vs_int32_t input_width;		///< Width of input picture.
    vs_float_t distance;       ///< The distance between the target object and the  lens needs to be stitching.
    vs_int32_t expected_height;  ///Expected height of stitching output image.
} vs_avs_calibration_attr_s;

typedef struct vs_avs_calibration_output_info {
	vs_int32_t output_width;  ///< single width of output picture.
	vs_int32_t output_height;  ///< height of output picture.
	vs_int32_t width_gap;    ///< width gap of output picture.
} vs_avs_calibration_output_info_s;


#ifdef __cplusplus
}
#endif

#endif /* __VS_AVS_DEINES_H__ */
