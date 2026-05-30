#ifndef _LCDC_H
#define _LCDC_H

#include <stdint.h>

void lcdc_get_dimensions(uint16_t *width, uint16_t *height);
void lcdc_set_vram_address(void* address);
void * lcdc_get_vram_address(void);

uint16_t lcdc_read_word(void);
void lcdc_write_word(uint16_t value);
void lcdc_write_cmd(uint16_t cmd);
void lcdc_copy_vram(void);
void lcdc_set_window(uint16_t x, uint16_t y, uint16_t width, uint16_t height);


#endif //_LCDC_H
