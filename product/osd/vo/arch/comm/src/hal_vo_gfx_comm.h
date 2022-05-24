/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */


#ifndef __HAL_VO_GFX_COMM_H__
#define __HAL_VO_GFX_COMM_H__

#include "hal_vo_def.h"
#include "drv_vo_coef_comm.h"
#include "inner_vo.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

#if VO_DESC("UBOOT_VO")
#if VO_DESC("gfx uboot only")
gk_void hal_gfx_set_layer_disp_rect(hal_disp_layer layer, gk_rect *rect);
gk_void hal_gfx_set_layer_video_rect(hal_disp_layer layer, gk_rect *rect);
gk_void hal_gfx_enable_layer(hal_disp_layer layer, gk_u32 enable);
gk_void hal_gfx_set_layer_data_fmt(hal_disp_layer layer, hal_disp_pixel_format data_fmt);
gk_void hal_gfx_set_layer_in_rect(hal_disp_layer layer, gk_rect *rect);
gk_bool hal_gfx_set_reg_up(hal_disp_layer layer);
gk_void hal_gfx_set_gfx_pre_mult(hal_disp_layer layer, gk_u32 enable);
gk_void hal_gfx_set_gfx_addr(hal_disp_layer layer, gk_phys_addr_t l_addr);
gk_void hal_gfx_set_gfx_stride(hal_disp_layer layer, gk_u16 pitch);
gk_void hal_gfx_set_src_resolution(hal_disp_layer layer, gk_rect *rect);
#endif

#if VO_DESC("gfx")
gk_void hal_gfx_set_pixel_alpha_range(hal_disp_layer layer, gk_u32 arange);
#endif
#endif /* #if VO_DESC("UBOOT_VO") */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif /* end of __HAL_VO_GFX_COMM_H__ */
