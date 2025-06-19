/**
* @file sample_common_vdec.c
* @brief implements the common functions of vdec example.
* @details
* @author Visinex Software Group
* @date 2021-07-20
* @version v0.01
* @Copyright (C) 2020, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "vs_comm.h"
#include "sample_common.h"

#ifdef __cplusplus
extern "C" {
#endif

vs_pool_type_e g_vdec_vb_source = VB_SOURCE_PRIVATE;


vs_int32_t sample_common_vdec_vb_pool_init(vs_int32_t vdec_chnnum, sample_vdec_cfg_s *p_sample_vdec_cfg)
{
    vs_vb_cfg_s vb_cfg;
    vs_int32_t i = 0, ret = VS_SUCCESS;
    vs_vb_pool_cfg_s vb_pool_cfg;

    memset(&vb_cfg, 0, sizeof(struct vs_vb_cfg));

    for (i = 0; i < vdec_chnnum; i++) {
        p_sample_vdec_cfg[i].frame_buf_size = vdec_frame_buffer_size_get(p_sample_vdec_cfg[i].decode_type,
                                                p_sample_vdec_cfg[i].width, p_sample_vdec_cfg[i].height,
                                                p_sample_vdec_cfg[i].video_format, p_sample_vdec_cfg[i].pixel_format,
                                                p_sample_vdec_cfg[i].compress_mode);
    }

    if (g_vdec_vb_source == VB_SOURCE_MODULE) {
        vs_mal_vb_modpool_exit(VS_VB_UID_VDEC);
        for (i = 0; i < vdec_chnnum; i++) {
            vb_cfg.ast_commpool[i].blk_cnt = p_sample_vdec_cfg[i].frame_buf_cnt;
            vb_cfg.ast_commpool[i].blk_size = p_sample_vdec_cfg[i].frame_buf_size;
            vb_cfg.ast_commpool[i].remap_mode = VB_REMAP_MODE_NONE;
        }
        vb_cfg.pool_cnt = vdec_chnnum;
        ret = vs_mal_modpool_cfg_set(VS_VB_UID_VDEC, &vb_cfg);
        if (ret != VS_SUCCESS) {
            vs_sample_trace("chnid[%d] vs_mal_modpool_cfg_set ret[0x%x]!\n",
                p_sample_vdec_cfg[i].vdec_chnid, ret);
            return VS_FAILED;
        }
        ret = vs_mal_vb_modpool_init(VS_VB_UID_VDEC);
        if (ret != VS_SUCCESS) {
            vs_sample_trace("chnid[%d] vs_mal_vb_modpool_init ret[0x%x]!\n",
                p_sample_vdec_cfg[i].vdec_chnid, ret);
            vs_mal_vb_modpool_exit(VS_VB_UID_VDEC);
            return VS_FAILED;
        }
    } else if (g_vdec_vb_source == VB_SOURCE_USER) {
        for (i = 0; i < vdec_chnnum; i++) {
            vb_pool_cfg.blk_size = p_sample_vdec_cfg[i].frame_buf_size;
            vb_pool_cfg.blk_cnt = p_sample_vdec_cfg[i].frame_buf_cnt;
            vb_pool_cfg.remap_mode = VB_REMAP_MODE_NONE;
            p_sample_vdec_cfg[i].vb_poolid = vs_mal_vb_pool_create(&vb_pool_cfg);
            if (p_sample_vdec_cfg[i].vb_poolid == VS_INVALID_POOLID) {
                vs_sample_trace("chnid[%d] vs_mal_vb_pool_create ret[0x%x]!\n",
                                p_sample_vdec_cfg[i].vdec_chnid, ret);
                goto exit_vb_pool_deinit;
            }
        }
    }

    return VS_SUCCESS;

exit_vb_pool_deinit:
    for (; i >= 0; i--) {
        if (p_sample_vdec_cfg[i].vb_poolid != VS_INVALID_POOLID) {
            ret = vs_mal_vb_pool_destory(p_sample_vdec_cfg[i].vb_poolid);
            if (ret != VS_SUCCESS) {
                vs_sample_trace("chnid[%d] vs_mal_vb_pool_destory ret[0x%x]!\n",
                                p_sample_vdec_cfg[i].vdec_chnid, ret);
            }
            p_sample_vdec_cfg[i].vb_poolid = VS_INVALID_POOLID;
        }
    }
    return VS_FAILED;
}

vs_int32_t sample_common_vdec_vb_pool_deinit(vs_int32_t vdec_chnnum, sample_vdec_cfg_s *p_sample_vdec_cfg)
{
    vs_int32_t i = 0, ret = VS_SUCCESS;

    if (g_vdec_vb_source == VB_SOURCE_MODULE) {
        vs_mal_vb_modpool_exit(VS_VB_UID_VDEC);
    } else if (g_vdec_vb_source == VB_SOURCE_USER) {
        for (i = 0; i < vdec_chnnum; i++) {
            if (p_sample_vdec_cfg[i].vb_poolid != VS_INVALID_POOLID) {
                ret = vs_mal_vdec_vbpool_detach(p_sample_vdec_cfg[i].vdec_chnid);
                if (ret != VS_SUCCESS) {
                    vs_sample_trace("vs_mal_vdec_vbpool_attach ret[0x%x]!\n", ret);
                }
                ret = vs_mal_vb_pool_destory(p_sample_vdec_cfg[i].vb_poolid);
                if (ret != VS_SUCCESS) {
                    vs_sample_trace("chnid[%d] vs_mal_vb_pool_destory ret[0x%x]!\n",
                                    p_sample_vdec_cfg[i].vdec_chnid, ret);
                }
                p_sample_vdec_cfg[i].vb_poolid = VS_INVALID_POOLID;
            }
        }
    }
    return ret;
}

vs_int32_t sample_common_vdec_start(vs_int32_t vdec_chnnum, sample_vdec_cfg_s *p_sample_vdec_cfg)
{
    vs_int32_t i = 0, ret = VS_SUCCESS;
    vs_vdec_mod_param_s mod_param;
    vs_vdec_chn_attr_s chn_attr;
    vs_vdec_chn_param_s chn_param;

    ret = vs_mal_vdec_mod_param_get(&mod_param);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_vdec_mod_param_get ret[0x%x]!\n", ret);
        return ret;
    }
    mod_param.vb_source = g_vdec_vb_source;
    ret = vs_mal_vdec_mod_param_set(&mod_param);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_vdec_mod_param_set ret[0x%x]!\n", ret);
        return ret;
    }

    memset(&chn_attr, 0, sizeof(chn_attr));
    for (i = 0; i < vdec_chnnum; i++) {
        chn_attr.type = p_sample_vdec_cfg[i].decode_type;
        chn_attr.input_mode = p_sample_vdec_cfg[i].input_mode;
        chn_attr.width = p_sample_vdec_cfg[i].width;
        chn_attr.height = p_sample_vdec_cfg[i].height;
        chn_attr.stream_buf_size = p_sample_vdec_cfg[i].width * p_sample_vdec_cfg[i].height * 2;
        chn_attr.frame_buf_cnt = p_sample_vdec_cfg[i].frame_buf_cnt;
        chn_attr.frame_buf_size = p_sample_vdec_cfg[i].frame_buf_size;
        if (p_sample_vdec_cfg[i].decode_type == E_PT_TYPE_H265 || p_sample_vdec_cfg[i].decode_type == E_PT_TYPE_H264) {
            chn_attr.video_attr.ref_frame_num = p_sample_vdec_cfg[i].ref_frame_num;
        }

        ret = vs_mal_vdec_chn_create(p_sample_vdec_cfg[i].vdec_chnid, &chn_attr);
        if (ret != VS_SUCCESS) {
            vs_sample_trace("vs_mal_vdec_chn_create ret[0x%x]!\n", ret);
            goto exit_comm_vdec_stop;
        }

        if (g_vdec_vb_source == VB_SOURCE_USER) {
            ret = vs_mal_vdec_vbpool_attach(p_sample_vdec_cfg[i].vdec_chnid, p_sample_vdec_cfg[i].vb_poolid);
            if (ret != VS_SUCCESS) {
                vs_sample_trace("vs_mal_vdec_vbpool_attach ret[0x%x]!\n", ret);
                goto exit_comm_vdec_stop;
            }
        }

        ret = vs_mal_vdec_chn_param_get(p_sample_vdec_cfg[i].vdec_chnid, &chn_param);
        if (ret != VS_SUCCESS) {
            vs_sample_trace("vs_mal_vdec_vbpool_attach ret[0x%x]!\n", ret);
            goto exit_comm_vdec_stop;
        }
        chn_param.type = p_sample_vdec_cfg[i].decode_type;
        chn_param.compress_mode = p_sample_vdec_cfg->compress_mode;
        chn_param.video_format = p_sample_vdec_cfg->video_format;
        chn_param.pixel_format = p_sample_vdec_cfg[i].pixel_format;
        chn_param.output_frame_num = p_sample_vdec_cfg[i].output_frame_num;

        ret = vs_mal_vdec_chn_param_set(p_sample_vdec_cfg[i].vdec_chnid, &chn_param);
        if (ret != VS_SUCCESS) {
            vs_sample_trace("vs_mal_vdec_chn_param_set ret[0x%x]!\n", ret);
            goto exit_comm_vdec_stop;
        }

        ret = vs_mal_vdec_chn_start(p_sample_vdec_cfg[i].vdec_chnid);
        if (ret != VS_SUCCESS) {
            vs_sample_trace("vs_mal_vdec_chn_start ret[0x%x]!\n", ret);
            goto exit_comm_vdec_stop;
        }

    }

    return VS_SUCCESS;

exit_comm_vdec_stop:
    for (; i >= 0; i--) {
        ret = vs_mal_vdec_chn_stop(p_sample_vdec_cfg[i].vdec_chnid);
        if (ret != VS_SUCCESS) {
            vs_sample_trace("vs_mal_vdec_chn_stop ret[0x%x]!\n", ret);
        }
        ret = vs_mal_vdec_chn_destroy(p_sample_vdec_cfg[i].vdec_chnid);
        if (ret != VS_SUCCESS) {
            vs_sample_trace("vs_mal_vdec_chn_destroy ret[0x%x]!\n", ret);
        }
    }
    return VS_FAILED;
}

vs_int32_t sample_common_vdec_stop(vs_int32_t vdec_chnnum, sample_vdec_cfg_s *p_sample_vdec_cfg)
{
    vs_int32_t i = 0, ret = VS_SUCCESS;
    for (i = 0; i < vdec_chnnum; i++) {
        ret = vs_mal_vdec_chn_stop(p_sample_vdec_cfg[i].vdec_chnid);
        if (ret != VS_SUCCESS) {
            vs_sample_trace("vs_mal_vdec_chn_stop ret[0x%x]!\n", ret);
        }
        ret = vs_mal_vdec_chn_destroy(p_sample_vdec_cfg[i].vdec_chnid);
        if (ret != VS_SUCCESS) {
            vs_sample_trace("vs_mal_vdec_chn_destroy ret[0x%x]!\n", ret);
        }
    }
    return VS_SUCCESS;
}

vs_bool_t sample_common_vdec_find_one_frame(sample_vdec_thread_param_s *p_vdec_thread_param, vs_uint8_t *p_buf,
                                                            vs_uint32_t len, vs_uint32_t *p_read_len)
{
    vs_uint32_t tmp = 0, jpeg_len = 0, i = 0;
    vs_bool_t find_start = VS_FALSE, find_end = VS_FALSE, find_frame = VS_FALSE, is_i_frame = VS_FALSE;

    if (p_vdec_thread_param->decode_type == E_PT_TYPE_H264) {
        if (len < 8) {
            return VS_FALSE;
        }

        for (i = 0; i < len - 8; i++) {
            if (p_buf[i] == 0 && p_buf[i + 1] == 0 && p_buf[i + 2] == 1) {
                tmp = p_buf[i + 3] & 0x1F;
                if (((tmp == 0x5 || tmp == 0x1) && ((p_buf[i + 4] & 0x80) == 0x80))
                    || (tmp == 20 && (p_buf[i + 7] & 0x80) == 0x80)) {
                    if (tmp == 0x5) {
                        is_i_frame = VS_TRUE;
                    }
                    find_start = VS_TRUE;
                    i += 8;
                    break;
                }
            }
        }

        for (; i < len - 8; i++) {
            if (p_buf[i] == 0 && p_buf[i + 1] == 0 && p_buf[i + 2] == 1) {
                tmp = p_buf[i + 3] & 0x1F;
                if (tmp == 15 || tmp == 7 || tmp == 8 || tmp == 6 || ((tmp == 5 || tmp == 1)
                    && ((p_buf[i + 4] & 0x80) == 0x80)) || (tmp == 20 && (p_buf[i + 7] & 0x80) == 0x80)) {
                    find_end = VS_TRUE;
                    break;
                }
            }
        }

        if(i > 0) {
            *p_read_len = p_buf[i - 1] == 0 ? i - 1 : i;
        }
        if (find_end == VS_FALSE) {
            *p_read_len = i + 8;
        }
        if (find_start == VS_TRUE && find_end == VS_TRUE) {
            find_frame = VS_TRUE;
        }
    } else if (p_vdec_thread_param->decode_type == E_PT_TYPE_H265) {
        if (len < 6) {
            return VS_FALSE;
        }

        for (i = 0; i < len - 6; i++) {
            if (p_buf[i] == 0 && p_buf[i + 1] == 0 && p_buf[i + 2] == 1) {
                tmp = (p_buf[i + 3] & 0x7E) >> 1;
                if ((tmp >= 0 && tmp <= 21) && ((p_buf[i + 5] & 0x80) == 0x80)) {
                    if (tmp == 19 || tmp == 20) {
                        is_i_frame = VS_TRUE;
                    }
                    find_start = VS_TRUE;
                    i += 6;
                    break;
                }
            }
        }

        for (; i < len - 6; i++) {
            if (p_buf[i] == 0 && p_buf[i + 1] == 0 && p_buf[i + 2] == 1) {
                tmp = (p_buf[i + 3] & 0x7E) >> 1;
                if (tmp == 32 || tmp == 33 || tmp == 34 || tmp == 39
                    || ((tmp >= 0 && tmp <= 21) && ((p_buf[i + 5] & 0x80) == 0x80))) {
                    find_end = VS_TRUE;
                    break;
                }
            }
        }

        if(i > 0) {
            *p_read_len = p_buf[i - 1] == 0 ? i - 1 : i;
        }
        if (find_end == VS_FALSE) {
            *p_read_len = i + 6;
        }
        if (find_start == VS_TRUE && find_end == VS_TRUE) {
            find_frame = VS_TRUE;
        }
    } else if (p_vdec_thread_param->decode_type == E_PT_TYPE_JPEG || p_vdec_thread_param->decode_type == E_PT_TYPE_MJPEG) {
        if (len < 3) {
            return VS_FALSE;
        }

        for (i = 0; i < len - 1; i++) {
            if (p_buf[i] == 0xFF && p_buf[i + 1] == 0xD8) {
                find_start = VS_TRUE;
                i += 2;
                break;
            }
        }

        for (; i < len - 3; i++) {
            if ((p_buf[i] == 0xFF) && (p_buf[i +  1] & 0xF0) == 0xE0) {
                jpeg_len = (p_buf[i + 2] << 8) + p_buf[i + 3];
                i += 1 + jpeg_len;
            } else {
                break;
            }
        }

        for (; i < len - 1; i++)
        {
            if (p_buf[i] == 0xFF && p_buf[i + 1] == 0xD9)
            {
                find_end = VS_TRUE;
                break;
            }
        }
        *p_read_len = i + 2;

        if (find_start == VS_TRUE && find_end == VS_TRUE)
        {
            //vs_sample_trace("chn %d find jpeg frame start and end! s32ReadLen[%u] len[%u].!\n", p_vdec_thread_param->vdec_chnid, *p_read_len, len);
            find_frame = VS_TRUE;
        }
    }else {
        find_frame = VS_FALSE;
    }

    if (find_frame == VS_FALSE) {
        //vs_sample_trace("chn[%d] type[%d] not find one frame s32ReadLen[%u] len[%u]!\n", p_vdec_thread_param->vdec_chnid, p_vdec_thread_param->decode_type, *p_read_len, len);
    }

    if (p_vdec_thread_param->ltrp_info.ltrp_dec_enable == VS_TRUE && is_i_frame == VS_TRUE) {
        p_vdec_thread_param->ltrp_info.frame_cnt = 0;
    }

    return find_frame;
}

vs_int32_t sample_common_vdec_segment_mode_send(sample_vdec_thread_param_s *p_vdec_thread_param, vs_vdec_stream_s *p_stream)
{
    vs_uint32_t send_time = 0, i = 0, send_byte = 0, send_len = 0;
    vs_vdec_stream_s stream = *p_stream;

    send_time = 6;
    send_len = p_stream->len / send_time;

    for (i = 0; i < send_time; i++) {
        if (i != send_time - 1) {
            stream.is_frame_end = VS_FALSE;
            stream.len = send_len;
            stream.p_virt_addr = p_stream->p_virt_addr + send_byte;

            vs_mal_vdec_stream_send(p_vdec_thread_param->vdec_chnid, &stream, -1);

            send_byte += send_len;
        } else {
            stream.is_frame_end = VS_TRUE;
            stream.len = p_stream->len - send_byte;
            stream.p_virt_addr = p_stream->p_virt_addr + send_byte;

            vs_mal_vdec_stream_send(p_vdec_thread_param->vdec_chnid, &stream, -1);
        }
    }
    return VS_SUCCESS;
}

vs_int32_t sample_common_vdec_stream_send(sample_vdec_thread_param_s *p_vdec_thread_param,
                                                        vs_uint8_t *p_buf, vs_uint32_t len)
{
    vs_int32_t ret = VS_SUCCESS;
    vs_vdec_stream_s stream = {0};

    if (p_vdec_thread_param->ltrp_info.ltrp_dec_enable == VS_TRUE) {
        /* olny send i/lp frame, drop other frame */
        if (p_vdec_thread_param->ltrp_info.frame_cnt != 0
            && (p_vdec_thread_param->ltrp_info.frame_cnt % p_vdec_thread_param->ltrp_info.interval_lp) != 0) {
            p_vdec_thread_param->ltrp_info.frame_cnt++;
            return VS_SUCCESS;
        }
    }

    stream.p_virt_addr = p_buf;
    stream.len = len;
    stream.is_frame_end = VS_FALSE;
    stream.is_stream_end = VS_FALSE;
    stream.is_display = VS_TRUE;
    if(p_vdec_thread_param->input_mode == E_VDEC_INPUT_MODE_CHUNK) {
        ret = sample_common_vdec_segment_mode_send(p_vdec_thread_param, &stream);
    } else {
        ret = vs_mal_vdec_stream_send(p_vdec_thread_param->vdec_chnid, &stream, p_vdec_thread_param->send_timeout_ms);
    }

    if (ret == VS_SUCCESS && p_vdec_thread_param->ltrp_info.ltrp_dec_enable == VS_TRUE) {
        p_vdec_thread_param->ltrp_info.frame_cnt++;
    }

    return ret;
}

vs_void_t *sample_common_vdec_stream_send_task(vs_void_t *arg)
{
    sample_vdec_thread_param_s *p_vdec_thread_param = (sample_vdec_thread_param_s *)arg;
    vs_int32_t ret = 0;
    vs_uint32_t read_offset = 0, read_len = 0;
    long file_len = 0;
    FILE *p_file;
    vs_uint32_t mini_buf_size = (p_vdec_thread_param->width * p_vdec_thread_param->height * 3) >> 1;
    vs_uint8_t *p_buf = NULL;
    vs_vdec_stream_s stream = {0};
    vs_bool_t find_frame = VS_FALSE;
    vs_uint32_t send_frame_num = 0;
    vs_uint32_t buf_offset = 0;
    vs_uint32_t buf_len = read_len;

    vs_sample_trace("[chn=%d] default_stream_send_task start !\n", p_vdec_thread_param->vdec_chnid);
    p_file = fopen((char *)p_vdec_thread_param->input_file, "rb");
    if (p_file == NULL) {
        vs_sample_trace("[chn=%d] fopen file *%s* error !\n", p_vdec_thread_param->vdec_chnid, p_vdec_thread_param->input_file);
        return NULL;
    }
    fseek(p_file,0L,SEEK_END);
    file_len = ftell(p_file);
    if (file_len <= 0) {
        vs_sample_trace("[chn=%d] file[%s] file_len[%lu] error !\n", p_vdec_thread_param->vdec_chnid, p_vdec_thread_param->input_file, file_len);
        return NULL;
    }
    fseek(p_file, 0, SEEK_SET);
    p_buf =  (vs_uint8_t *)malloc(mini_buf_size);
    if (p_buf == NULL) {
        vs_sample_trace("malloc read databuf error, size[%u] \n", mini_buf_size);
        return NULL;
    }

    while (!p_vdec_thread_param->stop_send_task) {
        fseek(p_file, read_offset, SEEK_SET);
        read_len = fread(p_buf, 1, mini_buf_size, p_file);
        if (read_len == 0) {
            if (!p_vdec_thread_param->is_circle_send) {
                break;
            } else {
                read_offset = 0;
                continue;
            }
        }

        if ((p_vdec_thread_param->input_mode == E_VDEC_INPUT_MODE_FRAME)
            || (p_vdec_thread_param->input_mode == E_VDEC_INPUT_MODE_CHUNK)) {
            buf_offset = 0;
            buf_len = read_len;

            while (!p_vdec_thread_param->stop_send_task) {
                find_frame = sample_common_vdec_find_one_frame(p_vdec_thread_param, p_buf + buf_offset,
                    buf_len - buf_offset, &read_len);
                if (find_frame == VS_FALSE) {
                    if (buf_len < mini_buf_size) {
                        if (p_vdec_thread_param->decode_type == E_PT_TYPE_H264 ||
                            p_vdec_thread_param->decode_type == E_PT_TYPE_H265) {

                            ret = sample_common_vdec_stream_send(p_vdec_thread_param, p_buf + buf_offset, buf_len - buf_offset);
                            if (ret == VS_SUCCESS) {
                                buf_offset = buf_len;
                                send_frame_num++;
                            } else {
                                continue;
                            }
                        } else if (p_vdec_thread_param->decode_type == E_PT_TYPE_MJPEG || p_vdec_thread_param->decode_type == E_PT_TYPE_JPEG) {
                            buf_offset = buf_len;
                        }
                    }
                    read_offset += buf_offset;
                    break;
                }

                ret = sample_common_vdec_stream_send(p_vdec_thread_param, p_buf + buf_offset, read_len);
                if (ret == VS_SUCCESS) {
                    buf_offset += read_len;
                    send_frame_num++;
                }
            }
        } else {
            ret = sample_common_vdec_stream_send(p_vdec_thread_param, p_buf, read_len);
            if (ret == VS_SUCCESS) {
                read_offset += read_len;
                send_frame_num++;
            }
        }
    }

    /*send the end stream package*/
    memset(&stream, 0, sizeof(vs_vdec_stream_s));
    stream.is_stream_end = VS_TRUE;
    ret = vs_mal_vdec_stream_send(p_vdec_thread_param->vdec_chnid, &stream, 6*1000);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("chn[%d] read_len = 0 in vs_mal_vdec_stream_send, ret[0x%x]\n", p_vdec_thread_param->vdec_chnid, ret);
    }
    if (p_buf != VS_NULL) {
        free(p_buf);
        p_buf = VS_NULL;
    }
    if (p_file != VS_NULL) {
        fclose(p_file);
        p_file = VS_NULL;
    }
    vs_sample_trace("chn[%d] default_stream_send_task sendok times[%d]\n", p_vdec_thread_param->vdec_chnid, send_frame_num);

    return NULL;
}

vs_int32_t sample_common_vdec_stream_send_start(vs_int32_t vdec_chnnum, sample_vdec_thread_param_s *p_vdec_thread_param)
{
    vs_int32_t i = 0;

    for (i = 0; i < vdec_chnnum; i++) {
        pthread_create(&p_vdec_thread_param[i].tid_send, NULL, sample_common_vdec_stream_send_task, (vs_void_t *)&p_vdec_thread_param[i]);
    }

    return VS_SUCCESS;
}

vs_int32_t sample_common_vdec_stream_send_stop(vs_int32_t vdec_chnnum, sample_vdec_thread_param_s *p_vdec_thread_param)
{
    vs_int32_t i = 0;

    for (i = 0; i < vdec_chnnum; i++) {
        p_vdec_thread_param[i].stop_send_task = VS_TRUE;
        vs_mal_vdec_chn_stop(p_vdec_thread_param[i].vdec_chnid);
        if (p_vdec_thread_param[i].tid_send != 0) {
            pthread_join(p_vdec_thread_param[i].tid_send, NULL);
            p_vdec_thread_param[i].tid_send = 0;
        }
    }

    return VS_SUCCESS;
}

vs_void_t sample_common_vdec_cmdrecv(vs_void_t)
{
    printf("=====Enter any key to continue=====\n");
    getchar();
}


#ifdef __cplusplus
}
#endif
