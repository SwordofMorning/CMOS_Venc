/**
* @file image_file_util.h
* @brief bmp or yuv file util
* @details bmp or yuv file util
* @author VS software group
* @date 2022-06-21
* @version v1.0
* @Copyright (C) 2022, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/

#ifndef _IMAGE_FILE_UTIL_H_
#define _IMAGE_FILE_UTIL_H_

#include "sample_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**** BMP file header structure ****/
typedef struct BMPFILEHEADER {
    vs_uint16_t   bfType;           /* Magic number for file */
    vs_int32_t    bfSize;           /* Size of file */
    vs_uint16_t   bfReserved1;      /* Reserved */
    vs_uint16_t   bfReserved2;      /* ... */
    vs_int32_t    bfOffBits;        /* Offset to bitmap data */
} BMPFILEHEADER;

/**** BMP file info structure ****/
typedef struct BMPINFOHEADER {
    vs_uint32_t   biSize;           /* Size of info header */
    vs_int32_t    biWidth;          /* Width of image */
    vs_int32_t    biHeight;         /* Height of image */
    vs_uint16_t   biPlanes;         /* Number of color planes */
    vs_uint16_t   biBitCount;       /* Number of bits per pixel */
    vs_uint32_t   biCompression;    /* Type of compression to use */
    vs_uint32_t   biSizeImage;      /* Size of image data */
    vs_int32_t    biXPelsPerMeter;  /* X pixels per meter */
    vs_int32_t    biYPelsPerMeter;  /* Y pixels per meter */
    vs_uint32_t   biClrUsed;        /* Number of colors used */
    vs_uint32_t   biClrImportant;   /* Number of important colors */
} BMPINFOHEADER;

vs_int32_t load_bmp(const char* filename, vs_void_t* p_virt_addr, vs_uint32_t stride);

vs_int32_t load_yuv_two_planes(const char* filename, vs_void_t* p_virt_addr[], vs_uint32_t imgsize[]);

vs_int32_t save_yuv(const char* filename, vs_void_t* p_virt_addr[], vs_uint32_t imgsize[]);

vs_int32_t file_to_frame(char *p_filename, vs_video_frame_info_s *g_frame_info);
vs_int32_t frame_to_file(vs_video_frame_info_s *g_frame_info, char *p_filename);

#ifdef __cplusplus
}
#endif

#endif