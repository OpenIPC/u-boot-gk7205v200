/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */
#ifndef __JPEGD_DRV_H__
#define __JPEGD_DRV_H__
#include "jpegd_reg.h"
#include "type.h"

#ifdef __cplusplus
extern "C" {
#endif /* End of #ifdef __cplusplus */

#define JPEGD_REGS_SIZE       0x6BF

#define JPEGD_IP_NUM          1

#define QUANT_TABLE_SIZE      64
#define HDC_TABLE_SIZE        12
#define HAC_MIN_TABLE_SIZE    8
#define HAC_BASE_TABLE_SIZE   8
#define HAC_SYMBOL_TABLE_SIZE 176
#define ZIGZAG_TABLE_SIZE     64

typedef struct {
    gk_bool int_dec_finish;
    gk_bool int_dec_err;
    gk_bool int_over_time;
    gk_bool int_bs_res;
} jpegd_vpu_status;

typedef enum {
    PIXEL_FORMAT_RGB_444 = 0,
    PIXEL_FORMAT_RGB_555,
    PIXEL_FORMAT_RGB_565,
    PIXEL_FORMAT_RGB_888,

    PIXEL_FORMAT_BGR_444,
    PIXEL_FORMAT_BGR_555,
    PIXEL_FORMAT_BGR_565,
    PIXEL_FORMAT_BGR_888,

    PIXEL_FORMAT_ARGB_1555,
    PIXEL_FORMAT_ARGB_4444,
    PIXEL_FORMAT_ARGB_8565,
    PIXEL_FORMAT_ARGB_8888,

    PIXEL_FORMAT_ABGR_1555,
    PIXEL_FORMAT_ABGR_4444,
    PIXEL_FORMAT_ABGR_8565,
    PIXEL_FORMAT_ABGR_8888,

    PIXEL_FORMAT_RGB_BAYER_8BPP,
    PIXEL_FORMAT_RGB_BAYER_10BPP,
    PIXEL_FORMAT_RGB_BAYER_12BPP,
    PIXEL_FORMAT_RGB_BAYER_14BPP,
    PIXEL_FORMAT_RGB_BAYER_16BPP,

    PIXEL_FORMAT_YUV_PLANAR_422,
    PIXEL_FORMAT_YUV_PLANAR_420,
    PIXEL_FORMAT_YUV_PLANAR_444,

    PIXEL_FORMAT_YVU_SEMIPLANAR_422,
    PIXEL_FORMAT_YVU_SEMIPLANAR_420,
    PIXEL_FORMAT_YVU_SEMIPLANAR_444,

    PIXEL_FORMAT_UYVY_PACKAGE_422,
    PIXEL_FORMAT_YUYV_PACKAGE_422,
    PIXEL_FORMAT_VYUY_PACKAGE_422,

    PIXEL_FORMAT_YUV_400,

    PIXEL_FORMAT_BUTT
} gk_pixel_format;

typedef enum {
    PICTURE_FORMAT_YUV420 = 0,
    PICTURE_FORMAT_YUV422 = 1, /* 422 2x1 */
    PICTURE_FORMAT_YUV444 = 2,
    PICTURE_FORMAT_YUV422V = 3, /* 422 1x2 */
    PICTURE_FORMAT_YUV400 = 4,
    PICTURE_FORMAT_BUTT
} picture_format;

typedef struct {
    int chn_id;
    picture_format pic_format;
    gk_pixel_format pixel_format;
    gk_bool out_yuv;
    unsigned char v_fac;
    unsigned char u_fac;
    unsigned char y_fac;
    unsigned int dri;
    unsigned int width;
    unsigned int height;
    unsigned int width_in_mcu;
    unsigned int height_in_mcu;
    unsigned int y_stride;
    unsigned int c_stride;
    unsigned int pic_type;
    unsigned int rgb_stride;
    unsigned int alpha;

    unsigned long long y_phy_addr;
    unsigned long long c_phy_addr;
    unsigned long long phy_str_buf_start;
    unsigned long long phy_str_buf_end;
    unsigned long long phy_str_start;
    unsigned long long phy_str_end;
    unsigned long long phy_emar_buffer0;
    unsigned long long phy_emar_buffer1;

    unsigned int quant_table[QUANT_TABLE_SIZE];
    unsigned int huffman_table[HDC_TABLE_SIZE];
    unsigned int huffman_min_table[HAC_MIN_TABLE_SIZE];
    unsigned int huffman_base_table[HAC_BASE_TABLE_SIZE];
    unsigned int huffman_symbol_table[HAC_SYMBOL_TABLE_SIZE];
} jpegd_vpu_config;

void jpegd_drv_write_regs(S_JPGD_REGS_TYPE *reg_base, jpegd_vpu_config *vpu_config);

void jpegd_drv_read_regs(S_JPGD_REGS_TYPE *reg_base, jpegd_vpu_status *vpu_status);

void jpegd_set_clock_en(int vpu_id, gk_bool is_run);
void jpegd_reset_select(int vpu_id, gk_bool is_run);

void jpegd_set_outstanding(int vpu_id, int outstanding);
void *jpegd_get_reg_addr(int vpu_id);
unsigned int jpegd_read_int(int vpu_id);
void jpegd_clear_int(int vpu_id);
void jpegd_set_int_mask(int vpu_id);
void jpegd_reset(int vpu_id);
void jpegd_start_vpu(int vpu_id);
int jpegd_set_time_out(int vpu_id, int time_out);

#ifdef __cplusplus
}
#endif /* End of #ifdef __cplusplus */

#endif /* End of __JPEGD_DRV_H__ */
