/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __GK_DEBUG_H__
#define __GK_DEBUG_H__

#include "type.h"
#include "common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define GK_DBG_EMERG      0   /* system is unusable                   */
#define GK_DBG_ALERT      1   /* action must be taken immediately     */
#define GK_DBG_CRIT       2   /* critical conditions                  */
#define GK_DBG_ERR        3   /* error conditions                     */
#define GK_DBG_WARN       4   /* warning conditions                   */
#define GK_DBG_NOTICE     5   /* normal but significant condition     */
#define GK_DBG_INFO       6   /* informational                        */
#define GK_DBG_DEBUG      7   /* debug-level messages                 */

#define GK_PRINT      printf

#define GK_ASSERT(expr)
#define GK_TRACE(level, enModId, fmt...)

#define GK_ERR_TRACE(mod, fmt...) printf(fmt)
#define GK_WARN_TRACE(mod, fmt...)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __GK_DEBUG_H__ */
