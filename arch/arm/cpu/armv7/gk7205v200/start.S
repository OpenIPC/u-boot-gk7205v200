/*
 * Copyright (c) Hunan Goke,Chengdu Goke,Shandong Goke. 2021. All rights reserved.
 */

#include <asm-offsets.h>
#include <config.h>
#include <asm/system.h>
#include <linux/linkage.h>
#include <asm/armv7.h>

/*************************************************************************
 *
 * Startup Code (reset vector)
 *
 * Do important init only if we don't start from memory!
 * Setup memory and board specific bits prior to relocation.
 * Relocate armboot to ram. Setup stack.
 *
 *************************************************************************/
.globl	_start
_start:
	b	reset
    b .
    b .
    b .
    b .
    b .
    b .
    b .
	.balignl 64,0xdeadbeef
	
	.globl	reset
	.globl	save_boot_params_ret
#ifdef CONFIG_ARMV7_LPAE
	.global	switch_to_hypervisor_ret
#endif

 __blank_zone_start:
	.fill 1024*5,1,0
__blank_zone_end:

	.globl _blank_zone_start
_blank_zone_start:
	.word __blank_zone_start


	.globl _blank_zone_end
_blank_zone_end:
	.word __blank_zone_end

	.balignl 16,0xdeadbeef
	.globl	_TEXT_BASE
_TEXT_BASE:
	.word TEXT_BASE
 

_clr_remap_fmc_entry:
	.word   FMC_MEM_BASE + do_clr_remap - CONFIG_SYS_TEXT_BASE_ORI

_start_armboot:
	.word start_armboot

reset:
	/* Allow the board to save important registers */
	b	save_boot_params
save_boot_params_ret:
#ifdef CONFIG_ARMV7_LPAE
/*
 * check for Hypervisor support
 */
	mrc	p15, 0, r0, c0, c1, 1		@ read ID_PFR1
	and	r0, r0, #CPUID_ARM_VIRT_MASK	@ mask virtualization bits
	cmp	r0, #(1 << CPUID_ARM_VIRT_SHIFT)
	beq	switch_to_hypervisor
switch_to_hypervisor_ret:
#endif
	/*
	 * disable interrupts (FIQ and IRQ), also set the cpu to SVC32 mode,
	 * except if in HYP mode already
	 */
	mrs	r0, cpsr
	and	r1, r0, #0x1f		@ mask mode bits
	teq	r1, #0x1a		@ test for HYP mode
	bicne	r0, r0, #0x1f		@ clear all mode bits
	orrne	r0, r0, #0x13		@ set SVC mode
	orr	r0, r0, #0xc0		@ disable FIQ and IRQ
	msr	cpsr,r0

/*
 * Setup vector:
 * (OMAP4 spl TEXT_BASE is not 32 byte aligned.
 * Continue to use ROM code vector only in OMAP4 spl)
 */
#if !(defined(CONFIG_OMAP44XX) && defined(CONFIG_SPL_BUILD))
	/* Set V=0 in CP15 SCTLR register - for VBAR to point to vector */
	mrc	p15, 0, r0, c1, c0, 0	@ Read CP15 SCTLR Register
	bic	r0, #CR_V		@ V = 0
	mcr	p15, 0, r0, c1, c0, 0	@ Write CP15 SCTLR Register

	/* Set vector address in CP15 VBAR register */
	adrl	r0, _start
	mcr	p15, 0, r0, c12, c0, 0	@Set VBAR
#endif

	/* the mask ROM code should have PLL and others stable */
#ifndef CONFIG_SKIP_LOWLEVEL_INIT
	bl	cpu_init_cp15
#ifndef CONFIG_SKIP_LOWLEVEL_INIT_ONLY
	bl	cpu_init_crit
#endif
#endif

#ifndef CONFIG_GK_DISABLE_DOWNLOAD
	/*
	 *  read system register REG_SC_GEN2
         *  check if ziju flag
	 */
	ldr	r0, =SYS_CTRL_REG_BASE
	ldr	r1, [r0, #REG_SC_GEN2]
	ldr	r2, =0x7a696a75          /* magic for "ziju" */
	cmp	r1, r2
	bne	normal_start_flow
	mov	r1, sp                   /* save sp */
	str	r1, [r0, #REG_SC_GEN2]  /* clear ziju flag */

    /* init PLL/DDRC/pin mux/... */
	ldr	r0, _blank_zone_start
	ldr	r1, _TEXT_BASE
	sub	r0, r0, r1
	ldr	r1, =RAM_START_ADRS
	ldr	sp, =STACK_TRAINING
	add	r0, r0, r1
	mov	r1, #0x0                 /* flags: 0->normal 1->pm */
	bl	init_registers           /* init PLL/DDRC/... */
	/* after ziju, we need ddr traning */
	ldr	r0, =SYS_CTRL_REG_BASE
	bl	start_ddr_training       /* DDR training */
    ldr	r0, =SYS_CTRL_REG_BASE
	ldr	r1, [r0, #REG_SC_GEN2]
	mov	sp, r1		         /* restore sp */
	ldr	r1, [r0, #REG_SC_GEN3]
	mov	pc, r1                  /* return to bootrom */
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	b	.                        /* bug here */

normal_start_flow:
#endif
	/* init serial and printf a string. */
	ldr     sp, =STACK_TRAINING
#ifdef ENABLE_MINI_BOOT
	bl	timer_init
#else
	bl      uart_early_init
	bl      msg_main_cpu_startup
#endif

	/*
	 * enable syscnt 
	 */
	ldr	r0, =SYSCNT_REG_BASE
	ldr 	r3, =SYSCNT_FREQ
	str	r3, [r0, #SYSCNT_FREQ_REG]
	mov	r3, #0x1
	str	r3, [r0, #SYSCNT_ENABLE_REG]

	@if running not boot from nand/spi/emmc,
	@we skipping boot_type checking.
	mov    r0, pc, lsr#24
	cmp    r0, #0x0
	bne    do_clr_remap

check_boot_type:
#ifndef ENABLE_MINI_BOOT
    ldr     r0, =SYS_CTRL_REG_BASE
    ldr     r0, [r0, #REG_SYSSTAT]
    mov     r6, r0, lsr#4
	and     r6, #0x1
    cmp     r6, #0x1
    ldrlo   pc, _clr_remap_fmc_entry
#else
	ldr pc, _clr_remap_fmc_entry
#endif

do_clr_remap:
#ifndef ENABLE_MINI_BOOT
        /* do clear remap */
	ldr     r4, =SYS_CTRL_REG_BASE
	ldr	r0, [r4, #REG_SC_CTRL]

	@Set clear remap bit.
	orr	r0, #(1<<8)
	str	r0, [r4, #REG_SC_CTRL]
#endif
	@enable I-Cache now
	mrc p15, 0, r0, c1, c0, 0
    orr r0, r0, #0x00001000 /* set bit 12 (I) I-Cache */
    mcr p15, 0, r0, c1, c0, 0

	@Check wether I'm running in dynamic mem bank:<0x40000000
	mov r0, pc, lsr#28
	cmp r0, #4
	blo	ddr_init

no_ddr_init:
	adrl	r0, _start
	b       copy_to_ddr

ddr_init:
	ldr     r0, _blank_zone_start
	ldr     r1, _TEXT_BASE
	sub     r0, r0, r1
	adrl	r1, _start
	add     r0, r0, r1
	mov     r1, #0          /* flags: 0->normal 1->pm */
	bl      init_registers

	ldr	sp, =STACK_TRAINING
	ldr	r0, =SYS_CTRL_REG_BASE
	bl	start_ddr_training       /* DDR training */

check_boot_mode:
#ifndef ENABLE_MINI_BOOT
	ldr	r0, =SYS_CTRL_REG_BASE
	ldr     r0, [r0, #REG_SYSSTAT]
	mov     r6, r0, lsr#4
	and	r6, #0x1
	cmp     r6, #BOOT_FROM_EMMC
	bne     copy_flash_to_ddr
#else
	b copy_flash_to_ddr
#endif

#ifndef ENABLE_MINI_BOOT
emmc_boot:
	ldr     r0, _TEXT_BASE
	ldr	r1, =__image_copy_start
	ldr	r2, =__bss_start
	sub	r1, r2, r1
	bl	emmc_boot_read
	b       relocate
#endif

copy_flash_to_ddr:
	/* relocate SPI nor/nand Boot to DDR  */
	ldr	r0, =FMC_MEM_BASE

copy_to_ddr:
	/* now, r0 stores __reset offset from where we get started */
	ldr     r1, =__image_copy_start

	/* compare source and target address, *
	 *if equal no copy to target address */
	cmp     r0, r1
	beq	start_armboot

	ldr     r2, =__image_copy_start/*_start*/
	ldr     r3, =__bss_start
	sub     r2, r3, r2      /* r2 <- size of armboot */
	/* memcpy(r1, r0, r2) */
	bl      memcpy

relocate:
	ldr r0, =_start_armboot
	ldr pc, [r0]

bug:
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	b       .                       /* bug here */
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@
@       void memcpy(r1, r0, r2);
@

.align  2
memcpy:
	add     r2, r0, r2
	memcpy_loop:
	ldmia   r0!, {r3 - r10}
	stmia   r1!, {r3 - r10}
	cmp     r0, r2
	ble     memcpy_loop
	mov     pc, lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.align  2
msg_main_cpu_startup:
	mov     r5, lr
	add     r0, pc, #4
	bl      uart_early_puts
	mov     pc, r5
	L10:
#ifndef CONFIG_SUPPORT_CA_RELEASE
	.ascii "\r\n\r\nSystem startup\r\n\0"
#else
	.ascii "\r\n\r\n\r\n\0"
#endif

/*------------------------------------------------------------------------------*/

ENTRY(c_runtime_cpu_setup)
/*
 * If I-cache is enabled invalidate it
 */
#ifndef CONFIG_SYS_ICACHE_OFF
	mcr	p15, 0, r0, c7, c5, 0	@ invalidate icache
	mcr     p15, 0, r0, c7, c10, 4	@ DSB
	mcr     p15, 0, r0, c7, c5, 4	@ ISB
#endif

	bx	lr

ENDPROC(c_runtime_cpu_setup)

/*************************************************************************
 *
 * void save_boot_params(u32 r0, u32 r1, u32 r2, u32 r3)
 *	__attribute__((weak));
 *
 * Stack pointer is not yet initialized at this moment
 * Don't save anything to stack even if compiled with -O0
 *
 *************************************************************************/
ENTRY(save_boot_params)
	b	save_boot_params_ret		@ back to my caller
ENDPROC(save_boot_params)
	.weak	save_boot_params

#ifdef CONFIG_ARMV7_LPAE
ENTRY(switch_to_hypervisor)
	b	switch_to_hypervisor_ret
ENDPROC(switch_to_hypervisor)
	.weak	switch_to_hypervisor
#endif

/*************************************************************************
 *
 * cpu_init_cp15
 *
 * Setup CP15 registers (cache, MMU, TLBs). The I-cache is turned on unless
 * CONFIG_SYS_ICACHE_OFF is defined.
 *
 *************************************************************************/
ENTRY(cpu_init_cp15)
	/*
	 * Invalidate L1 I/D
	 */
	mov	r0, #0			@ set up for MCR
	mcr	p15, 0, r0, c8, c7, 0	@ invalidate TLBs
	mcr	p15, 0, r0, c7, c5, 0	@ invalidate icache
	mcr	p15, 0, r0, c7, c5, 6	@ invalidate BP array
	mcr     p15, 0, r0, c7, c10, 4	@ DSB
	mcr     p15, 0, r0, c7, c5, 4	@ ISB

	/*
	 * disable MMU stuff and caches
	 */
	mrc	p15, 0, r0, c1, c0, 0
	bic	r0, r0, #0x00002000	@ clear bits 13 (--V-)
	bic	r0, r0, #0x00000007	@ clear bits 2:0 (-CAM)
	orr	r0, r0, #0x00000002	@ set bit 1 (--A-) Align
	orr	r0, r0, #0x00000800	@ set bit 11 (Z---) BTB
#ifdef CONFIG_SYS_ICACHE_OFF
	bic	r0, r0, #0x00001000	@ clear bit 12 (I) I-cache
#else
	orr	r0, r0, #0x00001000	@ set bit 12 (I) I-cache
#endif
	mcr	p15, 0, r0, c1, c0, 0

#ifdef CONFIG_ARM_ERRATA_716044
	mrc	p15, 0, r0, c1, c0, 0	@ read system control register
	orr	r0, r0, #1 << 11	@ set bit #11
	mcr	p15, 0, r0, c1, c0, 0	@ write system control register
#endif

#if (defined(CONFIG_ARM_ERRATA_742230) || defined(CONFIG_ARM_ERRATA_794072))
	mrc	p15, 0, r0, c15, c0, 1	@ read diagnostic register
	orr	r0, r0, #1 << 4		@ set bit #4
	mcr	p15, 0, r0, c15, c0, 1	@ write diagnostic register
#endif

#ifdef CONFIG_ARM_ERRATA_743622
	mrc	p15, 0, r0, c15, c0, 1	@ read diagnostic register
	orr	r0, r0, #1 << 6		@ set bit #6
	mcr	p15, 0, r0, c15, c0, 1	@ write diagnostic register
#endif

#ifdef CONFIG_ARM_ERRATA_751472
	mrc	p15, 0, r0, c15, c0, 1	@ read diagnostic register
	orr	r0, r0, #1 << 11	@ set bit #11
	mcr	p15, 0, r0, c15, c0, 1	@ write diagnostic register
#endif
#ifdef CONFIG_ARM_ERRATA_761320
	mrc	p15, 0, r0, c15, c0, 1	@ read diagnostic register
	orr	r0, r0, #1 << 21	@ set bit #21
	mcr	p15, 0, r0, c15, c0, 1	@ write diagnostic register
#endif

	mov	r5, lr			@ Store my Caller
	mrc	p15, 0, r1, c0, c0, 0	@ r1 has Read Main ID Register (MIDR)
	mov	r3, r1, lsr #20		@ get variant field
	and	r3, r3, #0xf		@ r3 has CPU variant
	and	r4, r1, #0xf		@ r4 has CPU revision
	mov	r2, r3, lsl #4		@ shift variant field for combined value
	orr	r2, r4, r2		@ r2 has combined CPU variant + revision

#ifdef CONFIG_ARM_ERRATA_798870
	cmp	r2, #0x30		@ Applies to lower than R3p0
	bge	skip_errata_798870      @ skip if not affected rev
	cmp	r2, #0x20		@ Applies to including and above R2p0
	blt	skip_errata_798870      @ skip if not affected rev

	mrc	p15, 1, r0, c15, c0, 0  @ read l2 aux ctrl reg
	orr	r0, r0, #1 << 7         @ Enable hazard-detect timeout
	push	{r1-r5}			@ Save the cpu info registers
	bl	v7_arch_cp15_set_l2aux_ctrl
	isb				@ Recommended ISB after l2actlr update
	pop	{r1-r5}			@ Restore the cpu info - fall through
skip_errata_798870:
#endif

#ifdef CONFIG_ARM_ERRATA_801819
	cmp	r2, #0x24		@ Applies to lt including R2p4
	bgt	skip_errata_801819      @ skip if not affected rev
	cmp	r2, #0x20		@ Applies to including and above R2p0
	blt	skip_errata_801819      @ skip if not affected rev
	mrc	p15, 0, r0, c0, c0, 6	@ pick up REVIDR reg
	and	r0, r0, #1 << 3		@ check REVIDR[3]
	cmp	r0, #1 << 3
	beq	skip_errata_801819	@ skip erratum if REVIDR[3] is set

	mrc	p15, 0, r0, c1, c0, 1	@ read auxilary control register
	orr	r0, r0, #3 << 27	@ Disables streaming. All write-allocate
					@ lines allocate in the L1 or L2 cache.
	orr	r0, r0, #3 << 25	@ Disables streaming. All write-allocate
					@ lines allocate in the L1 cache.
	push	{r1-r5}			@ Save the cpu info registers
	bl	v7_arch_cp15_set_acr
	pop	{r1-r5}			@ Restore the cpu info - fall through
skip_errata_801819:
#endif

#ifdef CONFIG_ARM_ERRATA_454179
	cmp	r2, #0x21		@ Only on < r2p1
	bge	skip_errata_454179

	mrc	p15, 0, r0, c1, c0, 1	@ Read ACR
	orr	r0, r0, #(0x3 << 6)	@ Set DBSM(BIT7) and IBE(BIT6) bits
	push	{r1-r5}			@ Save the cpu info registers
	bl	v7_arch_cp15_set_acr
	pop	{r1-r5}			@ Restore the cpu info - fall through

skip_errata_454179:
#endif

#ifdef CONFIG_ARM_ERRATA_430973
	cmp	r2, #0x21		@ Only on < r2p1
	bge	skip_errata_430973

	mrc	p15, 0, r0, c1, c0, 1	@ Read ACR
	orr	r0, r0, #(0x1 << 6)	@ Set IBE bit
	push	{r1-r5}			@ Save the cpu info registers
	bl	v7_arch_cp15_set_acr
	pop	{r1-r5}			@ Restore the cpu info - fall through

skip_errata_430973:
#endif

#ifdef CONFIG_ARM_ERRATA_621766
	cmp	r2, #0x21		@ Only on < r2p1
	bge	skip_errata_621766

	mrc	p15, 0, r0, c1, c0, 1	@ Read ACR
	orr	r0, r0, #(0x1 << 5)	@ Set L1NEON bit
	push	{r1-r5}			@ Save the cpu info registers
	bl	v7_arch_cp15_set_acr
	pop	{r1-r5}			@ Restore the cpu info - fall through

skip_errata_621766:
#endif

	mov	pc, r5			@ back to my caller
ENDPROC(cpu_init_cp15)

#if !defined(CONFIG_SKIP_LOWLEVEL_INIT) && \
	!defined(CONFIG_SKIP_LOWLEVEL_INIT_ONLY)
/*************************************************************************
 *
 * CPU_init_critical registers
 *
 * setup important registers
 * setup memory timing
 *
 *************************************************************************/
ENTRY(cpu_init_crit)
	/*
	 * Jump to board specific initialization...
	 * The Mask ROM will have already initialized
	 * basic memory. Go here to bump up clock rate and handle
	 * wake up conditions.
	 */
	b	lowlevel_init		@ go setup pll,mux,memory
ENDPROC(cpu_init_crit)
#endif

