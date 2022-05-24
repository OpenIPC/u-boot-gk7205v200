/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __GK_MATH_H__
#define __GK_MATH_H__

#include "type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define MAX2(x, y)       ((x) > (y) ? (x) : (y))
#define MIN2(x, y)       ((x) < (y) ? (x) : (y))
#define MAX3(x, y, z)     ((x) > (y) ? MAX2(x, z) : MAX2(y, z))
#define MIN3(x, y, z)     ((x) < (y) ? MIN2(x, z) : MIN2(y, z))
#define MEDIAN(x, y, z)   (((x) + (y) + (z) - MAX3(x, y, z)) - MIN3(x, y, z))
#define MEAN2(x, y)      (((x) + (y)) >> 1)

#define CLIP_MIN(x, min)          (((x) >= (min)) ? (x) : (min))
#define CLIP3(x, min, max)         ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))
#define CLIP_MAX(x, max)          ((x) > (max) ? (max) : (x))
#define WRAP_MAX(x, max, min)      ((x) >= (max) ? (min) : (x))
#define WRAP_MIN(x, min, max)      ((x) <= (min) ? (max) : (x))
#define VALUE_BETWEEN(x, min, max) (((x) >= (min)) && ((x) <= (max)))

#define MULTI_OF_2_POWER(x, a)    (!((x) & ((a) - 1)))
#define GK_CEILING(x, a)           (((x) + (a)- 1) / (a))

#define GK_ALIGN_UP(x, a)           ((((x) + ((a) - 1)) / (a)) * (a))
#define GK_ALIGN_DOWN(x, a)         (((x) / (a)) * (a))

#define GK_DIV_UP(x, a)             (((x) + ((a) - 1)) / (a))

#define FRACTION32(de, nu)       (((de) << 16) | (nu))
#define NUMERATOR32(x)          ((x) & 0xffff)
#define DENOMINATOR32(x)        ((x) >> 16)

#define RGB(r, g, b) ((((r) & 0xff) << 16) | (((g) & 0xff) << 8) | ((b) & 0xff))
#define RGB_R(c)   (((c) & 0xff0000) >> 16)
#define RGB_G(c)   (((c) & 0xff00) >> 8)
#define RGB_B(c)   ((c) & 0xff)

#define YUV(y, u, v) ((((y) & 0x03ff) << 20) | (((u) & 0x03ff) << 10) | ((v) & 0x03ff))
#define YUV_Y(c)   (((c) & 0x3ff00000) >> 20)
#define YUV_U(c)   (((c) & 0x000ffc00) >> 10)
#define YUV_V(c)   ((c) & 0x000003ff)

#define YUV_8BIT(y, u, v) ((((y) & 0xff) << 16) | (((u) & 0xff) << 8) | ((v) & 0xff))
#define YUV_8BIT_Y(c)   (((c) & 0xff0000) >> 16)
#define YUV_8BIT_U(c)   (((c) & 0xff00) >> 8)
#define YUV_8BIT_V(c)   ((c) & 0xff)

#define gk_usleep(usec) \
    do {                \
        usleep(usec);   \
    } while (0)

#define SPAN(type, begin, end) \
({                             \
    type b = (begin);          \
    type e = (end);            \
    (type)((b >= e) ? (b - e) : (b + ((~((type)0))-e))); \
})

#define  ENDIAN32(x)                     \
    (((x) << 24) |                       \
     (((x) & 0x0000ff00) << 8) |        \
     (((x) & 0x00ff0000) >> 8) |        \
     (((x) >> 24) & 0x000000ff))


#define  ENDIAN16(x)    ((((x) << 8) & 0xff00) | (((x) >> 8) & 255))

__inline static gk_void rgb_to_yc(gk_u16 r, gk_u16 g, gk_u16 b, gk_u16 *y, gk_u16 *cb, gk_u16 *cr)
{
    /* Y */
    *y = (gk_u16)((((r * 66 + g * 129 + b * 25) >> 8) + 16) << 2); /* 66, 129, 25, 8, 16, 2 coef */

    /* cb */
    *cb = (gk_u16)(((((b * 112 - r * 38) - g * 74) >> 8) + 128) << 2); /* 112, 38, 74, 8, 128, 2 coef */

    /* cr */
    *cr = (gk_u16)(((((r * 112 - g * 94) - b * 18) >> 8) + 128) << 2); /* 112, 94, 18, 8, 128, 2 coef */
}

__inline static gk_u32 rgb_to_yuv(gk_u32 rgb)
{
    gk_u16 y, u, v;

    rgb_to_yc(RGB_R(rgb), RGB_G(rgb), RGB_B(rgb), &y, &u, &v);

    return YUV(y, u, v);
}

__inline static gk_void rgb_to_yc_full(gk_u16 r, gk_u16 g, gk_u16 b, gk_u16 *y, gk_u16 *cb, gk_u16 *cr)
{
    gk_u16 y_tmp, cb_tmp, cr_tmp;

    y_tmp = (gk_u16)(((r * 76 + g * 150 + b * 29) >> 8) * 4); /* 76, 150, 29, 8, 4 coef */
    cb_tmp = (gk_u16)(CLIP_MIN(((((b * 130 - r * 44) - g * 86) >> 8) + 128), 0) * 4); /* 130 44 86 8 128 4 coef */
    cr_tmp = (gk_u16)(CLIP_MIN(((((r * 130 - g * 109) - b * 21) >> 8) + 128), 0) * 4); /* 130 109 21 8 128 4 coef */

    *y = MAX2(MIN2(y_tmp, 1023), 0); /* 1023 coef */
    *cb = MAX2(MIN2(cb_tmp, 1023), 0); /* 1023 coef */
    *cr = MAX2(MIN2(cr_tmp, 1023), 0); /* 1023 coef */
}

__inline static gk_u32 rgb_to_yuv_full(gk_u32 rgb)
{
    gk_u16 y, u, v;

    rgb_to_yc_full(RGB_R(rgb), RGB_G(rgb), RGB_B(rgb), &y, &u, &v);

    return YUV(y, u, v);
}

__inline static gk_void rgb_to_yc_8bit(gk_u8 r, gk_u8 g, gk_u8 b, gk_u8 *y, gk_u8 *cb, gk_u8 *cr)
{
    /* Y */
    *y = (gk_u8)(((r * 66 + g * 129 + b * 25) >> 8) + 16); /* 66 129 25 8 16 coef */

    /* cb */
    *cb = (gk_u8)((((b * 112 - r * 38) - g * 74) >> 8) + 128); /* 112 38 74 8 128 coef */

    /* cr */
    *cr = (gk_u8)((((r * 112 - g * 94) - b * 18) >> 8) + 128); /* 112 94 18 8 128 coef */
}

__inline static gk_u32 rgb_to_yuv_8bit(gk_u32 rgb)
{
    gk_u8 y, u, v;

    rgb_to_yc_8bit(RGB_R(rgb), RGB_G(rgb), RGB_B(rgb), &y, &u, &v);

    return YUV_8BIT(y, u, v);
}

__inline static gk_void rgb_to_yc_full_8bit(gk_u8 r, gk_u8 g, gk_u8 b, gk_u8 *y, gk_u8 *cb, gk_u8 *cr)
{
    gk_s16 y_tmp, cb_tmp, cr_tmp;

    y_tmp = (r * 76 + g * 150 + b * 29) >> 8; /* 76 150 29 8 coef */
    cb_tmp = (((b * 130 - r * 44) - g * 86) >> 8) + 128; /* 130 44 86 8 128 coef */
    cr_tmp = (((r * 130 - g * 109) - b * 21) >> 8) + 128; /* 130 109 21 8 128 coef */

    *y = MAX2(MIN2(y_tmp, 255), 0); /* 255 coef */
    *cb = MAX2(MIN2(cb_tmp, 255), 0); /* 255 coef */
    *cr = MAX2(MIN2(cr_tmp, 255), 0); /* 255 coef */
}

__inline static gk_u32 rgb2_yuv_full_8_bit(gk_u32 rgb)
{
    gk_u8 y, u, v;

    rgb_to_yc_full_8bit(RGB_R(rgb), RGB_G(rgb), RGB_B(rgb), &y, &u, &v);

    return YUV_8BIT(y, u, v);
}

__inline static gk_u32 get_low_addr(gk_phys_addr_t phys_addr)
{
    return (gk_u32)phys_addr;
}

__inline static gk_u32 get_high_addr(gk_phys_addr_t phys_addr)
{
    gk_u32 low_width = 32; /* 32bit low addr */
    return (sizeof(phys_addr) > (low_width / 8)) ? (gk_u32)(phys_addr >> low_width) : 0; /* 8 bits per byte */
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __GK_MATH_H__ */
