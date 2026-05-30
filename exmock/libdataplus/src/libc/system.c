
#include <sys/iosupport.h>
#include <stdint.h>


extern void __libexword_exit(int status);
extern void * __libexword_sbrk_r(struct _reent *ptr, ptrdiff_t incr);
extern void * __libexword_malloc(size_t size);
extern void __libexword_free(void *ptr);

extern void __console_init();
extern void __console_cleanup();

extern void __fs_init();

static void __init_syscall_array()
{
	__syscalls.sbrk_r = __libexword_sbrk_r;
	__syscalls.lock_init = NULL;
	__syscalls.lock_close = NULL;
	__syscalls.lock_release = NULL;
	__syscalls.lock_acquire = NULL;
	__syscalls.malloc = __libexword_malloc;
	__syscalls.free = __libexword_free;
	__syscalls.exit = __libexword_exit;
	__syscalls.gettod_r = NULL;
}

int __libc_cleanup_hook(void * ptr)
{
	__console_cleanup();

	return -3;
}

int __libc_init_hook(void *ptr)
{

	__init_syscall_array();	

	__console_init();
	__fs_init();
	return -3;

}

