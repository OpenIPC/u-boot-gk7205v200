/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */


#ifndef __MKP_VO_COMM_H__
#define __MKP_VO_COMM_H__

#include "inner_vo.h"

#define VO_RECT_INTERLACED_ALIGN  4

#if VO_DESC("check attr pub")
gk_s32 vo_check_bool_value(gk_bool bool_val);
gk_s32 vo_check_rect_align(const gk_rect *rect);
gk_s32 vo_check_csc_no_matrix(const gk_vo_csc *csc);
#endif

#if VO_DESC("check dev pub")
gk_bool vo_is_phy_dev(gk_vo_dev dev);
gk_bool vo_is_virt_dev(gk_vo_dev dev);
gk_bool vo_is_cas_dev(gk_vo_dev dev);
gk_bool vo_is_typical_intf_sync(gk_vo_intf_sync intf_sync);
gk_bool vo_is_user_intf_sync(gk_vo_intf_sync intf_sync);

#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif /* end of #ifndef __MKP_VO_COMM_H__ */
