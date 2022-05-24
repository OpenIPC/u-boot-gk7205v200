/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#include "cipher_adapt.h"
#include "../../../otp/hal_otp.h"

#define KLAD_REG_BASE_ADDR                      g_klad_base
#define KLAD_REG_KLAD_CTRL                      (KLAD_REG_BASE_ADDR + 0x00)
#define KLAD_REG_DAT_IN                         (KLAD_REG_BASE_ADDR + 0x10)
#define KLAD_REG_ENC_OUT                        (KLAD_REG_BASE_ADDR + 0x20)
#define KLAD_KEY_LEN                            4
#define CIPHER_WAIT_IDEL_TIMES                  1000

static gk_void *g_klad_base = GK_NULL;

static gk_s32 hal_cipher_klad_config(gk_u32 chn_id,
                                     gk_u32 opt_id,
                                     gk_cipher_klad_target klad_target,
                                     gk_bool is_decrypt)
{
    gk_s32 ret;
    gk_u32 ctrl;

    /* Load efuse or OTP key to KLAD */
    ret = hal_efuse_otp_load_cipher_key(chn_id, opt_id);
    if (ret != GK_SUCCESS)
        return ret;

    ctrl  = chn_id << 16;               /* 16: klad channel id */
    ctrl |= (gk_u32)klad_target << 2;   /* 2: cipher klad */
    ctrl |= (gk_u32)is_decrypt << 1;    /* decrypt */
    ctrl |= 0x00;                       /* start */

    (gk_void)hal_cipher_write_reg(KLAD_REG_KLAD_CTRL, ctrl);

    return GK_SUCCESS;
}

static gk_void hal_cipher_start_klad(void)
{
    gk_u32 ctrl;

    /* start */
    (gk_void)hal_cipher_read_reg(KLAD_REG_KLAD_CTRL, &ctrl);
    ctrl |= 0x01;
    (gk_void)hal_cipher_write_reg(KLAD_REG_KLAD_CTRL, ctrl);
}

static gk_void hal_cipher_set_klad_data(gk_u32 *data_input, gk_u32 data_len)
{
    gk_u32 i;

    for (i = 0; i < data_len; i++)
        (gk_void)hal_cipher_write_reg(KLAD_REG_DAT_IN + i * KLAD_KEY_LEN, data_input[i]);
}

static gk_void hal_cipher_get_klad_data(gk_u32 *data_output)
{
    gk_u32 i;

    for (i = 0; i < KLAD_KEY_LEN; i++)
        (gk_void)hal_cipher_read_reg(KLAD_REG_ENC_OUT + i * KLAD_KEY_LEN, &data_output[i]);
}

static gk_s32 hal_cipher_wait_klad_done(void)
{
    gk_u32 try_count = 0;
    gk_u32 ctrl;

    do {
        hal_cipher_read_reg(KLAD_REG_KLAD_CTRL, &ctrl);
        if ((ctrl & 0x01) == 0x00)
            return GK_SUCCESS;
        try_count++;
    } while (try_count < CIPHER_WAIT_IDEL_TIMES);

    gk_err_cipher("Klad time out!\n");

    return GK_FAILURE;
}

static gk_void hal_klad_init(gk_void)
{
    gk_u32 crg_value;
    gk_u32 *sys_addr;

    sys_addr = cipher_ioremap_nocache(CIPHER_KLAD_CRG_ADDR_PHY, 0x100);
    if (sys_addr == GK_NULL) {
        gk_err_cipher("ERROR: sys_addr ioremap with nocache failed!!\n");
        return ;
    }

    hal_cipher_read_reg(sys_addr, &crg_value);
    crg_value |= KLAD_CRG_RESET_BIT;   /* reset */
    crg_value |= KLAD_CRG_CLOCK_BIT;   /* set the bit 0, clock opened */
    hal_cipher_write_reg(sys_addr, crg_value);

    /* clock select and cancel reset 0x30100 */
    crg_value &= (~KLAD_CRG_RESET_BIT); /* cancel reset */
    crg_value |= KLAD_CRG_CLOCK_BIT;    /* set the bit 0, clock opened */
    hal_cipher_write_reg(sys_addr, crg_value);

    cipher_iounmap(sys_addr);
}

gk_s32 drv_klad_init(gk_void)
{
    gk_s32 ret;

    g_klad_base = cipher_ioremap_nocache(CIPHER_KLAD_REG_BASE_ADDR_PHY, 0x100);
    if (g_klad_base == GK_NULL) {
        gk_err_cipher("ERROR: osal_ioremap_nocache for KLAD failed!!\n");
        return GK_FAILURE;
    }

    ret = hal_efuse_otp_init();
    if (ret != GK_SUCCESS) {
        cipher_iounmap(g_klad_base);
        return ret;
    }

    hal_klad_init();

    return GK_SUCCESS;
}

gk_void drv_klad_deinit(gk_void)
{
    if (g_klad_base != GK_NULL) {
        cipher_iounmap(g_klad_base);
        g_klad_base = GK_NULL;
    }
    if (g_efuse_otp_reg_base != GK_NULL) {
        cipher_iounmap(g_efuse_otp_reg_base);
        g_efuse_otp_reg_base = GK_NULL;
    }
    return ;
}

static gk_void drv_cipher_invbuf(gk_u8 *buf, gk_u32 u32len)
{
    gk_u32 i;
    gk_u8 ch;

    for (i = 0; i < u32len / 2; i++) { /* 2 */
        ch = buf[i];
        buf[i] = buf[u32len - i - 1];
        buf[u32len - i - 1] = ch;
    }
}

gk_s32 drv_cipher_klad_load_key(gk_u32 chn_id,
                                gk_cipher_ca_type root_key,
                                gk_cipher_klad_target klad_target,
                                gk_u8 *data_input,
                                gk_u32 key_len)
{
    gk_s32 ret;
    gk_u32 i, opt_id;
    gk_u32 key[KLAD_KEY_LEN] = {0};

    crypto_memset(key, sizeof(key), 0, sizeof(key));

    if ((root_key < GK_CIPHER_KEY_SRC_KLAD_1) ||
        (root_key > GK_CIPHER_KEY_SRC_KLAD_3)) {
        gk_err_cipher("Error: Invalid Root Key src 0x%x!\n", root_key);
        return GK_FAILURE;
    }

    if (((key_len % 16) != 0) || (key_len == 0)) { /* key 16 align */
        gk_err_cipher("Error: Invalid key len 0x%x!\n", key_len);
        return GK_FAILURE;
    }

    opt_id = root_key - GK_CIPHER_KEY_SRC_KLAD_1 + 1;

    ret = hal_cipher_klad_config(chn_id, opt_id, klad_target, GK_TRUE);
    if (ret != GK_SUCCESS) {
        gk_err_cipher("Error: cipher klad config failed!\n");
        return GK_FAILURE;
    }

    for (i = 0; i < key_len / 16; i++) { /* key 16 align */
        crypto_memcpy(key, 16, data_input + i * 16, 16); /* key 16 align */
        hal_cipher_set_klad_data(key, KLAD_KEY_LEN);
        hal_cipher_start_klad();
        ret = hal_cipher_wait_klad_done();
        if (ret != GK_SUCCESS) {
            gk_err_cipher("Error: cipher klad wait done failed!\n");
            return GK_FAILURE;
        }
    }

    return GK_SUCCESS;

}

gk_s32 drv_cipher_klad_encrypt_key(gk_cipher_ca_type root_key,
    gk_cipher_klad_target klad_target, gk_u32 *clean_key, gk_u32 *encrypt_key)
{
    gk_s32 ret;
    gk_u32 opt_id;

    if ((root_key < GK_CIPHER_KEY_SRC_KLAD_1) ||
        (root_key >= GK_CIPHER_KEY_SRC_BUTT)) {
        gk_err_cipher("Error: Invalid Root Key src 0x%x!\n", root_key);
        return GK_FAILURE;
    }

    if ((clean_key == GK_NULL) || (encrypt_key == GK_NULL)) {
        gk_err_cipher("Clean key or encrypt key is null.\n");
        return GK_FAILURE;
    }

    opt_id = root_key - GK_CIPHER_KEY_SRC_KLAD_1 + 1;

    ret = hal_cipher_klad_config(0, opt_id, GK_CIPHER_KLAD_TARGET_AES, GK_FALSE);
    if (ret != GK_SUCCESS) {
        gk_err_cipher("Error: cipher klad config failed!\n");
        return GK_FAILURE;
    }

    if (klad_target == GK_CIPHER_KLAD_TARGET_RSA)
        drv_cipher_invbuf((gk_u8*)clean_key, 16); /* 16 clean key len */

    hal_cipher_set_klad_data(clean_key, KLAD_KEY_LEN);
    hal_cipher_start_klad();
    ret = hal_cipher_wait_klad_done();
    if (ret != GK_SUCCESS) {
        gk_err_cipher("Error: cipher klad wait done failed!\n");
        return GK_FAILURE;
    }
    hal_cipher_get_klad_data(encrypt_key);

    return GK_SUCCESS;
}

