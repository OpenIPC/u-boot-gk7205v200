/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */


#include "hal_vo_gfx_comm.h"
#include "hal_vo.h"
#include "math.h"
#include "mkp_vo.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

#if VO_DESC("UBOOT_VO")
#if VO_DESC("gfx uboot only")
gk_void hal_gfx_set_layer_disp_rect(hal_disp_layer layer, gk_rect *rect)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile reg_g0_dfpos g0_dfpos;
    volatile reg_g0_dlpos g0_dlpos;
    volatile gk_ulong addr_reg;

    vo_hal_check_gfx_layer_return(layer);

    addr_reg = vou_get_abs_addr(layer, (gk_ulong)(gk_uintptr_t)&(vo_reg->g0_dfpos.u32));
    g0_dfpos.u32 = hal_read_reg((gk_u32 *)(gk_uintptr_t) addr_reg);
    g0_dfpos.bits.disp_xfpos = rect->x;
    g0_dfpos.bits.disp_yfpos = rect->y;
    hal_write_reg((gk_u32 *)(gk_uintptr_t)addr_reg, g0_dfpos.u32);

    addr_reg = vou_get_abs_addr(layer, (gk_ulong)(gk_uintptr_t)&(vo_reg->g0_dlpos.u32));
    g0_dlpos.u32 = hal_read_reg((gk_u32 *)(gk_uintptr_t)addr_reg);
    g0_dlpos.bits.disp_xlpos = rect->x + rect->width - 1;
    g0_dlpos.bits.disp_ylpos = rect->y + rect->height - 1;
    hal_write_reg((gk_u32 *)(gk_uintptr_t)addr_reg, g0_dlpos.u32);
}

gk_void hal_gfx_set_layer_video_rect(hal_disp_layer layer, gk_rect *rect)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile reg_g0_vfpos g0_vfpos;
    volatile reg_g0_vlpos g0_vlpos;
    volatile gk_ulong addr_reg;

    vo_hal_check_gfx_layer_return(layer);

    addr_reg = vou_get_abs_addr(layer, (gk_ulong)(gk_uintptr_t)&(vo_reg->g0_vfpos.u32));
    g0_vfpos.u32 = hal_read_reg((gk_u32 *)(gk_uintptr_t)addr_reg);
    g0_vfpos.bits.video_xfpos = rect->x;
    g0_vfpos.bits.video_yfpos = rect->y;
    hal_write_reg((gk_u32 *)(gk_uintptr_t)addr_reg, g0_vfpos.u32);

    addr_reg = vou_get_abs_addr(layer, (gk_ulong)(gk_uintptr_t)&(vo_reg->g0_vlpos.u32));
    g0_vlpos.u32 = hal_read_reg((gk_u32 *)(gk_uintptr_t)addr_reg);
    g0_vlpos.bits.video_xlpos = rect->x + rect->width - 1;
    g0_vlpos.bits.video_ylpos = rect->y + rect->height - 1;
    hal_write_reg((gk_u32 *)(gk_uintptr_t)addr_reg, g0_vlpos.u32);
}

gk_void hal_gfx_enable_layer(hal_disp_layer layer, gk_u32 enable)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile reg_g0_ctrl g0_ctrl;
    volatile gk_ulong addr_reg;

    vo_hal_check_gfx_layer_return(layer);
    addr_reg = vou_get_abs_addr(layer, (gk_ulong)(gk_uintptr_t)&(vo_reg->g0_ctrl.u32));
    g0_ctrl.u32 = hal_read_reg((gk_u32 *)(gk_uintptr_t)addr_reg);
    g0_ctrl.bits.surface_en = enable;
    hal_write_reg((gk_u32 *)(gk_uintptr_t)addr_reg, g0_ctrl.u32);
}

gk_void hal_gfx_set_layer_data_fmt(hal_disp_layer layer, hal_disp_pixel_format data_fmt)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile reg_gfx_src_info gfx_src_info;
    volatile gk_ulong addr_reg;

    vo_hal_check_gfx_layer_return(layer);
    addr_reg = vou_get_gfx_abs_addr(layer, (gk_ulong)(gk_uintptr_t)&(vo_reg->gfx_src_info.u32));
    gfx_src_info.u32 = hal_read_reg((gk_u32 *)(gk_uintptr_t)addr_reg);
    gfx_src_info.bits.ifmt = data_fmt;
    hal_write_reg((gk_u32 *)(gk_uintptr_t)addr_reg, gfx_src_info.u32);
}

gk_void hal_gfx_set_layer_in_rect(hal_disp_layer layer, gk_rect *rect)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    reg_gfx_ireso gfx_ireso;
    volatile gk_ulong addr_reg;

    vo_hal_check_gfx_layer_return(layer);
    addr_reg = vou_get_gfx_abs_addr(layer, (gk_ulong)(gk_uintptr_t)&(vo_reg->gfx_ireso.u32));
    gfx_ireso.u32 = hal_read_reg((gk_u32 *)(gk_uintptr_t)addr_reg);
    gfx_ireso.bits.ireso_w = rect->width - 1;
    gfx_ireso.bits.ireso_h = rect->height - 1;
    hal_write_reg((gk_u32 *)(gk_uintptr_t)addr_reg, gfx_ireso.u32);
}

gk_bool hal_gfx_set_reg_up(hal_disp_layer layer)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    reg_g0_upd g0_upd;
    volatile gk_ulong addr_reg;

    switch (layer) {
        case HAL_DISP_LAYER_GFX0:
        case HAL_DISP_LAYER_GFX1:
        case HAL_DISP_LAYER_GFX2:
        case HAL_DISP_LAYER_GFX3:
            addr_reg = vou_get_abs_addr(layer, (gk_ulong)(gk_uintptr_t)&(vo_reg->g0_upd.u32));
            g0_upd.u32 = hal_read_reg((gk_u32 *)(gk_uintptr_t)addr_reg);
            /* graphic layer register update */
            g0_upd.bits.regup = 0x1;
            hal_write_reg((gk_u32 *)(gk_uintptr_t)addr_reg, g0_upd.u32);
            break;

        default:
            vo_err_trace("err layer id %d\n", layer);
            return GK_FALSE;
    }

    return GK_TRUE;
}

gk_void hal_gfx_set_gfx_pre_mult(hal_disp_layer layer, gk_u32 enable)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile reg_gfx_out_ctrl gfx_out_ctrl;
    volatile gk_ulong addr_reg;

    vo_hal_check_gfx_layer_return(layer);
    addr_reg = vou_get_gfx_abs_addr(layer, (gk_ulong)(gk_uintptr_t)&(vo_reg->gfx_out_ctrl.u32));
    gfx_out_ctrl.u32 = hal_read_reg((gk_u32 *)(gk_uintptr_t)addr_reg);
    gfx_out_ctrl.bits.premulti_en = enable;
    hal_write_reg((gk_u32 *)(gk_uintptr_t)addr_reg, gfx_out_ctrl.u32);
}

gk_void hal_gfx_set_gfx_addr(hal_disp_layer layer, gk_phys_addr_t l_addr)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile gk_ulong ul_gfx_addr_h;
    volatile gk_ulong ul_gfx_addr_l;

    vo_hal_check_gfx_layer_return(layer);
    // write low address to register.
    ul_gfx_addr_l = vou_get_gfx_abs_addr(layer, (gk_ulong)(gk_uintptr_t)&(vo_reg->gfx_addr_l));
    hal_write_reg((gk_u32 *)(gk_uintptr_t)ul_gfx_addr_l, get_low_addr(l_addr));
    // write high address to register.
    ul_gfx_addr_h = vou_get_gfx_abs_addr(layer, (gk_ulong)(gk_uintptr_t)&(vo_reg->gfx_addr_h));
    hal_write_reg((gk_u32 *)(gk_uintptr_t)ul_gfx_addr_h, get_high_addr(l_addr));
}

gk_void hal_gfx_set_gfx_stride(hal_disp_layer layer, gk_u16 pitch)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    volatile reg_gfx_stride gfx_stride;
    volatile gk_ulong addr_reg;

    vo_hal_check_gfx_layer_return(layer);
    addr_reg = vou_get_gfx_abs_addr(layer, (gk_ulong)(gk_uintptr_t)&(vo_reg->gfx_stride.u32));
    gfx_stride.u32 = hal_read_reg((gk_u32 *)(gk_uintptr_t)addr_reg);
    gfx_stride.bits.surface_stride = pitch;
    hal_write_reg((gk_u32 *)(gk_uintptr_t)addr_reg, gfx_stride.u32);
}

gk_void hal_gfx_set_src_resolution(hal_disp_layer layer, gk_rect *rect)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    reg_gfx_src_reso gfx_src_reso;
    volatile gk_ulong addr_reg;

    vo_hal_check_gfx_layer_return(layer);
    addr_reg = vou_get_gfx_abs_addr(layer, (gk_ulong)(gk_uintptr_t)&(vo_reg->gfx_src_reso.u32));
    gfx_src_reso.u32 = hal_read_reg((gk_u32 *)(gk_uintptr_t)addr_reg);
    gfx_src_reso.bits.src_w = rect->width - 1;
    gfx_src_reso.bits.src_h = rect->height - 1;
    hal_write_reg((gk_u32 *)(gk_uintptr_t)addr_reg, gfx_src_reso.u32);
}
#endif

#if VO_DESC("gfx")
gk_void hal_gfx_set_pixel_alpha_range(hal_disp_layer layer, gk_u32 arange)
{
    volatile reg_vdp_regs *vo_reg = vo_hal_get_reg();
    reg_gfx_out_ctrl gfx_out_ctrl;
    volatile gk_ulong addr_reg;

    vo_hal_check_gfx_layer_return(layer);
    addr_reg = vou_get_gfx_abs_addr(layer, (gk_ulong)(gk_uintptr_t)&(vo_reg->gfx_out_ctrl.u32));
    gfx_out_ctrl.u32 = hal_read_reg((gk_u32 *)(gk_uintptr_t)addr_reg);
    gfx_out_ctrl.bits.palpha_range = arange;
    hal_write_reg((gk_u32 *)(gk_uintptr_t)addr_reg, gfx_out_ctrl.u32);
}
#endif
#endif /* #if VO_DESC("UBOOT_VO") */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */
