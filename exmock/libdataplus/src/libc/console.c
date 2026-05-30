#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/iosupport.h>
#include <syscalls/syscalls.h>

int __console_write(struct _reent *r,int fd,const char *ptr,size_t len);
int __console_close(int fd);
int __console_open(int fd, const char * file);

struct __console {
	int fd;
	char *file;
};

static struct __console con[2] = {{-1, NULL}, {-1, NULL}};

const devoptab_t dotab_stdout = {
	"stdout",	// device name
	0,		// size of file structure
	NULL,		// device open
	NULL,		// device close
	__console_write,// device write
	NULL,		// device read
	NULL,		// device seek
	NULL,		// device fstat
	NULL,		// device stat
	NULL,		// device link
	NULL,		// device unlink
	NULL,		// device chdir
	NULL,		// device rename
	NULL,		// device mkdir
	0,		// dirStateSize
	NULL,		// device diropen_r
	NULL,		// device dirreset_r
	NULL,		// device dirnext_r
	NULL,		// device dirclose_r
	NULL,		// device statvfs_r
	NULL,		// device ftrunctate_r
	NULL,		// device fsync_r
	NULL,		// deviceData;
};

void __console_init()
{
	__console_open(STD_OUT, "\\\\drv0\\stdout.log");
	__console_open(STD_ERR, "\\\\drv0\\stderr.log");

	devoptab_list[STD_OUT] = &dotab_stdout;
	devoptab_list[STD_ERR] = &dotab_stdout;

/*
	setvbuf(stdout, NULL , _IONBF, 0);
	setvbuf(stderr, NULL , _IONBF, 0);
*/
}

void __console_cleanup()
{
	__console_close(STD_OUT);
	__console_close(STD_ERR);
}


int __console_close(int fd)
{
	struct __console *c = &con[fd - 1];
	if (c->fd >= 0) {
		sys_close(c->fd);
		free(c->file);
	}
	c->fd = -1;
	c->file = NULL;
	return 0;
}

int __console_open(int fd, const char * file)
{
	int ret = -1;
	struct __console *c = &con[fd - 1];
	c->file = strdup(file);
	if (c->file == NULL) {
		return ret;
	}
	sys_delete(c->file);
	if (sys_create(c->file, 1) == 0) {
		c->fd = sys_open(c->file, 0x3);
		if (c->fd == -1) {
			free(c->file);
			c->file = NULL;
		} else {
			ret = 0;
		}
	}
	return ret;
}

int __console_write(struct _reent *r,int fd,const char *ptr,size_t len)
{
	int ret = -1;
	struct __console *c;
	if (fd == STD_OUT || fd == STD_ERR) {
		c = &con[fd - 1];
		if (c->fd >= 0) {
			ret = sys_write(c->fd, ptr, len);
			if (ret < 0)
				r->_errno = EIO;
		} else {
			r->_errno = EINVAL;
		}
	} else {
		r->_errno = EBADF;
	}
	return ret;
}


void console_set_stdout(const char * file)
{
	__console_close(STD_OUT);
	__console_open(STD_OUT, file);
}

void console_set_stderr(const char * file)
{
	__console_close(STD_ERR);
	__console_open(STD_ERR, file);	
}

