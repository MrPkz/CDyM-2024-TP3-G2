/*
 * dht11.c
 *
 * Created: 3/6/2024 14:12:15
 *  Author: Barcala
 */ 

#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include "dht11.h"

static void inicializar_sensor(){
	//Protocolo de inicio del sensor DHT seg�n su hoja de datos
	DDRC |= (1 << PINC0);		//Configura el puerto PC0 en salida
	PORTC &= ~(1 << PINC0);		//Deja el puerto en bajo por 20ms
	_delay_ms(20);
	PORTC |= (1 << PINC0);		//Pone el puerto en alto por 30us
	_delay_us(30);
	DDRC &= ~(1 << PINC0);		//Queda esperando en bajo por 160us
	_delay_us(160);
}

uint8_t leer(uint8_t* res){
	uint8_t i, checksum= 0, aux = 0, posAct = 0, numAct = 0, salidaAct = 0, salidaAnt = 0; 
	DDRC |= (1 << PINC3);		//Configura el puerto PC3 en salida
	inicializar_sensor();		//Inicia comunicaci�n con sensor
	while(salidaAct != 1){
		salidaAct = (PINC & (1 << PINC0));	//Detecta el primer flanco de subida
	}
	salidaAnt = 1;				//Guarda el estado previo alto como salida anterior para el pr�ximo polling
	for(i = 0; i<32; i++){
		//PORTC |= (1 << PINC3);						C�digo utilizado para la validaci�n
		//_delay_us(1);									C�digo utilizado para la validaci�n
		//PORTC &= ~(1 << PINC3);						C�digo utilizado para la validaci�n
		_delay_us(50);			//Espera 50us para volver a leer el puerto
		aux |= ((PINC & (1 << PINC0)))<<(7-posAct);		//Lee la posici�n 7-posAct del dato que se est� leyendo; el valor en PINC0 indica si el bit transmitido es 0 o 1
														//La lectura empieza por su MSB y termina en su LSB
		if(++posAct == 8){					//Cuando termina de leer uno de los 4 datos
			res[numAct++] = aux;			//Guarda el dato leido en el vector de salida, y pasa al siguiente dato
			aux = 0;						//Adem�s, reinicia contadores y variables auxiliares
			posAct = 0;
		}
		while(!(salidaAct == 1 && salidaAnt == 0)){		//Espera el pr�ximo flanco de subida
			salidaAnt = salidaAct;
			salidaAct = (PINC & (1 << PINC0));
		}
		salidaAnt = 1;									//Guarda el estado previo alto como salida anterior para el pr�ximo polling
	}
	for(i = 0; i < 8; i++){				//Leer� el dato de checksum para corroborar que no hubo errores en la transmisi�n
		//El proceso de lectura es igual al de los datos previos
		_delay_us(50);
		checksum |= ((PINC & (1 << PINC0)))<<(7-i);
		while(!(salidaAct == 1 && salidaAnt == 0)){
			salidaAnt = salidaAct;
			salidaAct = (PINC & (1 << PINC0));
		}
		salidaAnt = 1;
	}
	aux = (res[0] + res[1] + res[2] + res[3]);		//Si los 4 datos sumados equivalen a aux, la transmisi�n no tuvo errores
	return (checksum == aux);						//Se devuelve 1 si la transmisi�n fue exitosa
}
