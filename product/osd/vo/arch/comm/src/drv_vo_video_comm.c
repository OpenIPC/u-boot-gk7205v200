/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#include "drv_vo_video_comm.h"
#include "drv_vo.h"
#include "hal_vo.h"
#include "drv_vo_coef_org_comm.h"
#include "vo.h"
#include <common.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

#if VO_DESC("UBOOT_VO")
#if VO_DESC("video")
gk_void vou_drv_init_default_csc(gk_vo_layer layer)
{
    gk_vo_csc *csc = GK_NULL;
    vo_drv_layer *drv_layer_ctx = GK_NULL;

    drv_layer_ctx = vo_drv_get_layer_ctx(layer);
    csc = &drv_layer_ctx->csc;
    csc->csc_matrix = GK_VO_CSC_MATRIX_BT601FULL_TO_BT601FULL;
    csc->luma = VO_CSC_DEF_VAL;
    csc->contrast = VO_CSC_DEF_VAL;
    csc->hue = VO_CSC_DEF_VAL;
    csc->saturation = VO_CSC_DEF_VAL;
    csc->ex_csc_en = GK_FALSE;
}

gk_void vou_drv_init_default_csc_param(gk_vo_layer layer)
{
    csc_coef_param *csc_param = GK_NULL;
    vo_drv_layer *drv_layer_ctx = GK_NULL;

    drv_layer_ctx = vo_drv_get_layer_ctx(layer);

    csc_param = &drv_layer_ctx->csc_param;
    csc_param->csc_scale2p = VO_LAYER_CSC_SCALE2P_DEF_VAL;
    csc_param->csc_clip_min = VO_LAYER_CSC_CLIP_MIN;
    csc_param->csc_clip_max = VO_LAYER_CSC_CLIP_MAX;
}

gk_void vo_drv_set_layer_attr(gk_vo_layer layer, const gk_vo_video_layer_attr *vo_layer_attr)
{
    vo_drv_layer *drv_layer_ctx = GK_NULL;

    drv_layer_ctx = vo_drv_get_layer_ctx(layer);
    memcpy(&drv_layer_ctx->vo_video_attr, vo_layer_attr,
        sizeof(gk_vo_video_layer_attr));
    drv_layer_ctx->video_config = GK_TRUE;
}

gk_void vou_drv_get_default_csc(gk_vo_layer layer, gk_vo_csc *csc)
{
    vo_drv_layer *drv_layer_ctx = vo_drv_get_layer_ctx(layer);
    csc->csc_matrix = drv_layer_ctx->csc.csc_matrix;
    csc->luma = drv_layer_ctx->csc.luma;
    csc->contrast = drv_layer_ctx->csc.contrast;
    csc->hue = drv_layer_ctx->csc.hue;
    csc->saturation = drv_layer_ctx->csc.saturation;
    csc->ex_csc_en = drv_layer_ctx->csc.ex_csc_en;
}

gk_void vo_drv_disable_layer(gk_vo_layer layer)
{
    vo_drv_layer *drv_layer_ctx = vo_drv_get_layer_ctx(layer);

    drv_layer_ctx->video_enable = GK_FALSE;
}

gk_s32 vou_drv_get_video_layer_bind_dev(gk_vo_layer layer)
{
    vo_drv_layer *layer_ctx = GK_NULL;
    layer_ctx = vo_drv_get_layer_ctx(layer);
    return layer_ctx->bind_dev;
}

#endif /* #if VO_DESC("UBOOT_VO") */

#if VO_DESC("video open close")
static gk_void video_layer_set_zme_cfg(unsigned int layer, gk_rect *disp_rect)
{
    vdp_vid_ip_cfg vid_cfg = {0};

    vid_cfg.csc_en = 0;
    vid_cfg.hfir_en = 1;
    vid_cfg.vid_iw = disp_rect->width;
    vid_cfg.vid_ih = disp_rect->height;

    vid_cfg.vid_ow = disp_rect->width;
    vid_cfg.vid_oh = disp_rect->height;
    vid_cfg.zme_en = GK_FALSE;
    vo_drv_video_set_zme_enable(layer, &vid_cfg);
}

gk_void vo_drv_layer_open(gk_vo_layer layer_id, const gk_vo_video_layer_attr *video_attr)
{
    hal_disp_layer layer;
    gk_rect disp_rect = {0};
    gk_ulong addr;
    gk_u32 strd;

    disp_rect = video_attr->display_rect;
    addr = video_attr->address;
    strd = video_attr->stride;

    layer = vou_drv_convert_layer(layer_id);

    hal_layer_set_layer_data_fmt(layer, HAL_INPUTFMT_YCBCR_SEMIPLANAR_420);
    hal_video_set_layer_disp_rect(layer, &disp_rect);
    hal_video_set_layer_video_rect(layer, &disp_rect);
    hal_layer_set_layer_in_rect(layer, &disp_rect);
    hal_layer_set_layer_global_alpha(layer, VO_ALPHA_OPACITY);
    hal_layer_set_src_resolution(layer, &disp_rect);

    video_layer_set_zme_cfg(layer, &disp_rect);

    hal_layer_set_zme_info(layer, disp_rect.width, disp_rect.height, HAL_DISP_ZME_OUTFMT422);

    hal_video_set_multi_area_lum_addr(layer, 0, addr, strd);
    hal_video_set_multi_area_chm_addr(layer, 0, addr + strd * VOU_ALIGN(disp_rect.height, VO_STRIDE_ALIGN), strd);
    hal_layer_enable_layer(layer, GK_TRUE);
    hal_layer_set_reg_up(layer);
    return;
}

gk_void vo_drv_layer_close(gk_vo_layer layer_id)
{
    hal_disp_layer layer;

    layer = vou_drv_convert_layer(layer_id);

    hal_layer_enable_layer(layer, GK_FALSE);
    hal_layer_set_reg_up(layer);
}
#endif
#endif /* #if VO_DESC("UBOOT_VO") */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */
