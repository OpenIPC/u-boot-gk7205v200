/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __VOU_COEF_H__
#define __VOU_COEF_H__

#include "type.h"

#ifdef __cplusplus
extern "C" {
#endif /* end of #ifdef __cplusplus */

#define VOU_GAMM_COEF_ROW 32
#define VOU_GAMM_COEF_COL 16

#define VOU_ZOOM_COEF_ITEM   18
#define VOU_ZOOM_COEF_MAXTAP 8

#define VOU_ACC_COEF_NUM 256

typedef enum {
    VOU_ZOOM_COEF_1 = 0,
    VOU_ZOOM_COEF_EQU_1,
    VOU_ZOOM_COEF_075,
    VOU_ZOOM_COEF_05,
    VOU_ZOOM_COEF_033,
    VOU_ZOOM_COEF_025,
    VOU_ZOOM_COEF_0,
    VOU_ZOOM_COEF_BUTT
} vou_zoom_coef;

typedef enum {
    VOU_ZOOM_TAP_8LH = 0,
    VOU_ZOOM_TAP_4CH,
    VOU_ZOOM_TAP_6LV,
    VOU_ZOOM_TAP_4LV,
    VOU_ZOOM_TAP_4CV,
    VOU_ZOOM_TAP_BUTT
} vou_zoom_tap;

typedef struct {
    gk_s32 bits_0 : 10;
    gk_s32 bits_1 : 10;
    gk_s32 bits_2 : 10;
    gk_s32 bits_32 : 2;
    gk_s32 bits_38 : 8;
    gk_s32 bits_4 : 10;
    gk_s32 bits_5 : 10;
    gk_s32 bits_64 : 4;
    gk_s32 bits_66 : 6;
    gk_s32 bits_7 : 10;
    gk_s32 bits_8 : 10;
    gk_s32 bits_96 : 6;
    gk_s32 bits_94 : 4;
    gk_s32 bits_10 : 10;
    gk_s32 bits_11 : 10;
    gk_s32 bits_12 : 8;
} vo_zoom_bit;

typedef struct {
    gk_u32 size;
    vo_zoom_bit bit[12]; /* 12 bit array */
} vo_zoom_bitarray;

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
    gk_s32 csc_scale2p;
    gk_s32 csc_clip_min;
    gk_s32 csc_clip_max;
} csc_coef_param;

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
} vo_csc_coef;

/* vga sharpen HF coefficient */
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
    gk_u32 vga_hsp_glb_unferth;
} hsp_coef;

#define VO_CSC_DC_MAGIC 0x07c30180

#ifdef __cplusplus
}
#endif /* end of #ifdef __cplusplus */

#endif /* end of #ifndef __VOU_COEF_H__ */
