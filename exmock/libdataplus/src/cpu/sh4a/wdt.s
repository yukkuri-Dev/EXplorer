.global _wdt_disable, _wdt_enable, _wdt_set_cnt


/* void wdt_set_cnt(uint8_t value) */
_wdt_set_cnt:
	extu.b	r4,r4
	mov.l	.L0 + 4, r1
	mov	#0x5a, r2
	shll8	r2
	or	r2, r4
	rts
	mov.w	r4, @r1

/* void wdt_disable() */
_wdt_disable:
	mov.l	.L0, r1
	mov	#0xa5, r4
	shll8	r4
	rts
	mov.w	r4, @r1

/* void wdt_enable(uint8_t timer) */
_wdt_enable:
	mov.l	.L0, r1
	mov	r4,    r0
	and	#7,    r0
	or	#0x80, r0
	mov	#0xa5, r4
	shll8	r4
	or	r4,    r0
	rts
	mov.w	r0,    @r1
.align 2
.L0:
.long 0xa4520004
.long 0xa4520000

