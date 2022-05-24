/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __VOU_COEF_ORG_H__
#define __VOU_COEF_ORG_H__

#include "vou_coef.h"

#ifdef __cplusplus
extern "C" {
#endif /* end of #ifdef __cplusplus */

/* RGB->YUV601 */
extern const vo_csc_coef g_csc_rgb_to_yuv601_tv;
/* RGB->YUV601 */
extern const vo_csc_coef g_csc_rgb_to_yuv601_pc;
/* RGB->YUV709 */
extern const vo_csc_coef g_csc_rgb_to_yuv709_tv;
/* RGB->YUV709 */
extern const vo_csc_coef g_csc_rgb_to_yuv709_pc;
extern const vo_csc_coef g_csc_rgb_to_yuv2020_pc;
/* YUV601->RGB */
extern const vo_csc_coef g_csc_yuv601_to_rgb_pc;
/* YUV709->RGB */
extern const vo_csc_coef g_csc_yuv709_to_rgb_pc;
extern const vo_csc_coef g_csc_yuv2020_to_rgb_pc;
/* YUV601->YUV709 */
extern const vo_csc_coef g_csc_yuv_to_yuv_601_709;
/* YUV709->YUV601 */
extern const vo_csc_coef g_csc_yuv_to_yuv_709_601;
/* YUV601->YUV709 */
extern const vo_csc_coef g_csc_init;

extern const int g_sin_table[61]; /* 61 sin table array */
extern const int g_cos_table[61]; /* 61 cos table array */

#ifdef __cplusplus
}
#endif /* end of #ifdef __cplusplus */

#endif
