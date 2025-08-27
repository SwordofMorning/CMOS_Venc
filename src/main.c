/**
 * @file main.c
 * @author XJT
 * @brief SPI Transfer
 * @version 0.3
 * @date 2025-06-19
 * 
 * @copyright Copyright (c) 2025
 * 
 */
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

#include "sample_common.h"
#include "vs_mal_regtools.h"

int main()
{
    vs_uint8_t* buf = NULL;
    vs_uint8_t* data = NULL;
    vs_uint8_t reg = 0x30;
    vs_int32_t len = 2;

    data = (vs_uint8_t*)malloc(len + 1);
    buf = (vs_uint8_t*)malloc(len + 1);
    
    if (!data || !buf) {
        printf("Memory allocation failed\n");
        if (data) free(data);
        if (buf) free(buf);
        return -1;
    }

    memset(buf, 0, len + 1);
    buf[0] = reg;

    if (vs_mal_spi_transfer(1, 0, E_SPI_MODE3, buf, buf, len + 1) != 0) {
        printf("SPI transfer failed\n");
        free(data);
        free(buf);
        return -1;
    }

    // 先使用buf中的数据
    memcpy(data, buf + 1, len);
    // 然后再释放buf
    free(buf);

    printf("data = [%02x][%02x]\n", data[0], data[1]);
    free(data);

    return 0;
}