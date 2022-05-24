/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */


#ifndef __HAL_VO_COMM_H__
#define __HAL_VO_COMM_H__

#include "hal_vo_def.h"
#include "inner_vo.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

#if VO_DESC("UBOOT_VO")

#define IO_ADDRESS(x) (x)

#ifdef GK_DEBUG
#define VO_HAL_CHECK_DEBUG /* vo hal check debug, debug only */
#endif
/* 4bytes */
#define REG_BYTES 4

#if VO_DESC("hal pub")
gk_void hal_vou_init(gk_void);
gk_void hal_write_reg(gk_u32 *address, gk_u32 value);
gk_u32 hal_read_reg(const gk_u32 *address);
#endif

#if VO_DESC("hal check")
gk_bool vo_hal_is_phy_dev(gk_vo_dev dev);
gk_bool vo_hal_is_video_layer(hal_disp_layer layer);
gk_bool vo_hal_is_gfx_layer(hal_disp_layer layer);
gk_bool vo_hal_is_video_gfx_layer(hal_disp_layer layer);
#endif

#ifdef VO_HAL_CHECK_DEBUG

#define vo_hal_check_phy_dev_return(dev)         \
    do {                                                    \
        if (vo_hal_is_phy_dev(dev) != GK_TRUE) {   \
            GK_PRINT("err dev in %s: L%d\n", __FUNCTION__, __LINE__); \
            return;          \
        }                     \
    } while (0)

#define vo_hal_check_video_layer_return(layer)         \
    do {                                                    \
        if (vo_hal_is_video_layer(layer) != GK_TRUE) {   \
            GK_PRINT("err layer in %s: L%d\n", __FUNCTION__, __LINE__); \
            return;          \
        }                     \
    } while (0)

#define vo_hal_check_gfx_layer_return(layer)         \
    do {                                                    \
        if (vo_hal_is_gfx_layer(layer) != GK_TRUE) {   \
            GK_PRINT("err layer in %s: L%d\n", __FUNCTION__, __LINE__); \
            return;          \
        }                     \
    } while (0)

#define vo_hal_check_video_gfx_layer_return(layer)         \
    do {                                                    \
        if (vo_hal_is_video_gfx_layer(layer) != GK_TRUE) {   \
            GK_PRINT("err layer in %s: L%d\n", __FUNCTION__, __LINE__); \
            return;          \
        }                     \
    } while (0)

#else

#define vo_hal_check_phy_dev_return(dev)
#define vo_hal_check_video_layer_return(layer)
#define vo_hal_check_gfx_layer_return(layer)
#define vo_hal_check_video_gfx_layer_return(layer)

#endif
#endif /* #if VO_DESC("UBOOT_VO") */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif /* end of __HAL_VO_COMM_H__ */
