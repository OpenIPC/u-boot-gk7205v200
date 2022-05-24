/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */


#include "mkp_vo_video.h"
#include "common_vo.h"
#include "mkp_vo.h"
#include "vo.h"
#include "sys_ext.h"

#include "mkp_vo_comm.h"
#include "mkp_vo_dev.h"
#include "common.h"

#if VO_DESC("public api")
vo_layer_info g_vo_layer[GK_VO_MAX_LAYER_NUM];

vo_layer_info *vo_get_layer_ctx(gk_vo_layer vo_layer)
{
    return &(g_vo_layer[vo_layer]);
}

static gk_void vo_init_def_layer_ctx(gk_vo_layer layer, vo_layer_info *ctx)
{
    vou_drv_init_default_csc(layer);
    vou_drv_init_default_csc_param(layer);
    vou_drv_get_default_csc(layer, &ctx->layer_csc);
}

gk_s32 vo_init_layer_ctx(gk_vo_layer layer)
{
    gk_u32 layer_ctx_len;
    vo_layer_info *ctx = GK_NULL;

    ctx = vo_get_layer_ctx(layer);

    layer_ctx_len = sizeof(vo_layer_info);

    memset(ctx, 0, layer_ctx_len);
    vo_init_def_layer_ctx(layer, ctx);
    return GK_SUCCESS;
}

gk_void vo_init_layer_resource(gk_void)
{
    vo_drv_layer_info_init();
}

gk_vo_dev vo_get_video_layer_bind_dev(gk_vo_layer layer)
{
    return vou_drv_get_video_layer_bind_dev(layer);
}

gk_s32 vo_check_layer_binded_dev(gk_vo_layer layer, gk_vo_dev dev)
{
    if (dev == GK_INVALID_DEV) {
        return GK_ERR_VO_NOT_BINDED;
    }

    if (vo_def_check_dev_id(dev) != GK_SUCCESS) {
        vo_warn_trace("vo layer %d binded dev %d is invalid!\n", layer, dev);
        return GK_ERR_VO_INVALID_DEV_ID;
    }

    return GK_SUCCESS;
}

gk_bool vo_is_layer_enabled(gk_vo_layer layer)
{
    vo_layer_info *ctx = GK_NULL;

    ctx = vo_get_layer_ctx(layer);
    if (ctx == GK_NULL) {
        return GK_FALSE;
    }

    return ctx->video_enable;
}

gk_bool vo_is_dev_layer_enabled(gk_vo_dev dev)
{
    gk_u32 layer_num;
    gk_u32 loop;
    gk_vo_layer layer[GK_VO_MAX_LAYER_IN_DEV] = {0};

    layer_num = vou_drv_get_dev_layer_num(dev);
    if (layer_num == 0) {
        return GK_FALSE;
    }
    vou_drv_get_dev_layer(dev, &(layer[0]), GK_VO_MAX_LAYER_IN_DEV);
    for (loop = 0; loop < layer_num; loop++) {

        if (vo_is_layer_enabled(layer[loop]) == GK_TRUE) {
            vo_err_trace("vo %d's video layer %d doesn't disable!\n", dev, layer[loop]);
            return GK_TRUE;
        }
    }

    return GK_FALSE;
}

#endif

#if VO_DESC("enable video layer")
gk_bool vo_is_layer_configured(gk_vo_layer layer)
{
    vo_layer_info *ctx = GK_NULL;

    ctx = vo_get_layer_ctx(layer);
    if (ctx == GK_NULL) {
        vo_err_trace("vo layer %d ctx is GK_NULL!\n", layer);
        return GK_FALSE;
    }

    return ctx->video_config;
}

static gk_s32 vo_check_enable_video_layer(gk_vo_layer layer)
{
    gk_vo_dev dev;

    if (vo_is_layer_configured(layer) != GK_TRUE) {
        vo_err_trace("layer%d's video layer hasn't configured!\n", layer);
        return GK_ERR_VO_NOT_CFG;
    }

    dev = vo_get_video_layer_bind_dev(layer);
    if (vo_check_layer_binded_dev(layer, dev) != GK_SUCCESS) {
        vo_err_trace("layer(%d) is not bind any dev!\n", layer);
        return GK_ERR_VO_NOT_BINDED;
    }

    if (vo_is_dev_enabled(dev) != GK_TRUE) {
        vo_err_trace("vo device %d doesn't enable!\n", dev);
        return GK_ERR_VO_NOT_ENABLE;
    }

    return GK_SUCCESS;
}

static gk_s32 vo_do_enable_video_layer(gk_vo_layer layer, const gk_vo_video_layer_attr *video_attr)
{
    gk_s32 ret;
    vo_layer_info *layer_ctx = GK_NULL;

    layer_ctx = vo_get_layer_ctx(layer);
    if (layer_ctx->video_enable == GK_TRUE) {
        vo_warn_trace("layer%d's video layer has been enabled!\n", layer);
        return GK_SUCCESS;
    }

    ret = vo_check_enable_video_layer(layer);
    if (ret != GK_SUCCESS) {
        return ret;
    }

    layer_ctx->video_enable = GK_TRUE;

    vo_drv_layer_open(layer, video_attr);

    return GK_SUCCESS;
}

gk_s32 vo_enable_video_layer(gk_vo_layer layer, const gk_vo_video_layer_attr *video_attr)
{
    gk_s32 ret;

    vo_check_layer_id_return(layer);
    vo_check_gfx_layer_return(layer);

    ret = vo_do_enable_video_layer(layer, video_attr);
    return ret;
}

#endif

#if VO_DESC("disable video layer")
gk_void vo_reset_layer_vars(gk_vo_layer layer)
{
    vo_layer_info *layer_ctx = GK_NULL;

    layer_ctx = vo_get_layer_ctx(layer);

    vou_drv_init_default_csc(layer);
    vou_drv_init_default_csc_param(layer);

    vou_drv_get_default_csc(layer, &layer_ctx->layer_csc);
}

static gk_s32 vo_check_disable_video_layer(gk_vo_layer layer)
{
    gk_vo_dev dev;

    dev = vo_get_video_layer_bind_dev(layer);
    if (vo_check_layer_binded_dev(layer, dev) != GK_SUCCESS) {
        vo_err_trace("layer(%d) is not bind any dev!\n", layer);
        return GK_ERR_VO_NOT_BINDED;
    }

    return GK_SUCCESS;
}

static gk_s32 vo_do_disable_video_layer(gk_vo_layer layer)
{
    gk_s32 ret;
    gk_s32 dev;
    vo_layer_info *layer_ctx = GK_NULL;

    ret = vo_check_disable_video_layer(layer);
    if (ret != GK_SUCCESS) {
        return ret;
    }

    layer_ctx = vo_get_layer_ctx(layer);
    if (layer_ctx->video_enable == GK_FALSE) {
        return GK_SUCCESS;
    }

    dev = vou_drv_get_video_layer_bind_dev(layer);
    if (vo_is_phy_dev(dev)) {
        vou_drv_layer_enable(layer, GK_FALSE);
        vou_drv_set_layer_reg_up(layer);
        /* sleep: msleep(40); */
    }

    layer_ctx->video_enable = GK_FALSE;
    layer_ctx->video_config = GK_FALSE;
    vo_drv_disable_layer(layer);

    vo_reset_layer_vars(layer);

    return GK_SUCCESS;
}

gk_s32 vo_disable_video_layer(gk_vo_layer layer_id)
{
    vo_check_layer_id_return(layer_id);
    vo_check_gfx_layer_return(layer_id);

    return vo_do_disable_video_layer(layer_id);
}

#endif

#if VO_DESC("layer attr")

static gk_s32 vo_check_layer_attr_display_rect(gk_vo_dev dev, gk_vo_layer layer, const gk_rect *rect)
{
    gk_size dev_size;

    vo_get_dev_max_size(dev, &dev_size);
    if ((rect->x < 0) || (rect->y < 0) || (rect->width < VO_RESO_MIN_WDT) || (rect->height < VO_RESO_MIN_HGT) ||
        ((rect->width + rect->x) > dev_size.width) || ((rect->height + rect->y) > dev_size.height)) {
        vo_err_trace("layer(%d) display rect(%d, %d, %u, %u) is illegal, should between (%u, %u) and (%u, %u)\n",
            layer, rect->x, rect->y, rect->width, rect->height,
            VO_RESO_MIN_WDT, VO_RESO_MIN_HGT, dev_size.width, dev_size.height);
        return GK_ERR_VO_ILLEGAL_PARAM;
    }

    if (vo_check_rect_align(rect) != GK_SUCCESS) {
        vo_err_trace("layer(%d), display rect (%d, %d, %u, %u) dosen't aligned by 2 pixels!\n",
            layer, rect->x, rect->y, rect->width, rect->height);
        return GK_ERR_VO_ILLEGAL_PARAM;
    }

    return vo_drv_check_layer_attr_display_rect(layer, rect);
}

static gk_s32 vo_check_layer_attr_stride(gk_vo_layer layer, const gk_vo_video_layer_attr *video_attr)
{
    if (((video_attr->stride % VO_RECT_ALIGN) != 0)) {
        vo_err_trace("layer%d's stride %d is illegal!\n", layer, video_attr->stride);
        return GK_ERR_VO_ILLEGAL_PARAM;
    }

    if ((video_attr->address % VO_ADDR_ALIGN) != 0) {
        vo_err_trace("layer%d's addr 0x%lx is illegal, it should be %d bytes align!\n", layer, video_attr->address,
            VO_ADDR_ALIGN);
        return GK_ERR_VO_ILLEGAL_PARAM;
    }

    return GK_SUCCESS;
}

static gk_s32 vo_check_layer_attr(gk_vo_dev dev, gk_vo_layer layer, const gk_vo_video_layer_attr *video_attr)
{
    gk_s32 ret;
    ret = vo_check_layer_attr_display_rect(dev, layer, &video_attr->display_rect);
    if (ret != GK_SUCCESS) {
        return ret;
    }

    return vo_check_layer_attr_stride(layer, video_attr);
}

static gk_s32 vo_check_set_video_layer_attr(gk_vo_layer layer, const gk_vo_video_layer_attr *video_attr)
{
    gk_vo_dev dev;

    dev = vo_get_video_layer_bind_dev(layer);
    if (vo_check_layer_binded_dev(layer, dev) != GK_SUCCESS) {
        vo_err_trace("vo layer %d can't set layer attr when the layer has not been binded!\n", layer);
        return GK_ERR_VO_NOT_BINDED;
    }

    if (vo_is_dev_enabled(dev) != GK_TRUE) {
        vo_err_trace("vo layer %d bind dev %d doesn't enable!\n", layer, dev);
        return GK_ERR_VO_NOT_ENABLE;
    }

    return vo_check_layer_attr(dev, layer, video_attr);
}

static gk_bool vo_is_layer_only_pos_changed(gk_vo_layer layer, const gk_vo_video_layer_attr *video_attr_set)
{
    gk_vo_video_layer_attr video_attr_old;
    vo_layer_info *layer_ctx = GK_NULL;

    layer_ctx = vo_get_layer_ctx(layer);

    memcpy(&video_attr_old, &layer_ctx->vou_video_attr,
        sizeof(gk_vo_video_layer_attr));

    video_attr_old.display_rect.x = video_attr_set->display_rect.x;
    video_attr_old.display_rect.y = video_attr_set->display_rect.y;

    if (memcmp(&video_attr_old, video_attr_set, sizeof(gk_vo_video_layer_attr)) == 0) {
        return GK_TRUE;
    }
    return GK_FALSE;
}

static gk_s32 vo_set_layer_attr_with_layer_enable(gk_vo_layer layer, const gk_vo_video_layer_attr *video_attr)
{
    vo_layer_info *layer_ctx = GK_NULL;

    layer_ctx = vo_get_layer_ctx(layer);
    if ((vo_is_layer_only_pos_changed(layer, video_attr))) {
            memcpy(&layer_ctx->vou_video_attr, video_attr,
                sizeof(gk_vo_video_layer_attr));
            vo_drv_set_layer_attr(layer, video_attr);
            return GK_SUCCESS;
    }

    vo_err_trace("layer(%d): change layer attr, but layer doesn't disabled!\n", layer);
    return GK_ERR_VO_NOT_DISABLE;
}

static gk_void vo_set_layer_attr_with_layer_disable(gk_vo_layer layer, const gk_vo_video_layer_attr *video_attr)
{
    vo_layer_info *layer_ctx = GK_NULL;

    layer_ctx = vo_get_layer_ctx(layer);

    memcpy(&layer_ctx->vou_video_attr, video_attr, sizeof(gk_vo_video_layer_attr));
    layer_ctx->video_config = GK_TRUE;
}

static gk_void vo_set_layer_attr_in_drv(gk_vo_layer layer, const gk_vo_video_layer_attr *video_attr)
{
    gk_vo_dev dev;
    vo_layer_info *layer_ctx = GK_NULL;
    csc_coef_param csc_param;

    dev = vou_drv_get_video_layer_bind_dev(layer);
    layer_ctx = vo_get_layer_ctx(layer);

    vo_drv_set_layer_attr(layer, video_attr);

    if (vo_is_phy_dev(dev)) {
        vou_drv_get_default_csc(layer, &layer_ctx->layer_csc);
        vou_drv_get_layer_csc_param(layer, &csc_param);
        vou_drv_layer_csc_config(layer, &layer_ctx->layer_csc, &csc_param);
        vou_drv_layer_csc_enable(layer, GK_TRUE);
    }
}

static gk_s32 vo_do_set_video_layer_attr(gk_vo_layer layer, const gk_vo_video_layer_attr *video_attr)
{
    gk_s32 ret;

    ret = vo_check_set_video_layer_attr(layer, video_attr);
    if (ret != GK_SUCCESS) {
        return ret;
    }

    if (vo_is_layer_enabled(layer)) {
        ret = vo_set_layer_attr_with_layer_enable(layer, video_attr);
        return ret;
    }

    vo_set_layer_attr_with_layer_disable(layer, video_attr);
    vo_set_layer_attr_in_drv(layer, video_attr);

    return GK_SUCCESS;
}

gk_s32 vo_set_video_layer_attr(gk_vo_layer layer, const gk_vo_video_layer_attr *video_attr)
{
    gk_s32 ret;

    vo_check_null_ptr_return(video_attr);
    vo_check_layer_id_return(layer);
    vo_check_gfx_layer_return(layer);

    ret = vo_do_set_video_layer_attr(layer, video_attr);
    return ret;
}

#endif

