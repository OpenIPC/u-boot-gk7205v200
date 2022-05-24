/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#include "cipher_adapt.h"

gk_s32 gk_drv_compat_init(void)
{
    gk_s32 ret;

    ret = drv_klad_init();
    if (ret != GK_SUCCESS) {
         return ret;
    }

    return GK_SUCCESS;
}

gk_s32 gk_drv_compat_deinit(void)
{
    drv_klad_deinit();

    return GK_SUCCESS;
}

