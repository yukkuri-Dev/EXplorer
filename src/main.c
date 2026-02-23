#include <graphics/drawing.h>
#include <graphics/color.h>
#include <graphics/text.h>
#include <graphics/lcdc.h>
#include <sh4a/input/keypad.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <syscalls/syscalls.h>
#include <libct/print.h>
#include <libct/fsc/fs-control.h>
#include <libct/input.h>
#include <libct/ui/dialog/user_input_dialog.h>
#include <libct/ui/dialog/user_select_dialog.h>
#include <libct/ui/dialog/system_storage_info.h>
#define SCREEN_WIDTH 528
#define SCREEN_HEIGHT 320
#define MAX_DISPLAY 15  // 画面に表示する最大ファイル数
#define MAX_FILES 100   // 読み込む最大ファイル数


struct file_list_result current_files = {0};
int scroll_offset = 0;
int selected_index = 0;
int prev_selected_index = 0;
int refresh_needed = 1;
static char *drive[2] = {
    "\\\\drv0\\",  // 内蔵ドライブ
    "\\\\crd0\\"   // SDカード
};

// 選択されたファイル名を取得
char* get_selected_filename(void) {
    if (selected_index >= 0 && selected_index < current_files.count) {
        return current_files.entries[selected_index].name;
    }
    return NULL;
}

// 選択されたファイルのフルパスを作成して返す（呼び出し側で解放すること）
char* get_selected_fullpath(const char *cwd) {
    if (selected_index >= 0 && selected_index < current_files.count) {
        const char *name = current_files.entries[selected_index].name;
        size_t cwdlen = strlen(cwd);
        size_t namelen = strlen(name);
        /* allocate enough for cwd (without '*'), optional backslash, name, trailing '\\', '*' and null */
        char *buf = memmgr_alloc(cwdlen + namelen + 4);
        if (!buf) return NULL;
        strcpy(buf, cwd);
        char *star = strchr(buf, '*');
        if (star) *star = '\0';
        /* ensure path ends with backslash */
        size_t l = strlen(buf);
        if (l == 0 || buf[l-1] != '\\') {
            buf[l] = '\\';
            buf[l+1] = '\0';
        }
        strcat(buf, name);
        return buf;
    }
    return NULL;
}

// 選択されたファイルタイプを取得
unsigned long get_selected_filetype(void) {
    if (selected_index >= 0 && selected_index < current_files.count) {
        return current_files.entries[selected_index].type;
    }
    return 0;
}

// ファイルリストを表示するヘルパー関数
void display_file_list(int start_index, struct font *fnt, char *display_name) {
    ct_screen_rect(0, 30, SCREEN_WIDTH, MAX_DISPLAY * (fnt->height + 2), create_rgb16(0, 0, 0));
    
    for (int i = 0; i < MAX_DISPLAY && (start_index + i) < current_files.count; i++) {
        set_pen(create_rgb16(255, 255, 255));
        int idx = start_index + i;
        if (current_files.entries[idx].type == 5) {
            sprintf(display_name, "[DIR]  %s", current_files.entries[idx].name);
        } else if (current_files.entries[idx].type == 1) {
            sprintf(display_name, "[FILE] %s", current_files.entries[idx].name);
        } else {
            sprintf(display_name, "[?]    %s", current_files.entries[idx].name);
        }
        render_text(10, 30 + i * (fnt->height + 2), display_name);
    }
}

int main(void) {
    memmgr_init();
    size_t path_len = strlen(drive[0]) + strlen("*") + 1;
    
    char *path = memmgr_alloc(path_len);
    if (path == NULL) {
      set_pen(create_rgb16(255, 0, 0));  // 赤色
      render_text(10, 10, "Memory allocation failed!");
      ct_print(10, 30, "Press POWER to exit.", create_rgb16(255, 0, 0));
      lcdc_copy_vram();
      while (1) {
        keypad_read();
        if (get_key_state(KEY_POWER)) {
            memmgr_free(path);
            return -2;
        }
      }
    }
    struct font *fnt = get_font();
    int y_pos = 30;  // 描画開始Y座標
    char display_name[80];  // 表示用バッファ
    
    // 背景を黒で塗りつぶす
    ct_screen_clear(create_rgb16(0, 0, 0));
    sprintf(path, "%s%s", drive[0], "*");  // "\\\\drv0\\*"
    // タイトル
    ct_print(10, 10, "=== File List: ", create_rgb16(255, 255, 0));  // 黄色
    ct_print(10 + strlen("=== File List: ") * fnt->width, 10, path, create_rgb16(255, 255, 0));
    ct_print(10 + (strlen("=== File List: ") + strlen(path)) * fnt->width, 10, " ===", create_rgb16(255, 255, 0));

    
    // ファイルリストを取得
    current_files = get_file_list(path);
    
    if (current_files.success == 0 && current_files.count > 0) {
        // 最初のページを描画
        display_file_list(0, fnt, display_name);
        
        // ファイル数を表示
        sprintf(display_name, "Total: %d files", current_files.count);
        ct_print(10, 30 + MAX_DISPLAY * (fnt->height + 2) + 10, display_name, create_rgb16(0, 255, 255));
        
    } else {
        // ファイルが見つからなかった
        ct_print(10, y_pos, "No files found!", create_rgb16(255, 0, 0));
    }
    
    // 終了メッセージ
    ct_print((SCREEN_WIDTH - strlen("Press POWER to exit. BACK to root. H to Help.") * fnt->width) / 2,
     SCREEN_HEIGHT - fnt->height - 10,
     "Press POWER to exit. BACK to root. H to Help.",
     create_rgb16(0, 255, 0));
    
    // 初期カーソルを描画
    if (current_files.count > 0) {
        ct_print(0,30,">",create_rgb16(0,0,255));
    }
    
    // VRAMにコピー（画面に反映）
    lcdc_copy_vram();

    // 入力待機ループ
    while (1) {
        keypad_read();
        if (get_key_state(KEY_POWER)) {// プログラム終了
            memmgr_free(path);
            if (current_files.entries) memmgr_free(current_files.entries);
            return -2;
        }
        if (get_key_state(KEY_BACK)){// ルートディレクトリに戻る
          return 0;
        }
        if (get_key_state(KEY_ENTER)){
          // 選択されたファイル名を取得
          char *selpath = get_selected_fullpath(path);
          char *selected_file_ptr = get_selected_filename();
          char selected_file_copy[64] = {0};
          if (selected_file_ptr) {
              strncpy(selected_file_copy, selected_file_ptr, sizeof(selected_file_copy) - 1);
          }
          unsigned long selected_type = get_selected_filetype();
          if (selected_file_ptr != NULL && selected_type == 1) {
            char info_name[80];
            char info_size[80];
            const char *items[] = {
                info_name,
                info_size,
                "Copy to SD Card Root",
                "Delete File", 
                "view with Binary Viewer",
                "[CANCEL]",
            };
            sprintf(info_name, "[INFO] File name:%s", selected_file_copy);
            if (selpath) {
                int fd = sys_open(selpath, FILE_RD);
                if (fd >= 0) {
                    int fs = sys_get_filesize(fd);
                    if (fs >= 0) {
                        sprintf(info_size, "[INFO] File size: %lu bytes", (unsigned long)fs);
                    } else {
                        sprintf(info_size, "[INFO] File size: Unknown");
                    }
                    sys_close(fd);
                } else {
                    sprintf(info_size, "[INFO] File size: Unknown");
                }
                memmgr_free(selpath);
                selpath = NULL;
            } else {
                sprintf(info_size, "[INFO] File size: Unknown");
            }
             if (get_key_state(KEY_ENTER)){while(get_key_state(KEY_ENTER)){keypad_read();}}// キーリピート防止
            int sel = user_select_dialog(items, 6);
            if (sel ==0 || sel == 1|| sel == 3) {
                // 情報表示のみ
                if (sel != 3){
                    popup_dialog("Hummm? File information displayed.", create_rgb16(0, 255, 0));
                }
                refresh_needed = 1;
                lcdc_copy_vram();
                return 0;
            }
            if (sel == 4) {
                char *view_path = get_selected_fullpath(path);
                if (view_path) {
                    while (get_key_state(KEY_ENTER))
                    {
                        keypad_read();
                    }
                    Binary_viewer(view_path);
                    memmgr_free(view_path);
                }
                refresh_needed = 1;
                return 0;
            }
            if (sel == 2){
                char temp_path[128];
                sprintf(temp_path, "%s%s", drive[1], selected_file_copy);
                char *src_full = get_selected_fullpath(path);
                if (src_full) {
                    int copy_result = file_copy_dialog(src_full, temp_path);
                    if (copy_result == 0) {
                        popup_dialog("File copied successfully.", create_rgb16(0, 255, 0));
                        
                        /* Debug: Check if file actually exists on SD */
                        int test_fd = sys_open(temp_path, FILE_RD);
                        if (test_fd >= 0) {
                            int test_size = sys_get_filesize(test_fd);
                            sys_close(test_fd);
                            if (test_size > 0) {
                                char debug_msg[80];
                                sprintf(debug_msg, "OK: File exists %d bytes", test_size);
                                popup_dialog(debug_msg, create_rgb16(0, 255, 255));
                            }
                        } else {
                            popup_dialog("ERROR: File not on SD!", create_rgb16(255, 0, 0));
                        }
                    } else if (copy_result == -2) {
                        popup_dialog("Copy cancelled.", create_rgb16(255, 165, 0));
                    } else {
                        popup_dialog("Copy failed.", create_rgb16(255, 0, 0));
                    }
                    memmgr_free(src_full);
                    
                    /* Force filesystem cache update by touching the directory */
                    {
                        int cache_fd = sys_open("\\\\crd0\\", FILE_RD);
                        if (cache_fd >= 0) {
                            sys_close(cache_fd);
                        }
                    }
                    
                    /* Reload file list for SD card to show new file */
                    if (current_files.entries) {
                        memmgr_free(current_files.entries);
                        current_files.entries = NULL;
                    }
                    
                    /* Get updated file list from SD card */
                    char sd_search_path[64];
                    sprintf(sd_search_path, "%s*", drive[1]);
                    current_files = get_file_list(sd_search_path);
                    selected_index = 0;
                    prev_selected_index = 0;
                    scroll_offset = 0;
                    
                    /* Full screen redraw */
                    ct_screen_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, create_rgb16(0, 0, 0));
                    
                    set_pen(create_rgb16(255, 255, 0));
                    char title_buf[80];
                    sprintf(title_buf, "=== File List: %s ===", sd_search_path);
                    render_text(10, 10, title_buf);
                    
                    if (current_files.count > 0) {
                        display_file_list(0, fnt, display_name);
                        sprintf(display_name, "Total: %d files", current_files.count);
                        ct_print(10, 30 + MAX_DISPLAY * (fnt->height + 2) + 10, display_name, create_rgb16(0, 255, 255));
                    } else {
                        ct_print(10, 30, "No files on SD card", create_rgb16(255, 0, 0));
                    }
                    
                    lcdc_copy_vram();
                    return 0;
                } else {
                    popup_dialog("No source path available.", create_rgb16(255,0,0));
                }
                return 0;
            }

          }










          if (selected_file_ptr != NULL && selected_type == 5) {
              // ディレクトリの場合、そのディレクトリに移動
              char *selected_full = get_selected_fullpath(path);
              if (selected_full != NULL) {
                  /* new_search_path = selected_full + "\\*" */
                  size_t need = strlen(selected_full) + 3;
                  char *new_search_path = memmgr_alloc(need);
                  if (new_search_path != NULL) {
                      strcpy(new_search_path, selected_full);
                      strcat(new_search_path, "\\");
                      strcat(new_search_path, "*");

                      // 前のファイルリストを解放
                      if (current_files.entries) {
                          memmgr_free(current_files.entries);
                      }

                      // 新しいディレクトリのファイル一覧を取得
                      current_files = get_file_list(new_search_path);
                      selected_index = 0;
                      prev_selected_index = 0;
                      scroll_offset = 0;

                      // pathを更新: 再割り当てしてからコピーする
                      {
                          size_t needed = strlen(new_search_path) + 1;
                          char *new_path = (char *)memmgr_alloc(needed);
                          if (new_path != NULL) {
                              strcpy(new_path, new_search_path);
                              if (path != NULL) {
                                  memmgr_free(path);
                              }
                              path = new_path;
                          }
                      }
                      memmgr_free(new_search_path);

                      // 画面全体を再描画
                      set_pen(create_rgb16(0, 0, 0));
                      draw_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

                      // ヘッダーを表示
                      set_pen(create_rgb16(255, 255, 0));
                      sprintf(display_name, "=== File List: %s ===", path);
                      render_text(10, 10, display_name);

                      if (current_files.count > 0) {
                          // ファイル一覧を表示
                          display_file_list(0, fnt, display_name);

                          // ファイル数を表示
                          sprintf(display_name, "Total: %d files", current_files.count);
                      } else {
                          // ファイルが見つからなかった
                          ct_print(10,30,"No files found in this directory",create_rgb16(255,0,0));
                      }

                      lcdc_copy_vram();
                      refresh_needed = 1;
                  }
                  memmgr_free(selected_full);
              }
          }
          
          while (get_key_state(KEY_ENTER))
          {
              keypad_read();
          }
        }
        if (get_key_state(KEY_UP)){// 上キーで選択を移動
          if (selected_index > 0) {
              prev_selected_index = selected_index;
              selected_index--;
              
              // スクロールが必要か確認
              if (selected_index < scroll_offset) {
                  scroll_offset = selected_index;
                  // 画面全体を再描画
                  display_file_list(scroll_offset, fnt, display_name);
              }
              
              refresh_needed = 1;
              while (get_key_state(KEY_UP))
              {
                  keypad_read();
              }
          }
        }
        if (get_key_state(KEY_DOWN)){// 下キーで選択を移動
          if (selected_index < current_files.count - 1) {
              prev_selected_index = selected_index;
              selected_index++;
              
              // スクロールが必要か確認
              if (selected_index >= scroll_offset + MAX_DISPLAY) {
                  scroll_offset = selected_index - MAX_DISPLAY + 1;
                  // 画面全体を再描画
                  display_file_list(scroll_offset, fnt, display_name);
              }
              
              refresh_needed = 1;
              while (get_key_state(KEY_DOWN))
              {
                  keypad_read();
              }
              
          }
        }
        if (get_key_state(KEY_RIGHT)){// サブメニュー（ファイル/ディレクトリ作成）
            const char *items[] = {
                "Create File",
                "Create Directory",
                "About Storage Limits(MiB)",
                "About Storage Limits(KiB)",
                "About Storage Limits(bytes)",
                "Format SD Card"
            };
            int sel = user_select_dialog(items, 6);

            if(sel == 0 || sel == 1){// ファイルまたはディレクトリ作成
                int rc = -1;
                char *file_name = user_input_dialog();
                if (file_name == NULL) {
                    ct_print(10, SCREEN_HEIGHT - fnt->height - 40, "File creation cancelled.", create_rgb16(255,0,0));
                    while (get_key_state(KEY_RIGHT)){
                        keypad_read();
                    }
                    return 0;
                }
                if (file_name[0] == '\0') {
                    ct_print(10, SCREEN_HEIGHT - fnt->height - 40, "File name cannot be empty!", create_rgb16(255,0,0));
                    while (get_key_state(KEY_RIGHT)){
                        keypad_read();
                    }
                    return 0;
                }
                if (sel == 1) {
                    rc = directory_create(path, file_name);
                }else if (sel == 0) {
                    rc = file_create(path, file_name);
                }
                if (rc < 0){
                    ct_print(10, SCREEN_HEIGHT - fnt->height - 40, "File creation failed!", create_rgb16(255,0,0));
                } else {
                    refresh_needed = 1;
                    lcdc_copy_vram();
                    return 0;
                }

            }else if (sel == 2){
                system_storage_info_dialog("MiB");
                return 0;// ストレージ情報表示
            }else if (sel == 3){
                system_storage_info_dialog("KiB");
                return 0;// ストレージ情報表示
            }else if (sel == 4){
                system_storage_info_dialog("bytes");
                return 0;// ストレージ情報表示
            }else if (sel == 5){
                /* show konami warning (dialog returns 0 on correct sequence, 1 on BACK) */
                if (konami_command_warn_dialog() == 1) {
                    /* user cancelled at konami dialog */
                    refresh_needed = 1;
                    lcdc_copy_vram();
                    return 0;
                }

                popup_dialog("This will ERASE all data on the SD card.", create_rgb16(255, 255, 0));
                int yn = yes_or_no_dialog("Format SD card?", create_rgb16(255, 0, 0));
                if (yn == 1) {
                    /* user selected No */
                    refresh_needed = 1;
                    lcdc_copy_vram();
                    return 0;
                }

                int fmt_result = sys_sdformat();
                if (fmt_result == 0) {
                    popup_dialog("SD card formatted.", create_rgb16(0, 255, 0));
                } else {
                    popup_dialog("Failed to format SD card.", create_rgb16(255, 0, 0));
                }
                refresh_needed = 1;
                lcdc_copy_vram();
                return 0;
            }
            return 0;
        }
        if (get_key_state(KEY_BACKSPACE)){// 選択されたファイルを削除
            char *selected_full = get_selected_fullpath(path);
            if (selected_full == NULL) {
                popup_dialog("No file selected!", create_rgb16(255, 0, 0));
                refresh_needed = 1;
                lcdc_copy_vram();
                return 0;
            }

            /* show konami warning (dialog returns 0 on correct sequence, 1 on BACK) */
            if (konami_command_warn_dialog() == 1) {
                // ユーザーがキャンセル
                memmgr_free(selected_full);
                refresh_needed = 1;
                lcdc_copy_vram();
                return 0;
            }

            // 確認ダイアログを表示（フルパスを見せる）
            popup_dialog(selected_full, create_rgb16(255, 255, 0));
            int yn = yes_or_no_dialog("Delete file?", create_rgb16(255, 0, 0));
            if (yn == 1) {
                // ユーザーがNoを選択
                memmgr_free(selected_full);
                refresh_needed = 1;
                lcdc_copy_vram();
                return 0;
            }
            int delete_result = sys_delete(selected_full);
            if (delete_result == 0) {
                popup_dialog("File deleted.", create_rgb16(0, 255, 0));
            } else {
                popup_dialog("Failed to delete file.", create_rgb16(255, 0, 0));
            }
            memmgr_free(selected_full);
            refresh_needed = 1;
            lcdc_copy_vram();
            if (current_files.entries != NULL) {
                memmgr_free(current_files.entries);
                current_files.entries = NULL;
            }
            if (path != NULL) {
                memmgr_free(path);
                path = NULL;
            }
            return 0;
        }
        if (get_key_state(KEY_LEFT)){// ドライブ切り替え（SDカード）
          /* Ensure 'path' buffer is large enough for drive[1] and '*' */
          {
              size_t needed = strlen(drive[1]) + strlen("*") + 1; /* drive + '*' + '\0' */
              char *new_path = (char *)memmgr_alloc(needed);
              if (!new_path) {
                  /* Allocation failed; abort drive switch to avoid overflow */
                  while (get_key_state(KEY_LEFT))
                  {
                      keypad_read();
                  }
                  continue;
              }
              if (path != NULL) {
                  memmgr_free(path);
              }
              path = new_path;
          }
          strcpy(path, drive[1]);
          strcat(path, "*");
          
          // 前のファイルリストを解放
          if (current_files.entries) {
              memmgr_free(current_files.entries);
          }
          
          // 新しいドライブのファイル一覧を取得
          current_files = get_file_list(path);
          selected_index = 0;
          prev_selected_index = 0;
          scroll_offset = 0;
          
          // 画面全体を再描画
          set_pen(create_rgb16(0, 0, 0));
          draw_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
          
          // ヘッダーを表示
          set_pen(create_rgb16(255, 255, 0));
          sprintf(display_name, "=== File List: %s ===", path);
          render_text(10, 10, display_name);
          
          if (current_files.count > 0) {
              // ファイル一覧を表示
              display_file_list(0, fnt, display_name);
              
              // ファイル数を表示
              sprintf(display_name, "Total: %d files", current_files.count);
              ct_print(10, 30 + MAX_DISPLAY * (fnt->height + 2) + 10, display_name, create_rgb16(0, 255, 255));
          } else {
              // ファイルが見つからなかった
              ct_print(10,30,"No files found!",create_rgb16(255,0,0));
          }
          
          lcdc_copy_vram();
          refresh_needed = 1;
          
          while (get_key_state(KEY_LEFT))
          {
              keypad_read();
          }
        }
        if (get_key_state(KEY_CHAR_H)){//ヘルプ表示
            const char *items[] = {
                "=== Help ===",
                "[H] Show Help",
                "[UP/DOWN] Move Cursor",
                "[ENTER] Select File/Enter Directory",
                "[BACK] Go to Root Directory",
                "[LEFT] Switch to SD Card",
                "[RIGHT] Show Submenu (Create File/Directory and ...)",
                "[BACKSPACE] Delete Selected File/Directory",
                "[POWER] Exit Application",
            };
            info_list(items, 9);
          refresh_needed = 1;
          lcdc_copy_vram();
          while (get_key_state(KEY_CHAR_H))
          {
              keypad_read();
          }
          return 0;
        }
        if (get_key_state(KEY_CHAR_C)){// Copy関数の作成用（未実装）
            char src_path[64] ;
            char dest_path[64] ;
            sprintf(src_path, "%s%s", drive[0], "C168_NOR.BIN");  // "\\\\sd0\\TEST"
            sprintf(dest_path, "%s%s", drive[1], "C168_NOR.BIN");  // "\\\\sd0\\TEST2"
            file_copy(src_path,dest_path);
            refresh_needed = 1;
            lcdc_copy_vram();
            while (get_key_state(KEY_CHAR_C))
            {
                keypad_read();
            }
        }
        if (refresh_needed) {// 画面を更新
            // 前のカーソルを消去（黒で上書き）
            int prev_screen_pos = prev_selected_index - scroll_offset;
            if (prev_screen_pos >= 0 && prev_screen_pos < MAX_DISPLAY) {
                ct_screen_rect(0, 30 + prev_screen_pos * (fnt->height + 2), 10, fnt->height + 2, create_rgb16(0, 0, 0));
                render_text(0, 30 + prev_screen_pos * (fnt->height + 2), ">");
            }
            
            // 新しいカーソルを描画
            int screen_pos = selected_index - scroll_offset;
            if (screen_pos >= 0 && screen_pos < MAX_DISPLAY) {
                set_pen(create_rgb16(0, 0, 255));  // 青色
                render_text(0, 30 + screen_pos * (fnt->height + 2), ">");  // 選択インジケータ
            }
            
            // デバッグ情報を表示
            ct_screen_rect(400, 10, 120, 20, create_rgb16(0, 0, 0));  // 前の情報を消去
            set_pen(create_rgb16(255, 255, 255));
            sprintf(display_name, "Sel: %d/%d", selected_index + 1, current_files.count);
            render_text(400, 10, display_name);
            
            lcdc_copy_vram();
            refresh_needed = 0;
        }
    }
}
