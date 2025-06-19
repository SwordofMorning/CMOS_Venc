/**
* @file vs_mal_isp_img_sensor.h
* @brief Declaration of sensor api and struct
* @details Declaration of sensor api and struct
* @author VS isp group
* @date 2022-01-01
* @version v0.10
* @Copyright (C) 2022, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/


#ifndef __VS_MAL_ISP_IMG_SENSOR_H__
#define __VS_MAL_ISP_IMG_SENSOR_H__
#include "vs_comm.h"
#include "vs_comm_video.h"
#include "vs_isp_defines.h"
#ifdef __cplusplus
extern "C"{
#endif

#define VS_ISP_IMG_SENSOR_MAX_REGISTER          (32)
#define VS_ISP_IMG_SENSOR_PRIVATE_DATA_NUM      (4)
/**
 * @brief Defines the sensor bus type.
 * @details
 */
typedef enum vs_isp_img_sensor_bus_type {
    E_IMG_SENSOR_I2C_TYPE = 0,          ///< sensor use i2c bus
    E_IMG_SENSOR_SPI_TYPE,              ///< sensor use spi bus
    E_IMG_SENSOR_TYPE_MAX,
} vs_isp_img_sensor_bus_type_e;

/**
* @brief Define sensor communication bus attributes.
* @details
*/
typedef union vs_isp_img_sensor_bus_attr {
    vs_int8_t   i2c_dev;                 ///< sensor i2c device id
    struct {
        vs_int8_t  spi_dev       : 4;    ///< sensor spi device id
        vs_int8_t  spi_chip      : 4;    ///< sensor spi chip id
    } spi_dev;
} vs_isp_img_sensor_bus_attr_u;

/**
* @brief Define sensor mode.
* @details
*/
typedef struct vs_isp_img_sensor_mode {
    vs_uint8_t  sensor_mode;    ///< sensor mode
    vs_float_t  fps;            ///< frame per second
    vs_uint16_t width;          ///< image width
    vs_uint16_t height;         ///< image height
} vs_isp_img_sensor_mode_s;

/**
 * @brief Defines the sensor i2c register.
 * @details
 */
typedef struct vs_isp_img_sensor_i2c_data {
	vs_bool_t    modified_flag;             ///< VS_TRUE: this register has been modified; VS_FALSE: this register has not been modified (comparing with the last frame)
	vs_uint8_t   delay_frame;               ///< delayed frames before the configuration take effect
	vs_uint8_t   effect_pos;                ///< position where the configuration take effect
	vs_uint8_t   device_addr;               ///< sensor device address
	vs_uint8_t   config_vc_id;		        /* 0 :config register at long frame interrupt, 1: config register for short frame interrupt */
	vs_uint32_t  reg_addr;                  ///< sensor register address
	vs_uint32_t  addr_size;                 ///< sensor register address size
	vs_uint32_t  reg_value;                 ///< sensor register data
	vs_uint32_t  value_size;                ///< sensor register data size
} vs_isp_img_sensor_i2c_data_s;

/**
 * @brief Defines the sensor spi register.
 * @details
 */
typedef struct vs_isp_img_sensor_spi_data {
	vs_bool_t    modified_flag;
	vs_uint8_t   delay_frame;
	vs_uint8_t   effect_pos;
	vs_uint8_t   device_addr;
	vs_uint8_t   config_vc_id;		/* 0 :config register at long frame interrupt, 1: config register for short frame interrupt */
	vs_uint32_t  reg_addr;
	vs_uint32_t  addr_size;
	vs_uint32_t  reg_value;
	vs_uint32_t  value_size;

} vs_isp_img_sensor_spi_data_s;

typedef struct vs_isp_img_sensor_private_data {
	vs_bool_t enable;
	vs_int32_t strobeid;
	vs_uint32_t pulse_width;
	vs_uint32_t pulse_delay;
} vs_isp_img_sensor_private_data_s;

/**
* @brief Define sensor register information.
* @details
*/
typedef struct vs_isp_img_sensor_register_info {
    vs_bool_t                   complete_flag;                                  ///< VS_TRUE: Register configuration completed; VS_FALSE: Register configuration not completed
    vs_uint32_t                 reg_num;                                        ///< sensor register number for exposure time configuration
    vs_uint8_t                  delay_max;                                      ///< sensor register delay maximum
    vs_isp_img_sensor_bus_type_e    bus_type;                                       ///< sensor type
    vs_isp_img_sensor_bus_attr_u    bus_attr;                                       ///< sensor bus attribute
    union {
        vs_isp_img_sensor_i2c_data_s i2c_data[VS_ISP_IMG_SENSOR_MAX_REGISTER];          ///< sensor i2c data
        vs_isp_img_sensor_spi_data_s spi_data[VS_ISP_IMG_SENSOR_MAX_REGISTER];          ///< sensor spi data
    };
    vs_isp_img_sensor_private_data_s private_data[VS_ISP_IMG_SENSOR_PRIVATE_DATA_NUM];                   ///
} vs_isp_img_sensor_register_info_s;

/**
* @brief Define auto exposure default information.
* @details
*/
typedef struct vs_isp_img_sensor_info {
    vs_uint32_t   again_max;              /* 8 bit accuracy */
    vs_uint32_t   dgain_max;              /* 8 bit accuracy */
    vs_uint32_t   integration_time_min;   /* unit: lines */
    vs_uint32_t   integration_time_max;   /* unit: lines */
    vs_uint32_t   integration_time_limit;  /* unit: lines */
    vs_uint32_t   integration_time_long_max;  /* unit: lines */
    vs_uint32_t   lines_per_second;          /* unit: lines */
    vs_uint32_t   exposure_ratio_max; /* 6 bit accuracy */
} vs_isp_img_sensor_info_s;


typedef struct vs_isp_img_sensor_ae_exposure_attr {
    vs_uint32_t   shut_time[VS_ISP_AE_EXPOSURE_NUM_MAX];  /* unit: lines */
    vs_uint32_t   again;                        /* 8 bit accuracy */
    vs_uint32_t   dgain;                        /* 8 bit accuracy */
    vs_uint32_t   isp_dgain;                    /* 8 bit accuracy */
} vs_isp_img_sensor_ae_exposure_attr_s;

typedef struct vs_isp_alg_default_attr {
	vs_isp_statistics_cfg_s *p_statistics_default;
	vs_isp_black_level_attr_s *p_black_level_default;
	vs_isp_green_equal_attr_s *p_green_equal_default;
	vs_isp_dynamic_dp_attr_s *p_dynamic_dpc_default;
	vs_isp_static_dp_attr_s *p_static_dpc_default;
	vs_isp_nr2d_attr_s *p_nr2d_default;
	vs_isp_cac_attr_s *p_cac_default;
	vs_isp_radial_shading_attr_s *p_radial_shading_default;
	vs_isp_mesh_shading_attr_s *p_mesh_shading_default;
	vs_isp_dra_attr_s *p_dra_default;
	vs_isp_demosaic_attr_s *p_demosaic_default;
	vs_isp_pfc_attr_s *p_pfc_default;
	vs_isp_cnr_attr_s *p_cnr_default;
	vs_isp_color_matrix_attr_s *p_color_matrix_default;
	vs_isp_lut3d_mem_attr_s *p_lut3d_default;
	vs_isp_gamma_attr_s *p_gamma_default;
	vs_isp_rgb_sharpen_attr_s *p_rgb_sharpen_default;
	vs_isp_csc_attr_s *p_csc_default;
	vs_isp_bypass_setting_s *p_bypass_setting;
	vs_isp_wdr_stitch_attr_s *p_wdr_stitch_default;
	vs_isp_decompander_attr_s *p_decompander_default;
	vs_isp_ae_attr_s *p_ae_default;
	vs_isp_awb_attr_s *p_awb_default;
	vs_isp_wdr_exp_attr_s *p_wdr_exp_default;
	vs_isp_presharpen_attr_s *p_presharpen_default;
	vs_isp_nr3d_attr_s *p_nr3d_default;
	vs_isp_postsharpen_attr_s *p_postsharpen_default;
	vs_isp_yuv_pfc_attr_s *p_yuv_pfc_default;
	vs_isp_sa_attr_s *p_yuv_sa_default;
	vs_isp_coloring_attr_s *p_yuv_coloring_default;
	vs_isp_yuv_af_attr_s *p_yuv_af_default;
	vs_isp_color_shading_attr_s *p_color_shading_default;
} vs_isp_alg_default_attr_s;

typedef struct vs_isp_img_sensor_ops {
    /**
    * @brief  Initialize a sensor by pipe id.
    * @details Initialize a sensor by pipe id.
    * @param [in]  vii_pipe Indicate the pipe id to be given.
    * @return   0 call success.
    * @warning   none0 call failed. see ERROR_CODE for detail.
    * @par
    *
    */
    vs_int32_t (*init)(vs_int32_t vii_pipe);

    /**
    * @brief  Deinitialize a sensor by pipe id.
    * @details Deinitialize a sensor by pipe id.
    * @param [in]  vii_pipe Indicate the pipe id to be given.
    * @return   0 call success.
    * @warning   none0 call failed. see ERROR_CODE for detail.
    * @par
    *
    */
    vs_int32_t (*deinit)(vs_int32_t vii_pipe);

    /**
    * @brief  Initialize sensor global variables by pipe id.
    * @details Initialize sensor global variables by pipe id.
    * @param [in]  vii_pipe Indicate the pipe id to be given.
    * @return   0 call success.
    * @warning   none0 call failed. see ERROR_CODE for detail.
    * @par
    *
    */
    vs_int32_t (*global_init)(vs_int32_t vii_pipe);

    /**
    * @brief  Switch sensor resolution and fps by pipe id and sensor image mode.
    * @details Switch sensor resolution and fps by pipe id and sensor image mode.
    * @param [in]  vii_pipe Indicate the pipe id to be given.
    * @param [in]  p_mode The pointer of sensor image mode, can not be NULL.
    * @return   0 call success.
    * @warning   none0 call failed. see ERROR_CODE for detail.
    * @par
    *
    */
    vs_int32_t (*mode_set)(vs_int32_t vii_pipe, vs_isp_img_sensor_mode_s *p_mode);

    /**
    * @brief  Set sensor wdr mode by pipe id and wdr mode.
    * @details Set sensor wdr mode by pipe id and wdr mode.
    * @param [in]  vii_pipe Indicate the pipe id to be given.
    * @param [in]  mode Indicate the mode to be given.
    * @return   0 call success.
    * @warning   none0 call failed. see ERROR_CODE for detail.
    * @par
    *
    */
    vs_int32_t (*wdr_mode_set)(vs_int32_t vii_pipe,  vs_wdr_mode_e mode);

    /**
    * @brief  Get sensor register information by pipe id.
    * @details Get sensor register information by pipe id.
    * @param [in]  vii_pipe Indicate the pipe id to be given.
    * @param [out]  p_register_info The pointer of struct sensor register information, can not be NULL.
    * @return   0 call success.
    * @warning   none0 call failed. see ERROR_CODE for detail.
    * @par
    *
    */
    vs_int32_t (*register_info_get)(vs_int32_t vii_pipe, vs_isp_img_sensor_register_info_s *p_register_info);

	/**
    * @brief  Set sensor fps by pipe id and fps.
    * @details Set sensor fps by pipe id and fps.
    * @param [in]  vii_pipe Indicate the pipe id to be given.
    * @param [in]  fps Indicate the fps to be given.
    * @param [out]  p_default The pointer of struct ae default parameters, can not be NULL.
    * @return   0 call success.
    * @warning   none0 call failed. see ERROR_CODE for detail.
    * @par
    *
    */
    vs_int32_t (*fps_set)(vs_int32_t vii_pipe, vs_float_t fps);

    /**
    * @brief  Update sensor integration time by pipe id, and integration time.
    * @details Update sensor integration time by pipe id, and integration time.
    * @param [in]  vii_pipe Indicate the pipe id to be given.
    * @param [in]  int_time The pointer of integration time, can not be NULL.
    * @param [in]  int_time_num Indicate the number of integration time to be given.
    * @return   0 call success.
    * @warning   none0 call failed. see ERROR_CODE for detail.
    * @par
    *
    */
    vs_int32_t (*inttime_update)(vs_int32_t vii_pipe, vs_uint32_t *int_time, vs_uint8_t int_time_num);

    /**
    * @brief  Update sensor analog gains by pipe id.
    * @details Update sensor analog gains by pipe id.
    * @param [in]  vii_pipe Indicate the pipe id to be given.
    * @param [in]  again Indicate the analog gain to be given.
    * @return   0 call success.
    * @warning   none0 call failed. see ERROR_CODE for detail.
    * @par
    *
    */
    vs_int32_t (*again_update)(vs_int32_t vii_pipe, vs_uint32_t *again);

    /**
    * @brief  Update sensor digital gains by pipe id.
    * @details Update sensor digital gains by pipe id.
    * @param [in]  vii_pipe Indicate the pipe id to be given.
    * @param [in]  dgain Indicate the digital gain to be given.
    * @return   0 call success.
    * @warning   none0 call failed. see ERROR_CODE for detail.
    * @par
    *
    */
    vs_int32_t (*dgain_update)(vs_int32_t vii_pipe, vs_uint32_t *dgain);

	/**
    * @brief  Update sensor information  by pipe id.
    * @details Update sensor information by pipe id.
    * @param [in]  vii_pipe Indicate the pipe id to be given.
    * @param [in]  p_sensor_info Indicate the digital gain to be given.
    * @return   0 call success.
    * @warning   none0 call failed. see ERROR_CODE for detail.
    * @par
    *
    */
    vs_int32_t (*sensor_info_get)(vs_int32_t vii_pipe, vs_isp_img_sensor_info_s* p_sensor_info);

	/**
    * @brief  Update isp algorithm default value  by pipe id.
    * @details Update isp algorithm default value by pipe id.
    * @param [in]  vii_pipe Indicate the pipe id to be given.
    * @param [in]  p_alg_default Indicate the isp algorithm default value to be get.
    * @return   0 call success.
    * @warning   none0 call failed. see ERROR_CODE for detail.
    * @par
    *
    */
    vs_int32_t (*isp_alg_default_get)(vs_int32_t vii_pipe, vs_isp_alg_default_attr_s *p_alg_default);

    /**
    * @brief  get ae initial exposure information  by pipe id.
    * @details get ae initial exposure information by pipe id.
    * @param [in]  vii_pipe Indicate the pipe id to be given.
    * @param [in]  p_initial_exposure_info Indicate the iniital exposure information to be get.
    * @return   0 call success.
    * @warning   none0 call failed. see ERROR_CODE for detail.
    * @par
    *
    */
    vs_int32_t (*ae_initial_exposure_get)(vs_int32_t vii_pipe,
										  vs_isp_img_sensor_ae_exposure_attr_s *p_initial_exposure_attr);

    /**
    * @brief  get sensor standby info  by pipe id.
    * @details get sensor standby info  by pipe id
    * @param [in]  vii_pipe Indicate the pipe id to be given.
    * @param [out]  p_standby_info Indicate the standby information to be get.
    * @param [out]  p_stream_info Indicate the stream information to be get.
    * @return   0 call success.
    * @warning   none0 call failed. see ERROR_CODE for detail.
    * @par
    *
    */
    vs_int32_t (*sensor_standby_info_get)(vs_int32_t vii_pipe,
            vs_isp_img_sensor_register_info_s* p_standby_info,
            vs_isp_img_sensor_register_info_s* p_stream_info);

} vs_isp_img_sensor_ops_s;


#ifdef __cplusplus
}
#endif
#endif /*!<  _VS_MAL_IMG_SENSOR_H_ */
