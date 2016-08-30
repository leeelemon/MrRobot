#include "main.h"

#ifndef MOTOR_H
#define	MOTOR_H

void moveCW(void);
void moveCCW(void);

signed char stepCount = 0;
char negativeFlag = 0;
char currentStep = 0;

//Defines stepping sequence
#define STEP0 0b00111100 
#define STEP1 0b00101110 
#define STEP2 0b00101011 
#define STEP3 0b00100111 
#define STEP4 0b00110101 
#define STEP5 0b00010111 
#define STEP6 0b00011011 
#define STEP7 0b00011110

#endif	/* MOTOR_H */





