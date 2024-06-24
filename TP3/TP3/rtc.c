/*
 * rtc.c
 *
 * Created: 9/6/2024 20:36:03
 *  Author: perei
 */ 

#include "i2c.h"
#include "rtc.h"

static uint8_t bcdToInt(uint8_t bcd) {
	int tens = (bcd >> 4) & 0x0F;					// Se queda con los 4 bits que representan el d�gito de decenas
	int ones = bcd & 0x0F;							// Se queda con los 4 bits que representan el d�gito de unidades
	return tens * 10 + ones;						// Devuelve el n�mero final
}

static uint8_t intToBcd(uint8_t in) {						//"in" es el n�mero a convertir
	uint8_t ans;
	ans= ((in/10)<<4)& (0xF0);						//Guarda en la parte alta de ans las decenas
	ans|= in % 10;									//Guarda en la parte baja de ans las unidades
	return ans;
}

void RTC_Init(void)
{
	i2c_init();										// Inicia el m�dulo I2C
	i2c_start();									// Comienza la comunicaci�n
	
	i2c_write(C_Ds3231WriteMode_U8);				// Conecta el m�dulo mandando la direcci�n al bus, indicando que va a escribir
	i2c_write(C_Ds3231ControlRegAddress_U8);		// Escribe la direcci�n de Control del dispositivo, que ser� la que escribir�
	
	i2c_write(0x00);								//Escribe 0x00 en el registro de control para habilitar la se�al de reloj
	
	i2c_stop();										//Termina la comunicaci�n
}

void RTC_SetDateTime(rtc_t rtc)
{
	i2c_start();									// Comienza la comunicaci�n
	
	i2c_write(C_Ds3231WriteMode_U8);				// Indica que va a escribir
	i2c_write(C_Ds3231SecondRegAddress_U8);			// Indica que la escritura se har� en la direcci�n de los segundos
	
	i2c_write(intToBcd(rtc.sec) & MASK_SEC);       // Escribe los segundos en la direcci�n pertinente, filtrando los bits pertinentes
	i2c_write(intToBcd(rtc.min) & MASK_MIN);		// Escribe los minutos en la direcci�n consecutiva, filtrando los bits pertinentes
	i2c_write(intToBcd(rtc.hour) & MASK_HORA);     // Escribe las horas en la direcci�n consecutiva, filtrando los bits pertinentes
													//Todas las escrituras las hace luego de pasar el n�mero de entero a BCD
	
	i2c_stop();										//Termina la comunicaci�n que env�a la hora
	
	i2c_start();									// Comienza la comunicaci�n
	
	i2c_write(C_Ds3231WriteMode_U8);				// Indica que va a escribir
	i2c_write(C_Ds3231DaysRegAddress_U8);			// Indica que la escritura se har� en la direcci�n de los d�as
	
	i2c_write(intToBcd(rtc.date)& MASK_DAY);       // Escribe los d�as en la direcci�n pertinente, filtrando los bits pertinentes
	i2c_write(intToBcd(rtc.month)& MASK_MES);		// Escribe los meses en la direcci�n consecutiva, filtrando los bits pertinentes
	i2c_write(intToBcd(rtc.year)& MASK_YEAR);      // Escribe los a�os en la direcci�n consecutiva, filtrando los bits pertinentes
	
	i2c_stop();										//Termina la comunicaci�n habiendo ya enviado la hora y fecha
}

void RTC_GetDateTime(rtc_t *rtc)
{
	i2c_start();									//Comienza la comunicaci�n
	
	i2c_write(C_Ds3231WriteMode_U8);				// Va a escribir
	i2c_write(C_Ds3231SecondRegAddress_U8);			// Escribe la direcci�n de los segundos, para luego poder leerlos
	
	i2c_stop();										// Termina la comunicaci�n
	
	i2c_start();									// Comienza la comunicaci�n
	i2c_write(C_Ds3231ReadMode_U8);					// Indica que va a leer
	
	rtc->sec = bcdToInt(i2c_read(0));               //Lee segundos, manda un ACK, lo convierte a entero y lo guarda en la estructura
	rtc->min = bcdToInt(i2c_read(0));               //Lee minutos, manda un ACK, lo convierte a entero y lo guarda en la estructura
	rtc->hour= bcdToInt(i2c_read(1));               //Lee horas, manda un NACK pues es el fin de la comunicaci�n, lo convierte a entero y lo guarda
	
	i2c_stop();										// Termina la comunicaci�n para leer la hora
	
	
	i2c_start();									// Inicia de nuevo la comunicaci�n
	
	i2c_write(C_Ds3231WriteMode_U8);				// Va a escribir
	i2c_write(C_Ds3231DaysRegAddress_U8);			// Escribe la direcci�n de los d�as para leerlos
	
	i2c_stop();										// Termina la comunicaci�n
	
	i2c_start();									// Comienza la comunicaci�n
	i2c_write(C_Ds3231ReadMode_U8);					// Indica que va a leer en la direcci�n antes escrita
	
	rtc->date= bcdToInt(i2c_read(0));				//Lee d�as, manda un ACK, lo convierte a entero y lo guarda en la estructura
	rtc->month= bcdToInt(i2c_read(0));				//Lee meses, manda un ACK, lo convierte a entero y lo guarda en la estructura
	rtc->year = bcdToInt(i2c_read(1));				//Lee a�os, manda un NACK pues es el fin de la comunicaci�n, lo convierte a entero y lo guarda
	
	i2c_stop();										// Termina la comunicaci�n luego de haber le�do todo
}