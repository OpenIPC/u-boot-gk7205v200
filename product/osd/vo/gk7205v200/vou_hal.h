/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __VOU_HAL_H__
#define __VOU_HAL_H__

#include "vou_reg.h"
#include "vou_def.h"
#include "vou_coef.h"

#ifdef __cplusplus
extern "C" {
#endif /* end of #ifdef __cplusplus */

gk_void hal_vo_init(gk_void);
gk_void hal_vo_exit(gk_void);

gk_void hal_write_reg(gk_u32 *address, gk_u32 value);
gk_u32 hal_read_reg(const gk_u32 *address);

gk_void hal_video_hfir_set_ck_gt_en(hal_disp_layer layer, gk_u32 ck_gt_en);
gk_bool hal_layer_set_src_resolution(hal_disp_layer layer,
                                     const gk_rect *rect);

gk_bool hal_intf_bt_set_dfir_en(gk_u32 dfir_en);

gk_s32 sys_hal_vo_bus_reset_sel(gk_bool reset);
gk_s32 sys_hal_vo_hd_out_pctrl(gk_bool clk_reverse);
gk_s32 sys_hal_vo_cfg_clk_en(gk_bool clk_en);
gk_s32 sys_hal_vo_core_clk_en(gk_s32 dev, gk_bool clk_en);
gk_s32 sys_hal_vou_ppc_clk_en(gk_s32 dev, gk_bool clk_en);
gk_s32 sys_hal_vo_out_clk_en(gk_s32 dev, gk_bool clk_en);
gk_s32 sys_hal_vo_apb_clk_en(gk_bool clk_en);
gk_s32 sys_hal_vo_bus_clk_en(gk_bool clk_en);
gk_s32 sys_hal_vo_dev_clk_en(gk_s32 vo_dev, gk_bool clk_en);
gk_s32 sys_hal_vo_hd0_ppc_sel(gk_u32 clk_sel);
gk_s32 sys_hal_vo_hd0_div_mode(gk_u32 hd0_div_mod);

gk_s32 sys_hal_vo_hd1_ppc_sel(gk_u32 bt_clk_ch_sel);
gk_s32 sys_hal_vo_hd1_div_mode(gk_u32 hd1_div_mod);
gk_s32 sys_hal_vo_out_clk_sel(gk_u32 clk_sel);
gk_s32 sys_hal_vo_hd_hdmi_clk_div(gk_u32 hdmi_clk_div);

gk_s32 sys_hal_vo_lcd_clk_en(gk_bool clk_en);
gk_s32 sys_hal_lcd_mclk_div(gk_u32 mclk_div);
gk_s32 sys_hal_lcd_data_mode(gk_u32 data_mode);
gk_s32 sys_hal_vo_bt_clk_en(gk_bool bt_clk_en);
gk_s32 sys_hal_vo_bt_clk_sel(gk_u32 bt_clk_ch_sel);
gk_s32 sys_hal_vo_mipi_chn_sel(gk_u32 mipi_ch_sel);
gk_s32 sys_hal_set_vo_low_power_ctrl(gk_bool low_power_clk_en);

gk_s32 sys_hal_set_vo_pll_frac(gk_u32 bits_set);
gk_s32 sys_hal_set_vo_pll_postdiv1(gk_u32 bits_set);
gk_s32 sys_hal_set_vo_pll_postdiv2(gk_u32 bits_set);
gk_s32 sys_hal_set_vo_pll_refdiv(gk_u32 bits_set);
gk_s32 sys_hal_set_vo_pll_fbdiv(gk_u32 bits_set);

gk_void hal_sys_control(gk_void);

gk_void hal_sys_set_arb_mode(gk_u32 mode);
gk_void hal_sys_vdp_reset_clk(gk_u32 sel);

gk_bool hal_disp_set_intf_enable(hal_disp_outputchannel chan, gk_bool intf);
gk_bool hal_disp_set_intf_ctrl(hal_disp_intf intf, const gk_u32 *ctrl_info);
gk_bool hal_disp_set_intf_sync(hal_disp_outputchannel chan,
                               const hal_disp_syncinfo *sync_info, const hal_disp_syncinv *inv);

gk_bool hal_disp_set_dev_multi_chn_en(hal_disp_outputchannel chn, hal_multi_chn multi_chn_en);
gk_bool hal_disp_set_intf_mux_sel(hal_disp_outputchannel chan, hal_disp_intf intf);
gk_bool hal_disp_set_bt1120_sel(hal_disp_outputchannel chan);

gk_bool hal_disp_set_intf_clip(hal_disp_intf intf, gk_bool clip, const hal_disp_clip *clip_data);
gk_bool hal_disp_set_vt_thd_mode(hal_disp_outputchannel chan, gk_u32 field_mode);
gk_bool hal_disp_set_vt_thd(hal_disp_outputchannel chan, gk_u32 vtthd);

gk_bool hal_disp_set_int_mask(gk_u32 mask_en);
gk_bool hal_disp_clr_int_mask(gk_u32 mask_en);

gk_bool hal_disp_set_lcd_serial_perd(gk_u32 serial_perd);

gk_void hal_disp_set_reg_up(hal_disp_outputchannel chan);

gk_bool hal_video_set_layer_up_mode(hal_disp_layer layer, gk_u32 up_mode);
gk_bool hal_video_set_hfir_mode(hal_disp_layer layer, hal_hfirmode mode);
gk_bool hal_video_hfir_set_coef(hal_disp_layer layer, const hfir_coef *coef);
gk_bool hal_video_set_layer_disp_rect(hal_disp_layer layer, const gk_rect *rect);
gk_bool hal_video_set_layer_video_rect(hal_disp_layer layer, const gk_rect *rect);
gk_bool hal_video_set_multi_area_l_addr(hal_disp_layer layer, gk_u32 area_num, gk_ulong l_addr, gk_u16 stride);
gk_bool hal_video_set_multi_area_c_addr(hal_disp_layer layer, gk_u32 area_num, gk_ulong c_addr, gk_u16 stride);
gk_bool hal_video_set_layer_alpha(hal_disp_layer layer, gk_u32 arange);
gk_void hal_video_cvfir_set_out_height(hal_disp_layer layer, gk_u32 out_height);
gk_void hal_video_cvfir_set_out_fmt(hal_disp_layer layer, gk_u32 out_fmt);
gk_void hal_video_cvfir_set_out_pro(hal_disp_layer layer, gk_u32 out_pro);
gk_void hal_video_cvfir_set_vzme_ck_gt_en(hal_disp_layer layer, gk_bool vzme_ck_gt_en);
gk_void hal_video_hfir_set_mid_en(hal_disp_layer layer, gk_u32 mid_en);
gk_void vdp_fdr_vid_set_chm_copy_en(gk_u32 layer, gk_u32 chm_copy_en);

gk_void hal_video_cvfir_set_cvfir_en(hal_disp_layer layer, gk_u32 cvfir_en);
gk_void hal_video_cvfir_set_cvmid_en(hal_disp_layer layer, gk_u32 cvmid_en);
gk_void hal_video_cvfir_set_cvfir_mode(hal_disp_layer layer, gk_u32 cvfir_mode);
gk_void hal_video_cvfir_set_vratio(hal_disp_layer layer, gk_u32 vratio);

gk_void hal_video_cvfir_set_v_chroma_offset(hal_disp_layer layer, gk_u32 vchroma_offset);
gk_void hal_video_cvfir_set_vb_chroma_offset(hal_disp_layer layer, gk_u32 vbchroma_offset);

gk_bool hal_layer_enable_layer(hal_disp_layer layer, gk_u32 enable);
gk_bool hal_layer_set_layer_data_fmt(hal_disp_layer layer,
                                     hal_disp_pixel_format data_fmt);
gk_bool hal_layer_set_layer_in_rect(hal_disp_layer layer, const gk_rect *rect);
gk_bool hal_layer_set_layer_galpha(hal_disp_layer layer,
                                   gk_u8 alpha0);
gk_bool hal_layer_set_zme_info(hal_disp_layer layer, gk_u32 width, gk_u32 height,
                               hal_disp_zme_outfmt zme_out_fmt);
gk_bool hal_layer_set_zme_enable(hal_disp_layer layer,
                                 hal_disp_zmemode mode,
                                 gk_u32 enable);
gk_bool hal_layer_set_reg_up(hal_disp_layer layer);

gk_bool hal_cbm_set_cbm_bkg(gk_u32 mixer_id, const hal_disp_bkcolor *bkg);
gk_bool hal_cbm_set_cbm_mixer_prio(hal_disp_layer layer, gk_u8 prio, gk_u8 mixer_id);
gk_bool hal_disp_set_vga_csc_en(hal_disp_layer layer, gk_bool csc_en);
gk_void hal_hdr_g_set_hdr_r2y_scale2p(gk_u32 hdr_r2y_scale2p);
gk_void hal_hdr_g_set_hdr_r2y_clip_min(gk_u32 hdr_r2y_clip_min);
gk_void hal_hdr_g_set_hdr_r2y_clip_max(gk_u32 hdr_r2y_clip_max);

gk_bool hal_graphic_set_gfx_ext(hal_disp_layer layer,
                                hal_gfx_bitextend mode);

gk_bool hal_graphic_set_gfx_pre_mult(hal_disp_layer layer, gk_u32 enable);
gk_bool hal_graphic_set_gfx_palpha(hal_disp_layer layer,
                                   gk_u32 alpha_en, gk_u32 arange,
                                   gk_u8 alpha0, gk_u8 alpha1);

gk_bool hal_graphic_set_gfx_addr(hal_disp_layer layer, gk_u64 l_addr);
gk_bool hal_graphic_set_gfx_stride(hal_disp_layer layer, gk_u16 pitch);
gk_bool hal_layer_set_csc_coef(hal_disp_layer layer, const vo_csc_coef *csc_coef);
gk_bool hal_layer_set_csc_en(hal_disp_layer layer, gk_bool csc_en);
gk_bool hal_gfx_set_src_resolution(hal_disp_layer layer, const gk_rect *rect);
gk_void hal_layer_csc_set_csc_mode(hal_disp_layer layer, gk_s32 csc_mode);

gk_bool hal_disp_set_dither_round_unlim(hal_disp_outputchannel chan, gk_u32 dither_mode);
gk_bool hal_disp_set_dither_data_in_out(hal_disp_outputchannel chan, gk_u32 i_data_width_dither,
                                        gk_u32 o_data_width_dither);
gk_bool hal_disp_set_dither_en(hal_disp_outputchannel chan, gk_u32 dither_en);
gk_bool hal_disp_set_dither_mode(hal_disp_outputchannel chan, gk_u32 dither_mode);
gk_bool hal_disp_set_dither_round(hal_disp_outputchannel chan, gk_u32 dither_round);
gk_bool hal_disp_set_dither_domain_mode(hal_disp_outputchannel chan, gk_u32 dither_domain_mode);
gk_bool hal_disp_set_dither_tap_mode(hal_disp_outputchannel chan, gk_u32 dither_tap_mode);
gk_bool hal_disp_set_dither_sed(hal_disp_outputchannel chan, const hal_disp_dihter_sed *dither_sed);
gk_bool hal_disp_set_dither_thr_min_max(hal_disp_outputchannel chan, gk_u32 thr_min, gk_u32 thr_max);

__inline static gk_u32 get_low_addr(gk_u64 phyaddr)
{
    return (gk_u32)phyaddr;
}

__inline static gk_u32 get_high_addr(gk_u64 phyaddr)
{
    return (gk_u32)(phyaddr >> 32); /* 32 to get high address */
}

#ifdef __cplusplus
}
#endif /* end of #ifdef __cplusplus */
#endif /* end of __VOU_HAL_H__ */

