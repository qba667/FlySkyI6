/*
 * print.c
 *
 *  Created on: 15 lut 2017
 *      Author: Kuba
 */
#include "print.h"
#include <stdint.h>
#include "flysky.h"
#include "screen.h"

void displaySmalString(char *buffer, unsigned int x, unsigned int y)
{

	uint32_t characterIndex = 0;
	unsigned int currentX = x;
	unsigned int bitmapIndex = 0;
	unsigned int page= y / pageH;
	unsigned int pageOffset = y - pageH*page;
	unsigned int segondPageBits = fontH-(pageH-pageOffset);
	unsigned int firstPageMask = 0x80;
	unsigned int secondPageMask = 0xFF;

	if(pageOffset > 0){
		firstPageMask = 0;
		for(int i = 0; i< pageOffset; i++){
			firstPageMask |= 1 << i;
		}
	}

	if(pageOffset > 1){
		for(int i = 0; i< segondPageBits; i++){
			secondPageMask &= ~(1 << i);
		}
	}

	uint8_t screenValue = 0;
	uint8_t fontValue = 0;
	unsigned int fontOffset = 0;

	while(buffer[characterIndex] !=0){
		fontOffset = (buffer[characterIndex]-START_CHAR)*fontW;

		for(bitmapIndex =0 ; bitmapIndex< fontW + 1; bitmapIndex++){
			if(fontOffset == 0 && bitmapIndex > 1) break;
			if(currentX > 127) continue;
			if(bitmapIndex == fontW) fontValue = 0;
			else fontValue = font[fontOffset+bitmapIndex];
			screenValue = screen_buffer[page*128 + currentX];
			screenValue &= firstPageMask;
			screenValue |= (0xFF &(fontValue << pageOffset));
			screen_buffer[page*128 + currentX] = screenValue;

			if(page <7 && secondPageMask != 0xFF){
				screenValue = screen_buffer[(page+1)*128 + currentX];
				screenValue &= secondPageMask;
				screenValue |= fontValue >> (7 - segondPageBits);
				screen_buffer[(page+1)*128 + currentX] = screenValue;
			}
			//next pixel
			currentX++;
		}

		sendPacketWithSysTick();
		characterIndex++;
	}

}


