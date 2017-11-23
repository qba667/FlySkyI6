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
#define PRECISION 13

#define FIXED(val) (val << PRECISION)
#define DECIMAL(val) (val >> PRECISION)
#define INV_LOG2_E_Q1DOT31  UINT32_C(0x58b90bfc) // Inverse log base 2 of e
#define R_div_G_MUL_AVG_TEMP_19_13 0x4300C6D;
#define R_div_G_MUL_AVG_TEMP_19_13_8_C 0x4074096

#define ALARM_OPERATOR_LT 0
#define ALARM_OPERATOR_GT 0

#define BATTERY_DEFAULT 0
#define	BATERY_LIPO_1S	1
#define	BATERY_LIPO_2S	2
#define BATERY_NICD_4S	3

#define AVG_TMP			19

#define SENSORS_ARRAY_LENGTH 11
/* 4 bytes*/
typedef struct sensorAlarm
{
	uint8_t sensorID;
	uint8_t operator;
	uint16_t value;
} sensorAlarm;

//24 + sensorAlarm (3 * 4 bytes) 12 bytes = 36 bytes
typedef struct modelConfStruct
{
	uint8_t ch11_12;
	uint8_t ch13_14;
	uint16_t timerAlarm;
	uint8_t timerCH;
	uint16_t timerStart;
	sensorAlarm alarm[3];
	uint32_t initAlt;
	uint16_t reserved1;
	uint16_t reserved2;
	uint16_t reserved3;
	uint16_t reserved4;
	uint16_t reserved5;
	uint16_t reserved6;
	uint8_t reserved7;
} modelConfStruct;
/*total 146*4 = 584 bytes*/
/*used: 3+ 16* 36 = 579*/
/*free: 5*/
typedef struct globalConfigStruct
{
	uint16_t batteryVoltage;
	uint8_t swConfig;
	modelConfStruct modelConfig[16];

} globalConfigStruct;


typedef struct configStruct
{
	/*@0*/	uint8_t timerCH;
	/*@1*/	uint16_t timerStart;
	/*@3*/	uint16_t timerAlarm;
	/*@5*/  uint16_t batteryVoltage;
	/*@7*//*@11*//*@115*/	sensorAlarm alarm[3];
	/*@19*/ /*uint8_t swB3Pos*/;
} configStruct;

typedef union config
{
	configStruct cfg;
} config;

globalConfigStruct __attribute__((section (".mod_modConfigAsModel17"))) modConfig2;
uint8_t __attribute__((section (".mod_mainScreenIndex"))) mainScreenIndex = 0; //referenced from assembly
//configStruct __attribute__((section (".mod_modConfigEeprom"))) modConfig; 			//16bytes
int32_t __attribute__((section (".mod_longSensors"))) longSensors[SENSORS_ARRAY_LENGTH]; 		//72bytes referenced from assembly
uint8_t __attribute__((section (".mod_timerBuffer"))) timerBuffer[10]; 		//10bytes
uint32_t __attribute__((section (".mod_timerValue"))) timerValue;
uint32_t __attribute__((section (".mod_lastTimerUpdate"))) lastTimerUpdate;
uint8_t __attribute__((section (".mod_ticks100ms"))) ticks100MS;
uint8_t __attribute__((section (".mod_mavlinkGPSFrame"))) mavlinkGPSFrame[22]; 	//22bytes
__attribute__((section (".mod_altconstVal"))) uint32_t constVal = 0;
__attribute__((section (".mod_altinitPressure"))) uint32_t initPressure = 0;
__attribute__((section (".mod_altinitPressureRaw"))) uint32_t initPressureRaw = 0;
__attribute__((section (".mod_auxMem_test1"))) uint32_t allocationTest = 0;


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


 __attribute__((section (".mod_channels1114"))) void auxChannels2();
 __attribute__((section (".mod_createPacketCh1114"))) void createPacketCh1114();
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
 __attribute__((section (".mod_yenyaSpace"))) void testMethod();


__attribute__((section (".mod_displaySensors"))) void displaySensors();
__attribute__((section (".mod_getSensorName"))) const uint8_t* getSensorName(int sensor);
__attribute__((section (".mod_formatSensorValue"))) void formatSensorValue(char* target, int sensorID, uint16_t sensorValue);
__attribute__((section (".mod_divMod"))) uint32_t divMod(uint32_t val, uint32_t divisor, uint32_t* mod);
__attribute__((section (".mod_parseCoord"))) void parseCoord(uint32_t *deg, uint32_t *min, uint32_t *sec, uint32_t *subSec, uint32_t coord);


__attribute__((section (".altmulu16"))) uint32_t mulu16(uint32_t x, uint32_t y);
__attribute__((section (".altinit"))) void init(uint32_t pressure);
__attribute__((section (".altgetALT"))) int16_t getALT(uint32_t pressure);
__attribute__((section (".altlogfix"))) int32_t logfix(uint32_t x, size_t precision);
__attribute__((section (".altlog2fix"))) int32_t log2fix(uint32_t x, size_t precision);

__attribute__((section (".reserved_main"))) uint32_t keep1 = 0;
__attribute__((section (".reserved_after_eeprom")))  uint32_t keep2 = 0;
__attribute__((section (".reserved_after_ac_sensor")))  uint32_t keep3 = 0;
__attribute__((section (".reserved_after_set_ch_val"))) uint32_t keep4 = 0;
__attribute__((section (".reserved_afterSwB_adac"))) uint32_t keep5 = 0;
__attribute__((section (".reserved_after_free_space_in_display"))) uint32_t keep6 = 0;
__attribute__((section (".reserved_after_AFHDSLogo"))) uint32_t keep7 = 0;
__attribute__((section (".reserved_after_free_space_in_display"))) uint16_t keepChecksum = 0;



#endif /* SOURCE_ALT_H_ */
