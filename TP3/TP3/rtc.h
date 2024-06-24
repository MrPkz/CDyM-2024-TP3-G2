/*
 * rtc.h
 *
 * Created: 9/6/2024 20:36:19
 *  Author: perei
 */ 


#ifndef RTC_H_
#define RTC_H_

#include <avr/io.h>		//Se utilizan definiciones de esta librería


#define C_Ds3231ControlRegAddress_U8 0x68		//Dirección de Control del RTC DS3231
//Para leer o escribir en el módulo, se debe utilizar la dirección de control y poner el último bit en 0 o 1 según lectura o escritura
//Los siguientes son los bytes para leer o escribir, con la dirección destino y la operación a realizar
#define C_Ds3231ReadMode_U8   (C_Ds3231ControlRegAddress_U8 << 1) | 0x01
#define C_Ds3231WriteMode_U8  (C_Ds3231ControlRegAddress_U8 << 1)
#define C_Ds3231SecondRegAddress_U8 0x00	//Dirección donde se guardan los segundos en el RTC DS3231
//Los minutos y horas se guardan en las 2 siguientes direcciones, 0x01 y 0x02,respectivamente 
#define C_Ds3231DaysRegAddress_U8 0x04
//Los meses y años se guardan en las 2 siguientes direcciones, 0x05 y 0x06,respectivamente 


//En cada dirección, los datos a buscar se encuentran en bits específicos de la dirección.
//Para filtrar bits que no sean necesarios, se utilizan las siguientes máscaras
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
}rtc_t;		//Define la estructura de fecha con la que se trabajará

void RTC_Init(void);

void RTC_SetDateTime(rtc_t rtc);

void RTC_GetDateTime(rtc_t *rtc);

#endif /* RTC_H_ */