/*
 * Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * This is template for main module created by New Kinetis SDK 2.x Project Wizard. Enjoy!
 **/

#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "delay.h"
#include "fsl_gpio.c"
#include "lcd.h"
#include "console.h"
#include "screen.h"
#include "debug.h"
#include "adc.h"
#include "backlight.h"
#include "MKL16Z4.h"
#include "fsl_common.h"
#include "fsl_dma.h"
#include "fsl_dmamux.h"
#include "fsl_uart.h"
#include "flysky.h"
#include "mod.h"
#include "alt.h"
#include "ibustelemetry.h"

//#define SILENT

extern void __call_configurePins_ASM();
extern void __call_extraChannels_ASM();
extern void __call_swE_ASM();
extern void __call_swB_ASM();
extern void __call_displaySensors1_ASM();
extern void __call_displaySensors2_ASM();
extern void __call_parseAcFrame_ASM();
extern void __call_getSensorName_ASM();
extern void __call_getSensorName2_ASM();
extern void __call_printTimer_ASM();
extern void __call_printTimer1_ASM();
extern void __call_printTimer2_ASM();
extern void __call_formatSensorValue_ASM();
extern void __call_formatSensorValue2_ASM();
extern void __call_loadSettings_ASM();
extern void __call_CreatePacket1_ASM();
extern void __call_CreatePacket2_ASM();
extern void __call_voltTelemetry_ASM();
char buffer[32];

int main(void) {
  BOARD_InitPins();
  BOARD_BootClockRUN();
  BOARD_SysTick();
  lcd_init();
  for(int i =0; i < 1024; i++) screen_buffer[i]= 0;
  console_init();
  adc_init();
  BOARD_InitDebugConsole();
  backlightInit();
  debug_init();
  (*(uint32_t *)(MODEL_SETTINGS)) = 0x200002AE;
  unsigned char* buf = 0;
  getSensorName(2);
  //loadModSettings();
  //saveModSettings();
  getModelModConfig();
  printTimer();
  #ifdef TGY_CAT01
  getALT(10000, 600);
  ASLConfig();
  #endif
  acData(buf);
  getAuxChannel(0);
  displayMenu();
  TimerConfig();
  BatteryType();
  SwBConfig();
  createPacketCh1114();
  mixConfig();
  mix(10000, 100,100,0);
  __call_configurePins_ASM();
  __call_extraChannels_ASM();
  __call_swE_ASM();
  __call_swB_ASM();
  __call_displaySensors1_ASM();
  __call_displaySensors2_ASM();
  __call_parseAcFrame_ASM();
  __call_getSensorName_ASM();
  __call_getSensorName2_ASM();
  __call_printTimer_ASM();
  __call_printTimer1_ASM();
  __call_printTimer2_ASM();
  __call_formatSensorValue_ASM();
  __call_formatSensorValue2_ASM();
  __call_loadSettings_ASM();
  __call_CreatePacket1_ASM();
  __call_CreatePacket2_ASM();
  __call_voltTelemetry_ASM();
  configurePINS2();
  CheckCustomAlarms();
  adjustVoltageConfig();
  //keep few regions
  if(keep1==0){keep1++;}
  if(keep2==0){keep2++;}
  if(keep3==0){keep3++;}
  if(keep4==0){keep4++;}
  if(keep5==0){keep5++;}
  if(keep6==0){keep6++;}
  if(keep7==0){keep7++;}
  if(keep8==0){keep8++;}
  if(keep9==0){keep9++;}
  if(keep10==0){keep10++;}
  if(keep11==0){keep11++;}
  if(keep12==0){keep12++;}
  if(keep13==0){keep13++;}


  if(txVoltageAddress==0){txVoltageAddress++;}
  if(timerBufferAddress==0){timerBufferAddress++;}
  if(timerValueAddress==0){timerValueAddress++;}
  if(mainScreenIndexAddress==0){mainScreenIndexAddress++;}
  if(extraMenuAddress==0){extraMenuAddress++;}
  if(extraMenuText==0){extraMenuText++;}
  if(voltAdjText==0){voltAdjText++;}
  if(voltAdjAddr==0){voltAdjAddr++;}


  if(keepChecksum == 0){keepChecksum++;}
  //keep this one because of without usage signature block will be removed
  if(SIGNATURE[0] == 1){
  }

  #ifdef SILENT
  beepSilent();
  #endif
  adjustVoltage((char *)mod_version);
  formatSensorValue((char *)SIGNATURE, 0,0);
  formatSensorValue((char *)mod_version, 0,0);
  //divFun dev = (divFun)0x1E5E;
  //char* format = "%d";
  //sprintfCall(buffer, format, 0);
  swBasADC();
  swEHandling();
  //uint32_t resutl = dev(100U, 10U);
  loadSettings();
  displaySensors();
  auxChannelsPage();
  AlarmConfig();
  printTimer(0);
  auxChannels2();
  mix(10000, -100, 100, 0);
  //__mul64((long long)1, (long long)2);
  //log2fix(1,1);
  //rxTest2();
  //initALT(100000);
  uint32_t reminder=0;
  __asm("mov r1, #4" : [output] "=r" (reminder));
  __asm("mov %[output], r1" : [output] "=r" (reminder));

  while(1) {
	 //int32_t alt = getALT(50000);
	 //if(alt > 0){ alt+=alt;alt-=alt;}
	//debug_flush();
	//adc_test();
	//adc_test2();
	//displaySmalString(testdata, 0U,0U);
	//displaySmalString(testdata, 0U,9U);
	//displaySmalString(testdata, 0U,21U);

	lcd_send_data(screen_buffer);
	//debug("###  OpenGround  ###\n");
    //GPIO_TogglePinsOutput(BACKLIGHT_LED_GPIO, (1U << BACKLIGHT_LED_PIN));
  }
}
