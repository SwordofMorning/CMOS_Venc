/**
* @file sample_common_rgn.c
* @brief common rgn sample
* @details common rgn sample
* @author visinex sw group
* @date 2023-08-18
* @version v0.01
* @Copyright (C) 2020, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vs_comm.h"
#include "vs_type.h"
#include "sample_common.h"
#include "image_file_util.h"

#define CoveMinHandle       0
#define CoverExMinHandle    20
#define OverlayMinHandle    40
#define OverlayExMinHandle  60
#define MosaicMinHandle     80

#define  RGN_CHN_BMP  "./rgn/logo.bmp"

vs_uint32_t sample_rgn_overlay_create(vs_uint32_t handle_num)
{
    vs_uint32_t ret = VS_SUCCESS;
    vs_rgn_attr_s   rgn_attr;
    vs_uint32_t     i;

    rgn_attr.type = E_RGN_TYPE_OVERLAY;
    rgn_attr.attr.overlay.size.width = 320;
    rgn_attr.attr.overlay.size.height = 256;
    rgn_attr.attr.overlay.format = E_PIXEL_FORMAT_ARGB8888;
    rgn_attr.attr.overlay.bgcolor = 0x00ff00;
    rgn_attr.attr.overlay.surface_num = 2;
    for(i = OverlayMinHandle; i < OverlayMinHandle + handle_num; i++) {
        ret = vs_mal_rgn_create(i, &rgn_attr);
        if (ret) {
            vs_sample_trace("rgn create failed!\n");
            return VS_FAILED;
        }
    }
    return ret;
}

vs_uint32_t sample_rgn_overlayex_create(vs_uint32_t handle_num)
{
    vs_uint32_t     ret = VS_SUCCESS;
    vs_rgn_attr_s   rgn_attr;
    vs_uint32_t     i;

    rgn_attr.type = E_RGN_TYPE_OVERLAYEX;
    rgn_attr.attr.overlayex.size.width = 320;
    rgn_attr.attr.overlayex.size.height = 256;
    rgn_attr.attr.overlayex.format = E_PIXEL_FORMAT_ARGB8888;
    rgn_attr.attr.overlayex.bgcolor = 0x00ff00;
    rgn_attr.attr.overlayex.surface_num = 2;

    for(i = OverlayExMinHandle; i < OverlayExMinHandle + handle_num; i++) {
        ret = vs_mal_rgn_create(i, &rgn_attr);
        if (ret) {
            vs_sample_trace("rgn create failed!\n");
            return VS_FAILED;
        }
    }
    return ret;
}

vs_uint32_t sample_rgn_cover_create(vs_uint32_t handle_num)
{
    vs_uint32_t     ret = VS_SUCCESS;
    vs_rgn_attr_s   rgn_attr;
    vs_uint32_t     i;

    rgn_attr.type = E_RGN_TYPE_COVER;
    for(i = CoveMinHandle; i < CoveMinHandle + handle_num; i++) {
        ret = vs_mal_rgn_create(i, &rgn_attr);
        if (ret) {
            vs_sample_trace("rgn create failed!\n");
            return VS_FAILED;
        }
    }
    return ret;
}

vs_uint32_t sample_rgn_coverex_create(vs_uint32_t handle_num)
{
    vs_uint32_t     ret = VS_SUCCESS;
    vs_rgn_attr_s   rgn_attr;
    vs_uint32_t     i;

    rgn_attr.type = E_RGN_TYPE_COVEREX;
    for(i = CoverExMinHandle; i < CoverExMinHandle + handle_num; i++) {
        ret = vs_mal_rgn_create(i, &rgn_attr);
        if (ret) {
            vs_sample_trace("rgn create failed!\n");
            return VS_FAILED;
        }
    }

    return ret;
}

vs_uint32_t sample_rgn_mosaic_create(vs_uint32_t handle_num)
{
    vs_uint32_t     ret = VS_SUCCESS;
    vs_rgn_attr_s   rgn_attr;
    vs_uint32_t     i;

    rgn_attr.type = E_RGN_TYPE_MOSAIC;
    for(i = MosaicMinHandle; i < MosaicMinHandle + handle_num; i++) {
        ret = vs_mal_rgn_create(i, &rgn_attr);
        if (ret) {
            vs_sample_trace("rgn create failed!\n");
            return VS_FAILED;
        }
    }

    return ret;
}

vs_uint32_t sample_rgn_chn_bind(vs_uint32_t handle, const vs_chn_s *p_chn, vs_rgn_disp_info_s *p_disp_info)
{
    vs_uint32_t ret = VS_SUCCESS;
    ret = vs_mal_rgn_chn_bind(handle, p_chn, p_disp_info);
    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("attach region to chn failed, ret[%#x]\n", ret);
        return VS_FAILED;
    }
    return ret;
}

vs_uint32_t sample_common_rgn_get_min_handle(vs_rgn_type_e rgn_type)
{
    vs_uint32_t min_handle = 0;
    switch(rgn_type)
    {
    case E_RGN_TYPE_COVER:
        min_handle = CoveMinHandle;
        break;
    case E_RGN_TYPE_COVEREX:
        min_handle = CoverExMinHandle;
        break;
    case E_RGN_TYPE_OVERLAY:
        min_handle = OverlayMinHandle;
        break;
    case E_RGN_TYPE_OVERLAYEX:
        min_handle = OverlayExMinHandle;
        break;
    case E_RGN_TYPE_MOSAIC:
        min_handle = MosaicMinHandle;
        break;
    default:
        min_handle = -1;
        break;
    }
    return min_handle;

}

vs_uint32_t rgn_bitmap_to_surface(const char* filename, vs_bitmap_s *bitmap, vs_bool_t VS_FALSE, vs_uint32_t color, vs_size_s *size, vs_uint32_t stride, vs_pixel_format_e format)
{
    vs_uint32_t ret = VS_SUCCESS;
    return ret;
}

vs_uint32_t sample_common_rgn_get_update_surface(vs_uint32_t handle)
{
    vs_uint32_t ret = VS_SUCCESS;
    vs_rgn_surface_info_s surface_info = {0};

    ret = vs_mal_rgn_surface_get(handle, &surface_info);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_rgn_surface_get failed! ret:%d.\n", ret);
        return ret;
    }

    load_bmp(RGN_CHN_BMP, surface_info.p_virt_addr, 0);
    ret = vs_mal_rgn_surface_update(handle);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_rgn_surface_update failed, ret:%d.\n", ret);
        return ret;
    }

    return ret;
}

vs_uint32_t sample_common_region_create(vs_uint32_t handle_num, vs_rgn_type_e rgn_type)
{
    vs_uint32_t ret = VS_SUCCESS;
    if(handle_num <= 0 || handle_num > 16)
    {
        vs_sample_trace("handle_num is illegal %d!\n", handle_num);
        return VS_FAILED;
    }
    if(rgn_type < 0 || rgn_type > 4)
    {
        vs_sample_trace("rgn_type is illegal %d!\n", rgn_type);
        return VS_FAILED;
    }

    switch(rgn_type)
    {
        case E_RGN_TYPE_OVERLAY:
             ret = sample_rgn_overlay_create(handle_num);
             break;
        case E_RGN_TYPE_OVERLAYEX:
             ret = sample_rgn_overlayex_create(handle_num);
             break;
        case E_RGN_TYPE_COVER:
             ret = sample_rgn_cover_create(handle_num);
             break;
        case E_RGN_TYPE_COVEREX:
             ret = sample_rgn_coverex_create(handle_num);
             break;
        case E_RGN_TYPE_MOSAIC:
             ret = sample_rgn_mosaic_create(handle_num);
             break;
            default:
                break;
    }

    if (ret != VS_SUCCESS)
    {
        vs_sample_trace("sample_common_region_create failed! handle_num:%d, rgn_type:%d!\n", handle_num, rgn_type);
        return VS_FAILED;
    }

    return ret;
}

vs_uint32_t sample_common_rgn_destroy(vs_uint32_t handle_num, vs_rgn_type_e rgn_type)
{
    vs_uint32_t ret = VS_SUCCESS;
    vs_uint32_t min_handle = 0;
    vs_uint32_t i = 0;

    if(handle_num <= 0 || handle_num > 16)
    {
        vs_sample_trace("handle_num is illegal %d!\n", handle_num);
        return VS_FAILED;
    }
    if(rgn_type < 0 || rgn_type > 4)
    {
        vs_sample_trace("rgn_type is illegal %d!\n", rgn_type);
        return VS_FAILED;
    }
    switch(rgn_type)
    {
    case E_RGN_TYPE_COVER:
        min_handle = CoveMinHandle;
        break;
    case E_RGN_TYPE_COVEREX:
        min_handle = CoverExMinHandle;
        break;
    case E_RGN_TYPE_OVERLAY:
        min_handle = OverlayMinHandle;
        break;
    case E_RGN_TYPE_OVERLAYEX:
        min_handle = OverlayExMinHandle;
        break;
    case E_RGN_TYPE_MOSAIC:
        min_handle = MosaicMinHandle;
        break;
    default:
        min_handle = -1;
        break;
    }
    for(i = min_handle; i < min_handle + handle_num; i++)
    {
        ret = vs_mal_rgn_destroy(i);
    }
    return ret;

}

vs_uint32_t sample_common_rgn_unbind(vs_uint32_t handle_num, vs_rgn_type_e rgn_type, const vs_chn_s *p_chn)
{
    vs_uint32_t ret = VS_SUCCESS;
    vs_uint32_t min_handle = 0;
    vs_uint32_t i = 0;

    if(handle_num <= 0 || handle_num > 16)
    {
        vs_sample_trace("handle_num is illegal %d!\n", handle_num);
        return VS_FAILED;
    }
    if(rgn_type < 0 || rgn_type > 4)
    {
        vs_sample_trace("rgn_type is illegal %d!\n", rgn_type);
        return VS_FAILED;
    }
    switch(rgn_type)
    {
    case E_RGN_TYPE_COVER:
        min_handle = CoveMinHandle;
        break;
    case E_RGN_TYPE_COVEREX:
        min_handle = CoverExMinHandle;
        break;
    case E_RGN_TYPE_OVERLAY:
        min_handle = OverlayMinHandle;
        break;
    case E_RGN_TYPE_OVERLAYEX:
        min_handle = OverlayExMinHandle;
        break;
    default:
        min_handle = -1;
        break;
    }
    for(i = min_handle; i < min_handle + handle_num; i++)
    {
        ret = vs_mal_rgn_chn_unbind(i, p_chn);
        if (ret != VS_SUCCESS)
        {
            vs_sample_trace("sample common rgn destroy failed!ret:%d.\n", ret);
            return ret;
        }
    }
    return ret;
}

vs_uint32_t sample_common_rgn_chn_bind(vs_uint32_t handle_num, vs_rgn_type_e rgn_type, const vs_chn_s *p_chn)
{
    vs_uint32_t ret = VS_SUCCESS;
    vs_uint32_t min_handle = 0;
    vs_uint32_t i = 0;
    vs_rgn_disp_info_s disp_info = {0};

    if(p_chn == VS_NULL)
    {
        vs_sample_trace("p_chn is NULL !\n");
        return VS_FAILED;
    }

    disp_info.show = VS_TRUE;
    disp_info.type = rgn_type;
    switch(rgn_type)
    {
        case E_RGN_TYPE_COVER    :
            min_handle = CoveMinHandle;
            disp_info.disp_attr.cover_disp.area = E_RGN_COVER_TYPE_RECT;
            disp_info.disp_attr.cover_disp.coord = E_COORDINATE_MODE_ABSOLUTE;
            disp_info.disp_attr.cover_disp.zorder = 1;
            disp_info.disp_attr.cover_disp.color = 0x0000A9E3;
            disp_info.disp_attr.cover_disp.rect.x = 0;
            disp_info.disp_attr.cover_disp.rect.y = 0;
            disp_info.disp_attr.cover_disp.rect.width= 100;
            disp_info.disp_attr.cover_disp.rect.height = 100;
            break;
        case E_RGN_TYPE_COVEREX  :
            min_handle = CoverExMinHandle;
            disp_info.disp_attr.coverex_disp.area = E_RGN_COVER_TYPE_RECT;
            disp_info.disp_attr.coverex_disp.coord = E_COORDINATE_MODE_ABSOLUTE;
            disp_info.disp_attr.coverex_disp.zorder = 1;
            disp_info.disp_attr.coverex_disp.color = 0x0000ffff;
            disp_info.disp_attr.coverex_disp.rect.x = 0;
            disp_info.disp_attr.coverex_disp.rect.y = 0;
            disp_info.disp_attr.coverex_disp.rect.width= 100;
            disp_info.disp_attr.coverex_disp.rect.height = 100;
            break;
        case E_RGN_TYPE_OVERLAY  :
            min_handle = OverlayMinHandle;
            disp_info.disp_attr.overlay_disp.coord = E_COORDINATE_MODE_ABSOLUTE;
            disp_info.disp_attr.overlay_disp.qp.enable = VS_FALSE;
            disp_info.disp_attr.overlay_disp.qp.abs_qp = VS_FALSE;
            disp_info.disp_attr.overlay_disp.qp.qp_value = 0;
            break;
        case E_RGN_TYPE_OVERLAYEX:
            min_handle = OverlayExMinHandle;
            disp_info.disp_attr.overlayex_disp.coord = E_COORDINATE_MODE_ABSOLUTE;
            disp_info.disp_attr.overlayex_disp.fgalpha = 127;
            disp_info.disp_attr.overlayex_disp.bgalpha = 127;
            break;
        case E_RGN_TYPE_MOSAIC:
            min_handle = MosaicMinHandle;
            disp_info.disp_attr.mosaic_disp.coord = E_COORDINATE_MODE_ABSOLUTE;
            disp_info.disp_attr.mosaic_disp.block = E_RGN_MOSAIC_BLOCK_32;
            disp_info.disp_attr.mosaic_disp.rect.width = 128;
            disp_info.disp_attr.mosaic_disp.rect.height = 128;
            break;
        default:
            break;
    }

    /*attach to Chn*/
    for(i = min_handle; i < min_handle + handle_num; i++)
    {
        if(rgn_type == E_RGN_TYPE_COVER)
        {
            disp_info.disp_attr.cover_disp.zorder = i - CoveMinHandle;
            disp_info.disp_attr.cover_disp.rect.x = 40 + 100*(i - CoveMinHandle);
            disp_info.disp_attr.cover_disp.rect.y = 40 + 100*(i - CoveMinHandle);

        }
        if(rgn_type == E_RGN_TYPE_COVEREX)
        {
            disp_info.disp_attr.coverex_disp.zorder = i - CoverExMinHandle;
            disp_info.disp_attr.coverex_disp.rect.x = 40 + 100*(i - CoverExMinHandle);
            disp_info.disp_attr.coverex_disp.rect.y = 40 + 100*(i - CoverExMinHandle);

        }
        if(rgn_type == E_RGN_TYPE_OVERLAY)
        {
            if((i - OverlayMinHandle) > 3) {
                disp_info.disp_attr.overlay_disp.zorder  = 0;
                disp_info.disp_attr.overlay_disp.point.x = 64 + 384*(i - OverlayMinHandle - 4);
                disp_info.disp_attr.overlay_disp.point.y = 384;
            } else {
                disp_info.disp_attr.overlay_disp.zorder  = 0;
                disp_info.disp_attr.overlay_disp.point.x = 64 + 384*(i - OverlayMinHandle);
                disp_info.disp_attr.overlay_disp.point.y = 64;
            }
        }
        if(rgn_type == E_RGN_TYPE_OVERLAYEX)
        {
            disp_info.disp_attr.overlayex_disp.zorder = i - OverlayExMinHandle;
            disp_info.disp_attr.overlayex_disp.point.x = 20 + 150*(i - OverlayExMinHandle);
            disp_info.disp_attr.overlayex_disp.point.y = 20 + 150*(i - OverlayExMinHandle);

        }
        if(rgn_type == E_RGN_TYPE_MOSAIC)
        {
            disp_info.disp_attr.mosaic_disp.zorder = 0;
            disp_info.disp_attr.mosaic_disp.rect.x = 64 + 256*(i - MosaicMinHandle);
            disp_info.disp_attr.mosaic_disp.rect.y = 64 + 256*(i - MosaicMinHandle);

        }
        ret = sample_rgn_chn_bind(i, p_chn, &disp_info);
        if(ret != VS_SUCCESS)
        {
            vs_sample_trace("sample_rgn_chn_bind failed! ret[%#x]\n", ret);
            return VS_FAILED;
        }
    }

    return ret;
}


