#include "pushbutton.h"
#include "LED.h"
#include "debounce.h"
#include "Accelerometer.h"

extern int xAverage;
extern int yAverage;
extern int zAverage;

extern int xMAX;
extern int yMAX;
extern int zMAX;

extern int xMIN;
extern int yMIN;
extern int zMIN;

extern signed int xOFFSET;
extern signed int yOFFSET;
extern signed int zOFFSET;

extern LEDStruct* info;
extern SwitchDefine* calibrationPushButton;
extern unsigned int calibrationCount;

#define BUTTON						BIT3
#define ENABLE_P1_INTERRUPT 		P1IE |= BUTTON
#define P1_RESISTOR_ENABLE			P1REN |= BUTTON
void InitializePushButtonPortPin (void)
{
	// Configure port pin for pushbutton
//	ENABLE_P1_INTERRUPT;
//	P1_RESISTOR_ENABLE;
//	SET_PUSHBUTTON_TO_AN_INPUT;
	ENABLE_PULL_UP_PULL_DOWN_RESISTORS;
	SELECT_PULL_UP_RESISTORS;
	SET_PUSHBUTTON_TO_AN_INPUT;
}

#pragma vector = PORT1_VECTOR
// Button interrupt service routine, add code here for POST LAB
__interrupt void Button_routine(void)
{

	P1IFG &= ~PUSHBUTTON_BIT; //CLEAR INTERRUPT FLAG
}
