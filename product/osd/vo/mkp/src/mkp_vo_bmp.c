/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#include "mkp_vo_bmp.h"
#include "common.h"

gk_s32 load_bmp(gk_phys_addr_t bmp_addr, osd_logo_t *video_logo)
{
    gk_u16 bpp;
    osd_bitmap_file_header bmp_file_header;
    osd_bitmap_info bmp_info;

    memcpy(&bmp_file_header, (char *)(uintptr_t)(bmp_addr + sizeof(gk_u16)), sizeof(osd_bitmap_file_header));
    memcpy(&bmp_info, (char *)(uintptr_t)(bmp_addr + sizeof(gk_u16) + sizeof(osd_bitmap_file_header)), sizeof(osd_bitmap_info));
    video_logo->width = bmp_info.bmi_header.bi_width;
    video_logo->height = bmp_info.bmi_header.bi_height;
    video_logo->stride = bmp_info.bmi_header.bi_width * (OSD_BI_BIT_COUNT_16BPP / 8); /* 8 bits per bytes */
    video_logo->rgb_buffer = bmp_addr + bmp_file_header.bf_off_bits;

    bpp = bmp_info.bmi_header.bi_bit_count;
    if (bpp != OSD_BI_BIT_COUNT_16BPP) { /* 16 bits, 2bytes per pixel bitmap format */
        printf("bitmap format bpp=%d not supported! only supoort format bpp=%d\n", bpp, OSD_BI_BIT_COUNT_16BPP);
        return -1;
    }
    return 0;
}
