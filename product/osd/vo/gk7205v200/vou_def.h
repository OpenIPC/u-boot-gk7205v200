/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __VOU_DEF_H__
#define __VOU_DEF_H__

#ifdef __cplusplus
extern "C" {
#endif /* end of #ifdef __cplusplus */

#define VO_BASE_ADDR      0x11280000
#define VO_VHD_BASE_ADDR  0x800
#define VO_GFX_BASE_ADDR  0x6000
#define VO_HC_BASE_ADDR   0x9800
#define VO_WBC0_BASE_ADDR 0xac00
#define VO_WBC1_BASE_ADDR 0xa400
#define VO_DHD_BASE_ADDR  0xc000
#define VO_DSD_BASE_ADDR  0xc800
#define VHD_REG_LEN       0x1000
#define VSD_REG_LEN       0x1000
#define GFX_REG_LEN       0x800
#define HC_REG_LEN        0x800
#define WBC_REG_LEN       0x400
#define DHD_REG_LEN       0x400
#define DSD_REG_LEN       0x400
#define INTF_REG_LEN      0x100
#define INTF_REGS_LEN     0x100
#define VHD_REGS_LEN      0x1000 /* len of V0's regs */
#define VSD_REGS_LEN      0x1000
#define GFX_REGS_LEN      0x800
#define WBC_REGS_LEN      0x400
#define DHD_REGS_LEN      0x1000
#define DSD_REGS_LEN      0x400
#define INTF_REGS_LEN     0x100
#define VID_REGS_LEN      0x200 /* len of VID regs */
#define GRF_REGS_LEN      0x200 /* len of GFX regs */

#define rgb_r(c) (((c) & 0xff0000) >> 16)
#define rgb_g(c) (((c) & 0xff00) >> 8)
#define rgb_b(c) ((c) & 0xff)

#define ZME_HPREC (1 << 20)
#define ZME_VPREC (1 << 12)

#define MAX_OFFSET 3
#define MIN_OFFSET (-1)

#define CHANNEL_DHD_START 0
#define CHANNEL_DHD_END   1
#define CHANNEL_DSD_START 2
#define CHANNEL_DSD_END   2

#define MULTI_AREA_CFG_LEN (4 * 8)
#define MULTI_AREA_1RGN    1
#define MULTI_AREA_4RGN    4
#define MULTI_AREA_32RGN   32
#define MULTI_AREA_64RGN   64

typedef enum {
    HAL_DISP_CHANNEL_DHD0 = 0,
    HAL_DISP_CHANNEL_DHD1 = 1,
    HAL_DISP_CHANNEL_WBC = 2,
    HAL_DISP_CHANNEL_DSD0 = 3,
    HAL_DISP_CHANNEL_BUTT
} hal_disp_outputchannel;

typedef enum {
    HAL_DISP_LAYER_VHD0 = 0,
    HAL_DISP_LAYER_VHD1 = 1,
    HAL_DISP_LAYER_VHD2 = 2,
    HAL_DISP_LAYER_VSD0 = 3,
    HAL_DISP_LAYER_GFX0 = 4, /* fb0 */
    HAL_DISP_LAYER_GFX1 = 5, /* fb2 */
    HAL_DISP_LAYER_HC0 = 6,  /* G2 is HC0 fb3 */
    HAL_DISP_LAYER_HC1 = 7,  /* G3 is HC1 fb4 */
    HAL_DISP_LAYER_GFX4 = 8, /* fb1 */
    HAL_DISP_LAYER_WBC_G0 = 9,
    HAL_DISP_LAYER_WBC_G4 = 10,
    HAL_DISP_LAYER_WBC_D = 11,
    HAL_DISP_LAYER_TT = 12,
    HAL_DISP_LAYER_WBC = 13,

    HAL_DISP_LAYER_BUTT,
    HAL_DISP_INVALID_LAYER = -1
} hal_disp_layer;

#define HAL_DISP_LAYER_GFX2 HAL_DISP_LAYER_HC0
#define HAL_DISP_LAYER_GFX3 HAL_DISP_LAYER_HC1

#define LAYER_VHD_START HAL_DISP_LAYER_VHD0
#define LAYER_VHD_END   HAL_DISP_LAYER_VHD0
#define LAYER_VSD_START HAL_DISP_LAYER_VSD0
#define LAYER_VSD_END   HAL_DISP_LAYER_VSD0
#define LAYER_GFX_START HAL_DISP_LAYER_GFX0
#define LAYER_GFX_END   HAL_DISP_LAYER_GFX0
#define LAYER_WBC_START HAL_DISP_LAYER_WBC_D
#define LAYER_WBC_END   HAL_DISP_LAYER_WBC_D

typedef enum {
    HAL_DISP_INTERLACE = 0,
    HAL_DISP_PROGRESSIVE = 1,
    HAL_DISP_TOP,
    HAL_DISP_BOTTOM,

    HAL_DISP_DATARMODE_BUTT
} hal_disp_datarmode;

typedef enum {
    HAL_INPUTFMT_Y_CB_CR_SEMIPLANAR_400 = 0x1,
    HAL_INPUTFMT_Y_CB_CR_SEMIPLANAR_411_1X4 = 0x2,
    HAL_INPUTFMT_Y_CB_CR_SEMIPLANAR_420 = 0x3,
    HAL_INPUTFMT_Y_CB_CR_SEMIPLANAR_422 = 0x4,
    HAL_INPUTFMT_Y_CB_CR_SEMIPLANAR_422_1X2 = 0x4,
    HAL_INPUTFMT_Y_CB_CR_SEMIPLANAR_444 = 0x5,
    HAL_INPUTFMT_Y_CB_CR_SEMIPLANAR_411_4X1 = 0x6,
    HAL_INPUTFMT_Y_CB_CR_SEMIPLANAR_422_2X1 = 0x7,

    HAL_INPUTFMT_CB_Y_CR_Y_PACKAGE_422 = 0x9,
    HAL_INPUTFMT_Y_CB_Y_CR_PACKAGE_422 = 0xa,
    HAL_INPUTFMT_Y_CR_Y_CB_PACKAGE_422 = 0xb,
    HAL_INPUTFMT_Y_CB_CR_PACKAGE_444 = 0x1000,

    HAL_INPUTFMT_CLUT_1BPP = 0x00,
    HAL_INPUTFMT_CLUT_2BPP = 0x10,
    HAL_INPUTFMT_CLUT_4BPP = 0x20,
    HAL_INPUTFMT_CLUT_8BPP = 0x30,
    HAL_INPUTFMT_ACLUT_44 = 0x38,

    HAL_INPUTFMT_RGB_444 = 0x40,
    HAL_INPUTFMT_RGB_555 = 0x41,
    HAL_INPUTFMT_RGB_565 = 0x42,
    HAL_INPUTFMT_CB_Y_CR_Y_PACKAGE_422_GRC = 0x43,
    HAL_INPUTFMT_Y_CB_Y_CR_PACKAGE_422_GRC = 0x44,
    HAL_INPUTFMT_Y_CR_Y_CB_PACKAGE_422_GRC = 0x45,
    HAL_INPUTFMT_ACLUT_88 = 0x46,
    HAL_INPUTFMT_ARGB_4444 = 0x48,
    HAL_INPUTFMT_ARGB_1555 = 0x49,

    HAL_INPUTFMT_RGB_888 = 0x50,
    HAL_INPUTFMT_Y_CB_CR_888 = 0x51,
    HAL_INPUTFMT_ARGB_8565 = 0x5a,
    HAL_INPUTFMT_ARGB_6666 = 0x5b,

    HAL_INPUTFMT_KRGB_888 = 0x60,
    HAL_INPUTFMT_ARGB_8888 = 0x68,
    HAL_INPUTFMT_A_Y_Cb_CR_8888 = 0x69,

    HAL_INPUTFMT_RGBA_4444 = 0xc8,
    HAL_INPUTFMT_RGBA_5551 = 0xc9,

    HAL_INPUTFMT_RGBA_6666 = 0xd8,
    HAL_INPUTFMT_RGBA_5658 = 0xda,

    HAL_INPUTFMT_RGBA_8888 = 0xe8,
    HAL_INPUTFMT_Y_CB_CR_A_8888 = 0xe9,

    HAL_DISP_PIXELFORMAT_BUTT
} hal_disp_pixel_format;

typedef enum {
    HAL_DISP_INTFDATAFMT_SPCBYCRY_422 = 0x0,
    HAL_DISP_INTFDATAFMT_SPYCBYCR_422 = 0x1,
    HAL_DISP_INTFDATAFMT_SPYCRYCB_422 = 0x2,

    HAL_DISP_INTFDATAFMT_ARGB8888 = 0x3,
    HAL_DISP_INTFDATAFMT_YCBCR420 = 0x4,
    HAL_DISP_INTFDATAFMT_YCBCR422 = 0x5,

    HAL_DISP_INTFDATAFMT_BUTT
} hal_disp_intfdatafmt;

typedef enum {
    VDP_PROC_FMT_SP_420 = 0x0,
    VDP_PROC_FMT_SP_422 = 0x1,
    VDP_PROC_FMT_SP_444 = 0x2,
    VDP_PROC_FMT_RGB_888 = 0x3,
    VDP_PROC_FMT_RGB_444 = 0x4,

    VDP_PROC_FMT_BUTT
} vdp_proc_fmt;

typedef enum {
    HAL_CS_UNKNOWN = 0,
    HAL_CS_BT601,
    HAL_CS_BT709,
    HAL_CS_RGB,
    HAL_CS_BUTT
} hal_cs;

typedef enum {
    HAL_CSC_MODE_NONE = 0,

    HAL_CSC_MODE_BT601_TO_BT601,
    HAL_CSC_MODE_BT709_TO_BT709,
    HAL_CSC_MODE_RGB_TO_RGB,

    HAL_CSC_MODE_BT601_TO_BT709,
    HAL_CSC_MODE_BT709_TO_BT601,

    HAL_CSC_MODE_BT601_TO_RGB_PC,
    HAL_CSC_MODE_BT709_TO_RGB_PC,
    HAL_CSC_MODE_BT2020_TO_RGB_PC,
    HAL_CSC_MODE_RGB_TO_BT601_PC,
    HAL_CSC_MODE_RGB_TO_BT709_PC,
    HAL_CSC_MODE_RGB_TO_BT2020_PC,

    HAL_CSC_MODE_BT601_TO_RGB_TV,
    HAL_CSC_MODE_BT709_TO_RGB_TV,
    HAL_CSC_MODE_RGB_TO_BT601_TV,
    HAL_CSC_MODE_RGB_TO_BT709_TV,

    HAL_CSC_MODE_BUTT
} hal_csc_mode;

typedef enum {
    HAL_DISP_BIT_WIDTH_1 = 0x0,
    HAL_DISP_BIT_WIDTH_2 = 0x1,
    HAL_DISP_BIT_WIDTH_3 = 0x2,
    HAL_DISP_BIT_WIDTH_BUTT
} hal_disp_bit_width;

typedef enum {
    HAL_CCD_IMGID_INVALID,
    HAL_CCD_IMGID_1080P60,
    HAL_CCD_IMGID_1080P30_ODD,
    HAL_CCD_IMGID_1080P30_EVEN,
} hal_ccd_imgid;

typedef enum {
    HAL_T_FIRST = 0,
    HAL_B_FIRST = 1,

    HAL_VHD_FOD_BUTT
} hal_vhd_fod;

/* vou coef load mode */
typedef enum {
    HAL_DISP_COEFMODE_HORL = 0,
    HAL_DISP_COEFMODE_HORC,
    HAL_DISP_COEFMODE_VERL,
    HAL_DISP_COEFMODE_VERC,
    HAL_DISP_COEFMODE_LUT,
    HAL_DISP_COEFMODE_GAM,
    HAL_DISP_COEFMODE_ACC,
    HAL_DISP_COEFMODE_ALL
} hal_disp_coefmode;

/* vou zoom mode */
typedef enum {
    HAL_DISP_ZMEMODE_HORL = 0,
    HAL_DISP_ZMEMODE_HORC,
    HAL_DISP_ZMEMODE_VERL,
    HAL_DISP_ZMEMODE_VERC,

    HAL_DISP_ZMEMODE_HOR,
    HAL_DISP_ZMEMODE_VER,
    HAL_DISP_ZMEMODE_ALPHA,
    HAL_DISP_ZMEMODE_ALPHAV,
    HAL_DISP_ZMEMODE_VERT,
    HAL_DISP_ZMEMODE_VERB,

    HAL_DISP_ZMEMODE_ALL,
    HAL_DISP_ZMEMODE_NONL,
    HAL_DISP_ZMEMODE_BUTT
} hal_disp_zmemode;

typedef enum {
    HAL_DISP_ZME_OUTFMT420 = 0,
    HAL_DISP_ZME_OUTFMT422,
    HAL_DISP_ZME_OUTFMT444,
    HAL_DISP_ZME_OUTFMT_BUTT
} hal_disp_zme_outfmt;

/* vou CBM MIXER */
typedef enum {
    HAL_CBMMIX1 = 0,
    HAL_CBMMIX2,
    HAL_CBMMIX3,

    HAL_CBMMIX1_BUTT
} hal_cbmmix;

/* vou graphic layer data extend mode */
typedef enum {
    HAL_GFX_BITEXTEND_1ST = 0,
    HAL_GFX_BITEXTEND_2ND = 0x2,
    HAL_GFX_BITEXTEND_3RD = 0x3,

    HAL_GFX_BITEXTEND_BUTT
} hal_gfx_bitextend;

/* vou acm block id information */
typedef enum {
    HAL_ACMBLK_ID0,
    HAL_ACMBLK_ID1,
    HAL_ACMBLK_ID2,
    HAL_ACMBLK_ID3,

    HAL_ACMBLK_ALL,

    HAL_ACMBLK_BUTT
} hal_acmblk_id;

typedef enum {
    HAL_DAC_SEL_HDDATE = 0x0,
    HAL_DAC_SEL_VGA = 0x01,
} hal_dac_sel;

typedef enum {
    HAL_DISP_SYNC_MODE_TIMING = 0x0,
    HAL_DISP_SYNC_MODE_SIGNAL = 0x1,
    HAL_DISP_SYNC_MODE_BUTT
} hal_disp_sync_mode;

typedef enum {
    HAL_GFX_LINE_TYPE_HYBRID,
    HAL_GFX_LINE_TYPE_VLC,
    HAL_GFX_LINE_TYPE_FLC,

    HAL_GFX_LINE_TYPE_BUTT,
} hal_gfx_linetype;

typedef enum {
    HAL_GFX_LINE_LEN_128,
    HAL_GFX_LINE_LEN_256,
    HAL_GFX_LINE_LEN_512,
    HAL_GFX_LINE_LEN_ALLLINE,

    HAL_GFX_LINE_LEN_BUTT,
} hal_gfx_linelen;

/* RM frame or filed infomation */
typedef enum {
    HAL_IFIRMODE_DISEN = 0,
    HAL_IFIRMODE_COPY,
    HAL_IFIRMODE_DOUBLE,
    HAL_IFIRMODE_6TAPFIR,

    HAL_IFIRMODE_BUTT
} hal_ifirmode;

typedef enum {
    HAL_HFIRMODE_MEDEN = 0,
    HAL_HFIRMODE_COPY,
    HAL_HFIRMODE_DOUBLE,
    HAL_HFIRMODE_6TAPFIR,

    HAL_HFIRMODE_BUTT
} hal_hfirmode;

/* vou mixer prio id */
typedef enum {
    HAL_DISP_MIX_PRIO0 = 0,
    HAL_DISP_MIX_PRIO1,
    HAL_DISP_MIX_PRIO2,
    HAL_DISP_MIX_PRIO3,
    HAL_DISP_MIX_BUTT
} hal_disp_mix_prio;

/* LTI/CTI mode */
typedef enum {
    HAL_DISP_TIMODE_LUM = 0,
    HAL_DISP_TIMODE_CHM,

    HAL_DISP_TIMODE_ALL,
    HAL_DISP_TIMODE_NON,
    HAL_DISP_TIMODE_BUTT,
} hal_disp_timode;

typedef struct {
    gk_u32 synm;
    gk_u32 iop;
    gk_u8 intfb;

    gk_u16 vact;
    gk_u16 vbb;
    gk_u16 vfb;

    gk_u16 hact;
    gk_u16 hbb;
    gk_u16 hfb;
    gk_u16 hmid;

    gk_u16 bvact;
    gk_u16 bvbb;
    gk_u16 bvfb;

    gk_u16 hpw;
    gk_u16 vpw;

    gk_u32 idv;
    gk_u32 ihs;
    gk_u32 ivs;
} hal_disp_syncinfo;

typedef struct {
    gk_u32 dither_sed_y0;
    gk_u32 dither_sed_u0;
    gk_u32 dither_sed_v0;
    gk_u32 dither_sed_w0;

    gk_u32 dither_sed_y1;
    gk_u32 dither_sed_u1;
    gk_u32 dither_sed_v1;
    gk_u32 dither_sed_w1;

    gk_u32 dither_sed_y2;
    gk_u32 dither_sed_u2;
    gk_u32 dither_sed_v2;
    gk_u32 dither_sed_w2;

    gk_u32 dither_sed_y3;
    gk_u32 dither_sed_u3;
    gk_u32 dither_sed_v3;
    gk_u32 dither_sed_w3;
} hal_disp_dihter_sed;

typedef enum {
    /* the numbers are to define the enum, not magic numbers */
    HAL_DISP_INTF_CVBS = (0x01L << 0),
    HAL_DISP_INTF_HDDATE = (0x01L << 1),
    HAL_DISP_INTF_VGA = (0x01L << 2),
    HAL_DISP_INTF_BT656 = (0x01L << 3),
    HAL_DISP_INTF_BT1120 = (0x01L << 4),
    HAL_DISP_INTF_HDMI = (0x01L << 5),
    HAL_DISP_INTF_LCD = (0x01L << 6),
    HAL_DISP_INTF_DATE = (0x01L << 7),
    HAL_DISP_INTF_LCD_6BIT = (0x01L << 9),
    HAL_DISP_INTF_LCD_8BIT = (0x01L << 10),
    HAL_DISP_INTF_LCD_16BIT = (0x01L << 11),
    HAL_DISP_INTF_LCD_18BIT = (0x01L << 12),
    HAL_DISP_INTF_LCD_24BIT = (0x01L << 13),
    HAL_DISP_INTF_MIPI = (0x01L << 14),
    HAL_DISP_INTF_BUTT = (0x01L << 15),
} hal_disp_intf;

typedef struct {
    gk_u32 f_inv;
    gk_u32 vs_inv;
    gk_u32 hs_inv;
    gk_u32 dv_inv;
} hal_disp_syncinv;

typedef enum {
    HAL_MULTICHN_EN_1P1C = 0x1,
    HAL_MULTICHN_EN_2P1C = 0x2,
} hal_multi_chn;

/* vou background color */
typedef struct {
    gk_u16 bkg_a;
    gk_u16 bkg_y;
    gk_u16 bkg_cb;
    gk_u16 bkg_cr;
} hal_disp_bkcolor;

typedef struct {
    gk_u16 clip_low_y;
    gk_u16 clip_low_cb;
    gk_u16 clip_low_cr;

    gk_u16 clip_high_y;
    gk_u16 clip_high_cb;
    gk_u16 clip_high_cr;
} hal_disp_clip;

typedef struct {
    gk_s32 x;
    gk_s32 y;
    gk_u32 width;
    gk_u32 height;
} gk_rect;

typedef struct {
    gk_u32 fbdiv;
    gk_u32 frac;
    gk_u32 refdiv;
    gk_u32 postdiv1;
    gk_u32 postdiv2;
} vo_user_intfsync_pll;

typedef enum {
    VO_CLK_SOURCE_PLL = 0,
    VO_CLK_SOURCE_LCDMCLK,
    VO_CLK_SOURCE_BUTT
} vo_clk_source;

typedef struct {
    vo_clk_source clk_source;
    union {
        vo_user_intfsync_pll user_sync_pll;
        gk_u32 lcd_m_clk_div;
    };
} vo_user_intfsync_attr;

typedef struct {
    vo_user_intfsync_attr user_intf_sync_attr;
    gk_u32 pre_div;
    gk_u32 dev_div;
    gk_bool clk_reverse;
} vo_user_intfsync_info;

#ifdef __cplusplus
}
#endif /* end of #ifdef __cplusplus */
#endif /* end of __VOU_DEF_H__ */

