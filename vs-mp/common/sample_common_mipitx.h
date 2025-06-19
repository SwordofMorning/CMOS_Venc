/**
* @file sample_common_mipitx.h
* @brief sample common interface for mipitx
* @details contains APIs
* @author Visinex Software Group
* @date 2022-05-25
* @version v0.10
* @Copyright (C) 2022, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/

#ifndef _SAMPLE_COMMON_MIPITX__H_
#define _SAMPLE_COMMON_MIPITX__H_

#include "vs_comm.h"

#include "vs_mal_vo.h"
#include "vs_mal_mipi_tx.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum mipitx_board_type {
#ifdef VS_ORION
	MIPITX_BOARD_VS819L_OI01,
	MIPITX_BOARD_VS819L_OI02,
#else
	MIPITX_BOARD_VS8X9_CI01,
	MIPITX_BOARD_VS8X9_CI02,
	MIPITX_BOARD_VS8X9_CI03,
	MIPITX_BOARD_VS909,
#endif
	MIPITX_BOARD_TYPE_MAX,
} mipitx_board_type_e;

int mipitx_board_setting_get(char *argv);
int sample_common_mipitx_start(vs_vo_output_type_e vo_output);
int sample_common_mipitx_stop();
char *sample_common_mipitx_board_type_get(mipitx_board_type_e board_type);

#ifdef __cplusplus
}
#endif

#endif ///< end of _SAMPLE_COMMON_MIPITX_H_
