/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#include "hal_otp.h"
#include "cipher_adapt.h"

#ifdef OTP_SUPPORT

typedef enum {
	OTP_USER_LOCK_STA0_TYPE,
	OTP_USER_LOCK_STA1_TYPE,
	OTP_USER_LOCK_UNKNOWN_STA,
} opt_lock_sta_type_e;

typedef enum {
	OTP_READ_LOCK_STA_MODE,
	OTP_LOCK_CIPHER_KEY_MODE,
	OTP_WRITE_KEY_ID_OR_PASSWD_MODE,
	OTP_KEY_ID_OR_PASSWD_CRC_MODE,
	OTP_SET_FLAG_ENABLE_MODE,
	OTP_WRITE_USER_ROOM_MODE,
	OTP_READ_USER_ROOM_MODE,
	OTP_UNKOOWN_MODE,
} otp_user_work_mode_e;

typedef enum {
	OTP_USER_KEY0,
	OTP_USER_KEY1,
	OTP_USER_KEY2,
	OTP_USER_KEY3,
	OTP_USER_KEY_JTAG_PW_ID,
	OTP_USER_KEY_JTAG_PW,
	OTP_USER_KEY_ROOTKEY,
	OTP_USER_KEY_UNKNOWN,
} otp_user_key_index_e;

typedef enum {
	OTP_KEY_LENGTH_64BIT,
	OTP_KEY_LENGTH_128BIT,
	OTP_KEY_LENGTH_256BIT,
	OTP_KEY_LENGTH_UNSUPPORT,
} otp_user_key_length_e;

gk_void *g_efuse_otp_reg_base = GK_NULL;

/* OTP init */
gk_s32 hal_efuse_otp_init(gk_void)
{
	gk_u32 crg_value = 0;
	gk_u32 *sys_addr = GK_NULL;

	sys_addr = cipher_ioremap_nocache(REG_SYS_OTP_CLK_ADDR_PHY, 0x100);
	if (sys_addr == GK_NULL) {
		gk_err_cipher("ERROR: sys_addr ioremap with nocache failed!!\n");
		return GK_FAILURE;
	}

	hal_cipher_read_reg(sys_addr, &crg_value);
#ifdef OTP_CRG_RESET_SUPPORT
	crg_value |= OTP_CRG_RESET_BIT;   /* reset */
	crg_value |= OTP_CRG_CLOCK_BIT;   /* set the bit 0, clock opened */
	hal_cipher_write_reg(sys_addr, crg_value);

	/* clock select and cancel reset 0x30100 */
	crg_value &= (~OTP_CRG_RESET_BIT); /* cancel reset */
#endif
	crg_value |= OTP_CRG_CLOCK_BIT;   /* set the bit 0, clock opened */
	hal_cipher_write_reg(sys_addr, crg_value);

	cipher_iounmap(sys_addr);
	sys_addr = GK_NULL;

	g_efuse_otp_reg_base = cipher_ioremap_nocache(CIPHER_OTP_REG_BASE_ADDR_PHY, 0x100);
	if (g_efuse_otp_reg_base == GK_NULL) {
		gk_err_cipher("ERROR: osal_ioremap_nocache for OTP failed!!\n");
		return GK_FAILURE;
	}

	return GK_SUCCESS;
}

static gk_s32 hal_otp_wait_free(gk_void)
{
	gk_u32 timeout_cnt = 0;
	gk_u32 reg_value = 0;

	while (1) {
		hal_cipher_read_reg(OTP_USER_CTRL_STA, &reg_value);
		if ((reg_value & 0x1) == 0) /* bit0:otp_op_busy 0:idle, 1:busy */
			return GK_SUCCESS;

		timeout_cnt++;
		if (timeout_cnt >= 10000) { /* 10000 count */
			gk_err_cipher("OTP_WaitFree TimeOut!\n");
			break;
		}
	}
	return GK_FAILURE;
}

static gk_s32 hal_otp_set_mode(otp_user_work_mode_e otp_mode)
{
	gk_u32 reg_value = otp_mode;

	if (otp_mode >= OTP_UNKOOWN_MODE) {
		gk_err_cipher("Mode Unknown!\n");
		return  GK_FAILURE;
	}

	(gk_void)hal_cipher_write_reg(OTP_USER_WORK_MODE, reg_value);
	return GK_SUCCESS;
}

static gk_void hal_otp_op_start(gk_void)
{
	gk_u32 reg_value = 0x1acce551;
	(gk_void)hal_cipher_write_reg(OTP_USER_OP_START, reg_value);
}

static gk_s32 hal_otp_wait_op_done(gk_void)
{
	gk_u32 timeout_cnt = 0;
	gk_u32 reg_value = 0;

	while (1) {
		hal_cipher_read_reg(OTP_USER_CTRL_STA, &reg_value);
		if (reg_value & 0x2) {
			return GK_SUCCESS;
		}

		timeout_cnt++;
		if (timeout_cnt >= 10000) { /* 10000 count */
			gk_err_cipher("OTP_Wait_OP_done TimeOut!\n");
			break;
		}
	}
	return GK_FAILURE;
}

static gk_void hal_choose_otp_key(otp_user_key_index_e which_key)
{
	gk_u32 reg_value;

	reg_value = which_key;
	(gk_void)hal_cipher_write_reg(OTP_USER_KEY_INDEX, reg_value);
}

/* set otp key to klad */
gk_s32 hal_efuse_otp_load_cipher_key(gk_u32 chn_id, gk_u32 opt_id)
{
	if (opt_id > OTP_USER_KEY3)
		opt_id = OTP_USER_KEY0;

	if (GK_FAILURE == hal_otp_wait_free())
		return GK_FAILURE;
	hal_choose_otp_key(opt_id);

	if (hal_otp_set_mode(OTP_LOCK_CIPHER_KEY_MODE))
		return GK_FAILURE;

	hal_otp_op_start();

	if (GK_FAILURE == hal_otp_wait_op_done())
		return GK_FAILURE;

	return  GK_SUCCESS;
}
#endif

