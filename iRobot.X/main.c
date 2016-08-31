
#include "main.h"
#include "LCD.h"
#include "ser.h"
#include "SPI.h"
#include "ADC.h"
#include "motor.h"

volatile unsigned int time_count;
volatile bit FLAG_1000MS;
unsigned char PB8Counter = 0;
unsigned char rxbyte = 0;
signed int stepClosest = 0;
signed int adcClosest = 1000;
signed int highByte = 0;
signed int lowByte = 0;
signed int distTrav = 0;
unsigned char controlByte = 0;
signed int x = 0;


// Interrupt service routine
void interrupt isr(void){
//Timer 1
    if(TMR0IF){
        TMR0IF = 0;
        TMR0 = TMR0_VAL;
        time_count++;

        if(time_count % 1 == 0) {
            //FLAG_10MS = 1;
            SM_STEP();
        }

        if(time_count % 1000 == 0){
            FLAG_1000MS = 1;	// Raise flag for 500ms
            time_count = 0;	// Clear time_count        
        }
        if (PB8 == 1)
            PB8Counter++;
        
    }
}


void main(void){
  
//Initialise and setup
    setupSPI();
    ser_init();
    setupLCD();
    setupADC();
    
    unsigned char controlByte = 0b00001101;
    spi_transfer(controlByte);

    
    ser_putch(128);     //Startup
    ser_putch(132);     //Full mode

    ser_putch(136);     //Demo mode
    ser_putch(4);       //Demo 4 - Figure 8
    
    __delay_ms(10000);  //Waits 10 seconds before requesting the distance traveled
    ser_putch(142);     //Requests packet of sensor data
    ser_putch(19);      //Specifies distance packet for request
    
    ser_getch();        //Gets the distance value returned as 2 bytes, high byte first.
    highByte = rxbyte;  //Puts the high byte into variable
    ser_getch();        //Gets the low byte of the sensor packet
    lowByte = rxbyte;   //Puts the low byte into variable
    
    distTrav = (4*highByte + lowByte);  //Converts 2 byte value into a single binary
    
    lcdWriteToDigitBCD(distTrav); //Writes received packet value to LCD in millimeters.
    
    
   while(1){       
        if(FLAG_1000MS){
            RB0 = !RB0;
            FLAG_1000MS = 0;
        }
        
        //Rotates 360 and checks at each half step to see if the object the sensor
        //is looking at is closer than the previous closest object.
        if (PB8Counter >= 10 && PB8 == 0){
            for (x=0; x==400; x++){
                moveCW();
                ADCMain();
                if (adcRAW < adcClosest){
                    adcClosest = adcRAW;
                    stepClosest = stepCount;
                }
                
            }
            //Moves CCW until stepCount(initialy -400) matches the step of the closest object
            for (x=stepCount; x=stepClosest; x++){
                moveCCW();
            }
            
        }
        
        
        
        
        
    }
}
