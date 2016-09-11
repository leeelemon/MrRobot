

#ifndef MAIN_H
#define	MAIN_H

#include <xc.h>
#pragma config BOREN = OFF, CPD = OFF, WRT = OFF, FOSC = HS, WDTE = OFF, CP = OFF, LVP = OFF, PWRTE = OFF

//Defines
//Osillator (Hz)
#define _XTAL_FREQ 20000000

#define TMR0_VAL 100	//timer0 start count
#define STEPS180  200	//half steps for a 180 degree rotation
#define SM_Control_Byte SSPBUF	//SM_Control_Byte is SSPBUF

//Macros that you can call for ease of use
#define     SELECT_NONE()	RC0 = 0; RC1 = 0;
#define     SELECT_EEPROM()	RC0 = 0; RC1 = 1;
#define     SELECT_RF()		RC0 = 1; RC1 = 0;
#define     SELECT_SM()		RC0 = 1; RC1 = 1;
#define     SM_STEP()		RC2 = 1; NOP(); RC2 = 0;

#define PB6 !RB2
#define PB7 !RB1
#define PB8 !RB0


unsigned char rxbyte = 0;
signed int stepClosest = 0;
signed int adcClosest = 1000;
unsigned char highByte = 0;
unsigned char lowByte = 0;
signed int distTrav = 0;
signed int totalDistTrav = 0;
signed int angleTurned = 0;
signed int totalangleTurned = 0;
int scan = 0;
int ADChigh = 0;
int position = 0;



#endif	/* MAIN_H */

