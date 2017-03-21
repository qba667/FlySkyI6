/*
 * mod.c
 *
 *  Created on: 16 lut 2017
 *      Author: Kuba
 */
#include "flysky.h"
#include "mod.h"
#include "ibustelemetry.h"
#define AUX_CHANNELS_COUNT 6
#define CH_NAME_PTR 0xCC68
#define AUX_PAGE_NAME 0xCC5A
#define CHANEL_NAME_LEN 10
#define ARROW_GFX_INFO 0xCBF8

signed int  auxChannelsPage(){
	uint8_t aux[AUX_CHANNELS_COUNT] = {0};
	uint32_t modelPtr = (*(uint32_t *)(MODEL_SETTINGS));
	uint32_t channelIndex = 0;
	char * channelNamePtr = 0;
	modelPtr += 60;
	aux[0] = *(uint8_t *)(modelPtr);
	aux[1] = *(uint8_t *)(++modelPtr);
	modelPtr += 80;
	aux[2] = *(uint8_t *)(modelPtr);
	aux[3] = aux[2] >> 8;
	aux[2] &= 0xF;
	modelPtr += 4;
	aux[4] = *(uint8_t *)(modelPtr);
	aux[5] = aux[4] >> 8;
	aux[4] &= 0xF;



	while(1){
		//callSetupDMAandSend();
		//displayPageHeader((char *)(AUX_PAGE_NAME));
		channelIndex = 0;
		channelNamePtr = (char *)(CH_NAME_PTR);
		do{
			channelNamePtr += CHANEL_NAME_LEN;
			if(aux[channelIndex]){
				//displayTextAt(channelNamePtr, 8, 8*channelIndex +12, 0);
				// displayTextAt(channelNamePtr, 8, 8 * channelIndex + 12, 0);
			}
		}
		while(channelIndex < AUX_CHANNELS_COUNT);
	}

}
