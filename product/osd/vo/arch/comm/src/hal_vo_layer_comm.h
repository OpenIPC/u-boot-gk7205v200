/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */


#ifndef __HAL_VO_LAYER_COMM_H__
#define __HAL_VO_LAYER_COMM_H__

#include "hal_vo_def.h"
#include "drv_vo_coef_comm.h"
#include "inner_vo.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

#if VO_DESC("UBOOT_VO")
#if VO_DESC("layer")
gk_void hal_video_set_layer_alpha(hal_disp_layer layer, gk_u32 arange);
gk_void hal_layer_set_csc_coef(hal_disp_layer layer, csc_coef *coef);
gk_void hal_layer_set_layer_global_alpha(hal_disp_layer layer, gk_u8 alpha0);
gk_void hal_layer_csc_set_enable_ck_gt_en(hal_disp_layer layer, gk_bool csc_en, gk_bool ck_gt_en);
#endif
#endif /* #if VO_DESC("UBOOT_VO") */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif /* end of __HAL_VO_LAYER_COMM_H__ */
