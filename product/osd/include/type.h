/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __GK_TYPE_H__
#define __GK_TYPE_H__

#ifdef __KERNEL__

#include <linux/types.h>
#else

#include <stdint.h>
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#ifndef NULL
    #define NULL                0L
#endif

#define GK_NULL                 0L
#define GK_SUCCESS              0
#define GK_FAILURE              (-1)

typedef unsigned char           gk_uchar;
typedef unsigned char           gk_u8;
typedef unsigned short          gk_u16;
typedef unsigned int            gk_u32;
typedef unsigned long           gk_ulong;

typedef char                    gk_char;
typedef signed char             gk_s8;
typedef short                   gk_s16;
typedef int                     gk_s32;
typedef long                    gk_slong;

typedef float                   gk_float;
typedef double                  gk_double;

typedef void                    gk_void;

#ifndef _M_IX86
    typedef unsigned long long  gk_u64;
    typedef long long           gk_s64;
#else
    typedef unsigned __int64    gk_u64;
    typedef __int64             gk_s64;
#endif
typedef unsigned long int       uintptr_t;

typedef unsigned long           gk_size_t;
typedef unsigned long           gk_length_t;
typedef unsigned long int       gk_phys_addr_t;
typedef gk_u32                  gk_handle;
typedef uintptr_t               gk_uintptr_t;
typedef unsigned int            gk_fr32;

typedef enum {
    GK_FALSE = 0,
    GK_TRUE  = 1,
} gk_bool;


/** @} */  /** <!-- ==== Structure Definition end ==== */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __GK_TYPE_H__ */

