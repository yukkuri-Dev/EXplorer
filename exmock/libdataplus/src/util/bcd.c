
#include <util/bcd.h>

uint8_t bin2bcd(uint8_t binary)
{
	return ((binary / 10) << 4) + (binary % 10);
}

uint8_t bcd2bin(uint8_t bcd)
{
	return (bcd & 0xf) + (bcd >> 4) * 10;
}
