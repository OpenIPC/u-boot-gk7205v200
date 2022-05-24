/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#include "cipher_adapt.h"

static gk_void *g_rng_reg_base;

#define REG_RNG_BASE_ADDR                   g_rng_reg_base
#define SEC_COM_TRNG_CTRL                 (REG_RNG_BASE_ADDR + 0x200)
#define SEC_COM_TRNG_FIFO_DATA            (REG_RNG_BASE_ADDR + 0x204)
#define SEC_COM_TRNG_DATA_ST              (REG_RNG_BASE_ADDR + 0x208)

gk_s32 drv_rng_init(gk_void)
{
    gk_u32 rng_stat = 0;

    g_rng_reg_base = cipher_ioremap_nocache(CIPHER_RNG_REG_BASE_ADDR_PHY, 0x1000);
    if (g_rng_reg_base == GK_NULL) {
        gk_err_cipher("ioremap_nocache sha2 Reg failed\n");
        return GK_FAILURE;
    }

    hal_cipher_read_reg(CIPHER_RNG_CRG_ADDR_PHY, &rng_stat);
    rng_stat |= RNG_CRG_CLOCK_BIT;
    rng_stat &= ~RNG_CRG_RESET_BIT;
    hal_cipher_write_reg(CIPHER_RNG_CRG_ADDR_PHY, rng_stat);

    return GK_SUCCESS;
}

gk_void drv_rng_deinit(gk_void)
{
    cipher_iounmap(g_rng_reg_base);
}

static gk_s32 drv_cipher_get_random_number(cipher_rng_s *rng)
{
    gk_u32 rng_stat = 0;
    gk_u32 time_out = 0;

    if (rng->time_out_us == 0) {
        /* low 3bit(RNG_data_count[2:0]), indicate how many RNGs in the fifo is available now */
        hal_cipher_read_reg(SEC_COM_TRNG_DATA_ST, &rng_stat);
        if (((rng_stat >> 8) & 0x3F) <= 0) /* 8 right shift */
            return GK_ERR_CIPHER_NO_AVAILABLE_RNG;
    } else {
        while (time_out++ < rng->time_out_us) {
            /* low 3bit(RNG_data_count[2:0]), indicate how many RNGs in the fifo is available now */
            hal_cipher_read_reg(SEC_COM_TRNG_DATA_ST, &rng_stat);
            if (((rng_stat >> 8) & 0x3F) > 0) /* 8 right shift */
                break;
        }

        if (time_out >= rng->time_out_us)
            return GK_ERR_CIPHER_NO_AVAILABLE_RNG;
    }

    hal_cipher_read_reg(SEC_COM_TRNG_FIFO_DATA, &rng->ci_rng);

    return GK_SUCCESS;
}

gk_u32 drv_cipher_rand(gk_void)
{
    cipher_rng_s rng;

    rng.time_out_us = -1;
    if (drv_cipher_get_random_number(&rng) != GK_SUCCESS)
        gk_err_cipher("Get random number failed!\n");

    return rng.ci_rng;
}

gk_s32 gk_drv_cipher_get_random_number(cipher_rng_s *rng)
{
    gk_s32 ret;

    if (rng == NULL) {
        gk_err_cipher("Invalid params!\n");
        return GK_FAILURE;
    }

    ret = drv_cipher_get_random_number(rng);

    return ret;
}

