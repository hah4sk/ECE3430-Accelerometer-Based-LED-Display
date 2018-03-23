#ifndef USCI_SPI_H
#define USCI_SPI_H

#include <msp430.h>

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * USCI A0 Master-In-Slave-Out (MISO) 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * GPIO      :  P1.1
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
#define USCIA0_MISO_BIT					BIT1
#define USCIA0_MISO_PORT				P1IN
#define USCIA0_MISO_DDR					P1DIR
#define SET_USCIA0_MISO_AS_AN_INPUT		USCIA0_MISO_DDR &= ~USCIA0_MISO_BIT
#define SET_MISO						USCIA0_MISO_PORT |= USCIA0_MISO_BIT
#define RESET_MISO						USCIA0_MISO_PORT &= ~USCIA0_MISO_BIT

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * USCI A0 Master-Out-Slave-In (MOSI) 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * GPIO      :  P1.2
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
#define USCIA0_MOSI_BIT					BIT7
#define USCIA0_MOSI_PORT				P1OUT
#define USCIA0_MOSI_DDR 				P1DIR
#define SET_USCIA0_MOSI_AS_AN_OUTPUT	USCIA0_MOSI_DDR |= USCIA0_MOSI_BIT
#define SET_MOSI						USCIA0_MOSI_PORT |= USCIA0_MOSI_BIT
#define RESET_MOSI						USCIA0_MOSI_PORT &= ~USCIA0_MOSI_BIT
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Slave Clock for GPIO Flash Memory Board
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * GPIO      :  P1.4
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
#define SPI_SCK_BIT					BIT5
#define SPI_SCK_PORT				P1OUT // SPI Slave Clock output
#define SPI_SCK_DDR					P1DIR // SPI Slave Clock direction
#define SET_SPI_SCK_AS_AN_OUTPUT	SPI_SCK_DDR |= SPI_SCK_BIT
#define TURN_OFF_SCLK				SPI_SCK_PORT &= ~SPI_SCK_BIT
#define TOGGLE_SCLK					SPI_SCK_PORT ^= SPI_SCK_BIT

/*
 * This function initializes all hardware and port pins to support SPI.
 */

#define LATCH_BIT					BIT0
#define LATCH_PORT					P2OUT
#define LATCH_DDR					P2DIR
#define SET_LATCH_AS_AN_OUTPUT		LATCH_DDR |= LATCH_BIT
#define SET_LATCH					LATCH_PORT |= LATCH_BIT
#define DISABLE_LATCH				LATCH_PORT &= ~LATCH_BIT


#define BLANK_BIT					BIT4
#define BLANK_PORT					P1OUT
#define BLANK_DDR					P1DIR
#define SET_BLANK_AS_AN_OUTPUT		BLANK_DDR |= BLANK_BIT
#define ENABLE_LEDS					BLANK_PORT &= ~BLANK_BIT
#define DISABLE_LEDS				BLANK_PORT |= BLANK_BIT

void InitializeSPI();

/*
 * This function sends the byte, SendValue, using SPI.
 */
void SPISendByte(unsigned char SendValue);

/*
 * This function receives a byte using SPI.
 *
 * Return Value: The byte that is received over SPI.
 */
unsigned char SPIReceiveByte();

#endif
