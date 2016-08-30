
#include "main.h"
#include "ADC.h"
#include "LCD.h"


//Setup of ADC module ports and registers
void setupADC(void){
    PORTA = 0;              //Clears portA pins
    TRISA = 0b11111111;     //Set Port A to input

    //Set control registers
    ADCON0 = 0b10001001;	//Fosc/32, Channel 1, ADC on
    ADCON1 = 0b00000010;	//left justified, PortE Digital, PortA Analogue
    
    __delay_us(50);         // Delay for ADC aquisition
}

void ADCMain(void){
               
        getADC();                   //Get rawADC value from sensor
        lcdSetCursor(0b10000000);   //First line, first position
        lcdWriteToDigitBCD(adcRAW); //Write the raw ADC value
      
//        adcConvertMM(adcRAW);       //Use Liam's conversion and sensor
}

//get an adc result from the selected channel
unsigned int getADC(void){

    ADRESH	= 0; 	 	// Reset the ADRES value register
    ADRESL = 0;

    GO = 1;                 // Staring the ADC process
    while(GO) continue;     // Wait for conversion complete

    //Calculates the 10bit ADC output
    
    adcRAW = (4*ADRESH);    //Converts ADRESH to the top 8 bits of the 10bit value
    //Adds 1, 2 or 3 to the total depending on the last 2 bits of the 10bit value (ADRESL)
    switch(ADRESL){         
        case 0b01000000: adcRAW = adcRAW + 1;
        break;
        case 0b10000000: adcRAW = adcRAW + 2;
        break;
        case 0b11000000: adcRAW = adcRAW + 3;
        break;
        default: adcRAW = adcRAW;
    }

    return(adcRAW); // Return the value of the ADC process
}
    

//Converts 10bit value to a distance (mm) using the following equation.
//Interpolates between 10cm intervals based on the 10 bit ADC value.
//(adcMAX-adcRAW)/(adcMAX - adcMIN)]*(DistMax - DistMin) + DistMin
/*
unsigned int adcConvertMM(unsigned int adcRAW){

    if (adcRAW < 517 && adcRAW >= 513)                          //1: 10-20
            Distance = (((51700 - adcRAW*100)/4) + 100);
            
    if (adcRAW < 513 && adcRAW >= 393)                          //2: 20-30
            Distance = (((51300 - adcRAW*100)/120) + 200);
         
    if (adcRAW < 393 && adcRAW >= 298)                          //3: 30-40
            Distance = (((39300 - adcRAW*100)/95) + 300);
            
    if (adcRAW < 298 && adcRAW >= 236)                          //4: 40-50
            Distance = (((29800 - adcRAW*100)/62) + 400);
         
    if (adcRAW < 236 && adcRAW >= 194)                          //5: 50-60
            Distance = (((23600 - adcRAW*100)/42) + 500);
           
    if (adcRAW < 194 && adcRAW >= 168)                          //6: 60-70
            Distance = (((19400 - adcRAW*100)/26) + 600);
         
    if (adcRAW < 168 && adcRAW >= 144)                          //7: 70-80
            Distance = (((16800 - adcRAW*100)/24) + 700);
           
    if (adcRAW < 144 && adcRAW >= 129)                          //8: 80-90
            Distance = (((14400 - adcRAW*100)/15) + 800);
          
    if (adcRAW < 129 && adcRAW >= 110)                          //9: 90-100
            Distance = (((12900 - adcRAW*100)/19) + 900 + 50);  //Adds a constant to minimise errors (discussed in report)
           
    if (adcRAW < 110 && adcRAW >= 98)                           //10: 100-110
            Distance = (((11000 - adcRAW*100)/12) + 1000 + 100);//Adds a constant to minimise errors (discussed in report)
            
    if (adcRAW < 98 && adcRAW >= 90)                            //11: 110-120
            Distance = (((9800 - adcRAW*100)/8) + 1100 + 150);  //Adds a constant to minimise errors (discussed in report)
    
    if(adcRAW < 90 || adcRAW >= 517) //Anything outside the sensors reliable range defaults to 0
        Distance = 0;
            
    return(Distance); //Returns the converted distance to be written to LCD    
}
        
*/         
        
        
            
   
    
   
    
    
  


