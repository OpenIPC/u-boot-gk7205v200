/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __GK_COMMON_VIDEO_H__
#define __GK_COMMON_VIDEO_H__

#include "type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

typedef enum  {
    GK_OP_MODE_AUTO   = 0,
    GK_OP_MODE_MANUAL = 1,
    GK_OP_MODE_BUTT
} gk_op_mode;

typedef enum  {
    GK_VB_SRC_COMMON  = 0,
    GK_VB_SRC_MOD  = 1,
    GK_VB_SRC_PRIVATE = 2,
    GK_VB_SRC_USER    = 3,
    GK_VB_SRC_BUTT
} gk_vb_src;

typedef enum  {
    GK_ASPECT_RATIO_NONE   = 0,        /* full screen */
    GK_ASPECT_RATIO_AUTO   = 1,        /* ratio no change, 1:1*/
    GK_ASPECT_RATIO_MANUAL = 2,        /* ratio manual set */
    GK_ASPECT_RATIO_BUTT
} gk_aspect_ratio_type;


typedef enum  {
    GK_VIDEO_FIELD_TOP         = 1,    /* even field */
    GK_VIDEO_FIELD_BOTTOM      = 2,    /* odd field */
    GK_VIDEO_FIELD_INTERLACED  = 3,    /* two interlaced fields */
    GK_VIDEO_FIELD_FRAME       = 4,    /* frame */

    GK_VIDEO_FIELD_BUTT
} gk_video_field;

typedef enum  {
    GK_VIDEO_FORMAT_LINEAR = 0,       /* nature video line */
    GK_VIDEO_FORMAT_TILE_64x16,       /* tile cell: 64pixel x 16line */
    GK_VIDEO_FORMAT_BUTT
} gk_video_format;

typedef enum  {
    GK_COMPRESS_MODE_NONE = 0,      /* no compress */
    GK_COMPRESS_MODE_SEG,           /* compress unit is 256x1 bytes as a segment.*/
    GK_COMPRESS_MODE_SEG_COMPACT,   /* compact compress unit is 256x1 bytes as a segment.*/
    GK_COMPRESS_MODE_TILE,          /* compress unit is a tile.*/
    GK_COMPRESS_MODE_LINE,          /* compress unit is the whole line.*/
    GK_COMPRESS_MODE_FRAME,         /* compress unit is the whole frame. YUV for VPSS(3DNR) */

    GK_COMPRESS_MODE_BUTT
} gk_compress_mode;

typedef enum  {
    GK_VIDEO_DISPLAY_MODE_PREVIEW  = 0,
    GK_VIDEO_DISPLAY_MODE_PLAYBACK = 1,

    GK_VIDEO_DISPLAY_MODE_BUTT
} gk_video_display_mode;

typedef enum  {
    GK_FRAME_FLAG_SNAP_FLASH  = 0x1 << 0,
    GK_FRAME_FLAG_SNAP_CUR    = 0x1 << 1,
    GK_FRAME_FLAG_SNAP_REF    = 0x1 << 2,
    GK_FRAME_FLAG_SNAP_END    = 0x1 << 3,
    GK_FRAME_FLAG_MIRROR      = 0x1 << 4,
    GK_FRAME_FLAG_FLIP        = 0x1 << 5,
    GK_FRAME_FLAG_BUTT
} gk_frame_flag;


typedef enum  {
    GK_COLOR_GAMUT_BT601 = 0,
    GK_COLOR_GAMUT_BT709,
    GK_COLOR_GAMUT_BT2020,
    GK_COLOR_GAMUT_USER,
    GK_COLOR_GAMUT_BUTT
} gk_color_gamut;

/* we ONLY define picture format used, all unused will be deleted! */
typedef enum {
    GK_PIXEL_FORMAT_RGB_444 = 0,
    GK_PIXEL_FORMAT_RGB_555,
    GK_PIXEL_FORMAT_RGB_565,
    GK_PIXEL_FORMAT_RGB_888,

    GK_PIXEL_FORMAT_BGR_444,
    GK_PIXEL_FORMAT_BGR_555,
    GK_PIXEL_FORMAT_BGR_565,
    GK_PIXEL_FORMAT_BGR_888,

    GK_PIXEL_FORMAT_ARGB_1555,
    GK_PIXEL_FORMAT_ARGB_4444,
    GK_PIXEL_FORMAT_ARGB_8565,
    GK_PIXEL_FORMAT_ARGB_8888,
    GK_PIXEL_FORMAT_ARGB_2BPP,
    GK_PIXEL_FORMAT_ARGB_CLUT2,
    GK_PIXEL_FORMAT_ARGB_CLUT4,

    GK_PIXEL_FORMAT_ABGR_1555,
    GK_PIXEL_FORMAT_ABGR_4444,
    GK_PIXEL_FORMAT_ABGR_8565,
    GK_PIXEL_FORMAT_ABGR_8888,

    GK_PIXEL_FORMAT_RGB_BAYER_8BPP,
    GK_PIXEL_FORMAT_RGB_BAYER_10BPP,
    GK_PIXEL_FORMAT_RGB_BAYER_12BPP,
    GK_PIXEL_FORMAT_RGB_BAYER_14BPP,
    GK_PIXEL_FORMAT_RGB_BAYER_16BPP,

    GK_PIXEL_FORMAT_YVU_PLANAR_422,
    GK_PIXEL_FORMAT_YVU_PLANAR_420,
    GK_PIXEL_FORMAT_YVU_PLANAR_444,

    GK_PIXEL_FORMAT_YVU_SEMIPLANAR_422,
    GK_PIXEL_FORMAT_YVU_SEMIPLANAR_420,
    GK_PIXEL_FORMAT_YVU_SEMIPLANAR_444,

    GK_PIXEL_FORMAT_YUV_SEMIPLANAR_422,
    GK_PIXEL_FORMAT_YUV_SEMIPLANAR_420,
    GK_PIXEL_FORMAT_YUV_SEMIPLANAR_444,

    GK_PIXEL_FORMAT_YUYV_PACKAGE_422,
    GK_PIXEL_FORMAT_YVYU_PACKAGE_422,
    GK_PIXEL_FORMAT_UYVY_PACKAGE_422,
    GK_PIXEL_FORMAT_VYUY_PACKAGE_422,
    GK_PIXEL_FORMAT_YYUV_PACKAGE_422,
    GK_PIXEL_FORMAT_YYVU_PACKAGE_422,
    GK_PIXEL_FORMAT_UVYY_PACKAGE_422,
    GK_PIXEL_FORMAT_VUYY_PACKAGE_422,
    GK_PIXEL_FORMAT_VY1UY0_PACKAGE_422,

    GK_PIXEL_FORMAT_YUV_400,
    GK_PIXEL_FORMAT_UV_420,

    /* SVP data format */
    GK_PIXEL_FORMAT_BGR_888_PLANAR,
    GK_PIXEL_FORMAT_HSV_888_PACKAGE,
    GK_PIXEL_FORMAT_HSV_888_PLANAR,
    GK_PIXEL_FORMAT_LAB_888_PACKAGE,
    GK_PIXEL_FORMAT_LAB_888_PLANAR,
    GK_PIXEL_FORMAT_S8C1,
    GK_PIXEL_FORMAT_S8C2_PACKAGE,
    GK_PIXEL_FORMAT_S8C2_PLANAR,
    GK_PIXEL_FORMAT_S8C3_PLANAR,
    GK_PIXEL_FORMAT_S16C1,
    GK_PIXEL_FORMAT_U8C1,
    GK_PIXEL_FORMAT_U16C1,
    GK_PIXEL_FORMAT_S32C1,
    GK_PIXEL_FORMAT_U32C1,
    GK_PIXEL_FORMAT_U64C1,
    GK_PIXEL_FORMAT_S64C1,

    GK_PIXEL_FORMAT_BUTT
} gk_pixel_format;


typedef enum  {
    GK_DYNAMIC_RANGE_SDR8 = 0,
    GK_DYNAMIC_RANGE_SDR10,
    GK_DYNAMIC_RANGE_HDR10,
    GK_DYNAMIC_RANGE_HLG,
    GK_DYNAMIC_RANGE_SLF,
    GK_DYNAMIC_RANGE_XDR,
    GK_DYNAMIC_RANGE_BUTT
} gk_dynamic_range;


typedef enum  {
    GK_DATA_BIT_WIDTH_8 = 0,
    GK_DATA_BIT_WIDTH_10,
    GK_DATA_BIT_WIDTH_12,
    GK_DATA_BIT_WIDTH_14,
    GK_DATA_BIT_WIDTH_16,
    GK_DATA_BIT_WIDTH_BUTT
} gk_data_bit_width;


typedef struct {
    gk_u32 top_width;
    gk_u32 bottom_width;
    gk_u32 left_width;
    gk_u32 right_width;
    gk_u32 color;
} gk_border;

typedef struct {
    gk_s32 x;
    gk_s32 y;
} gk_point;

typedef struct {
    gk_u32 width;
    gk_u32 height;
} gk_size;

typedef struct {
    gk_s32 x;
    gk_s32 y;
    gk_u32 width;
    gk_u32 height;
} gk_rect;

typedef enum {
    GK_COORD_ABS = 0,                          /* Absolute coordinate. */
    GK_COORD_RATIO,                            /* Ratio coordinate. */
    GK_COORD_BUTT
} gk_coord;

typedef enum {
    GK_COVER_RECT = 0,                         /* Rectangle cover. */
    GK_COVER_QUAD,                             /* Quadrangle cover. */
    GK_COVER_BUTT
} gk_cover_type;

typedef struct {
    gk_bool enable;
    gk_rect  rect;
} gk_crop_info;

typedef struct {
    gk_s32  src_frame_rate;        /* RW; source frame rate */
    gk_s32  dst_frame_rate;        /* RW; dest frame rate */
} gk_frame_rate_ctrl;

typedef struct {
    gk_aspect_ratio_type mode;          /* aspect ratio mode: none/auto/manual */
    gk_u32         bg_color;      /* background color, RGB 888 */
    gk_rect         video_rect;     /* valid in ASPECT_RATIO_MANUAL mode */
} gk_aspect_ratio;

typedef struct {
    gk_u32 vb_size;

    gk_u32 head_stride;
    gk_u32 head_size;
    gk_u32 head_y_size;

    gk_u32 main_stride;
    gk_u32 main_size;
    gk_u32 main_y_size;

    gk_u32 ex_stride;
    gk_u32 ex_y_size;
} gk_vb_calc_cfg;

/* Angle of rotation */
typedef enum  {
    GK_ROTATION_0   = 0,
    GK_ROTATION_90  = 1,
    GK_ROTATION_180 = 2,
    GK_ROTATION_270 = 3,
    GK_ROTATION_BUTT
} gk_rotation;

typedef enum  {
    GK_FRAME_INTERRUPT_START,
    GK_FRAME_INTERRUPT_EARLY,
    GK_FRAME_INTERRUPT_EARLY_END,
    GK_FRAME_INTERRUPT_BUTT,
} gk_frame_interrupt_type;

typedef struct {
    gk_frame_interrupt_type interrupt_type;
    gk_u32 early_line;
} gk_frame_interrupt_attr;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __GK_COMMON_VIDEO_H__ */
