.global _frqcr_kick, _set_pll_mult, _set_iclk_div, _set_shclk_div, _set_bclk_div, _set_pclk_div

/*
 * FRQCR
 * 31: Kick
 *   Writing 1 here updates CPG settings
 * 29-24: PLL
 *   Multiplier = (PLL + 1) * 2
 * 23-20: I_CLK
 *   CPU Clock Divisor
 * 15-12: SH_CLK
 *   SuperHighway Clock Divisor
 * 11-8: B_CLK
 *   Bus Clock Divisor
 * 3-0: P_CLK
 *   Peripheral Clock Divisor
 *
 * Divisor values:
 *   0000 x1/2
 *   0001 x1/3
 *   0010 x1/4
 *   0011 x1/6
 *   0100 x1/8
 *   0101 x1/12
 *   0110 x1/16
 *   1000 x1/24
 *   1001 x1/32
 *   1010 x1/36
 *   1011 x1/48
 *   1101 x1/72
 *
 * Updates to the FRQCR do not go into effect until 1 is written to the kick bit.
 *
 */

/* void frqcr_kick() */
.type _frqcr_kick, @function
_frqcr_kick:
	mov.l	.L0, r1
	mov.l	.L0 + 4, r3
	mov	#0x80, r0
	shll16	r0
	shll8	r0
	mov.l	@r1, r2
	or	r0, r2
	mov.l	r2, @r1
	synco
.L6:
	mov.l	@r3, r0
	tst	#1, r0
	bf	.L6
	rts
	nop
.L0:
.long 0xa4150000
.long 0xa4150060
.size _frqcr_kick, .-_frqcr_kick

/* void set_pclk_div(int divisor) */
.type _set_pclk_div, @function
_set_pclk_div:
	mov	r4, r0
	and	#0xF, r0
	mov.l	.L1, r1
	mov.l	.L1 + 4, r3
	mov.l	@r1, r2
	and	r3, r2
	or	r0, r2
	rts
	mov.l	r2, @r1
.align 2
.L1:
.long 0xa4150000
.long 0xFFFFFFF0
.size _set_pclk_div, .-_set_pclk_div

/* void set_bclk_div(int divisor) */
.type _set_bclk_div, @function
_set_bclk_div:
	mov	r4, r0
	and	#0xF, r0
	shll8	r0
	mov.l	.L2, r1
	mov.l	.L2 + 4, r3
	mov.l	@r1, r2
	and	r3, r2
	or	r0, r2
	rts
	mov.l	r2, @r1
.L2:
.long 0xa4150000
.long 0xFFFFF0FF
.size _set_bclk_div, .-_set_bclk_div

/* void set_shclk_div(int divisor) */
.type _set_shclk_div, @function
_set_shclk_div:
	mov	r4, r0
	and	#0xF, r0
	shll8	r0
	shll2	r0
	shll2	r0
	mov.l	.L3, r1
	mov.l	.L3 + 4, r3
	mov.l	@r1, r2
	and	r3, r2
	or	r0, r2
	rts
	mov.l	r2, @r1
.L3:
.long 0xa4150000
.long 0xFFFF0FFF
.size _set_shclk_div, .-_set_shclk_div

/* void set_iclk_div(int divisor) */
.type _set_iclk_div, @function
_set_iclk_div:
	mov	r4, r0
	and	#0xF, r0
	shll16	r0
	shll2	r0
	shll2	r0
	mov.l	.L4, r1
	mov.l	.L4 + 4, r3
	mov.l	@r1, r2
	and	r3, r2
	or	r0, r2
	rts
	mov.l	r2, @r1
.L4:
.long 0xa4150000
.long 0xFF0FFFFF
.size _set_iclk_div, .-_set_iclk_div

/* void set_pll_mult(int multiplier) */
.type _set_pll_mult, @function
_set_pll_mult:
	mov	r4, r0
	and	#0x3F, r0
	shll16	r0
	shll8	r0
	mov.l	.L5, r1
	mov.l	.L5 + 4, r3
	mov.l	@r1, r2
	and	r3, r2
	or	r0, r2
	rts
	mov.l	r2, @r1
.align 2
.L5:
.long 0xa4150000
.long 0xC0FFFFFF
.size _set_pll_mult, .-_set_pll_mult
