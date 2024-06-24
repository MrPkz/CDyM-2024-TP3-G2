/*
 * rtc.c
 *
 * Created: 9/6/2024 20:36:03
 *  Author: perei
 */ 

#include "i2c.h"
#include "rtc.h"

static uint8_t bcdToInt(uint8_t bcd) {
	int tens = (bcd >> 4) & 0x0F;					// Se queda con los 4 bits que representan el dígito de decenas
	int ones = bcd & 0x0F;							// Se queda con los 4 bits que representan el dígito de unidades
	return tens * 10 + ones;						// Devuelve el número final
}

static uint8_t intToBcd(uint8_t in) {						//"in" es el número a convertir
	uint8_t ans;
	ans= ((in/10)<<4)& (0xF0);						//Guarda en la parte alta de ans las decenas
	ans|= in % 10;									//Guarda en la parte baja de ans las unidades
	return ans;
}

void RTC_Init(void)
{
	i2c_init();										// Inicia el módulo I2C
	i2c_start();									// Comienza la comunicación
	
	i2c_write(C_Ds3231WriteMode_U8);				// Conecta el módulo mandando la dirección al bus, indicando que va a escribir
	i2c_write(C_Ds3231ControlRegAddress_U8);		// Escribe la dirección de Control del dispositivo, que será la que escribirá
	
	i2c_write(0x00);								//Escribe 0x00 en el registro de control para habilitar la señal de reloj
	
	i2c_stop();										//Termina la comunicación
}

void RTC_SetDateTime(rtc_t rtc)
{
	i2c_start();									// Comienza la comunicación
	
	i2c_write(C_Ds3231WriteMode_U8);				// Indica que va a escribir
	i2c_write(C_Ds3231SecondRegAddress_U8);			// Indica que la escritura se hará en la dirección de los segundos
	
	i2c_write(intToBcd(rtc.sec) & MASK_SEC);       // Escribe los segundos en la dirección pertinente, filtrando los bits pertinentes
	i2c_write(intToBcd(rtc.min) & MASK_MIN);		// Escribe los minutos en la dirección consecutiva, filtrando los bits pertinentes
	i2c_write(intToBcd(rtc.hour) & MASK_HORA);     // Escribe las horas en la dirección consecutiva, filtrando los bits pertinentes
													//Todas las escrituras las hace luego de pasar el número de entero a BCD
	
	i2c_stop();										//Termina la comunicación que envía la hora
	
	i2c_start();									// Comienza la comunicación
	
	i2c_write(C_Ds3231WriteMode_U8);				// Indica que va a escribir
	i2c_write(C_Ds3231DaysRegAddress_U8);			// Indica que la escritura se hará en la dirección de los días
	
	i2c_write(intToBcd(rtc.date)& MASK_DAY);       // Escribe los días en la dirección pertinente, filtrando los bits pertinentes
	i2c_write(intToBcd(rtc.month)& MASK_MES);		// Escribe los meses en la dirección consecutiva, filtrando los bits pertinentes
	i2c_write(intToBcd(rtc.year)& MASK_YEAR);      // Escribe los años en la dirección consecutiva, filtrando los bits pertinentes
	
	i2c_stop();										//Termina la comunicación habiendo ya enviado la hora y fecha
}

void RTC_GetDateTime(rtc_t *rtc)
{
	i2c_start();									//Comienza la comunicación
	
	i2c_write(C_Ds3231WriteMode_U8);				// Va a escribir
	i2c_write(C_Ds3231SecondRegAddress_U8);			// Escribe la dirección de los segundos, para luego poder leerlos
	
	i2c_stop();										// Termina la comunicación
	
	i2c_start();									// Comienza la comunicación
	i2c_write(C_Ds3231ReadMode_U8);					// Indica que va a leer
	
	rtc->sec = bcdToInt(i2c_read(0));               //Lee segundos, manda un ACK, lo convierte a entero y lo guarda en la estructura
	rtc->min = bcdToInt(i2c_read(0));               //Lee minutos, manda un ACK, lo convierte a entero y lo guarda en la estructura
	rtc->hour= bcdToInt(i2c_read(1));               //Lee horas, manda un NACK pues es el fin de la comunicación, lo convierte a entero y lo guarda
	
	i2c_stop();										// Termina la comunicación para leer la hora
	
	
	i2c_start();									// Inicia de nuevo la comunicación
	
	i2c_write(C_Ds3231WriteMode_U8);				// Va a escribir
	i2c_write(C_Ds3231DaysRegAddress_U8);			// Escribe la dirección de los días para leerlos
	
	i2c_stop();										// Termina la comunicación
	
	i2c_start();									// Comienza la comunicación
	i2c_write(C_Ds3231ReadMode_U8);					// Indica que va a leer en la dirección antes escrita
	
	rtc->date= bcdToInt(i2c_read(0));				//Lee días, manda un ACK, lo convierte a entero y lo guarda en la estructura
	rtc->month= bcdToInt(i2c_read(0));				//Lee meses, manda un ACK, lo convierte a entero y lo guarda en la estructura
	rtc->year = bcdToInt(i2c_read(1));				//Lee años, manda un NACK pues es el fin de la comunicación, lo convierte a entero y lo guarda
	
	i2c_stop();										// Termina la comunicación luego de haber leído todo
}