/**
* @file sample_common_mipitx.c
* @brief sample program for mipi tx
* @author VS software group
* @date 2022-08-08
* @version v1.0
* @Copyright (C) 2022, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "sample_common_mipitx.h"

#define DPHY_DIV_UPPER_LIMIT	8000
#define DPHY_DIV_LOWER_LIMIT	2000
#define MIN_OUTPUT_FREQ		80

#define STR1(R)			#R
#define STR2(R)			STR1(R)

#define INIT_AND_SEND_WR_CMD_V0(wr, type, data0, data1) \
	do { \
		(wr).p_data = VS_NULL; \
		(wr).data_type = (type); \
		(wr).data = ((data0) & 0xff) | (((data1) & 0xff) << 8); \
		if (vs_mal_mipi_tx_cmd_write(0, &(wr))) { \
			printf("%d: vs_mal_mipi_tx_cmd_write failed!\n", __LINE__); \
			return -1; \
		} \
	} while (0)

typedef struct mipitx_board_setting {
	vs_uint32_t lcd_reset;
	vs_uint32_t pwm_chip;
	vs_uint32_t pwm_pin;
} mipitx_board_setting_s;

mipitx_board_type_e g_mipitx_board_type = 0;

mipitx_board_setting_s g_board_settings[MIPITX_BOARD_TYPE_MAX] = {
#ifdef VS_ORION
	/* MIPITX_BOARD_VS819L_OI01 */
	{
		.lcd_reset = 398, /* GPIO110 */
		.pwm_chip = 0,
		.pwm_pin = 15,
	},
	/* MIPITX_BOARD_VS819L_OI02 */
	{
		.lcd_reset = 398, /* GPIO110 */
		.pwm_chip = 0,
		.pwm_pin = 11,
	},
#else
	/* MIPITX_BOARD_VS8X9_CI01 */
	{
		.lcd_reset = 410, /* GPIO122 */
		.pwm_chip = 0,
		.pwm_pin = 6,
	},
	/* MIPITX_BOARD_VS8X9_CI02 */
	{
		.lcd_reset = 482, /* GPIO2 */
		.pwm_chip = 0,
		.pwm_pin = 6,
	},
	/* MIPITX_BOARD_VS8X9_CI03 */
	{
		.lcd_reset = 442, /* GPIO90 */
		.pwm_chip = 0,
		.pwm_pin = 6,
	},
	/* MIPITX_BOARD_VS909 */
	{
		.lcd_reset = 420, /* GPIO68 */
		.pwm_chip = 0,
		.pwm_pin = 6,
	},
#endif
};

int mipitx_board_setting_get(char *argv)
{
	g_mipitx_board_type = atoi(argv);
	if (g_mipitx_board_type >= MIPITX_BOARD_TYPE_MAX)
		g_mipitx_board_type = 0;

	return 0;
}

/* for mipitx to hdmi chip */
static void mipitx_timing_get(vs_mipi_tx_config_s *config,
vs_vo_output_type_e vo_output)
{
	switch (vo_output) {
		case E_VO_OUTPUT_TYPE_1080P60:
		case E_VO_OUTPUT_TYPE_1080P30:
			config->sync_info.packet_size = 1920;
			config->sync_info.hpw = 44;
			config->sync_info.hbp = 148;
			config->sync_info.htotal = 2200;
			config->sync_info.vactive = 1080;
			config->sync_info.vbp = 36;
			config->sync_info.vfp = 4;
			config->sync_info.vpw = 5;

			if (vo_output == E_VO_OUTPUT_TYPE_1080P30) {
				config->pixel_clk = 74250;
				config->phy_data_rate = 450;
			} else {
				config->pixel_clk = 148500;
				config->phy_data_rate = 1100;
			}

			config->video_mode = E_BURST_MODE;
			break;

		case E_VO_OUTPUT_TYPE_3840x2160_30:
			config->sync_info.packet_size = 3840;
			config->sync_info.hpw = 88;
			config->sync_info.hbp = 296;
			config->sync_info.htotal = 4400;
			config->sync_info.vactive = 2160;
			config->sync_info.vbp = 72;
			config->sync_info.vfp = 8;
			config->sync_info.vpw = 10;
			config->pixel_clk = 297000;
			config->phy_data_rate = 1800;
			config->video_mode = E_NON_BURST_MODE_SYNC_PULSES;
			break;
		case E_VO_OUTPUT_TYPE_1080P50:
		case E_VO_OUTPUT_TYPE_1080P25:
			config->sync_info.packet_size = 1920;
			config->sync_info.hpw = 44;
			config->sync_info.hbp = 148;
			config->sync_info.htotal = 2640;
			config->sync_info.vactive = 1080;
			config->sync_info.vbp = 36;
			config->sync_info.vfp = 4;
			config->sync_info.vpw = 5;

			if (vo_output == E_VO_OUTPUT_TYPE_1080P25) {
				config->pixel_clk = 74250;
				config->phy_data_rate = 450;
			} else {
				config->pixel_clk = 148500;
				config->phy_data_rate = 1100;
			}

			config->video_mode = E_BURST_MODE;
			break;
		default:
			break;
	}
}

static int mipitx_config(vs_vo_output_type_e vo_output)
{
	vs_int32_t ret;
	vs_mipi_tx_config_s config = {};
	int phy_rate = 1000;
	int mode = E_BURST_MODE;

	config.phy_data_rate = phy_rate;
	config.pixel_clk = 138000; //138MHz
	config.lanes = 4;
	config.pixel_format = E_MIPI_TX_PIXEL_RGB888;
	config.tx_mode = E_MIPI_TX_MODE_DSI_VIDEO;
	config.video_mode = mode;
	config.sync_info.packet_size = 1080;
	config.sync_info.hpw = 8;
	config.sync_info.hbp = 16;
	config.sync_info.htotal = 1176;
	config.sync_info.vactive = 1920;
	config.sync_info.vbp = 8;
	config.sync_info.vfp = 15;
	config.sync_info.vpw = 2;

	mipitx_timing_get(&config, vo_output);

	ret = vs_mal_mipi_tx_config(0, &config);
	if (ret)
		printf("vs_mal_mipi_tx_config failed!\n");
	return ret;
}

int panel_backlight_set(unsigned int brightness)
{
	FILE *fp;
	char buf[10] = {0};
	char file[64] = {0};

	vs_uint32_t pwm_chip = g_board_settings[g_mipitx_board_type].pwm_chip;
	vs_uint32_t pwm_pin = g_board_settings[g_mipitx_board_type].pwm_pin;

	if (brightness > 100)
		brightness = 100;
	printf("New brightness: %d\n", brightness);

	snprintf(file, sizeof(file), "/sys/class/pwm/pwmchip%d/pwm%d/period", pwm_chip, pwm_pin);
	fp = fopen(file, "w");
	if (!fp) {
		snprintf(file, sizeof(file), "/sys/class/pwm/pwmchip%d/export", pwm_chip);
		fp = fopen(file, "w");
		if (!fp) {
			printf("Failed to open %s\n", file);
			goto fail;
		}
		snprintf(buf, sizeof(buf), "%d", pwm_pin);
		fwrite(buf, 1, strlen(buf), fp);
		fclose(fp);
		fp = NULL;

		snprintf(file, sizeof(file), "/sys/class/pwm/pwmchip%d/pwm%d/period", pwm_chip, pwm_pin);
		fp = fopen(file, "w");
		if (!fp) {
			printf("Failed to open %s\n", file);
			goto fail;
		}
	}

	fwrite("20000\n", 1, 6, fp);
	fclose(fp);
	fp = NULL;

	snprintf(file, sizeof(file), "/sys/class/pwm/pwmchip%d/pwm%d/duty_cycle", pwm_chip, pwm_pin);
	fp = fopen(file, "w");
	if (!fp) {
		printf("Failed to open %s\n", file);
		goto fail;
	}
	snprintf(buf, 10, "%d\n", 20000 * brightness / 100);
	fwrite(buf, 1, strlen(buf), fp);
	fclose(fp);

	snprintf(file, sizeof(file), "/sys/class/pwm/pwmchip%d/pwm%d/enable", pwm_chip, pwm_pin);
	fp = fopen(file, "w");
	if (!fp) {
		printf("Failed to open %s\n", file);
		goto fail;
	}
	fwrite("1\n", 1, 2, fp);
	fclose(fp);

	printf("Set brightness complete\n");
	return 0;

fail:
	printf("Set brightness failed\n");
	return -1;
}

static int panel_reset(void)
{
	char *export = "/sys/class/gpio/export";
	vs_uint32_t reset_gpio = g_board_settings[g_mipitx_board_type].lcd_reset;
	char file[64] = {0};
	char gpio_str[10] = {0};

	snprintf(gpio_str, sizeof(gpio_str), "%d", reset_gpio);


	FILE *fp_gpio, *fp;

	snprintf(file, sizeof(file), "/sys/class/gpio/gpio%d/direction", reset_gpio);
	fp_gpio = fopen(file, "w");
	if (!fp_gpio) {
		fp = fopen(export, "w");
		if (!fp) {
			printf("Failed to open %s\n", export);
			goto fail;
		}
		snprintf(gpio_str, sizeof(gpio_str), "%d", reset_gpio);
		fwrite(gpio_str, 1, strlen(gpio_str), fp);
		fclose(fp);
		fp = NULL;

		fp_gpio = fopen(file, "w");
		if (!fp_gpio) {
			printf("Failed to open %s\n", file);
			goto fail;
		}
	}

	fwrite("out", 1, 3, fp_gpio);
	fclose(fp_gpio);
	fp_gpio = NULL;

	snprintf(file, sizeof(file), "/sys/class/gpio/gpio%d/value", reset_gpio);
	fp_gpio = fopen(file, "w");
	if (!fp_gpio) {
		printf("Failed to open %s\n", file);
		goto fail;
	}

	fwrite("1\n", 1, 2, fp_gpio);
	fflush(fp_gpio);
	usleep(1000);
	fwrite("0\n", 1, 2, fp_gpio);
	fflush(fp_gpio);
	usleep(1000);
	fwrite("1\n", 1, 2, fp_gpio);
	fflush(fp_gpio);
	usleep(120000);
	fclose(fp_gpio);

	return 0;
fail:
	printf("%s failed\n", __func__);
	if (fp_gpio)
		fclose(fp_gpio);
	if (fp)
		fclose(fp);
	return -1;
}

static int panel_init(vs_bool_t bist)
{
	vs_mipi_tx_cmd_write_s wr;

	//CMD1&CMD3			//Command1
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xFF, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xFB, 0x01);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xBA, 0x03);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x35, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x36, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xB0, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xD3, 0x0A);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xD4, 0x0F);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xD5, 0x0F);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xD6, 0x48);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xD7, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xD9, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xFF, 0xEE);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xFB, 0x01);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x02, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x40, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x02, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x41, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x02, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x42, 0x00);
	//Command2 Page0 Power Setting Cmd			//Command2 Page0
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xFF, 0x01);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xFB, 0x01);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x01, 0x33);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x04, 0x0C);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x05, 0x3A);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x06, 0x50);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x07, 0xD0);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x0A, 0x0F);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x0C, 0x06);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x0D, 0x6B);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x0E, 0x6B);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x0F, 0x70);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x10, 0x63);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x11, 0x3C);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x12, 0x5A);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x13, 0x5A);	//VCOM
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x14, 0x5A);	//VCOM
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x15, 0x60);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x16, 0x15);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x17, 0x15);
	/*
	write 0x23  ID
	write 0x24  ID
	write 0x25  ID
	write 0x26  ID
	write 0x27  ID
	write 0x28  ID
	write 0x44  ID
	write 0x45  ID
	write 0x46  ID
	*/
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x5B, 0xCA);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x5C, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x5D, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x5E, 0x23);		//VCOM/2
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x5F, 0x1B);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x60, 0xD5);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x61, 0xF0);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x68, 0x13);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x6C, 0xAB);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x6D, 0x44);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x6E, 0x80);
	//"Command2 Page4
	//GIP Timing
	//Power On/Off setting"		//Command2 Page4
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xFF, 0x05);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xFB, 0x01);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x00, 0x3F);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x01, 0x3F);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x02, 0x3F);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x03, 0x3F);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x04, 0x38);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x05, 0x3F);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x06, 0x3F);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x07, 0x19);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x08, 0x1B);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x09, 0x3F);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x0A, 0x3F);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x0B, 0x1D);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x0C, 0x17);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x0D, 0x3F);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x0E, 0x3F);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x0F, 0x08);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x10, 0x3F);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x11, 0x10);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x12, 0x3F);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x13, 0x3F);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x14, 0x3F);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x15, 0x3F);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x16, 0x3F);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x17, 0x3F);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x18, 0x38);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x19, 0x18);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x1A, 0x1A);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x1B, 0x3F);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x1C, 0x3F);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x1D, 0x1C);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x1E, 0x16);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x1F, 0x3F);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x20, 0x3F);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x21, 0x3F);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x22, 0x3F);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x23, 0x06);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x24, 0x3F);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x25, 0x0E);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x26, 0x3F);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x27, 0x3F);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x54, 0x08);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x55, 0x07);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x56, 0x06);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x58, 0x05);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x59, 0x36);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x5A, 0x36);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x5B, 0x01);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x5C, 0x64);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x5E, 0x27);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x5F, 0x28);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x60, 0x29);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x61, 0x2A);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x62, 0x18);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x63, 0x01);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x64, 0x01);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x65, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x66, 0x44);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x67, 0x11);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x68, 0x01);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x69, 0x01);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x6A, 0x06);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x6B, 0x22);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x6C, 0x08);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x6D, 0x08);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x78, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x79, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x7E, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x7F, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x80, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x81, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x8D, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x8E, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x8F, 0xC0);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x90, 0x73);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x91, 0x10);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x92, 0x09);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x96, 0x11);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x97, 0x14);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x98, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x99, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x9A, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x9B, 0x61);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x9C, 0x15);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x9D, 0x30);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x9F, 0x0F);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xA2, 0xB0);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xA7, 0x0A);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xA9, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xAA, 0x70);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xAB, 0xDA);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xAC, 0xFF);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xAE, 0xF4);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xAF, 0x40);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xB0, 0x7F);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xB1, 0x16);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xB2, 0x53);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xB3, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xB4, 0x2A);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xB5, 0x3A);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xB6, 0xF0);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xBC, 0x85);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xBD, 0xF8);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xBE, 0x3B);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xBF, 0x13);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xC0, 0x77);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xC1, 0x77);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xC2, 0x77);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xC3, 0x77);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xC4, 0x77);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xC5, 0x77);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xC6, 0x77);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xC7, 0x77);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xC8, 0xAA);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xC9, 0x2A);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xCA, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xCB, 0xAA);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xCC, 0x92);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xCD, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xCE, 0x18);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xCF, 0x88);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xD0, 0xAA);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xD1, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xD2, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xD3, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xD6, 0x02);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xD7, 0x31);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xD8, 0x7E);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xED, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xEE, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xEF, 0x70);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xFA, 0x03);
	//Gamma Setting		//Command2 Page0
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xFF, 0x01);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xFB, 0x01);
	//GAMMA RED+
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x75, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x76, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x77, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x78, 0x10);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x79, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x7A, 0x2C);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x7B, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x7C, 0x43);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x7D, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x7E, 0x57);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x7F, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x80, 0x69);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x81, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x82, 0x7A);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x83, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x84, 0x89);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x85, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x86, 0x98);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x87, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x88, 0xC9);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x89, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x8A, 0xF2);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x8B, 0x01);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x8C, 0x33);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x8D, 0x01);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x8E, 0x68);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x8F, 0x01);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x90, 0xBA);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x91, 0x01);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x92, 0xFA);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x93, 0x01);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x94, 0xFC);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x95, 0x02);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x96, 0x35);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x97, 0x02);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x98, 0x71);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x99, 0x02);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x9A, 0x97);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x9B, 0x02);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x9C, 0xCC);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x9D, 0x02);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x9E, 0xEE);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x9F, 0x03);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xA0, 0x16);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xA2, 0x03);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xA3, 0x2D);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xA4, 0x03);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xA5, 0x3D);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xA6, 0x03);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xA7, 0x4F);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xA9, 0x03);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xAA, 0x64);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xAB, 0x03);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xAC, 0x7C);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xAD, 0x03);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xAE, 0x9C);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xAF, 0x03);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xB0, 0xC5);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xB1, 0x03);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xB2, 0xCD);
	//GAMMA RED-
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xB3, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xB4, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xB5, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xB6, 0x10);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xB7, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xB8, 0x2C);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xB9, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xBA, 0x43);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xBB, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xBC, 0x57);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xBD, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xBE, 0x69);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xBF, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xC0, 0x7A);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xC1, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xC2, 0x89);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xC3, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xC4, 0x98);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xC5, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xC6, 0xC9);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xC7, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xC8, 0xF2);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xC9, 0x01);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xCA, 0x33);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xCB, 0x01);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xCC, 0x68);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xCD, 0x01);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xCE, 0xBA);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xCF, 0x01);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xD0, 0xFA);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xD1, 0x01);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xD2, 0xFC);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xD3, 0x02);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xD4, 0x35);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xD5, 0x02);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xD6, 0x71);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xD7, 0x02);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xD8, 0x97);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xD9, 0x02);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xDA, 0xCC);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xDB, 0x02);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xDC, 0xEE);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xDD, 0x03);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xDE, 0x16);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xDF, 0x03);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xE0, 0x2D);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xE1, 0x03);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xE2, 0x3D);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xE3, 0x03);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xE4, 0x4F);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xE5, 0x03);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xE6, 0x64);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xE7, 0x03);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xE8, 0x7C);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xE9, 0x03);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xEA, 0x9C);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xEB, 0x03);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xEC, 0xC5);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xED, 0x03);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xEE, 0xCD);
	//GAMMA Green+
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xEF, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xF0, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xF1, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xF2, 0x10);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xF3, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xF4, 0x2C);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xF5, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xF6, 0x43);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xF7, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xF8, 0x57);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xF9, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xFA, 0x69);
	//Command2 Page1
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xFF, 0x02);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xFB, 0x01);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x00, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x01, 0x7A);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x02, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x03, 0x89);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x04, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x05, 0x98);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x06, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x07, 0xC9);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x08, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x09, 0xF2);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x0A, 0x01);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x0B, 0x33);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x0C, 0x01);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x0D, 0x68);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x0E, 0x01);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x0F, 0xBA);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x10, 0x01);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x11, 0xFA);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x12, 0x01);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x13, 0xFC);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x14, 0x02);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x15, 0x35);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x16, 0x02);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x17, 0x71);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x18, 0x02);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x19, 0x97);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x1A, 0x02);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x1B, 0xCC);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x1C, 0x02);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x1D, 0xEE);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x1E, 0x03);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x1F, 0x16);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x20, 0x03);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x21, 0x2D);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x22, 0x03);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x23, 0x3D);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x24, 0x03);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x25, 0x4F);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x26, 0x03);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x27, 0x64);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x28, 0x03);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x29, 0x7C);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x2A, 0x03);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x2B, 0x9C);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x2D, 0x03);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x2F, 0xC5);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x30, 0x03);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x31, 0xCD);
	//LP		//GAMMA Green-
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x32, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x33, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x34, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x35, 0x10);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x36, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x37, 0x2C);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x38, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x39, 0x43);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x3A, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x3B, 0x57);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x3D, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x3F, 0x69);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x40, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x41, 0x7A);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x42, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x43, 0x89);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x44, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x45, 0x98);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x46, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x47, 0xC9);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x48, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x49, 0xF2);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x4A, 0x01);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x4B, 0x33);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x4C, 0x01);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x4D, 0x68);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x4E, 0x01);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x4F, 0xBA);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x50, 0x01);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x51, 0xFA);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x52, 0x01);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x53, 0xFC);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x54, 0x02);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x55, 0x35);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x56, 0x02);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x58, 0x71);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x59, 0x02);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x5A, 0x97);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x5B, 0x02);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x5C, 0xCC);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x5D, 0x02);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x5E, 0xEE);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x5F, 0x03);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x60, 0x16);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x61, 0x03);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x62, 0x2D);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x63, 0x03);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x64, 0x3D);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x65, 0x03);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x66, 0x4F);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x67, 0x03);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x68, 0x64);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x69, 0x03);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x6A, 0x7C);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x6B, 0x03);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x6C, 0x9C);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x6D, 0x03);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x6E, 0xC5);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x6F, 0x03);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x70, 0xCD);
	//LP		//GAMMA Blue+
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x71, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x72, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x73, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x74, 0x10);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x75, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x76, 0x2C);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x77, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x78, 0x43);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x79, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x7A, 0x57);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x7B, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x7C, 0x69);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x7D, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x7E, 0x7A);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x7F, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x80, 0x89);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x81, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x82, 0x98);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x83, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x84, 0xC9);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x85, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x86, 0xF2);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x87, 0x01);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x88, 0x33);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x89, 0x01);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x8A, 0x68);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x8B, 0x01);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x8C, 0xBA);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x8D, 0x01);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x8E, 0xFA);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x8F, 0x01);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x90, 0xFC);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x91, 0x02);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x92, 0x35);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x93, 0x02);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x94, 0x71);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x95, 0x02);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x96, 0x97);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x97, 0x02);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x98, 0xCC);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x99, 0x02);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x9A, 0xEE);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x9B, 0x03);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x9C, 0x16);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x9D, 0x03);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x9E, 0x2D);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x9F, 0x03);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xA0, 0x3D);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xA2, 0x03);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xA3, 0x4F);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xA4, 0x03);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xA5, 0x64);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xA6, 0x03);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xA7, 0x7C);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xA9, 0x03);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xAA, 0x9C);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xAB, 0x03);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xAC, 0xC5);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xAD, 0x03);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xAE, 0xCD);
	//LP		//GAMMA Blue-
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xAF, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xB0, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xB1, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xB2, 0x10);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xB3, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xB4, 0x2C);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xB5, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xB6, 0x43);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xB7, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xB8, 0x57);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xB9, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xBA, 0x69);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xBB, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xBC, 0x7A);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xBD, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xBE, 0x89);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xBF, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xC0, 0x98);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xC1, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xC2, 0xC9);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xC3, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xC4, 0xF2);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xC5, 0x01);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xC6, 0x33);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xC7, 0x01);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xC8, 0x68);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xC9, 0x01);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xCA, 0xBA);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xCB, 0x01);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xCC, 0xFA);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xCD, 0x01);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xCE, 0xFC);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xCF, 0x02);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xD0, 0x35);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xD1, 0x02);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xD2, 0x71);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xD3, 0x02);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xD4, 0x97);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xD5, 0x02);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xD6, 0xCC);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xD7, 0x02);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xD8, 0xEE);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xD9, 0x03);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xDA, 0x16);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xDB, 0x03);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xDC, 0x2D);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xDD, 0x03);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xDE, 0x3D);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xDF, 0x03);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xE0, 0x4F);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xE1, 0x03);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xE2, 0x64);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xE3, 0x03);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xE4, 0x7C);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xE5, 0x03);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xE6, 0x9C);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xE7, 0x03);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xE8, 0xC5);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xE9, 0x03);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xEA, 0xCD);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xFF, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xFB, 0x01);

	//bist mode
	if (bist) {
		printf("Enter BIST mode\n");
		INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xFF, 0x05);
		INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xFB, 0x01);
		INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xEC, 0x21);
		INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xEA, 0xff);
		INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xEB, 0xFF);
	}

	//package=DCS

	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xFF, 0x00);

	//write 0x11
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x11, 0x00);
	usleep(120 * 1000);

	//write 0x29
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x29, 0x00);
	usleep(50 * 1000);

	return 0;
}

int sample_common_mipitx_start(vs_vo_output_type_e vo_output)
{
	vs_int32_t ret;

	ret = vs_mal_mipi_tx_open();
	if (ret) {
		printf("mipi tx open failed!\n");
		return -1;
	}

	ret = mipitx_config(vo_output);
	if (ret) {
		printf("mipitx_config failed!\n");
		goto exit;
	}

	ret = panel_backlight_set(5);
	if (ret) {
		printf("panel_backlight_set failed!\n");
		goto exit;
	}

	ret = panel_reset();
	if (ret) {
		printf("panel_reset failed!\n");
		goto exit;
	}

	ret = panel_init(VS_FALSE);
	if (ret) {
		printf("panel_init failed!\n");
		goto exit;
	}

	ret = vs_mal_mipi_tx_start(0);
	if (ret) {
		printf("vs_mal_mipi_tx_start failed!\n");
		goto exit;
	}
exit:
	vs_mal_mipi_tx_close();

	return ret;
}

int sample_common_mipitx_stop()
{
	vs_int32_t ret;

	ret = vs_mal_mipi_tx_open();
	if (ret) {
		printf("mipi tx open failed!\n");
		return -1;
	}

	ret = panel_backlight_set(0);
	if (ret) {
		printf("panel_backlight_set failed!\n");
	}

	vs_mal_mipi_tx_stop(0);

	vs_mal_mipi_tx_close();

	return 0;
}

char *sample_common_mipitx_board_type_get(mipitx_board_type_e board_type)
{
	switch (board_type) {
#ifdef VS_ORION
		case MIPITX_BOARD_VS819L_OI01:
			return "MIPITX_BOARD_VS819L_OI01";
		case MIPITX_BOARD_VS819L_OI02:
			return "MIPITX_BOARD_VS819L_OI02";
#else
		case MIPITX_BOARD_VS8X9_CI01:
			return "MIPITX_BOARD_VS8X9_CI01";
		case MIPITX_BOARD_VS8X9_CI02:
			return "MIPITX_BOARD_VS8X9_CI02";
		case MIPITX_BOARD_VS8X9_CI03:
			return "MIPITX_BOARD_VS8X9_CI03";
		case MIPITX_BOARD_VS909:
			return "MIPITX_BOARD_VS909";
#endif
		default:
			return "N/A";
	}
}
