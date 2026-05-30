
#include <_syslist.h>
#include <reent.h>
#include <errno.h>

extern char edata;

void* _DEFUN(__libexword_sbrk_r,(r,incr),
			 struct _reent *r _AND
			 ptrdiff_t incr)
{
	static char *heap_end; 
	char *prev_heap_end; 

	if (heap_end == 0)
		heap_end = &edata;

	if ((heap_end + incr) >= (char*)0x74008000) {
		r->_errno = ENOMEM;
		return (char*)-1;
	}

	prev_heap_end = heap_end;
	heap_end += incr;

	return (void*)prev_heap_end;
}
