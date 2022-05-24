/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */


#ifndef __MKP_VO_DEV_H__
#define __MKP_VO_DEV_H__

#include "common_vo.h"
#include "drv_vo.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

#define VO_DEV_TIME_REF_STEP        2

#define VO_DEV_MAX_FRAME_RATE       240

typedef struct {
    gk_vo_intf_sync intf_sync;
    gk_u32 width;
    gk_u32 height;
} vo_intf_sync_info;

typedef struct {
    gk_bool vo_enable;
    gk_bool config;
    gk_vo_pub_attr vou_attr;

    gk_u32 max_width;
    gk_u32 max_height;

    gk_bool user_config;
    gk_vo_user_sync_info vo_user_sync_info;
    gk_vo_vga_param vga_param;
    gk_vo_hdmi_param hdmi_param;
    gk_vo_rgb_param rgb_param;
    gk_vo_bt_param bt_param;
} vo_dev_info;

gk_void vo_init_dev_info(gk_void);
vo_dev_info *vo_get_dev_ctx(gk_vo_dev vo_dev);
gk_s32 vo_init_dev_ctx(gk_vo_dev dev);

gk_bool vo_is_dev_enabled(gk_vo_dev dev);
gk_void vo_get_dev_max_size(gk_vo_dev dev, gk_size *dev_size);
gk_void vo_get_intf_sync_size(gk_vo_intf_sync intf_sync, gk_u32 *width, gk_u32 *height);
gk_void vo_get_dev_max_size(gk_vo_dev dev, gk_size *dev_size);

gk_s32 vo_enable(gk_vo_dev dev);
gk_s32 vo_disable(gk_vo_dev dev);

gk_s32 vo_set_pub_attr(gk_vo_dev dev, const gk_vo_pub_attr *pub_attr);
gk_s32 vo_get_pub_attr(gk_vo_dev dev, gk_vo_pub_attr *pub_attr);

gk_s32 vo_check_dev_user_div(gk_vo_dev dev, gk_u32 dev_div, gk_u32 pre_div);
gk_s32 vo_set_user_sync_info(gk_vo_dev dev, const gk_vo_user_sync_info *sync_info);

gk_u32 vo_dev_get_bg_color(gk_vo_dev dev);
gk_void vo_dev_set_bg_color(gk_vo_dev dev, gk_u32 bg_color);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif /* end of #ifndef __MKP_VO_DEV_H__ */
