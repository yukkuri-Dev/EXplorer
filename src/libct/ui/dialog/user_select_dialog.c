#include "user_select_dialog.h"
#include <libct/print.h>
#include <libct/input.h>
#include <graphics/drawing.h>
#include <graphics/color.h>
#include <graphics/text.h>
#include <graphics/lcdc.h>
#include <sh4a/input/keypad.h>
#include <string.h>

#define SCREEN_WIDTH 528
#define SCREEN_HEIGHT 320
#define MAX_DISPLAY_ITEMS 12

int user_select_dialog(const char **names, int count){
    if (!names || count <= 0) return -1;

    ct_screen_clear(create_rgb16(0,0,0));
    struct font *fnt = get_font();

    ct_print(10, 10, "=== Select Item ===", create_rgb16(255, 255, 0));
    ct_print((SCREEN_WIDTH - strlen("Press ENTER to Select. BACK to Cancel.") * fnt->width) / 2,
             SCREEN_HEIGHT - fnt->height - 10,
             "Press ENTER to Select. BACK to Cancel.",
             create_rgb16(0, 255, 0));
    lcdc_copy_vram();

    int selected = 0;
    int prev_selected = 0;
    int scroll = 0;

    /* initial draw */
    for (int i = 0; i < MAX_DISPLAY_ITEMS && i < count; ++i) {
        const char *name = names[i];
        if (!name) name = "(null)";
        render_text(10, 30 + i * (fnt->height + 2), (char *)name);
    }
    /* draw cursor */
    ct_print(0, 30, ">", create_rgb16(0,0,255));
    lcdc_copy_vram();

    while (1) {
        keypad_read();
        if (get_key_state(KEY_ENTER)) {
            while (get_key_state(KEY_ENTER)) keypad_read();
            return selected;
        }
        if (get_key_state(KEY_BACK)) {
            while (get_key_state(KEY_BACK)) keypad_read();
            return -1;
        }
        if (get_key_state(KEY_UP)) {
            if (selected > 0) {
                int old = selected;
                --selected;
                if (selected < scroll) {
                    scroll = selected;
                    /* redraw list */
                    ct_screen_rect(0, 30, SCREEN_WIDTH, MAX_DISPLAY_ITEMS * (fnt->height + 2), create_rgb16(0,0,0));
                    for (int i = 0; i < MAX_DISPLAY_ITEMS && (scroll + i) < count; ++i) {
                        const char *name = names[scroll + i];
                        if (!name) name = "(null)";
                        render_text(10, 30 + i * (fnt->height + 2), (char *)name);
                    }
                }
                /* clear previous cursor if it was visible */
                int prev_screen_pos = old - scroll;
                if (prev_screen_pos >= 0 && prev_screen_pos < MAX_DISPLAY_ITEMS) {
                    ct_screen_rect(0, 30 + prev_screen_pos * (fnt->height + 2), fnt->width + 4, fnt->height + 2, create_rgb16(0,0,0));
                }
                /* draw new cursor */
                ct_print(0, 30 + (selected - scroll) * (fnt->height + 2), ">", create_rgb16(0,0,255));
                lcdc_copy_vram();
                prev_selected = selected;
            }
            while (get_key_state(KEY_UP)) keypad_read();
        }
        if (get_key_state(KEY_DOWN)) {
            if (selected < count - 1) {
                int old = selected;
                ++selected;
                if (selected >= scroll + MAX_DISPLAY_ITEMS) {
                    scroll = selected - MAX_DISPLAY_ITEMS + 1;
                    /* redraw list */
                    ct_screen_rect(0, 30, SCREEN_WIDTH, MAX_DISPLAY_ITEMS * (fnt->height + 2), create_rgb16(0,0,0));
                    for (int i = 0; i < MAX_DISPLAY_ITEMS && (scroll + i) < count; ++i) {
                        const char *name = names[scroll + i];
                        if (!name) name = "(null)";
                        render_text(10, 30 + i * (fnt->height + 2), (char *)name);
                    }
                }
                /* clear previous cursor if it was visible */
                int prev_screen_pos = old - scroll;
                if (prev_screen_pos >= 0 && prev_screen_pos < MAX_DISPLAY_ITEMS) {
                    ct_screen_rect(0, 30 + prev_screen_pos * (fnt->height + 2), fnt->width + 4, fnt->height + 2, create_rgb16(0,0,0));
                }
                /* draw new cursor */
                ct_print(0, 30 + (selected - scroll) * (fnt->height + 2), ">", create_rgb16(0,0,255));
                lcdc_copy_vram();
                prev_selected = selected;
            }
            while (get_key_state(KEY_DOWN)) keypad_read();
        }
    }
}
