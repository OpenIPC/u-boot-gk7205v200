/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __SYS_DRV_H__
#define __SYS_DRV_H__

#include "common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

gk_s32 sys_drv_drv_ioctrl(gk_mpp_chn *mpp_chn, gk_u32 func_id, gk_void *io_args);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif /* end of #ifndef __SYS_DRV_H__ */