#ifndef ACC_H
#define ACC_H
#include <msp430.h>
#include "LED.h"

//prototypes
void getAccelerometerValues(void);
void filter(void);
void determineTilt(LEDStruct *info);
void Calibrate(LEDStruct *info);

#endif
