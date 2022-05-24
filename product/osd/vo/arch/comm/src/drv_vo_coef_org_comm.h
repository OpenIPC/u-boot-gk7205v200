/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */


#ifndef __DRV_VO_COMM_COEF_ORG_H__
#define __DRV_VO_COMM_COEF_ORG_H__

#include "drv_vo_coef_comm.h"
#include "hal_vo_def.h"
#include "common_vo.h"
#include "inner_vo.h"

#if VO_DESC("UBOOT_VO")
const csc_coef *vo_get_csc_coef(gk_vo_csc_matrix csc_matrix);
const int *vo_get_sin_table(gk_void);
const int *vo_get_cos_table(gk_void);
#endif /* #if VO_DESC("UBOOT_VO") */

#endif
