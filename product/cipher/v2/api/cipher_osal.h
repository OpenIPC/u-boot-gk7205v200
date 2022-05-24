/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __CIPHER_OSAL_H__
#define __CIPHER_OSAL_H__

#ifndef GK_MINIBOOT_SUPPORT
#include <common.h>
#else
#include "string.h"
#include "stdio.h"
#endif
#include "common.h"
#include "malloc.h"
#include "types.h"
#include "drv_cipher.h"
#include "drv_cipher_ioctl.h"
#include "mpi_cipher.h"
#define CIPHER_KLAD_SUPPORT
#define HASH_BLOCK_SIZE                     128
#define HASH_RESULT_MAX_LEN                 64
#define HASH_RESULT_MAX_LEN_IN_WORD         16

#define GK_ERR_CIPHER_NOT_INIT              (gk_s32)(0x804D0001)
#define GK_ERR_CIPHER_INVALID_HANDLE        (gk_s32)(0x804D0002)
#define GK_ERR_CIPHER_INVALID_POINT         (gk_s32)(0x804D0003)
#define GK_ERR_CIPHER_INVALID_PARA          (gk_s32)(0x804D0004)
#define GK_ERR_CIPHER_FAILED_INIT           (gk_s32)(0x804D0005)
#define GK_ERR_CIPHER_FAILED_GETHANDLE      (gk_s32)(0x804D0006)
#define GK_ERR_CIPHER_FAILED_RELEASEHANDLE  (gk_s32)(0x804D0007)
#define GK_ERR_CIPHER_FAILED_CONFIGAES      (gk_s32)(0x804D0008)
#define GK_ERR_CIPHER_FAILED_CONFIGDES      (gk_s32)(0x804D0009)
#define GK_ERR_CIPHER_FAILED_ENCRYPT        (gk_s32)(0x804D000A)
#define GK_ERR_CIPHER_FAILED_DECRYPT        (gk_s32)(0x804D000B)
#define GK_ERR_CIPHER_BUSY                  (gk_s32)(0x804D000C)
#define GK_ERR_CIPHER_NO_AVAILABLE_RNG      (gk_s32)(0x804D000D)

#define cipher_cpu_to_be16(v)               (((v) << 8) | ((v) >> 8))

#define cipher_cpu_to_be32(v)                                   \
    ((((gk_u32)(v)) >> 24) | ((((gk_u32)(v)) >> 8) & 0xff00) |  \
     ((((gk_u32)(v)) << 8) & 0xff0000) | (((gk_u32)(v)) << 24))

#define cipher_cpu_to_be64(x)                                           \
    ((((gk_u64)(x) & (gk_u64)0x00000000000000ffULL) << 56) |            \
     (((gk_u64)(x) & (gk_u64)0x000000000000ff00ULL) << 40) |            \
     (((gk_u64)(x) & (gk_u64)0x0000000000ff0000ULL) << 24) |            \
     (((gk_u64)(x) & (gk_u64)0x00000000ff000000ULL) << 8)  |            \
     (((gk_u64)(x) & (gk_u64)0x000000ff00000000ULL) >> 8)  |            \
     (((gk_u64)(x) & (gk_u64)0x0000ff0000000000ULL) >> 24) |            \
     (((gk_u64)(x) & (gk_u64)0x00ff000000000000ULL) >> 40) |            \
     (((gk_u64)(x) & (gk_u64)0xff00000000000000ULL) >> 56))

#define cipher_min(a, b) ((a) < (b) ? (a) : (b))

#define UMAP_DEVNAME_CIPHER "cipher"
#define UMAP_CIPHER_MINOR_BASE              50

#define get_ulong_low(dw) (unsigned int)(dw)
#define get_ulong_high(dw) 0
#define make_ulong(low, high) (low)

#define GK_PRINT printf
#define gk_err_cipher(fmt...)       GK_PRINT(fmt)
#define gk_info_cipher(fmt...)      // GK_PRINT(fmt)

/* ************************* params check api ******************** */
#define inlet_var_over_max_return(val, max)    \
    do {                                       \
        if ((val) > (max)) {                   \
            gk_err_cipher("inlet param %s = 0x%x overflow, must less than %s(0x%x).\n", \
                #val, val, #max, max);         \
            return GK_ERR_CIPHER_INVALID_PARA; \
        }                                      \
    } while (0)

#define inlet_var_is_zero_return(val)                           \
    do {                                                        \
        if ((val) == 0) {                                       \
            gk_err_cipher("inlet param " #val " is zero\n");    \
            return GK_ERR_CIPHER_INVALID_PARA;                  \
        }                                                       \
    } while (0)

#define inlet_var_is_null_return(p)                         \
    do {                                                    \
        if ((p) == GK_NULL) {                               \
            gk_err_cipher("inlet param " #p " is null\n");  \
            return GK_ERR_CIPHER_INVALID_POINT;             \
        }                                                   \
    } while (0)

#define CIPHER_MUTEX gk_void *
#define cipher_mutex_init(x)
#define cipher_mutex_lock(x)
#define cipher_mutex_unlock(x)
#define cipher_mutex_destroy(x)

#define cipher_open(a, b, c)            (cipher_module_init(), 1)
#define cipher_close(x)                 (cipher_module_exit())
#define cipher_ioctl(dev, cmd, argp)    drv_cipher_ioctl(cmd, argp, NULL)

#define cipher_malloc(x) malloc(x)
#define cipher_free(x) free(x)

gk_void *crypto_memset(gk_void *dst, unsigned int dlen, unsigned val, unsigned int len);
int crypto_memcmp(const gk_void *a, const gk_void *b, unsigned len);
gk_void *crypto_memcpy(gk_void *dst, unsigned dstlen, const gk_void *src, unsigned len);

gk_s32 mpi_cipher_get_random_number(gk_u32 *random_number, gk_u32 time_out_us);

extern gk_s32 g_cipher_dev_fd;
extern gk_s32 g_cipher_init_counter;

#define check_cipher_not_open_return()              \
    do {                                            \
        if (g_cipher_init_counter < 0) {            \
            gk_err_cipher("CIPHER is not open.\n"); \
            return GK_ERR_CIPHER_NOT_INIT;          \
        }                                           \
    } while (0)

typedef struct {
    gk_void *start_vir_addr;
    gk_size_t start_phy_addr;
    gk_u32 mmz_size;
} cipher_mmz_buf_t;

#endif
