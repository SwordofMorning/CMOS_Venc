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

struct Rectangle {
    int x, y;
    int velo_x, velo_y;
    uint32_t color;
};

struct Rectangle rects[NUM_RECTS];
static volatile sig_atomic_t stop_flag = 0;

// Framebuffer info
int fb_fd;
struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;
char *framebuffer;
long screensize;
int width, height, bytes_per_pixel;

void signal_handler(int sig) {
    stop_flag = 1;
}

int init_framebuffer() {
    // Open framebuffer device
    fb_fd = open("/dev/fb0", O_RDWR);
    if (fb_fd == -1) {
        perror("Error opening framebuffer device");
        return -1;
    }

    // Get variable screen information
    if (ioctl(fb_fd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
        perror("Error reading variable information");
        close(fb_fd);
        return -1;
    }

    printf("Display info: %dx%d, %d bpp\n", 
           vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);

    // Get fixed screen information
    if (ioctl(fb_fd, FBIOGET_FSCREENINFO, &finfo) == -1) {
        perror("Error reading fixed information");
        close(fb_fd);
        return -1;
    }

    // Calculate screen parameters
    width = vinfo.xres;
    height = vinfo.yres;
    bytes_per_pixel = vinfo.bits_per_pixel / 8;
    screensize = width * height * bytes_per_pixel;

    // Map framebuffer to memory
    framebuffer = (char*)mmap(0, screensize, PROT_READ | PROT_WRITE, 
                              MAP_SHARED, fb_fd, 0);
    
    if (framebuffer == MAP_FAILED) {
        perror("Error mapping framebuffer device to memory");
        close(fb_fd);
        return -1;
    }

    printf("Framebuffer mapped to memory\n");
    printf("Screen size: %dx%d\n", width, height);
    printf("Bytes per pixel: %d\n", bytes_per_pixel);
    
    return 0;
}

void cleanup_framebuffer() {
    if (framebuffer != MAP_FAILED) {
        munmap(framebuffer, screensize);
    }
    if (fb_fd != -1) {
        close(fb_fd);
    }
}

void initialize_rects() {
    uint32_t colors[NUM_RECTS] = {
        0xFFFF0000,  // Red
        0xFF00FF00,  // Green  
        0xFF0000FF,  // Blue
        0xFFFFFF00,  // Yellow
        0xFFFF00FF,  // Purple
        0xFF00FFFF,  // Cyan
        0xFF000000,  // Black
        0xFFFFFFFF   // White
    };

    srand(time(NULL));
    
    for (int i = 0; i < NUM_RECTS; i++) {
        rects[i].x = rand() % (width - RECT_WIDTH);
        rects[i].y = rand() % (height - RECT_HEIGHT);
        rects[i].velo_x = (rand() % 10) + 5;
        rects[i].velo_y = (rand() % 10) + 5;
        rects[i].color = colors[i];
        
        // Random direction
        if (rand() % 2) rects[i].velo_x = -rects[i].velo_x;
        if (rand() % 2) rects[i].velo_y = -rects[i].velo_y;
    }
}

void update_rect_positions() {
    for (int i = 0; i < NUM_RECTS; i++) {
        rects[i].x += rects[i].velo_x;
        rects[i].y += rects[i].velo_y;

        // Bounce off boundaries
        if (rects[i].x <= 0 || rects[i].x + RECT_WIDTH >= width) {
            rects[i].velo_x = -rects[i].velo_x;
            rects[i].x = (rects[i].x <= 0) ? 0 : width - RECT_WIDTH;
        }
        if (rects[i].y <= 0 || rects[i].y + RECT_HEIGHT >= height) {
            rects[i].velo_y = -rects[i].velo_y;
            rects[i].y = (rects[i].y <= 0) ? 0 : height - RECT_HEIGHT;
        }
    }
}

void put_pixel(int x, int y, uint32_t color) {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        return;
    }
    
    long location = (x + vinfo.xoffset) * (vinfo.bits_per_pixel / 8) +
                    (y + vinfo.yoffset) * finfo.line_length;
    
    if (vinfo.bits_per_pixel == 32) {
        *((uint32_t*)(framebuffer + location)) = color;
    } else if (vinfo.bits_per_pixel == 16) {
        // Convert ARGB8888 to RGB565
        uint16_t r = (color >> 16) & 0xFF;
        uint16_t g = (color >> 8) & 0xFF;
        uint16_t b = color & 0xFF;
        uint16_t color565 = ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
        *((uint16_t*)(framebuffer + location)) = color565;
    }
}

void clear_screen() {
    memset(framebuffer, 0, screensize);
}

void draw_rectangle(int x, int y, int w, int h, uint32_t color) {
    for (int py = y; py < y + h; py++) {
        for (int px = x; px < x + w; px++) {
            put_pixel(px, py, color);
        }
    }
}

void draw_rects() {
    clear_screen();
    
    for (int i = 0; i < NUM_RECTS; i++) {
        draw_rectangle(rects[i].x, rects[i].y, RECT_WIDTH, RECT_HEIGHT, rects[i].color);
    }
}

void draw_border() {
    uint32_t border_color = 0xFFFFFFFF; // White border
    
    // Top and bottom borders
    for (int x = 0; x < width; x++) {
        put_pixel(x, 0, border_color);
        put_pixel(x, height - 1, border_color);
    }
    
    // Left and right borders  
    for (int y = 0; y < height; y++) {
        put_pixel(0, y, border_color);
        put_pixel(width - 1, y, border_color);
    }
}

int main() {
    printf("Starting framebuffer rectangle collision demo\n");
    
    // Setup signal handler
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    
    // Initialize framebuffer
    if (init_framebuffer() != 0) {
        return 1;
    }

    // Initialize rectangles
    initialize_rects();

    // Main animation loop
    struct timespec start, end;
    long frame_duration = 1000000000 / FPS; // nanoseconds per frame
    
    printf("Starting animation loop (Press Ctrl+C to exit)\n");
    
    while (!stop_flag) {
        clock_gettime(CLOCK_MONOTONIC, &start);

        // Update positions
        update_rect_positions();
        
        // Draw frame
        draw_rects();
        draw_border();

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

    printf("\nCleaning up...\n");
    clear_screen();
    cleanup_framebuffer();
    
    printf("Demo completed\n");
    return 0;
}