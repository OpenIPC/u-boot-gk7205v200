/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#include "vou_coef_org.h"

/* RGB->YUV601 */
const vo_csc_coef g_csc_rgb_to_yuv601_tv = {
    /* csc */
    258, 504, 98, -148, -291, 439, 439, -368, -71,
    /* csc(IDC) */
    0, 0, 0,
    /* csc(ODC) */
    16, 128, 128
};

/* RGB->YUV601 */
const vo_csc_coef g_csc_rgb_to_yuv601_pc = {
    /* csc */
    299, 587, 114, -172, -339, 511, 511, -428, -83,
    /* csc(IDC) */
    0, 0, 0,
    /* csc(ODC) */
    0, 128, 128
};

/* RGB->YUV709 */
const vo_csc_coef g_csc_rgb_to_yuv709_tv = {
    /* csc */
    5966, 20071, 2026, -3289, -11063, 14352, 14352, -13021, -1316,
    /* csc(IDC) */
    0, 0, 0,
    /* csc(ODC) */
    256, 2048, 2048
};

/* RGB->YUV709 output full[0,255] */
const vo_csc_coef g_csc_rgb_to_yuv709_pc = {
    /* csc */
    213, 715, 72, -117, -394, 511, 511, -464, -47,
    /* csc(IDC) */
    0, 0, 0,
    /* csc(ODC) */
    0, 128, 128
};

/* RGB -> YUV BT.2020 coefficient matrix */
const vo_csc_coef g_csc_rgb_to_yuv2020_pc = {
    /* csc multiplier coefficient */
    263, 678, 59, -143, -368, 511, 511, -470, -41,
    /* csc input DC component (IDC) */
    0, 0, 0,
    /* csc output DC component (ODC) */
    0, 128, 128
};

/* YUV601->RGB  */
const vo_csc_coef g_csc_yuv601_to_rgb_pc = {
    /* csc */
    1000, 0, 1371, 1000, -336, -689, 1000, 1732, 0,
    /* csc(IDC) */
    0, -128, -128,
    /* csc(ODC) */
    0, 0, 0
};

/* YUV709->RGB  */
const vo_csc_coef g_csc_yuv709_to_rgb_pc = {
    /* csc */
    1000, 0, 1540, 1000, -183, -459, 1000, 1816, 0,
    /* csc(IDC) */
    0, -128, -128,
    /* csc(ODC) */
    0, 0, 0
};

/* BT.2020 YUV -> RGB coefficient matrix */
const vo_csc_coef g_csc_yuv2020_to_rgb_pc = {
    /* csc multiplier coefficient */
    1000, 0, 1442, 1000, -160, -560, 1000, 1841, 0,
    /* csc input DC component (IDC) */
    0, -128, -128,
    /* csc output DC component (ODC) */
    0, 0, 0
};

/* YUV601->YUV709 output full[0,255] */
const vo_csc_coef g_csc_yuv_to_yuv_601_709 = {
    /* csc */
    1000, -116, 208, 0, 1017, 114, 0, 75, 1025,
    /* csc(IDC) */
    -16, -128, -128,
    /* csc(ODC) */
    16, 128, 128
};

/* YUV709->YUV601 full[0,255] */
const vo_csc_coef g_csc_yuv_to_yuv_709_601 = {
    /* csc */
    1000, 99, 192, 0, 990, -111, 0, -72, 983,
    /* csc(IDC) */
    -16, -128, -128,
    /* csc(ODC) */
    16, 128, 128
};

/* YUV601->YUV709  */
const vo_csc_coef g_csc_init = {
    /* csc */
    1000, 0, 0, 0, 1000, 0, 0, 0, 1000,
    /* csc(IDC) */
    -16, -128, -128,
    /* csc(ODC) */
    16, 128, 128
};

const int g_sin_table[61] = { /* 61 sin table length */
    -500, -485, -469, -454, -438, -422, -407, -391, -374, -358,
    -342, -325, -309, -292, -276, -259, -242, -225, -208, -191,
    -174, -156, -139, -122, -104, -87, -70, -52, -35, -17,
    0, 17, 35, 52, 70, 87, 104, 122, 139, 156,
    174, 191, 208, 225, 242, 259, 276, 292, 309, 325,
    342, 358, 374, 391, 407, 422, 438, 454, 469, 485,
    500
};

const int g_cos_table[61] = { /* 61 cos table length */
    866, 875, 883, 891, 899, 906, 914, 921, 927, 934,
    940, 946, 951, 956, 961, 966, 970, 974, 978, 982,
    985, 988, 990, 993, 995, 996, 998, 999, 999, 1000,
    1000, 1000, 999, 999, 998, 996, 995, 993, 990, 988,
    985, 982, 978, 974, 970, 966, 961, 956, 951, 946,
    940, 934, 927, 921, 914, 906, 899, 891, 883, 875,
    866
};

