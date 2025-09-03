#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <time.h>
#include <linux/fb.h>
#include <signal.h>
#include <stdint.h>

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

// Vsync
#ifndef FBIO_WAITFORVSYNC
#define FBIO_WAITFORVSYNC           _IOW('F', 0x20, __u32)
#endif

// DRM
#ifndef DRM_IOCTL_WAIT_VBLANK
#define DRM_IOCTL_WAIT_VBLANK       _IOWR('d', 0x3a, union drm_wait_vblank)
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

#define FB_LAYER_MASK_BUFMODE    0x01
#define FB_LAYER_MASK_CANVASSIZE 0x02

typedef enum {
    BUFFER_MODE_SINGLE = 0,
    BUFFER_MODE_DOUBLE_PAN,
    BUFFER_MODE_DOUBLE_COPY,
    BUFFER_MODE_TRIPLE,
} buffer_mode_t;

typedef enum {
    E_PIXEL_FORMAT_ARGB8888 = 0,
    E_PIXEL_FORMAT_RGB565,
    E_PIXEL_FORMAT_ARGB1555,
    E_PIXEL_FORMAT_ARGB4444,
} pixel_format_e;

struct Rectangle {
    int x, y;
    int velo_x, velo_y;
    uint32_t color;
};

typedef struct {
    int fb_id;                  // framebuffer设备ID (0-3)
    int fd;                     // 文件描述符
    int vo_layerid;             // video output layer ID
    pixel_format_e pixel_format; // 像素格式
    struct fb_point point;      // layer在屏幕上的位置
    int img_width;              // layer图像宽度
    int img_height;             // layer图像高度
    char *framebuffer;          // mmap的framebuffer指针
    long screensize;            // framebuffer大小
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    
    // 缓冲区管理
    buffer_mode_t buffer_mode;  // 缓冲模式
    int current_buffer;         // 当前缓冲区索引
    char *buffer[3];            // 最多三个缓冲区指针
    int use_vsync;              // 是否使用垂直同步
    int enhanced_sync;          // 是否使用增强同步
    
    // 性能统计
    long frame_count;
    struct timespec last_fps_time;
} fb_layer_config;

struct Rectangle rects[NUM_RECTS];
static volatile sig_atomic_t stop_flag = 0;
static fb_layer_config layer_config = {0};

void signal_handler(int sig) {
    stop_flag = 1;
}

void print_usage(const char* prog_name) {
    printf("Usage: %s [fb_id] [x_pos] [y_pos] [width] [height] [buffer_mode] [vsync] [enhanced_sync]\n", prog_name);
    printf("  fb_id:         framebuffer device ID (0-3), default: 0\n");
    printf("  x_pos:         layer X position on screen, default: 50\n");
    printf("  y_pos:         layer Y position on screen, default: 50\n");
    printf("  width:         layer width, default: 800\n");
    printf("  height:        layer height, default: 600\n");
    printf("  buffer_mode:   buffering mode, default: 2\n");
    printf("                 0 = Single buffer\n");
    printf("                 1 = Double buffer with pan display\n");
    printf("                 2 = Double buffer with memory copy\n");
    printf("                 3 = Triple buffer\n");
    printf("  vsync:         enable vertical sync (0/1), default: 1\n");
    printf("  enhanced_sync: use enhanced sync methods (0/1), default: 1\n");
    printf("\nExample:\n");
    printf("  %s 0 100 100 800 600 2 1 1  # Use fb0 with double buffer copy mode\n", prog_name);
    printf("  %s 1 200 200 640 480 1 1 0  # Use fb1 with double buffer pan mode\n", prog_name);
}

int enhanced_wait_for_vsync(fb_layer_config *config) {
    if (!config->use_vsync) {
        return 0;
    }
    
    static int vsync_method = -1; // -1: 未测试, 0: 不支持, 1: 标准支持, 2: 增强支持
    
    if (vsync_method == -1) {
        // 第一次调用，测试支持情况
        __u32 dummy = 0;
        if (ioctl(config->fd, FBIO_WAITFORVSYNC, &dummy) == 0) {
            vsync_method = 1;
            printf("Standard FBIO_WAITFORVSYNC supported\n");
        } else {
            vsync_method = 0;
            printf("FBIO_WAITFORVSYNC not supported, using timer-based sync\n");
        }
    }
    
    if (vsync_method == 1) {
        __u32 dummy = 0;
        if (ioctl(config->fd, FBIO_WAITFORVSYNC, &dummy) == 0) {
            return 0;
        }
    }
    
    // 回退到精确的定时器同步
    struct timespec sleep_time;
    sleep_time.tv_sec = 0;
    sleep_time.tv_nsec = 1000000000 / 60; // 60Hz
    nanosleep(&sleep_time, NULL);
    
    return -1;
}

// 双缓冲内存拷贝模式
int copy_buffer_to_display(fb_layer_config *config, char *source_buffer) {
    if (!source_buffer) {
        return -1;
    }
    
    // 等待垂直同步
    if (config->enhanced_sync) {
        enhanced_wait_for_vsync(config);
    }
    
    // 将后台缓冲区内容拷贝到显示缓冲区
    size_t buffer_size = config->finfo.line_length * config->img_height;
    memcpy(config->framebuffer, source_buffer, buffer_size);
    
    // 同步缓存（某些系统需要）
    msync(config->framebuffer, buffer_size, MS_SYNC);
    
    return 0;
}

// Pan Display模式
int pan_display(fb_layer_config *config, int buffer_index) {
    if (config->buffer_mode != BUFFER_MODE_DOUBLE_PAN && 
        config->buffer_mode != BUFFER_MODE_TRIPLE) {
        return 0;
    }
    
    struct fb_var_screeninfo var = config->vinfo;
    
    // 等待垂直同步
    if (config->enhanced_sync) {
        enhanced_wait_for_vsync(config);
    }
    
    // 设置y偏移来切换缓冲区
    var.yoffset = buffer_index * config->img_height;
    
    if (ioctl(config->fd, FBIOPAN_DISPLAY, &var) != 0) {
        printf("Warning: FBIOPAN_DISPLAY failed, falling back to copy mode\n");
        return -1;
    }
    
    return 0;
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
    
    // 根据缓冲模式设置虚拟高度
    switch (config->buffer_mode) {
        case BUFFER_MODE_SINGLE:
            config->vinfo.yres_virtual = config->img_height;
            printf("Single buffering mode\n");
            break;
            
        case BUFFER_MODE_DOUBLE_PAN:
            config->vinfo.yres_virtual = config->img_height * 2;
            printf("Double buffering with pan display\n");
            break;
            
        case BUFFER_MODE_DOUBLE_COPY:
            config->vinfo.yres_virtual = config->img_height;
            printf("Double buffering with memory copy\n");
            break;
            
        case BUFFER_MODE_TRIPLE:
            config->vinfo.yres_virtual = config->img_height * 3;
            printf("Triple buffering mode\n");
            break;
    }
    
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

    // 设置缓冲区指针
    size_t buffer_size = config->finfo.line_length * config->img_height;
    
    switch (config->buffer_mode) {
        case BUFFER_MODE_SINGLE:
            config->buffer[0] = config->framebuffer;
            config->current_buffer = 0;
            memset(config->framebuffer, 0, config->screensize);
            break;
            
        case BUFFER_MODE_DOUBLE_PAN:
            config->buffer[0] = config->framebuffer;
            config->buffer[1] = config->framebuffer + buffer_size;
            config->current_buffer = 0;
            memset(config->buffer[0], 0, buffer_size);
            memset(config->buffer[1], 0, buffer_size);
            break;
            
        case BUFFER_MODE_DOUBLE_COPY:
            config->buffer[0] = config->framebuffer;
            config->buffer[1] = malloc(buffer_size);
            if (!config->buffer[1]) {
                printf("Failed to allocate back buffer\n");
                munmap(config->framebuffer, config->screensize);
                close(config->fd);
                return -1;
            }
            config->current_buffer = 0;
            memset(config->buffer[0], 0, buffer_size);
            memset(config->buffer[1], 0, buffer_size);
            printf("Allocated separate back buffer: %p\n", (void*)config->buffer[1]);
            break;
            
        case BUFFER_MODE_TRIPLE:
            config->buffer[0] = config->framebuffer;
            config->buffer[1] = config->framebuffer + buffer_size;
            config->buffer[2] = config->framebuffer + buffer_size * 2;
            config->current_buffer = 0;
            memset(config->buffer[0], 0, buffer_size);
            memset(config->buffer[1], 0, buffer_size);
            memset(config->buffer[2], 0, buffer_size);
            break;
    }

    set_layer_position(config);
    set_layer_show(config, 1);
    
    // 初始化性能统计
    config->frame_count = 0;
    clock_gettime(CLOCK_MONOTONIC, &config->last_fps_time);
    
    printf("Buffer mode: %d, Enhanced sync: %s\n", 
           config->buffer_mode, config->enhanced_sync ? "enabled" : "disabled");
    
    return 0;
}

void cleanup_framebuffer_layer(fb_layer_config *config) {
    // 清理分配的后台缓冲区
    if (config->buffer_mode == BUFFER_MODE_DOUBLE_COPY && config->buffer[1]) {
        free(config->buffer[1]);
        config->buffer[1] = NULL;
    }
    
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
    
    if (config->vinfo.bits_per_pixel == 32) {
        *((uint32_t*)(buffer + location)) = color;
    } else if (config->vinfo.bits_per_pixel == 16) {
        uint16_t r = (color >> 16) & 0xFF;
        uint16_t g = (color >> 8) & 0xFF;
        uint16_t b = color & 0xFF;
        uint16_t color565 = ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
        *((uint16_t*)(buffer + location)) = color565;
    }
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

void swap_buffers(fb_layer_config *config) {
    switch (config->buffer_mode) {
        case BUFFER_MODE_SINGLE:
            // 单缓冲模式，无需切换
            break;
            
        case BUFFER_MODE_DOUBLE_PAN:
            // Pan Display模式
            if (pan_display(config, config->current_buffer) == 0) {
                config->current_buffer = 1 - config->current_buffer;
            } else {
                // Pan Display失败，回退到拷贝模式
                copy_buffer_to_display(config, config->buffer[config->current_buffer]);
                config->current_buffer = 1 - config->current_buffer;
            }
            break;
            
        case BUFFER_MODE_DOUBLE_COPY:
            // 内存拷贝模式
            copy_buffer_to_display(config, config->buffer[config->current_buffer]);
            config->current_buffer = 1 - config->current_buffer;
            break;
            
        case BUFFER_MODE_TRIPLE:
            // 三缓冲模式
            if (pan_display(config, config->current_buffer) == 0) {
                config->current_buffer = (config->current_buffer + 1) % 3;
            } else {
                copy_buffer_to_display(config, config->buffer[config->current_buffer]);
                config->current_buffer = (config->current_buffer + 1) % 3;
            }
            break;
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
        
        printf("FPS: %.2f (Mode: %d, VSync: %s, Enhanced: %s)\n", 
               fps, config->buffer_mode, 
               config->use_vsync ? "on" : "off",
               config->enhanced_sync ? "on" : "off");
        
        config->last_fps_time = current_time;
    }
}

int main(int argc, char *argv[]) {
    printf("Starting framebuffer layer rectangle collision demo with enhanced sync\n");
    
    // 设置默认参数
    layer_config.fb_id = 0;
    layer_config.point.x = 50;
    layer_config.point.y = 50;
    layer_config.img_width = 800;
    layer_config.img_height = 600;
    layer_config.pixel_format = E_PIXEL_FORMAT_ARGB8888;
    layer_config.fd = -1;
    layer_config.buffer_mode = BUFFER_MODE_DOUBLE_COPY;  // 默认使用内存拷贝模式
    layer_config.use_vsync = 1;
    layer_config.enhanced_sync = 1;
    layer_config.current_buffer = 0;
    
    // 解析命令行参数
    if (argc > 1) {
        if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
            print_usage(argv[0]);
            return 0;
        }
        layer_config.fb_id = atoi(argv[1]);
        if (layer_config.fb_id < 0 || layer_config.fb_id > 3) {
            printf("Error: fb_id must be 0-3\n");
            print_usage(argv[0]);
            return 1;
        }
    }
    
    if (argc > 2) layer_config.point.x = atoi(argv[2]);
    if (argc > 3) layer_config.point.y = atoi(argv[3]);
    if (argc > 4) layer_config.img_width = atoi(argv[4]);
    if (argc > 5) layer_config.img_height = atoi(argv[5]);
    if (argc > 6) {
        int buffer_mode = atoi(argv[6]);
        if (buffer_mode >= 0 && buffer_mode <= 3) {
            layer_config.buffer_mode = (buffer_mode_t)buffer_mode;
        }
    }
    if (argc > 7) layer_config.use_vsync = atoi(argv[7]);
    if (argc > 8) layer_config.enhanced_sync = atoi(argv[8]);
    
    // 设置对应的layer ID
    if (layer_config.fb_id == 0) {
        layer_config.vo_layerid = 2;
    } else if (layer_config.fb_id == 1) {
        layer_config.vo_layerid = 1;
    } else {
        layer_config.vo_layerid = 0;
    }
    
    printf("Configuration:\n");
    printf("  Framebuffer: /dev/fb%d\n", layer_config.fb_id);
    printf("  Layer ID: %d\n", layer_config.vo_layerid);
    printf("  Position: (%d, %d)\n", layer_config.point.x, layer_config.point.y);
    printf("  Size: %dx%d\n", layer_config.img_width, layer_config.img_height);
    printf("  Buffer Mode: %d\n", layer_config.buffer_mode);
    printf("  VSync: %s\n", layer_config.use_vsync ? "enabled" : "disabled");
    printf("  Enhanced Sync: %s\n", layer_config.enhanced_sync ? "enabled" : "disabled");
    
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    
    if (init_framebuffer_layer(&layer_config) != 0) {
        return 1;
    }

    initialize_rects(&layer_config);

    struct timespec start, end;
    long frame_duration = 1000000000 / FPS;
    
    printf("Starting animation loop on layer %d (Press Ctrl+C to exit)\n", layer_config.fb_id);
    printf("Try different buffer modes if you see refresh artifacts:\n");
    printf("  Mode 0: Single buffer (fastest, may tear)\n");
    printf("  Mode 1: Double buffer with pan display (hardware dependent)\n");
    printf("  Mode 2: Double buffer with memory copy (most compatible)\n");
    printf("  Mode 3: Triple buffer (smoothest, uses more memory)\n");
    
    while (!stop_flag) {
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
        {
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
    }

    printf("\nCleaning up layer %d...\n", layer_config.fb_id);
    cleanup_framebuffer_layer(&layer_config);
    
    printf("Demo completed\n");
    return 0;
}