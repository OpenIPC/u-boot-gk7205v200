/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */


#ifndef __DRV_VO_COMM_COEF_H__
#define __DRV_VO_COMM_COEF_H__

#include "type.h"
#include "inner_vo.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

#if VO_DESC("UBOOT_VO")

typedef struct {
    gk_s32 csc_coef00;
    gk_s32 csc_coef01;
    gk_s32 csc_coef02;

    gk_s32 csc_coef10;
    gk_s32 csc_coef11;
    gk_s32 csc_coef12;

    gk_s32 csc_coef20;
    gk_s32 csc_coef21;
    gk_s32 csc_coef22;
} vdp_csc_coef;

typedef struct {
    gk_s32 csc_in_dc0;
    gk_s32 csc_in_dc1;
    gk_s32 csc_in_dc2;

    gk_s32 csc_out_dc0;
    gk_s32 csc_out_dc1;
    gk_s32 csc_out_dc2;
} vdp_csc_dc_coef;

typedef struct {
    /* for old version csc */
    gk_s32 csc_coef00;
    gk_s32 csc_coef01;
    gk_s32 csc_coef02;

    gk_s32 csc_coef10;
    gk_s32 csc_coef11;
    gk_s32 csc_coef12;

    gk_s32 csc_coef20;
    gk_s32 csc_coef21;
    gk_s32 csc_coef22;

    gk_s32 csc_in_dc0;
    gk_s32 csc_in_dc1;
    gk_s32 csc_in_dc2;

    gk_s32 csc_out_dc0;
    gk_s32 csc_out_dc1;
    gk_s32 csc_out_dc2;

    gk_s32 new_csc_scale2p;
    gk_s32 new_csc_clip_min;
    gk_s32 new_csc_clip_max;
} csc_coef;

typedef struct {
    gk_s32 csc_scale2p;
    gk_s32 csc_clip_min;
    gk_s32 csc_clip_max;
} csc_coef_param;

typedef struct {
    gk_s32 vga_hsp_tmp0;
    gk_s32 vga_hsp_tmp1;
    gk_s32 vga_hsp_tmp2;
    gk_s32 vga_hsp_tmp3;
    gk_u32 vga_hsp_coring;
    gk_s32 vga_hsp_gainneg;
    gk_s32 vga_hsp_gainpos;
    gk_s32 vga_hsp_adpshooten;
    gk_u32 vga_hsp_winsize;
    gk_u32 vga_hsp_mixratio;
    gk_u32 vga_hsp_underth;
    gk_u32 vga_hsp_overth;
} hsp_hf_coef;

typedef struct {
    gk_u32 vga_hsp_hf_shootdiv;
    gk_u32 vga_hsp_lti_ratio;
    gk_u32 vga_hsp_ldti_gain;
    gk_u32 vga_hsp_cdti_gain;
    gk_u32 vga_hsp_peak_ratio;
    gk_u32 vga_hsp_glb_overth;
    gk_u32 vga_hsp_glb_underth;
} hsp_coef;

/* CVFIR VCOEF */
typedef struct {
    gk_s32 vccoef00;
    gk_s32 vccoef01;
    gk_s32 vccoef02;
    gk_s32 vccoef03;
    gk_s32 vccoef10;
    gk_s32 vccoef11;
    gk_s32 vccoef12;
    gk_s32 vccoef13;
} cvfir_coef;

/* HFIR VCOEF */
typedef struct {
    gk_s32 coef0;
    gk_s32 coef1;
    gk_s32 coef2;
    gk_s32 coef3;
    gk_s32 coef4;
    gk_s32 coef5;
    gk_s32 coef6;
    gk_s32 coef7;
} hfir_coef;
#endif /* #if VO_DESC("UBOOT_VO") */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif /* end of #ifndef __DRV_VO_COMM_COEF_H__ */
