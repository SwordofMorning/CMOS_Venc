/**
 * @file vs_mal_isp.h
 * @brief Declaration of isp enumeration and struct
 * @details Declaration of isp enumeration and struct
 * @author VS isp group
 * @date 2022-1-29
 * @version v0.10
 * @Copyright (C) 2022, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
 */


#ifndef __VS_MAL_ISP_H__
#define __VS_MAL_ISP_H__

#include "vs_type.h"
#include "vs_comm.h"
#include "vs_comm_video.h"
#include "vs_isp_defines.h"
#include "vs_mal_isp_img_sensor.h"
#ifdef __cplusplus
extern "C"{
#endif

/**
 * @brief  Initial isp memory by pipe id.
 * @details Initial isp memory by pipe id.
 * @param [in]  vii pipe Indicate the pipe id to be given.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_mem_init(vs_int32_t vii_pipe);

/**
 * @brief  Initial isp algorithm by pipe id.
 * @details Initial isp algorithm by pipe id.
 * @param [in]  vii pipe Indicate the pipe id to be given.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_init(vs_int32_t vii_pipe);

/**
 * @brief   Start run isp algorithm by pipe id.
 * @details The func will not return untill call vs_mal_isp_stop. you should call this function in a thread.
 * @param [in]  vii pipe Indicate the pipe id to be given.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_run(vs_int32_t vii_pipe);

/**
 * @brief   Stop isp algorithm by pipe id.
 * @details Stop isp algorithm by pipe id.
 * @param [in]  vii pipe Indicate the pipe id to be given.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_stop(vs_int32_t vii_pipe);

/**
 * @brief   Run isp algorithm one time by pipe id.
 * @details All isp algorithm just run once
 * @param [in]  vii pipe Indicate the pipe id to be given.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_runonce(vs_int32_t vii_pipe);

/**
 * @brief   Deinit isp algorithm  by pipe id.
 * @details Deinit isp algorithm  by pipe id.
 * @param [in]  vii pipe Indicate the pipe id to be given.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_deinit(vs_int32_t vii_pipe);

/**
 * @brief  Register image sensor lib to pipe.
 * @details Register image sensor lib to pipe.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [in]  p_isp_ops Indicate the image sensor lib info
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_img_sensor_register(vs_int32_t vii_pipe, vs_isp_img_sensor_ops_s *p_isp_ops);

/**
 * @brief  Set active auto exposure lib and auto white balance lib.
 * @details Set active auto exposure lib and auto white balance lib.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [in]  p_bind_attr Indicate the bind attributes.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_bind_attr_set(vs_int32_t vii_pipe, const vs_isp_bind_attr_s *p_bind_attr);

/**
 * @brief  Get the active auto exposure lib and auto white balance lib.
 * @details Get the active auto exposure lib and auto white balance lib.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [in]  p_bind_attr Indicate the bind attributes.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_bind_attr_get(vs_int32_t vii_pipe, vs_isp_bind_attr_s *p_bind_attr);

/**
 * @brief  Register auto exposure lib to pipe.
 * @details Register auto exposure lib to pipe.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [in]  p_aelib Indicate the auto exposure lib attributes.
 * @param [in]  p_ae_ops Indicate the auto exposure lib operation functions.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_aelib_register(vs_int32_t vii_pipe, vs_isp_alg_lib_s *p_aelib, vs_isp_aelib_ops_s *p_ae_ops);

/**
 * @brief  Register auto white balance lib to pipe.
 * @details Register auto white balance lib to pipe.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [in]  p_awblib Indicate the auto white balance lib attributes.
 * @param [in]  p_awb_ops Indicate the auto white balance lib operation functions.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_awblib_register(vs_int32_t vii_pipe, vs_isp_alg_lib_s *p_awblib, vs_isp_awblib_ops_s *p_awb_ops);

/**
 * @brief  unregister auto exposure lib to pipe.
 * @details unregister auto exposure lib to pipe.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [in]  p_aelib Indicate the auto exposure lib attributes.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_aelib_unregister(vs_int32_t vii_pipe, vs_isp_alg_lib_s *p_aelib);

/**
 * @brief  Register auto white balance lib to pipe.
 * @details Register auto white balance lib to pipe.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [in]  p_awblib Indicate the auto white balance lib attributes.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_awblib_unregister(vs_int32_t vii_pipe, vs_isp_alg_lib_s *p_awblib);

/**
 * @brief  Set isp statistics update interval.
 * @details Set isp statistics update interval.
 * @param [in]  vii_pipe Indicate the pipe id to be set.
 * @param [in]  update_interval Indicate the update interval.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_statistics_update_interval_set(vs_int32_t vii_pipe, vs_uint32_t update_interval);

/**
 * @brief  Get the isp statistics update interval.
 * @details Get the isp statistics update interval.
 * @param [in]  vii_pipe Indicate the pipe id to be get.
 * @param [in]  p_update_interval indicate the update interval.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_statistics_update_interval_get(vs_int32_t vii_pipe, vs_uint32_t *p_update_interval);

/**
 * @brief set isp stitch group attr.
 * @details set isp stitch group attr.
 * @param [in] stitch_group Indicate the stitch group id to be register.
 * @param [in] p_isp_stitch_attr Indicate the isp stitch attr set.
 * @return 0 call success.
 * @warning none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_stitch_group_attr_set(vs_int32_t stitch_group, const vs_isp_stitch_group_attr_s *p_isp_stitch_attr);

/**
 * @brief get isp stitch group attr.
 * @details get isp stitch group attr.
 * @param [in] stitch_group Indicate the stitch group id to be register.
 * @param [out] p_isp_stitch_attr Indicate the isp stitch attr get.
 * @return 0 call success.
 * @warning none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_stitch_group_attr_get(vs_int32_t stitch_group, vs_isp_stitch_group_attr_s *p_isp_stitch_attr);

/**
 * @brief get isp stitch group ae statistics.
 * @details get isp stitch group ae statistics.
 * @param [in] stitch_group Indicate the stitch group id to be register.
 * @param [out] p_ae_statistics Indicate the isp stitch group ae statistics get.
 * @return 0 call success.
 * @warning none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_ae_stitch_statistics_get(vs_int32_t stitch_group, vs_isp_ae_stitch_statistics_s *p_ae_statistics);

/**
 * @brief get isp stitch group awb statistics.
 * @details get isp stitch group awb statistics.
 * @param [in] stitch_group Indicate the stitch group id to be register.
 * @param [out] p_awb_statistics Indicate the isp stitch group awb statistics get.
 * @return 0 call success.
 * @warning none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_awb_stitch_statistics_get(vs_int32_t stitch_group, vs_isp_awb_stitch_statistics_s *p_awb_statistics);

/**
 * @brief   Isp algorithm common attributes set
 * @details Isp algorithm common attributes set
 * @param [in]  vii pipe Indicate the pipe id to be given.
 * @param [in]  p_comm_attr The pointer of common algorithm attributes, can not be NULL.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_comm_attr_set(vs_int32_t vii_pipe, const vs_isp_comm_attr_s *p_comm_attr);

/**
 * @brief   Isp algorithm common attributes get
 * @details Isp algorithm common attributes get
 * @param [in]  vii pipe Indicate the pipe id to be given.
 * @param [out]  p_comm_attr The pointer of common algorithm attributes, can not be NULL.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_comm_attr_get(vs_int32_t vii_pipe, vs_isp_comm_attr_s *p_comm_attr);

/**
 * @brief   wait isp interrupt
 * @details this function will block untill isp receive a interrupt set by user
 * @param [in]  vii pipe Indicate the pipe id to be given.
 * @param [in]  isp_wait_type Indicate the isp interrupt wait type
 * @param [in]  timeout_ms Indicate wait timeout value
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_interrupt_wait(vs_int32_t vii_pipe, vs_isp_wait_type_e isp_wait_type, vs_int32_t timeout_ms);

/**
 * @brief   Set isp fareware state
 * @details Set isp fareware state, run or stop
 * @param [in]  vii pipe Indicate the pipe id to be given.
 * @param [in]  state Indicate the state to be set.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_fw_state_set(vs_int32_t vii_pipe, vs_isp_fw_state_e state);

/**
 * @brief   Get isp fareware state
 * @details Get isp fareware state, run or stop
 * @param [in]  vii pipe Indicate the pipe id to be given.
 * @param [out]  p_state Indicate the state to be get.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_fw_state_get(vs_int32_t vii_pipe, vs_isp_fw_state_e *p_state);


/**
 * @brief  get the ae statistics.
 * @details get the ae statistics.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [out]  p_ae_statistics Indicate the ae statistics.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_ae_statistics_get(vs_int32_t vii_pipe, vs_isp_ae_statistics_s *p_ae_statistics);


/**
 * @brief  get the awb statistics.
 * @details get the awb statistics.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [out]  p_awb_statistics Indicate the awb statistics.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_awb_statistics_get(vs_int32_t vii_pipe, vs_isp_awb_statistics_s *p_awb_statistics);


/**
 * @brief  get the focus statistics.
 * @details get the focus statistics.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [in]  p_af_statistics Indicate the focus statistics.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_af_statistics_get(vs_int32_t vii_pipe, vs_isp_af_statistics_s *p_af_statistics);

/**
 * @brief  get the luma statistics.
 * @details get the luma statistics.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [out]  p_luma_var_statistics Indicate the luma statistics.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_luma_var_statistics_get(vs_int32_t vii_pipe, vs_isp_luma_var_statistics_s *p_luma_var_statistics);

/**
 * @brief  set the statistics configure.
 * @details set the statistics configure.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [in]  p_statistic_cfg Indicate the statistics configure.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_statistics_cfg_set(vs_int32_t vii_pipe, const vs_isp_statistics_cfg_s *p_statistic_cfg);

/**
 * @brief  get the statistics configure.
 * @details get the statistics configure.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [out]  p_statistic_cfg Indicate the statistics configure.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_statistics_cfg_get(vs_int32_t vii_pipe, vs_isp_statistics_cfg_s *p_statistic_cfg);

/**
 * @brief  set the ae statistics configure.
 * @details set the ae statistics configure.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [in]  p_ae_statistic_cfg Indicate the ae statistics configure.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_ae_statistics_cfg_set(vs_int32_t vii_pipe, const vs_isp_ae_statistics_cfg_s *p_ae_statistic_cfg);

/**
 * @brief  get the ae statistics configure.
 * @details get the ae statistics configure.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [out]  p_ae_statistic_cfg Indicate the statistics configure.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_ae_statistics_cfg_get(vs_int32_t vii_pipe, vs_isp_ae_statistics_cfg_s *p_ae_statistic_cfg);

/**
 * @brief  set the awb statistics configure.
 * @details set the awb statistics configure.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [in]  p_awb_statistic_cfg Indicate the ae statistics configure.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_awb_statistics_cfg_set(vs_int32_t vii_pipe, const vs_isp_awb_statistics_cfg_s *p_awb_statistic_cfg);

/**
 * @brief  get the awb statistics configure.
 * @details get the awb statistics configure.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [out]  p_awb_statistic_cfg Indicate the statistics configure.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_awb_statistics_cfg_get(vs_int32_t vii_pipe, vs_isp_awb_statistics_cfg_s *p_awb_statistic_cfg);
/**
 * @brief  set the af statistics configure.
 * @details set the af statistics configure.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [in]  p_af_statistic_cfg Indicate the ae statistics configure.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_af_statistics_cfg_set(vs_int32_t vii_pipe, const vs_isp_af_raw_statistics_cfg_s *p_af_statistic_cfg);

/**
 * @brief  get the af statistics configure.
 * @details get the af statistics configure.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [out]  p_af_statistic_cfg Indicate the statistics configure.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_af_statistics_cfg_get(vs_int32_t vii_pipe, vs_isp_af_raw_statistics_cfg_s *p_af_statistic_cfg);

/**
 * @brief  set the hard ware register.
 * @details set the hard ware register.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [in]  reg_addr Indicate the register address.
 * @param [in]  reg_value Indicate the register value to be set.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_register_set(vs_int32_t vii_pipe, vs_uint32_t reg_addr, vs_uint32_t reg_value);

/**
 * @brief  get the hard ware register.
 * @details get the hard ware register.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [in]  reg_addr Indicate the register address.
 * @param [out]  p_reg_value Indicate the register value.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_register_get(vs_int32_t vii_pipe, vs_uint32_t reg_addr, vs_uint32_t *p_reg_value);

/**
 * @brief  set the slave sensor attribution.
 * @details set the slave sensor attribution.
 * @param [in]  slave_dev_id indicate the slave dev id.
 * @param [in]  p_attr indicate the attribution of slave device.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_slave_dev_attr_set(vs_int32_t slave_dev_id, const vs_isp_slave_dev_attr_s *p_attr);

/**
 * @brief  get the slave sensor attribution.
 * @details get the slave sensor attribution.
 * @param [in]  slave_dev_id indicate the slave dev id.
 * @param [in]  p_attr indicate the attribution of slave sensor.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_slave_dev_attr_get(vs_int32_t slave_dev_id, vs_isp_slave_dev_attr_s *p_attr);

/**
 * @brief  set the black level attributes.
 * @details set the black level attributes.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [in]  p_black_level_attr Indicate the black level attributes.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_black_level_attr_set(vs_int32_t vii_pipe, const vs_isp_black_level_attr_s *p_black_level_attr);

/**
 * @brief  get the black level attributes.
 * @details get the black level attributes.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [out]  p_black_level_attr Indicate the black level attributes.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_black_level_attr_get(vs_int32_t vii_pipe, vs_isp_black_level_attr_s *p_black_level_attr);

/**
 * @brief  set the green equal attributes.
 * @details set the green equal attributes.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [in]  p_green_equal_attr Indicate the green equal attributes.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_green_equal_attr_set(vs_int32_t vii_pipe, const vs_isp_green_equal_attr_s *p_green_equal_attr);

/**
 * @brief  get the green equal attributes.
 * @details get the green equal attributes.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [out]  p_green_equal_attr Indicate the green equal attributes.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_green_equal_attr_get(vs_int32_t vii_pipe, vs_isp_green_equal_attr_s *p_green_equal_attr);

/**
 * @brief  set the dynamic defective pixel correction attributes.
 * @details set the dynamic defective pixel correction attributes.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [in]  p_dynamic_dp_attr Indicate the dynamic defective pixel correction attributes.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_dynamic_dp_attr_set(vs_int32_t vii_pipe, const vs_isp_dynamic_dp_attr_s *p_dynamic_dp_attr);

/**
 * @brief  get the dynamic defective pixel correction attributes.
 * @details get the dynamic defective pixel correction attributes.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [out]  p_dynamic_dp_attr Indicate the dynamic defective pixel correction attributes.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_dynamic_dp_attr_get(vs_int32_t vii_pipe, vs_isp_dynamic_dp_attr_s *p_dynamic_dp_attr);

/**
 * @brief  set the static defective pixel correction attributes.
 * @details set the static defective pixel correction attributes.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [in]  p_static_dp_attr Indicate the static defective pixel correction attributes.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_static_dp_attr_set(vs_int32_t vii_pipe, const vs_isp_static_dp_attr_s *p_static_dp_attr);

/**
 * @brief  get the static defective pixel correction attributes.
 * @details get the static defective pixel correction attributes.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [out]  p_static_dp_attr Indicate the static defective pixel correction attributes.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_static_dp_attr_get(vs_int32_t vii_pipe, vs_isp_static_dp_attr_s *p_static_dp_attr);

/**
 * @brief  set the sinter attributes.
 * @details set the sinter attributes.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [in]  p_nr2d_attr Indicate the sinter attributes.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_nr2d_attr_set(vs_int32_t vii_pipe, const vs_isp_nr2d_attr_s *p_nr2d_attr);

/**
 * @brief  get the sinter attributes.
 * @details get the sinter attributes.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [out]  p_nr2d_attr Indicate the sinter attributes.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_nr2d_attr_get(vs_int32_t vii_pipe, vs_isp_nr2d_attr_s *p_nr2d_attr);


/**
 * @brief  set the chromatic aberration correction attributes.
 * @details set the chromatic aberration correction attributes.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [in]  p_cac_attr Indicate the chromatic aberration correction attributes.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_cac_attr_set(vs_int32_t vii_pipe, const vs_isp_cac_attr_s *p_cac_attr);

/**
 * @brief  get the chromatic aberration correction attributes.
 * @details get the chromatic aberration correction attributes.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [out]  p_cac_attr Indicate the chromatic aberration correction attributes.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_cac_attr_get(vs_int32_t vii_pipe, vs_isp_cac_attr_s *p_cac_attr);

/**
 * @brief  set the radial shading correction attributes.
 * @details set the radial shading correction attributes.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [in]  p_radial_shading_attr Indicate the radial shading correction attributes.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_radial_shading_attr_set(vs_int32_t vii_pipe, const vs_isp_radial_shading_attr_s *p_radial_shading_attr);

/**
 * @brief  get the radial shading correction attributes.
 * @details get the radial shading correction attributes.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [out]  p_radial_shading_attr Indicate the radial shading correction attributes.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_radial_shading_attr_get(vs_int32_t vii_pipe, vs_isp_radial_shading_attr_s *p_radial_shading_attr);

/**
 * @brief  set the mesh shading correction attributes.
 * @details set the radial shading correction attributes.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [in]  p_mesh_shading_attr Indicate the mesh shading correction attributes.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_mesh_shading_attr_set(vs_int32_t vii_pipe, const vs_isp_mesh_shading_attr_s *p_mesh_shading_attr);

/**
 * @brief  get the mesh shading correction attributes.
 * @details get the radial shading correction attributes.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [out]  p_mesh_shading_attr Indicate the mesh shading correction attributes.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_mesh_shading_attr_get(vs_int32_t vii_pipe, vs_isp_mesh_shading_attr_s *p_mesh_shading_attr);

/**
 * @brief  set the iridix attributes.
 * @details set the iridix attributes.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [in]  p_dra_attr Indicate the iridix attributes.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_dra_attr_set(vs_int32_t vii_pipe, const vs_isp_dra_attr_s *p_dra_attr);

/**
 * @brief  get the iridix attributes.
 * @details get the iridix attributes.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [out]  p_dra_attr Indicate the iridix attributes.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_dra_attr_get(vs_int32_t vii_pipe, vs_isp_dra_attr_s *p_dra_attr);

/**
 * @brief  set the demosaic attributes.
 * @details set the demosaic attributes.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [in]  p_demosaic_attr Indicate the demosaic attributes.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_demosaic_attr_set(vs_int32_t vii_pipe, const vs_isp_demosaic_attr_s *p_demosaic_attr);

/**
 * @brief  get the demosaic attributes.
 * @details get the demosaic attributes.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [out]  p_demosaic_attr Indicate the demosaic attributes.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_demosaic_attr_get(vs_int32_t vii_pipe, vs_isp_demosaic_attr_s *p_demosaic_attr);

/**
 * @brief  set the purple fringing correction attributes.
 * @details set the demosaic attributes.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [in]  p_pfc_attr Indicate the purple fringing correction attributes.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_pfc_attr_set(vs_int32_t vii_pipe, const vs_isp_pfc_attr_s *p_pfc_attr);

/**
 * @brief  get the purple fringing correction attributes.
 * @details get the demosaic attributes.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [out]  p_pfc_attr Indicate the purple fringing correction attributes.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_pfc_attr_get(vs_int32_t vii_pipe, vs_isp_pfc_attr_s *p_pfc_attr);

/**
 * @brief  set the color correction matrix attributes.
 * @details set the color correction matrix attributes.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [in]  p_color_matrix_attr Indicate the color correction matrix attributes.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_color_matrix_attr_set(vs_int32_t vii_pipe, const vs_isp_color_matrix_attr_s *p_color_matrix_attr);

/**
 * @brief  get the color correction matrix attributes.
 * @details get the color correction matrix attributes.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [out]  p_color_matrix_attr Indicate the color correction matrix attributes.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_color_matrix_attr_get(vs_int32_t vii_pipe, vs_isp_color_matrix_attr_s *p_color_matrix_attr);

/**
 * @brief  set the color noise reduction attributes.
 * @details set the color noise reduction attributes.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [in]  p_cnr_attr Indicate the color noise reduction attributes.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_cnr_attr_set(vs_int32_t vii_pipe, const vs_isp_cnr_attr_s *p_cnr_attr);

/**
 * @brief  get the color noise reduction attributes.
 * @details get the color noise reduction attributes.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [out]  p_cnr_attr Indicate the color noise reduction attributes.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_cnr_attr_get(vs_int32_t vii_pipe, vs_isp_cnr_attr_s *p_cnr_attr);

/**
 * @brief  set the lut3d attributes.
 * @details set the lut3d attributes.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [in]  p_lut3d_mem_attr Indicate the lut3d attributes.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_lut3d_mem_attr_set(vs_int32_t vii_pipe, const vs_isp_lut3d_mem_attr_s *p_lut3d_mem_attr);

/**
 * @brief  get the lut3d attributes.
 * @details get the lut3d attributes.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [out]  p_lut3d_mem_attr Indicate the lut3d attributes.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_lut3d_mem_attr_get(vs_int32_t vii_pipe, vs_isp_lut3d_mem_attr_s *p_lut3d_mem_attr);

/**
 * @brief  set the gamma attributes.
 * @details set the gamma attributes.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [in]  p_gamma_attr Indicate the gamma attributes.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_gamma_attr_set(vs_int32_t vii_pipe, const vs_isp_gamma_attr_s *p_gamma_attr);

/**
 * @brief  set the gamma attributes.
 * @details set the gamma attributes.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [out]  p_gamma_attr Indicate the gamma attributes.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_gamma_attr_get(vs_int32_t vii_pipe, vs_isp_gamma_attr_s *p_gamma_attr);

/**
 * @brief  set the sharpen attributes.
 * @details set the sharpen attributes.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [out]  p_rgb_sharpen_attr Indicate the fr sharpen attributes.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_rgb_sharpen_attr_set(vs_int32_t vii_pipe, const vs_isp_rgb_sharpen_attr_s *p_rgb_sharpen_attr);

/**
 * @brief  get the sharpen attributes.
 * @details get the sharpen attributes.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [out]  p_rgb_sharpen_attr Indicate the fr sharpen attributes.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_rgb_sharpen_attr_get(vs_int32_t vii_pipe, vs_isp_rgb_sharpen_attr_s *p_rgb_sharpen_attr);

/**
 * @brief  set the color space covert attributes.
 * @details set the color space covert attributes.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [in]  p_csc_attr Indicate the color space covert attributes.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_csc_attr_set(vs_int32_t vii_pipe, const vs_isp_csc_attr_s *p_csc_attr);

/**
 * @brief  get the color space covert attributes.
 * @details get the color space covert attributes.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [out]  p_csc_attr Indicate the color space covert attributes.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_csc_attr_get(vs_int32_t vii_pipe, vs_isp_csc_attr_s *p_csc_attr);

/**
 * @brief  set the wdr stitch  attributes.
 * @details set the wdr stitch  attributes.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [in]  p_wdr_stitch_attr Indicate the wdr stitch  attributes.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_wdr_stitch_attr_set(vs_int32_t vii_pipe, const vs_isp_wdr_stitch_attr_s *p_wdr_stitch_attr);

/**
 * @brief  get the wdr stitch attributes.
 * @details get the wdr stitch attributes.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [out]  p_wdr_stitch_attr Indicate the wdr stitch attributes.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_wdr_stitch_attr_get(vs_int32_t vii_pipe, vs_isp_wdr_stitch_attr_s *p_wdr_stitch_attr);

/**
 * @brief  set the decompander  attributes.
 * @details set the decompander  attributes.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [in]  p_wdr_stitch_attr Indicate the decompander  attributes.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_decompander_attr_set(vs_int32_t vii_pipe, const vs_isp_decompander_attr_s *p_decompander_attr);

/**
 * @brief  get the decompander attributes.
 * @details get the decompander attributes.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [out]  p_decompander_attr Indicate the decompander attributes.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_decompander_attr_get(vs_int32_t vii_pipe, vs_isp_decompander_attr_s *p_decompander_attr);

/**
 * @brief  set the algorithm module bypass.
 * @details set the algorithm module bypass.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [in]  p_module_bypass Indicate the algorithm module bypass set.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_module_bypass_set(vs_int32_t vii_pipe, const vs_isp_bypass_setting_s *p_module_bypass);

/**
 * @brief  get the algorithm module bypass.
 * @details get the algorithm module bypass.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [out]  p_module_bypass Indicate the algorithm module bypass get.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_module_bypass_get(vs_int32_t vii_pipe, vs_isp_bypass_setting_s *p_module_bypass);

/**
 * @brief  set the auto export attr param.
 * @details set the auto export attr param.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [in]  p_ae_attr Indicate the ae attributes set.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_ae_attr_set(vs_int32_t vii_pipe, const vs_isp_ae_attr_s *p_ae_attr);

/**
 * @brief  get the auto export attr param.
 * @details get the auto export attr param.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [out]  p_ae_attr Indicate the ae attributes get.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_ae_attr_get(vs_int32_t vii_pipe, vs_isp_ae_attr_s *p_ae_attr);

/**
 * @brief  set the wdr export wdr attr param.
 * @details set the wdr export wdr attr param.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [in]  p_wdr_exp_attr Indicate the wdr_exp attributes set.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_wdr_exp_attr_set(vs_int32_t vii_pipe, const vs_isp_wdr_exp_attr_s *p_wdr_exp_attr);

/**
 * @brief  get the wdr export attr param.
 * @details get the wdr export attr param.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [out]  p_wdr_exp_attr Indicate the wdr_exp attributes get.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_wdr_exp_attr_get(vs_int32_t vii_pipe, vs_isp_wdr_exp_attr_s *p_wdr_exp_attr);

/**
 * @brief  calculate  auto white balance gain by color temperature.
 * @details calculate  auto white balance gain by color temperature.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [in]  p_awb_attr Indicate the awb attributes set.
 * @param [in]  color_temperature Indicate the color temperature.
 * @param [in]  shift Indicate the color temperature point distance with Planck curve
 * @param [out]  p_awb_gain Indicate the result of auto white balance gain
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_awb_gain_calc(vs_int32_t vii_pipe, const vs_isp_awb_attr_s *p_awb_attr, vs_uint16_t color_temperature, vs_int16_t shift, vs_uint16_t *p_awb_gain);

/**
 * @brief  set the auto white balance attr param.
 * @details set the auto white balance attr param.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [in]  p_awb_attr Indicate the awb attributes set.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_awb_attr_set(vs_int32_t vii_pipe, const vs_isp_awb_attr_s *p_awb_attr);

/**
 * @brief  get the auto white balance attr param.
 * @details get the auto white balance attr param.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [out]  p_isp_awb_attr Indicate the awb attributes get.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_awb_attr_get(vs_int32_t vii_pipe, vs_isp_awb_attr_s *p_awb_attr);

/**
 * @brief  get isp information.
 * @details get isp information.
 * @param [in]  vii_ Indicate the pipe id to be register.
 * @param [out]  p_isp_info Indicate the isp info get.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_info_get(vs_int32_t vii_pipe, vs_isp_info_attr_s *p_isp_info);

/**
 * @brief  set the presharpen attributes.
 * @details set the presharpen attributes.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [out]  p_presharpen_attr Indicate the presharpen attributes get.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_presharpen_attr_set(vs_int32_t vii_pipe, const vs_isp_presharpen_attr_s *p_presharpen_attr);

/**
 * @brief  get the presharpen attributes.
 * @details get the presharpen attributes.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [in]  p_presharpen_attr Indicate the presharpen attributes set.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_presharpen_attr_get(vs_int32_t vii_pipe, vs_isp_presharpen_attr_s *p_presharpen_attr);

/**
 * @brief  set the presharpen attributes.
 * @details set the presharpen attributes.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [out]  p_postsharpen_attr Indicate the presharpen attributes get.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_postsharpen_attr_set(vs_int32_t vii_pipe, const vs_isp_postsharpen_attr_s *p_postsharpen_attr);

/**
 * @brief  get the postsharpen attributes.
 * @details get the postsharpen attributes.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [out]  p_postsharpen_attr Indicate the postsharpen attributes get.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_postsharpen_attr_get(vs_int32_t vii_pipe, vs_isp_postsharpen_attr_s *p_postsharpen_attr);

/**
 * @brief  set the nr3d attributes.
 * @details set the nr3d attributes.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [in]  p_nr3d_attr Indicate the nr3d attributes set.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_nr3d_attr_set(vs_int32_t vii_pipe, const vs_isp_nr3d_attr_s *p_nr3d_attr);

/**
 * @brief  get the nr3d attributes.
 * @details get the nr3d attributes.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [out]  p_nr3d_attr Indicate the nr3d attributes get.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_nr3d_attr_get(vs_int32_t vii_pipe, vs_isp_nr3d_attr_s *p_nr3d_attr);

/**
 * @brief  set the yuv_pfc attributes.
 * @details set the yuv_pfc attributes.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [in]  p_yuv_pfc_attr Indicate the yuv_pfc attributes set.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_yuv_pfc_attr_set(vs_int32_t vii_pipe, const vs_isp_yuv_pfc_attr_s *p_yuv_pfc_attr);

/**
 * @brief  get the yuv_pfc attributes.
 * @details get the yuv_pfc attributes.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [out] p_yuv_pfc_attr Indicate the yuv_pfc attributes.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_yuv_pfc_attr_get(vs_int32_t vii_pipe, vs_isp_yuv_pfc_attr_s *p_yuv_pfc_attr);


/**
 * @brief  set the sa attributes.
 * @details set the sa attributes.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [in]  p_sa_attr Indicate the sa attributes.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_sa_attr_set(vs_int32_t vii_pipe, const vs_isp_sa_attr_s *p_sa_attr);

/**
 * @brief  get the sa attributes.
 * @details get the sa attributes.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [out]  p_sa_attr Indicate the sa attributes.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_sa_attr_get(vs_int32_t vii_pipe, vs_isp_sa_attr_s *p_sa_attr);

/**
 * @brief  set the coloring attributes.
 * @details set the coloring attributes.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [in]  p_coloring_attr Indicate the coloring attributes set.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_coloring_attr_set(vs_int32_t vii_pipe, const vs_isp_coloring_attr_s *p_coloring_attr);

/**
 * @brief  get the coloring attributes.
 * @details get the coloring attributes.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [out]  p_coloring_attr Indicate the coloring attributes set.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_coloring_attr_get(vs_int32_t vii_pipe, vs_isp_coloring_attr_s *p_coloring_attr);

/**
 * @brief  set the yuv_af attributes.
 * @details set the yuv_af attributes.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [in]  p_yuv_af_attr Indicate the yuv_af attributes set.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_yuv_af_attr_set(vs_int32_t vii_pipe, const vs_isp_yuv_af_attr_s *p_yuv_af_attr);

/**
 * @brief  get the yuv_af attributes.
 * @details get the yuv_af attributes.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [out] p_yuv_af_attr Indicate the yuv_af attributes set.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_yuv_af_attr_get(vs_int32_t vii_pipe, vs_isp_yuv_af_attr_s *p_yuv_af_attr);

/**
 * @brief  get the yuv_af statistics.
 * @details get the yuv_af statistics.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [out] p_yuv_af_statistics Indicate the yuv_af statistics get.
 * @param [in] timeout_ms Indicate wait timeout value.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_yuv_af_statistics_get(vs_int32_t vii_pipe, vs_isp_yuv_af_statistics_attr_s *p_yuv_af_statistics, vs_int32_t timeout_ms);

/**
 * @brief  set ae result to sensor.
 * @details set ae result to sensor.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [in] p_ae_result Indicate ae result.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_ae_result_set(vs_int32_t vii_pipe, vs_isp_ae_result_s *p_ae_result);

/**
 * @brief  get smart info from isp.
 * @details get smart info from isp.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [out] p_smart_info Indicate smart info.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_smart_info_get(vs_int32_t vii_pipe, vs_isp_smart_info_s *p_smart_info);

/**
 * @brief  set smart info to isp.
 * @details set smart info to isp.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [in] p_smart_info Indicate smart info.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_smart_info_set(vs_int32_t vii_pipe, const vs_isp_smart_info_s *p_smart_info);

/**
 * @brief  set smart exposure control param to isp.
 * @details set smart exposure control param to isp.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [in] p_smart_ae_attr Indicate smart exposure control param.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_smart_expo_attr_set(vs_int32_t vii_pipe, const vs_isp_smart_expo_attr_s *p_smart_ae_attr);

/**
 * @brief  get smart exposure control param from isp.
 * @details get smart exposure control param from isp.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [out] p_smart_ae_attr Indicate smart exposure control param.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_smart_expo_attr_get(vs_int32_t vii_pipe, vs_isp_smart_expo_attr_s *p_smart_ae_attr);

/**
 * @brief  get focus  control param from isp.
 * @details get focus control param from isp.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [out] p_focus_attr Indicate focus control param.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_focus_attr_get(vs_int32_t vii_pipe, vs_isp_focus_attr_s *p_focus_attr);

/**
 * @brief  set focus control param to isp.
 * @details set focus control param to isp.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [in] p_focus_attr Indicate focus control param.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_focus_attr_set(vs_int32_t vii_pipe, const vs_isp_focus_attr_s *p_focus_attr);

/**
 * @brief  get focus value from isp.
 * @details get focus value from isp.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [out] p_focus_value Indicate the focus statistics value.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 *
 */
vs_int32_t vs_mal_isp_focus_value_get(vs_int32_t vii_pipe, vs_isp_focus_value_s *p_focus_value);

/**
 * @brief  set the color shading correction attributes.
 * @details set the color shading correction attributes.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [in]  p_color_shading_attr Indicate the color shading correction attributes.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 */
vs_int32_t vs_mal_isp_color_shading_attr_set(vs_int32_t vii_pipe, const vs_isp_color_shading_attr_s *p_color_shading_attr);

/**
 * @brief  get the color shading correction attributes.
 * @details get the color shading correction attributes.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [out]  p_color_shading_attr Indicate the color shading correction attributes.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 */
vs_int32_t vs_mal_isp_color_shading_attr_get(vs_int32_t vii_pipe, vs_isp_color_shading_attr_s *p_color_shading_attr);

/**
 * @brief  set the OTP correction attributes.
 * @details set the OTP correction attributes.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [in]  p_otp_attr Indicate the OTP correction attributes.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 */
vs_int32_t vs_mal_isp_otp_attr_set(vs_int32_t vii_pipe, const vs_isp_otp_attr_s* p_otp_attr);

/**
 * @brief  get the OTP correction attributes.
 * @details get the OTP correction attributes.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [in]  p_otp_attr Indicate the OTP correction attributes.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 */
vs_int32_t vs_mal_isp_otp_attr_get(vs_int32_t vii_pipe, vs_isp_otp_attr_s *p_otp_attr);

/**
 * @brief  get the af raw statistics data.
 * @details get the af raw statistics data.
 * @param [in]  vii_pipe Indicate the pipe id to be register.
 * @param [in]  p_af_raw_statistics Indicate af raw statistics data.
 * @return   0 call success.
 * @warning   none0 call failed. see ERROR_CODE for detail.
 * @par
 */
vs_int32_t vs_mal_isp_af_raw_statistics_get(vs_int32_t vii_pipe, vs_isp_af_raw_statistics_s *p_af_raw_statistics);

#ifdef __cplusplus
}

#endif
#endif /*!<  _VS_MAL_ISP_H_ */
