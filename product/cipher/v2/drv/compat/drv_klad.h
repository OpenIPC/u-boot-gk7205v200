/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef DRV_KLAD_H
#define DRV_KLAD_H

#include "types.h"
#include "common.h"
#include "drv_cipher_ioctl.h"

extern gk_void *g_efuse_otp_reg_base;

gk_s32 drv_klad_init(gk_void);
gk_void drv_klad_deinit(gk_void);

gk_s32 drv_cipher_klad_load_key(gk_u32 chn_id,
                                gk_cipher_ca_type root_key,
                                gk_cipher_klad_target klad_target,
                                gk_u8 *data_input,
                                gk_u32 key_len);

gk_s32 drv_cipher_klad_encrypt_key(gk_cipher_ca_type root_key,
                                   gk_cipher_klad_target klad_target,
                                   gk_u32 *clean_key,
                                   gk_u32 *encrypt_key);

#endif