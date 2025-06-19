
/**
* @file vs_audio_defines.h
* @brief Declaration of audio enumeration, struct.
* @details Declaration of audio enumeration, struct.
* @author VS software group
* @date 2021-11-02
* @version v0.01
* @copyright (C) 2022, Shanghai Visinex Technologies Co., Ltd.
*            All rights reserved.
*/

#ifndef __VS_AUDIO_DEFINES_H__
#define __VS_AUDIO_DEFINES_H__

/*****************************************************************************
 *                    Includes
 *****************************************************************************/
#include "vs_type.h"
#include "vs_comm.h"
#include "vs_errno.h"
#include "vs_capability.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 *                    Macro Definitions
 *****************************************************************************/

#define AUDIO_POOL_BUFF_NUM_MAX     (128)

#define AUDIO_NAME_MAX      (32)

#define AUDIO_FILE_NAME_MAX (256)

#define AQE_MASK_HPF       (0x1)    ///< HPF Mask Bit
#define AQE_MASK_AEC       (0x2)    ///< AEC Mask Bit
#define AQE_MASK_AGC       (0x4)    ///< AGC Mask Bit
#define AQE_MASK_ANR       (0x8)    ///< ANR Mask Bit


/*****************************************************************************
 *                    Type Definitions
 *****************************************************************************/


/**
* @brief the audio module parameters
*/
typedef struct vs_audio_mod_param {
	vs_int32_t reserved;
} vs_audio_mod_param_s;

/** Invalid parameter */
#define VS_ERR_AUDIO_INVALID_PARAM     VS_ERR_CODE(E_MOD_ID_AUDIO, E_ERR_INVALID_PARAM)
/** invalid device ID */
#define VS_ERR_AUDIO_INVALID_DEVID     VS_ERR_CODE(E_MOD_ID_AUDIO, E_ERR_INVALID_DEVID)
/** Invalid channel ID */
#define VS_ERR_AUDIO_INVALID_CHNID     VS_ERR_CODE(E_MOD_ID_AUDIO, E_ERR_INVALID_CHNID)
/** Bad state machine */
#define VS_ERR_AUDIO_BAD_STATE         VS_ERR_CODE(E_MOD_ID_AUDIO, E_ERR_BAD_STATE)
/** Unexpected Null pointer */
#define VS_ERR_AUDIO_NULL_PTR          VS_ERR_CODE(E_MOD_ID_AUDIO, E_ERR_NULL_PTR)
/** Resource already exists */
#define VS_ERR_AUDIO_EXIST             VS_ERR_CODE(E_MOD_ID_AUDIO, E_ERR_EXIST)
/** Resource unexists */
#define VS_ERR_AUDIO_UNEXIST           VS_ERR_CODE(E_MOD_ID_AUDIO, E_ERR_UNEXIST)
/** Operation not supported */
#define VS_ERR_AUDIO_NOT_SUPPORT       VS_ERR_CODE(E_MOD_ID_AUDIO, E_ERR_NOT_SUPPORT)
/** Request not implemented */
#define VS_ERR_AUDIO_NOT_IMPLEMENTED   VS_ERR_CODE(E_MOD_ID_AUDIO, E_ERR_NOT_IMPLEMENTED)
/** Target not initialized */
#define VS_ERR_AUDIO_NOT_INITIALIZED   VS_ERR_CODE(E_MOD_ID_AUDIO, E_ERR_NOT_INITIALIZED)
/** Operation not permitted */
#define VS_ERR_AUDIO_NOT_PERM          VS_ERR_CODE(E_MOD_ID_AUDIO, E_ERR_NOT_PERM)
/** Out of memory */
#define VS_ERR_AUDIO_NO_MEMORY         VS_ERR_CODE(E_MOD_ID_AUDIO, E_ERR_NO_MEMORY)
/** Buffer is empty */
#define VS_ERR_AUDIO_BUF_EMPTY         VS_ERR_CODE(E_MOD_ID_AUDIO, E_ERR_BUF_EMPTY)
/** Buffer is full */
#define VS_ERR_AUDIO_BUF_FULL          VS_ERR_CODE(E_MOD_ID_AUDIO, E_ERR_BUF_FULL)
/** Operation timed out */
#define VS_ERR_AUDIO_TIMEOUT           VS_ERR_CODE(E_MOD_ID_AUDIO, E_ERR_TIMEOUT)
/** Device or resource busy */
#define VS_ERR_AUDIO_BUSY              VS_ERR_CODE(E_MOD_ID_AUDIO, E_ERR_BUSY)
/** Device is unexisted */
#define VS_ERR_AUDIO_DEVICE_UNEXIST    VS_ERR_CODE(E_MOD_ID_AUDIO, E_ERR_DEVICE_UNEXIST)
/** Not enough data in buffer */
#define VS_ERR_AUDIO_BUF_LACK_DATA     VS_ERR_CODE(E_MOD_ID_AUDIO, E_AUDIO_ERR_BUF_LACK_DATA)
/** Operation fails */
#define VS_ERR_AUDIO_OPT_FAIL          VS_ERR_CODE(E_MOD_ID_AUDIO, E_AUDIO_ERR_OPT_FAIL)
/** Buffer is overlaid */
#define VS_ERR_AUDIO_BUF_OVERLAID      VS_ERR_CODE(E_MOD_ID_AUDIO, E_AUDIO_ERR_BUF_OVERLAID)

/**
* @brief the audio self-defined error code
*/
typedef enum vs_audio_error_code {
	E_AUDIO_ERR_BUF_LACK_DATA = 0x40,   ///< No enough data in buffer to decode
	E_AUDIO_ERR_OPT_FAIL,               ///< Operation fails
	E_AUDIO_ERR_BUF_OVERLAID,           ///< the audio buffer is overlaid
	E_AUDIO_ERR_MAX
} vs_audio_error_code_e;

/**
* @brief the audio sample rate
*/
typedef enum vs_audio_sample_rate {
	E_AUDIO_SAMPLE_RATE_8000   = 8000,    ///< 8KHz samplerate
	E_AUDIO_SAMPLE_RATE_12000  = 12000,   ///< 12KHz samplerate
	E_AUDIO_SAMPLE_RATE_11025  = 11025,   ///< 11.025KHz samplerate
	E_AUDIO_SAMPLE_RATE_16000  = 16000,   ///< 16KHz samplerate
	E_AUDIO_SAMPLE_RATE_22050  = 22050,   ///< 22.050KHz samplerate
	E_AUDIO_SAMPLE_RATE_24000  = 24000,   ///< 24KHz samplerate
	E_AUDIO_SAMPLE_RATE_32000  = 32000,   ///< 32KHz samplerate
	E_AUDIO_SAMPLE_RATE_44100  = 44100,   ///< 44.1KHz samplerate
	E_AUDIO_SAMPLE_RATE_48000  = 48000,   ///< 48KHz samplerate
	E_AUDIO_SAMPLE_RATE_64000  = 64000,   ///< 64KHz samplerate
	E_AUDIO_SAMPLE_RATE_96000  = 96000,   ///< 96KHz samplerate
	E_AUDIO_SAMPLE_RATE_192000 = 192000,  ///< 192KHz samplerate
	E_AUDIO_SAMPLE_RATE_MAX
} vs_audio_sample_rate_e;


/**
* @brief the bit-width of the audio sample
*/
typedef enum vs_audio_sample_bitwidth {
	E_AUDIO_SAMPLE_BITWIDTH_8  = 0,  ///< 8-bit width sample
	E_AUDIO_SAMPLE_BITWIDTH_16,      ///< 16-bit width sample
	E_AUDIO_SAMPLE_BITWIDTH_24,      ///< 24-bit width sample
	E_AUDIO_SAMPLE_BITWIDTH_32,      ///< 32-bit width sample
	E_AUDIO_SAMPLE_BITWIDTH_MAX
} vs_audio_sample_bitwidth_e;


/**
* @brief I2S/PCM Work Mode
*/
typedef enum vs_aio_mode {
	E_AIO_MODE_I2S_MASTER_STD = 0x00000,  ///< Audio I/O works on I2S Master STD mode
	E_AIO_MODE_I2S_MASTER_MSB,            ///< Audio I/O works on I2S Master Left Justify mode
	E_AIO_MODE_I2S_MASTER_LSB,            ///< Audio I/O works on I2S Master Right Justify mode
	E_AIO_MODE_I2S_MASTER_NORMAL,         ///< Audio I/O works on I2S Master Normal mode

	E_AIO_MODE_PCM_MASTER_STD = 0x00010,  ///< Audio I/O works on PCM Master STD mode
	E_AIO_MODE_PCM_MASTER_MSB,            ///< Audio I/O works on PCM Master Left Justify mode
	E_AIO_MODE_PCM_MASTER_LSB,            ///< Audio I/O works on PCM Master Right Justify mode

	E_AIO_MODE_I2S_SLAVE_STD  = 0x10000,  ///< Audio I/O works on I2S Slave STD mode
	E_AIO_MODE_I2S_SLAVE_MSB,             ///< Audio I/O works on I2S Slave Left Justify mode
	E_AIO_MODE_I2S_SLAVE_LSB,             ///< Audio I/O works on I2S Slave Right Justify mode
	E_AIO_MODE_I2S_SLAVE_NORMAL,          ///< Audio I/O works on I2S Slave Normal mode

	E_AIO_MODE_PCM_SLAVE_STD  = 0x10010,  ///< Audio I/O works on PCM Slave STD mode
	E_AIO_MODE_PCM_SLAVE_MSB,             ///< Audio I/O works on PCM Slave left Justify mode
	E_AIO_MODE_PCM_SLAVE_LSB,             ///< Audio I/O works on PCM Slave Right Justify mode

} vs_aio_mode_e;


/**
* @brief The target device of the I2S/PCM to connect
*/
typedef enum vs_aio_target {
	E_AIO_CONNECT_INNER_CODEC = 0,   ///< Audio I/O I2S/PCM is connected with the inner audio codec
	E_AIO_CONNECT_INNER_HDMI,        ///< Audio I/O I2S/PCM is connected with the inner HDMI TX
	E_AIO_CONNECT_EXTERN_I2S,        ///< Audio I/O I2S/PCM is connected with the extern audio device
	E_AIO_CONNECT_MAX,
} vs_aio_target_e;


/**
* @brief The sound mode of the audio
*/
typedef enum vs_audio_sound_mode {
	E_AUDIO_SOUND_MODE_MONO   = 0,  ///< The sound consists of one channel
	E_AUDIO_SOUND_MODE_STEREO,      ///< The sound consists of stereo(two) channel
	E_AUDIO_SOUND_MODE_MAX
} vs_audio_sound_mode_e;


/**
* @brief The attribute of the audio I/O device
*/
typedef struct vs_aiodev_attr {
	vs_audio_sample_rate_e      sample_rate;       ///< audio I/O sample rate
	vs_audio_sample_bitwidth_e  sample_width;      ///< the bit-width of the audio sample
	vs_aio_mode_e               work_mode;         ///< the work mode of I2S/PCM
	vs_audio_sound_mode_e       sound_mode;        ///< the sound mode(mono/stereo)
	vs_uint32_t                 pool_frame_num;    ///< the number of the audio frames in the pool, >2
	vs_uint32_t                 frame_sample_num;  ///< the number of the samples in one audio frame
	vs_uint32_t                 chn_num;           ///< the number of the audio I/O channels
	vs_uint32_t                 clk_source;        ///< 0: AI use itself clock source, 1: AI uses AO's clock source
	vs_aio_target_e             i2s_target;        ///< the device that is connected with internal aio via i2s/pcm
} vs_aiodev_attr_s;


/**
* @brief The audio pcm frame
*/
typedef struct vs_audio_frame {
	vs_audio_sample_bitwidth_e  sample_width;     ///< the bit-width of the audio sample
	vs_audio_sound_mode_e       sound_mode;       ///< the sound mode(mono/stereo) of the audio frame
	vs_uint8_t                 *p_virt_addr[2];   ///< the virtual address to store the audio frame
	vs_uint64_t                 phys_addr[2];     ///< the physical address to store the audio frame
	vs_uint64_t                 pts;              ///< the pts of the audio frame
	vs_uint32_t                 frame_no;         ///< audio frame no
	vs_uint32_t                 frame_chn_bytes;  ///< the size(bytes) of the audio data per channel in frame
	vs_uint32_t                 pool_id[2];       ///< the pool id where the audio frame comes from
	vs_bool_t                   is_eos;           ///< the last frame flag
} vs_audio_frame_s;

/**
* @brief the aec reference frame
*/
typedef struct vs_audio_aecframe {
	vs_audio_frame_s  refer_frame;  ///< the AEC reference frame
	vs_bool_t         valid;        ///< the frame is valid or not
	vs_bool_t         is_sys_bind;  ///< the frame is from the binder or not
} vs_audio_aecframe_s;


/**
* @brief the audio compressed stream
*/
typedef struct vs_audio_stream {
	vs_uint8_t  *p_stream;      ///< the virtual address of the audio stream
	vs_uint64_t  phys_addr;     ///< the physical address of the audio stream
	vs_uint32_t  stream_length; ///< the stream length, unit: byte
	vs_uint64_t  pts;           ///< the time stamp of the stream
	vs_uint32_t  frame_no;      ///< frame no, 0 if it is not a valid frame
} vs_audio_stream_s;


/**
* @brief the audio input parameter setting
*/
typedef struct vs_ain_chn_param {
	vs_uint32_t pool_buffer_depth;  ///< the depth of the audio pool for audio input channel
} vs_ain_chn_param_s;

/**
* @brief the work environment of sqe
*/
typedef enum vs_aqe_work_env {
	E_AQE_WORKENV_COMMON  = 0,  ///< common environment
	E_AQE_WORKENV_MUSIC,        ///< music environment
	E_AQE_WORKENV_NOISY,        ///< noisy environment
	E_AQE_WORKENV_REVERBERANT,  ///< reverberant environment
	E_AQE_WORKENV_MAX
} vs_aqe_work_env_e;

/**
* @brief the HPF configuration frequency
*/
typedef enum vs_aqe_hpf_freq {
	E_AUDIO_HPF_FREQ_80   = 80,    ///< 80Hz
	E_AUDIO_HPF_FREQ_120  = 120,   ///< 120Hz
	E_AUDIO_HPF_FREQ_150  = 150,   ///< 150Hz
	E_AUDIO_HPF_FREQ_MAX,
} vs_aqe_hpf_freq_e;

/**
* @brief the HPF configuration parameters
*/
typedef struct vs_aqe_hpf_cfg {
	vs_bool_t         is_user_mode;   ///< algorithm cfg set enable
	vs_aqe_hpf_freq_e hpf_freq;       ///< the frequence of high-pass
} vs_aqe_hpf_cfg_s;


/**
* @brief the ANR configuration parameters
*/
typedef struct vs_aqe_anr_cfg {

	vs_bool_t   is_user_mode;    ///< algorithm cfg set enable
	vs_int32_t  dc_enable;       ///< distortion control enable
	vs_float_t  noise_intensity; ///< noise reduction level control
	vs_float_t  min_gain;        ///< distortion control factor

} vs_aqe_anr_cfg_s;

/**
* @brief the AGC configuration parameters
*/
typedef struct vs_aqe_agc_cfg {

	vs_bool_t   is_user_mode;   ///< algorithm cfg set enable
	vs_float_t  max_gain;       ///< max volume control
	vs_float_t  min_gain;       ///< min volume control
	vs_int32_t  sensitivity;    ///< sensitivity control - No change recommended
	vs_int32_t  decay_time;     ///< decay time for gain control [4, 1000]
	vs_int32_t  attack_time;    ///< attack time for gain control [4, 1000]
	vs_int32_t  release_time;   ///< release time for gain control [4, 1000]
} vs_aqe_agc_cfg_s;

/**
* @brief the AEC configuration parameters
*/
typedef struct vs_aqe_aec_cfg {

	vs_bool_t   is_user_mode;   ///< algorithm cfg set enable
	vs_int32_t  es_enable;      ///< echo suppression enable
	vs_int32_t  dt_enable;      ///< double talk enable
	// main road
	vs_int32_t  aec_m;          ///< channels of filter banks - No change recommended
	vs_int32_t  aec_r;          ///< decimator factor, using to adjust different reberberant environment
	vs_int32_t  aec_p;          ///< order of prototype filter - No change recommended
	vs_int32_t  aec_nblock;     ///< control the length of adptive filter in time-domain
	vs_int32_t  aec_it;         ///< iteration
	vs_float_t  aec_mu;         ///< learning rate - No change recommended
	vs_float_t  aec_eps;        ///< convergence control
	// side road
	vs_int32_t  dt_m;           ///< channels of filter banks for side road - No change recommended
	vs_int32_t  dt_r;           ///< decimator factor for side road, using to adjust different reberberant environment
	vs_int32_t  dt_p;           ///< order of prototype filter for side road - No change recommended
	vs_int32_t  dt_nblock;      ///< control the length of adptive filter in time-domain for side road
	vs_int32_t  dt_it;          ///< iteration for side road
	vs_float_t  dt_mu;          ///< learning rate for side road - No change recommended
	vs_float_t  dt_eps;         ///< convergence control for side road
	// double talk
	vs_float_t  dt_th;          ///< threshhold for double talk
	vs_float_t  dt_lamda;       ///< forgeting factor
	// dalay estimation
	vs_int32_t  delay_mode;        ///< delay estimation mode
	vs_int32_t  max_delay_samples;  ///< max length for delay estimation


} vs_aqe_aec_cfg_s;

/**
* @brief the record aqe channel configuration parameters
*/
typedef struct vs_ain_record_aqe_cfg {
	vs_uint32_t             mask_enable;       ///< enable the corresponding aqe feature

	vs_audio_sample_rate_e  work_sample_rate;  ///< work sample rate
	vs_int32_t              chn_num;           ///< the number of channels to process
	vs_aqe_work_env_e       work_env;          ///< the work environment

	vs_aqe_hpf_cfg_s        hpf_cfg;           ///< the parameters of HPF
	vs_aqe_anr_cfg_s        anr_cfg;           ///< the parameters of ANR
	vs_aqe_agc_cfg_s        agc_cfg;           ///< the parameters of AGC

} vs_ain_record_aqe_cfg_s;


/**
* @brief the talk aqe channel configuration parameters on audio
*        input path
*/
typedef struct vs_ain_talk_aqe_cfg {
	vs_uint32_t             mask_enable;        ///< enable the corresponding aqe feature

	vs_audio_sample_rate_e  work_sample_rate;   ///< work sample rate
	vs_int32_t              chn_num;            ///< the number of channels to process
	vs_aqe_work_env_e       work_env;           ///< the work environment

	vs_aqe_hpf_cfg_s        hpf_cfg;            ///< the parameters of HPF
	vs_aqe_aec_cfg_s        aec_cfg;            ///< the parameters of AEC
	vs_aqe_anr_cfg_s        anr_cfg;            ///< the parameters of ANR
	vs_aqe_agc_cfg_s        agc_cfg;            ///< the parameters of AGC

} vs_ain_talk_aqe_cfg_s;


/**
* @brief the aqe channel configuration parameters on audio output path
*/
typedef struct vs_aout_aqe_cfg {
	vs_uint32_t             mask_enable;        ///< enable the corresponding aqe feature

	vs_audio_sample_rate_e  work_sample_rate;   ///< work sample rate
	vs_int32_t              chn_num;            ///< the number of channels to process
	vs_aqe_work_env_e       work_env;           ///< the work environment

	vs_aqe_hpf_cfg_s        hpf_cfg;            ///< the parameters of HPF
	vs_aqe_anr_cfg_s        anr_cfg;            ///< the parameters of ANR
	vs_aqe_agc_cfg_s        agc_cfg;            ///< the parameters of AGC

} vs_aout_aqe_cfg_s;
/**
* @brief the dual-mono mode to control the left and right channel output from the L/R input
*/
typedef enum vs_audio_dualmode {
	E_AUDIO_DUAL_NORMAL = 0,    ///< Lout = Lin, Rout = Rin
	E_AUDIO_DUAL_BOTH_LEFT,     ///< Lout = Lin, Rout = Lin
	E_AUDIO_DUAL_BOTH_RIGHT,    ///< Lout = Rin, Rout = Rin
	E_AUDIO_DUAL_RIGHT_LEFT,    ///< Lout = Rin, Rout = Lin
	E_AUDIO_DUAL_MIX,           ///< Lout = Lin + Rin, Rout = Lin + Rin
	E_AUDIO_DUAL_LEFT_MUTE,     ///< Lout = mute, Rout = Rin
	E_AUDIO_DUAL_RIGHT_MUTE,    ///< Lout = Lin, Rout = mute
	E_AUDIO_DUAL_BOTH_MUTE,     ///< Lout = mute, Rout = mute
	E_AUDIO_DUAL_MAX

} vs_audio_dualmode_e;

/**
* @brief the fade rate
*/
typedef enum vs_audio_fade_rate {
	E_AIO_FADE_RATE_1 = 0,  ///< fade every sample
	E_AIO_FADE_RATE_2,      ///< fade every 2 samples
	E_AIO_FADE_RATE_4,      ///< fade every 4 samples
	E_AIO_FADE_RATE_8,      ///< fade every 8 samples
	E_AIO_FADE_RATE_16,     ///< fade every 16 samples
	E_AIO_FADE_RATE_32,     ///< fade every 32 samples
	E_AIO_FADE_RATE_64,     ///< fade every 64 samples
	E_AIO_FADE_RATE_128,    ///< fade every 128 samples
	E_AIO_FADE_RATE_MAX
} vs_audio_fade_rate_e;


/**
* @brief the fade in/out rate control
*/
typedef struct vs_audio_fade {
	vs_bool_t             enable;           ///< enable flag for the fade-in/out function
	vs_audio_fade_rate_e  fade_in_rate;     ///< fade in rate
	vs_audio_fade_rate_e  fade_out_rate;    ///< fade out rate
} vs_audio_fade_s;


/**
* @brief dump audio module's output data to file
*/
typedef struct vs_audio_dumpfile {
	vs_bool_t   enable;                         ///< enable or not to dump data to file
	vs_char_t   file_name[AUDIO_FILE_NAME_MAX]; ///< the whole filepath+filename
	vs_uint32_t file_size;                      ///< the max size to dump to file, unit: KB

} vs_audio_dumpfile_s;

/**
* @brief dump status
*/
typedef struct vs_audio_dumpfile_status {
	vs_bool_t     is_dumping;   ///< dumping data or not
} vs_audio_dumpfile_status_s;

/**
* @brief the pool status of audio output channel
*/
typedef struct vs_aout_chn_status {
	vs_uint32_t   total_buffer_num; ///< the total number of the buffers in the pool
	vs_uint32_t   free_buffer_num;  ///< the number of the free buffers in the pool
	vs_uint32_t   used_buffer_num;  ///< the number of the used buffers in the pool

} vs_aout_chn_status_s;

/**
 * @brief G726 BPS
 */
typedef enum vs_audio_g726_bps {
	E_AUDIO_G726_16K = 0,       ///< G726 16KBPS
	E_AUDIO_G726_24K,           ///< G726 24KBPS
	E_AUDIO_G726_32K,           ///< G726 32KBPS
	E_AUDIO_G726_40K,           ///< G726 40KBPS
	E_AUDIO_G726_ASF_16K,       ///< G726 ASF 16KBPS
	E_AUDIO_G726_ASF_24K,       ///< G726 ASF 24KBPS
	E_AUDIO_G726_ASF_32K,       ///< G726 ASF 32KBPS
	E_AUDIO_G726_ASF_40K,       ///< G726 ASF 40KBPS
	E_AUDIO_G726_MAX,
} vs_audio_g726_bps_e;

/**
 * @brief ADPCM type
 */
typedef enum vs_audio_adpcm_type {
	E_AUDIO_ADPCM_TYPE_DVI4 = 0,    ///< ADPCM(DVI4)
	E_AUDIO_ADPCM_TYPE_IMA,         ///< ADPCM(IMA)
	E_AUDIO_ADPCM_TYPE_ORG_DVI4,    ///< ADPCM(ORG_DVI4)
	E_AUDIO_ADPCM_TYPE_MAX,
} vs_audio_adpcm_type_e;

/**
 * @brief G711 encoder attribute
 */
typedef struct vs_aenc_g711_attr {
	vs_int32_t rsv;
} vs_aenc_g711_attr_s;

/**
 * @brief G711 decoder attribute
 */
typedef struct vs_adec_g711_attr {
	vs_int32_t rsv;
} vs_adec_g711_attr_s;

/**
 * @brief G726 encoder attribute
 */
typedef struct vs_aenc_g726_attr {
	vs_audio_g726_bps_e g726_bps;     ///< G726 BPS
} vs_aenc_g726_attr_s;

/**
 * @brief G726 decoder attribute
 */
typedef struct vs_adec_g726_attr {
	vs_audio_g726_bps_e g726_bps;     ///< G726 BPS
} vs_adec_g726_attr_s;


/**
 * @brief ADPCM encoder attribute
 */
typedef struct vs_enc_adpcm_attr {
	vs_audio_adpcm_type_e adpcm_type;  ///< ADPCM format type
} vs_aenc_adpcm_attr_s;

/**
 * @brief ADPCM decoder attribute
 */
typedef struct vs_adec_adpcm_attr {
	vs_audio_adpcm_type_e adpcm_type;  ///< ADPCM format type
} vs_adec_adpcm_attr_s;

/**
 * @brief LPCM encoder attribute
 */
typedef struct vs_aenc_lpcm_attr {
	vs_int32_t rsv;
} vs_aenc_lpcm_attr_s;

/**
 * @brief LPCM decoder attribute
 */
typedef struct vs_adec_lpcm_attr {
	vs_int32_t rsv;
} vs_adec_lpcm_attr_s;

/**
* @brief audio encoder attribute
*/
typedef struct vs_aenc_attr {
	vs_payload_type_e   encoder_type;       ///< the encoder type
	vs_uint32_t         frame_sample_num;   ///< frame length, unit: sample
	vs_uint32_t         pool_buffer_depth;  ///< the number of the buffer in the pool
	vs_void_t          *p_attr;             ///< the attribute of the specific audio encoder
} vs_aenc_attr_s;


/**
* @brief the audio encoder object definition
*/
typedef struct vs_aenc_encoder {
	vs_payload_type_e       encoder_type;                   ///< the encoder type
	vs_uint32_t             max_frame_size;                 ///< the max frame size
	vs_char_t               encoder_name[AUDIO_NAME_MAX];   ///< the encoder name
	vs_int32_t            (*fn_encoder_open)(vs_void_t *p_encoder_attr, vs_void_t **pp_encoder);
	vs_int32_t            (*fn_frame_encode)(vs_void_t *p_encoder, const vs_audio_frame_s *p_data, vs_uint8_t *p_out_buffer, vs_uint32_t *p_out_size);
	vs_int32_t            (*fn_encoder_close)(vs_void_t *p_encoder);
} vs_aenc_encoder_s;


/**
* @brief the mode to parse the input compressed stream
*/
typedef enum vs_adec_stream_mode {
	E_ADEC_STREAM_MODE_FRAME = 0,   ///< the stream has been packed to a complete frame encode result
	E_ADEC_STREAM_MODE_STREAM,      ///< raw stream which has not been packed
	E_ADEC_STREAM_MODE_MAX
} vs_adec_stream_mode_e;


/**
* @brief audio decoder attribute
*/
typedef struct vs_adec_attr {
	vs_payload_type_e       decoder_type;       ///< the decoder type
	vs_uint32_t             pool_buffer_depth;  ///< the number of the buffer in the pool
	vs_adec_stream_mode_e   in_stream_mode;     ///< the mode to parse the input compressed stream
	vs_void_t              *p_attr;             ///< the attribute of the specific audio decoder
} vs_adec_attr_s;


/**
* @brief audio decoder object definition
*/
typedef struct vs_adec_decoder {
	vs_payload_type_e   decoder_type;                   ///< the decoder type
	vs_char_t           decoder_name[AUDIO_NAME_MAX];   ///< the decoder name
	vs_int32_t        (*fn_decoder_open)(vs_void_t *p_decoder_attr, vs_void_t **pp_decoder);
	vs_int32_t        (*fn_frame_decode)(vs_void_t *p_decoder, vs_uint8_t **pp_in_buffer, vs_int32_t *p_remain_byte, vs_uint16_t *p_out_buffer, vs_uint32_t *p_out_size, vs_uint32_t *p_chn_num);
	vs_int32_t        (*fn_frame_info_get)(vs_void_t *p_decoder, vs_void_t *p_info);
	vs_int32_t        (*fn_decoder_close)(vs_void_t *p_decoder);
	vs_int32_t        (*fn_decoder_reset)(vs_void_t *p_decoder);
} vs_adec_decoder_s;


/**
* @brief audio decoder status
*/
typedef struct vs_adec_status {
	vs_bool_t   is_eos;                 ///< the end of stream flag
	vs_uint32_t total_buffer_num;       ///< the total number of the buffer in the channel pool
	vs_uint32_t free_buffer_num;        ///< the number of the free buffer in the channel pool
	vs_uint32_t used_buffer_num;        ///< the number of the used buffer in the channel pool
} vs_adec_status_s;


/**
* @brief the resample attribute
*/
typedef struct vs_resample_setting {
	vs_audio_sample_rate_e  in_samplerate;      ///< the src sample rate
	vs_audio_sample_rate_e  out_samplerate;     ///< the dst sample rate
} vs_resample_setting_s;


#ifdef __cplusplus
}
#endif

#endif

