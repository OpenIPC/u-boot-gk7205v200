/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */


#ifndef __MKP_VO_GFX_H__
#define __MKP_VO_GFX_H__

#include "type.h"
#include "common.h"
#include "common_vo.h"
#include "drv_vo_gfx.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

gk_void vo_init_gfx(gk_void);
gk_bool vo_is_dev_gfx_layer_enabled(gk_vo_dev dev);
gk_s32 vo_set_gfx_attr(gk_vo_layer layer, const gk_vo_gfx_attr *attr);
gk_s32 vo_enable_gfx_layer(gk_vo_layer layer, const gk_vo_gfx_attr *gfx_attr);
gk_s32 vo_disable_gfx_layer(gk_vo_layer layer_id);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif /* end of #ifndef __MKP_VO_GFX_H__ */
