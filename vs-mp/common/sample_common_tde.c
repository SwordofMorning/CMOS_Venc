/**
* @file sample_common_tde.c
* @brief common tde sample
* @details common tde sample
* @author visinex sw group
* @date 2025-04-07
* @version v0.01
* @Copyright (C) 2020, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vs_comm.h"
#include "vs_type.h"
#include "sample_common.h"
#include "vs_mal_tde.h"

static vs_int32_t sample_common_tde_pixel_bytes(vs_pixel_format_e format)
{
    switch (format) {
    case E_PIXEL_FORMAT_ARGB8888:
    case E_PIXEL_FORMAT_ABGR8888:
        return 4;
    case E_PIXEL_FORMAT_ARGB1555:
    case E_PIXEL_FORMAT_ARGB4444:
    case E_PIXEL_FORMAT_ABGR1555:
    case E_PIXEL_FORMAT_ABGR4444:
        return 2;
    default:
        return 0;
    }
}

vs_int32_t sample_common_tde_init(vs_void_t)
{
    vs_int32_t ret;
    ret = vs_mal_tde_us_init();
    if (ret != VS_SUCCESS)
        vs_sample_trace("vs_mal_tde_us_init failed, ret:%d.\n", ret);

    return ret;
}

vs_int32_t sample_common_tde_exit(vs_void_t)
{
    vs_int32_t ret;
    ret = vs_mal_tde_us_exit();
    if (ret != VS_SUCCESS)
        vs_sample_trace("vs_mal_tde_us_exit failed, ret:%d.\n", ret);

    return ret;
}

vs_int32_t sample_common_tde_quick_scale(vs_uint64_t src_phy_addr, vs_size_s *p_src_size,
    vs_uint64_t dst_phy_addr, vs_size_s *p_dst_size, vs_pixel_format_e format)
{
    vs_int32_t handle;
    vs_tde_surface_s src_surf = {0}, dst_surf = {0};
    vs_rect_s src_rect = {0}, dst_rect = {0};
    vs_int32_t ret;

    ret = vs_mal_tde_job_start(&handle);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_tde_job_start failed, ret:%d.\n", ret);
        return ret;
    }

    src_surf.phy_addr = src_phy_addr;
    src_surf.pixel_format = format;
    src_surf.height = p_src_size->height;
    src_surf.width = p_src_size->width;
    src_surf.stride = src_surf.width * sample_common_tde_pixel_bytes(format);
    src_rect.height = p_src_size->height;
    src_rect.width = p_src_size->width;

    dst_surf.phy_addr = dst_phy_addr;
    dst_surf.pixel_format = format;
    dst_surf.height = p_dst_size->height;
    dst_surf.width = p_dst_size->width;
    dst_surf.stride = dst_surf.width * sample_common_tde_pixel_bytes(format);
    dst_rect.height = p_dst_size->height;
    dst_rect.width = p_dst_size->width;

    ret = vs_mal_tde_quick_scale(handle, &src_surf, &src_rect, &dst_surf, &dst_rect);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_tde_quick_scale failed, ret:%d.\n", ret);
        return ret;
    }

    ret = vs_mal_tde_job_finish(handle, VS_TRUE, 500);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_tde_job_finish failed, ret:%d.\n", ret);
    }

    return ret;
}