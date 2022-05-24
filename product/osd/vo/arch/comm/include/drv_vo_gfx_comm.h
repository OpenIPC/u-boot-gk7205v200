/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */


#ifndef __DRV_GFX_COMM_H__
#define __DRV_GFX_COMM_H__

#include "type.h"
#include "common_vo.h"
#include "mkp_vo.h"
#include "inner_vo.h"
#include "hal_vo_def.h"
#include "drv_vo_coef_comm.h"

#if VO_DESC("UBOOT_VO")
#define GFX_CSC_SCALE    0xa
#define GFX_CSC_CLIP_MIN 0x0
#define GFX_CSC_CLIP_MAX 0x3ff
gk_s32 vo_drv_set_gfx_layer_csc(gk_vo_layer gfx_layer, gk_vo_csc *csc, csc_coef_param *csc_param);
typedef struct {
    gk_bool opened;

    gk_vo_csc gfx_csc;
    csc_coef_param csc_param;
} vo_gfxlayer_context;

gk_s32 graphic_drv_set_csc_coef(hal_disp_layer gfx_layer, gk_vo_csc *gfx_csc, csc_coef_param *csc_param);
gk_void gfx_drv_init_default_csc(gk_vo_layer layer, gk_vo_csc *csc);
gk_void gfx_drv_init_default_csc_param(gk_vo_layer layer, csc_coef_param *csc_param);
#endif /* #if VO_DESC("UBOOT_VO") */

#endif
