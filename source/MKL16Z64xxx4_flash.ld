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


  .GFX_ARRAY 0x1FFFF8C0 :
  {
    *(.screenBuffer);
  } > m_data

  .MOD_CONFIG2 0x20000B32 :
  {
    *(.mod_modConfigAsModel17);
  }  > m_data

  /*  20000E0C */

  .AUX_MEM 0x20000F4A :
  {
    *(.mod_mainScreenIndex)
    *(.mod_longSensors);
    *(.mod_mavlinkGPSFrame)
    *(.mod_timerBuffer);
    *(.mod_timerValue);
    *(.mod_ticks100ms);
    *(.mod_lastTimerUpdate);
    *(.mod_altSensorMemory);
    *(.mod_varioMem);
    *(.mod_maxSNR);
    *(.mod_altSensorMemory);
    KEEP(*(.AUX_MEM))
  } > m_data


  /*still some space available*/
  /*16 bytes*/
  . = ALIGN(1);
  .mod_25C8_277F 0x25C8 :
  {
	*(.mod_sensors00);
	*(.mod_sensors41);
	*(.mod_sensors7d);
	*(.mod_sensors80);
	*(.mod_sensorsFA);
	*(.mod_unknownSensor);
	*(.mod_flyModesOffset);
	*(.mod_flyModes);
	*(.mod_formatNumberFractial);
	*(.mod_formatNumber);
	*(.mod_formatGPS);
	*(.mod_timerMaxValues);
	*(.mod_timerLabels);
  *(.mod_foramtCoord);
  *(.mod_units);
	*(.mod_unitsOffsets);
	*(.mod_varioStrings);
  *(.mod_timerNull);
  }
  .mod_SIGNATURE_2780 0x2780 :
  {
	*(.mod_signature);
	*(.mod_SIGNATURE_2780*);
  }

  /*still some space available*/
  /*85 bytes*/
  .mod_2790_27FF 0x2790 :
  {
  *(.mod_sensDesc00)
 	*(.mod_sensDesc80)
 	*(.mod_sensDescFA)
  *(.mod_sensorsScreens);
  *(.mod_timerFormat);
  *(.mod_armed);
  *(.mod_defASL);
  *(.mixPos);
  }
  .reserved_main 0x2800:
  {
    KEEP(*(.reserved_main))
  }

  .mod_beep_silent 0x9C3C : {
    *(.mod_beep_silent);
  }

  /*calls from FW to C code*/
  .mod_configurePins_ASM 0xC0D2 : {
    *(.mod_configurePins_ASM);
  }

  /*
  FREE SPACE AC34 AC57
  */

  .mod_extraChannels_ASM 0xAB5C : {
    *(.mod_extraChannels_ASM);
  }
  .reserved_after_code_extraChannels_ASM 0xAB9F : {
    *(.reserved_after_code_extraChannels_ASM);
  }

  .mod_swB_ASM 0x2896 : {
    *(.mod_swB_ASM);
  }
  .mod_swE_ASM 0x289A : {
    *(.mod_swE_ASM);
  }
  .mod_displaySensors1_ASM 0xC1D8 : {
    *(.mod_displaySensors1_ASM);
  }
  .mod_displaySensors2_ASM 0xC5C2 : {
    *(.mod_displaySensors2_ASM);
  }
  .mod_parseAcFrame_ASM 0x65AE : {
    *(.mod_parseAcFrame_ASM);
  }
  .mod_getSensorName_ASM 0x2FA0 : {
    *(.mod_getSensorName_ASM);
  }
  .mod_getSensorName2_ASM 0x376A : {
    *(.mod_getSensorName2_ASM);
  }
  .mod_loadSettings_ASM 0xC070 : {
    *(.mod_loadSettings_ASM);
  }
  .mod_printTimer_ASM 0xC1DE : {
    *(.mod_printTimer_ASM);
  }
  .mod_printTimer1_ASM 0xC1F4 : {
    *(.mod_printTimer1_ASM);
  }
  .mod_printTimer2_ASM 0xC5E2 : {
    *(.mod_printTimer2_ASM);
  }
  .mod_formatSensorValue_ASM 0x5418 : {
    *(.mod_formatSensorValue_ASM);
  }
  .mod_formatSensorValue2_ASM 0xC236 : {
    *(.mod_formatSensorValue2_ASM);
  }


  .mod_callCreatePacket1_ASM 0xA8D2 : {
    *(.mod_callCreatePacket1_ASM);
  }
  .mod_callCreatePacket2_ASM 0xA8E8 : {
    *(.mod_callCreatePacket2_ASM);
  }

  /*variables referenced from assembly*/

  .mod_mainScreenIndexAddress 0xC9A0 : {
    *(.mod_mainScreenIndexAddress);
  }
  .mod_timerValueAddress 0xC9A4 : {
    *(.mod_timerValueAddress);
  }
  .mod_timerBufferAddress 0xC9A8 : {
    *(.mod_timerBufferAddress);
  }
  .mod_tx_voltage_alarm_address 0x6344 : {
    *(.mod_tx_voltage_alarm_address);
  }

  .mod_extra_menu_str_ptr 0xD4C8 : {
    *(.mod_extra_menu_str_ptr);
  }
  .mod_extra_menu_ptr 0xD4CC : {
    *(.mod_extra_menu_ptr);
  }
  .mod_version 0xEEB5 : {
    *(.mod_version);
  }

  /*never change - referenced from assembly*/
  .mod_radioModes 0x3194 :
  {
    *(.mod_radioModes);
  }
  .mod_rxMenuSpeedDistStr 0xD2EE :
  {
    *(.mod_rxMenuSpeedDistStr);
    *(.mod_rxMenuSpeedDistMethod);
  }



  /*Free space for code*/
  .mod_code_AC34 0xAC34 : {
  *(.mod_log2fix);
  *(.mod_getALT);
  *(.mod_prevSensorID);
  *(.mod_dead);
  }
  .reserved_after_code_AC34  0xAE44 : {
  *(.reserved_after_code_AC34);
  }
  .mod_code_9A60 0x9A60 :
{
  *(.mod_asl);
}
.reserved_after_code_9A60  0x9BC0 : {
*(.reserved_after_code_9BC0);
}

.mod_code_9830 0x9830 :
{

}
.reserved_after_code_9830  0x9928 : {
*(.reserved_after_code_9830);
}

/*
Seed distnace page menu
ROM:0000D2A0 stru_D2A0       manuEntry <0xEE3D, 0xAC59> ; DATA XREF: SpeedDistancePage
ROM:0000D2A8                 manuEntry <0xEE06, 0x9A61>
ROM:0000D2B0                 manuEntry <0xED92, 0x9831>
ROM:0000D2B8                 manuEntry <0xEDA2, 0x98AD>
*/


  /*Free space for code 288 bytes*/
  .mod_code_5174_5353 0x5174 : {
    *(.mod_varioSensorSelect);
    *(.mod_configurePins);
    *(.mod_SNR);

  }
  .reserved_after_code_5174_5353 0x5354 : {
    *(.reserved_after_code_5174_5353);
  }
  .mod_595C 0x595C : {
      *(.mod_voltADJConfig);
      *(.mod_voltSensors);
      *(.mod_acHelper);
      *(.mod_mapSNR);
      *(.mod_aslLabels);
  }
  .reserved_after_code_595C 0x5B28 : {
    *(.reserved_after_code_595C);
  }

  .mod_code_65C8_68A7 0x65C8 :
  {
    *(.mod_parseAC);
    *(.mod_displaySensors);
    *(.mod_loadSettingsExt);
  }
  .reserved_after_code_65C8_68A7 0x68A8 : {
    *(.reserved_after_code_65C8_68A7);
  }

  .mod_code_in_display_method 0xC256  :
  {
     KEEP(*(.mod_code_in_display_method))
  }

  .reserved_after_code_in_display_method 0xC306  :
  {
     KEEP(*(.reserved_after_code_in_display_method))
  }

  .mod_code_C9B0_CA4F 0xC9B0 :
  {
	*(.mod_modelConfig);
	*(.mod_customAlarmsPlay);
	*(.mod_nextSensorID);
	*(.mod_asm_aeabi_lmul);
  }

  .reserved_after_code_C9B0_CA4F 0xCA50 :
  {
	*(.reserved_after_code_C9B0_CA4F);
  }
  .mod_code_D510_D5EF 0xD50C :
  {
   *(.mod_swBHandling);
  }
  .mod_code_D560_D5EF 0xD560 :
  {
  *(.mod_swEHandling);
  *(.mod_checkTimerActive);

  }

  .reserved_after_code_D50C_D5EF 0xD5F0 :
  {
	*(.reserved_after_code_D510_D5EF);
  }

 . = ALIGN(4);

  .mod_code_E140_E754 0xE140 :
  {
    *(.mod_customAlarmsCheck);
    *(.mod_timerConfig);
    *(.mod_SW_B_config);
    *(.mod_getSWState);
    *(.mod_extractConfig);
    *(.mod_createPacketCh1114);
    *(.mod_auxChannelsText);
  }
  .reserved_after_code_E140_E754 0xE754 :{
   	 *(.reserved_after_code_E140_E754);
  }
  .mod_voltTelemetry_ASM 0x57DA :{
   	 *(.mod_voltTelemetry_ASM);
  }
  .reserved_after_voltTelemetry_ASM 0x57EE :{
   	 *(.reserved_after_voltTelemetry_ASM);
  }


  .mod_code_F4D0_FF8F 0xF4D0:
  {
   *(.mod_channels1114*);
 	 *(.mod_getAuxChannel);
   *(.mod_getSensorName);
   *(.mod_batteryConfig);
   *(.mod_printTimer);
   *(.mod_formatSensorValue);
   *(.mod_divMod);
   *(.mod_alarmConfig);
   *(.mod_timerConfig);
   *(.mod_modMenuListFun);
   *(.mod_mix);
   *(.mod_voltADJ);
   *(.mod_extraMenu);
  }

  .reserved_checksum 0xFFFD :{
   	 *(.reserved_checksum*);
  }
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
