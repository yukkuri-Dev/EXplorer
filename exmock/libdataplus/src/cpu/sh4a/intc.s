.global _get_interrupt, _set_interrupt

/* void * get_interrupt(uint16_t code) */
.type _get_interrupt, @function
_get_interrupt:
	mov.l	.L0, r0
	extu.w	r4, r4
	add	#-64, r4
	shlr	r4
	shlr2	r4
	rts
	mov.l	@(r0, r4), r0
.align 2
.L0:
.long	0xfd800000
.size _get_interrupt, .-_get_interrupt

/* void * set_interrupt(uint16_t code, void* func_ptr) */
.type _set_interrupt, @function
_set_interrupt:
	mov.l	.L1, r0
	extu.w	r4, r4
	add	#-64, r4
	shlr	r4
	shlr2	r4
	mov.l	@(r0, r4), r1
	mov.l	r5, @(r0, r4)
	synco
	rts
	mov	r1, r0
.align 2
.L1:
.long	0xfd800000
.size _set_interrupt, .-_set_interrupt


/* void cli(void) */
.type _cli, @function
_cli:
	mov.l	.L2, r1
	mov.l	.L2 + 4, r2
	rts
	mov.l	r2, @r1
.L2:
.long 0xa4700000
.long 0xa50000f0
.size _cli, .-_cli


/* void sei(void) */
.type _sei, @function
_sei:
	mov.l	.L3, r1
	mov.l	.L3 + 4, r2
	rts
	mov.l	r2, @r1
.L3:
.long 0xa4700000
.long 0xa5000000
.size _sei, .-_sei
