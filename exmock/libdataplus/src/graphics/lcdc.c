
#include <stdint.h>

static volatile uint32_t vram = 0xac200000;
static volatile uint16_t screen_width = 0x210;
static volatile uint16_t screen_height = 0x140;

static volatile uint16_t * const LCDC = (uint16_t *)0xb4000000;
static volatile uint8_t  * const PORTR = (uint8_t *)0xa405013c;


static volatile uint32_t * const  DMAC0_SAR3 = (uint32_t *)0xfe008050;
static volatile uint32_t * const  DMAC0_DAR3 = (uint32_t *)0xfe008054;
static volatile uint32_t * const  DMAC0_TCR3 = (uint32_t *)0xfe008058;
static volatile uint32_t * const  DMAC0_CHCR3 = (uint32_t *)0xfe00805c;
static volatile uint16_t * const  DMAC0_DMAOR = (uint16_t *)0xfe008060;

#pragma GCC optimize ("3")

void lcdc_set_vram_address(void* address)
{
	vram = (uint32_t)address;
}

void * lcdc_get_vram_address(void)
{
	return (void*)vram;
}

void lcdc_get_dimensions(uint16_t *width, uint16_t *height)
{
	*width = screen_width;
	*height = screen_height;
}

uint16_t lcdc_read_word(void)
{
	return *LCDC;
}

void lcdc_write_word(uint16_t value)
{
	*LCDC = value;
	asm("synco");
}

void lcdc_write_cmd(uint16_t cmd)
{
	*PORTR = *PORTR & 0xef;
	asm("synco");
	*LCDC = cmd;
	asm("synco");
	*PORTR = *PORTR | 0x10;
	asm("synco");
}


void lcdc_set_window(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
	screen_width = width;
	screen_height = height;

	width = (x + width) - 1;
        y += 0x28;
        height = (y + height) - 1;

        lcdc_write_cmd(0x2a);
        lcdc_write_word((x >> 8) & 0xFF);
        lcdc_write_word(x & 0xFF);
        lcdc_write_word((width >> 8) & 0xFF);
        lcdc_write_word(width & 0xFF);

        lcdc_write_cmd(0x2b);
        lcdc_write_word((y >> 8) & 0xFF);
        lcdc_write_word(y & 0xFF);
        lcdc_write_word((height >> 8) & 0xFF);
        lcdc_write_word(height & 0xFF);
}

void lcdc_copy_vram()
{
	lcdc_write_cmd(0x2c);
	*DMAC0_TCR3 = ((screen_width * screen_height)) >> 4;
	*DMAC0_SAR3 = vram & 0x1fffffff;
	*DMAC0_DAR3 = 0xb4000000 & 0x1fffffff;
	*DMAC0_DMAOR = *DMAC0_DMAOR & 0xfffe;
	*DMAC0_CHCR3 =  0x40101401;
	*DMAC0_DMAOR = *DMAC0_DMAOR | 0x1;
	while (!(*DMAC0_CHCR3 & 0x2));
}

