
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
            RB0 = !RB0;     //Toggle LED           
            //    FLAG_1000MS = 1;	// Raise flag for 500ms
            time_count = 0;	// Clear time_count        
        }
        if (PB8 == 1)
            PB8Counter++;
        if (PB7 == 1)
            PB7Counter++;
        if (PB6 == 1)
            PB6Counter++;
        
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
    
    while(1){       
        
        //Rotates 360 and compares adcRAW at every half step.
        //If it detects a closer object than the previous closest then it stores 
        //the stepCount corresponding to that object.
        if (PB8Counter >= 10 && PB8 == 0){
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
        }

       
        //This might make it drive 4m forward, 250mm/s and takes 16 seconds. 
        //Or it might not.
        if (PB7Counter >= 10 && PB7 == 0){
            totalDistTrav = 0;  //Resets distance traveled
            
            for (totalDistTrav = 0; totalDistTrav < 400;){
            Drive(0,250,0x7F,0xFF); //Drive, 250mm/s, straight
            getDistTrav();
            
            }                
            Drive(0,0,0x7F,0xFF);   //Drive, 0mm/s, straight (STOP)             
        }
        
        //Perform 'Square' manoeuvre... maybe
        if (PB6Counter >= 10 && PB6 == 0){
            totalDistTrav = 0;  //Resets distance traveled
            
            for (loop = 0; loop < 4; loop++){
                Drive(0,250,0xFF,0xFF);     //Drive, 250mm/s, turn on spot left 
                __delay_ms(1000);           //Delay some amount of time to turn 90 deg    
                
                for (totalDistTrav = 0; totalDistTrav < 100;){                        
                    Drive(0,250,0x7F,0xFF); //Drive, 250mm/s, straight
                    getDistTrav();          //Gets distance and print to LCD                         
                }
                totalDistTrav = 0;
            }
            
            Drive(0,0,0x7F,0xFF); //Drive, 0mm/s, straight (STOP) 
                
        }


    }
}
