#include "main.h"
#include "ser.h"
#include "lcd.h"

void ser_init(void){
//	PORTC = 0;
	TXSTA = 0b00100100;			//setup uart serial comms as below
	RCSTA = 0b10011000;			

/*
	BRGH=1;
	SYNC=0;						//asynchronous
	SPEN=1;						//enable serial port pins
	CREN=1;						//enable reception
	SREN=0;						//no effect
	TXIE=0;						//disable tx interrupts
	RCIE=0;						//disable rx interrupts
	TX9=0;						//8-bit transmission
	RX9=0;						//8-bit reception
	TXEN=0;						//reset transmitter
	TXEN=1;						//enable the transmitter
*/
    
	SPBRG = 20; 					//Baud rate 57600
//	RCIE = 1;						//enable receive interrupt
	PEIE = 1;						//enable peripheral interrupt
	ei();							//enable interrupts
}

void ser_putch(unsigned char c){	
	while(!TRMT);					//while buffer is not empty, wait
	TXREG=c;						//load register with data to be transmitted
}

unsigned char ser_getch(){
	while(!RCIF);
	unsigned char rxbyte = RCREG;
	return rxbyte;
    
}

//Drive command (Velocity high, Velocity low, Radius high, Radius low)
//Radius of 0x7FFF is a straight line
//Negative radius turns right
void Drive(signed int speedH, signed int speedL, signed int radH, signed int radL){
    ser_putch(137);
        ser_putch(speedH);
        ser_putch(speedL);
        ser_putch(radH);
        ser_putch(radL);
}


//Will return as requested sensor data, specify packetID and how many bytes the expected output is.
signed int getSensorData(unsigned char packetID, unsigned char bytes){
    ser_putch(142);
        ser_putch(packetID);
    
    if (bytes == 2)
        highByte = ser_getch();  
    else 
        highByte = 0;
        
    lowByte = ser_getch();
    
    signed int sensor = (256*highByte + lowByte);
    
    return sensor;
    
    
    
}