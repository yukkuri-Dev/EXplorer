.global _adc_read, _adc_read_multi


/* void adc_read_multi(struct adc *channels) */
.type _adc_read_multi, @function
_adc_read_multi:
	mov.l	.L3, r2
	mov	#0x20, r0
	shll8	r0
	mov	r0, r1
	or	#0x63, r0
	mov.w	r0, @r2
.L0:
	mov.w	@r2, r0
	tst	r0, r1
	bf	.L0
	mov.l	.L3 + 4, r2
	mov.w	@(0, r2), r0
	extu.w	r0, r0
	shlr2	r0
	shlr2	r0
	shlr2	r0
	mov.w	r0, @(0, r4)
	mov.w	@(2, r2), r0
	extu.w	r0, r0
	shlr2	r0
	shlr2	r0
	shlr2	r0
	mov.w	r0, @(2, r4)
	mov.w	@(4, r2), r0
	extu.w	r0, r0
	shlr2	r0
	shlr2	r0
	shlr2	r0
	mov.w	r0, @(4, r4)
	mov.w	@(6, r2), r0
	extu.w	r0, r0
	shlr2	r0
	shlr2	r0
	shlr2	r0
	rts
	mov.w	r0, @(6, r4)
.align 2
.L3:
.long 0xA4610088
.long 0xA4610080
.size _adc_read_multi, .-_adc_read_multi

/* unsigned short adc_read(int channel) */
.type _adc_read, @function
_adc_read:
	mov.l	.L2, r2
	mov	r4, r0
	and	#3, r0
	mov	r0, r4
	or	#0x40, r0
	mov	#0x20, r1
	shll8	r1
	or	r1, r0
	mov.w	r0, @r2
.L1:
	mov.w	@r2, r0
	tst	r0, r1
	bf	.L1
	mov.l	.L2 + 4, r0
	shll	r4
	mov.w	@(r0, r4), r0
	extu.w	r0, r0
	shlr2	r0
	shlr2	r0
	rts
	shlr2	r0
.align 2
.L2:
.long 0xA4610088
.long 0xA4610080
.size _adc_read, .-_adc_read
