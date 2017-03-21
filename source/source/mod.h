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


const uint8_t __attribute__((section (".s_sensors00"))) SENSOR_00[] = {
/*00@0*/		0x49, 0x6E, 0x74, 0x56, 0x00, 0x00, 0x00, 0x00,  //IntV....
/*01@8*/		0x54, 0x65, 0x6D, 0x70, 0x00, 0x00, 0x00, 0x00,  //Temp....
/*02@16*/		0x20, 0x4D, 0x6F, 0x74, 0x00, 0x00, 0x00, 0x00,  // Mot....
/*03@24*/		0x45, 0x78, 0x74, 0x56, 0x00, 0x00, 0x00, 0x00,  //ExtV....
/*04@32*/		0x48, 0x65, 0x61, 0x64, 0x69, 0x6E, 0x67, 0x00,  //Heading.
/*05@40*/		0x43, 0x75, 0x72, 0x72, 0x65, 0x6E, 0x74, 0x00,  //Current.
/*06@48*/		0x43, 0x6C, 0x69, 0x6D, 0x62, 0x00, 0x00, 0x00,  //Climb... ->RoC
/*07@56*/		0x41, 0x63, 0x63, 0x20, 0x5A, 0x00, 0x00, 0x00,  //Acc Z...
/*08@64*/		0x41, 0x63, 0x63, 0x20, 0x59, 0x00, 0x00, 0x00,  //Acc Y...
/*09@72*/		0x41, 0x63, 0x63, 0x20, 0x58, 0x00, 0x00, 0x00,  //Acc X...
/*0a@80*/		0x56, 0x53, 0x70, 0x65, 0x65, 0x64, 0x00, 0x00,  //VSpeed..
/*0b@88*/		0x53, 0x70, 0x65, 0x65, 0x64, 0x00, 0x00, 0x00,  //Speed...
/*0c@96*/		0x44, 0x69, 0x73, 0x74, 0x00, 0x00, 0x00, 0x00,  //Dist....
/*0d@104*/		0x41, 0x72, 0x6D, 0x65, 0x64, 0x00, 0x00, 0x00,  //Armed...
/*0e@112*/		0x4D, 0x6F, 0x64, 0x65, 0x00, 0x00, 0x00, 0x00,  //Mode....
/*0f@120*/		0x54, 0x69, 0x6d, 0x65, 0x72, 0x00, 0x00, 0x00,  //Timer...
/*10@128*/		0x4d, 0x6f, 0x64, 0x65, 0x6c, 0x00, 0x00, 0x00,  //Model...
/*11@136*/		0x52, 0x65, 0x73, 0x65, 0x72, 0x76, 0x31, 0x00,  //Reserv1.
/*12@144*/		0x52, 0x65, 0x73, 0x65, 0x72, 0x76, 0x32, 0x00,  //Reserv2.
};
const uint8_t __attribute__((section (".s_sensors41"))) SENSOR_41[] = {
/*41@0*/				0x50, 0x72, 0x65, 0x73, 0x73, 0x2E, 0x00, 0x00	//Press...
};

const uint8_t __attribute__((section (".s_sensors7d"))) SENSORS_7D[] = {
/*7c@0*/		0x4F, 0x64, 0x6F, 0x31, 0x00, 0x00, 0x00, 0x00,			//Odo1....
/*7d@8*/		0x4F, 0x64, 0x6F, 0x32, 0x00, 0x00, 0x00, 0x00,			//Odo2....
/*7e@16*/		0x20, 0x53, 0x70, 0x65, 0x00, 0x00, 0x00, 0x00,			// Spe....
/*7f@24*/		0x54, 0x78, 0x20, 0x56, 0x00, 0x00, 0x00, 0x00,			//Tx V....
};

const uint8_t __attribute__((section (".s_sensors80"))) SENSORS_80[] = {
/*80@0*/		0x43, 0x6F, 0x47, 0x00, 						  //CoG.
/*81@4*/		0x47, 0x50, 0x53, 0x00, 						  //GPS.
/*82@8*/		0x4C, 0x6F, 0x6E, 0x00, 						  //Lon.
/*83@12*/		0x4C, 0x61, 0x74, 0x00, 						  //Lat.
/*84@16*/		0x41, 0x6C, 0x74, 0x00, 						  //Alt.
/*85@20*/		0x73, 0x38, 0x35, 0x00, 						  //s85.
/*86@24*/		0x73, 0x38, 0x36, 0x00, 						  //s86.
/*87@28*/		0x73, 0x38, 0x37, 0x00, 						  //s87.
/*88@32*/		0x73, 0x38, 0x38, 0x00, 						  //s88.
/*89@36*/		0x73, 0x38, 0x39, 0x00, 						  //s89.
/*8a@40*/		0x73, 0x39, 0x30, 0x00, 						  //s8a.
/*8b@44*/		0x73, 0x39, 0x31, 0x00, 						  //s8b.
};

const uint8_t __attribute__((section (".s_sensorsF9"))) SENSORS_F9[] = {
/*f9@0*/		0x41, 0x6C, 0x74, 0x00, 0x00, 0x00, 0x00, 0x00,				//Alt....
/*fa@8*/		0x53, 0x4E, 0x52, 0x00, 0x00, 0x00, 0x00, 0x00,			  	//SNR....
/*fb@16*/		0x4E, 0x6F, 0x69, 0x73, 0x65, 0x00, 0x00, 0x00, 			//Noise...
/*fc@24*/		0x52, 0x53, 0x53, 0x49, 0x00, 0x00, 0x00, 0x00, 			//RSSI....
/*fd@32*/		0x53, 0x65, 0x6E, 0x46, 0x44, 0x00, 0x00, 0x00,				//SensFD..
/*fe@40*/		0x45, 0x72, 0x72, 0x6F, 0x72, 0x00, 0x00, 0x00,				//Error...
/*ff@48*/		0x4E, 0x6F, 0x6E, 0x65, 0x00, 0x00, 0x00, 0x00, 			//None....
};

const uint8_t __attribute__((section (".s_unknownSensor"))) UNKNOWN_SENSOR[] = {
				0x55, 0x6e, 0x6b, 0x6e, 0x6f, 0x77, 0x6e, 0x00,				//Unknown
};

const uint8_t __attribute__((section (".s_radioModes"))) RADIO_MODES[] = {
	0x49, 0x42, 0x55, 0x53, 0x2F, 0x50, 0x57, 0x4D, 0x00,						//IBUS/PWM.
	0x49, 0x42, 0x55, 0x53, 0x2F, 0x50, 0x50, 0x4D, 0x00,						//IBUS/PPM.
	0x53, 0x42, 0x55, 0x53, 0x2F, 0x50, 0x57, 0x4D, 0x00,						//SBUS/PWM
	0x53, 0x42, 0x55, 0x53, 0x2F, 0x50, 0x50, 0x4D, 0x00						//SBUS/PWM
};

const uint8_t __attribute__((section (".s_armed"))) ARMED[] = {0x41, 0x72, 0x6d, 0x65, 0x64}; //Armed

const uint8_t __attribute__((section (".s_flyModesOffset"))) flyModesOffset[] = {
	0, 5, 10, 16, 21, 28, 35, 39, 46, 52
};
const uint8_t __attribute__((section (".s_flyModes"))) FLY_MODES[] = {
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

const uint8_t __attribute__((section (".s_sensorsScreens"))) sensorsScreens[] = {
	IBUS_MEAS_TYPE_FLIGHT_MODE, IBUS_MEAS_TYPE_ARMED, IBUS_MEAS_TYPE_ERR, IBUS_MEAS_TYPE_RSSI, IBUS_MEAS_TYPE_NOISE, IBUS_MEAS_TYPE_SNR,
	IBUS_MEAS_TYPE_GPS_STATUS, IBUS_MEAS_TYPE_GPS_LAT, IBUS_MEAS_TYPE_GPS_LON, IBUS_MEAS_TYPE_GPS_ALT, IBUS_MEAS_TYPE_GPS_DIST, 0xFF,
	IBUS_MEAS_TYPE_TX_V, IBUS_MEAS_TYPE_INTV, IBUS_MEAS_TYPE_EXTV, IBUS_MEAS_TYPE_BAT_CURR, IBUS_MEAS_TYPE_TEM, IBUS_MEAS_TYPE_MOT,
	IBUS_MEAS_TYPE_CMP_HEAD, IBUS_MEAS_TYPE_COG, IBUS_MEAS_TYPE_ACC_X, IBUS_MEAS_TYPE_ACC_Y, IBUS_MEAS_TYPE_ACC_Z, IBUS_MEAS_TYPE_CLIMB_RATE,
	IBUS_MEAS_TYPE_SPE, IBUS_MEAS_TYPE_VERTICAL_SPEED, IBUS_MEAS_TYPE_GROUND_SPEED, IBUS_MEAS_TYPE_PRES, IBUS_MEAS_TYPE_ODO1, IBUS_MEAS_TYPE_ODO2
};
const uint8_t __attribute__((section (".s_timerFormat"))) timerFormat[] = {
		'%', '0', '2', 'u', ':', '%', '0', '2', 'u', ':', '%', '0', '2', 'u',  0x00
};
const uint8_t __attribute__((section (".s_timerNull"))) timerNull[] = {
		'0', '0', ':', '0', '0', ':', '0', '0', 0x00
};
const uint8_t __attribute__((section (".s_alarm"))) alarm[] = {
		'A', 'l', 'a', 'r', 'm',  0x00
};
const uint8_t __attribute__((section (".s_extraMenu"))) extraMenu[] = {
		'E', 'x', 't', 'r', 'a',  0x00
};
const uint8_t __attribute__((section (".s_altSensor"))) altSensor[] = {
		'A', 'L', 'T', ' ', 'S', 'e', 'n',  0x00
};
const uint8_t __attribute__((section (".s_txBat"))) txBat[] = {
		'T', 'X', ' ', 'B', 'a', 't',  0x00
};

const uint32_t __attribute__((section (".s_modMenuList"))) menuList[] = {
	TEXT_TIMMER, 0xFE29, 0xFF98, 0xFC61,  0xFFA0, 0xf821, 0xFFA8, 0xf821
};


/*
CSMMUUUU
C 1 -custom 0 std
S 1 -signed 0 unsigned
MM mulitplayed by 00=1 10=100
UUUU UNIT
 * */

const uint8_t __attribute__((section (".s_sensDesc00"))) sensorDesc00[] = {
		STD_SENSOR|UNSIGNED|MUL_100|UNIT_V,		//IntV
		STD_SENSOR|SIGNED__|MUL_001|UNIT_DEG, 	//Temp
		STD_SENSOR|UNSIGNED|MUL_001|UNIT_NONE, 	//Mot
		STD_SENSOR|UNSIGNED|MUL_100|UNIT_V,		//ExtV
		STD_SENSOR|UNSIGNED|MUL_001|UNIT_DEG,  	//Heading
		STD_SENSOR|UNSIGNED|MUL_100|UNIT_A,  	//Current
		STD_SENSOR|SIGNED__|MUL_100|UNIT_MS, 	//Climb
		STD_SENSOR|SIGNED__|MUL_100|UNIT_DEG, 	//Acc z
		STD_SENSOR|SIGNED__|MUL_100|UNIT_DEG, 	//Acc y
		STD_SENSOR|SIGNED__|MUL_100|UNIT_DEG, 	//Acc x
		STD_SENSOR|SIGNED__|MUL_100|UNIT_MS, 	//VSpeed
		STD_SENSOR|UNSIGNED|MUL_100|UNIT_MS, 	//Speed
		STD_SENSOR|UNSIGNED|MUL_001|UNIT_M,  	//Dist
		CUS_SENSOR|UNSIGNED|MUL_001|UNIT_NONE,  //Armed
		CUS_SENSOR|UNSIGNED|MUL_001|UNIT_NONE,  //Mode
		CUS_SENSOR|UNSIGNED|MUL_001|UNIT_NONE,  //Model
		CUS_SENSOR|UNSIGNED|MUL_001|UNIT_NONE,  //Reserv1
		CUS_SENSOR|UNSIGNED|MUL_001|UNIT_NONE,  //Reserv2
};
//0x41 is custom

const uint8_t __attribute__((section (".s_sensDesc80"))) sensorDesc80[] = {
		STD_SENSOR|UNSIGNED|MUL_001|UNIT_DEG,	//CoG.
		CUS_SENSOR|UNSIGNED|MUL_001|UNIT_NONE,	//GPS.
		CUS_SENSOR|SIGNED__|MUL_001|UNIT_NONE,	//Lon.
		CUS_SENSOR|SIGNED__|MUL_001|UNIT_NONE,	//Lat.
		STD_SENSOR|SIGNED__|MUL_100|UNIT_M,		//Alt -> GPS alt
		STD_SENSOR|SIGNED__|MUL_001|UNIT_NONE,	//s85
		STD_SENSOR|SIGNED__|MUL_001|UNIT_NONE,	//s86
		STD_SENSOR|SIGNED__|MUL_001|UNIT_NONE,	//s87
		STD_SENSOR|SIGNED__|MUL_100|UNIT_NONE,	//s88
		STD_SENSOR|SIGNED__|MUL_100|UNIT_NONE,	//s89
		STD_SENSOR|SIGNED__|MUL_100|UNIT_NONE,	//s8a
		STD_SENSOR|SIGNED__|MUL_100|UNIT_NONE,	//s8a
};

const uint8_t __attribute__((section (".s_sensDescF9"))) sensorDescF9[] = {
		STD_SENSOR|SIGNED__|MUL_001|UNIT_M,		//Alt //maybe *100
		STD_SENSOR|UNSIGNED|MUL_001|UNIT_DB,	//SNR
		STD_SENSOR|UNSIGNED|MUL_001|UNIT_DBM,	//Noise
		STD_SENSOR|UNSIGNED|MUL_001|UNIT_DBM,	//RSSI
		STD_SENSOR|UNSIGNED|MUL_001|UNIT_NONE,	//SensFD
		STD_SENSOR|UNSIGNED|MUL_001|UNIT_PROC,	//Error
		STD_SENSOR|UNSIGNED|MUL_001|UNIT_NONE,	//None
};


const uint8_t __attribute__((section (".s_foramtCoord"))) formatCoord[] = { 0x25,0x75, 0x7f, 0x25, 0x30, 0x32, 0x75, 0x27, 0x25, 0x30, 0x32, 0x75,0x2e, 0x25, 0x30, 0x33, 0x75, 0x22, 0x00};//"%u°%02u'%02u.%u\"";
const uint8_t __attribute__((section (".s_formatGPS"))) formatGPS[] = {0x25, 0x75, 0x20, 0x25, 0x30, 0x32, 0x75, 0x00};// "%u %02u";
const uint8_t __attribute__((section (".s_formatNumber"))) formatNumber[] = {0x25, 0x75, 0x00}; //"%u";
const uint8_t __attribute__((section (".s_formatNumberFractial"))) formatNumberFractial[] = {0x25, 0x75, 0x2e, 0x25, 0x30, 0x32, 0x75, 0x00};// "%u.%02u";

const uint8_t __attribute__((section (".s_signature"))) SIGNATURE[] = {
		0xCC, 0x51 , 0x72, 0xC8, 0xA0, 0x7E, 0x01, 0x92, 0xF0, 0xE7, 0x00, 0x00, 0x0A, 0x00, 0x01, 0x00
};


//if unit > 0 sub 1
const uint8_t __attribute__((section (".s_unitsOffsets"))) unitsOffsets[] = {
	0, 2, 4, 6, 8, 10, 14, 18, 21
};


const uint8_t __attribute__((section (".s_units"))) units[] = {
	/*1@0*/  0x6d, 0x00,						//m;
	/*2@2*/  0x7f, 0x00,						//°
	/*3@4*/ 0x41, 0x00,							//A
	/*4@6*/ 0x25, 0x00,						//%
	/*5@8*/ 0x56, 0x00,						//V
	/*6@10*/  0x6d, 0x2f, 0x73, 0x00,			//m/s
	/*7@14*/ 0x64, 0x42, 0x6d, 0x00,			//dBm
	/*8@18*/  0x64, 0x42, 0x00,					//dB
	/*9@21*/ 0x6B, 0x6d, 0x2f, 0x68, 0x00,		//km/h
};

__attribute__((section (".s_MOD_SPACE"))) signed int  auxChannelsPage();





#endif /* SOURCE_MOD_H_ */
