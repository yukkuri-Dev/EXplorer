#ifndef FS_CONTROL_H
#define FS_CONTROL_H

#include <stddef.h>

#define MAX_FILES 100

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

// 関数宣言
/**
 * @brief 検索パスに一致するファイル一覧を取得する。
 * @param search_path ファイルを検索するパス。
 * @return ファイル一覧・件数・成功状態を含む構造体。
 * @note 戻り値の構造体に含まれるentries配列の解放は呼び出し側が行う。
 */
struct file_list_result get_file_list(char *search_path);


/**
 * @brief 指定されたパスのファイルまたはディレクトリを削除する。
 * @param selected_full 削除するファイルまたはディレクトリのフルパス。
 * @return 0: 成功、負数: エラーコード。
 */
int element_delete(char *selected_full);

/**
 * @brief 指定されたパスに新しいファイルを作成する。
 * @param path ファイルを作成するディレクトリのパス。
 * @param file_name 作成するファイルの名前。
 * @return 0: 成功、負数: エラーコード。
 */
int file_create(char *path,char *file_name);
/**
 * @brief 指定されたパスに新しいディレクトリを作成する。
 * @param path ディレクトリを作成する親ディレクトリのパス。
 * @param file_name 作成するディレクトリの名前。
 * @return 0: 成功、負数: エラーコード。
 */
int directory_create(char *path,char *file_name);
/**
 * @brief 指定されたパス+ファイル名のデータを指定されたディレクトリにコピーする。
 * @param src_path コピー元のファイルのパス+ファイル名。
 * @param dest_path コピー先のディレクトリのパス+ファイル名。
 * @return 0: 成功、負数: エラーコード。
 */
int file_copy(char *src_path,char *dest_path);
#endif