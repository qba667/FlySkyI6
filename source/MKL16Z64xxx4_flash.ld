/*
** ###################################################################
**     Processors:          MKL16Z64VFM4
**                          MKL16Z64VFT4
**                          MKL16Z64VLH4
**
**     Compiler:            GNU C Compiler
**     Reference manual:    KL26P121M48SF4RM Rev. 3.2, October 2013
**     Version:             rev. 1.7, 2015-07-29
**     Build:               b160406
**
**     Abstract:
**         Linker file for the GNU C Compiler
**
**     Copyright (c) 2016 Freescale Semiconductor, Inc.
**     All rights reserved.
**
**     Redistribution and use in source and binary forms, with or without modification,
**     are permitted provided that the following conditions are met:
**
**     o Redistributions of source code must retain the above copyright notice, this list
**       of conditions and the following disclaimer.
**
**     o Redistributions in binary form must reproduce the above copyright notice, this
**       list of conditions and the following disclaimer in the documentation and/or
**       other materials provided with the distribution.
**
**     o Neither the name of Freescale Semiconductor, Inc. nor the names of its
**       contributors may be used to endorse or promote products derived from this
**       software without specific prior written permission.
**
**     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**     ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
**     http:                 www.freescale.com
**     mail:                 support@freescale.com
**
** ###################################################################
*/

/* Entry Point */
ENTRY(Reset_Handler)

HEAP_SIZE  = DEFINED(__heap_size__)  ? __heap_size__  : 0x0200;
STACK_SIZE = DEFINED(__stack_size__) ? __stack_size__ : 0x0100;
M_VECTOR_RAM_SIZE = DEFINED(__ram_vector_table__) ? 0x0200 : 0x0;

/* Specify the memory areas */
MEMORY
{
  m_interrupts          (RX)  : ORIGIN = 0x00000000, LENGTH = 0x00000200
  m_flash_config        (RX)  : ORIGIN = 0x00000400, LENGTH = 0x00000010
  m_text                (RX)  : ORIGIN = 0x00000410, LENGTH = 0x0000FBF0
  m_data                (RW)  : ORIGIN = 0x1FFFF800, LENGTH = 0x00002000
}



/* Define output sections */
SECTIONS
{
  /* The startup code goes first into internal flash */
  .interrupts :
  {
    __VECTOR_TABLE = .;
    . = ALIGN(4);
    KEEP(*(.isr_vector))     /* Startup code */
    . = ALIGN(4);
  } > m_interrupts

  .flash_config :
  {
    . = ALIGN(4);
    KEEP(*(.FlashConfig))    /* Flash Configuration Field (FCF) */
    . = ALIGN(4);
  } > m_flash_config



  /* The program code and other data goes into internal flash */
  .text :
  {
    . = ALIGN(4);
    *(.text)                 /* .text sections (code) */
    *(.text*)                /* .text* sections (code) */
    *(.rodata)               /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)              /* .rodata* sections (constants, strings, etc.) */
    *(.glue_7)               /* glue arm to thumb code */
    *(.glue_7t)              /* glue thumb to arm code */
    *(.eh_frame)
    KEEP (*(.init))
    KEEP (*(.fini))
    . = ALIGN(4);
  } > m_text
  
  
  .s_FLASH_25C8 0x25C8 :
{
	*(.s_sensors00);
	*(.s_sensors41);
	*(.s_sensors7d);
	*(.s_sensors80);
	*(.s_sensorsFA);
	*(.s_unknownSensor);

	*(.s_flyModesOffset);
	*(.s_flyModes);
	*(.s_armed);
	*(.s_formatNumberFractial);
	*(.s_formatNumber);
	*(.s_formatGPS);		
	
	*(.s_FLASH_25C8*);
}


.s_RadioMOD 0x3194 :
{
	*(.s_radioModes);
	*(.s_RadioMOD*);
}
.s_SIGNATURE_2780 0x2780 :
{
	*(.s_signature);
	*(.s_SIGNATURE_2780*);
}
  .s_FLASH_2790 0x2790 :
{
	*(.s_foramtCoord)
	*(.s_unitsOffsets)
	*(.s_units)
	*(.s_sensDesc00)
 	*(.s_sensDesc80)
 	*(.s_sensDescFA)
}

/*
*(.s_foramtDBm);
	*(.s_foramtAmp);
	*(.s_formatNumber);
	*(.s_formatMsFractial);
	*(.s_foramtNumFractial);
	*(.s_foramtAngleFractial);
	*(.s_foramtAngle);			
	*(.s_foramtCoord);	
*/
 
   .s_MOD_EEPROM 0xC9B0 :
{
	*(.s_loadModEeprom);
	*(.s_saveModEeprom);
}


 
  .s_AC_SENSOR 0x65C8 :
{
	*(.s_parseAC);
	*(.s_acHelper);
}
 . = ALIGN(4);
 
    .s_MOD_CHANNELS_11_14 0xE400 :
{
	*(.s_channels1114);
}
 
   .s_MOD_ALARMS 0xE140 :
{
	*(.s_customAlarms);
}


	.s_CONFIG_PINS 0xe660 :
  {
 	 *(.s_configurePins*);
  }


	.s_AUX_CH_SW_STATE 0xE6A0 :
  {
 	 *(.s_getSWState*);
  }
 	.s_AUX_CH_SET_VAL 0xE700 :
  {
 	 *(.s_createPacketCh1114*);
  }


.s_MOD_SWB 0xE570 :
{
	*(.s_SW_B_config);
}


   .s_MOD_SwE 0x52F0 :
{
	*(.s_swEHandling);
}

   .s_MOD_SwbADC 0x5310 :
{
	*(.s_swBHandling);
}

 . = ALIGN(1);
  /*5*128 bytes FREE up to E752*/
  
  .FontSmal 0xE471 :
  {
    KEEP(*(.FontSmal))
  } > m_text
  . = ALIGN(4);
  
  
   . = ALIGN(4);
  /*method redefinition not needed any more*/
  
  .s_displaySmallTextAt 0x5174 :
  {
    KEEP(*(.s_displaySmallTextAt))
  }
  
   .s_sensorConst 0x5260 : /*5260 - 533F */
  {
	*(.s_sensorsScreens);
	*(.s_timerOff);
	*(.s_timerNull);
	*(.s_timerFormat);
  }


 


.s_extraMenu 0xFF90 : /*FFA0 - FFFF*/ 
  {
  	*(.s_extraMenu); 
  	*(.s_alarm);
  	*(.s_swbMenu);
  	*(.s_txBat);
 }
 .s_modMenuFun 0xFFB0 : /*FFC0 - FFFF*/
  {
	*(.s_modMenuListFun);
	*(.s_modMenuList);
  }

   . = ALIGN(1);
  
  .s_FREE_FLASH_65A8_768 0x65A8 : 
  {
     KEEP(*(.FREE_FLASH_65A8_768)) /* 65A8 redirect rx packet and add handling for ac sensor main screen goes here */
  }
  . = ALIGN(4);
  .s_SENSORS_MAIN_SCREEN_6704 0x6704 : 
{
  *(.s_displaySensors); 
    KEEP(*(.SENSORS_MAIN_SCREEN_6704))
}
   . = ALIGN(4);
      
  .TestCODE 0xB600 : /*moved while implementing sensors move it back to s_ALT*/
  {
  *(.altmulu16); 
  *(.altinit);
  *(.altgetALT);
  *(.altlogfix);
  *(.altlog2fix); 
    KEEP(*(.TestCODE))
    
  } > m_text
  
  
  s.FREE_FLASH_C1DC_308 0xC1DC :
  {
     KEEP(*(.FREE_FLASH_C1DC_308)) /* there is one jump in the moddle clean it */
  }
  

  
  .s_MOD_SPACE 0xF4D0 :
  {
 	 *(.s_MOD_SPACE*);
  }

   
   .s_battery 0xF820 :
  {
    *(.s_batteryConfig);
  }   
 
 	.s_AUX_CH 0xF66C :
  {
 	 *(.s_getAuxChannel*);
  }
 
 
 .s_getSensorName 0xF7B0 : /*f020 - f0b7*/
  {
    KEEP(*(.s_getSensorName))
  }
  
 	.s_TIMER_SPACE 0xF8E0 :
  {
 	 *(.s_printTimer*);
 	 *(.s_checkTimerActive*);
  }
     
  .s_formatSensorValue 0xFA00 :
  {
    *(.s_formatSensorValue);
    *(.s_divMod);
    *(.s_parseCoord);
  }
  
  .s_alarmPage 0xFC00 :
  {
   	 *(.s_alarmConfig);
  }

 .s_TimerConfigPage 0xFDE8 :
  {
   	 *(.s_timerConfig);
  }
    

  
  
  /* to be added later
  .s_ALT 0xFD20 :
  {

  *(.altmulu16);
  *(.altinit);
  *(.altgetALT);
  *(.altlogfix);
  *(.altlog2fix);
  *(.s_ALT*);
  }
  */

  .ARM.extab :
  {
    *(.ARM.extab* .gnu.linkonce.armextab.*)
  } > m_text

  .ARM :
  {
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
  } > m_text

 .ctors :
  {
    __CTOR_LIST__ = .;
    /* gcc uses crtbegin.o to find the start of
       the constructors, so we make sure it is
       first.  Because this is a wildcard, it
       doesn't matter if the user does not
       actually link against crtbegin.o; the
       linker won't look for a file to match a
       wildcard.  The wildcard also means that it
       doesn't matter which directory crtbegin.o
       is in.  */
    KEEP (*crtbegin.o(.ctors))
    KEEP (*crtbegin?.o(.ctors))
    /* We don't want to include the .ctor section from
       from the crtend.o file until after the sorted ctors.
       The .ctor section from the crtend file contains the
       end of ctors marker and it must be last */
    KEEP (*(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors))
    KEEP (*(SORT(.ctors.*)))
    KEEP (*(.ctors))
    __CTOR_END__ = .;
  } > m_text

  .dtors :
  {
    __DTOR_LIST__ = .;
    KEEP (*crtbegin.o(.dtors))
    KEEP (*crtbegin?.o(.dtors))
    KEEP (*(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*(.dtors))
    __DTOR_END__ = .;
  } > m_text

  .preinit_array :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } > m_text

  .init_array :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
  } > m_text

  .fini_array :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } > m_text

  __etext = .;    /* define a global symbol at end of code */
  __DATA_ROM = .; /* Symbol is used by startup for data initialization */

  /* reserve MTB memory at the beginning of m_data */
  .mtb : /* MTB buffer address as defined by the hardware */
  {
    . = ALIGN(8);
    _mtb_start = .;
    KEEP(*(.mtb_buf)) /* need to KEEP Micro Trace Buffer as not referenced by application */
    . = ALIGN(8);
    _mtb_end = .;
  } > m_data

  .interrupts_ram :
  {
    . = ALIGN(4);
    __VECTOR_RAM__ = .;
    __interrupts_ram_start__ = .; /* Create a global symbol at data start */
    *(.m_interrupts_ram)     /* This is a user defined section */
    . += M_VECTOR_RAM_SIZE;
    . = ALIGN(4);
    __interrupts_ram_end__ = .; /* Define a global symbol at data end */
  } > m_data

  __VECTOR_RAM = DEFINED(__ram_vector_table__) ? __VECTOR_RAM__ : ORIGIN(m_interrupts);
  __RAM_VECTOR_TABLE_SIZE_BYTES = DEFINED(__ram_vector_table__) ? (__interrupts_ram_end__ - __interrupts_ram_start__) : 0x0;

  .data : AT(__DATA_ROM)
  {
    . = ALIGN(4);
    __DATA_RAM = .;
    __data_start__ = .;      /* create a global symbol at data start */
    *(.data)                 /* .data sections */
    *(.data*)                /* .data* sections */
    KEEP(*(.jcr*))
    . = ALIGN(4);
    __data_end__ = .;        /* define a global symbol at data end */
  } > m_data

  .GFX_ARRAY 0x1FFFF8C0 :
  {
      *(.screenBuffer);
  }
  
  .MOD_CONFIG2 0x20000D7A :
  {
      *(.s_modConfigAsModel20);
  }

  
  .AUX_MEM 0x20000F4A :
  {
  *(.s_mainScreenIndex)
  *(.s_modConfigEeprom);
  *(.s_longSensors);
  *(.s_mavlinkGPSFrame)
  *(.s_timerBuffer);
  *(.s_timerValue);
  *(.s_ticks100ms);
  *(.s_lastTimerUpdate);
  *(.altconstVal);
  *(.altconstVal);
  *(.altinitPressure);
  *(.altinitPressureRaw);
  
    KEEP(*(.AUX_MEM))
  }
  

  __DATA_END = __DATA_ROM + (__data_end__ - __data_start__);
  text_end = ORIGIN(m_text) + LENGTH(m_text);
  ASSERT(__DATA_END <= text_end, "region m_text overflowed with text and data")

  /* Uninitialized data section */
  .bss :
  {
    /* This is used by the startup in order to initialize the .bss section */
    . = ALIGN(4);
    __START_BSS = .;
    __bss_start__ = .;
    *(.bss)
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    __bss_end__ = .;
    __END_BSS = .;
  } > m_data


  .heap :
  {
    . = ALIGN(8);
    __end__ = .;
    PROVIDE(end = .);
    __HeapBase = .;
    . += HEAP_SIZE;
    __HeapLimit = .;
    __heap_limit = .; /* Add for _sbrk */
  } > m_data

  .stack :
  {
    . = ALIGN(8);
    . += STACK_SIZE;
  } > m_data


  /* Initializes stack on the end of block */
  __StackTop   = ORIGIN(m_data) + LENGTH(m_data);
  __StackLimit = __StackTop - STACK_SIZE;
  PROVIDE(__stack = __StackTop);

  .ARM.attributes 0 : { *(.ARM.attributes) }

  ASSERT(__StackLimit >= __HeapLimit, "region m_data overflowed with stack and heap")
}

