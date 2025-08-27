/**
* @file sample_common_infra.c
* @brief common infra sample
* @details common infra sample
* @author visinex sw group
* @date 2024-11-09
* @version v0.01
* @Copyright (C) 2024, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <dlfcn.h>

#include "sample_common.h"
#include "vs_ref_infra_sensor_ctrl.h"
#include "vs_mal_infra.h"

#define INFRA_DEV_ID (2)

typedef vs_int32_t (*infra_sensor_attr_get)(vs_int32_t mode_id, vs_ref_infra_sensor_attr_s *p_attr);
typedef vs_int32_t (*infra_sensor_init)(vs_int32_t mode_id);
typedef vs_int32_t (*infra_sensor_deinit)(vs_int32_t mode_id);
typedef vs_int32_t (*infra_sensor_stream_on)(vs_int32_t mode_id);
typedef vs_int32_t (*infra_sensor_stream_off)(vs_int32_t mode_id);
typedef vs_int32_t (*infra_sensor_ooc_update)(vs_int32_t mode_id,
        vs_infra_ooc_cfg_s *p_ooc_cfg, vs_bool_t *is_inbound);
typedef vs_int32_t (*infra_sensor_frame_process)(vs_int32_t mode_id,
        vs_video_frame_info_s *p_src_frm, vs_video_frame_info_s *p_dst_frm);

typedef struct sample_infra_sensor_ops {
    vs_void_t *lib_handle;
    infra_sensor_attr_get attr_get;
    infra_sensor_init init;
    infra_sensor_deinit deinit;
    infra_sensor_stream_on stream_on;
    infra_sensor_stream_off stream_off;
    infra_sensor_ooc_update ooc_update;
    infra_sensor_frame_process frame_process;
} sample_infra_sensor_ops_s;

typedef enum sample_infra_status {
    E_INFRA_STATUS_UNINIT = 0,
    E_INFRA_STATUS_INIT,
    E_INFRA_STATUS_START,
    E_INFRA_STATUS_STOP,
    E_INFRA_STATUS_MAX
} sample_infra_status_e;

typedef struct sample_infra_ctx {
    sample_infra_cfg_s sample_cfg;
    sample_infra_sensor_ops_s sensor_ops;
    sample_infra_status_e status;
    vs_ref_infra_sensor_attr_s sensor_attr;
} sample_infra_ctx_s;

static sample_infra_ctx_s g_infra_ctx = {0};

static vs_int32_t sample_common_infra_sensor_ops_init(vs_char_t *sensor_name, sample_infra_sensor_ops_s *p_ops)
{
    char sensor_lib_name[128];

    sprintf(sensor_lib_name, "libinfrasnsr_%s.so", sensor_name);
    vs_sample_trace("infra-sensorname: %s\n", sensor_name);
    p_ops->lib_handle = dlopen(sensor_lib_name, RTLD_LAZY);
    if (p_ops->lib_handle == NULL) {
        vs_sample_trace("open %s library fail - %s\n", sensor_lib_name, dlerror());
        return VS_FAILED;
    }

    p_ops->attr_get = dlsym(p_ops->lib_handle, "vs_ref_infra_sensor_attr_get");
    p_ops->init = dlsym(p_ops->lib_handle, "vs_ref_infra_sensor_init");
    p_ops->deinit = dlsym(p_ops->lib_handle, "vs_ref_infra_sensor_deinit");
    p_ops->stream_on = dlsym(p_ops->lib_handle, "vs_ref_infra_sensor_stream_on");
    p_ops->stream_off = dlsym(p_ops->lib_handle, "vs_ref_infra_sensor_stream_off");
    p_ops->ooc_update = dlsym(p_ops->lib_handle, "vs_ref_infra_sensor_ooc_update");
    p_ops->frame_process = dlsym(p_ops->lib_handle, "vs_ref_infra_sensor_frame_process");

    if (p_ops->attr_get == NULL || p_ops->init == NULL
            || p_ops->deinit == NULL || p_ops->stream_on == NULL
            || p_ops->stream_off == NULL || p_ops->ooc_update == NULL
            || p_ops->frame_process == NULL) {
        vs_sample_trace(" open %s interface fail.\n", sensor_lib_name);
        return VS_FAILED;
    }

    return VS_SUCCESS;
}

static vs_int32_t sample_common_infra_attr_set(vs_int32_t mode_id,
        sample_infra_sensor_ops_s *p_ops, vs_ref_infra_sensor_attr_s *p_attr)
{
    vs_int32_t ret = VS_SUCCESS;

    // 1. get all the attribution from sensor lib
    ret = p_ops->attr_get(mode_id, p_attr);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("failed to get infra attr from sensor lib, ret=%d\n", ret);
        return ret;
    }

    // 2. set infra attr
    ret = vs_mal_infra_attr_set(INFRA_DEV_ID, &p_attr->infra_attr);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("failed to set infra attr, ret=0x%x\n", ret);
        return ret;
    }

    // 3. if tx of infra is enable, set ooc and iframe-config
    if (p_attr->infra_attr.tx_attr.enable) {
        ret = vs_mal_infra_ooc_config_set(INFRA_DEV_ID, &p_attr->ooc_cfg);
        if (ret != VS_SUCCESS) {
            vs_sample_trace("failed to set ooc, ret=0x%x\n", ret);
            return ret;
        }

        if (p_attr->array[0].cfg.p_cfg == NULL || p_attr->array[0].cfg.num == 0) {
            vs_sample_trace("bypass first iframe config\n");
        } else {
            ret = vs_mal_infra_iframe_config_set(INFRA_DEV_ID, &p_attr->array[0].cfg);
            if (ret != VS_SUCCESS) {
                vs_sample_trace("failed to set iframe config, ret=0x%x\n", ret);
                return ret;
            }
        }
    }

    // 4. init sensor lib
    ret = p_ops->init(mode_id);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("failed to init infra sensor\n");
    }

    return ret;
}

vs_void_t sample_common_infra_sensor_info_get(sample_infra_sensor_info_s *p_sensor_info)
{
    p_sensor_info->image_size = g_infra_ctx.sensor_attr.infra_attr.image_size;
    p_sensor_info->fps = g_infra_ctx.sensor_attr.infra_attr.fps;
}

vs_int32_t sample_common_infra_init(sample_infra_cfg_s *p_infra_cfg)
{
    vs_int32_t ret = VS_SUCCESS;

    if (p_infra_cfg == NULL) {
        vs_sample_trace("p_infra_cfg is NULL.\n");
        return VS_FAILED;
    }

    memset(&g_infra_ctx, 0x0, sizeof(sample_infra_ctx_s));
    ret = sample_common_infra_sensor_ops_init(p_infra_cfg->sensor_name,
            &g_infra_ctx.sensor_ops);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("failed to get sensor ops\n");
        return ret;
    }

    ret = sample_common_infra_attr_set(p_infra_cfg->mode_id,
            &g_infra_ctx.sensor_ops, &g_infra_ctx.sensor_attr);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("failed to init infra\n");
        return ret;
    }

    memcpy(&g_infra_ctx.sample_cfg, p_infra_cfg, sizeof(sample_infra_cfg_s));
    g_infra_ctx.status = E_INFRA_STATUS_INIT;
    return ret;
}

vs_int32_t sample_common_infra_deinit()
{
    vs_int32_t ret = VS_SUCCESS;

    if (g_infra_ctx.status == E_INFRA_STATUS_UNINIT) {
        vs_sample_trace("failed to deinit infra\n");
        return VS_FAILED;
    }

    ret = g_infra_ctx.sensor_ops.deinit(g_infra_ctx.sample_cfg.mode_id);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("failed to deinit infra sensor\n");
    }

    if (g_infra_ctx.sensor_ops.lib_handle) {
        dlclose(g_infra_ctx.sensor_ops.lib_handle);
        g_infra_ctx.sensor_ops.lib_handle = NULL;
    }
    g_infra_ctx.status = E_INFRA_STATUS_UNINIT;

    return ret;
}

vs_int32_t sample_common_infra_start()
{
    vs_int32_t ret = VS_SUCCESS;

    if (g_infra_ctx.status != E_INFRA_STATUS_INIT) {
        vs_sample_trace("invalid status %d\n", g_infra_ctx.status);
        return VS_FAILED;
    }

    // 6. trigger infra sensor stream on
    ret = g_infra_ctx.sensor_ops.stream_on(g_infra_ctx.sample_cfg.mode_id);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("failed to stream on infra sensor\n");
    }

    g_infra_ctx.status = E_INFRA_STATUS_START;
    return ret;
}

vs_int32_t sample_common_infra_stop()
{
    vs_int32_t ret = VS_SUCCESS;

    if (g_infra_ctx.status != E_INFRA_STATUS_START) {
        vs_sample_trace("invalid status %d\n", g_infra_ctx.status);
        return VS_FAILED;
    }

    ret = g_infra_ctx.sensor_ops.stream_off(g_infra_ctx.sample_cfg.mode_id);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("failed to stream off infra sensor\n");
    }
    g_infra_ctx.status = E_INFRA_STATUS_STOP;

    return ret;
}

vs_int32_t sample_common_infra_run(sample_infra_info_type_e info, vs_task_attr_s *p_video_frame_task)
{
    vs_int32_t ret = VS_SUCCESS;
    vs_bool_t is_inbound = VS_FALSE;
    vs_infra_ooc_cfg_s in_bound_ooc = {0};

    if (g_infra_ctx.status != E_INFRA_STATUS_START){
        vs_sample_trace("invalid status %d\n", g_infra_ctx.status);
        return VS_FAILED;
    }

    if (info & E_INFRA_INFO_IFRAME) {
        // update the second iframe cfg
        if (g_infra_ctx.sensor_attr.array[1].cfg.num != 0
                && g_infra_ctx.sensor_attr.array[1].cfg.p_cfg != NULL) {
            usleep(g_infra_ctx.sensor_attr.array[1].delay_ms * 1000);
            ret = vs_mal_infra_iframe_config_set(INFRA_DEV_ID,
                    &g_infra_ctx.sensor_attr.array[1].cfg);
            if (ret != VS_SUCCESS) {
                vs_sample_trace("failed to set iframe config[1], ret=0x%x\n", ret);
                return ret;
            }
        }
    }

    if (info & E_INFRA_INFO_OOC) {
        ret = g_infra_ctx.sensor_ops.ooc_update(g_infra_ctx.sample_cfg.mode_id,
                &in_bound_ooc, &is_inbound);
        if (ret == VS_SUCCESS) {
            if (is_inbound) {
                // update inbound ooc through mal interface
                ret = vs_mal_infra_ooc_config_set(INFRA_DEV_ID, &in_bound_ooc);
                if (ret != VS_SUCCESS) {
                    vs_sample_trace("failed to set in bound ooc config, ret=0x%x\n", ret);
                }
            }
        }
    }

    if (info & E_INFRA_INFO_VIDEO_FRAME) {
        if (p_video_frame_task) {
            // process video frame
            ret = g_infra_ctx.sensor_ops.frame_process(g_infra_ctx.sample_cfg.mode_id,
                    &p_video_frame_task->src_frame, &p_video_frame_task->dst_frame);
            if (ret != VS_SUCCESS) {
                vs_sample_trace("failed to process video frame\n");
            }
        }
    }

    return ret;
}