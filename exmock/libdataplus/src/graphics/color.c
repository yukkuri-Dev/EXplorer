
#include <graphics/color.h>

static uint16_t pen_color = 0;

uint16_t create_rgb16(uint8_t red, uint8_t green, uint8_t blue)
{
	return ((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3);
}

void set_pen(uint16_t color)
{
	pen_color = color;
}

uint16_t get_pen()
{
	return pen_color;
}
