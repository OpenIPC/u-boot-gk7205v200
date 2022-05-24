/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#include "cipher_adapt.h"
#include "drv_rng.h"
#include "drv_klad.h"

#ifdef RSA_ENABLE

CIPHER_MUTEX g_rsa_mutex_kernel;
static gk_void *g_rsa_reg_base  = GK_NULL;
static gk_void *g_rsa_reg_crg   = GK_NULL;
static gk_u32   g_rsa_done      = GK_FALSE;

#define cipher_rsa_return_invalid_param(param) \
    do { \
        if (param) { \
            gk_err_cipher("Invalid params!\n"); \
            return GK_ERR_CIPHER_INVALID_PARA; \
        } \
    } while (0)

#define RSA_INTERRUPT_ENABLE
#define RSA_IRQ_NUMBER                          136

#define CIPHER_RSA_REG_BASE_RSA                   g_rsa_reg_base
#define SEC_RSA_BUSY_REG                          (CIPHER_RSA_REG_BASE_RSA + 0x50)
#define SEC_RSA_MOD_REG                           (CIPHER_RSA_REG_BASE_RSA + 0x54)
#define SEC_RSA_WSEC_REG                          (CIPHER_RSA_REG_BASE_RSA + 0x58)
#define SEC_RSA_WDAT_REG                          (CIPHER_RSA_REG_BASE_RSA + 0x5c)
#define SEC_RSA_RPKT_REG                          (CIPHER_RSA_REG_BASE_RSA + 0x60)
#define SEC_RSA_RRSLT_REG                         (CIPHER_RSA_REG_BASE_RSA + 0x64)
#define SEC_RSA_START_REG                         (CIPHER_RSA_REG_BASE_RSA + 0x68)
#define SEC_RSA_ADDR_REG                          (CIPHER_RSA_REG_BASE_RSA + 0x6C)
#define SEC_RSA_ERROR_REG                         (CIPHER_RSA_REG_BASE_RSA + 0x70)
#define SEC_RSA_CRC16_REG                         (CIPHER_RSA_REG_BASE_RSA + 0x74)
#define SEC_RSA_KEY_RANDOM_1                      (CIPHER_RSA_REG_BASE_RSA + 0x7c)
#define SEC_RSA_INT_EN                            (CIPHER_RSA_REG_BASE_RSA + 0x80)
#define SEC_RSA_INT_STATUS                        (CIPHER_RSA_REG_BASE_RSA + 0x84)
#define SEC_RSA_INT_RAW                           (CIPHER_RSA_REG_BASE_RSA + 0x88)
#define SEC_RSA_INT_ERR_CLR                       (CIPHER_RSA_REG_BASE_RSA + 0x8c)
#define SEC_RSA_KEY_RANDOM_2                      (CIPHER_RSA_REG_BASE_RSA + 0x94)
#define SEC_RSA_VERSION                           (CIPHER_RSA_REG_BASE_RSA + 0x90)

#define RSA_DATA_CLR               (7 << 4)
#define RSA_DATA_CLR_KEY           (1 << 4)
#define RSA_DATA_CLR_INPUT         (2 << 4)
#define RSA_DATA_CLR_OUTPUT        (4 << 4)
#define RSA_MOD_SEL                (3 << 0)
#define RSA_MOD_SEL_OPT            (0 << 0)
#define RSA_MOD_SEL_KEY_UPDATA     (1 << 0)
#define RSA_MOD_SEL_RAM_CLAER      (2 << 0)
#define RSA_MOD_SEL_CRC16          (3 << 0)
#define RSA_BUSY                   (1 << 0)
#define RSA_START                  (1 << 0)

#define RSA_RTY_CNT             50000
#define RSA_TIME_OUT            1000

#define RSA_RETRY_CNT           3

#define CRC16_POLYNOMIAL        0x1021

typedef enum {
    CIPHER_RSA_DATA_TYPE_CONTEXT,
    CIPHER_RSA_DATA_TYPE_MODULE,
    CIPHER_RSA_DATA_TYPE_KEY,
} cipher_rsa_data_type_e;

typedef enum {
    CIPHER_RSA_KEY_WIDTH_1K   = 0x00,
    CIPHER_RSA_KEY_WIDTH_2K   = 0x01,
    CIPHER_RSA_KEY_WIDTH_4K   = 0x02,
    CIPHER_RSA_KEY_WIDTH_3K   = 0x03,
    CIPHER_RSA_KEY_WIDTH_BUTT = 0xff,
} cipher_rsa_key_width_e;

static gk_void hal_rsa_start(gk_void)
{
    hal_cipher_write_reg(SEC_RSA_START_REG, 0x05);
}

static gk_s32 hal_rsa_wait_free(gk_void)
{
    gk_u32 value;
    gk_u32 try_count = 0;

    do {
        hal_cipher_read_reg(SEC_RSA_BUSY_REG, &value);
        if ((value & RSA_BUSY) == 0)
            return GK_SUCCESS;
        try_count++;
        cipher_msleep(1);
    } while (try_count < RSA_RTY_CNT);

    return GK_FAILURE;
}

static gk_void hal_rsa_clear_ram(gk_void)
{
    gk_u32 value;

    hal_cipher_read_reg(SEC_RSA_MOD_REG, &value);
    value &= 0x0c;
    value |= RSA_DATA_CLR_INPUT | RSA_DATA_CLR_OUTPUT | RSA_DATA_CLR_KEY | RSA_MOD_SEL_RAM_CLAER;
    hal_cipher_write_reg(SEC_RSA_MOD_REG, value);
}

static gk_void hal_rsa_config_mode(cipher_rsa_key_width_e ken_width)
{
    gk_u32 value;

    value = ((gk_u32)ken_width << 2) | RSA_MOD_SEL_OPT; /* 2 left shift */
    hal_cipher_write_reg(SEC_RSA_MOD_REG, value);
}

static gk_void hal_rsa_write_data(cipher_rsa_data_type_e data_type,
                                  gk_u8 *data,
                                  gk_u32 data_len,
                                  gk_u32 length,
                                  gk_u32 random[2]) /* 2 random size */
{
    gk_u32 *reg = GK_NULL;
    gk_u8 *pos = GK_NULL;
    gk_u32 i, value;
    gk_bool id = 0;

    if (data_type == CIPHER_RSA_DATA_TYPE_CONTEXT)
        reg = SEC_RSA_WDAT_REG;
    else
        reg = SEC_RSA_WSEC_REG;

    pos = data;
    for (i = 0; i < length; i += 4) { /* 4 groups */
        value  = (gk_u32)pos[0];
        value |= ((gk_u32)pos[1]) << 8;  /* 1 index, 8  left shift */
        value |= ((gk_u32)pos[2]) << 16; /* 2 index, 16 left shift */
        value |= ((gk_u32)pos[3]) << 24; /* 3 index, 24 left shift */
        if (data_type != CIPHER_RSA_DATA_TYPE_CONTEXT)
            value ^= random[id];

        hal_cipher_write_reg(reg, value);
        pos += 4; /* 4 groups */
        id = (gk_u32)id ^ 0x01;
    }
}

static gk_void hal_rsa_read_data(gk_u8 *data, gk_u32 data_len, gk_u32 klen)
{
    gk_u32 value;
    gk_u8 *pos = GK_NULL;
    gk_u32 i;

    pos = data;
    for (i = 0; i < klen; i += 4) { /* 4 groups */
        hal_cipher_read_reg(SEC_RSA_RRSLT_REG, &value);
        pos[0] = (gk_u8)(value & 0xFF);
        pos[1] = (gk_u8)((value >> 8) & 0xFF);  /* 1 index, 8  right shift */
        pos[2] = (gk_u8)((value >> 16) & 0xFF); /* 2 index, 16 right shift */
        pos[3] = (gk_u8)((value >> 24) & 0xFF); /* 3 index, 24 right shift */
        pos += 4; /* 4 groups */
    }
}

static gk_u32 hal_rsa_get_error_code(gk_void)
{
    gk_u32 value;

    hal_cipher_read_reg(SEC_RSA_ERROR_REG, &value);

    return value;
}

static gk_void hal_rsa_disable_int(gk_void)
{
    (gk_void)hal_cipher_write_reg(SEC_RSA_INT_EN, 0x00);
}

static gk_void hal_rsa_enable(gk_void)
{
    gk_u32 value;

    hal_cipher_read_reg(g_rsa_reg_crg, &value);
    hal_set_bit(value, 15); /* 15bit clock opened */
    hal_cipher_write_reg(g_rsa_reg_crg, value);
    cipher_msleep(1);

    hal_clear_bit(value, 14); /* 14bit cancel reset */
    hal_cipher_write_reg(g_rsa_reg_crg, value);
    cipher_msleep(1);

#ifdef INT_ENABLE
    hal_rsa_enable_int();
#endif
}

static gk_void hal_rsa_disable(gk_void)
{
    gk_u32 value;

    hal_cipher_read_reg(g_rsa_reg_crg, &value);

    hal_set_bit(value, 14); /* 14bit reset */
    hal_cipher_write_reg(g_rsa_reg_crg, value);
    cipher_msleep(1);

    hal_clear_bit(value, 15); /* 15bit clock closed */
    hal_cipher_write_reg(g_rsa_reg_crg, value);
}

#ifdef RSA_RAND_MASK

static gk_u16 g_crc_table[256]; /* 256 table size */

static gk_void drv_rsa_crc16_init(gk_void)
{
    gk_u16 remainder;
    gk_u16 n, m;
    gk_u16 *table = g_crc_table;

    for (n = 0; n < 256; n++) { /* 256 */
        remainder = (gk_u16)n << 8; /* 8 left shift */
        for (m = 8; m > 0; m--) { /* 8 */
            if (remainder & 0x8000)
                remainder = (remainder << 1) ^ CRC16_POLYNOMIAL;
            else
                remainder = (remainder << 1);
        }
        *(table + n) = remainder;
    }
}

static gk_u16 drv_rsa_crc16_block(gk_u16 crc, gk_u8 block[8], gk_u8 random[8]) /* 8 */
{
    gk_u8 i, j;
    gk_u8 val;

    for (i = 0; i < 2; i++) { /* 2 */
        for (j = 0; j < 4; j++) { /* 4 */
            val = block[i * 4 + 3 - j] ^ random[i * 4 + 3 - j]; /* 4, 3 */
            crc = (crc << 8) ^ g_crc_table[((crc >> 8) ^ val) & 0xFF]; /* 8 right shift */
        }
    }

    return crc;
}

static gk_u16 drv_rsa_key_crc(gk_u8 *rsa_n, gk_u8 *rsa_k, gk_u32 klen, gk_u32 random[2]) /* 2 */
{
    gk_u32 i;
    gk_u16 crc = 0;

    for (i = 0; i < klen; i += 8) /* 8 */
        crc = drv_rsa_crc16_block(crc, rsa_n + i, (gk_u8*)random);

    for (i = 0; i < klen; i += 8) /* 8 */
        crc = drv_rsa_crc16_block(crc, rsa_k + i, (gk_u8*)random);

    return crc;
}
#endif

CIPHER_QUEUE_HEAD g_rsa_wait_queue;
#ifdef INT_ENABLE
static CIPHER_IRQRETURN_T drv_rsa_isr(gk_s32 irq, gk_void *dev_id)
{
    gk_u32 int_stat;

    int_stat = hal_rsa_get_int();

    gk_info_cipher("RSA INT: 0x%x\n", int_stat);

    if (int_stat & 0x01) {
        g_rsa_done = GK_TRUE;
        gk_info_cipher("RSA Done\n");
        cipher_queue_wait_up(&g_rsa_wait_queue);
    }

    hal_rsa_clr_int();

    return CIPHER_IRQ_HANDLED;
}
#endif

gk_s32 drv_rsa_init(gk_void)
{
    gk_u32 rsa_stat = 0;
    gk_u32 rng_stat = 0;
#ifdef INT_ENABLE
    gk_s32 ret;
#endif

    cipher_mutex_init(&g_rsa_mutex_kernel);
    cipher_queue_init(&g_rsa_wait_queue);

    g_rsa_reg_crg = cipher_ioremap_nocache(CIPHER_RSA_CRG_ADDR_PHY, 16); /* 16 */
    if (g_rsa_reg_crg == 0) {
        gk_err_cipher("ioremap_nocache phy addr err:%x.\n", CIPHER_RSA_CRG_ADDR_PHY);
        return GK_FAILURE;
    }

    /* rng reset and clock */
    hal_cipher_read_reg(CIPHER_RNG_CRG_ADDR_PHY, &rng_stat);
    rng_stat |= RNG_CRG_CLOCK_BIT;
    rng_stat &= ~RNG_CRG_RESET_BIT;
    hal_cipher_write_reg(CIPHER_RNG_CRG_ADDR_PHY, rng_stat);
    cipher_msleep(5); /* sleep 5ms */

    /* rsa reset and clock */
    hal_cipher_read_reg(CIPHER_RSA_CRG_ADDR_PHY, &rsa_stat);
    rsa_stat |= RSA_CRG_CLOCK_BIT;
    rsa_stat |= RSA_CRG_RESET_BIT;
    hal_cipher_write_reg(CIPHER_RSA_CRG_ADDR_PHY, rsa_stat);
    cipher_msleep(5); /* sleep 5ms */

    /* rsa cancel reset */
    rsa_stat &= ~RSA_CRG_RESET_BIT;
    hal_cipher_write_reg(CIPHER_RSA_CRG_ADDR_PHY, rsa_stat);

    cipher_iounmap(g_rsa_reg_crg);
    g_rsa_reg_crg = GK_NULL;

    g_rsa_reg_base = cipher_ioremap_nocache(CIPHER_RSA_REG_BASE_ADDR_PHY, 0x1000);
    if (g_rsa_reg_base == GK_NULL) {
        gk_err_cipher("ioremap_nocache rsa Reg failed\n");
        return GK_FAILURE;
    }

#ifdef INT_ENABLE
    /* request irq */
    ret = cipher_request_irq(RSA_IRQ_NUMBER, drv_rsa_isr, "rsa");
    if (ret != GK_SUCCESS) {
        hal_rsa_disable_int();
        gk_err_cipher("Irq request failure, ret=%#x.\n", ret);
        return GK_FAILURE;
    }
    hal_rsa_enable_int();
#endif

#ifdef RSA_RAND_MASK
    drv_rsa_crc16_init();
#endif

    return GK_SUCCESS;
}

gk_void drv_rsa_deinit(gk_void)
{
    hal_rsa_disable_int();

#ifdef INT_ENABLE
    cipher_free_irq(RSA_IRQ_NUMBER, "rsa");
#endif

    if (g_rsa_reg_base != GK_NULL) {
        cipher_iounmap(g_rsa_reg_base);
        g_rsa_reg_base = GK_NULL;
    }
}

static gk_s32 drv_rsa_wait_done(gk_void)
{
#ifdef INT_ENABLE
    if (cipher_queue_wait_timeout(&g_rsa_wait_queue, &g_rsa_done, RSA_TIME_OUT) == 0) {
        gk_err_cipher("RSA time out! \n");
        return GK_FAILURE;
    }

    return GK_SUCCESS;
#else
    return hal_rsa_wait_free();
#endif
}

static gk_s32 drv_cipher_check_rsa_data(gk_u8 *rsa_n, gk_u8 *rsa_e, gk_u8 *rsa_mc, gk_u32 length)
{
    gk_u32 i;

    /* formula: rsa_mc > 0 */
    for (i = 0; i < length; i++) {
        if (rsa_mc[i] > 0)
            break;
    }
    if (i >= length) {
        gk_err_cipher("RSA M/C is zero, error!\n");
        return GK_ERR_CIPHER_INVALID_PARA;
    }

    /* formula: rsa_mc < rsa_n */
    for (i = 0; i < length; i++) {
        if (rsa_mc[i] < rsa_n[i])
            break;
    }
    if (i >= length) {
        gk_err_cipher("RSA M/C is larger than rsa_n, error!\n");
        return GK_ERR_CIPHER_INVALID_PARA;
    }

    /* formula: rsa_e >= 1 */
    for (i = 0; i < length; i++) {
        if (rsa_e[i] > 0)
            break;
    }
    if (i >= length) {
        gk_err_cipher("RSA D/rsa_e is zero, error!\n");
        return GK_ERR_CIPHER_INVALID_PARA;
    }

    return GK_SUCCESS;
}

static gk_s32 drv_cipher_clear_rsa_ram(gk_void)
{
    if (hal_rsa_wait_free() != GK_SUCCESS) {
        gk_err_cipher("RSA is busy and timeout,error!\n");
        return GK_FAILURE;
    }

    g_rsa_done = GK_FALSE;

    hal_rsa_clear_ram();
    hal_rsa_start();

    if (drv_rsa_wait_done() != GK_SUCCESS) {
        gk_err_cipher("RSA is busy and timeout,error!\n");
        return GK_FAILURE;
    }

    return GK_SUCCESS;
}

static gk_u8 g_rsa_n[CIPHER_MAX_RSA_KEY_LEN];
static gk_u8 g_rsa_k[CIPHER_MAX_RSA_KEY_LEN];
static gk_u8 g_rsa_m[CIPHER_MAX_RSA_KEY_LEN];

static gk_void drv_rsa_rand_mask(cipher_rsa_data_s *rsa_data,
                                 gk_u32 key_len,
                                 gk_u32 *random)
{
#ifdef RSA_RAND_MASK
    gk_u16 crc;

    random[0] = drv_cipher_rand();
    random[1] = drv_cipher_rand();
    crc = drv_rsa_key_crc(rsa_data->rsa_n, rsa_data->rsa_k, key_len, random);
    gk_info_cipher("CRC16: 0x%x\n", crc);
    hal_rsa_set_random(random);
    hal_rsa_set_crc(crc);
#endif
}

static gk_s32 drv_rsa_cipher_klad(cipher_rsa_data_s *rsa_data,
                                  gk_u32 key_len,
                                  gk_u32 *random)
{
    gk_s32 ret = GK_SUCCESS;
#ifdef CIPHER_KLAD_SUPPORT
    if (rsa_data->ca_type != GK_CIPHER_KEY_SRC_USER) {
        drv_cipher_klad_load_key(0, rsa_data->ca_type,
            GK_CIPHER_KLAD_TARGET_RSA, rsa_data->rsa_k, rsa_data->rsa_k_len);
        if (ret != GK_SUCCESS) {
            gk_err_cipher("drv_cipher_klad_load_key, error!\n");
            return ret;
        }
    } else {
        hal_rsa_write_data(CIPHER_RSA_DATA_TYPE_KEY, rsa_data->rsa_k,
            rsa_data->rsa_n_len, key_len, random);
    }
#else
    hal_rsa_write_data(CIPHER_RSA_DATA_TYPE_KEY, rsa_data->rsa_k,
        rsa_data->rsa_n_len, key_len, random);
#endif
    return ret;
}

static gk_s32 drv_rsa_key_info(cipher_rsa_data_s *rsa_data,
                               gk_u32 *key_len,
                               cipher_rsa_key_width_e *key_width)
{
    gk_u8 *p = GK_NULL;

    /* Only support the key width of 1024, 2048 and 4096 */
    if (rsa_data->rsa_n_len <= 128) { /* key n size 128 */
        *key_len = 128; /* key n size 128 */
        *key_width = CIPHER_RSA_KEY_WIDTH_1K;
    } else if (rsa_data->rsa_n_len <= 256) { /* key n size 256 */
        *key_len = 256; /* key n size 256 */
        *key_width = CIPHER_RSA_KEY_WIDTH_2K;
    } else if (rsa_data->rsa_n_len <= 384) { /* key n size 384 */
        *key_len = 384; /* key n size 384 */
        *key_width = CIPHER_RSA_KEY_WIDTH_3K;
    } else if (rsa_data->rsa_n_len <= 512) { /* key n size 512 */
        *key_len = 512; /* key n size 512 */
        *key_width = CIPHER_RSA_KEY_WIDTH_4K;
    } else {
        gk_err_cipher("rsa_n_len(0x%x) is invalid\n", rsa_data->rsa_n_len);
        return GK_ERR_CIPHER_INVALID_POINT;
    }

    /* if dataLen < key_len, padding 0 before data */
    p = g_rsa_n + (*key_len - rsa_data->rsa_n_len);
    if (cipher_copy_from_user(p, rsa_data->rsa_n, rsa_data->rsa_n_len)) {
        gk_err_cipher("copy data from user fail!\n");
        return GK_FAILURE;
    }

    p = g_rsa_k + (*key_len - rsa_data->rsa_k_len);
    if (cipher_copy_from_user(p, rsa_data->rsa_k, rsa_data->rsa_k_len)) {
        gk_err_cipher("copy data from user fail!\n");
        return GK_FAILURE;
    }

    p = g_rsa_m + (*key_len - rsa_data->data_len);
    if (cipher_copy_from_user(p, rsa_data->input_data, rsa_data->data_len)) {
        gk_err_cipher("copy data from user fail!\n");
        return GK_FAILURE;
    }

    return GK_SUCCESS;
}

static gk_s32 drv_cipher_calc_rsa_ex(cipher_rsa_data_s *rsa_data,
                                     gk_u32 key_len,
                                     cipher_rsa_key_width_e key_width)
{
    gk_u8 err_cnt;
    gk_s32 ret;
    gk_u32 err_code;
    gk_u64 random = 0;

    ret = drv_cipher_check_rsa_data(rsa_data->rsa_n, rsa_data->rsa_k, rsa_data->input_data, key_len);
    if (ret != GK_SUCCESS) {
        gk_err_cipher("RSA data invalid!\n");
        return ret;
    }

    g_rsa_done = GK_FALSE;

    for (err_cnt = 0; err_cnt < RSA_RETRY_CNT; err_cnt++) {
        hal_rsa_enable();

        ret = hal_rsa_wait_free();
        if (ret != GK_SUCCESS) {
            gk_err_cipher("RSA is busy!\n");
            return ret;
        }

        /* Config Mode */
        hal_rsa_config_mode(key_width);

        drv_rsa_rand_mask(rsa_data, key_len, (gk_u32 *)&random);

        /* Write rsa_n, rsa_e, rsa_m */
        hal_rsa_write_data(CIPHER_RSA_DATA_TYPE_MODULE,
            rsa_data->rsa_n, rsa_data->rsa_n_len, key_len, (gk_u32 *)&random);

        ret = drv_rsa_cipher_klad(rsa_data, key_len, (gk_u32 *)&random);
        if (ret != GK_SUCCESS)
            return ret;

        hal_rsa_write_data(CIPHER_RSA_DATA_TYPE_CONTEXT,
            rsa_data->input_data, rsa_data->rsa_n_len, key_len, (gk_u32 *)&random);

        /* Sart */
        hal_rsa_start();

        ret = drv_rsa_wait_done();
        if (ret != GK_SUCCESS) {
            gk_err_cipher("RSA is busy and timeout,error!\n");
            return ret;
        }

        /* Get result */
        hal_rsa_read_data(rsa_data->output_data, rsa_data->rsa_n_len, key_len);

        if (drv_cipher_clear_rsa_ram() != GK_SUCCESS)
            return GK_FAILURE;

        err_code = hal_rsa_get_error_code();

        hal_rsa_disable();

        if (err_code == 0)
            return GK_SUCCESS;
        else
            continue;
    }

    gk_err_cipher("RSA is err: chipset error code: 0x%x!\n", err_code);
    return GK_FAILURE;
}
#endif

static gk_s32 drv_cipher_calc_rsa(cipher_rsa_data_s *rsa_data)
{
    gk_s32 ret;
    gk_u32 key_len = 0;
    cipher_rsa_data_s cipher_rsa_data;
    cipher_rsa_key_width_e key_width = CIPHER_RSA_KEY_WIDTH_BUTT;

    cipher_rsa_return_invalid_param(rsa_data == GK_NULL);
    cipher_rsa_return_invalid_param(rsa_data->input_data == GK_NULL);
    cipher_rsa_return_invalid_param(rsa_data->output_data == GK_NULL);
    cipher_rsa_return_invalid_param(rsa_data->rsa_n == GK_NULL);
    cipher_rsa_return_invalid_param(rsa_data->rsa_k == GK_NULL);
    cipher_rsa_return_invalid_param(rsa_data->data_len != rsa_data->rsa_n_len);
    cipher_rsa_return_invalid_param(rsa_data->rsa_k_len > rsa_data->rsa_n_len);

    crypto_memset(g_rsa_n, sizeof(g_rsa_n), 0, sizeof(g_rsa_n));
    crypto_memset(g_rsa_k, sizeof(g_rsa_k), 0, sizeof(g_rsa_k));
    crypto_memset(g_rsa_m, sizeof(g_rsa_m), 0, sizeof(g_rsa_m));

    ret = drv_rsa_key_info(rsa_data, &key_len, &key_width);
    if (ret != GK_SUCCESS)
        return ret;

    crypto_memset(&cipher_rsa_data, sizeof(cipher_rsa_data), 0, sizeof(cipher_rsa_data_s));
    cipher_rsa_data.rsa_n = g_rsa_n;
    cipher_rsa_data.rsa_k = g_rsa_k;
    cipher_rsa_data.rsa_n_len = key_len;
    cipher_rsa_data.rsa_k_len = key_len;
    cipher_rsa_data.input_data = g_rsa_m;
    cipher_rsa_data.data_len = key_len;
    cipher_rsa_data.output_data = g_rsa_m;
    cipher_rsa_data.ca_type = rsa_data->ca_type;

    ret = drv_cipher_calc_rsa_ex(&cipher_rsa_data, key_len, key_width);
    if (ret != GK_SUCCESS)
        return GK_FAILURE;

    if (cipher_copy_to_user(rsa_data->output_data,
        g_rsa_m + (key_len - rsa_data->rsa_n_len), rsa_data->rsa_n_len)) {
        gk_err_cipher("copy data to user fail!\n");
        return GK_FAILURE;
    }

    return ret;
}

gk_s32 gk_drv_cipher_calc_rsa(cipher_rsa_data_s *rsa_data)
{
    gk_s32 ret;

    if (rsa_data == GK_NULL) {
        gk_err_cipher("Invalid params!\n");
        return GK_ERR_CIPHER_INVALID_PARA;
    }

    if (cipher_mutex_lock(&g_rsa_mutex_kernel)) {
        gk_err_cipher("down_interruptible failed!\n");
        return GK_FAILURE;
    }

    ret = drv_cipher_calc_rsa(rsa_data);

    cipher_mutex_unlock(&g_rsa_mutex_kernel);

    return ret;
}

