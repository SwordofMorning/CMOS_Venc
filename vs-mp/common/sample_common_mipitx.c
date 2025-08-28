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
	int phy_rate = 1000;
	int mode = E_BURST_MODE;

	config.phy_data_rate = phy_rate;
	config.pixel_clk = 81081;
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
	
	system("/foot/app/spitransfer 1 0 3 0x00 -w 1 0x02");
	system("/foot/app/spitransfer 1 0 3 0x01 -w 1 0x4b");

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
