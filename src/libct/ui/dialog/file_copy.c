#include <libct/ui/dialog/user_input_dialog.h>
#include <libct/print.h>
#include <libdataplus/sh4a/input/keypad.h>
#include <syscalls/syscalls.h>
#define MAX_FILES 100
#define COPY_BUF 4096
#define SCREEN_WIDTH 528
#define SCREEN_HEIGHT 320

int file_copy_dialog(char *src_path,char *dest_path){
    yes_or_no_dialog("Starting file copy...", create_rgb16(0, 255, 0));
    ct_screen_clear(create_rgb16(0,0,0));
    struct font *fnt = get_font();
    char buf[COPY_BUF];

    int fd = sys_open(src_path, FILE_RD);
    if (fd < 0) {
        popup_dialog("Failed to open source file.", create_rgb16(255, 0, 0));
        return fd;
    }

    int ret_create = sys_create(dest_path, 1);
    if (ret_create < 0) {
        popup_dialog("Failed to create destination file.", create_rgb16(255, 0, 0));
        sys_close(fd);
        return ret_create;
    }

    int dest_fd = sys_open(dest_path, FILE_WR);
    if (dest_fd < 0) {
        popup_dialog("Failed to open destination file.", create_rgb16(255, 0, 0));
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
            popup_dialog("Write error!", create_rgb16(255,0,0));
            sys_close(fd);
            sys_close(dest_fd);
            return -1;
        }

        copied += r;

        /* 進捗を表示。filesize が不明または0/負の場合はバイト数表示にする */
        int percent = 0;
        char msg[64];
        if (!unknown_size) {
            long long numer = (long long)copied * 100LL;
            percent = (int)(numer / (long long)filesize);
            if (percent < 0) percent = 0;
            if (percent > 100) percent = 100;
            sprintf(msg, "Copying... %d%%", percent);
        } else {
            sprintf(msg, "Copying... %ld bytes", copied);
        }
        int bar_width = 200;
        int bar_height = 20;
        int bar_x = (SCREEN_WIDTH - bar_width) / 2;
        int bar_y = SCREEN_HEIGHT / 2 + 30;

        if (get_key_state(KEY_BACK)) {
            popup_dialog("Copy cancelled by user.", create_rgb16(255, 0, 0));
            sys_close(fd);
            sys_close(dest_fd);
            return -2;
        }
        ct_screen_clear(create_rgb16(0,0,0));
        ct_print((SCREEN_WIDTH - strlen(msg) * fnt->width) / 2,
                 SCREEN_HEIGHT / 2 - fnt->height / 2,
                 msg,
                 create_rgb16(255, 255, 0));
        // Draw background bar
        ct_screen_rect(bar_x, bar_y, bar_width, bar_height, create_rgb16(100, 100, 100));

        // Draw progress fill
        int filled_width = (bar_width * percent) / 100;
        ct_screen_rect(bar_x, bar_y, filled_width, bar_height, create_rgb16(255, 255, 255));
        ct_screen_rect(bar_x, bar_y, filled_width, bar_height, create_rgb16(0, 255, 0));
        lcdc_copy_vram();
    }

    sys_close(fd);
    sys_close(dest_fd);
    return 0;
}
