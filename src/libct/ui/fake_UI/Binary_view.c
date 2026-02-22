#include <libct/print.h>
#include <libct/input.h>
#include <graphics/drawing.h>
#include <graphics/color.h>
#include <graphics/text.h>
#include <graphics/lcdc.h>
#include <sh4a/input/keypad.h>
#include <string.h>
#include <stdio.h>
#include <syscalls/syscalls.h>

#define SCREEN_WIDTH 528
#define SCREEN_HEIGHT 320
#define BYTES_PER_ROW 16
#define MAX_DISPLAY_ROWS 14
#define MAX_ASCII_ROWS 5  // Limit ASCII display to 5 rows
#define BUFFER_SIZE (BYTES_PER_ROW * MAX_DISPLAY_ROWS)

static void display_hex_byte(char *buf, unsigned char byte) {
    const char *hex = "0123456789ABCDEF";
    buf[0] = hex[(byte >> 4) & 0x0F];
    buf[1] = hex[byte & 0x0F];
    buf[2] = '\0';
}

static int is_printable(unsigned char c) {
    return (c >= 0x20 && c < 0x7F);
}

static void redraw_screen(const char *filepath, unsigned char *buffer, int buffer_len, 
                          unsigned int file_offset, int file_size, struct font *fnt,
                          int y_start, int cursor_row, int cursor_col, int fd) {
    ct_screen_clear(create_rgb16(0, 0, 0));
    
    // Header
    ct_print(5, 0, "OFFSET   00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F", 
             create_rgb16(0, 255, 0));
    
    // File info
    char info[100];
    sprintf(info, "File: %s  Size: %d bytes", filepath, file_size);
    ct_print(5, fnt->height + 2, info, create_rgb16(100, 200, 255));
    
    // ASCII area - fixed at a position that won't be cut off
    int ascii_y_start = SCREEN_HEIGHT - fnt->height * 7;  // 5 rows + gap + footer
    
    // Calculate absolute file offset of current cursor
    unsigned int absolute_cursor_offset = file_offset + cursor_row * BYTES_PER_ROW;
    
    // Hex dump - centered on cursor with +/- rows
    int hex_center_line = 5;  // Middle line in hex display (more rows than ASCII)
    int hex_start_row = cursor_row - hex_center_line;
    int hex_start_offset = file_offset + hex_start_row * BYTES_PER_ROW;
    
    // Clamp start offset to file boundaries
    if (hex_start_offset < 0) {
        hex_start_offset = 0;
    }
    if (hex_start_offset >= file_size) {
        hex_start_offset = file_size - BYTES_PER_ROW;
        if (hex_start_offset < 0) hex_start_offset = 0;
    }
    
    // Read hex data from file
    unsigned char hex_buffer[BYTES_PER_ROW * 11];  // 11 rows (5 before, cursor, 5 after)
    sys_seek(fd, hex_start_offset, 0);
    int hex_bytes_read = sys_read(fd, hex_buffer, sizeof(hex_buffer));
    
    // Hex dump content - centered on cursor
    int y = y_start;
    int hex_display_rows = 0;
    int max_hex_rows = 11;  // Display up to 11 rows
    
    for (int i = 0; i < max_hex_rows && y < ascii_y_start - fnt->height; i++) {
        int buffer_offset = i * BYTES_PER_ROW;
        unsigned int row_file_offset = hex_start_offset + buffer_offset;
        
        // Check if this row is within the file
        if (row_file_offset < file_size && buffer_offset < hex_bytes_read) {
            char line[100];
            char *p = line;
            
            // Offset
            p += sprintf(p, "%08X ", row_file_offset);
            
            // Hex bytes
            for (int col = 0; col < BYTES_PER_ROW; col++) {
                unsigned char byte = hex_buffer[buffer_offset + col];
                p += sprintf(p, "%02X ", byte);
            }
            *p = '\0';
            
            ct_print(5, y, line, create_rgb16(0, 255, 0));
            
            // Draw inverted cursor at the center line when this is the cursor row
            if (row_file_offset / BYTES_PER_ROW == absolute_cursor_offset / BYTES_PER_ROW) {
                int hex_x = 5 + (9 + cursor_col * 3) * fnt->width;
                invert_rect(hex_x, y, fnt->width * 2, fnt->height);
            }
            
            y += fnt->height;
            hex_display_rows++;
        }
    }
    
    // ASCII representation area - absolute file position based on cursor
    // Cursor in ASCII is always at line 2 (middle of 5 lines)
    int ascii_center_line = 2;  // Middle line (0-4)
    int ascii_start_row = cursor_row - ascii_center_line;
    int ascii_start_offset = file_offset + ascii_start_row * BYTES_PER_ROW;
    
    // Clamp start offset to file boundaries
    if (ascii_start_offset < 0) {
        ascii_start_offset = 0;
    }
    
    // Temporary buffer for ASCII display
    unsigned char ascii_buffer[BYTES_PER_ROW * MAX_ASCII_ROWS];
    
    // Read ASCII data from file
    sys_seek(fd, ascii_start_offset, 0);
    int ascii_bytes_read = sys_read(fd, ascii_buffer, BYTES_PER_ROW * MAX_ASCII_ROWS);
    
    // Display ASCII rows
    for (int i = 0; i < MAX_ASCII_ROWS; i++) {
        int buffer_offset = i * BYTES_PER_ROW;
        
        // Check if this row is within the file
        if (ascii_start_offset + buffer_offset < file_size && buffer_offset < ascii_bytes_read) {
            char ascii_line[20];
            char *p = ascii_line;
            
            // ASCII representation  
            for (int col = 0; col < BYTES_PER_ROW; col++) {
                unsigned char byte = ascii_buffer[buffer_offset + col];
                *p++ = is_printable(byte) ? byte : '.';
            }
            *p = '\0';
            
            int ascii_y = ascii_y_start + i * fnt->height;
            ct_print(5, ascii_y, ascii_line, create_rgb16(0, 200, 255));
            
            // Draw inverted cursor at the center line (line 2) when this is the current row
            if (i == ascii_center_line) {
                int ascii_x = 5 + cursor_col * fnt->width;
                invert_rect(ascii_x, ascii_y, fnt->width, fnt->height);
            }
        }
    }
    
    // Footer
    char footer[100];
    int pages = (file_size + BUFFER_SIZE - 1) / BUFFER_SIZE;
    int current_page = (file_offset / BUFFER_SIZE) + 1;
    sprintf(footer, "Page %d/%d  [UP/DOWN/LEFT/RIGHT] Move  [BACK] Exit", current_page, pages);
    ct_print(5, SCREEN_HEIGHT - fnt->height - 5, footer, create_rgb16(100, 255, 100));
    
    lcdc_copy_vram();
}

int Binary_viewer(const char *filepath) {
    struct font *fnt = get_font();
    
    // Open file
    int fd = sys_open(filepath, FILE_RD);
    if (fd < 0) {
        ct_screen_clear(create_rgb16(0, 0, 0));
        ct_print(10, 30, "Failed to open file!", create_rgb16(255, 0, 0));
        lcdc_copy_vram();
        return -1;
    }
    
    // Get file size
    int file_size = sys_get_filesize(fd);
    if (file_size <= 0) {
        ct_screen_clear(create_rgb16(0, 0, 0));
        ct_print(10, 30, "Failed to get file size!", create_rgb16(255, 0, 0));
        lcdc_copy_vram();
        sys_close(fd);
        return -1;
    }
    
    // Allocate buffer
    unsigned char *buffer = (unsigned char *)memmgr_alloc(BUFFER_SIZE);
    if (!buffer) {
        ct_screen_clear(create_rgb16(0, 0, 0));
        ct_print(10, 30, "Memory allocation failed!", create_rgb16(255, 0, 0));
        lcdc_copy_vram();
        sys_close(fd);
        return -1;
    }
    
    unsigned int file_offset = 0;
    unsigned int prev_file_offset = -1;  // Force initial load
    int y_start = fnt->height * 3;
    int cursor_row = 0;
    int cursor_col = 0;
    int prev_key_up = 0, prev_key_down = 0, prev_key_left = 0, prev_key_right = 0;
    int bytes_read = 0;
    int need_redraw = 1;  // Force initial draw
    
    // Initial read
    sys_seek(fd, 0, 0);
    bytes_read = sys_read(fd, buffer, BUFFER_SIZE);
    if (bytes_read <= 0) {
        ct_screen_clear(create_rgb16(0, 0, 0));
        ct_print(10, 30, "Failed to read file!", create_rgb16(255, 0, 0));
        lcdc_copy_vram();
        memmgr_free(buffer);
        sys_close(fd);
        return -1;
    }
    
    // Main loop - optimized for responsive input
    while (1) {
        // Key input - do this first for responsiveness
        keypad_read();
        
        if (get_key_state(KEY_BACK)) {
            break;
        }
        
        // Cursor movement - detect key press edges (not just state)
        int max_rows = (bytes_read + BYTES_PER_ROW - 1) / BYTES_PER_ROW;
        int cur_key_up = get_key_state(KEY_UP);
        int cur_key_down = get_key_state(KEY_DOWN);
        int cur_key_left = get_key_state(KEY_LEFT);
        int cur_key_right = get_key_state(KEY_RIGHT);
        
        // Move UP (key newly pressed)
        if (cur_key_up && !prev_key_up) {
            if (cursor_row > 0) {
                cursor_row--;
            } else if (file_offset >= BUFFER_SIZE) {
                file_offset -= BUFFER_SIZE;
                cursor_row = MAX_DISPLAY_ROWS - 1;
            }
            need_redraw = 1;
        }
        // Move DOWN
        else if (cur_key_down && !prev_key_down) {
            if (cursor_row < max_rows - 1) {
                cursor_row++;
            } else if (file_offset + BUFFER_SIZE < file_size) {
                file_offset += BUFFER_SIZE;
                cursor_row = 0;
            }
            need_redraw = 1;
        }
        // Move LEFT
        else if (cur_key_left && !prev_key_left) {
            if (cursor_col > 0) {
                cursor_col--;
            } else if (cursor_row > 0) {
                cursor_row--;
                cursor_col = BYTES_PER_ROW - 1;
            } else if (file_offset > 0) {
                file_offset -= BUFFER_SIZE;
                cursor_row = MAX_DISPLAY_ROWS - 1;
                cursor_col = BYTES_PER_ROW - 1;
            }
            need_redraw = 1;
        }
        // Move RIGHT
        else if (cur_key_right && !prev_key_right) {
            if (cursor_col < BYTES_PER_ROW - 1) {
                cursor_col++;
            } else if (cursor_row < max_rows - 1) {
                cursor_row++;
                cursor_col = 0;
            } else if (file_offset + BUFFER_SIZE < file_size) {
                file_offset += BUFFER_SIZE;
                cursor_row = 0;
                cursor_col = 0;
            }
            need_redraw = 1;
        }
        
        // Save current key state for next frame
        prev_key_up = cur_key_up;
        prev_key_down = cur_key_down;
        prev_key_left = cur_key_left;
        prev_key_right = cur_key_right;
        
        // File read only if offset changed
        if (file_offset != prev_file_offset) {
            sys_seek(fd, file_offset, 0);
            bytes_read = sys_read(fd, buffer, BUFFER_SIZE);
            
            if (bytes_read <= 0) {
                // Reset to beginning if at end
                file_offset = 0;
                cursor_row = 0;
                cursor_col = 0;
                sys_seek(fd, 0, 0);
                bytes_read = sys_read(fd, buffer, BUFFER_SIZE);
            }
            prev_file_offset = file_offset;
            need_redraw = 1;
        }
        
        // Redraw only when needed
        if (need_redraw) {
            redraw_screen(filepath, buffer, bytes_read, file_offset, file_size, fnt, 
                         y_start, cursor_row, cursor_col, fd);
            need_redraw = 0;
        }
    }
    
    // Cleanup
    memmgr_free(buffer);
    sys_close(fd);
    
    return 0;
}