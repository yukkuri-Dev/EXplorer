
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/iosupport.h>
#include <syscalls/syscalls.h>

extern const devoptab_t *devoptab_list[STD_MAX];

int __fs_open(struct _reent *r, void *fileStruct, const char *path, int flags, int mode);
int __fs_close(struct _reent *r, int fd);
off_t __fs_seek(struct _reent *r, int fd, off_t pos, int whence);
ssize_t __fs_write(struct _reent *r, int fd, const char *buf, size_t count);
ssize_t __fs_read(struct _reent *r, int fd, char *buf, size_t count);
int __fs_mkdir(struct _reent *r, const char *path, int mode);
int __fs_unlink(struct _reent *r, const char *path);

struct __FILE {
	int fd;
	int append;
	off_t offset;
	int size;
	char* filename;
};

const devoptab_t dotab_drv = {
	"drv",		// device name
	sizeof(struct __FILE),	// size of file structure
	__fs_open,	// device open
	__fs_close,	// device close
	__fs_write,	// device write
	__fs_read,	// device read
	__fs_seek,	// device seek
	NULL,		// device fstat
	NULL,		// device stat
	NULL,		// device link
	__fs_unlink,	// device unlink
	NULL,		// device chdir
	NULL,		// device rename
	__fs_mkdir,	// device mkdir
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

const devoptab_t dotab_crd = {
	"crd",		// device name
	sizeof(struct __FILE),	// size of file structure
	__fs_open,	// device open
	__fs_close,	// device close
	__fs_write,	// device write
	__fs_read,	// device read
	__fs_seek,	// device seek
	NULL,		// device fstat
	NULL,		// device stat
	NULL,		// device link
	__fs_unlink,	// device unlink
	NULL,		// device chdir
	NULL,		// device rename
	__fs_mkdir,	// device mkdir
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

char* __path_to_native(const char* path)
{
	char *native;
	int len;
	int i;
	char * sep = strchr(path, ':');
	if (sep == NULL)
		return NULL;
	len = sep - path;
	native = malloc(len + strlen(sep + 1) + 3);
	if (native == NULL)
		return NULL;
	strcpy(native, "\\\\");
	strncat(native, path, len);
	strcat(native, sep + 1);
	for (i = 0; i < strlen(native); ++i) {
		if (native[i] == '/')
			native[i] = '\\';
	}
	return native;
}


void __fs_init()
{
	devoptab_list[4] = &dotab_drv;
	devoptab_list[5] = &dotab_crd;
}

int __fs_open(struct _reent *r, void *fileStruct, const char *path, int flags, int mode)
{
	int creat = 0;
	struct __FILE * f = (struct __FILE *)fileStruct;
	memset(f, 0, sizeof(struct __FILE));
	int accmode = (flags & O_ACCMODE) + 1;
	f->filename = __path_to_native(path);
	f->append = (flags & O_APPEND);
	printf("Append Mode: %X\r\n", flags & O_APPEND);
	if (f->filename == NULL) {
		r->_errno = ENOMEM;
		return -1;
	}
	f->fd = sys_open(f->filename, accmode);
	if (f->fd >= 0) {
		if (flags & O_EXCL) {
			sys_close(f->fd);
			f->fd = -99;
		} else if ((flags & O_TRUNC) &&
			   (accmode & 0x2)) {
			sys_close(f->fd);
			sys_delete(f->filename);
			sys_create(f->filename, 1);
			f->fd = sys_open(f->filename, accmode);
		}
	} else if (f->fd == -1 && (flags & O_CREAT)) {
		sys_create(f->filename, 1);
		f->fd = sys_open(f->filename, accmode);
		if (f->fd == -1)
			f->fd = -98;
		printf("open with O_CREAT fd = %d\r\n", f->fd);
	}
	if (f->fd < 0) {
		switch(f->fd) {
		case -1:
			r->_errno = ENOENT;
			break;
		case -2:
			r->_errno = ENXIO;
			break;
		case -33:
			r->_errno = ENODEV;
			break;
		case -98:
			r->_errno = EISDIR;
			break;
		case -99:
			r->_errno = EEXIST;
			break;
		default:
			printf("open unknown error code %d\r\n", f->fd);
			r->_errno = EINVAL;
		}
		free(f->filename);
		f->fd = -1;
	} else {
		f->size = sys_get_filesize(f->fd);
	}
	return f->fd;
}

int __fs_close(struct _reent *r, int fd)
{
	int ret = 0;
	struct __FILE * f = (struct __FILE *)fd;
	free(f->filename);
	ret = sys_close(f->fd);
	if (ret < 0) {
		if (ret == -6)
			r->_errno = EBADF;
		else
			r->_errno = EIO;
		ret = -1;
	}
	return ret;
}

off_t __fs_seek(struct _reent *r, int fd, off_t pos, int whence)
{
	off_t offset;
	struct __FILE * f = (struct __FILE *)fd;
	if (whence == SEEK_SET) {
		offset = pos;
	} else if (whence == SEEK_CUR) {
		offset = f->offset + pos;
	} else if (whence == SEEK_END) {
		offset = f->size + pos;
	} else {
		printf("Invalid whence: %d offset %d\r\n", whence, pos);
		offset = -1;
	}
	if (offset < 0) {
		offset = -1;
		r->_errno = EINVAL;
	} else {
		f->offset = offset;
	}
	return offset;
}

ssize_t __fs_write(struct _reent *r, int fd, const char *buf, size_t count)
{
	int ret;
	char zero[512];
	memset(zero, 0, 512);
	struct __FILE * f = (struct __FILE *)fd;
	off_t offset = (f->append ? f->size : f->offset);
	while (offset > f->size) {
		size_t wrlen = 	offset - f->size < 512 ? offset - f->size : 512;
		sys_write2(f->fd, zero, wrlen, f->size);
		f->size = sys_get_filesize(f->fd);
	}
	ret = sys_write2(f->fd, buf, count, offset);
	f->offset += ret;
	f->size = sys_get_filesize(f->fd);
	return ret;
}

ssize_t __fs_read(struct _reent *r, int fd, char *buf, size_t count)
{
	int ret;
	struct __FILE * f = (struct __FILE *)fd;
	int offset = f->offset;
	if (offset >= f->size) {
		ret = 0;
	} else {
		size_t rdlen = f->size - offset < count ? f->size - offset : count;
		sys_seek(f->fd, f->offset, 0);
		ret = sys_read(f->fd, buf, rdlen);
		f->offset += ret;
	}
	return ret;
}

int __fs_mkdir(struct _reent *r, const char *path, int mode)
{
	char *filename = __path_to_native(path);
	if (filename == NULL) {
		r->_errno = ENOMEM;
		return -1;
	}
	int result = sys_create(filename, 5);
	free(filename);
	if (result < 0) {
		switch(result) {
		case -33:
			r->_errno = ENODEV;
			break;
		default:
			r->_errno = EIO;
		}
		return -1;
	}
	return 0;
}

int __fs_unlink(struct _reent *r, const char *path)
{
	char *filename = __path_to_native(path);
	if (filename == NULL) {
		r->_errno = ENOMEM;
		return -1;
	}
	sys_delete(filename);
	free(filename);
	return 0;
}

