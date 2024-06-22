/*
 * rtc.c
 *
 * Created: 9/6/2024 20:36:03
 *  Author: perei
 */ 

#include "i2c.h"
#include "rtc.h"

uint8_t bcdToInt(uint8_t bcd) {
	int tens = (bcd >> 4) & 0x0F; // Extract the tens place (higher nibble)
	int ones = bcd & 0x0F;        // Extract the ones place (lower nibble)
	return tens * 10 + ones;
}

uint8_t intToBcd(uint8_t in) {
	uint8_t ans;
	ans= ((in/10)<<4)& (0xF0);
	ans|= in % 10;
	return ans;
}

void RTC_Init(void)
{
	i2c_init();                             // Initialize the I2c module.
	i2c_start();                            // Start I2C communication
	
	i2c_write(C_Ds3231WriteMode_U8);        // Connect to DS1307 by sending its ID on I2c Bus
	i2c_write(C_Ds3231ControlRegAddress_U8);// Select the Ds1307 ControlRegister to configure Ds1307
	
	i2c_write(0x00);                        // Write 0x00 to Control register to disable SQW-Out
	
	i2c_stop();                             // Stop I2C communication after initializing DS1307
}

void RTC_SetDateTime(rtc_t *rtc)
{
	i2c_start();                          // Start I2C communication
	
	i2c_write(C_Ds3231WriteMode_U8);      // connect to DS1307 by sending its ID on I2c Bus
	i2c_write(C_Ds3231SecondRegAddress_U8); // Request sec RAM address at 00H
	
	i2c_write(intToBcd(rtc->sec) & MASK_SEC);                    // Write sec from RAM address 00H
	i2c_write(intToBcd(rtc->min) & MASK_MIN);                    // Write min from RAM address 01H
	i2c_write(intToBcd(rtc->hour) & MASK_HORA);                    // Write hour from RAM address 02H
	
	i2c_stop();                              // Stop I2C communication after Setting the Date
	
	
	i2c_start();                          // Start I2C communication
	
	i2c_write(C_Ds3231WriteMode_U8);      // connect to DS1307 by sending its ID on I2c Bus
	i2c_write(C_Ds3231DaysRegAddress_U8); // Request sec RAM address at 00H
	
	i2c_write(intToBcd(rtc->date)& MASK_DAY);                    // Write date on RAM address 04H
	i2c_write(intToBcd(rtc->month)& MASK_MES);                    // Write month on RAM address 05H
	i2c_write(intToBcd(rtc->year)& MASK_YEAR);                    // Write year on RAM address 06h
	
	i2c_stop();                              // Stop I2C communication after Setting the Date
}

void RTC_GetDateTime(rtc_t *rtc)
{
	i2c_start();                            // Start I2C communication
	
	i2c_write(C_Ds3231WriteMode_U8);        // connect to DS1307 by sending its ID on I2c Bus
	i2c_write(C_Ds3231SecondRegAddress_U8); // Request Sec RAM address at 00H
	
	i2c_stop();                                // Stop I2C communication after selecting Sec Register
	
	i2c_start();                            // Start I2C communication
	i2c_write(C_Ds3231ReadMode_U8);            // connect to DS1307(Read mode) by sending its ID
	
	rtc->sec = bcdToInt(i2c_read(0));                // read second and return Positive ACK
	rtc->min = bcdToInt(i2c_read(0));                 // read minute and return Positive ACK
	rtc->hour= bcdToInt(i2c_read(1));               // read hour and return Negative/No ACK
	
	i2c_stop();                              // Stop I2C communication after reading the Date
	
	
	i2c_start();                            // Start I2C communication
	
	i2c_write(C_Ds3231WriteMode_U8);        // connect to DS1307 by sending its ID on I2c Bus
	i2c_write(C_Ds3231DaysRegAddress_U8); // Request Sec RAM address at 00H
	
	i2c_stop();                                // Stop I2C communication after selecting Sec Register
	
	i2c_start();                            // Start I2C communication
	i2c_write(C_Ds3231ReadMode_U8);            // connect to DS1307(Read mode) by sending its ID
	
	rtc->date= bcdToInt(i2c_read(0));              // read Date and return Positive ACK
	rtc->month= bcdToInt(i2c_read(0));            // read Month and return Positive ACK
	rtc->year = bcdToInt(i2c_read(1));             // read Year and return Negative/No ACK
	
	i2c_stop();                              // Stop I2C communication after reading the Date
}