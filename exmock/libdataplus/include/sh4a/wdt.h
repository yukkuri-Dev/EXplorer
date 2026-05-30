#ifndef _WDT_H
#define _WDT_H

#include <stdint.h>

void wdt_set_cnt(uint8_t value);
void wdt_disable();
void wdt_enable(uint8_t timer);

#endif //_WDT_H
