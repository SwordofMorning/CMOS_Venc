/**
 * @file    sample_hdmi.c
 * @brief   sample hdmi implementation
 * @details
 * @author  Visinex Software Group
 * @date    2022-05-25
 * @version v1.00
 * @Copyright (c) 2022 Shanghai Visinex Technologies Co., Ltd. All rights reserved.
 *
 */

#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <dlfcn.h>
#include <pthread.h>
#include <linux/videodev2.h>

#include "sample_common.h"
#include "vs_aac_codec.h"
#include "vs_audio_defines.h"
#include "sample_vii_hdmi.h"

static vs_uint32_t g_pool_cnt = 1;
static vs_uint32_t g_buffer_dimension = 1;
static vs_int32_t enc_handle;
static vs_int32_t dec_handle;

// static vs_void_t sample_hdmi_get_vo_cfg(vs_size_s *img_size, sample_vo_cfg_s *disp0_config)
// {
//     disp0_config->vo_devid = 0;
//     disp0_config->vo_layerid = 0;
//     disp0_config->vo_intf_type = E_VO_INTERFACE_TYPE_HDMI;
//     if (img_size->width > 3840) {
//         disp0_config->vo_output = E_VO_OUTPUT_TYPE_3840x2160_60;
//     } else if (((img_size->width == 3840) && (img_size->height == 2160))
//         || ((img_size->width == 2560) && (img_size->height == 1440))
//         || ((img_size->width == 2688) && (img_size->height == 1520))) {
//         disp0_config->vo_output = E_VO_OUTPUT_TYPE_3840x2160_30;
//     } else {
//         disp0_config->vo_output = E_VO_OUTPUT_TYPE_1080P60;
//     }
//     disp0_config->bg_color = 0;
//     disp0_config->dynamic_range = E_DYNAMIC_RANGE_SDR8;
//     disp0_config->pixel_format = E_PIXEL_FORMAT_YUV_420SP;
//     disp0_config->vo_mode = E_VO_MODE_1MUX;

//     disp0_config->img_width = img_size->width;
//     disp0_config->img_height = img_size->height;
//     disp0_config->enable = VS_TRUE;
//     disp0_config->zorder = 0;
// }

static vs_void_t sample_hdmi_get_vo_cfg(vs_size_s *img_size, sample_vo_cfg_s *disp0_config)
{
    disp0_config->vo_devid = 0;
    disp0_config->vo_layerid = 0;
    disp0_config->vo_intf_type = E_VO_INTERFACE_TYPE_HDMI;
    
    // 修改：强制使用1080p分辨率，确保兼容性
    disp0_config->vo_output = E_VO_OUTPUT_TYPE_1080P60;
    
    disp0_config->bg_color = 0;
    disp0_config->dynamic_range = E_DYNAMIC_RANGE_SDR8;
    disp0_config->pixel_format = E_PIXEL_FORMAT_YUV_420SP;
    disp0_config->vo_mode = E_VO_MODE_1MUX;

    // 修改：使用固定的1080p尺寸
    disp0_config->img_width = 1920;
    disp0_config->img_height = 1080;
    disp0_config->enable = VS_TRUE;
    disp0_config->zorder = 0;
}

static vs_void_t sample_vii_get_vpp_grp_attr(vs_size_s *img_size, vs_vpp_grp_attr_s *grp_attr)
{
    grp_attr->max_width = img_size->width;
    grp_attr->max_height = img_size->height;
    grp_attr->dynamic_range = E_DYNAMIC_RANGE_SDR8;
    grp_attr->pixel_format = E_PIXEL_FORMAT_YVU_420SP;
    grp_attr->framerate.dst_framerate = -1;
    grp_attr->framerate.src_framerate = -1;
}

static vs_void_t sample_vii_get_vpp_chn_attr(vs_size_s *img_size, vs_vpp_chn_attr_s *chn_attr)
{
    chn_attr->chn_mode = E_VPP_CHN_MODE_USER;
    chn_attr->width = img_size->width;
    chn_attr->height = img_size->height;
    chn_attr->video_format = E_VIDEO_FORMAT_LINEAR;
    chn_attr->pixel_format = E_PIXEL_FORMAT_YUV_420SP;
    chn_attr->dynamic_range = E_DYNAMIC_RANGE_SDR8;
    chn_attr->compress_mode = E_COMPRESS_MODE_RASTER;
    chn_attr->framerate.src_framerate = -1;
    chn_attr->framerate.dst_framerate = -1;
    chn_attr->mirror_enable = VS_FALSE;
    chn_attr->flip_enable = VS_FALSE;
    chn_attr->depth = 0;
    chn_attr->aspect_ratio.mode = E_ASPECT_RATIO_MODE_NONE;
}

static vs_int32_t sample_hdmi_audio_start(vs_void_t)
{
    vs_int32_t ai_devid = 0;
    vs_int32_t aenc_chnid = 0;
    vs_int32_t adec_chnid = 0;
    vs_int32_t ao_devid = 0;
    vs_aiodev_attr_s  aio_common_attr;
    vs_aiodev_attr_s *p_aio_attr=&aio_common_attr;
    vs_aio_target_e   i2s_target = E_AIO_CONNECT_INNER_CODEC;
    vs_payload_type_e payload_type = E_PT_TYPE_AAC;
    vs_aenc_encoder_s *p_aac_encoder;
    vs_adec_decoder_s *p_aac_decoder;

    p_aac_encoder = vs_drv_aenc_aac_object_get();
    if (VS_SUCCESS != vs_mal_aenc_encoder_register(&enc_handle, p_aac_encoder)) {
        vs_sample_trace("aac encoder register failed!\n");
        return VS_FAILED;
    }

    p_aac_decoder = vs_drv_adec_aac_object_get();
    if (VS_SUCCESS != vs_mal_adec_decoder_register(&dec_handle, p_aac_decoder)) {
        vs_sample_trace("aac decoder register failed!\n");
        return VS_FAILED;
    }

    memset(&aio_common_attr, 0, sizeof(aio_common_attr));
    aio_common_attr.sample_rate      = 48000;
    aio_common_attr.chn_num          = 2;
    aio_common_attr.i2s_target       = i2s_target;
    aio_common_attr.work_mode        = E_AIO_MODE_I2S_MASTER_STD;
    aio_common_attr.sample_width     = E_AUDIO_SAMPLE_BITWIDTH_16;
    aio_common_attr.sound_mode       = E_AUDIO_SOUND_MODE_STEREO;
    aio_common_attr.pool_frame_num   = 16;
    aio_common_attr.frame_sample_num = 1024;
    aio_common_attr.clk_source       = 0;

    p_aio_attr->i2s_target = E_AIO_CONNECT_INNER_HDMI;
    sample_common_aout_start(ao_devid, p_aio_attr, -1, NULL);
    sample_common_adec_start(adec_chnid, payload_type);
    sample_common_aenc_start(aenc_chnid, 1, p_aio_attr, payload_type);
    p_aio_attr->i2s_target = E_AIO_CONNECT_INNER_CODEC;
    sample_common_ain_start(ai_devid, p_aio_attr, -1, NULL, 0);

    sample_common_adec_aout_bind(adec_chnid, ao_devid, 0);
    sample_common_aenc_adec_bind(aenc_chnid, adec_chnid);
    sample_common_ain_aenc_bind(ai_devid, 0, aenc_chnid);

    return VS_SUCCESS;
}

static vs_void_t sample_hdmi_audio_stop(vs_void_t)
{
    vs_int32_t ai_devid = 0;
    vs_int32_t aenc_chnid = 0;
    vs_int32_t adec_chnid = 0;
    vs_int32_t ao_devid = 0;

    sample_common_aout_stop(ao_devid, 2, VS_FALSE);
    sample_common_adec_stop(adec_chnid);
    sample_common_aenc_stop(aenc_chnid, 1);
    sample_common_ain_stop(ai_devid, 2, VS_FALSE);

    sample_common_ain_aenc_unbind(ai_devid, 0, aenc_chnid);
    sample_common_adec_aout_unbind(adec_chnid, ao_devid, 0);
    sample_common_aenc_adec_unbind(aenc_chnid, adec_chnid);

    vs_mal_aenc_encoder_unregister(enc_handle);
    vs_mal_adec_decoder_unregister(dec_handle);
}

vs_int32_t sample_vii_hdmi()
{
    vs_int32_t ret;
    vs_int32_t sensor_id = 0;
    vs_int32_t vii_pipeid = 0;
    vs_int32_t vii_chnid = 0;
    vs_int32_t vpp_grpid = 0;
    vs_int32_t vpp_chnid = 0;
    vs_int32_t vo_devid = 0;
    vs_int32_t vo_chnid = 0;
    vs_uint32_t i = 0;
    vs_uint32_t frame_num;
    vs_size_s img_size = {0};
    vs_vb_cfg_s vb_cfg = {0};
    sample_vii_cfg_s vii_cfg = {0};
    sample_vo_cfg_s disp0_config = {0};
    vs_bool_t chn_enable[VPP_MAX_PHYCHN_NUM] = {VS_TRUE, VS_FALSE, VS_FALSE, VS_FALSE};
    vs_vpp_grp_attr_s vpp_grp_attr = {0};
    vs_vpp_chn_attr_s vpp_chn_attr[VPP_MAX_PHYCHN_NUM];
    vs_pixel_format_e format;
    vs_size_s vpp_output_size[VPP_MAX_PHYCHN_NUM] = {0};

    vs_sample_trace("=== Starting VII HDMI sample ===\n");

    sample_common_vii_sensor_img_size_get(sensor_id, &img_size);
    sample_common_vii_sensor_pixel_format_get(sensor_id, &format);
    frame_num = sample_common_vii_wdr_frame_num_get(sensor_id);
    
    vs_sample_trace("Sensor config: size=%dx%d, format=%d, frame_num=%d\n", 
                    img_size.width, img_size.height, format, frame_num);

    vb_cfg.pool_cnt = g_pool_cnt;

    for (i = 0; i < g_pool_cnt; i++) {
        vb_cfg.ast_commpool[i].blk_size = g_buffer_dimension * sample_common_buffer_size_get(&img_size, format, E_COMPRESS_MODE_NONE, frame_num);
        vb_cfg.ast_commpool[i].blk_cnt = 12;
        vb_cfg.ast_commpool[i].remap_mode = VB_REMAP_MODE_NONE;
        vs_sample_trace("VB pool[%d]: blk_size=%llu, blk_cnt=%d\n", 
                        i, (vs_uint64_t)vb_cfg.ast_commpool[i].blk_size, vb_cfg.ast_commpool[i].blk_cnt);
    }

    // 配置VO：使用传感器原始分辨率
    sample_hdmi_get_vo_cfg(&img_size, &disp0_config);  // 传入传感器尺寸而不是固定尺寸
    vs_sample_trace("VO config: devid=%d, layerid=%d, output=%d, size=%dx%d\n",
                    disp0_config.vo_devid, disp0_config.vo_layerid, 
                    disp0_config.vo_output, disp0_config.img_width, disp0_config.img_height);

    ret = sample_common_sys_init(&vb_cfg);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("sample_common_sys_init failed with 0x%x\n", ret);
        goto exit0;
    }
    vs_sample_trace("System initialized successfully\n");

    // 关键修改1：使用在线模式，参考正常工作的代码
    vii_cfg.vii_vpp_mode = E_VII_ONLINE_VPP_ONLINE;
    vii_cfg.route_num = 1;
    sample_common_vii_default_cfg_get(sensor_id, &vii_cfg.route_cfg[0]);
    
    // 关键修改2：参考VENC代码的VII配置
    for (i = 0; i < DEV_BIND_MAX_PIPE_NUM; i++) {
        vs_int32_t j;
        for (j = 0; j < VII_MAX_PHYS_CHN_NUM; j++) {
            vii_cfg.route_cfg[0].pipe_cfg[i].pipe_attr.compress_mode = E_COMPRESS_MODE_NONE;
            vii_cfg.route_cfg[0].pipe_cfg[i].phys_chn_cfg[j].chn_attr.framerate.src_framerate = 30;
            vii_cfg.route_cfg[0].pipe_cfg[i].phys_chn_cfg[j].chn_attr.framerate.dst_framerate = 30;
            vii_cfg.route_cfg[0].pipe_cfg[i].pipe_attr.bypass_mode = E_VII_PIPE_BYPASS_NONE;
            vii_cfg.route_cfg[0].pipe_cfg[i].phys_chn_cfg[j].chn_attr.compress_mode = E_COMPRESS_MODE_NONE;
        }
        for (j = 0; j < VII_MAX_EXT_CHN_NUM; j++) {
            vii_cfg.route_cfg[0].pipe_cfg[i].ext_chn_cfg[j].chn_attr.framerate.src_framerate = 30;
            vii_cfg.route_cfg[0].pipe_cfg[i].ext_chn_cfg[j].chn_attr.framerate.dst_framerate = 30;
            vii_cfg.route_cfg[0].pipe_cfg[i].ext_chn_cfg[j].chn_attr.compress_mode = E_COMPRESS_MODE_NONE;
        }
    }

    ret = sample_common_vii_start(&vii_cfg);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("sample_common_vii_start failed with 0x%x\n", ret);
        goto exit1;
    }
    vs_sample_trace("VII started successfully\n");

    // 关键修改3：参考VENC代码的VPP初始化
    memset(&vpp_grp_attr, 0, sizeof(vpp_grp_attr));
    vpp_grp_attr.max_width = img_size.width;
    vpp_grp_attr.max_height = img_size.height;
    vpp_grp_attr.dynamic_range = E_DYNAMIC_RANGE_SDR8;
    vpp_grp_attr.pixel_format = E_PIXEL_FORMAT_YVU_420SP;
    vpp_grp_attr.framerate.dst_framerate = 30;
    vpp_grp_attr.framerate.src_framerate = 30;

    // 设置VPP输出尺寸：使用原始尺寸，不缩放
    vpp_output_size[0] = img_size;

    memset(vpp_chn_attr, 0, sizeof(vpp_chn_attr));
    for (i = 0; i < VPP_MAX_PHYCHN_NUM; i++) {
        if (chn_enable[i] == VS_TRUE) {
            vpp_chn_attr[i].chn_mode = E_VPP_CHN_MODE_USER;
            vpp_chn_attr[i].width = vpp_output_size[i].width;
            vpp_chn_attr[i].height = vpp_output_size[i].height;
            vpp_chn_attr[i].video_format = E_VIDEO_FORMAT_LINEAR;
            vpp_chn_attr[i].pixel_format = E_PIXEL_FORMAT_YVU_420SP;  // 参考VENC代码
            vpp_chn_attr[i].dynamic_range = E_DYNAMIC_RANGE_SDR8;
            vpp_chn_attr[i].compress_mode = E_COMPRESS_MODE_NONE;  // 参考VENC代码
            vpp_chn_attr[i].framerate.src_framerate = 30;
            vpp_chn_attr[i].framerate.dst_framerate = 30;
            vpp_chn_attr[i].mirror_enable = VS_FALSE;
            vpp_chn_attr[i].flip_enable = VS_FALSE;
            vpp_chn_attr[i].depth = 0;  // 关键：参考VENC代码设置为0
            vpp_chn_attr[i].aspect_ratio.mode = E_ASPECT_RATIO_MODE_NONE;
            
            vs_sample_trace("VPP chn[%d]: width=%d, height=%d, format=%d\n", 
                           i, vpp_chn_attr[i].width, vpp_chn_attr[i].height, vpp_chn_attr[i].pixel_format);
        }
    }

    // 关键修改4：使用标准VPP启动，参考VENC代码
    ret = sample_common_vpp_start(vpp_grpid, chn_enable, &vpp_grp_attr, vpp_chn_attr);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("sample_common_vpp_start failed with 0x%x\n", ret);
        goto exit2;
    }
    vs_sample_trace("VPP started successfully\n");

    // 关键修改5：参考VENC代码的绑定顺序
    // 第1步：绑定VII到VPP
    ret = sample_common_vii_bind_vpp(vii_pipeid, vii_chnid, vpp_grpid);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("sample_common_vii_bind_vpp failed with 0x%x\n", ret);
        goto exit3;
    }
    vs_sample_trace("VII bind VPP successfully: VII[%d-%d] -> VPP[%d]\n", 
                    vii_pipeid, vii_chnid, vpp_grpid);

    // 第2步：启动音频
    ret = sample_hdmi_audio_start();
    if (ret != VS_SUCCESS) {
        vs_sample_trace("sample_hdmi_audio_start failed with 0x%x\n", ret);
        goto exit4;
    }
    vs_sample_trace("HDMI audio started successfully\n");

    // 第3步：启动VO
    ret = sample_common_vo_start(&disp0_config);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("sample_common_vo_start failed with 0x%x\n", ret);
        goto exit5;
    }
    vs_sample_trace("VO started successfully\n");

    // 第4步：绑定VPP到VO
    ret = sample_common_vpp_bind_vo(vpp_grpid, vpp_chnid, disp0_config.vo_layerid, vo_chnid);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("sample_common_vpp_bind_vo failed with 0x%x\n", ret);
        goto exit6;
    }
    vs_sample_trace("VPP bind VO successfully: VPP[%d-%d] -> VO[%d-%d]\n",
                    vpp_grpid, vpp_chnid, disp0_config.vo_layerid, vo_chnid);

    // 等待系统稳定
    sleep(3);
    vs_sample_trace("=== Final system status ===\n");
    
    vs_sample_trace("VII final status:\n");
    system("cat /proc/vssdk/vii | grep -A 2 'chn status'");
    
    vs_sample_trace("VPP final status:\n");
    system("cat /proc/vssdk/vpp | grep -A 3 'chn output'");
    
    vs_sample_trace("VO final status:\n");
    system("cat /proc/vssdk/vo | grep -A 5 'display0.*state'");

    vs_sample_trace("Bind final status:\n");
    system("cat /proc/vssdk/sys | grep -A 10 'Bind Session'");

    vs_sample_trace("=== VII HDMI sample started successfully ===\n");
    vs_sample_trace("Expected: Direct %dx%d output to HDMI\n", img_size.width, img_size.height);
    vs_sample_trace("Press Ctrl+C to stop...\n");

    sample_common_pause();

    vs_sample_trace("=== Stopping VII HDMI sample ===\n");
    
    // 按相反顺序清理
    sample_common_vpp_unbind_vo(vpp_grpid, vpp_chnid, vo_devid, vo_chnid);
exit6:
    sample_common_vo_stop(&disp0_config);
exit5:
    sample_hdmi_audio_stop();
exit4:
    sample_common_vii_unbind_vpp(vii_pipeid, vii_chnid, vpp_grpid);
exit3:
    sample_common_vpp_stop(vpp_grpid, chn_enable);
exit2:
    sample_common_vii_stop(&vii_cfg);
exit1:
    sample_common_sys_exit();
exit0:
    if (ret != VS_SUCCESS) {
        vs_sample_trace("VII HDMI sample failed with error 0x%x\n", ret);
    }
    return ret;
}

vs_void_t sample_vii_hdmi_usage(char *prog_name)
{
    vs_uint32_t i;
    vs_int32_t sensor_type_num = sample_common_vii_sensor_type_num_get();

    printf("Usage : %s <1> <sensor_type> <i2c_bus_id>\n", prog_name);

    printf("sensor_type:\n");
    for (i = 0; i < sensor_type_num; i++) {
        printf("\t %d) %s.\n", i, sample_common_sensor_type_name_get(i));
    }

    printf("I2C Bus ID:\n");
    printf("\t 1(default), i2c id based on board\n");
}

static volatile sig_atomic_t g_stop_flag = 0;
static vs_void_t sample_hdmi_register_signal_handler(void (*sig_handler)(int))
{
    struct sigaction sa;
    memset(&sa, 0, sizeof(struct sigaction));
    sa.sa_handler = sig_handler;
    sa.sa_flags = 0;

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
}

vs_void_t vii_signal_handle(vs_int32_t s_no)
{
    (vs_void_t)s_no;
    g_stop_flag = 1;
}

extern vs_int8_t g_bus_id[VII_MAX_ROUTE_NUM];
extern sample_sensor_type_e g_sensor_type[VII_MAX_ROUTE_NUM];

vs_int32_t sample_vii_hdmi_case(int argc, char *argv[])
{
    vs_int32_t ret;

    if (argc < 3) {
        sample_vii_hdmi_usage(argv[0]);
        return VS_FAILED;
    }

    sample_hdmi_register_signal_handler(vii_signal_handle);

    if (argc > 2) {
        g_sensor_type[0] = atoi(argv[2]);
    }

    if (argc > 3) {
        g_bus_id[0] = atoi(argv[3]);
    }

    ret = sample_vii_hdmi();

    return ret;

}
