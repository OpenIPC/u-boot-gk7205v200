/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __VOU_H__
#define __VOU_H__

#include "gk7205v200_vo.h"

#ifdef __cplusplus
extern "C" {
#endif /* end of #ifdef __cplusplus */

int set_vobg(unsigned int dev, unsigned int rgb);
int start_vo(unsigned int dev, unsigned int type, unsigned int sync);
int stop_vo(unsigned int dev);
int start_videolayer(unsigned int layer, unsigned long addr, unsigned int strd, gk_vo_rect layer_rect);
int stop_videolayer(unsigned int layer);
int start_gx(unsigned int layer, unsigned long addr, unsigned int strd, gk_vo_rect gx_rect);
int stop_gx(unsigned int layer);

#ifdef __cplusplus
}
#endif /* end of #ifdef __cplusplus */

#endif /* end of #ifdef __VOU_H__ */
