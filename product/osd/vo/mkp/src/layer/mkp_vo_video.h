/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */


#ifndef __MKP_VO_VIDEO_H__
#define __MKP_VO_VIDEO_H__

#include "common_vo.h"
#include "common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

#define VO_LAYER_DISPLAY_MAX_FRAME_RATE  240 /* max display frame rate */

#define VO_LAYER_PLAY_TOLERATION_UNIT   1000

gk_bool vo_is_multi_layer(gk_vo_layer layer);
gk_bool vo_is_single_layer(gk_vo_layer layer);
gk_bool vo_is_layer_enabled(gk_vo_layer layer);
gk_bool vo_is_layer_configured(gk_vo_layer layer);
gk_bool vo_is_layer_bypass(gk_vo_layer layer);
gk_bool vo_is_layer_cluster_mode_enabled(gk_vo_layer layer);
gk_vo_dev vo_get_video_layer_bind_dev(gk_vo_layer layer);
gk_s32 vo_check_layer_binded_dev(gk_vo_layer layer, gk_vo_dev dev);
gk_bool vo_is_layer_binded_dev(gk_vo_layer layer);
gk_bool vo_is_dev_layer_enabled(gk_vo_dev dev);
gk_bool vo_is_layer_chn_enabled(gk_vo_layer layer);
gk_void vo_exit_layer(gk_void);

gk_s32 vo_init_layer_ctx(gk_vo_layer layer);

gk_void vo_init_layer_info(gk_void);
gk_void vo_init_layer_resource(gk_void);
gk_void vo_deinit_layer_info(gk_void);

gk_s32 vo_enable_video_layer(gk_vo_layer layer, const gk_vo_video_layer_attr *video_attr);
gk_s32 vo_disable_video_layer(gk_vo_layer layer);

gk_s32 vo_set_video_layer_attr(gk_vo_layer layer, const gk_vo_video_layer_attr *video_attr);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif /* end of #ifndef __MKP_VO_VIDEO_H__ */

