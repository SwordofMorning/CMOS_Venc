/**
* @file vs_buffer.h
* @brief buffer info
* @details buffer size calc
* @author VS codec group
* @date 2022-06-05
* @version v0.10
* @Copyright (C) 2021, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/

#ifndef __VS_BUFFER_H__
#define __VS_BUFFER_H__

#include "vs_comm.h"
#include "vs_comm_video.h"

#ifdef __cplusplus
extern "C" {
#endif

#define STRIDE_UP(variable,alignment) (((variable) + (alignment) - 1) & (~((alignment) - 1)))

struct common_frame_buffer_attr {
    vs_size_s img_size;
    vs_pixel_format_e pixel_format;
    vs_compress_mode_e compress_mode;
};

static inline vs_uint32_t common_frame_buffer_stride_get(const struct common_frame_buffer_attr *p_frame_attr)
{
    vs_uint32_t bit_width = 0;
    vs_uint32_t stride = 0;
    vs_uint32_t line_align = 0;

    if (p_frame_attr == NULL) {
        return 0;
    }

    if ((p_frame_attr->pixel_format == E_PIXEL_FORMAT_BAYER_8BPP)
            || (p_frame_attr->pixel_format == E_PIXEL_FORMAT_YVU_422SP)
            || (p_frame_attr->pixel_format == E_PIXEL_FORMAT_YUV_422SP)
            || (p_frame_attr->pixel_format == E_PIXEL_FORMAT_YVU_420SP)
            || (p_frame_attr->pixel_format == E_PIXEL_FORMAT_YUV_420SP)) {
        bit_width = 8;
    } else if ((p_frame_attr->pixel_format == E_PIXEL_FORMAT_BAYER_10BPP)
            || (p_frame_attr->pixel_format == E_PIXEL_FORMAT_BAYER_12BPP)
            || (p_frame_attr->pixel_format == E_PIXEL_FORMAT_BAYER_14BPP)
            || (p_frame_attr->pixel_format == E_PIXEL_FORMAT_BAYER_16BPP)
            || (p_frame_attr->pixel_format == E_PIXEL_FORMAT_ARGB1555)
            || (p_frame_attr->pixel_format == E_PIXEL_FORMAT_ARGB4444)
            || (p_frame_attr->pixel_format == E_PIXEL_FORMAT_RGB565)) {
        bit_width = 16;
    } else if ((p_frame_attr->pixel_format == E_PIXEL_FORMAT_ARGB8888)) {
        bit_width = 32;
    } else {
        bit_width = 0;
    }

    // compress 128 align
    // non-compress 16 align
    if (p_frame_attr->compress_mode == E_COMPRESS_MODE_NONE) {
        line_align = 64;
    } else {
        line_align = 256;
    }

    stride = STRIDE_UP((p_frame_attr->img_size.width * bit_width / 8), line_align);

    return stride;
}

static inline vs_uint32_t common_frame_buffer_compress_cache_size_get(vs_pixel_format_e pixel_format, vs_uint32_t buf_size)
{
    vs_uint8_t index = 0;
    vs_uint16_t tile_width, tile_height, bpp = 0;
    vs_uint32_t tile_size = 0;
    vs_uint16_t compress_map[3][3] = {
        {256, 1, 8},
        {128, 1, 16},
        {64, 1, 16},
    };

    if (pixel_format == E_PIXEL_FORMAT_YUV_400) {
        index = 0;
    } else if (pixel_format == E_PIXEL_FORMAT_UV_420){
        index = 1;
    } else {
        index = 2;
    }

    tile_width = compress_map[index][0];
    tile_height = compress_map[index][1];
    bpp = compress_map[index][2];

    tile_size = ((tile_width * tile_height) >> 3) * bpp;

    if (pixel_format >= E_PIXEL_FORMAT_BAYER_10BPP && pixel_format <= E_PIXEL_FORMAT_BAYER_16BPP) {
        tile_size = tile_size << 1;
    }

    return STRIDE_UP(((buf_size / tile_size) >> 1), 256);
}

static inline vs_uint32_t common_frame_buffer_size_get(const struct common_frame_buffer_attr *p_frame_attr)
{
    vs_uint32_t stride = 0;
    vs_uint32_t cmp_buf_size, img_buf_size = 0;
    vs_uint32_t buf_size = 0;
    vs_uint32_t y_size, uv_size = 0;
    vs_uint32_t y_cmp_size, uv_cmp_size = 0;
    vs_pixel_format_e y_compress_fmt, uv_compress_fmt;

    if (p_frame_attr == NULL) {
        return 0;
    }

    stride = common_frame_buffer_stride_get(p_frame_attr);

    y_size  = stride * p_frame_attr->img_size.height;
    if (p_frame_attr->pixel_format == E_PIXEL_FORMAT_YVU_422SP ||
        p_frame_attr->pixel_format == E_PIXEL_FORMAT_YUV_422SP) {
        uv_size = y_size;
        y_compress_fmt = E_PIXEL_FORMAT_YUV_400;
        uv_compress_fmt = E_PIXEL_FORMAT_UV_420;
    } else if (p_frame_attr->pixel_format == E_PIXEL_FORMAT_YVU_420SP ||
               p_frame_attr->pixel_format == E_PIXEL_FORMAT_YUV_420SP) {
        uv_size = y_size / 2;
        y_compress_fmt = E_PIXEL_FORMAT_YUV_400;
        uv_compress_fmt = E_PIXEL_FORMAT_UV_420;
    } else {
        uv_size = 0;
        y_compress_fmt = p_frame_attr->pixel_format;
        uv_compress_fmt = p_frame_attr->pixel_format;
    }

    img_buf_size = y_size + uv_size;

    if (p_frame_attr->compress_mode == E_COMPRESS_MODE_NONE) {
        cmp_buf_size = 0;
    } else {
        y_cmp_size  = common_frame_buffer_compress_cache_size_get(y_compress_fmt, y_size);
        uv_cmp_size = common_frame_buffer_compress_cache_size_get(uv_compress_fmt, uv_size);
        cmp_buf_size = y_cmp_size + uv_cmp_size;
    }

    buf_size = cmp_buf_size + img_buf_size;

    return buf_size;
}

static inline vs_uint32_t venc_qpmap_buffer_size_get(vs_uint32_t width, vs_uint32_t height)
{
    vs_uint32_t qpmap_table_stride = 0, qpmap_table_height = 0, qpmap_buffer_size = 0;

    qpmap_table_stride =  STRIDE_UP(width, 64) / 8;
    qpmap_table_height = STRIDE_UP(height, 64) / 8;
    qpmap_buffer_size = STRIDE_UP(qpmap_table_stride * qpmap_table_height, 64);

    return qpmap_buffer_size;
}

static inline vs_uint32_t vdec_frame_buffer_size_get(vs_payload_type_e type,
                                                    vs_uint32_t output_width, vs_uint32_t output_height,
                                                    vs_video_format_e video_format, vs_pixel_format_e pixel_format,
                                                    vs_compress_mode_e compress_mode)
{
#ifndef VS_ORION
    vs_uint32_t align = 0;
    vs_uint32_t stride = 0;
    vs_uint32_t height = 0;
    vs_uint32_t luma_size = 0;
    vs_uint32_t chroma_size = 0;
    vs_uint32_t frame_buff_size = 0;
    vs_uint32_t compress_table_size = 0;
    vs_uint32_t compress_luma_table_size = 0;
    vs_uint32_t compress_chroma_table_size = 0;

    if (video_format != E_VIDEO_FORMAT_LINEAR
        && video_format != E_VIDEO_FORMAT_TILE_4x4) {
        //video_format not support
        return frame_buff_size;
    }

    if (pixel_format != E_PIXEL_FORMAT_YVU_420SP
        && pixel_format != E_PIXEL_FORMAT_YUV_420SP
        && pixel_format != E_PIXEL_FORMAT_YUV_400) {
        //pixel_format not support
        return frame_buff_size;
    }

    if (compress_mode != E_COMPRESS_MODE_NONE
        && compress_mode != E_COMPRESS_MODE_RASTER) {
        //compress_mode not support
        return frame_buff_size;
    }

    /* align */
    if (compress_mode != E_COMPRESS_MODE_NONE) {
        align = 256;
    } else {
        align = 64;
    }

    /* luma */
    if (video_format == E_VIDEO_FORMAT_TILE_4x4) {
        stride = STRIDE_UP(4 * output_width, align);
        height = STRIDE_UP(output_height, 4) / 4;
    } else {
        stride = STRIDE_UP(output_width, align);
        height = output_height;
    }
    luma_size = stride * height;

    /* chroma */
    if (pixel_format != E_PIXEL_FORMAT_YUV_400) {
        if (video_format == E_VIDEO_FORMAT_TILE_4x4) {
            stride = STRIDE_UP(4 * output_width, align);
            height = STRIDE_UP(output_height / 2, 4) / 4;
        } else {
            stride = STRIDE_UP(output_width, align);
            height = output_height / 2;
        }
        chroma_size = stride * height;
    }

    /* frame buff size */
    frame_buff_size = STRIDE_UP(luma_size + chroma_size, 16);

    /* compress table size */
    if (compress_mode == E_COMPRESS_MODE_RASTER) {
        compress_luma_table_size = STRIDE_UP((luma_size / 256 * 4 + 7) / 8, 16);
        compress_chroma_table_size = STRIDE_UP((chroma_size / 256 * 4 + 7) / 8, 16);
        compress_table_size = compress_luma_table_size + compress_chroma_table_size;
    }

    return STRIDE_UP(frame_buff_size + compress_table_size, 4096);
#else
    vs_uint32_t pic_size_in_mbs = 0, frame_buff_size = 0;

    /* not support changing default video_format/pixel_format/compress_mode */

    if (type != E_PT_TYPE_H264 && type != E_PT_TYPE_JPEG && type != E_PT_TYPE_MJPEG) {
        return 0;
    }

    if (type == E_PT_TYPE_H264) {
        pic_size_in_mbs = ((output_width + 15) / 16) * ((output_height + 15) / 16);
        frame_buff_size = (pic_size_in_mbs * 384) + (pic_size_in_mbs * 64) + 32;
    } else {
        frame_buff_size = (STRIDE_UP(output_width, 16) * STRIDE_UP(output_height, 16) * 3) >> 1;
    }

    return STRIDE_UP(frame_buff_size, 4096);
#endif
}

#ifdef __cplusplus
}
#endif

#endif /*!<  __VS_BUFFER_H__ */

