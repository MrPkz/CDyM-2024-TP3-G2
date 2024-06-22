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




void inicializar_sensor(){
	DDRC |= (1 << PINC0);
	PORTC &= ~(1 << PINC0);
	_delay_ms(20);
	PORTC |= (1 << PINC0);
	_delay_us(30);
	DDRC &= ~(1 << PINC0);
	_delay_us(160);
}

uint8_t leer(uint8_t* res){
	uint8_t i, checksum= 0, aux = 0, posAct = 0, numAct = 0, salidaAct = 0, salidaAnt = 0; 
	DDRC |= (1 << PINC3);
	inicializar_sensor();
	while(salidaAct != 1){
		salidaAct = (PINC & (1 << PINC0));
	}
	salidaAnt = 1;
	for(i = 0; i<32; i++){
		_delay_us(50);
		aux |= ((PINC & (1 << PINC0)))<<(7-posAct);
		if(++posAct == 8){
			res[numAct++] = aux;
			aux = 0;
			posAct = 0;
		}
		while(!(salidaAct == 1 && salidaAnt == 0)){
			salidaAnt = salidaAct;
			salidaAct = (PINC & (1 << PINC0));
		}
		PORTC |= (1 << PINC3);
		_delay_us(1);
		PORTC &= ~(1 << PINC3);
		salidaAnt = 1;
	}
	for(i = 0; i < 8; i++){
		_delay_us(50);
		checksum |= ((PINC & (1 << PINC0)))<<(7-i);
		while(!(salidaAct == 1 && salidaAnt == 0)){
			salidaAnt = salidaAct;
			salidaAct = (PINC & (1 << PINC0));
		}
		salidaAnt = 1;
	}
	aux = (res[0] + res[1] + res[2] + res[3]);
	return (checksum == aux);
}
