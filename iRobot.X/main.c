
#include "main.h"
#include "LCD.h"
#include "ser.h"
#include "SPI.h"
#include "ADC.h"
#include "motor.h"

volatile unsigned int time_count;
volatile bit FLAG_1000MS;


unsigned char controlByte = 0;
signed int loop = 0;

unsigned char PB6Counter = 0;
unsigned char PB7Counter = 0;
unsigned char PB8Counter = 0;

char count = 0;




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
            RB4 = !RB4;     //Toggle LED           
            //    FLAG_1000MS = 1;	// Raise flag for 500ms
            time_count = 0;	// Clear time_count        
        }
        
        if (PB8 == 1)
            PB8Counter++;
        if (PB7 == 1)
            PB7Counter++;
        if (PB6 == 1)
            PB6Counter++;
        
  /*     if(getSensorData(18,1) == 0b00000001)
            FLAG_Play++;
        
        if(getSensorData(18,1) == 0b00000100)
            FLAG_Advanced++;
        
        if(getSensorData(18,1) == 0b00000101)
            FLAG_AdvPlay++;
            
   */    
        
    
                
        
    }
}


void main(void){
__delay_ms(5000);

//Initialise and setup
    setupSPI();
    ser_init();
    setupLCD();
    setupADC();
    
    unsigned char controlByte = 0b00001101;
    spi_transfer(controlByte);
   
    __delay_ms(1000);
    ser_putch(128);     //Startup
    __delay_ms(1000);
    ser_putch(132);     //Full mode
    __delay_ms(1000);
    lcdWriteToDigitBCD(totalDistTrav);
    
   
    while(1){       
        
        //Rotates 360 and compares adcRAW at every half step.
        //If it detects a closer object than the previous closest then it stores 
        //the stepCount corresponding to that object.
        if (PB8Counter >= 10 && PB8 == 0){
            //Loops motor back to stepCount 0
            while (stepCount < 0){
                moveCCW();
            }
            
            
            adcClosest = 0;
            for (loop = 0; loop < 400; loop++){           
                moveCW();
                ADCMain();
                if (adcRAW > adcClosest){
                    adcClosest = adcRAW;
                    stepClosest = stepCount;              
                }
            }           
            //Moves CCW until stepCount(initial -400) matches the step of the closest object
            for (loop = stepCount; loop != stepClosest; loop++){
                moveCCW();
            } 
            PB8Counter = 0;
        }

       
        //Drive forward 4m straight line

        if (getSensorData(18,1) == 0b00000001){ //Play button
            totalDistTrav = 0; 
            DriveDirect(200,200); //Drive, 250mm/s | 250mm/s
            while (totalDistTrav < 2){
                distTrav = getSensorData(19,2);   //Distance packetID, 2 bytes expected
                totalDistTrav = (totalDistTrav + distTrav);
                                        
                lcdSetCursor(0x00);     //Print distance on first row first position
                lcdWriteToDigitBCD(totalDistTrav);  
            }            
                            
            DriveDirect(0,0);   //Drive, 0mm/s, straight (STOP)
           
        }
        
        //Perform 'Square' manoeuvre
        if (getSensorData(18,1) == 0b00000100){ //Advanced button
            totalDistTrav = 0;  //Resets distance traveled
            
            for (loop = 0; loop < 4; loop++){   //Loop 4 times
                
                //Turn 90 degrees
                Drive(0,250,0xFF,0xFF);     //Drive, 250mm/s, turn on spot right 
                while (angleTurned > -90){
                    angleTurned = getSensorData(20,2);  //Angle packetID, 2 bytes expected
                }  
                
                //Drive 1m
                Drive(0,250,0x7F,0xFF);     //Drive, 250mm/s, straight
                while (totalDistTrav < 1000){                        
                    distTrav = getSensorData(19,2);   //Distance packetID, 2 bytes expected
                    totalDistTrav = (totalDistTrav + distTrav);
                                        
                    lcdSetCursor(0x00);     //Print distance on first row first position
                    lcdWriteToDigitBCD(totalDistTrav);                  
                }
                totalDistTrav = 0;
            }
            
            DriveDirect(0,0); //Drive, 0mm/s, straight (STOP) 
               
        }
        
        //Do something when both buttons are pushed (wall follow)
        if(getSensorData(18,1) == 0b00000101){  //Play and Advanced at the same time
            
            
            
            
        }
        
    }
}
