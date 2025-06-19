/**
* @file image_file_util.h
* @brief bmp or yuv file util
* @details bmp or yuv file util
* @author VS software group
* @date 2022-06-21
* @version v1.0
* @Copyright (C) 2022, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "image_file_util.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * 'read_word()' - Read a 16-bit unsigned integer.
 */
vs_uint16_t read_word(FILE *fp)
{
    vs_uint8_t b0, b1; /* Bytes from file */
    b0 = getc(fp);
    b1 = getc(fp);
    return ((b1 << 8) | b0);
}

/*
 * 'read_dword()' - Read a 32-bit unsigned integer.
 */
vs_uint32_t read_dword(FILE *fp)
{
    vs_uint8_t b0, b1, b2, b3; /* Bytes from file */
    b0 = getc(fp);
    b1 = getc(fp);
    b2 = getc(fp);
    b3 = getc(fp);
    return ((((((b3 << 8) | b2) << 8) | b1) << 8) | b0);
}

/*
 * 'read_long()' - Read a 32-bit signed integer.
 */
vs_int32_t read_long(FILE *fp)
{
    vs_uint8_t b0, b1, b2, b3; /* Bytes from file */
    b0 = getc(fp);
    b1 = getc(fp);
    b2 = getc(fp);
    b3 = getc(fp);
    return ((vs_int32_t)(((((b3 << 8) | b2) << 8) | b1) << 8) | b0);
}

vs_int32_t load_bmp(const char* filename, vs_void_t* p_virt_addr, vs_uint32_t stride)
{
    vs_int32_t ret = VS_SUCCESS;
    vs_uint32_t buf_stride;

    if (p_virt_addr == VS_NULL) {
        vs_sample_trace("virt addr wrong, please alloc mem\n");
        return VS_FAILED;
    }
    BMPFILEHEADER header = {0};
    FILE* pfile = fopen(filename, "rb");
    if (pfile == VS_NULL) {
        vs_sample_trace("fopen fail\n");
        return VS_FAILED;
    }
    header.bfType      = read_word(pfile);
    header.bfSize      = read_dword(pfile);
    header.bfReserved1 = read_word(pfile);
    header.bfReserved2 = read_word(pfile);
    header.bfOffBits   = read_dword(pfile);

    BMPINFOHEADER bmpInfoHeader = {0};
    bmpInfoHeader.biSize          = read_dword(pfile);
    bmpInfoHeader.biWidth         = read_long(pfile);
    bmpInfoHeader.biHeight        = read_long(pfile);
    bmpInfoHeader.biPlanes        = read_word(pfile);
    bmpInfoHeader.biBitCount      = read_word(pfile);
    bmpInfoHeader.biCompression   = read_dword(pfile);
    bmpInfoHeader.biSizeImage     = read_dword(pfile);
    bmpInfoHeader.biXPelsPerMeter = read_long(pfile);
    bmpInfoHeader.biYPelsPerMeter = read_long(pfile);
    bmpInfoHeader.biClrUsed       = read_dword(pfile);
    bmpInfoHeader.biClrImportant  = read_dword(pfile);

    vs_uint32_t byteSize = 0;
    if ((byteSize = bmpInfoHeader.biSizeImage) == 0) {
        byteSize = bmpInfoHeader.biWidth * ((bmpInfoHeader.biBitCount + 7) / 8) * bmpInfoHeader.biHeight;
    }
    vs_uint8_t* pBmpBuf = (vs_uint8_t*) malloc(byteSize);
    ret = fseek(pfile, header.bfOffBits, SEEK_SET);
    if (ret != 0) {
        vs_sample_trace("bmp file seek image fail, ret[%d]\n", ret);
        goto onError;
    }
    ret = fread(pBmpBuf, byteSize, 1, pfile);
    if (ret != 1) {
        vs_sample_trace("bmp file read image fail, ret[%d]\n", ret);
    }
    vs_uint32_t Bpp = bmpInfoHeader.biBitCount / 8;
    vs_uint32_t pic_stride = bmpInfoHeader.biWidth * Bpp;
    vs_uint32_t height = bmpInfoHeader.biHeight;

    if (stride != 0) {
        if (stride < pic_stride) {
            vs_sample_trace("buffer stride[%d] can't less than picture stride[%d]\n", stride, pic_stride)
            goto onError;
        }
        buf_stride = stride;
    } else {
       buf_stride = pic_stride;
    }

    for (vs_uint32_t i = 0; i < height; i++) {
        memcpy(p_virt_addr + i * buf_stride, pBmpBuf + (height - 1 - i) * pic_stride, pic_stride);
    }

onError:
    if (pBmpBuf != VS_NULL) {
        free(pBmpBuf);
        pBmpBuf = VS_NULL;
    }
    fclose(pfile);
    return ret;
}

vs_int32_t load_yuv_two_planes(const char* filename, vs_void_t* p_virt_addr[], vs_uint32_t imgsize[])
{
    vs_int32_t ret = VS_SUCCESS;
    FILE* pfile = fopen(filename, "rb");
    if (pfile == VS_NULL) {
        vs_sample_trace("fopen fail\n");
        return VS_FAILED;
    }

    vs_void_t *p_virt_addr_Y = p_virt_addr[0];
    ret = fread(p_virt_addr_Y, imgsize[0], 1, pfile);
    if (ret != 1) {
        vs_sample_trace("yuv file read Y stream fail, ret[%d]\n", ret);
        goto onError;
    }

    vs_void_t *p_virt_addr_uv = p_virt_addr[1];
    ret = fread(p_virt_addr_uv, imgsize[1], 1, pfile);
    if (ret != 1) {
        vs_sample_trace("yuv file read uv stream fail, ret[%d]\n", ret);
        goto onError;
    }

onError:
    fclose(pfile);
    return ret;
}

vs_int32_t save_yuv(const char* filename, vs_void_t* p_virt_addr[], vs_uint32_t imgsize[])
{
    vs_int32_t ret = VS_SUCCESS;
    FILE* pfile = fopen(filename, "wb");
    if (pfile == VS_NULL) {
        vs_sample_trace("fopen fail\n");
        return VS_FAILED;
    }
    ret = fwrite(p_virt_addr[0], imgsize[0], 1, pfile);
    if (ret != 1) {
        vs_sample_trace("yuv file write Y stream fail, ret[%d]\n", ret);
        goto onError;
    }
    if (p_virt_addr[1] != VS_NULL && imgsize[1] > 0) {
        ret = fwrite(p_virt_addr[1], imgsize[1], 1, pfile);
        if (ret != 1) {
            vs_sample_trace("yuv file write uv/u stream fail, ret[%d]\n", ret);
            goto onError;
        }
    }
    if (p_virt_addr[2] != VS_NULL && imgsize[2] > 0) {
        ret = fwrite(p_virt_addr[2], imgsize[2], 1, pfile);
        if (ret != 1) {
            vs_sample_trace("yuv file write v stream fail, ret[%d]\n", ret);
            goto onError;
        }
    }

onError:
    fclose(pfile);
    return ret;
}

static vs_void_t frame_vb_map(vs_video_frame_s *p_frame)
{
	vs_uint64_t y_size = 0;
	vs_uint64_t uv_size = 0;
	vs_uint64_t y_head_size = 0;
	vs_uint64_t uv_head_size = 0;

	y_size = p_frame->height * p_frame->stride[0];
	y_head_size = COMPRESSION_HEADLINE * p_frame->height;

	switch (p_frame->pixel_format) {
		case E_PIXEL_FORMAT_YUV_422SP:
		case E_PIXEL_FORMAT_YVU_422SP:
			uv_size = y_size;
			uv_head_size = y_head_size;
			break;
		case E_PIXEL_FORMAT_YUV_420SP:
		case E_PIXEL_FORMAT_YVU_420SP:
			uv_size = y_size / 2;
			uv_head_size = y_head_size / 2;
			break;
		case E_PIXEL_FORMAT_YUV_400:
		default :
			break;
	}

	p_frame->virt_addr[0] =  (vs_uint64_t)vs_mal_sys_mmap(p_frame->phys_addr[0], y_size + uv_size + y_head_size + uv_head_size);
	p_frame->virt_addr[1] = p_frame->virt_addr[0] + y_size;

	if (p_frame->compress_mode == E_COMPRESS_MODE_RASTER) {
		p_frame->compress_header_virt_addr[0] = p_frame->virt_addr[1] + uv_size;
		p_frame->compress_header_virt_addr[1] = p_frame->compress_header_virt_addr[0] + y_head_size;
	}

	return ;
}

static vs_void_t frame_vb_unmap(vs_video_frame_s *p_frame)
{
	vs_uint64_t y_size = 0;
	vs_uint64_t uv_size = 0;
	vs_uint64_t y_head_size = 0;
	vs_uint64_t uv_head_size = 0;

	y_size = p_frame->height * p_frame->stride[0];
	y_head_size = COMPRESSION_HEADLINE * p_frame->height;

	switch (p_frame->pixel_format) {
		case E_PIXEL_FORMAT_YUV_422SP:
		case E_PIXEL_FORMAT_YVU_422SP:
			uv_size = y_size;
			uv_head_size = y_head_size;
			break;
		case E_PIXEL_FORMAT_YUV_420SP:
		case E_PIXEL_FORMAT_YVU_420SP:
			uv_size = y_size / 2;
			uv_head_size = y_head_size / 2;
			break;
		case E_PIXEL_FORMAT_YUV_400:
		default :
			break;
	}

	vs_mal_sys_unmap((vs_void_t *)p_frame->virt_addr[0], (vs_uint32_t)(y_size + uv_size + y_head_size + uv_head_size));
	p_frame->virt_addr[0] =  0;
	p_frame->virt_addr[1] = 0;

	if (p_frame->compress_mode == E_COMPRESS_MODE_RASTER) {
		p_frame->compress_header_virt_addr[0] = 0;
		p_frame->compress_header_virt_addr[1] = 0;
	}

	return ;
}

vs_int32_t file_to_frame(char *p_filename, vs_video_frame_info_s *g_frame_info)
{
	vs_int32_t ret, i, width = g_frame_info->frame.width, height = g_frame_info->frame.height;
	vs_bool_t head_flag = VS_FALSE, mam_flap = VS_FALSE;
	char *pbuf;
	FILE* p_fp;

	if (!g_frame_info) {
		vs_sample_trace("g_frame_info is NULL!\n");
		return VS_FAILED;
	}

	if (!g_frame_info->frame.phys_addr[0]) {
		vs_sample_trace("g_frame_info->frame phys_addr is null!\n");
		return VS_FAILED;
	}

	if (!p_filename) {
		vs_sample_trace("p_filename is NULL!\n");
		return VS_FAILED;
	}

	if (g_frame_info->frame.compress_mode) {
		head_flag = VS_TRUE;
	}

	p_fp = fopen(p_filename, "rb");
	if (p_fp == VS_NULL) {
		vs_sample_trace("picture %s open fail\n", p_filename);
		return VS_FAILED;
	}
	vs_sample_trace("picture %s open success p_fpch=%p \n", p_filename, p_fp)

	vs_sample_trace("start p_fp=0x%llx width=%u height=%u \n", (vs_uint64_t)p_fp, width, height);
	(vs_void_t)fgetc(p_fp);
	if (feof(p_fp)) {
		vs_sample_trace("end of file!\n");
		ret = fseek(p_fp, 0 , SEEK_SET );
		if (0 != ret) {
			vs_sample_trace("fseek1 failed! errcode = %d\n", ret);
			fclose(p_fp);
			return ret;
		}
	} else {
		ret = fseek(p_fp, -1 , SEEK_CUR );
		if (0 != ret) {
			vs_sample_trace("fseek2 failed! errcode = %d\n", ret);
			fclose(p_fp);
			return ret;
		}
	}

	if (!g_frame_info->frame.virt_addr[0]) {
		frame_vb_map(&g_frame_info->frame);
		mam_flap = VS_TRUE;
	}

	pbuf = (char *)g_frame_info->frame.virt_addr[0];
	for (i = 0; i < g_frame_info->frame.height; i++) {
		ret = fread(pbuf, width, 1, p_fp);
		if (1 != ret) {
			vs_sample_trace("Read file y %d fail! errcode = %d\n", i, ret);
			if (mam_flap) {
				frame_vb_unmap(&g_frame_info->frame);
			}
			fclose(p_fp);
			return VS_FAILED;
		}
		pbuf += g_frame_info->frame.stride[0];
	}
	vs_sample_trace("Read file y success line%d \n", g_frame_info->frame.height);

	switch (g_frame_info->frame.pixel_format) {
		case E_PIXEL_FORMAT_YUV_422SP:
		case E_PIXEL_FORMAT_YVU_422SP:
			height = g_frame_info->frame.height;
			break;
		case E_PIXEL_FORMAT_YUV_420SP:
		case E_PIXEL_FORMAT_YVU_420SP:
			height = g_frame_info->frame.height / 2;
			break;
		case E_PIXEL_FORMAT_YUV_400:
			height = 0;
			break;
		default:
			height = g_frame_info->frame.height;
			vs_sample_trace(" pixel_format error pixel_format = %d \n", g_frame_info->frame.pixel_format);
			break;
	}

	pbuf = (char *)g_frame_info->frame.virt_addr[1];
	for (i = 0; i < height; i++) {
		ret = fread(pbuf, width, 1, p_fp);
		if ( 1 != ret) {
			vs_sample_trace("Read file uv %d fail! errcode = %d\n", i, ret);
			if (mam_flap) {
				frame_vb_unmap(&g_frame_info->frame);
			}
			fclose(p_fp);
			return VS_FAILED;
		}
		pbuf += g_frame_info->frame.stride[1];
	}
	vs_sample_trace("Read file uv success line%d \n", height);

	if (head_flag) {
		pbuf = (char *)g_frame_info->frame.compress_header_virt_addr[0];
		for (i = 0; i < g_frame_info->frame.height; i++) {
			ret = fread(pbuf, COMPRESSION_HEADLINE, 1, p_fp);
			if ( 1 != ret) {
				vs_sample_trace("Read file hy %d fail! errcode = %d\n", i, ret);
				if (mam_flap) {
					frame_vb_unmap(&g_frame_info->frame);
				}
				fclose(p_fp);
				return VS_FAILED;
			}
			pbuf += COMPRESSION_HEADLINE;
		}
		vs_sample_trace("Read file hy success height=%d \n", g_frame_info->frame.height);

		pbuf = (char *)g_frame_info->frame.compress_header_virt_addr[1];
		for (i = 0; i < height; i++) {
			ret = fread(pbuf, COMPRESSION_HEADLINE, 1, p_fp);
			if ( 1 != ret) {
				vs_sample_trace("Read file huv %d fail! errcode = %d\n", i, ret);
				if (mam_flap) {
					frame_vb_unmap(&g_frame_info->frame);
				}
				fclose(p_fp);
				return VS_FAILED;
			}
			pbuf += COMPRESSION_HEADLINE;
		}
		vs_sample_trace("Read file huv success height=%d \n", height);
	}

	if (mam_flap) {
		frame_vb_unmap(&g_frame_info->frame);
	}
	fclose(p_fp);
	vs_sample_trace("end \n");

	return VS_SUCCESS;
}

vs_int32_t frame_to_file(vs_video_frame_info_s *g_frame_info, char *p_filename)
{
	vs_int32_t ret, i, write_cnt, write_rest, write_index, width = g_frame_info->frame.width, height = g_frame_info->frame.height;
	vs_bool_t head_flag = VS_FALSE, mam_flap = VS_FALSE;
	char *pbuf;
	FILE* p_fp = VS_NULL;

	if (!g_frame_info) {
		vs_sample_trace("g_frame_info is NULL!\n");
		return VS_FAILED;
	}

	if (!g_frame_info->frame.phys_addr[0]) {
		vs_sample_trace("g_frame_info->frame phys_addr is null!\n");
		return VS_FAILED;
	}

	if (!p_filename) {
		vs_sample_trace("p_filename is NULL!\n");
		return VS_FAILED;
	}

	if (g_frame_info->frame.compress_mode == E_COMPRESS_MODE_RASTER) {
		head_flag = VS_TRUE;
	}

	p_fp = fopen(p_filename, "wb");
	if (p_fp == VS_NULL) {
		vs_sample_trace("picture %s open fail\n", p_filename);
		return VS_FAILED;
	}
	vs_sample_trace("picture %s open success p_fpch=%p \n", p_filename, p_fp);

	write_cnt = width / WRITE_FILE_LEN_MAX;
	write_rest = width % WRITE_FILE_LEN_MAX;

	if (!g_frame_info->frame.virt_addr[0]) {
		frame_vb_map(&g_frame_info->frame);
		mam_flap = VS_TRUE;
	}
	pbuf = (char *)g_frame_info->frame.virt_addr[0];
	for (i = 0; i < g_frame_info->frame.height; i++) {
		for (write_index = 0; write_index < write_cnt; write_index++) {
			ret = fwrite(pbuf, 1, WRITE_FILE_LEN_MAX, p_fp);
			if ( WRITE_FILE_LEN_MAX != ret) {
				vs_sample_trace("Write file line_y[%d] index[%d] fail! errcode = %d\n", i, write_index, ret);
				if (mam_flap) {
					frame_vb_unmap(&g_frame_info->frame);
				}
				fclose(p_fp);
				return VS_FAILED;
			}
			pbuf += WRITE_FILE_LEN_MAX;
		}

		if (write_rest) {
			ret = fwrite(pbuf, 1, write_rest, p_fp);
			if ( write_rest != ret) {
				vs_sample_trace("Write file line_y_rest %d fail! errcode = %d\n", i, ret);
				if (mam_flap) {
					frame_vb_unmap(&g_frame_info->frame);
				}
				fclose(p_fp);
				return VS_FAILED;
			}
			pbuf += write_rest;
		}
		pbuf += (g_frame_info->frame.stride[0] - width);
	}
	vs_sample_trace("Write file y success line%d \n", g_frame_info->frame.height);

	switch (g_frame_info->frame.pixel_format) {
		case E_PIXEL_FORMAT_YUV_422SP:
		case E_PIXEL_FORMAT_YVU_422SP:
			height = g_frame_info->frame.height;
			break;
		case E_PIXEL_FORMAT_YUV_420SP:
		case E_PIXEL_FORMAT_YVU_420SP:
			height = g_frame_info->frame.height / 2;
			break;
		case E_PIXEL_FORMAT_YUV_400:
			height = 0;
			break;
		default:
			height = g_frame_info->frame.height;
			vs_sample_trace(" pixel_format error pixel_format = %d \n", g_frame_info->frame.pixel_format);
			break;
	}

	pbuf = (char *)g_frame_info->frame.virt_addr[1];
	for (i = 0; i < height; i++) {
		for (write_index = 0; write_index < write_cnt; write_index++) {
			ret = fwrite(pbuf, 1, WRITE_FILE_LEN_MAX, p_fp);
			if ( WRITE_FILE_LEN_MAX != ret) {
				vs_sample_trace("Write file line_uv[%d] index[%d] fail! errcode = %d\n", i, write_index, ret);
				if (mam_flap) {
					frame_vb_unmap(&g_frame_info->frame);
				}
				fclose(p_fp);
				return VS_FAILED;
			}
			pbuf += WRITE_FILE_LEN_MAX;
		}
		if (write_rest) {
			ret = fwrite(pbuf, 1, write_rest, p_fp);
			if ( write_rest != ret) {
				vs_sample_trace("Write file line_uv_rest %d fail! errcode = %d\n", i, ret);
				if (mam_flap) {
					frame_vb_unmap(&g_frame_info->frame);
				}
				fclose(p_fp);
				return VS_FAILED;
			}
			pbuf += write_rest;
		}
		pbuf += (g_frame_info->frame.stride[1] - width);
	}
	vs_sample_trace("Write file uv success line%d \n", height);

	if (head_flag) {
		pbuf = (char *)g_frame_info->frame.compress_header_virt_addr[0];
		for (i = 0; i < g_frame_info->frame.height; i++) {
			ret = fwrite(pbuf, 1, COMPRESSION_HEADLINE, p_fp);
			if ( COMPRESSION_HEADLINE != ret) {
				vs_sample_trace("Write file line_hy[%d] fail! errcode = %d\n", i, ret);
				if (mam_flap) {
					frame_vb_unmap(&g_frame_info->frame);
				}
				fclose(p_fp);
				return VS_FAILED;
			}
			pbuf += COMPRESSION_HEADLINE;
		}
		vs_sample_trace("Read file hy success height=%d \n", g_frame_info->frame.height);

		pbuf = (char *)g_frame_info->frame.compress_header_virt_addr[1];
		for (i = 0; i < height; i++) {
			ret = fwrite(pbuf, 1, COMPRESSION_HEADLINE, p_fp);
			if ( COMPRESSION_HEADLINE != ret) {
				vs_sample_trace("Write file line_huv[%d] fail! errcode = %d\n", i, ret);
				if (mam_flap) {
					frame_vb_unmap(&g_frame_info->frame);
				}
				fclose(p_fp);
				return VS_FAILED;
			}
			pbuf += COMPRESSION_HEADLINE;
		}

		vs_sample_trace("Read file huv success height=%d \n", height);
	}
	if (mam_flap) {
		frame_vb_unmap(&g_frame_info->frame);
	}
	fclose(p_fp);
	vs_sample_trace("end \n");
	return VS_SUCCESS;
}


#ifdef __cplusplus
}
#endif
