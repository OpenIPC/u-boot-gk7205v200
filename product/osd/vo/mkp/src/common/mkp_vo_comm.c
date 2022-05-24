/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */


#include "mkp_vo_comm.h"
#include "vo.h"
#include "common_vo.h"
#include "common_video.h"

#include "mkp_vo.h"
#include "sys_ext.h"

#include "board.h"
#include "drv_vo.h"

#if VO_DESC("dev check")

gk_s32 vo_def_check_dev_id(gk_vo_dev dev)
{
    if ((dev < 0) || (dev >= GK_VO_MAX_PHYS_DEV_NUM)) {
        return GK_ERR_VO_INVALID_DEV_ID;
    }

    return GK_SUCCESS;
}

gk_s32 vo_def_check_layer_id(gk_vo_layer layer)
{
    if ((layer < 0) || (layer >= GK_VO_MAX_LAYER_NUM)) {
        return GK_ERR_VO_INVALID_LAYER_ID;
    }

    return GK_SUCCESS;
}

gk_bool vo_def_is_gfx_layer_id(gk_vo_layer layer)
{
    if ((layer >= GK_VO_LAYER_G0) && (layer < VO_LAYER_BUTT)) {
        return GK_TRUE;
    }

    return GK_FALSE;
}

#endif

#if VO_DESC("check attr pub")

gk_s32 vo_check_bool_value(gk_bool bool_val)
{
    if ((bool_val != GK_TRUE) && (bool_val != GK_FALSE)) {
        return GK_ERR_VO_ILLEGAL_PARAM;
    }

    return GK_SUCCESS;
}

gk_s32 vo_check_rect_align(const gk_rect *rect)
{
    if ((rect->x % VO_RECT_ALIGN) || (rect->y % VO_RECT_ALIGN) ||
        (rect->width % VO_RECT_ALIGN) || (rect->height % VO_RECT_ALIGN)) {
        return GK_ERR_VO_ILLEGAL_PARAM;
    }

    return GK_SUCCESS;
}
#endif

#if VO_DESC("check dev pub")

gk_bool vo_is_phy_dev(gk_vo_dev dev)
{
    return GK_TRUE;
}

gk_bool vo_is_typical_intf_sync(gk_vo_intf_sync intf_sync)
{
    if ((intf_sync < GK_VO_OUT_BUTT) && (intf_sync != GK_VO_OUT_USER)) {
        return GK_TRUE;
    }

    return GK_FALSE;
}

gk_bool vo_is_user_intf_sync(gk_vo_intf_sync intf_sync)
{
    if (intf_sync == GK_VO_OUT_USER) {
        return GK_TRUE;
    }

    return GK_FALSE;
}

#endif
