#ifndef _MSTP_H
#define _MSTP_H

#include <type.h>

#define MSTPCR0 0
#define MSTPCR1 1
#define MSTPCR2 2

void mstp_enable(uint8_t cr, uint8_t bit);
void mstp_disable(uint8_t cr, uint8_t bit);

#endif //_MSTP_H
