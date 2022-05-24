/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __TYPES_H__
#define __TYPES_H__

typedef signed char s8;
typedef unsigned char u8;

typedef signed short s16;
typedef unsigned short u16;

typedef signed int s32;
typedef unsigned int u32;

typedef signed long long s64;
typedef unsigned long long u64;

typedef int bool;

#define INT8    char
#define INT16   short
#define INT32   int

#define UINT8   unsigned INT8
#define UINT16  unsigned INT16
#define UINT32  unsigned INT32

typedef unsigned long long      uint64_t;
typedef signed long long        int64_t;
typedef unsigned int            uint32_t;
typedef signed int              int32_t;
typedef unsigned short          uint16_t;
typedef signed short            int16_t;
typedef unsigned char           uint8_t;
typedef signed char             int8_t;
void udc_puts(const char *s);

#ifndef NULL
#define NULL                    (void *)0
#endif

#define TRUE    1
#define FALSE   0

#define BOOL    int

#endif /* end of __TYPES_H__ */

