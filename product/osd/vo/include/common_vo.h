/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */


#ifndef __GK_COMMON_VO_H__
#define __GK_COMMON_VO_H__

#include "type.h"
#include "errno.h"
#include "common_video.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

/* Vo define error code */
#define GK_ERR_VO_INVALID_DEV_ID      GK_DEFINE_ERR(GK_ID_VO, GK_ERR_LEVEL_ERROR, GK_ERR_INVALID_DEV_ID)
#define GK_ERR_VO_INVALID_CHN_ID      GK_DEFINE_ERR(GK_ID_VO, GK_ERR_LEVEL_ERROR, GK_ERR_INVALID_CHN_ID)
#define GK_ERR_VO_INVALID_LAYER_ID    GK_DEFINE_ERR(GK_ID_VO, GK_ERR_LEVEL_ERROR, GK_ERR_INVALID_LAYER_ID)
#define GK_ERR_VO_ILLEGAL_PARAM       GK_DEFINE_ERR(GK_ID_VO, GK_ERR_LEVEL_ERROR, GK_ERR_ILLEGAL_PARAM)
#define GK_ERR_VO_NULL_PTR            GK_DEFINE_ERR(GK_ID_VO, GK_ERR_LEVEL_ERROR, GK_ERR_NULL_PTR)
#define GK_ERR_VO_NOT_CFG             GK_DEFINE_ERR(GK_ID_VO, GK_ERR_LEVEL_ERROR, GK_ERR_NOT_CFG)
#define GK_ERR_VO_NOT_SUPPORT         GK_DEFINE_ERR(GK_ID_VO, GK_ERR_LEVEL_ERROR, GK_ERR_NOT_SUPPORT)
#define GK_ERR_VO_NOT_PERM            GK_DEFINE_ERR(GK_ID_VO, GK_ERR_LEVEL_ERROR, GK_ERR_NOT_PERM)
#define GK_ERR_VO_NOT_ENABLE          GK_DEFINE_ERR(GK_ID_VO, GK_ERR_LEVEL_ERROR, GK_ERR_NOT_ENABLE)
#define GK_ERR_VO_NOT_DISABLE         GK_DEFINE_ERR(GK_ID_VO, GK_ERR_LEVEL_ERROR, GK_ERR_NOT_DISABLE)
#define GK_ERR_VO_NO_MEM              GK_DEFINE_ERR(GK_ID_VO, GK_ERR_LEVEL_ERROR, GK_ERR_NO_MEM)
#define GK_ERR_VO_NOT_READY           GK_DEFINE_ERR(GK_ID_VO, GK_ERR_LEVEL_ERROR, GK_ERR_NOT_READY)
#define GK_ERR_VO_TIMEOUT             GK_DEFINE_ERR(GK_ID_VO, GK_ERR_LEVEL_ERROR, GK_ERR_TIMEOUT)
#define GK_ERR_VO_BUSY                GK_DEFINE_ERR(GK_ID_VO, GK_ERR_LEVEL_ERROR, GK_ERR_BUSY)
#define GK_ERR_VO_NOT_BINDED          GK_DEFINE_ERR(GK_ID_VO, GK_ERR_LEVEL_ERROR, GK_ERR_NOT_BINDED)
#define GK_ERR_VO_BINDED              GK_DEFINE_ERR(GK_ID_VO, GK_ERR_LEVEL_ERROR, GK_ERR_BINDED)

/* VO video output interface type */
#define GK_VO_INTF_CVBS       (0x01L << 0)
#define GK_VO_INTF_VGA        (0x01L << 1)
#define GK_VO_INTF_BT656      (0x01L << 2)
#define GK_VO_INTF_BT1120     (0x01L << 3)
#define GK_VO_INTF_HDMI       (0x01L << 4)
#define GK_VO_INTF_RGB_6BIT   (0x01L << 5)
#define GK_VO_INTF_RGB_8BIT   (0x01L << 6)
#define GK_VO_INTF_RGB_16BIT  (0x01L << 7)
#define GK_VO_INTF_RGB_18BIT  (0x01L << 8)
#define GK_VO_INTF_RGB_24BIT  (0x01L << 9)
#define GK_VO_INTF_MIPI       (0x01L << 10)
#define GK_VO_INTF_MIPI_SLAVE (0x01L << 11)

typedef gk_u32 gk_vo_intf_type;

typedef enum {
    GK_VO_OUT_PAL = 0, /* PAL standard */
    GK_VO_OUT_NTSC = 1, /* NTSC standard */
    GK_VO_OUT_960H_PAL = 2, /* ITU-R BT.1302 960 x 576 at 50 Hz (interlaced) */
    GK_VO_OUT_960H_NTSC = 3, /* ITU-R BT.1302 960 x 480 at 60 Hz (interlaced) */

    GK_VO_OUT_640x480_60 = 4, /* VESA 640 x 480 at 60 Hz (non-interlaced) CVT */
    GK_VO_OUT_480P60 = 5, /* 720  x  480 at 60 Hz. */
    GK_VO_OUT_576P50 = 6, /* 720  x  576 at 50 Hz. */
    GK_VO_OUT_800x600_60 = 7, /* VESA 800 x 600 at 60 Hz (non-interlaced) */
    GK_VO_OUT_1024x768_60 = 8, /* VESA 1024 x 768 at 60 Hz (non-interlaced) */
    GK_VO_OUT_720P50 = 9, /* 1280 x  720 at 50 Hz. */
    GK_VO_OUT_720P60 = 10, /* 1280 x  720 at 60 Hz. */
    GK_VO_OUT_1280x800_60 = 11, /* 1280*800@60Hz VGA@60Hz */
    GK_VO_OUT_1280x1024_60 = 12, /* VESA 1280 x 1024 at 60 Hz (non-interlaced) */
    GK_VO_OUT_1366x768_60 = 13, /* VESA 1366 x 768 at 60 Hz (non-interlaced) */
    GK_VO_OUT_1400x1050_60 = 14, /* VESA 1400 x 1050 at 60 Hz (non-interlaced) CVT */
    GK_VO_OUT_1440x900_60 = 15, /* VESA 1440 x 900 at 60 Hz (non-interlaced) CVT Compliant */
    GK_VO_OUT_1600x1200_60 = 16, /* VESA 1600 x 1200 at 60 Hz (non-interlaced) */
    GK_VO_OUT_1680x1050_60 = 17, /* VESA 1680 x 1050 at 60 Hz (non-interlaced) */

    GK_VO_OUT_1080P24 = 18, /* 1920 x 1080 at 24 Hz. */
    GK_VO_OUT_1080P25 = 19, /* 1920 x 1080 at 25 Hz. */
    GK_VO_OUT_1080P30 = 20, /* 1920 x 1080 at 30 Hz. */
    GK_VO_OUT_1080I50 = 21, /* 1920 x 1080 at 50 Hz, interlaced. */
    GK_VO_OUT_1080I60 = 22, /* 1920 x 1080 at 60 Hz, interlaced. */
    GK_VO_OUT_1080P50 = 23, /* 1920 x 1080 at 50 Hz. */
    GK_VO_OUT_1080P60 = 24, /* 1920 x 1080 at 60 Hz. */

    GK_VO_OUT_1920x1200_60 = 25, /* VESA 1920 x 1200 at 60 Hz (non-interlaced) CVT (Reduced Blanking) */
    GK_VO_OUT_1920x2160_30 = 26, /* 1920x2160_30 */
    GK_VO_OUT_2560x1440_30 = 27, /* 2560x1440_30 */
    GK_VO_OUT_2560x1440_60 = 28, /* 2560x1440_60 */
    GK_VO_OUT_2560x1600_60 = 29, /* 2560x1600_60 */

    GK_VO_OUT_3840x2160_24 = 30, /* 3840x2160_24 */
    GK_VO_OUT_3840x2160_25 = 31, /* 3840x2160_25 */
    GK_VO_OUT_3840x2160_30 = 32, /* 3840x2160_30 */
    GK_VO_OUT_3840x2160_50 = 33, /* 3840x2160_50 */
    GK_VO_OUT_3840x2160_60 = 34, /* 3840x2160_60 */
    GK_VO_OUT_4096x2160_24 = 35, /* 4096x2160_24 */
    GK_VO_OUT_4096x2160_25 = 36, /* 4096x2160_25 */
    GK_VO_OUT_4096x2160_30 = 37, /* 4096x2160_30 */
    GK_VO_OUT_4096x2160_50 = 38, /* 4096x2160_50 */
    GK_VO_OUT_4096x2160_60 = 39, /* 4096x2160_60 */
    GK_VO_OUT_7680x4320_30 = 40, /* 7680x4320_30 */

    GK_VO_OUT_240x320_50 = 41, /* 240x320_50 */
    GK_VO_OUT_320x240_50 = 42, /* 320x240_50 */
    GK_VO_OUT_240x320_60 = 43, /* 240x320_60 */
    GK_VO_OUT_320x240_60 = 44, /* 320x240_60 */
    GK_VO_OUT_800x600_50 = 45, /* 800x600_60 */

    GK_VO_OUT_720x1280_60 = 46, /* For MIPI DSI Tx 720 x1280 at 60 Hz */
    GK_VO_OUT_1080x1920_60 = 47, /* For MIPI DSI Tx 1080x1920 at 60 Hz */

    GK_VO_OUT_USER = 48, /* User timing. */

    GK_VO_OUT_BUTT,
} gk_vo_intf_sync;

typedef struct {
    gk_bool syncm; /* RW; sync mode(0:timing,as BT.656; 1:signal,as LCD) */
    gk_bool iop; /* RW; interlaced or progressive display(0:i; 1:p) */
    gk_u8 intfb; /* RW; interlaced bit width while output */

    gk_u16 vact; /* RW; vertical active area */
    gk_u16 vbb; /* RW; vertical back blank porch */
    gk_u16 vfb; /* RW; vertical front blank porch */

    gk_u16 hact; /* RW; horizontal active area */
    gk_u16 hbb; /* RW; horizontal back blank porch */
    gk_u16 hfb; /* RW; horizontal front blank porch */
    gk_u16 hmid; /* RW; bottom horizontal active area */

    gk_u16 bvact; /* RW; bottom vertical active area */
    gk_u16 bvbb; /* RW; bottom vertical back blank porch */
    gk_u16 bvfb; /* RW; bottom vertical front blank porch */

    gk_u16 hpw; /* RW; horizontal pulse width */
    gk_u16 vpw; /* RW; vertical pulse width */

    gk_bool idv; /* RW; inverse data valid of output */
    gk_bool ihs; /* RW; inverse horizontal synchronization signal */
    gk_bool ivs; /* RW; inverse vertical synchronization signal */
} gk_vo_sync_info;

typedef struct {
    gk_u32 bg_color; /* RW; background color of a device, in RGB format. */
    gk_vo_intf_type intf_type; /* RW; type of a VO interface */
    gk_vo_intf_sync intf_sync; /* RW; type of a VO interface timing */
    gk_vo_sync_info sync_info; /* RW; information about VO interface timing */
} gk_vo_pub_attr;

typedef struct {
    gk_bool exit_dev_en;  /* RW, range: [0, 1];  whether to disable the vo device when sys exit */
    gk_bool dev_clk_ext_en;  /* RW, range: [0, 1];  whether to open the vo device clock by the external user */
    gk_bool vga_detect_en;   /* RW, range: [0, 1];  whether to enable the vga detect */
    gk_u32 vdac_detect_cycle;  /* RW; VDAC(video digital-to-analog converter) detect cycle; Unit: frame interruption */
} gk_vo_mod_param;

typedef enum {
    GK_VO_CLK_SRC_PLL = 0,       /* Clock source type PLL */
    GK_VO_CLK_SRC_LCDMCLK = 1,   /* Clock source type LCDMCLK */
    GK_VO_CLK_SRC_PLL_FOUT4 = 2, /* Clock source type PLL FOUT4 */
    GK_VO_CLK_SRC_FIXED = 3,     /* Clock source type FIXED */

    GK_VO_CLK_SRC_BUTT,
} gk_vo_clk_src;

typedef enum {
    GK_VO_FIXED_CLK_65M    = 0, /* Fixed clock source 65MHz */
    GK_VO_FIXED_CLK_74_25M = 1, /* Fixed clock source 74.25MHz */
    GK_VO_FIXED_CLK_108M   = 2, /* Fixed clock source 108MHz */
    GK_VO_FIXED_CLK_135M   = 3, /* Fixed clock source 135MHz */
    GK_VO_FIXED_CLK_148_5M = 4, /* Fixed clock source 148.5MHz */
    GK_VO_FIXED_CLK_297M   = 5, /* Fixed clock source 297MHz */

    GK_VO_FIXED_CLK_BUTT,
} gk_vo_fixed_clk;

typedef struct {
    gk_u32 fb_div;    /* RW, range: [0, 0xfff];  frequency double division */
    gk_u32 frac;      /* RW, range: [0, 0xffffff]; fractional division */
    gk_u32 ref_div;   /* RW, range: (0, 0x3f]; reference clock division */
    gk_u32 post_div1; /* RW, range: (0, 0x7]; level 1 post division */
    gk_u32 post_div2; /* RW, range: (0, 0x7]; level 2 post division */
} gk_vo_pll;

typedef struct {
    gk_vo_clk_src clk_src; /* RW; clock source type */

    union {
        /*
         * RW; user synchronization timing clock PLL information.
         * AUTO: gk_vo_clk_src:GK_VO_CLK_SRC_PLL, GK_VO_CLK_SRC_PLL_FOUT4;
         */
        gk_vo_pll vo_pll;
        /*
         * RW, range: [1, 8473341]; LCD clock division.
         * AUTO: gk_vo_clk_src:GK_VO_CLK_SRC_LCDMCLK;
         */
        gk_u32 lcd_m_clk_div;

        gk_vo_fixed_clk fixed_clk; /* RW; fixed clock. AUTO: gk_vo_clk_src:GK_VO_CLK_SRC_FIXED; */
    };
} gk_vo_user_sync_attr;

typedef struct {
    gk_vo_user_sync_attr user_sync_attr; /* RW; user synchronization timing attribute */
    gk_u32 pre_div;                      /* RW, range: [1, 32]; device previous division */
    gk_u32 dev_div;                      /* RW, range: [1, 4]; device clock division */
    gk_bool clk_reverse_en;              /* RW, range: [0, 1]; whether to reverse clock  */
} gk_vo_user_sync_info;

typedef struct {
    gk_bool enable;  /* RW, less buf enable */
    gk_u32 vtth;     /* RW, vtth value */
}gk_vo_less_buf_attr;

typedef enum {
    GK_VO_INTF_STATUS_NO_PLUG = 0,  /* Interface status is not plugged */
    GK_VO_INTF_STATUS_PLUG = 1,     /* Interface status is plugged */
    GK_VO_INTF_STATUS_BUTT,
}gk_vo_intf_plug_status;

typedef struct {
    gk_vo_intf_plug_status plug_status;  /* R; Interface plug status */
}gk_vo_intf_status;

typedef enum {
    GK_VO_CSC_MATRIX_BT601LIMIT_TO_BT601LIMIT = 0, /* Identity matrix.   from BT.601 limit to BT.601 limit */
    GK_VO_CSC_MATRIX_BT601FULL_TO_BT601LIMIT = 1,  /* Change color space from BT.601 full to BT.601 limit */
    GK_VO_CSC_MATRIX_BT709LIMIT_TO_BT601LIMIT = 2, /* Change color space from BT.709 limit to BT.601 limit */
    GK_VO_CSC_MATRIX_BT709FULL_TO_BT601LIMIT = 3,  /* Change color space from BT.709 full to BT.601 limit */

    GK_VO_CSC_MATRIX_BT601LIMIT_TO_BT709LIMIT = 4, /* Change color space from BT.601 limit to BT.709 limit */
    GK_VO_CSC_MATRIX_BT601FULL_TO_BT709LIMIT = 5,  /* Change color space from BT.601 full to BT.709 limit */
    GK_VO_CSC_MATRIX_BT709LIMIT_TO_BT709LIMIT = 6, /* Identity matrix.   from BT.709 limit to BT.709 limit */
    GK_VO_CSC_MATRIX_BT709FULL_TO_BT709LIMIT = 7,  /* Change color space from BT.709 full to BT.709 limit */

    GK_VO_CSC_MATRIX_BT601LIMIT_TO_BT601FULL = 8,  /* Change color space from BT.601 limit to BT.601 full */
    GK_VO_CSC_MATRIX_BT601FULL_TO_BT601FULL = 9,   /* Identity matrix.   from BT.601 full to BT.601 full */
    GK_VO_CSC_MATRIX_BT709LIMIT_TO_BT601FULL = 10,  /* Change color space from BT.709 limit to BT.601 full */
    GK_VO_CSC_MATRIX_BT709FULL_TO_BT601FULL = 11,   /* Change color space from BT.709 full to BT.601 full */

    GK_VO_CSC_MATRIX_BT601LIMIT_TO_BT709FULL = 12,  /* Change color space from BT.601 limit to BT.709 full */
    GK_VO_CSC_MATRIX_BT601FULL_TO_BT709FULL = 13,   /* Change color space from BT.601 full to BT.709 full */
    GK_VO_CSC_MATRIX_BT709LIMIT_TO_BT709FULL = 14,  /* Change color space from BT.709 limit to BT.709 full */
    GK_VO_CSC_MATRIX_BT709FULL_TO_BT709FULL = 15,   /* Identity matrix.   from BT.709 full to BT.709 full */

    GK_VO_CSC_MATRIX_BT601LIMIT_TO_RGBFULL = 16,    /* Change color space from BT.601 limit to RGB full */
    GK_VO_CSC_MATRIX_BT601FULL_TO_RGBFULL = 17,     /* Change color space from BT.601 full to RGB full */
    GK_VO_CSC_MATRIX_BT709LIMIT_TO_RGBFULL = 18,    /* Change color space from BT.709 limit to RGB full */
    GK_VO_CSC_MATRIX_BT709FULL_TO_RGBFULL = 19,     /* Change color space from BT.709 full to RGB full */

    GK_VO_CSC_MATRIX_BT601LIMIT_TO_RGBLIMIT = 20,   /* Change color space from BT.601 limit to RGB limit */
    GK_VO_CSC_MATRIX_BT601FULL_TO_RGBLIMIT = 21,    /* Change color space from BT.709 full to RGB limit */
    GK_VO_CSC_MATRIX_BT709LIMIT_TO_RGBLIMIT = 22,   /* Change color space from BT.601 limit to RGB limit */
    GK_VO_CSC_MATRIX_BT709FULL_TO_RGBLIMIT = 23,    /* Change color space from BT.709 full to RGB limit */

    GK_VO_CSC_MATRIX_RGBFULL_TO_BT601LIMIT = 24,    /* Change color space from RGB full to BT.601 limit */
    GK_VO_CSC_MATRIX_RGBFULL_TO_BT601FULL = 25,     /* Change color space from RGB full to BT.601 full */
    GK_VO_CSC_MATRIX_RGBFULL_TO_BT709LIMIT = 26,    /* Change color space from RGB full to BT.709 limit */
    GK_VO_CSC_MATRIX_RGBFULL_TO_BT709FULL = 27,     /* Change color space from RGB full to BT.709 full */

    GK_VO_CSC_MATRIX_BUTT,
} gk_vo_csc_matrix;

typedef struct {
    gk_vo_csc_matrix csc_matrix; /* RW; CSC matrix */
    gk_u32 luma; /* RW; range: [0, 100]; luminance, default: 50 */
    gk_u32 contrast; /* RW; range: [0, 100]; contrast, default: 50 */
    gk_u32 hue; /* RW; range: [0, 100]; hue, default: 50 */
    gk_u32 saturation; /* RW; range: [0, 100]; saturation, default: 50 */
    gk_bool ex_csc_en; /* RW; range: [0, 1]; extended csc switch for luminance, default: 0 */
} gk_vo_csc;

typedef struct {
    gk_vo_csc csc;  /* RW; color space */
    gk_u32 gain;  /* RW; range: [0, 64); current gain of VGA signals. */
    gk_s32 sharpen_strength;  /* RW; range: [0, 255]; current sharpen strength of VGA signals. */
} gk_vo_vga_param;

typedef struct {
    gk_vo_csc csc; /* RW, color space */
} gk_vo_hdmi_param;

typedef struct {
    gk_vo_csc csc; /* RW, color space */
    gk_bool rgb_inverted_en; /* RW; component r g b inverted, rgb to bgr, default: 0, rgb */
    gk_bool bit_inverted_en; /* RW; data's bit inverted,
                                rgb6bit: bit[5:0] to bit[0:5],
                                rgb8bit: bit[7:0] to bit[0:7],
                                rgb16bit: bit[15:0] to bit[0:15],
                                rgb18bit: bit[17:0] to bit[0:17],
                                rgb24bit: bit[23:0] to bit[0:23],
                                default: 0, bit[5/7/15/17/23:0] */
} gk_vo_rgb_param;

typedef struct {
    gk_bool yc_inverted_en; /* RW; component y c inverted, yc to cy, default: 0, yc */
    gk_bool bit_inverted_en; /* RW; data's bit inverted, bt.656: bit[7:0] to bit[0:7],
                                bt.1120: bit[15:0] to bit[0:15],
                                default: 0, bit[7:0] or bit[15:0] */
} gk_vo_bt_param;

typedef enum {
    GK_VO_PARTITION_MODE_SINGLE = 0, /* Single partition, use software to make multi-picture in one hardware cell */
    GK_VO_PARTITION_MODE_MULTI = 1,  /* Multi partition, each partition is a hardware cell */
    GK_VO_PARTITION_MODE_BUTT,
} gk_vo_partition_mode;

typedef struct {
    gk_rect display_rect; /* RW; display resolution */
    gk_phys_addr_t address;
    gk_u32 stride;
} gk_vo_video_layer_attr;

typedef enum {
    GK_VO_GFX_TYPE_ARGB1555 = 0,
    GK_VO_GFX_TYPE_BMP1555 = 1,

    GK_VO_GFX_TYPE_BUTT
}gk_vo_gfx_type;

typedef struct {
    gk_rect display_rect;
    gk_phys_addr_t address;
    gk_u32 stride;
    gk_vo_gfx_type type;
} gk_vo_gfx_attr;

typedef struct {
    gk_aspect_ratio aspect_ratio; /* RW; aspect ratio */
} gk_vo_layer_param;

typedef struct {
    gk_u32 priority; /* RW; video out overlay priority sd */
    gk_rect rect; /* RW; rectangle of video output channel */
    gk_bool deflicker_en; /* RW; deflicker or not sd */
} gk_vo_chn_attr;

typedef struct {
    gk_aspect_ratio aspect_ratio; /* RW; aspect ratio */
} gk_vo_chn_param;

typedef enum {
    GK_VO_ZOOM_IN_RECT = 0, /* Zoom in by rect */
    GK_VO_ZOOM_IN_RATIO = 1, /* Zoom in by ratio */
    GK_VO_ZOOM_IN_BUTT,
} gk_vo_zoom_in_type;

typedef struct {
    /* RW; range: [0, 1000]; x_ratio = x * 1000 / W, x means start point to be zoomed, W means channel's width. */
    gk_u32 x_ratio;
    /* RW; range: [0, 1000]; y_ratio = y * 1000 / H, y means start point to be zoomed, H means channel's height. */
    gk_u32 y_ratio;
    /* RW; range: [0, 1000]; width_ratio = w * 1000 / W, w means width to be zoomed, W means channel's width. */
    gk_u32 width_ratio;
    /* RW; range: [0, 1000]; height_ratio = h * 1000 / H, h means height to be zoomed, H means channel's height. */
    gk_u32 height_ratio;
} gk_vo_zoom_ratio;

typedef struct {
    gk_vo_zoom_in_type zoom_type; /* RW; choose the type of zoom in */
    union {
        gk_rect zoom_rect; /* RW; zoom in by rect. AUTO:gk_vo_zoom_in_type:GK_VO_ZOOM_IN_RECT; */
        gk_vo_zoom_ratio zoom_ratio; /* RW; zoom in by ratio. AUTO:gk_vo_zoom_in_type:GK_VO_ZOOM_IN_RATIO; */
    };
} gk_vo_zoom_attr;

typedef struct {
    gk_bool enable; /* RW; do frame or not */
    gk_border border; /* RW; frame's top, bottom, left, right width and color */
} gk_vo_border;

typedef struct {
    gk_u32 chn_buf_used; /* R; channel buffer that been used */
} gk_vo_chn_status;

typedef struct {
    gk_size target_size; /* RW; WBC zoom target size */
    gk_pixel_format pixel_format; /* RW; the pixel format of WBC output */
    gk_u32 frame_rate; /* RW; frame rate control */
    gk_dynamic_range dynamic_range; /* RW; write back dynamic range type */
    gk_compress_mode compress_mode; /* RW; write back compressing mode   */
} gk_vo_wbc_attr;

typedef enum {
    GK_VO_WBC_MODE_NORM = 0, /* In this mode, wbc will capture frames according to dev frame rate
                                and wbc frame rate */
    GK_VO_WBC_MODE_DROP_REPEAT = 1, /* In this mode, wbc will drop dev repeat frame, and capture the real frame
                                according to video layer's display rate and wbc frame rate */
    GK_VO_WBC_MODE_PROGRESSIVE_TO_INTERLACED = 2, /* In this mode, wbc will drop dev repeat frame which repeats more
                                than 3 times, and change two progressive frames to one interlaced frame */
    GK_VO_WBC_MODE_BUTT,
} gk_vo_wbc_mode;

typedef enum {
    GK_VO_WBC_SRC_DEV = 0, /* WBC source is device */
    GK_VO_WBC_SRC_VIDEO = 1, /* WBC source is video layer */
    GK_VO_WBC_SRC_BUTT,
} gk_vo_wbc_src_type;

typedef struct {
    gk_vo_wbc_src_type src_type; /* RW; WBC source's type */
    gk_u32 src_id; /* RW; WBC source's ID */
} gk_vo_wbc_src;

typedef enum {
    GK_VO_CAS_MODE_SINGLE = 0, /* cascade mode is single */
    GK_VO_CAS_MODE_DUAL = 1, /* cascade mode is dual */
    GK_VO_CAS_MODE_BUTT,
} gk_vo_cas_mode;

typedef enum {
    GK_VO_CAS_MODE_SINGLE_EDGE = 0, /* single transmission,clock rising edge or falling edge trigger transmission */
    GK_VO_CAS_MODE_DUAL_EDGE = 1, /* dual transmission,clock rising edge and falling edge trigger transmission */
    GK_VO_CAS_MODE_EDGE_BUTT,
} gk_vo_cas_data_transmission_mode;

typedef enum {
    GK_VO_CAS_RGN_64 = 0, /* cascade region number 64 */
    GK_VO_CAS_RGN_32 = 1, /* cascade region number 32 */
    GK_VO_CAS_RGN_BUTT,
} gk_vo_cas_rgn;

typedef struct {
    gk_bool         is_slave;                        /* RW; GK_TRUE: slave mode, GK_FALSE: master mode */
    gk_vo_cas_rgn   cas_rgn;                         /* RW; cascade region number */
    gk_vo_cas_mode  cas_mode;                        /* RW; cascade mode */
    gk_vo_cas_data_transmission_mode cas_edge_mode;  /* RW; cascade data transmission mode  */
} gk_vo_cas_attr;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif /* end of #ifndef __GK_COMMON_VO_H__ */
