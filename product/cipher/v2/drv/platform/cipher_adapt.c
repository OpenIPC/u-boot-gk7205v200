/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#include "cipher_adapt.h"

gk_s32 cipher_mmz_alloc_remap(gk_char *name, cipher_mmz_buf_t *cipher_mmz)
{
    if (cipher_mmz->mmz_size == 0) {
        gk_err_cipher("Error: length of malloc is invalid!\n");
        return GK_FAILURE;
    }

    cipher_mmz->start_phy_addr = (gk_size_t)(uintptr_t)memalign(ARCH_DMA_MINALIGN, cipher_mmz->mmz_size);

    if (cipher_mmz->start_phy_addr == 0) {
        gk_err_cipher("Error: Get phyaddr for cipher input failed!\n");
        return GK_FAILURE;
    }
    cipher_mmz->start_vir_addr = (gk_u8 *)(uintptr_t)cipher_mmz->start_phy_addr;

    return GK_SUCCESS;
}

gk_void cipher_mmz_release_unmap(cipher_mmz_buf_t *cipher_mmz)
{
    if (cipher_mmz->start_phy_addr > 0) {
        free(cipher_mmz->start_vir_addr);
        cipher_mmz->start_phy_addr = 0;
        cipher_mmz->start_vir_addr = NULL;
    }
}

gk_s32 cipher_mmz_map(cipher_mmz_buf_t *cipher_mmz)
{
    cipher_mmz->start_vir_addr = (gk_u8 *)(uintptr_t)cipher_mmz->start_phy_addr;

    return GK_SUCCESS;
}

gk_void cipher_mmz_unmap(cipher_mmz_buf_t *cipher_mmz)
{

}

gk_s32 cipher_waitdone_callback(gk_void *param)
{
    gk_bool *done = GK_NULL;

    done = param;

    return  *done != GK_FALSE;
}

/************************* SYSTEM API ************************/
gk_void *crypto_memcpy(gk_void *dst, unsigned dstlen, const gk_void *src, unsigned len)
{
    if ((dst == NULL) || (src == NULL) || (dstlen < len)) {
        gk_err_cipher("Error: cipher call %s with invalid parameter.\n", __FUNCTION__);
        return NULL;
    }

    return memcpy(dst, src, len);
}

gk_void *crypto_memset(gk_void *dst, unsigned int dlen, unsigned val, unsigned int len)
{
    if ((dst == NULL) || (dlen < len)) {
        gk_err_cipher("Error: cipher call %s with invalid parameter.\n", __FUNCTION__);
        return NULL;
    }

    return memset(dst, val, len);
}

int crypto_memcmp(const gk_void *a, const gk_void *b, unsigned int len)
{
    if ((a == NULL) || (b == NULL)) {
        gk_err_cipher("Error: cipher call %s with invalid parameter, point is null.\n", __FUNCTION__);
        return GK_FAILURE;
    }

    if (a == b) {
        gk_err_cipher("Error: cipher call %s with invalid parameter, comparing with the same address.\n", __FUNCTION__);
        return GK_FAILURE;
    }

    return memcmp(a, b, len);
}

void hex2str(char buf[2], gk_u8 val) /* 2 buf size */
{
    gk_u8 high, low;

    high = (val >> 4) & 0x0F; /* 4 */
    low = val & 0x0F;

    if (high <= 9) /* 9 */
        buf[0] = high + '0';
    else
        buf[0] = (high - 0x0A) + 'A';

    if (low <= 9) /* 9 */
        buf[1] = low + '0';
    else
        buf[1] = (low - 0x0A) + 'A';
}

void print_data(const char *name, gk_u8 *data, gk_u32 size)
{
    gk_u32 i;

    if (name != GK_NULL)
        GK_PRINT("[%s]:\n", name);

    for (i = 0; i < size; i++) {
        if (i % 16 == 0 && i != 0) /* 16 align */
            GK_PRINT("\n");
        GK_PRINT("%02X ", data[i]);
    }
    GK_PRINT("\n");
}
