#include <libct/ui/dialog/system_storage_info.h>
#include <libct/ui/dialog/user_input_dialog.h>
#include <libct/print.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

static const char *drive[2] = {
    "drv0",  // 内蔵ドライブ
    "crd0"   // SDカード
};

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

    // 修正：残り容量を正しく計算
    size_t remaining = (off < len) ? (len - off) : 0;
    if (remaining > 1) {
        strncat(buf, unit, remaining - 1);
    }
}

void system_storage_info_dialog(const char *unit) {
    ct_screen_clear(create_rgb16(0,0,0));

    char info_buf[9][64];
    memset(info_buf, 0, sizeof(info_buf));  // ← 初期化を追加

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
        
        // 修正：strncpy を使用してバッファオーバーフロー防止
        strncpy(info_buf[2], ">Total size: ", sizeof(info_buf[2]) - 1);
        info_buf[2][sizeof(info_buf[2]) - 1] = '\0';
        format_size(total, unit,
                    info_buf[2] + strlen(info_buf[2]),
                    sizeof(info_buf[2]) - strlen(info_buf[2]));
        
        strncpy(info_buf[3], ">Using size: ", sizeof(info_buf[3]) - 1);
        info_buf[3][sizeof(info_buf[3]) - 1] = '\0';
        format_size(used, unit,
                    info_buf[3] + strlen(info_buf[3]),
                    sizeof(info_buf[3]) - strlen(info_buf[3]));
        
        strncpy(info_buf[4], ">Free size: ", sizeof(info_buf[4]) - 1);
        info_buf[4][sizeof(info_buf[4]) - 1] = '\0';
        format_size(free_spc, unit,
                    info_buf[4] + strlen(info_buf[4]),
                    sizeof(info_buf[4]) - strlen(info_buf[4]));
    } else {
        strncpy(info_buf[2], ">Total size: N/A", sizeof(info_buf[2]) - 1);
        strncpy(info_buf[3], ">Using size: N/A", sizeof(info_buf[3]) - 1);
        strncpy(info_buf[4], ">Free size: N/A", sizeof(info_buf[4]) - 1);
        info_buf[2][sizeof(info_buf[2]) - 1] = '\0';
        info_buf[3][sizeof(info_buf[3]) - 1] = '\0';
        info_buf[4][sizeof(info_buf[4]) - 1] = '\0';
    }

    // SDカード状態判定
    unsigned long sd_total = 0, sd_free = 0, sd_used = 0;
    int sd_status = sys_totaldiskspace(drive[1], &sd_total);
    
    if (sd_status == -33) {  // カード未挿入
        strncpy(info_buf[5], "SD Card Drive: Not Mounted", sizeof(info_buf[5]) - 1);
        strncpy(info_buf[6], ">Total size: N/A", sizeof(info_buf[6]) - 1);
        strncpy(info_buf[7], ">Using size: N/A", sizeof(info_buf[7]) - 1);
        strncpy(info_buf[8], ">Free size: N/A", sizeof(info_buf[8]) - 1);
        info_buf[5][sizeof(info_buf[5]) - 1] = '\0';
        info_buf[6][sizeof(info_buf[6]) - 1] = '\0';
        info_buf[7][sizeof(info_buf[7]) - 1] = '\0';
        info_buf[8][sizeof(info_buf[8]) - 1] = '\0';
    } else if (sd_status == 0) { // 挿入済み且つ容量取得成功
        strncpy(info_buf[5], "SD Card Drive: Mounted", sizeof(info_buf[5]) - 1);
        info_buf[5][sizeof(info_buf[5]) - 1] = '\0';

        if (sys_freediskspace(drive[1], &sd_free) == 0) {
            sd_used = sd_total - sd_free;

            strncpy(info_buf[6], ">Total size: ", sizeof(info_buf[6]) - 1);
            info_buf[6][sizeof(info_buf[6]) - 1] = '\0';
            format_size(sd_total, unit,
                        info_buf[6] + strlen(info_buf[6]),
                        sizeof(info_buf[6]) - strlen(info_buf[6]));
            
            strncpy(info_buf[7], ">Using size: ", sizeof(info_buf[7]) - 1);
            info_buf[7][sizeof(info_buf[7]) - 1] = '\0';
            format_size(sd_used, unit,
                        info_buf[7] + strlen(info_buf[7]),
                        sizeof(info_buf[7]) - strlen(info_buf[7]));
            
            strncpy(info_buf[8], ">Free size: ", sizeof(info_buf[8]) - 1);
            info_buf[8][sizeof(info_buf[8]) - 1] = '\0';
            format_size(sd_free, unit,
                        info_buf[8] + strlen(info_buf[8]),
                        sizeof(info_buf[8]) - strlen(info_buf[8]));
        } else {
            strncpy(info_buf[6], ">Total size: Unknown", sizeof(info_buf[6]) - 1);
            strncpy(info_buf[7], ">Using size: Unknown", sizeof(info_buf[7]) - 1);
            strncpy(info_buf[8], ">Free size: Unknown", sizeof(info_buf[8]) - 1);
            info_buf[6][sizeof(info_buf[6]) - 1] = '\0';
            info_buf[7][sizeof(info_buf[7]) - 1] = '\0';
            info_buf[8][sizeof(info_buf[8]) - 1] = '\0';
        }
    } else { // その他のエラー（通常は -2 = ドライブ無効）
        strncpy(info_buf[5], "SD Card Drive: Not valid", sizeof(info_buf[5]) - 1);
        strncpy(info_buf[6], ">Total size: N/A", sizeof(info_buf[6]) - 1);
        strncpy(info_buf[7], ">Using size: N/A", sizeof(info_buf[7]) - 1);
        strncpy(info_buf[8], ">Free size: N/A", sizeof(info_buf[8]) - 1);
        info_buf[5][sizeof(info_buf[5]) - 1] = '\0';
        info_buf[6][sizeof(info_buf[6]) - 1] = '\0';
        info_buf[7][sizeof(info_buf[7]) - 1] = '\0';
        info_buf[8][sizeof(info_buf[8]) - 1] = '\0';
    }

    info_list(info_items, 9);
}