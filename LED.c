#include "LED.h"
#include "spi.h"


int mainLED=0;


void InitializeLEDInfo(LEDStruct *info){
	info->LedStatus = 0x0;
	info->Period = 420; //total ticks before reset
	info->ticksDone = 0; //keeps track of ticks
	info->LEDState = Uncalibrated;
	int i;
	for (i=0; i < 8; i++){
		info->Onticks[i] = 0;
	}

}

void SetLEDDisplay(LEDStruct* info){
	/*USE PREVIOUS INFO TO SET LEDStatus*/
			int changed = 0;
			int resetNEBug = 0;
			int resetWBug = 0;
			int resetEBug = 0;

			int i; //in case of 'for' loop for level

			//indices of LEDs of interest
			int nearestN1=0;
			int nearestN2=0;
			int nearN1=0;
			int nearN2=0;

			int mainBrightness = info->Intensity;
			int neighborBrightness = info->Intensity/2;
			int neighborBrightness2 = neighborBrightness/2;

			switch(info->LEDState){
						//adjusts Onticks based on main LED
						case North:
							if(mainLED != 1){
							info->LedStatus = NORTH;
							info->ticksDone = 0;
							mainLED = 1;
							changed = 1;
							}
							break;
						case South:
							if(mainLED != 5){
							info->LedStatus = SOUTH;
							info->ticksDone = 0;
							mainLED = 5;
							changed = 1;
							}
							break;
						case East:
							if(mainLED != 3){
							info->LedStatus = EAST;
							info->ticksDone = 0;
							mainLED = 3;
							changed=1;

							resetEBug = 1; //make sure PWM
							}
							break;
						case West:
							if(mainLED != 7
									){
							info->LedStatus = WEST;
							info->ticksDone = 0;
							mainLED = 7;
							changed=1;

							resetWBug = 1; //make sure PWM
							}
							break;
						case Northwest:
							if(mainLED != 2){
							info->LedStatus = NORTHWEST;
							info->ticksDone = 0;
							mainLED = 2;
							changed=1;
							}
							break;
						case Northeast:
							if(mainLED != 0){
							info->LedStatus = NORTHEAST;
							info->ticksDone = 0;
							mainLED = 0;
							changed=1;

							resetNEBug = 1; //makes sure all 5 LEDs light up
							}
							break;
						case Southwest:
							if(mainLED != 4){
							info->LedStatus = SOUTHWEST;
							info->ticksDone = 0;
							mainLED = 4;
							changed=1;
							}
							break;
						case Southeast:
							if(mainLED != 6){
							info->LedStatus = SOUTHEAST;
							info->ticksDone = 0;
							mainLED = 6;
							changed=1;
							}
							break;
						case swag:
							for(i=0; i < 8;i++){
								info->LedStatus = LEVEL;
								info->Onticks[i] = 50;
							}
							info->ticksDone = 0;
							break;
						case Uncalibrated:
							break;
						default:
							break;
					}

			if(info->LEDState != Uncalibrated){

				if(changed){ //resets neighbors if change in position
				nearestN1 = mainLED-1;
				nearestN2 = mainLED+1;
				nearN1 = nearestN1-1;
				nearN2 = nearestN2+1;

				//cases when main led is placed such that neighbours
				//would have negative indices
				if(mainLED == 1){
					nearestN1 = 0;
					nearN1 = 7;
					nearestN2 = 2;
					nearN2 = 3;
				}
				if(mainLED == 0){
					nearestN1 = 1;
					nearestN2 = 7;
					nearN1 = 6;
					nearN1 = 2;
				}

				//mod all by 8
				nearestN1 &= 0x07;
				nearestN2 &= 0x07;
				nearN1 &= 0x07;
				nearN2 &= 0x07;


				//set corresponding ticks
				info->Onticks[mainLED] = mainBrightness;
				int NearestNeighbors = neighborBrightness;
				int SecondNeighbors = neighborBrightness2;
//				info->Onticks[mainLED] = 200;
//				int NearestNeighbors = 50;
//				int SecondNeighbors = 5;
				info->Onticks[nearestN1] = NearestNeighbors;
				info->Onticks[nearestN2] = NearestNeighbors;
				info->Onticks[nearN1] = SecondNeighbors;
				info->Onticks[nearN2] = SecondNeighbors;


				changed = 0;//waits for next change
				}//end of 'changed' check
			}

			if(resetNEBug){
				info->LedStatus = NORTHEAST;
				int k;
				for(k=0; k<8;k++){
					info->Onticks[k]=0;
				}
//				info->Onticks[0]=200;
//				info->Onticks[1]=50;
//				info->Onticks[2]=5;
//				info->Onticks[7]=50;
//				info->Onticks[6]=5;
				info->Onticks[0] = mainBrightness;
				info->Onticks[1] = neighborBrightness;
				info->Onticks[2] = neighborBrightness2;
				info->Onticks[7] = neighborBrightness;
				info->Onticks[6] = neighborBrightness2;
				resetNEBug = 0; //waits for next bug
			}

			if(resetWBug){
				int l;
				for(l=0; l<8;l++){
					info->Onticks[l]=0;
				}
//				info->Onticks[3]=200;
//				info->Onticks[4]=50;
//				info->Onticks[5]=5;
//				info->Onticks[2]=50;
//				info->Onticks[1]=5;
				info->Onticks[3] = mainBrightness;
				info->Onticks[4] = neighborBrightness;
				info->Onticks[5] = neighborBrightness2;
				info->Onticks[2] = neighborBrightness;
				info->Onticks[1] = neighborBrightness2;
				resetWBug = 0; //wait for next bug
			}

			if(resetEBug){
				int m;
				for(m=0; m<8;m++){
					info->Onticks[m]=0;
				}
//				info->Onticks[7]=200;
//				info->Onticks[6]=50;
//				info->Onticks[5]=5;
//				info->Onticks[0]=50;
//				info->Onticks[1]=5;
				info->Onticks[7] = mainBrightness;
				info->Onticks[6] = neighborBrightness;
				info->Onticks[5] = neighborBrightness2;
				info->Onticks[0] = neighborBrightness;
				info->Onticks[1] = neighborBrightness2;
				resetEBug = 0; //wait for next bug
			}

			SPISendByte(info->LedStatus);
			SET_LATCH;
			DISABLE_LATCH;
			ENABLE_LEDS;

}
