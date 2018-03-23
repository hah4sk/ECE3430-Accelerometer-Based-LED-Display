#ifndef DEBOUNCE_H_
#define DEBOUNCE_H_

#include <msp430.h>

// Definitions for debounce Times for the P1.3 button (ms)
#define HIGH_THRESHOLD 10		//Definitions for debounce Times for the P1.3 button (ms)
#define LOW_THRESHOLD 20

typedef enum {
	DbExpectHigh, DbValidateHigh, DbExpectLow, DbValidateLow
} DbState;

typedef enum {
	Low, High
} SwitchStatus;

typedef struct {
	DbState CurrentState;			// Current state of the FSM
	char * SwitchPort;				// Input port associated with switch
	unsigned char SwitchPortBit;	// Port pin associated with switch
	unsigned char HoldTime;			// Time switch must remain On
	unsigned char ReleaseTime;		// Time switch must remain Off after begin On
	unsigned int EventTime;			// Records time of an event - either initially On or Off
} SwitchDefine;


// Function Prototypes
void InitializeSwitch(SwitchDefine *Switch,char *SwitchPort,unsigned char SwitchBit,
		unsigned char HoldTime,unsigned char ReleaseTime);

//This function returns the instantaneous value of the selected switch
SwitchStatus GetSwitch(SwitchDefine *Switch);

//This function debounces a switch input
SwitchStatus Debouncer(SwitchDefine *Switch);

#endif
