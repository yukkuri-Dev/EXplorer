#ifndef _TEXT_H
#define _TEXT_H

#include <stdint.h>

struct font {
	uint8_t width; // should always be a multiple of eight
	uint8_t height;
	const uint8_t *widths;
	const uint8_t *data;
};

void draw_glyph(uint16_t x, uint16_t y, uint8_t width, uint8_t height, uint8_t scanline, const unsigned char * buffer);
struct font * get_font();
struct font * set_font(struct font *fnt);
void render_text(uint16_t x, uint16_t y, char* text);
#endif //_TEXT_H
