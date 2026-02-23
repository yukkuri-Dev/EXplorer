#include <libct/print.h>
#include <libct/input.h>
#include <graphics/drawing.h>
#include <graphics/color.h>
#include <graphics/text.h>
#include <graphics/lcdc.h>
#include <sh4a/input/keypad.h>
#include <string.h>
#include <stdio.h>
#include <libct/fonts/jpn-font.h>
#include <stdint.h>
#define SCREEN_WIDTH 528
#define SCREEN_HEIGHT 320
#define MAX_DISPLAY_ITEMS 12

static const char *key_name(int kc) {
    switch (kc) {
        case KEY_UP: return "UP";
        case KEY_DOWN: return "DOWN";
        case KEY_LEFT: return "LEFT";
        case KEY_RIGHT: return "RIGHT";
        case KEY_ENTER: return "ENTER";
        case KEY_BACK: return "BACK";
        case KEY_POWER: return "POWER";
        case KEY_BACKSPACE: return "BACKSPACE";
        default: return "KEY";
    }
}

char *user_input_dialog(void){
    ct_screen_clear(create_rgb16(0,0,0));
    struct font *fnt = get_font();

    ct_print(10,10,"Please enter a string",create_rgb16(255,255,255));
    ct_print(0,30,">",create_rgb16(0,0,255));
    ct_print((SCREEN_WIDTH - strlen("Press POWER to Enter Press BACK to Cancel.") * fnt->width) / 2,
        SCREEN_HEIGHT - fnt->height - 10,
        "Press POWER to Enter Press BACK to Cancel.",
        create_rgb16(0, 255, 0));
    lcdc_copy_vram();

    static char buf[256];
    static int idx = 0;
    buf[0] = '\0';
    idx = 0;

    while (1) {
        keypad_read();
        int keycode = ct_get_current_keycode();
        if (keycode == -1) continue;

        /* Handle control keys first */
        if (get_key_state(KEY_ENTER)) {
            /* Return current buffer (may be empty) */
            while(get_key_state(KEY_ENTER)) { keypad_read(); }
            return buf;
        }
        if (keycode == KEY_BACK) {
            return NULL;
        }

        /* Handle backspace key */
        if (keycode == KEY_BACKSPACE) {
            if (idx > 0) buf[--idx] = '\0';
            /* Clear input area then draw */
            ct_screen_rect(10 + fnt->width, 30, SCREEN_WIDTH - (10 + fnt->width), fnt->height, create_rgb16(0,0,0));
            ct_print(10 + fnt->width, 30, buf, create_rgb16(255,255,255));
            lcdc_copy_vram();
            /* Wait until key is released */
            while (get_key_state(keycode)) { keypad_read(); }
            continue;
        }

        /* Translate keycode to text (if available) */
        char now_input = keycode_to_text(keycode);
        if (!now_input) {
            /* Unmapped key - ignore */
            while (get_key_state(keycode)) { keypad_read(); }
            continue;
        }

        /* Backspace handling */
        if (now_input == '\b' || now_input == 127) {
            if (idx > 0) buf[--idx] = '\0';
            /* Clear input area then draw */
            ct_screen_rect(10 + fnt->width, 30, SCREEN_WIDTH - (10 + fnt->width), fnt->height, create_rgb16(0,0,0));
            ct_print(10 + fnt->width, 30, buf, create_rgb16(255,255,255));
            lcdc_copy_vram();
            /* Wait until key is released */
            while (get_key_state(keycode)) { keypad_read(); }
            continue;
        } else {
            if (idx < (int)sizeof(buf) - 1) {
                buf[idx++] = now_input;
                buf[idx] = '\0';
            }
        }

        /* Draw input */
        ct_print(10 + fnt->width, 30, buf, create_rgb16(255,255,255));
        lcdc_copy_vram();

        /* Wait until key is released */
        while (get_key_state(keycode)) {
            keypad_read();
        }
    }
}
int yes_or_no_dialog(const char *message,uint16_t color){
    ct_screen_clear(create_rgb16(0,0,0));
    struct font *fnt = get_font();
    ct_print(10,SCREEN_HEIGHT / 2,message,color);
    ct_print((SCREEN_WIDTH - strlen("Press ENTER for Yes, BACK for No.") * fnt->width) / 2,
        SCREEN_HEIGHT - fnt->height - 10,
        "Press ENTER for Yes, BACK for No.",
        create_rgb16(0, 255, 0));
    lcdc_copy_vram();

    while (1) {
        keypad_read();
        if (get_key_state(KEY_ENTER)) {
            while(get_key_state(KEY_ENTER)) { keypad_read(); }
            return 0; // Yes
        }
        if (get_key_state(KEY_BACK)) {
            while(get_key_state(KEY_BACK)) { keypad_read(); }
            return 1; //No
        }
    }
}
void popup_dialog(const char *message,uint16_t color){
        ct_screen_clear(create_rgb16(0,0,0));
        struct font *fnt = get_font();
        ct_print(10,SCREEN_HEIGHT / 2,message,color);
        ct_print((SCREEN_WIDTH - strlen("Press ENTER to Continue.") * fnt->width) / 2,
            SCREEN_HEIGHT - fnt->height - 10,
            "Press ENTER to Continue.",
            create_rgb16(0, 255, 0));
        lcdc_copy_vram();

        while (1) {
            keypad_read();
            if (get_key_state(KEY_ENTER)) {
                while(get_key_state(KEY_ENTER)) { keypad_read(); }
                return;
            }
    }
}



void info_list(const char **names, int count){
    if (!names || count <= 0) return;

    ct_screen_clear(create_rgb16(0,0,0));
    struct font *fnt = get_font();

    ct_print(10, 10, "=== Info List ===", create_rgb16(255, 255, 0));
    ct_print((SCREEN_WIDTH - strlen("Press ENTER and BACK to Cancel.") * fnt->width) / 2,
             SCREEN_HEIGHT - fnt->height - 10,
             "Press ENTER and BACK to Cancel.",
             create_rgb16(0, 255, 0));
    lcdc_copy_vram();

    int selected = 0;
    int scroll = 0;

    /* initial draw */
    for (int i = 0; i < MAX_DISPLAY_ITEMS && i < count; ++i) {
        const char *name = names[i];
        if (!name) name = "(null)";
        render_text(10, 30 + i * (fnt->height + 2), (char *)name);
    }
    /* draw cursor */
    lcdc_copy_vram();
    while (1) {
        keypad_read();
        if (get_key_state(KEY_ENTER)) {
            while (get_key_state(KEY_ENTER)) keypad_read();
            return ;
        }
        if (get_key_state(KEY_BACK)) {
            while (get_key_state(KEY_BACK)) keypad_read();
            return ;
        }
    }
}




int konami_command_warn_dialog(void){
    ct_screen_clear(create_rgb16(0, 0, 0));
    struct font *fnt = get_font();

    ct_print(SCREEN_WIDTH / 2 - (strlen("WAIT! STOP! WARNING!") * fnt->width) / 2,
             SCREEN_HEIGHT / 2 - fnt->height,
             "WAIT! STOP! WARNING!",
             create_rgb16(255, 0, 0));

    ct_print(SCREEN_WIDTH / 2 - (strlen("You're attempting an operation that may cause irreversible changes.") * fnt->width) / 2,
             SCREEN_HEIGHT / 2,
             "You're attempting an operation that may cause irreversible changes.",
             create_rgb16(255, 0, 0));

    ct_print(SCREEN_WIDTH / 2 - (strlen("Proceed only if you understand the risks involved.") * fnt->width) / 2,
             SCREEN_HEIGHT / 2 + fnt->height,
             "Proceed only if you understand the risks involved.",
             create_rgb16(255, 0, 0));

    lcdc_copy_vram();

    /* Require the sequence: UP, UP, DOWN, DOWN, LEFT, RIGHT, LEFT, RIGHT, ENTER */
    static const int seq[] = {KEY_UP, KEY_UP, KEY_DOWN, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_LEFT, KEY_RIGHT, KEY_ENTER};
    int pos = 0;
    const int seq_len = sizeof(seq) / sizeof(seq[0]);

    while (1) {
        keypad_read();
        int kc = ct_get_current_keycode();
        if (kc == -1) continue;

        /* BACK immediately cancels and returns 1 */
        if (kc == KEY_BACK) {
            /* wait for release */
            while (get_key_state(KEY_BACK)) { keypad_read(); }
            return 1;
        }

        if (kc == seq[pos]) {
            pos++;
            if (pos == seq_len) {
                /* matched full sequence */
                /* wait for final key release */
                while (get_key_state(seq[seq_len - 1])) { keypad_read(); }
                return 0;
            }
        } else {
            /* If the key equals the first element, start at 1, else reset */
            if (kc == seq[0]) pos = 1; else pos = 0;
        }

        /* Display current progress */
        uint16_t color = create_rgb16(128, 128, 128); /* Gray default */
        if (pos > 0) {
            /* Gradient from gray to yellow based on progress */
            int intensity = (pos * 255) / seq_len;
            color = create_rgb16(255, 255, (255 - intensity) / 2);
        }
        
        /* Build and display full sequence, highlighting current position */
        char progress_text[128];
        int len = 0;
        for (int i = 0; i < seq_len && len < (int)sizeof(progress_text) - 1; ++i) {
            const char *name = key_name(seq[i]);
            if (i > 0 && len < (int)sizeof(progress_text) - 1) {
                progress_text[len++] = ' ';
            }
            /* Use fixed-size temporary buffer + sprintf, then copy safely to avoid overflow */
            char tmp[16];
            if (i < pos) {
                sprintf(tmp, "%s", name);
            } else if (i == pos) {
                sprintf(tmp, "[%s]", name);
            } else {
                sprintf(tmp, "%s", name);
            }
            int tn = strlen(tmp);
            int can_copy = (int)sizeof(progress_text) - len - 1;
            if (tn > can_copy) tn = can_copy;
            if (tn > 0) {
                memcpy(progress_text + len, tmp, tn);
                len += tn;
            } else {
                break; /* no more room */
            }
        }
        progress_text[len] = '\0';
        ct_screen_rect(10, SCREEN_HEIGHT - fnt->height - 30, SCREEN_WIDTH - 20, fnt->height, create_rgb16(0, 0, 0));
        ct_print(10, SCREEN_HEIGHT - fnt->height - 30, progress_text, color);
        lcdc_copy_vram();

        /* wait until this key is released to avoid duplicate reads */
        if (kc >= 0) {
            while (get_key_state(kc)) { keypad_read(); }
        }
    }
}