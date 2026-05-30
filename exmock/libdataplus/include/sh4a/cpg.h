#ifndef _CPG_H
#define _CPG_H

#include <stdint.h>

#define PLL_48x 0b010111
#define PLL_36x 0b010001
#define PLL_32x 0b001111
#define PLL_30x 0b001110
#define PLL_24X 0b001011
#define PLL_16x 0b000111
#define PLL_12x 0b000101

#define CLK_DIV_2   0b0000
#define CLK_DIV_3   0b0001
#define CLK_DIV_4   0b0010
#define CLK_DIV_6   0b0011
#define CLK_DIV_8   0b0100
#define CLK_DIV_12  0b0101
#define CLK_DIV_16  0b0110
#define CLK_DIV_24  0b1000
#define CLK_DIV_32  0b1001
#define CLK_DIV_36  0b1010
#define CLK_DIV_48  0b1011
#define CLK_DIV_72  0b1101

void set_pll_mult(uint8_t multiplier);
void set_pclk_div(uint8_t divisor);
void set_bclk_div(uint8_t divisor);
void set_shclk_div(uint8_t divisor);
void set_iclk_div(uint8_t divisor);
void frqcr_kick(void);

#endif //_CPG_H
