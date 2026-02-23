#include <stdint.h>
#include <libdataplus/graphics/color.h>
#include <libdataplus/graphics/text.h>

/**
 * @brief 指定した座標に指定した色で文字列を表示する
 * @param x 文字列を表示するx座標
 * @param y 文字列を表示するy座標
 * @param text 表示するヌル終端文字列へのポインタ
 * @param color 文字列描画に使用する色の値
 * @return void
 */
void ct_print(uint16_t x,uint16_t y,char *text,uint16_t color);
/**
 * @brief 指定した色で画面全体をクリアする
 * @param color 画面を塗りつぶす色の値
 * @return void
 */
void ct_screen_clear(uint16_t color);
/**
 * @brief 指定した色で画面全体をクリアする
 * @param x クリアする矩形の左上x座標
 * @param y クリアする矩形の左上y座標
 * @param width クリアする矩形の幅
 * @param height クリアする矩形の高さ
 * @param color 画面を塗りつぶす色の値
 * @return void
 */
void ct_screen_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color);