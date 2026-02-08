#include "libc/memmgr.h"
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <syscalls/syscalls.h>

#include <libdataplus/sh4a/input/keypad.h>

#define MAX_FILES 100

int ret, handle;
static char *drive[2] = {
    "\\\\drv0\\",  // 内蔵ドライブ
                    //internal drive
    "\\\\crd0\\"   // SDカード
                    //SD card
};
unsigned long type;
char filename[64];

// ファイル情報を保存する構造体
struct file_entry {
    char name[64];
    unsigned long type;
};

// ファイルリスト返却用構造体
struct file_list_result {
    struct file_entry *entries;  // ファイルエントリの配列
    int count;                   // ファイル数
    int success;                 // 成功フラグ (0: 成功, 負数: エラー)
};

struct file_list_result get_file_list(char *search_path){
    struct file_list_result result = {0};
    struct file_entry *file_list = NULL;
    int total_files = 0;
    
    // ファイルリスト用メモリを確保
    file_list = (struct file_entry *)memmgr_alloc(sizeof(struct file_entry) * MAX_FILES);
    if (file_list == NULL) {
        result.success = -1;
        result.count = 0;
        result.entries = NULL;
        return result;
    }
    
    // ファイルリストを取得
    ret = sys_findfirst(search_path, &handle, filename, &type);
    
    if (ret == 0) {
        /* 最初のファイルを保存（境界チェックしてヌル終端） */
        strncpy(file_list[total_files].name, filename, sizeof(file_list[total_files].name) - 1);
        file_list[total_files].name[sizeof(file_list[total_files].name) - 1] = '\0';
        file_list[total_files].type = type;
        total_files++;
        
        /* 残りのファイルを順次取得して保存 */
        while (total_files < MAX_FILES) {
            ret = sys_findnext(handle, filename, &type);
            if (ret != 0) break;
            
            strncpy(file_list[total_files].name, filename, sizeof(file_list[total_files].name) - 1);
            file_list[total_files].name[sizeof(file_list[total_files].name) - 1] = '\0';
            file_list[total_files].type = type;
            total_files++;
        }
        
        sys_findclose(handle);
        result.success = 0;
        result.entries = file_list;
        result.count = total_files;
    } else {
        /* ファイルが見つからなかった／エラー：確保したバッファを解放してエラーを返す */
        memmgr_free(file_list);
        result.success = -2;
        result.entries = NULL;
        result.count = 0;
    }
    
    return result;
}
int element_delete(char *selected_full){
    int delete_result = sys_delete(selected_full);
    return delete_result;
}
#define COPY_BUF 4096
int file_create(char *path,char *file_name){
    char fullpath[128];
    char path_clean[128];

    /* NULL 安全と '*' の除去 */
    if (!path) path = "";
    strncpy(path_clean, path, sizeof(path_clean) - 1);
    path_clean[sizeof(path_clean) - 1] = '\0';
    char *star = strchr(path_clean, '*');
    if (star) *star = '\0';

    size_t path_len = strlen(path_clean);
    size_t name_len = file_name ? strlen(file_name) : 0;
    int need_sep = (path_len == 0 || path_clean[path_len-1] == '\\' || path_clean[path_len-1] == '/') ? 0 : 1;
    if (path_len + need_sep + name_len + 1 > sizeof(fullpath)) {
        return -3;
    }
    if (need_sep)
        sprintf(fullpath, "%s\\%s", path_clean, file_name);
    else
        sprintf(fullpath, "%s%s", path_clean, file_name ? file_name : "");
    /* 正しいパスでファイルを作成 */
    int ret = sys_create(fullpath, 1); /* 1 = file */
    return ret; 
}

int directory_create(char *path,char *file_name){
    char fullpath[128];
    char path_clean[128];

    /* NULL 安全と '*' の除去 */
    if (!path) path = "";
    strncpy(path_clean, path, sizeof(path_clean) - 1);
    path_clean[sizeof(path_clean) - 1] = '\0';
    char *star = strchr(path_clean, '*');
    if (star) *star = '\0';

    size_t path_len = strlen(path_clean);
    size_t name_len = file_name ? strlen(file_name) : 0;
    int need_sep = (path_len == 0 || path_clean[path_len-1] == '\\' || path_clean[path_len-1] == '/') ? 0 : 1;
    if (path_len + need_sep + name_len + 1 > sizeof(fullpath)) {
        return -3;
    }
    if (need_sep)
        sprintf(fullpath, "%s\\%s", path_clean, file_name);
    else
        sprintf(fullpath, "%s%s", path_clean, file_name ? file_name : "");
    /* 正しいパスでファイルを作成 */
    int ret = sys_create(fullpath, 5); /* 5 = directory */
    return ret; 
}

#define SCREEN_WIDTH 528
#define SCREEN_HEIGHT 320

int file_copy(char *src_path,char *dest_path){
    char buf[COPY_BUF];

    int fd = sys_open(src_path, FILE_RD);
    if (fd < 0) {
        return fd;
    }

    int ret_create = sys_create(dest_path, 1);
    if (ret_create < 0) {
        sys_close(fd);
        return ret_create;
    }

    int dest_fd = sys_open(dest_path, FILE_WR);
    if (dest_fd < 0) {
        sys_close(fd);
        return dest_fd;
    }
    int filesize = sys_get_filesize(fd);
    long copied = 0; /* accumulate bytes safely */
    int unknown_size = 0;
    if (filesize <= 0) {
        /* sys_get_filesize failed or returned 0: mark unknown to avoid div-by-zero/negative percent */
        unknown_size = 1;
    }

    int r;
    while ((r = sys_read(fd, buf, sizeof(buf))) > 0) {
        int w = sys_write(dest_fd, buf, r);
        if (w != r) {
            sys_close(fd);
            sys_close(dest_fd);
            return -1;
        }
    }

    sys_close(fd);
    sys_close(dest_fd);
    return 0;
}
