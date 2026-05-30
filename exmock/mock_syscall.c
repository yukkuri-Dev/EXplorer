#include "mock_syscall.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define MOCK_TOTAL_SPACE  (1024UL * 1024UL * 1024UL) // 1GB
#define MOCK_FREE_SPACE   (512UL * 1024UL * 1024UL)  // 512MB

int sys_sdformat(void) {
    return 0; // Success
}

int sys_totaldiskspace(const char *device, unsigned long *total) {
    if (!device || !total) return -1;
    *total = MOCK_TOTAL_SPACE;
    return 0;
}

int sys_freediskspace(const char *device, unsigned long *free) {
    if (!device || !free) return -1;
    *free = MOCK_FREE_SPACE;
    return 0;
}

int sys_create(const char *path, int flags) {
    if (!path) return -1;
    char real_path[1024];
    snprintf(real_path, sizeof(real_path), "./%s", path);

    // 1 = File, 5 = Directory
    if (flags == 5) {
        if (mkdir(real_path, 0755) == 0) return 0;
        return -1;
    } else {
        int fd = open(real_path, O_CREAT | O_RDWR, 0644);
        if (fd < 0) return -1;
        close(fd);
        return 0;
    }
}

int sys_delete(const char *path) {
    if (!path) return -1;
    char real_path[1024];
    snprintf(real_path, sizeof(real_path), "./%s", path);
    if (remove(real_path) == 0) return 0;
    return -1;
}

int sys_open(const char *path, int flags) {
    if (!path) return -1;
    char real_path[1024];
    snprintf(real_path, sizeof(real_path), "./%s", path);
    // Dummy flag handling, treats everything as O_RDWR
    return open(real_path, O_RDWR | O_CREAT, 0644);
}

int sys_close(int fd) {
    return close(fd);
}

int sys_get_filesize(int fd) {
    struct stat st;
    if (fstat(fd, &st) == 0) {
        return st.st_size;
    }
    return -1;
}

int sys_seek(int fd, int offset, int whence) {
    if (lseek(fd, offset, whence) == (off_t)-1) return -1;
    return 0;
}

int sys_read(int fd, void *buffer, int size) {
    return read(fd, buffer, size);
}

int sys_write(int fd, const void *buffer, int size) {
    return write(fd, buffer, size);
}

int sys_write2(int fd, const void *buffer, int size, int offset) {
    return pwrite(fd, buffer, size, offset);
}

unsigned int sys_strdiff(const char *s1, const char *s2) {
    if (!s1 || !s2) return 0;
    unsigned int i = 0;
    while (s1[i] != '\0' || s2[i] != '\0') {
        if (s1[i] != s2[i]) return i;
        i++;
    }
    return i;
}

unsigned int sys_strlen(const char *s1) {
    if (!s1) return 0;
    return (unsigned int)strlen(s1);
}

int sys_strcmp(const char *s1, const char *s2) {
    if (!s1 || !s2) return 0;
    int res = strcmp(s1, s2);
    if (res < 0) return -1;
    if (res > 0) return 1;
    return 0;
}

int sys_strrcmp(const char *s1, const char *s2) {
    if (!s1 || !s2) return 0;
    int len1 = strlen(s1);
    int len2 = strlen(s2);
    int i = len1 - 1;
    int j = len2 - 1;
    while (i >= 0 && j >= 0) {
        if (s1[i] < s2[j]) return -1;
        if (s1[i] > s2[j]) return 1;
        i--;
        j--;
    }
    if (i < 0) return -1;
    if (j < 0) return 1;
    return 0;
}

void sys_strtolower(char *str) {
    if (!str) return;
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}
