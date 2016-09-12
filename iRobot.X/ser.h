#ifndef _SER_H
#define _SER_H

#include "main.h"

unsigned char speedH = 0;
unsigned char speedL = 0;
unsigned char radH = 0;
unsigned char radL = 0;

unsigned int VelocityRight = 0;
unsigned int VelocityLeft = 0;
unsigned char RightSpeedH = 0;
unsigned char RightSpeedL = 0;
unsigned char LeftSpeedH = 0;
unsigned char LeftSpeedL = 0;

//initalise serial comms
void ser_init(void);	

//transmits char data through serial
void ser_putch(unsigned char c);	

//receives serial data
unsigned char ser_getch();

//Drive command
void Drive(unsigned char speedH, unsigned char speedL, unsigned char radH, unsigned char radL);

void DriveDirect(signed int VelocityRight, signed int VelocityLeft);

//Given sensor packetID and expected number of bytes, outputs sensor value in a single variable.
signed int getSensorData(unsigned char packetID, unsigned char bytes);

#endif //_SER_H