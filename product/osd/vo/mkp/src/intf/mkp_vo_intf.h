/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __MKP_VO_INTF_H__
#define __MKP_VO_INTF_H__

#include "common_vo.h"
#include "mkp_vo.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

gk_void vo_init_vga_param(gk_vo_vga_param *vga_param);
gk_void vo_init_hdmi_param(gk_vo_hdmi_param *hdmi_param);
gk_void vo_init_rgb_param(gk_vo_rgb_param *rgb_param);
gk_void vo_init_bt_param(gk_vo_bt_param *bt_param);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif /* end of #ifndef __MKP_VO_INTF_H__ */
