/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __JPEGD_ERROR_H__
#define __JPEGD_ERROR_H__

#include <common.h>

/* for SOF error */
#define GK_ERR_NOT_BASELINE        0x00000001
#define GK_UNSUPPORT_GRAY_PIC      0x00000002
#define GK_UNSUPPORT_PIC_SIZE      0x00000004
#define GK_UNSUPPORT_PIC_STRUCT    0x00000008
#define GK_UNSUPPORT_YUV411        0x00000010

#define GK_ERR_QUANT_TABLE         0x00000020
#define GK_ERR_HUFFMAN_TABLE       0x00000040
#define GK_ERR_FRAME_HEADER        0x00000080
#define GK_ERR_SCAN_HEADER         0x00000100
#define GK_ERR_RESTART_ERROR       0x00000200
#define GK_ERR_UNSUPPORT_DNL       0x00000400

#define gk_trace(fmt...)                             \
    do {                                             \
        printf("[%s]-%d: ", __FUNCTION__, __LINE__); \
        printf(fmt);                                 \
    } while (0)

#endif /* End of __JPEGD_ERROR_H__ */