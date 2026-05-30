
#include <graphics/init.h>
#include <graphics/lcdc.h>

void graphics_init(uint16_t width, uint16_t height, void * vram)
{
	lcdc_set_window(0, 0, width, height);
	lcdc_set_vram_address(vram);
}

