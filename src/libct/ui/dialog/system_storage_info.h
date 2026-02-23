#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
/**
 * @brief システムのストレージ情報ダイアログ
 * 
 */
void system_storage_info_dialog(const char *unit);
/**
 * @brief バイト数を指定された単位に変換する
 * 
 * @param bytes バイト数
 * @param unit 変換後の単位 ("KiB", "MiB", "GiB")
 * @return unsigned long 変換後の値
 */
unsigned long scale_bytes(unsigned long bytes, const char *unit);