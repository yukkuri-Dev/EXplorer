

#include <asm.h>

DEFINE_SYSCALL(0x31, sys_sdformat);
DEFINE_SYSCALL(0x32, sys_totaldiskspace); 
DEFINE_SYSCALL(0x33, sys_freediskspace);
DEFINE_SYSCALL(0x34, sys_create);
DEFINE_SYSCALL(0x35, sys_delete);
DEFINE_SYSCALL(0x36, sys_rename);
DEFINE_SYSCALL(0x37, sys_open);
DEFINE_SYSCALL(0x38, sys_close);
DEFINE_SYSCALL(0x39, sys_closeall);
DEFINE_SYSCALL(0x3a, sys_get_filesize);
DEFINE_SYSCALL(0x3b, sys_seek);
DEFINE_SYSCALL(0x3c, sys_read);
DEFINE_SYSCALL(0x3d, sys_write);
DEFINE_SYSCALL(0x3e, sys_write2);
DEFINE_SYSCALL(0x3f, sys_findfirst);
DEFINE_SYSCALL(0x40, sys_findnext);
DEFINE_SYSCALL(0x41, sys_findclose);
