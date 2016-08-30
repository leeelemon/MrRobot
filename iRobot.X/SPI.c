#include "main.h"
#include "SPI.h"

void setupSPI(void){
    //PortB
    TRISB =  0b00000000;

    SSPSTAT = 0b01000000;
    SSPCON = 0b00100000;
    TRISC = 0b11010000;
    PORTC = 0b00000011;

    //timer0
    OPTION_REG = 0b00000100;
    //enable timer0 interrupt
    TMR0IE = 1;

    //Enable all interrupts
    ei();
}

//Transfers data through SPI using SSPBUF/SM_Control_Byte. Note used interchangeably here for illustration purposes
unsigned char spi_transfer(unsigned char data){
    unsigned char temp = 0;

    SSPIF = 0;
    SSPBUF = data;

    while (SSPIF == 0);
    temp = SSPBUF;
    SSPIF = 0;

    return temp;
}