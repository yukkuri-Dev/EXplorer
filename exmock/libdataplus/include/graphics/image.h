#ifndef _IMAGE_H
#define _IMAGE_H

struct image {
	uint32_t width;
	uint32_t height;
	uint32_t pitch;
	uint8_t  bpp;
}

struct image_ops {
	open_fn open;
	scanline_fn get_next_scanline;
	close_fn close;
	void * priv;
}

render_image(const char *filename, uint16_t x, uint16_t y, uint16_t width, uint16_t height);

#endif //_IMAGE_H
