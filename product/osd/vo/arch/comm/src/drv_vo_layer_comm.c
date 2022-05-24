/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#include "drv_vo_layer_comm.h"
#include "drv_vo.h"
#include "hal_vo.h"
#include "drv_vo_coef_org_comm.h"
#include "vo.h"
#include "math.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

#if VO_DESC("UBOOT_VO")
#if VO_DESC("layer csc")

gk_s32 vo_drv_get_csc_matrix(gk_vo_csc_matrix csc_matrix, const csc_coef **csc_tmp)
{
    *csc_tmp = vo_get_csc_coef(csc_matrix);
    if (*csc_tmp == GK_NULL) {
        return GK_ERR_VO_NULL_PTR;
    }
    return GK_SUCCESS;
}

gk_void vo_drv_calculate_yuv2rgb(hal_csc_value *csc_value, const csc_coef *csc_tmp, csc_coef *coef)
{
    gk_s32 luma;
    gk_s32 contrast;
    gk_s32 hue;
    gk_s32 satu;
    const gk_s32 csc_value_times = 100;
    const gk_s32 table_times = 1000;
    gk_s32 square_cv_times = csc_value_times * csc_value_times;
    const int *cos_table = vo_get_cos_table();
    const int *sin_table = vo_get_sin_table();

    luma = csc_value->luma;
    contrast = csc_value->cont;
    hue = csc_value->hue;
    satu = csc_value->satu;

    coef->csc_coef00 = (contrast * csc_tmp->csc_coef00) / csc_value_times;
    coef->csc_coef01 = (contrast * satu * ((csc_tmp->csc_coef01 * cos_table[hue] - csc_tmp->csc_coef02 *
                                            sin_table[hue]) / table_times)) / square_cv_times;
    coef->csc_coef02 = (contrast * satu * ((csc_tmp->csc_coef01 * sin_table[hue] + csc_tmp->csc_coef02 *
                                            cos_table[hue]) / table_times)) / square_cv_times;
    coef->csc_coef10 = (contrast * csc_tmp->csc_coef10) / csc_value_times;
    coef->csc_coef11 = (contrast * satu * ((csc_tmp->csc_coef11 * cos_table[hue] - csc_tmp->csc_coef12 *
                                            sin_table[hue]) / table_times)) / square_cv_times;
    coef->csc_coef12 = (contrast * satu * ((csc_tmp->csc_coef11 * sin_table[hue] + csc_tmp->csc_coef12 *
                                            cos_table[hue]) / table_times)) / square_cv_times;
    coef->csc_coef20 = (contrast * csc_tmp->csc_coef20) / csc_value_times;
    coef->csc_coef21 = (contrast * satu * ((csc_tmp->csc_coef21 * cos_table[hue] - csc_tmp->csc_coef22 *
                                            sin_table[hue]) / table_times)) / square_cv_times;
    coef->csc_coef22 = (contrast * satu * ((csc_tmp->csc_coef21 * sin_table[hue] + csc_tmp->csc_coef22 *
                                            cos_table[hue]) / table_times)) / square_cv_times;
    coef->csc_in_dc0 += (contrast != 0) ? (luma * 100 / contrast) : luma * 100;  /* 100 */
}

gk_void vo_drv_calculate_rgb2yuv(hal_csc_value *csc_value, const csc_coef *csc_tmp, csc_coef *coef)
{
    gk_s32 luma;
    gk_s32 contrast;
    gk_s32 hue;
    gk_s32 satu;
    const gk_s32 csc_value_times = 100;
    const gk_s32 table_times = 1000;
    gk_s32 square_cv_times = csc_value_times * csc_value_times;
    const int *cos_table = vo_get_cos_table();
    const int *sin_table = vo_get_sin_table();

    luma = csc_value->luma;
    contrast = csc_value->cont;
    hue = csc_value->hue;
    satu = csc_value->satu;

    coef->csc_coef00 = (contrast * csc_tmp->csc_coef00) / csc_value_times;
    coef->csc_coef01 = (contrast * csc_tmp->csc_coef01) / csc_value_times;
    coef->csc_coef02 = (contrast * csc_tmp->csc_coef02) / csc_value_times;
    coef->csc_coef10 = (contrast * satu * ((csc_tmp->csc_coef10 * cos_table[hue] + csc_tmp->csc_coef20 *
                                            sin_table[hue]) / table_times)) / square_cv_times;
    coef->csc_coef11 = (contrast * satu * ((csc_tmp->csc_coef11 * cos_table[hue] + csc_tmp->csc_coef21 *
                                            sin_table[hue]) / table_times)) / square_cv_times;
    coef->csc_coef12 = (contrast * satu * ((csc_tmp->csc_coef12 * cos_table[hue] + csc_tmp->csc_coef22 *
                                            sin_table[hue]) / table_times)) / square_cv_times;
    coef->csc_coef20 = (contrast * satu * ((csc_tmp->csc_coef20 * cos_table[hue] - csc_tmp->csc_coef10 *
                                            sin_table[hue]) / table_times)) / square_cv_times;
    coef->csc_coef21 = (contrast * satu * ((csc_tmp->csc_coef21 * cos_table[hue] - csc_tmp->csc_coef11 *
                                            sin_table[hue]) / table_times)) / square_cv_times;
    coef->csc_coef22 = (contrast * satu * ((csc_tmp->csc_coef22 * cos_table[hue] - csc_tmp->csc_coef12 *
                                            sin_table[hue]) / table_times)) / square_cv_times;
    coef->csc_out_dc0 += luma;
}

gk_void vou_drv_calc_csc_matrix(const gk_vo_csc *csc, gk_vo_csc_matrix csc_matrix, csc_coef *coef)
{
    gk_s32 ret;
    const csc_coef *csc_tmp = GK_NULL;
    hal_csc_value csc_value;

    if (csc->ex_csc_en == GK_FALSE) {
        csc_value.luma = (gk_s32)csc->luma * 64 / 100 - 32; /* 64: -32~32 100  */
    } else {
        csc_value.luma = (gk_s32)csc->luma * 256 / 100 - 128; /* 256: -128~128 128 100  */
    }

    csc_value.cont = ((gk_s32)csc->contrast - 50) * 2 + 100; /* 50 2 100 */
    csc_value.hue = (gk_s32)csc->hue * 60 / 100;             /* 60 100  */
    csc_value.satu = ((gk_s32)csc->saturation - 50) * 2 + 100; /* 50 2 100  */

    ret = vo_drv_get_csc_matrix(csc_matrix, &csc_tmp);
    if (ret != GK_SUCCESS) {
        return;
    }

    coef->csc_in_dc0 = csc_tmp->csc_in_dc0;
    coef->csc_in_dc1 = csc_tmp->csc_in_dc1;
    coef->csc_in_dc2 = csc_tmp->csc_in_dc2;
    coef->csc_out_dc0 = csc_tmp->csc_out_dc0;
    coef->csc_out_dc1 = csc_tmp->csc_out_dc1;
    coef->csc_out_dc2 = csc_tmp->csc_out_dc2;

    if ((csc_matrix >= GK_VO_CSC_MATRIX_BT601LIMIT_TO_RGBFULL) &&
        (csc_matrix <= GK_VO_CSC_MATRIX_BT709FULL_TO_RGBLIMIT)) {
        vo_drv_calculate_yuv2rgb(&csc_value, csc_tmp, coef);
    } else {
        vo_drv_calculate_rgb2yuv(&csc_value, csc_tmp, coef);
    }
}

gk_void vo_drv_clip_layer_csc_coef(csc_coef *coef)
{
    gk_s32 min_coef = 0;
    gk_s32 max_coef = 0;

    vo_drv_get_layer_csc_coef_range(&min_coef, &max_coef);
    coef->csc_coef00 = CLIP3(coef->csc_coef00, min_coef, max_coef);
    coef->csc_coef01 = CLIP3(coef->csc_coef01, min_coef, max_coef);
    coef->csc_coef02 = CLIP3(coef->csc_coef02, min_coef, max_coef);

    coef->csc_coef10 = CLIP3(coef->csc_coef10, min_coef, max_coef);
    coef->csc_coef11 = CLIP3(coef->csc_coef11, min_coef, max_coef);
    coef->csc_coef12 = CLIP3(coef->csc_coef12, min_coef, max_coef);

    coef->csc_coef20 = CLIP3(coef->csc_coef20, min_coef, max_coef);
    coef->csc_coef21 = CLIP3(coef->csc_coef21, min_coef, max_coef);
    coef->csc_coef22 = CLIP3(coef->csc_coef22, min_coef, max_coef);
}

gk_void vou_drv_layer_csc_config(gk_vo_layer layer, gk_vo_csc *csc, csc_coef_param *csc_param)
{
    csc_coef coef;
    vo_drv_layer *drv_layer_ctx = GK_NULL;

    vou_drv_calc_csc_matrix(csc, csc->csc_matrix, &coef);

    drv_layer_ctx = vo_drv_get_layer_ctx(layer);
    coef.new_csc_clip_max = drv_layer_ctx->csc_param.csc_clip_max;
    coef.new_csc_clip_min = drv_layer_ctx->csc_param.csc_clip_min;
    coef.new_csc_scale2p = drv_layer_ctx->csc_param.csc_scale2p;

    vo_drv_csc_trans_to_register(&coef);

    vo_drv_clip_layer_csc_coef(&coef);
    hal_layer_set_csc_coef(layer, &coef);
}

gk_void vou_drv_get_layer_csc_param(gk_vo_layer layer, csc_coef_param *csc_param)
{
    vo_drv_layer *drv_layer_ctx  = vo_drv_get_layer_ctx(layer);
    memcpy(csc_param, &drv_layer_ctx->csc_param, sizeof(csc_coef_param));
}
#endif

#if VO_DESC("layer cfg")

gk_vo_layer vo_drv_get_hw_layer(gk_vo_layer layer)
{
    return layer;
}

gk_void vou_drv_layer_enable(gk_vo_layer layer, gk_bool enable)
{
    gk_vo_layer hw_layer = vo_drv_get_hw_layer(layer);

    hal_video_hfir_set_ck_gt_en(hw_layer, enable);
    hal_layer_enable_layer(hw_layer, enable);
}

gk_void vou_drv_set_layer_reg_up(gk_vo_layer layer)
{
    gk_vo_layer hw_layer = vo_drv_get_hw_layer(layer);
    hal_layer_set_reg_up(hw_layer);
}

gk_void vou_drv_set_layer_data_fmt(gk_vo_layer layer, vou_layer_pixerl_format data_fmt)
{
    gk_vo_layer hw_layer = vo_drv_get_hw_layer(layer);
    hal_disp_layer hal_layer;
    hal_disp_pixel_format disp_data_fmt;

    hal_layer = vou_drv_convert_layer(hw_layer);
    disp_data_fmt = vou_drv_convert_data_format(data_fmt);
    hal_layer_set_layer_data_fmt(hal_layer, disp_data_fmt);
}
#endif
#endif /* #if VO_DESC("UBOOT_VO") */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */
