
#include <graphics/drawing.h>
#include <graphics/color.h>
#include <graphics/lcdc.h>

uint16_t get_pixel(uint16_t x, uint16_t y)
{
	uint16_t lcd_width, lcd_height;
	uint16_t * vram = lcdc_get_vram_address();
	lcdc_get_dimensions(&lcd_width, &lcd_height);
	if (x >= lcd_width || y >= lcd_height)
		return 0;
	uint32_t index = (y * lcd_width) + x;
	return vram[index];
}

void set_pixel(uint16_t x, uint16_t y, uint16_t color)
{
	uint16_t lcd_width, lcd_height;
	uint16_t * vram = lcdc_get_vram_address();
	lcdc_get_dimensions(&lcd_width, &lcd_height);
	if (x >= lcd_width || y >= lcd_height)
		return;
	uint32_t index = (y * lcd_width) + x;
	vram[index] = color;
}

void draw_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
	uint16_t lcd_width, lcd_height;
	uint16_t * vram = lcdc_get_vram_address();
	lcdc_get_dimensions(&lcd_width, &lcd_height);
	uint32_t stopx, stopy;
	uint16_t color = get_pen();
	uint16_t x1, y1;
	uint32_t index;
	stopx = (width + x > lcd_width ? lcd_width : width + x);
	stopy = (height + y > lcd_height ? lcd_height : height + y);
	for (y1 = y; y1 < stopy; ++y1) {
		for(x1 = x; x1 < stopx; ++x1) {
			index = (y1 * lcd_width) + x1;
			vram[index] = color;
		}
	}
}


void invert_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
	uint16_t lcd_width, lcd_height;
	uint16_t * vram = lcdc_get_vram_address();
	lcdc_get_dimensions(&lcd_width, &lcd_height);
	uint32_t stopx, stopy;
	uint16_t x1, y1;
	uint32_t index;
	stopx = (width + x > lcd_width ? lcd_width : width + x);
	stopy = (height + y > lcd_height ? lcd_height : height + y);
	for (y1 = y; y1 < stopy; ++y1) {
		for(x1 = x; x1 < stopx; ++x1) {
			index = (y1 * lcd_width) + x1;
			vram[index] ^= 0xFFFF;
		}
	}
}

void draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	int32_t i, delta_x, delta_y, sdelta_x, sdelta_y, absdelta_x, absdelta_y;
	int32_t x, y, plot_x, plot_y;
	uint16_t color = get_pen();
	delta_x = x2 - x1;
	delta_y = y2 - y1;
	absdelta_x = (delta_x >= 0 ? delta_x : -delta_x);
	absdelta_y = (delta_y >= 0 ? delta_y : -delta_y);
	sdelta_x = (delta_x >= 0 ? 1 : -1);
	sdelta_y = (delta_y >= 0 ? 1 : -1);
	x = absdelta_y>>1;
  	y = absdelta_x>>1;
	plot_x = x1;
	plot_y = y1;

	if (absdelta_x >= absdelta_y) {
		for(i = 0; i < absdelta_x; i++) {
			y += absdelta_y;
			if (y >= absdelta_x) {
				y -= absdelta_x;
				plot_y += sdelta_y;
			}
			plot_x += sdelta_x;
			set_pixel(plot_x, plot_y, color);
		}
	} else {
		for(i = 0; i < absdelta_y; i++) {
			x += absdelta_x;
			if (x >= absdelta_y) {
				x -= absdelta_y;
				plot_x += sdelta_x;
			}
			plot_y += sdelta_y;
			set_pixel(plot_x, plot_y, color);
		}
	}
}

void draw_ellipse(uint16_t x, uint16_t y, uint16_t x_radius, uint16_t y_radius)
{
	uint16_t x1, y1;
	int32_t delta_x, delta_y;
	int32_t error;
	int32_t cva, cvb;
	int32_t stopx, stopy;
	cva = 2 * x_radius * x_radius;
	cvb = 2 * y_radius * y_radius;
	x1 = x_radius;
	y1 = 0;
	delta_x = y_radius * y_radius * (1 - 2 * x_radius);
	delta_y = x_radius * x_radius;
	error = 0;
	stopx = cvb * x_radius;
	stopy = 0;
	while (stopx >= stopy) {
		draw_line(x - x1, y + y1, x + x1, y + y1);
		draw_line(x - x1, y - y1, x + x1, y - y1);
/*
		set_pixel(x + x1, y + y1);
		set_pixel(x - x1, y + y1);
		set_pixel(x - x1, y - y1);
		set_pixel(x + x1, y - y1);
*/
		++y1;
		stopy += cva;
		error += delta_y;
		delta_y += cva;
		if ((2 * error + delta_x) > 0) {
			--x1;
			stopx -= cvb;
			error += delta_x;
			delta_x += cvb;
		}
	}
	x1 = 0;
	y1 = y_radius;
	delta_x = y_radius * y_radius;
	delta_y = x_radius * x_radius * (1 - 2 * y_radius);
	error = 0;
	stopx = 0;
	stopy = cva * y_radius;
	while (stopx <= stopy) {
		draw_line(x - x1, y + y1, x + x1, y + y1);
		draw_line(x - x1, y - y1, x + x1, y - y1);
/*
		set_pixel(x + x1, y + y1);
		set_pixel(x - x1, y + y1);
		set_pixel(x - x1, y - y1);
		set_pixel(x + x1, y - y1);
*/
		++x1;
		stopx += cvb;
		error += delta_x;
		delta_x += cvb;
		if ((2 * error + delta_y) > 0) {
			--y1;
			stopy -= cva;
			error += delta_y;
			delta_y += cva;
		}
	}
}

void draw_circle(uint16_t x, uint16_t y, uint16_t radius)
{
	draw_ellipse(x, y, radius, radius);
}

