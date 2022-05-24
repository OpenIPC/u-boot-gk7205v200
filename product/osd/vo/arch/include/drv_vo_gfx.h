/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */


#ifndef __DRV_GFX_H__
#define __DRV_GFX_H__

#include "type.h"
#include "common_vo.h"
#include "mkp_vo.h"
#include "drv_vo_gfx_comm.h"

gk_s32 vo_drv_check_gfx_id(gk_vo_layer gfx_layer);
gk_s32 vo_drv_get_hal_gfx_layer(gk_vo_layer gfx_layer, hal_disp_layer *hal_layer);
gk_s32 vou_drv_get_gfx_bind_dev(gk_vo_layer layer);

gk_s32 graphic_drv_init(gk_void);
gk_s32 vo_drv_check_gfx_layer_id(gk_vo_layer layer);
gk_s32 vo_drv_check_gfx_attr_display_rect(gk_vo_layer layer, const gk_rect *rect);

gk_s32 vo_drv_gfx_convert_gfx_attr(const gk_vo_gfx_attr *gfx_attr_in, gk_vo_gfx_attr *gfx_attr_out);
gk_void vo_drv_gfx_open(gk_vo_layer gfx_id, const gk_vo_gfx_attr *gfx_attr);
gk_void vo_drv_gfx_close(gk_vo_layer layer_id);
#endif
