/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __GK_TYPE_H__
#define __GK_TYPE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/* ----------------------------------------------*
 * const defination                             *
 * ---------------------------------------------- */
typedef enum {
    GK_FALSE = 0,
    GK_TRUE = 1,
} gk_bool;

#ifndef NULL
#define NULL       0L
#endif

#define GK_NULL    0L
#define GK_SUCCESS 0
#define GK_FAILURE (-1)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __GK_TYPE_H__ */
