#include <msp430.h>

//additional includes
#include "timerA.h"
#include "ADC.h"
#include "spi.h"
#include "LED.h"
#include "pushbutton.h"
#include "Accelerometer.h"
#include "debounce.h"
#include <stdlib.h>

//global variables
unsigned int calibrationCount = 0;
unsigned int g1msTimeout = 0;
unsigned int g1msTimer = 0;
unsigned int LEDRefreshTimer = 0;
//array sizes
#define NUMBER_OF_CURRENT_SAMPLES 3
#define BUFFER_SIZE 8

//max/min/offset values for calibration
int xMAX=0;
int yMAX=0;
int zMAX=0;

int xMIN=0;
int yMIN=0;
int zMIN=0;

signed int xOFFSET=0;
signed int yOFFSET=0;
signed int zOFFSET=0;


//arrays for most recent samples
unsigned int currentSamples[NUMBER_OF_CURRENT_SAMPLES];
//individual arrays for each coordinate
int xBuffer[BUFFER_SIZE];
int yBuffer[BUFFER_SIZE];
int zBuffer[BUFFER_SIZE];

//various values for accelerometer
int CurrentSample;
unsigned int CurrentSampleIndex;
int xCumulativeSum;
int yCumulativeSum;
int zCumulativeSum;

signed int xAverage;
signed int yAverage;
signed int zAverage;

//LED struct for LED info
LEDStruct* info;
//calibration button
SwitchDefine* calibrationPushButton; // Used to calibrate ADC

// Local prototypes
void ConfigureClockModule();
void InitializeSampleBuffer(void);
void ManageSoftwareTimers(void);

int main(void)

 {
	// Stop the watchdog timer, and configure the clock module.
	WDTCTL = WDTPW + WDTHOLD;
	ConfigureClockModule();

	InitializePushButtonPortPin();
	ConfigureTimerA();
	ConfigureADC();
	info = (LEDStruct*)malloc(sizeof(LEDStruct));
	InitializeLEDInfo(info);
	// Initialize the array used to store the ADC samples, along with the variables
	// used to compute the average in filter().
	InitializeSampleBuffer();
	InitializeSPI();



	//initialize button
	calibrationPushButton = (SwitchDefine*)malloc(sizeof(SwitchDefine));
	InitializeSwitch(calibrationPushButton,(char *) &PUSHBUTTON_PORT_IN,(unsigned char) PUSHBUTTON_BIT,
	HIGH_THRESHOLD,LOW_THRESHOLD);

	info->LedStatus = 0x80; //lights up Xmax for x calibration
	SetLEDDisplay(info);


	_enable_interrupts();
	while(calibrationCount < 6){ //waits for calibration to finish
		ManageSoftwareTimers();
		getAccelerometerValues();
		Calibrate(info);
		SetLEDDisplay(info);
	}
		info->LedStatus = 0x00;
		SetLEDDisplay(info);
		info->LedStatus = 0x00; //yeah i have no fucking idea why but for some
								//reason if i don't have this before the 'while' loop
								//then the SetLEDDisplay(info) call right above this displays some random
								//shit so whatever #junglebeatshollaatme


		free(calibrationPushButton);
	 //Infinite loop
	while (1) {
		//ManageSoftwareTimers();
		getAccelerometerValues();
		determineTilt(info); //gets theta and phi to change the values of LEDState and LEDIntensity
		SetLEDDisplay(info);
		//timerA decrements everything in "Onticks"
		//we want timerA to be happening quicker than SetLEDDisplay
	}
}

void InitializeSampleBuffer(void)
{
	//sets everything to zero
	xAverage = 0;
	yAverage = 0;
	zAverage = 0;
	xCumulativeSum = 0;
	yCumulativeSum = 0;
	zCumulativeSum = 0;
	CurrentSampleIndex = 0;

	//set all values in array to 0
	int i;
	for(i = 0; i < NUMBER_OF_CURRENT_SAMPLES; i++){
		currentSamples[i] = 0;
	}
	int j;
	for(j = 0; j < BUFFER_SIZE; j++){
		xBuffer[j] = 0;
		yBuffer[j] = 0;
		zBuffer[j] = 0;
	}

	ADC10SA = (unsigned int)currentSamples; //sets address to automatically place values in
}

void ConfigureClockModule(void)
{
    // Configure Digitally Controlled Oscillator (DCO) for 1 MHz using factory
    // calibrations.
	DCOCTL  = CALDCO_16MHZ;
	BCSCTL1 = CALBC1_16MHZ;
}

void ManageSoftwareTimers(void)
{
	if (g1msTimeout) {
		g1msTimeout--;
		g1msTimer++;
		LEDRefreshTimer++;
	}

}
