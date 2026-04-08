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
#include "vs_mal_regtools.h"

// DCS
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

// DCS (0x39)
#define INIT_AND_SEND_LONG_CMD_V0(wr, type, len, array) \
	do { \
		(wr).p_data = (void *)(array); \
		(wr).data_type = (type); \
		(wr).data = (len); \
		if (vs_mal_mipi_tx_cmd_write(0, &(wr))) { \
			printf("%d: vs_mal_mipi_tx_cmd_write long packet failed!\n", __LINE__); \
			return -1; \
		} \
	} while (0)

// DCS (0x05)
#define INIT_AND_SEND_SHORT_CMD_NP_V0(wr, type, cmd) \
	do { \
		(wr).p_data = VS_NULL; \
		(wr).data_type = (type); \
		(wr).data = (cmd); \
		if (vs_mal_mipi_tx_cmd_write(0, &(wr))) { \
			printf("%d: vs_mal_mipi_tx_cmd_write NP failed!\n", __LINE__); \
			return -1; \
		} \
	} while (0)

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

	// PHY Rate = (pixel_clk_rate * 24 bit) / 4 lane = (132 * 24) / 4 = 792 Mbps
	int phy_rate = 792;
	int mode = E_BURST_MODE;

	config.phy_data_rate = phy_rate;
	config.pixel_clk = 132000;
	config.lanes = 4;
	config.pixel_format = E_MIPI_TX_PIXEL_RGB888;
	config.tx_mode = E_MIPI_TX_MODE_DSI_VIDEO;
	config.video_mode = mode;
	
	config.sync_info.packet_size = 1920;
	config.sync_info.hpw = 16;
	config.sync_info.hbp = 24;
	config.sync_info.htotal = 2000;   // H_Total = 1920 + 16 + 24 + 40
	config.sync_info.vactive = 1080;
	config.sync_info.vpw = 2;
	config.sync_info.vbp = 8;
	config.sync_info.vfp = 10;        // V_Total = 1080 + 2 + 8 + 10 = 1100

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

static int spi_write_reg(vs_uint8_t reg, vs_uint8_t val)
{
    vs_uint8_t tx_buf[2];
    vs_int32_t ret;
    
    // 构造发送缓冲区：[寄存器地址, 数据值]
    tx_buf[0] = reg;
    tx_buf[1] = val;
    
    // 调用SPI传输API
    ret = vs_mal_spi_transfer(1, 0, 0, tx_buf, NULL, 2);
    if (ret != 0) {
        printf("SPI write reg 0x%02x = 0x%02x failed, ret=%d\n", reg, val, ret);
        return -1;
    }
    
    return 0;
}

static int panel_init(vs_bool_t bist)
{
	vs_mipi_tx_cmd_write_s wr;
	
	printf("Starting 1920x1080 DCS panel initialization...\n");

	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x03, 0x80);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x53, 0x29);
	
	{ vs_uint8_t p[] = {0x51, 0xFF, 0x01}; INIT_AND_SEND_LONG_CMD_V0(wr, 0x39, sizeof(p), p); }
	
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x03, 0x00);
	
	{ vs_uint8_t p[] = {0x80, 0x00, 0xE0, 0xE0, 0x0E, 0x00, 0x31}; INIT_AND_SEND_LONG_CMD_V0(wr, 0x39, sizeof(p), p); }
	{ vs_uint8_t p[] = {0x81, 0x03, 0x04, 0x00, 0x10, 0x00, 0x10, 0x00}; INIT_AND_SEND_LONG_CMD_V0(wr, 0x39, sizeof(p), p); }
	{ vs_uint8_t p[] = {0x82, 0x03, 0x04, 0x00, 0x10, 0x00, 0x10, 0x01}; INIT_AND_SEND_LONG_CMD_V0(wr, 0x39, sizeof(p), p); }
	
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x35, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x26, 0x20);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x69, 0x00);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x6B, 0x00);
	
	{ vs_uint8_t p[] = {0xF0, 0xAA, 0x11}; INIT_AND_SEND_LONG_CMD_V0(wr, 0x39, sizeof(p), p); }
	
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xC0, 0x00);
	
	{ vs_uint8_t p[] = {0xC2, 0x03, 0xFF, 0x03, 0xFF, 0x03, 0xFF, 0x03, 0xFF, 0x82, 0x00, 0x00}; INIT_AND_SEND_LONG_CMD_V0(wr, 0x39, sizeof(p), p); }
	{ vs_uint8_t p[] = {0xF0, 0xAA, 0x12}; INIT_AND_SEND_LONG_CMD_V0(wr, 0x39, sizeof(p), p); }
	{ vs_uint8_t p[] = {0xBF, 0x37, 0xA9}; INIT_AND_SEND_LONG_CMD_V0(wr, 0x39, sizeof(p), p); }
	{ vs_uint8_t p[] = {0xFF, 0x5A, 0x80}; INIT_AND_SEND_LONG_CMD_V0(wr, 0x39, sizeof(p), p); }
	
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x65, 0x2F);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xF2, 0x01);
	
	{ vs_uint8_t p[] = {0xFF, 0x5A, 0x81}; INIT_AND_SEND_LONG_CMD_V0(wr, 0x39, sizeof(p), p); }
	
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x65, 0x17);
	
	{ vs_uint8_t p[] = {0xF9, 0x5E, 0x62, 0x66, 0x6A, 0x6F, 0x73, 0x77, 0x7B, 0x7F, 0x84, 0x88, 0x8C, 0x90}; INIT_AND_SEND_LONG_CMD_V0(wr, 0x39, sizeof(p), p); }
	{ vs_uint8_t p[] = {0xFF, 0x5A, 0x81}; INIT_AND_SEND_LONG_CMD_V0(wr, 0x39, sizeof(p), p); }
	
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x65, 0x05);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xF2, 0x22);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0x65, 0x0A);
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xF2, 0x00);

	// DTS 中的 05 14 01 11 代表发送指令 0x11，并延迟 0x14 (20 ms)
	INIT_AND_SEND_SHORT_CMD_NP_V0(wr, 0x05, 0x11);
	usleep(20 * 1000);

	// DTS 中的 05 64 01 29 代表发送指令 0x29，并延迟 0x64 (100 ms)
	INIT_AND_SEND_SHORT_CMD_NP_V0(wr, 0x05, 0x29);
	usleep(100 * 1000);

	// 额外延迟 20 ms
	usleep(20 * 1000);

	{ vs_uint8_t p[] = {0xF0, 0xAA, 0x11}; INIT_AND_SEND_LONG_CMD_V0(wr, 0x39, sizeof(p), p); }
	
	INIT_AND_SEND_WR_CMD_V0(wr, 0x15, 0xC0, 0xFF);

	printf("Panel initialization completed successfully\n");
	return 0;
}

vs_int32_t sample_common_mipitx_start(vs_vo_output_type_e vo_output, vs_vo_timing_s *timing_info,
	vs_vo_clk_info_s *clk_info, vs_uint32_t mipitx_phy_rate)
{
	vs_int32_t ret;

	system("echo off > /sys/devices/platform/gpio-controller/panel_reset");

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

	system("echo on > /sys/devices/platform/gpio-controller/panel_reset");

	sleep(1);
	
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
