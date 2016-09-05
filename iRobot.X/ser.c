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
    __delay_ms(100);
    
}

signed int ser_getch(){
	while(!RCIF);
	signed int rxbyte = RCREG;
    __delay_ms(100);
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

signed int getDistTrav(void){
    ser_putch(142);     //Requests packet of sensor data
    ser_putch(19);      //Specifies distance packet for request
    __delay_ms(10);
          
    highByte = ser_getch();  //Puts the high byte into variable
    __delay_ms(10);
    lowByte = ser_getch();   //Puts the low byte into variable
     
    distTrav = (256*highByte + lowByte);    //Distance traveled since data was last requested
    totalDistTrav = ((totalDistTrav + distTrav));  //Total distance traveled since button push in CM
  
    
    lcdSetCursor(0b11000000);   //Second row, first position
    lcdWriteToDigitBCD(totalDistTrav);  //Print the total distance to LCD
}