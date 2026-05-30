
#include <asm.h>

DEFINE_SYSCALL(0x66, sys_strdiff); // return index of first differing character
DEFINE_SYSCALL(0x67, sys_strlen);
DEFINE_SYSCALL(0x68, sys_strcmp);
DEFINE_SYSCALL(0x69, sys_strrcmp); // compares two strings from right to left
DEFINE_SYSCALL(0x6c, sys_bzero);
DEFINE_SYSCALL(0x16a, sys_strtolower);
