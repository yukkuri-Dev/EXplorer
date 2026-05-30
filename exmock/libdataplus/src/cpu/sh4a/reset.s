.global _reboot

_reboot:
	mov.l	.L0, r1
	jmp	@r1
	nop
	nop
.L0:
.long	0xA0000000
