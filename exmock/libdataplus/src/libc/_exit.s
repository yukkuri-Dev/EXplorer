
.text
.globl ___libexword_exit
.type ___libexword_exit, @function
___libexword_exit:
        mov.l .L0, r1
        mov.l @r1+, r8
        mov.l @r1+, r9
        mov.l @r1+, r10
        mov.l @r1+, r11
        mov.l @r1+, r12
        mov.l @r1+, r13
        mov.l @r1+, r14
        mov.l @r1+, r15
	lds.l @r1+, pr
        mov   r4, r0
        rts
	ldc.l @r1+, gbr

.align 2
.L0:
.long 0x74000024
.size ___libexword_exit, .-___libexword_exit

