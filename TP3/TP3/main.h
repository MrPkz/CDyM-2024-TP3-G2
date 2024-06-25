/*
 * main.h
 *
 * Created: 23/6/2024 19:50:10
 *  Author: perei
 */ 


#ifndef MAIN_H_
#define MAIN_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <util/delay.h>
#include "dht11.h"
#include "rtc.h"
#include "serialPort.h"
#define BR9600 (0x67)	// 0x67=103 configura BAUDRATE=9600 a 16MHz

#endif /* MAIN_H_ */