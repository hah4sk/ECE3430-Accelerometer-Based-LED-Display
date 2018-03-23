#include <msp430.h>
#include "Accelerometer.h"
#include "LED.h"
#include "cordic.h"
#include "debounce.h"
#include "pushbutton.h"
#include <stdlib.h>
#include "timerA.h"

//array sizes
#define NUMBER_OF_CURRENT_SAMPLES 3
#define BUFFER_SIZE 8
#define CORDIC_GAIN 0.607252935103139

//arrays for most recent samples
extern volatile int currentSamples[NUMBER_OF_CURRENT_SAMPLES];
//individual arrays for each coordinate
extern volatile int xBuffer[BUFFER_SIZE];
extern volatile int yBuffer[BUFFER_SIZE];
extern volatile int zBuffer[BUFFER_SIZE];

extern int CurrentSample;
extern unsigned int CurrentSampleIndex;
extern int xCumulativeSum;
extern int yCumulativeSum;
extern int zCumulativeSum;
extern signed volatile int xAverage;
extern signed volatile int yAverage;
extern signed volatile int zAverage;

extern int xMAX;
extern int yMAX;
extern int zMAX;

extern volatile int xMIN;
extern volatile int yMIN;
extern volatile int zMIN;

extern signed int xOFFSET;
extern signed int yOFFSET;
extern signed int zOFFSET;

//values from 'main'
extern unsigned int calibrationCount;
extern unsigned int g1msTimeout;
extern unsigned int g1msTimer;

extern LEDStruct* info;
extern SwitchDefine* calibrationPushButton;

void getAccelerometerValues(){

	ADC10CTL0 |=  (ENC | ADC10SC);	// Enable and begin sampling and conversion


	while(ADC10CTL1 & UCBUSY); //wait
	while(ADC10CTL1 & UCBUSY); //wait
	while(ADC10CTL1 & UCBUSY); //wait

	filter();
	ADC10CTL0 &= ~(ENC| ADC10SC); //disable and end sampling and conversion

}


void filter(){

	xCumulativeSum += currentSamples[2] - xBuffer[CurrentSampleIndex];
	xBuffer[CurrentSampleIndex] = currentSamples[2];
	yCumulativeSum += currentSamples[1] - yBuffer[CurrentSampleIndex];
	yBuffer[CurrentSampleIndex] = currentSamples[1];
	zCumulativeSum += currentSamples[0] - zBuffer[CurrentSampleIndex];
	zBuffer[CurrentSampleIndex] = currentSamples[0];

	CurrentSampleIndex++;
	CurrentSampleIndex &= 7; //mod 8

	xAverage = xCumulativeSum >> 3; //divide by 8
	yAverage = yCumulativeSum >> 3; //divide by 8
	zAverage = zCumulativeSum >> 3; //divide by 8

}

void Calibrate(LEDStruct *info){


	//keep checking for pushbutton until calibration complete
		if(Debouncer(calibrationPushButton) == High){
			switch(calibrationCount){
				case 0: //first step determine offsets
					xMAX = xAverage;
					info->LedStatus = 0x08; //lights up LED to indicate Xmin
					calibrationPushButton->CurrentState = DbExpectHigh;
					while(Debouncer(calibrationPushButton) == High);
					break;
				case 1: //next determine x max/min
					xMIN = xAverage;
					xOFFSET = (xMAX+xMIN)/2; //can calc offset now that max and min are known
					info->LedStatus = 0x02; //lights up LED to indicate Ymax
					calibrationPushButton->CurrentState = DbExpectHigh;
					while(Debouncer(calibrationPushButton) == High);
					break;
				case 2:
					yMAX = yAverage;
					calibrationPushButton->CurrentState = DbExpectHigh;
					info->LedStatus = 0x20; //lights up LED to indicate Ymin
					while(Debouncer(calibrationPushButton) == High);
					break;
				case 3: //next determine y max/min
					yMIN = yAverage;
					yOFFSET = (yMAX+yMIN)/2;
					info->LedStatus = 0xAA; //lights up plus-sign LED Display to indicate z-axis calibration
					calibrationPushButton->CurrentState = DbExpectHigh;
					while(Debouncer(calibrationPushButton) == High);
					break;
				case 4:
					zMAX = zAverage;
					info->LedStatus = 0x55; //lights up cross-sign LED Display to indicate z-axis calibration
					calibrationPushButton->CurrentState = DbExpectHigh;
					while(Debouncer(calibrationPushButton) == High);
					break;
				case 5: //next determine z max/min
					zMIN = zAverage;
					zOFFSET = (zMAX+zMIN)/2;
					info->LedStatus = 0x00; //turns off all LEDs before interacting with accelerometer
					//calibrationPushButton->CurrentState = DbExpectHigh;
					//while(Debouncer(calibrationPushButton) == High);
					break;
				default:
					break;
					}
				calibrationCount++;
		}
	}




void determineTilt(LEDStruct *info){

	calculations calcs;
	volatile long theta; //between x and y
	volatile float phi;
	volatile float hypotenuse;
	theta = 0;
	phi=0;

	volatile int xCoordinate = xAverage - xOFFSET;
	volatile int yCoordinate = yAverage - yOFFSET;
	volatile int zCoordinate = zAverage - zOFFSET;

	int xCopy = xCoordinate << 8;
	int yCopy = yCoordinate << 8;
	int zCopy = zCoordinate << 8;

	calcs.x = xCoordinate << 8;
	calcs.y = yCoordinate << 8;
	calcs.angle = 0;

	if(calcs.x < 0){
			calcs.x = -calcs.x;
		}

	Cordic(&calcs, ATAN_HYP);
	//theta in calcs.angle and hypotenuse in calcs.x
	theta = calcs.angle;
	theta = theta >> 8;


	//Now, calculate Phi
//	calcs.y = zCopy;
//	hypotenuse = calcs.x*CORDIC_GAIN/256;
//	calcs.x = hypotenuse * 256;
//
//	Cordic(&calcs, ATAN_HYP);
//	//theta in calcs.angle and hypotenuse in calcs.x
//	phi = calcs.angle;
//	phi = phi /256;


	//correct if Q2 or Q3
	if(xCopy < 0 && yCopy > 0){
		theta  = 180-theta;
	}
	else if(xCopy < 0 && yCopy < 0){
		theta = theta-180;
	}
	_nop();




			/*DETERMINE CENTER LED BASED ON THETA*/

		if (theta < 22.5 && theta > -22.5)
			{
				info->LEDState=East;
//				info->LEDState = West;
			}
			else if (theta > 22.5 && theta < 67.5){
			    info->LEDState=Northeast;
//				info->LEDState=Southwest;
			}
			else if (theta > 67.5 && theta < 112.5){
			    info->LEDState=North;
//				info->LEDState=South;
			}
			else if (theta > 112.5 && theta < 157.5){
			    info->LEDState=Northwest;
//				info->LEDState=Southeast;
			}
			else if (theta > 157.5 && theta < 202.5){
			    info->LEDState=West;
//				info->LEDState=East;
			}
			else if (theta > 202.5 && theta < 247.5){ //never happens
			    info->LEDState=Southwest;
//				info->LEDState=Northeast;
			}
			else if (theta > 247.5 || theta < -67.5){
			    info->LEDState=South;
//				info->LEDState=North;
			    if(xCoordinate < 0){
			    	info->LEDState=Southwest;
			    }
			}
			else if (theta < 0 && theta > -67.5){
			    info->LEDState=Southeast;
//				info->LEDState=Northwest;
			}

		if(abs(xCoordinate) < 10 && abs(yCoordinate) < 10){
			info->LEDState=swag; //level
		}


//	/*DETERMINE INTENSITY WITH PHI*/
//		info->Intensity = abs( (phi/90) * BRIGHTEST );

		float absZCoordinate = abs(zCoordinate);

		if(absZCoordinate <= 10){
			info->Intensity = B1;
		}
		else if(absZCoordinate > 10 && absZCoordinate <= 20){
			info->Intensity = B2;
		}
		else if(absZCoordinate > 20 && absZCoordinate <= 30){
			info->Intensity = B3;
		}
		else if(absZCoordinate > 30 && absZCoordinate <= 40){
					info->Intensity = B4;
		}
		else if(absZCoordinate > 40 && absZCoordinate <= 50){
					info->Intensity = B5;
		}
		else if(absZCoordinate > 50){
					info->Intensity = B6;
		}
}


