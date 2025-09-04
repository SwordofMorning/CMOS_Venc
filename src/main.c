#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>

#define DEVICE_PATH "/dev/input/event0"

static int running = 1;

void signal_handler(int sig)
{
    if (sig == SIGINT || sig == SIGTERM) {
        printf("\nReceived signal %d, exiting...\n", sig);
        running = 0;
    }
}

const char* get_event_type_name(int type)
{
    switch (type) {
        case EV_SYN:
            return "EV_SYN";
        case EV_KEY:
            return "EV_KEY";
        case EV_REL:
            return "EV_REL";
        case EV_ABS:
            return "EV_ABS";
        case EV_MSC:
            return "EV_MSC";
        case EV_SW:
            return "EV_SW";
        case EV_LED:
            return "EV_LED";
        case EV_SND:
            return "EV_SND";
        case EV_REP:
            return "EV_REP";
        case EV_FF:
            return "EV_FF";
        case EV_PWR:
            return "EV_PWR";
        case EV_FF_STATUS:
            return "EV_FF_STATUS";
        default:
            return "UNKNOWN";
    }
}

const char* get_key_name(int code)
{
    switch (code) {
        case KEY_SPACE:
            return "KEY_SPACE (Single Click)";
        case KEY_ENTER:
            return "KEY_ENTER (Long Press)";
        case KEY_TAB:
            return "KEY_TAB (Double Click)";
        default:
            return "UNKNOWN_KEY";
    }
}

const char* get_rel_name(int code)
{
    switch (code) {
        case REL_WHEEL:
            return "REL_WHEEL (Rotation)";
        case REL_X:
            return "REL_X";
        case REL_Y:
            return "REL_Y";
        default:
            return "UNKNOWN_REL";
    }
}

void print_device_info(int fd)
{
    char device_name[256] = {0};
    char device_phys[256] = {0};
    char device_uniq[256] = {0};
    struct input_id device_id;
    
    printf("=== Device Information ===\n");
    
    if (ioctl(fd, EVIOCGNAME(sizeof(device_name)), device_name) >= 0) {
        printf("Device name: %s\n", device_name);
    }
    
    if (ioctl(fd, EVIOCGPHYS(sizeof(device_phys)), device_phys) >= 0) {
        printf("Device physical location: %s\n", device_phys);
    }
    
    if (ioctl(fd, EVIOCGUNIQ(sizeof(device_uniq)), device_uniq) >= 0) {
        printf("Device unique identifier: %s\n", device_uniq);
    }
    
    if (ioctl(fd, EVIOCGID, &device_id) >= 0) {
        printf("Device ID: bustype %d, vendor %04x, product %04x, version %04x\n",
               device_id.bustype, device_id.vendor, device_id.product, device_id.version);
    }
    
    printf("=============================\n\n");
}

void print_supported_events(int fd)
{
    unsigned char evtype_bitmask[EV_MAX/8 + 1];
    int i;
    
    printf("=== Supported Event Types ===\n");
    
    memset(evtype_bitmask, 0, sizeof(evtype_bitmask));
    if (ioctl(fd, EVIOCGBIT(0, EV_MAX), evtype_bitmask) >= 0) {
        for (i = 0; i < EV_MAX; i++) {
            if (evtype_bitmask[i/8] & (1 << (i%8))) {
                printf("  %s (%d)\n", get_event_type_name(i), i);
            }
        }
    }
    
    printf("==============================\n\n");
}

int main(int argc, char *argv[])
{
    int fd;
    struct input_event ev;
    ssize_t bytes;
    const char *device_path = DEVICE_PATH;
    
    /* 处理命令行参数 */
    if (argc > 1) {
        device_path = argv[1];
    }
    
    /* 设置信号处理 */
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    
    printf("RE08115MX Rotary Encoder Event Reader\n");
    printf("=====================================\n");
    printf("Device: %s\n", device_path);
    printf("Press Ctrl+C to exit\n\n");
    
    /* 打开设备 */
    fd = open(device_path, O_RDONLY);
    if (fd < 0) {
        perror("Failed to open device");
        fprintf(stderr, "Make sure the device exists and you have permission to read it\n");
        fprintf(stderr, "Try running as root: sudo %s\n", argv[0]);
        return 1;
    }
    
    /* 打印设备信息 */
    print_device_info(fd);
    print_supported_events(fd);
    
    printf("Waiting for events... (Ctrl+C to exit)\n");
    printf("Time\t\tType\t\tCode\t\tValue\t\tDescription\n");
    printf("----\t\t----\t\t----\t\t-----\t\t-----------\n");
    
    /* 主循环：读取和处理事件 */
    while (running) {
        bytes = read(fd, &ev, sizeof(ev));
        
        if (bytes < 0) {
            if (errno == EINTR) {
                continue; /* 被信号中断，继续循环 */
            }
            perror("Error reading from device");
            break;
        }
        
        if (bytes != sizeof(ev)) {
            fprintf(stderr, "Warning: incomplete event read (%zd bytes)\n", bytes);
            continue;
        }
        
        /* 格式化时间戳 */
        printf("%ld.%06ld\t", ev.time.tv_sec, ev.time.tv_usec);
        
        /* 解析事件类型 */
        printf("%s\t", get_event_type_name(ev.type));
        
        /* 打印事件代码和值 */
        printf("%d\t\t%d\t\t", ev.code, ev.value);
        
        /* 解析具体事件含义 */
        switch (ev.type) {
            case EV_SYN:
                if (ev.code == SYN_REPORT) {
                    printf("Event separator");
                }
                break;
                
            case EV_KEY:
                printf("%s ", get_key_name(ev.code));
                if (ev.value == 1) {
                    printf("(PRESSED)");
                } else if (ev.value == 0) {
                    printf("(RELEASED)");
                } else if (ev.value == 2) {
                    printf("(REPEAT)");
                }
                
                /* 特殊处理编码器按键事件 */
                if (ev.value == 1) {
                    switch (ev.code) {
                        case KEY_SPACE:
                            printf(" -> Single Click Detected!");
                            break;
                        case KEY_ENTER:
                            printf(" -> Long Press Detected!");
                            break;
                        case KEY_TAB:
                            printf(" -> Double Click Detected!");
                            break;
                    }
                }
                break;
                
            case EV_REL:
                printf("%s ", get_rel_name(ev.code));
                if (ev.code == REL_WHEEL) {
                    if (ev.value > 0) {
                        printf("-> Clockwise Rotation (%d steps)", ev.value);
                    } else if (ev.value < 0) {
                        printf("-> Counter-clockwise Rotation (%d steps)", -ev.value);
                    }
                }
                break;
                
            default:
                printf("Raw event");
                break;
        }
        
        printf("\n");
        fflush(stdout);
    }
    
    close(fd);
    printf("\nEvent reader terminated.\n");
    return 0;
}