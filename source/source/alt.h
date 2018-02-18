/*
 * alt.h
 *
 *  Created on: 18 lut 2017
 *      Author: Kuba
 */

#ifndef SOURCE_ALT_H_
#define SOURCE_ALT_H_

#include <stddef.h>
#include <stdint.h>

#define precision 15
#define FIXED(val) (val << precision)
#define DECIMAL(val) (val >> precision)
#define TMP_OFFSET 19
#define ALT_MASK 0x7FFFF;
#define R_DIV_G_MUL_10_Q15 UINT64_C(9591506)
#define INV_LOG2_E_Q1DOT31 UINT64_C(0x58b90bfc) // Inverse log base 2 of e

#define ALARM_OPERATOR_LT 0
#define ALARM_OPERATOR_GT 0

#define BATTERY_DEFAULT 0
#define	BATERY_LIPO_1S	1
#define	BATERY_LIPO_2S	2
#define BATERY_NICD_4S	3

#define AVG_TMP			19

	#define MAX_INDEX 14
#ifdef SWE
	#define MAX_INDEX 15
#endif

#define SENSORS_ARRAY_LENGTH 11
/* 4 bytes*/
typedef struct sensorAlarm
{
	uint8_t sensorID;
	uint8_t operator;
	uint16_t value;
} sensorAlarm;

#define VARIO_MAX_GAIN_BITS	4	// do not need the whole byte
//24 + sensorAlarm (3 * 4 bytes) 12 bytes = 36 bytes

#define TOTAL_MODELS 15
#define OLD_MODEL_COUNT 20
#define OLD_CONFIGSIZE 146
#define NEW_MODEL_MEM MODEL_SETTINGS + TOTAL_MODELS * OLD_CONFIGSIZE
#define MIX_CONFIG_SIZE_BYTES 24

#define VERSION_MAGIC 0x175
//to use this in linker we need to run linker on ld file
//https://stackoverflow.com/questions/28837199/can-i-use-preprocessor-directives-in-ld-file
//for now just calcualte
//MODEL_SETTINGS 0x200002A4
//0x200002A4 + 15* 146 = 0x20000B32


typedef struct mixConfStruct{
	int8_t min;
	int8_t subtrim;
	int8_t max;
} mixConfStruct;


typedef struct modelConfStruct
{
	uint8_t ch11_12;											//	1
	uint8_t ch13_14;											//	1
	uint16_t timerAlarm;										//	2
	uint8_t timerCH;											//	1
	uint16_t timerStart;										//	2
	sensorAlarm alarm[3];										//	3*4 = 12
	uint32_t initAlt;											// 	4
	uint8_t varioSensorID;										// 1
	uint8_t varioGain : VARIO_MAX_GAIN_BITS;					// 1
	uint8_t reserved_bits : (8 - VARIO_MAX_GAIN_BITS);			// 1
	mixConfStruct mix[8];										// 8*3 = 24
	uint16_t intVoltAdj;
	uint16_t extVoltAdj;
    uint8_t varioDeadBand;
    uint8_t reserved[3];										// 8
} modelConfStruct;												// 58
//7 + 12 + 4 +2 +24 +8 =

//Change allocation to 15
//TOTAL 58 * 15 = 870
//Version magic 2 bytes, battery 2 bytes, sw config 1 byte
//Total 875
//5 *146 = 876

/*total 146*4 = 584 bytes*/
/*used: 3+ 16* 55 = 803*/
/*needed 6 -> 876 -803
/*free: 5*/
//clear method
//mem set to 0
//mix default -100, 100, 0


typedef struct globalConfigStruct
{
	uint16_t batteryVoltage;
	uint8_t swConfig;
	uint16_t versionMagic;
	modelConfStruct modelConfig[TOTAL_MODELS];
} globalConfigStruct;


__attribute__((section (".mod_modConfigAsModel17"))) globalConfigStruct modConfig2;
__attribute__((section (".mod_mainScreenIndex"))) uint8_t  mainScreenIndex = 0; //referenced from assembly
__attribute__((section (".mod_longSensors"))) int32_t  longSensors[SENSORS_ARRAY_LENGTH]; 		//72bytes referenced from assembly
__attribute__((section (".mod_timerBuffer"))) uint8_t  timerBuffer[10]; 		//10bytes
__attribute__((section (".mod_timerValue"))) uint32_t  timerValue;
__attribute__((section (".mod_lastTimerUpdate"))) uint32_t  lastTimerUpdate;
__attribute__((section (".mod_ticks100ms"))) uint8_t  ticks100MS;
__attribute__((section (".mod_mavlinkGPSFrame"))) uint8_t mavlinkGPSFrame[22]; 	//22bytes
__attribute__((section (".mod_maxSNR"))) int32_t maxSNR;

__attribute__((section (".mod_altSensorMemory"))) uint32_t initPressure = 0;
__attribute__((section (".mod_altSensorMemory"))) int32_t initTemperature = 0;

__attribute__((section (".mod_varioMem"))) int32_t varioPrevValue = 0;
__attribute__((section (".mod_varioMem"))) int32_t varioPrevTime = 0;
__attribute__((section (".mod_mapSNR "))) int mapSNR();
__attribute__((section (".mod_beep_silent "))) void beepSilent();
__attribute__((section (".mod_configurePins"))) void configurePINS2();
__attribute__((section (".mod_getAuxChannel"))) int getAuxChannel(uint32_t request);
__attribute__((section (".mod_getSWState"))) int getSWState(uint32_t swIndex);
__attribute__((section (".mod_modMenuListFun"))) void displayMenu();
__attribute__((section (".mod_acHelper")))  void add2ByteSensor(uint8_t sensorID, uint8_t sensorIndex, uint16_t value);
__attribute__((section (".mod_parseAC"))) void acData(uint8_t* rxBuffer);
 __attribute__((section (".mod_printTimer"))) void printTimer();
 __attribute__((section (".mod_checkTimerActive"))) uint32_t isTimerActive();
 __attribute__((section (".mod_timerConfig"))) void TimerConfig();
 __attribute__((section (".mod_batteryConfig"))) void BatteryType();



  __attribute__((section (".mod_asl"))) void ASLConfig();

 __attribute__((section (".mod_nextSensorID"))) uint8_t prevSensorID(uint8_t sensorID);
 __attribute__((section (".mod_prevSensorID")))  uint8_t nextSensorID(uint8_t sensorID);

 __attribute__((section (".mod_channels1114"))) void auxChannels2();
 __attribute__((section (".mod_voltADJConfig")))void adjustVoltageConfig();
  __attribute__((section (".mod_extractConfig"))) void extractConfigCh7_14(uint8_t* result);
	__attribute__((section (".mod_extractConfig"))) void saveAuxCh5_14(uint8_t* current);


 __attribute__((section (".mod_createPacketCh1114"))) void createPacketCh1114();
  __attribute__((section (".mod_createPacketCh1114"))) void extractConfig(uint8_t val, uint8_t* result);
 __attribute__((section (".mod_SW_B_config"))) void SwBConfig();
 __attribute__((section (".mod_alarmConfig"))) void AlarmConfig();
 __attribute__((section (".mod_customAlarmsCheck"))) void CheckCustomAlarms();
 __attribute__((section (".mod_customAlarmsPlay"))) void play(int freq, int duration, int pause);
 __attribute__((section (".mod_swEHandling"))) void swEHandling();
 __attribute__((section (".mod_swBHandling"))) void swBasADC();
 __attribute__((section (".mod_modelConfig"))) modelConfStruct* getModelModConfig();
 /*
 __attribute__((section (".mod_loadModEeprom"))) void loadModSettings();
 __attribute__((section (".mod_saveModEeprom"))) void saveModSettings();
 */

__attribute__((section (".mod_voltADJ"))) void adjustVoltage(uint8_t* sensorArray);
__attribute__((section (".mod_loadSettingsExt"))) void loadSettings();
__attribute__((section (".mod_varioSensorSelect"))) void varioSensorSelect();
__attribute__((section (".mod_displaySensors"))) void displaySensors();
__attribute__((section (".mod_getSensorName"))) const uint8_t* getSensorName(int sensor);
__attribute__((section (".mod_formatSensorValue"))) void formatSensorValue(char* target, int sensorID, uint16_t sensorValue);
__attribute__((section (".mod_divMod"))) uint32_t divMod(uint32_t val, uint32_t divisor, uint32_t* mod);
__attribute__((section (".mod_log2fix"))) int32_t log2fix(uint32_t x);
__attribute__((section (".mod_getALT"))) uint16_t ibusTempToK(int16_t tempertureIbus);
__attribute__((section (".mod_getALT"))) void getInitPressure(uint32_t* pressure, int32_t* temperature);
__attribute__((section (".mod_getALT"))) int getALT(uint32_t pressurePa, uint16_t tempertureIbus);

__attribute__((section (".mod_mix"))) int mix(int value, int8_t min, int8_t max, int8_t subtrim);
__attribute__((section (".mod_mix"))) void mixConfig();


__attribute__((section (".reserved_main"))) uint32_t keep1 = 0;
__attribute__((section (".reserved_after_code_C9B0_CA4F")))  uint32_t keep2 = 0;
__attribute__((section (".reserved_after_code_5174_5353"))) uint32_t keep5 = 0;
__attribute__((section (".reserved_after_code_65C8_68A7")))  uint32_t keep3 = 0;
__attribute__((section (".reserved_after_code_E140_E754"))) uint32_t keep4 = 0;
__attribute__((section (".reserved_after_code_in_display_method"))) uint32_t keep6 = 0;
__attribute__((section (".reserved_after_code_D510_D5EF"))) uint32_t keep7 = 0;
__attribute__((section (".reserved_after_code_extraChannels_ASM"))) uint32_t keep8 = 0;
__attribute__((section (".reserved_after_voltTelemetry_ASM"))) uint32_t keep9 = 0;
__attribute__((section (".reserved_after_code_595C"))) uint32_t keep10 = 0;
__attribute__((section (".reserved_after_code_AC34"))) uint32_t keep11 = 0;
__attribute__((section (".reserved_after_code_9BC0"))) uint32_t keep12 = 0;
__attribute__((section (".reserved_after_code_9830"))) uint32_t keep13 = 0;




__attribute__((section (".reserved_checksum"))) uint16_t keepChecksum = 0;




#endif /* SOURCE_ALT_H_ */
