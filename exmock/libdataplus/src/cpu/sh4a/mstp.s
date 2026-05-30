.global _mstp_enable, _mstp_disable

/* void mstp_disable(uint8_t cr, uint8_t bit) */
.type _mstp_disable, @function
_mstp_disable:
	extu.b	r5, r5
	mov	r4, r0
	and	#3, r0
	shll2	r0
	mov.l	.L0, r1
	mov.l	@(r0,r1), r2
	mov	#1, r3
	shld	r5, r3
	or	r3, r2
	rts
	mov.l	r2, @(r0,r1)
.size _mstp_disable, .-_mstp_disable

/* void mstp_enable(uint8_t cr, uint8_t bit) */
.type _mstp_enable, @function
_mstp_enable:
	extu.b	r5, r5
	mov	r4, r0
	and	#3, r0
	shll2	r0
	mov.l	.L0, r1
	mov.l	@(r0,r1), r2
	mov	#1, r3
	shld	r5, r3
	not	r3, r3
	and	r3, r2
	rts
	mov.l	r2, @(r0,r1)
.size _mstp_enable, .-_mstp_enable
.align 2
.L0:
.long 0xa4150030
