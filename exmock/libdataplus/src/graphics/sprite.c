
#include <graphics/sprite.h>
#include <graphics/drawing.h>

#ifndef MAX
#define MAX(a,b)    (((a) > (b)) ? (a) : (b))
#define MIN(a,b)    (((a) < (b)) ? (a) : (b))
#endif

int8_t scale2x(struct sprite *dst, struct sprite *src)
{
	uint16_t x, y;

	uint16_t src_pitch = src->width;
	uint16_t dst_pitch = dst->width;
	uint16_t E0, E1, E2, E3, B, D, E, F, H;

	if (dst_pitch * dst->height != src_pitch * src->height * 4)
		return -1;
	for(y = 0; y < src->height; ++y) {
		for(x = 0; x < src->width; ++x) {
			    	B = *(src->data + (MAX(0, y - 1) * src_pitch) + (x));
			    	D = *(src->data + (y * src_pitch) + (MAX(0, x - 1)));
			    	E = *(src->data + (y * src_pitch) + (x));
			    	F = *(src->data + (y * src_pitch) + (MIN(src->width - 1, x + 1)));
			    	H = *(src->data + (MIN(src->height - 1, y + 1) * src_pitch) + (x));
				
				if (B != H && D != F) {
					E0 = D == B ? D : E;
					E1 = B == F ? F : E;
					E2 = D == H ? D : E;
					E3 = H == F ? F : E;
				} else {
					E0 = E;
					E1 = E;
					E2 = E;
					E3 = E;
				}

				*(dst->data + y * 2 * dst_pitch + x * 2) = E0;
				*(dst->data + y * 2 * dst_pitch + (x * 2 + 1)) = E1;
				*(dst->data + (y * 2 + 1) * dst_pitch + x * 2) = E2;
				*(dst->data + (y * 2 + 1) * dst_pitch + (x * 2 + 1)) = E3;
		}
	}
	return 0;
}

void draw_sprite(struct sprite * sprite, uint16_t x, uint16_t y)
{
	uint16_t x1, y1;
	for (y1 = 0; y1 < sprite->height; ++y1) {
		for (x1 = 0; x1 < sprite->width; ++x1) {
			uint16_t color;
			uint16_t index = (y1 * sprite->width) + x1;
			color = sprite->data[index];
			if(color != sprite->mask) {
				set_pixel(x + x1, y + y1, color);
			}
		}
	}
}

