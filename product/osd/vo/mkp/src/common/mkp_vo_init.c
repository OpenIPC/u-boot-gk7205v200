/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */


#include "mkp_vo_init.h"
#include "mkp_vo_dev.h"
#include "mkp_vo_gfx.h"
#include "mkp_vo_video.h"
#include "vo.h"
#include "common_vo.h"
#include "common_video.h"

#include "mkp_vo.h"
#include "sys_ext.h"

#include "board.h"
#include "drv_vo.h"

#if VO_DESC("clk init")
static gk_void vo_init_set_sys_clk(gk_void)
{
    vo_lpw_bus_reset(GK_FALSE);
}

static gk_void vo_init_crg_clk(gk_void)
{
    vo_drv_set_all_crg_clk(GK_TRUE);
}

static gk_void vo_init_board(gk_void)
{
    vo_drv_board_init();
}
#endif

#if VO_DESC("init context")

static gk_void vo_init_context(gk_void)
{
    vo_init_dev_info();

    vo_init_layer_resource();
}
#endif

#if VO_DESC("vo init flag")

static gk_bool g_is_initialized = GK_FALSE;
gk_bool vo_get_vo_init_flag(gk_void)
{
    return g_is_initialized;
}

static gk_void vo_set_vo_init_flag(gk_bool flag)
{
    g_is_initialized = flag;
}
#endif

#if VO_DESC("init operation")

gk_s32 vo_init(gk_void)
{
    gk_vo_dev dev;
    gk_vo_layer layer;
    if (vo_get_vo_init_flag() == GK_FALSE) {
        /* unreset */
        vo_init_set_sys_clk();

        /* clk enable */
        vo_init_crg_clk();

        /* step1: init drv context */
        vo_init_context();

        /* step2: init mkp context */
        for (dev = 0; dev < GK_VO_MAX_PHYS_DEV_NUM; dev++) {
            vo_init_dev_ctx(dev);
        }
        for (layer = 0; layer < GK_VO_MAX_PHYS_VIDEO_LAYER_NUM; layer++) {
            vo_init_layer_ctx(layer);
        }

        /* step3: init mkp gfx context */
        vo_init_gfx();

        /* hardware init */
        vo_init_board();

        vo_set_vo_init_flag(GK_TRUE);
    }

    return GK_SUCCESS;
}
#endif
