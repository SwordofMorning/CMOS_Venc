/**
 * @file main.c
 * @author XJT
 * @brief SPI Transfer Tool - Similar to i2ctransfer
 * @version 0.5
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

void print_usage(const char* prog_name) {
    printf("Usage:\n");
    printf("  Read:  %s [spi_dev] [csn] [spi_mode] [reg_addr] -r [bytes]\n", prog_name);
    printf("  Write: %s [spi_dev] [csn] [spi_mode] [reg_addr] -w [data_bytes] [data1] [data2] ...\n", prog_name);
    printf("\nExamples:\n");
    printf("  %s 1 0 3 0x30 -r 2              # Read 2 bytes from 8-bit register 0x30\n", prog_name);
    printf("  %s 1 0 3 0x3040 -r 2            # Read 2 bytes from 16-bit register 0x3040\n", prog_name);
    printf("  %s 1 0 3 0x30 -w 2 0x11 0x22    # Write 2 bytes (0x11,0x22) to register 0x30\n", prog_name);
    printf("  %s 1 0 3 0x3040 -w 2 0x11 0x22  # Write 2 bytes (0x11,0x22) to 16-bit register 0x3040\n", prog_name);
    printf("\nSPI Modes:\n");
    printf("  0: CPOL:0, CPHA:0\n");
    printf("  1: CPOL:0, CPHA:1\n");
    printf("  2: CPOL:1, CPHA:0\n");
    printf("  3: CPOL:1, CPHA:1\n");
    printf("\nRegister Address:\n");
    printf("  - 8-bit:  0x00 to 0xFF\n");
    printf("  - 16-bit: 0x0100 to 0xFFFF\n");
}

int parse_hex_or_dec(const char* str) {
    char* endptr;
    int value;
    
    if (strncmp(str, "0x", 2) == 0 || strncmp(str, "0X", 2) == 0) {
        value = strtol(str, &endptr, 16);
    } else {
        value = strtol(str, &endptr, 10);
    }
    
    if (*endptr != '\0') {
        return -1; // Invalid number
    }
    
    return value;
}

int get_reg_addr_size(int reg_addr) {
    if (reg_addr >= 0 && reg_addr <= 0xFF) {
        return 1; // 8-bit register
    } else if (reg_addr >= 0x100 && reg_addr <= 0xFFFF) {
        return 2; // 16-bit register
    } else {
        return -1; // Invalid register address
    }
}

int main(int argc, char* argv[]) {
    if (argc < 6) {
        print_usage(argv[0]);
        return -1;
    }

    // Parse basic parameters
    int spi_dev = parse_hex_or_dec(argv[1]);
    int csn = parse_hex_or_dec(argv[2]);
    int spi_mode = parse_hex_or_dec(argv[3]);
    int reg_addr = parse_hex_or_dec(argv[4]);
    
    if (spi_dev < 0 || csn < 0 || spi_mode < 0 || spi_mode >= E_SPI_MODE_MAX) {
        printf("Error: Invalid spi_dev, csn, or spi_mode\n");
        print_usage(argv[0]);
        return -1;
    }

    // Check register address and determine its size
    int reg_addr_size = get_reg_addr_size(reg_addr);
    if (reg_addr_size < 0) {
        printf("Error: Invalid register address 0x%x (must be 0x00-0xFF or 0x0100-0xFFFF)\n", reg_addr);
        return -1;
    }

    // Check operation mode
    if (strcmp(argv[5], "-r") == 0) {
        // Read mode
        if (argc != 7) {
            printf("Error: Read mode requires exactly 7 arguments\n");
            print_usage(argv[0]);
            return -1;
        }
        
        int read_bytes = parse_hex_or_dec(argv[6]);
        
        if (read_bytes <= 0 || read_bytes > 255) {
            printf("Error: Invalid read_bytes (must be 1-255)\n");
            return -1;
        }

        // Allocate buffers
        int total_transfer_bytes = reg_addr_size + read_bytes;
        vs_uint8_t* buf = (vs_uint8_t*)malloc(total_transfer_bytes);
        vs_uint8_t* data = (vs_uint8_t*)malloc(read_bytes);
        
        if (!buf || !data) {
            printf("Error: Memory allocation failed\n");
            if (buf) free(buf);
            if (data) free(data);
            return -1;
        }

        // Prepare tx buffer with register address
        memset(buf, 0, total_transfer_bytes);
        if (reg_addr_size == 1) {
            buf[0] = (vs_uint8_t)reg_addr;
        } else { // reg_addr_size == 2
            buf[0] = (vs_uint8_t)((reg_addr >> 8) & 0xFF); // High byte
            buf[1] = (vs_uint8_t)(reg_addr & 0xFF);        // Low byte
        }

        // Perform SPI transfer
        if (vs_mal_spi_transfer((vs_uint8_t)spi_dev, (vs_uint8_t)csn, 
                               (vs_spi_mode_e)spi_mode, buf, buf, total_transfer_bytes) != 0) {
            printf("Error: SPI transfer failed\n");
            free(buf);
            free(data);
            return -1;
        }

        // Copy received data (skip register address bytes)
        memcpy(data, buf + reg_addr_size, read_bytes);
        free(buf);

        // Print results
        if (reg_addr_size == 1) {
            printf("Read %d bytes from register 0x%02x: ", read_bytes, reg_addr);
        } else {
            printf("Read %d bytes from register 0x%04x: ", read_bytes, reg_addr);
        }
        
        for (int i = 0; i < read_bytes; i++) {
            printf("0x%02x", data[i]);
            if (i < read_bytes - 1) printf(" ");
        }
        printf("\n");

        free(data);

    } else if (strcmp(argv[5], "-w") == 0) {
        // Write mode
        if (argc < 8) {
            printf("Error: Write mode requires at least 8 arguments\n");
            print_usage(argv[0]);
            return -1;
        }
        
        int data_bytes = parse_hex_or_dec(argv[6]);
        
        if (data_bytes <= 0 || data_bytes > 255) {
            printf("Error: Invalid data_bytes (must be 1-255)\n");
            return -1;
        }
        
        if (argc != 7 + data_bytes) {
            printf("Error: Expected %d data bytes, got %d\n", data_bytes, argc - 7);
            return -1;
        }

        // Allocate buffer
        int total_transfer_bytes = reg_addr_size + data_bytes;
        vs_uint8_t* buf = (vs_uint8_t*)malloc(total_transfer_bytes);
        
        if (!buf) {
            printf("Error: Memory allocation failed\n");
            return -1;
        }

        // Prepare tx buffer with register address
        if (reg_addr_size == 1) {
            buf[0] = (vs_uint8_t)reg_addr;
        } else { // reg_addr_size == 2
            buf[0] = (vs_uint8_t)((reg_addr >> 8) & 0xFF); // High byte
            buf[1] = (vs_uint8_t)(reg_addr & 0xFF);        // Low byte
        }
        
        // Add data bytes
        for (int i = 0; i < data_bytes; i++) {
            int data_val = parse_hex_or_dec(argv[7 + i]);
            if (data_val < 0 || data_val > 255) {
                printf("Error: Invalid data value at position %d: %s (must be 0x00-0xFF)\n", i, argv[7 + i]);
                free(buf);
                return -1;
            }
            buf[reg_addr_size + i] = (vs_uint8_t)data_val;
        }

        // Perform SPI transfer
        if (vs_mal_spi_transfer((vs_uint8_t)spi_dev, (vs_uint8_t)csn, 
                               (vs_spi_mode_e)spi_mode, buf, NULL, total_transfer_bytes) != 0) {
            printf("Error: SPI transfer failed\n");
            free(buf);
            return -1;
        }

        // Print confirmation
        if (reg_addr_size == 1) {
            printf("Write %d bytes to register 0x%02x: ", data_bytes, reg_addr);
        } else {
            printf("Write %d bytes to register 0x%04x: ", data_bytes, reg_addr);
        }
        
        for (int i = 0; i < data_bytes; i++) {
            printf("0x%02x", buf[reg_addr_size + i]);
            if (i < data_bytes - 1) printf(" ");
        }
        printf("\n");

        free(buf);

    } else {
        printf("Error: Invalid operation mode. Use -r for read or -w for write\n");
        print_usage(argv[0]);
        return -1;
    }

    return 0;
}