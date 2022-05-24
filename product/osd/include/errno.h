/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __GK_ERRNO_H__
#define __GK_ERRNO_H__

#include "debug.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

#define GK_ERR_APP_ID (0x80000000L + 0x20000000L)

#define GK_DEFINE_ERR(mod, level, err_id) \
    ((gk_s32)((GK_ERR_APP_ID) | ((mod) << 16) | ((level) << 13) | (err_id)))

typedef enum {
    GK_ERR_LEVEL_DEBUG = 0,  /* debug-level                                  */
    GK_ERR_LEVEL_INFO,       /* informational                                */
    GK_ERR_LEVEL_NOTICE,     /* normal but significant condition             */
    GK_ERR_LEVEL_WARNING,    /* warning conditions                           */
    GK_ERR_LEVEL_ERROR,      /* error conditions                             */
    GK_ERR_LEVEL_CRIT,       /* critical conditions                          */
    GK_ERR_LEVEL_ALERT,      /* action must be taken immediately             */
    GK_ERR_LEVEL_FATAL,      /* just for compatibility with previous version */
    GK_ERR_LEVEL_BUTT
} gk_err_level;

typedef enum {
    GK_ERR_INVALID_DEV_ID  =  0x1, /* invlalid device ID                          */
    GK_ERR_INVALID_PIPE_ID =  0x2, /* invlalid pipe ID                           */
    GK_ERR_INVALID_CHN_ID  =  0x3, /* invlalid channel ID                          */
    GK_ERR_INVALID_LAYER_ID = 0x4, /* invlalid channel ID                          */

    GK_ERR_ILLEGAL_PARAM   = 0x7, /* at lease one parameter is illagal
                               * eg, an illegal enumeration value             */
    GK_ERR_EXIST           = 0x8, /* resource exists                              */
    GK_ERR_UNEXIST         = 0x9, /* resource unexists                            */
    GK_ERR_NULL_PTR        = 0xa, /* using a NULL point                           */
    GK_ERR_NOT_CFG         = 0xb, /* try to enable or initialize system, device
                              ** or channel, before configing attribute       */
    GK_ERR_NOT_SUPPORT     = 0xc, /* operation or type is not supported by NOW    */
    GK_ERR_NOT_PERM        = 0xd, /* operation is not permitted
                              ** eg, try to change static attribute           */
    GK_ERR_NOT_ENABLE      = 0x10,
    GK_ERR_NOT_DISABLE     = 0x11,
    GK_ERR_NOT_START       = 0x12,
    GK_ERR_NOT_STOP        = 0x13,
    GK_ERR_NO_MEM          = 0x14, /* failure caused by malloc memory              */
    GK_ERR_NO_BUF          = 0x15, /* failure caused by malloc buffer              */
    GK_ERR_BUF_EMPTY       = 0x16, /* no data in buffer                            */
    GK_ERR_BUF_FULL        = 0x17, /* no buffer for new data                       */
    GK_ERR_NOT_READY       = 0x18, /* System is not ready, maybe not initialed or
                              ** loaded. Returning the error code when opening
                              ** a device file failed.                        */
    GK_ERR_TIMEOUT         = 0x20,
    GK_ERR_BAD_ADDR        = 0x21, /* bad address,
                              ** eg. used for copy_from_user & copy_to_user   */
    GK_ERR_BUSY            = 0x22, /* resource is busy,
                              ** eg. destroy a venc chn without unregister it */
    GK_ERR_SIZE_NOT_ENOUGH = 0x23, /* buffer size is smaller than the actual size required */

    GK_ERR_NOT_BINDED      = 0x24,
    GK_ERR_BINDED          = 0x25,
    GK_ERR_BUTT            = 0x3f, /* maxium code, private error code of all modules
                              ** must be greater than it                      */
} gk_errno;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __GK_ERRNO_H__ */
