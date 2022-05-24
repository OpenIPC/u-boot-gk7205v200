/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */


#ifndef __HAL_VO_DEV_COMM_H__
#define __HAL_VO_DEV_COMM_H__

#include "hal_vo_def.h"
#include "drv_vo_coef_comm.h"
#include "inner_vo.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

#if VO_DESC("UBOOT_VO")
#if VO_DESC("intf")
#define VO_CVBS_DATE_NTSC 0x108410dc
gk_void hal_sys_set_rd_bus_id(gk_u32 mode);
gk_void hal_disp_set_intf_enable(gk_vo_dev dev, gk_bool intf);
gk_void hal_disp_get_intf_enable(gk_vo_dev dev, gk_bool *intf_en);
gk_void hal_disp_set_hdmi_mode(gk_vo_dev dev, gk_u32 color_space);
gk_void hal_disp_set_dev_multi_chn_en(gk_vo_dev dev, hal_multichn_en multi_chn_en);
#endif

#if VO_DESC("disp intf")
gk_void hal_disp_set_vtthd_mode(gk_vo_dev dev, gk_u32 field_mode);
gk_void hal_disp_set_vtthd(gk_vo_dev dev, gk_u32 vtthd, gk_u32 vtthd2);
gk_void hal_disp_clear_int_status(gk_u32 int_msk);
gk_void hal_intf_bt_set_dfir_en(gk_u32 dfir_en);
gk_void hal_intf_bt_set_data_width(gk_u32 data_width);
gk_void hal_disp_set_reg_up(gk_vo_dev dev);
gk_u32 hal_disp_get_reg_up(gk_vo_dev dev);
#endif

#if VO_DESC("intf")
gk_void vo_hal_intf_set_mux_sel(gk_vo_dev dev, gk_vo_intf_type intf);
gk_void vo_hal_intf_set_bt_sync_inv(hal_disp_syncinv *inv);
gk_void vo_hal_intf_set_cvbs_sync_inv(hal_disp_syncinv *inv);
gk_void vo_hal_intf_set_vga_sync_inv(hal_disp_syncinv *inv);
gk_void vo_hal_intf_set_hdmi_sync_inv(hal_disp_syncinv *inv);
gk_void vo_hal_intf_set_hdmi_csc_dc_coef(vdp_csc_dc_coef *csc_dc_coef);
gk_void vo_hal_intf_set_vga_csc_dc_coef(vdp_csc_dc_coef *csc_dc_coef);
gk_void vo_hal_intf_set_hdmi_csc_coef(vdp_csc_coef *csc_coef);
gk_void vo_hal_intf_set_vga_csc_coef(vdp_csc_coef *csc_coef);
gk_void vo_hal_intf_set_csc_cfg(gk_vo_intf_type intf, csc_coef *coef);
gk_void vo_hal_intf_set_dac_sel(gk_vo_intf_type intf_type);
gk_void vo_hal_intf_set_vga_and_cvbs_dac_enable(gk_bool enable);
gk_void vo_hal_intf_set_cvbs_dac_cfg(gk_void);
gk_void vo_hal_intf_set_dac_chn_enable(gk_vo_intf_type intf_type, gk_bool enable);
gk_void vo_hal_intf_set_dac_gc(gk_vo_intf_type intf_type, gk_u32 dac_gc);
gk_void vo_hal_intf_set_dac_cablectr(gk_vo_intf_type intf_type, gk_u32 cablectr);
gk_void vo_hal_intf_set_dac_det(gk_vo_dev dev, gk_vo_intf_type intf_type, gk_bool enable);
gk_void vo_hal_intf_set_vga_hsp_cfg(gk_bool enable, gk_u32 strength);
gk_void vo_hal_intf_set_date_cvbs_burst_start(gk_void);
gk_void vo_hal_intf_set_date_cvbs_color_burst(gk_void);
gk_u32 vo_hal_intf_get_date(gk_vo_intf_sync intf_sync, gk_u32 vact);
gk_void vo_hal_intf_set_date_cfg(gk_vo_intf_sync intf_sync, gk_u32 date);
gk_void vo_hal_intf_set_clip_cfg(gk_vo_intf_type intf_type, gk_bool clip, hal_disp_clip *clip_data);
gk_void vo_hal_intf_set_sync_info(gk_vo_dev dev, hal_disp_syncinfo *sync_info);
#endif
#endif /* #if VO_DESC("UBOOT VO") */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif /* end of __HAL_VO_DEV_COMM_H__ */
