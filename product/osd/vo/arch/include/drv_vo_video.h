/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */


#ifndef __DRV_VO_VIDEO_H__
#define __DRV_VO_VIDEO_H__

#include "drv_vo_video_comm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

#if VO_DESC("UBOOT_VO")

#if VO_DESC("layer")

typedef struct {
    gk_u32 vid_iw;
    gk_u32 vid_ih;
    gk_u32 vid_ow;
    gk_u32 vid_oh;

    gk_u32 zme_en;
    gk_u32 hfir_en;
    gk_u32 csc_en;
    gk_u32 hdr_en;
} vdp_vid_ip_cfg;

typedef struct {
    gk_u32 zme_vprec;
    gk_u32 zme_hprec;
} vo_zme_ds_info;

#endif

#if VO_DESC("layer zme")
hal_disp_layer vou_drv_convert_layer(gk_vo_layer layer);
#endif

#if VO_DESC("layer")
gk_void vo_drv_get_zme_comm_pq_cfg(vo_zme_ds_info *ds_info,
                                   vo_zme_mode zme_mode,
                                   vo_zme_comm_pq_cfg *comm_pq_cfg);

gk_void vo_drv_get_cvfir_pq_cfg(vo_zme_ds_info *ds_info,
                                vo_zme_mode zme_mode,
                                vo_zme_comm_pq_cfg *comm_pq_cfg);
gk_void vo_drv_layer_info_init(gk_void);
gk_void vo_drv_video_set_zme_enable(gk_u32 layer, vdp_vid_ip_cfg *vid_cfg);
#endif

gk_void vou_drv_layer_csc_enable(gk_vo_layer layer, gk_bool csc_en);
gk_void vo_drv_get_layer_csc_coef_range(gk_s32 *min_coef, gk_s32 *max_coef);
#if VO_DESC("layer")
gk_s32 vo_drv_check_video_layer_id(gk_vo_layer layer);
gk_s32 vo_drv_check_layer_attr_display_rect(gk_vo_layer layer, const gk_rect *rect);
#endif

#if VO_DESC("layer open")
gk_void vo_drv_layer_open(gk_vo_layer layer_id, const gk_vo_video_layer_attr *video_attr);
gk_void vo_drv_layer_close(gk_vo_layer layer_id);
#endif

#endif /* #if VO_DESC("UBOOT_VO") */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif /* end of __DRV_VO_VIDEO_H__ */
