/**
* @file sample_common_sys.c
* @brief implement the common functions of sys example.
* @details
* @author Visinex Software Group
* @date 2022-05-20
* @version v0.01
* @Copyright (C) 2022, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/
#include "sample_common.h"
#include "vs_mal_mmz.h"
#include "vs_mal_sys.h"
#include <unistd.h>
#include "string.h"

vs_int32_t sample_common_sys_init(vs_vb_cfg_s *vb_cfg)
{
    vs_int32_t ret;

    ret = vs_mal_sys_exit();
    if (ret != VS_SUCCESS) {
        vs_sample_trace("sys exit failed, ret: 0x%x.\n", ret);
        return VS_FAILED;
    }

    ret = vs_mal_vb_exit();
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vb exit failed, ret: 0x%x.\n", ret);
        return VS_FAILED;
    }

    // sensor reset & unreset
    vs_mal_mipi_rx_sensor_reset(0);
    usleep(500);
    vs_mal_mipi_rx_sensor_unreset(0);

    ret = vs_mal_vb_cfg_set(vb_cfg);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("set vb cfg failed, ret: 0x%x.\n", ret);
        return VS_FAILED;
    }

    ret = vs_mal_vb_init();
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vb init failed, ret: 0x%x.\n", ret);
        vs_mal_sys_exit();
        return VS_FAILED;
    }

    ret = vs_mal_sys_init();
    if (ret != VS_SUCCESS) {
        vs_sample_trace("sys init failed, ret: 0x%x.\n", ret);
        vs_mal_sys_exit();
        vs_mal_vb_exit();
        return VS_FAILED;
    }

    return VS_SUCCESS;
}

vs_void_t sample_common_sys_exit(vs_void_t)
{
    vs_int32_t ret;

    ret = vs_mal_sys_exit();
    if (ret != VS_SUCCESS) {
        vs_sample_trace("sys exit failed, ret: 0x%x.\n", ret);
    }

    ret = vs_mal_vb_exit();
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vb exit failed, ret: 0x%x.\n", ret);
    }
}


vs_int32_t sample_common_dsp_init(vs_uint32_t dsp_id, vs_char_t* name)
{
#if !defined(VS_815) && !defined(VS_816)
    vs_int32_t ret;

    vs_mal_dsp_disable(dsp_id);
    vs_mal_dsp_poweroff(dsp_id);

    ret = vs_mal_dsp_poweron(dsp_id);
    if(ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_dsp_poweron fail, ret: 0x%x\n",ret);
        goto exit0;
    }
    ret = vs_mal_dsp_load(dsp_id, name);
    if(ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_dsp_load fail,ret: 0x%x\n",ret);
        goto exit1;
    }
    ret = vs_mal_dsp_enable(dsp_id);
    if(ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_dsp_enable fail,ret: 0x%x\n",ret);
        goto exit1;
    }
    return VS_SUCCESS;
exit1:
    vs_mal_dsp_poweroff(dsp_id);
exit0:
    return ret;
#endif

    return VS_SUCCESS;
}

vs_void_t sample_common_dsp_exit(vs_uint32_t dsp_id)
{
#if !defined(VS_815) && !defined(VS_816)
    vs_int32_t ret;

    ret = vs_mal_dsp_disable(dsp_id);
    if(ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_dsp_disable fail,ret: 0x%x\n",ret);
    }
    ret = vs_mal_dsp_poweroff(dsp_id);
    if(ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_dsp_poweroff fail, ret: 0x%x\n",ret);
    }
#endif
}

vs_int32_t sample_common_dsp_support(vs_int32_t dsp_id, vs_bool_t *p_support)
{
    const char* p_chipid = (const char*)vs_mal_sys_chipid_get();
    vs_bool_t dsp1_support = VS_TRUE;
    vs_bool_t dsp0_support = VS_TRUE;

    if ( !p_support ){
        vs_sample_trace("argument(p_support) is NULL\n")
        return VS_FAILED;
    }

    if ( dsp_id < 0 || dsp_id > 1 ){
        vs_sample_trace("invalid dsp_id[%d], should be 0/1\n", dsp_id);
        return VS_FAILED;
    }

    //vs_sample_trace("chipid[%s] \n", p_chipid);
    if ( dsp_id == 1 ){
        if (strncmp(p_chipid, VS819_CHIP_ID, strlen(VS819_CHIP_ID)) == 0
            || strncmp(p_chipid, VS819L_CHIP_ID, strlen(VS819L_CHIP_ID)) == 0
            || strncmp(p_chipid, VS816_CHIP_ID, strlen(VS816_CHIP_ID)) == 0
            || strncmp(p_chipid, VS815_CHIP_ID, strlen(VS815_CHIP_ID)) == 0) {
                dsp1_support = VS_FALSE;
        }

        *p_support = dsp1_support;

        return VS_SUCCESS;
    }

    if (strncmp(p_chipid, VS816_CHIP_ID, strlen(VS816_CHIP_ID)) == 0
        || strncmp(p_chipid, VS815_CHIP_ID, strlen(VS815_CHIP_ID)) == 0) {
            dsp0_support = VS_FALSE;
    }

    *p_support = dsp0_support;

    return VS_SUCCESS;
}

vs_void_t sample_common_pause(vs_void_t)
{
    printf("\n=====Press enter to exit=====\n");
    while (1) {
        if (getchar() == '\n') {
            break;
        }
        usleep(1000);
    };
    printf("\n=====exit=====\n");
}

vs_int32_t sample_common_vii_bind_vpp(vs_int32_t vii_pipeid, vs_int32_t vii_chnid, vs_int32_t vpp_grpid)
{
    vs_int32_t ret;
    vs_chn_s producer, consumer;

    producer.modid = E_MOD_ID_VII;
    producer.devid = vii_pipeid;
    producer.chnid = vii_chnid;

    consumer.modid = E_MOD_ID_VPP;
    consumer.devid = vpp_grpid;
    consumer.chnid = 0;
    ret = vs_mal_sys_bind(&producer, &consumer);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("sys bind vii-vpp failed! vii pipeid %d, chnid %d, vpp grpid %d, ret: 0x%x.\n",
			vii_pipeid, vii_chnid, vpp_grpid, ret);
        return VS_FAILED;
    }

    return VS_SUCCESS;
}

vs_int32_t sample_common_vii_unbind_vpp(vs_int32_t vii_pipeid, vs_int32_t vii_chnid, vs_int32_t vpp_grpid)
{
    vs_int32_t ret;
    vs_chn_s producer, consumer;

    producer.modid = E_MOD_ID_VII;
    producer.devid = vii_pipeid;
    producer.chnid = vii_chnid;


    consumer.modid = E_MOD_ID_VPP;
    consumer.devid = vpp_grpid;
    consumer.chnid = 0;
    ret = vs_mal_sys_unbind(&producer, &consumer);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("sys unbind vii-vpp failed! vii pipeid %d, chnid %d, vpp grpid %d, ret: 0x%x.\n",
			vii_pipeid, vii_chnid, vpp_grpid, ret);
        return VS_FAILED;
    }

    return VS_SUCCESS;
}

vs_int32_t sample_common_vpp_bind_vo(vs_int32_t vpp_grpid, vs_int32_t vpp_chnid, vs_int32_t vo_devid, vs_int32_t vo_chnid)
{
    vs_int32_t ret;
    vs_chn_s producer, consumer;

    producer.modid = E_MOD_ID_VPP;
    producer.devid = vpp_grpid;
    producer.chnid = vpp_chnid;


    consumer.modid = E_MOD_ID_VO;
    consumer.devid = vo_devid;
    consumer.chnid = vo_chnid;
    ret = vs_mal_sys_bind(&producer, &consumer);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("sys bind vpp-vo failed! vpp grpid %d, chnid %d, vo devid %d, chnid %d, ret: 0x%x.\n",
			vpp_grpid, vpp_chnid, vo_devid, vo_chnid, ret);
        return VS_FAILED;
    }

    return VS_SUCCESS;
}

vs_int32_t sample_common_vii_bind_vo(vs_int32_t vii_pipeid, vs_int32_t vii_chnid,  vs_int32_t vo_devid, vs_int32_t vo_chnid)
{
    vs_int32_t ret;
    vs_chn_s producer, consumer;

    producer.modid = E_MOD_ID_VII;
    producer.devid = vii_pipeid;
    producer.chnid = vii_chnid;


    consumer.modid = E_MOD_ID_VO;

	consumer.devid = vo_devid;
	consumer.chnid = vo_chnid;

    ret = vs_mal_sys_bind(&producer, &consumer);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("sys bind vii-vo failed! vpp vii_pipeid %d, vii_chnid %d, vo devid %d, chnid %d, ret: 0x%x.\n",
			vii_pipeid, vii_chnid, vo_devid, vo_chnid, ret);
        return VS_FAILED;
    }

    return VS_SUCCESS;
}


vs_int32_t sample_common_vii_unbind_vo(vs_int32_t vii_pipeid, vs_int32_t vii_chnid,  vs_int32_t vo_devid, vs_int32_t vo_chnid)
{
    vs_int32_t ret;
    vs_chn_s producer, consumer;

    producer.modid = E_MOD_ID_VII;
    producer.devid = vii_pipeid;
    producer.chnid = vii_chnid;


    consumer.modid = E_MOD_ID_VO;

	consumer.devid = vo_devid;
	consumer.chnid = vo_chnid;

    ret = vs_mal_sys_unbind(&producer, &consumer);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("sys unbind vii-vo failed! vpp vii_pipeid %d, vii_chnid %d, vo devid %d, chnid %d, ret: 0x%x.\n",
			vii_pipeid, vii_chnid, vo_devid, vo_chnid, ret);
        return VS_FAILED;
    }

    return VS_SUCCESS;
}



vs_int32_t sample_common_avs_bind_vo(vs_int32_t avs_devid, vs_int32_t avs_chnid,vs_int32_t vo_devid, vs_int32_t vo_chnid)
{
    vs_int32_t ret;
    vs_chn_s producer, consumer;

    producer.modid = E_MOD_ID_AVS;
    producer.devid = avs_devid;
    producer.chnid = avs_chnid;


    consumer.modid = E_MOD_ID_VO;
    consumer.devid = vo_devid;
    consumer.chnid = vo_chnid;
    ret = vs_mal_sys_bind(&producer, &consumer);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("sys bind avs-vo failed! avs grpid %d, chnid %d, vo devid %d, vo_chnid %d, ret: 0x%x.\n",
			avs_devid, avs_chnid, avs_devid, vo_chnid, ret);
        return VS_FAILED;
    }

    return VS_SUCCESS;
}

vs_int32_t sample_common_avs_unbind_vo(vs_int32_t avs_devid, vs_int32_t avs_chnid,vs_int32_t vo_devid, vs_int32_t vo_chnid)
{
    vs_int32_t ret;
    vs_chn_s producer, consumer;

    producer.modid = E_MOD_ID_AVS;
    producer.devid = avs_devid;
    producer.chnid = avs_chnid;


    consumer.modid = E_MOD_ID_VO;
    consumer.devid = vo_devid;
    consumer.chnid = vo_chnid;
    ret = vs_mal_sys_unbind(&producer, &consumer);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("sys bind avs-vo failed! avs grpid %d, chnid %d, vo devid %d, vo_chnid %d, ret: 0x%x.\n",
			avs_devid, avs_chnid, avs_devid, vo_chnid, ret);
        return VS_FAILED;
    }

    return VS_SUCCESS;
}



vs_int32_t sample_common_vpp_bind_avs(vs_int32_t vpp_grpid, vs_int32_t vpp_chnid, vs_int32_t avs_devid, vs_int32_t avs_pipeid)
{
    vs_int32_t ret;
    vs_chn_s producer, consumer;

    producer.modid = E_MOD_ID_VPP;
    producer.devid = vpp_grpid;
    producer.chnid = vpp_chnid;


    consumer.modid = E_MOD_ID_AVS;
    consumer.devid = avs_devid;
    consumer.chnid = avs_pipeid;
    ret = vs_mal_sys_bind(&producer, &consumer);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("sys bind vpp-vo failed! vpp grpid %d, chnid %d, avs devid %d, pipe_id %d, ret: 0x%x.\n",
			vpp_grpid, vpp_chnid, avs_devid, avs_pipeid, ret);
        return VS_FAILED;
    }

    return VS_SUCCESS;
}


vs_int32_t sample_common_vii_bind_avs(vs_int32_t vii_pipeid, vs_int32_t vii_chnid, vs_int32_t avs_devid, vs_int32_t avs_pipeid)
{
    vs_int32_t ret;
    vs_chn_s producer, consumer;

    producer.modid = E_MOD_ID_VII;
    producer.devid = vii_pipeid;
    producer.chnid = vii_chnid;


    consumer.modid = E_MOD_ID_AVS;
    consumer.devid = avs_devid;
    consumer.chnid = avs_pipeid;
    ret = vs_mal_sys_bind(&producer, &consumer);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("sys bind vii-avs failed! vii_pipeid %d, vii_chnid %d, avs devid %d, pipe_id %d, ret: 0x%x.\n",
			vii_pipeid, vii_chnid, avs_devid, avs_pipeid, ret);
        return VS_FAILED;
    }

    return VS_SUCCESS;
}

vs_int32_t sample_common_vii_unbind_avs(vs_int32_t vii_pipeid, vs_int32_t vii_chnid, vs_int32_t avs_devid, vs_int32_t avs_pipeid)
{
    vs_int32_t ret;
    vs_chn_s producer, consumer;

    producer.modid = E_MOD_ID_VII;
    producer.devid = vii_pipeid;
    producer.chnid = vii_chnid;


    consumer.modid = E_MOD_ID_AVS;
    consumer.devid = avs_devid;
    consumer.chnid = avs_pipeid;
    ret = vs_mal_sys_unbind(&producer, &consumer);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("sys unbind vii-avs failed! vii_pipeid %d, vii_chnid %d, avs devid %d, pipe_id %d, ret: 0x%x.\n",
			vii_pipeid, vii_chnid, avs_devid, avs_pipeid, ret);
        return VS_FAILED;
    }

    return VS_SUCCESS;
}



vs_int32_t sample_common_vpp_unbind_avs(vs_int32_t vpp_grpid, vs_int32_t vpp_chnid, vs_int32_t avs_devid, vs_int32_t avs_pipeid)
{
    vs_int32_t ret;
    vs_chn_s producer, consumer;

    producer.modid = E_MOD_ID_VPP;
    producer.devid = vpp_grpid;
    producer.chnid = vpp_chnid;


    consumer.modid = E_MOD_ID_AVS;
    consumer.devid = avs_devid;
    consumer.chnid = avs_pipeid;
    ret = vs_mal_sys_unbind(&producer, &consumer);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("sys bind vpp-vo failed! vpp grpid %d, chnid %d, avs devid %d, pipe_id %d, ret: 0x%x.\n",
			vpp_grpid, vpp_chnid, avs_devid, avs_pipeid, ret);
        return VS_FAILED;
    }

    return VS_SUCCESS;
}



vs_int32_t sample_common_vpp_unbind_vo(vs_int32_t vpp_grpid, vs_int32_t vpp_chnid, vs_int32_t vo_devid, vs_int32_t vo_chnid)
{
    vs_int32_t ret;
    vs_chn_s producer, consumer;

    producer.modid = E_MOD_ID_VPP;
    producer.devid = vpp_grpid;
    producer.chnid = vpp_chnid;

    consumer.modid = E_MOD_ID_VO;
    consumer.devid = vo_devid;
    consumer.chnid = vo_chnid;
    ret = vs_mal_sys_unbind(&producer, &consumer);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("sys unbind vpp-vo failed! vpp grpid %d, chnid %d, vo devid %d, chnid %d, ret: 0x%x.\n",
			vpp_grpid, vpp_chnid, vo_devid, vo_chnid, ret);
        return VS_FAILED;
    }

    return VS_SUCCESS;
}

vs_int32_t sample_common_vii_bind_venc(vs_int32_t vii_pipeid, vs_int32_t vii_chnid, vs_int32_t venc_devid, vs_int32_t venc_chnid)
{
    vs_int32_t ret;
    vs_chn_s producer, consumer;

    producer.modid = E_MOD_ID_VII;
    producer.devid = vii_pipeid;
    producer.chnid = vii_chnid;

    consumer.modid = E_MOD_ID_VENC;
    consumer.devid = venc_devid;
    consumer.chnid = venc_chnid;
    ret = vs_mal_sys_bind(&producer, &consumer);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("sys bind vii-venc failed! vii pipeid %d, chnid %d, venc devid %d, chnid %d, ret: 0x%x.\n",
			vii_pipeid, vii_chnid, venc_devid, venc_chnid, ret);
        return VS_FAILED;
    }

    return VS_SUCCESS;
}
vs_int32_t sample_common_vii_unbind_venc(vs_int32_t vii_pipeid, vs_int32_t vii_chnid, vs_int32_t venc_devid, vs_int32_t venc_chnid)
{
    vs_int32_t ret;
    vs_chn_s producer, consumer;

    producer.modid = E_MOD_ID_VII;
    producer.devid = vii_pipeid;
    producer.chnid = vii_chnid;

    consumer.modid = E_MOD_ID_VENC;
    consumer.devid = venc_devid;
    consumer.chnid = venc_chnid;
    ret = vs_mal_sys_unbind(&producer, &consumer);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("sys unbind vii-venc failed! vii pipeid %d, chnid %d, venc devid %d, chnid %d, ret: 0x%x.\n",
			vii_pipeid, vii_chnid, venc_devid, venc_chnid, ret);
        return VS_FAILED;
    }

    return VS_SUCCESS;
}
vs_int32_t sample_common_vpp_bind_venc(vs_int32_t vpp_grpid, vs_int32_t vpp_chnid, vs_int32_t venc_devid, vs_int32_t venc_chnid)
{
    vs_int32_t ret;
    vs_chn_s producer, consumer;

    producer.modid = E_MOD_ID_VPP;
    producer.devid = vpp_grpid;
    producer.chnid = vpp_chnid;


    consumer.modid = E_MOD_ID_VENC;
    consumer.devid = venc_devid;
    consumer.chnid = venc_chnid;
    ret = vs_mal_sys_bind(&producer, &consumer);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("sys bind vpp-venc failed! vpp grpid %d, chnid %d, venc devid %d, chnid %d, ret: 0x%x.\n",
			vpp_grpid, vpp_chnid, venc_devid, venc_chnid, ret);
        return VS_FAILED;
    }

    return VS_SUCCESS;
}
vs_int32_t sample_common_vpp_unbind_venc(vs_int32_t vpp_grpid, vs_int32_t vpp_chnid, vs_int32_t venc_devid, vs_int32_t venc_chnid)
{
    vs_int32_t ret;
    vs_chn_s producer, consumer;

    producer.modid = E_MOD_ID_VPP;
    producer.devid = vpp_grpid;
    producer.chnid = vpp_chnid;

    consumer.modid = E_MOD_ID_VENC;
    consumer.devid = venc_devid;
    consumer.chnid = venc_chnid;
    ret = vs_mal_sys_unbind(&producer, &consumer);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("sys unbind vpp-venc failed! vpp grpid %d, chnid %d, venc devid %d, chnid %d, ret: 0x%x.\n",
			vpp_grpid, vpp_chnid, venc_devid, venc_chnid, ret);
        return VS_FAILED;
    }

    return VS_SUCCESS;
}

vs_int32_t sample_common_avs_bind_venc(vs_int32_t avs_grpid, vs_int32_t avs_chnid, vs_int32_t venc_devid, vs_int32_t venc_chnid)
{
    vs_int32_t ret;
    vs_chn_s producer, consumer;

    producer.modid = E_MOD_ID_AVS;
    producer.devid = avs_grpid;
    producer.chnid = avs_chnid;


    consumer.modid = E_MOD_ID_VENC;
    consumer.devid = venc_devid;
    consumer.chnid = venc_chnid;
    ret = vs_mal_sys_bind(&producer, &consumer);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("sys bind avs-venc failed! avs grpid %d, chnid %d, venc devid %d, chnid %d, ret: 0x%x.\n",
			avs_grpid, avs_chnid, venc_devid, venc_chnid, ret);
        return VS_FAILED;
    }

    return VS_SUCCESS;
}
vs_int32_t sample_common_avs_unbind_venc(vs_int32_t avs_grpid, vs_int32_t avs_chnid, vs_int32_t venc_devid, vs_int32_t venc_chnid)
{
    vs_int32_t ret;
    vs_chn_s producer, consumer;

    producer.modid = E_MOD_ID_AVS;
    producer.devid = avs_grpid;
    producer.chnid = avs_chnid;

    consumer.modid = E_MOD_ID_VENC;
    consumer.devid = venc_devid;
    consumer.chnid = venc_chnid;
    ret = vs_mal_sys_unbind(&producer, &consumer);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("sys unbind avs-venc failed! avs grpid %d, chnid %d, venc devid %d, chnid %d, ret: 0x%x.\n",
			avs_grpid, avs_chnid, venc_devid, venc_chnid, ret);
        return VS_FAILED;
    }

    return VS_SUCCESS;
}


vs_int32_t sample_common_vdec_bind_vpp(vs_int32_t vdec_devid, vs_int32_t vdec_chnid, vs_int32_t vpp_grpid)
{
    vs_int32_t ret;
    vs_chn_s producer, consumer;

    producer.modid = E_MOD_ID_VDEC;
    producer.devid = vdec_devid;
    producer.chnid = vdec_chnid;

    consumer.modid = E_MOD_ID_VPP;
    consumer.devid = vpp_grpid;
    consumer.chnid = 0;
    ret = vs_mal_sys_bind(&producer, &consumer);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("sys unbind vdec-vpp failed! vdec devid %d, chnid %d, vpp grpid %d, ret: 0x%x.\n",
			vdec_devid, vdec_chnid, vpp_grpid, ret);
        return VS_FAILED;
    }

    return VS_SUCCESS;
}

vs_int32_t sample_common_vdec_unbind_vpp(vs_int32_t vdec_devid, vs_int32_t vdec_chnid, vs_int32_t vpp_grpid)
{
    vs_int32_t ret;
    vs_chn_s producer, consumer;

    producer.modid = E_MOD_ID_VDEC;
    producer.devid = vdec_devid;
    producer.chnid = vdec_chnid;


    consumer.modid = E_MOD_ID_VPP;
    consumer.devid = vpp_grpid;
    consumer.chnid = 0;
    ret = vs_mal_sys_unbind(&producer, &consumer);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("sys unbind vdec-vpp failed! vdec devid %d, chnid %d, vpp grpid %d, ret: 0x%x.\n",
			vdec_devid, vdec_chnid, vpp_grpid, ret);
        return VS_FAILED;
    }

    return VS_SUCCESS;
}

vs_int32_t sample_common_vdec_bind_venc(vs_int32_t vdec_devid, vs_int32_t vdec_chnid, vs_int32_t venc_devid, vs_int32_t venc_chnid)
{
    vs_int32_t ret;
    vs_chn_s producer, consumer;

    producer.modid = E_MOD_ID_VDEC;
    producer.devid = vdec_devid;
    producer.chnid = vdec_chnid;

    consumer.modid = E_MOD_ID_VENC;
    consumer.devid = venc_devid;
    consumer.chnid = venc_chnid;
    ret = vs_mal_sys_bind(&producer, &consumer);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("sys bind vdec-venc failed! vdec devid %d, chnid %d, venc devid %d, chnid %d, ret: 0x%x.\n",
			vdec_devid, vdec_chnid, venc_devid, venc_chnid, ret);
        return VS_FAILED;
    }

    return VS_SUCCESS;
}

vs_int32_t sample_common_vdec_unbind_venc(vs_int32_t vdec_devid, vs_int32_t vdec_chnid, vs_int32_t venc_devid, vs_int32_t venc_chnid)
{
    vs_int32_t ret;
    vs_chn_s producer, consumer;

    producer.modid = E_MOD_ID_VDEC;
    producer.devid = vdec_devid;
    producer.chnid = vdec_chnid;

    consumer.modid = E_MOD_ID_VENC;
    consumer.devid = venc_devid;
    consumer.chnid = venc_chnid;
    ret = vs_mal_sys_unbind(&producer, &consumer);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("sys unbind vdec-venc failed! vdec devid %d, chnid %d, venc devid %d, chnid %d, ret: 0x%x.\n",
			vdec_devid, vdec_chnid, venc_devid, venc_chnid, ret);
        return VS_FAILED;
    }

    return VS_SUCCESS;
}
vs_int32_t sample_common_vpp_bind_pciv(vs_int32_t vpp_grpid, vs_int32_t vpp_chnid, vs_int32_t pciv_chnid)
{
    vs_int32_t ret;
    vs_chn_s producer, consumer;

    producer.modid = E_MOD_ID_VPP;
    producer.devid = vpp_grpid;
    producer.chnid = vpp_chnid;


    consumer.modid = E_MOD_ID_PCIV;
    consumer.devid = 0;
    consumer.chnid = pciv_chnid;
    ret = vs_mal_sys_bind(&producer, &consumer);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("sys bind vpp-pciv failed! vpp grpid %d, chnid %d, pciv chnid %d, ret: 0x%x.\n",
			vpp_grpid, vpp_chnid, pciv_chnid, ret);
        return VS_FAILED;
    }

    return VS_SUCCESS;
}

vs_int32_t sample_common_vpp_unbind_pciv(vs_int32_t vpp_grpid, vs_int32_t vpp_chnid, vs_int32_t pciv_chnid)
{
    vs_int32_t ret;
    vs_chn_s producer, consumer;

    producer.modid = E_MOD_ID_VPP;
    producer.devid = vpp_grpid;
    producer.chnid = vpp_chnid;

    consumer.modid = E_MOD_ID_PCIV;
    consumer.devid = 0;
    consumer.chnid = pciv_chnid;
    ret = vs_mal_sys_unbind(&producer, &consumer);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("sys unbind vpp-pciv failed! vpp grpid %d, chnid %d, pciv chnid %d, ret: 0x%x.\n",
			vpp_grpid, vpp_chnid, pciv_chnid, ret);
        return VS_FAILED;
    }

    return VS_SUCCESS;
}

vs_int32_t sample_common_pciv_bind_vo(vs_int32_t pciv_chnid, vs_int32_t vo_devid, vs_int32_t vo_chnid)
{
    vs_int32_t ret;
    vs_chn_s producer, consumer;

    producer.modid = E_MOD_ID_PCIV;
    producer.devid = 0;
    producer.chnid = pciv_chnid;


    consumer.modid = E_MOD_ID_VO;
    consumer.devid = vo_devid;
    consumer.chnid = vo_chnid;
    ret = vs_mal_sys_bind(&producer, &consumer);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("sys bind pciv-vo failed! pciv chnid %d, vo devid %d, chnid %d, ret: 0x%x.\n",
			pciv_chnid, vo_devid, vo_chnid, ret);
        return VS_FAILED;
    }

    return VS_SUCCESS;
}

vs_int32_t sample_common_pciv_unbind_vo(vs_int32_t pciv_chnid, vs_int32_t vo_devid, vs_int32_t vo_chnid)
{
    vs_int32_t ret;
    vs_chn_s producer, consumer;

    producer.modid = E_MOD_ID_PCIV;
    producer.devid = 0;
    producer.chnid = pciv_chnid;

    consumer.modid = E_MOD_ID_VO;
    consumer.devid = vo_devid;
    consumer.chnid = vo_chnid;
    ret = vs_mal_sys_unbind(&producer, &consumer);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("sys unbind pciv-vo failed! pciv, chnid %d, vo devid %d, chnid %d, ret: 0x%x.\n",
			pciv_chnid, vo_devid, vo_chnid, ret);
        return VS_FAILED;
    }

    return VS_SUCCESS;
}

vs_int32_t sample_common_vo_bind_venc(vs_int32_t wbc_id, vs_int32_t venc_devid, vs_int32_t venc_chnid)
{
    vs_int32_t ret;
    vs_chn_s producer, consumer;

    producer.modid = E_MOD_ID_VO;
    producer.devid = wbc_id;
    producer.chnid = 0;

    consumer.modid = E_MOD_ID_VENC;
    consumer.devid = venc_devid;
    consumer.chnid = venc_chnid;

    ret = vs_mal_sys_bind(&producer, &consumer);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("sys bind vo-venc failed! vo wbcid %d, venc devid %d, chnid %d, ret: 0x%x.\n",
            wbc_id, venc_devid, venc_chnid, ret);
        return VS_FAILED;
    }

    return VS_SUCCESS;
}

vs_int32_t sample_common_vo_unbind_venc(vs_int32_t wbc_id, vs_int32_t venc_devid, vs_int32_t venc_chnid)
{
    vs_int32_t ret;
    vs_chn_s producer, consumer;

    producer.modid = E_MOD_ID_VO;
    producer.devid = wbc_id;
    producer.chnid = 0;

    consumer.modid = E_MOD_ID_VENC;
    consumer.devid = venc_devid;
    consumer.chnid = venc_chnid;
    ret = vs_mal_sys_unbind(&producer, &consumer);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("sys unbind vo-venc failed! vo wbcid %d, venc devid %d, chnid %d, ret: 0x%x.\n",
            wbc_id, venc_devid, venc_chnid, ret);
        return VS_FAILED;
    }

    return VS_SUCCESS;
}

vs_uint32_t sample_common_buffer_size_get(vs_size_s *img_size, vs_pixel_format_e pixel_format,
        vs_compress_mode_e compress_mode, vs_uint32_t wdr_frame_num)
{
    vs_uint32_t size = 0;
    struct common_frame_buffer_attr buffer_attr;
    buffer_attr.img_size.width = img_size->width;
    buffer_attr.img_size.height = img_size->height;
    buffer_attr.compress_mode = compress_mode;
    buffer_attr.pixel_format = pixel_format;
#ifdef VS_ORION
    size = common_frame_buffer_size_get(&buffer_attr);
#else
    size = common_frame_buffer_size_get(&buffer_attr) * wdr_frame_num;
#endif
    vs_sample_trace("common buffer size: %u\n", size);
    return size;
}
