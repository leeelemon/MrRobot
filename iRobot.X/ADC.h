

#ifndef ADC_H
#define	ADC_H

volatile unsigned int adcRAW = 0;
volatile unsigned int Distance = 0;

void ADCMain(void);
void setupADC(void);
unsigned int getADC(void);
unsigned int adcConvertMM(unsigned int);

#endif	/* ADC_H */



