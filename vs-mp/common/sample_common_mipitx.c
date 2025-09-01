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
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/gpio.h>

#include "sample_common.h"

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
		.lcd_reset = 110, /* GPIO110 */
		.pwm_chip = 0,
		.pwm_pin = 15,
	},
	/* MIPITX_BOARD_VS819L_OI02 */
	{
		.lcd_reset = 110, /* GPIO110 */
		.pwm_chip = 0,
		.pwm_pin = 11,
	},
#else
	/* MIPITX_BOARD_VS8X9_CI01 */
	{
		.lcd_reset = 122, /* GPIO122 */
		.pwm_chip = 0,
		.pwm_pin = 6,
	},
	/* MIPITX_BOARD_VS8X9_CI02 */
	{
		.lcd_reset = 2, /* GPIO2 */
		.pwm_chip = 0,
		.pwm_pin = 6,
	},
	/* MIPITX_BOARD_VS8X9_CI03 */
	{
		.lcd_reset = 90, /* GPIO90 */
		.pwm_chip = 0,
		.pwm_pin = 6,
	},
	/* MIPITX_BOARD_VS909 */
	{
		.lcd_reset = 68, /* GPIO68 */
		.pwm_chip = 0,
		.pwm_pin = 6,
	},
#endif
};

vs_int32_t mipitx_board_setting_get(char *argv)
{
	g_mipitx_board_type = atoi(argv);
	if (g_mipitx_board_type >= MIPITX_BOARD_TYPE_MAX)
		g_mipitx_board_type = 0;

	return 0;
}

/* for mipitx to hdmi chip */
static void mipitx_timing_get(vs_mipi_tx_config_s *config,
	vs_vo_output_type_e vo_output, vs_vo_timing_s *timing_info,
	vs_vo_clk_info_s *clk_info, vs_uint32_t mipitx_phy_rate)
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
		case E_VO_OUTPUT_TYPE_USER:
			if (!clk_info || !timing_info)
				break;

			if (!clk_info->pixel_clk_rate || !mipitx_phy_rate ||
				!timing_info->hactive || !timing_info->vactive)
				break;

			config->pixel_clk = clk_info->pixel_clk_rate / 1000;  //kHz
			config->phy_data_rate = mipitx_phy_rate;
			config->sync_info.packet_size = timing_info->hactive;
			config->sync_info.hpw = timing_info->hpw;
			config->sync_info.hbp = timing_info->hbp;
			config->sync_info.htotal = timing_info->hactive + timing_info->hpw + timing_info->hbp + timing_info->hfp;
			config->sync_info.vactive = timing_info->vactive;
			config->sync_info.vbp = timing_info->vbp;
			config->sync_info.vfp = timing_info->vfp;
			config->sync_info.vpw = timing_info->vpw;
			break;

		default:
			break;
	}
}

static int mipitx_config(vs_vo_output_type_e vo_output, vs_vo_timing_s *timing_info,
	vs_vo_clk_info_s *clk_info, vs_uint32_t mipitx_phy_rate)
{
	vs_int32_t ret;
	vs_mipi_tx_config_s config = {};
	int phy_rate = 324;
	int mode = E_BURST_MODE;

	config.phy_data_rate = phy_rate;
	config.pixel_clk = 81000;
	config.lanes = 3;
	config.pixel_format = E_MIPI_TX_PIXEL_RGB888;
	config.tx_mode = E_MIPI_TX_MODE_DSI_VIDEO;
	config.video_mode = mode;
	config.sync_info.packet_size = 1280;
	config.sync_info.hpw = 9;
	config.sync_info.hbp = 13;
	config.sync_info.htotal = 1365;
	config.sync_info.vactive = 960;
	config.sync_info.vbp = 8;
	config.sync_info.vfp = 18;
	config.sync_info.vpw = 4;

	mipitx_timing_get(&config, vo_output, timing_info, clk_info, mipitx_phy_rate);

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
	int fd = -1;
	struct gpiohandle_request req;
	struct gpiohandle_data data;
	char chipname[17];
	unsigned char reset_gpiochip = g_board_settings[g_mipitx_board_type].lcd_reset / 32;
	vs_uint32_t reset_gpioline = g_board_settings[g_mipitx_board_type].lcd_reset % 32;

	snprintf(chipname, sizeof(chipname), "/dev/gpiochip%d", reset_gpiochip);

	fd = open(chipname, O_RDWR);
	if (fd < 0) {
		printf("Failed to open %s\n", chipname);
		goto fail;
	}

	memset(&req, 0, sizeof(req));
	req.fd = -1;
	req.lineoffsets[0] = reset_gpioline;
	req.flags = GPIOHANDLE_REQUEST_OUTPUT;
	req.lines = 1;
	req.default_values[0] = 1;

	if (ioctl(fd, GPIO_GET_LINEHANDLE_IOCTL, &req) < 0) {
		printf("Failed to request line handle\n");
		goto fail;
	}

	memset(&data, 0, sizeof(data));
	data.values[0] = 1;

	if (ioctl(req.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data) < 0) {
		printf("Failed to set GPIO value\n");
		goto fail;
	}

	usleep(1000);
	data.values[0] = 0;

	if (ioctl(req.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data) < 0) {
		printf("Failed to set GPIO value\n");
		goto fail;
	}

	usleep(1000);
	data.values[0] = 1;

	if (ioctl(req.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data) < 0) {
		printf("Failed to set GPIO value\n");
		goto fail;
	}

	usleep(120000);
	close(req.fd);
	close(fd);

	return 0;
fail:
	if(fd >= 0)
		close(fd);
	if(req.fd >= 0)
		close(req.fd);
	return -1;
}

static int panel_init(vs_bool_t bist)
{
	system("echo 1 > /sys/class/gpio/gpio419/value");
	usleep(100);
    system("/root/app/spitransfer 1 0 3 0x00 -w 1 0x02");
	system("/root/app/spitransfer 1 0 3 0x01 -w 1 0x43");
	usleep(80);
    system("/root/app/spitransfer 1 0 3 0x01 -w 1 0x4b");

    system("/root/app/spitransfer 1 0 3 0x02 -w 1 0x4f");
    system("/root/app/spitransfer 1 0 3 0x03 -w 1 0x02");
    system("/root/app/spitransfer 1 0 3 0x04 -w 1 0xd0");
    system("/root/app/spitransfer 1 0 3 0x05 -w 1 0x00");
    system("/root/app/spitransfer 1 0 3 0x06 -w 1 0x4f");
    system("/root/app/spitransfer 1 0 3 0x07 -w 1 0x03");
    system("/root/app/spitransfer 1 0 3 0x08 -w 1 0x00");
    system("/root/app/spitransfer 1 0 3 0x09 -w 1 0x02");
    system("/root/app/spitransfer 1 0 3 0x0a -w 1 0x49");
    system("/root/app/spitransfer 1 0 3 0x0b -w 1 0x00");
    system("/root/app/spitransfer 1 0 3 0x0c -w 1 0x41");
    system("/root/app/spitransfer 1 0 3 0x0d -w 1 0x00");
    system("/root/app/spitransfer 1 0 3 0x0e -w 1 0x00");
    system("/root/app/spitransfer 1 0 3 0x0f -w 1 0x00");
    system("/root/app/spitransfer 1 0 3 0x10 -w 1 0x00");
    system("/root/app/spitransfer 1 0 3 0x11 -w 1 0x00");
    system("/root/app/spitransfer 1 0 3 0x12 -w 1 0x00");
    system("/root/app/spitransfer 1 0 3 0x13 -w 1 0x00");
    system("/root/app/spitransfer 1 0 3 0x14 -w 1 0x12");
    system("/root/app/spitransfer 1 0 3 0x15 -w 1 0x19");

    system("/root/app/spitransfer 1 0 3 0x16 -w 1 0xe0");
    system("/root/app/spitransfer 1 0 3 0x17 -w 1 0xef");

    system("/root/app/spitransfer 1 0 3 0x18 -w 1 0x99");
    system("/root/app/spitransfer 1 0 3 0x19 -w 1 0x82");
    system("/root/app/spitransfer 1 0 3 0x1a -w 1 0x00");
    system("/root/app/spitransfer 1 0 3 0x1b -w 1 0x7f");
    system("/root/app/spitransfer 1 0 3 0x1c -w 1 0x1e");
    system("/root/app/spitransfer 1 0 3 0x1d -w 1 0x50");
    system("/root/app/spitransfer 1 0 3 0x1e -w 1 0xa9");
    system("/root/app/spitransfer 1 0 3 0x1f -w 1 0x02");
    system("/root/app/spitransfer 1 0 3 0x20 -w 1 0x42");
    system("/root/app/spitransfer 1 0 3 0x21 -w 1 0x71");
    system("/root/app/spitransfer 1 0 3 0x22 -w 1 0x92");
    system("/root/app/spitransfer 1 0 3 0x23 -w 1 0xcd");
    system("/root/app/spitransfer 1 0 3 0x24 -w 1 0x1e");
    system("/root/app/spitransfer 1 0 3 0x25 -w 1 0x9b");
    system("/root/app/spitransfer 1 0 3 0x26 -w 1 0xda");
    system("/root/app/spitransfer 1 0 3 0x27 -w 1 0x16");
    system("/root/app/spitransfer 1 0 3 0x28 -w 1 0x51");
    system("/root/app/spitransfer 1 0 3 0x29 -w 1 0x72");
    system("/root/app/spitransfer 1 0 3 0x2a -w 1 0x99");
    system("/root/app/spitransfer 1 0 3 0x2b -w 1 0xcb");
    system("/root/app/spitransfer 1 0 3 0x2c -w 1 0x3e");
    system("/root/app/spitransfer 1 0 3 0x2d -w 1 0xcd");
    system("/root/app/spitransfer 1 0 3 0x2e -w 1 0x06");
    system("/root/app/spitransfer 1 0 3 0x2f -w 1 0x32");
    system("/root/app/spitransfer 1 0 3 0x30 -w 1 0x54");
    system("/root/app/spitransfer 1 0 3 0x31 -w 1 0x74");
    system("/root/app/spitransfer 1 0 3 0x32 -w 1 0x95");
    system("/root/app/spitransfer 1 0 3 0x33 -w 1 0xc8");
    system("/root/app/spitransfer 1 0 3 0x34 -w 1 0x21");
    system("/root/app/spitransfer 1 0 3 0x35 -w 1 0x00");
    system("/root/app/spitransfer 1 0 3 0x36 -w 1 0x5e");
    system("/root/app/spitransfer 1 0 3 0x37 -w 1 0x78");
    system("/root/app/spitransfer 1 0 3 0x38 -w 1 0x5b");
    system("/root/app/spitransfer 1 0 3 0x39 -w 1 0x80");
    system("/root/app/spitransfer 1 0 3 0x3a -w 1 0x80");
    system("/root/app/spitransfer 1 0 3 0x3b -w 1 0x80");
    system("/root/app/spitransfer 1 0 3 0x3c -w 1 0x80");
    system("/root/app/spitransfer 1 0 3 0x3d -w 1 0x00");
    system("/root/app/spitransfer 1 0 3 0x3e -w 1 0x3b");
    system("/root/app/spitransfer 1 0 3 0x3f -w 1 0x72");
    system("/root/app/spitransfer 1 0 3 0x40 -w 1 0x40");
    system("/root/app/spitransfer 1 0 3 0x41 -w 1 0xd0");
    system("/root/app/spitransfer 1 0 3 0x42 -w 1 0x3d");
    system("/root/app/spitransfer 1 0 3 0x43 -w 1 0x30");
    system("/root/app/spitransfer 1 0 3 0x44 -w 1 0x02");
    system("/root/app/spitransfer 1 0 3 0x45 -w 1 0x7b");
    system("/root/app/spitransfer 1 0 3 0x46 -w 1 0x0a");
    system("/root/app/spitransfer 1 0 3 0x47 -w 1 0x59");
    system("/root/app/spitransfer 1 0 3 0x48 -w 1 0x77");
    system("/root/app/spitransfer 1 0 3 0x49 -w 1 0x9a");
    system("/root/app/spitransfer 1 0 3 0x4a -w 1 0xa7");
    system("/root/app/spitransfer 1 0 3 0x4b -w 1 0x0f");
    system("/root/app/spitransfer 1 0 3 0x4c -w 1 0xaf");
    system("/root/app/spitransfer 1 0 3 0x4d -w 1 0x66");
    system("/root/app/spitransfer 1 0 3 0x4e -w 1 0x68");
    system("/root/app/spitransfer 1 0 3 0x4f -w 1 0xff");
    system("/root/app/spitransfer 1 0 3 0x50 -w 1 0xff");
    system("/root/app/spitransfer 1 0 3 0x51 -w 1 0x92");
    system("/root/app/spitransfer 1 0 3 0x52 -w 1 0x93");
    system("/root/app/spitransfer 1 0 3 0x53 -w 1 0x0a");
    system("/root/app/spitransfer 1 0 3 0x54 -w 1 0x58");
    system("/root/app/spitransfer 1 0 3 0x55 -w 1 0x78");
    system("/root/app/spitransfer 1 0 3 0x56 -w 1 0x99");
    system("/root/app/spitransfer 1 0 3 0x57 -w 1 0xa8");
    system("/root/app/spitransfer 1 0 3 0x58 -w 1 0xff");
    system("/root/app/spitransfer 1 0 3 0x59 -w 1 0xff");
    system("/root/app/spitransfer 1 0 3 0x5a -w 1 0xff");
    system("/root/app/spitransfer 1 0 3 0x5b -w 1 0xff");
    system("/root/app/spitransfer 1 0 3 0x5c -w 1 0x20");
    system("/root/app/spitransfer 1 0 3 0x5d -w 1 0x06");
    system("/root/app/spitransfer 1 0 3 0x5e -w 1 0x18");
    system("/root/app/spitransfer 1 0 3 0x5f -w 1 0x19");
    system("/root/app/spitransfer 1 0 3 0x60 -w 1 0xa8");
    system("/root/app/spitransfer 1 0 3 0x61 -w 1 0xf0");
    system("/root/app/spitransfer 1 0 3 0x62 -w 1 0x00");
    system("/root/app/spitransfer 1 0 3 0x63 -w 1 0x00");
    system("/root/app/spitransfer 1 0 3 0x64 -w 1 0x76");
    system("/root/app/spitransfer 1 0 3 0x65 -w 1 0xec");
    system("/root/app/spitransfer 1 0 3 0x66 -w 1 0x05");
    system("/root/app/spitransfer 1 0 3 0x67 -w 1 0x5a");
    system("/root/app/spitransfer 1 0 3 0x68 -w 1 0x85");
    system("/root/app/spitransfer 1 0 3 0x69 -w 1 0xc0");
    system("/root/app/spitransfer 1 0 3 0x6a -w 1 0xfb");
    system("/root/app/spitransfer 1 0 3 0x6b -w 1 0x36");
    system("/root/app/spitransfer 1 0 3 0x6c -w 1 0x71");
    system("/root/app/spitransfer 1 0 3 0x6d -w 1 0xac");
    system("/root/app/spitransfer 1 0 3 0x6e -w 1 0xe7");
    system("/root/app/spitransfer 1 0 3 0x6f -w 1 0x22");
    system("/root/app/spitransfer 1 0 3 0x70 -w 1 0x5d");
    system("/root/app/spitransfer 1 0 3 0x71 -w 1 0x00");
    system("/root/app/spitransfer 1 0 3 0x72 -w 1 0x1a");
    system("/root/app/spitransfer 1 0 3 0x73 -w 1 0x3c");
    system("/root/app/spitransfer 1 0 3 0x74 -w 1 0x72");
    system("/root/app/spitransfer 1 0 3 0x75 -w 1 0xe4");
    system("/root/app/spitransfer 1 0 3 0x76 -w 1 0x05");
    system("/root/app/spitransfer 1 0 3 0x77 -w 1 0x5a");
    system("/root/app/spitransfer 1 0 3 0x78 -w 1 0x86");
    system("/root/app/spitransfer 1 0 3 0x79 -w 1 0xc1");
    system("/root/app/spitransfer 1 0 3 0x7a -w 1 0xfc");
    system("/root/app/spitransfer 1 0 3 0x7b -w 1 0x37");
    system("/root/app/spitransfer 1 0 3 0x7c -w 1 0x72");
    system("/root/app/spitransfer 1 0 3 0x7d -w 1 0xad");
    system("/root/app/spitransfer 1 0 3 0x7e -w 1 0xe8");
    system("/root/app/spitransfer 1 0 3 0x7f -w 1 0x23");
    system("/root/app/spitransfer 1 0 3 0x80 -w 1 0x5e");
    system("/root/app/spitransfer 1 0 3 0x81 -w 1 0x22");
    system("/root/app/spitransfer 1 0 3 0x82 -w 1 0x72");
    system("/root/app/spitransfer 1 0 3 0x83 -w 1 0xa7");
    system("/root/app/spitransfer 1 0 3 0x84 -w 1 0x73");
    system("/root/app/spitransfer 1 0 3 0x85 -w 1 0xa8");
    system("/root/app/spitransfer 1 0 3 0x86 -w 1 0x02");
    system("/root/app/spitransfer 1 0 3 0x87 -w 1 0x00");
    system("/root/app/spitransfer 1 0 3 0x88 -w 1 0x00");
    system("/root/app/spitransfer 1 0 3 0x89 -w 1 0x00");
    system("/root/app/spitransfer 1 0 3 0x8a -w 1 0x95");
    system("/root/app/spitransfer 1 0 3 0x8b -w 1 0x00");
    system("/root/app/spitransfer 1 0 3 0x8c -w 1 0x00");
    system("/root/app/spitransfer 1 0 3 0x8d -w 1 0x05");
    system("/root/app/spitransfer 1 0 3 0x8e -w 1 0x98");
    system("/root/app/spitransfer 1 0 3 0x8f -w 1 0x94");
    system("/root/app/spitransfer 1 0 3 0x90 -w 1 0x13");
    system("/root/app/spitransfer 1 0 3 0x91 -w 1 0x94");
    system("/root/app/spitransfer 1 0 3 0x92 -w 1 0xe6");
    system("/root/app/spitransfer 1 0 3 0x93 -w 1 0x4a");
    system("/root/app/spitransfer 1 0 3 0x94 -w 1 0x0e");
    system("/root/app/spitransfer 1 0 3 0x95 -w 1 0xe1");
    system("/root/app/spitransfer 1 0 3 0x96 -w 1 0x8a");
    system("/root/app/spitransfer 1 0 3 0x97 -w 1 0x8d");
    system("/root/app/spitransfer 1 0 3 0x98 -w 1 0x00");
    system("/root/app/spitransfer 1 0 3 0x99 -w 1 0x00");
    system("/root/app/spitransfer 1 0 3 0x9a -w 1 0x00");
    system("/root/app/spitransfer 1 0 3 0x9b -w 1 0x00");
    system("/root/app/spitransfer 1 0 3 0x9c -w 1 0x01");
    system("/root/app/spitransfer 1 0 3 0x9d -w 1 0x00");
    system("/root/app/spitransfer 1 0 3 0x9e -w 1 0x00");
    system("/root/app/spitransfer 1 0 3 0x9f -w 1 0x02");
    system("/root/app/spitransfer 1 0 3 0xa0 -w 1 0xc0");
	system("/root/app/spitransfer 1 0 3 0xa1 -w 1 0x00");

	// SOT
	system("/root/app/spitransfer 1 0 3 0x16 -w 1 0x60");
    system("/root/app/spitransfer 1 0 3 0x17 -w 1 0xe0");

    return 0;
}

vs_int32_t sample_common_mipitx_start(vs_vo_output_type_e vo_output, vs_vo_timing_s *timing_info,
	vs_vo_clk_info_s *clk_info, vs_uint32_t mipitx_phy_rate)
{
	vs_int32_t ret;

	ret = vs_mal_mipi_tx_open();
	if (ret) {
		printf("mipi tx open failed!\n");
		return -1;
	}

	ret = mipitx_config(vo_output, timing_info, clk_info, mipitx_phy_rate);
	if (ret) {
		printf("mipitx_config failed!\n");
		goto exit;
	}

	// ret = panel_backlight_set(5);
	// if (ret) {
	// 	printf("panel_backlight_set failed!\n");
	// 	goto exit;
	// }

	// ret = panel_reset();
	// if (ret) {
	// 	printf("panel_reset failed!\n");
	// 	goto exit;
	// }

	ret = panel_init(VS_FALSE);
	if (ret) {
		printf("panel_init failed!\n");
		goto exit;
	}

	// Need set high in mipitx_config
	system("echo 1 > /sys/class/gpio/gpio419/value");

	ret = vs_mal_mipi_tx_start(0);
	if (ret) {
		printf("vs_mal_mipi_tx_start failed!\n");
		goto exit;
	}
exit:
	vs_mal_mipi_tx_close();

	return ret;
}

vs_int32_t sample_common_mipitx_stop()
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

vs_char_t *sample_common_mipitx_board_type_get(mipitx_board_type_e board_type)
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
