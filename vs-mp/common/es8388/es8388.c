/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2018 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
 *
 * Permission is hereby granted for use on all ESPRESSIF SYSTEMS products, in which case,
 * it is free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include "es8388.h"
#include "vs_type.h"


vs_int32_t i2c_read(vs_uint8_t i2c_dev, vs_uint8_t dev_addr,
                    vs_int32_t reg_addr, vs_int32_t addr_len, vs_uint8_t *data_buf,
                    vs_int32_t data_len)
{
	struct i2c_rdwr_ioctl_data data = {0};
	struct i2c_msg messages[2] = {0};
	vs_int32_t i2c_fd = -1;
	char file_name[16];
	vs_uint8_t reg_buf[4];
	vs_int32_t i;

	if (!data_buf)
		return -1;

	if (addr_len > 4 || addr_len <= 0) {
		printf("%s:invalid addr_len: %d\n", __func__, addr_len);
		return -1;
	}

	for (i = 0; i < addr_len; i++)
		reg_buf[i] = (reg_addr >> ((addr_len - i - 1) * 8)) & 0xff;

	sprintf(file_name, "/dev/i2c-%u", i2c_dev);
	i2c_fd = open(file_name, O_RDWR);
	if (i2c_fd < 0)
		return -1;

	messages[0].addr = dev_addr;
	messages[0].flags = 0;
	messages[0].len = addr_len;
	messages[0].buf = reg_buf;

	messages[1].addr = dev_addr;
	messages[1].flags = I2C_M_RD;
	messages[1].len = data_len;
	messages[1].buf = data_buf;

	data.msgs = &messages[0];
	data.nmsgs = 2;

	if (ioctl(i2c_fd, I2C_RDWR, &data) < 0) {
		close(i2c_fd);
		return -1;
	}

	close(i2c_fd);
	return 0;
}

vs_int32_t i2c_write(vs_uint8_t i2c_dev, vs_uint8_t dev_addr,
                     vs_int32_t reg_addr, vs_int32_t addr_len, vs_uint8_t *data_buf,
                     vs_int32_t data_len)
{
	vs_uint8_t msg_buf[1024];
	struct i2c_rdwr_ioctl_data data = {0};
	struct i2c_msg messages[2] = {0};
	char file_name[16];
	vs_int32_t i2c_fd = -1;
	int i;

	if (!data_buf)
		return -1;

	if (addr_len > 4 || addr_len <= 0) {
		printf("%s:invalid addr_len: %d\n", __func__, addr_len);
		return -1;
	}

	for (i = 0; i < addr_len; i++)
		msg_buf[i] = (reg_addr >> ((addr_len - i - 1) * 8)) & 0xff;

	if (data_len + addr_len < 1028) {
		memcpy((void *) &msg_buf[addr_len], data_buf, data_len);
	}
	else {
		printf("%s:only support up to 1024 bytes at a time, invalid data_len: %d\n",
		       __func__, data_len);
		return -1;
	}

	sprintf(file_name, "/dev/i2c-%u", i2c_dev);
	i2c_fd = open(file_name, O_RDWR);
	if (i2c_fd < 0)
		return -1;

	messages[0].addr = dev_addr;
	messages[0].flags = 0;
	messages[0].len = data_len + addr_len;
	messages[0].buf = msg_buf;

	data.msgs = &messages[0];
	data.nmsgs = 1;

	if (ioctl(i2c_fd, I2C_RDWR, &data) < 0) {
		close(i2c_fd);
		return -1;
	}

	close(i2c_fd);
	return 0;
}



/**
* @brief  delay(unit: ms)
* @note
* @retval None
*/
void ES8388_Delay_1ms( unsigned int ms )
{
	unsigned int i, j;

	for( i=0; i<ms; i++ )
		for( j=0; j<42088; j++ );
}



/**
* @brief  write es8388 register
* @param reg_add:register address
 *               data: the data to write to
 *               register
* @note
* @retval 0: success others: failed
*/
static unsigned char es8388_write_reg( unsigned char reg_addr, unsigned char data )
{
	if ( 0 != i2c_write(1, ES8388_ADDR, reg_addr, 1, &data, 1) ) {
		return 1;
	}

	return 0;
}



/**
* @brief  read register from es8388
* @param reg_add:the register address
 *               p_data: the data read from the
 *               register
* @note
* @retval 0: success others: failed
*/
static unsigned char es8388_read_reg(unsigned char reg_addr, unsigned char *p_data)
{
	if ( 0 != i2c_read(1, ES8388_ADDR, reg_addr, 1, p_data, 1) ) {
		return 1;
	}

	return 0;
}


/**
* @brief  show all es8388 registers
* @param none
* @note
* @retval none
*/
void es8388_read_all()
{
	unsigned char reg = 0;
	int i;

	printf("\r\n");
	for(i = 0; i < 50; i++) {
		reg = 0;
		if( es8388_read_reg(i, &reg) == 0 ) {
			printf("reg: %x,val: %x\r\n", i, reg);
		}
	}
	printf("\r\n");
}




/**
 * @brief Configure ES8388 ADC and DAC volume. Basicly you can consider this as ADC and DAC gain
 *
 * @param mode:             set ADC or DAC or all
 * @param volume:           -96 ~ 0              for example Es8388SetAdcDacVolume(ES_MODULE_ADC, 30, 6); means set ADC volume -30.5db
 * @param dot:              whether include 0.5. for example Es8388SetAdcDacVolume(ES_MODULE_ADC, 30, 4); means set ADC volume -30db
 *
 * @return
 *     - (1) Parameter error
 *     - (0)   Success
 */
unsigned char  es8388_set_adc_dac_volume(int mode, signed int volume, unsigned int dot)
{
	int res = 0;

	if ( volume < -96 || volume > 0 ) {
		printf("Warning: volume < -96! or > 0!\r\n");
		if (volume < -96)
			volume = -96;
		else
			volume = 0;
	}

	dot = (dot >= 5 ? 1 : 0);
	volume = (-volume << 1) + dot;
	if (mode == ES_MODULE_ADC || mode == ES_MODULE_ADC_DAC) {
		res |= es8388_write_reg(ES8388_ADCCONTROL8, volume);
		res |= es8388_write_reg( ES8388_ADCCONTROL9, volume );  //ADC Right Volume=0db
	}
	if (mode == ES_MODULE_DAC || mode == ES_MODULE_ADC_DAC) {
		res |= es8388_write_reg(ES8388_DACCONTROL5, volume);
		res |= es8388_write_reg(ES8388_DACCONTROL4, volume);
	}
	return res;
}




/**
 * @brief  Start ES8388 codec chip
 *
 * @param mode:  set ADC or DAC or both
 *
 * @return
 *     - (1) Parameter error
 *     - (0) Success
 */
unsigned char es8388_start(es_module_t mode)
{
	unsigned char res = 0;
	unsigned char prev_data = 0, data = 0;

	es8388_read_reg(ES8388_DACCONTROL21, &prev_data);
	if (mode == ES_MODULE_LINE) {
		res |= es8388_write_reg(ES8388_DACCONTROL16, 0x09); // 0x00 audio on LIN1&RIN1,  0x09 LIN2&RIN2 by pass enable
		res |= es8388_write_reg(ES8388_DACCONTROL17, 0x50); // left DAC to left mixer enable  and  LIN signal to left mixer enable 0db  : bupass enable
		res |= es8388_write_reg(ES8388_DACCONTROL20, 0x50); // right DAC to right mixer enable  and  LIN signal to right mixer enable 0db : bupass enable
		res |= es8388_write_reg(ES8388_DACCONTROL21, 0xC0); //enable adc
	}
	else {
		res |= es8388_write_reg(ES8388_DACCONTROL21, 0x80);   //enable dac
	}
	es8388_read_reg(ES8388_DACCONTROL21, &data);
	if (prev_data != data) {
		res |= es8388_write_reg(ES8388_CHIPPOWER, 0xF0);   //start state machine
		// res |= es_write_reg(ES8388_ADDR, ES8388_CONTROL1, 0x16);
		// res |= es_write_reg(ES8388_ADDR, ES8388_CONTROL2, 0x50);
		res |= es8388_write_reg(ES8388_CHIPPOWER, 0x00);   //start state machine
	}
	if (mode == ES_MODULE_ADC || mode == ES_MODULE_ADC_DAC || mode == ES_MODULE_LINE) {
		res |= es8388_write_reg(ES8388_ADCPOWER, 0x00);   //power up adc and line in
	}
	if (mode == ES_MODULE_DAC || mode == ES_MODULE_ADC_DAC || mode == ES_MODULE_LINE) {
		res |= es8388_write_reg(ES8388_DACPOWER, 0x30);   //power up dac and line out
		res |= es8388_set_voice_mute(0);
		printf("es8388_start default is mode:%d\r\n", mode );
	}

	return res;
}




/**
 * @brief  Stop ES8388 codec chip
 *
 * @param mode:  set ADC or DAC or both
 *
 * @return
 *     - (1) Parameter error
 *     - (0) Success
 */
unsigned char es8388_stop(es_module_t mode)
{
	unsigned char res = 0;

	if (mode == ES_MODULE_LINE) {
		res |= es8388_write_reg( ES8388_DACCONTROL21, 0x80); //enable dac
		res |= es8388_write_reg( ES8388_DACCONTROL16, 0x00); // 0x00 audio on LIN1&RIN1,  0x09 LIN2&RIN2
		res |= es8388_write_reg( ES8388_DACCONTROL17, 0x90); // only left DAC to left mixer enable 0db
		res |= es8388_write_reg( ES8388_DACCONTROL20, 0x90); // only right DAC to right mixer enable 0db
		return res;
	}

	if (mode == ES_MODULE_DAC || mode == ES_MODULE_ADC_DAC) {
		res |= es8388_write_reg( ES8388_DACPOWER, 0x00);
		res |= es8388_set_voice_mute(1); //res |= Es8388SetAdcDacVolume(ES_MODULE_DAC, -96, 5);      // 0db
		//res |= es_write_reg(ES8388_ADDR, ES8388_DACPOWER, 0xC0);  //power down dac and line out
	}

	if (mode == ES_MODULE_ADC || mode == ES_MODULE_ADC_DAC) {
		//res |= Es8388SetAdcDacVolume(ES_MODULE_ADC, -96, 5);      // 0db
		res |= es8388_write_reg( ES8388_ADCPOWER, 0xFF);  //power down adc and line in
	}
	if (mode == ES_MODULE_ADC_DAC) {
		res |= es8388_write_reg( ES8388_DACCONTROL21, 0x9C);  //disable mclk
//        res |= es_write_reg(ES8388_ADDR, ES8388_CONTROL1, 0x00);
//        res |= es_write_reg(ES8388_ADDR, ES8388_CONTROL2, 0x58);
//        res |= es_write_reg(ES8388_ADDR, ES8388_CHIPPOWER, 0xF3);  //stop state machine
	}

	return res;
}




/**
 * @brief Config I2s clock in MSATER mode
 *
 * @param cfg.sclkDiv:      generate SCLK by dividing MCLK in MSATER mode
 * @param cfg.lclkDiv:      generate LCLK by dividing MCLK in MSATER mode
 *
 * @return
 *     - (1)  Error
 *     - (0)  Success
 */
unsigned char es8388_i2s_config_clock(es_i2s_clock_t cfg)
{
	unsigned char res = 0;

	res |= es8388_write_reg( ES8388_MASTERMODE, cfg.sclk_div);
	res |= es8388_write_reg( ES8388_ADCCONTROL5, cfg.lclk_div);  //ADCFsMode,singel SPEED,RATIO=256
	res |= es8388_write_reg( ES8388_DACCONTROL2, cfg.lclk_div);  //ADCFsMode,singel SPEED,RATIO=256
	return res;
}



/**
 * @brief Deinitialize ES8388 codec chip
 *
 * @return
 *     - (1)  Error
 *     - (0)  Success
 */
unsigned char es8388_deinit(void)
{
	int res = 0;
	res = es8388_write_reg( ES8388_CHIPPOWER, 0xFF);  //reset and stop es8388
	return res;
}




/**
 * @brief Initialize ES8388 codec chip
 *
 * @param dac_out: headphone output channel
 * @param adc_in: mic-phone input channel
 * @param mode: es8388 I2S work mode
 *
 * @return
 *     - (1)  Error
 *     - (0)  Success
 */
unsigned char es8388_init(audio_hal_dac_output_t dac_out, audio_hal_adc_input_t adc_in, audio_hal_iface_mode_t mode )
{
	int res = 0;
	int tmp;

	res |= es8388_write_reg( ES8388_DACCONTROL3, 0x04);  // 0x04 mute/0x00 unmute&ramp;DAC unmute and  disabled digital volume control soft ramp
	/* Chip Control and Power Management */
	res |= es8388_write_reg( ES8388_CONTROL2, 0x50);
	res |= es8388_write_reg( ES8388_CHIPPOWER, 0x00); //normal all and power up all
	res |= es8388_write_reg( ES8388_MASTERMODE, mode); //CODEC IN I2S SLAVE MODE

	/* dac */
	if (AUDIO_HAL_DAC_OUTPUT_NONE != dac_out) {
		res |= es8388_write_reg( ES8388_DACPOWER, 0xC0);  //disable DAC and disable Lout/Rout/1/2
		res |= es8388_write_reg( ES8388_CONTROL1, 0x15);  //Enfr=0,Play&Record Mode,(0x17-both of mic&paly)
		//    res |= es_write_reg(ES8388_ADDR, ES8388_CONTROL2, 0);  //LPVrefBuf=0,Pdn_ana=0
		res |= es8388_write_reg( ES8388_DACCONTROL1, 0x18);//1a 0x18:16bit iis , 0x00:24
		res |= es8388_write_reg( ES8388_DACCONTROL2, 0x02);  //DACFsMode,SINGLE SPEED; DACFsRatio,256
		res |= es8388_write_reg( ES8388_DACCONTROL16, 0x00); // 0x00 audio on LIN1&RIN1,  0x09 LIN2&RIN2
		res |= es8388_write_reg( ES8388_DACCONTROL17, 0x90); // only left DAC to left mixer enable 0db
		res |= es8388_write_reg( ES8388_DACCONTROL20, 0x90); // only right DAC to right mixer enable 0db
		res |= es8388_write_reg( ES8388_DACCONTROL21, 0x80); //set internal ADC and DAC use the same LRCK clock, ADC LRCK as internal LRCK
		res |= es8388_write_reg( ES8388_DACCONTROL23, 0x00);   //vroi=0
		res |= es8388_set_adc_dac_volume(ES_MODULE_DAC, 0, 0);  // 0db
		res |= es8388_write_reg( ES8388_DACCONTROL24, 0 );
		res |= es8388_write_reg( ES8388_DACCONTROL25, 0 );
		res |= es8388_write_reg( ES8388_DACCONTROL26, 0 );
		res |= es8388_write_reg( ES8388_DACCONTROL27, 0 );

		tmp = 0;
		if (AUDIO_HAL_DAC_OUTPUT_LINE2 == dac_out) {
			tmp = DAC_OUTPUT_LOUT2 | DAC_OUTPUT_ROUT2;
		}
		else if (AUDIO_HAL_DAC_OUTPUT_LINE1 == dac_out) {
			tmp = DAC_OUTPUT_LOUT1 | DAC_OUTPUT_ROUT1;
		}
		else {
			tmp = DAC_OUTPUT_LOUT1 | DAC_OUTPUT_LOUT2 | DAC_OUTPUT_ROUT1 | DAC_OUTPUT_ROUT2;
		}

		res |= es8388_write_reg( ES8388_DACPOWER, tmp);  //0x3c Enable DAC and Enable Lout/Rout/1/2
	}


	/* adc */
	if (AUDIO_HAL_ADC_INPUT_NONE != adc_in) {
		res |= es8388_write_reg( ES8388_ADCPOWER, 0xFF);
		res |= es8388_write_reg( ES8388_ADCCONTROL1, 0xbb); // MIC Left and Right channel PGA gain

		tmp = 0;
		if (AUDIO_HAL_ADC_INPUT_LINE1 == adc_in) {
			tmp = ADC_INPUT_LINPUT1_RINPUT1;
		}
		else if (AUDIO_HAL_ADC_INPUT_LINE2 == adc_in) {
			tmp = ADC_INPUT_LINPUT2_RINPUT2;
		}
		else {
			tmp = ADC_INPUT_DIFFERENCE;
		}

		res |= es8388_write_reg( ES8388_ADCCONTROL2, tmp );  //0x00 LINSEL & RINSEL, LIN1/RIN1 as ADC Input; DSSEL,use one DS Reg11; DSR, LINPUT1-RINPUT1
		res |= es8388_write_reg( ES8388_ADCCONTROL3, 0x02 );
		res |= es8388_write_reg( ES8388_ADCCONTROL4, 0x0d ); // Left/Right data, Left/Right justified mode, Bits length, I2S format
		res |= es8388_write_reg( ES8388_ADCCONTROL5, 0x02 );  //ADCFsMode,singel SPEED,RATIO=256

		//ALC for Microphone
		res |= es8388_set_adc_dac_volume( ES_MODULE_ADC, 0, 0 );      // 0db
		//res |= es8388_write_reg( ES8388_ADCPOWER, 0x09 ); //Power on ADC, Enable LIN&RIN, Power off MICBIAS, set int1lp to low power mode
	}

//    /* enable es8388 PA */
//    es8388_pa_power(true);
	printf( "init,out:%02x, in:%02x\r\n", dac_out, adc_in );
	return res;
}




/**
 * @brief Configure ES8388 I2S format
 *
 * @param mode:  set ADC or DAC or all
 * @param fmt:   i2s timing mode
 *
 * @return
 *     - (1) Error
 *     - (0) Success
 */
unsigned char es8388_config_fmt(es_module_t mode, es_i2s_fmt_t fmt)
{
	unsigned char res = 0;
	unsigned char reg = 0;

	if (mode == ES_MODULE_ADC || mode == ES_MODULE_ADC_DAC) {
		res = es8388_read_reg(ES8388_ADCCONTROL4, &reg);
		reg = reg & 0xfc;
		res |= es8388_write_reg( ES8388_ADCCONTROL4, reg | fmt );
	}
	if (mode == ES_MODULE_DAC || mode == ES_MODULE_ADC_DAC) {
		res = es8388_read_reg(ES8388_DACCONTROL1, &reg);
		reg = reg & 0xf9;
		res |= es8388_write_reg( ES8388_DACCONTROL1, reg | (fmt << 1) );
	}
	return res;
}




/**
 * @brief set ES8388 output volume of headphone
 *
 * @param dac_out: headphone output channel
 * @param volume: 0 ~ 100
 *
 * @return
 *     - (1)  Error
 *     - (0)   Success
 */
unsigned char es8388_set_voice_volume(audio_hal_dac_output_t dac_out, int volume)
{
	unsigned char res = 1;

	if (volume < 0)
		volume = 0;
	else if (volume > 100)
		volume = 100;
	volume /= 3;

	if( dac_out == AUDIO_HAL_DAC_OUTPUT_LINE1 ) {
		res = es8388_write_reg( ES8388_DACCONTROL24, volume );
		res |= es8388_write_reg( ES8388_DACCONTROL25, volume );
	}

	if( dac_out == AUDIO_HAL_DAC_OUTPUT_LINE2 ) {
		res = es8388_write_reg( ES8388_DACCONTROL26, volume );
		res |= es8388_write_reg( ES8388_DACCONTROL27, volume );
	}

	if( dac_out == AUDIO_HAL_DAC_OUTPUT_ALL ) {
		res = es8388_write_reg( ES8388_DACCONTROL24, volume );
		res |= es8388_write_reg( ES8388_DACCONTROL25, volume );
		res |= es8388_write_reg( ES8388_DACCONTROL26, volume );
		res |= es8388_write_reg( ES8388_DACCONTROL27, volume );
	}

	return res;
}




/**
 * @brief Get voice volume
 *
 * @param dac_out: headphone output channel
 * @param[out] *volume:  voice volume (0~100)
 *
 * @return
 *     - (1) Parameter error
 *     - (0) Success
 */
unsigned char es8388_get_voice_volume(audio_hal_dac_output_t dac_out, int *volume)
{
	unsigned char res = 1;
	unsigned char  reg = 0;

	if( dac_out == AUDIO_HAL_DAC_OUTPUT_LINE1 || dac_out == AUDIO_HAL_DAC_OUTPUT_ALL ) {
		res = es8388_read_reg(ES8388_DACCONTROL24, &reg);
	}

	if( dac_out == AUDIO_HAL_DAC_OUTPUT_LINE2 ) {
		res = es8388_read_reg(ES8388_DACCONTROL26, &reg);
	}

	if (res == 1) {
		*volume = 0;
	}
	else {
		*volume = reg;
		*volume *= 3;
		if (*volume == 99)
			*volume = 100;
	}
	return res;
}



/**
 * @brief Configure ES8388 data sample bits
 *
 * @param mode:        set ADC or DAC or all
 * @param bits_length: sample bit-width
 *
 * @return
 *     - (1) Parameter error
 *     - (0) Success
 */
unsigned char es8388_set_bits_per_sample(es_module_t mode, es_bits_length_t bits_length)
{
	unsigned char res = 0;
	unsigned char reg = 0;
	int bits = (int)bits_length;

	if (mode == ES_MODULE_ADC || mode == ES_MODULE_ADC_DAC) {
		res = es8388_read_reg(ES8388_ADCCONTROL4, &reg);
		reg = reg & 0xe3;
		res |=  es8388_write_reg( ES8388_ADCCONTROL4, reg | (bits << 2) );
	}
	if (mode == ES_MODULE_DAC || mode == ES_MODULE_ADC_DAC) {
		res = es8388_read_reg(ES8388_DACCONTROL1, &reg);
		reg = reg & 0xc7;
		res |= es8388_write_reg( ES8388_DACCONTROL1, reg | (bits << 3) );
	}
	return res;
}




/**
 * @brief Configure ES8388 DAC mute or not. Basically you can use this function to mute the output or unmute
 *
 * @param enable: enable or disable
 *
 * @return
 *     - (1) Parameter error
 *     - (0) Success
 */
unsigned char es8388_set_voice_mute(unsigned char enable)
{
	unsigned char res = 0;
	unsigned char reg = 0;

	res = es8388_read_reg(ES8388_DACCONTROL3, &reg);
	reg = reg & 0xFB;
	res |= es8388_write_reg( ES8388_DACCONTROL3, reg | (((int)enable) << 2) );
	return res;
}



/**
 * @brief Get ES8388 DAC mute status
 *
 *  @return
 *     - 1 Parameter error
 *     - 0 Success
 */
unsigned char es8388_get_voice_mute(void)
{
	unsigned char res = 0;
	unsigned char reg = 0;
	res = es8388_read_reg(ES8388_DACCONTROL3, &reg);
	if (res == 0) {
		reg = (reg & 0x04) >> 2;
	}
	return res == 0 ? reg : res;
}





/**
 * @brief set mic-phone gain
 *
 * @param gain: see es_mic_gain_t
 *
 * @return
 *     - (1) Parameter error
 *     - (0) Success
 */
unsigned char es8388_set_mic_gain(es_mic_gain_t gain)
{
	unsigned char res, gain_n;

	gain_n = (int)gain / 3;
	res = es8388_write_reg( ES8388_ADCCONTROL1, gain_n ); //MIC PGA
	return res;
}




/**
 * @brief Configure ES8388 I2S interface
 *
 * @param mode:  set ADC or DAC or all
 * @param fmt: i2s timing mode
 * @param bits: sample bit-width
 *
 * @return
 *     - 1 Parameter error
 *     - 0 Success
 */
unsigned char es8388_config_i2s( es_module_t mode, es_i2s_fmt_t fmt, audio_hal_iface_bits_t bits)
{
	unsigned char res = 0;
	es_bits_length_t tmp;

	res |= es8388_config_fmt(mode, fmt);
	if (bits == AUDIO_HAL_BIT_LENGTH_16BITS) {
		tmp = BIT_LENGTH_16BITS;
	}
	else if (bits == AUDIO_HAL_BIT_LENGTH_24BITS) {
		tmp = BIT_LENGTH_24BITS;
	}
	else {
		tmp = BIT_LENGTH_32BITS;
	}
	res |= es8388_set_bits_per_sample(mode, tmp);
	return res;
}

