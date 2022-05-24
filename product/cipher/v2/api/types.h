/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __GK_TYPE_H__
#define __GK_TYPE_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/*--------------------------------------------------------------------------------------------------------------*
 * Defintion of basic data types. The data types are applicable to both the application layer and kernel codes. *                                             *
 *--------------------------------------------------------------------------------------------------------------*/
/*************************** Structure Definition ****************************/
/** \addtogroup      Common_TYPE */
/** @{ */  /** <!-- [Common_TYPE] */

typedef unsigned char           gk_u8;
typedef unsigned char           gk_uchar;
typedef unsigned short          gk_u16;
typedef unsigned int            gk_u32;
typedef unsigned long           gk_ulong;

typedef signed char             gk_s8;
typedef short                   gk_s16;
typedef int                     gk_s32;
typedef long                    gk_slong;

#ifndef _M_IX86
typedef unsigned long long      gk_u64;
typedef long long               gk_s64;
#else
typedef __int64                 gk_u64;
typedef __int64                 gk_s64;
#endif

typedef char                    gk_char;
typedef char*                   gk_pchar;

typedef unsigned long           gk_ul;
typedef signed long             gk_sl;

typedef float                   gk_float;
typedef double                  gk_double;
#define gk_void                 void

typedef unsigned long           gk_size_t;
typedef unsigned long           gk_length_t;

typedef gk_u32                  gk_handle;

typedef unsigned int            gk_phys_addr_t;

#ifdef CONFIG_ARCH_LP64_MODE
typedef unsigned long long      gk_virt_addr_t;
#else
typedef unsigned int            gk_virt_addr_t;
#endif

/** Constant Definition */
typedef enum {
    GK_FALSE    = 0,
    GK_TRUE     = 1,
} gk_bool;

#ifndef NULL
#define NULL                0L
#endif

#define GK_NULL             0L
#define GK_NULL_PTR         0L

#define GK_SUCCESS          0
#define GK_FAILURE          (-1)

#define GK_INVALID_HANDLE   0xffffffff
#define GK_INVALID_PTS      0xffffffff
#define GK_INVALID_TIME     0xffffffff

#define GK_OS_LINUX         0xabcd
#define GK_OS_WIN32         0xcdef

#ifdef _WIN32
#define GK_OS_TYPE          GK_OS_WIN32
#else
#define __OS_LINUX__
#define GK_OS_TYPE          GK_OS_LINUX
#endif

#ifdef GK_ADVCA_SUPPORT
#define __INIT__
#define __EXIT__
#else
#define __INIT__  __init
#define __EXIT__  __exit
#endif

/**

define of gk_handle :
bit31                                                           bit0
  |<----   16bit --------->|<---   8bit    --->|<---  8bit   --->|
  |--------------------------------------------------------------|
  |      GK_MOD_ID_E       |  mod defined data |     chnID       |
  |--------------------------------------------------------------|

mod defined data: private data define by each module(for example: sub-mod id), usually, set to 0.
*/

#define gk_handle_make_handle(mod, privatedata, chnid) \
    (gk_handle)((((mod) & 0xffff) << 16) | ((((privatedata) & 0xff) << 8)) | (((chnid) & 0xff)))

#define handle_get_mod_id(handle)     (((handle) >> 16) & 0xffff)
#define handle_get_pri_data(handle)   (((handle) >> 8) & 0xff)
#define handle_get_chn_id(handle)     (((handle)) & 0xff)

/** @} */  /** <!-- ==== Structure Definition end ==== */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __GK_TYPE_H__ */

