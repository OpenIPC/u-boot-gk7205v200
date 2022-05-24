/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */


#ifndef __DRV_VO_VIDEO_COMM_H__
#define __DRV_VO_VIDEO_COMM_H__

#include "drv_vo_comm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

#if VO_DESC("UBOOT_VO")

#if VO_DESC("layer cfg")
gk_void vou_drv_layer_enable(gk_vo_layer layer, gk_bool enable);
gk_void vo_drv_disable_layer(gk_vo_layer layer);
gk_void vou_drv_set_layer_reg_up(gk_vo_layer layer);
gk_void vou_drv_set_layer_data_fmt(gk_vo_layer layer, vou_layer_pixerl_format data_fmt);
#endif

#if VO_DESC("layer")
gk_void vou_drv_init_default_csc(gk_vo_layer layer);
gk_void vou_drv_init_default_csc_param(gk_vo_layer layer);
gk_void vou_drv_set_default_csc(gk_vo_layer layer, gk_vo_csc *csc);
gk_s32 vou_drv_get_video_layer_bind_dev(gk_vo_layer layer);
gk_void vo_drv_set_layer_attr(gk_vo_layer layer, const gk_vo_video_layer_attr *vo_layer_attr);
gk_void vou_drv_get_default_csc(gk_vo_layer layer, gk_vo_csc *csc);

#endif

#endif /* #if VO_DESC("UBOOT_VO") */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* end of #ifdef __cplusplus */
#endif

#endif /* end of __DRV_VO_COMM_LAYER_H__ */
