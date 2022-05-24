/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */


#ifndef __DRV_VO_LAYER_COMM_H__
#define __DRV_VO_LAYER_COMM_H__

#include "inner_vo.h"
#include "drv_vo_coef_comm.h"
#include "hal_vo_def.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

#if VO_DESC("UBOOT_VO")

#if VO_DESC("layer")
#define VO_LAYER_CSC_SCALE2P_DEF_VAL 0xa
#define VO_LAYER_CSC_CLIP_MIN 0x0
#define VO_LAYER_CSC_CLIP_MAX 0xfff
#endif

#if VO_DESC("layer csc")
gk_s32 vo_drv_get_csc_matrix(gk_vo_csc_matrix csc_matrix, const csc_coef **csc_tmp);
gk_void vo_drv_calculate_yuv2rgb(hal_csc_value *csc_value, const csc_coef *csc_tmp, csc_coef *coef);
gk_void vo_drv_calculate_rgb2yuv(hal_csc_value *csc_value, const csc_coef *csc_tmp, csc_coef *coef);
gk_void vo_drv_clip_layer_csc_coef(csc_coef *coef);
gk_void vou_drv_calc_csc_matrix(const gk_vo_csc *csc, gk_vo_csc_matrix csc_matrix, csc_coef *coef);
gk_void vou_drv_layer_csc_config(gk_vo_layer layer, gk_vo_csc *csc, csc_coef_param *csc_param);
gk_void vou_drv_get_layer_csc_param(gk_vo_layer layer, csc_coef_param *csc_param);
#endif
#endif /* #if VO_DESC("UBOOT_VO") */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* end of #ifdef __cplusplus */
#endif

#endif /* end of __DRV_VO_LAYER_COMM_H__ */
