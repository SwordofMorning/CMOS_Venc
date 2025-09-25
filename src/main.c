/**
 * @file    bootanimation.c
 * @brief   Combined VO initialization and static logo display
 * @details Initialize display hardware and show static BMP logo
 * @author  Combined for integrated usage
 * @date    2025-01-01
 * @version v1.00
 */
#define _GNU_SOURCE
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <dlfcn.h>
#include <pthread.h>
#include <linux/videodev2.h>
#include <sys/mman.h>
#include <time.h>
#include <linux/fb.h>

#include "sample_common.h"
#include "vs_mal_vii.h"
#include "vs_mal_vpp.h"

// 固定配置参数
#define DISPLAY_WIDTH 1280
#define DISPLAY_HEIGHT 960
#define FB_ID 0
#define LAYER_POS_X 0
#define LAYER_POS_Y 0
#define LOGO_FILE "logo.bmp"

// VSSDK Define
#define FBIOPUT_SHOW_VS_FB          _IOW('F', 0x101, int)
#define FBIOGET_SHOW_VS_FB          _IOR('F', 0x102, int)
#define FBIOPUT_SCREEN_ORIGIN_VS_FB _IOW('F', 0x103, struct fb_point)
#define FBIOGET_SCREEN_ORIGIN_VS_FB _IOR('F', 0x104, struct fb_point)
#define FBIOPUT_LAYER_INFO          _IOW('F', 0x105, struct fb_layer_info)
#define FBIOGET_LAYER_INFO          _IOR('F', 0x106, struct fb_layer_info)
#define FBIO_REFRESH                _IOW('F', 0x107, struct fb_buffer)

// VS Struct
struct fb_point {
    int x;
    int y;
};

struct fb_layer_info {
    int layer_id;
    int buf_mode;
    int canvas_width;
    int canvas_height;
    int mask;
};

struct fb_buffer {
    struct fb_surface {
        uint32_t phy_addr;
        uint32_t width;
        uint32_t height;
        uint32_t pitch;
    } canvas;
    struct fb_rect {
        int x, y, w, h;
    } update_rect;
};

// BMP文件头结构
#pragma pack(push, 1)
typedef struct {
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
} BITMAPFILEHEADER;

typedef struct {
    uint32_t biSize;
    int32_t biWidth;
    int32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t biXPelsPerMeter;
    int32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
} BITMAPINFOHEADER;
#pragma pack(pop)

typedef struct {
    int fb_id;
    int fd;
    int vo_layerid;
    struct fb_point point;
    int img_width;
    int img_height;
    char *framebuffer;
    long screensize;
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
} fb_layer_config;

static volatile sig_atomic_t g_stop_flag = 0;
static fb_layer_config layer_config = {0};

void signal_handler(int sig) {
    g_stop_flag = 1;
    vs_sample_trace("Received signal %d, preparing to exit...\n", sig);
}

static vs_void_t sample_vo_only_get_vo_cfg(vs_size_s *img_size, sample_vo_cfg_s *vo_cfg)
{
    vo_cfg->vo_devid = 0;
    vo_cfg->vo_layerid = 0;

    vo_cfg->vo_intf_type = E_VO_INTERFACE_TYPE_MIPI;
    vo_cfg->vo_output = E_VO_OUTPUT_TYPE_USER;
    vo_cfg->bg_color = 0x000000;  // 黑色背景
    vo_cfg->dynamic_range = E_DYNAMIC_RANGE_SDR8;
    vo_cfg->pixel_format = E_PIXEL_FORMAT_YVU_420SP;
    vo_cfg->vo_mode = E_VO_MODE_1MUX;
    
    vo_cfg->img_width = img_size->width;
    vo_cfg->img_height = img_size->height;
    vo_cfg->enable = VS_TRUE;
    vo_cfg->zorder = 0;
    vo_cfg->mipitx_phy_rate = 820;
    vo_cfg->rotation_enable = VS_FALSE;
    
    vs_sample_trace("VO config: %dx%d, format=%d, intf=%d\n", 
                    vo_cfg->img_width, vo_cfg->img_height, 
                    vo_cfg->pixel_format, vo_cfg->vo_intf_type);
}

static vs_int32_t init_vo_hardware(vs_size_s *display_size)
{
    vs_int32_t ret;
    vs_vb_cfg_s vb_cfg = {0};
    sample_vo_cfg_s vo_cfg = {0};
    vs_char_t dsp_firmware[100] = "/lib/firmware/vs_dsp0.bin";

    vs_sample_trace("Initializing VO hardware...\n");

    // 配置最小的视频缓冲区
    vb_cfg.pool_cnt = 1;
    vb_cfg.ast_commpool[0].blk_size = sample_common_buffer_size_get(display_size, 
                                                                    E_PIXEL_FORMAT_YVU_420SP, 
                                                                    E_COMPRESS_MODE_NONE, 1);
    vb_cfg.ast_commpool[0].blk_cnt = 2;
    vb_cfg.ast_commpool[0].remap_mode = VB_REMAP_MODE_NONE;

    // 初始化系统
    ret = sample_common_sys_init(&vb_cfg);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("sample_common_sys_init failed, ret: 0x%x\n", ret);
        return ret;
    }

    // 获取VO配置
    sample_vo_only_get_vo_cfg(display_size, &vo_cfg);

    // 初始化DSP（MIPI-TX需要）
    if (vo_cfg.vo_intf_type == E_VO_INTERFACE_TYPE_MIPI) {
        ret = sample_common_dsp_init(0, dsp_firmware);
        if (ret != VS_SUCCESS) {
            vs_sample_trace("sample_common_dsp_init failed, ret: 0x%x\n", ret);
            sample_common_sys_exit();
            return ret;
        }
    }

    // 启动VO
    ret = sample_common_vo_start(&vo_cfg);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("sample_common_vo_start failed, ret: 0x%x\n", ret);
        if (vo_cfg.vo_intf_type == E_VO_INTERFACE_TYPE_MIPI) {
            sample_common_dsp_exit(0);
        }
        sample_common_sys_exit();
        return ret;
    }

    vs_sample_trace("VO and MIPI-TX initialization completed successfully!\n");
    vs_sample_trace("Display resolution: %dx%d\n", display_size->width, display_size->height);
    
    return VS_SUCCESS;
}

static void cleanup_vo_hardware(void)
{
    vs_sample_trace("Cleaning up VO hardware...\n");
    
    sample_vo_cfg_s vo_cfg = {0};
    vs_size_s display_size = {DISPLAY_WIDTH, DISPLAY_HEIGHT};
    
    sample_vo_only_get_vo_cfg(&display_size, &vo_cfg);
    sample_common_vo_stop(&vo_cfg);
    
    if (vo_cfg.vo_intf_type == E_VO_INTERFACE_TYPE_MIPI) {
        sample_common_dsp_exit(0);
    }
    
    sample_common_sys_exit();
}

int set_layer_show(fb_layer_config *config, int show) {
    if (ioctl(config->fd, FBIOPUT_SHOW_VS_FB, &show) != 0) {
        printf("Warning: Failed to set layer show state (using standard fb)\n");
        return 0;
    }
    printf("Layer %d show state set to: %s\n", config->fb_id, show ? "visible" : "hidden");
    return 0;
}

int set_layer_position(fb_layer_config *config) {
    if (ioctl(config->fd, FBIOPUT_SCREEN_ORIGIN_VS_FB, &config->point) != 0) {
        printf("Warning: Failed to set layer position (using standard fb)\n");
        return 0;
    }
    printf("Layer %d position set to: (%d, %d)\n", 
           config->fb_id, config->point.x, config->point.y);
    return 0;
}

int init_framebuffer_layer(fb_layer_config *config) {
    char dev_name[16];
    
    snprintf(dev_name, sizeof(dev_name), "/dev/fb%d", config->fb_id);
    
    config->fd = open(dev_name, O_RDWR);
    if (config->fd == -1) {
        perror("Error opening framebuffer device");
        return -1;
    }
    
    printf("Opened framebuffer device: %s\n", dev_name);

    set_layer_show(config, 0);

    if (ioctl(config->fd, FBIOGET_VSCREENINFO, &config->vinfo) == -1) {
        perror("Error reading variable information");
        close(config->fd);
        return -1;
    }

    // 设置framebuffer参数
    config->vinfo.xres = config->img_width;
    config->vinfo.yres = config->img_height;
    config->vinfo.xres_virtual = config->img_width;
    config->vinfo.yres_virtual = config->img_height;  // 静态显示只需要单缓冲
    
    // 设置像素格式为ARGB8888
    config->vinfo.bits_per_pixel = 32;
    config->vinfo.red.offset = 16;
    config->vinfo.red.length = 8;
    config->vinfo.green.offset = 8;
    config->vinfo.green.length = 8;
    config->vinfo.blue.offset = 0;
    config->vinfo.blue.length = 8;
    config->vinfo.transp.offset = 24;
    config->vinfo.transp.length = 8;

    if (ioctl(config->fd, FBIOPUT_VSCREENINFO, &config->vinfo) == -1) {
        perror("Error setting variable information");
        close(config->fd);
        return -1;
    }

    if (ioctl(config->fd, FBIOGET_FSCREENINFO, &config->finfo) == -1) {
        perror("Error reading fixed information");
        close(config->fd);
        return -1;
    }

    printf("Layer info: %dx%d, %d bpp, virtual: %dx%d\n", 
           config->vinfo.xres, config->vinfo.yres, config->vinfo.bits_per_pixel,
           config->vinfo.xres_virtual, config->vinfo.yres_virtual);

    config->screensize = config->finfo.smem_len;

    config->framebuffer = (char*)mmap(0, config->screensize, PROT_READ | PROT_WRITE, 
                                     MAP_SHARED, config->fd, 0);
    
    if (config->framebuffer == MAP_FAILED) {
        perror("Error mapping framebuffer device to memory");
        close(config->fd);
        return -1;
    }

    printf("Framebuffer mapped to memory, size: %ld bytes\n", config->screensize);

    // 清空framebuffer
    memset(config->framebuffer, 0, config->screensize);

    set_layer_position(config);
    set_layer_show(config, 1);
    
    printf("Single buffer mode initialized for static display\n");
    
    return 0;
}

void cleanup_framebuffer_layer(fb_layer_config *config) {
    if (config->framebuffer && config->framebuffer != MAP_FAILED) {
        memset(config->framebuffer, 0, config->screensize);
        munmap(config->framebuffer, config->screensize);
        config->framebuffer = NULL;
    }
    
    if (config->fd != -1) {
        set_layer_show(config, 0);
        close(config->fd);
        config->fd = -1;
    }
}

void put_pixel_to_buffer(char *buffer, fb_layer_config *config, int x, int y, uint32_t color) {
    if (x < 0 || x >= config->img_width || y < 0 || y >= config->img_height) {
        return;
    }
    
    long location = x * (config->vinfo.bits_per_pixel / 8) + y * config->finfo.line_length;
    *((uint32_t*)(buffer + location)) = color;
}

int load_bmp_to_framebuffer(const char* filename, fb_layer_config *config) {
    FILE *file;
    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;
    
    // 打开BMP文件
    file = fopen(filename, "rb");
    if (!file) {
        printf("Error: Cannot open BMP file: %s\n", filename);
        return -1;
    }
    
    // 读取文件头
    if (fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, file) != 1) {
        printf("Error: Cannot read BMP file header\n");
        fclose(file);
        return -1;
    }
    
    // 检查BMP标识
    if (fileHeader.bfType != 0x4D42) { // "BM"
        printf("Error: Not a valid BMP file\n");
        fclose(file);
        return -1;
    }
    
    // 读取信息头
    if (fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, file) != 1) {
        printf("Error: Cannot read BMP info header\n");
        fclose(file);
        return -1;
    }
    
    printf("BMP Info: %dx%d, %d bits\n", infoHeader.biWidth, infoHeader.biHeight, infoHeader.biBitCount);
    
    // 检查图片尺寸
    if (infoHeader.biWidth != DISPLAY_WIDTH || abs(infoHeader.biHeight) != DISPLAY_HEIGHT) {
        printf("Error: BMP size (%dx%d) doesn't match display size (%dx%d)\n", 
               infoHeader.biWidth, abs(infoHeader.biHeight), DISPLAY_WIDTH, DISPLAY_HEIGHT);
        fclose(file);
        return -1;
    }
    
    // 目前只支持24位和32位BMP
    if (infoHeader.biBitCount != 24 && infoHeader.biBitCount != 32) {
        printf("Error: Only 24-bit and 32-bit BMP files are supported\n");
        fclose(file);
        return -1;
    }
    
    // 跳转到像素数据
    fseek(file, fileHeader.bfOffBits, SEEK_SET);
    
    // 计算行填充
    int row_padded = (infoHeader.biWidth * infoHeader.biBitCount / 8 + 3) & (~3);
    unsigned char *row_data = malloc(row_padded);
    if (!row_data) {
        printf("Error: Cannot allocate memory for row data\n");
        fclose(file);
        return -1;
    }
    
    // 读取像素数据
    int height = abs(infoHeader.biHeight);
    int is_bottom_up = (infoHeader.biHeight > 0);
    
    for (int y = 0; y < height; y++) {
        if (fread(row_data, row_padded, 1, file) != 1) {
            printf("Error: Cannot read pixel data at row %d\n", y);
            free(row_data);
            fclose(file);
            return -1;
        }
        
        // 计算实际的y坐标（BMP通常是底部向上存储）
        int actual_y = is_bottom_up ? (height - 1 - y) : y;
        
        for (int x = 0; x < infoHeader.biWidth; x++) {
            uint32_t pixel = 0xFF000000; // Alpha = 255
            
            if (infoHeader.biBitCount == 24) {
                // 24位BMP: BGR顺序
                unsigned char b = row_data[x * 3 + 0];
                unsigned char g = row_data[x * 3 + 1];
                unsigned char r = row_data[x * 3 + 2];
                pixel |= (r << 16) | (g << 8) | b;
            } else if (infoHeader.biBitCount == 32) {
                // 32位BMP: BGRA顺序
                unsigned char b = row_data[x * 4 + 0];
                unsigned char g = row_data[x * 4 + 1];
                unsigned char r = row_data[x * 4 + 2];
                unsigned char a = row_data[x * 4 + 3];
                pixel = (a << 24) | (r << 16) | (g << 8) | b;
            }
            
            put_pixel_to_buffer(config->framebuffer, config, x, actual_y, pixel);
        }
    }
    
    free(row_data);
    fclose(file);
    
    printf("BMP file loaded successfully: %s\n", filename);
    return 0;
}

static void register_signal_handler(void (*sig_handler)(int))
{
    struct sigaction sa;
    memset(&sa, 0, sizeof(struct sigaction));
    sa.sa_handler = sig_handler;
    sa.sa_flags = 0;

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
}

void print_usage(const char* prog_name) {
    printf("Usage: %s [logo_file]\n", prog_name);
    printf("logo_file: BMP file to display (default: logo.bmp)\n");
    printf("\nThis program initializes VO/MIPI-TX and displays a static BMP logo.\n");
    printf("Fixed configuration: 1280x960, fb0, position (0,0)\n");
    printf("BMP file must be 1280x960, 24-bit or 32-bit format\n");
}

int main(int argc, char *argv[])
{
    vs_int32_t ret = VS_SUCCESS;
    vs_size_s display_size = {DISPLAY_WIDTH, DISPLAY_HEIGHT};
    const char *logo_file = LOGO_FILE;

    printf("Starting integrated VO initialization and logo display\n");

    // 解析命令行参数
    if (argc > 1) {
        if (!strncmp(argv[1], "-h", 2)) {
            print_usage(argv[0]);
            return VS_SUCCESS;
        }
        logo_file = argv[1];
    }

    printf("Logo file: %s\n", logo_file);

    // 注册信号处理器
    register_signal_handler(signal_handler);

    // 第一阶段：初始化VO硬件
    ret = init_vo_hardware(&display_size);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("VO hardware initialization failed with error: 0x%x\n", ret);
        return ret;
    }

    // 第二阶段：初始化framebuffer
    vs_sample_trace("Initializing framebuffer layer...\n");
    
    // 设置固定配置
    layer_config.fb_id = FB_ID;
    layer_config.point.x = LAYER_POS_X;
    layer_config.point.y = LAYER_POS_Y;
    layer_config.img_width = DISPLAY_WIDTH;
    layer_config.img_height = DISPLAY_HEIGHT;
    layer_config.fd = -1;
    layer_config.vo_layerid = 2;  // fb0对应layer 2

    printf("Configuration:\n");
    printf("  Display: %dx%d\n", DISPLAY_WIDTH, DISPLAY_HEIGHT);
    printf("  Framebuffer: /dev/fb%d\n", FB_ID);
    printf("  Position: (%d, %d)\n", LAYER_POS_X, LAYER_POS_Y);
    printf("  Mode: Static logo display\n");

    if (init_framebuffer_layer(&layer_config) != 0) {
        vs_sample_trace("Framebuffer initialization failed\n");
        cleanup_vo_hardware();
        return 1;
    }

    // 第三阶段：加载并显示logo
    vs_sample_trace("Loading and displaying logo...\n");
    
    if (load_bmp_to_framebuffer(logo_file, &layer_config) != 0) {
        vs_sample_trace("Failed to load logo file: %s\n", logo_file);
        cleanup_framebuffer_layer(&layer_config);
        cleanup_vo_hardware();
        return 1;
    }

    vs_sample_trace("Logo displayed successfully. Press Ctrl+C to exit.\n");

    // 等待退出信号
    while (!g_stop_flag) {
        usleep(100000); // 100ms
    }

    // 清理资源
    printf("\nCleaning up...\n");
    cleanup_framebuffer_layer(&layer_config);
    cleanup_vo_hardware();
    
    printf("Program completed successfully\n");
    return ret;
}