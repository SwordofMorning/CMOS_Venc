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

	// PHY Rate = (pixel_clk_rate × 24 × 1) / 3
	int phy_rate = 648;
	int mode = E_NON_BURST_MODE_SYNC_EVENTS;

	config.phy_data_rate = phy_rate;
	// pixel_clk_rate = 1365 × 990 × 59.95 ≈ 81,000,000 Hz
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

	// mipitx_timing_get(&config, vo_output, timing_info, clk_info, mipitx_phy_rate);

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
    ret = vs_mal_spi_transfer(1, 0, 3, tx_buf, NULL, 2);
    if (ret != 0) {
        printf("SPI write reg 0x%02x = 0x%02x failed, ret=%d\n", reg, val, ret);
        return -1;
    }
    
    return 0;
}

static int panel_init(vs_bool_t bist)
{
    int ret = 0;

    // 初始启动序列
    ret |= spi_write_reg(0x00, 0x02);
    ret |= spi_write_reg(0x01, 0x43);
    usleep(80);

    ret |= spi_write_reg(0x01, 0x4b);
    // 主要配置序列
    ret |= spi_write_reg(0x02, 0x4f);
    ret |= spi_write_reg(0x03, 0x02);
    ret |= spi_write_reg(0x04, 0xd0);
    ret |= spi_write_reg(0x05, 0x00);
    ret |= spi_write_reg(0x06, 0x4f);
    ret |= spi_write_reg(0x07, 0x03);
    ret |= spi_write_reg(0x08, 0x00);
    ret |= spi_write_reg(0x09, 0x02);
    ret |= spi_write_reg(0x0a, 0x49);
    ret |= spi_write_reg(0x0b, 0x00);
    ret |= spi_write_reg(0x0c, 0x41);
    ret |= spi_write_reg(0x0d, 0x00);
    ret |= spi_write_reg(0x0e, 0x00);
    ret |= spi_write_reg(0x0f, 0x00);
    ret |= spi_write_reg(0x10, 0x00);
    ret |= spi_write_reg(0x11, 0x00);
    ret |= spi_write_reg(0x12, 0x00);
    ret |= spi_write_reg(0x13, 0x00);
    ret |= spi_write_reg(0x14, 0x12);
    ret |= spi_write_reg(0x15, 0x19);
    
    ret |= spi_write_reg(0x16, 0xe0);
    ret |= spi_write_reg(0x17, 0xef);
    
    ret |= spi_write_reg(0x18, 0x99);
    ret |= spi_write_reg(0x19, 0x82);
    ret |= spi_write_reg(0x1a, 0x00);
    ret |= spi_write_reg(0x1b, 0x7f);
    ret |= spi_write_reg(0x1c, 0x1e);
    ret |= spi_write_reg(0x1d, 0x50);
    ret |= spi_write_reg(0x1e, 0xa9);
    ret |= spi_write_reg(0x1f, 0x02);
    ret |= spi_write_reg(0x20, 0x42);
    ret |= spi_write_reg(0x21, 0x71);
    ret |= spi_write_reg(0x22, 0x92);
    ret |= spi_write_reg(0x23, 0xcd);
    ret |= spi_write_reg(0x24, 0x1e);
    ret |= spi_write_reg(0x25, 0x9b);
    ret |= spi_write_reg(0x26, 0xda);
    ret |= spi_write_reg(0x27, 0x16);
    ret |= spi_write_reg(0x28, 0x51);
    ret |= spi_write_reg(0x29, 0x72);
    ret |= spi_write_reg(0x2a, 0x99);
    ret |= spi_write_reg(0x2b, 0xcb);
    ret |= spi_write_reg(0x2c, 0x3e);
    ret |= spi_write_reg(0x2d, 0xcd);
    ret |= spi_write_reg(0x2e, 0x06);
    ret |= spi_write_reg(0x2f, 0x32);
    ret |= spi_write_reg(0x30, 0x54);
    ret |= spi_write_reg(0x31, 0x74);
    ret |= spi_write_reg(0x32, 0x95);
    ret |= spi_write_reg(0x33, 0xc8);
    ret |= spi_write_reg(0x34, 0x21);
    ret |= spi_write_reg(0x35, 0x00);
    ret |= spi_write_reg(0x36, 0x5e);
    ret |= spi_write_reg(0x37, 0x78);
    ret |= spi_write_reg(0x38, 0x5b);
    ret |= spi_write_reg(0x39, 0x80);
    ret |= spi_write_reg(0x3a, 0x80);
    ret |= spi_write_reg(0x3b, 0x80);
    ret |= spi_write_reg(0x3c, 0x80);
    ret |= spi_write_reg(0x3d, 0x00);
    ret |= spi_write_reg(0x3e, 0x3b);
    ret |= spi_write_reg(0x3f, 0x72);
    ret |= spi_write_reg(0x40, 0x40);
    ret |= spi_write_reg(0x41, 0xd0);
    ret |= spi_write_reg(0x42, 0x3d);
    ret |= spi_write_reg(0x43, 0x30);
    ret |= spi_write_reg(0x44, 0x02);
    ret |= spi_write_reg(0x45, 0x7b);
    ret |= spi_write_reg(0x46, 0x0a);
    ret |= spi_write_reg(0x47, 0x59);
    ret |= spi_write_reg(0x48, 0x77);
    ret |= spi_write_reg(0x49, 0x9a);
    ret |= spi_write_reg(0x4a, 0xa7);
    ret |= spi_write_reg(0x4b, 0x0f);
    ret |= spi_write_reg(0x4c, 0xaf);
    ret |= spi_write_reg(0x4d, 0x66);
    ret |= spi_write_reg(0x4e, 0x68);
    ret |= spi_write_reg(0x4f, 0xff);
    ret |= spi_write_reg(0x50, 0xff);
    ret |= spi_write_reg(0x51, 0x92);
    ret |= spi_write_reg(0x52, 0x93);
    ret |= spi_write_reg(0x53, 0x0a);
    ret |= spi_write_reg(0x54, 0x58);
    ret |= spi_write_reg(0x55, 0x78);
    ret |= spi_write_reg(0x56, 0x99);
    ret |= spi_write_reg(0x57, 0xa8);
    ret |= spi_write_reg(0x58, 0xff);
    ret |= spi_write_reg(0x59, 0xff);
    ret |= spi_write_reg(0x5a, 0xff);
    ret |= spi_write_reg(0x5b, 0xff);
    ret |= spi_write_reg(0x5c, 0x20);
    ret |= spi_write_reg(0x5d, 0x06);
    ret |= spi_write_reg(0x5e, 0x18);
    ret |= spi_write_reg(0x5f, 0x19);
    ret |= spi_write_reg(0x60, 0xa8);
    ret |= spi_write_reg(0x61, 0xf0);
    ret |= spi_write_reg(0x62, 0x00);
    ret |= spi_write_reg(0x63, 0x00);
    ret |= spi_write_reg(0x64, 0x76);
    ret |= spi_write_reg(0x65, 0xec);
    ret |= spi_write_reg(0x66, 0x05);
    ret |= spi_write_reg(0x67, 0x5a);
    ret |= spi_write_reg(0x68, 0x85);
    ret |= spi_write_reg(0x69, 0xc0);
    ret |= spi_write_reg(0x6a, 0xfb);
    ret |= spi_write_reg(0x6b, 0x36);
    ret |= spi_write_reg(0x6c, 0x71);
    ret |= spi_write_reg(0x6d, 0xac);
    ret |= spi_write_reg(0x6e, 0xe7);
    ret |= spi_write_reg(0x6f, 0x22);
    ret |= spi_write_reg(0x70, 0x5d);
    ret |= spi_write_reg(0x71, 0x00);
    ret |= spi_write_reg(0x72, 0x1a);
    ret |= spi_write_reg(0x73, 0x3c);
    ret |= spi_write_reg(0x74, 0x72);
    ret |= spi_write_reg(0x75, 0xe4);
    ret |= spi_write_reg(0x76, 0x05);
    ret |= spi_write_reg(0x77, 0x5a);
    ret |= spi_write_reg(0x78, 0x86);
    ret |= spi_write_reg(0x79, 0xc1);
    ret |= spi_write_reg(0x7a, 0xfc);
    ret |= spi_write_reg(0x7b, 0x37);
    ret |= spi_write_reg(0x7c, 0x72);
    ret |= spi_write_reg(0x7d, 0xad);
    ret |= spi_write_reg(0x7e, 0xe8);
    ret |= spi_write_reg(0x7f, 0x23);
    ret |= spi_write_reg(0x80, 0x5e);
    ret |= spi_write_reg(0x81, 0x22);
    ret |= spi_write_reg(0x82, 0x72);
    ret |= spi_write_reg(0x83, 0xa7);
    ret |= spi_write_reg(0x84, 0x73);
    ret |= spi_write_reg(0x85, 0xa8);
    ret |= spi_write_reg(0x86, 0x02);
    ret |= spi_write_reg(0x87, 0x00);
    ret |= spi_write_reg(0x88, 0x00);
    ret |= spi_write_reg(0x89, 0x00);
    ret |= spi_write_reg(0x8a, 0x95);
    ret |= spi_write_reg(0x8b, 0x00);
    ret |= spi_write_reg(0x8c, 0x00);
    ret |= spi_write_reg(0x8d, 0x05);
    ret |= spi_write_reg(0x8e, 0x98);
    ret |= spi_write_reg(0x8f, 0x94);
    ret |= spi_write_reg(0x90, 0x13);
    ret |= spi_write_reg(0x91, 0x94);
    ret |= spi_write_reg(0x92, 0xe6);
    ret |= spi_write_reg(0x93, 0x4a);
    ret |= spi_write_reg(0x94, 0x0e);
    ret |= spi_write_reg(0x95, 0xe1);
    ret |= spi_write_reg(0x96, 0x8a);
    ret |= spi_write_reg(0x97, 0x8d);
    ret |= spi_write_reg(0x98, 0x00);
    ret |= spi_write_reg(0x99, 0x00);
    ret |= spi_write_reg(0x9a, 0x00);
    ret |= spi_write_reg(0x9b, 0x00);
    ret |= spi_write_reg(0x9c, 0x01);
    ret |= spi_write_reg(0x9d, 0x00);
    ret |= spi_write_reg(0x9e, 0x00);
    ret |= spi_write_reg(0x9f, 0x02);
    ret |= spi_write_reg(0xa0, 0xc0);
    ret |= spi_write_reg(0xa1, 0x00);
    
    if (ret != 0) {
        printf("Panel initialization failed\n");
        return -1;
    }
    
    printf("Panel initialization completed successfully\n");
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

	system("echo 1 > /sys/class/gpio/gpio419/value");
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
	usleep(100);
	// SOT (Start of Transmission) 配置
	spi_write_reg(0x16, 0x60);
	spi_write_reg(0x17, 0xe0);
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
