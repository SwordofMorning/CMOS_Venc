#ifndef __VS_ISPCAL_DEFINES_H__
#define __VS_ISPCAL_DEFINES_H__
#include "vs_type.h"
#include "vs_isp_defines.h"

#define VS_ISPCAL_SDP_MAX_NUM 4095

/**
 * @brief Defines sdp mode attr.
 * @details
 */
typedef enum vs_ispcal_sdp_mode
{
    E_ISPCAL_SDP_MODE_DARK = 0,
    E_ISPCAL_SDP_MODE_BRIGHT,
    E_ISPCAL_SDP_MODE_BOTH,
    E_ISPCAL_SDP_MODE_MAX
}vs_ispcal_sdp_mode_e;

/**
 * @brief Defines sdp param attr.
 * @details
 */
typedef struct vs_ispcal_sdp_param
{
    vs_uint32_t                  width;                 // RW ; Range: [0x1, 0x1FFF];
    vs_uint32_t                  height;                // RW ; Range: [0x1, 0x1FFF];
    vs_uint32_t                  frame_num;             // RW ; Range: [0x0, 0x20];
    vs_isp_bayer_format_e        bayer_format;          // RW ; Range: [0x0, 0x3];
    vs_ispcal_sdp_mode_e         mode;                  // RW ; Range: [0x0, 0x2];
    vs_uint16_t                  dark_pixel_r_thr;      // RW ; Range: [0x0, 0xFFF];
    vs_uint16_t                  dark_pixel_g_thr;      // RW ; Range: [0x0, 0xFFF];
    vs_uint16_t                  dark_pixel_b_thr;      // RW ; Range: [0x0, 0xFFF];
    vs_uint16_t                  bright_pixel_thr;      // RW ; Range: [0x0, 0xFFF];
}vs_ispcal_sdp_param_s;

/**
 * @brief Defines sdp state attr.
 * @details
 */
typedef enum vs_ispcal_sdp_state {
    E_ISPCAL_SDP_STATE_SUCCESS = 0,
    E_ISPCAL_SDP_STATE_OVERFLOW,
    E_ISPCAL_SDP_STATE_MAX,
} vs_ispcal_sdp_state_e;

/**
 * @brief Defines sdp result attr.
 * @details
 */
typedef struct vs_ispcal_sdp_result
{
    vs_uint32_t  table[VS_ISPCAL_SDP_MAX_NUM];    //R;  Range: [0x0, 0x1FFFFFF];
    vs_uint16_t  table_len;                       //RW; Range: [0x0, 0xFFFF];
    vs_ispcal_sdp_state_e  bright_state;          //RW; Range: [0x0, 0x1];
    vs_ispcal_sdp_state_e  dark_state;            //RW; Range: [0x0, 0x1];
}vs_ispcal_sdp_result_s;

#endif // !__VS_ISPCAL_DEFINES_H__
