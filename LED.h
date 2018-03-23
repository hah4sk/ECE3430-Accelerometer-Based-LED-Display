#ifndef LED_H
#define LED_H

#include <msp430.h>

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *                 LED #1
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *   Color     :  Red
 *   Polarity  :  Active High
 *   GPIO      :  P1.0
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
#define LED1 1
#define LED1_BIT				BIT0
#define LED1_PORT				P1OUT
#define LED1_DDR				P1DIR
#define SET_LED1_AS_AN_OUTPUT	LED1_DDR |= LED1_BIT
#define TURN_ON_LED1			LED1_PORT |= LED1_BIT
#define TURN_OFF_LED1			LED1_PORT &= ~LED1_BIT
#define TOGGLE_LED1				LED1_PORT ^= LED1_BIT

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *                 LED #2
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *   Color     :  Green
 *   Polarity  :  Active High
 *   GPIO      :  P1.6
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
#define LED2 2
#define LED2_BIT				BIT6
#define LED2_PORT				P1OUT
#define LED2_DDR				P1DIR
#define SET_LED2_AS_AN_OUTPUT	LED2_DDR |= LED2_BIT
#define TURN_ON_LED2			LED2_PORT |= LED2_BIT
#define TURN_OFF_LED2			LED2_PORT &= ~LED2_BIT
#define TOGGLE_LED2				LED2_PORT ^= LED2_BIT


/* LED Names */
#define N_LED   BIT7
#define NE_LED  BIT6
#define E_LED   BIT5
#define SE_LED  BIT4
#define S_LED   BIT3
#define SW_LED  BIT2
#define W_LED   BIT1
#define NW_LED  BIT0

#define NORTH	0x8F
#define SOUTH	0xF8
#define WEST	0x3E
#define EAST	0xE3
#define	NORTHWEST	0x1F
#define SOUTHEAST	0xF1
#define	NORTHEAST	0xC7
#define SOUTHWEST	0x7C
#define LEVEL		0xFF
//#define NORTH	0x70
//#define SOUTH	0x07
//#define WEST	0xC1
//#define EAST	0x1C
//#define	NORTHWEST	0xE0
//#define SOUTHEAST	0x0E
//#define	NORTHEAST	0x38
//#define SOUTHWEST	0x83

/* LED Brightnesses */
#define B1	400
#define B2	330
#define B3	260
#define B4	190
#define B5	120
#define B6	50

typedef enum LEDStates {
North, Northwest, West, Southwest, South, Southeast, East, Northeast, swag, Uncalibrated } //swag means "completely flat"
LEDState;

typedef struct LED_struct {
	unsigned char LedStatus;  //Good for mask, can send through SPI to turn on LEDs
	unsigned int Period;		//a recommended value is 50ms
	unsigned int Onticks[8]; //stores pulse width for each LED
	LEDState LEDState; //identifies which state the program is in
	unsigned int Intensity; //based on value of phi
	unsigned int ticksDone;
}LEDStruct;
// Prototypes
void InitializeLEDInfo(LEDStruct* info);
void SetLEDDisplay(LEDStruct* info);

#endif
