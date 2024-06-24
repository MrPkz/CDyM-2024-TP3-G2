/*
 * i2c.c
 *
 * Created: 9/6/2024 19:30:47
 *  Author: perei
 */ 

#include "i2c.h"

void i2c_init(void){
	TWSR=0x00;		//Establece el Prescaler en 0
	TWBR=152;		//Para una frec. externa XTAL=16M, hace que la frecuencia de SCL sea 50kHz
	TWCR=0x04;		//Habilita o "prende" el módulo
}

void i2c_start(void){		//Envia start y espera confirmacion
	TWCR= (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);	//Prende los bits de interrupción, start y enable
	while((TWCR & (1<<TWINT))==0);			//Espera activacion de flag TWINT para ver transmision completa
}

void i2c_write(uint8_t data){
	TWDR= data; //Copia dato a enviar
	TWCR=(1<<TWINT)|(1<<TWEN);	//Pone en 1 el enable y la interrupción para iniciar transmision
	while((TWCR & (1<<TWINT))==0);	//Espera activacion de flag TWINT para ver transmision completa
}

uint8_t i2c_read(uint8_t isLast){
	/*
		Siempre manda TWEN =1 y TWINT =1 con el bit de Acknowledge (TWEA) para la recepción
		Luego, espera activación del flag TWINT (recepción completa) y lee el dato desde el TWDR
	*/
	if(isLast==0)	//isLast se utiliza para saber si hay que enviar ACK o NACK
		TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWEA);	//Envía ACK
	else	
		TWCR=(1<<TWINT)|(1<<TWEN);				//Envía NACK (al enviar un 0 en TWEA)
	while((TWCR & (1<<TWINT))==0);				//Espera que la comunicación haya sido recibida
	return TWDR;
}

void i2c_stop(){
	TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWSTO);		//Manda condición de Stop con TWSTO
	//No se debe usar el TWINT luego de esta operación a menos que se reinicie de vuelta el módulo
}