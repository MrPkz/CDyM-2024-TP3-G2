/*
 * rtc.h
 *
 * Created: 9/6/2024 20:36:19
 *  Author: perei
 */ 


#ifndef RTC_H_
#define RTC_H_

#include <avr/io.h>

#define C_Ds3231ControlRegAddress_U8 0x68
#define C_Ds3231ReadMode_U8   (C_Ds3231ControlRegAddress_U8 << 1) | 0x01
#define C_Ds3231WriteMode_U8  (C_Ds3231ControlRegAddress_U8 << 1)
#define C_Ds3231SecondRegAddress_U8 0x00	//defines the register address for the seconds value in the DS3231 RTC module
#define C_Ds3231DaysRegAddress_U8 0x04
	//defines the register address for the control register in the DS3231 RTC module
//Everybody say gracias ChatGPT por las ultimas 2 direcciones!!!!

#define MASK_SEC 0b01111111
#define MASK_MIN 0b01111111
#define MASK_HORA 0b00111111
#define MASK_DAY 0b00111111
#define MASK_MES 0b00011111
#define MASK_YEAR 0b11111111

typedef struct
{
	uint8_t sec;
	uint8_t min;
	uint8_t hour;
	uint8_t date;
	uint8_t month;
	uint8_t year;
}rtc_t;

void RTC_Init(void);

void RTC_SetDateTime(rtc_t *rtc);

void RTC_GetDateTime(rtc_t *rtc);

#endif /* RTC_H_ */