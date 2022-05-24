#ifndef _ACODEC_DEF_H_
#define _ACODEC_DEF_H_


#define ACODEC_REGS_CRG  0x1201019C  //aio crg
#define AIAO_CLK_TX0_CRG 0x100E0140
#define AIAO_CLK_TX0_CFG 0x100E0144
#define AIAO_REGS_BASE   0x100E0000

#define AIAO_CLK_TX0_CRG_OFFSET 0x140
#define AIAO_CLK_TX0_CFG_OFFSET 0x144

#define ACODEC_CRG_BASE    0x12010000
#define ACODEC_CRG_OFFSET  0x019C
#define ACODEC_REGS_OFFSET 0x0

#define ACODEC_REGS_BASE 0x100F0000

/*acodec analog register*/
#define ACODEC_ANAREG0_ADDR (0x00)
#define ACODEC_ANAREG1_ADDR (0x04)
#define ACODEC_ANAREG2_ADDR (0x08)
#define ACODEC_ANAREG3_ADDR (0x0C)
#define ACODEC_ANAREG4_ADDR (0x10)
#define ACODEC_ANAREG5_ADDR (0x14)

/*acodec dig control register*/
#define ACODEC_DIGCTRL1_ADDR (0xCC)
#define ACODEC_DIGCTRL2_ADDR (0xD0)
#define ACODEC_DIGCTRL3_ADDR (0xD4)
#define ACODEC_DIGCTRL4_ADDR (0xD8)
#define ACODEC_DIGCTRL5_ADDR (0xDC)

/*default value of acodec analog register*/
#define ACODEC_ANAREG0_DEFAULT 0x3434DFFF
#define ACODEC_ANAREG1_DEFAULT 0xDF605E65
#define ACODEC_ANAREG2_DEFAULT 0x00255548
#define ACODEC_ANAREG3_DEFAULT 0x04293B50
#define ACODEC_ANAREG4_DEFAULT 0x00000000
#define ACODEC_ANAREG5_DEFAULT 0x00000000

#define ACODEC_AUDIO_REG (0xE0)

/* Define the union ACODEC_ANAREG0_U */
typedef union ACODEC_ANAREG0_u
{
	/* Define the struct bits */
	struct
	{
		unsigned int    mute_linein_r         : 1   ; /* [0]  */
		unsigned int    mute_linein_l         : 1   ; /* [1]  */
		unsigned int    pd_linein_r           : 1   ; /* [2]  */
		unsigned int    pd_linein_l           : 1   ; /* [3]  */
		unsigned int    pd_adcr               : 1   ; /* [4]  */
		unsigned int    pd_adcl               : 1   ; /* [5]  */
		unsigned int    pd_lineoutr           : 1   ; /* [6]  */
		unsigned int    pd_lineoutl           : 1   ; /* [7]  */
		unsigned int    pd_vref               : 1   ; /* [8]  */
		unsigned int    pd_ibias              : 1   ; /* [9]  */
		unsigned int    pd_ctcm_rx            : 1   ; /* [10]  */
		unsigned int    pd_ctcm_tx            : 1   ; /* [11]  */
		unsigned int    pd_dac_vref           : 1   ; /* [12]  */
		unsigned int    pd_ldo                : 1   ; /* [13]  */
		unsigned int    pd_micbias2           : 1   ; /* [14]  */
		unsigned int    pd_micbias1           : 1   ; /* [15]  */
		unsigned int    linein_l_sel          : 3   ; /* [18..16]  */
		unsigned int    linein_l_gain         : 5   ; /* [23..19]  */
		unsigned int    linein_r_sel          : 3   ; /* [26..24]  */
		unsigned int    linein_r_gain         : 5   ; /* [31..27]  */

	} bits;
	unsigned int ul32;

} ACODEC_ANAREG0_U;

/* Define the union ACODEC_ANAREG1_U */
typedef union ACODEC_ANAREG1_u
{
	/* Define the struct bits */
	struct
	{
		unsigned int    pd_rctune             : 1   ; /* [0]  */
		unsigned int    en_rctune             : 1   ; /* [1]  */
		unsigned int    adc_tune_sel          : 1   ; /* [2]  */
		unsigned int    adc_tune_code         : 5   ; /* [7..3]  */
		unsigned int    byp_adc_dwa           : 1   ; /* [8]  */
		unsigned int    adc_the_clk_phsel     : 1   ; /* [9]  */
		unsigned int    adc_bin_clk_phsel     : 1   ; /* [10]  */
		unsigned int    adc_chop_clk_phsel    : 1   ; /* [11]  */
		unsigned int    linein_chop_clk_sel   : 2   ; /* [13..12]  */
		unsigned int    adc_chop_clk_sel      : 2   ; /* [15..14]  */
		unsigned int    boost_adcr            : 1   ; /* [16]  */
		unsigned int    boost_adcl            : 1   ; /* [17]  */
		unsigned int    mode_adcr             : 1   ; /* [18]  */
		unsigned int    mode_adcl             : 1   ; /* [19]  */
		unsigned int    adc_dac_ib_sel        : 2   ; /* [21..20]  */
		unsigned int    adc_fls_vref          : 2   ; /* [23..22]  */
		unsigned int    adc_mis_selp          : 4   ; /* [27..24]  */
		unsigned int    adc_mis_seln          : 4   ; /* [31..28]  */

	} bits;
	unsigned int ul32;

} ACODEC_ANAREG1_U;

/* Define the union ACODEC_ANAREG2_U */
typedef union ACODEC_ANAREG2_u
{
	/* Define the struct bits */
	struct
	{
		unsigned int    ana_loop              : 1   ; /* [0]  */
		unsigned int    fs_vref               : 1   ; /* [1]  */
		unsigned int    vref_pu_pdb           : 1   ; /* [2]  */
		unsigned int    ldo_bk_en             : 1   ; /* [3]  */
		unsigned int    ss_sel_vref           : 4   ; /* [7..4]  */
		unsigned int    ibadj_linein          : 2   ; /* [9..8]  */
		unsigned int    ibadj_adc             : 2   ; /* [11..10]  */
		unsigned int    ibadj_ctcm            : 2   ; /* [13..12]  */
		unsigned int    ibadj_micbias         : 2   ; /* [15..14]  */
		unsigned int    ibadj_dac_vref        : 2   ; /* [17..16]  */
		unsigned int    ibadj_lineout         : 2   ; /* [19..18]  */
		unsigned int    ldo_sel               : 2   ; /* [21..20]  */
		unsigned int    byp_chop_linein       : 1   ; /* [22]  */
		unsigned int    byp_chop_adc          : 1   ; /* [23]  */
		unsigned int    byp_chop_dac_vref     : 1   ; /* [24]  */
		unsigned int    byp_chop_ctcm_tx      : 1   ; /* [25]  */
		unsigned int    byp_chop_ctcm_rx      : 1   ; /* [26]  */
		unsigned int    micbias_adj           : 3   ; /* [29..27]  */
		unsigned int    reserved_0            : 2   ; /* [31..30]  */

	} bits;
	unsigned int ul32;

} ACODEC_ANAREG2_U;

/* Define the union ACODEC_ANAREG3_U */
typedef union ACODEC_ANAREG3_u
{
	/* Define the struct bits */
	struct
	{
		unsigned int    rstn                  : 1   ; /* [0]  */
		unsigned int    ctrl_mclk_ph          : 1   ; /* [1]  */
		unsigned int    ctrl_clk_dac_ph       : 1   ; /* [2]  */
		unsigned int    ctrl_clk_adc_ph       : 1   ; /* [3]  */
		unsigned int    sel_clk_chop_dac_vref : 2   ; /* [5..4]  */
		unsigned int    sel_clk_chop_ctcm     : 2   ; /* [7..6]  */
		unsigned int    mute_dacr             : 1   ; /* [8]  */
		unsigned int    mute_dacl             : 1   ; /* [9]  */
		unsigned int    rstb_dac              : 1   ; /* [10]  */
		unsigned int    pd_dacr_dff           : 1   ; /* [11]  */
		unsigned int    pd_dacl_dff           : 1   ; /* [12]  */
		unsigned int    pd_dac_clk            : 1   ; /* [13]  */
		unsigned int    reserved_0            : 2   ; /* [15..14]  */
		unsigned int    pop_sel_0h1s          : 1   ; /* [16]  */
		unsigned int    pop_lineout_pull_en   : 1   ; /* [17]  */
		unsigned int    pu_pop_pullb_reg      : 1   ; /* [18]  */
		unsigned int    pop_r_track_l_en      : 1   ; /* [19]  */
		unsigned int    pullout_weak          : 1   ; /* [20]  */
		unsigned int    pop_spd_cfg           : 2   ; /* [22..21]  */
		unsigned int    reserved_1            : 1   ; /* [23]  */
		unsigned int    pop_dis               : 1   ; /* [24]  */
		unsigned int    pop_rst               : 1   ; /* [25]  */
		unsigned int    pop_pdm_dly_cfg       : 2   ; /* [27..26]  */
		unsigned int    reserved_2            : 4   ; /* [31..28]  */

	} bits;
	unsigned int ul32;

} ACODEC_ANAREG3_U;

/* Define the union ACODEC_ANAREG4_U */
typedef union ACODEC_ANAREG4_u
{
	/* Define the struct bits */
	struct
	{
		unsigned int    reserved_0            : 8   ; /* [7..0]  */
		unsigned int    vref_pd_res_sel       : 12  ; /* [19..8]  */
		unsigned int    reserved_1            : 4   ; /* [23..20]  */
		unsigned int    pop_res_sel           : 8   ; /* [31..24]  */

	} bits;
	unsigned int ul32;

} ACODEC_ANAREG4_U;

/* Define the union ACODEC_ANAREG5_U */
typedef union ACODEC_ANAREG5_u
{
	/* Define the struct bits */
	struct
	{
		unsigned int    reserved              : 8   ; /* [7..0]  */

	} bits;
	unsigned int ul32;

} ACODEC_ANAREG5_U;

/* Define the union ACODEC_DIGCTRL1_U */
typedef union ACODEC_DIGCTRL1_u
{
	/* Define the struct bits */
	struct
	{
		unsigned int    reserved_0            : 8   ; /* [7..0]  */
		unsigned int    i2s2_fs_sel           : 5   ; /* [12..8]  */
		unsigned int    i2s1_fs_sel           : 5   ; /* [17..13]  */
		unsigned int    dig_loop              : 1   ; /* [18]  */
		unsigned int    dig_bypass            : 1   ; /* [19]  */
		unsigned int    i2s2_data_bits        : 2   ; /* [21..20]  */
		unsigned int    i2s1_data_bits        : 2   ; /* [23..22]  */
		unsigned int    adcr_en               : 1   ; /* [24]  */
		unsigned int    adcl_en               : 1   ; /* [25]  */
		unsigned int    dacr_en               : 1   ; /* [26]  */
		unsigned int    dacl_en               : 1   ; /* [27]  */
		unsigned int    adcr_rst_n            : 1   ; /* [28]  */
		unsigned int    adcl_rst_n            : 1   ; /* [29]  */
		unsigned int    dacr_rst_n            : 1   ; /* [30]  */
		unsigned int    dacl_rst_n            : 1   ; /* [31]  */
	} bits;

	/* Define an unsigned member */
	unsigned int    ul32;

} ACODEC_DIGCTRL1_U;

/* Define the union ACODEC_DIGCTRL2_U */
typedef union ACODEC_DIGCTRL2_u
{
	/* Define the struct bits */
	struct
	{
		unsigned int    dacr_lrsel            : 1   ; /* [0]  */
		unsigned int    dacr_i2ssel           : 1   ; /* [1]  */
		unsigned int    dacl_lrsel            : 1   ; /* [2]  */
		unsigned int    dal_i2ssel            : 1   ; /* [3]  */
		unsigned int    reserved_0            : 15  ; /* [18..4]  */
		unsigned int    dacr_deemph           : 2   ; /* [20..19]  */
		unsigned int    dacl_deemph           : 2   ; /* [22..21]  */
		unsigned int    muter_rate            : 2   ; /* [24..23]  */
		unsigned int    mutel_rate            : 2   ; /* [26..25]  */
		unsigned int    dacvu                 : 1   ; /* [27]  */
		unsigned int    sunmuter              : 1   ; /* [28]  */
		unsigned int    sunmutel              : 1   ; /* [29]  */
		unsigned int    smuter                : 1   ; /* [30]  */
		unsigned int    smutel                : 1   ; /* [31]  */
	} bits;

	/* Define an unsigned member */
	unsigned int    ul32;

} ACODEC_DIGCTRL2_U;

/* Define the union ACODEC_DIGCTRL3_U */
typedef union ACODEC_DIGCTRL3_u
{
	/* Define the struct bits */
	struct
	{
		unsigned int    dacl2dacr_vol         : 7   ; /* [6..0]  */
		unsigned int    dacl2dacr_en          : 1   ; /* [7]  */
		unsigned int    dacr2dacl_vol         : 7   ; /* [14..8]  */
		unsigned int    dacr2dacl_en          : 1   ; /* [15]  */
		unsigned int    dacr_vol              : 7   ; /* [22..16]  */
		unsigned int    dacr_mute             : 1   ; /* [23]  */
		unsigned int    dacl_vol              : 7   ; /* [30..24]  */
		unsigned int    dacl_mute             : 1   ; /* [31]  */
	} bits;

	/* Define an unsigned member */
	unsigned int    ul32;

} ACODEC_DIGCTRL3_U;

/* Define the union ACODEC_DIGCTRL4_U */
typedef union ACODEC_DIGCTRL4_u
{
	/* Define the struct bits */
	struct
	{
		unsigned int    adcr_lrsel            : 1   ; /* [0]  */
		unsigned int    adcr_i2ssel           : 1   ; /* [1]  */
		unsigned int    adcl_lrsel            : 1   ; /* [2]  */
		unsigned int    adcl_i2ssel           : 1   ; /* [3]  */
		unsigned int    reserved_0            : 10  ; /* [13..4]  */
		unsigned int    adcr_hpf_en           : 1   ; /* [14]  */
		unsigned int    adcl_hpf_en           : 1   ; /* [15]  */
		unsigned int    adcr_vol              : 7   ; /* [22..16]  */
		unsigned int    adcr_mute             : 1   ; /* [23]  */
		unsigned int    adcl_vol              : 7   ; /* [30..24]  */
		unsigned int    adcl_mute             : 1   ; /* [31]  */
	} bits;

	/* Define an unsigned member */
	unsigned int    ul32;

} ACODEC_DIGCTRL4_U;

/* Define the union ACODEC_DIGCTRL5_U */
typedef union ACODEC_DIGCTRL5_u
{
	/* Define the struct bits */
	struct
	{
		unsigned int    adcr2dacr_vol         : 7   ; /* [6..0]  */
		unsigned int    adcr2dacr_en          : 1   ; /* [7]  */
		unsigned int    adcl2dacr_vol         : 7   ; /* [14..8]  */
		unsigned int    adcl2dacr_en          : 1   ; /* [15]  */
		unsigned int    adcr2dacl_vol         : 7   ; /* [22..16]  */
		unsigned int    adcr2dacl_en          : 1   ; /* [23]  */
		unsigned int    adcl2dacl_vol         : 7   ; /* [30..24]  */
		unsigned int    adcl2dacl_en          : 1   ; /* [31]  */
	} bits;

	/* Define an unsigned member */
	unsigned int    ul32;

} ACODEC_DIGCTRL5_U;

/* Define the union AUDIO_REG_1_U */
typedef union
{
	/* Define the struct bits */
	struct
	{
		unsigned int    pad_tx_en            : 1   ; /* [0]  */
		unsigned int    audio_rx_bclk_sel    : 1   ; /* [1]  */
		unsigned int    rx_sd_sel            : 1   ; /* [2]  */
		unsigned int    audio_mclk_sel       : 1   ; /* [3]  */
		unsigned int    reserved_0           : 28  ; /* [31..4]  */
	} bits;

	/* Define an unsigned member */
	unsigned int    ul32;

} AUDIO_REG_1_U;

//==============================================================================
/* Define the global struct */
typedef struct
{
	volatile ACODEC_ANAREG0_U       AUDIO_ANA_CTRL_0;
	volatile ACODEC_ANAREG1_U       AUDIO_ANA_CTRL_1;
	volatile ACODEC_ANAREG2_U       AUDIO_ANA_CTRL_2;
	volatile ACODEC_ANAREG3_U       AUDIO_ANA_CTRL_3;
	volatile ACODEC_ANAREG4_U       AUDIO_ANA_CTRL_4;
	volatile ACODEC_ANAREG5_U       AUDIO_ANA_CTRL_5;
	volatile ACODEC_DIGCTRL1_U      AUDIO_CTRL_REG_1;
	volatile ACODEC_DIGCTRL2_U      AUDIO_DAC_REG_0;
	volatile ACODEC_DIGCTRL3_U      AUDIO_DAC_REG_1;
	volatile ACODEC_DIGCTRL4_U      AUDIO_ADC_REG_0;
	volatile ACODEC_DIGCTRL5_U      AUDIO_ADC_REG_1;
	volatile AUDIO_REG_1_U          AUDIO_REG_1;
} S_ACODEC_REGS_TYPE;
#endif /* End of #ifndef _ACODEC_H */

