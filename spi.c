#include "spi.h"
#include "LED.h"


void InitializeSPI()
{
	//RESET_MISO;
	//SET_USCIA0_MISO_AS_AN_INPUT;

	RESET_MOSI;
	SET_USCIA0_MOSI_AS_AN_OUTPUT;

	TURN_OFF_SCLK;
	SET_SPI_SCK_AS_AN_OUTPUT;

	DISABLE_LATCH;
	SET_LATCH_AS_AN_OUTPUT;

	DISABLE_LEDS;
	SET_BLANK_AS_AN_OUTPUT;

}

void SPISendByte(unsigned char SendValue)
{
	unsigned char tmp = SendValue;

	unsigned char mask = 0x80; //mask = 1000 0000 in binary

	int i;
	for (i = 0; i < 8; i++){

	if ((tmp & mask) > 0){
		SET_MOSI;
	}
	else{
		RESET_MOSI;
	}
	mask /= 2;

	//toggle sclk twice
	TOGGLE_SCLK;
	TOGGLE_SCLK;
}

}

unsigned char SPIReceiveByte()
{
	unsigned char receiveValue = 0;
	unsigned int i;

	for (i=0; i < 8; i++){

		receiveValue <<=1;  //shift current value of receiveValue by 1
		receiveValue |= ((BIT1 & P1IN)>>1);


		TOGGLE_SCLK;
		TOGGLE_SCLK;
	}

	return receiveValue;
}


