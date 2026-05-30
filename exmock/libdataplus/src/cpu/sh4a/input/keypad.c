
#include <sh4a/input/keypad.h>

volatile unsigned long *  keypad = (unsigned long*)0xA44B0000;

volatile unsigned long key_state[3];
volatile unsigned long key_press[3];

/*
	keypad debouncer
*/
void keypad_read()
{
	static long ct0[3], ct1[3]; 
	long i;

	i = key_state[0] ^ (keypad[0]);          // key changed ? 
	ct0[0] = ~( ct0[0] & i );                   // reset or count ct0 
	ct1[0] = ct0[0] ^ (ct1[0] & i);                // reset or count ct1 
	i &= ct0[0] & ct1[0];                       // count until roll over ? 
	key_state[0] ^= i;                       // then toggle debounced state 
                                        // now debouncing finished 
	key_press[0] |= key_state[0] & i;

	i = key_state[1] ^ (keypad[1]);          // key changed ? 
	ct0[1] = ~( ct0[1] & i );                   // reset or count ct0 
	ct1[1] = ct0[1] ^ (ct1[1] & i);                // reset or count ct1 
	i &= ct0[1] & ct1[1];                       // count until roll over ? 
	key_state[1] ^= i;                       // then toggle debounced state 
                                        // now debouncing finished 
	key_press[1] |= key_state[1] & i;

	i = key_state[2] ^ (keypad[2]);          // key changed ? 
	ct0[2] = ~( ct0[2] & i );                   // reset or count ct0 
	ct1[2] = ct0[2] ^ (ct1[2] & i);                // reset or count ct1 
	i &= ct0[2] & ct1[2];                       // count until roll over ? 
	key_state[2] ^= i;                       // then toggle debounced state 
                                        // now debouncing finished 
	key_press[2] |= key_state[2] & i;
}

uint16_t get_key_pressed(uint8_t key_code)
{
	int row, col, word, bit, mask; 
	row = key_code % 10;
	col = key_code / 10 - 1;
	word = row >> 1;
	bit = col + 8 * (row & 1);
	mask = (((unsigned short*)key_press)[word] & 1 << bit);
	((unsigned short*)key_press)[word] ^= mask;
	return mask;
}

uint16_t get_key_state(uint8_t key_code)
{
	int row, col, word, bit, mask; 
	row = key_code % 10;
	col = key_code / 10 - 1;
	word = row >> 1;
	bit = col + 8 * (row & 1);
	return (((unsigned short*)key_state)[word] & 1 << bit);
}

