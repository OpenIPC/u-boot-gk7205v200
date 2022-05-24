/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#include "cipher_osal.h"

#define CIPHER_MIN_CRYPT_LEN 8
#define CIPHER_MAX_CRYPT_LEN 0xfffff

#define gk_cipher_lock()
#define gk_cipher_unlock()
#define gk_hash_lock()
#define gk_hash_unlock()

gk_s32 g_cipher_dev_fd = -1;
gk_s32 g_cipher_init_counter = -1;

gk_s32 gk_api_cipher_init(gk_void)
{
    gk_cipher_lock();

    if (g_cipher_init_counter > 0) {
        g_cipher_init_counter++;
        gk_cipher_unlock();
        return GK_SUCCESS;
    }

    g_cipher_dev_fd = cipher_open("/dev/" UMAP_DEVNAME_CIPHER, O_RDWR, 0);
    if (g_cipher_dev_fd < 0) {
        gk_err_cipher("Open CIPHER err.\n");
        gk_cipher_unlock();
        return GK_ERR_CIPHER_FAILED_INIT;
    }

    g_cipher_init_counter = 1;
    gk_cipher_unlock();

    return GK_SUCCESS;
}

gk_s32 gk_api_cipher_deinit(gk_void)
{
    gk_cipher_lock();

    check_cipher_not_open_return();

    if (g_cipher_init_counter > 0)
        g_cipher_init_counter--;

    if (g_cipher_init_counter != 0) {
        gk_cipher_unlock();
        return GK_SUCCESS;
    }

    cipher_close(g_cipher_dev_fd);

    g_cipher_init_counter = -1;

    gk_cipher_unlock();

    return GK_SUCCESS;
}

gk_s32 gk_api_cipher_create_handle(gk_handle *handle, const gk_cipher_attr *cipher_attr)
{
    gk_s32 ret;
    cipher_handle_s ci_handle;

    check_cipher_not_open_return();
    inlet_var_is_null_return(handle);
    inlet_var_is_null_return(cipher_attr);

    crypto_memset(&ci_handle, sizeof(ci_handle), 0, sizeof(ci_handle));
    crypto_memcpy(&ci_handle.cipher_atts, sizeof(ci_handle.cipher_atts),
        cipher_attr, sizeof(gk_cipher_attr));
    ret = cipher_ioctl(g_cipher_dev_fd, CMD_CIPHER_CREATEHANDLE, &ci_handle);
    if (ret != GK_SUCCESS) {
        return ret;
    }

    *handle = ci_handle.ci_handle;

    return ret;
}

gk_s32 gk_api_cipher_destroy_handle(gk_handle handle)
{
    check_cipher_not_open_return();
    return cipher_ioctl(g_cipher_dev_fd, CMD_CIPHER_DESTROYHANDLE, &handle);
}

gk_s32 gk_api_cipher_config_handle(gk_handle handle, gk_cipher_ctrl *cipher_ctrl)
{
    cipher_config_ctrl_ex_s configdata;

    check_cipher_not_open_return();
    inlet_var_is_null_return(cipher_ctrl);

    crypto_memset(&configdata, sizeof(configdata), 0, sizeof(cipher_config_ctrl_ex_s));

    crypto_memcpy(&configdata.key, sizeof(configdata.key),
        cipher_ctrl->key, sizeof(cipher_ctrl->key));
    crypto_memcpy(&configdata.iv, sizeof(configdata.iv),
        cipher_ctrl->iv, sizeof(cipher_ctrl->iv));
    configdata.key_by_ca = cipher_ctrl->key_by_ca;
    configdata.ca_type = cipher_ctrl->ca_type;
    configdata.ci_alg = cipher_ctrl->alg;
    configdata.bit_width = cipher_ctrl->bit_width;
    configdata.work_mode = cipher_ctrl->work_mode;
    configdata.key_len = cipher_ctrl->key_len;
    configdata.change_flags = cipher_ctrl->chg_flags;
    configdata.ci_handle = handle;

    return cipher_ioctl(g_cipher_dev_fd, CMD_CIPHER_CONFIGHANDLE_EX, &configdata);
}

static gk_void mpi_cipher_config_aes(gk_cipher_ctrl_ex *ctrl_ex,
                                     cipher_config_ctrl_ex_s *configdata)
{
    if ((ctrl_ex->work_mode == GK_CIPHER_WORK_MODE_CCM) ||
        (ctrl_ex->work_mode == GK_CIPHER_WORK_MODE_GCM)) {
        gk_cipher_ctrl_aes_ccm_gcm *aes_ccm_gcm_ctrl =
            (gk_cipher_ctrl_aes_ccm_gcm *)ctrl_ex->param;

        crypto_memcpy(&configdata->key, sizeof(configdata->key),
            aes_ccm_gcm_ctrl->key, sizeof(aes_ccm_gcm_ctrl->key));
        crypto_memcpy(&configdata->iv, sizeof(configdata->iv),
            aes_ccm_gcm_ctrl->iv, sizeof(aes_ccm_gcm_ctrl->iv));

        configdata->bit_width = GK_CIPHER_BIT_WIDTH_128BIT;
        configdata->key_len = aes_ccm_gcm_ctrl->key_len;
        configdata->iv_len = aes_ccm_gcm_ctrl->iv_len;
        configdata->tag_len = aes_ccm_gcm_ctrl->tag_len;
        configdata->aphy_addr = aes_ccm_gcm_ctrl->aad_phys_addr;
        configdata->aphy_addr_high = get_ulong_high(aes_ccm_gcm_ctrl->aad_phy_addr);
        configdata->alen = aes_ccm_gcm_ctrl->aad_len;
        configdata->change_flags.bits_iv = 1;
    } else {
        gk_cipher_ctrl_aes *aes_ctrl = (gk_cipher_ctrl_aes *)ctrl_ex->param;

        crypto_memcpy(&configdata->key, sizeof(configdata->key),
            aes_ctrl->even_key, sizeof(aes_ctrl->even_key));
        crypto_memcpy(&configdata->odd_key, sizeof(configdata->odd_key),
            aes_ctrl->odd_key, sizeof(aes_ctrl->odd_key));
        crypto_memcpy(&configdata->iv, sizeof(configdata->iv),
            aes_ctrl->iv, sizeof(aes_ctrl->iv));

        configdata->bit_width = aes_ctrl->bit_width;
        configdata->key_len = aes_ctrl->key_len;
        configdata->change_flags = aes_ctrl->chg_flags;
        configdata->iv_len = 16; /* 16 - aes iv len */
    }
}

static gk_void mpi_cipher_config_sm1(gk_cipher_ctrl_ex *ctrl_ex,
                                     cipher_config_ctrl_ex_s *configdata)
{
    gk_cipher_ctrl_sm1 *sm1_ctrl = (gk_cipher_ctrl_sm1 *)ctrl_ex->param;

    crypto_memcpy(&configdata->key[0], sizeof(sm1_ctrl->ek),
        sm1_ctrl->ek, sizeof(sm1_ctrl->ek));
    crypto_memcpy(&configdata->key[4], sizeof(sm1_ctrl->ak), /* 4 - sm1 ak index */
        sm1_ctrl->ak, sizeof(sm1_ctrl->ak));
    crypto_memcpy(&configdata->key[8], sizeof(sm1_ctrl->sk), /* 8 - sm1 sk index */
        sm1_ctrl->sk, sizeof(sm1_ctrl->sk));
    crypto_memcpy(&configdata->iv, sizeof(configdata->iv),
        sm1_ctrl->iv, sizeof(sm1_ctrl->iv));

    configdata->sm1_round = sm1_ctrl->sm1_round;
    configdata->key_len = GK_CIPHER_KEY_DEFAULT;
    configdata->change_flags = sm1_ctrl->chg_flags;
    configdata->bit_width = sm1_ctrl->bit_width;
    configdata->iv_len = 16; /* 16 - sm1 iv len */
}

static gk_void mpi_cipher_config_sm4(gk_cipher_ctrl_ex *ctrl_ex,
                                     cipher_config_ctrl_ex_s *configdata)
{
    gk_cipher_ctrl_sm4 *sm4_ctrl = (gk_cipher_ctrl_sm4 *)ctrl_ex->param;

    crypto_memcpy(&configdata->key, sizeof(configdata->key),
        sm4_ctrl->key, sizeof(sm4_ctrl->key));
    crypto_memcpy(&configdata->iv, sizeof(configdata->iv),
        sm4_ctrl->iv, sizeof(sm4_ctrl->iv));

    configdata->key_len = GK_CIPHER_KEY_DEFAULT;
    configdata->change_flags = sm4_ctrl->chg_flags;
    configdata->bit_width = GK_CIPHER_BIT_WIDTH_128BIT;
    configdata->iv_len = 16; /* 16 - sm4 iv len */
}

gk_s32 gk_api_cipher_config_handle_ex(gk_handle handle, gk_cipher_ctrl_ex *ctrl_ex)
{
    cipher_config_ctrl_ex_s configdata;

    check_cipher_not_open_return();
    inlet_var_is_null_return(ctrl_ex);

    if ((ctrl_ex->alg != GK_CIPHER_ALG_DMA) && (ctrl_ex->param == NULL)) {
        gk_err_cipher("para ctrl_ex param is invalid.\n");
        return GK_ERR_CIPHER_INVALID_PARA;
    }

    crypto_memset(&configdata, sizeof(configdata), 0, sizeof(cipher_config_ctrl_ex_s));
    configdata.ci_handle = handle;
    configdata.ci_alg = ctrl_ex->alg;
    configdata.work_mode = ctrl_ex->work_mode;
    configdata.key_by_ca = ctrl_ex->key_by_ca;

    /* ****************************************************************************
     * for AES, the pointer should point to gk_cipher_ctrl_aes;
     * for AES_CCM or AES_GCM, the pointer should point to gk_cipher_ctrl_aes_ccm_gcm;
     * for DES, the pointer should point to GK_CIPHER_CTRL_DES_S;
     * for 3DES, the pointer should point to GK_CIPHER_CTRL_3DES_S;
     * for SM1, the pointer should point to gk_cipher_ctrl_sm1;
     * for SM4, the pointer should point to gk_cipher_ctrl_sm4;
     */
    switch (ctrl_ex->alg) {
        case GK_CIPHER_ALG_AES:
            mpi_cipher_config_aes(ctrl_ex, &configdata);
            break;
        case GK_CIPHER_ALG_SM1:
            mpi_cipher_config_sm1(ctrl_ex, &configdata);
            break;
        case GK_CIPHER_ALG_SM4:
            mpi_cipher_config_sm4(ctrl_ex, &configdata);
            break;
        case GK_CIPHER_ALG_DMA:
            break;
        default:
            gk_err_cipher("para set CIPHER alg is invalid.\n");
            return GK_ERR_CIPHER_INVALID_PARA;
    }

    return cipher_ioctl(g_cipher_dev_fd, CMD_CIPHER_CONFIGHANDLE_EX, &configdata);
}

gk_s32 gk_api_cipher_encrypt(gk_handle handle, gk_size_t src_phy_addr, gk_size_t dest_phy_addr, gk_u32 byte_len)
{
    cipher_data_s ci_data;

    check_cipher_not_open_return();
    ci_data.src_phy_addr = get_ulong_low(src_phy_addr);
    ci_data.src_phy_addr_high = get_ulong_high(src_phy_addr);
    ci_data.dest_phy_addr = get_ulong_low(dest_phy_addr);
    ci_data.dest_phy_addr_high = get_ulong_high(dest_phy_addr);

    ci_data.data_length = byte_len;
    ci_data.ci_handle = handle;

    return cipher_ioctl(g_cipher_dev_fd, CMD_CIPHER_ENCRYPT, &ci_data);
}

gk_s32 gk_api_cipher_decrypt(gk_handle handle, gk_size_t src_phy_addr, gk_size_t dest_phy_addr, gk_u32 byte_len)
{
    cipher_data_s ci_data;

    check_cipher_not_open_return();

    ci_data.src_phy_addr = get_ulong_low(src_phy_addr);
    ci_data.src_phy_addr_high = get_ulong_high(src_phy_addr);
    ci_data.dest_phy_addr = get_ulong_low(dest_phy_addr);
    ci_data.dest_phy_addr_high = get_ulong_high(dest_phy_addr);
    ci_data.data_length = byte_len;
    ci_data.ci_handle = handle;

    return cipher_ioctl(g_cipher_dev_fd, CMD_CIPHER_DECRYPT, &ci_data);
}

gk_s32 gk_api_cipher_encrypt_multi_pack(gk_handle handle, gk_cipher_data *data_pkg, gk_u32 data_pkg_num)
{
    cipher_pkg_s ci_pkg;
    gk_cipher_data *ci_data = NULL;
    gk_u32 len;
    gk_s32 ret;

    check_cipher_not_open_return();
    inlet_var_is_null_return(data_pkg);
    inlet_var_is_zero_return(data_pkg_num);
    inlet_var_over_max_return(data_pkg_num, MAX_MULTI_PKG_NUM);

    len = sizeof(gk_cipher_data) * data_pkg_num;
    ci_data = cipher_malloc(len);
    if (ci_data == GK_NULL) {
        gk_err_cipher("Error, malloc pkg buffer failed\n");
        return GK_FAILURE;
    }
    crypto_memcpy(ci_data, len, data_pkg, len);

    ci_pkg.ci_handle = handle;
    ci_pkg.cipher_data = ci_data;
    ci_pkg.pkg_num = data_pkg_num;
    ci_pkg.user_bit_width = sizeof(gk_size_t);
    ret = cipher_ioctl(g_cipher_dev_fd, CMD_CIPHER_ENCRYPTMULTI, &ci_pkg);

    cipher_free(ci_data);
    ci_data = GK_NULL;

    return ret;
}

gk_s32 gk_api_cipher_decrypt_multi_pack(gk_handle handle, gk_cipher_data *data_pkg, gk_u32 data_pkg_num)
{
    cipher_pkg_s ci_pkg;
    gk_cipher_data *ci_data = NULL;
    gk_u32 len;
    gk_s32 ret;

    check_cipher_not_open_return();
    inlet_var_is_null_return(data_pkg);
    inlet_var_is_zero_return(data_pkg_num);
    inlet_var_over_max_return(data_pkg_num, MAX_MULTI_PKG_NUM);

    len = sizeof(gk_cipher_data) * data_pkg_num;
    ci_data = cipher_malloc(len);
    if (ci_data == GK_NULL) {
        gk_err_cipher("Error, malloc pkg buffer failed\n");
        return GK_FAILURE;
    }
    crypto_memcpy(ci_data, len, data_pkg, len);

    ci_pkg.ci_handle = handle;
    ci_pkg.cipher_data = ci_data;
    ci_pkg.pkg_num = data_pkg_num;
    ci_pkg.user_bit_width = sizeof(gk_size_t);

    ret = cipher_ioctl(g_cipher_dev_fd, CMD_CIPHER_DECRYPTMULTI, &ci_pkg);

    cipher_free(ci_data);
    ci_data = GK_NULL;

    return ret;
}

gk_s32 gk_api_cipher_get_handle_cfg(gk_handle handle, gk_cipher_ctrl *cipher_ctrl)
{
    gk_s32 ret;
    cipher_config_ctrl_ex_s configdata;

    check_cipher_not_open_return();
    inlet_var_is_null_return(cipher_ctrl);

    crypto_memset(&configdata, sizeof(configdata), 0, sizeof(cipher_config_ctrl_ex_s));
    configdata.ci_handle = handle;
    ret = cipher_ioctl(g_cipher_dev_fd, CMD_CIPHER_GETHANDLECONFIG_EX, &configdata);
    if (ret != GK_SUCCESS) {
        return ret;
    }

    crypto_memcpy(cipher_ctrl->key, sizeof(cipher_ctrl->key),
        &configdata.key, sizeof(cipher_ctrl->key));
    crypto_memcpy(cipher_ctrl->iv, sizeof(cipher_ctrl->iv),
        &configdata.iv, sizeof(configdata.iv));

    cipher_ctrl->key_by_ca = configdata.key_by_ca;
    cipher_ctrl->ca_type = configdata.ca_type;
    cipher_ctrl->alg = configdata.ci_alg;
    cipher_ctrl->bit_width = configdata.bit_width;
    cipher_ctrl->work_mode = configdata.work_mode;
    cipher_ctrl->key_len = configdata.key_len;
    cipher_ctrl->chg_flags = configdata.change_flags;

    return GK_SUCCESS;
}

#ifdef CIPHER_KLAD_SUPPORT
gk_s32 gk_api_cipher_klad_encrypt_key(gk_cipher_ca_type root_key,
                                      gk_cipher_klad_target klad_target,
                                      gk_u8 *clean_key,
                                      gk_u8 *enc_key,
                                      gk_u32 key_len)
{
    gk_s32 ret = GK_SUCCESS;
    cipher_klad_key_s ci_klad;
    gk_u32 i;

    check_cipher_not_open_return();
    memset(&ci_klad, 0, sizeof(cipher_klad_key_s));

    if ((clean_key == GK_NULL) || (enc_key == GK_NULL)) {
        gk_err_cipher("invalid para.\n");
        return GK_FAILURE;
    }

    if ((key_len == 0) || (key_len % 16 != 0)) { /* key_len must multiples of 16 */
        gk_err_cipher("invalid key len 0x%x.\n", key_len);
        return GK_FAILURE;
    }

    if ((root_key >= GK_CIPHER_KEY_SRC_BUTT) || (klad_target >= GK_CIPHER_KLAD_TARGET_BUTT)) {
        gk_err_cipher("invalid para.\n");
        return GK_FAILURE;
    }

    ci_klad.root_key = root_key;
    ci_klad.klad_target = klad_target;

    for (i = 0; i < key_len / 16; i++) { /* 16 groups */
        memcpy(ci_klad.clean_key, clean_key + i * 16, 16); /* 16 groups */
        ret = cipher_ioctl(g_cipher_dev_fd, CMD_CIPHER_KLAD_KEY, &ci_klad);
        if (ret != GK_SUCCESS) {
            return ret;
        }
        memcpy(enc_key + i * 16, ci_klad.encrypt_key, 16); /* 16 groups */
    }

    return ret;
}
#endif

gk_s32 gk_api_cipher_get_tag(gk_handle handle, gk_u8 *tag, gk_u32 *tag_len)
{
    cipher_tag_s ci_tag;
    gk_s32 ret;

    check_cipher_not_open_return();
    inlet_var_is_null_return(tag);
    inlet_var_is_null_return(tag_len);

    crypto_memset(&ci_tag, sizeof(cipher_tag_s), 0, sizeof(cipher_tag_s));
    ci_tag.ci_handle = handle;
    ret = cipher_ioctl(g_cipher_dev_fd, CMD_CIPHER_GETTAG, &ci_tag);
    if (ret == GK_SUCCESS) {
        inlet_var_over_max_return(ci_tag.tag_len, *tag_len);
        crypto_memcpy(tag, *tag_len, ci_tag.tag, ci_tag.tag_len);
        *tag_len = ci_tag.tag_len;
    }

    return ret;
}
