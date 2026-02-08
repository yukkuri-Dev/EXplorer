#include <stdint.h>


/**
 * @brief ユーザーからの文字列入力ダイアログを表示する。
 * @return 入力された文字列。キャンセルされた場合はNULLを返す。
 */
char *user_input_dialog(void);
/**
 * @brief ユーザーにYes/Noの選択を促すダイアログを表示する。
 * @param message 表示するメッセージ。
 * @param color メッセージの表示色。
 * @return Yesが選択された場合は0、Noが選択された場合は1
 * @note ユーザーがENTERキーを押すとYes(0)、BACKキーを押すとNo(1)とみなされる。
 */
int yes_or_no_dialog(const char *message,uint16_t color);
/**
 * @brief ユーザーにメッセージを表示するポップアップダイアログを表示する。
 * @param message 表示するメッセージ。
 * @param color メッセージの表示色。
 * @note ユーザーがENTERキーを押すまで表示される。
 */
void popup_dialog(const char *message,uint16_t color);

/**
 * @brief ユーザーに情報リストを表示するダイアログを表示する。
 * @param names 表示する情報名の配列。 
 * @param count 配列内の情報名の数。
 * @note ユーザーがENTERキーまたはBACKキーを押すまで表示される。
 */
void info_list(const char **names, int count);


/**
 * @brief ユーザーにこの操作にはリスクが伴うことを表示する。
 * @note konamiコードの入力を待ち、正しく入力された場合にのみ戻る。
 * @return 正しく入力された場合は0、キャンセルされた場合は1を返す。
 */
int konami_command_warn_dialog(void);