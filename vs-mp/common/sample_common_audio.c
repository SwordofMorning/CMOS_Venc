/**
* @file sample_common_audio.c
* @brief common audio sample
* @details common audio sample
* @author visinex sw group
* @date 2022-05-28
* @version v0.01
* @Copyright (C) 2020, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>
#include <time.h>
#include <unistd.h>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/prctl.h>
#include <arpa/inet.h>
#include <errno.h>
#include <assert.h>

#include "vs_comm.h"
#include "sample_common.h"
#include "vs_acodec_ioctl.h"
#include "vs_aac_codec.h"


#ifdef __cplusplus
extern "C" {
#endif

	typedef struct vs_acodec_agc_attr {
		vs_int32_t enable;
		vs_int32_t sample_rate;
		vs_int32_t max_level; ///< low 16-bit
		vs_int32_t min_level;
		vs_float_t alc_vol;  ///< -18dB ~ 28.5dB, step: 1.5dB
	} vs_acodec_agc_attr_s;
#define VS_ACODEC_ENABLE_AGC	                _IOW(VS_IOC_ACODEC_MAGIC, 77, vs_acodec_agc_attr_s)
#define VS_ACODEC_SET_MIC_BIAS_VOLTAGE	        _IOW(VS_IOC_ACODEC_MAGIC, 70, vs_int32_t)

typedef struct vs_thread_aenc {
	vs_bool_t        is_start;

	vs_bool_t        bind_with_adec;

	vs_aiodev_attr_s aio_common_attr;
	pthread_t        pid;

	char             ofile_name[64];

} vs_thread_aenc_s;

typedef struct vs_thread_ain_source {
	vs_bool_t        is_start;
	vs_int32_t       ai_devid;
	vs_int32_t       ai_chnid;
	vs_int32_t       ai_chn_num;

	pthread_t        pid; /* thread pid */

	vs_bool_t        send_to_aenc;
	vs_int32_t       aenc_chnid;

	vs_bool_t        send_to_aout;
	vs_int32_t       ao_devid;
	vs_int32_t       ao_chnid;

	vs_bool_t        send_to_file;
	char             ofile_name[128];

} vs_thread_ain_source_s;

typedef struct vs_thread_aout_sink {

	vs_bool_t        is_start;
	vs_int32_t       ao_devid;
	vs_int32_t       ao_chnid;

	vs_aiodev_attr_s aio_attr;

	FILE            *fp;

	pthread_t        pid;

} vs_thread_aout_sink_s;

typedef struct vs_thread_aenc_sink {

	vs_bool_t        is_start;
	vs_int32_t       aenc_chnid;
	vs_payload_type_e encoder_type;
	vs_aiodev_attr_s aio_common_attr;

	FILE            *fp;

	pthread_t        pid; /* thread pid */

} vs_thread_aenc_sink_s;

typedef struct vs_thread_aenc_source {
	vs_bool_t        is_start;
	vs_int32_t       aenc_chnid;

	pthread_t        pid; /* thread pid */

	vs_bool_t        send_to_adec;
	vs_int32_t       adec_chnid;

	vs_bool_t        send_to_file;
	char             ofile_name[128];

} vs_thread_aenc_source_s;

typedef struct vs_thread_adec_sink {

	vs_bool_t        is_start;
	vs_int32_t       adec_chnid;
	vs_payload_type_e decoder_type;
	FILE            *fp;

	pthread_t        pid; /* thread pid */

} vs_thread_adec_sink_s;

typedef struct vs_thread_adec_source {
	vs_bool_t        is_start;
	vs_int32_t       adec_chnid;
	vs_payload_type_e decoder_type;
	pthread_t        pid; /* thread pid */

	vs_bool_t        send_to_ao;
	vs_int32_t       ao_devid;
	vs_int32_t       ao_chnid;

	vs_bool_t        send_to_aenc;
	vs_int32_t       aenc_chnid;

	vs_bool_t        send_to_file;
	char             ofile_name[128];

} vs_thread_adec_source_s;

typedef void (*udp_data_recv_callback)(unsigned char *buf, int len);
typedef void (*udp_data_send_callback)(unsigned char **buf, int *len);

typedef struct simple_jitter_buffer_st {
	vs_int32_t frame_size;
	vs_int32_t frame_count;
	vs_uint32_t wp_idx;
	vs_uint32_t rp_idx;
	vs_uint32_t mask;
	vs_uint8_t *buf;
	pthread_mutex_t lock;
} simple_jitter_buffer_t;

typedef struct udp_info {
	int socket_fd;
	struct sockaddr_in socket_addr;
	int stop_flag;
	udp_data_recv_callback recv_callback;
	udp_data_send_callback send_callback;
	pthread_t pid;
	void *p_priv;
} udp_info_t;

static udp_info_t s_local_udp = {0};
static udp_info_t s_remote_udp = {0};
static simple_jitter_buffer_t s_aout_buf = {0};
static simple_jitter_buffer_t s_ain_buf = {0};

static vs_thread_ain_source_s  s_thread_ain_source[AIN_DEV_NUM_MAX][AIN_CHN_NUM_MAX] = {0};
static vs_thread_aout_sink_s   s_thread_aout_sink[AOUT_DEV_NUM_MAX][AOUT_CHN_NUM_MAX] = {0};

static vs_thread_aenc_sink_s   s_thread_aenc_sink[AENC_CHN_NUM_MAX] = {0};
static vs_thread_aenc_source_s s_thread_aenc_source[AENC_CHN_NUM_MAX] = {0};

static vs_thread_adec_sink_s   s_thread_adec_sink[ADEC_CHN_NUM_MAX] = {0};
static vs_thread_adec_source_s s_thread_adec_source[ADEC_CHN_NUM_MAX] = {0};

static char *gp_outfile_buffer = NULL;
static int   g_outfile_size = 0;
static int   g_outfile_pos = 0;

void get_date_str(char *p_date)
{
	time_t curr_time;
	struct tm *p;

	time(&curr_time);
	p = localtime(&curr_time);

	snprintf(p_date, 16, "%02d%02d_%02d%02d-%02d", (p->tm_mon + 1), p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);
}

int load_audio_data_file(int load_new_file)
{
	char data_file[128];
	FILE *fp = NULL;
	int ret;

	if (!load_new_file) {
		if (gp_outfile_buffer) {
			return 0;
		}
	}

	if (gp_outfile_buffer) {
		printf("to free audio data buffer\n");
		free(gp_outfile_buffer);
		gp_outfile_buffer = NULL;
		g_outfile_size = 0;
		g_outfile_pos = 0;
	}

		do {
			printf("Load New Data File:");
			ret = scanf("%s", data_file);
			getchar();
			if (ret < 0) {
				printf("scanf fails.\n");
				continue;
			}

		fp = fopen(data_file, "rb");

		if (!fp) {
			printf("file<%s> does not exist, input again\n", data_file);
		}
		else {
			fseek(fp, 0, SEEK_END);
			g_outfile_size = ftell(fp);

			if (!g_outfile_size) {
				printf("File Size: %08x, please select new file\n", g_outfile_size);
				continue;
			}
		}
	}
	while (!fp || !g_outfile_size);

	fseek(fp, 0, SEEK_END);
	g_outfile_size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	gp_outfile_buffer = (char *)malloc(g_outfile_size);

	if (!gp_outfile_buffer) {
		printf("ERROR: fail to malloc for file read\n");
		exit(0);
	}

	printf("File Buffer: %p, File Size: %08x\n", gp_outfile_buffer, g_outfile_size);

	if (fread(gp_outfile_buffer, 1, g_outfile_size, fp) != g_outfile_size) {
		printf("ERROR: read file data fails\n");
		exit(0);
	}

	g_outfile_pos = 0;

	return 0;
}

void reset_audio_data_file(void)
{
	g_outfile_pos = 0;
}

static int read_file(char *pdst, int size)
{
	if (!gp_outfile_buffer || !g_outfile_size) {
		vs_sample_trace("WARN: the source data file has not been loaded\n");
		vs_sample_trace("Please select 0001 to load the data file firstly\n")
		exit(-1);
	}

	if (g_outfile_pos + size <= g_outfile_size) {
		memcpy(pdst, gp_outfile_buffer + g_outfile_pos, size);
		g_outfile_pos += size;

		if (g_outfile_pos == g_outfile_size) {
			g_outfile_pos = 0;
			printf("file rewind\n");
		}
	}
	else if (g_outfile_pos + size > g_outfile_size) {
		int left_size;
		left_size = g_outfile_size - g_outfile_pos;
		memcpy(pdst, gp_outfile_buffer + g_outfile_pos, left_size);
		memcpy(pdst + left_size, gp_outfile_buffer, size - left_size);
		g_outfile_pos = size - left_size;
		printf("file rewind\n");
	}

	return size;
}

static int read_pcm_data(char *pdst, int size)
{
	if (read_file(pdst, size) != size) {
		printf("fail to read file\n");
		return -1;
	}

	return size;
}

static inline vs_bool_t jb_is_full(simple_jitter_buffer_t *p_jb)
{
	vs_bool_t ret = VS_FALSE;
	pthread_mutex_lock(&p_jb->lock);
	ret = ((p_jb->wp_idx & p_jb->mask) == (p_jb->rp_idx & p_jb->mask)) && (p_jb->wp_idx > p_jb->rp_idx);
	pthread_mutex_unlock(&p_jb->lock);
	return ret;
}

static inline vs_bool_t jb_is_empty(simple_jitter_buffer_t *p_jb)
{
	vs_bool_t ret = VS_FALSE;
	pthread_mutex_lock(&p_jb->lock);
	ret = p_jb->wp_idx == p_jb->rp_idx;
	pthread_mutex_unlock(&p_jb->lock);
	return ret;
}

static inline vs_uint8_t *jb_read_address(simple_jitter_buffer_t *p_jb)
{
	vs_uint8_t *p_ret;
	pthread_mutex_lock(&p_jb->lock);
	p_ret = p_jb->buf + (p_jb->rp_idx & p_jb->mask) * p_jb->frame_size;
	pthread_mutex_unlock(&p_jb->lock);
	return p_ret;
}

static inline vs_uint8_t *jb_write_address(simple_jitter_buffer_t *p_jb)
{
	vs_uint8_t *p_ret;
	pthread_mutex_lock(&p_jb->lock);
	p_ret = p_jb->buf + (p_jb->wp_idx & p_jb->mask) * p_jb->frame_size;
	pthread_mutex_unlock(&p_jb->lock);
	return p_ret;
}

static inline vs_void_t *jb_push_data(simple_jitter_buffer_t *p_jb, unsigned char *buf, int len)
{
	memcpy(jb_write_address(p_jb), buf, p_jb->frame_size);
	pthread_mutex_lock(&p_jb->lock);
	p_jb->wp_idx++;
	pthread_mutex_unlock(&p_jb->lock);
	return NULL;
}

static inline vs_void_t *jb_remove_data(simple_jitter_buffer_t *p_jb)
{
	pthread_mutex_lock(&p_jb->lock);
	p_jb->rp_idx++;
	pthread_mutex_unlock(&p_jb->lock);

	return NULL;
}

static inline vs_void_t *jb_clear_data(simple_jitter_buffer_t *p_jb)
{
	pthread_mutex_lock(&p_jb->lock);
	p_jb->rp_idx = p_jb->wp_idx;
	pthread_mutex_unlock(&p_jb->lock);
	return NULL;
}

static int write_audio_frame_to_file(vs_audio_frame_s *p_frame, vs_audio_sample_bitwidth_e sample_width, FILE *fp)
{
	vs_int32_t  frame_samples, ch_num;
	vs_uint32_t tmp_buffer[1024];
	vs_uint16_t * p_dst16, *p_chan_l, *p_chan_r;
	vs_uint32_t *p_dst32;
	vs_int32_t   write_pos;
	int i;

	p_chan_l = (vs_uint16_t *)p_frame->p_virt_addr[0];
	p_chan_r = (vs_uint16_t *)p_frame->p_virt_addr[1];
	ch_num = (p_frame->sound_mode == E_AUDIO_SOUND_MODE_STEREO) ? 2 : 1;
	write_pos = 0;

	//vs_sample_trace("frame.%d data: %04x %04x\n", p_frame->frame_no, p_chan_l[0], p_chan_r[0]);
	if (sample_width == E_AUDIO_SAMPLE_BITWIDTH_16) {
		frame_samples = p_frame->frame_chn_bytes >> 1;
		p_dst16 = (vs_uint16_t *)tmp_buffer;

		for (i = 0; i < frame_samples; i++) {
			*p_dst16++ = *p_chan_l++;
			*p_dst16++ = (ch_num == 2) ? *p_chan_r++ : 0;
			write_pos += 4;

			if (write_pos == sizeof(tmp_buffer)) {
				fwrite(tmp_buffer, 1, write_pos, fp);
				write_pos = 0;
				p_dst16 = (vs_uint16_t *)tmp_buffer;
			}
		}

		if (write_pos) {
			//vs_sample_trace("FILLED FRAME\n");
			fwrite(tmp_buffer, 1, write_pos, fp);
		}
	}
	else {
		vs_int32_t shift_bit;
		frame_samples = p_frame->frame_chn_bytes >> 2;
		shift_bit = (sample_width == E_AUDIO_SAMPLE_BITWIDTH_24) ? 8 : 16;
		p_dst32 = (vs_uint32_t *)tmp_buffer;

		for (i = 0; i < frame_samples; i++) {
			*p_dst32++ = (*p_chan_l++) << shift_bit;
			*p_dst32++ = (ch_num == 2) ? ((*p_chan_r++) << shift_bit) : 0;
			write_pos += 8;

			if (write_pos == sizeof(tmp_buffer)) {
				fwrite(tmp_buffer, 1, write_pos, fp);
				write_pos = 0;
				p_dst32 = (vs_uint32_t *)tmp_buffer;
			}
		}

		if (write_pos) {
			fwrite(tmp_buffer, 1, write_pos, fp);
		}
	}

	fflush(fp);

	return 0;
}

static void udp_data_send(udp_info_t *p_udp_info, unsigned char *buf, int len)
{
	sendto(p_udp_info->socket_fd, buf, len, 0, (struct sockaddr *)&p_udp_info->socket_addr, sizeof(struct sockaddr_in));
}

static int udp_socket_create(const char *p_udp_host, int udp_port, udp_info_t *p_udp_info)
{
	p_udp_info->socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if(p_udp_info->socket_fd < 0) {
		vs_sample_trace("socket_fd create failed for %s:%d\n", p_udp_host, udp_port);
		return -1;
	}

	bzero(&p_udp_info->socket_addr, sizeof(struct sockaddr_in));
	p_udp_info->socket_addr.sin_family = AF_INET;
	inet_pton(AF_INET, p_udp_host, &p_udp_info->socket_addr.sin_addr.s_addr);
	p_udp_info->socket_addr.sin_port = htons(udp_port);

	return 0;
}

static void udp_socket_close(udp_info_t *p_udp_info)
{
	close(p_udp_info->socket_fd);
}


static void udp_text_recv_callback(unsigned char *buf, int len)
{
	int i = 0;

	for (i = 0; i < len; i++) {
		printf("0x%02X ", buf[i]);
	}
	printf("\n");
}

static void udp_text_send_callback(unsigned char **buf, int *len)
{
	static unsigned char val = 0;
	static unsigned char s_buf[10] = {0};
	memset(s_buf, val++, sizeof(s_buf));
	*buf = s_buf;
	*len = 10;
	sleep(1);
}

static void udp_speech_recv_callback(unsigned char *buf, int len)
{
	assert(len == 320 && buf != NULL);

	if (!jb_is_full(&s_aout_buf)) {
		jb_push_data(&s_aout_buf, buf, len);
	}
	else {
		// Overlaid
		jb_clear_data(&s_aout_buf);
		jb_push_data(&s_aout_buf, buf, len);
		vs_sample_trace("AOUT Jitter Buffer Overlaid\n");
	}
}

static void udp_speech_send_callback(unsigned char **buf, int *len)
{
	static unsigned char s_buf[320] = {0};
	memset(s_buf, 0, sizeof(s_buf));
	*buf = NULL;
	*len = 0;

	if (!jb_is_empty(&s_ain_buf)) {
		memcpy(s_buf, jb_read_address(&s_ain_buf), s_ain_buf.frame_size);
		*buf = s_buf;
		*len = 320;
		jb_remove_data(&s_ain_buf);
	}
	else {
		usleep(1000);
	}
}

static void *udp_recv_task(void *p_arg)
{
	int ret = 0, recv_len = 0, addr_len = sizeof(struct sockaddr_in);
	fd_set fds;
	struct timeval tv;
	unsigned char buf[65535] = {0};
	udp_info_t *p_udp_info = (udp_info_t *)p_arg;

	ret = bind(p_udp_info->socket_fd, (struct sockaddr*)&p_udp_info->socket_addr, sizeof(p_udp_info->socket_addr));
	if (ret < 0) {
		vs_sample_trace("socket[%d] bind port failed\n", p_udp_info->socket_fd);
		return NULL;
	}

	vs_sample_trace("start to receive data\n");

	while (!p_udp_info->stop_flag) {
		FD_ZERO(&fds);
		FD_SET(p_udp_info->socket_fd, &fds);

		tv.tv_sec = 1;
		tv.tv_usec = 0;
		ret = select(p_udp_info->socket_fd + 1, &fds, NULL, NULL, &tv);
		if (ret < 0) {
			vs_sample_trace("udp_recv_task select error strerror[%s] !\n", strerror(errno));
			if (errno != EINTR) {
				return NULL;
			}
		}
		else if (ret > 0) {
			if(FD_ISSET(p_udp_info->socket_fd, &fds) > 0) {
				// vs_sample_trace("udp_recv_task select success !\n");
				recv_len = recvfrom(p_udp_info->socket_fd, buf, sizeof(buf), 0,
				                    (struct sockaddr *)&p_udp_info->socket_addr, (socklen_t *)&addr_len);

				if (recv_len > 0 && p_udp_info->recv_callback) {
					p_udp_info->recv_callback(buf, recv_len);
				}
			}
		}
		else {
			vs_sample_trace("udp_recv_task select timeout !\n");
		}
	}

	return NULL;
}

static void *udp_send_task(void *p_arg)
{
	udp_info_t *p_udp_info = (udp_info_t *)p_arg;
	unsigned char *buf = NULL;
	int len = 0;

	vs_sample_trace("start to send data\n");

	while (!p_udp_info->stop_flag) {
		p_udp_info->send_callback(&buf, &len);
		if (buf != NULL && len > 0) {
			udp_data_send(p_udp_info, buf, len);
		}
	}

	return NULL;
}

static vs_int32_t jb_init(simple_jitter_buffer_t *p_jb)
{
	p_jb->frame_size = 320;
	p_jb->frame_count = 256;
	p_jb->mask = p_jb->frame_count -1;
	p_jb->rp_idx = 0;
	p_jb->wp_idx = 10; // This is critical value that is related to delay.
	p_jb->buf = malloc(p_jb->frame_size * p_jb->frame_count);
	assert(p_jb->buf);
	memset(p_jb->buf, 0, p_jb->frame_size * p_jb->frame_count);

	pthread_mutex_init(&p_jb->lock, NULL);
	return VS_SUCCESS;
}

vs_int32_t sample_common_rtc_speech_jb_init() {
	vs_int32_t ret;
	ret = jb_init(&s_ain_buf);
	ret = jb_init(&s_aout_buf);
	return ret;
}

vs_int32_t jb_exit(simple_jitter_buffer_t *p_jb)
{
	if (p_jb->buf) {
		free(p_jb->buf);
	}

	memset(p_jb, 0, sizeof(simple_jitter_buffer_t));
	pthread_mutex_destroy(&p_jb->lock);
	return VS_SUCCESS;
}

vs_int32_t sample_common_rtc_speech_jb_exit() {
	vs_int32_t ret;
	ret = jb_exit(&s_ain_buf);
	ret = jb_exit(&s_aout_buf);
	return ret;
}

vs_int32_t sample_common_rtc_prepare(vs_int32_t local_port, const char *remote_ip, vs_int32_t remote_port, vs_int32_t is_speech)
{
	vs_int32_t ret = 0;

	memset(&s_local_udp, 0, sizeof(s_local_udp));
	memset(&s_remote_udp, 0, sizeof(s_remote_udp));

	ret = udp_socket_create("0.0.0.0", local_port, &s_local_udp);
	if (ret != 0) {
		return -1;
	}

	ret = udp_socket_create(remote_ip, remote_port, &s_remote_udp);
	if (ret != 0) {
		udp_socket_close(&s_local_udp);
		return -1;
	}

	if (is_speech) {
		s_local_udp.recv_callback = udp_speech_recv_callback;
		s_remote_udp.send_callback = udp_speech_send_callback;
	}
	else {
		s_local_udp.recv_callback = udp_text_recv_callback;
		s_remote_udp.send_callback = udp_text_send_callback;
	}

	return ret;
}

vs_int32_t sample_common_rtc_recv_thread_create()
{
	pthread_attr_t			 pthread_attr;
	struct sched_param		 param;

	vs_sample_trace("To Create UDP recv Thread\n");

	// init policy;
	pthread_attr_init(&pthread_attr);
	param.sched_priority = 90;
	pthread_attr_setschedpolicy(&pthread_attr, SCHED_FIFO);
	pthread_attr_setschedparam(&pthread_attr, &param);
	pthread_attr_setinheritsched(&pthread_attr, PTHREAD_EXPLICIT_SCHED);

	pthread_create(&s_local_udp.pid, &pthread_attr, udp_recv_task, &s_local_udp);

	return 0;
}

vs_int32_t sample_common_rtc_recv_thread_destroy()
{
	vs_sample_trace("To Destory RTC Receive Thread\n");

	if (!s_local_udp.stop_flag) {
		s_local_udp.stop_flag = 1;
		pthread_join(s_local_udp.pid, 0);
	}
	udp_socket_close(&s_local_udp);

	return 0;
}


vs_int32_t sample_common_rtc_send_thread_create()
{
	pthread_attr_t			 pthread_attr;
	struct sched_param		 param;

	vs_sample_trace("To Create UDP send Thread\n");

	// init policy;
	pthread_attr_init(&pthread_attr);
	param.sched_priority = 90;
	pthread_attr_setschedpolicy(&pthread_attr, SCHED_FIFO);
	pthread_attr_setschedparam(&pthread_attr, &param);
	pthread_attr_setinheritsched(&pthread_attr, PTHREAD_EXPLICIT_SCHED);

	pthread_create(&s_remote_udp.pid, &pthread_attr, udp_send_task, &s_remote_udp);

	return 0;
}

vs_int32_t sample_common_rtc_send_thread_destroy()
{
	vs_sample_trace("To Destory RTC Send Thread\n");

	if (!s_remote_udp.stop_flag) {
		s_remote_udp.stop_flag = 1;
		pthread_join(s_remote_udp.pid, 0);
	}

	udp_socket_close(&s_remote_udp);

	return 0;
}

vs_int32_t sample_common_adec_aout_bind(vs_int32_t adec_chnid, vs_int32_t ao_devid, vs_int32_t ao_chnid)
{
	vs_chn_s producer_chn, consumer_chn;

	producer_chn.modid = E_MOD_ID_ADEC;
	producer_chn.devid = 0;
	producer_chn.chnid = adec_chnid;
	consumer_chn.modid = E_MOD_ID_AOUT;
	consumer_chn.devid = ao_devid;
	consumer_chn.chnid = ao_chnid;

	return vs_mal_sys_bind(&producer_chn, &consumer_chn);
}

vs_int32_t sample_common_adec_aout_unbind(vs_int32_t adec_chnid, vs_int32_t ao_devid, vs_int32_t ao_chnid)
{
	vs_chn_s producer_chn, consumer_chn;

	producer_chn.modid = E_MOD_ID_ADEC;
	producer_chn.devid = 0;
	producer_chn.chnid = adec_chnid;
	consumer_chn.modid = E_MOD_ID_AOUT;
	consumer_chn.devid = ao_devid;
	consumer_chn.chnid = ao_chnid;

	return vs_mal_sys_unbind(&producer_chn, &consumer_chn);
}

vs_int32_t sample_common_ain_aout_bind(vs_int32_t ai_devid, vs_int32_t ai_chnid, vs_int32_t ao_devid, vs_int32_t ao_chnid)
{
	vs_chn_s producer_chn, consumer_chn;

	producer_chn.modid = E_MOD_ID_AIN;
	producer_chn.devid = ai_devid;
	producer_chn.chnid = ai_chnid;
	consumer_chn.modid = E_MOD_ID_AOUT;
	consumer_chn.devid = ao_devid;
	consumer_chn.chnid = ao_chnid;

	return vs_mal_sys_bind(&producer_chn, &consumer_chn);
}

vs_int32_t sample_common_ain_aout_unbind(vs_int32_t ai_devid, vs_int32_t ai_chnid, vs_int32_t ao_devid, vs_int32_t ao_chnid)
{
	vs_chn_s producer_chn, consumer_chn;

	producer_chn.modid = E_MOD_ID_AIN;
	producer_chn.devid = ai_devid;
	producer_chn.chnid = ai_chnid;
	consumer_chn.modid = E_MOD_ID_AOUT;
	consumer_chn.devid = ao_devid;
	consumer_chn.chnid = ao_chnid;

	return vs_mal_sys_unbind(&producer_chn, &consumer_chn);
}

vs_int32_t sample_common_ain_aenc_bind(vs_int32_t ai_devid, vs_int32_t ai_chnid, vs_int32_t aenc_chnid)
{
	vs_chn_s producer_chn, consumer_chn;

	producer_chn.modid = E_MOD_ID_AIN;
	producer_chn.devid = ai_devid;
	producer_chn.chnid = ai_chnid;
	consumer_chn.modid = E_MOD_ID_AENC;
	consumer_chn.devid = 0;
	consumer_chn.chnid = aenc_chnid;

	return vs_mal_sys_bind(&producer_chn, &consumer_chn);
}

vs_int32_t sample_common_ain_aenc_unbind(vs_int32_t ai_devid, vs_int32_t ai_chnid, vs_int32_t aenc_chnid)
{
	vs_chn_s producer_chn, consumer_chn;

	producer_chn.modid = E_MOD_ID_AIN;
	producer_chn.devid = ai_devid;
	producer_chn.chnid = ai_chnid;
	consumer_chn.modid = E_MOD_ID_AENC;
	consumer_chn.devid = 0;
	consumer_chn.chnid = aenc_chnid;

	return vs_mal_sys_unbind(&producer_chn, &consumer_chn);
}

vs_int32_t sample_common_aenc_adec_bind(vs_int32_t aenc_chnid, vs_int32_t adec_chnid)
{
	vs_chn_s producer_chn, consumer_chn;

	producer_chn.modid = E_MOD_ID_AENC;
	producer_chn.devid = 0;
	producer_chn.chnid = aenc_chnid;
	consumer_chn.modid = E_MOD_ID_ADEC;
	consumer_chn.devid = 0;
	consumer_chn.chnid = adec_chnid;

	return vs_mal_sys_bind(&producer_chn, &consumer_chn);
}

vs_int32_t sample_common_aenc_adec_unbind(vs_int32_t aenc_chnid, vs_int32_t adec_chnid)
{
	vs_chn_s producer_chn, consumer_chn;

	producer_chn.modid = E_MOD_ID_AENC;
	producer_chn.devid = 0;
	producer_chn.chnid = aenc_chnid;
	consumer_chn.modid = E_MOD_ID_ADEC;
	consumer_chn.devid = 0;
	consumer_chn.chnid = adec_chnid;

	return vs_mal_sys_unbind(&producer_chn, &consumer_chn);
}

vs_int32_t sample_common_adec_aenc_bind(vs_int32_t adec_chnid, vs_int32_t aenc_chnid)
{
	vs_chn_s producer_chn, consumer_chn;

	vs_sample_trace("To Bind ADEC(%d) and AENC(%d)\n", adec_chnid, aenc_chnid);

	producer_chn.modid = E_MOD_ID_ADEC;
	producer_chn.devid = 0;
	producer_chn.chnid = adec_chnid;
	consumer_chn.modid = E_MOD_ID_AENC;
	consumer_chn.devid = 0;
	consumer_chn.chnid = aenc_chnid;

	return vs_mal_sys_bind(&producer_chn, &consumer_chn);
}

vs_int32_t sample_common_adec_aenc_unbind(vs_int32_t adec_chnid, vs_int32_t aenc_chnid)
{
	vs_chn_s producer_chn, consumer_chn;

	vs_sample_trace("To Unbind ADEC(%d) and AENC(%d)\n", adec_chnid, aenc_chnid);

	producer_chn.modid = E_MOD_ID_ADEC;
	producer_chn.devid = 0;
	producer_chn.chnid = adec_chnid;
	consumer_chn.modid = E_MOD_ID_AENC;
	consumer_chn.devid = 0;
	consumer_chn.chnid = aenc_chnid;

	return vs_mal_sys_unbind(&producer_chn, &consumer_chn);
}

static void* ain_source_thread(void *p_arg)
{
	vs_thread_ain_source_s *p_ain = (vs_thread_ain_source_s *)p_arg;

	vs_audio_frame_s    audio_frame;
	vs_audio_aecframe_s aec_frame = {0};
	vs_int32_t          ai_devid;
	vs_int32_t          ai_chnid;
	vs_int32_t          aenc_chnid;
	vs_int32_t          ao_devid;
	vs_int32_t          ao_chnid;
	FILE               *fp = NULL;
	FILE               *fp_aec = NULL;
	char                thread_name[32];
#if defined(SELECT_MODE_ENABLE)
	fd_set              read_fd;
	vs_int32_t          ain_fd;
	struct timeval      timeout;
	vs_int32_t          retval;
#endif

	if (p_ain->send_to_file) {
		if (!(fp = fopen(p_ain->ofile_name, "wb"))) {
			vs_sample_trace("Fail to create file (%s)\n", p_ain->ofile_name);
		}
		else {
			vs_sample_trace("Save AIN file to %s\n", p_ain->ofile_name);
		}
	}

	ai_devid   = p_ain->ai_devid;
	ai_chnid   = p_ain->ai_chnid;
	ao_devid   = p_ain->ao_devid;
	ao_chnid   = p_ain->ao_chnid;
	aenc_chnid = p_ain->aenc_chnid;

	snprintf(thread_name, sizeof(thread_name), "ain_source_%d_%d", ai_devid, ai_chnid);
	prctl(PR_SET_NAME, thread_name);

#if defined(SELECT_MODE_ENABLE)
	FD_ZERO(&read_fd);
	ain_fd = vs_mal_ain_fd_get(ai_devid, ai_chnid);
	FD_SET(ain_fd, &read_fd);
#endif

	while (p_ain->is_start) {
#if defined(SELECT_MODE_ENABLE)
		timeout.tv_sec = 2;
		timeout.tv_usec = 0;

		FD_ZERO(&read_fd);
		FD_SET(ain_fd, &read_fd);

		retval = select(ain_fd + 1, &read_fd, NULL, NULL, &timeout);

		if (retval < 0) {
			vs_sample_trace("Error return from select()\n");
			break;
		}
		else if (0 == retval) {
			vs_sample_trace("timeout to acquire ain frame in select mode\n");
			break;
		}

		if (FD_ISSET(ain_fd, &read_fd)) {

			if (VS_SUCCESS != vs_mal_ain_frame_acquire(ai_devid, ai_chnid, &audio_frame, &aec_frame, 0)) {
				if (p_ain->is_start) {
					vs_sample_trace("break as vs_mal_ain_frame_acquire() fails\n");
				}

				break;
			}

#else
		{

			if (VS_SUCCESS != vs_mal_ain_frame_acquire(ai_devid, ai_chnid, &audio_frame, &aec_frame, -1)) {
				if (p_ain->is_start) {
					vs_sample_trace("break as vs_mal_ain_frame_acquire() fails\n");
				}

				break;
			}

#endif


			if (p_ain->send_to_aenc) {
				if (VS_SUCCESS != vs_mal_aenc_frame_send(aenc_chnid, &audio_frame, NULL)) {
					vs_sample_trace("Fail return from vs_mal_aenc_frame_send\n");
					break;
				}
			}
			else if (p_ain->send_to_aout) {
				if (VS_SUCCESS != vs_mal_aout_frame_send(ao_devid, ao_chnid, &audio_frame, -1)) {
					vs_sample_trace("Fail return from vs_mal_aout_frame_send\n");
					break;
				}
			}

			if (fp) {
				write_audio_frame_to_file(&audio_frame, E_AUDIO_SAMPLE_BITWIDTH_16, fp);

				if (aec_frame.valid && !fp_aec) {
					fp_aec = fopen("aec_frame.pcm", "wb");
				}

				//printf("valid:%d, %llx, %llx, %llx\n", aec_frame.valid, aec_frame.refer_frame.phys_addr, (vs_uint64_t)aec_frame.refer_frame.p_virt_addr[0], (vs_uint64_t)aec_frame.refer_frame.p_virt_addr[1]);
				if (aec_frame.valid && fp_aec && p_ain->is_start) {
					write_audio_frame_to_file(&aec_frame.refer_frame, E_AUDIO_SAMPLE_BITWIDTH_16, fp_aec);
				}
			}

			if (VS_SUCCESS != vs_mal_ain_frame_release(ai_devid, ai_chnid, &audio_frame, &aec_frame)) {
				vs_sample_trace("vs_mal_ain_frame_release fails\n");
			}
		}
	}

	if (fp) {
		fclose(fp);
	}

	if (fp_aec) {
		fclose(fp_aec);
	}

	vs_sample_trace("QUIT AIN SOURCE THREAD\n");

	return NULL;
}

static void* ain_multi_channel_thread(void *p_arg)
{
	vs_thread_ain_source_s *p_ain = (vs_thread_ain_source_s *)p_arg;
	vs_audio_frame_s audio_frame[10];
	vs_int32_t ai_devid;
	vs_int32_t pair, i;
	FILE *fp[10] = {0};
	char thread_name[32];
	char pcm_filename[128];
	char date[16];

	ai_devid = p_ain->ai_devid;
	pair = (p_ain->ai_chn_num + 1) >> 1;
	if (p_ain->send_to_file) {
		get_date_str(date);
		for (i = 0; i < pair; i++) {
			snprintf(pcm_filename, sizeof(pcm_filename), "./aidev%dch%d_%s.pcm", ai_devid, i, date);
			if (!(fp[i] = fopen(pcm_filename, "wb"))) {
				vs_sample_trace("Fail to create file (%s)\n", pcm_filename);
			}
			else {
				vs_sample_trace("Save AIN file to %s\n", pcm_filename);
			}
		}
	}

	snprintf(thread_name, sizeof(thread_name), "ain_multi_channel_%d", ai_devid);
	prctl(PR_SET_NAME, thread_name);

	while (p_ain->is_start) {
		vs_int32_t i;
		for (i = 0; i < pair; i++) {
			if (VS_SUCCESS != vs_mal_ain_frame_acquire(ai_devid, i, &audio_frame[i], NULL, -1)) {
				if (p_ain->is_start) {
					vs_sample_trace("break as vs_mal_ain_frame_acquire() fails\n");
				}
				break;
			}

			if (fp[i]) {
				write_audio_frame_to_file(&audio_frame[i], E_AUDIO_SAMPLE_BITWIDTH_16, fp[i]);
			}

			if (VS_SUCCESS != vs_mal_ain_frame_release(ai_devid, i, &audio_frame[i], NULL)) {
				vs_sample_trace("vs_mal_ain_frame_release fails\n");
			}
		}
		if (i < pair) {
			break;
		}
	}

	for (i = 0; i < pair; i++) {
		if (fp[i]) {
			fclose(fp[i]);
		}
	}

	vs_sample_trace("QUIT AIN MULTI CHANNEL THREAD\n");

	return NULL;
}

vs_int32_t sample_common_ain_multi_channel_thread_create(vs_int32_t ai_devid, vs_int32_t ai_chn_num, vs_bool_t send_to_file)
{
	pthread_attr_t			 pthread_attr;
	struct sched_param		 param;
	vs_thread_ain_source_s	*p_ain;

	vs_sample_trace("To Create AIN(%d) Multi Source Thread\n", ai_devid);

	p_ain = &s_thread_ain_source[ai_devid][0];
	p_ain->is_start 	= 1;
	p_ain->ai_devid 	= ai_devid;
	p_ain->ai_chnid 	= 0;
	p_ain->ai_chn_num 	= ai_chn_num;
	p_ain->send_to_aenc = VS_FALSE;
	p_ain->aenc_chnid	= -1;
	p_ain->send_to_aout = VS_FALSE;
	p_ain->ao_devid 	= -1;
	p_ain->ao_chnid 	= -1;
	p_ain->send_to_file = send_to_file;

	// init policy;
	pthread_attr_init(&pthread_attr);
	param.sched_priority = 90;
	pthread_attr_setschedpolicy(&pthread_attr, SCHED_FIFO);
	pthread_attr_setschedparam(&pthread_attr, &param);
	pthread_attr_setinheritsched(&pthread_attr, PTHREAD_EXPLICIT_SCHED);

	pthread_create(&p_ain->pid, &pthread_attr, ain_multi_channel_thread, p_ain);

	return 0;
}

vs_int32_t sample_common_ain_multi_channel_thread_destroy(vs_int32_t ai_devid)
{
	vs_thread_ain_source_s *p_ain;

	vs_sample_trace("To Destory AIN(%d) Multi Channel Thread\n", ai_devid);

	p_ain = &s_thread_ain_source[ai_devid][0];

	if (p_ain->is_start) {
		p_ain->is_start = 0;
		pthread_join(p_ain->pid, 0);
	}

	memset(p_ain, 0, sizeof(*p_ain));

	return 0;
}


vs_int32_t sample_common_ain_source_thread_create(vs_int32_t ai_devid, vs_int32_t ai_chnid, vs_int32_t aenc_chnid,
        vs_int32_t ao_devid, vs_int32_t ao_chnid, vs_bool_t send_to_file)
{
	pthread_attr_t           pthread_attr;
	struct sched_param       param;
	vs_thread_ain_source_s  *p_ain;
	char date[16];

	vs_sample_trace("To Create AIN(%d, %d) Source Thread\n", ai_devid, ai_chnid);

	p_ain = &s_thread_ain_source[ai_devid][ai_chnid];
	p_ain->is_start     = 1;
	p_ain->ai_devid     = ai_devid;
	p_ain->ai_chnid     = ai_chnid;
	p_ain->send_to_aenc = (aenc_chnid != -1) ? VS_TRUE : VS_FALSE;
	p_ain->aenc_chnid   = aenc_chnid;
	p_ain->send_to_aout = (ao_devid != -1) ? VS_TRUE : VS_FALSE;
	p_ain->ao_devid     = ao_devid;
	p_ain->ao_chnid     = ao_chnid;
	p_ain->send_to_file = send_to_file;

	if (p_ain->send_to_file) {
		get_date_str(date);
		snprintf(p_ain->ofile_name, sizeof(p_ain->ofile_name), "./aidev%dch%d_%s.pcm", ai_devid, ai_chnid, date);
	}
	else {
		p_ain->ofile_name[0] = '\0';
	}

	// init policy;
	pthread_attr_init(&pthread_attr);
	param.sched_priority = 90;
	pthread_attr_setschedpolicy(&pthread_attr, SCHED_FIFO);
	pthread_attr_setschedparam(&pthread_attr, &param);
	pthread_attr_setinheritsched(&pthread_attr, PTHREAD_EXPLICIT_SCHED);

	pthread_create(&p_ain->pid, &pthread_attr, ain_source_thread, p_ain);
	//pthread_create(&p_ain->pid, NULL, ain_source_thread, p_aid);

	return 0;
}

vs_int32_t sample_common_ain_source_thread_destroy(vs_int32_t ai_devid, vs_int32_t ai_chnid)
{
	vs_thread_ain_source_s *p_ain;

	vs_sample_trace("To Destory AIN(%d, %d) Source Thread\n", ai_devid, ai_chnid);

	p_ain = &s_thread_ain_source[ai_devid][ai_chnid];

	if (p_ain->is_start) {
		p_ain->is_start = 0;
		pthread_join(p_ain->pid, 0);
	}

	memset(p_ain, 0, sizeof(*p_ain));

	return 0;
}

vs_int32_t sample_common_ain_start(vs_int32_t ai_devid, vs_aiodev_attr_s *p_aio_attr, vs_int32_t out_samplerate, void *p_aqe_attr, vs_int32_t aqe_type)
{
	vs_int32_t retval;
	vs_int32_t paired_num;

	vs_sample_trace("Start AIN%d Device\n", ai_devid);

	retval = vs_mal_ain_attr_set(ai_devid, p_aio_attr);

	if (VS_SUCCESS != retval) {
		vs_sample_trace("fail to set aidev(%d) attr, retval <%#x>\n", ai_devid, retval);
		return VS_FAILED;
	}

	retval = vs_mal_ain_enable(ai_devid);

	if (VS_SUCCESS != retval) {
		vs_sample_trace("fail to enable aidev(%d) device, retval <%#x>\n", ai_devid, retval);
		return VS_FAILED;
	}

	paired_num = (p_aio_attr->sound_mode == E_AUDIO_SOUND_MODE_STEREO) ? ((p_aio_attr->chn_num + 1) >> 1) : p_aio_attr->chn_num;

	for (int ch = 0; ch < paired_num; ch++) {

		retval = vs_mal_ain_chn_enable(ai_devid, ch);

		if (VS_SUCCESS != retval) {
			vs_sample_trace("fail to enable aidev(%d,%d) channel with <%#x>\n", ai_devid, ch, retval);
			return VS_FAILED;
		}

		if (out_samplerate != -1) {
			retval = vs_mal_ain_resample_enable(ai_devid, ch, out_samplerate);

			if (VS_SUCCESS != retval) {
				vs_sample_trace("fail to enable aidev(%d,%d) resample with <%#x>\n", ai_devid, ch, retval);
			}
		}

		if (p_aqe_attr) {
			if (aqe_type == 0) {
				if (VS_SUCCESS != (retval = vs_mal_ain_record_aqe_attr_set(ai_devid, ch, (vs_ain_record_aqe_cfg_s *)p_aqe_attr))) {
					vs_sample_trace("fail to set aidev(%d,%d) record aqe attr with <%#x>\n", ai_devid, ch, retval);
					return retval;
				}

				if (VS_SUCCESS != (retval = vs_mal_ain_aqe_enable(ai_devid, ch))) {
					vs_sample_trace("fail to enable aidev(%d,%d) record aqe attr with <%#x>\n", ai_devid, ch, retval);
					return retval;
				}
			}
			else if (aqe_type == 1) {
				vs_int32_t ao_devid = 0;
				vs_int32_t ao_chnid = 0;

				if (VS_SUCCESS != (retval = vs_mal_ain_talk_aqe_attr_set(ai_devid, ch, ao_devid, ao_chnid, (vs_ain_talk_aqe_cfg_s *)p_aqe_attr))) {
					vs_sample_trace("fail to set aidev(%d,%d) talk aqe attr with <%#x>\n", ai_devid, ch, retval);
					return retval;
				}

				if (VS_SUCCESS != (retval = vs_mal_ain_aqe_enable(ai_devid, ch))) {
					vs_sample_trace("fail to enable aidev(%d,%d) talk aqe attr with <%#x>\n", ai_devid, ch, retval);
					return retval;
				}
			}
		}
	}

	if (ai_devid == 0 && p_aio_attr->i2s_target == E_AIO_CONNECT_INNER_CODEC) {
		sample_common_acodec_set(-1, -1, 20, 0, 3, -1, -1);
	}

	return VS_SUCCESS;
}

vs_int32_t sample_common_ain_stop(vs_int32_t ai_devid, vs_int32_t chn_num, vs_bool_t aqe_en)
{
	vs_int32_t retval;

	vs_sample_trace("Stop AIN%d Device\n", ai_devid);

	for (int i = 0; i < chn_num; i++) {
		if (aqe_en) {
			retval = vs_mal_ain_aqe_disable(ai_devid, i);

			if (VS_SUCCESS != retval) {
				vs_sample_trace("fail to disable aidev(%d,%d) aqe with <%#x>\n", ai_devid, i, retval);
				return retval;
			}
		}

		retval = vs_mal_ain_chn_disable(ai_devid, i);

		if (VS_SUCCESS != retval) {
			vs_sample_trace("fail to disable aidev(%d,%d) channel with <%#x>\n", ai_devid, i, retval);
			return retval;
		}

		retval = vs_mal_ain_resample_disable(ai_devid, i);

		if (VS_SUCCESS != retval) {
			vs_sample_trace("fail to disable aidev(%d,%d) resample with <%#x>\n", ai_devid, i, retval);
			return retval;
		}
	}

	retval = vs_mal_ain_disable(ai_devid);

	if (VS_SUCCESS != retval) {
		vs_sample_trace("fail to disable aidev(%d) device with <%#x>\n", ai_devid, retval);
		return retval;
	}

	return VS_SUCCESS;
}

static void* aout_sink_thread(void *p_arg)
{
	vs_thread_aout_sink_s *p_aout = (vs_thread_aout_sink_s *)p_arg;
	vs_int32_t             ao_devid;
	vs_int32_t             ao_chnid;
	vs_audio_frame_s       audio_frame = {0};
	FILE                  *fp;
	vs_int32_t             chn_num;
	vs_int16_t            * p_chan_l, *p_chan_r;
	vs_uint32_t            tmp_buffer[256];
	vs_int32_t             read_size, real_size;
	vs_int32_t             remain_bytes;
#if defined(SELECT_MODE_ENABLE)
	fd_set                 write_fd;
	vs_int32_t             aout_fd;
	struct timeval         timeout;
	vs_int32_t             retval;
#endif

	if (!(fp = p_aout->fp)) {
		vs_sample_trace("fp is NULL, use preload audio data\n");
		reset_audio_data_file();
	}

	ao_devid = p_aout->ao_devid;
	ao_chnid = p_aout->ao_chnid;

	chn_num = (p_aout->aio_attr.sound_mode == E_AUDIO_SOUND_MODE_STEREO) ? 2 : 1;

	audio_frame.frame_chn_bytes = p_aout->aio_attr.frame_sample_num * 2;
	audio_frame.sound_mode      = p_aout->aio_attr.sound_mode;
	audio_frame.sample_width    = E_AUDIO_SAMPLE_BITWIDTH_16;
	audio_frame.frame_no = 0;
	audio_frame.pts = 0;

	if (!(audio_frame.p_virt_addr[0] = (vs_uint8_t *)malloc(audio_frame.frame_chn_bytes * chn_num))) {
		vs_sample_trace("Fail to allocate dynamic memory\n");
		return NULL;
	}

	audio_frame.p_virt_addr[1] = audio_frame.p_virt_addr[0] + audio_frame.frame_chn_bytes;

#if defined(SELECT_MODE_ENABLE)
	FD_ZERO(&write_fd);
	aout_fd = vs_mal_aout_fd_get(ao_devid, ao_chnid);
	FD_SET(aout_fd, &write_fd);
#endif

	while (p_aout->is_start) {

		p_chan_l = (vs_int16_t *)audio_frame.p_virt_addr[0];
		p_chan_r = (vs_int16_t *)audio_frame.p_virt_addr[1];
		remain_bytes = audio_frame.frame_chn_bytes * chn_num;

		do {
			if (remain_bytes > sizeof(tmp_buffer)) {
				read_size = sizeof(tmp_buffer);
			}
			else {
				read_size = remain_bytes;
			}

			if (fp) {
				real_size = fread(tmp_buffer, 1, read_size, fp);

				if (real_size <= 0) {
					vs_sample_trace("rewind\n");
					fseek(fp, 0, SEEK_SET); /* rewind */
					break;
				}
			}
			else {
				real_size = read_pcm_data((char *)tmp_buffer, read_size);
			}

			if (chn_num == 1) {
				memcpy(p_chan_l, tmp_buffer, read_size);
				p_chan_l += read_size >> 1;
			}
			else {
				vs_int32_t temp32;

				for (int i = 0; i < (read_size >> 2); i++) {
					temp32 = tmp_buffer[i];
					*p_chan_l++ = temp32 & 0xffff;
					*p_chan_r++ = (temp32 >> 16) & 0xffff;
				}
			}

			remain_bytes -= read_size;

		}
		while (remain_bytes > 0);

#if defined(SELECT_MODE_ENABLE)
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;

		FD_ZERO(&write_fd);
		FD_SET(aout_fd, &write_fd);

		retval = select(aout_fd + 1, NULL, &write_fd, NULL, &timeout);

		if (retval < 0) {
			vs_sample_trace("Error return from select()\n");
			break;
		}
		else if (0 == retval) {
			vs_sample_trace("timeout to acquire ain frame in select mode\n");
			break;
		}

		if (FD_ISSET(aout_fd, &write_fd)) {

			if (VS_SUCCESS != vs_mal_aout_frame_send(ao_devid, ao_chnid, &audio_frame, 0)) {
				if (p_aout->is_start) {
					vs_sample_trace("break as vs_mal_aout_frame_send() fails\n");
				}

				break;
			}

#else
		{

			if (VS_SUCCESS != vs_mal_aout_frame_send(ao_devid, ao_chnid, &audio_frame, -1)) {
				if (p_aout->is_start) {
					vs_sample_trace("break as vs_mal_aout_frame_send() fails\n");
				}

				break;
			}

#endif
			audio_frame.frame_no++;
			audio_frame.pts += 1000 * p_aout->aio_attr.frame_sample_num / p_aout->aio_attr.sample_rate;
		}
	}

	free(audio_frame.p_virt_addr[0]);

	if (fp) {
		fclose(fp);
	}

	p_aout->is_start = VS_FALSE;

	vs_sample_trace("QUIT AOUT SINK THREAD\n");

	return NULL;
}

vs_int32_t sample_common_aout_sink_thread_create(vs_int32_t ao_devid, vs_int32_t ao_chnid, vs_aiodev_attr_s *p_aio_common_attr, FILE *fp)
{
	pthread_attr_t          pthread_attr;
	struct sched_param      param;
	vs_thread_aout_sink_s  *p_aout;

	p_aout = &s_thread_aout_sink[ao_devid][ao_chnid];
	p_aout->is_start   = 1;
	p_aout->ao_devid   = ao_devid;
	p_aout->ao_chnid   = ao_chnid;
	p_aout->fp         = fp;
	memcpy(&p_aout->aio_attr, p_aio_common_attr, sizeof(*p_aio_common_attr));

	// init policy;
	pthread_attr_init(&pthread_attr);
	param.sched_priority = 90;
	pthread_attr_setschedpolicy(&pthread_attr, SCHED_FIFO);
	pthread_attr_setschedparam(&pthread_attr, &param);
	pthread_attr_setinheritsched(&pthread_attr, PTHREAD_EXPLICIT_SCHED);

	pthread_create(&p_aout->pid, &pthread_attr, aout_sink_thread, p_aout);
	//pthread_create(&p_aout->pid, NULL, aout_sink_thread, p_aid);

	return 0;
}

vs_int32_t sample_common_aout_sink_thread_destroy(vs_int32_t ao_devid, vs_int32_t ao_chnid)
{
	vs_thread_aout_sink_s *p_aout;

	p_aout = &s_thread_aout_sink[ao_devid][ao_chnid];

	if (p_aout->is_start) {
		p_aout->is_start = 0;
		pthread_join(p_aout->pid, 0);
	}

	return 0;
}

vs_int32_t sample_common_hdmi_aout_start(vs_int32_t ao_devid, vs_aiodev_attr_s *p_aio_attr)
{
	vs_int32_t       phy_chn_num;
	vs_int32_t       retval;

	vs_sample_trace("Start HDMI_AOUT%d Device\n", ao_devid);

	if (VS_SUCCESS != (retval = vs_mal_aout_attr_set(ao_devid, p_aio_attr))) {
		vs_sample_trace("fail to set hdmi_aodev(%d) attr, retval <%#x>\n", ao_devid, retval);
		return VS_FAILED;
	}

	if (VS_SUCCESS != (retval = vs_mal_aout_enable(ao_devid))) {
		vs_sample_trace("fail to enable hdmi_aodev(%d) device, retval <%#x>\n", ao_devid, retval);
		return VS_FAILED;
	}

	phy_chn_num = (p_aio_attr->sound_mode == E_AUDIO_SOUND_MODE_STEREO) ? 2 : 1;

	for (int i = 0; i < phy_chn_num >> p_aio_attr->sound_mode; i++) {
		if (VS_SUCCESS != (retval = vs_mal_aout_chn_enable(ao_devid, i))) {
			vs_sample_trace("fail to enable hdmi_aodev(%d,%d) channel with <0x%08x>\n", ao_devid, i, retval);
			return VS_FAILED;
		}
	}

	return VS_SUCCESS;
}

vs_int32_t sample_common_aout_start(vs_int32_t ao_devid, vs_aiodev_attr_s *p_aio_attr, vs_int32_t in_samplerate, vs_void_t *p_aqe_attr)
{
	vs_int32_t       phy_chn_num;
	vs_int32_t       retval;

	vs_sample_trace("Start AOUT%d Device\n", ao_devid);

	if (VS_SUCCESS != (retval = vs_mal_aout_attr_set(ao_devid, p_aio_attr))) {
		vs_sample_trace("fail to set aodev(%d) attr, retval <%#x>\n", ao_devid, retval);
		return VS_FAILED;
	}

	if (VS_SUCCESS != (retval = vs_mal_aout_enable(ao_devid))) {
		vs_sample_trace("fail to enable aodev(%d) device, retval <%#x>\n", ao_devid, retval);
		return VS_FAILED;
	}

	phy_chn_num = (p_aio_attr->sound_mode == E_AUDIO_SOUND_MODE_STEREO) ? 2 : 1;

	for (int i = 0; i<phy_chn_num >> p_aio_attr->sound_mode; i++) {
		if (VS_SUCCESS != (retval = vs_mal_aout_chn_enable(ao_devid, i))) {
			vs_sample_trace("fail to enable aodev(%d,%d) channel with <0x%08x>\n", ao_devid, i, retval);
			return VS_FAILED;
		}

		if (in_samplerate != -1) {
			retval = vs_mal_aout_resample_enable(ao_devid, i, in_samplerate);

			if (VS_SUCCESS != retval) {
				vs_sample_trace("fail to enable aodev(%d,%d) resample with <%#x>\n", ao_devid, i, retval);
			}
		}

		if (p_aqe_attr) {
			if (VS_SUCCESS != (retval = vs_mal_aout_aqe_attr_set(ao_devid, i, (vs_aout_aqe_cfg_s *)p_aqe_attr))) {
				vs_sample_trace("fail to set aodev(%d,%d) playback aqe attr with <%#x>\n", ao_devid, i, retval);
				return retval;
			}

			if (VS_SUCCESS != (retval = vs_mal_aout_aqe_enable(ao_devid, i))) {
				vs_sample_trace("fail to enable aodev(%d,%d) playback aqe attr with <%#x>\n", ao_devid, i, retval);
				return retval;
			}
		}
	}

	return VS_SUCCESS;
}

vs_int32_t sample_common_aout_stop(vs_int32_t ao_devid, vs_int32_t chn_num, vs_bool_t aqe_en)
{
	vs_int32_t        retval;

	vs_sample_trace("Stop AOUT%d Device\n", ao_devid);

	for (int i = 0; i < chn_num; i++) {
		if (aqe_en) {
			retval = vs_mal_aout_aqe_disable(ao_devid, i);

			if (VS_SUCCESS != retval) {
				vs_sample_trace("fail to disable aodev(%d,%d) aqe with <%#x>\n", ao_devid, i, retval);
				return retval;
			}
		}

		if (VS_SUCCESS != (retval = vs_mal_aout_chn_disable(ao_devid, i))) {
			vs_sample_trace("fail to disable aodev(%d,%d) channel with <%#x>\n", ao_devid, i, retval);
			return retval;
		}

		if (VS_SUCCESS != (retval = vs_mal_aout_resample_disable(ao_devid, i))) {
			vs_sample_trace("fail to disable aodev(%d,%d) resample with <%#x>\n", ao_devid, i, retval);
			return retval;
		}
	}

	if (VS_SUCCESS != (retval = vs_mal_aout_disable(ao_devid))) {
		vs_sample_trace("fail to disable aodev(%d) device with <%#x>\n", ao_devid, retval);
		return retval;
	}

	return VS_SUCCESS;
}

static void* aenc_sink_thread(void *p_arg)
{
	vs_thread_aenc_sink_s *p_aenc = (vs_thread_aenc_sink_s *)p_arg;
	vs_int32_t             aenc_chnid;
	vs_audio_frame_s       audio_frame = {0};
	FILE                  *fp;
	vs_int32_t             chn_num;
	vs_int16_t            *p_chan_l, *p_chan_r;
	vs_uint32_t            tmp_buffer[256];
	vs_int32_t             read_size, real_size;
	vs_int32_t             remain_bytes;

	if (!(fp = p_aenc->fp)) {
		vs_sample_trace("fp is NULL, use preload audio data\n");
		reset_audio_data_file();
	}

	aenc_chnid = p_aenc->aenc_chnid;

	chn_num = (p_aenc->aio_common_attr.sound_mode == E_AUDIO_SOUND_MODE_STEREO) ? 2 : 1;

	audio_frame.frame_chn_bytes = p_aenc->aio_common_attr.frame_sample_num * 2;
	audio_frame.sound_mode      = p_aenc->aio_common_attr.sound_mode;
	audio_frame.sample_width = E_AUDIO_SAMPLE_BITWIDTH_16;
	audio_frame.frame_no = 0;
	audio_frame.pts = 0;

	if (!(audio_frame.p_virt_addr[0] = (vs_uint8_t *)malloc(audio_frame.frame_chn_bytes * chn_num))) {
		vs_sample_trace("Fail to allocate dynamic memory\n");
		return NULL;
	}

	audio_frame.p_virt_addr[1] = audio_frame.p_virt_addr[0] + audio_frame.frame_chn_bytes;

	while (p_aenc->is_start) {
		p_chan_l = (vs_int16_t *)audio_frame.p_virt_addr[0];
		p_chan_r = (vs_int16_t *)audio_frame.p_virt_addr[1];

		remain_bytes = audio_frame.frame_chn_bytes * chn_num;

		do {
			if (remain_bytes > sizeof(tmp_buffer)) {
				read_size = sizeof(tmp_buffer);
			}
			else {
				read_size = remain_bytes;
			}

			if (fp) {
				real_size = fread(tmp_buffer, 1, read_size, fp);

				if (real_size <= 0) {
					vs_sample_trace("rewind\n");
					fseek(fp, 0, SEEK_SET); /* rewind */
					break;
				}
			}
			else {
				real_size = read_pcm_data((char *)tmp_buffer, read_size);
			}

			if (chn_num == 1) {
				memcpy(p_chan_l, tmp_buffer, read_size);
				p_chan_l += read_size >> 1;
			}
			else {
				vs_int32_t temp32;

				for (int i = 0; i < (read_size >> 2); i++) {
					temp32 = tmp_buffer[i];
					*p_chan_l++ = temp32 & 0xffff;
					*p_chan_r++ = (temp32 >> 16) & 0xffff;
				}
			}

			remain_bytes -= read_size;

		}
		while (remain_bytes > 0);

		do {
			if (VS_SUCCESS == vs_mal_aenc_frame_send(aenc_chnid, &audio_frame, NULL)) {
				//vs_sample_trace("Fail to send AENC(chn%d) frame\n", aenc_chnid);
				break;
			}
			else {
				usleep(2000);
			}
		}
		while (p_aenc->is_start);
	}

	free(audio_frame.p_virt_addr[0]);

	if (fp) {
		fclose(fp);
	}

	p_aenc->is_start = VS_FALSE;

	vs_sample_trace("QUIT AENC SINK THREAD\n");

	return NULL;
}

vs_int32_t sample_common_aenc_sink_thread_create(vs_int32_t aenc_chnid, vs_aiodev_attr_s *p_aio_common_attr, vs_payload_type_e encoder_type, FILE *fp)
{
	pthread_attr_t          pthread_attr;
	struct sched_param      param;
	vs_thread_aenc_sink_s  *p_aenc;

	p_aenc = &s_thread_aenc_sink[aenc_chnid];
	p_aenc->is_start   = 1;
	p_aenc->aenc_chnid = aenc_chnid;
	p_aenc->fp         = fp;
	p_aenc->encoder_type = encoder_type;
	memcpy(&p_aenc->aio_common_attr, p_aio_common_attr, sizeof(*p_aio_common_attr));

	// init policy;
	pthread_attr_init(&pthread_attr);
	param.sched_priority = 90;
	pthread_attr_setschedpolicy(&pthread_attr, SCHED_FIFO);
	pthread_attr_setschedparam(&pthread_attr, &param);
	pthread_attr_setinheritsched(&pthread_attr, PTHREAD_EXPLICIT_SCHED);

	pthread_create(&p_aenc->pid, &pthread_attr, aenc_sink_thread, p_aenc);
	//pthread_create(&p_aenc->pid, NULL, aenc_process_thread, p_aid);

	return 0;
}

vs_int32_t sample_common_aenc_sink_thread_destroy(vs_int32_t aenc_chnid)
{
	vs_thread_aenc_sink_s *p_aenc;

	p_aenc = &s_thread_aenc_sink[aenc_chnid];

	if (p_aenc->is_start) {
		p_aenc->is_start = 0;
		pthread_join(p_aenc->pid, 0);
	}

	return 0;
}

static void* aenc_source_thread(void *p_arg)
{
	vs_thread_aenc_source_s *p_aenc = (vs_thread_aenc_source_s *)p_arg;
	vs_int32_t               aenc_chnid;
	vs_int32_t               adec_chnid;
	vs_audio_stream_s        audio_stream = {0};
	vs_int32_t               retval;
	FILE                    *fp = NULL;
#if defined(SELECT_MODE_ENABLE)
	fd_set                   read_fd;
	vs_int32_t               aenc_fd;
	struct timeval           timeout;
#endif

	aenc_chnid = p_aenc->aenc_chnid;
	adec_chnid = p_aenc->adec_chnid;

	if (p_aenc->send_to_file) {
		if (!(fp = fopen(p_aenc->ofile_name, "wb"))) {
			vs_sample_trace("Fail to create file (%s)\n", p_aenc->ofile_name);
		}
		else {
			vs_sample_trace("Save AENC file to %s\n", p_aenc->ofile_name);
		}
	}

#if defined(SELECT_MODE_ENABLE)
	FD_ZERO(&read_fd);
	aenc_fd = vs_mal_aenc_fd_get(aenc_chnid);
	FD_SET(aenc_fd, &read_fd);
#endif

	while (p_aenc->is_start) {
#if defined(SELECT_MODE_ENABLE)
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;

		FD_ZERO(&read_fd);
		FD_SET(aenc_fd, &read_fd);

		retval = select(aenc_fd + 1, &read_fd, NULL, NULL, &timeout);

		if (retval < 0) {
			vs_sample_trace("Error return from select()\n");
			break;
		}
		else if (0 == retval) {
			vs_sample_trace("timeout to acquire aenc stream in select mode\n");
			break;
		}

		if (FD_ISSET(aenc_fd, &read_fd)) {

			if (VS_SUCCESS != (retval = vs_mal_aenc_stream_acquire(aenc_chnid, &audio_stream, 0))) {
				if (p_aenc->is_start) {
					vs_sample_trace("break as vs_mal_aenc_frame_acquire() fails(0x%08x) on aenc_chn%d\n", retval, aenc_chnid);
				}

				break;
			}

#else
		{

			if (VS_SUCCESS != (retval = vs_mal_aenc_stream_acquire(aenc_chnid, &audio_stream, -1))) {
				if (p_aenc->is_start) {
					vs_sample_trace("break as vs_mal_aenc_frame_acquire() fails(0x%08x) on aenc_chn%d\n", retval, aenc_chnid);
				}

				break;
			}

#endif
			//printf("frame.%d, pts:%lld\n", audio_stream.frame_no, audio_stream.pts);

			if (p_aenc->send_to_adec) {
				if (VS_SUCCESS != (retval = vs_mal_adec_stream_send(adec_chnid, &audio_stream, VS_TRUE))) {
					vs_sample_trace("vs_mal_adec_stream_send fails(0x%08x) on adec_chn%d\n", retval, adec_chnid);
					break;
				}
			}

			if (fp) {
				fwrite(audio_stream.p_stream, 1, audio_stream.stream_length, fp);
				fflush(fp);
			}

			if (VS_SUCCESS != (retval = vs_mal_aenc_stream_release(aenc_chnid, &audio_stream))) {
				vs_sample_trace("vs_mal_aenc_stream_release fails(0x%08x) on aenc_chn%d\n", retval, aenc_chnid);
			}
		}
	}

	if (fp) {
		fclose(fp);
	}

	vs_sample_trace("QUIT AENC SOURCE THREAD\n");

	return NULL;
}

vs_int32_t sample_common_aenc_source_thread_create(vs_int32_t aenc_chnid, vs_int32_t adec_chnid, vs_payload_type_e encoder_type, vs_bool_t send_to_file)
{
	pthread_attr_t           pthread_attr;
	struct sched_param       param;
	vs_thread_aenc_source_s *p_aenc;
	char date[16];

	p_aenc = &s_thread_aenc_source[aenc_chnid];
	p_aenc->is_start     = 1;
	p_aenc->aenc_chnid   = aenc_chnid;
	p_aenc->adec_chnid   = adec_chnid;
	p_aenc->send_to_adec = (adec_chnid != -1) ? VS_TRUE : VS_FALSE;
	p_aenc->send_to_file = send_to_file;

	if (p_aenc->send_to_file) {
		get_date_str(date);

		switch (encoder_type) {
		case E_PT_TYPE_G711A:
			snprintf(p_aenc->ofile_name, sizeof(p_aenc->ofile_name), "./audio_ch%d_%s.g711a", aenc_chnid, date);
			break;
		case E_PT_TYPE_G711U:
			snprintf(p_aenc->ofile_name, sizeof(p_aenc->ofile_name), "./audio_ch%d_%s.g711u", aenc_chnid, date);
			break;
		case E_PT_TYPE_G726:
			snprintf(p_aenc->ofile_name, sizeof(p_aenc->ofile_name), "./audio_ch%d_%s.g726", aenc_chnid, date);
			break;
		case E_PT_TYPE_ADPCMA:
			snprintf(p_aenc->ofile_name, sizeof(p_aenc->ofile_name), "./audio_ch%d_%s.adpcm", aenc_chnid, date);
			break;
		case E_PT_TYPE_LPCM:
			snprintf(p_aenc->ofile_name, sizeof(p_aenc->ofile_name), "./audio_ch%d_%s.pcm", aenc_chnid, date);
			break;
		case E_PT_TYPE_AAC:
			snprintf(p_aenc->ofile_name, sizeof(p_aenc->ofile_name), "./audio_ch%d_%s.aac", aenc_chnid, date);
			break;
		default:
			snprintf(p_aenc->ofile_name, sizeof(p_aenc->ofile_name), "./audio_ch%d_%s.enc", aenc_chnid, date);
			break;
		}
	}
	else {
		p_aenc->ofile_name[0] = '\0';
	}

	// init policy;
	pthread_attr_init(&pthread_attr);
	param.sched_priority = 90;
	pthread_attr_setschedpolicy(&pthread_attr, SCHED_FIFO);
	pthread_attr_setschedparam(&pthread_attr, &param);
	pthread_attr_setinheritsched(&pthread_attr, PTHREAD_EXPLICIT_SCHED);

	pthread_create(&p_aenc->pid, &pthread_attr, aenc_source_thread, p_aenc);
	//pthread_create(&p_aenc->pid, NULL, aenc_source_thread, p_aid);

	return 0;
}

vs_int32_t sample_common_aenc_source_thread_destroy(vs_int32_t aenc_chnid)
{
	vs_thread_aenc_source_s *p_aenc;

	vs_sample_trace("To Destory AENC(%d) Source Thread\n", aenc_chnid);

	p_aenc = &s_thread_aenc_source[aenc_chnid];

	if (p_aenc->is_start) {
		p_aenc->is_start = 0;
		pthread_join(p_aenc->pid, 0);
	}

	return 0;
}


/**
 *
 * @param aenc_chnid [in]  -1: refer to paired_chn_num
 * @param paired_chn_num [in] -1: refer to aenc_chnid
 * @param p_aio_attr
 * @param encoder_type
 *
 * @return vs_int32_t
 */
vs_int32_t sample_common_aenc_start(vs_int32_t aenc_chnid, vs_int32_t paired_chn_num, vs_aiodev_attr_s *p_aio_attr, vs_payload_type_e encoder_type)
{
	vs_aenc_attr_s      aenc_chn_attr = {0};
	vs_aenc_lpcm_attr_s aenc_lpcm = {0};
	vs_aenc_g711_attr_s aenc_g711a = {0};
	vs_aenc_g711_attr_s aenc_g711u = {0};
	vs_aenc_g726_attr_s aenc_g726 = {0};
	vs_aenc_adpcm_attr_s aenc_adpcm = {0};
	vs_aenc_aac_attr_s aenc_aac = {0};
	//vs_int32_t          paired_chn_num;
	vs_int32_t          retval;

	aenc_chn_attr.encoder_type      = encoder_type;
	aenc_chn_attr.pool_buffer_depth = 16;
	aenc_chn_attr.frame_sample_num  = p_aio_attr->frame_sample_num;

#if 0
	paired_chn_num = p_aio_attr->chn_num;

	if (p_aio_attr->sound_mode == E_AUDIO_SOUND_MODE_STEREO) {
		paired_chn_num >>= 1;
	}

#endif

	switch (encoder_type) {
	case E_PT_TYPE_LPCM:
		//aenc_lpcm.sample_width = p_aio_attr->sample_width;
		//aenc_lpcm.chn_num      = p_aio_attr->sound_mode == E_AUDIO_SOUND_MODE_STEREO ? 2 : 1;
		aenc_chn_attr.p_attr   = &aenc_lpcm;
		break;
	case E_PT_TYPE_G711A:
		aenc_chn_attr.p_attr   = &aenc_g711a;
		break;
	case E_PT_TYPE_G711U:
		aenc_chn_attr.p_attr   = &aenc_g711u;
		break;
	case E_PT_TYPE_G726:
		aenc_g726.g726_bps     = E_AUDIO_G726_40K;
		aenc_chn_attr.p_attr   = &aenc_g726;
		break;
	case E_PT_TYPE_ADPCMA:
		aenc_adpcm.adpcm_type  = E_AUDIO_ADPCM_TYPE_DVI4;
		aenc_chn_attr.p_attr   = &aenc_adpcm;
		break;
	case E_PT_TYPE_AAC:
		aenc_aac.aac_type = E_AUDIO_AAC_TYPE_AACLC;
		aenc_aac.bitrate = E_AUDIO_BPS_64K;
		aenc_aac.sample_rate   = p_aio_attr->sample_rate;
		aenc_aac.bit_width = E_AUDIO_SAMPLE_BITWIDTH_16;
		aenc_aac.sound_mode = p_aio_attr->sound_mode;
		aenc_aac.trans_type = E_AUDIO_AAC_TRANS_TYPE_ADTS;
		aenc_chn_attr.p_attr   = &aenc_aac;

		if (E_AUDIO_AAC_TYPE_AACLC == aenc_aac.aac_type) {
			if (p_aio_attr->frame_sample_num != AACLC_FRAME_SAMPLE_NUM) {
				vs_sample_trace("Invalid frame_sample_num: %d\t, aaclc must be %d\n", p_aio_attr->frame_sample_num, AACLC_FRAME_SAMPLE_NUM);
				return VS_FAILED;
			}
		}
		else if (E_AUDIO_AAC_TYPE_EAAC == aenc_aac.aac_type) {
			if (p_aio_attr->frame_sample_num != EAAC_FRAME_SAMPLE_NUM) {
				vs_sample_trace("Invalid frame_sample_num: %d\t, eaac must be %d\n", p_aio_attr->frame_sample_num, EAAC_FRAME_SAMPLE_NUM);
				return VS_FAILED;
			}
		}
		else if (E_AUDIO_AAC_TYPE_EAACPLUS == aenc_aac.aac_type) {
			if (p_aio_attr->frame_sample_num != EAACPLUS_FRAME_SAMPLE_NUM) {
				vs_sample_trace("Invalid frame_sample_num: %d\t, eaacplus must be %d\n", p_aio_attr->frame_sample_num, EAACPLUS_FRAME_SAMPLE_NUM);
				return VS_FAILED;
			}
		}
		else if (E_AUDIO_AAC_TYPE_AACLD == aenc_aac.aac_type) {
			if (p_aio_attr->frame_sample_num != AACLD_FRAME_SAMPLE_NUM) {
				vs_sample_trace("Invalid frame_sample_num: %d\t, aacld must be %d\n", p_aio_attr->frame_sample_num, AACLD_FRAME_SAMPLE_NUM);
				return VS_FAILED;
			}
		}
		else if (E_AUDIO_AAC_TYPE_AACELD == aenc_aac.aac_type) {
			if (p_aio_attr->frame_sample_num != AACELD_FRAME_SAMPLE_NUM) {
				vs_sample_trace("Invalid frame_sample_num: %d\t, aaceld must be %d\n", p_aio_attr->frame_sample_num, AACELD_FRAME_SAMPLE_NUM);
				return VS_FAILED;
			}
		}

		break;
	default:
		vs_sample_trace("Invalid encoder type<%d>\n", encoder_type);
		break;
	}

	if (aenc_chnid != -1) {
		if (VS_SUCCESS != (retval = vs_mal_aenc_create(aenc_chnid, &aenc_chn_attr))) {
			vs_sample_trace("Fail to create AENC channel(%d), error_code(0x%x)\n", aenc_chnid, retval);
			return retval;
		}
	}
	else {
		for (int ch = 0; ch < paired_chn_num; ch++) {
			if (VS_SUCCESS != (retval = vs_mal_aenc_create(ch, &aenc_chn_attr))) {
				vs_sample_trace("Fail to create AENC channel(%d), error_code(0x%x)\n", ch, retval);
				return retval;
			}
		}
	}

	return VS_SUCCESS;
}

vs_int32_t sample_common_aenc_stop(vs_int32_t aenc_chnid, vs_int32_t paired_chn_num)
{
	vs_int32_t retval;

	if (aenc_chnid != -1) {
		if (VS_SUCCESS != (retval = vs_mal_aenc_destroy(aenc_chnid))) {
			vs_sample_trace("Fail to destroy AENC channel(%d), error_code(0x%x)\n", aenc_chnid, retval);
			return retval;
		}
	}
	else {
		for (int ch = 0; ch < paired_chn_num; ch++) {
			if (VS_SUCCESS != (retval = vs_mal_aenc_destroy(ch))) {
				vs_sample_trace("Fail to destroy AENC channel(%d), error_code(0x%x)\n", ch, retval);
				return retval;
			}
		}
	}

	return VS_SUCCESS;
}

static void* adec_source_thread(void *p_arg)
{
	vs_thread_adec_source_s *p_adec = (vs_thread_adec_source_s *)p_arg;
	vs_int32_t               adec_chnid;
	vs_audio_frame_s         audio_frame = {0};
	FILE                    *fp = NULL;
#if defined(SELECT_MODE_ENABLE)
	fd_set                   read_fd;
	vs_int32_t               adec_fd;
	struct timeval           timeout;
	vs_int32_t               retval;
#endif

	if (p_adec->send_to_file) {
		fp = fopen(p_adec->ofile_name, "wb");

		if (!fp) {
			vs_sample_trace("FAIL to create file <%s>\n", p_adec->ofile_name);
			return NULL;
		}
		else {
			vs_sample_trace("Save ADEC file to %s\n", p_adec->ofile_name);
		}
	}

	adec_chnid = p_adec->adec_chnid;

#if defined(SELECT_MODE_ENABLE)
	FD_ZERO(&read_fd);
	adec_fd = vs_mal_adec_fd_get(adec_chnid);
	FD_SET(adec_fd, &read_fd);
#endif

	while (p_adec->is_start) {
#if defined(SELECT_MODE_ENABLE)
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;

		FD_ZERO(&read_fd);
		FD_SET(adec_fd, &read_fd);

		retval = select(adec_fd + 1, &read_fd, NULL, NULL, &timeout);

		if (retval < 0) {
			vs_sample_trace("Error return from select()\n");
			break;
		}
		else if (0 == retval) {
			vs_sample_trace("timeout to acquire ain frame in select mode\n");
			break;
		}

		if (FD_ISSET(adec_fd, &read_fd)) {

			if (VS_SUCCESS != vs_mal_adec_frame_acquire(adec_chnid, &audio_frame, VS_FALSE)) {
				if (p_adec->is_start) {
					vs_sample_trace("break as vs_mal_adec_frame_acquire() fails\n");
				}

				break;
			}

#else
		{

			if (VS_SUCCESS != vs_mal_adec_frame_acquire(adec_chnid, &audio_frame, VS_TRUE)) {
				if (p_adec->is_start) {
					vs_sample_trace("break as vs_mal_adec_frame_acquire() fails\n");
				}

				break;
			}

#endif

			if (p_adec->send_to_ao) {
				if (VS_SUCCESS != vs_mal_aout_frame_send(p_adec->ao_devid, p_adec->ao_chnid, &audio_frame, -1)) {
					vs_sample_trace("break as vs_mal_aout_frame_send() fails\n");
					break;
				}
			}
			else if (p_adec->send_to_aenc) {
				if (VS_SUCCESS != vs_mal_aenc_frame_send(p_adec->aenc_chnid, &audio_frame, NULL)) {
					vs_sample_trace("break as vs_mal_aenc_frame_send() fails\n");
					break;
				}
			}
			else {
				if (fp) {
					write_audio_frame_to_file(&audio_frame, E_AUDIO_SAMPLE_BITWIDTH_16, fp);
				}
			}

			if (VS_SUCCESS != vs_mal_adec_frame_release(adec_chnid, &audio_frame)) {
				vs_sample_trace("vs_mal_adec_stream_release fails\n");
			}
		}
	}

	p_adec->is_start = VS_FALSE;

	if (fp) {
		fclose(fp);
	}

	vs_sample_trace("QUIT ADEC SOURCE THREAD\n");

	return NULL;
}

vs_int32_t sample_common_adec_source_thread_create(vs_int32_t adec_chnid, vs_int32_t ao_devid, vs_int32_t ao_chnid,
        vs_int32_t aenc_chnid, vs_bool_t send_to_file)
{
	pthread_attr_t           pthread_attr;
	struct sched_param       param;
	vs_thread_adec_source_s *p_adec;
	char date[16];

	p_adec = &s_thread_adec_source[adec_chnid];
	p_adec->is_start     = 1;
	p_adec->adec_chnid   = adec_chnid;
	p_adec->ao_devid     = ao_devid;
	p_adec->ao_chnid     = ao_chnid;
	p_adec->send_to_ao   = (ao_devid != -1) ? VS_TRUE : VS_FALSE;
	p_adec->send_to_file = send_to_file;
	p_adec->send_to_aenc = (aenc_chnid != -1) ? VS_TRUE : VS_FALSE;
	p_adec->aenc_chnid   = aenc_chnid;

	if (p_adec->send_to_file) {
		get_date_str(date);
		snprintf(p_adec->ofile_name, sizeof(p_adec->ofile_name), "./audio_ch%d_%s.pcm", adec_chnid, date);
	}
	else {
		p_adec->ofile_name[0] = '\0';
	}

	// init policy;
	pthread_attr_init(&pthread_attr);
	param.sched_priority = 90;
	pthread_attr_setschedpolicy(&pthread_attr, SCHED_FIFO);
	pthread_attr_setschedparam(&pthread_attr, &param);
	pthread_attr_setinheritsched(&pthread_attr, PTHREAD_EXPLICIT_SCHED);

	pthread_create(&p_adec->pid, &pthread_attr, adec_source_thread, p_adec);
	//pthread_create(&p_adec->pid, NULL, adec_source_thread, p_adec);

	return 0;
}

vs_int32_t sample_common_adec_source_thread_destroy(vs_int32_t adec_chnid)
{
	vs_thread_adec_source_s *p_adec;

	vs_sample_trace("To Destory ADEC(%d) Source Thread\n", adec_chnid);

	p_adec = &s_thread_adec_source[adec_chnid];

	if (p_adec->is_start) {
		p_adec->is_start = 0;
		pthread_join(p_adec->pid, 0);
	}

	return 0;
}

static void* adec_sink_thread(void *p_arg)
{
	vs_thread_adec_sink_s *p_adec = (vs_thread_adec_sink_s *)p_arg;
	vs_int32_t             adec_chnid;
	vs_audio_stream_s      audio_stream = {0};
	FILE                  *fp;
	vs_int32_t             read_size;
	vs_int32_t             real_size = 0;
	vs_int32_t             aac_frame_length;
	vs_int32_t             aac_head_len = 6;
	vs_uint8_t             *p_tmp;

	if (!(fp = p_adec->fp)) {
		vs_sample_trace("fp is NULL, use preload audio data\n");
		reset_audio_data_file();
	}

	adec_chnid = p_adec->adec_chnid;

	if (!(audio_stream.p_stream = (vs_uint8_t *)malloc(2048 * 4))) {
		vs_sample_trace("Fail to allocate dynamic memory\n");
		return NULL;
	}

	while (p_adec->is_start) {
		if (p_adec->decoder_type == E_PT_TYPE_AAC) {
			read_size = aac_head_len;

			if (fp) {
				real_size = fread(audio_stream.p_stream, 1, read_size, fp);
			}
			else {
				real_size = read_pcm_data((char *)audio_stream.p_stream, read_size);
			}

			if (real_size == read_size) {
				p_tmp = audio_stream.p_stream;

				if (0x0fff != ((p_tmp[0] << 8) | p_tmp[1]) >> 4) {
					vs_sample_trace("frame head error, %02x %02x\n", p_tmp[0], p_tmp[1]);
					break;
				}

				aac_frame_length = ((p_tmp[3] & 0x3) << 11) | (p_tmp[4] << 3) | (p_tmp[5] >> 5);

				//printf("aac_frame_length = %d %x\n", aac_frame_length, aac_frame_length);
				read_size = aac_frame_length - aac_head_len;

				if (fp) {
					real_size = fread(audio_stream.p_stream + aac_head_len, 1, read_size, fp);
				}
				else {
					real_size = read_pcm_data((char *)audio_stream.p_stream + aac_head_len, read_size);
				}

				if (real_size == read_size) {
					read_size = aac_frame_length;
					real_size = aac_frame_length;
				}
			}
		}
		else {
			switch (p_adec->decoder_type) {
			case E_PT_TYPE_LPCM:
				read_size = 4096;
				break;
			case E_PT_TYPE_G711A:
			case E_PT_TYPE_G711U:
				read_size = 480 + 4;
				break;
			case E_PT_TYPE_G726:
				read_size = 480 * 5 / 8 + 4;
				break;
			case E_PT_TYPE_ADPCMA:
				read_size = 480 / 2 + 8;
				break;
			default:
				vs_sample_trace("audio decoder<%d> is not supported\n", p_adec->decoder_type);
				return NULL;
			}

			if (fp) {
				real_size = fread(audio_stream.p_stream, 1, read_size, fp);
			}
			else {
				real_size = read_pcm_data((char *)audio_stream.p_stream, read_size);
			}
		}

		if (real_size != read_size) {
			if (VS_SUCCESS != vs_mal_adec_eos_set(adec_chnid, VS_FALSE)) {
				vs_sample_trace("Fail to set ADEC(chn%d) EOS, error_code\n", adec_chnid);
			}

			if (fp) {
				fseek(fp, 0, SEEK_SET); /* rewind */
			}

			continue;
		}

		audio_stream.stream_length = real_size;

		if (VS_SUCCESS != vs_mal_adec_stream_send(adec_chnid, &audio_stream, VS_TRUE)) {
			vs_sample_trace("Fail to send ADEC(chn%d) stream\n", adec_chnid);
			break;
		}
	}

	free(audio_stream.p_stream);

	if (fp) {
		fclose(fp);
	}

	p_adec->is_start = VS_FALSE;

	vs_sample_trace("QUIT ADEC SINK THREAD\n");

	return NULL;
}

vs_int32_t sample_common_adec_sink_thread_create(vs_int32_t adec_chnid, vs_payload_type_e decoder_type, FILE *fp)
{
	pthread_attr_t          pthread_attr;
	struct sched_param      param;
	vs_thread_adec_sink_s  *p_adec;

	p_adec = &s_thread_adec_sink[adec_chnid];
	p_adec->is_start   = 1;
	p_adec->adec_chnid = adec_chnid;
	p_adec->fp         = fp;
	p_adec->decoder_type = decoder_type;
	// init policy;
	pthread_attr_init(&pthread_attr);
	param.sched_priority = 90;
	pthread_attr_setschedpolicy(&pthread_attr, SCHED_FIFO);
	pthread_attr_setschedparam(&pthread_attr, &param);
	pthread_attr_setinheritsched(&pthread_attr, PTHREAD_EXPLICIT_SCHED);

	pthread_create(&p_adec->pid, &pthread_attr, adec_sink_thread, p_adec);
	//pthread_create(&p_adec->pid, NULL, adec_sink_thread, p_adec);

	return 0;
}

vs_int32_t sample_common_adec_sink_thread_destroy(vs_int32_t adec_chnid)
{
	vs_thread_adec_sink_s *p_adec;

	vs_sample_trace("To Destory ADEC(%d) Sink Thread\n", adec_chnid);

	p_adec = &s_thread_adec_sink[adec_chnid];

	if (p_adec->is_start) {
		p_adec->is_start = 0;
		pthread_join(p_adec->pid, 0);
	}

	return 0;
}

vs_int32_t sample_common_adec_start(vs_int32_t adec_chnid, vs_payload_type_e decoder_type)
{
	vs_adec_attr_s       adec_chn_attr = {0};
	vs_adec_lpcm_attr_s  adec_lpcm = {0};
	vs_adec_g711_attr_s  adec_g711a = {0};
	vs_adec_g711_attr_s  adec_g711u = {0};
	vs_adec_g726_attr_s  adec_g726 = {0};
	vs_adec_adpcm_attr_s adec_adpcm = {0};
	vs_adec_aac_attr_s   adec_aac = {0};
	vs_int32_t           retval;

	adec_chn_attr.decoder_type      = decoder_type;
	adec_chn_attr.pool_buffer_depth = 16;
	adec_chn_attr.in_stream_mode    = E_ADEC_STREAM_MODE_FRAME;

	switch (decoder_type) {
	case E_PT_TYPE_LPCM:
		adec_chn_attr.p_attr = &adec_lpcm;
		adec_chn_attr.in_stream_mode = E_ADEC_STREAM_MODE_FRAME;
		break;
	case E_PT_TYPE_G711A:
		adec_chn_attr.p_attr = &adec_g711a;
		adec_chn_attr.in_stream_mode = E_ADEC_STREAM_MODE_STREAM;
		break;
	case E_PT_TYPE_G711U:
		adec_chn_attr.p_attr = &adec_g711u;
		adec_chn_attr.in_stream_mode = E_ADEC_STREAM_MODE_STREAM;
		break;
	case E_PT_TYPE_G726:
		adec_g726.g726_bps = E_AUDIO_G726_40K;
		adec_chn_attr.p_attr = &adec_g726;
		adec_chn_attr.in_stream_mode = E_ADEC_STREAM_MODE_STREAM;
		break;
	case E_PT_TYPE_ADPCMA:
		adec_adpcm.adpcm_type = E_AUDIO_ADPCM_TYPE_DVI4;
		adec_chn_attr.p_attr = &adec_adpcm;
		adec_chn_attr.in_stream_mode = E_ADEC_STREAM_MODE_STREAM;
		break;
	case E_PT_TYPE_AAC:
		adec_aac.trans_type = E_AUDIO_AAC_TRANS_TYPE_ADTS;
		adec_chn_attr.p_attr = &adec_aac;
		adec_chn_attr.in_stream_mode = E_ADEC_STREAM_MODE_FRAME;
		break;
	default:
		vs_sample_trace("Invalid decoder type<%d>\n", decoder_type);
		break;
	}

	printf("audio decode mode: %s\n", adec_chn_attr.in_stream_mode == E_ADEC_STREAM_MODE_STREAM ? "STREAM" : "FRAME");

	if (VS_SUCCESS != (retval = vs_mal_adec_create(adec_chnid, &adec_chn_attr))) {
		vs_sample_trace("Fail to create ADEC channel(%d), error_code(0x%x)\n", adec_chnid, retval);
		return retval;
	}

	return VS_SUCCESS;
}

vs_int32_t sample_common_adec_stop(vs_int32_t adec_chnid)
{
	vs_int32_t retval;

	vs_sample_trace("To STOP ADEC Channel(%d)\n", adec_chnid);

	if (VS_SUCCESS != (retval = vs_mal_adec_destroy(adec_chnid))) {
		vs_sample_trace("Fail to create ADEC channel(%d), error_code(0x%x)\n", adec_chnid, retval);
		return retval;
	}

	return VS_SUCCESS;
}

static vs_int32_t g_acodec_fd = -1;


#define CHECK_OPENED() do{ if (g_acodec_fd <=0) {g_acodec_fd = open(VS_ACODEC_DEVICE_PATH, O_RDWR);} \
		if (g_acodec_fd <= 0) {vs_sample_trace("Fail to Open ACODEC DRV\n"); return 0;} \
	} while(0)


vs_int32_t sample_common_acodec_gain_get(void)
{
	vs_int32_t input_gain, output_gain, mic_boost, mic_gain, adc_gain, dac_gain, hp_gain;

	CHECK_OPENED();

	ioctl(g_acodec_fd, VS_ACODEC_INPUT_VOLUME_GET, &input_gain);
	vs_sample_trace("[ACODEC] input total gain: %ddB\n", input_gain);

	ioctl(g_acodec_fd, VS_ACODEC_MICL_BOOST_GET, &mic_boost);

	switch (mic_boost) {
	case 0:
		mic_boost = 0;
		break;
	case 1:
		mic_boost = 6;
		break;
	case 2:
		mic_boost = 20;
		break;
	case 3:
		mic_boost = 30;
		break;
	default:
		break;
	}

	vs_sample_trace("[ACODEC] MICL boost gain: %ddB\n", mic_boost);
	ioctl(g_acodec_fd, VS_ACODEC_MICR_BOOST_GET, &mic_boost);

	switch (mic_boost) {
	case 0:
		mic_boost = 0;
		break;
	case 1:
		mic_boost = 6;
		break;
	case 2:
		mic_boost = 20;
		break;
	case 3:
		mic_boost = 30;
		break;
	default:
		break;
	}

	vs_sample_trace("[ACODEC] MICR boost gain: %ddB\n", mic_boost);

	ioctl(g_acodec_fd, VS_ACODEC_MICL_GAIN_GET, &mic_gain);
	mic_gain = -18 * 2 + mic_gain * 3; /* Q1 */
	vs_sample_trace("[ACODEC] MICL gain: %d.%ddB\n", (mic_gain / 2), (mic_gain & 0x1) * 10 / 2);

	ioctl(g_acodec_fd, VS_ACODEC_MICR_GAIN_GET, &mic_gain);
	mic_gain = -18 * 2 + mic_gain * 3; /* Q1 */
	vs_sample_trace("[ACODEC] MICR gain: %d.%ddB\n", (mic_gain / 2), (mic_gain & 0x1) * 10 / 2);

	ioctl(g_acodec_fd, VS_ACODEC_ADCL_VOLUME_GET, &adc_gain);
	adc_gain = (adc_gain - 1) - 97 * 2;
	vs_sample_trace("[ACODEC] ADCL volume: %d.%ddB\n", (adc_gain / 2), (adc_gain & 0x1) * 10 / 2);

	ioctl(g_acodec_fd, VS_ACODEC_ADCR_VOLUME_GET, &adc_gain);
	adc_gain = (adc_gain - 1) - 97 * 2;
	vs_sample_trace("[ACODEC] ADCR volume: %d.%ddB\n", (adc_gain / 2), (adc_gain & 0x1) * 10 / 2);

	ioctl(g_acodec_fd, VS_ACODEC_OUTPUT_VOLUME_GET, &output_gain);
	vs_sample_trace("[ACODEC] output total gain: %ddB\n", output_gain);

	ioctl(g_acodec_fd, VS_ACODEC_HPOUTL_GAIN_GET, &hp_gain);
	hp_gain = -39 * 2 + 3 * hp_gain;
	vs_sample_trace("[ACODEC] HPL gain: %d.%ddB\n", (hp_gain / 2), (hp_gain & 0x1) * 10 / 2);

	ioctl(g_acodec_fd, VS_ACODEC_HPOUTR_GAIN_GET, &hp_gain);
	hp_gain = -39 * 2 + 3 * hp_gain;
	vs_sample_trace("[ACODEC] HPR gain: %d.%ddB\n", (hp_gain / 2), (hp_gain & 0x1) * 10 / 2);


	ioctl(g_acodec_fd, VS_ACODEC_DAC_VOLUME_GET, &dac_gain);
	dac_gain = (dac_gain - 1) - 120 * 2;
	vs_sample_trace("[ACODEC] DAC volume: %d.%ddB\n", (dac_gain / 2), (dac_gain & 0x1) * 10 / 2);

	return 0;
}

vs_int32_t sample_common_acodec_set(vs_int32_t input_gain, vs_int32_t output_gain, vs_int32_t mic_boost, vs_int32_t mic_gain, vs_int32_t adc_gain,
                                    vs_int32_t dac_gain, vs_int32_t hp_gain)
{
#define NA_GAIN     (999)
	vs_int32_t reg_val;

	CHECK_OPENED();

	if (input_gain != NA_GAIN) {
		mic_gain  = NA_GAIN;
		mic_boost = NA_GAIN;
		adc_gain  = NA_GAIN;

		ioctl(g_acodec_fd, VS_ACODEC_INPUT_VOLUME_SET, &input_gain);
	}

	if (output_gain != NA_GAIN) {
		dac_gain = NA_GAIN;
		hp_gain  = NA_GAIN;

		ioctl(g_acodec_fd, VS_ACODEC_OUTPUT_VOLUME_SET, &output_gain);
	}

	if (mic_boost != NA_GAIN) {

		if (mic_boost == 0 || mic_boost == 6 || mic_boost == 20 || mic_boost == 30) {
			reg_val = 0;

			if (mic_boost == 6) {
				reg_val = 1;
			}
			else if (mic_boost == 20) {
				reg_val = 2;
			}
			else if (mic_boost == 30) {
				reg_val = 3;
			}

			ioctl(g_acodec_fd, VS_ACODEC_MICL_BOOST_SET, &reg_val);
			ioctl(g_acodec_fd, VS_ACODEC_MICR_BOOST_SET, &reg_val);

				reg_val = 7;
				ioctl(g_acodec_fd, VS_ACODEC_SET_MIC_BIAS_VOLTAGE, &reg_val);

			} else {
				vs_sample_trace("ERROR: Invalid MIC boost gain<%d>, should be [0, 6, 20, 30]\n", mic_boost);
				return VS_ERR_AUDIO_INVALID_PARAM;
			}
		}

	if (mic_gain != NA_GAIN) {
		/* 29dB is regarded as 28.5dB */
		if (mic_gain >= -18 && mic_gain <= 29) {
			if (mic_gain != 29) {
				reg_val = (mic_gain + 18) * 2 / 3;
			}
			else {
				reg_val = 31;
			}

			ioctl(g_acodec_fd, VS_ACODEC_MICL_GAIN_SET, &reg_val);
			ioctl(g_acodec_fd, VS_ACODEC_MICR_GAIN_SET, &reg_val);
		}
		else {
			vs_sample_trace("ERROR: Invalid MIC gain<%d>, should be [-18, 28.5]\n", mic_gain);
			return VS_ERR_AUDIO_INVALID_PARAM;
		}
	}

	if (adc_gain != NA_GAIN) {
		if (adc_gain >= -97 && adc_gain <= 30) {
			adc_gain = 0xff - (30 - adc_gain) * 2;
			ioctl(g_acodec_fd, VS_ACODEC_ADCL_VOLUME_SET, &adc_gain);
			ioctl(g_acodec_fd, VS_ACODEC_ADCR_VOLUME_SET, &adc_gain);
		}
		else {
			vs_sample_trace("ERROR: Invalid ADC volume<%d>, should be [-97, 30]\n", adc_gain);
			return VS_ERR_AUDIO_INVALID_PARAM;
		}
	}

	if (dac_gain != NA_GAIN) {
		if (dac_gain < -120) {
			dac_gain = -120;
		}
		else if (dac_gain > 7) {
			dac_gain = 7;
		}

		dac_gain = 1 + (dac_gain + 120) * 2;
		ioctl(g_acodec_fd, VS_ACODEC_DAC_VOLUME_SET, &dac_gain);
	}

	if (hp_gain != NA_GAIN) {
		if (hp_gain < -39) {
			hp_gain = -39;
		}
		else if (hp_gain > 6) {
			hp_gain = 6;
		}

		hp_gain = (hp_gain + 39) * 2 / 3;
		ioctl(g_acodec_fd, VS_ACODEC_HPOUTL_GAIN_SET, &hp_gain);
		ioctl(g_acodec_fd, VS_ACODEC_HPOUTR_GAIN_SET, &hp_gain);
	}

	return 0;
}

	vs_int32_t sample_common_acodec_alc_set(void *attr)
	{
		vs_acodec_agc_attr_s *attr_inner = (vs_acodec_agc_attr_s *)attr;
		CHECK_OPENED();

		vs_sample_trace("INFO: agc_enable, enable[%d], samplerate[%d], alc_vol[%f], max_level[%d], min_level[%d]\n",
				attr_inner->enable,
				attr_inner->sample_rate,
				attr_inner->alc_vol,
				attr_inner->max_level,
				attr_inner->min_level);
		return ioctl(g_acodec_fd, VS_ACODEC_ENABLE_AGC, attr_inner);
	}


static void* rtc_aout_thread(void *p_arg)
{
	vs_thread_aout_sink_s *p_aout = (vs_thread_aout_sink_s *)p_arg;
	vs_int32_t             ao_devid;
	vs_int32_t             ao_chnid;
	vs_audio_frame_s       audio_frame = {0};
	fd_set                 write_fd;
	vs_int32_t             aout_fd;
	struct timeval         timeout;
	vs_int32_t             retval;
	vs_uint8_t             lr_buf[640] = {0};

	ao_devid = 0;
	ao_chnid = 0;
	audio_frame.frame_chn_bytes = 320;
	audio_frame.sound_mode      = E_AUDIO_SOUND_MODE_STEREO;
	audio_frame.sample_width    = E_AUDIO_SAMPLE_BITWIDTH_16;
	audio_frame.p_virt_addr[0] = &lr_buf[0];
	audio_frame.p_virt_addr[1] = &lr_buf[320];

	aout_fd = vs_mal_aout_fd_get(ao_devid, ao_chnid);

	while (p_aout->is_start) {
		FD_ZERO(&write_fd);
		FD_SET(aout_fd, &write_fd);
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;
		retval = select(aout_fd + 1, NULL, &write_fd, NULL, &timeout);
		if (retval < 0) {
			vs_sample_trace("Error return from select()\n");
		}
		else if (0 == retval) {
			vs_sample_trace("Timeout to get space in select mode\n");
		}
		else {
			if (FD_ISSET(aout_fd, &write_fd)) {
				if (jb_is_empty(&s_aout_buf)) {
					memset(audio_frame.p_virt_addr[0], 0, s_aout_buf.frame_size);
					memset(audio_frame.p_virt_addr[1], 0, s_aout_buf.frame_size);
					if (VS_SUCCESS != vs_mal_aout_frame_send(ao_devid, ao_chnid, &audio_frame, 0)) {
						vs_sample_trace("vs_mal_aout_frame_send() fails in select mode\n");
					}
				} else {
					memcpy(audio_frame.p_virt_addr[0], jb_read_address(&s_aout_buf), s_aout_buf.frame_size);
					memcpy(audio_frame.p_virt_addr[1], jb_read_address(&s_aout_buf), s_aout_buf.frame_size);
					if (VS_SUCCESS != vs_mal_aout_frame_send(ao_devid, ao_chnid, &audio_frame, 0)) {
						vs_sample_trace("vs_mal_aout_frame_send() fails in select mode\n");
					}	else {
						// vs_sample_trace("vs_mal_aout_frame_send() success\n");
						jb_remove_data(&s_aout_buf);
					}
				}
			}
		}
	}

	p_aout->is_start = VS_FALSE;

	vs_sample_trace("QUIT AOUT RTC THREAD\n");

	return NULL;
}

static void* rtc_ain_thread(void *p_arg)
{
	vs_thread_ain_source_s *p_ain = (vs_thread_ain_source_s *)p_arg;
	vs_audio_frame_s	audio_frame = {0};
	vs_audio_aecframe_s aec_frame = {0};
	vs_int32_t			ai_devid;
	vs_int32_t			ai_chnid;
	fd_set				read_fd;
	vs_int32_t			ain_fd;
	struct timeval		timeout;
	vs_int32_t          timeout_aec = -1;
	vs_int32_t			retval;
	FILE               *fp_aqe = NULL;
	static unsigned char s_inbuf[320] = {0};
	memset(s_inbuf, 0, sizeof(s_inbuf));

	ai_devid = 0;
	ai_chnid = 0;
	ain_fd = vs_mal_ain_fd_get(ai_devid, ai_chnid);

	if (!(fp_aqe = fopen("ain_aqe.pcm", "wb"))) {
		vs_sample_trace("Fail to create file (%s)\n", "ain_aqe.pcm");
	}
	else {
		vs_sample_trace("Save AIN file to %s\n", "ain_aqe.pcm");
	}

	while (p_ain->is_start) {
		FD_ZERO(&read_fd);
		FD_SET(ain_fd, &read_fd);
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;
		retval = select(ain_fd + 1, &read_fd, NULL, NULL, &timeout);
		if (retval < 0) {
			vs_sample_trace("Error return from select()\n");
		}
		else if (0 == retval) {
			vs_sample_trace("timeout to acquire ain frame in select mode\n");
		}
		else {
			if (FD_ISSET(ain_fd, &read_fd)) {
				if (VS_SUCCESS != vs_mal_ain_frame_acquire(ai_devid, ai_chnid, &audio_frame, &aec_frame, timeout_aec)) {
					vs_sample_trace("vs_mal_ain_frame_acquire() fails in select mode\n");
				}
				else {
					if (fp_aqe) {
						write_audio_frame_to_file(&audio_frame, E_AUDIO_SAMPLE_BITWIDTH_16, fp_aqe);
					}
					assert(audio_frame.frame_chn_bytes == 320);
					memcpy(s_inbuf, audio_frame.p_virt_addr[0], audio_frame.frame_chn_bytes);
					if (!jb_is_full(&s_ain_buf)) {
						jb_push_data(&s_ain_buf, s_inbuf, s_ain_buf.frame_size);
					}
					else {
						jb_clear_data(&s_ain_buf);
						jb_push_data(&s_ain_buf, s_inbuf, s_ain_buf.frame_size);
						vs_sample_trace("Input Buffer Overlaid\n");
					}
					if (VS_SUCCESS != vs_mal_ain_frame_release(ai_devid, ai_chnid, &audio_frame, &aec_frame)) {
						vs_sample_trace("vs_mal_ain_frame_release fails\n");
					}
				}
			}
		}
	}

	p_ain->is_start = VS_FALSE;

	if (fp_aqe) {
		fclose(fp_aqe);
	}

	vs_sample_trace("QUIT AIN RTC THREAD\n");

	return NULL;

}

vs_int32_t sample_common_rtc_aout_thread_create(vs_int32_t ao_devid, vs_int32_t ao_chnid, vs_aiodev_attr_s *p_aio_common_attr)
{
	pthread_attr_t          pthread_attr;
	struct sched_param      param;
	vs_thread_aout_sink_s  *p_aout;

	p_aout = &s_thread_aout_sink[ao_devid][ao_chnid];
	p_aout->is_start   = 1;
	p_aout->ao_devid   = ao_devid;
	p_aout->ao_chnid   = ao_chnid;
	memcpy(&p_aout->aio_attr, p_aio_common_attr, sizeof(*p_aio_common_attr));

	pthread_attr_init(&pthread_attr);
	param.sched_priority = 90;
	pthread_attr_setschedpolicy(&pthread_attr, SCHED_FIFO);
	pthread_attr_setschedparam(&pthread_attr, &param);
	pthread_attr_setinheritsched(&pthread_attr, PTHREAD_EXPLICIT_SCHED);

	pthread_create(&p_aout->pid, &pthread_attr, rtc_aout_thread, p_aout);

	return 0;
}

vs_int32_t sample_common_rtc_ain_thread_create(vs_int32_t ai_devid, vs_int32_t ai_chnid)
{
	pthread_attr_t          pthread_attr;
	struct sched_param      param;
	vs_thread_ain_source_s  *p_ain;

	p_ain = &s_thread_ain_source[ai_devid][ai_chnid];
	p_ain->is_start   = 1;
	p_ain->ai_devid   = ai_devid;
	p_ain->ai_chnid   = ai_chnid;

	pthread_attr_init(&pthread_attr);
	param.sched_priority = 90;
	pthread_attr_setschedpolicy(&pthread_attr, SCHED_FIFO);
	pthread_attr_setschedparam(&pthread_attr, &param);
	pthread_attr_setinheritsched(&pthread_attr, PTHREAD_EXPLICIT_SCHED);

	pthread_create(&p_ain->pid, &pthread_attr, rtc_ain_thread, p_ain);

	return 0;
}

vs_int32_t sample_common_rtc_aout_thread_destroy()
{
	vs_thread_aout_sink_s *p_aout;

	p_aout = &s_thread_aout_sink[0][0];

	if (p_aout->is_start) {
		p_aout->is_start = 0;
		pthread_join(p_aout->pid, 0);
	}

	return 0;
}

vs_int32_t sample_common_rtc_ain_thread_destroy()
{
	vs_thread_ain_source_s *p_ain;

	p_ain = &s_thread_ain_source[0][0];

	if (p_ain->is_start) {
		p_ain->is_start = 0;
		pthread_join(p_ain->pid, 0);
	}

	return 0;
}

#ifdef __cplusplus
}
#endif
