/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __GK_COMMON_H__
#define __GK_COMMON_H__

#include "type.h"
#include "defines.h"
#ifndef __KERNEL__
#include "securec.h"
#endif


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

#ifndef VER_X
#define VER_X                  1
#endif

#ifndef VER_Y
#define VER_Y                  0
#endif

#ifndef VER_Z
#define VER_Z                  0
#endif

#ifndef VER_P
#define VER_P                  0
#endif

#ifndef VER_B
#define VER_B                  0
#endif

#ifdef GK_DEBUG
#define VER_D                  " Debug"
#else
#define VER_D                  " Release"
#endif

#define GK_COMPAT_POINTER(ptr, type) \
    do { \
        gk_ulong ulAddr = (gk_ulong)ptr; \
        gk_u32 u32Addr = (gk_u32)ulAddr; \
        ptr = (type)(gk_ulong)u32Addr; \
    } while (0)

#define __MK_VERSION(x, y, z, p, b) #x "." #y "." #z "." #p " B0" #b
#define MK_VERSION(x, y, z, p, b) __MK_VERSION(x, y, z, p, b)
#define GK_MPP_VERSION            GK_CHIP_NAME GK_MPP_VER_PRIX MK_VERSION(VER_X, VER_Y, VER_Z, VER_P, VER_B) VER_D


#define GK_MAX_VERSION_NAME_LEN    64
#define GK_INVALID_CHN         (-1)
#define GK_INVALID_WAY         (-1)
#define GK_INVALID_LAYER       (-1)
#define GK_INVALID_DEV         (-1)
#define GK_INVALID_HANDLE      (-1)
#define GK_INVALID_VALUE       (-1)
#define GK_INVALID_TYPE        (-1)

#define GK_MPP_MOD_VI             "vi"
#define GK_MPP_MOD_VO             "vo"
#define GK_MPP_MOD_AVS            "avs"
#define GK_MPP_MOD_HDMI           "hdmi"
#define GK_MPP_MOD_VGS            "vgs"
#define GK_MPP_MOD_GDC            "gdc"
#define GK_MPP_MOD_DIS            "dis"
#define GK_MPP_MOD_GYRODIS        "gyrodis"
#define GK_MPP_MOD_CHNL           "chnl"
#define GK_MPP_MOD_VENC           "venc"
#define GK_MPP_MOD_VPSS           "vpss"
#define GK_MPP_MOD_RGN            "rgn"
#define GK_MPP_MOD_IVE            "ive"
#define GK_MPP_MOD_FD             "fd"
#define GK_MPP_MOD_MD             "md"
#define GK_MPP_MOD_IVP            "ivp"
#define GK_MPP_MOD_SVP            "svp"
#define GK_MPP_MOD_SVP_NNIE       "nnie"
#define GK_MPP_MOD_SVP_DSP        "dsp"
#define GK_MPP_MOD_SVP_ALG        "svp_alg"
#define GK_MPP_MOD_DPU_RECT       "rect"
#define GK_MPP_MOD_DPU_MATCH      "match"
#define GK_MPP_MOD_SVP_MAU        "mau"
#define GK_MPP_MOD_H264E          "h264e"
#define GK_MPP_MOD_H265E          "h265e"
#define GK_MPP_MOD_JPEGE          "jpege"
#define GK_MPP_MOD_MPEG4E         "mpeg4e"
#define GK_MPP_MOD_VEDU           "vedu"
#define GK_MPP_MOD_PRORES         "prores"
#define GK_MPP_MOD_VDEC           "vdec"
#define GK_MPP_MOD_H264D          "h264d"
#define GK_MPP_MOD_JPEGD          "jpegd"
#define GK_MPP_MOD_AI             "ai"
#define GK_MPP_MOD_AO             "ao"
#define GK_MPP_MOD_AENC           "aenc"
#define GK_MPP_MOD_ADEC           "adec"
#define GK_MPP_MOD_AIO            "aio"
#define GK_MPP_MOD_ACODEC         "acodec"
#define GK_MPP_MOD_VB             "vb"
#define GK_MPP_MOD_SYS            "sys"
#define GK_MPP_MOD_CMPI           "cmpi"
#define GK_MPP_MOD_PCIV           "pciv"
#define GK_MPP_MOD_PCIVFMW        "pcivfmw"
#define GK_MPP_MOD_PROC           "proc"
#define GK_MPP_MOD_LOG            "logmpp"
#define GK_MPP_MOD_FB             "fb"
#define GK_MPP_MOD_RC             "rc"
#define GK_MPP_MOD_TDE            "tde"
#define GK_MPP_MOD_ISP            "isp"
#define GK_MPP_MOD_USR            "usr"
#define GK_MPP_MOD_MCF            "mcf"
#define GK_MPP_MOD_PM             "pm"
#define GK_MPP_MOD_MFUSION        "motionfusion"
#define GK_MPP_MOD_VDA            "vda"

typedef struct {
    gk_char version[GK_MAX_VERSION_NAME_LEN];
} gk_mpp_version;

typedef gk_s32 gk_ai_chn;
typedef gk_s32 gk_ao_chn;
typedef gk_s32 gk_aenc_chn;
typedef gk_s32 gk_adec_chn;
typedef gk_s32 gk_audio_dev;
typedef gk_s32 gk_vi_dev;
typedef gk_s32 gk_vi_pipe;
typedef gk_s32 gk_vi_way;
typedef gk_s32 gk_vi_chn;
typedef gk_s32 gk_vo_dev;
typedef gk_s32 gk_vo_layer;
typedef gk_s32 gk_vo_chn;
typedef gk_s32 gk_vo_wbc;
typedef gk_s32 gk_gfx_layer;
typedef gk_s32 gk_venc_chn;
typedef gk_s32 gk_vdec_chn;
typedef gk_s32 gk_ive_handle;
typedef gk_s32 gk_fd_chn;
typedef gk_s32 gk_md_chn;
typedef gk_s32 gk_sensor_id;
typedef gk_s32 gk_mipi_dev;
typedef gk_s32 gk_slave_dev;
typedef gk_s32 gk_svp_nnie_handle;
typedef gk_s32 gk_svp_dsp_handle;
typedef gk_s32 gk_svp_alg_chn;
typedef gk_s32 gk_svp_mau_handle;
typedef gk_s32 gk_vpss_grp;
typedef gk_s32 gk_vpss_grp_pipe;
typedef gk_s32 gk_vpss_chn;
typedef gk_s32 gk_avs_grp;
typedef gk_s32 gk_avs_pipe;
typedef gk_s32 gk_avs_chn;
typedef gk_s32 gk_mcf_grp;
typedef gk_s32 gk_mcf_pipe;
typedef gk_s32 gk_mcf_chn;
typedef gk_s32 gk_vda_chn;
typedef gk_s32 gk_pciv_chn;

typedef enum {
    GK_ID_CMPI    = 0,
    GK_ID_VB      = 1,
    GK_ID_SYS     = 2,
    GK_ID_RGN      = 3,
    GK_ID_CHNL    = 4,
    GK_ID_VDEC    = 5,
    GK_ID_AVS     = 6,
    GK_ID_VPSS    = 7,
    GK_ID_VENC    = 8,
    GK_ID_SVP     = 9,
    GK_ID_H264E   = 10,
    GK_ID_JPEGE   = 11,
    GK_ID_H265E   = 13,
    GK_ID_JPEGD   = 14,
    GK_ID_VO      = 15,
    GK_ID_VI      = 16,
    GK_ID_DIS     = 17,
    GK_ID_VALG    = 18,
    GK_ID_RC      = 19,
    GK_ID_AIO     = 20,
    GK_ID_AI      = 21,
    GK_ID_AO      = 22,
    GK_ID_AENC    = 23,
    GK_ID_ADEC    = 24,
    GK_ID_VPU    = 25,
    GK_ID_PCIV    = 26,
    GK_ID_PCIVFMW = 27,
    GK_ID_ISP      = 28,
    GK_ID_IVE      = 29,
    GK_ID_USER    = 30,
    GK_ID_PROC    = 33,
    GK_ID_LOG     = 34,
    GK_ID_VFMW    = 35,
    GK_ID_GDC     = 37,
    GK_ID_PHOTO   = 38,
    GK_ID_FB      = 39,
    GK_ID_HDMI    = 40,
    GK_ID_VOIE    = 41,
    GK_ID_TDE     = 42,
    GK_ID_HDR      = 43,
    GK_ID_PRORES  = 44,
    GK_ID_VGS     = 45,
    GK_ID_FD      = 47,
    GK_ID_OD      = 48,
    GK_ID_LPR      = 50,
    GK_ID_SVP_NNIE     = 51,
    GK_ID_SVP_DSP      = 52,
    GK_ID_DPU_RECT     = 53,
    GK_ID_DPU_MATCH    = 54,

    GK_ID_MOTIONSENSOR = 55,
    GK_ID_MOTIONFUSION = 56,

    GK_ID_GYRODIS      = 57,
    GK_ID_PM           = 58,
    GK_ID_SVP_ALG      = 59,
	GK_ID_IVP          = 60,
	GK_ID_MCF          = 61,
    GK_ID_SVP_MAU      = 62,
    GK_ID_VDA     = 63,

    GK_ID_BUTT,
} gk_mod_id;

typedef struct {
    gk_mod_id    mod_id;
    gk_s32      dev_id;
    gk_s32      chn_id;
} gk_mpp_chn;

/* We just coyp this value of payload type from RTP/RTSP definition */
typedef enum {
        GK_PT_PCMU          = 0,
        GK_PT_1016          = 1,
        GK_PT_G721          = 2,
        GK_PT_GSM           = 3,
        GK_PT_G723          = 4,
        GK_PT_DVI4_8K       = 5,
        GK_PT_DVI4_16K      = 6,
        GK_PT_LPC           = 7,
        GK_PT_PCMA          = 8,
        GK_PT_G722          = 9,
        GK_PT_S16BE_STEREO  = 10,
        GK_PT_S16BE_MONO    = 11,
        GK_PT_QCELP         = 12,
        GK_PT_CN            = 13,
        GK_PT_MPEGAUDIO     = 14,
        GK_PT_G728          = 15,
        GK_PT_DVI4_3        = 16,
        GK_PT_DVI4_4        = 17,
        GK_PT_G729          = 18,
        GK_PT_G711A         = 19,
        GK_PT_G711U         = 20,
        GK_PT_G726          = 21,
        GK_PT_G729A         = 22,
        GK_PT_LPCM          = 23,
        GK_PT_CelB          = 25,
        GK_PT_JPEG          = 26,
        GK_PT_CUSM          = 27,
        GK_PT_NV            = 28,
        GK_PT_PICW          = 29,
        GK_PT_CPV           = 30,
        GK_PT_H261          = 31,
        GK_PT_MPEGVIDEO     = 32,
        GK_PT_MPEG2TS       = 33,
        GK_PT_H263          = 34,
        GK_PT_SPEG          = 35,
        GK_PT_MPEG2VIDEO    = 36,
        GK_PT_AAC           = 37,
        GK_PT_WMA9STD       = 38,
        GK_PT_HEAAC         = 39,
        GK_PT_PCM_VOICE     = 40,
        GK_PT_PCM_AUDIO     = 41,
        GK_PT_MP3           = 43,
        GK_PT_ADPCMA        = 49,
        GK_PT_AEC           = 50,
        GK_PT_X_LD          = 95,
        GK_PT_H264          = 96,
        GK_PT_D_GSM_HR      = 200,
        GK_PT_D_GSM_EFR     = 201,
        GK_PT_D_L8          = 202,
        GK_PT_D_RED         = 203,
        GK_PT_D_VDVI        = 204,
        GK_PT_D_BT656       = 220,
        GK_PT_D_H263_1998   = 221,
        GK_PT_D_MP1S        = 222,
        GK_PT_D_MP2P        = 223,
        GK_PT_D_BMPEG       = 224,
        GK_PT_MP4VIDEO      = 230,
        GK_PT_MP4AUDIO      = 237,
        GK_PT_VC1           = 238,
        GK_PT_JVC_ASF       = 255,
        GK_PT_D_AVI         = 256,
        GK_PT_DIVX3         = 257,
        GK_PT_AVS           = 258,
        GK_PT_REAL8         = 259,
        GK_PT_REAL9         = 260,
        GK_PT_VP6           = 261,
        GK_PT_VP6F          = 262,
        GK_PT_VP6A          = 263,
        GK_PT_SORENSON      = 264,
        GK_PT_H265          = 265,
        GK_PT_VP8           = 266,
        GK_PT_MVC           = 267,
        GK_PT_PNG           = 268,
        /* add by vendor */
        GK_PT_AMR           = 1001,
        GK_PT_MJPEG         = 1002,
        GK_PT_AMRWB         = 1003,
        GK_PT_PRORES        = 1006,
        GK_PT_OPUS          = 1007,
        GK_PT_BUTT
    } gk_payload_type;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

#endif /* __GK_COMMON_H__ */

