#ifndef _DRAWING_H
#define _DRAWING_H

#include <stdint.h>

uint16_t get_pixel(uint16_t x, uint16_t y);
void set_pixel(uint16_t x, uint16_t y, uint16_t color);
void draw_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height);
void invert_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height);
void draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void draw_circle(uint16_t x, uint16_t y, uint16_t radius);
void draw_ellipse(uint16_t x, uint16_t y, uint16_t x_radius, uint16_t y_radius);

#endif //_DRAWING_H
