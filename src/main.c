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
 
 #include "sample_hdmi_path.h"
 #include "sample_vii_hdmi.h"
 
 #define HDMI_CMD_LEN 128
 #define sample_hdmi_get_input_cmd(input_cmd) fgets((char *)(input_cmd), (sizeof(input_cmd) - 1), stdin)
 
 static volatile sig_atomic_t g_stop_flag = 0;
 vs_bool_t g_audio_start = VS_FALSE;
 
 char *sample_hdmi_video_format_name_get(vs_hdmi_video_format_e video_format)
 {
     switch (video_format) {
     case E_HDMI_VIDEO_FORMAT_1080P_60:
         return "E_HDMI_VIDEO_FORMAT_1080P_60";
     case E_HDMI_VIDEO_FORMAT_1080P_50:
         return "E_HDMI_VIDEO_FORMAT_1080P_50";
     case E_HDMI_VIDEO_FORMAT_1080P_30:
         return "E_HDMI_VIDEO_FORMAT_1080P_30";
     case E_HDMI_VIDEO_FORMAT_1080P_25:
         return "E_HDMI_VIDEO_FORMAT_1080P_25";
     case E_HDMI_VIDEO_FORMAT_1080P_24:
         return "E_HDMI_VIDEO_FORMAT_1080P_24";
     case E_HDMI_VIDEO_FORMAT_1080I_60:
         return "E_HDMI_VIDEO_FORMAT_1080I_60";
     case E_HDMI_VIDEO_FORMAT_1080I_50:
         return "E_HDMI_VIDEO_FORMAT_1080I_50";
     case E_HDMI_VIDEO_FORMAT_720P_60:
         return "E_HDMI_VIDEO_FORMAT_720P_60";
     case E_HDMI_VIDEO_FORMAT_720P_50:
         return "E_HDMI_VIDEO_FORMAT_720P_50";
     case E_HDMI_VIDEO_FORMAT_576P_50:
         return "E_HDMI_VIDEO_FORMAT_576P_50";
     case E_HDMI_VIDEO_FORMAT_480P_60:
         return "E_HDMI_VIDEO_FORMAT_480P_60";
     case E_HDMI_VIDEO_FORMAT_640x480_60:
         return "E_HDMI_VIDEO_FORMAT_640x480_60";
     case E_HDMI_VIDEO_FORMAT_720x1280_60:
         return "E_HDMI_VIDEO_FORMAT_720x1280_60";
     case E_HDMI_VIDEO_FORMAT_800x600_50:
         return "E_HDMI_VIDEO_FORMAT_800x600_50";
     case E_HDMI_VIDEO_FORMAT_800x600_60:
         return "E_HDMI_VIDEO_FORMAT_800x600_60";
     case E_HDMI_VIDEO_FORMAT_1024x768_60:
         return "E_HDMI_VIDEO_FORMAT_1024x768_60";
     case E_HDMI_VIDEO_FORMAT_1080x1920_60:
         return "E_HDMI_VIDEO_FORMAT_1080x1920_60";
     case E_HDMI_VIDEO_FORMAT_1280x800_60:
         return "E_HDMI_VIDEO_FORMAT_1280x800_60";
     case E_HDMI_VIDEO_FORMAT_1280x1024_60:
         return "E_HDMI_VIDEO_FORMAT_1280x1024_60";
     case E_HDMI_VIDEO_FORMAT_1366x768_60:
         return "E_HDMI_VIDEO_FORMAT_1366x768_60";
     case E_HDMI_VIDEO_FORMAT_1440x900_60:
         return "E_HDMI_VIDEO_FORMAT_1440x900_60";
     case E_HDMI_VIDEO_FORMAT_1600x1200_60:
         return "E_HDMI_VIDEO_FORMAT_1600x1200_60";
     case E_HDMI_VIDEO_FORMAT_1680x1050_60:
         return "E_HDMI_VIDEO_FORMAT_1680x1050_60";
     case E_HDMI_VIDEO_FORMAT_1920x1200_60:
         return "E_HDMI_VIDEO_FORMAT_1920x1200_60";
     case E_HDMI_VIDEO_FORMAT_2560x1440_30:
         return "E_HDMI_VIDEO_FORMAT_2560x1440_30";
     case E_HDMI_VIDEO_FORMAT_2560x1440_60:
         return "E_HDMI_VIDEO_FORMAT_2560x1440_60";
     case E_HDMI_VIDEO_FORMAT_2560x1600_60:
         return "E_HDMI_VIDEO_FORMAT_2560x1600_60";
     case E_HDMI_VIDEO_FORMAT_1920x2160_30:
         return "E_HDMI_VIDEO_FORMAT_1920x2160_30";
     case E_HDMI_VIDEO_FORMAT_3840x2160P_24:
         return "E_HDMI_VIDEO_FORMAT_3840x2160P_24";
     case E_HDMI_VIDEO_FORMAT_3840x2160P_25:
         return "E_HDMI_VIDEO_FORMAT_3840x2160P_25";
     case E_HDMI_VIDEO_FORMAT_3840x2160P_30:
         return "E_HDMI_VIDEO_FORMAT_3840x2160P_30";
     case E_HDMI_VIDEO_FORMAT_3840x2160P_50:
         return "E_HDMI_VIDEO_FORMAT_3840x2160P_50";
     case E_HDMI_VIDEO_FORMAT_3840x2160P_60:
         return "E_HDMI_VIDEO_FORMAT_3840x2160P_60";
     case E_HDMI_VIDEO_FORMAT_4096x2160P_24:
         return "E_HDMI_VIDEO_FORMAT_4096x2160P_24";
     case E_HDMI_VIDEO_FORMAT_4096x2160P_25:
         return "E_HDMI_VIDEO_FORMAT_4096x2160P_25";
     case E_HDMI_VIDEO_FORMAT_4096x2160P_30:
         return "E_HDMI_VIDEO_FORMAT_4096x2160P_30";
     case E_HDMI_VIDEO_FORMAT_4096x2160P_50:
         return "E_HDMI_VIDEO_FORMAT_4096x2160P_50";
     case E_HDMI_VIDEO_FORMAT_4096x2160P_60:
         return "E_HDMI_VIDEO_FORMAT_4096x2160P_60";
     default:
         return "NA";
     }
 }
 
 vs_void_t sample_hdmi_vdec_usage(vs_void_t)
 {
     printf("\n\n usage: please select hdmi configuration below\n");
     printf("\t 0) hdmi_hdmi_force      force to hdmi output\n");
     printf("\t 1) hdmi_auto_select     select hdmi or dvi according to edid\n");
     printf("\t 2) hdmi_video_timing    set video output timing format\n");
     printf("\t 3) hdmi_video_mode      set video color output(RGB/YUV)\n");
     printf("\t 4) hdmi_audio_freq      set audio output frequence\n");
     printf("\t 5) hdmi_auth_mode       auth mode enable or disable\n");
 
     return;
 }
 
 vs_void_t sample_hdmi_video_mode_usage(vs_void_t)
 {
     printf("\n\n please select video mode\n");
     printf("\t 0) RGB\n");
     printf("\t 1) YUV444\n");
     printf("\t 2) YUV422\n");
 
     return;
 }
 
 vs_void_t sample_hdmi_video_format_usage(vs_void_t)
 {
     vs_int32_t i;
 
     printf("\n\n please select video format\n");
     for (i = 0; i < E_HDMI_VIDEO_FORMAT_CUSTOMER_DEFINE; i++) {
         printf("\t %d) %s\n", i, sample_hdmi_video_format_name_get(i));
     }
 }
 
 vs_void_t sample_hdmi_audio_freq_usage(vs_void_t)
 {
     printf("\n\n please select audio frequency\n");
     printf("\t 0) 32000\n");
     printf("\t 1) 44100\n");
     printf("\t 2) 48000\n");
 }
 
 vs_void_t sample_hdmi_auth_mode_usage(vs_void_t)
 {
     printf("\n\n please select auth mode frequency\n");
     printf("\t 0) disable auth mode\n");
     printf("\t 1) enable auth mode\n");
 }
 
 vs_void_t sample_hdmi_get_input_int_value(vs_int32_t *p_input_value)
 {
     char *line = NULL;
     size_t len = 0;
     ssize_t read;
 
     *p_input_value = -1;
     if ((read = getline(&line, &len, stdin)) != -1) {
         if (read > 0) {
             if (atoi(line) != 0) {
                 *p_input_value = atoi(line);
             } else {
                 if (line[0] == '0') {
                     *p_input_value = 0;
                 } else {
                     printf("input line[0]=%d \n", line[0]);
                 }
             }
         }
     } else {
         clearerr(stdin);
     }
     if (line)
         free(line);
     return;
 }
 
 vs_hdmi_video_mode_e sample_hdmi_video_mode_param_get(vs_void_t)
 {
     vs_int32_t input_value = -1;
 
     do {
         if (g_stop_flag == 1) {
             return VS_FAILED;
         }
 
         sample_hdmi_video_mode_usage();
         sample_hdmi_get_input_int_value(&input_value);
         if (input_value == 0) {
             printf("video mode select succ!\n\n\n");
             return E_HDMI_VIDEO_MODE_RGB444;
         } else if (input_value == 1) {
             printf("video mode select succ!\n\n\n");
             return E_HDMI_VIDEO_MODE_YCBCR444;
         } else if (input_value == 2) {
             printf("video mode select succ!\n\n\n");
             return E_HDMI_VIDEO_MODE_YCBCR422;
         } else {
             printf("input cmd: [%d], not support!\n", input_value);
             continue;
         }
     } while (1);
 }
 
 vs_hdmi_video_format_e sample_hdmi_video_format_param_get(vs_void_t)
 {
     vs_int32_t input_value = -1;
 
     do {
         if (g_stop_flag == 1) {
             return VS_FAILED;
         }
 
         sample_hdmi_video_format_usage();
         sample_hdmi_get_input_int_value(&input_value);
 
         if (input_value >= E_HDMI_VIDEO_FORMAT_CUSTOMER_DEFINE || input_value < 0) {
             printf("input cmd: [%d], not support!\n", input_value);
             continue;
         }
 
         printf("video format select succ!\n\n\n");
         return input_value;
     } while (1);
 }
 
 vs_int32_t sample_hdmi_audio_freq_param_get(vs_void_t)
 {
     vs_int32_t input_value = -1;
 
     do {
         if (g_stop_flag == 1) {
             return VS_FAILED;
         }
 
         sample_hdmi_audio_freq_usage();
         sample_hdmi_get_input_int_value(&input_value);
 
         if (input_value < 0 || input_value > 3) {
             printf("input cmd: [%d], not support!\n", input_value);
             continue;
         }
 
         printf("audio freq select succ!\n\n\n");
         return input_value;
     } while (1);
 }
 
 vs_int32_t sample_hdmi_auth_mode_param_get(vs_void_t)
 {
     vs_int32_t input_value = -1;
 
     do {
         if (g_stop_flag == 1) {
             return VS_FAILED;
         }
 
         sample_hdmi_auth_mode_usage();
         sample_hdmi_get_input_int_value(&input_value);
 
         if (input_value < 0 || input_value > 1) {
             printf("input cmd: [%d], not support!\n", input_value);
             continue;
         }
 
         printf("auth mode select succ!\n\n\n");
         return input_value;
     } while (1);
 }
 
 vs_int32_t sample_hdmi_hdmi_force(vs_void_t)
 {
     vs_int32_t ret;
     vs_hdmi_attr_s hdmi_attr;
     ret = vs_mal_hdmi_stop(0);
     if (ret != VS_SUCCESS) {
         vs_sample_trace("vs_mal_hdmi_stop failed with 0x%x\n", ret);
         return VS_FAILED;
     }
 
     ret = vs_mal_hdmi_attr_get(0, &hdmi_attr);
     if (ret != VS_SUCCESS) {
         vs_sample_trace("vs_mal_hdmi_attr_get failed with 0x%x\n", ret);
         return VS_FAILED;
     }
 
     hdmi_attr.hdmi_enable = VS_TRUE;
     hdmi_attr.audio_enable = VS_TRUE;
 
     ret = vs_mal_hdmi_attr_set(0, &hdmi_attr);
     if (ret != VS_SUCCESS) {
         vs_sample_trace("vs_mal_hdmi_attr_set failed with 0x%x\n", ret);
         return VS_FAILED;
     }
 
     ret = vs_mal_hdmi_start(0);
     if (ret != VS_SUCCESS) {
         vs_sample_trace("vs_mal_hdmi_start failed with 0x%x\n", ret);
         return VS_FAILED;
     }
 
     return VS_SUCCESS;
 }
 
 vs_int32_t sample_hdmi_auto_select(vs_void_t)
 {
     vs_int32_t ret;
     vs_hdmi_attr_s hdmi_attr;
 
     ret = vs_mal_hdmi_stop(0);
     if (ret != VS_SUCCESS) {
         vs_sample_trace("vs_mal_hdmi_stop failed with 0x%x\n", ret);
         return VS_FAILED;
     }
 
     ret = vs_mal_hdmi_attr_get(0, &hdmi_attr);
     if (ret != VS_SUCCESS) {
         vs_sample_trace("vs_mal_hdmi_attr_get failed with 0x%x\n", ret);
         return VS_FAILED;
     }
 
     hdmi_attr.hdmi_enable = VS_FALSE;
 
     ret = vs_mal_hdmi_attr_set(0, &hdmi_attr);
     if (ret != VS_SUCCESS) {
         vs_sample_trace("vs_mal_hdmi_attr_set failed with 0x%x\n", ret);
         return VS_FAILED;
     }
 
     ret = vs_mal_hdmi_start(0);
     if (ret != VS_SUCCESS) {
         vs_sample_trace("vs_mal_hdmi_start failed with 0x%x\n", ret);
         return VS_FAILED;
     }
 
     return VS_SUCCESS;
 }
 
 vs_int32_t sample_hdmi_video_timing(vs_void_t)
 {
     vs_int32_t ret;
     vs_hdmi_attr_s hdmi_attr;
     vs_hdmi_video_format_e video_format;
 
     video_format = sample_hdmi_video_format_param_get();
 
     ret = vs_mal_hdmi_stop(0);
     if (ret != VS_SUCCESS) {
         vs_sample_trace("vs_mal_hdmi_stop failed with 0x%x\n", ret);
         return VS_FAILED;
     }
 
     ret = vs_mal_hdmi_attr_get(0, &hdmi_attr);
     if (ret != VS_SUCCESS) {
         vs_sample_trace("vs_mal_hdmi_attr_get failed with 0x%x\n", ret);
         return VS_FAILED;
     }
 
     hdmi_attr.video_format = video_format;
     ret = vs_mal_hdmi_attr_set(0, &hdmi_attr);
     if (ret != VS_SUCCESS) {
         vs_sample_trace("vs_mal_hdmi_attr_set failed with 0x%x\n", ret);
         return VS_FAILED;
     }
 
     ret = sample_hdmi_vo_fromat_change(video_format);
     if (ret != VS_SUCCESS) {
         vs_sample_trace("sample_hdmi_vo_fromat_change failed with %x\n", ret);
         return VS_FAILED;
     }
 
     ret = vs_mal_hdmi_start(0);
     if (ret != VS_SUCCESS) {
         vs_sample_trace("vs_mal_hdmi_start failed with 0x%x\n", ret);
         return VS_FAILED;
     }
 
     return VS_SUCCESS;
 }
 
 vs_int32_t sample_hdmi_video_mode(vs_void_t)
 {
     vs_int32_t ret;
     vs_hdmi_attr_s hdmi_attr;
     vs_hdmi_video_mode_e video_mode;
 
     video_mode = sample_hdmi_video_mode_param_get();
 
     ret = vs_mal_hdmi_stop(0);
     if (ret != VS_SUCCESS) {
         vs_sample_trace("vs_mal_hdmi_stop failed with 0x%x\n", ret);
         return VS_FAILED;
     }
 
     ret = vs_mal_hdmi_attr_get(0, &hdmi_attr);
     if (ret != VS_SUCCESS) {
         vs_sample_trace("vs_mal_hdmi_attr_get failed with 0x%x\n", ret);
         return VS_FAILED;
     }
 
     hdmi_attr.video_out_mode = video_mode;
 
     ret = vs_mal_hdmi_attr_set(0, &hdmi_attr);
     if (ret != VS_SUCCESS) {
         vs_sample_trace("vs_mal_hdmi_attr_set failed with 0x%x\n", ret);
         return VS_FAILED;
     }
 
     ret = vs_mal_hdmi_start(0);
     if (ret != VS_SUCCESS) {
         vs_sample_trace("vs_mal_hdmi_start failed with 0x%x\n", ret);
         return VS_FAILED;
     }
 
     return VS_SUCCESS;
 }
 
 vs_int32_t sample_hdmi_audio_freq(vs_void_t)
 {
     vs_int32_t ret;
     vs_hdmi_attr_s hdmi_attr;
     vs_int32_t index;
     vs_int32_t aud_sample_rate;
 
     index = sample_hdmi_audio_freq_param_get();
 
     ret = vs_mal_hdmi_stop(0);
     if (ret != VS_SUCCESS) {
         vs_sample_trace("vs_mal_hdmi_stop failed with 0x%x\n", ret);
         return VS_FAILED;
     }
 
     ret = vs_mal_hdmi_attr_get(0, &hdmi_attr);
     if (ret != VS_SUCCESS) {
         vs_sample_trace("vs_mal_hdmi_attr_get failed with 0x%x\n", ret);
         return VS_FAILED;
     }
 
     switch (index) {
     case 0:
         hdmi_attr.audio_sample_rate = E_HDMI_SAMPLE_RATE_32K;
         aud_sample_rate = 32000;
         break;
     case 1:
         hdmi_attr.audio_sample_rate = E_HDMI_SAMPLE_RATE_44K;
         aud_sample_rate = 44100;
         break;
     case 2:
         hdmi_attr.audio_sample_rate = E_HDMI_SAMPLE_RATE_48K;
         aud_sample_rate = 48000;
         break;
     default:
         vs_sample_trace("wrong input aduio frequency\n");
         return VS_FAILED;
     }
 
     sample_hdmi_audio_stop();
 
     ret = vs_mal_hdmi_attr_set(0, &hdmi_attr);
     if (ret != VS_SUCCESS) {
         vs_sample_trace("vs_mal_hdmi_attr_set failed with 0x%x\n", ret);
         return VS_FAILED;
     }
 
     ret = vs_mal_hdmi_start(0);
     if (ret != VS_SUCCESS) {
         vs_sample_trace("vs_mal_hdmi_start failed with 0x%x\n", ret);
         return VS_FAILED;
     }
 
     ret = sample_hdmi_audio_start(aud_sample_rate);
     if (ret != VS_SUCCESS) {
         vs_sample_trace("sample_hdmi_vdec_vpss_vo_start fail\n");
         return VS_FAILED;
     }
 
     return VS_SUCCESS;
 }
 
 vs_int32_t sample_hdmi_auth_mode(vs_void_t)
 {
     vs_int32_t ret;
     vs_hdmi_attr_s hdmi_attr;
     vs_int32_t index;
 
     index = sample_hdmi_auth_mode_param_get();
 
     ret = vs_mal_hdmi_stop(0);
     if (ret != VS_SUCCESS) {
         vs_sample_trace("vs_mal_hdmi_stop failed with 0x%x\n", ret);
         return VS_FAILED;
     }
 
     ret = vs_mal_hdmi_attr_get(0, &hdmi_attr);
     if (ret != VS_SUCCESS) {
         vs_sample_trace("vs_mal_hdmi_attr_get failed with 0x%x\n", ret);
         return VS_FAILED;
     }
 
     switch (index) {
     case 0:
         hdmi_attr.is_auth_mode = VS_FALSE;
         break;
     case 1:
         hdmi_attr.is_auth_mode = VS_TRUE;
         break;
     default:
         vs_sample_trace("wrong input auth mode frequency\n");
         return VS_FAILED;
     }
 
     ret = vs_mal_hdmi_attr_set(0, &hdmi_attr);
     if (ret != VS_SUCCESS) {
         vs_sample_trace("vs_mal_hdmi_attr_set failed with 0x%x\n", ret);
         return VS_FAILED;
     }
 
     ret = vs_mal_hdmi_start(0);
     if (ret != VS_SUCCESS) {
         vs_sample_trace("vs_mal_hdmi_start failed with 0x%x\n", ret);
         return VS_FAILED;
     }
 
     return VS_SUCCESS;
 }
 
 static vs_void_t sample_hdmi_register_signal_handler(void (*sig_handler)(int))
 {
     struct sigaction sa;
     memset(&sa, 0, sizeof(struct sigaction));
     sa.sa_handler = sig_handler;
     sa.sa_flags = 0;
 
     sigaction(SIGINT, &sa, NULL);
     sigaction(SIGTERM, &sa, NULL);
 }
 
 vs_void_t hdmi_signal_handle(vs_int32_t s_no)
 {
     (vs_void_t)s_no;
     g_stop_flag = 1;
 }
 
 int sample_hdmi_case(vs_void_t)
 {
     vs_int32_t ret = VS_SUCCESS;
     vs_int32_t idx;
     char hdmi_cmd[HDMI_CMD_LEN] = {0};
     vs_hdmi_video_format_e format = E_HDMI_VIDEO_FORMAT_1080P_60;
     vs_int32_t aud_sample_rate = 48000;
 
     sample_hdmi_register_signal_handler(hdmi_signal_handle);
 
     ret = sample_hdmi_vdec_vpss_vo_start(format);
     if (ret != VS_SUCCESS) {
         vs_sample_trace("sample_hdmi_vdec_vpss_vo_start fail\n");
         return VS_FAILED;
     }
 
     ret = sample_hdmi_hdmi_start(format);
     if (ret != VS_SUCCESS) {
         vs_sample_trace("sample_hdmi_hdmi_start fail\n");
         return VS_FAILED;
     }
 
     g_audio_start = VS_TRUE;
     if (g_audio_start == VS_TRUE) {
         ret = sample_hdmi_audio_start(aud_sample_rate);
         if (ret != VS_SUCCESS) {
             vs_sample_trace("sample_hdmi_vdec_vpss_vo_start fail\n");
             return VS_FAILED;
         }
     }
 
     printf("\n >>>>>>>>>> Please input 'h' to get help or 'q' to quit! <<<<<<<<<<\n");
     while (!g_stop_flag) {
         printf("vs_cmd >");
         sample_hdmi_get_input_cmd(hdmi_cmd);
         if (hdmi_cmd[0] == 'q') {
             printf("sample_hdmi quit!\n");
             break;
         }
         if (hdmi_cmd[0] == 'h' || hdmi_cmd[0] == '?') {
             sample_hdmi_vdec_usage();
             continue;
         }
 
         idx = atoi(hdmi_cmd);
         if(idx == 0 && hdmi_cmd[0] != '0')
             continue;
 
         switch (idx) {
             case 0:
                 vs_sample_trace("sample_hdmi_hdmi_force runing\n\n\n");
                 ret = sample_hdmi_hdmi_force();
                 break;
             case 1:
                 vs_sample_trace("sample_hdmi_auto_select runing\n\n\n");
                 ret = sample_hdmi_auto_select();
                 break;
             case 2:
                 sample_hdmi_video_timing();
                 break;
             case 3:
                 sample_hdmi_video_mode();
                 break;
             case 4:
                 sample_hdmi_audio_freq();
                 break;
             case 5:
                 sample_hdmi_auth_mode();
                 break;
             default:
                 printf("cmd not support, please input 'h' to get help or 'q' to quit!\n");
                 break;
         }
         if (idx < 5)
             printf(" >>>>>>>>>> Please input 'h' to get help or 'q' to quit! <<<<<<<<<<\n");
     }
 
     sample_hdmi_audio_stop();
 
     sample_hdmi_hdmi_stop();
 
     sample_hdmi_vdec_vpss_vo_stop();
 
     return ret;
 }
 
 vs_void_t sample_hdmi_usage(char *prog_name)
 {
     printf("Usage : %s <index>\n", prog_name);
     printf("index:\n");
     printf("\t 0)vdec -> video & audio -> hdmi.\n");
     printf("\t 1)vii -> video & audio -> hdmi.\n");
 }
 
 int main(int argc, char *argv[])
 {
     vs_int32_t ret = VS_SUCCESS;
     vs_int32_t idx;
 
     if (argc < 2) {
         sample_hdmi_usage(argv[0]);
         return VS_FAILED;
     }
 
     if (!strncmp(argv[1], "-h", 2)) {
         sample_hdmi_usage(argv[0]);
         return VS_FAILED;
     }
 
     idx = atoi(argv[1]);
 
     switch (idx) {
         case 0:
             ret = sample_hdmi_case();
             break;
         case 1:
             ret = sample_vii_hdmi_case(argc, argv);
             break;
         default:
             sample_hdmi_usage(argv[0]);
             return VS_FAILED;
     }
 
     return ret;
 }
 