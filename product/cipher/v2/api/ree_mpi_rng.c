/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#include "cipher_osal.h"

gk_s32 mpi_cipher_get_random_number(gk_u32 *random_number, gk_u32 time_out_us)
{
    gk_s32 ret;
    cipher_rng_s ci_rng;

    inlet_var_is_null_return(random_number);

    ci_rng.ci_rng = 0;
    ci_rng.time_out_us = time_out_us;

    ret = cipher_ioctl(g_cipher_dev_fd, CMD_CIPHER_GETRANDOMNUMBER, &ci_rng);

    if (ret == GK_SUCCESS)
        *random_number = ci_rng.ci_rng;

    return ret;
}

gk_s32 gk_api_cipher_get_random_number(gk_u32 *random_number)
{
    check_cipher_not_open_return();

    return mpi_cipher_get_random_number(random_number, 0);
}

