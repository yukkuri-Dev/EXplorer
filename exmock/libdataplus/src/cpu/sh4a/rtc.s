.global _rtc_set_time, _rtc_get_time

.extern _bin2bcd, _bcd2bin

/* void rtc_set_time(struct rtc_time *tm) */
.type _rtc_set_time, @function
_rtc_set_time:
	mov.l	.L0, r1
	mov.l	.L0 + 4, r2
	mov.b	@r2, r0
	and	#0xfe, r0
	or	#2, r0
	mov.b	r0, @r2

	mov.b	@(0, r4), r0
	mov.b	r0, @(0, r1)
	mov.b	@(1, r4), r0
	mov.b	r0, @(2, r1)
	mov.b	@(2, r4), r0
	mov.b	r0, @(4, r1)
	mov.b	@(3, r4), r0
	mov.b	r0, @(6, r1)
	mov.b	@(4, r4), r0
	mov.b	r0, @(8, r1)
	mov.b	@(5, r4), r0
	mov.b	r0, @(10, r1)
	mov.w	@(6, r4), r0
	mov.w	r0, @(12, r1)

	mov.b	@r2, r0
	or	#1, r0
	rts
	mov.b	r0, @r2
.L0:
.long 0xa413fec2
.long 0xa413fede
.long _bin2bcd
.size _rtc_set_time, .-_rtc_set_time

.type _rtc_get_time, @function
_rtc_get_time:
	mov.l	.L1, r1
	mov.l	.L1 + 4, r2
	mov.b	@r2, r0
.L2:
	and	#0x6f, r0
	mov.b	r0, @r2

	mov.b	@(0, r1), r0
	mov.b	r0, @(0, r4)

	mov.b	@(2, r1), r0
	mov.b	r0, @(1, r4)

	mov.b	@(4, r1), r0
	mov.b	r0, @(2, r4)

	mov.b	@(6, r1), r0
	mov.b	r0, @(3, r4)

	mov.b	@(8, r1), r0
	mov.b	r0, @(4, r4)

	mov.b	@(10, r1), r0
	mov.b	r0, @(5, r4)

	mov.w	@(12, r1), r0
	mov.w	r0, @(6, r4)

	mov.b	@r2, r0
	tst	#0x80, r0
	bf	.L2
	rts
	nop
.L1:
.long 0xa413fec2
.long 0xa413fedc
.long _bcd2bin
.size _rtc_get_time, .-_rtc_get_time
