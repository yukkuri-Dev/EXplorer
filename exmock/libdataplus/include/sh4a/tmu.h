#ifndef _TMU_H
#define _TMU_H

#include <stdint.h>

#define TIMER0 0
#define TIMER1 1
#define TIMER2 2

#define TIMER_CLK_4    0
#define TIMER_CLK_16   1
#define TIMER_CLK_64   2
#define TIMER_CLK_256  3
#define TIMER_CLK_1024 4

void tmu0_start(uint8_t timer, uint8_t clk, uint32_t initial);
void tmu0_stop(uint8_t timer);


void tmu0_enable(uint8_t timer);
void tmu0_disable(uint8_t timer);
void tmu0_set_priority(uint8_t timer, uint8_t pri);

#endif //_TMU_H
