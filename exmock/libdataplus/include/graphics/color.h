#ifndef _COLOR_H
#define _COLOR_H

#include <stdint.h>

uint16_t create_rgb16(uint8_t red, uint8_t green, uint8_t blue);
void set_pen(uint16_t color);
uint16_t get_pen();

#endif //_COLOR_H
