/**
* @file sample_common_venc.c
* @brief sample venc common api implementation
* @details
* @author  Visinex Software Group
* @date 2021-07-20
* @version v0.01
* @Copyright (C) 2020, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/select.h>

#include "vs_comm.h"
#include "sample_common.h"

#ifdef __cplusplus
extern "C" {
#endif

#define QPMAP_SKIP_FLAG_SET 0x80  // skip mb
#define QPMAP_SKIP_FLAG_CLR 0x00
#define QPMAP_ABSQP_FLAG_SET 0x40 // relative qp
#define QPMAP_ABSQP_FLAG_CLR 0x00// Absolute qp
#define VS_VENC_LCU_SIZE_H264 16
#define VS_VENC_LCU_SIZE_H265 64
#define QPMAP_RESET_VALUE 0x40 // skip disable & relative qp & qpdelta 0
#define QPMAP_FRAME_BUF_NUM 8
#define QPMAP_VENC_MAX_CHN 2

typedef struct {
    vs_uint32_t qpmap_table_size[QPMAP_VENC_MAX_CHN];
    vs_uint64_t qpmap_phys_addr[QPMAP_VENC_MAX_CHN][QPMAP_FRAME_BUF_NUM];
    vs_void_t *qpmap_virt_addr[QPMAP_VENC_MAX_CHN][QPMAP_FRAME_BUF_NUM];
} sample_common_venc_qpmap_addr_info;

static vs_uint32_t venc_get_aligned_byte_stride(vs_int32_t width, vs_int32_t input_format, vs_uint32_t *p_luma_stride,
                    vs_uint32_t *p_chroma_stride, vs_uint32_t input_alignment)
{
	vs_uint32_t alignment = input_alignment == 0 ? 1 : input_alignment;
	vs_uint32_t pixelByte = 1;

	if (p_luma_stride == VS_NULL || p_chroma_stride == VS_NULL)
		return 1;

	switch (input_format) {
    case E_PIXEL_FORMAT_YVU_420SP://VCENC_YUV420_SEMIPLANAR
    case E_PIXEL_FORMAT_YUV_420SP://VCENC_YUV420_SEMIPLANAR_VU
		*p_luma_stride = STRIDE_UP(width,alignment);
		*p_chroma_stride = STRIDE_UP(width,alignment);
		break;
    default:
		*p_luma_stride = 0;
		*p_chroma_stride = 0;
		break;
    }
//  vs_sample_trace("luma_stride[%u] chroma_stride[%u] \n", *p_luma_stride, *p_chroma_stride);

  return pixelByte;
}

static vs_void_t venc_get_aligned_picsize_by_format(vs_pixel_format_e type, vs_uint32_t width, vs_uint32_t height,
        vs_uint32_t alignment, vs_uint32_t *p_luma_Size, vs_uint32_t *p_chroma_Size, vs_uint32_t *p_frame_Size)
{
	vs_uint32_t luma_stride = 0, chroma_stride = 0;
	vs_uint32_t luma_size = 0, chroma_size = 0, frame_size = 0;

	switch(type) {
    case E_PIXEL_FORMAT_YVU_420SP:
    case E_PIXEL_FORMAT_YUV_420SP:
		venc_get_aligned_byte_stride(width, E_PIXEL_FORMAT_YUV_420SP, &luma_stride, &chroma_stride, alignment);
		luma_size = luma_stride * height;
		chroma_size = chroma_stride * height/2;
		break;
    default:
		vs_sample_trace("not support this format\n");
		chroma_size = luma_size = 0;
		break;
	}

	frame_size = luma_size + chroma_size;
	if (p_luma_Size != VS_NULL) {
		*p_luma_Size = luma_size;
    }
	if (p_chroma_Size != VS_NULL) {
		*p_chroma_Size = chroma_size;
    }
	if (p_frame_Size != VS_NULL) {
		*p_frame_Size = frame_size;
    }
//	vs_sample_trace("luma_size[%u] chroma_size[%u] frame_size[%u]\n", luma_size, chroma_size, frame_size);
}

static void venc_get_compress_table_by_format(vs_pixel_format_e type, vs_uint32_t width, vs_uint32_t height,
                vs_uint32_t alignment, vs_uint32_t *p_luma_size, vs_uint32_t *p_chroma_size, vs_uint32_t *p_frame_size)
{
	vs_uint32_t luma_stride = 0, chroma_stride = 0;
	vs_uint32_t luma_size = 0, chroma_size = 0, frame_size = 0;
	vs_uint32_t chromaPaddingSize = 0;
	vs_uint32_t tile_size = 256;
	vs_uint32_t bits_tile_in_table = 4;

	if (alignment == 0) {
		alignment = 64;
	}
	venc_get_aligned_byte_stride(width, type, &luma_stride, &chroma_stride, alignment);

	switch (type) {
	case E_PIXEL_FORMAT_YVU_420SP:
	case E_PIXEL_FORMAT_YUV_420SP:
		luma_size = STRIDE_UP(STRIDE_UP((luma_stride * height) / tile_size * bits_tile_in_table, 8) / 8, 16);
		chroma_size = STRIDE_UP(STRIDE_UP((chroma_stride * height / 2) / tile_size * bits_tile_in_table, 8) / 8, 16);
		break;
	default:
		vs_sample_trace("not support this format\n");
		chroma_size = luma_size = 0;
		break;
	}

	frame_size = luma_size + chroma_size;
	frame_size += chromaPaddingSize;

	if (p_luma_size != VS_NULL) {
		*p_luma_size = luma_size;
    }
	if (p_chroma_size != VS_NULL) {
		*p_chroma_size = chroma_size;
    }
	if (p_frame_size != VS_NULL) {
		*p_frame_size = frame_size;
    }
}

vs_int32_t sample_common_venc_user_vb_pool_init(vs_size_s frame_size, vs_pixel_format_e format,
                                                                   vs_bool_t compress, VB_POOL *p_vb_poolid)
{
    struct vs_vb_pool_cfg vb_pool_cfg;
    vs_uint32_t align_size = 0, compress_frame_table_size = 0;
    vs_uint32_t alignment = 64;

    memset(&vb_pool_cfg, 0, sizeof(struct vs_vb_pool_cfg));

    venc_get_aligned_picsize_by_format(format, frame_size.width, frame_size.height, alignment, VS_NULL, VS_NULL, &align_size);
    align_size = STRIDE_UP(align_size, 4096);
    vb_pool_cfg.blk_size = align_size;
    if (compress == VS_TRUE) {
		venc_get_compress_table_by_format(format, frame_size.width, frame_size.height, alignment,
												VS_NULL, VS_NULL, &compress_frame_table_size);
		compress_frame_table_size = STRIDE_UP(compress_frame_table_size, 4096);
		vb_pool_cfg.blk_size += compress_frame_table_size;
	}

    vb_pool_cfg.blk_cnt = 10;
    vb_pool_cfg.remap_mode = VB_REMAP_MODE_NONE;
    *p_vb_poolid = vs_mal_vb_pool_create(&vb_pool_cfg);
    if (*p_vb_poolid == VS_INVALID_POOLID) {
        vs_sample_trace("width[%d] height[%d] vs_mal_vb_pool_create error\n", frame_size.width, frame_size.height);
        return VS_FAILED;
    }
    vs_sample_trace("vb_pool_init vb_poolid[%u] \n", *p_vb_poolid);
    return VS_SUCCESS;
}

vs_int32_t sample_common_venc_user_vb_pool_deinit(VB_POOL vb_poolid)
{
    vs_int32_t ret = VS_SUCCESS;
    ret = vs_mal_vb_pool_destory(vb_poolid);
    if(ret != VS_SUCCESS) {
        vs_sample_trace("vb_poolid[%d] ret[%d] vs_mal_vb_pool_destory error\n", vb_poolid, ret);
        return ret;
    }
    vs_sample_trace("vb_pool_deinit vb_poolid[%u] \n", vb_poolid);
    return VS_SUCCESS;
}

vs_int32_t sample_common_venc_gop_attr_get(vs_venc_gop_mode_e gop_mode, vs_venc_gop_attr_s *p_gop_attr)
{
    if (gop_mode == E_VENC_GOP_MODE_NORMP) {
        p_gop_attr->mode = E_VENC_GOP_MODE_NORMP;
        p_gop_attr->normp.qpdelta_i_p = 1;
    } else if (gop_mode == E_VENC_GOP_MODE_LTRP) {
        p_gop_attr->mode = E_VENC_GOP_MODE_LTRP;
        p_gop_attr->ltrp.interval_lp = 5;
        p_gop_attr->ltrp.qpdelta_i_p = 1;
        p_gop_attr->ltrp.qpdelta_lp_p = 1;
        p_gop_attr->ltrp.lp_pred_enable = VS_FALSE;
    } else if (gop_mode == E_VENC_GOP_MODE_DUALP) {
        p_gop_attr->mode = E_VENC_GOP_MODE_DUALP;
        p_gop_attr->dualp.interval_sp = 2;
        p_gop_attr->dualp.qpdelta_sp_p = 1;
        p_gop_attr->dualp.qpdelta_i_p = 1;
    } else if (gop_mode == E_VENC_GOP_MODE_ADVP) {
        p_gop_attr->mode = E_VENC_GOP_MODE_ADVP;
        p_gop_attr->advp.interval_lp = 2;
        p_gop_attr->advp.qpdelta_lp_p = 1;
        p_gop_attr->advp.qpdelta_i_p = 1;
    } else if (gop_mode == E_VENC_GOP_MODE_NORMB) {
        p_gop_attr->mode = E_VENC_GOP_MODE_NORMB;
        p_gop_attr->normb.frame_num_b = 2;
        p_gop_attr->normb.qpdelta_b_p = 0;
        p_gop_attr->normb.qpdelta_i_p = 1;
    } else if (gop_mode == E_VENC_GOP_MODE_LOWDELAYB) {
        p_gop_attr->mode = E_VENC_GOP_MODE_LOWDELAYB;
        p_gop_attr->lowdelayb.qpdelta_i_b = 1;
    } else {
        printf("gop mode[%d] not support!\n", gop_mode);
        return VS_FAILED;
    }

    return VS_SUCCESS;
}

vs_int32_t sample_common_venc_framerate_set(vs_int32_t venc_chnid, vs_video_frc_s framerate)
{
    vs_int32_t ret = VS_SUCCESS;
    vs_venc_chn_attr_s venc_chn_attr;

    /*set vui time_scale by vii real input frame rate*/
    ret = vs_mal_venc_chn_attr_get(venc_chnid, &venc_chn_attr);
    if (VS_SUCCESS != ret) {
        vs_sample_trace("venc chnid[%d] vs_mal_venc_chn_attr_get error ret [0x%x] !\n",venc_chnid,ret);
        return ret;
    }
    switch(venc_chn_attr.brc_attr.brc_mode) {
	case E_VENC_BRC_MODE_H264_CBR:
		venc_chn_attr.brc_attr.h264_cbr.src_framerate = framerate.src_framerate;
		venc_chn_attr.brc_attr.h264_cbr.dst_framerate = framerate.dst_framerate;
		break;
	case E_VENC_BRC_MODE_H264_VBR:
		venc_chn_attr.brc_attr.h264_vbr.src_framerate = framerate.src_framerate;
        venc_chn_attr.brc_attr.h264_vbr.dst_framerate = framerate.dst_framerate;
		break;
	case E_VENC_BRC_MODE_H264_AVBR:
		venc_chn_attr.brc_attr.h264_avbr.src_framerate = framerate.src_framerate;
        venc_chn_attr.brc_attr.h264_avbr.dst_framerate = framerate.dst_framerate;
		break;
	case E_VENC_BRC_MODE_H264_CVBR:
		venc_chn_attr.brc_attr.h264_cvbr.src_framerate = framerate.src_framerate;
        venc_chn_attr.brc_attr.h264_cvbr.dst_framerate = framerate.dst_framerate;
		break;
	case E_VENC_BRC_MODE_H264_FIXQP:
		venc_chn_attr.brc_attr.h264_fixqp.src_framerate = framerate.src_framerate;
        venc_chn_attr.brc_attr.h264_fixqp.dst_framerate = framerate.dst_framerate;
		break;
	case E_VENC_BRC_MODE_H264_QPMAP:
		venc_chn_attr.brc_attr.h264_qpmap.src_framerate = framerate.src_framerate;
        venc_chn_attr.brc_attr.h264_qpmap.dst_framerate = framerate.dst_framerate;
		break;
	case E_VENC_BRC_MODE_H265_CBR:
		venc_chn_attr.brc_attr.h265_cbr.src_framerate = framerate.src_framerate;
        venc_chn_attr.brc_attr.h265_cbr.dst_framerate = framerate.dst_framerate;
		break;
	case E_VENC_BRC_MODE_H265_VBR:
		venc_chn_attr.brc_attr.h265_vbr.src_framerate = framerate.src_framerate;
        venc_chn_attr.brc_attr.h265_vbr.dst_framerate = framerate.dst_framerate;
		break;
	case E_VENC_BRC_MODE_H265_AVBR:
		venc_chn_attr.brc_attr.h265_avbr.src_framerate = framerate.src_framerate;
        venc_chn_attr.brc_attr.h265_avbr.dst_framerate = framerate.dst_framerate;
		break;
	case E_VENC_BRC_MODE_H265_CVBR:
		venc_chn_attr.brc_attr.h265_cvbr.src_framerate = framerate.src_framerate;
        venc_chn_attr.brc_attr.h265_cvbr.dst_framerate = framerate.dst_framerate;
		break;
	case E_VENC_BRC_MODE_H265_FIXQP:
		venc_chn_attr.brc_attr.h265_fixqp.src_framerate = framerate.src_framerate;
        venc_chn_attr.brc_attr.h265_fixqp.dst_framerate = framerate.dst_framerate;
		break;
	case E_VENC_BRC_MODE_H265_QPMAP:
		venc_chn_attr.brc_attr.h265_qpmap.src_framerate = framerate.src_framerate;
        venc_chn_attr.brc_attr.h265_qpmap.dst_framerate = framerate.dst_framerate;
		break;
	case E_VENC_BRC_MODE_MJPEG_CBR:
		venc_chn_attr.brc_attr.mjpeg_cbr.src_framerate = framerate.src_framerate;
        venc_chn_attr.brc_attr.mjpeg_cbr.dst_framerate = framerate.dst_framerate;
		break;
	case E_VENC_BRC_MODE_MJPEG_VBR:
		venc_chn_attr.brc_attr.mjpeg_vbr.src_framerate = framerate.src_framerate;
        venc_chn_attr.brc_attr.mjpeg_vbr.dst_framerate = framerate.dst_framerate;
		break;
	case E_VENC_BRC_MODE_MJPEG_FIXQP:
		venc_chn_attr.brc_attr.mjpeg_fixqp.src_framerate = framerate.src_framerate;
        venc_chn_attr.brc_attr.mjpeg_fixqp.dst_framerate = framerate.dst_framerate;
		break;
	default:
		return VS_FAILED;
	}
    ret = vs_mal_venc_chn_attr_set(venc_chnid, &venc_chn_attr);
    if (VS_SUCCESS != ret) {
        vs_sample_trace("venc chnid[%d] vs_mal_venc_chn_attr_set error ret [0x%x] !\n",venc_chnid,ret);
        return ret;
    }
    return VS_SUCCESS;
}

vs_int32_t sample_common_venc_video_vui_enable(vs_int32_t venc_chnid)
{
    vs_int32_t ret = VS_SUCCESS, framerate = 0;
    vs_venc_chn_attr_s venc_chn_attr;

    /*set vui time_scale by vii real input frame rate*/
    ret = vs_mal_venc_chn_attr_get(venc_chnid, &venc_chn_attr);
    if (VS_SUCCESS != ret) {
        vs_sample_trace("venc chnid[%d] vs_mal_venc_chn_attr_get error ret [0x%x] !\n",venc_chnid,ret);
        return ret;
    }
    switch(venc_chn_attr.brc_attr.brc_mode) {
	case E_VENC_BRC_MODE_H264_CBR:
		framerate = venc_chn_attr.brc_attr.h264_cbr.dst_framerate;
		break;
	case E_VENC_BRC_MODE_H264_VBR:
		framerate = venc_chn_attr.brc_attr.h264_vbr.dst_framerate;
		break;
	case E_VENC_BRC_MODE_H264_AVBR:
		framerate = venc_chn_attr.brc_attr.h264_avbr.dst_framerate;
		break;
	case E_VENC_BRC_MODE_H264_CVBR:
		framerate = venc_chn_attr.brc_attr.h264_cvbr.dst_framerate;
		break;
	case E_VENC_BRC_MODE_H264_FIXQP:
		framerate = venc_chn_attr.brc_attr.h264_fixqp.dst_framerate;
		break;
	case E_VENC_BRC_MODE_H264_QPMAP:
		framerate = venc_chn_attr.brc_attr.h264_qpmap.dst_framerate;
		break;
	case E_VENC_BRC_MODE_H265_CBR:
		framerate = venc_chn_attr.brc_attr.h265_cbr.dst_framerate;
		break;
	case E_VENC_BRC_MODE_H265_VBR:
		framerate = venc_chn_attr.brc_attr.h265_vbr.dst_framerate;
		break;
	case E_VENC_BRC_MODE_H265_AVBR:
		framerate = venc_chn_attr.brc_attr.h265_avbr.dst_framerate;
		break;
	case E_VENC_BRC_MODE_H265_CVBR:
		framerate = venc_chn_attr.brc_attr.h265_cvbr.dst_framerate;
		break;
	case E_VENC_BRC_MODE_H265_FIXQP:
		framerate = venc_chn_attr.brc_attr.h265_fixqp.dst_framerate;
		break;
	case E_VENC_BRC_MODE_H265_QPMAP:
		framerate = venc_chn_attr.brc_attr.h265_qpmap.dst_framerate;
		break;
	case E_VENC_BRC_MODE_MJPEG_CBR:
		framerate = venc_chn_attr.brc_attr.mjpeg_cbr.dst_framerate;
		break;
	case E_VENC_BRC_MODE_MJPEG_VBR:
		framerate = venc_chn_attr.brc_attr.mjpeg_vbr.dst_framerate;
		break;
	case E_VENC_BRC_MODE_MJPEG_FIXQP:
		framerate = venc_chn_attr.brc_attr.mjpeg_fixqp.dst_framerate;
		break;
	default:
		return VS_FAILED;
	}
    if (venc_chn_attr.enc_attr.type == E_PT_TYPE_H264) {
        vs_venc_h264_vui_s vui;
        ret = vs_mal_venc_h264_vui_get(venc_chnid,&vui);
        if (VS_SUCCESS != ret) {
            vs_sample_trace("venc chnid[%d] vs_mal_venc_h264_vui_get error ret [0x%x] !\n",venc_chnid, ret);
            return ret;
        }
        vui.time_info.timing_info_present_flag =1;
        vui.time_info.num_units_in_tick = 1;
        vui.time_info.time_scale = framerate * 2;
        ret = vs_mal_venc_h264_vui_set(venc_chnid,&vui);
        if (VS_SUCCESS != ret) {
            vs_sample_trace("venc chnid[%d] vs_mal_venc_h264_vui_set error ret [0x%x] !\n",venc_chnid, ret);
            return ret;
        }
    } else if (venc_chn_attr.enc_attr.type == E_PT_TYPE_H265){
        vs_venc_h265_vui_s vui;
        ret = vs_mal_venc_h265_vui_get(venc_chnid,&vui);
        if (VS_SUCCESS != ret) {
            vs_sample_trace("venc chnid[%d] vs_mal_venc_h265_vui_get error ret [0x%x] !\n",venc_chnid, ret);
            return ret;
        }
        vui.time_info.timing_info_present_flag =1;
        vui.time_info.num_units_in_tick = 1;
        vui.time_info.time_scale = framerate;
        ret = vs_mal_venc_h265_vui_set(venc_chnid,&vui);
        if (VS_SUCCESS != ret) {
            vs_sample_trace("venc chnid[%d] vs_mal_venc_h265_vui_set error ret [0x%x] !\n",venc_chnid, ret);
            return ret;
        }
    } else {
        vs_sample_trace("venc chnid[%d] encode_type[%d], do not need to set vui!\n",
                        venc_chnid, venc_chn_attr.enc_attr.type);
        return VS_FAILED;
    }

    return VS_SUCCESS;
}

vs_uint32_t sample_common_venc_bitrate_calc(vs_venc_brc_mode_e brc_mode, vs_size_s frame_size, vs_uint32_t frame_rate)
{
    vs_uint32_t bitrate = 0;

    if (brc_mode >= E_VENC_BRC_MODE_MJPEG_CBR && brc_mode <= E_VENC_BRC_MODE_MJPEG_FIXQP) {
        if (frame_size.width <= PIC_D1_WIDTH && frame_size.height <= PIC_D1_HEIGHT) {
            bitrate =(6 * 1024 + frame_size.width + frame_size.height) * frame_rate / 30;
        } else if (frame_size.width <= PIC_720P_WIDTH && frame_size.height <= PIC_720P_HEIGHT) {
            bitrate = (9 * 1024 + frame_size.width + frame_size.height) * frame_rate / 30;
        } else if (frame_size.width <= PIC_1080P_WIDTH && frame_size.height <= PIC_1080P_HEIGHT) {
            bitrate = (18 * 1024 + frame_size.width + frame_size.height) * frame_rate / 30;
        } else if (frame_size.width <= PIC_2K_WIDTH && frame_size.height <= PIC_2K_HEIGHT) {
            bitrate = (36 * 1024 + frame_size.width + frame_size.height) * frame_rate / 30;
        } else if (frame_size.width <= PIC_2K_2688_WIDTH && frame_size.height <= PIC_2K_1520_HEIGHT) {
            bitrate = (36 * 1024 + frame_size.width + frame_size.height) * frame_rate / 30;
        } else if (frame_size.width <= PIC_4K_WIDTH && frame_size.height <= PIC_4K_HEIGHT) {
            bitrate = (75 * 1024 + frame_size.width + frame_size.height) * frame_rate / 30;
        } else {
            bitrate = (75 * 1024 + frame_size.width + frame_size.height) * frame_rate / 30;
            vs_sample_trace("frame_size.width[%u] use default bitrate[%u]\n", frame_size.width, bitrate);
        }
    } else {
        if (frame_size.width <= PIC_D1_WIDTH && frame_size.height <= PIC_D1_HEIGHT) {
            bitrate = 1 * 1024 + (frame_size.width + frame_size.height) * frame_rate / 30;
        } else if (frame_size.width <= PIC_720P_WIDTH && frame_size.height <= PIC_720P_HEIGHT) {
            bitrate = 2 * 1024 + (frame_size.width + frame_size.height) * frame_rate / 30;
        } else if (frame_size.width <= PIC_1080P_WIDTH && frame_size.height <= PIC_1080P_HEIGHT) {
            bitrate = 2 * 1024 + (frame_size.width + frame_size.height) * frame_rate / 30;
        } else if (frame_size.width <= PIC_2K_WIDTH && frame_size.height <= PIC_2K_HEIGHT) {
            bitrate = 3 * 1024 + (frame_size.width + frame_size.height) * frame_rate / 30;
            bitrate = 6 * 1024;
        } else if (frame_size.width <= PIC_2K_2688_WIDTH && frame_size.height <= PIC_2K_1520_HEIGHT) {
            bitrate = 3 * 1024 + (frame_size.width + frame_size.height) * frame_rate / 30;
            bitrate = 4 * 1024;
        } else if (frame_size.width <= PIC_4K_WIDTH && frame_size.height <= PIC_4K_HEIGHT) {
            bitrate = 5 * 1024 + (frame_size.width + frame_size.height) * frame_rate / 30;
        } else {
            bitrate = 16 * 1024 + (frame_size.width + frame_size.height) * frame_rate / 30;
            vs_sample_trace("frame_size.width[%u] use default bitrate[%u]\n", frame_size.width, bitrate);
        }
    }
    if (bitrate > VENC_MAX_BITRATE) {
        bitrate = VENC_MAX_BITRATE;
    }
    vs_sample_trace("brc_mode[%d] frame_size.width[%u] height[%u] calc bitrate[%u]\n",
                    brc_mode, frame_size.width, frame_size.height, bitrate);
    return bitrate;
}

vs_int32_t sample_common_venc_bitrate_set(vs_int32_t venc_chnid, vs_uint32_t bitrate)
{
    vs_int32_t ret = VS_SUCCESS;
    vs_venc_chn_attr_s venc_chn_attr;
    vs_venc_brc_mode_e brc_mode = E_VENC_BRC_MODE_MAX;

    ret = vs_mal_venc_chn_attr_get(venc_chnid, &venc_chn_attr);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("chn[%d] bitrate[%d] vs_mal_venc_chn_attr_get error! ret[0x%x]\n", venc_chnid, bitrate, ret);
        return VS_FAILED;
    }

    brc_mode = venc_chn_attr.brc_attr.brc_mode;
    switch (venc_chn_attr.enc_attr.type) {
    case E_PT_TYPE_H265:
        if (brc_mode == E_VENC_BRC_MODE_H265_CBR) {
            venc_chn_attr.brc_attr.h265_cbr.bitrate = bitrate;
        } else if (brc_mode == E_VENC_BRC_MODE_H265_VBR) {
            venc_chn_attr.brc_attr.h265_vbr.max_bitrate = bitrate;
        } else if (brc_mode == E_VENC_BRC_MODE_H265_AVBR) {
            venc_chn_attr.brc_attr.h265_avbr.max_bitrate = bitrate;
        } else if (brc_mode == E_VENC_BRC_MODE_H265_CVBR) {
            venc_chn_attr.brc_attr.h265_cvbr.max_bitrate = bitrate;
            venc_chn_attr.brc_attr.h265_cvbr.long_term_max_bitrate = venc_chn_attr.brc_attr.h265_cvbr.max_bitrate;
            venc_chn_attr.brc_attr.h265_cvbr.long_term_min_bitrate = venc_chn_attr.brc_attr.h265_cvbr.max_bitrate / 50 + 10;
        } else if (brc_mode == E_VENC_BRC_MODE_H265_FIXQP) {
            vs_sample_trace("chn[%d] payload_type[%d] brc_mode[%d] not support set bitrate!\n", venc_chnid, venc_chn_attr.enc_attr.type, brc_mode);
            return VS_FAILED;
        } else if (brc_mode == E_VENC_BRC_MODE_H265_QPMAP) {
            vs_sample_trace("chn[%d] payload_type[%d] brc_mode[%d] not support set bitrate!\n", venc_chnid, venc_chn_attr.enc_attr.type, brc_mode);
            return VS_FAILED;
        } else {
            vs_sample_trace("chn[%d] brc_mode[%d] not support!\n", venc_chnid, brc_mode);
            return VS_FAILED;
        }
        break;
    case E_PT_TYPE_H264:
        if (brc_mode == E_VENC_BRC_MODE_H264_CBR) {
            venc_chn_attr.brc_attr.h264_cbr.bitrate = bitrate;
        } else if (brc_mode == E_VENC_BRC_MODE_H264_VBR) {
            venc_chn_attr.brc_attr.h264_vbr.max_bitrate = bitrate;
        } else if (brc_mode == E_VENC_BRC_MODE_H264_AVBR) {
            venc_chn_attr.brc_attr.h264_avbr.max_bitrate = bitrate;
        } else if (brc_mode == E_VENC_BRC_MODE_H264_CVBR) {
            venc_chn_attr.brc_attr.h264_cvbr.max_bitrate = bitrate;
            venc_chn_attr.brc_attr.h264_cvbr.long_term_max_bitrate = venc_chn_attr.brc_attr.h264_cvbr.max_bitrate;
            venc_chn_attr.brc_attr.h264_cvbr.long_term_min_bitrate = venc_chn_attr.brc_attr.h264_cvbr.max_bitrate / 50 + 10;
        } else if (brc_mode == E_VENC_BRC_MODE_H264_FIXQP) {
            vs_sample_trace("chn[%d] payload_type[%d] brc_mode[%d] not support set bitrate!\n", venc_chnid, venc_chn_attr.enc_attr.type, brc_mode);
            return VS_FAILED;
        } else if (brc_mode == E_VENC_BRC_MODE_H264_QPMAP) {
            vs_sample_trace("chn[%d] payload_type[%d] brc_mode[%d] not support set bitrate!\n", venc_chnid, venc_chn_attr.enc_attr.type, brc_mode);
            return VS_FAILED;
        } else {
            vs_sample_trace("chn[%d] brc_mode[%d] not support!\n", venc_chnid, brc_mode);
            return VS_FAILED;
        }
        break;
    case E_PT_TYPE_MJPEG:
        if (brc_mode == E_VENC_BRC_MODE_MJPEG_CBR) {
            venc_chn_attr.brc_attr.mjpeg_cbr.bitrate = bitrate;
        } else if (brc_mode == E_VENC_BRC_MODE_MJPEG_VBR) {
            venc_chn_attr.brc_attr.mjpeg_vbr.max_bitrate = bitrate;
        } else if (brc_mode == E_VENC_BRC_MODE_MJPEG_FIXQP) {
            vs_sample_trace("chn[%d] payload_type[%d] brc_mode[%d] not support set bitrate!\n", venc_chnid, venc_chn_attr.enc_attr.type, brc_mode);
            return VS_FAILED;
        } else {
            vs_sample_trace("chn[%d] brc_mode[%d] not support!\n", venc_chnid, brc_mode);
            return VS_FAILED;
        }
        break;
    case E_PT_TYPE_JPEG:
        vs_sample_trace("chn[%d] payload_type[%d] brc_mode[%d] not support set bitrate!\n", venc_chnid, venc_chn_attr.enc_attr.type, brc_mode);
        return VS_FAILED;
        //break;
    default:
        vs_sample_trace("chn[%d] payload type[%d] not support!\n", venc_chnid, venc_chn_attr.enc_attr.type);
        return VS_FAILED;
    }

    ret = vs_mal_venc_chn_attr_set(venc_chnid, &venc_chn_attr);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_venc_chn_attr_set[%d] failed, ret[0x%x]\n",venc_chnid, ret);
        return ret;
    } else {
        vs_sample_trace("chn[%d] payload_type[%d] brc_mode[%d] set bitrate[%u]!\n", venc_chnid, venc_chn_attr.enc_attr.type, brc_mode, bitrate);
    }

    return VS_SUCCESS;
}

static vs_int32_t sample_common_venc_create(vs_int32_t venc_chnid, sample_venc_cfg_s *p_sample_venc_cfg)
{
    vs_int32_t ret = VS_SUCCESS;
    vs_venc_chn_attr_s venc_chn_attr;
    vs_uint32_t gop = 30;
    vs_uint32_t bitrate_window = 1;
    vs_payload_type_e type = p_sample_venc_cfg->type;
    vs_venc_profile_e profile = p_sample_venc_cfg->profile;
    vs_size_s frame_size = p_sample_venc_cfg->frame_size;
    sample_brc_mode_e brc_mode = p_sample_venc_cfg->brc_mode;
    vs_venc_gop_attr_s gop_attr = p_sample_venc_cfg->gop_attr;
    vs_video_frc_s frc = p_sample_venc_cfg->frc;

    if (frc.dst_framerate < 1 || frc.dst_framerate > 240
        || frc.src_framerate < 1 || frc.src_framerate > 240) {
        vs_sample_trace("dst_framerate[%u] src_framerate[%u] not support! \n", frc.dst_framerate, frc.src_framerate);
        return VS_FAILED;
    }
	switch(frc.dst_framerate) {
	case 10:
		gop = 30;
		bitrate_window = 3;
		break;
	case 15:
		gop = 30;
		bitrate_window = 2;
		break;
	case 25:
		gop = 50;
		bitrate_window = 2;
		break;
	case 30:
		gop = 30;
		bitrate_window = 1;
		break;
	case 60:
		gop = 60;
		bitrate_window = 1;
		break;
	default:
		gop = 30;
		bitrate_window = 1;
		break;
	}

    memset(&venc_chn_attr, 0, sizeof(venc_chn_attr));
    venc_chn_attr.enc_attr.type = type;
    venc_chn_attr.enc_attr.max_frame_width = frame_size.width;
    venc_chn_attr.enc_attr.max_frame_height = frame_size.height;
    venc_chn_attr.enc_attr.frame_width = frame_size.width;
    venc_chn_attr.enc_attr.frame_height = frame_size.height;
    venc_chn_attr.enc_attr.stream_buf_size = STRIDE_UP(frame_size.height * frame_size.width * 2, 4096);
    venc_chn_attr.enc_attr.profile = profile;
    venc_chn_attr.enc_attr.stream_mode = E_VENC_STREAM_MODE_FRAME;

    switch (type) {
    case E_PT_TYPE_H265:
        if (brc_mode == E_VENC_BRC_CBR) {
            vs_venc_brc_h265_cbr_s h265cbr;

            venc_chn_attr.brc_attr.brc_mode = E_VENC_BRC_MODE_H265_CBR;
            h265cbr.gop = gop;
            h265cbr.bitrate_window = bitrate_window;
            h265cbr.src_framerate = frc.src_framerate;
            h265cbr.dst_framerate = frc.dst_framerate;
            h265cbr.bitrate = sample_common_venc_bitrate_calc(venc_chn_attr.brc_attr.brc_mode, frame_size, frc.dst_framerate);
            memcpy(&venc_chn_attr.brc_attr.h265_cbr, &h265cbr, sizeof(vs_venc_brc_h265_cbr_s));
        } else if (brc_mode == E_VENC_BRC_VBR) {
            vs_venc_brc_h265_vbr_s h265vbr;

            venc_chn_attr.brc_attr.brc_mode = E_VENC_BRC_MODE_H265_VBR;
            h265vbr.gop = gop;
            h265vbr.bitrate_window = bitrate_window;
            h265vbr.src_framerate = frc.src_framerate;
            h265vbr.dst_framerate = frc.dst_framerate;
            h265vbr.max_bitrate = sample_common_venc_bitrate_calc(venc_chn_attr.brc_attr.brc_mode, frame_size, frc.dst_framerate);
            memcpy(&venc_chn_attr.brc_attr.h265_vbr, &h265vbr, sizeof(vs_venc_brc_h265_vbr_s));
        } else if (brc_mode == E_VENC_BRC_AVBR) {
            vs_venc_brc_h265_avbr_s h265avbr;

            venc_chn_attr.brc_attr.brc_mode = E_VENC_BRC_MODE_H265_AVBR;
            h265avbr.gop = gop;
            h265avbr.bitrate_window = bitrate_window;
            h265avbr.src_framerate = frc.src_framerate;
            h265avbr.dst_framerate = frc.dst_framerate;
            h265avbr.max_bitrate = sample_common_venc_bitrate_calc(venc_chn_attr.brc_attr.brc_mode, frame_size, frc.dst_framerate);
            memcpy(&venc_chn_attr.brc_attr.h265_avbr, &h265avbr, sizeof(vs_venc_brc_h265_avbr_s));
            //vs_sample_trace("brc_mode[%d] not support!\n", brc_mode);
            //return VS_FAILED;
        } else if (brc_mode == E_VENC_BRC_CVBR) {
            vs_venc_brc_h265_cvbr_s h265cvbr;

            venc_chn_attr.brc_attr.brc_mode = E_VENC_BRC_MODE_H265_CVBR;
            h265cvbr.gop = gop;
            h265cvbr.bitrate_window = bitrate_window;
            h265cvbr.src_framerate = frc.src_framerate;
            h265cvbr.dst_framerate = frc.dst_framerate;
            h265cvbr.max_bitrate = sample_common_venc_bitrate_calc(venc_chn_attr.brc_attr.brc_mode, frame_size, frc.dst_framerate);
            h265cvbr.short_term_bitrate_window = (frc.dst_framerate / 10 > 2) ? (frc.dst_framerate / 10) : 3;
            h265cvbr.long_term_bitrate_window = frc.dst_framerate;
            h265cvbr.long_term_max_bitrate = h265cvbr.max_bitrate;
            h265cvbr.long_term_min_bitrate = h265cvbr.max_bitrate / 50 + 10;
            memcpy(&venc_chn_attr.brc_attr.h265_cvbr, &h265cvbr, sizeof(vs_venc_brc_h265_cvbr_s));
            //vs_sample_trace("brc_mode[%d] not support!\n", brc_mode);
            //return VS_FAILED;
        } else if (brc_mode == E_VENC_BRC_FIXQP) {
            vs_venc_brc_h265_fixqp_s h265fixqp;

            venc_chn_attr.brc_attr.brc_mode = E_VENC_BRC_MODE_H265_FIXQP;
            h265fixqp.gop = gop;
            h265fixqp.src_framerate = frc.src_framerate;
            h265fixqp.dst_framerate = frc.dst_framerate;
            h265fixqp.fixqp_i = 24;
            h265fixqp.fixqp_p = 30;
            h265fixqp.fixqp_b = 32;
            memcpy(&venc_chn_attr.brc_attr.h265_fixqp, &h265fixqp, sizeof(vs_venc_brc_h265_fixqp_s));
        } else if (brc_mode == E_VENC_BRC_QPMAP) {
            vs_venc_brc_h265_qpmap_s h265qpmap;

            venc_chn_attr.brc_attr.brc_mode = E_VENC_BRC_MODE_H265_QPMAP;
            h265qpmap.gop = gop;
            h265qpmap.bitrate_window = bitrate_window;
            h265qpmap.src_framerate = frc.src_framerate;
            h265qpmap.dst_framerate = frc.dst_framerate;
            memcpy(&venc_chn_attr.brc_attr.h265_qpmap, &h265qpmap, sizeof(vs_venc_brc_h265_qpmap_s));
        } else {
            vs_sample_trace("brc_mode[%d] not support!\n", brc_mode);
            return VS_FAILED;
        }
        venc_chn_attr.enc_attr.h265_attr.bandwidth_save_strength = p_sample_venc_cfg->bandwidth_save_strength;
        break;
    case E_PT_TYPE_H264:
        if (brc_mode == E_VENC_BRC_CBR) {
            vs_venc_brc_h264_cbr_s h264cbr;

            venc_chn_attr.brc_attr.brc_mode = E_VENC_BRC_MODE_H264_CBR;
            h264cbr.gop = gop;
            h264cbr.bitrate_window = bitrate_window;
            h264cbr.src_framerate = frc.src_framerate;
            h264cbr.dst_framerate = frc.dst_framerate;
            h264cbr.bitrate = sample_common_venc_bitrate_calc(venc_chn_attr.brc_attr.brc_mode, frame_size, frc.dst_framerate);
            memcpy(&venc_chn_attr.brc_attr.h264_cbr, &h264cbr, sizeof(vs_venc_brc_h264_cbr_s));
        } else if (brc_mode == E_VENC_BRC_VBR) {
            vs_venc_brc_h264_vbr_s h264vbr;

            venc_chn_attr.brc_attr.brc_mode = E_VENC_BRC_MODE_H264_VBR;
            h264vbr.gop = gop;
            h264vbr.bitrate_window = bitrate_window;
            h264vbr.src_framerate = frc.src_framerate;
            h264vbr.dst_framerate = frc.dst_framerate;
            h264vbr.max_bitrate = sample_common_venc_bitrate_calc(venc_chn_attr.brc_attr.brc_mode, frame_size, frc.dst_framerate);
            memcpy(&venc_chn_attr.brc_attr.h264_vbr, &h264vbr, sizeof(vs_venc_brc_h264_vbr_s));
        } else if (brc_mode == E_VENC_BRC_AVBR) {
            vs_venc_brc_h264_avbr_s h264avbr;

            venc_chn_attr.brc_attr.brc_mode = E_VENC_BRC_MODE_H264_AVBR;
            h264avbr.gop = gop;
            h264avbr.bitrate_window = bitrate_window;
            h264avbr.src_framerate = frc.src_framerate;
            h264avbr.dst_framerate = frc.dst_framerate;
            h264avbr.max_bitrate = sample_common_venc_bitrate_calc(venc_chn_attr.brc_attr.brc_mode, frame_size, frc.dst_framerate);
            memcpy(&venc_chn_attr.brc_attr.h264_avbr, &h264avbr, sizeof(vs_venc_brc_h264_avbr_s));
            //vs_sample_trace("brc_mode[%d] not support!\n", brc_mode);
            //return VS_FAILED;
        } else if (brc_mode == E_VENC_BRC_CVBR) {
            vs_venc_brc_h264_cvbr_s h264cvbr;

            venc_chn_attr.brc_attr.brc_mode = E_VENC_BRC_MODE_H264_CVBR;
            h264cvbr.gop = gop;
            h264cvbr.bitrate_window = bitrate_window;
            h264cvbr.src_framerate = frc.src_framerate;
            h264cvbr.dst_framerate = frc.dst_framerate;
            h264cvbr.max_bitrate = sample_common_venc_bitrate_calc(venc_chn_attr.brc_attr.brc_mode, frame_size, frc.dst_framerate);
            h264cvbr.short_term_bitrate_window = (frc.dst_framerate / 10 > 2) ? (frc.dst_framerate / 10) : 3;
            h264cvbr.long_term_bitrate_window = frc.dst_framerate;
            h264cvbr.long_term_max_bitrate = h264cvbr.max_bitrate;
            h264cvbr.long_term_min_bitrate = h264cvbr.max_bitrate / 50 + 10;
            memcpy(&venc_chn_attr.brc_attr.h264_cvbr, &h264cvbr, sizeof(vs_venc_brc_h264_cvbr_s));
            //vs_sample_trace("brc_mode[%d] not support!\n", brc_mode);
            //return VS_FAILED;
        } else if (brc_mode == E_VENC_BRC_FIXQP) {
            vs_venc_brc_h264_fixqp_s h264fixqp;

            venc_chn_attr.brc_attr.brc_mode = E_VENC_BRC_MODE_H264_FIXQP;
            h264fixqp.gop = gop;
            h264fixqp.src_framerate = frc.src_framerate;
            h264fixqp.dst_framerate = frc.dst_framerate;
            h264fixqp.fixqp_i = 24;
            h264fixqp.fixqp_p = 30;
            h264fixqp.fixqp_b = 32;
            memcpy(&venc_chn_attr.brc_attr.h264_fixqp, &h264fixqp, sizeof(vs_venc_brc_h264_fixqp_s));
        } else if (brc_mode == E_VENC_BRC_QPMAP) {
            vs_venc_brc_h264_qpmap_s h264qpmap;

            venc_chn_attr.brc_attr.brc_mode = E_VENC_BRC_MODE_H264_QPMAP;
            h264qpmap.gop = gop;
            h264qpmap.bitrate_window = bitrate_window;
            h264qpmap.src_framerate = frc.src_framerate;
            h264qpmap.dst_framerate = frc.dst_framerate;
            memcpy(&venc_chn_attr.brc_attr.h264_qpmap, &h264qpmap, sizeof(vs_venc_brc_h264_qpmap_s));
        } else {
            vs_sample_trace("brc_mode[%d] not support!\n", brc_mode);
            return VS_FAILED;
        }
        venc_chn_attr.enc_attr.h264_attr.bandwidth_save_strength = p_sample_venc_cfg->bandwidth_save_strength;
        break;
    case E_PT_TYPE_MJPEG:
        venc_chn_attr.enc_attr.jpeg_attr.input_type = p_sample_venc_cfg->format;
        if (brc_mode == E_VENC_BRC_CBR) {
            vs_venc_brc_mjpeg_cbr_s mjpegcbr;

            venc_chn_attr.brc_attr.brc_mode = E_VENC_BRC_MODE_MJPEG_CBR;
            mjpegcbr.bitrate_window = bitrate_window;
            mjpegcbr.src_framerate = frc.src_framerate;
            mjpegcbr.dst_framerate = frc.dst_framerate;
            mjpegcbr.bitrate = sample_common_venc_bitrate_calc(venc_chn_attr.brc_attr.brc_mode, frame_size, frc.dst_framerate);
            memcpy(&venc_chn_attr.brc_attr.mjpeg_cbr, &mjpegcbr, sizeof(vs_venc_brc_mjpeg_cbr_s));
        } else if (brc_mode == E_VENC_BRC_VBR || brc_mode == E_VENC_BRC_AVBR || brc_mode == E_VENC_BRC_CVBR) {
            vs_venc_brc_mjpeg_vbr_s mjpegvbr;

            venc_chn_attr.brc_attr.brc_mode = E_VENC_BRC_MODE_MJPEG_VBR;
            mjpegvbr.bitrate_window = bitrate_window;
            mjpegvbr.src_framerate = frc.src_framerate;
            mjpegvbr.dst_framerate = frc.dst_framerate;
            mjpegvbr.max_bitrate = sample_common_venc_bitrate_calc(venc_chn_attr.brc_attr.brc_mode, frame_size, frc.dst_framerate);
            memcpy(&venc_chn_attr.brc_attr.mjpeg_vbr, &mjpegvbr, sizeof(vs_venc_brc_mjpeg_vbr_s));
        } else if (brc_mode == E_VENC_BRC_FIXQP) {
            vs_venc_brc_mjpeg_fixqp_s mjpegfixqp;

            venc_chn_attr.brc_attr.brc_mode = E_VENC_BRC_MODE_MJPEG_FIXQP;
            mjpegfixqp.src_framerate = frc.src_framerate;
            mjpegfixqp.dst_framerate = frc.dst_framerate;
            mjpegfixqp.qfactor = 24;
            memcpy(&venc_chn_attr.brc_attr.mjpeg_fixqp, &mjpegfixqp, sizeof(vs_venc_brc_mjpeg_fixqp_s));
        } else {
            vs_sample_trace("brc_mode[%d] not support!\n", brc_mode);
            return VS_FAILED;
        }
        break;
    case E_PT_TYPE_JPEG:
        venc_chn_attr.enc_attr.jpeg_attr.input_type = p_sample_venc_cfg->format;//E_PIXEL_FORMAT_YUV_420SP;
        venc_chn_attr.enc_attr.jpeg_attr.rcv_mode = E_VENC_PIC_RECEIVE_MODE_SINGLE;
        venc_chn_attr.enc_attr.jpeg_attr.mpf_cfg.thumbnail_num = 0;
        break;
    default:
        vs_sample_trace("payload type[%d] not support!\n", type);
        return VS_FAILED;
    }

    if (type == E_PT_TYPE_H264 || type == E_PT_TYPE_H265) {
        memcpy(&venc_chn_attr.gop_attr, &gop_attr, sizeof(vs_venc_gop_attr_s));
    } else if (type == E_PT_TYPE_MJPEG || type == E_PT_TYPE_JPEG) {
        venc_chn_attr.gop_attr.mode = E_VENC_GOP_MODE_NORMP;
        venc_chn_attr.gop_attr.normp.qpdelta_i_p = 0;
    }

    ret = vs_mal_venc_chn_create(venc_chnid, &venc_chn_attr);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_venc_chn_create[%d] failed, ret[0x%x]\n",venc_chnid, ret);
        return ret;
    }

    if (type == E_PT_TYPE_JPEG) {
        vs_venc_chn_param_s chn_param;
        ret = vs_mal_venc_chn_param_get(venc_chnid, &chn_param);
        if (ret != VS_SUCCESS) {
            vs_sample_trace("vs_mal_venc_chn_param_get[%d] failed, ret[0x%x]\n",venc_chnid, ret);
            return ret;
        }
        chn_param.framerate_cfg.src_framerate = frc.src_framerate;
        chn_param.framerate_cfg.dst_framerate = frc.dst_framerate;
        ret = vs_mal_venc_chn_param_set(venc_chnid, &chn_param);
        if (ret != VS_SUCCESS) {
            vs_sample_trace("vs_mal_venc_chn_param_set[%d] failed, ret[0x%x]\n",venc_chnid, ret);
            return ret;
        }
    }

    return VS_SUCCESS;
}

vs_int32_t sample_common_venc_start(vs_int32_t venc_chnid, sample_venc_cfg_s *p_sample_venc_cfg)
{
    vs_int32_t ret = VS_SUCCESS;
    vs_venc_start_param_s start_param;

    /*create encode chn*/
    ret = sample_common_venc_create(venc_chnid, p_sample_venc_cfg);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("chnid[%d] comm_venc_create failed, ret[0x%x]\n", venc_chnid, ret);
        return ret;
    }

    if (p_sample_venc_cfg->type == E_PT_TYPE_H264 || p_sample_venc_cfg->type == E_PT_TYPE_H265) {
        ret = sample_common_venc_video_vui_enable(venc_chnid);
        if (ret != VS_SUCCESS) {
            vs_sample_trace("chnid[%d] video_vui_enable failed, ret[0x%x]\n", venc_chnid, ret);
            vs_mal_venc_chn_destroy(venc_chnid);
            return ret;
        }
    }

    /*start recv venc frame*/
    start_param.rcvframe_num = 0;
    ret = vs_mal_venc_chn_start(venc_chnid, &start_param);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("chnid[%d] comm_venc_create failed, ret[0x%x]\n", venc_chnid, ret);
        return ret;
    }

    return VS_SUCCESS;
}

vs_int32_t sample_common_venc_stop(vs_int32_t venc_chnid)
{
    vs_int32_t ret = VS_SUCCESS;

    /*stop recv venc frame*/
    ret = vs_mal_venc_chn_stop(venc_chnid);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("chnid[%d] vs_mal_venc_chn_stop failed, ret[0x%x]\n", venc_chnid, ret);
        return ret;
    }

    /*destroy encode chn*/
    ret = vs_mal_venc_chn_destroy(venc_chnid);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("chnid[%d] vs_mal_venc_chn_destroy failed, ret[0x%x]\n", venc_chnid, ret);
        return ret;
    }

    return VS_SUCCESS;
}

vs_int32_t sample_common_venc_mpf_start(vs_int32_t venc_chnid, sample_venc_cfg_s *p_sample_venc_cfg, vs_venc_jpeg_attr_s *p_jpeg_attr)
{
    vs_int32_t ret = VS_SUCCESS;
    vs_venc_chn_attr_s venc_chn_attr;
    vs_size_s frame_size = p_sample_venc_cfg->frame_size;

    memset(&venc_chn_attr, 0, sizeof(venc_chn_attr));
    venc_chn_attr.enc_attr.type = p_sample_venc_cfg->type;
    venc_chn_attr.enc_attr.max_frame_width = frame_size.width;
    venc_chn_attr.enc_attr.max_frame_height = frame_size.height;
    venc_chn_attr.enc_attr.frame_width = frame_size.width;
    venc_chn_attr.enc_attr.frame_height = frame_size.height;
    venc_chn_attr.enc_attr.stream_buf_size = STRIDE_UP(frame_size.height * frame_size.width * 2, 4096);
    venc_chn_attr.enc_attr.profile = p_sample_venc_cfg->profile;;
    venc_chn_attr.enc_attr.stream_mode = E_VENC_STREAM_MODE_FRAME;
    memcpy(&venc_chn_attr.enc_attr.jpeg_attr, p_jpeg_attr, sizeof(vs_venc_jpeg_attr_s));

    ret = vs_mal_venc_chn_create(venc_chnid, &venc_chn_attr);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_venc_chn_create[%d] failed, ret[0x%x]\n",venc_chnid, ret);
        return ret;
    }

    if (p_sample_venc_cfg->type == E_PT_TYPE_JPEG) {
        vs_venc_chn_param_s chn_param;
        ret = vs_mal_venc_chn_param_get(venc_chnid, &chn_param);
        if (ret != VS_SUCCESS) {
            vs_sample_trace("vs_mal_venc_chn_param_get[%d] failed, ret[0x%x]\n",venc_chnid, ret);
            return ret;
        }
        chn_param.framerate_cfg.src_framerate = p_sample_venc_cfg->frc.src_framerate;
        chn_param.framerate_cfg.dst_framerate = p_sample_venc_cfg->frc.dst_framerate;
        ret = vs_mal_venc_chn_param_set(venc_chnid, &chn_param);
        if (ret != VS_SUCCESS) {
            vs_sample_trace("vs_mal_venc_chn_param_set[%d] failed, ret[0x%x]\n",venc_chnid, ret);
            return ret;
        }
    }

    return VS_SUCCESS;
}

vs_int32_t sample_common_venc_mpf_stop(vs_int32_t venc_chnid)
{
    vs_int32_t ret = VS_SUCCESS;

    /*destroy encode chn*/
    ret = vs_mal_venc_chn_destroy(venc_chnid);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("chnid[%d] vs_mal_venc_chn_destroy failed, ret[0x%x]\n", venc_chnid, ret);
        return ret;
    }

    return VS_SUCCESS;
}

static vs_void_t venc_get_encodetype_postfix(vs_payload_type_e type, char *p_postfix_buf, vs_int32_t buf_len)
{
    memset(p_postfix_buf, 0, buf_len);
    switch (type){
    case E_PT_TYPE_H264:
        strcpy(p_postfix_buf, "h264");
        break;
    case E_PT_TYPE_H265:
        strcpy(p_postfix_buf, "h265");
        break;
    case E_PT_TYPE_MJPEG:
        strcpy(p_postfix_buf, "mjpeg");
        break;
    case E_PT_TYPE_JPEG:
        strcpy(p_postfix_buf, "jpeg");
        break;
    default:
        vs_sample_trace("payload_type[%d] not support!\n", type);
        strcpy(p_postfix_buf, "UnknownEncodeType");
        break;
    }
}

static vs_int32_t venc_stream_output_open(sample_venc_acquire_stream_param_s *p_task_param, vs_int32_t chn_index, vs_int32_t stream_index)
{
    char postfix[MAX_FILE_PATH_LEN];
	char output_file_name[MAX_FILE_PATH_LEN];
    vs_int32_t position = 0;

    memset(output_file_name, 0, sizeof(output_file_name));
    if (p_task_param->p_out_prefix[chn_index] != VS_NULL) {
        position += snprintf(output_file_name + position, MAX_FILE_PATH_LEN - position, "%s.chn%d.index%d.",
                             p_task_param->p_out_prefix[chn_index], p_task_param->venc_chnid[chn_index], stream_index);
        venc_get_encodetype_postfix(p_task_param->chn_attr[chn_index].enc_attr.type, postfix, MAX_FILE_PATH_LEN);
        position += snprintf(output_file_name + position, MAX_FILE_PATH_LEN - position, "%s", postfix);
    } else {
        position += snprintf(output_file_name + position, MAX_FILE_PATH_LEN - position, "%s.chn%d.index%d.",
                                 "strm", p_task_param->venc_chnid[chn_index], stream_index);
        venc_get_encodetype_postfix(p_task_param->chn_attr[chn_index].enc_attr.type, postfix, MAX_FILE_PATH_LEN);
        position += snprintf(output_file_name + position, MAX_FILE_PATH_LEN - position, "%s", postfix);
    }

	p_task_param->p_out_file[chn_index] = fopen(output_file_name, "wb");
	if (VS_NULL == p_task_param->p_out_file[chn_index]) {
	    vs_sample_trace(" chn[%d] fopen output_path=%s error!\n",
            p_task_param->venc_chnid[chn_index], output_file_name);
		p_task_param->stop_stream_task = VS_TRUE;
		return VS_FAILED;
	} else {
        vs_sample_trace(" chn[%d] fopen output_path=%s success!\n",
            p_task_param->venc_chnid[chn_index], output_file_name);
    }

	return VS_SUCCESS;
}

static vs_int32_t venc_stream_data_write(sample_venc_acquire_stream_param_s *p_task_param, vs_venc_stream_s *p_stream, vs_bool_t *p_end, vs_int32_t chn_index)
{
	vs_uint32_t i = 0, j = 0, wr_len = 0;

	for (i = 0; i < p_stream->pack_num ; i++) {
		if (0 == p_stream->p_pack[i].length) {
			continue;
		}

		if (VS_TRUE == p_stream->p_pack[i].is_frame_end) {
			*p_end = VS_TRUE;
		}

		switch (p_task_param->chn_attr[chn_index].enc_attr.stream_mode) {
		case E_VENC_STREAM_MODE_STREAM:
            fwrite(p_stream->p_pack[i].virt_addr, 1, p_stream->p_pack[i].length, p_task_param->p_out_file[chn_index]);
            fflush(p_task_param->p_out_file[chn_index]);
			break;
		case E_VENC_STREAM_MODE_FRAME:
            wr_len = 0;
            for (j = 0; j < p_stream->p_pack[i].data_num; j++) {
        		fwrite(p_stream->p_pack[i].virt_addr + p_stream->p_pack[i].pack_info[j].pack_offset, 1,
                   p_stream->p_pack[i].pack_info[j].pack_len, p_task_param->p_out_file[chn_index]);
        		wr_len += p_stream->p_pack[i].pack_info[j].pack_len;
        	}
            fwrite(p_stream->p_pack[i].virt_addr + p_stream->p_pack[i].offset, 1,
                   p_stream->p_pack[i].length - wr_len, p_task_param->p_out_file[chn_index]);
            fflush(p_task_param->p_out_file[chn_index]);
			break;
		case E_VENC_STREAM_MODE_NALU:
            fwrite(p_stream->p_pack[i].virt_addr, 1, p_stream->p_pack[i].length, p_task_param->p_out_file[chn_index]);
            fflush(p_task_param->p_out_file[chn_index]);
			break;
		default:
            vs_sample_trace(" chn[%d] stream_mode=%d not support!\n",
                            p_task_param->venc_chnid[chn_index], p_task_param->chn_attr[chn_index].enc_attr.stream_mode);
			break;
		}
	}

	return VS_SUCCESS;
}

vs_int32_t sample_common_venc_acquire_jpeg_mpf_process(sample_venc_acquire_stream_param_s *p_acquire_stream_param, vs_int32_t rcvframe_num)
{
    vs_int32_t ret = VS_SUCCESS;
	vs_int32_t i = 0;
	vs_int32_t venc_fd = -1;
	vs_bool_t frame_end = VS_FALSE;
	struct timeval timeout_val;
	fd_set read_fds;
    //vs_int32_t received_frame_num[VENC_MAX_CHN_NUM] = {0};
	struct vs_venc_chn_status chn_status;
	vs_venc_stream_s out_stream;

    vs_venc_start_param_s start_param;

    vs_sample_trace("sample_common_venc_acquire_jpeg_mpf started! store_strm[%d] rcvframe_num[%d]\n",
                    p_acquire_stream_param->store_strm, rcvframe_num);
    /*start recv venc frame*/
    start_param.rcvframe_num = rcvframe_num;
    ret = vs_mal_venc_chn_start(p_acquire_stream_param->venc_chnid[0], &start_param);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("chnid[%d] comm_venc_create failed, ret[0x%x]\n", p_acquire_stream_param->venc_chnid[0], ret);
        return ret;
    }
    ret = vs_mal_venc_chn_attr_get(p_acquire_stream_param->venc_chnid[0], &p_acquire_stream_param->chn_attr[0]);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("sample_common_vpp_bind_venc failed, ret[0x%x]\n", ret);
        return ret;
    }
    venc_fd = vs_mal_venc_chn_fd_get(p_acquire_stream_param->venc_chnid[0]);
    if (venc_fd < 0) {
        vs_sample_trace("chnid[%d] comm_venc_create failed, venc_fd[%d]\n", p_acquire_stream_param->venc_chnid[0], venc_fd);
        return VS_FAILED;
    }

	for (i = 0; i < rcvframe_num; i++) {
		timeout_val.tv_sec = 1;
        timeout_val.tv_usec = 0;
        FD_ZERO(&read_fds);
        FD_SET(venc_fd, &read_fds);
        ret = select(venc_fd + 1, &read_fds, NULL, NULL, &timeout_val);
        if (ret < 0) {
            vs_sample_trace(" get venc frame select error, ret[%d] break! \n", ret);
            break;
        } else if (0 == ret) {
            vs_sample_trace(" get venc frame select time out, continue! \n");
            continue;
        } else {
            if (FD_ISSET(venc_fd, &read_fds)) {
				ret = vs_mal_venc_chn_status_get(p_acquire_stream_param->venc_chnid[0], &chn_status);
				if (VS_SUCCESS != ret || (chn_status.left_stream_frame_num < 1)) {
					continue;
				}

				memset(&out_stream, 0, sizeof(vs_venc_stream_s));
				out_stream.p_pack = (vs_venc_pack_s *)calloc(1, sizeof(vs_venc_pack_s) * chn_status.cur_pack_num);
				if (VS_NULL == out_stream.p_pack) {
					vs_sample_trace(" chn[%d] no memory for out_stream.p_pack len[%lu] error!!! \n",
                        p_acquire_stream_param->venc_chnid[0], sizeof(vs_venc_pack_s) * chn_status.cur_pack_num);
					p_acquire_stream_param->stop_stream_task = VS_TRUE;
					continue;
				}

				out_stream.pack_num = chn_status.cur_pack_num;
				ret = vs_mal_venc_stream_acquire(p_acquire_stream_param->venc_chnid[0], &out_stream, 2000);
				if (VS_SUCCESS != ret || 0 == out_stream.pack_num) {
					if(VS_NULL != out_stream.p_pack) {
                        free(out_stream.p_pack);
                        out_stream.p_pack = VS_NULL;
                    }
                    continue;
				}

				frame_end = VS_FALSE;
                if (p_acquire_stream_param->store_strm == VS_TRUE) {
                    if(VS_SUCCESS == venc_stream_output_open(p_acquire_stream_param, 0,
                                                             p_acquire_stream_param->received_frame_num[0])) {
                        if (VS_NULL != p_acquire_stream_param->p_out_file[0]) {
                            venc_stream_data_write(p_acquire_stream_param, &out_stream, &frame_end, 0);
                            fclose(p_acquire_stream_param->p_out_file[0]);
                            p_acquire_stream_param->p_out_file[0] = VS_NULL;
                        }
                    }
                }
                vs_mal_venc_stream_release(p_acquire_stream_param->venc_chnid[0], &out_stream);
				if(VS_NULL != out_stream.p_pack) {
                    free(out_stream.p_pack);
                    out_stream.p_pack = VS_NULL;
                }

				if (VS_TRUE == frame_end) {
					p_acquire_stream_param->received_frame_num[0]++;
				}

//				vs_sample_trace("chn[%d] rcv frame[%d] \n", venc_chnid, received_frame_num);
            }

        }
	};

    vs_sample_trace("chnid[%d] received_frame_num[%d] store_strm[%d]\n",
                    p_acquire_stream_param->venc_chnid[0], p_acquire_stream_param->received_frame_num[0], p_acquire_stream_param->store_strm);
    vs_mal_venc_chn_fd_close(p_acquire_stream_param->venc_chnid[0]);
    ret = vs_mal_venc_chn_stop(p_acquire_stream_param->venc_chnid[0]);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("chnid[%d] vs_mal_venc_chn_stop failed, ret[0x%x]\n", p_acquire_stream_param->venc_chnid[0], ret);
        return ret;
    }

    return VS_NULL;
}

static vs_void_t *venc_acquire_stream_task(vs_void_t *arg)
{
    vs_int32_t ret = VS_SUCCESS;
	vs_int32_t chn_index = 0;
	vs_int32_t chn_num_started = 0;
//	vs_int32_t stoped_chn = 0;
	vs_int32_t venc_fd[VENC_MAX_CHN_NUM], max_venc_fd = 0;
	vs_bool_t frame_end = VS_FALSE;
	struct timeval timeout_val;
	fd_set read_fds;
    //vs_int32_t received_frame_num[VENC_MAX_CHN_NUM] = {0};
	struct vs_venc_chn_status chn_status;
	vs_venc_stream_s out_stream;
	sample_venc_acquire_stream_param_s *p_task_param = (sample_venc_acquire_stream_param_s *)arg;

	for(chn_index = 0; chn_index < p_task_param->chn_num; chn_index++) {
		venc_fd[chn_index] = vs_mal_venc_chn_fd_get(p_task_param->venc_chnid[chn_index]);
		if (venc_fd[chn_index] <= 0) {
			vs_sample_trace(" chn[%d] vs_mal_venc_chn_fd_get get error\n", p_task_param->venc_chnid[chn_index]);
			return VS_NULL;
		}
		max_venc_fd = (max_venc_fd > venc_fd[chn_index]) ? max_venc_fd : venc_fd[chn_index];
		chn_num_started++;

        ret = vs_mal_venc_chn_attr_get(p_task_param->venc_chnid[chn_index], &p_task_param->chn_attr[chn_index]);
        if (ret != VS_SUCCESS) {
            vs_sample_trace("sample_common_vpp_bind_venc failed, ret[0x%x]\n", ret);
            return VS_NULL;
        }

        if (p_task_param->chn_attr[chn_index].enc_attr.type != E_PT_TYPE_JPEG) {
            if(VS_SUCCESS != venc_stream_output_open(p_task_param, chn_index,
                                                     p_task_param->received_frame_num[chn_index])) {
                vs_sample_trace(" chn[%d] file open failed, case can't go on.\n", p_task_param->venc_chnid[chn_index]);
                if(VS_NULL != out_stream.p_pack) {
                    free(out_stream.p_pack);
                    out_stream.p_pack = VS_NULL;
                }
                return VS_NULL;
            }
        }
	}
	vs_sample_trace("chn_num[%d] started! store_strm[%d]\n", chn_num_started, p_task_param->store_strm);

	while (VS_TRUE != p_task_param->stop_stream_task) {
		timeout_val.tv_sec = 1;
        timeout_val.tv_usec = 0;
        FD_ZERO(&read_fds);
        for(chn_index = 0; chn_index < chn_num_started; chn_index++) {
            FD_SET(venc_fd[chn_index], &read_fds);
        }
        ret = select(max_venc_fd + 1, &read_fds, NULL, NULL, &timeout_val);
        if (ret < 0) {
            vs_sample_trace(" get venc frame select error, ret[%d] break! \n", ret);
            break;
        } else if (0 == ret) {
            //vs_sample_trace(" get venc frame select time out, continue! \n");
            continue;
        }

		for (chn_index = 0; chn_index < chn_num_started; chn_index++) {
            if (FD_ISSET(venc_fd[chn_index], &read_fds)) {
				ret = vs_mal_venc_chn_status_get(p_task_param->venc_chnid[chn_index], &chn_status);
				if (VS_SUCCESS != ret || (chn_status.left_stream_frame_num < 1)) {
					continue;
				}

				memset(&out_stream, 0, sizeof(vs_venc_stream_s));
				out_stream.p_pack = (vs_venc_pack_s *)calloc(1, sizeof(vs_venc_pack_s) * chn_status.cur_pack_num);
				if (VS_NULL == out_stream.p_pack) {
					vs_sample_trace(" chn[%d] no memory for out_stream.p_pack len[%lu] error!!! \n",
                        p_task_param->venc_chnid[chn_index], sizeof(vs_venc_pack_s) * chn_status.cur_pack_num);
					p_task_param->stop_stream_task = VS_TRUE;
					continue;
				}

				out_stream.pack_num = chn_status.cur_pack_num;
				ret = vs_mal_venc_stream_acquire(p_task_param->venc_chnid[chn_index], &out_stream, 2000);
				if (VS_SUCCESS != ret || 0 == out_stream.pack_num) {
					if(VS_NULL != out_stream.p_pack) {
                        free(out_stream.p_pack);
                        out_stream.p_pack = VS_NULL;
                    }
                    continue;
				} else if (p_task_param->stream_acquire_func != VS_NULL) {
                    p_task_param->stream_acquire_func(p_task_param->venc_chnid[chn_index], p_task_param, &out_stream);
                }

				frame_end = VS_FALSE;
                if (p_task_param->store_strm == VS_TRUE) {
                    if (p_task_param->chn_attr[chn_index].enc_attr.type == E_PT_TYPE_JPEG) {
                        if(VS_SUCCESS == venc_stream_output_open(p_task_param, chn_index,
                                                                 p_task_param->received_frame_num[chn_index])) {
                            if (VS_NULL != p_task_param->p_out_file[chn_index]) {
                                venc_stream_data_write(p_task_param, &out_stream, &frame_end, chn_index);
                                fclose(p_task_param->p_out_file[chn_index]);
                                p_task_param->p_out_file[chn_index] = VS_NULL;
                            }
                        }
                    } else {
                        venc_stream_data_write(p_task_param, &out_stream, &frame_end, chn_index);
                    }
                }
                vs_mal_venc_stream_release(p_task_param->venc_chnid[chn_index], &out_stream);
				if(VS_NULL != out_stream.p_pack) {
                    free(out_stream.p_pack);
                    out_stream.p_pack = VS_NULL;
                }

				if (VS_TRUE == frame_end) {
					p_task_param->received_frame_num[chn_index]++;
				}

//				vs_sample_trace("chn[%d] rcv frame[%d] \n", venc_chnid, received_frame_num);
            }

        }
	};

    vs_sample_trace("Total[%d] channel(s) store_strm[%d] exit stream_get_task!\n", chn_num_started, p_task_param->store_strm);
    for (chn_index = 0; chn_index < chn_num_started; chn_index++) {
        vs_sample_trace("chnid[%d] received_frame_num[%d]!\n",
                        p_task_param->venc_chnid[chn_index], p_task_param->received_frame_num[chn_index]);
        vs_mal_venc_chn_fd_close(p_task_param->venc_chnid[chn_index]);
        if (VS_NULL != p_task_param->p_out_file[chn_index]) {
            fclose(p_task_param->p_out_file[chn_index]);
            p_task_param->p_out_file[chn_index] = VS_NULL;
        }
	}

    return VS_NULL;
}

vs_int32_t sample_common_venc_acquire_stream_start(sample_venc_acquire_stream_param_s *p_acquire_stream_param)
{
    vs_sample_trace("sample_common_venc_acquire_stream_start chn_num[%d]\n", p_acquire_stream_param->chn_num);
    return pthread_create(&p_acquire_stream_param->venc_pid, 0,
                           venc_acquire_stream_task, (vs_void_t*)p_acquire_stream_param);
}

vs_int32_t sample_common_venc_acquire_stream_stop(sample_venc_acquire_stream_param_s *p_acquire_stream_param)
{
    if (p_acquire_stream_param->stop_stream_task == VS_FALSE) {
        p_acquire_stream_param->stop_stream_task = VS_TRUE;
        vs_sample_trace("pthread_join venc_pid[%lu]! \n", p_acquire_stream_param->venc_pid);
        pthread_join(p_acquire_stream_param->venc_pid, 0);
    }
    vs_sample_trace("sample_common_venc_acquire_stream_stop!\n");
    return VS_SUCCESS;
}

vs_int32_t venc_frame_buffer_get(vs_video_frame_s *p_frame, vs_uint32_t alignment,
                                            vs_bool_t compress, VB_POOL pool)
{
    vs_pixel_format_e format = p_frame->pixel_format;
    vs_uint32_t width = p_frame->width;
    vs_uint32_t height = p_frame->height;
    vs_uint32_t luma_stride = 0, chroma_stride = 0;
    vs_uint32_t luma_size = 0, chroma_size = 0, align_size = 0;
    vs_uint32_t compress_luma_tablesize = 0, compress_chr_tablesize = 0, compress_frame_tablesize = 0;
    vs_uint32_t blk_size = 0;
    VB_BLK blk = VS_INVALID_VB_HANDLE;

    venc_get_aligned_picsize_by_format(format, width, height, alignment, &luma_size, &chroma_size, &align_size);
    align_size = STRIDE_UP(align_size, 4096);
    blk_size = align_size;
    if (VS_TRUE == compress) {
        venc_get_compress_table_by_format(format, width, height, alignment,
                                                &compress_luma_tablesize, &compress_chr_tablesize, &compress_frame_tablesize);
        compress_frame_tablesize = STRIDE_UP(compress_frame_tablesize, 4096);
        blk_size += compress_frame_tablesize;
    }

    if (VS_INVALID_VB_HANDLE == (blk = vs_mal_vb_block_get(pool, blk_size, VS_NULL))) {
        vs_sample_trace("get size[%u] of block failed.\n", blk_size);
        return VS_FAILED;
    }

    if (0 == (p_frame->phys_addr[0] = vs_mal_vb_handle2physaddr(blk))) {
        vs_sample_trace("get phys of block[%u] failed.\n", blk);
        vs_mal_vb_block_release(blk);
        blk = VS_INVALID_VB_HANDLE;
        return VS_FAILED;
    }
//    vs_sample_trace("get phys[%llu][%llx] of block[%u] ok.\n", p_frame->phys_addr[0], p_frame->phys_addr[0], blk);
    if (0 == (p_frame->virt_addr[0] = (vs_uint64_t)vs_mal_sys_mmap_cached(p_frame->phys_addr[0], blk_size))) {
        vs_sample_trace("get map phys[0x%llx] to virt failed.\n", p_frame->phys_addr[0]);
        vs_mal_vb_block_release(blk);
        blk = VS_INVALID_VB_HANDLE;
        return VS_FAILED;
    }
    venc_get_aligned_byte_stride(width, format, &luma_stride, &chroma_stride, alignment);
    p_frame->stride[0] = luma_stride;
    p_frame->stride[1] = 0;
    p_frame->stride[2] = 0;
    p_frame->virt_addr[1] = 0;
    p_frame->virt_addr[2] = 0;
    p_frame->phys_addr[1] = 0;
    p_frame->phys_addr[2] = 0;
    p_frame->compress_header_virt_addr[0] = 0;
    p_frame->compress_header_virt_addr[1] = 0;
    p_frame->compress_header_virt_addr[2] = 0;
    p_frame->compress_header_phys_addr[0] = 0;
    p_frame->compress_header_phys_addr[1] = 0;
    p_frame->compress_header_phys_addr[2] = 0;

    if (E_PIXEL_FORMAT_YUV_400 != format) {
        p_frame->virt_addr[1] = p_frame->virt_addr[0] + luma_stride * height;
        p_frame->virt_addr[2] = p_frame->virt_addr[0] + luma_stride * height;
        p_frame->phys_addr[1] = p_frame->phys_addr[0] + luma_stride * height;
        p_frame->phys_addr[2] = p_frame->phys_addr[0] + luma_stride * height;
        p_frame->stride[1] = chroma_stride;
        p_frame->stride[2] = chroma_stride;
    }

    if (VS_TRUE == compress) {//only support nv21/nv12
        p_frame->compress_header_virt_addr[0] = p_frame->virt_addr[0] + align_size;
        p_frame->compress_header_virt_addr[1] = p_frame->compress_header_virt_addr[0] + compress_luma_tablesize;
        p_frame->compress_header_virt_addr[2] = p_frame->compress_header_virt_addr[1];
        p_frame->compress_header_phys_addr[0] = p_frame->phys_addr[0] + align_size;
        p_frame->compress_header_phys_addr[1] = p_frame->compress_header_phys_addr[0] + +compress_luma_tablesize;
        p_frame->compress_header_phys_addr[2] = p_frame->compress_header_phys_addr[1];
    }

    return VS_SUCCESS;
}

static vs_int32_t venc_frame_buffer_release(vs_video_frame_s *p_frame, vs_uint32_t alignment, vs_bool_t compress)
{
	vs_pixel_format_e format = p_frame->pixel_format;
	vs_uint32_t width = p_frame->width;
	vs_uint32_t height = p_frame->height;
	vs_uint32_t luma_size = 0, chroma_size = 0, align_size = 0;
	vs_uint32_t compress_luma_tablesize = 0, compress_chr_tablesize = 0, compress_frame_tablesize = 0;
	vs_uint32_t blk_size = 0;
	VB_BLK blk = VS_INVALID_VB_HANDLE;

	venc_get_aligned_picsize_by_format(format, width, height, alignment, &luma_size, &chroma_size, &align_size);
	align_size = STRIDE_UP(align_size, 4096);
	blk_size = align_size;
	if (VS_TRUE == compress) {
		venc_get_compress_table_by_format(format, width, height, alignment,
												&compress_luma_tablesize, &compress_chr_tablesize, &compress_frame_tablesize);
		compress_frame_tablesize = STRIDE_UP(compress_frame_tablesize, 4096);
		blk_size += compress_frame_tablesize;
	}

	vs_mal_sys_unmap((vs_void_t*)p_frame->virt_addr[0], blk_size);
	blk = vs_mal_vb_physaddr2handle(p_frame->phys_addr[0]);
	if (VS_INVALID_VB_HANDLE != blk) {
		vs_mal_vb_block_release(blk);
	} else {
		vs_sample_trace("can't get block handle for phys[0x%llx] failed.\n", p_frame->phys_addr[0]);
		return VS_FAILED;
	}

	return VS_SUCCESS;
}

static vs_uint32_t venc_source_frame_size_get(vs_pixel_format_e type, vs_uint32_t width, vs_uint32_t height)
{
	vs_uint32_t luma_size = 0, chroma_size = 0, frame_size = 0;

	switch(type) {
    case E_PIXEL_FORMAT_YVU_420SP:
    case E_PIXEL_FORMAT_YUV_420SP:
		luma_size = width * height;
		chroma_size = width * height/2;
		break;
    default:
		vs_sample_trace("not support this format\n");
		chroma_size = luma_size = 0;
		break;
	}

	frame_size = luma_size + chroma_size;
//	vs_sample_trace("luma_size[%u] chroma_size[%u] frame_size[%u]\n", luma_size, chroma_size, frame_size);

	return frame_size;
}

vs_int32_t venc_frame_input_open(sample_venc_send_frame_param_s *p_send_frame_param, vs_uint32_t *p_frame_cnt)
{
	vs_uint32_t frame_size = 0;
	vs_uint32_t frame_cnt = 0xFFFFFFFF;
    vs_uint64_t file_length = 0;

	frame_size = venc_source_frame_size_get(p_send_frame_param->pixel_format,
										p_send_frame_param->width,
										p_send_frame_param->height);
	p_send_frame_param->p_input_yuv = VS_NULL;
    p_send_frame_param->p_input_yuv = fopen((char *)p_send_frame_param->input_yuv_path, "rb");
    if (VS_NULL == p_send_frame_param->p_input_yuv) {
		vs_sample_trace("chn[%d] fopen [%s] failed\n",
                p_send_frame_param->chnid, p_send_frame_param->input_yuv_path);
		return VS_FAILED;
    }
	vs_sample_trace("chn[%d] open input_file[%s] OK \n",
		                    p_send_frame_param->chnid, p_send_frame_param->input_yuv_path);
	if (strlen(p_send_frame_param->input_table_path) > 0) {
		p_send_frame_param->p_input_table = fopen((char *)p_send_frame_param->input_table_path, "rb");
		if (VS_NULL == p_send_frame_param->p_input_table) {
			vs_sample_trace("chn[%d] fopen compressfile [%s] failed\n",
                    p_send_frame_param->chnid, p_send_frame_param->input_table_path);
			if (VS_NULL != p_send_frame_param->p_input_yuv) {
				fclose(p_send_frame_param->p_input_yuv);
				p_send_frame_param->p_input_yuv = VS_NULL;
			}
			return VS_FAILED;
		}
		vs_sample_trace("chn[%d] open input_table_path[%s] OK \n",
		                    p_send_frame_param->chnid, p_send_frame_param->input_table_path);
	}

    fseeko(p_send_frame_param->p_input_yuv, 0, SEEK_END);
	file_length = ftello(p_send_frame_param->p_input_yuv);
	fseeko(p_send_frame_param->p_input_yuv, 0, SEEK_SET);
	frame_cnt = file_length / frame_size;
	vs_sample_trace("chn[%d] input file has [%d] frames.\n", p_send_frame_param->chnid, frame_cnt);

	*p_frame_cnt = frame_cnt;

	return VS_SUCCESS;
}

vs_int32_t venc_file_read(FILE *file, vs_uint8_t *data, vs_uint64_t seek, size_t size)
{
	size_t read_size = 0;

	if ((file == VS_NULL) || (data == VS_NULL)) {
        vs_sample_trace("venc_file_read file=%p, data=%p!\n", file, data );
        return VS_FAILED;
    }

	if (0 != fseeko(file, seek, SEEK_SET)) {
		vs_sample_trace("read[seek] file end \n");
		return VS_FAILED;
	}

	read_size = fread(data, sizeof(vs_uint8_t), size, file);
	if (read_size < size) {
		if (feof(file)) {
			vs_sample_trace("read[read] file end \n");
			return VS_FAILED;
		}
		vs_sample_trace("plan read size=%d, real read_size=%d, seek=%llu \n", (int)size, (int)read_size, seek);
		return VS_FAILED;
	}

	return VS_SUCCESS;
}

vs_int32_t venc_read_frame(FILE *in, vs_video_frame_s *p_in_frame, vs_int32_t index, vs_uint32_t input_format,
                vs_uint32_t alignment, vs_uint32_t src_img_size, vs_uint32_t src_width, vs_uint32_t src_height)
{
	vs_int32_t ret;
	vs_int32_t num;
	vs_uint64_t seek;
	vs_uint8_t *lum;
	vs_uint8_t *cb;
	vs_uint32_t i;
	vs_uint32_t luma_stride;
	vs_uint32_t chroma_stride;
	//vs_uint32_t alignment = 16;
	vs_video_frame_s *p_frame = p_in_frame;

	num = index;
	seek = ((vs_uint64_t)num) * ((vs_uint64_t)src_img_size);
	lum = (vs_uint8_t *)p_frame->virt_addr[0];
	cb = (vs_uint8_t *)p_frame->virt_addr[1];
//	vs_sample_trace("read_frame[seek] file seek[%llu] \n", seek);

	venc_get_aligned_byte_stride(src_width, input_format, &luma_stride, &chroma_stride, alignment);

	switch(input_format) {
    case E_PIXEL_FORMAT_YVU_420SP:
    case E_PIXEL_FORMAT_YUV_420SP:
    	/* Lum */
    	for (i = 0; i < src_height; i++) {
    		ret = venc_file_read(in, lum , seek, src_width);
    		if (VS_SUCCESS != ret)
    			return ret;
    		seek += src_width;
    		lum += luma_stride;
    	}
    	/* CbCr */
    	for (i = 0; i < (src_height / 2); i++) {
    		ret = venc_file_read(in, cb, seek, src_width);
    		if (VS_SUCCESS != ret)
    			return ret;
    		seek += src_width;
    		cb += chroma_stride;
    	}
	    break;
	default:
		break;
  }

  return VS_SUCCESS;
}

vs_int32_t venc_read_compress_head(FILE *in, vs_video_frame_s *p_in_frame, vs_int32_t index, vs_uint32_t input_format,
            vs_uint32_t luma_tablesize, vs_uint32_t chr_tablesize, vs_uint32_t frame_tablesize)
{
	vs_int32_t ret = 0;
	vs_int32_t num;
	vs_uint64_t seek;
	vs_uint8_t *lum;
	vs_uint8_t *cb;
	vs_uint32_t luma_size;
	vs_uint32_t chr_size;
	vs_uint32_t src_img_size;

	num = index;

	/*hw compress table*/
	luma_size = luma_tablesize;
	chr_size = chr_tablesize;
	src_img_size = frame_tablesize;

	seek = ((vs_uint64_t)num) * ((vs_uint64_t)src_img_size);
	lum = (vs_uint8_t *)p_in_frame->compress_header_virt_addr[0];
	cb = (vs_uint8_t *)p_in_frame->compress_header_virt_addr[1];

	switch (input_format) {
	case E_PIXEL_FORMAT_YUV_420SP:
	case E_PIXEL_FORMAT_YVU_420SP:
		ret = venc_file_read(in, lum, seek, luma_size);
		if (VS_SUCCESS != ret) {
			return ret;
        }
		seek += luma_size;
		ret = venc_file_read(in, cb, seek, chr_size);
		if (VS_SUCCESS != ret) {
			return ret;
        }
		break;
	default:
		vs_sample_trace("[TEST_CASE]Compress HW not support this format\n");
		return ret;
	}
	return VS_SUCCESS;
}

vs_int32_t venc_frame_buffer_read(vs_video_frame_s *p_frame, vs_uint32_t index, vs_uint32_t alignment,
									FILE *p_file, FILE *p_compress)
{
	vs_uint32_t frame_size = 0;
	vs_int32_t  ret = VS_SUCCESS;
    vs_pixel_format_e format = p_frame->pixel_format;
    vs_uint32_t width = p_frame->width;
    vs_uint32_t height = p_frame->height;
    vs_uint32_t luma_size = 0, chroma_size = 0, align_size = 0;
    vs_uint32_t blk_size = 0;

	frame_size = venc_source_frame_size_get(p_frame->pixel_format, p_frame->width, p_frame->height);

	ret = venc_read_frame(p_file, p_frame, index, p_frame->pixel_format, alignment,
						frame_size, p_frame->width, p_frame->height);
	if (VS_SUCCESS != ret) {
		return ret;
	}

	if (VS_NULL != p_compress) {
		vs_uint32_t compress_luma_tablesize = 0, compress_chr_tablesize = 0, compress_frame_tablesize = 0;

		venc_get_compress_table_by_format(p_frame->pixel_format, p_frame->width, p_frame->height, alignment,
											&compress_luma_tablesize, &compress_chr_tablesize, &compress_frame_tablesize);
		ret = venc_read_compress_head(p_compress, p_frame, index, p_frame->pixel_format, compress_luma_tablesize,
                            compress_chr_tablesize, compress_frame_tablesize);
	}

    venc_get_aligned_picsize_by_format(format, width, height, alignment, &luma_size, &chroma_size, &align_size);
    align_size = STRIDE_UP(align_size, 4096);
    blk_size = align_size;
    if (p_frame->virt_addr[0] != 0) {
        vs_mal_sys_cache_flush(p_frame->phys_addr[0], (vs_uint64_t* )p_frame->virt_addr[0], blk_size);
    }
	return ret;
}

static vs_void_t *venc_frame_send_task(vs_void_t *arg)
{
    vs_int32_t ret = VS_SUCCESS;
    vs_uint32_t frame_index = 0;
    vs_uint32_t frame_cnt = 0xFFFFFFFF;
    sample_venc_send_frame_param_s *p_send_frame_param = (sample_venc_send_frame_param_s *)arg;
    vs_pixel_format_e format = p_send_frame_param->pixel_format;
    vs_uint32_t alignment = p_send_frame_param->input_alignment;
    vs_uint32_t width = p_send_frame_param->width;
    vs_uint32_t height = p_send_frame_param->height;
    vs_video_frame_info_s frame_info;
    vs_bool_t move_forward = VS_TRUE;

    if (VS_SUCCESS != venc_frame_input_open(p_send_frame_param, &frame_cnt)) {
        vs_sample_trace("chn[%d] input file open failed, please check config file.\n",
                                        p_send_frame_param->chnid);
        p_send_frame_param->stop_send_task = VS_TRUE;
        return VS_NULL;
    }

    memset(&frame_info,0,sizeof(vs_video_frame_info_s));
    frame_info.frame.id = 0;
    frame_info.frame.pts = 0;
    frame_info.frame.width = width;
    frame_info.frame.height = height;
    frame_info.frame.pixel_format = format;
    frame_info.poolid = p_send_frame_param->vb_poolid;
    frame_info.modid = E_MOD_ID_USER;

    while(VS_FALSE == p_send_frame_param->stop_send_task) {
        do {
            ret = venc_frame_buffer_get(&(frame_info.frame), alignment, p_send_frame_param->compress,
                                            p_send_frame_param->vb_poolid);
            if (VS_SUCCESS != ret) {
                vs_sample_trace(" chn[%d] cant't get buffer for frame[%d], retry later.\n",
                                    p_send_frame_param->chnid, frame_index);
                usleep(100 * 1000);
            }
        } while (VS_SUCCESS != ret && VS_FALSE == p_send_frame_param->stop_send_task);
        if(VS_TRUE == p_send_frame_param->stop_send_task) {
            vs_sample_trace("send_task have be stop by other\n");
            if(VS_SUCCESS == ret)
                ret = venc_frame_buffer_release(&(frame_info.frame), alignment, p_send_frame_param->compress);
            break;
        }
        ret = venc_frame_buffer_read(&(frame_info.frame), frame_index, alignment,
                                     p_send_frame_param->p_input_yuv, p_send_frame_param->p_input_table);

        if (VS_SUCCESS != ret) {
            vs_sample_trace(" chn[%d] read frame[%d], no more data can be read out form file[%s], so break!\n",
                                p_send_frame_param->chnid, frame_index, p_send_frame_param->input_yuv_path);
            ret = venc_frame_buffer_release(&(frame_info.frame), alignment, p_send_frame_param->compress);
            p_send_frame_param->stop_send_task = VS_TRUE;
            break;
        }
        if (p_send_frame_param->frame_send_func != VS_NULL) {
            p_send_frame_param->frame_send_func(p_send_frame_param, &frame_info);
        } else {
            vs_sample_trace(" chn[%d] frame_send_func should not be NULL!\n", p_send_frame_param->chnid);
        }
        ret = venc_frame_buffer_release(&(frame_info.frame), alignment, p_send_frame_param->compress);
        frame_info.frame.id += 1;
        frame_info.frame.pts += 333333;

        /*circle send process*/
        if (p_send_frame_param->circle_send == VS_TRUE) {
            if ((frame_index == (frame_cnt - 1)) && (VS_TRUE == move_forward)) {
        		move_forward = VS_FALSE;
        	} else if ((frame_index == 0) && (VS_FALSE == move_forward)) {
        		move_forward = VS_TRUE;
        	}
        }
        if (VS_TRUE == move_forward) {
    		frame_index = frame_index + 1;
    	} else {
    		frame_index = frame_index - 1;
    	}

        usleep(20*1000);
    }

    if (VS_NULL != p_send_frame_param->p_input_yuv) {
		fclose(p_send_frame_param->p_input_yuv);
		p_send_frame_param->p_input_yuv = VS_NULL;
	}

	if (VS_NULL != p_send_frame_param->p_input_table) {
		fclose(p_send_frame_param->p_input_table);
		p_send_frame_param->p_input_table = VS_NULL;
	}
    vs_sample_trace("chn[%d] venc_frame_send_task exited!\n", p_send_frame_param->chnid);
    return VS_NULL;
}

vs_int32_t sample_common_venc_send_frame_start(sample_venc_send_frame_param_s *p_send_frame_param)
{
    vs_int32_t ret;

    p_send_frame_param->stop_send_task = VS_FALSE;
    ret = pthread_create(&p_send_frame_param->thread_pid, 0, venc_frame_send_task, (vs_void_t*)p_send_frame_param);
    vs_sample_trace("chnid[%d] thread_pid[%lu].\n", p_send_frame_param->chnid, p_send_frame_param->thread_pid);
    return ret;
}

vs_int32_t sample_common_venc_send_frame_stop(sample_venc_send_frame_param_s *p_send_frame_param)
{
    if (p_send_frame_param->stop_send_task == VS_FALSE) {
        p_send_frame_param->stop_send_task = VS_TRUE;
        vs_sample_trace("pthread_join thread_pid[%lu]! \n", p_send_frame_param->thread_pid);
        pthread_join(p_send_frame_param->thread_pid, 0);
    }
    vs_sample_trace("venc_frame_send_task stop!\n");
    return VS_SUCCESS;
}

static vs_void_t sample_common_venc_frame_info_print(vs_video_frame_s *p_frame)
{
    vs_sample_trace("[video_frame] id[%u].\n", p_frame->id);
    vs_sample_trace("[video_frame] pts[%llu].\n", p_frame->pts);
    vs_sample_trace("[video_frame] width[%u].\n", p_frame->width);
    vs_sample_trace("[video_frame] height[%u].\n", p_frame->height);
    vs_sample_trace("[video_frame] pixel_format[%d].\n", p_frame->pixel_format);
    vs_sample_trace("[video_frame] phys_addr[0][0x%llx].\n", p_frame->phys_addr[0]);
    vs_sample_trace("[video_frame] phys_addr[1][0x%llx].\n", p_frame->phys_addr[1]);
    vs_sample_trace("[video_frame] phys_addr[2][0x%llx].\n", p_frame->phys_addr[2]);
    vs_sample_trace("[video_frame] virt_addr[0][0x%llx].\n", p_frame->virt_addr[0]);
    vs_sample_trace("[video_frame] virt_addr[1][0x%llx].\n", p_frame->virt_addr[1]);
    vs_sample_trace("[video_frame] virt_addr[2][0x%llx].\n", p_frame->virt_addr[2]);
    vs_sample_trace("[video_frame] stride[0][%u].\n", p_frame->stride[0]);
    vs_sample_trace("[video_frame] stride[1][%u].\n", p_frame->stride[1]);
    vs_sample_trace("[video_frame] stride[2][%u].\n", p_frame->stride[2]);
    vs_sample_trace("[video_frame] compress_header_phys_addr[0][0x%llx].\n", p_frame->compress_header_phys_addr[0]);
    vs_sample_trace("[video_frame] compress_header_phys_addr[1][0x%llx].\n", p_frame->compress_header_phys_addr[1]);
    vs_sample_trace("[video_frame] compress_header_phys_addr[2][0x%llx].\n", p_frame->compress_header_phys_addr[2]);
    vs_sample_trace("[video_frame] compress_header_virt_addr[0][0x%llx].\n", p_frame->compress_header_virt_addr[0]);
    vs_sample_trace("[video_frame] compress_header_virt_addr[1][0x%llx].\n", p_frame->compress_header_virt_addr[1]);
    vs_sample_trace("[video_frame] compress_header_virt_addr[2][0x%llx].\n", p_frame->compress_header_virt_addr[2]);
    vs_sample_trace("[video_frame] compress_header_stride[0][%u].\n", p_frame->compress_header_stride[0]);
    vs_sample_trace("[video_frame] compress_header_stride[1][%u].\n", p_frame->compress_header_stride[1]);
    vs_sample_trace("[video_frame] compress_header_stride[2][%u].\n", p_frame->compress_header_stride[2]);
}

static vs_uint8_t venc_create_qpmap_value(vs_int32_t lcu_line, vs_bool_t is_absqp , vs_bool_t is_skip)
{
	vs_uint8_t qpmap_value = 0;
	if(lcu_line % 4 == 0) {
		if(VS_TRUE == is_skip) {
			qpmap_value |= QPMAP_SKIP_FLAG_SET;
		} else {
			qpmap_value |= QPMAP_SKIP_FLAG_CLR;
		}
		if(VS_TRUE == is_absqp) {
			qpmap_value |= QPMAP_ABSQP_FLAG_CLR;
			qpmap_value |= 0x28; // 40
		} else {
			qpmap_value |= QPMAP_ABSQP_FLAG_SET;
			qpmap_value |= 0x40 - 3;// absqp +3   should >= 0x20
		}
	} else if (lcu_line % 4 == 1) {
		if(VS_TRUE == is_skip) {
			qpmap_value |= QPMAP_SKIP_FLAG_SET;
		} else {
			qpmap_value |= QPMAP_SKIP_FLAG_CLR;
		}
		if(VS_TRUE == is_absqp) {
			qpmap_value |= QPMAP_ABSQP_FLAG_CLR;
			qpmap_value |= 0x26; // 38
		} else {
			qpmap_value |= QPMAP_ABSQP_FLAG_SET;
			qpmap_value |= 0x40 - 1;// absqp +1   should >= 0x20
		}
	} else if (lcu_line % 4 == 2) {
		if(VS_TRUE == is_skip) {
			qpmap_value |= QPMAP_SKIP_FLAG_SET;
		} else {
			qpmap_value |= QPMAP_SKIP_FLAG_CLR;
		}
		if(VS_TRUE == is_absqp) {
			qpmap_value |= QPMAP_ABSQP_FLAG_CLR;
			qpmap_value |= 0x24; // 36
		} else {
			qpmap_value |= QPMAP_ABSQP_FLAG_SET;
			qpmap_value |= 0x00 + 1;// absqp -1   should < 0x20
		}
	} else {
		if(VS_TRUE == is_skip) {
			qpmap_value |= QPMAP_SKIP_FLAG_SET;
		} else {
			qpmap_value |= QPMAP_SKIP_FLAG_CLR;
		}
		if(VS_TRUE == is_absqp) {
			qpmap_value |= QPMAP_ABSQP_FLAG_CLR;
			qpmap_value |= 0x22; // 34
		} else {
			qpmap_value |= QPMAP_ABSQP_FLAG_SET;
			qpmap_value |= 0x00 + 3;// absqp -3   should < 0x20
		}
	}
	return qpmap_value;
}
static vs_int32_t venc_qpmap_data_default_get(vs_payload_type_e enc_type,
                                                            vs_venc_qpmap_frame_info_s *p_frame, vs_uint8_t *p_data,
                                                            vs_venc_map_blk_unit_e blk_unit, vs_uint32_t send_frame)
{
    vs_int32_t ret = 0;
	vs_uint32_t lcu_line_index = 0, index = 0;
    vs_uint8_t *p_buf = VS_NULL;
	vs_bool_t is_absqp = VS_FALSE, is_skip = VS_FALSE;
	vs_int32_t lcu_size = VS_VENC_LCU_SIZE_H264, qpmap_table_offset = 0;
	vs_uint32_t lcu_stride = 0, lcu_height = 0;
	vs_uint8_t lcu_qpmap_value = 0;
	if (E_PT_TYPE_H264 == enc_type) {
		lcu_size = VS_VENC_LCU_SIZE_H264;
	} else if (E_PT_TYPE_H265 == enc_type) {
		lcu_size = VS_VENC_LCU_SIZE_H265;
	}
	lcu_stride =  STRIDE_UP(p_frame->user_frame_info.frame.width, lcu_size) / lcu_size;
	lcu_height =  STRIDE_UP(p_frame->user_frame_info.frame.height, lcu_size) / lcu_size;

    p_buf = p_data;

	if(send_frame % 2) {
		is_absqp = VS_FALSE;
	} else {
		is_absqp = VS_TRUE;
	}

	qpmap_table_offset = 0;
	for(lcu_line_index = 0;lcu_line_index < lcu_height;lcu_line_index++) {  // lcu line num
		if((send_frame % 30 != 0) \
		&& (lcu_line_index == (send_frame % 4 + 1))) {
			is_skip = VS_TRUE;
		} else {
			is_skip = VS_FALSE;
		}
		for(index = 0;index < lcu_stride;index++) {   // one lcu line
			if(E_PT_TYPE_H264 == enc_type) {
				lcu_qpmap_value = venc_create_qpmap_value(lcu_line_index, is_absqp, is_skip);// 1 lcu
				memset(p_buf + qpmap_table_offset, lcu_qpmap_value, 4);
				qpmap_table_offset += 4;
			} else {
				switch(blk_unit) {
				case E_VENC_MAP_BLK_UNIT_32X32:
					lcu_qpmap_value = venc_create_qpmap_value(lcu_line_index, is_absqp, is_skip);// 1/4 lcu
					memset(p_buf + qpmap_table_offset, lcu_qpmap_value, 4);
					memset(p_buf + qpmap_table_offset + 8, lcu_qpmap_value, 4);
					memset(p_buf + qpmap_table_offset + 16, lcu_qpmap_value, 4);
					memset(p_buf + qpmap_table_offset + 24, lcu_qpmap_value, 4);
					qpmap_table_offset += 4;
					lcu_qpmap_value = venc_create_qpmap_value(lcu_line_index, is_absqp, is_skip);// 1/4 lcu
					memset(p_buf + qpmap_table_offset, lcu_qpmap_value, 4);
					memset(p_buf + qpmap_table_offset + 8, lcu_qpmap_value, 4);
					memset(p_buf + qpmap_table_offset + 16, lcu_qpmap_value, 4);
					memset(p_buf + qpmap_table_offset + 24, lcu_qpmap_value, 4);
					qpmap_table_offset += 28;
					lcu_qpmap_value = venc_create_qpmap_value(lcu_line_index, is_absqp, is_skip);// 1/4 lcu
					memset(p_buf + qpmap_table_offset, lcu_qpmap_value, 4);
					memset(p_buf + qpmap_table_offset + 8, lcu_qpmap_value, 4);
					memset(p_buf + qpmap_table_offset + 16, lcu_qpmap_value, 4);
					memset(p_buf + qpmap_table_offset + 24, lcu_qpmap_value, 4);
					qpmap_table_offset += 4;
					lcu_qpmap_value = venc_create_qpmap_value(lcu_line_index, is_absqp, is_skip);// 1/4 lcu
					memset(p_buf + qpmap_table_offset, lcu_qpmap_value, 4);
					memset(p_buf + qpmap_table_offset + 8, lcu_qpmap_value, 4);
					memset(p_buf + qpmap_table_offset + 16, lcu_qpmap_value, 4);
					memset(p_buf + qpmap_table_offset + 24, lcu_qpmap_value, 4);
					qpmap_table_offset += 28;
					break;
				case E_VENC_MAP_BLK_UNIT_16X16:
					lcu_qpmap_value = venc_create_qpmap_value(lcu_line_index, is_absqp, is_skip);// 1/16 lcu
					memset(p_buf + qpmap_table_offset, lcu_qpmap_value, 2);
					memset(p_buf + qpmap_table_offset + 8, lcu_qpmap_value, 2);
					qpmap_table_offset += 2;
					lcu_qpmap_value = venc_create_qpmap_value(lcu_line_index, is_absqp, is_skip);// 1/16 lcu
					memset(p_buf + qpmap_table_offset, lcu_qpmap_value, 2);
					memset(p_buf + qpmap_table_offset + 8, lcu_qpmap_value, 2);
					qpmap_table_offset += 2;
					lcu_qpmap_value = venc_create_qpmap_value(lcu_line_index, is_absqp, is_skip);// 1/16 lcu
					memset(p_buf + qpmap_table_offset, lcu_qpmap_value, 2);
					memset(p_buf + qpmap_table_offset + 8, lcu_qpmap_value, 2);
					qpmap_table_offset += 2;
					lcu_qpmap_value = venc_create_qpmap_value(lcu_line_index, is_absqp, is_skip);// 1/16 lcu
					memset(p_buf + qpmap_table_offset, lcu_qpmap_value, 2);
					memset(p_buf + qpmap_table_offset + 8, lcu_qpmap_value, 2);
					qpmap_table_offset += 10;
					lcu_qpmap_value = venc_create_qpmap_value(lcu_line_index, is_absqp, is_skip);// 1/16 lcu
					memset(p_buf + qpmap_table_offset, lcu_qpmap_value, 2);
					memset(p_buf + qpmap_table_offset + 8, lcu_qpmap_value, 2);
					qpmap_table_offset += 2;
					lcu_qpmap_value = venc_create_qpmap_value(lcu_line_index, is_absqp, is_skip);// 1/16 lcu
					memset(p_buf + qpmap_table_offset, lcu_qpmap_value, 2);
					memset(p_buf + qpmap_table_offset + 8, lcu_qpmap_value, 2);
					qpmap_table_offset += 2;
					lcu_qpmap_value = venc_create_qpmap_value(lcu_line_index, is_absqp, is_skip);// 1/16 lcu
					memset(p_buf + qpmap_table_offset, lcu_qpmap_value, 2);
					memset(p_buf + qpmap_table_offset + 8, lcu_qpmap_value, 2);
					qpmap_table_offset += 2;
					lcu_qpmap_value = venc_create_qpmap_value(lcu_line_index, is_absqp, is_skip);// 1/16 lcu
					memset(p_buf + qpmap_table_offset, lcu_qpmap_value, 2);
					memset(p_buf + qpmap_table_offset + 8, lcu_qpmap_value, 2);
					qpmap_table_offset += 10;
					lcu_qpmap_value = venc_create_qpmap_value(lcu_line_index, is_absqp, is_skip);// 1/16 lcu
					memset(p_buf + qpmap_table_offset, lcu_qpmap_value, 2);
					memset(p_buf + qpmap_table_offset + 8, lcu_qpmap_value, 2);
					qpmap_table_offset += 2;
					lcu_qpmap_value = venc_create_qpmap_value(lcu_line_index, is_absqp, is_skip);// 1/16 lcu
					memset(p_buf + qpmap_table_offset, lcu_qpmap_value, 2);
					memset(p_buf + qpmap_table_offset + 8, lcu_qpmap_value, 2);
					qpmap_table_offset += 2;
					lcu_qpmap_value = venc_create_qpmap_value(lcu_line_index, is_absqp, is_skip);// 1/16 lcu
					memset(p_buf + qpmap_table_offset, lcu_qpmap_value, 2);
					memset(p_buf + qpmap_table_offset + 8, lcu_qpmap_value, 2);
					qpmap_table_offset += 2;
					lcu_qpmap_value = venc_create_qpmap_value(lcu_line_index, is_absqp, is_skip);// 1/16 lcu
					memset(p_buf + qpmap_table_offset, lcu_qpmap_value, 2);
					memset(p_buf + qpmap_table_offset + 8, lcu_qpmap_value, 2);
					qpmap_table_offset += 10;
					lcu_qpmap_value = venc_create_qpmap_value(lcu_line_index, is_absqp, is_skip);// 1/16 lcu
					memset(p_buf + qpmap_table_offset, lcu_qpmap_value, 2);
					memset(p_buf + qpmap_table_offset + 8, lcu_qpmap_value, 2);
					qpmap_table_offset += 2;
					lcu_qpmap_value = venc_create_qpmap_value(lcu_line_index, is_absqp, is_skip);// 1/16 lcu
					memset(p_buf + qpmap_table_offset, lcu_qpmap_value, 2);
					memset(p_buf + qpmap_table_offset + 8, lcu_qpmap_value, 2);
					qpmap_table_offset += 2;
					lcu_qpmap_value = venc_create_qpmap_value(lcu_line_index, is_absqp, is_skip);// 1/16 lcu
					memset(p_buf + qpmap_table_offset, lcu_qpmap_value, 2);
					memset(p_buf + qpmap_table_offset + 8, lcu_qpmap_value, 2);
					qpmap_table_offset += 2;
					lcu_qpmap_value = venc_create_qpmap_value(lcu_line_index, is_absqp, is_skip);// 1/16 lcu
					memset(p_buf + qpmap_table_offset, lcu_qpmap_value, 2);
					memset(p_buf + qpmap_table_offset + 8, lcu_qpmap_value, 2);
					qpmap_table_offset += 10;
					break;
				case E_VENC_MAP_BLK_UNIT_64X64:
					lcu_qpmap_value = venc_create_qpmap_value(lcu_line_index, is_absqp, is_skip);// one lcu
					memset(p_buf + qpmap_table_offset, lcu_qpmap_value, 64);
					qpmap_table_offset += 64;
					break;
				default:
    				vs_sample_trace("blk_unit %d error.\n", blk_unit);
    				break;
				}
			}
		}
	}

    return ret;
}
static vs_int32_t sample_common_qpmap_alloc(sample_venc_send_qpmap_frame_param_s *p_send_qpmap_param,
    sample_common_venc_qpmap_addr_info *qpmap_addr_info) {
    vs_uint32_t buf_index;
    vs_int32_t chn_index, ret;
    vs_void_t *qpmap_virt_addr = NULL;
    vs_uint64_t qpmap_phys_addr = 0;

    for (chn_index = 0; chn_index < p_send_qpmap_param->venc_chnnum; chn_index++) {
        qpmap_addr_info->qpmap_table_size[chn_index] = venc_qpmap_buffer_size_get(p_send_qpmap_param->width, p_send_qpmap_param->height);
        ret = vs_mal_mmz_alloc(NULL, (vs_char_t *)"venc_qpmap", qpmap_addr_info->qpmap_table_size[chn_index] * QPMAP_FRAME_BUF_NUM, &qpmap_phys_addr);
        if (VS_SUCCESS != ret) {
            vs_sample_trace("vpp_chnid[%d] no memory for qpmap block \n", p_send_qpmap_param->vpp_chnid);
            return VS_FAILED;
        }
        qpmap_virt_addr = vs_mal_sys_mmap_cached(qpmap_phys_addr, qpmap_addr_info->qpmap_table_size[chn_index] * QPMAP_FRAME_BUF_NUM);
        if (VS_NULL == qpmap_virt_addr) {
            vs_mal_mmz_free(qpmap_phys_addr);
            vs_sample_trace("vpp_chnid[%d] no virt memory for qpmap block \n", p_send_qpmap_param->vpp_chnid);
            return VS_FAILED;
        }
        vs_sample_trace("qpmap phys[0x%llx] to virt[%p].\n", qpmap_phys_addr, qpmap_virt_addr);
        memset(qpmap_virt_addr, QPMAP_RESET_VALUE, qpmap_addr_info->qpmap_table_size[chn_index] * QPMAP_FRAME_BUF_NUM);
        for (buf_index = 0; buf_index < QPMAP_FRAME_BUF_NUM; buf_index++) {
            qpmap_addr_info->qpmap_phys_addr[chn_index][buf_index] = (vs_uint64_t)(qpmap_phys_addr + buf_index * qpmap_addr_info->qpmap_table_size[chn_index]);
            qpmap_addr_info->qpmap_virt_addr[chn_index][buf_index] = qpmap_virt_addr + buf_index * qpmap_addr_info->qpmap_table_size[chn_index];
        }
    }

    return VS_SUCCESS;
}

static vs_void_t *venc_qpmap_frame_send_task(vs_void_t *arg)
{
    sample_venc_send_qpmap_frame_param_s *p_send_qpmap_param = (sample_venc_send_qpmap_frame_param_s *)arg;
    vs_int32_t ret = VS_SUCCESS, chn_index = 0;
    vs_uint32_t send_frame_num = 0;
    vs_venc_qpmap_frame_info_s frame_info[QPMAP_VENC_MAX_CHN][QPMAP_FRAME_BUF_NUM] = {0};
    sample_common_venc_qpmap_addr_info qpmap_addr_info = {0};
    vs_vpp_chn_attr_s vpp_chn_attr;
    vs_uint32_t frame_buf_index = 0;

    if (p_send_qpmap_param->venc_chnnum > QPMAP_VENC_MAX_CHN) {
        vs_sample_trace("qpmap_send_frame not support venc chn num(%d) > qpmap max num(%d).\n",
                        p_send_qpmap_param->venc_chnnum, QPMAP_VENC_MAX_CHN);
        return VS_NULL;
    }

    if ((ret = sample_common_qpmap_alloc(p_send_qpmap_param, &qpmap_addr_info)) != VS_SUCCESS) {
        vs_sample_trace("sample_common_qpmap_alloc failed.\n");
        goto error;
    }

    ret = vs_mal_vpp_chn_attr_get(p_send_qpmap_param->vpp_grpid, p_send_qpmap_param->vpp_chnid, &vpp_chn_attr);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_vpp_chn_attr_get(grp %d chn %d) failed with 0x%x.\n",
                        p_send_qpmap_param->vpp_grpid, p_send_qpmap_param->vpp_chnid, ret);
        return VS_NULL;
    }
    vpp_chn_attr.depth += 2;
    ret = vs_mal_vpp_chn_attr_set(p_send_qpmap_param->vpp_grpid, p_send_qpmap_param->vpp_chnid, &vpp_chn_attr);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_vpp_chn_attr_set(grp %d chn %d) failed with 0x%x.\n",
                        p_send_qpmap_param->vpp_grpid, p_send_qpmap_param->vpp_chnid, ret);
        return VS_NULL;
    }

    while(VS_FALSE == p_send_qpmap_param->stop_send_task) {
        for (chn_index = 0; chn_index < p_send_qpmap_param->venc_chnnum; chn_index++) {
            vs_video_frame_info_s *video_frame = &frame_info[chn_index][frame_buf_index].user_frame_info;
            do {
                ret = vs_mal_vpp_chn_frame_acquire(p_send_qpmap_param->vpp_grpid, p_send_qpmap_param->vpp_chnid, video_frame, 1000);
                if (VS_SUCCESS != ret) {
                    vs_sample_trace("vpp_chnid[%d] cant't get buffer for ret[0x%x] send_frame_num[%d], retry later.\n",
                                        p_send_qpmap_param->vpp_chnid, ret, send_frame_num);
                    usleep(100 * 1000);// 5 frame time
                }
            } while (VS_SUCCESS != ret && VS_FALSE == p_send_qpmap_param->stop_send_task);
            if(VS_TRUE == p_send_qpmap_param->stop_send_task) {
                vs_sample_trace("send_task have be stop by other.\n");
                if(VS_SUCCESS == ret) {
                    vs_mal_vpp_chn_frame_release(p_send_qpmap_param->vpp_grpid, p_send_qpmap_param->vpp_chnid, video_frame);
                }
                break;
            }
            frame_info[chn_index][frame_buf_index].user_frame_info.frame.id = 0;
            frame_info[chn_index][frame_buf_index].user_frame_info.frame.pts = 0;
            frame_info[chn_index][frame_buf_index].user_frame_info.frame.width = p_send_qpmap_param->width;
            frame_info[chn_index][frame_buf_index].user_frame_info.frame.height = p_send_qpmap_param->height;
            frame_info[chn_index][frame_buf_index].user_frame_info.frame.pixel_format = p_send_qpmap_param->pixel_format;

            frame_info[chn_index][frame_buf_index].qpmap_info.map_size = qpmap_addr_info.qpmap_table_size[chn_index];
            frame_info[chn_index][frame_buf_index].qpmap_info.map_phys_addr = qpmap_addr_info.qpmap_phys_addr[chn_index][frame_buf_index];

            frame_info[chn_index][frame_buf_index].qpmap_info.skipmap_enable = VS_TRUE;//whether to enable skipmap encoding.
            frame_info[chn_index][frame_buf_index].qpmap_info.first_blk_qp = 18;//fisrt blk qp value in qpmap mode
            frame_info[chn_index][frame_buf_index].qpmap_info.blk_unit = 2;//enum of block unit size.
            frame_info[chn_index][frame_buf_index].qpmap_info.frame_type = E_VENC_QPMAP_FRAME_TYPE_NONE;

            venc_qpmap_data_default_get(p_send_qpmap_param->venc_type[chn_index], &frame_info[chn_index][frame_buf_index], (vs_uint8_t *)qpmap_addr_info.qpmap_virt_addr[chn_index][frame_buf_index],
                                        frame_info[chn_index][frame_buf_index].qpmap_info.blk_unit, send_frame_num);
		    if (qpmap_addr_info.qpmap_virt_addr[chn_index][frame_buf_index] != 0) {
                vs_mal_sys_cache_flush(frame_info[chn_index][frame_buf_index].qpmap_info.map_phys_addr, qpmap_addr_info.qpmap_virt_addr[chn_index][frame_buf_index], frame_info[chn_index][frame_buf_index].qpmap_info.map_size);
			}
            ret = vs_mal_venc_qpmap_frame_send(p_send_qpmap_param->venc_chnid[chn_index], &frame_info[chn_index][frame_buf_index], 1000);
            if (VS_SUCCESS != ret) {
                sample_common_venc_frame_info_print(&frame_info[chn_index][frame_buf_index].user_frame_info.frame);
                vs_sample_trace("venc_chnid[%d] send qpmap frame[%d] error, ret[0x%x].\n", p_send_qpmap_param->venc_chnid[chn_index], send_frame_num, ret);
            }


            vs_mal_vpp_chn_frame_release(p_send_qpmap_param->vpp_grpid, p_send_qpmap_param->vpp_chnid, &frame_info[chn_index][frame_buf_index].user_frame_info);

            frame_info[chn_index][frame_buf_index].user_frame_info.frame.id++;
        }
        frame_buf_index++;
        if (frame_buf_index >= QPMAP_FRAME_BUF_NUM) {
            frame_buf_index = 0;
        }
        send_frame_num++;
        usleep(10*1000);
    }
error:

    for (chn_index = 0; chn_index < p_send_qpmap_param->venc_chnnum; chn_index++) {
        if (qpmap_addr_info.qpmap_phys_addr[chn_index][0] != 0) {
            if (qpmap_addr_info.qpmap_virt_addr[chn_index][0] != NULL) {
                vs_mal_sys_unmap(qpmap_addr_info.qpmap_virt_addr[chn_index][0], \
                    qpmap_addr_info.qpmap_table_size[chn_index] * QPMAP_FRAME_BUF_NUM);
            }
            vs_mal_mmz_free(qpmap_addr_info.qpmap_phys_addr[chn_index][0]);
        }
    }

    vs_sample_trace("vpp_chnid[%d] venc_qpmap_frame_send_task exited !!!!!! \n", p_send_qpmap_param->vpp_chnid);
    return VS_NULL;
}

vs_int32_t sample_common_venc_send_qpmap_frame_start(sample_venc_send_qpmap_frame_param_s *p_send_qpmap_param)
{
    vs_int32_t ret;

    p_send_qpmap_param->stop_send_task = VS_FALSE;
    ret = pthread_create(&p_send_qpmap_param->thread_pid, 0, venc_qpmap_frame_send_task, (void*)p_send_qpmap_param);
    vs_sample_trace("venc_qpmap_frame_send_task start vpp_chnid[%d] thread_pid[%lu].\n",
                    p_send_qpmap_param->vpp_chnid, p_send_qpmap_param->thread_pid);
    return ret;
}

vs_int32_t sample_common_venc_send_qpmap_frame_stop(sample_venc_send_qpmap_frame_param_s *p_send_qpmap_param)
{
    if (p_send_qpmap_param->stop_send_task == VS_FALSE) {
        p_send_qpmap_param->stop_send_task = VS_TRUE;
        vs_sample_trace("pthread_join thread_pid[%lu]! \n", p_send_qpmap_param->thread_pid);
        pthread_join(p_send_qpmap_param->thread_pid, 0);
    }
    vs_sample_trace("venc_qpmap_frame_send_task stop!\n");
    return VS_SUCCESS;
}

#ifdef __cplusplus
}
#endif
