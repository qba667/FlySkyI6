/*
 * mod.h
 *
 *  Created on: 16 lut 2017
 *      Author: Kuba
 */

#ifndef SOURCE_MOD_H_
#define SOURCE_MOD_H_
#include <stdint.h>
#include <stddef.h>
#include "ibustelemetry.h"
#include "alt.h"
#define SENSORS_PER_PAGE	8
#define SENSORS_PAGES		4

#define PRESSURE_OFFSET UINT32_C(136)
#define ODO1_OFFSET	UINT32_C(144)
#define COG_OFFSET	UINT32_C(167)
#define ALT_OFFSET	UINT32_C(216)

#define STD_SENSOR 0 << 7
#define CUS_SENSOR 1 << 7
#define SIGNED__	1 << 6
#define UNSIGNED	0 << 6
#define MUL_001		0 << 4
#define MUL_100		2 << 4

#define UNIT_NONE 	0
#define UNIT_M 		1
#define UNIT_DEG 	2
#define UNIT_A		3
#define	UNIT_PROC	4
#define UNIT_V		5
#define UNIT_MS		6
#define UNIT_DBM	7
#define UNIT_DB		8
#define UNIT_KMH	9
#define UNIT_CM_S	10
#define UNIT_MAH	11


const uint8_t __attribute__((section (".mod_sensors00"))) SENSOR_00[] = {
/*00@0*/		'I', 'n', 't', 'V', '\0',
/*01@5*/		'T', 'e', 'm', 'p', '\0',
/*02@10*/		'M', 'o', 't', '\0','\0',
/*03@15*/		'E', 'x', 't', 'V', '\0',
/*04@20*/		'C', 'e', 'l', 'l', '\0',
/*05@25*/		'C', 'u', 'r', '\0','\0',
/*06@30*/		'F', 'u', 'e', 'l', '\0',
/*07@35*/		'R', 'P', 'M', '\0','\0',
/*08@40*/		'H', 'd', 'g', '\0','\0',
/*09@45*/		'R', 'o', 'C', '\0','\0',	//Climb... ->RoC
/*10@50*/		'C', 'o', 'G', '\0','\0',
/*11@55*/		'G', 'P', 'S', '\0','\0',
/*12@60*/		'A', 'c', 'c', 'X', '\0',
/*13@65*/		'A', 'c', 'c', 'Y', '\0',
/*14@70*/		'A', 'c', 'c', 'Z', '\0',
/*15@75*/		'R', 'o', 'l', 'l', '\0',
/*16@80*/		'P', 'i', 't', '.', '\0',
/*17@85*/		'Y', 'a', 'w','\0', '\0',
/*18@90*/		'V', 'S', 'p', 'e', '\0',
/*19@95*/		'G', 'S', 'p', 'e', '\0',
/*20@100*/		'D', 'i', 's', 't', '\0',
/*21@105*/		'A', 'r', 'm', '.', '\0',
/*22@110*/		'M', 'o', 'd', 'e', '\0',


};
const uint8_t __attribute__((section (".mod_sensors41"))) SENSOR_41[] = {
/*41@0*/		'P', 'r', 'e', 's', 's', '.', 0x00, 0x00
};

const uint8_t __attribute__((section (".mod_sensors7d"))) SENSORS_7D[] = {
/*7c@0*/		'O', 'd','o', '1', '\0',			//Odo1....
/*7d@5*/		'O', 'd','o', '2', '\0',			//Odo2....
/*7e@10*/		'S', 'p','e', ' ', '\0',			// Spe....
/*7f@15*/		'T', 'x', ' ', 'V', '\0',			//Tx V....
};

const uint8_t __attribute__((section (".mod_sensors80"))) SENSORS_80[] = {
			'L', 'a', 't', 0x0, 0x0, /*0x80*/
			'L', 'o', 'n', 0x0, 0x0, /*0x81*/
			'G', 'A', 'l', 't', 0x0, /*0x82*/
			'A', 'l', 't', 0x0, 0x0, /*0x83*/
			'S', '8', '4', 0x0, 0x0, /*0x84*/
			'S', '8', '5', 0x0, 0x0, /*0x85*/
			'S', '8', '6', 0x0, 0x0, /*0x86*/
			'S', '8', '7', 0x0, 0x0, /*0x87*/
			'S', '8', '8', 0x0, 0x0, /*0x88*/
			'S', '8', '9', 0x0, 0x0, /*0x89*/
			'S', '9', '0', 0x0, 0x0, /*0x8a*/
};


const uint8_t __attribute__((section (".mod_sensorsFA"))) SENSORS_FA[] = {
			'S', 'N', 'R', 0x0, 0x0,
			'N', 'o', 'i', '.', 0x0,
			'R', 'S', 'S', 'I', 0x0,
			'S', ' ', 'F', 'D', 0x0,
			'E', 'r', 'r', '.', 0x0,
			'N', 'o', 'n', 'e', 0x0,
};

const uint8_t __attribute__((section (".mod_unknownSensor"))) UNKNOWN_SENSOR[] = {
			'U', 'n', 'k', 'n', 'o', 'w', 'n', 0x0,
};
/////////////////////////////////////////
//never change - referenced from assembly
/////////////////////////////////////////
const uint8_t __attribute__((section (".mod_radioModes"))) RADIO_MODES[] = {
	'I', 'B', 'U', 'S', '/', 'P', 'W', 'M', 0x00,						//IBUS/PWM.
	'I', 'B', 'U', 'S', '/', 'P', 'P', 'M', 0x00,						//IBUS/PPM.
	'S', 'B', 'U', 'S', '/', 'P', 'W', 'M', 0x00,						//SBUS/PWM
	'S', 'B', 'U', 'S', '/', 'P', 'P', 'M', 0x00,						//SBUS/PPM
};

const uint8_t __attribute__((section (".mod_armed"))) ARMED[] = {0x41, 0x72, 0x6d, 0x65, 0x64}; //Armed

const uint8_t __attribute__((section (".mod_flyModesOffset"))) flyModesOffset[] = {
	0, 5, 10, 16, 21, 28, 35, 39, 46, 52
};
const uint8_t __attribute__((section (".mod_flyModes"))) FLY_MODES[] = {
	/*0@00*/ 0x53, 0x74, 0x61, 0x62, 0x00,						//stab
	/*1@05*/ 0x41, 0x63, 0x72, 0x6F, 0x00, 						//acro
	/*2@10*/ 0x41, 0x48, 0x6F, 0x6C, 0x64, 0x00, 				//AHold
	/*3@16*/ 0x41, 0x75, 0x74, 0x6F, 0x00, 						//Auto
	/*4@21*/ 0x47, 0x75, 0x69, 0x64, 0x65, 0x64, 0x00, 			//Guided
	/*5@28*/ 0x4C, 0x6F, 0x69, 0x74, 0x65, 0x72, 0x00, 			//Loiter
	/*6@35*/ 0x52, 0x54, 0x4C, 0x00,							//RTL
	/*7@39*/ 0x43, 0x69, 0x72, 0x63, 0x6C, 0x65, 0x00, 			//Circle
	/*8@46*/ 0x50, 0x48, 0x6F, 0x6C, 0x64, 0x00, 				//PHold
	/*9@52*/ 0x4C, 0x61, 0x6E, 0x64, 0x00, 						//Land
};

const uint8_t __attribute__((section (".mod_sensorsScreens"))) sensorsScreens[] = {
	IBUS_MEAS_TYPE_FLIGHT_MODE,
	IBUS_MEAS_TYPE_ARMED,
	IBUS_MEAS_TYPE_ERR,
	IBUS_MEAS_TYPE_RSSI,
	IBUS_MEAS_TYPE_NOISE,
	IBUS_MEAS_TYPE_SNR,
	IBUS_MEAS_TYPE_TX_V,
	IBUS_MEAS_TYPE_INTV,

	IBUS_MEAS_TYPE_GPS_STATUS,
	IBUS_MEAS_TYPE_GPS_LAT,
	IBUS_MEAS_TYPE_GPS_LON,
	IBUS_MEAS_TYPE_GPS_ALT,
	IBUS_MEAS_TYPE_ALT,
	IBUS_MEAS_TYPE_GPS_DIST,
	IBUS_MEAS_TYPE_CMP_HEAD,
	IBUS_MEAS_TYPE_COG,

	IBUS_MEAS_TYPE_EXTV,
	IBUS_MEAS_TYPE_TEM,
	IBUS_MEAS_TYPE_CELL,
	IBUS_MEAS_TYPE_BAT_CURR,
	IBUS_MEAS_TYPE_FUEL,
	IBUS_MEAS_TYPE_ACC_X,
	IBUS_MEAS_TYPE_ACC_Y,
	IBUS_MEAS_TYPE_ACC_Z,

	IBUS_MEAS_TYPE_ROLL,
	IBUS_MEAS_TYPE_PITCH,
	IBUS_MEAS_TYPE_YAW,
	IBUS_MEAS_TYPE_SPE,
	IBUS_MEAS_TYPE_GROUND_SPEED,
	IBUS_MEAS_TYPE_VERTICAL_SPEED,
	IBUS_MEAS_TYPE_CLIMB_RATE,
	IBUS_MEAS_TYPE_RPM
};
const uint8_t __attribute__((section (".mod_timerFormat"))) timerFormat[] = {
		'%', '0', '2', 'u', ':', '%', '0', '2', 'u', ':', '%', '0', '2', 'u',  0x00
};
const uint8_t __attribute__((section (".mod_timerNull"))) timerNull[] = {
		'0', '0', ':', '0', '0', ':', '0', '0', 0x00
};
const uint8_t __attribute__((section (".mod_alarm"))) alarm[] = {
		'A', 'l', 'a', 'r', 'm',  0x00
};
const uint8_t __attribute__((section (".mod_extraMenu"))) extraMenu[] = {
		'E', 'x', 't', 'r', 'a',  0x00
};
const uint8_t __attribute__((section (".mod_altText"))) altSensor[] = {
		'A', 'l', 't',  0x00,  
};
const uint8_t __attribute__((section (".mod_txBat"))) txBat[] = {
		'T', 'X', ' ', 'B', 'a', 't',  0x00
};
//contains pointers to functions!!!
const uint32_t __attribute__((section (".mod_modMenuList"))) menuList[] = {
	TEXT_TIMMER, (uint32_t)(&TimerConfig+1), 
	(uint32_t)&alarm, (uint32_t)(&AlarmConfig+1), 
	SW_B_C, (uint32_t)(&SwBConfig+1), 
	(uint32_t)&txBat, (uint32_t)(&BatteryType+1), 
	AUX_CH_TEXT, (uint32_t)(&auxChannels2+1),
	(uint32_t)&altSensor, (uint32_t)(&displayMenu+1), 
};


/*
CSMMUUUU
C 1 -custom 0 std
S 1 -signed 0 unsigned
MM mulitplayed by 00=1 10=100
UUUU UNIT
 * */

const uint8_t __attribute__((section (".mod_sensDesc00"))) sensorDesc00[] = {
/*00*/	STD_SENSOR|UNSIGNED|MUL_100|UNIT_V,		//IntV
/*01*/	STD_SENSOR|SIGNED__|MUL_001|UNIT_DEG, 	//Temp
/*02*/	STD_SENSOR|UNSIGNED|MUL_001|UNIT_NONE, 	//Mot
/*03*/	STD_SENSOR|UNSIGNED|MUL_100|UNIT_V,		//ExtV
/*04*/	STD_SENSOR|UNSIGNED|MUL_100|UNIT_V,		//Cell
/*05*/	STD_SENSOR|UNSIGNED|MUL_100|UNIT_A,		//Current
/*06*/	STD_SENSOR|UNSIGNED|MUL_001|UNIT_PROC,	//remaining battery percentage / mah drawn otherwise or fuel level no unit!
/*07*/	STD_SENSOR|UNSIGNED|MUL_001|UNIT_NONE,	//RPM bat
/*08*/	STD_SENSOR|UNSIGNED|MUL_001|UNIT_DEG,  	//Heading
/*09*/	STD_SENSOR|SIGNED__|MUL_100|UNIT_MS, 	//Climb
/*0a*/	STD_SENSOR|SIGNED__|MUL_001|UNIT_DEG,	//CoG.
/*0b*/	CUS_SENSOR|UNSIGNED|MUL_001|UNIT_NONE,	//GPS. status
/*0c*/	STD_SENSOR|SIGNED__|MUL_100|UNIT_CM_S, 	//Acc x
/*0d*/	STD_SENSOR|SIGNED__|MUL_100|UNIT_CM_S, 	//Acc y
/*0e*/	STD_SENSOR|SIGNED__|MUL_100|UNIT_CM_S, 	//Acc z
/*0f*/	STD_SENSOR|SIGNED__|MUL_100|UNIT_DEG, 	//roll
/*10*/	STD_SENSOR|SIGNED__|MUL_100|UNIT_DEG, 	//pitch
/*11*/	STD_SENSOR|SIGNED__|MUL_100|UNIT_DEG, 	//yaw
/*12*/	STD_SENSOR|SIGNED__|MUL_100|UNIT_MS, 	//VSpeed
/*13*/	STD_SENSOR|UNSIGNED|MUL_100|UNIT_MS, 	//ground Speed
/*14*/	STD_SENSOR|UNSIGNED|MUL_001|UNIT_M,  	//Dist
/*15*/	CUS_SENSOR|UNSIGNED|MUL_001|UNIT_NONE,  //Armed
/*16*/	CUS_SENSOR|UNSIGNED|MUL_001|UNIT_NONE,  //Mode
};
//0x41 is custom

const uint8_t __attribute__((section (".mod_sensDesc80"))) sensorDesc80[] = {
		CUS_SENSOR|SIGNED__|MUL_001|UNIT_NONE,	//Lat.
		CUS_SENSOR|SIGNED__|MUL_001|UNIT_NONE,	//Lon.
		STD_SENSOR|SIGNED__|MUL_100|UNIT_M,		//Alt -> GPS alt
		STD_SENSOR|SIGNED__|MUL_100|UNIT_M,		//ALT
		STD_SENSOR|SIGNED__|MUL_100|UNIT_NONE,	//s84
		STD_SENSOR|SIGNED__|MUL_100|UNIT_NONE,	//s85
		STD_SENSOR|SIGNED__|MUL_100|UNIT_NONE,	//s86
		STD_SENSOR|SIGNED__|MUL_100|UNIT_NONE,	//s87
		STD_SENSOR|SIGNED__|MUL_100|UNIT_NONE,	//s88
		STD_SENSOR|SIGNED__|MUL_100|UNIT_NONE,	//s89
		STD_SENSOR|SIGNED__|MUL_100|UNIT_NONE,	//s89
};

const uint8_t __attribute__((section (".mod_sensDescFA"))) sensorDescFA[] = {
		STD_SENSOR|UNSIGNED|MUL_001|UNIT_DB,	//SNR
		STD_SENSOR|UNSIGNED|MUL_001|UNIT_DBM,	//Noise
		STD_SENSOR|UNSIGNED|MUL_001|UNIT_DBM,	//RSSI
		STD_SENSOR|UNSIGNED|MUL_001|UNIT_NONE,	//SensFD
		STD_SENSOR|UNSIGNED|MUL_001|UNIT_PROC,	//Error
		STD_SENSOR|UNSIGNED|MUL_001|UNIT_NONE,	//None
};

const uint8_t __attribute__((section (".mod_foramtCoord"))) formatCoord[] = { '%', 'u', '.', '%', '0', '6', 'u', 0x7f, 0x00};
const uint8_t __attribute__((section (".mod_formatGPS"))) formatGPS[] = {0x25, 0x75, 0x20, 0x25, 0x30, 0x32, 0x75, 0x00};// "%u %02u";
const uint8_t __attribute__((section (".mod_formatNumber"))) formatNumber[] = {0x25, 0x75, 0x00}; //"%u";
const uint8_t __attribute__((section (".mod_formatNumberFractial"))) formatNumberFractial[] = {0x25, 0x75, 0x2e, 0x25, 0x30, 0x32, 0x75, 0x00};// "%u.%02u";

const uint8_t __attribute__((section (".mod_signature"))) SIGNATURE[] = {
		0xCC, 0x51 , 0x72, 0xC8, 0xA0, 0x7E, 0x01, 0x92, 0xF0, 0xE7, 0x00, 0x00, 0x0A, 0x00, 0x01, 0x00
};


//if unit > 0 sub 1
const uint8_t __attribute__((section (".mod_unitsOffsets"))) unitsOffsets[] = {
	0, 2, 4, 6, 8, 10, 14, 18, 21, 26, 31
};


const uint8_t __attribute__((section (".mod_units"))) units[] = {
	/*1@0*/  'm', 0x00,						//m;
	/*2@2*/  0x7f, 0x00,					//°
	/*3@4*/ 'A', 0x00,						//A
	/*4@6*/ '%', 0x00,						//%
	/*5@8*/ 'V', 0x00,						//V
	/*6@10*/ 'm', '/', 's', 0x00,			//m/s
	/*7@14*/ 'd', 'B', 'm', 0x00,			//dBm
	/*8@18*/ 'd', 'B', 0x00,				//dB
	/*9@21*/ 'k', 'm', '/', 'h', 0x00,		//km/h
	/*10@26*/'c', 'm', '/', 's', 0x00,		//cm/s
	/*11@31*/'m', 'A', 'h', 0x00,
};



__attribute__((section (".notImplemented_AUX_CHANNEL_PAGE"))) signed int  auxChannelsPage();





#endif /* SOURCE_MOD_H_ */
