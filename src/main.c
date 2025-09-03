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

// 厂商特定的framebuffer扩展定义
#define FBIOPUT_SHOW_VS_FB          _IOW('F', 0x101, int)
#define FBIOGET_SHOW_VS_FB          _IOR('F', 0x102, int)
#define FBIOPUT_SCREEN_ORIGIN_VS_FB _IOW('F', 0x103, struct fb_point)
#define FBIOGET_SCREEN_ORIGIN_VS_FB _IOR('F', 0x104, struct fb_point)
#define FBIOPUT_LAYER_INFO          _IOW('F', 0x105, struct fb_layer_info)
#define FBIOGET_LAYER_INFO          _IOR('F', 0x106, struct fb_layer_info)

// 厂商特定的结构体定义
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

#define FB_LAYER_MASK_BUFMODE    0x01
#define FB_LAYER_MASK_CANVASSIZE 0x02

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
} fb_layer_config;

struct Rectangle rects[NUM_RECTS];
static volatile sig_atomic_t stop_flag = 0;
static fb_layer_config layer_config = {0};

void signal_handler(int sig) {
    stop_flag = 1;
}

void print_usage(const char* prog_name) {
    printf("Usage: %s [fb_id] [x_pos] [y_pos] [width] [height]\n", prog_name);
    printf("  fb_id:  framebuffer device ID (0-3), default: 0\n");
    printf("  x_pos:  layer X position on screen, default: 50\n");
    printf("  y_pos:  layer Y position on screen, default: 50\n");
    printf("  width:  layer width, default: 800\n");
    printf("  height: layer height, default: 600\n");
    printf("\nExample:\n");
    printf("  %s 0 100 100 800 600  # Use fb0 at position (100,100) with size 800x600\n", prog_name);
    printf("  %s 1 200 200 640 480  # Use fb1 at position (200,200) with size 640x480\n", prog_name);
}

int set_layer_show(fb_layer_config *config, int show) {
    if (ioctl(config->fd, FBIOPUT_SHOW_VS_FB, &show) != 0) {
        printf("Warning: Failed to set layer show state (using standard fb)\n");
        // 如果厂商特定接口失败，继续使用标准接口
        return 0;
    }
    printf("Layer %d show state set to: %s\n", config->fb_id, show ? "visible" : "hidden");
    return 0;
}

int set_layer_position(fb_layer_config *config) {
    if (ioctl(config->fd, FBIOPUT_SCREEN_ORIGIN_VS_FB, &config->point) != 0) {
        printf("Warning: Failed to set layer position (using standard fb)\n");
        // 如果厂商特定接口失败，继续使用标准接口
        return 0;
    }
    printf("Layer %d position set to: (%d, %d)\n", 
           config->fb_id, config->point.x, config->point.y);
    return 0;
}

int init_framebuffer_layer(fb_layer_config *config) {
    char dev_name[16];
    
    // 构造设备文件名
    snprintf(dev_name, sizeof(dev_name), "/dev/fb%d", config->fb_id);
    
    // 打开framebuffer设备
    config->fd = open(dev_name, O_RDWR);
    if (config->fd == -1) {
        perror("Error opening framebuffer device");
        return -1;
    }
    
    printf("Opened framebuffer device: %s\n", dev_name);

    // 先设置layer为不可见状态
    set_layer_show(config, 0);

    // 获取可变屏幕信息
    if (ioctl(config->fd, FBIOGET_VSCREENINFO, &config->vinfo) == -1) {
        perror("Error reading variable information");
        close(config->fd);
        return -1;
    }

    // 设置framebuffer参数
    config->vinfo.xres = config->img_width;
    config->vinfo.yres = config->img_height;
    config->vinfo.xres_virtual = config->img_width;
    config->vinfo.yres_virtual = config->img_height;
    
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

    // 应用可变屏幕信息设置
    if (ioctl(config->fd, FBIOPUT_VSCREENINFO, &config->vinfo) == -1) {
        perror("Error setting variable information");
        close(config->fd);
        return -1;
    }

    // 获取固定屏幕信息
    if (ioctl(config->fd, FBIOGET_FSCREENINFO, &config->finfo) == -1) {
        perror("Error reading fixed information");
        close(config->fd);
        return -1;
    }

    printf("Layer info: %dx%d, %d bpp\n", 
           config->vinfo.xres, config->vinfo.yres, config->vinfo.bits_per_pixel);

    // 计算屏幕大小
    config->screensize = config->finfo.smem_len;

    // 映射framebuffer到内存
    config->framebuffer = (char*)mmap(0, config->screensize, PROT_READ | PROT_WRITE, 
                                     MAP_SHARED, config->fd, 0);
    
    if (config->framebuffer == MAP_FAILED) {
        perror("Error mapping framebuffer device to memory");
        close(config->fd);
        return -1;
    }

    printf("Framebuffer mapped to memory, size: %ld bytes\n", config->screensize);

    // 设置layer位置
    set_layer_position(config);
    
    // 清空framebuffer
    memset(config->framebuffer, 0, config->screensize);
    
    // 设置layer为可见状态
    set_layer_show(config, 1);
    
    return 0;
}

void cleanup_framebuffer_layer(fb_layer_config *config) {
    if (config->framebuffer && config->framebuffer != MAP_FAILED) {
        // 清空framebuffer
        memset(config->framebuffer, 0, config->screensize);
        munmap(config->framebuffer, config->screensize);
        config->framebuffer = NULL;
    }
    
    if (config->fd != -1) {
        // 设置layer为不可见状态
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
        
        // Random direction
        if (rand() % 2) rects[i].velo_x = -rects[i].velo_x;
        if (rand() % 2) rects[i].velo_y = -rects[i].velo_y;
    }
}

void update_rect_positions(fb_layer_config *config) {
    for (int i = 0; i < NUM_RECTS; i++) {
        rects[i].x += rects[i].velo_x;
        rects[i].y += rects[i].velo_y;

        // Bounce off boundaries
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

void put_pixel(fb_layer_config *config, int x, int y, uint32_t color) {
    if (x < 0 || x >= config->img_width || y < 0 || y >= config->img_height) {
        return;
    }
    
    long location = (x + config->vinfo.xoffset) * (config->vinfo.bits_per_pixel / 8) +
                    (y + config->vinfo.yoffset) * config->finfo.line_length;
    
    if (config->vinfo.bits_per_pixel == 32) {
        *((uint32_t*)(config->framebuffer + location)) = color;
    } else if (config->vinfo.bits_per_pixel == 16) {
        // Convert ARGB8888 to RGB565
        uint16_t r = (color >> 16) & 0xFF;
        uint16_t g = (color >> 8) & 0xFF;
        uint16_t b = color & 0xFF;
        uint16_t color565 = ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
        *((uint16_t*)(config->framebuffer + location)) = color565;
    }
}

void clear_screen(fb_layer_config *config) {
    memset(config->framebuffer, 0, config->screensize);
}

void draw_rectangle(fb_layer_config *config, int x, int y, int w, int h, uint32_t color) {
    for (int py = y; py < y + h; py++) {
        for (int px = x; px < x + w; px++) {
            put_pixel(config, px, py, color);
        }
    }
}

void draw_rects(fb_layer_config *config) {
    clear_screen(config);
    
    for (int i = 0; i < NUM_RECTS; i++) {
        draw_rectangle(config, rects[i].x, rects[i].y, RECT_WIDTH, RECT_HEIGHT, rects[i].color);
    }
}

void draw_border(fb_layer_config *config) {
    uint32_t border_color = 0xFFFFFFFF; // White border
    
    // Top and bottom borders
    for (int x = 0; x < config->img_width; x++) {
        put_pixel(config, x, 0, border_color);
        put_pixel(config, x, config->img_height - 1, border_color);
    }
    
    // Left and right borders  
    for (int y = 0; y < config->img_height; y++) {
        put_pixel(config, 0, y, border_color);
        put_pixel(config, config->img_width - 1, y, border_color);
    }
}

int main(int argc, char *argv[]) {
    printf("Starting framebuffer layer rectangle collision demo\n");
    
    // 设置默认参数
    layer_config.fb_id = 0;           // 默认使用fb0
    layer_config.point.x = 50;        // 默认X位置
    layer_config.point.y = 50;        // 默认Y位置
    layer_config.img_width = 800;     // 默认宽度
    layer_config.img_height = 600;    // 默认高度
    layer_config.pixel_format = E_PIXEL_FORMAT_ARGB8888;
    layer_config.fd = -1;
    
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
    
    // 设置对应的layer ID
    if (layer_config.fb_id == 0) {
        layer_config.vo_layerid = 2;  // fb0 -> layer 2
    } else if (layer_config.fb_id == 1) {
        layer_config.vo_layerid = 1;  // fb1 -> layer 1
    } else {
        layer_config.vo_layerid = 0;  // fb2/fb3 -> cursor layer
    }
    
    printf("Configuration:\n");
    printf("  Framebuffer: /dev/fb%d\n", layer_config.fb_id);
    printf("  Layer ID: %d\n", layer_config.vo_layerid);
    printf("  Position: (%d, %d)\n", layer_config.point.x, layer_config.point.y);
    printf("  Size: %dx%d\n", layer_config.img_width, layer_config.img_height);
    
    // Setup signal handler
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    
    // Initialize framebuffer layer
    if (init_framebuffer_layer(&layer_config) != 0) {
        return 1;
    }

    // Initialize rectangles
    initialize_rects(&layer_config);

    // Main animation loop
    struct timespec start, end;
    long frame_duration = 1000000000 / FPS; // nanoseconds per frame
    
    printf("Starting animation loop on layer %d (Press Ctrl+C to exit)\n", layer_config.fb_id);
    
    while (!stop_flag) {
        clock_gettime(CLOCK_MONOTONIC, &start);

        // Update positions
        update_rect_positions(&layer_config);
        
        // Draw frame
        draw_rects(&layer_config);
        draw_border(&layer_config);

        // Frame rate control
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

    printf("\nCleaning up layer %d...\n", layer_config.fb_id);
    cleanup_framebuffer_layer(&layer_config);
    
    printf("Demo completed\n");
    return 0;
}