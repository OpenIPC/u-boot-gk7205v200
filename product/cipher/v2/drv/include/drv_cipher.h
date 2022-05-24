/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#ifndef __GK_DRV_CIPHER_H__
#define __GK_DRV_CIPHER_H__

#include "types.h"
#include "mpi_cipher.h"

#ifdef __cplusplus
extern "C" {
#endif    /* __cplusplus */

#define MAX_MULTI_PKG_NUM           128
#define CIPHER_SOFT_CHAN_NUM        8
#define CIPHER_INVALID_CHN          0xffffffff
#define CIPHER_MAX_NODE_BUF_SIZE    0xFFFF0   /* 1M-16 */
#define CIPHER_MAX_RSA_KEY_LEN      512
#define HASH_OUTPUT_SIZE_WORD       (1600 / 8 / 4)    /* sha3 state */

#define HDCP_KEY_RAM_SIZE           320
#define HDCP_KEY_PRIME_SIZE         320
#define HDCP_KEY_TOOL_FILE_SIZE     384
#define HDCP_KEY_CHIP_FILE_SIZE     (HDCP_KEY_RAM_SIZE + 12)

#define HASH_ALG_SHA2               0x01
#define HASH_ALG_SHA3               0x02
#define HASH_ALG_SM3                0x03

#define HASH_MODE_RAW               0x01
#define HASH_MODE_MAC               0x02
#define HASH_MODE_SHAKE             0x03

#define CPU_BIT_WIDTH_32            32
#define CPU_BIT_WIDTH_64            64
#define MY_CPU_BIT_WIDTH            sizeof(gk_size_t)

typedef enum {
    GK_DRV_HASH_STEP_UPDATE = 0x0,
    GK_DRV_HASH_STEP_INIT   = 0x01,
    GK_DRV_HASH_STEP_FINAL  = 0x02,
} gk_drv_hash_step_e;

typedef struct {
    gk_handle ci_handle;
    gk_cipher_attr cipher_atts;
} cipher_handle_s;

typedef struct {
    gk_handle ci_handle;
    gk_u32 src_phy_addr;
    gk_u32 src_phy_addr_high;
    gk_u32 dest_phy_addr;
    gk_u32 dest_phy_addr_high;
    gk_u32 data_length;
} cipher_data_s;

typedef struct {
    gk_handle ci_handle;
    gk_u32 pkg_num;
    gk_u32 user_bit_width;
    gk_cipher_data* cipher_data;
} cipher_pkg_s;

typedef struct {
    gk_handle ci_handle;
    gk_cipher_ctrl cipher_ctrl;
} cipher_config_ctrl_s;

typedef struct {
    gk_handle ci_handle;
    gk_u32 time_out;
} cipher_wait_done_s;

typedef struct {
    gk_handle ci_handle;
    gk_u32 key[12];                                 /* Key input, EK||AK||SK for SM1, 12 */
    gk_u32 odd_key[8];                              /* Key input, Old Key, 8 */
    gk_u32 iv[4];                                   /* Initialization vector (IV), 4 */
    /* Encryption using advanced conditional access (CA) or decryption using keys */
    gk_bool key_by_ca;
    gk_cipher_ca_type ca_type;                /* Select keyladder type when using advanced CA */
    gk_cipher_alg ci_alg;                     /* Cipher algorithm */
    gk_cipher_bit_width bit_width;            /* Bit width for encryption or decryption */
    gk_cipher_work_mode work_mode;            /* Operating mode */
    gk_cipher_key_len key_len;             /* Key length */
    /* control information exchange choices,
     * we default all woulde be change except they have been in the choices */
    gk_cipher_ctrl_chg_flag change_flags;
    gk_cipher_sm1_round sm1_round;            /* SM1 round number, should be 8, 10, 12 or 14 */
    /* IV lenght for CCM/GCM, which is an element of {4, 6, 8, 10, 12, 14, 16} for CCM,
     * and is an element of [1-16] for GCM */
    gk_u32 iv_len;
    /* Tag lenght for CCM which is an element of {4, 6, 8, 10, 12, 14, 16} */
    gk_u32 tag_len;
    gk_u32 alen;                                    /* Associated data for CCM and GCM */
    gk_u32 aphy_addr;
    gk_u32 aphy_addr_high;                          /* Physical address of Associated data for CCM and GCM */
} cipher_config_ctrl_ex_s;

typedef struct {
    gk_cipher_hash_type sha_type;
    gk_u32 hard_chn;
    gk_u32 sha_val[16]; /* 16 size */
    gk_u32 data_phy;
    gk_u32 data_phy_high;
    gk_u32 data_len;
} cipher_hash_data_s;

typedef struct {
    gk_cipher_hash_attr hash_attr;
    gk_handle hash_handle;
} cipher_hash_init_s;

typedef struct {
    gk_handle hash_handle;
    gk_u8 *input_data;
    gk_u32 input_data_len;
} cipher_hash_update_s;

typedef struct {
    gk_handle hash_handle;
    gk_u8 *output_hash;
} cipher_hash_finish_s;

typedef struct {
    gk_u32 time_out_us;
    gk_u32 ci_rng;
} cipher_rng_s;

typedef struct {
    gk_handle ci_handle;
    gk_u32 tag_len;
    gk_u32 tag[4]; /* 4 tag size */
} cipher_tag_s;

typedef struct {
    gk_u8 *input_data;
    gk_u8 *output_data;
    gk_u32 data_len;
    gk_u8 *rsa_n;
    gk_u8 *rsa_k;
    gk_u16 rsa_n_len;
    gk_u16 rsa_k_len;
    gk_cipher_ca_type ca_type;               /* Select keyladder type when using advanced CA */
} cipher_rsa_data_s;

/** RSA private key struct */
typedef struct {
    gk_u8 *rsa_n;                      /*!<  public modulus    */
    gk_u8 *rsa_e;                      /*!<  public exponent   */
    gk_u8 *rsa_d;                      /*!<  private exponent  */
    gk_u8 *rsa_p;                      /*!<  1st prime factor  */
    gk_u8 *rsa_q;                      /*!<  2nd prime factor  */
    gk_u8 *rsa_dp;                     /*!<  D % (P - 1)       */
    gk_u8 *rsa_dq;                     /*!<  D % (Q - 1)       */
    gk_u8 *rsa_qp;                     /*!<  1 / (Q % P)       */
    gk_u16 rsa_n_len;                   /* length of public modulus */
    gk_u16 rsa_e_len;                   /* length of public exponent */
    gk_u16 rsa_d_len;                   /* length of private exponent */
    gk_u16 rsa_p_len;                   /* length of 1st prime factor */
    gk_u16 rsa_q_len;                   /* length of 2nd prime factor */
    gk_u16 rsa_dp_len;                  /* length of D % (P - 1) */
    gk_u16 rsa_dq_len;                  /* length of D % (Q - 1) */
    gk_u16 rsa_qp_len;                  /* length of 1 / (Q % P) */
} cipher_rsa_pri_key_s;

typedef struct {
    cipher_rsa_pri_key_s pri_key;
    gk_u32 num_bits;
    gk_u32 exponent;
} cipher_rsa_key_s;

typedef enum {
    CIPHER_TEST_PRINT_PHY = 0x01,
    CIPHER_TEST_PRINT_VIA,
    CIPHER_TEST_MEMSET,
    CIPHER_TEST_MEMCMP,
    CIPHER_TEST_MEMCPY,
    CIPHER_TEST_MEMCMP_PHY,
    CIPHER_TEST_READ_REG,
    CIPHER_TEST_WRITE_REG,
    CIPHER_TEST_AES  = 0x10,
    CIPHER_TEST_HMAC,
    CIPHER_TEST_RSA,
    CIPHER_TEST_HASH,
    CIPHER_TEST_DES,
    CIPHER_TEST_RSA_PRIM,
    CIPHER_TEST_RSA_KG,
    CIPHER_TEST_RND,
    CIPHER_TEST_BUTT,
} cipher_test_e;

#ifdef CONFIG_COMPAT
typedef struct {
    gk_handle ci_handle;
    gk_u32 pkg_num;
    gk_u32 pkg_via;
} cipher_compat_pkg_s;

typedef struct {
    gk_u32 input_via;
    gk_u32 output_via;
    gk_u32 data_len;
    gk_u32 rsa_n_via;
    gk_u32 rsa_k_via;
    gk_u16 rsa_n_len;
    gk_u16 rsa_k_len;
} cipher_compat_rsa_data_s;
#endif

typedef struct {
    gk_cipher_ca_type root_key;
    gk_cipher_klad_target klad_target;
    gk_u32 clean_key[4];   /* 4 key size */
    gk_u32 encrypt_key[4]; /* 4 key size */
    gk_u32 key_len;
} cipher_klad_key_s;

gk_s32 gk_drv_cipher_create_handle(cipher_handle_s *ci_handle, gk_void *file);
gk_s32 gk_drv_cipher_config_chn(gk_u32 soft_chn_id, gk_cipher_ctrl *config);
gk_s32 gk_drv_cipher_config_chn_ex(gk_handle ci_handle, cipher_config_ctrl_ex_s *config);
gk_s32 gk_drv_cipher_get_handle_config(cipher_config_ctrl_s *cipher_config);
gk_s32 gk_drv_cipher_get_handle_config_ex(cipher_config_ctrl_ex_s *cipher_config);
gk_s32 gk_drv_cipher_get_iv(gk_u32 chn_id, gk_u32 iv[4]); /* 4 iv size */
gk_s32 gk_drv_cipher_destory_handle(gk_handle cipher_chn);
gk_s32 gk_drv_cipher_encrypt(cipher_data_s *ci_data);
gk_s32 gk_drv_cipher_decrypt(cipher_data_s *ci_data);
gk_s32 gk_drv_cipher_encrypt_multi(cipher_pkg_s *pkg);
gk_s32 gk_drv_cipher_decrypt_multi(cipher_pkg_s *pkg);
gk_s32 gk_drv_cipher_encrypt_multi_asyn(cipher_pkg_s *pkg);
gk_s32 gk_drv_cipher_decrypt_multi_asyn(cipher_pkg_s *pkg);
gk_s32 gk_drv_cipher_wait_done(cipher_wait_done_s *wait_done);
gk_s32 gk_drv_cipher_hash_wait_done(gk_handle handle);
gk_s32 gk_drv_cipher_get_random_number(cipher_rng_s *rng);
gk_s32 gk_drv_cipher_soft_reset(gk_void);
gk_s32 gk_drv_cipher_calc_hash_init(cipher_hash_data_s *cipher_hash_data);
gk_s32 gk_drv_cipher_calc_hash_update(cipher_hash_data_s *cipher_hash_data);
gk_s32 gk_drv_cipher_calc_hash_final(cipher_hash_data_s *cipher_hash_data);
gk_s32 gk_drv_cipher_get_tag(cipher_tag_s *tag);
gk_s32 gk_drv_cipher_calc_rsa(cipher_rsa_data_s *cipher_rsa_data);
gk_s32 gk_drv_cipher_klad_encrypt_key(cipher_klad_key_s *klad_key);

gk_void gk_drv_cipher_suspend(gk_void);
gk_s32 gk_drv_cipher_resume(gk_void);

#ifdef __cplusplus
}
#endif    /* __cplusplus */

#endif    /* End of #ifndef __GK_DRV_CIPHER_H__ */

