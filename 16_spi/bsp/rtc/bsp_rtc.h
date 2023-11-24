#ifndef _BSP_RTC_H_
#define _BSP_RTC_H_


#include "main.h"


#define SECONDS_IN_A_DAY        (86400)
#define SECONDS_IN_A_HOUR       (3600)
#define SECONDS_IN_A_MINUTE     (60)
#define DAYS_IN_A_YEAR          (365)
#define YEAR_RANGE_START        (1970)
#define YAER_RANGE_END          (2099)

extern struct rtc_datetime rtc_Date;

struct rtc_datetime
{
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
};



void rtc_enable(void);
void rtc_disable(void);
unsigned char rtc_isleapyear(unsigned short year);
void rtc_set_datetime(struct rtc_datetime *datetime);
void my_rtc_init(void);
uint64_t rtc_get_seconds(void);
void rtc_convertseconds_to_datetime(u64 seconds, struct rtc_datetime *datetime);
void rtc_get_datetime(struct rtc_datetime *rtc_date);
#endif // ! _BSP_RTC_H_