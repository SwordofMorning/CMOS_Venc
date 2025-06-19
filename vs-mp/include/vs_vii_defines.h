/**
* @file vs_vii_defines.h
* @brief Declaration of vii enumeration and struct.
* @details Declaration of vii enumeration and struct.
* @author Visinex software group
* @date 2022-05-23
* @version v0.10
* @copyright (C) 2022, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/


#ifndef __VS_VII_DEFINES_H__
#define __VS_VII_DEFINES_H__

#include "vs_type.h"
#include "vs_comm.h"
#include "vs_comm_video.h"
#include "vs_errno.h"
#include "vs_capability.h"

#ifdef __cplusplus
extern "C"{
#endif

///< parameter is invalid
#define VS_ERR_VII_INVALID_PARAM      VS_ERR_CODE(E_MOD_ID_VII, E_ERR_INVALID_PARAM)
///< invalid device ID */
#define VS_ERR_VII_INVALID_DEVID      VS_ERR_CODE(E_MOD_ID_VII, E_ERR_INVALID_DEVID)
///< invalid channel ID */
#define VS_ERR_VII_INVALID_CHNID      VS_ERR_CODE(E_MOD_ID_VII, E_ERR_INVALID_CHNID)
///< invalid pipe ID */
#define VS_ERR_VII_INVALID_PIPEID     VS_ERR_CODE(E_MOD_ID_VII, E_ERR_INVALID_PIPEID)
///< using a NULL pointer
#define VS_ERR_VII_NULL_PTR           VS_ERR_CODE(E_MOD_ID_VII, E_ERR_NULL_PTR)
///< channel has been created
#define VS_ERR_VII_PIPE_EXIST         VS_ERR_CODE(E_MOD_ID_VII, E_ERR_EXIST)
///< channel is not created
#define VS_ERR_VII_PIPE_UNEXIST       VS_ERR_CODE(E_MOD_ID_VII, E_ERR_UNEXIST)
///< operation is not supported
#define VS_ERR_VII_NOT_SUPPORT        VS_ERR_CODE(E_MOD_ID_VII, E_ERR_NOT_SUPPORT)
///< system is not ready
#define VS_ERR_VII_NOT_INITIALIZED    VS_ERR_CODE(E_MOD_ID_VII, E_ERR_NOT_INITIALIZED)
///< operation is not permitted
#define VS_ERR_VII_NOT_PERM           VS_ERR_CODE(E_MOD_ID_VII, E_ERR_NOT_PERM)
///< no memory for allocated
#define VS_ERR_VII_NO_MEMORY           VS_ERR_CODE(E_MOD_ID_VII, E_ERR_NO_MEMORY)
///< buffer is empty
#define VS_ERR_VII_BUF_EMPTY          VS_ERR_CODE(E_MOD_ID_VII, E_ERR_BUF_EMPTY)
///< buffer is full
#define VS_ERR_VII_BUF_FULL           VS_ERR_CODE(E_MOD_ID_VII, E_ERR_BUF_FULL)
///< address is bad
#define VS_ERR_VII_BAD_ADDR           VS_ERR_CODE(E_MOD_ID_VII, E_ERR_BAD_ADDR)
///< system busy
#define VS_ERR_VII_BUSY               VS_ERR_CODE(E_MOD_ID_VII, E_ERR_BUSY)
///< task timeout
#define VS_ERR_VII_TIMEOUT            VS_ERR_CODE(E_MOD_ID_VII, E_ERR_TIMEOUT)
///< device unexist
#define VS_ERR_VII_DEVICE_UNEXIST     VS_ERR_CODE(E_MOD_ID_VII, E_ERR_DEVICE_UNEXIST)


/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/
/**
* @brief Defines vii interface type.
* @details
*/
typedef enum vs_vii_intf_type {
    E_VII_INTF_MIPI = 0,       ///< mipi interface
    E_VII_INTF_SLVDS,          ///< sub_lvds interface
    E_VII_INTF_BT656,          ///< bt.656 interface
    E_VII_INTF_BT1120,         ///< bt.1120 interface
    E_VII_INTF_DVP,            ///< dvp interface
    E_VII_INTF_INFRARED,          ///< infrared interface
    E_VII_INTF_MAX
} vs_vii_intf_type_e;

/**
* @brief Defines vii vc id.
* @details
*/
typedef enum vs_vii_vc_id {
    E_VII_VC_ID_NONE = 0,   ///< not set vc id
    E_VII_VC_ID_0,          ///< vc id 0
    E_VII_VC_ID_1,          ///< vc id 1
    E_VII_VC_ID_2,          ///< vc id 2
    E_VII_VC_ID_3,          ///< vc id 3
    E_VII_VC_ID_MAX
} vs_vii_vc_id_e;

/**
* @brief Defines vii vc attr.
* @details
*/
typedef struct vs_vii_vc_attr {
    vs_vii_vc_id_e vc[VII_MAX_VC_NUM];  ///< RW; Range:[0, 4]; vc id.
} vs_vii_vc_attr_s;


/**
* @brief Defines vii sync mode.
* @details
*/
typedef enum vs_vii_sync_mode {
    E_VII_SYNC_HIGH = 0,   ///< high level is valid
    E_VII_SYNC_LOW,        ///< low level is valid
    E_VII_SYNC_MAX,
} vs_vii_sync_mode_e;

/**
* @brief Defines vii bt656 attr.
* @details
*/
typedef struct vs_vii_bt656_attr {
    vs_uint32_t sync_code;               ///< bt656 sync code
    vs_bool_t bit_revert;                ///< RW; Range:[0, 1]; bt656 bit revert
    vs_vii_sync_mode_e sync_clk;         ///< RW; Range:[0, 1]; bt656 clock revert
} vs_vii_bt656_attr_s;

/**
* @brief Defines vii bt1120 attr.
* @details
*/
typedef struct vs_vii_bt1120_attr {
    vs_uint32_t sync_code;               ///< bt1120 sync code
    vs_bool_t low_bit_revert;            ///< RW; Range:[0, 1]; bt1120 low bit[0:7] revert
    vs_bool_t high_bit_revert;           ///< RW; Range:[0, 1]; bt1120 high bit[8:15] revert
    vs_vii_sync_mode_e sync_clk;         ///< RW; Range:[0, 1]; bt1120 clock mode
} vs_vii_bt1120_attr_s;

/**
* @brief Defines dvp bit output order.
* @details
*/
typedef enum vs_vii_dvp_mode {
    E_VII_DVP_MODE_6_BIT_FIRST = 0,   ///< dvp output 6bit first
    E_VII_DVP_MODE_6_BIT_LAST,        ///< dvp output 6bit last
    E_VII_DVP_MODE_MAX,
} vs_vii_dvp_mode_e;

/**
* @brief Defines vii dvp attr.
* @details
*/
typedef struct vs_vii_dvp_attr {
    vs_bool_t bit_revert;                ///< RW; Range:[0, 1]; dvp bit revert
    vs_vii_sync_mode_e sync_clk;         ///< RW; Range:[0, 1]; dvp clock mode
    vs_vii_dvp_mode_e mode;              ///< RW; Range:[0, 1]; dvp output order
    vs_size_s image_size;                ///< dvp input image size
    vs_vii_sync_mode_e vsync;            ///< RW; Range:[0, 1]; dvp vsync
    vs_vii_sync_mode_e hsync;            ///< RW; Range:[0, 1]; dvp hsync
} vs_vii_dvp_attr_s;

/**
* @brief Defines infrared sensor type.
* @details
*/
typedef enum vs_vii_infrared_sensor_type {
    E_VII_INFRARED_SENSOR_TYPE_0 = 0,    ///< infrared sensor type 0
    E_VII_INFRARED_SENSOR_TYPE_1,        ///< infrared sensor type 1
    E_VII_INFRARED_SENSOR_TYPE_2,        ///< infrared sensor type 2
    E_VII_INFRARED_SENSOR_TYPE_MAX,
} vs_vii_infrared_sensor_type_e;

/**
* @brief Defines vii INFRA attr.
* @details
*/
typedef struct vs_vii_infrared_attr {
    vs_vii_infrared_sensor_type_e type;               ///< RW; Range:[0, 2]; infrared sensor type
    vs_uint32_t fps;                                  ///< infrared sensor fps
    vs_uint32_t reg_array[VII_MAX_INFRARED_REG_NUM];  ///< infrared reg value array
    vs_uint32_t reg_num;                           ///< infrared reg number
    vs_uint32_t phys_addr;                         ///< infrared ooc data physical address
    vs_uint32_t phys_size;                         ///< infrared ooc data size
} vs_vii_infrared_attr_s;

/**
* @brief Defines vii dev attr.
* @details
*/
typedef struct vs_vii_dev_attr {
    vs_vii_intf_type_e intf_type;        ///< RW; Range:[0, 5]; vii interface type
    union {
        vs_vii_bt656_attr_s bt656_attr;  ///< bt656 attributes
        vs_vii_bt1120_attr_s bt1120_attr;///< bt1120 attributes
        vs_vii_dvp_attr_s dvp_attr;      ///< dvp attributes
        vs_vii_infrared_attr_s infrared_attr;  ///< infra attributes
    };
    vs_pixel_format_e pixel_format;      ///< RW; Range:[0, 39]; device input pixel format
} vs_vii_dev_attr_s;

/**
* @brief Defines vii dev pipe bind.
* @details
*/
typedef struct vs_vii_dev_pipe_bind {
    vs_uint32_t num;
    vs_int32_t pipe_id[VII_MAX_PHYS_PIPE_NUM];  ///< RW; Range:[0, 4]; vii interface type
} vs_vii_dev_pipe_bind_s;

/**
* @brief Defines vii vpp mode.
* @details
*/
typedef enum vs_vii_vpp_mode {
    E_VII_OFFLINE_VPP_OFFLINE = 0,  ///< vi_offline_vpp_offline mode.
    E_VII_ONLINE_VPP_ONLINE,        ///< vi_online_vpp_online mode.
    E_VII_ONLINE_VPP_OFFLINE,       ///< vi_online_vpp_offline mode.
    E_VII_OFFLINE_VPP_ONLINE,       ///< vi_offline_vpp_online mode.
    E_VII_VPP_MODE_MAX,
} vs_vii_vpp_mode_e;

/**
* @brief Defines vii vpp online or offline attr.
* @details
*/
typedef struct vs_vii_vpp_online_offline_attr {
    vs_vii_vpp_mode_e mode[VII_MAX_PIPE_NUM]; ///< RW; Range:[0, 3];
} vs_vii_vpp_online_offline_attr_s;

/**
* @brief Defines vii pipe bypass mode.
* @details
*/
typedef enum vs_vii_pipe_bypass_mode {
    E_VII_PIPE_BYPASS_NONE = 0,  ///< pipe not bypass any process.
    E_VII_PIPE_BYPASS_FE,        ///< pipe bypass fe.
    E_VII_PIPE_BYPASS_BE,        ///< pipe bypass be.
    E_VII_PIPE_BYPASS_ALL,       ///< pipe bypass fe and be.
    E_VII_PIPE_BYPASS_MAX,
} vs_vii_pipe_bypass_mode_e;

/**
* @brief Defines vii pipe input source.
* @details
*/
typedef enum vs_vii_pipe_source {
    E_VII_PIPE_SOURCE_DEV = 0,  ///< input source of be is device capture frame.
    E_VII_PIPE_SOURCE_USER,     ///< input source of be is user frame.
    E_VII_PIPE_SOURCE_MAX,
} vs_vii_pipe_source_e;

/**
* @brief Defines vii pipe attr.
* @details
*/
typedef struct vs_vii_pipe_attr {
    vs_vii_pipe_bypass_mode_e bypass_mode; ///< RW; Range:[0, 2]; pipe bypass mode.
    vs_size_s          image_size;         ///< pipe input size.
    vs_uint32_t        real_fps;           ///< pipe input real frame rate.
    vs_pixel_format_e  pixel_format;       ///< RW; Range:[0, 39]; pipe input pixel format.
    vs_wdr_mode_e      wdr_mode;           ///< RW; Range:[0, 4]; pipe wdr mode.
    vs_compress_mode_e compress_mode;      ///< RW; Range:[0, 1]; video compression format
    vs_video_frc_s     framerate;          ///< frame rate control information
} vs_vii_pipe_attr_s;

/**
* @brief Defines vii pipe advance mode.
* @details
*/
typedef enum vs_vii_pipe_advance_mode {
    E_VII_PIPE_ADVANCE_MODE_NONE = 0,  ///< pipe not enable advance mode.
    E_VII_PIPE_ADVANCE_MODE_SUB_FRAME_LAST,     ///< pipe enable advance mode of sub_frame_last.
    E_VII_PIPE_ADVANCE_MODE_SUB_FRAME_FIRST,    ///< pipe enable advance mode of sub_frame_first.
    E_VII_PIPE_ADVANCE_MODE_STATISTICS,         ///< pipe enable advance mode of statistics when vii online mode.
    E_VII_PIPE_ADVANCE_MODE_MAX,
} vs_vii_pipe_advance_mode_e;

/**
* @brief Defines vii pipe early interrupt attr.
* @details
*/
typedef struct vs_vii_pipe_early_int_attr {
    vs_bool_t enable;         ///< pipe early int enable, default disable.
    vs_uint32_t write_lines;  ///< pipe write lines to report early int.
} vs_vii_pipe_early_int_attr_s;

/**
* @brief Defines vii pipe param.
* @details
*/
typedef struct vs_vii_pipe_param {
    vs_vii_pipe_advance_mode_e mode;
    vs_vii_pipe_early_int_attr_s early_int_attr;
} vs_vii_pipe_param_s;

/**
* @brief Defines vii operation type
* @details
*/
typedef enum vs_vii_op_type {
	E_VII_OP_TYPE_AUTO = 0,
	E_VII_OP_TYPE_MANUAL,
	E_VII_OP_TYPE_MAX
} vs_vii_op_type_e;

/**
 * @brief Defines vii fpn manual attribute
 * @details
 */
typedef struct vs_vii_fpn_manual_attr {
	vs_uint16_t strength;         ///<   RW; Range:[0x0, 0xFFF]; fpn strength value.
} vs_vii_fpn_manual_attr_s;


#define VS_VII_FPN_AUTO_STRENGTH_SIZE 11
/**
 * @brief Defines vii fpn auto attribute
 * @details
 */
typedef struct vs_vii_fpn_auto_attr {
	vs_uint16_t strength[VS_VII_FPN_AUTO_STRENGTH_SIZE];   ///<   RW; Range:[0x0, 0xFFF]; fpn strength value.
} vs_vii_fpn_auto_attr_s;

/**
* @brief Defines vii pipe fpn attr.
* @details
*/
typedef struct vs_vii_pipe_fpn_attr {
    vs_bool_t enable;
    vs_vii_op_type_e op_type;
    vs_uint32_t iso;
    vs_uint32_t offset;
    vs_video_frame_info_s fpn_frame;
    vs_vii_fpn_auto_attr_s auto_attr;
    vs_vii_fpn_manual_attr_s manual_attr;
} vs_vii_pipe_fpn_attr_s;

/**
* @brief Defines vii pipe dump frame attr.
* @details
*/
typedef struct vs_vii_pipe_dump_frame_attr {
    vs_bool_t enable;    ///< RW; Range:[0, 1]; pipe dump frame is enable or not.
    vs_uint32_t depth;   ///< pipe dump frame queue depth.
} vs_vii_pipe_dump_frame_attr_s;

/**
* @brief Defines vii pipe status.
* @details
*/
typedef struct vs_vii_pipe_status {
    vs_bool_t enable;             ///< RW; Range:[0, 1]; pipe is start or not.
    vs_uint32_t int_cnt;          ///< pipe interrupt report count.
    vs_uint32_t frame_rate;       ///< pipe output frame rate.
    vs_uint32_t lost_frame_cnt;   ///< pipe lost frame count.
    vs_uint32_t vb_fail_cnt;      ///< pipe get vb failed count.
    vs_size_s   out_size;         ///< pipe frame out size.
} vs_vii_pipe_status_s;

/**
* @brief Defines vii chn attr.
* @details
*/
typedef struct vs_vii_chn_attr {
    vs_size_s           image_size;         ///< channel output size
    vs_video_format_e   video_format;       ///< RW; Range:[0, 1]; video_format, only support E_VIDEO_FORMAT_LINEAR
    vs_pixel_format_e   pixel_format;       ///< RW; Range:[0, 39]; pixel format, only support SP422/SP420/SP400
    vs_dynamic_range_e  dynamic_range;      ///< RW; Range:[0, 0]; dynamic range of the frame
    vs_compress_mode_e  compress_mode;      ///< RW; Range:[0, 1]; video compression format, only support E_COMPRESS_MODE_NONE
    vs_video_frc_s      framerate;          ///< frame rate control information
    vs_uint32_t         depth;              ///< channel frame queue depth
} vs_vii_chn_attr_s;

/**
* @brief Define vii extended channel input src.
* @details
*/
typedef enum vs_vii_extchn_src {
    E_VII_EXTCHN_SRC_TAIL = 0,
    E_VII_EXTCHN_SRC_HEAD,

    E_VII_EXTCHN_SRC_MAX,
} vs_vii_extchn_src_e;

/**
* @brief Define vii extended channel attributes.
* @details
*/
typedef struct vs_vii_extchn_attr {
    vs_int32_t          bind_chnid;         ///< extended channel bind channel id
    vs_vii_extchn_src_e in_src;             ///< RW; Range:[0, 1]; extended channel input source select.

    vs_uint32_t         width;              ///< extended channel width
    vs_uint32_t         height;             ///< extended channel height
    vs_video_format_e   video_format;       ///< RW; Range:[0, 1]; video_format, only support E_VIDEO_FORMAT_LINEAR
    vs_pixel_format_e   pixel_format;       ///< RW; Range:[0, 39]; pixel format, only support SP422/SP420/SP400
    vs_dynamic_range_e  dynamic_range;      ///< RW; Range:[0, 0]; dynamic range of the frame
    vs_compress_mode_e  compress_mode;      ///< RW; Range:[0, 1]; video compression format, only support E_COMPRESS_MODE_NONE
    vs_video_frc_s      framerate;          ///< frame rate control information
    vs_uint32_t         depth;              ///< extended channel frame queue depth
} vs_vii_extchn_attr_s;

/**
* @brief Define vii crop information.
* @details
*/
typedef struct vs_vii_crop_info {
    vs_bool_t               enable;             ///< RW; Range:[0, 1]; enable crop or not
    vs_coordinate_mode_e    coordinate_mode;    ///< RW; Range:[0, 1]; coordinate  mode
    vs_rect_s               rect;               ///< crop rect information
} vs_vii_crop_info_s;

/**
* @brief Defines vii chn status.
* @details
*/
typedef struct vs_vii_chn_status {
    vs_bool_t enable;            ///< RW; Range:[0, 1]; channel is enable or not.
    vs_uint32_t frame_rate;      ///< channel output frame rate.
    vs_uint32_t lost_frame_cnt;  ///< channel lost frame count.
    vs_uint32_t vb_fail_cnt;     ///< channel get vb failed count.
    vs_size_s   out_size;        ///< channel frame out size.
} vs_vii_chn_status_s;


#ifdef __cplusplus
}
#endif
#endif /*!<  _VS_VII_DEFINES_H_ */
