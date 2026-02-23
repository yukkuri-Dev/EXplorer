#ifndef USER_SELECT_DIALOG_H
#define USER_SELECT_DIALOG_H

#include <stdint.h>

/** 
 *@brief ユーザーに項目を選択させるダイアログを表示する。
 *@param names 選択可能な表示名の配列（配列長は count で指定）。
 *@param count 項目の数。
 *@return 選択された項目のインデックス。 キャンセルされた場合は-1を返す。
 */
int user_select_dialog(const char **names, int count);

/* Example:
 *
 *   const char *items[] = { "Create File", "Create Directory" };
 *   int sel = user_select_dialog(items, 2);
 *   if (sel >= 0) {
 *       // sel を利用して処理
 *   }
 */

#endif /* USER_SELECT_DIALOG_H */
