/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __CIPHER_ADAPT_H_
#define __CIPHER_ADAPT_H_

#ifndef GK_MINIBOOT_SUPPORT
#include <common.h>
#else
#include "delay.h"
#include "malloc.h"
#include "string.h"
#include "stdio.h"
#endif
#include "malloc.h"

#include "types.h"
#include "drv_cipher_ioctl.h"
#include "drv_cipher.h"
#include "cipher_config.h"
#include "spacc_intf.h"
#include "drv_rng.h"
#include "drv_rsa.h"
#include "drv_klad.h"
#include "../../../otp/hal_otp.h"
#include "drv_compat.h"

/**************************** M A C R O ****************************/
#define GK_ERR_CIPHER_NOT_INIT                     (gk_s32)(0x804D0001)
#define GK_ERR_CIPHER_INVALID_HANDLE               (gk_s32)(0x804D0002)
#define GK_ERR_CIPHER_INVALID_POINT                (gk_s32)(0x804D0003)
#define GK_ERR_CIPHER_INVALID_PARA                 (gk_s32)(0x804D0004)
#define GK_ERR_CIPHER_FAILED_INIT                  (gk_s32)(0x804D0005)
#define GK_ERR_CIPHER_FAILED_GETHANDLE             (gk_s32)(0x804D0006)
#define GK_ERR_CIPHER_FAILED_RELEASEHANDLE         (gk_s32)(0x804D0007)
#define GK_ERR_CIPHER_FAILED_CONFIGAES             (gk_s32)(0x804D0008)
#define GK_ERR_CIPHER_FAILED_CONFIGDES             (gk_s32)(0x804D0009)
#define GK_ERR_CIPHER_FAILED_ENCRYPT               (gk_s32)(0x804D000A)
#define GK_ERR_CIPHER_FAILED_DECRYPT               (gk_s32)(0x804D000B)
#define GK_ERR_CIPHER_BUSY                         (gk_s32)(0x804D000C)
#define GK_ERR_CIPHER_NO_AVAILABLE_RNG             (gk_s32)(0x804D000D)

#define GK_ID_CIPHER                        100

#define CIPHER_IOR                          _IOWR
#define CIPHER_IOW                          _IOW
#define CIPHER_IOWR                         _IOWR

#define CIPHER_IOC_DIR                      _IOC_DIR
#define CIPHER_IOC_TYPE                     _IOC_TYPE
#define CIPHER_IOC_NR                       _IOC_NR
#define CIPHER_IOC_SIZE                     _IOC_SIZE
#define u32_to_point(addr)                  ((gk_void*)((gk_size_t)(addr)))
#define point_to_u32(addr)                  ((gk_u32)((gk_size_t)(addr)))

#define hal_cipher_read_reg(addr, result)   (*(result) = *(volatile unsigned int *)(addr))
#define hal_cipher_write_reg(addr, result)  (*(volatile unsigned int *)(addr) = (result))

#define hal_set_bit(src, bit)               ((src) |= (1 << (bit)))
#define hal_clear_bit(src, bit)             ((src) &= ~(1 << (bit)))

#define cipher_cpu_to_be16(v) (((v) << 8) | ((v) >> 8))
#define cipher_cpu_to_be32(v) (((v) >> 24) | (((v) >> 8) & 0xff00) | (((v) << 8) & 0xff0000) | ((v) << 24))
#define cipher_cpu_to_be64(x) ((gk_u64)(                         \
    (((x) & 0x00000000000000ffULL) << 56) | (((x) & 0x000000000000ff00ULL) << 40) |   \
    (((x) & 0x0000000000ff0000ULL) << 24) | (((x) & 0x00000000ff000000ULL) <<  8) |   \
    (((x) & 0x000000ff00000000ULL) >>  8) | (((x) & 0x0000ff0000000000ULL) >> 24) |   \
    (((x) & 0x00ff000000000000ULL) >> 40) | (((x) & 0xff00000000000000ULL) >> 56)))

/**************************** S T D L I B ****************************/
#define cipher_ioremap_nocache(addr, size)  (gk_void*)(addr)
#define cipher_iounmap(x)

void *crypto_memset(gk_void *dst, unsigned int dlen, unsigned val, unsigned int len);
int  crypto_memcmp(const gk_void *a, const gk_void *b, unsigned len);
void *crypto_memcpy(gk_void *dst, unsigned dstlen, const gk_void *src, unsigned len);
gk_s32 cipher_waitdone_callback(gk_void *param);

#define CIPHER_MUTEX                        gk_void *
#define cipher_mutex_init(x)
#define cipher_mutex_lock(x)                0
#define cipher_mutex_unlock(x)

#define CIPHER_QUEUE_HEAD                   gk_void *
#define cipher_queue_init(x)
#define cipher_queue_wait_up(x)
#define cipher_queue_wait_timeout(head, con, time)

#define cipher_request_irq(irq, func, name)
#define cipher_free_irq(irq, name)
#define CIPHER_IRQRETURN_T                  gk_s32
#define CIPHER_IRQ_HANDLED                  1

#define cipher_copy_from_user(s, d, l)      (memcpy(s, d, l), 0)
#define cipher_copy_to_user(s, d, l)        (memcpy(s, d, l), 0)

#define cipher_malloc(x)                    malloc(x)
#define cipher_free(x)                      free(x)

#define cipher_msleep(msec)                 udelay(1000)
#define GK_PRINT                            printf
#define gk_err_cipher(fmt...)               GK_PRINT(fmt)
#define gk_info_cipher(fmt...)              // GK_PRINT(fmt)
#define CIPHER_PROC_PRINTF                  // osal_seq_printf

#define cipher_min(a, b) ((a) < (b) ? (a) : (b))

void hex2str(char buf[2], gk_u8 val); /* 2 buf size */

/**************************** P R I V A T E ****************************/
typedef struct {
    gk_void *start_vir_addr;
    gk_size_t start_phy_addr;
    gk_u32 mmz_addr;
    gk_u32 mmz_size;
} cipher_mmz_buf_t;

gk_s32  cipher_mmz_alloc_remap(gk_char *name, cipher_mmz_buf_t *cipher_mmz);
gk_void cipher_mmz_release_unmap(cipher_mmz_buf_t *cipher_mmz);
gk_s32  cipher_mmz_map(cipher_mmz_buf_t *cipher_mmz);
gk_void cipher_mmz_unmap(cipher_mmz_buf_t *cipher_mmz);

#endif

