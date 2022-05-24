/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */


#include "hal_vo_comm.h"
#include "drv_vo.h"
#include "hal_vo.h"
#include "board.h"
#include <common.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

#if VO_DESC("UBOOT_VO")
#if VO_DESC("hal pub")

gk_void hal_vou_init(gk_void)
{
    volatile reg_vdp_regs *reg = vo_hal_get_reg();
    if (reg == GK_NULL) {
        reg = (volatile reg_vdp_regs *)IO_ADDRESS(VOU_REGS_ADDR);
        vo_hal_set_reg(reg);
    }

    if (reg == GK_NULL) {
        GK_PRINT("ioremap failed\n");
    }
}

gk_void hal_write_reg(gk_u32 *address, gk_u32 value)
{
    *(volatile gk_u32 *)address = value;
}

gk_u32 hal_read_reg(const gk_u32 *address)
{
    return *(volatile gk_u32 *)(address);
}

#endif
#if VO_DESC("hal check")

gk_bool vo_hal_is_phy_dev(gk_vo_dev dev)
{
    if ((dev >= DEV_PHY_START) && (dev <= DEV_PHY_END)) {
        return GK_TRUE;
    }

    return GK_FALSE;
}

gk_bool vo_hal_is_video_layer(hal_disp_layer layer)
{
    if ((layer >= LAYER_VID_START) && (layer <= LAYER_VID_END)) {
        return GK_TRUE;
    }

    return GK_FALSE;
}

gk_bool vo_hal_is_gfx_layer(hal_disp_layer layer)
{
    if ((layer >= LAYER_GFX_START) && (layer <= LAYER_GFX_END)) {
        return GK_TRUE;
    }

    return GK_FALSE;
}

#endif

#ifdef VO_HAL_CHECK_DEBUG

gk_bool vo_hal_is_video_gfx_layer(hal_disp_layer layer)
{
    if (vo_hal_is_video_layer(layer) || vo_hal_is_gfx_layer(layer)) {
        return GK_TRUE;
    }

    return GK_FALSE;
}

#endif

#endif /* #if VO_DESC("UBOOT_VO") */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */
