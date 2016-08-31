
#include "main.h"
#include "LCD.h"
#include "ser.h"
#include "SPI.h"
#include "ADC.h"
#include "motor.h"

volatile unsigned int time_count;
volatile bit FLAG_1000MS;
signed int highByte = 0;
signed int lowByte = 0;
signed int dist = 0;
unsigned int x = 0;

unsigned char PB8Counter = 0;

unsigned char rxbyte = 0;
signed int stepClosest = 0;
signed int adcClosest = 1000;
signed int highByte = 0;
signed int lowByte = 0;
signed int distTrav = 0;
unsigned char controlByte = 0;
signed int x = 0;


unsigned char PB7Counter = 0;
unsigned char PB8Counter = 0;




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
        if (PB7 == 1)
            PB7Counter++;
        
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
    
    //This is just the testing bit, it wont be in the final code
    __delay_ms(10000);  //Waits 10 seconds before requesting the distance traveled
    ser_putch(142);     //Requests packet of sensor data
    ser_putch(19);      //Specifies distance packet for request
    
    ser_getch();        //Gets the distance value returned as 2 bytes, high byte first.
    highByte = rxbyte;  //Puts the high byte into variable
    ser_getch();        //Gets the low byte of the sensor packet
    lowByte = rxbyte;   //Puts the low byte into variable
    
    distTrav = (256*highByte + lowByte);  //Converts 2 byte value into a single binary
    
    lcdWriteToDigitBCD(distTrav); //Writes received packet value to LCD in millimeters.
    
    
   while(1){       
        if(FLAG_1000MS){
            RB0 = !RB0;
            FLAG_1000MS = 0;
        }
        
        //Rotates 360 and compares adcRAW at every half step.
        //If it detects a closer object than the previous closest then it stores 
        //the stepCount corresponding to that object.
        if (PB8Counter >= 10 && PB8 == 0){
            for (x = 0; x<400; x++){           
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

       
        //This might make it drive 4m forward, 250mm/s and takes 16 seconds. 
        //Or it might not.
        if (PB7Counter >= 10 && PB7 = 0){
            ser_putch(137);     //Drive command [Velocity high][Velocity low][Radius high][Radius low]
                ser_putch(0x00);    //Velocity high byte in mm/s
                ser_putch(0b11111010);    //Velocity low byte in mm/s
                ser_putch(0xFF);  //Radius high byte, higher value means straighter path
                ser_putch(0xFF);  //Radius low byte, negative value means turn right
                
            __delay_ms(16000);    //Delay long enough to move 4m
            
            ser_putch(137);     //Drive command [Velocity high][Velocity low][Radius high][Radius low]
                ser_putch(0x00);    //Set velocity to 0 mm/s to stop.
                ser_putch(0x00);    
                ser_putch(0xFF);    
                ser_putch(0xFF);    
            
        }


    }
}
