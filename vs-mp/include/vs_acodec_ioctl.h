/**
* @file
* @brief acodec mal api ioctl struct definition
* @details acodec mal api ioctl struct definition
* @author VS software group
* @date 2022-01-10
* @version v1.0
* @Copyright (C) 2022, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/
#ifndef _VS_ACODEC_IOCTL_H
#define _VS_ACODEC_IOCTL_H

#include "vs_type.h"

#define VS_ACODEC_DEVICE_PATH   "/dev/vs_acodec"
#define VS_ACODEC_DEVICE_NAME   "vs_acodec"

#define VS_IOC_ACODEC_MAGIC		'u'

/* set input volume, range: [-115, 88]dB */
#define VS_ACODEC_INPUT_VOLUME_SET              _IOW(VS_IOC_ACODEC_MAGIC,  1, vs_int32_t)
#define VS_ACODEC_INPUT_VOLUME_GET              _IOW(VS_IOC_ACODEC_MAGIC,  2, vs_int32_t)

/* set output volume, range: [-149, 14]dB */
#define VS_ACODEC_OUTPUT_VOLUME_SET             _IOW(VS_IOC_ACODEC_MAGIC,  3, vs_int32_t)
#define VS_ACODEC_OUTPUT_VOLUME_GET             _IOW(VS_IOC_ACODEC_MAGIC,  4, vs_int32_t)

/* set mic boost gain, range: [0, 3], 0:0dB, 1:6dB, 2:20dB, 3:30dB */
#define VS_ACODEC_MICL_BOOST_SET	            _IOW(VS_IOC_ACODEC_MAGIC,  5, vs_int32_t)
#define VS_ACODEC_MICR_BOOST_SET	            _IOW(VS_IOC_ACODEC_MAGIC,  6, vs_int32_t)
#define VS_ACODEC_MICL_BOOST_GET	            _IOR(VS_IOC_ACODEC_MAGIC,  7, vs_int32_t)
#define VS_ACODEC_MICR_BOOST_GET	            _IOR(VS_IOC_ACODEC_MAGIC,  8, vs_int32_t)

/* set adc volume, range: [0, 255], 0:mute, 1:-97dB, 255:30dB, step:0.5dB */
#define VS_ACODEC_ADCL_VOLUME_SET	            _IOW(VS_IOC_ACODEC_MAGIC,  9, vs_int32_t)
#define VS_ACODEC_ADCR_VOLUME_SET	            _IOW(VS_IOC_ACODEC_MAGIC, 10, vs_int32_t)
#define VS_ACODEC_ADCL_VOLUME_GET	            _IOR(VS_IOC_ACODEC_MAGIC, 11, vs_int32_t)
#define VS_ACODEC_ADCR_VOLUME_GET	            _IOR(VS_IOC_ACODEC_MAGIC, 12, vs_int32_t)

/* set mic gain, range: [0, 31], 0:-18dB, 31:28.5dB, step:1.5dB */
#define VS_ACODEC_MICL_GAIN_SET	                _IOW(VS_IOC_ACODEC_MAGIC, 13, vs_int32_t)
#define VS_ACODEC_MICR_GAIN_SET	                _IOW(VS_IOC_ACODEC_MAGIC, 14, vs_int32_t)
#define VS_ACODEC_MICL_GAIN_GET	                _IOR(VS_IOC_ACODEC_MAGIC, 15, vs_int32_t)
#define VS_ACODEC_MICR_GAIN_GET	                _IOR(VS_IOC_ACODEC_MAGIC, 16, vs_int32_t)

/* set dac volume, range: [0, 255], 0:mute, 1:-120dB, 255:7dB, step:0.5dB */
#define VS_ACODEC_DAC_VOLUME_SET	            _IOW(VS_IOC_ACODEC_MAGIC, 17, vs_int32_t)
#define VS_ACODEC_DAC_VOLUME_GET	            _IOR(VS_IOC_ACODEC_MAGIC, 18, vs_int32_t)

/* set hpout volume, range: [0, 31], 0:-39dB, 31:7.5dB, step:1.5dB */
#define VS_ACODEC_HPOUTL_GAIN_SET	            _IOW(VS_IOC_ACODEC_MAGIC, 19, vs_int32_t)
#define VS_ACODEC_HPOUTR_GAIN_SET	            _IOW(VS_IOC_ACODEC_MAGIC, 20, vs_int32_t)
#define VS_ACODEC_HPOUTL_GAIN_GET	            _IOR(VS_IOC_ACODEC_MAGIC, 21, vs_int32_t)
#define VS_ACODEC_HPOUTR_GAIN_GET	            _IOR(VS_IOC_ACODEC_MAGIC, 22, vs_int32_t)

#define VS_ACODEC_MICL_MUTE_SET	                _IOW(VS_IOC_ACODEC_MAGIC, 23, vs_int32_t)
#define VS_ACODEC_MICR_MUTE_SET	                _IOW(VS_IOC_ACODEC_MAGIC, 24, vs_int32_t)

#define VS_ACODEC_DACL_MUTE_SET	                _IOW(VS_IOC_ACODEC_MAGIC, 25, vs_int32_t)
#define VS_ACODEC_DACR_MUTE_SET	                _IOW(VS_IOC_ACODEC_MAGIC, 26, vs_int32_t)

#define VS_ACODEC_ADC_HPF_ENABLE	            _IOW(VS_IOC_ACODEC_MAGIC, 27, vs_int32_t)


#endif
