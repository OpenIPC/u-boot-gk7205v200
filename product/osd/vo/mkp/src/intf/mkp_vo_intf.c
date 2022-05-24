/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#include "mkp_vo_intf.h"
#include "drv_vo.h"
#include "vo.h"
#include "mkp_vo.h"

#include "mkp_vo_comm.h"
#include "mkp_vo_dev.h"

#if VO_DESC("hdmi param")
gk_void vo_init_hdmi_param(gk_vo_hdmi_param *hdmi_param)
{
    hdmi_param->csc.csc_matrix = GK_VO_CSC_MATRIX_BT601FULL_TO_BT601FULL;
    hdmi_param->csc.luma = VO_CSC_DEF_VAL;
    hdmi_param->csc.contrast = VO_CSC_DEF_VAL;
    hdmi_param->csc.hue = VO_CSC_DEF_VAL;
    hdmi_param->csc.saturation = VO_CSC_DEF_VAL;
    hdmi_param->csc.ex_csc_en = GK_FALSE;
}
#endif
#if VO_DESC("vga param")
gk_void vo_init_vga_param(gk_vo_vga_param *vga_param)
{
    vga_param->csc.csc_matrix = GK_VO_CSC_MATRIX_BT601FULL_TO_RGBFULL;
    vga_param->csc.luma = VO_CSC_DEF_VAL;
    vga_param->csc.contrast = VO_CSC_DEF_VAL;
    vga_param->csc.hue = VO_CSC_DEF_VAL;
    vga_param->csc.saturation = VO_CSC_DEF_VAL;
    vga_param->csc.ex_csc_en = GK_FALSE;
    vga_param->gain = vo_drv_get_vga_gain_def();
    vga_param->sharpen_strength = VO_VGA_SHARPEN_STRENGTH_DEF;
}
#endif

#if VO_DESC("rgb param")
gk_void vo_init_rgb_param(gk_vo_rgb_param *rgb_param)
{
    rgb_param->csc.csc_matrix = GK_VO_CSC_MATRIX_BT601FULL_TO_RGBFULL;
    rgb_param->csc.luma = VO_CSC_DEF_VAL;
    rgb_param->csc.contrast = VO_CSC_DEF_VAL;
    rgb_param->csc.hue = VO_CSC_DEF_VAL;
    rgb_param->csc.saturation = VO_CSC_DEF_VAL;
    rgb_param->csc.ex_csc_en = GK_FALSE;
    rgb_param->rgb_inverted_en = GK_FALSE;
    rgb_param->bit_inverted_en = GK_FALSE;
}
#endif

#if VO_DESC("bt param")
gk_void vo_init_bt_param(gk_vo_bt_param *bt_param)
{
    bt_param->yc_inverted_en = GK_FALSE;
    bt_param->bit_inverted_en = GK_FALSE;
}
#endif
