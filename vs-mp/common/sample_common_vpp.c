/**
* @file sample_common_vpp.c
* @brief implement the common functions of vpp example.
* @details
* @author Visinex Software Group
* @date 2022-05-25
* @version v1.00
* @Copyright (C) 2022, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

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

sample_vpp_thread_param_s g_vpp_thread;
vs_bool_t g_rot_enable[VPP_MAX_PHYCHN_NUM] = {VS_FALSE, VS_FALSE, VS_FALSE, VS_FALSE};

static vs_int32_t sample_common_vpp_rotation_set(vs_int32_t vpp_grpid, vs_int32_t chnid)
{
	vs_rotation_attr_s rot_attr = {0};

	rot_attr.enable = VS_TRUE;
	rot_attr.mode = E_ROTATION_MODE_FIXED;
	rot_attr.fixed_rotation = E_FIXED_ROTATION_90;

	return vs_mal_vpp_chn_rotation_set(vpp_grpid, chnid, &rot_attr);
}

vs_int32_t sample_common_vpp_rotation_enable(vs_int32_t chnid)
{
	g_rot_enable[chnid] = VS_TRUE;
	return 0;
}

vs_int32_t sample_common_vpp_rotation_disable(vs_int32_t vpp_grpid, vs_int32_t chnid)
{
	vs_int32_t ret = VS_SUCCESS;
	vs_rotation_attr_s rot_attr = {0};

	ret = vs_mal_vpp_chn_rotation_get(vpp_grpid, chnid, &rot_attr);
	if (ret != VS_SUCCESS) {
		vs_sample_trace("vs_mal_vpp_chn_rotation_get(grp %d chn %d) failed with 0x%x\n", vpp_grpid, chnid, ret);
		return VS_FAILED;
	}
	rot_attr.enable = VS_FALSE;
	ret = vs_mal_vpp_chn_rotation_set(vpp_grpid, chnid, &rot_attr);
	if (ret != VS_SUCCESS) {
		vs_sample_trace("vs_mal_vpp_chn_rotation_set(grp %d chn %d) failed with 0x%x\n", vpp_grpid, chnid, ret);
		return VS_FAILED;
	}

	return ret;
}

vs_int32_t sample_common_vpp_start(vs_int32_t vpp_grpid, vs_bool_t* p_chn_enable, vs_vpp_grp_attr_s* p_vpp_grp_attr, vs_vpp_chn_attr_s* p_vpp_chn_attr)
{
	vs_int32_t ret;
	vs_int32_t i;

	ret = vs_mal_vpp_grp_create(vpp_grpid, p_vpp_grp_attr);
	if (ret != VS_SUCCESS) {
		vs_sample_trace("vs_mal_vpp_grp_create(grp %d) failed with 0x%x\n", vpp_grpid, ret);
		return VS_FAILED;
	}

	ret = vs_mal_vpp_grp_start(vpp_grpid);
	if (ret != VS_SUCCESS) {
		vs_sample_trace("vs_mal_vpp_grp_start(grp %d) failed with 0x%x\n", vpp_grpid, ret);
		return VS_FAILED;
	}

	for (i = 0; i < VPP_MAX_PHYCHN_NUM; i++) {
		if (p_chn_enable[i] == VS_TRUE) {
			ret = vs_mal_vpp_chn_attr_set(vpp_grpid, i, &p_vpp_chn_attr[i]);
			if (ret != VS_SUCCESS) {
				vs_sample_trace("vs_mal_vpp_chn_attr_set(grp %d chn %d) failed with 0x%x\n", vpp_grpid, i, ret);
				return VS_FAILED;
			}

			if (g_rot_enable[i]) {
				ret = sample_common_vpp_rotation_set(vpp_grpid, i);
				if (ret != VS_SUCCESS) {
					vs_sample_trace("vs_mal_vpp_chn_rotation_set(grp %d chn %d) failed with 0x%x\n", vpp_grpid, i, ret);
					return VS_FAILED;
				}
			}

			ret = vs_mal_vpp_chn_enable(vpp_grpid, i);
			if (ret != VS_SUCCESS) {
				vs_sample_trace("vs_mal_vpp_chn_enable(grp %d chn %d) failed with 0x%x\n", vpp_grpid, i, ret);
				return VS_FAILED;
			}
		}
	}

	return VS_SUCCESS;
}

vs_int32_t sample_common_vpp_lowlatency_start(vs_int32_t vpp_grpid, vs_bool_t* p_chn_enable, vs_vpp_grp_attr_s* p_vpp_grp_attr, vs_vpp_chn_attr_s* p_vpp_chn_attr)
{
	vs_int32_t ret;
	vs_int32_t i;

	ret = vs_mal_vpp_grp_create(vpp_grpid, p_vpp_grp_attr);
	if (ret != VS_SUCCESS) {
		vs_sample_trace("vs_mal_vpp_grp_create(grp %d) failed with 0x%x\n", vpp_grpid, ret);
		return VS_FAILED;
	}

	ret = vs_mal_vpp_grp_start(vpp_grpid);
	if (ret != VS_SUCCESS) {
		vs_sample_trace("vs_mal_vpp_grp_start(grp %d) failed with 0x%x\n", vpp_grpid, ret);
		return VS_FAILED;
	}

	for (i = 0; i < VPP_MAX_PHYCHN_NUM; i++) {
		if (p_chn_enable[i] == VS_TRUE) {
			ret = vs_mal_vpp_chn_attr_set(vpp_grpid, i, &p_vpp_chn_attr[i]);
			if (ret != VS_SUCCESS) {
				vs_sample_trace("vs_mal_vpp_chn_attr_set(grp %d chn %d) failed with 0x%x\n", vpp_grpid, i, ret);
				return VS_FAILED;
			}
		}
	}

	return VS_SUCCESS;
}

vs_int32_t sample_comm_vpp_extchn_attr_set(vs_int32_t vpp_grpid, vs_bool_t* p_chn_enable, vs_vpp_extchn_attr_s* p_vpp_chn_attr)
{
	vs_int32_t ret;
	vs_int32_t i;
	vs_int32_t ext_chnid;

	for (i = 0; i < VPP_MAX_EXTCHN_NUM; i++) {
		if (p_chn_enable[i] == VS_TRUE) {
			ext_chnid = i + VPP_MAX_PHYCHN_NUM;
			ret = vs_mal_vpp_extchn_attr_set(vpp_grpid, ext_chnid, &p_vpp_chn_attr[i]);
			if (ret != VS_SUCCESS) {
				vs_sample_trace("vs_mal_vpp_extchn_attr_set(grp %d chn %d) failed with 0x%x\n", vpp_grpid, ext_chnid, ret);
				return VS_FAILED;
			}

			ret = vs_mal_vpp_chn_enable(vpp_grpid, ext_chnid);
			if (ret != VS_SUCCESS) {
				vs_sample_trace("vs_mal_vpp_chn_enable(grp %d chn %d) failed with 0x%x\n", vpp_grpid, ext_chnid, ret);
				return VS_FAILED;
			}
		}
	}

	return VS_SUCCESS;
}

vs_int32_t sample_common_vpp_stop(vs_int32_t vpp_grpid, vs_bool_t* p_chn_enable)
{
	vs_int32_t ret;
	vs_int32_t i;

	vs_int32_t ext_chnid;
	vs_vpp_extchn_attr_s extchn_attr = {0};

	for (i = 0; i < VPP_MAX_EXTCHN_NUM; i++) {
		ext_chnid = i + VPP_MAX_PHYCHN_NUM;
		ret = vs_mal_vpp_extchn_attr_get(vpp_grpid, ext_chnid, &extchn_attr);
		if (ret != VS_SUCCESS) {
			vs_sample_trace("vs_mal_vpp_extchn_attr_get(grp %d  chn %d) failed with %x\n", vpp_grpid, ext_chnid, ret);
			return VS_FAILED;
		}
		if (extchn_attr.width != 0) {
			ret = vs_mal_vpp_chn_disable(vpp_grpid, ext_chnid);
			if (ret != VS_SUCCESS) {
				vs_sample_trace("vs_mal_vpp_chn_disable(grp %d chn %d) failed with 0x%x\n", vpp_grpid, ext_chnid, ret);
				return VS_FAILED;
			}
		}
	}

	for (i = 0; i < VPP_MAX_PHYCHN_NUM; i++) {
		if (p_chn_enable[i] == VS_TRUE) {
			ret = vs_mal_vpp_chn_disable(vpp_grpid, i);
			if (ret != VS_SUCCESS) {
				vs_sample_trace("vs_mal_vpp_chn_disable(grp %d chn %d) failed with 0x%x\n", vpp_grpid, i, ret);
				return VS_FAILED;
			}
		}
	}

	ret = vs_mal_vpp_grp_stop(vpp_grpid);
	if (ret != VS_SUCCESS) {
		vs_sample_trace("vs_mal_vpp_grp_stop(grp %d) failed with 0x%x\n", vpp_grpid, ret);
		return VS_FAILED;
	}

	ret = vs_mal_vpp_grp_destroy(vpp_grpid);
	if (ret != VS_SUCCESS) {
		vs_sample_trace("vs_mal_vpp_grp_destroy(grp %d) failed with 0x%x\n", vpp_grpid, ret);
		return VS_FAILED;
	}

	return VS_SUCCESS;
}

static vs_void_t *vpp_chn_acquire_task(vs_void_t *arg)
{
	vs_int32_t ret;
	sample_vpp_thread_param_s *p_vpp_thread;
	vs_vpp_extchn_attr_s vpp_extchn_attr;
	vs_int32_t vpp_grpid, vpp_chnid;
	vs_video_frame_info_s frame_info;

	p_vpp_thread = (sample_vpp_thread_param_s *)arg;

	vpp_grpid = p_vpp_thread->vpp_grpid;
	vpp_chnid = p_vpp_thread->vpp_chnid;

	ret = vs_mal_vpp_extchn_attr_get(vpp_grpid, vpp_chnid, &vpp_extchn_attr);
	if (ret != VS_SUCCESS) {
		vs_sample_trace("vs_mal_vpp_extchn_attr_get(grp %d  chn %d) failed with 0x%x\n", vpp_grpid, vpp_chnid, ret);
		return VS_NULL;
	}
	vpp_extchn_attr.depth = 2;
	ret = vs_mal_vpp_extchn_attr_set(vpp_grpid, vpp_chnid, &vpp_extchn_attr);
	if (ret != VS_SUCCESS) {
		vs_sample_trace("vs_mal_vpp_extchn_attr_set(grp %d  chn %d) failed with 0x%x\n", vpp_grpid, vpp_chnid, ret);
		return VS_NULL;
	}

	vs_sample_trace("start vpp grp %d chn %d frame acquire\n", vpp_grpid, vpp_chnid);

	while (p_vpp_thread->stop_task != VS_TRUE) {
		ret = vs_mal_vpp_chn_frame_acquire(vpp_grpid, vpp_chnid, &frame_info, 100);
		if (ret != VS_SUCCESS) {
			vs_sample_trace("vs_mal_vpp_chn_frame_acquire(grp %d  chn %d) failed with 0x%x\n", vpp_grpid, vpp_chnid, ret);
			continue;
		}
		vs_mal_vpp_chn_frame_release(vpp_grpid, vpp_chnid, &frame_info);
		usleep(30000);
	}
	return VS_NULL;
}

vs_int32_t sample_common_vpp_chn_acquire_start(vs_int32_t vpp_grpid, vs_int32_t vpp_chnid)
{
	memset(&g_vpp_thread, 0, sizeof(sample_vpp_thread_param_s));

	g_vpp_thread.vpp_grpid = vpp_grpid;
	g_vpp_thread.vpp_chnid = vpp_chnid;
	g_vpp_thread.stop_task = VS_FALSE;

	return pthread_create(&g_vpp_thread.vpp_pid, 0, vpp_chn_acquire_task, (vs_void_t*)&g_vpp_thread);
}

vs_int32_t sample_common_vpp_chn_acquire_stop()
{
	if (g_vpp_thread.stop_task == VS_FALSE) {
		g_vpp_thread.stop_task = VS_TRUE;
		vs_sample_trace("pthread_join vpp_pid[%lu]! \n", g_vpp_thread.vpp_pid);
		pthread_join(g_vpp_thread.vpp_pid, 0);
	}
	vs_sample_trace("sample_common_vpp_chn_acquire_stop!\n");
	return VS_SUCCESS;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

