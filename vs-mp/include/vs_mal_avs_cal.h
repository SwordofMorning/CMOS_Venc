/**
* @file vs_mal_avs_cal.h
* @brief Declaraction of avs mal interface
* @details  Declaraction of avs mal interface
* @author visinextek
* @date 2022-08-27
* @version v0.1
* @Copyright (C) 2021, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/

#ifndef __VS_MAL_AVS_CAL_H__
#define __VS_MAL_AVS_CAL_H__

#include "vs_errno.h"
#include "vs_comm_video.h"
#include "vs_avs_cal_defines.h"

#ifdef __cplusplus
extern "C" {
#endif
/**
* @brief	   Calibration is required before image stitching.
* @details	   Calibration is required before image stitching.
* @param [in]  input_file Model calibration parameters.
* @param [in]  p_cal_attr Input attributes for machine calibration to be calibrated.
* @param [out] ouput_file Calibration Camera Lookup Table.
* @param [out] p_cal_output Calibration output attributes.
* @return	   0 call success.
* @warning	   none 0 call failed. see ERROR_CODE for detail.
* @par
*/
vs_int32_t vs_mal_avs_calibrate(char *input_file, char *ouput_file, vs_avs_calibration_attr_s *p_cal_attr,  vs_avs_calibration_output_info_s * p_cal_output);

#ifdef __cplusplus
}
#endif

#endif /* __VS_MAL_AVS_CAL_H__ */
