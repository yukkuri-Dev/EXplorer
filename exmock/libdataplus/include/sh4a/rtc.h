#ifndef _RTC_H
#define _RTC_H


#include <stdint.h>

struct rtc_time {
	uint8_t seconds;
	uint8_t minutes;
	uint8_t	hours;
	uint8_t	day;
	uint8_t date;
	uint8_t month;
	uint16_t year;
} __attribute__((packed));

void rtc_set_time(struct rtc_time *tm);
void rtc_get_time(struct rtc_time *tm);

#endif //_RTC_H
