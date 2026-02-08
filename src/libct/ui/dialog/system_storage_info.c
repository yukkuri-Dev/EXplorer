#include <libct/ui/dialog/system_storage_info.h>
#include <libct/ui/dialog/user_input_dialog.h>
#include <libct/print.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

static const char *drive[2] = {
    "drv0",  // 内蔵ドライブ
    "crd0"   // SDカード
};//パスを指定するとsyscallが不安定になるためドライブ名のみ使用

unsigned long scale_bytes(unsigned long bytes, const char *unit) {
    if (strcmp(unit, "KiB") == 0) return bytes / 1024UL;
    if (strcmp(unit, "MiB") == 0) return bytes / (1024UL * 1024);
    if (strcmp(unit, "GiB") == 0) return bytes / (1024UL * 1024 * 1024);
    return bytes;
}

static size_t utoa_to_buf(unsigned long v, char *dst, size_t dstlen) {
    if (dstlen == 0) return 0;

    char tmp[32];
    int i = 0;

    if (v == 0) {
        tmp[i++] = '0';
    } else {
        while (v && i < (int)sizeof(tmp) - 1) {
            tmp[i++] = (char)('0' + (v % 10));
            v /= 10;
        }
    }

    size_t tocopy = (size_t)i < (dstlen - 1) ? (size_t)i : (dstlen - 1);
    for (size_t k = 0; k < tocopy; ++k) {
        dst[k] = tmp[i - 1 - k];
    }
    dst[tocopy] = '\0';
    return tocopy;
}

void format_size(unsigned long bytes, const char *unit,
                 char *buf, size_t len) {
    if (len == 0) return;

    unsigned long v = scale_bytes(bytes, unit);
    size_t off = 0;

    off += utoa_to_buf(v, buf + off, len - off);

    if (off + 1 < len) {
        buf[off++] = ' ';
        buf[off] = '\0';
    }

    strncat(buf, unit, len - strlen(buf) - 1);
}

void system_storage_info_dialog(const char *unit) {
    ct_screen_clear(create_rgb16(0,0,0));

    char info_buf[9][64];
    const char *info_items[] = {
        "=== Storage Limits ===",
        "Internal Drive:",
        info_buf[2],
        info_buf[3],
        info_buf[4],
        "SD Card Drive:",
        info_buf[6],
        info_buf[7],
        info_buf[8],
    };

    // 内蔵ドライブ情報
    unsigned long total, free_spc, used;
    if (sys_totaldiskspace(drive[0], &total) == 0 &&
        sys_freediskspace(drive[0], &free_spc) == 0) {
        used = total - free_spc;
        strcpy(info_buf[2], ">Total size: ");
        format_size(total, unit,
                    info_buf[2] + strlen(info_buf[2]),
                    sizeof(info_buf[2]) - strlen(info_buf[2]));
        strcpy(info_buf[3], ">Using size: ");
        format_size(used, unit,
                    info_buf[3] + strlen(info_buf[3]),
                    sizeof(info_buf[3]) - strlen(info_buf[3]));
        strcpy(info_buf[4], ">Free size: ");
        format_size(free_spc, unit,
                    info_buf[4] + strlen(info_buf[4]),
                    sizeof(info_buf[4]) - strlen(info_buf[4]));
    } else {
        strcpy(info_buf[2], ">Total size: N/A");
        strcpy(info_buf[3], ">Using size: N/A");
        strcpy(info_buf[4], ">Free size: N/A");
    }

    // SDカード状態判定
    int sd_check_fd = sys_open("crd0\\SDCheck.check", 0);
    if (sd_check_fd == -33) {  // 未挿入
        strcpy(info_buf[5], "SD Card Drive: Not Mounted");
        strcpy(info_buf[6], ">Total size: N/A");
        strcpy(info_buf[7], ">Using size: N/A");
        strcpy(info_buf[8], ">Free size: N/A");
    } else if (sd_check_fd == 0 || sd_check_fd == -13 || sd_check_fd == -11) { // 挿入済み
        strcpy(info_buf[5], "SD Card Drive: Mounted");

        // 容量取得
        unsigned long sd_total = 0, sd_free = 0, sd_used = 0;
        if (sys_totaldiskspace(drive[1], &sd_total) == 0 &&
            sys_freediskspace(drive[1], &sd_free) == 0) {
            sd_used = sd_total - sd_free;

            strcpy(info_buf[6], ">Total size: ");
            format_size(sd_total, unit,
                        info_buf[6] + strlen(info_buf[6]),
                        sizeof(info_buf[6]) - strlen(info_buf[6]));
            strcpy(info_buf[7], ">Using size: ");
            format_size(sd_used, unit,
                        info_buf[7] + strlen(info_buf[7]),
                        sizeof(info_buf[7]) - strlen(info_buf[7]));
            strcpy(info_buf[8], ">Free size: ");
            format_size(sd_free, unit,
                        info_buf[8] + strlen(info_buf[8]),
                        sizeof(info_buf[8]) - strlen(info_buf[8]));
        } else {
            strcpy(info_buf[6], ">Total size: Unknown");
            strcpy(info_buf[7], ">Using size: Unknown");
            strcpy(info_buf[8], ">Free size: Unknown");
        }

    } else if (sd_check_fd == -2) { // 無効
        strcpy(info_buf[5], "SD Card Drive: Not valid");
        strcpy(info_buf[6], ">Total size: N/A");
        strcpy(info_buf[7], ">Using size: N/A");
        strcpy(info_buf[8], ">Free size: N/A");
    }

    info_list(info_items, 9);
}
