/**
 * @file    bootanimation.c
 * @brief   Combined VO initialization and framebuffer animation
 * @details Initialize display hardware and run rectangle collision animation
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
#define BUFFER_MODE 3    // Triple buffer
#define USE_VSYNC 0      // Disable VSync
#define ENHANCED_SYNC 0  // Disable enhanced sync

// 动画参数
#define RECT_WIDTH 80
#define RECT_HEIGHT 80
#define NUM_RECTS 8
#define FPS 60

// VSSDK Define
#define FBIOPUT_SHOW_VS_FB          _IOW('F', 0x101, int)
#define FBIOGET_SHOW_VS_FB          _IOR('F', 0x102, int)
#define FBIOPUT_SCREEN_ORIGIN_VS_FB _IOW('F', 0x103, struct fb_point)
#define FBIOGET_SCREEN_ORIGIN_VS_FB _IOR('F', 0x104, struct fb_point)
#define FBIOPUT_LAYER_INFO          _IOW('F', 0x105, struct fb_layer_info)
#define FBIOGET_LAYER_INFO          _IOR('F', 0x106, struct fb_layer_info)
#define FBIO_REFRESH                _IOW('F', 0x107, struct fb_buffer)

#ifndef FBIO_WAITFORVSYNC
#define FBIO_WAITFORVSYNC           _IOW('F', 0x20, __u32)
#endif

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

typedef enum {
    BUFFER_MODE_SINGLE = 0,
    BUFFER_MODE_DOUBLE_PAN,
    BUFFER_MODE_DOUBLE_COPY,
    BUFFER_MODE_TRIPLE,
} buffer_mode_t;

struct Rectangle {
    int x, y;
    int velo_x, velo_y;
    uint32_t color;
};

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
    
    buffer_mode_t buffer_mode;
    int current_buffer;
    char *buffer[3];
    int use_vsync;
    int enhanced_sync;
    
    long frame_count;
    struct timespec last_fps_time;
} fb_layer_config;

static volatile sig_atomic_t g_stop_flag = 0;
static struct Rectangle rects[NUM_RECTS];
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
    config->vinfo.yres_virtual = config->img_height * 3;  // Triple buffer
    
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

    // 设置三缓冲区指针
    size_t buffer_size = config->finfo.line_length * config->img_height;
    config->buffer[0] = config->framebuffer;
    config->buffer[1] = config->framebuffer + buffer_size;
    config->buffer[2] = config->framebuffer + buffer_size * 2;
    config->current_buffer = 0;
    
    // 清空所有缓冲区
    memset(config->buffer[0], 0, buffer_size);
    memset(config->buffer[1], 0, buffer_size);
    memset(config->buffer[2], 0, buffer_size);

    set_layer_position(config);
    set_layer_show(config, 1);
    
    // 初始化性能统计
    config->frame_count = 0;
    clock_gettime(CLOCK_MONOTONIC, &config->last_fps_time);
    
    printf("Triple buffering mode initialized\n");
    
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

void initialize_rects(fb_layer_config *config) {
    uint32_t colors[NUM_RECTS] = {
        0xFFFF0000,  // Red
        0xFF00FF00,  // Green  
        0xFF0000FF,  // Blue
        0xFFFFFF00,  // Yellow
        0xFFFF00FF,  // Purple
        0xFF00FFFF,  // Cyan
        0xFF808080,  // Gray
        0xFFFFFFFF   // White
    };

    srand(time(NULL));
    
    for (int i = 0; i < NUM_RECTS; i++) {
        rects[i].x = rand() % (config->img_width - RECT_WIDTH);
        rects[i].y = rand() % (config->img_height - RECT_HEIGHT);
        rects[i].velo_x = (rand() % 10) + 5;
        rects[i].velo_y = (rand() % 10) + 5;
        rects[i].color = colors[i];
        
        if (rand() % 2) rects[i].velo_x = -rects[i].velo_x;
        if (rand() % 2) rects[i].velo_y = -rects[i].velo_y;
    }
}

void update_rect_positions(fb_layer_config *config) {
    for (int i = 0; i < NUM_RECTS; i++) {
        rects[i].x += rects[i].velo_x;
        rects[i].y += rects[i].velo_y;

        if (rects[i].x <= 0 || rects[i].x + RECT_WIDTH >= config->img_width) {
            rects[i].velo_x = -rects[i].velo_x;
            rects[i].x = (rects[i].x <= 0) ? 0 : config->img_width - RECT_WIDTH;
        }
        if (rects[i].y <= 0 || rects[i].y + RECT_HEIGHT >= config->img_height) {
            rects[i].velo_y = -rects[i].velo_y;
            rects[i].y = (rects[i].y <= 0) ? 0 : config->img_height - RECT_HEIGHT;
        }
    }
}

void put_pixel_to_buffer(char *buffer, fb_layer_config *config, int x, int y, uint32_t color) {
    if (x < 0 || x >= config->img_width || y < 0 || y >= config->img_height) {
        return;
    }
    
    long location = x * (config->vinfo.bits_per_pixel / 8) + y * config->finfo.line_length;
    *((uint32_t*)(buffer + location)) = color;
}

void clear_buffer(char *buffer, fb_layer_config *config) {
    memset(buffer, 0, config->finfo.line_length * config->img_height);
}

void draw_rectangle_to_buffer(char *buffer, fb_layer_config *config, int x, int y, int w, int h, uint32_t color) {
    for (int py = y; py < y + h; py++) {
        for (int px = x; px < x + w; px++) {
            put_pixel_to_buffer(buffer, config, px, py, color);
        }
    }
}

void draw_rects_to_buffer(char *buffer, fb_layer_config *config) {
    clear_buffer(buffer, config);
    
    for (int i = 0; i < NUM_RECTS; i++) {
        draw_rectangle_to_buffer(buffer, config, rects[i].x, rects[i].y, RECT_WIDTH, RECT_HEIGHT, rects[i].color);
    }
}

void draw_border_to_buffer(char *buffer, fb_layer_config *config) {
    uint32_t border_color = 0xFFFFFFFF;
    
    for (int x = 0; x < config->img_width; x++) {
        put_pixel_to_buffer(buffer, config, x, 0, border_color);
        put_pixel_to_buffer(buffer, config, x, config->img_height - 1, border_color);
    }
    
    for (int y = 0; y < config->img_height; y++) {
        put_pixel_to_buffer(buffer, config, 0, y, border_color);
        put_pixel_to_buffer(buffer, config, config->img_width - 1, y, border_color);
    }
}

int pan_display(fb_layer_config *config, int buffer_index) {
    struct fb_var_screeninfo var = config->vinfo;
    var.yoffset = buffer_index * config->img_height;
    
    if (ioctl(config->fd, FBIOPAN_DISPLAY, &var) != 0) {
        return -1;
    }
    
    return 0;
}

void swap_buffers(fb_layer_config *config) {
    // Triple buffer mode with pan display
    if (pan_display(config, config->current_buffer) == 0) {
        config->current_buffer = (config->current_buffer + 1) % 3;
    }
}

void print_fps_stats(fb_layer_config *config) {
    config->frame_count++;
    
    if (config->frame_count % 60 == 0) {
        struct timespec current_time;
        clock_gettime(CLOCK_MONOTONIC, &current_time);
        
        long elapsed_ns = (current_time.tv_sec - config->last_fps_time.tv_sec) * 1000000000 +
                         (current_time.tv_nsec - config->last_fps_time.tv_nsec);
        double fps = 60.0 * 1000000000.0 / elapsed_ns;
        
        printf("FPS: %.2f (Triple buffer mode)\n", fps);
        
        config->last_fps_time = current_time;
    }
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
    printf("Usage: %s [resolution_index]\n", prog_name);
    printf("resolution_index:\n");
    printf("\t 0) 1280x960 (default)\n");
    printf("\nThis program initializes VO/MIPI-TX and runs rectangle collision animation.\n");
    printf("Fixed configuration: fb0, position (0,0), triple buffering, no VSync\n");
}

int main(int argc, char *argv[])
{
    vs_int32_t ret = VS_SUCCESS;
    vs_size_s display_size = {DISPLAY_WIDTH, DISPLAY_HEIGHT};

    printf("Starting integrated VO initialization and framebuffer animation\n");

    if (argc > 1) {
        if (!strncmp(argv[1], "-h", 2)) {
            print_usage(argv[0]);
            return VS_SUCCESS;
        }
    }

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
    layer_config.buffer_mode = BUFFER_MODE_TRIPLE;
    layer_config.use_vsync = USE_VSYNC;
    layer_config.enhanced_sync = ENHANCED_SYNC;
    layer_config.current_buffer = 0;
    layer_config.vo_layerid = 2;  // fb0对应layer 2

    printf("Configuration:\n");
    printf("  Display: %dx%d\n", DISPLAY_WIDTH, DISPLAY_HEIGHT);
    printf("  Framebuffer: /dev/fb%d\n", FB_ID);
    printf("  Position: (%d, %d)\n", LAYER_POS_X, LAYER_POS_Y);
    printf("  Buffer Mode: Triple buffering\n");
    printf("  VSync: Disabled\n");

    if (init_framebuffer_layer(&layer_config) != 0) {
        vs_sample_trace("Framebuffer initialization failed\n");
        cleanup_vo_hardware();
        return 1;
    }

    // 第三阶段：运行动画
    vs_sample_trace("Starting rectangle collision animation...\n");
    
    initialize_rects(&layer_config);

    struct timespec start, end;
    long frame_duration = 1000000000 / FPS;
    
    printf("Animation started (Press Ctrl+C to exit)\n");
    
    while (!g_stop_flag) {
        clock_gettime(CLOCK_MONOTONIC, &start);

        // 在后台缓冲区绘制
        char *back_buffer = layer_config.buffer[layer_config.current_buffer];
        
        update_rect_positions(&layer_config);
        draw_rects_to_buffer(back_buffer, &layer_config);
        draw_border_to_buffer(back_buffer, &layer_config);

        // 交换缓冲区
        swap_buffers(&layer_config);
        
        // 性能统计
        print_fps_stats(&layer_config);

        // 帧率控制
        clock_gettime(CLOCK_MONOTONIC, &end);
        long elapsed = (end.tv_sec - start.tv_sec) * 1000000000 + 
                      (end.tv_nsec - start.tv_nsec);
        
        if (elapsed < frame_duration) {
            struct timespec remaining;
            remaining.tv_sec = 0;
            remaining.tv_nsec = frame_duration - elapsed;
            nanosleep(&remaining, NULL);
        }
    }

    // 清理资源
    printf("\nCleaning up...\n");
    cleanup_framebuffer_layer(&layer_config);
    cleanup_vo_hardware();
    
    printf("Program completed successfully\n");
    return ret;
}