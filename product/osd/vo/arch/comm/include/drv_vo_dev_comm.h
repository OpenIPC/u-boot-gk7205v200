/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */


#ifndef __DRV_VO_COMM_DEV_H__
#define __DRV_VO_COMM_DEV_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

#if VO_DESC("UBOOT_VO")
typedef enum {
    VO_HD_HW_DEV = 0,  /* HD dev */
    VO_SD_HW_DEV,  /* SD dev */
    VO_UHD_HW_DEV,  /* UHD dev */
    VO_CAS_DEV,  /* cascade dev */
    VO_VIRT_DEV,  /* virtual dev */
    VO_DEV_TYPE_BUTT,
} vo_dev_type;

/* 50000: 50ms */
#define VO_DEV_REG_UP_TIMEOUT 50

#define VO_MIN_DIV_MODE      1
#define VO_MAX_DIV_MODE      4
#define VO_INTF_HDMI_DIV_MODE      1
#define VO_INTF_CVBS_DIV_MODE      4
#define VO_INTF_BT656_DIV_MODE     2
#define VO_INTF_RGB_DIV_MODE_1     1
#define VO_INTF_RGB_DIV_MODE_3     3
#define VO_INTF_RGB_DIV_MODE_4     4
#define VO_MIN_PRE_DIV_MODE  1
#define VO_MAX_PRE_DIV_MODE  32
#define VO_INTF_NO_HDMI_PRE_DIV_MODE      1

#define VO_MAX_USER_SYNC_INTFB 255
#define VO_MAX_USER_SYNC_VACT  4096
#define VO_MIN_USER_SYNC_VACT  100
#define VO_MAX_USER_SYNC_VBB   256
#define VO_MIN_USER_SYNC_VBB   1
#define VO_MAX_USER_SYNC_VFB   256
#define VO_MIN_USER_SYNC_VFB   1
#define VO_MAX_USER_SYNC_HACT  4096
#define VO_MIN_USER_SYNC_HACT  1
#define VO_MAX_USER_SYNC_HBB   65535
#define VO_MIN_USER_SYNC_HBB   1
#define VO_MAX_USER_SYNC_HFB   65535
#define VO_MIN_USER_SYNC_HFB   1
#define VO_MAX_USER_SYNC_BVACT 4096
#define VO_MAX_USER_SYNC_BVBB  256
#define VO_MAX_USER_SYNC_BVFB  256
#define VO_MAX_USER_SYNC_HPW   65535
#define VO_MIN_USER_SYNC_HPW   1
#define VO_MAX_USER_SYNC_VPW   256
#define VO_MIN_USER_SYNC_VPW   1

#if VO_DESC("dev drv api")
gk_u32 vo_drv_get_dev_bg_color(gk_vo_dev dev);
gk_vo_intf_type vo_drv_get_dev_intf_type(gk_vo_dev dev);
gk_vo_intf_sync vo_drv_get_dev_intf_sync(gk_vo_dev dev);
gk_u32 vo_drv_get_dev_vtth(gk_vo_dev dev);
gk_u32 vo_drv_get_dev_vtth2(gk_vo_dev dev);
gk_void vou_drv_set_disp_max_size(gk_vo_dev dev, gk_u32 max_width, gk_u32 max_height);
#endif

#if VO_DESC("dev")
gk_void vo_drv_get_sync_info(gk_vo_dev dev, hal_disp_syncinfo *sync_info);
gk_void vo_drv_set_sync_info(gk_vo_dev dev, hal_disp_syncinfo *sync_info);
gk_bool vo_drv_is_progressive(gk_vo_dev dev);
gk_void vo_drv_int_reg_up_mode(gk_vo_layer layer, vo_int_mode int_mode);
gk_void vo_drv_set_dev_default_vtth(gk_vo_dev dev);
gk_s32 vo_drv_set_dev_vtth(gk_vo_dev dev, gk_u32 vtth);
gk_s32 vo_drv_set_dev_vtth2(gk_vo_dev dev, gk_u32 vtth);
gk_bool vou_drv_get_dev_enable(gk_vo_dev dev);
gk_s32 vo_drv_check_dev_clkvalue(gk_vo_dev dev, const gk_vo_user_sync_info *sync_info);
gk_s32 vou_drv_check_dsd_intf(gk_vo_dev dev, gk_vo_intf_type intf_type);
gk_s32 vou_drv_check_dev_sync(gk_vo_dev dev, gk_vo_intf_type intf_type, gk_vo_intf_sync intf_sync);
gk_s32 vou_drv_check_dev_pub_attr(gk_vo_dev dev, const gk_vo_pub_attr *pub_attr);
gk_void vo_drv_get_dev_reg_up(gk_vo_dev dev);
#endif

#if VO_DESC("dev layer prio")
gk_void vou_drv_set_layer_priority(gk_vo_dev dev, gk_vo_layer layer, gk_u32 priority);
#endif

#if VO_DESC("dev intf")
gk_void vou_drv_intf_csc_config(gk_vo_intf_type intf, const gk_vo_csc *csc);
gk_void vo_drv_set_vga_param(gk_vo_dev dev, const gk_vo_vga_param *vga_param);
gk_void vo_drv_set_hdmi_param(gk_vo_dev dev, const gk_vo_hdmi_param *hdmi_param);
gk_void vo_drv_set_rgb_param(gk_vo_dev dev, const gk_vo_rgb_param *rgb_param);
gk_void vo_drv_set_dev_mpp_chn(gk_vo_dev dev, gk_mpp_chn *mpp_chn);
gk_bool vo_drv_is_hdmi_intf(gk_vo_intf_type intf_type);
gk_bool vo_drv_is_rgb_intf(gk_vo_intf_type intf_type);
gk_bool vo_drv_is_mipi_intf(gk_vo_intf_type intf_type);
gk_bool vo_drv_is_bt1120_intf(gk_vo_intf_type intf_type);
#endif

#if VO_DESC("dev interrupt")
gk_void vo_drv_int_set_mode(gk_vo_dev dev, vo_int_mode int_mode);
#endif

#if VO_DESC("check id")
gk_s32 vo_drv_check_dev_id_pub(gk_vo_dev dev);
gk_s32 vo_drv_check_layer_id_pub(gk_vo_layer layer);
#endif

gk_void vo_drv_set_pub_attr(gk_vo_dev dev, const gk_vo_pub_attr *vo_pub_attr);
gk_void vo_drv_enable(gk_vo_dev dev);
gk_void vo_drv_disable(gk_vo_dev dev);
gk_u32 vou_drv_get_dev_layer_num(gk_vo_dev dev);
gk_void vou_drv_get_dev_layer(gk_vo_dev dev, gk_vo_layer *layer, gk_u32 vo_layer_num);
gk_vo_layer vo_drv_get_gfx_layer(gk_vo_dev dev);

gk_bool vo_drv_is_phy_dev_uhd(gk_vo_dev dev);
gk_bool vo_drv_is_phy_dev_hd(gk_vo_dev dev);
gk_bool vo_drv_is_phy_dev_sd(gk_vo_dev dev);

#endif /* #if VO_DESC("UBOOT_VO") */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif /* end of __DRV_VO_COMM_DEV_H__ */
