/*
 * flysky.c
 *
 *  Created on: 15 lut 2017
 *      Author: Kuba
 */
#include <stdint.h>
#include <stddef.h>
#include "flysky.h"


/*
__attribute__((section (".s_SendPacketWithSysTick"))) void SendPacketWithSysTick(void) {}
__attribute__((section (".s_sprintf")))int sprintf ( char* buffer, const char* format, ...){return 0;}
__attribute__((section (".s_strLength"))) size_t _strlen( const char * str ) {return 0;}
__attribute__((section (".s_memset"))) void *_memset(void *str, int c, size_t n){return (void*)0;}


__attribute__((section (".s_getKeyCode"))) int getKeyCode(){return 0;};
__attribute__((section (".s_getInputStates"))) int getInputStates(){return 0;};
__attribute__((section (".s_chooseSensors"))) void chooseSensors(){} //must re reimpl
__attribute__((section (".s_callSetupDMAandSend"))) void callSetupDMAandSend(){}
__attribute__((section (".s_readEeprom"))) int readEeprom(uint8_t * target, int16_t eepromOffset, int length){return 0;}
__attribute__((section (".s_saveEeprom"))) int saveEeprom(uint8_t * target, int16_t eepromOffset, int length){return 0;}
__attribute__((section (".s_displayGFX"))) int displayGFX(gfxInfo* gfx, int x, int y){return 0;}
__attribute__((section (".s_fillRegion"))) int fillRegion(int x1, int y1, int x2, int y2, int value){return 0;}  //value 0 for clear 1 for fill
__attribute__((section (".s_drawLine"))) int drawLine(int x1, int y1, int x2, int y2, int value){return 0;}  //value always 0
__attribute__((section (".s_setPixel"))) int setPixel(int x1, int y1, int value){return 0;};  //value always 0
__attribute__((section (".s_drawRect"))) signed int drawRect(int x1, signed int y1, int x2, signed int y2, int value){return 0;}//value is always 0
__attribute__((section (".s_displayTextAt"))) signed int displayTextAt(char* text, int x, int y, int val){return 0;}//value is always 0
__attribute__((section (".s_displayPageHeader"))) signed int displayPageHeader(char* text){return 0;}
//inline __attribute__((section (".s_displaySmallTextAt"))) signed int displaySmallTextAt(int x1, signed int y1, int x2, signed int y2, int value){return 0;}//value is always 0

__attribute__((section (".s_getSensorValue"))) int getSensorValue(int sensorID, int index){return 0;}//value is always 0
//s_formatSensorData
__attribute__((section (".s_getSensorValue"))) int setSensorValue(char* sensorArray){return 0;}
__attribute__((section (".s_updateVoltageTelemetry"))) void updateVoltageTelemetry(){}

__attribute__((section (".s_checkAlarms"))) void checkAlarms(){} //reimplement


__attribute__((section (".s_sensorsListMainScreen"))) void sensorsListMainScreen(); //already moved find correct lcoaotpn
__attribute__((section (".s_showNavigationPage"))) signed int showNavigationPage(char* sensorArray, int numberofitems, manuEntry* menuStruct); //already moved find correct lcoaotpn


__attribute__((section (".s_beep"))) void beep(int freq, int duration);
__attribute__((section (".s_crcCheck"))) int crcCheck(char* data, int length);
__attribute__((section (".s_getCurrentTicks"))) int getCurrentTicks();
*/
