/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */


#ifndef __HAL_VO_VIDEO_COMM_H__
#define __HAL_VO_VIDEO_COMM_H__

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
gk_void hal_video_set_layer_up_mode(hal_disp_layer layer, gk_u32 up_mode);
gk_void hal_video_set_layer_disp_rect(hal_disp_layer layer, gk_rect *rect);
gk_void hal_video_set_layer_video_rect(hal_disp_layer layer, gk_rect *rect);
gk_void hal_video_set_multi_area_lum_addr(hal_disp_layer layer, gk_u32 area_num, gk_ulong addr,
    gk_u16 stride);
gk_void hal_video_set_multi_area_chm_addr(hal_disp_layer layer, gk_u32 area_num, gk_ulong addr,
    gk_u16 stride);
#endif

#if VO_DESC("layer zme")
gk_void vo_hal_layer_set_zme_cfg(hal_disp_layer layer, gk_u32 vratio, gk_u32 hratio,
    vo_v0_zme_cfg *cfg, vo_zme_comm_pq_cfg *pq_cfg);
#endif

#if VO_DESC("cvfir")
gk_void vo_hal_layer_set_v1_cvfir_cfg(gk_u32 vratio, vdp_v1_cvfir_cfg *cfg,
    vo_zme_comm_pq_cfg *pq_cfg);
gk_void vo_hal_layer_set_v2_cvfir_cfg(gk_u32 vratio, vdp_v1_cvfir_cfg *cfg,
    vo_zme_comm_pq_cfg *pq_cfg);

gk_void vo_hal_layer_set_chroma_copy(gk_u32 layer, gk_bool chroma_copy_flag);
#endif

#if VO_DESC("layer")
/* video layer HFIR relative hal functions */
gk_void hal_video_hfir_set_ck_gt_en(hal_disp_layer layer, gk_u32 ck_gt_en);
gk_void hal_video_hfir_set_mid_en(hal_disp_layer layer, gk_u32 mid_en);
gk_void hal_video_hfir_set_hfir_mode(hal_disp_layer layer, gk_u32 hfir_mode);
gk_void hal_video_hfir_set_coef(hal_disp_layer layer, hfir_coef *coef);

gk_void hal_layer_enable_layer(hal_disp_layer layer, gk_u32 enable);
gk_void hal_layer_set_layer_data_fmt(hal_disp_layer layer, hal_disp_pixel_format data_fmt);
gk_void hal_layer_set_src_resolution(hal_disp_layer layer, const gk_rect *rect);
gk_void hal_layer_set_layer_in_rect(hal_disp_layer layer, gk_rect *rect);
gk_void hal_layer_set_zme_info(hal_disp_layer layer, gk_u32 width, gk_u32 height,
                               hal_disp_zme_outfmt data_fmt);
gk_void hal_layer_set_reg_up(hal_disp_layer layer);
gk_void hal_cbm_set_cbm_attr(hal_disp_layer layer, gk_vo_dev dev);
gk_void hal_cbm_set_cbm_bkg(hal_cbmmix mixer, hal_disp_bkcolor *bkg);
gk_void hal_cbm_set_cbm_mixer_prio(gk_vo_layer layer, gk_u8 prio, gk_u8 mixer_id);
#endif
#endif /* #if VO_DESC("UBOOT_VO") */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif /* end of __HAL_VO_VIDEO_COMM_H__ */
