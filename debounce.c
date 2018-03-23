#include "debounce.h"
#include "LED.h"
#include <math.h>

#define FALSE 0
#define TRUE 1

extern unsigned int g1msTimer;

void InitializeSwitch(SwitchDefine *Switch,char *SwitchPort,unsigned char SwitchBit,
		unsigned char HoldTime,unsigned char ReleaseTime)
{
	Switch->CurrentState = DbExpectHigh;
	Switch->SwitchPort = SwitchPort;
	Switch->SwitchPortBit = SwitchBit;

	Switch->HoldTime = HoldTime; // units equal milliseconds
	Switch->ReleaseTime = ReleaseTime; // units equal milliseconds
	Switch->EventTime = 0;
}

SwitchStatus GetSwitch(SwitchDefine *Switch)
{
	volatile unsigned char SwitchValue;

	SwitchValue = *(Switch->SwitchPort) & Switch->SwitchPortBit;
	if(SwitchValue){
		return Low;
	}
	else {
		return High;
	}
}

SwitchStatus Debouncer(SwitchDefine *Switch)
{
	SwitchStatus CurrentSwitchReading = GetSwitch(Switch);
	DbState NextState;

	volatile DbState currentState; //DEBUGGING
	currentState = Switch->CurrentState;

	// First, determine the current inputs.
	//X1 = > or < threshold
	//X0 = button pushed or not (High or Low)

	volatile SwitchStatus x0;
	x0 = CurrentSwitchReading;
	int x1;


	if (g1msTimer < Switch->EventTime){
		x1 = (2^16 -1) + Switch->EventTime - g1msTimer;
	}
	else{
		x1 = g1msTimer - Switch->EventTime; //difference between current time and time the switch went high or low
	}



	// Next, based on the input values and the current state, determine the next state.
	switch (Switch->CurrentState) {
		case DbExpectHigh:
			if (CurrentSwitchReading == High){
				NextState = DbValidateHigh;
				Switch->EventTime = g1msTimer; //capture current time and begin validating
			}
			if (CurrentSwitchReading == Low){
				NextState = DbExpectHigh;
			}
		break;
		case DbValidateHigh:
			if (CurrentSwitchReading == High){
					if (x1 >= Switch->HoldTime){ //if high for long enough
						NextState = DbExpectLow; //wait for release
					}
   		else{
						NextState = DbValidateHigh; //keep waiting
					}
			}
			if (CurrentSwitchReading == Low){
				NextState = DbExpectHigh; //must be mid-bounce, go back to beginning and wait for it to be pressed again
			}
		break;
		case DbExpectLow:
			if (CurrentSwitchReading == High){
				NextState = DbExpectLow;  //Stay, wait for low to happen again
			}
			if (CurrentSwitchReading == Low){
				NextState = DbValidateLow;
				Switch->EventTime = g1msTimer; //capture current time and begin validating
			}
		break;
		case DbValidateLow:
			if (CurrentSwitchReading == High){
				NextState = DbExpectLow;  //must be mid-bounce, go back to expecting another low and wait for it to be released again
			}
			if (CurrentSwitchReading == Low){
				if (x1 >= Switch->ReleaseTime){ //if low for long enough
					NextState = DbExpectHigh; //begin waiting for button to be pressed again
				}
				else {
					NextState = DbValidateLow; //keep waiting
				}
			}
		break;
		default: NextState = DbExpectHigh;
	}

	// Finally, update the current state with the next state.
	Switch->CurrentState = NextState;
	//return Low;
	//return CurrentSwitchReading;															//added this one from next if statement
	if (Switch->CurrentState == DbExpectHigh || Switch->CurrentState == DbValidateHigh || Switch->CurrentState == DbExpectLow){
		//SET_TEST;
		return Low;
	}

	if (Switch->CurrentState == DbExpectLow || Switch->CurrentState == DbValidateLow){
		//RESET_TEST;
		return High;
	}

}


