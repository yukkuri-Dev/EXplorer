#ifndef _ADC_H
#define _ADC_H

#include <stdint.h>

struct adc {
	uint16_t channel_a;
	uint16_t channel_b;
	uint16_t channel_c;
	uint16_t channel_d;
};

void adc_read_multi(struct adc *channels);
uint16_t adc_read(uint8_t channel);

#endif //_ADC_H
