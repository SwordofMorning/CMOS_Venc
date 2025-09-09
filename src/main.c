// xOTA.c - A/B System OTA Upgrade Tool
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <getopt.h>
#include <stdint.h>

#define MISC_DEVICE "/dev/mmcblk0p4"
#define DTB_A_DEVICE "/dev/mmcblk0p5"
#define DTB_B_DEVICE "/dev/mmcblk0p6"
#define KERNEL_A_DEVICE "/dev/mmcblk0p7"
#define KERNEL_B_DEVICE "/dev/mmcblk0p8"
#define ROOTFS_A_DEVICE "/dev/mmcblk0p9"
#define ROOTFS_B_DEVICE "/dev/mmcblk0p10"

#define PROC_CMDLINE "/proc/cmdline"
#define BUFFER_SIZE 4096

#define DEFAULT_RETRY_COUNT 1

// 更新的Misc partition structure，与ab_boot保持一致
typedef struct {
    char magic[16];           // "ANDROID_BOOT" + padding
    char slot_suffix[32];     // Current active slot ("_a" or "_b")
    
    // Slot A metadata
    uint8_t bootable_a;       // 1 = bootable, 0 = not bootable
    uint8_t successful_a;     // 1 = successful, 0 = not successful
    uint8_t active_a;         // 1 = active, 0 = not active
    uint8_t retry_count_a;    // Boot retry count
    
    // Slot B metadata  
    uint8_t bootable_b;       // 1 = bootable, 0 = not bootable
    uint8_t successful_b;     // 1 = successful, 0 = not successful
    uint8_t active_b;         // 1 = active, 0 = not active
    uint8_t retry_count_b;    // Boot retry count
    
    // 新增字段
    uint8_t boot_attempts_a;  // Slot A boot attempts counter
    uint8_t boot_attempts_b;  // Slot B boot attempts counter
    uint8_t last_boot_slot;   // Last boot slot ('a' or 'b')
    
    char reserved[973];       // 调整reserved大小 (976 - 3 = 973)
} misc_info_t;

// Function prototypes
int read_misc_partition(misc_info_t *misc);
int write_misc_partition(const misc_info_t *misc);
char get_current_slot(void);
char get_inactive_slot(char current_slot);
int flash_partition(const char *device, const char *file);
int copy_file_to_device(const char *src_file, const char *dst_device);
void print_usage(const char *program_name);
void print_slot_info(const misc_info_t *misc);
void init_misc_partition(misc_info_t *misc);

// 验证rootfs文件格式的函数
int validate_rootfs_file(const char *filename)
{
    if (!filename) {
        printf("Error: Rootfs filename is NULL\n");
        return -1;
    }
    
    size_t len = strlen(filename);
    if (len < 4) {
        printf("Error: Rootfs filename too short\n");
        return -1;
    }
    
    // 检查文件扩展名
    const char *ext = filename + len - 4;
    if (strcmp(ext, ".img") == 0) {
        printf("Info: Rootfs file format validated (.img)\n");
        return 0;
    } else if (strcmp(ext, "simg") == 0 || strstr(filename, ".simg")) {
        printf("Error: .simg files are not supported. Please use .img files only.\n");
        printf("Hint: Convert .simg to .img using: simg2img %s %s\n", 
               filename, filename);
        return -1;
    } else {
        printf("Error: Unsupported rootfs file format. Only .img files are supported.\n");
        printf("Current file: %s\n", filename);
        return -1;
    }
}

int main(int argc, char *argv[])
{
    int opt;
    int show_slot = 0;
    int upgrade = 0;
    int set_label = 0;
    char *dtb_file = NULL;
    char *kernel_file = NULL;
    char *rootfs_file = NULL;
    char target_slot = 0;
    char *label_name = NULL;
    int label_value = 0;
    
    static struct option long_options[] = {
        {"slot", no_argument, 0, 's'},
        {"upgrade", no_argument, 0, 'u'},
        {"set-label", no_argument, 0, 'l'},
        {"dtb", required_argument, 0, 'd'},
        {"kernel", required_argument, 0, 'k'},
        {"rootfs", required_argument, 0, 'f'},
        {"target-slot", required_argument, 0, 't'},
        {"label", required_argument, 0, 'n'},
        {"value", required_argument, 0, 'v'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };
    
    while ((opt = getopt_long(argc, argv, "suld:k:f:t:n:v:h", long_options, NULL)) != -1) {
        switch (opt) {
            case 's':
                show_slot = 1;
                break;
            case 'u':
                upgrade = 1;
                break;
            case 'l':
                set_label = 1;
                break;
            case 'd':
                dtb_file = optarg;
                break;
            case 'k':
                kernel_file = optarg;
                break;
            case 'f':
                rootfs_file = optarg;
                if (validate_rootfs_file(rootfs_file) != 0) {
                    return 1;
                }
                break;
            case 't':
                target_slot = optarg[0];
                break;
            case 'n':
                label_name = optarg;
                break;
            case 'v':
                label_value = atoi(optarg);
                break;
            case 'h':
            default:
                print_usage(argv[0]);
                return 1;
        }
    }
    
    // Show current slot
    if (show_slot) {
        char current_slot = get_current_slot();
        if (current_slot) {
            printf("Current slot: %c\n", current_slot);
            
            misc_info_t misc;
            if (read_misc_partition(&misc) == 0) {
                print_slot_info(&misc);
            }
        } else {
            printf("Failed to determine current slot\n");
            return 1;
        }
        return 0;
    }
    
    // Upgrade system
    if (upgrade) {
        if (!dtb_file || !kernel_file || !rootfs_file) {
            printf("Error: All three files (dtb, kernel, rootfs) are required for upgrade\n");
            print_usage(argv[0]);
            return 1;
        }
        
        if (validate_rootfs_file(rootfs_file) != 0) {
            return 1;
        }
        
        char current_slot = get_current_slot();
        if (!current_slot) {
            printf("Failed to determine current slot\n");
            return 1;
        }
        
        char inactive_slot = get_inactive_slot(current_slot);
        printf("Current slot: %c, upgrading slot: %c\n", current_slot, inactive_slot);
        
        // Flash inactive slot
        const char *dtb_device = (inactive_slot == 'A') ? DTB_A_DEVICE : DTB_B_DEVICE;
        const char *kernel_device = (inactive_slot == 'A') ? KERNEL_A_DEVICE : KERNEL_B_DEVICE;
        const char *rootfs_device = (inactive_slot == 'A') ? ROOTFS_A_DEVICE : ROOTFS_B_DEVICE;
        
        printf("Flashing DTB to %s...\n", dtb_device);
        if (flash_partition(dtb_device, dtb_file) != 0) {
            printf("Failed to flash DTB\n");
            return 1;
        }
        
        printf("Flashing kernel to %s...\n", kernel_device);
        if (flash_partition(kernel_device, kernel_file) != 0) {
            printf("Failed to flash kernel\n");
            return 1;
        }
        
        printf("Flashing rootfs to %s...\n", rootfs_device);
        if (flash_partition(rootfs_device, rootfs_file) != 0) {
            printf("Failed to flash rootfs\n");
            return 1;
        }
        
        // Update misc partition to switch to new slot
        misc_info_t misc;
        if (read_misc_partition(&misc) != 0) {
            printf("Failed to read misc partition\n");
            return 1;
        }
        
        // Set inactive slot as active and bootable
        if (inactive_slot == 'A') {
            misc.bootable_a = 1;
            misc.successful_a = 0;  // Will be set by s99_ota.sh after successful boot
            misc.active_a = 1;
            misc.retry_count_a = DEFAULT_RETRY_COUNT;
            misc.boot_attempts_a = 0;  // 新增：重置启动尝试计数
            
            misc.active_b = 0;
            strcpy(misc.slot_suffix, "_a");
        } else {
            misc.bootable_b = 1;
            misc.successful_b = 0;  // Will be set by s99_ota.sh after successful boot
            misc.active_b = 1;
            misc.retry_count_b = DEFAULT_RETRY_COUNT;
            misc.boot_attempts_b = 0;  // 新增：重置启动尝试计数
            
            misc.active_a = 0;
            strcpy(misc.slot_suffix, "_b");
        }
        
        if (write_misc_partition(&misc) != 0) {
            printf("Failed to write misc partition\n");
            return 1;
        }
        
        printf("Upgrade completed successfully!\n");
        printf("Please reboot to switch to slot %c\n", inactive_slot);
        printf("Note: Fast failover enabled (retry count = %d)\n", DEFAULT_RETRY_COUNT);
        return 0;
    }
    
    // Set label
    if (set_label) {
        if (!target_slot || !label_name) {
            printf("Error: Target slot and label name are required\n");
            print_usage(argv[0]);
            return 1;
        }
        
        misc_info_t misc;
        if (read_misc_partition(&misc) != 0) {
            printf("Failed to read misc partition\n");
            return 1;
        }
        
        if (target_slot == 'A' || target_slot == 'a') {
            if (strcmp(label_name, "bootable") == 0) {
                misc.bootable_a = label_value;
            } else if (strcmp(label_name, "successful") == 0) {
                misc.successful_a = label_value;
            } else if (strcmp(label_name, "active") == 0) {
                misc.active_a = label_value;
                if (label_value) {
                    misc.active_b = 0;
                    strcpy(misc.slot_suffix, "_a");
                }
            } else if (strcmp(label_name, "retry_count") == 0) {
                misc.retry_count_a = (uint8_t)label_value;
            } else if (strcmp(label_name, "boot_attempts") == 0) {
                // 新增：支持设置boot_attempts
                misc.boot_attempts_a = (uint8_t)label_value;
            } else if (strcmp(label_name, "last_boot_slot") == 0) {
                // 新增：支持设置last_boot_slot
                if (label_value == 'a' || label_value == 'A') {
                    misc.last_boot_slot = 'a';
                } else if (label_value == 'b' || label_value == 'B') {
                    misc.last_boot_slot = 'b';
                } else {
                    printf("Invalid last_boot_slot value: %d (should be 'a' or 'b')\n", label_value);
                    return 1;
                }
            } else {
                printf("Unknown label: %s\n", label_name);
                printf("Available labels: bootable, successful, active, retry_count, boot_attempts, last_boot_slot\n");
                return 1;
            }
        } else if (target_slot == 'B' || target_slot == 'b') {
            if (strcmp(label_name, "bootable") == 0) {
                misc.bootable_b = label_value;
            } else if (strcmp(label_name, "successful") == 0) {
                misc.successful_b = label_value;
            } else if (strcmp(label_name, "active") == 0) {
                misc.active_b = label_value;
                if (label_value) {
                    misc.active_a = 0;
                    strcpy(misc.slot_suffix, "_b");
                }
            } else if (strcmp(label_name, "retry_count") == 0) {
                misc.retry_count_b = (uint8_t)label_value;
            } else if (strcmp(label_name, "boot_attempts") == 0) {
                // 新增：支持设置boot_attempts
                misc.boot_attempts_b = (uint8_t)label_value;
            } else if (strcmp(label_name, "last_boot_slot") == 0) {
                // 新增：支持设置last_boot_slot
                if (label_value == 'a' || label_value == 'A') {
                    misc.last_boot_slot = 'a';
                } else if (label_value == 'b' || label_value == 'B') {
                    misc.last_boot_slot = 'b';
                } else {
                    printf("Invalid last_boot_slot value: %d (should be 'a' or 'b')\n", label_value);
                    return 1;
                }
            } else {
                printf("Unknown label: %s\n", label_name);
                printf("Available labels: bootable, successful, active, retry_count, boot_attempts, last_boot_slot\n");
                return 1;
            }
        } else {
            printf("Invalid slot: %c (must be A or B)\n", target_slot);
            return 1;
        }
        
        if (write_misc_partition(&misc) != 0) {
            printf("Failed to write misc partition\n");
            return 1;
        }
        
        printf("Label %s set to %d for slot %c\n", label_name, label_value, target_slot);
        return 0;
    }
    
    print_usage(argv[0]);
    return 1;
}

int read_misc_partition(misc_info_t *misc)
{
    int fd = open(MISC_DEVICE, O_RDONLY);
    if (fd < 0) {
        perror("Failed to open misc partition");
        return -1;
    }
    
    ssize_t bytes_read = read(fd, misc, sizeof(misc_info_t));
    close(fd);
    
    if (bytes_read != sizeof(misc_info_t)) {
        printf("Failed to read misc partition completely\n");
        return -1;
    }
    
    // Check magic and initialize if needed
    if (strncmp(misc->magic, "ANDROID_BOOT", 12) != 0) {
        printf("Misc partition not initialized, initializing...\n");
        init_misc_partition(misc);
        return write_misc_partition(misc);
    }
    
    // 新增：兼容性检查和初始化新字段
    if (misc->boot_attempts_a > 10) misc->boot_attempts_a = 0;
    if (misc->boot_attempts_b > 10) misc->boot_attempts_b = 0;
    if (misc->last_boot_slot != 'a' && misc->last_boot_slot != 'b') {
        misc->last_boot_slot = 'a';
    }
    
    return 0;
}

int write_misc_partition(const misc_info_t *misc)
{
    int fd = open(MISC_DEVICE, O_WRONLY);
    if (fd < 0) {
        perror("Failed to open misc partition for writing");
        return -1;
    }
    
    ssize_t bytes_written = write(fd, misc, sizeof(misc_info_t));
    close(fd);
    
    if (bytes_written != sizeof(misc_info_t)) {
        printf("Failed to write misc partition completely\n");
        return -1;
    }
    
    sync();  // Ensure data is written to disk
    return 0;
}

char get_current_slot(void)
{
    FILE *fp = fopen(PROC_CMDLINE, "r");
    if (!fp) {
        perror("Failed to open /proc/cmdline");
        return 0;
    }
    
    char line[1024];
    if (fgets(line, sizeof(line), fp) == NULL) {
        fclose(fp);
        return 0;
    }
    fclose(fp);
    
    // Look for root=/dev/mmcblk0p9 (slot A) or root=/dev/mmcblk0p10 (slot B)
    if (strstr(line, "root=/dev/mmcblk0p9")) {
        return 'A';
    } else if (strstr(line, "root=/dev/mmcblk0p10")) {
        return 'B';
    }
    
    return 0;
}

char get_inactive_slot(char current_slot)
{
    return (current_slot == 'A') ? 'B' : 'A';
}

int flash_partition(const char *device, const char *file)
{
    return copy_file_to_device(file, device);
}

int copy_file_to_device(const char *src_file, const char *dst_device)
{
    int src_fd, dst_fd;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read, bytes_written;
    
    src_fd = open(src_file, O_RDONLY);
    if (src_fd < 0) {
        printf("Failed to open source file %s: %s\n", src_file, strerror(errno));
        return -1;
    }
    
    dst_fd = open(dst_device, O_WRONLY);
    if (dst_fd < 0) {
        printf("Failed to open destination device %s: %s\n", dst_device, strerror(errno));
        close(src_fd);
        return -1;
    }
    
    while ((bytes_read = read(src_fd, buffer, BUFFER_SIZE)) > 0) {
        bytes_written = write(dst_fd, buffer, bytes_read);
        if (bytes_written != bytes_read) {
            printf("Write error: wrote %zd bytes, expected %zd\n", bytes_written, bytes_read);
            close(src_fd);
            close(dst_fd);
            return -1;
        }
    }
    
    close(src_fd);
    close(dst_fd);
    sync();
    
    if (bytes_read < 0) {
        printf("Read error: %s\n", strerror(errno));
        return -1;
    }
    
    return 0;
}

// 更新：初始化函数，添加新字段的初始化
void init_misc_partition(misc_info_t *misc)
{
    memset(misc, 0, sizeof(misc_info_t));
    strcpy(misc->magic, "ANDROID_BOOT");
    strcpy(misc->slot_suffix, "_a");
    
    // Initialize slot A as active by default
    misc->bootable_a = 1;
    misc->successful_a = 1;
    misc->active_a = 1;
    misc->retry_count_a = DEFAULT_RETRY_COUNT;
    misc->boot_attempts_a = 0;  // 新增：初始化启动尝试计数
    
    // Initialize slot B as inactive
    misc->bootable_b = 1;
    misc->successful_b = 0;
    misc->active_b = 0;
    misc->retry_count_b = DEFAULT_RETRY_COUNT;
    misc->boot_attempts_b = 0;  // 新增：初始化启动尝试计数
    
    misc->last_boot_slot = 'a';  // 新增：初始化上次启动slot
}

// 更新：打印函数，显示新字段
void print_slot_info(const misc_info_t *misc)
{
    printf("Misc partition info:\n");
    printf("  Magic: %.16s\n", misc->magic);
    printf("  Active slot: %s\n", misc->slot_suffix);
    printf("  Last boot slot: %c\n", misc->last_boot_slot);
    printf("\n");
    printf("Slot A:\n");
    printf("  Bootable: %d\n", misc->bootable_a);
    printf("  Successful: %d\n", misc->successful_a);
    printf("  Active: %d\n", misc->active_a);
    printf("  Retry count: %d\n", misc->retry_count_a);
    printf("  Boot attempts: %d\n", misc->boot_attempts_a);  // 新增
    printf("\n");
    printf("Slot B:\n");
    printf("  Bootable: %d\n", misc->bootable_b);
    printf("  Successful: %d\n", misc->successful_b);
    printf("  Active: %d\n", misc->active_b);
    printf("  Retry count: %d\n", misc->retry_count_b);
    printf("  Boot attempts: %d\n", misc->boot_attempts_b);  // 新增
}

// 更新：帮助信息，添加新的label类型
void print_usage(const char *program_name)
{
    printf("Usage: %s [OPTIONS]\n", program_name);
    printf("\n");
    printf("Options:\n");
    printf("  --slot, -s                    Show current slot information\n");
    printf("  --upgrade, -u                 Upgrade system to inactive slot\n");
    printf("    --dtb, -d <file>            DTB file for upgrade\n");
    printf("    --kernel, -k <file>         Kernel file for upgrade\n");
    printf("    --rootfs, -f <file>         Rootfs file for upgrade (.img format only)\n");
    printf("  --set-label, -l               Set slot label\n");
    printf("    --target-slot, -t <A|B>     Target slot (A or B)\n");
    printf("    --label, -n <name>          Label name (see below)\n");
    printf("    --value, -v <value>         Label value\n");
    printf("  --help, -h                    Show this help message\n");
    printf("\n");
    printf("Available labels:\n");
    printf("  bootable                      Bootable flag (0 or 1)\n");
    printf("  successful                    Successful flag (0 or 1)\n");
    printf("  active                        Active flag (0 or 1)\n");
    printf("  retry_count                   Retry count (0-255)\n");
    printf("  boot_attempts                 Boot attempts counter (0-255)\n");
    printf("  last_boot_slot               Last boot slot ('a' or 'b' as ASCII value)\n");
    printf("\n");
    printf("Notes:\n");
    printf("  - Fast failover is enabled (default retry count = %d)\n", DEFAULT_RETRY_COUNT);
    printf("  - Only .img files are supported for rootfs. Convert .simg using simg2img.\n");
    printf("  - Boot attempts counter is used for automatic failover detection.\n");
    printf("\n");
    printf("Examples:\n");
    printf("  %s --slot\n", program_name);
    printf("  %s --upgrade -d dtb.dtb -k Image -f rootfs.img\n", program_name);
    printf("  %s --set-label -t A -n active -v 1\n", program_name);
    printf("  %s --set-label -t B -n boot_attempts -v 0\n", program_name);
    printf("  %s --set-label -t A -n last_boot_slot -v 97  # 'a' as ASCII\n", program_name);
}