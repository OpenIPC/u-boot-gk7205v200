/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */


#ifndef __DRV_VO_H__
#define __DRV_VO_H__

#include "drv_vo_comm.h"
#include "drv_vo_dev.h"
#include "drv_vo_layer.h"
#include "drv_vo_video.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */
#if VO_DESC("UBOOT_VO")

#define VO_ADDR_ALIGN        16 /* bytes */
#define VO_STRIDE_ALIGN      16

#define VO_SD_VTTH_WATERLINE 100
#define VO_HD_VTTH_WATERLINE 240
#define VO_SD_VTTH_WATERLINE2  2
#define VO_HD_VTTH_WATERLINE2  2

/* PLL FREF 24MHz */
#define VO_PLL_FREF 24

/* PLL frac precision */
#define VO_PLL_FRAC_PREC (1 << 24)
#define VO_PLL_FOUTCVO_MIN 800
#define VO_PLL_FOUTCVO_MAX 3200
#define VO_PLL_MAX_CLK_594000 594000
#define VO_PLL_MAX_CLK_297000 297000
#define VO_PLL_MAX_CLK_148500 148500

/* LCDMCLKDIV max ((75 / 1188) * 2^27 )(max 75Mhz) */
#define VO_LCDMCLK_DIV_MAX 8473341

#define VO_SD_SRC_CLK 54000
#define VO_CLK_MHZ_UNIT 1000

/* vou mixer prio id */
typedef enum {
    VOU_MIX_PRIO0 = 0,
    VOU_MIX_PRIO1,
    VOU_MIX_PRIO2,
    VOU_MIX_PRIO3,
    VOU_MIX_PRIO4,
    VOU_MIX_BUTT
} vou_mix_prio;

typedef enum {
    VO_CSC_DEF_VAL = 50,
    VO_CSC_LUMA_MAX = 100,
    VO_CSC_LUMA_MIN = 0,

    VO_CSC_CONT_MAX = 100,
    VO_CSC_CONT_MIN = 0,

    VO_CSC_HUE_MAX = 100,
    VO_CSC_HUE_MIN = 0,

    VO_CSC_SAT_MAX = 100,
    VO_CSC_SAT_MIN = 0,
} vo_csc_range;

#define VO_VGA_SHARPEN_STRENGTH_MIN  0
#define VO_VGA_SHARPEN_STRENGTH_MAX  0xFF
#define VO_VGA_SHARPEN_STRENGTH_DEF   128
#define VO_VGA_GAIN_MAX                 0x3F
#define VO_CVBS_DAC_GC_DEF              0x3C

#define VO_DAC_CABLE_CTR_DEF            3

typedef struct {
    vo_dev_type dev_type; /* dev type */
    gk_bool support_wbc; /* WBC support or not */
} vo_dev_capability;

#define VO_RESO_MIN_WDT      32 /* minimal width of display resolution */
#define VO_RESO_MIN_HGT      32 /* minimal height of display resolution */

/* desc : device context, which contains device public attribute. */
typedef struct {
    gk_bool vo_enable;
    gk_bool config;
    gk_vo_pub_attr vou_attr;

    gk_u32 layer_num;

    gk_u32 max_width;
    gk_u32 max_height;
    gk_u32 vtth;
    gk_u32 vtth2;
} vo_drv_dev;

typedef struct {
    gk_bool video_enable;
    gk_bool video_config;

    gk_s32 bind_dev;
    gk_vo_video_layer_attr vo_video_attr;
    gk_vo_csc csc;
    csc_coef_param csc_param;
} vo_drv_layer;

#if VO_DESC("ctx")
vo_drv_dev *vo_drv_get_dev_ctx(gk_vo_dev vo_dev);
vo_drv_layer *vo_drv_get_layer_ctx(gk_vo_layer layer);
#endif

#if VO_DESC("pub")
gk_void vo_drv_set_clk_reverse(gk_vo_dev dev, gk_bool reverse);

gk_void vou_drv_def_layer_bind_dev(gk_void);
gk_void vo_drv_default_setting(gk_void);
#endif

#endif /* #if VO_DESC("UBOOT_VO") */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif /* end of __DRV_VO_H__ */
