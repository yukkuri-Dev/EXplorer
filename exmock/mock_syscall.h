// mock_syscall.h の先頭に追加
#pragma once
#define _SYSCALLS_H // これで syscalls.h の内容が二重に定義されるのを防ぐ

#ifndef MOCK_SYSCALL_H
#define MOCK_SYSCALL_H

#ifdef __cplusplus
extern "C" {
#endif

#define FILE_RD   0x1
#define FILE_WR   0x2
#define FILE_RDWR 0x3

// File system & storage
int sys_sdformat(void);
int sys_totaldiskspace(const char *device, unsigned long *total);
int sys_freediskspace(const char *device, unsigned long *free);
int sys_create(const char *path, int flags);
int sys_delete(const char *path);
int sys_open(const char *path, int flags);
int sys_close(int fd);
int sys_get_filesize(int fd);
int sys_seek(int fd, int offset, int whence);
int sys_read(int fd, void *buffer, int size);
int sys_write(int fd, const void *buffer, int size);
int sys_write2(int fd, const void *buffer, int size, int offset);

// String operations
unsigned int sys_strdiff(const char *s1, const char *s2);
unsigned int sys_strlen(const char *s1);
int sys_strcmp(const char *s1, const char *s2);
int sys_strrcmp(const char *s1, const char *s2);
void sys_strtolower(char *str);

#ifdef __cplusplus
}
#endif

#endif // MOCK_SYSCALL_H
