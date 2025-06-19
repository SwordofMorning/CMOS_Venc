/*
 * Copyright (C) 2020, Visinex Technology. All rights reserved.
 * Description: General data types
 * Create: 2020-11-24
 */

#ifndef __VS_TYPE_H__
#define __VS_TYPE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <asm/types.h>

typedef __u8            vs_uint8_t;
typedef __s8            vs_int8_t;
typedef __u16           vs_uint16_t;
typedef __s16           vs_int16_t;
typedef __u32           vs_uint32_t;
typedef __s32           vs_int32_t;
typedef __u64           vs_uint64_t;
typedef __s64           vs_int64_t;
typedef __u8		vs_uchar_t;

typedef float           vs_float_t;
typedef double          vs_double_t;

typedef void            vs_void_t;
typedef unsigned long   vs_ulong_t;
typedef long            vs_long_t;
typedef unsigned long	vs_size_t;
typedef __s8            vs_char_t;

typedef enum {
	VS_FALSE = 0,
	VS_TRUE  = 1,
} vs_bool_t;

#ifndef NULL
#define NULL            0L
#endif

#define VS_NULL         0L
#define VS_SUCCESS      0
#define VS_FAILED      (-1)

#ifdef __cplusplus
}
#endif

#endif /* __VS_TYPE_H__ */
