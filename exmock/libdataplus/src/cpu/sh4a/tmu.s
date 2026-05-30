.global _tmu0_stop, _tmu0_start, _tmu0_enable, _tmu0_disable, _tmu0_set_priority

/* void tmu0_set_priority(uint8_t timer, uint8_t pri) */
_tmu0_set_priority:
	mov.l	.L0 + 16, r1
	mov	r4, r0
	mov	#3, r4
	and	#3, r0
	sub	r0, r4
	shll2	r4
	mov	#0xF, r0
	shad	r4, r0
	not	r0, r0
	mov.w	@r1, r2
	and	r0, r2
	mov	r5, r0
	and	#0xF, r0
	shad	r4, r0
	or	r0, r2
	mov.w	r2, @r1
	synco
	rts
	nop

/* void tmu0_enable(uint8_t timer) */
_tmu0_enable:
	mov.l	.L0 + 12, r1
	mov	r4, r0
	mov	#0x10, r4
	and	#3, r0
	shad	r0, r4
	mov.b	r4, @r1
	synco
	rts
	nop

/* void tmu0_disable(uint8_t timer) */
_tmu0_disable:
	mov.l	.L0 + 8, r1
	mov	r4, r0
	mov	#0x10, r4
	and	#3, r0
	shad	r0, r4
	mov.b	r4, @r1
	synco
	rts
	nop

/* void tmu0_start(uint8_t timer, uint8_t clk, uint32_t initial) */

_tmu0_start:
	mov.l	.L0, r1
	mov.l	.L0 + 4, r2
	mov	r4, r0
	mov	#1, r4
	and	#3, r0
	shad	r0, r4
	not	r4, r4
	mov	#0xC, r3
	mulu.w	r3, r0
	sts	macl, r0
	add	r0, r2
	mov	r5, r0
	and	#7, r0
	or	#32, r0
	mov.b	@r1, r3
	and	r4, r3
	mov.b	r3, @r1
	synco
	mov.l	r6, @r2
	mov.l	r6, @(4, r2)
	mov.w	r0, @(8, r2)
	synco
	not	r4, r4
	mov.b	@r1, r3
	or	r4, r3
	mov.b	r3, @r1
	synco
	rts
	nop

/* void tmu0_stop(uint8_t timer) */
_tmu0_stop:
	mov	r4, r0
	mov	#1, r4
	and	#3, r0
	shad	r0, r4
	not	r4, r4
	mov.l	.L0, r1
	mov.b	@r1, r2
	and	r4, r2
	rts
	mov.b	r2, @r1
.align 2
.L0:
.long 0xa4490004
.long 0xa4490008
.long 0xa4080090
.long 0xa40800d0
.long 0xa4080000
