/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __SYS_EXT_H__
#define __SYS_EXT_H__

#include "type.h"
#include "common_video.h"
#include "sys_drv.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

typedef enum {
    SYS_VO_PLL_POSTDIV2_SET,
    SYS_VO_PLL_POSTDIV1_SET,
    SYS_VO_PLL_FRAC_SET,
    SYS_VO_PLL_REFDIV_SET,
    SYS_VO_PLL_FBDIV_SET,
    SYS_VO_PLL_POWER_CTRL,
    SYS_VO_BUS_RESET_SEL,
    SYS_VO_CFG_CLK_EN,
    SYS_VO_APB_CLK_EN,
    SYS_VO_BUS_CLK_EN,
    SYS_VO_HD_CLKOUT_PHASIC_REVERSE_EN,
    SYS_VO_OUT_DLY_TUNE,
    SYS_VO_OUT_HD_DIV,
    SYS_VO_HD0_DIV_MOD,
    SYS_VO_HD_CLK_SEL,
    SYS_VO_CORE_CLK_EN,
    SYS_VO_DEV_CLK_EN,
    SYS_VO_HD1_DIV_MOD,
    SYS_VO_SD_CLK_DIV,
    SYS_VO_SD_CLK_SEL,
    SYS_VO_BT1120_CLK_EN,
    SYS_VO_BT1120_CLK_SEL,
    SYS_VO_DAC_CLK_PHASIC_REVERSE_EN,
    SYS_VO_HD_DAC_SEL,
    SYS_VO_DEV_DAC_EN,
    SYS_VO_HDMI_SSC_VDP_DIV,
    SYS_VO_HDMI_CLK_EN,
    SYS_VO_HDMI_CLK_SEL,

    SYS_BUTT,
} sys_func;

#define call_sys_drv_ioctrl(mpp_chn, func_id, io_args) \
    sys_drv_drv_ioctrl(mpp_chn, func_id, io_args);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif /* __SYS_EXT_H__ */
