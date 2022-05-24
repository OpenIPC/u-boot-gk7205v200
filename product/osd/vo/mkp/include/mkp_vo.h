/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */


#ifndef __MKP_VO_H__
#define __MKP_VO_H__

#include "common.h"
#include "common_video.h"
#include "common_vo.h"
#include "inner_vo.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

#define VO_LAYER_BUTT GK_VO_MAX_PHYS_LAYER_NUM

#define vo_err_trace(fmt, ...)                                                                            \
    GK_ERR_TRACE(GK_ID_VO, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__);

#define vo_warn_trace(fmt, ...)                                                                            \
    GK_WARN_TRACE(GK_ID_VO, "[Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__);

gk_s32 vo_def_check_dev_id(gk_vo_dev dev);
gk_s32 vo_def_check_layer_id(gk_vo_layer layer);
gk_bool vo_def_is_gfx_layer_id(gk_vo_layer layer);

#define vo_check_null_ptr_return(ptr)                              \
    do {                                                           \
        if ((ptr) == GK_NULL) {                                    \
            vo_err_trace("null ptr!\n");                           \
            return GK_ERR_VO_NULL_PTR;                             \
        }                                                          \
    } while (0)

/* vo device relative */
#define vo_check_dev_id_return(dev)                         \
    do {                                                    \
        if (vo_drv_check_dev_id(dev) != GK_SUCCESS) {       \
            vo_err_trace("dev %d invalid!\n", dev);         \
            return GK_ERR_VO_INVALID_DEV_ID;                \
        }                                                   \
    } while (0)

/* vo video relative */
#define vo_check_layer_id_return(layer)                   \
    do {                                                  \
        if (vo_drv_check_layer_id(layer) != GK_SUCCESS) { \
            vo_err_trace("layer %d invalid!\n", layer);   \
            return GK_ERR_VO_INVALID_LAYER_ID;            \
        }                                                 \
    } while (0)

#define vo_check_gfx_id_return(gfx_layer)                       \
    do {                                                        \
        if (vo_check_gfx_id(gfx_layer) != GK_SUCCESS) {         \
            vo_err_trace("gfx layer %d invalid!\n", gfx_layer); \
            return GK_ERR_VO_INVALID_LAYER_ID;                  \
        }                                                       \
    } while (0)

#define vo_check_gfx_layer_return(layer)                                     \
    do {                                                                     \
        if (vo_def_is_gfx_layer_id(layer)) {                                 \
            vo_err_trace("vo layer %d is gfx layer, not support!\n", layer); \
            return GK_ERR_VO_NOT_SUPPORT;                                    \
        }                                                                    \
    } while (0)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif /* end of #ifndef __MKP_VO_H__ */
