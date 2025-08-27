/**
* @file vs_infra_defines.h
* @brief Declaration of infra enumeration and struct.
* @details Declaration of infra enumeration and struct.
* @author Visinex software group
* @date 2024-7-16
* @version v0.10
* @copyright (C) 2024, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/
#ifndef __VS_INFRA_DEFINES_H__
#define __VS_INFRA_DEFINES_H__

#include "vs_type.h"
#include "vs_errno.h"
#include "vs_comm.h"
#include "vs_comm_video.h"

#ifdef __cplusplus
extern "C" {
#endif

///< parameter is invalid
#define VS_ERR_INFRA_INVALID_PARAM     VS_ERR_CODE(E_MOD_ID_INFRA, E_ERR_INVALID_PARAM)
///< invalid device ID
#define VS_ERR_INFRA_INVALID_DEVID     VS_ERR_CODE(E_MOD_ID_INFRA, E_ERR_INVALID_DEVID)
///< using a NULL pointer
#define VS_ERR_INFRA_NULL_PTR          VS_ERR_CODE(E_MOD_ID_INFRA, E_ERR_NULL_PTR)
///< infra unexist
#define VS_ERR_INFRA_UNEXIST           VS_ERR_CODE(E_MOD_ID_INFRA, E_ERR_UNEXIST)
///< operation is not supported
#define VS_ERR_INFRA_NOT_SUPPORT       VS_ERR_CODE(E_MOD_ID_INFRA, E_ERR_NOT_SUPPORT)
///< system is not ready
#define VS_ERR_INFRA_NOT_INITIALIZED   VS_ERR_CODE(E_MOD_ID_INFRA, E_ERR_NOT_INITIALIZED)
///< operation is not permitted
#define VS_ERR_INFRA_NOT_PERM          VS_ERR_CODE(E_MOD_ID_INFRA, E_ERR_NOT_PERM)
///< no memory for allocated
#define VS_ERR_INFRA_NO_MEMORY         VS_ERR_CODE(E_MOD_ID_INFRA, E_ERR_NO_MEMORY)
///< operation aborted
#define VS_ERR_INFRA_ABORTED           VS_ERR_CODE(E_MOD_ID_INFRA, E_ERR_ABORTED)
///< infra device unexist
#define VS_ERR_INFRA_DEVICE_UNEXIST    VS_ERR_CODE(E_MOD_ID_INFRA, E_ERR_DEVICE_UNEXIST)

/**
 * @brief Defines the type of infra sensor.
 * @details This enumeration lists the different types of infra sensors
 *  supported by the system.
 */
typedef enum vs_infra_sensor_type {
    E_INFRA_SENSOR_TYPE_0 = 0,             ///< infra sensor type 0
    E_INFRA_SENSOR_TYPE_1,                 ///< infra sensor type 1
    E_INFRA_SENSOR_TYPE_2,                 ///< infra sensor type 2
    E_INFRA_SENSOR_TYPE_MAX,
} vs_infra_sensor_type_e;


/**
 * @brief Defines the edge of clock polarity.
 * @details This enumeration describes the clock polarity options for infra modules.
 */
typedef enum vs_infra_polarity {
    E_INFRA_POLARITY_PRE_RISING_EDGE = 0,  ///<  Pre rising edge of clock polarity.
    E_INFRA_POLARITY_PRE_FALLING_EDGE,     ///<  Pre falling edge of clock polarity.
    E_INFRA_POLARITY_POST_RISING_EDGE,     ///<  Post rising edge of clock polarity.
    E_INFRA_POLARITY_POST_FALLING_EDGE,    ///<  Post rising edge of clock polarity.
    E_INFRA_POLARITY_MAX,
}  vs_infra_polarity_e;

/**
 * @brief Defines the infra clock attribute.
 * @details This structure describes the clock configuration for the infra module.
 * All parameters are valid only if `enable` is set to `VS_TRUE`.
 */
typedef struct vs_infra_clk_attr { //if enable
    vs_bool_t enable;                       ///<  RW; Range:[0, 1]; Enable signal of infra clock.
    vs_uint32_t freq_div;                   ///<  RW; Range: > 0; Division of clock frequency.
    vs_infra_polarity_e tx_polarity;        ///<  RW; Range:[0, 1]; Tx clock polarity.
    vs_infra_polarity_e rx_polarity;        ///<  RW; Range:[0, 1]; Rx clock polarity.
} vs_infra_clk_attr_s;

/**
 * @brief Defines the infra tx attribute.
 * @details This structure describes the tx configuration for the infra module.
 * All parameters are valid only if `enable` is set to `VS_TRUE`.
 */
typedef struct vs_infra_tx_attr {
    vs_bool_t enable;                       ///<  RW; Range:[0, 1]; Enable signal of infra tx.
    vs_uint32_t xhs_period;                 ///<  RW; Clock cycle in one xhs period.
    vs_uint32_t vmax_lines;                 ///<  RW; Ouput xhs signal nums in one xvs period.
    vs_uint32_t fs_line_num;                ///<  RW; Line number of frame start.
    vs_uint32_t fe_line_num;                ///<  RW; Line number of frame end.
    vs_crop_s roi;                          ///<  RW; The effective area of image acquisition.
    vs_uint32_t xhs_delay;                  ///<  RW; Delay time after hsync.
} vs_infra_tx_attr_s;

/**
 * @brief Defines the infra attribute.
 * @details This structure contains the essential attributes for configuring
 * an infra sensor.
 */
typedef struct vs_infra_attr {
    vs_infra_sensor_type_e type;            ///<  RW; Range:[0, 2]; Infra sensor type.
    vs_char_t *sensor_name;                 ///<  RW; Infra sensor name.
    vs_size_s image_size;                   ///<  RW; Infra sensor image size.
    vs_uint32_t fps;                        ///<  RW; Infra sensor fps.
    vs_infra_clk_attr_s clk_attr;           ///<  RW; Infra sensor clock attribute.
    vs_infra_tx_attr_s tx_attr;             ///<  RW; Infra sensor tx attribute.
    vs_void_t *p_reserved;                  ///<  Reserved.
} vs_infra_attr_s;

/**
 * @brief Defines the infra inter-frame cfg.
 * @details This structure specifies the inter-frame data for the infra sensor.
 */
typedef struct vs_infra_iframe_cfg {
    vs_uint8_t *p_cfg;                     ///<  RW; Pointer of infra inter-frame data.
    vs_uint32_t num;                       ///<  RW; Range: > 0; Number of infra inter-frame data.
} vs_infra_iframe_cfg_s;

/**
 * @brief Defines the infra ooc cfg.
 * @details This structure contains the OOC data for the infra sensor.
 */
typedef struct vs_infra_ooc_cfg {
    vs_uint8_t *p_data;                     ///<  RW; Pointer of infra ooc data.
    vs_size_s size;                         ///<  RW; Size of infra ooc data.
    vs_bool_t rx_clean;                     ///<  RW; Range:[0, 1]; Enable rx clean.
} vs_infra_ooc_cfg_s;

#ifdef __cplusplus
}
#endif

#endif // __VS_INFRA_DEFINES_H__