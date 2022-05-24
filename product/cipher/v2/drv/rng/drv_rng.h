/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __DRV_RNG_H__
#define __DRV_RNG_H__

/* add include here */
#include "drv_cipher.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Macro Definition ******************************/
gk_u32 drv_cipher_rand(gk_void);
gk_s32 drv_rng_init(gk_void);
gk_void drv_rng_deinit(gk_void);

#ifdef __cplusplus
}
#endif
#endif /* __DRV_CIPHER_H__ */
