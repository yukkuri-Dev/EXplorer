#ifndef _ASM_H
#define _ASM_H


#define DECLARE_SYSCALL(rettype, funcname, ...) rettype funcname(__VA_ARGS__);

#define DEFINE_SYSCALL(no, funcname) \
	__asm__ (".text\n" \
             ".globl _" #funcname ";\n" \
             ".type _" #funcname ", @function\n" \
             "_" #funcname ":\n" \
             "mov.l @(0xc, pc), r0;\n" \
             "mov.l @(0xe, pc), r1;\n" \
             "mov.l @r1, r1;\n" \
             "jmp @r1;\n" \
             "nop;\n" \
             ".align 2\n" \
             ".long " #no ";\n" \
             ".long 0x74000000;\n" \
             ".size   _" #funcname ", .-_" #funcname "\n");

#endif
