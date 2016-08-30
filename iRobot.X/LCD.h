/* 
 * File:   LCD.h
 * Author: Brendan
 *
 * Created on 11 August 2016, 12:53 PM
 */

#ifndef LCD_H
#define	LCD_H

//Defines LCD
#define RS RE0			//LCD Register Select
#define RW RE1			//LCD Read Write
#define EN RE2			//LCD Enable
#define LCD_DATA PORTD	//LCD data pins are connected to Port D

volatile unsigned int stepCountNeg = 0;
volatile unsigned int degrees = 0;

//write current step count to LCD
void lcdWriteStepCount(unsigned int stepCount);

//write controls to LCD
void lcdWriteControl(unsigned char databyte);

//write data to LCD
void lcdWriteData(unsigned char databyte);

//move the LCD cursor to a particular location
void lcdSetCursor(unsigned char address);

//write string to LCD
void lcdWriteString(char * s);

//function accepts char between 0 and 99 and writes it to LCD display in 2 digits
void lcdWriteToDigitBCD(unsigned int data);

//function initialises the LCD module - check that ADCON1 setting doesn't conflict
void setupLCD(void);

#endif	/* LCD_H */

