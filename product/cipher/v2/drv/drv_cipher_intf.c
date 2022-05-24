/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#include "cipher_adapt.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

gk_s32 drv_cipher_ioctl(gk_u32 cmd, gk_void *argp, gk_void *private_data)
{
    gk_s32 ret = GK_SUCCESS;

    if (argp == GK_NULL) {
        gk_err_cipher("Error, argp is NULL!\n");
        return GK_FAILURE;
    }

    switch (cmd) {
        case CMD_CIPHER_CREATEHANDLE: {
            cipher_handle_s *handle = (cipher_handle_s *)argp;
            ret = gk_drv_cipher_create_handle(handle,  private_data);
            break;
        }
        case CMD_CIPHER_DESTROYHANDLE: {
            gk_handle handle = *(gk_handle *)argp;
            ret = gk_drv_cipher_destory_handle(handle);
            break;
        }
        case CMD_CIPHER_CONFIGHANDLE_EX: {
            cipher_config_ctrl_ex_s config_ex = *(cipher_config_ctrl_ex_s *)argp;
            ret = gk_drv_cipher_config_chn_ex(config_ex.ci_handle, &config_ex);
            break;
        }
        case CMD_CIPHER_ENCRYPT: {
            cipher_data_s *data = (cipher_data_s *)argp;
            ret = gk_drv_cipher_encrypt(data);
            break;
        }
        case CMD_CIPHER_DECRYPT: {
            cipher_data_s *data = (cipher_data_s *)argp;
            ret = gk_drv_cipher_decrypt(data);
            break;
        }
        case CMD_CIPHER_ENCRYPTMULTI: {
            cipher_pkg_s *pkg = (cipher_pkg_s *)argp;
            ret = gk_drv_cipher_encrypt_multi(pkg);
            break;
        }
        case CMD_CIPHER_DECRYPTMULTI: {
            cipher_pkg_s *pkg = (cipher_pkg_s *)argp;
            ret = gk_drv_cipher_decrypt_multi(pkg);
            break;
        }
#ifdef CIPHER_KLAD_SUPPORT
        case CMD_CIPHER_KLAD_KEY: {
            cipher_klad_key_s *klad_data = (cipher_klad_key_s *)argp;
            ret = gk_drv_cipher_klad_encrypt_key(klad_data);
            break;
        }
#endif
        case CMD_CIPHER_GETTAG: {
            cipher_tag_s *tag = (cipher_tag_s *)argp;
            ret = gk_drv_cipher_get_tag(tag);
            break;
        }
        case CMD_CIPHER_GETRANDOMNUMBER: {
            cipher_rng_s *rng  = (cipher_rng_s *)argp;
            ret = gk_drv_cipher_get_random_number(rng);
            break;
        }
        case CMD_CIPHER_GETHANDLECONFIG_EX:  {
            cipher_config_ctrl_ex_s *data = (cipher_config_ctrl_ex_s *)argp;
            ret = gk_drv_cipher_get_handle_config_ex(data);
            break;
        }
        case CMD_CIPHER_CALCHASHINIT: {
            cipher_hash_data_s *hash_data = (cipher_hash_data_s*)argp;
            ret = gk_drv_cipher_calc_hash_init(hash_data);
            break;
        }
        case CMD_CIPHER_CALCHASHUPDATE: {
            cipher_hash_data_s *hash_data = (cipher_hash_data_s*)argp;
            ret = gk_drv_cipher_calc_hash_update(hash_data);
            break;
        }
        case CMD_CIPHER_CALCHASHFINAL: {
            cipher_hash_data_s *hash_data = (cipher_hash_data_s*)argp;
            ret = gk_drv_cipher_calc_hash_final(hash_data);
            break;
        }
        case CMD_CIPHER_CALCRSA: {
            cipher_rsa_data_s *rsa_data = (cipher_rsa_data_s*)argp;
            ret = gk_drv_cipher_calc_rsa(rsa_data);
            break;
        }
#ifdef CONFIG_COMPAT
#ifdef CONFIG_RSA_HARDWARE_SUPPORT
        case CMD_CIPHER_COMPAT_CALCRSA: {
            cipher_compat_rsa_data_s *compat_rsa_data = (cipher_compat_rsa_data_s*)argp;
            cipher_rsa_data_s rsa_data;

            rsa_data.input_data = u32_to_point(compat_rsa_data->input_via);
            rsa_data.output_data = u32_to_point(compat_rsa_data->output_via);
            rsa_data.rsa_k = u32_to_point(compat_rsa_data->rsa_k_via);
            rsa_data.rsa_n = u32_to_point(compat_rsa_data->rsa_n_via);
            rsa_data.rsa_k_len = compat_rsa_data->rsa_k_len;
            rsa_data.rsa_n_len = compat_rsa_data->rsa_n_len;
            rsa_data.data_len = compat_rsa_data->data_len;

            ret = gk_drv_cipher_calc_rsa(&rsa_data);
            break;
        }
#endif
        case CMD_CIPHER_COMPAT_ENCRYPTMULTI: {
            cipher_compat_pkg_s *compat_pkg = (cipher_compat_pkg_s *)argp;
            cipher_pkg_s pkg;

            pkg.ci_handle = compat_pkg->ci_handle;
            pkg.pkg_num = compat_pkg->pkg_num;
            pkg.cipher_data = u32_to_point(compat_pkg->pkg_via);

            ret = gk_drv_cipher_encrypt_multi(&pkg);
            break;
        }
        case CMD_CIPHER_COMPAT_DECRYPTMULTI: {
            cipher_compat_pkg_s *compat_pkg = (cipher_compat_pkg_s *)argp;
            cipher_pkg_s pkg;

            pkg.ci_handle = compat_pkg->ci_handle;
            pkg.pkg_num = compat_pkg->pkg_num;
            pkg.cipher_data = u32_to_point(compat_pkg->pkg_via);

            ret = gk_drv_cipher_decrypt_multi(&pkg);
            break;
        }
#endif
        default:
            gk_err_cipher("Unsupport cmd, MOD_ID=0x%02X, NR=0x%02x, SIZE=0x%02x!\n",
                ree_cipher_ioc_type (cmd), ree_cipher_ioc_nr (cmd), ree_cipher_ioc_size(cmd));
            ret = GK_FAILURE;
            break;
    }

    return ret;
}

gk_s32 cipher_module_init(gk_void)
{
    gk_s32 ret;

    ret = drv_cipher_init();
    if (ret != GK_SUCCESS) {
        return ret;
    }

    ret = drv_rng_init();
    if (ret != GK_SUCCESS) {
        (gk_void)drv_cipher_deinit();
        return ret;
    }

#ifdef CIPHER_KLAD_SUPPORT
    ret = gk_drv_compat_init();
    if (ret != GK_SUCCESS) {
        (gk_void)drv_cipher_deinit();
        (gk_void)drv_rng_deinit();
        return ret;
    }
#endif

    ret = drv_rsa_init();
    if (ret != GK_SUCCESS) {
        (gk_void)drv_cipher_deinit();
        (gk_void)drv_rng_deinit();
#ifdef CIPHER_KLAD_SUPPORT
        (gk_void)gk_drv_compat_deinit();
#endif
        return ret;
    }

    return GK_SUCCESS;
}

gk_void cipher_module_exit(gk_void)
{
    (gk_void)drv_cipher_deinit();
    (gk_void)drv_rng_deinit();
    (gk_void)drv_rsa_deinit();

#ifdef CIPHER_KLAD_SUPPORT
    (gk_void)gk_drv_compat_deinit();
#endif
    return ;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
