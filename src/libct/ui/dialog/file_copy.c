#include <libct/ui/dialog/user_input_dialog.h>
#include <libct/print.h>
#include <libdataplus/sh4a/input/keypad.h>
#include <syscalls/syscalls.h>
#include <libc/memmgr.h>
#define MAX_FILES 100
#define COPY_BUF 4096
#define SCREEN_WIDTH 528
#define SCREEN_HEIGHT 320

int file_copy_dialog(char *src_path,char *dest_path){
    if (!src_path || !dest_path) {
        popup_dialog("Invalid path.", create_rgb16(255, 0, 0));
        return -1;
    }
    int confirm = yes_or_no_dialog("Starting copy...", create_rgb16(0, 255, 0));
    if (confirm != 0) {
        return -2;
    }
    ct_screen_clear(create_rgb16(0,0,0));
    ct_print(10, 10, "Copying file...", create_rgb16(255, 255, 0));
    lcdc_copy_vram();
    
    char *buf = (char *)memmgr_alloc(COPY_BUF);
    if (!buf) {
        popup_dialog("Out of memory.", create_rgb16(255,0,0));
        return -1;
    }

    int fd = sys_open(src_path, FILE_RD);
    if (fd < 0) {
        popup_dialog("Cannot open source.", create_rgb16(255, 0, 0));
        memmgr_free(buf);
        return fd;
    }

    int ret_create = sys_create(dest_path, 1);
    if (ret_create < 0) {
        popup_dialog("Cannot create dest.", create_rgb16(255, 0, 0));
        sys_close(fd);
        memmgr_free(buf);
        return ret_create;
    }

    int dest_fd = sys_open(dest_path, FILE_WR);
    if (dest_fd < 0) {
        popup_dialog("Cannot open dest.", create_rgb16(255, 0, 0));
        sys_close(fd);
        memmgr_free(buf);
        return dest_fd;
    }
    int filesize = sys_get_filesize(fd);
    long copied = 0; /* accumulate bytes safely */

    int r;
    while ((r = sys_read(fd, buf, COPY_BUF)) > 0) {
        /* sys_write2 with offset to ensure correct positioning */
        int w = sys_write2(dest_fd, buf, r, copied);
        if (w != r) {
            popup_dialog("Write failed.", create_rgb16(255,0,0));
            sys_close(fd);
            sys_close(dest_fd);
            memmgr_free(buf);
            return -1;
        }

        copied += r;

        /* Only check for cancel periodically */
        if (copied % (256 * COPY_BUF) == 0) {
            if (get_key_state(KEY_BACK)) {
                popup_dialog("Cancelled.", create_rgb16(255, 0, 0));
                sys_close(fd);
                sys_close(dest_fd);
                memmgr_free(buf);
                return -2;
            }
        }
    }
    if (r < 0) {
        popup_dialog("Read error.", create_rgb16(255,0,0));
        sys_close(fd);
        sys_close(dest_fd);
        memmgr_free(buf);
        return -1;
    }

    sys_close(fd);
    sys_close(dest_fd);
    
    /* Verify final file exists and check size */
    int verify_fd = sys_open(dest_path, FILE_RD);
    if (verify_fd < 0) {
        popup_dialog("Verify failed.", create_rgb16(255,0,0));
        memmgr_free(buf);
        return -1;
    }
    int final_size = sys_get_filesize(verify_fd);
    sys_close(verify_fd);
    
    /* Show final size info */
    if (final_size == 0 && copied > 0) {
        /* File exists but is empty - write failed silently */
        popup_dialog("File is empty!", create_rgb16(255,0,0));
        memmgr_free(buf);
        return -1;
    }
    
    if (final_size != copied) {
        char dbg[80];
        sprintf(dbg, "Size mismatch: wrote %ld, file %d", copied, final_size);
        popup_dialog(dbg, create_rgb16(255,165,0));
    }
    
    memmgr_free(buf);
    return 0;
}
