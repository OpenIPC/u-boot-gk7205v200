/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __HAL_OTP_H__
#define __HAL_OTP_H__

#include "types.h"

#define OTP_USER_IF_BASE            g_efuse_otp_reg_base
#define OTP_USER_WORK_MODE          (OTP_USER_IF_BASE + 0x0000)
#define OTP_USER_OP_START           (OTP_USER_IF_BASE + 0x0004)
#define OTP_USER_KEY_INDEX          (OTP_USER_IF_BASE + 0x0008)
#define OTP_USER_KEY_DATA0          (OTP_USER_IF_BASE + 0x000c)
#define OTP_USER_KEY_DATA1          (OTP_USER_IF_BASE + 0x0010)
#define OTP_USER_KEY_DATA2          (OTP_USER_IF_BASE + 0x0014)
#define OTP_USER_KEY_DATA3          (OTP_USER_IF_BASE + 0x0018)
#define OTP_USER_KEY_DATA4          (OTP_USER_IF_BASE + 0x001c)
#define OTP_USER_KEY_DATA5          (OTP_USER_IF_BASE + 0x0020)
#define OTP_USER_KEY_DATA6          (OTP_USER_IF_BASE + 0x0024)
#define OTP_USER_KEY_DATA7          (OTP_USER_IF_BASE + 0x0028)
#define OTP_USER_KEY_DATA8          (OTP_USER_IF_BASE + 0x002c)
#define OTP_USER_FLAG_VALUE         (OTP_USER_IF_BASE + 0x0030)
#define OTP_USER_FLAG_INDEX         (OTP_USER_IF_BASE + 0x0034)
#define OTP_USER_REV_ADDR           (OTP_USER_IF_BASE + 0x0038)
#define OTP_USER_REV_WDATA          (OTP_USER_IF_BASE + 0x003c)
#define OTP_USER_REV_RDATA          (OTP_USER_IF_BASE + 0x0040)
#define OTP_USER_LOCK_STA0          (OTP_USER_IF_BASE + 0x0044)
#define OTP_USER_LOCK_STA1          (OTP_USER_IF_BASE + 0x0048)
#define OTP_USER_CTRL_STA           (OTP_USER_IF_BASE + 0x004c)

#if (defined(CONFIG_TARGET_GK7205V200) || defined(CONFIG_TARGET_GK7205V300) || \
       defined(CONFIG_TARGET_GK7202V300) || defined(CONFIG_TARGET_GK7605V100))
#define REG_SYS_OTP_CLK_ADDR_PHY    0x120101BC
#define OTP_CRG_CLOCK_BIT           (0x01 << 1)
#endif

gk_s32 hal_efuse_otp_init(gk_void);
gk_s32 hal_efuse_otp_load_cipher_key(gk_u32 chn_id, gk_u32 opt_id);

#endif
