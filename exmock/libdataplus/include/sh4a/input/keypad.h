#ifndef _KEYPAD_H
#define _KEYPAD_H


#include <stdint.h>
#include <sh4a/input/exword_keys.h>

void keypad_read();
uint16_t get_key_pressed(uint8_t key_code);
uint16_t get_key_state(uint8_t key_code);

#endif //_KEYPAD_H
