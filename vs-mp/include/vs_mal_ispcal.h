
#ifndef __VS_ISPCAL_H__
#define __VS_ISPCAL_H__

#include "vs_ispcal_defines.h"


#ifdef __cplusplus
extern "C" {
#endif
 /**
 * @brief detect defective piexl 
 * @details detect defective piexl 
 * @param [in]  p_bright_frame[] indicate an array of bright frames
 * @param [in]  p_dark_frame[] indicate an array of bright frames
 * @param [in]  p_param indicate the sdp attributes set
 * @param [out] p_result indicate sdp output attributes set
 * return 0 call success
 * warning nonoe0 failed.see ERR_CODE for detail
 */
vs_int32_t vs_mal_ispcal_sdp_proc(vs_uint16_t* p_bright_frame[], vs_uint16_t* p_dark_frame[], vs_ispcal_sdp_param_s* p_param, vs_ispcal_sdp_result_s* p_result);
#ifdef __cplusplus
};
#endif

#endif // !__VS_ISPCAL_H__
