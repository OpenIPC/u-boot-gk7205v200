/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __CIPHER_CONFIG_H_
#define __CIPHER_CONFIG_H_

#define RSA_ENABLE
#define OTP_SUPPORT
#define CIPHER_HASH_SUPPORT
#define CIPHER_EFUSE_SUPPORT
#define CIPHER_KLAD_SUPPORT

#define CIPHER_IRQ_NUMBER                       59

#if (defined(CONFIG_TARGET_GK7205V200) || defined(CONFIG_TARGET_GK7205V300) || \
       defined(CONFIG_TARGET_GK7202V300) || defined(CONFIG_TARGET_GK7605V100))
#define CIPHER_RNG_REG_BASE_ADDR_PHY            (0x10080000)
#define CIPHER_CIPHER_REG_BASE_ADDR_PHY         (0x10050000)
#define CIPHER_RSA_REG_BASE_ADDR_PHY            (0x10070000)

#define CIPHER_RSA_CRG_ADDR_PHY                 (0x120101A0)
#define RSA_CRG_CLOCK_BIT                       (0x01 << 5)
#define RSA_CRG_RESET_BIT                       (0x01 << 4)
#define CIPHER_SPACC_CRG_ADDR_PHY               (0x120101A0)
#define SPACC_CRG_CLOCK_BIT                     (0x01 << 9)
#define SPACC_CRG_RESET_BIT                     (0x01 << 8)
#define CIPHER_RNG_CRG_ADDR_PHY                 (0x120101A0)
#define RNG_CRG_CLOCK_BIT                       (0x01 << 3)
#define RNG_CRG_RESET_BIT                       (0x01 << 2)

#define CIPHER_KLAD_REG_BASE_ADDR_PHY           (0x10060000)
#define CIPHER_OTP_REG_BASE_ADDR_PHY            (0x10090000)
#define CIPHER_KLAD_CRG_ADDR_PHY                (0x120101A0)

#define KLAD_CRG_CLOCK_BIT                      (0x01 << 1)
#define KLAD_CRG_RESET_BIT                      (0x01 << 0)
#endif

#endif

