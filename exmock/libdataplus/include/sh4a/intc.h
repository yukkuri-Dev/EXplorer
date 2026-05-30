#ifndef _INTC_H
#define _INTC_H

#include <stdint.h>

typedef void (*interrupt)(void);

interrupt get_interrupt(uint16_t code);
interrupt set_interrupt(uint16_t code, interrupt func_ptr);

void cei(void);
void sei(void);

#endif //_INTC_H
