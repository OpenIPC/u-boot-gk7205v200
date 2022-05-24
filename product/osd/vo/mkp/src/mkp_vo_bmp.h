/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#ifndef __MKP_VO_BMP_H__
#define __MKP_VO_BMP_H__

#include "type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

typedef struct {
    gk_u16 bi_size;
    gk_u32 bi_width;
    gk_s32 bi_height;
    gk_u16 bi_planes;
    gk_u16 bi_bit_count;
    gk_u32 bi_compression;
    gk_u32 bi_size_image;
    gk_u32 bi_x_pels_per_meter;
    gk_u32 bi_y_pels_per_meter;
    gk_u32 bi_clr_used;
    gk_u32 bi_clr_important;
} osd_bitmap_info_header;

typedef struct {
    gk_u8 rgb_blue;
    gk_u8 rgb_green;
    gk_u8 rgb_red;
    gk_u8 rgb_reserved;
} osd_rgb_quad;

typedef struct {
    gk_u32 bf_size;
    gk_u16 bf_reserved1;
    gk_u16 bf_reserved2;
    gk_u32 bf_off_bits;
} osd_bitmap_file_header;

typedef struct {
    osd_bitmap_info_header bmi_header;
    osd_rgb_quad bmi_colors[1];
} osd_bitmap_info;

typedef struct {
    gk_u32 width;       /* out */
    gk_u32 height;      /* out */
    gk_u32 stride;      /* in */
    gk_phys_addr_t rgb_buffer; /* in */
} osd_logo_t;

typedef enum {
    OSD_BI_BIT_COUNT_1BPP = 1,
    OSD_BI_BIT_COUNT_4BPP = 4,
    OSD_BI_BIT_COUNT_8BPP = 8,
    OSD_BI_BIT_COUNT_16BPP = 16,
    OSD_BI_BIT_COUNT_24BPP = 24,
    OSD_BI_BIT_COUNT_32BPP = 32,
} osd_bi_bit_count;

gk_s32 load_bmp(gk_phys_addr_t bmp_addr, osd_logo_t *video_logo);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif /* end of #ifndef __MKP_VO_BMP__ */
