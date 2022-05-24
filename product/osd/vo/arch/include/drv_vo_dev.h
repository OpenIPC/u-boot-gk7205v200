/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */


#ifndef __DRV_VO_DEV_H__
#define __DRV_VO_DEV_H__

#include "common.h"
#include "sys_ext.h"
#include "common_vo.h"
#include "inner_vo.h"
#include "drv_vo_comm.h"
#include "drv_vo_dev_comm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

#if VO_DESC("UBOOT_VO")

typedef enum {
    VO_CLK_DIV1 = 0,
    VO_CLK_DIV2 = 1,
    VO_CLK_DIV_BUTT
} vo_clk_div;

typedef enum {
    SSC_VDP_DIV_25_TO_50 = 15, /* 25MHz~50MHz, 16div */
    SSC_VDP_DIV_50_TO_100 = 7, /* 50MHz~100MHz, 8div */
    SSC_VDP_DIV_100_TO_200 = 3, /* 100MHz~200MHz, 4div */
    SSC_VDP_DIV_200_TO_340 = 1, /* 200MHz~340MHz, 2div */
    SSC_VDP_DIV_340_TO_600 = 0, /* 340MHz~600MHz, 1div */

    SSC_VDP_DIV_BUTT
}gk_vo_hdmi_ssc_vdp_div_mode;

typedef struct {
    gk_vo_intf_sync index;
    gk_vo_pll pll;
    gk_vo_hdmi_ssc_vdp_div_mode div;
}vo_pll_param;

typedef enum {
    VO_CLK_PLL_SRC_FOUTPOSTDIV = 0, /* FOUTVCO / (postdiv1 * postdiv2) */
    VO_CLK_PLL_SRC_FOUT4 = 1, /* FOUTVCO / (postdiv1 * postdiv2 * 8) */

    VO_CLK_PLL_SRC_BUTT
}vo_clk_pll_src;

#if VO_DESC("dev")
gk_void vo_drv_set_dev_user_intf_sync_attr(gk_vo_dev dev, const gk_vo_user_sync_info *sync_info);
gk_void vo_drv_set_dev_div(gk_vo_dev dev, gk_u32 dev_div);
gk_void vo_drv_set_hdmi_div(gk_vo_dev dev, gk_u32 pre_div);
gk_s32 vo_drv_check_dev_clk_src(gk_vo_dev dev, gk_vo_clk_src clk_src);
gk_s32 vo_drv_check_intf_user_dev_div(gk_vo_dev dev, gk_u32 dev_div);
gk_s32 vo_drv_check_dev_fixed_clk(gk_vo_dev dev, gk_vo_fixed_clk fixed_clk);
#endif

#if VO_DESC("dev intf")
gk_s32 vou_drv_check_hdmi_sync(gk_vo_dev dev, gk_vo_intf_type intf_type, gk_vo_intf_sync intf_sync);
gk_s32 vou_drv_check_bt_sync(gk_vo_dev dev, gk_vo_intf_type intf_type, gk_vo_intf_sync intf_sync);
gk_s32 vou_drv_check_vga_sync(gk_vo_dev dev, gk_vo_intf_type intf_type, gk_vo_intf_sync intf_sync);
gk_s32 vou_drv_check_rgb_sync(gk_vo_dev dev, gk_vo_intf_type intf_type, gk_vo_intf_sync intf_sync);
gk_s32 vou_drv_check_dev_intf(gk_vo_dev dev, gk_vo_intf_type intf_type);
gk_u32 vo_drv_get_vga_gain_def(gk_void);
#endif

#if VO_DESC("dev")

gk_void vo_drv_open(gk_vo_dev dev);
gk_void vo_drv_close(gk_vo_dev dev);

gk_void vo_drv_dev_info_init(gk_void);
gk_vo_layer vo_drv_get_layer(gk_vo_dev dev);
gk_void vo_drv_default_setting(gk_void);
gk_s32 vo_drv_check_dev_id(gk_vo_dev dev);
gk_s32 vo_drv_check_layer_id(gk_vo_layer layer);
gk_void vou_drv_set_dev_clk(gk_vo_dev dev);
gk_void vo_drv_set_out_clk_div(gk_u32 dev, gk_u32 vo_out_clk_div);
gk_void vo_drv_set_dev_clk_en_sel(gk_vo_dev dev);
gk_void vo_drv_dev_get_clk_pll_src(gk_vo_dev dev, gk_u32 *pll_src);
gk_void vo_drv_set_intf_dither(gk_vo_intf_type intf_type, gk_u32 in_bit_width);
gk_u32 vo_drv_dev_get_max_clk(gk_vo_dev dev);
#endif

gk_void vo_drv_dev_get_pll_cfg(gk_vo_dev dev, gk_vo_pll *pll);
gk_void vo_drv_get_pll_cfg_no_div(gk_vo_intf_sync intf_sync, gk_vo_pll *pll);
gk_void vo_drv_get_hdmi_ssc_vdp_div(gk_vo_intf_sync intf_sync, gk_u32 *hdmi_ssc_vdp_div);
gk_void vo_drv_dev_set_pll_cfg(gk_vo_dev dev, gk_vo_pll *pll);

#if VO_DESC("dev")
gk_void vo_drv_set_all_crg_clk(gk_bool clk_en);
gk_void vo_lpw_bus_reset(gk_bool reset);
#endif

gk_bool vo_drv_is_dev_support_rgb(gk_vo_dev dev);

#endif /* #if VO_DESC("UBOOT_VO") */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif /* end of __DRV_VO_DEV_H__ */
