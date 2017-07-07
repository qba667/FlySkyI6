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

typedef struct sensorAlarm
{
	uint8_t sensorID;
	uint8_t operator;
	uint16_t value;
} sensorAlarm;

//146 bytes by 19 models
typedef struct modelConfStruct
{
	uint8_t ch11_12;
	uint8_t ch12_13;
	uint8_t ch14;
	uint16_t timerAlarm;
} modelConfStruct;

typedef struct globalConfigStruct
{
	uint16_t batteryVoltage;
	uint8_t swB3Pos;
	uint8_t timerCH;
	uint16_t timerStart;
	sensorAlarm alarm[3];
	modelConfStruct modelConfig[19];

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

globalConfigStruct __attribute__((section (".s_modConfigAsModel20"))) modConfig2;
uint8_t __attribute__((section (".s_mainScreenIndex"))) mainScreenIndex = 0; //referenced from assembly
configStruct __attribute__((section (".s_modConfigEeprom"))) modConfig; 			//16bytes
int32_t __attribute__((section (".s_longSensors"))) longSensors[SENSORS_ARRAY_LENGTH]; 		//72bytes referenced from assembly
uint8_t __attribute__((section (".s_timerBuffer"))) timerBuffer[10]; 		//10bytes
uint32_t __attribute__((section (".s_timerValue"))) timerValue;
uint32_t __attribute__((section (".s_lastTimerUpdate"))) lastTimerUpdate;
uint8_t __attribute__((section (".s_ticks100ms"))) ticks100MS;

uint8_t __attribute__((section (".s_mavlinkGPSFrame"))) mavlinkGPSFrame[22]; 	//22bytes

 __attribute__((section (".s_getAuxChannel"))) int getAuxChannel(uint32_t request);
 __attribute__((section (".s_getSWState"))) int getSWState(uint32_t swIndex);

__attribute__((section (".s_modMenuListFun"))) void displayMenu();

__attribute__((section (".s_acHelper")))  void add2ByteSensor(uint8_t sensorID, uint8_t sensorIndex, uint16_t value);
__attribute__((section (".s_parseAC"))) void acData(uint8_t* rxBuffer);


 __attribute__((section (".s_printTimer"))) void printTimer();
 __attribute__((section (".s_checkTimerActive"))) uint32_t isTimerActive();
 __attribute__((section (".s_timerConfig"))) void TimerConfig();

 __attribute__((section (".s_batteryConfig"))) void BatteryType();

 __attribute__((section (".s_SW_B_config"))) void SwBConfig();
 __attribute__((section (".s_alarmConfig"))) void AlarmConfig();
 __attribute__((section (".s_customAlarms"))) void ChackCustomAlarms();

 __attribute__((section (".s_swBHandling"))) uint8_t swBasADC();
__attribute__((section (".s_loadModEeprom"))) void loadModSettings();
__attribute__((section (".s_saveModEeprom"))) void saveModSettings();

__attribute__((section (".s_displaySensors"))) void displaySensors();
__attribute__((section (".s_getSensorName"))) const uint8_t* getSensorName(int sensor);
__attribute__((section (".s_formatSensorValue"))) void formatSensorValue(char* target, int sensorID, uint16_t sensorValue);
__attribute__((section (".s_divMod"))) uint32_t divMod(uint32_t val, uint32_t divisor, uint32_t* mod);
__attribute__((section (".s_parseCoord"))) void parseCoord(uint32_t *deg, uint32_t *min, uint32_t *sec, uint32_t *subSec, uint32_t coord);

__attribute__((section (".altconstVal"))) uint32_t constVal = 0;
__attribute__((section (".altinitPressure"))) uint32_t initPressure = 0;
__attribute__((section (".altinitPressureRaw"))) uint32_t initPressureRaw = 0;



__attribute__((section (".altmulu16"))) uint32_t mulu16(uint32_t x, uint32_t y);
__attribute__((section (".altinit"))) void init(uint32_t pressure);
__attribute__((section (".altgetALT"))) int16_t getALT(uint32_t pressure);
__attribute__((section (".altlogfix"))) int32_t logfix(uint32_t x, size_t precision);
__attribute__((section (".altlog2fix"))) int32_t log2fix(uint32_t x, size_t precision);

#endif /* SOURCE_ALT_H_ */
