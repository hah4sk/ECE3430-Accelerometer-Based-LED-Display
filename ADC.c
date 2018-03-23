#include "ADC.h"

//array sizes
#define NUMBER_OF_CURRENT_SAMPLES 3
#define BUFFER_SIZE 8

//arrays for most recent samples
extern volatile int currentSamples[NUMBER_OF_CURRENT_SAMPLES];
//individual arrays for each coordinate
extern volatile int xBuffer[BUFFER_SIZE];
extern volatile int yBuffer[BUFFER_SIZE];
extern volatile int zBuffer[BUFFER_SIZE];

extern volatile int CurrentSample;
extern volatile unsigned int CurrentSampleIndex;
extern volatile int xCumulativeSum;
extern volatile int yCumulativeSum;
extern volatile int zCumulativeSum;
extern volatile int xAverage;
extern volatile int yAverage;
extern volatile int zAverage;

void ConfigureADC(void)
{
	ADC10CTL0 &= ~(ENC| ADC10SC);
	/* ADC10 Control register 1
	 * INCH_0 = choose A0 as the analog input
	 * ADC10DIV_3 = divide ADC clock by 4 (for temperature sensor)
	 */
	ADC10CTL1 = (INCH_2 | ADC10DIV_3 | CONSEQ_1);



	/* ADC10 Control register 0
	 * SREF_1: Choose 1.5V reference for temperature sensor
	 * ADC10SHT_3 = 64 ADC10CLKs (for temperature sensor)
	 */

	ADC10DTC0 = 0;
	ADC10DTC0 |= ADC10CT;


	ADC10DTC0 &= ~ADC10TB; //one block mode
	ADC10DTC1 = 0x03;



	ADC10AE0 |= BIT0 | BIT1 | BIT2;
	ADC10CTL0 = (SREF_0 | ADC10SHT_3 | REFON | ADC10ON | MSC);

	_delay_cycles(1000);			// Wait for the ADC to adjust and stabilize
}
