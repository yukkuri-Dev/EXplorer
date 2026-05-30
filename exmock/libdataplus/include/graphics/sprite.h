#ifndef _SPRITE_H
#define _SPRITE_H

#include <stdint.h>

struct sprite {
	uint16_t width;
	uint16_t height;
	uint16_t mask;
	uint16_t *data;
};

void draw_sprite(struct sprite *sprite, uint16_t x, uint16_t y);
int8_t scale2x(struct sprite *dst, struct sprite *src);

#endif //_SPRITE_H
