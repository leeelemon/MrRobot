#ifndef _SER_H
#define _SER_H

#include "main.h"

signed int speedH = 0;
signed int speedL = 0;
signed int radH = 0;
signed int radL = 0;

//initalise serial comms
void ser_init(void);	

//transmits char data through serial
void ser_putch(unsigned char c);	

//receives serial data
unsigned char ser_getch();

//Drive command
void Drive(signed int speedH, signed int speedL, signed int radH, signed int radL);

//Given sensor packetID and expected number of bytes, outputs sensor value in a single variable.
signed int getSensorData(unsigned char packetID, unsigned char bytes);

#endif //_SER_H