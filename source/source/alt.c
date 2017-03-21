/*
 * alt.c
 *
 *  Created on: 18 lut 2017
 *      Author: Kuba
 */


#include "alt.h"
#include "flysky.h"
#include "ibustelemetry.h"
#include "mod.h"
#include "print.h"


void add2ByteSensor(uint8_t sensorID, uint8_t sensorIndex, uint16_t value){
	char shortSensor[4];
	shortSensor[0] = sensorID;
	shortSensor[1] = sensorIndex;
	shortSensor[2] = value & 0xFF;
	shortSensor[3] = value >> 8 & 0xFF;
	setSensorValue(shortSensor);
}

#pragma GCC optimize ("Os")
/*
 * getALT(sensorData & 0x7FFFF)
 * */
void acData(uint8_t* rxBuffer){
	uint32_t sensorData = 0;
	uint8_t sensorIndex = 0;
	int index = 1;
	int i = 0;
	int tmp = 0;

	do{
		//ID|Index|Length|Data
		sensorIndex = rxBuffer[index +1];
		sensorData = (rxBuffer[index+6] << 24 | rxBuffer[index+5] << 16 | rxBuffer[index+4] << 8 | rxBuffer[index+3]);

		if(rxBuffer[index]==IBUS_MEAS_TYPE_GPS_FULL){
			add2ByteSensor(IBUS_MEAS_TYPE_GPS_STATUS, sensorIndex, rxBuffer[index+4] << 8 | rxBuffer[index+3]);
			tmp = index+5;
			for(i=0; i <3; i++){
				int32_t offset = i+2;
				sensorData = rxBuffer[tmp] | rxBuffer[tmp+1] << 8 | rxBuffer[tmp+2] << 16 | rxBuffer[tmp+3] << 24;
				longSensors[offset] = sensorData; //0 timer 1 pressure
				add2ByteSensor(IBUS_MEAS_TYPE_GPS_LON + i, sensorIndex, offset);
				tmp+=4;
			}
		}
		else if(rxBuffer[index]==IBUS_MEAS_TYPE_PRES || (rxBuffer[index] >= IBUS_MEAS_TYPE_GPS_LON && rxBuffer[index] <= IBUS_MEAS_TYPE_S92)){
			int offset = rxBuffer[index] - IBUS_MEAS_TYPE_GPS_LON + 2;
			if(offset < 0) offset = 0;
			longSensors[offset] = sensorData;
			add2ByteSensor(rxBuffer[index], sensorIndex, offset);
		}
		index+=rxBuffer[index + 2] + 3;
	}
	while (index <= 28 );
}

void loadModSettings(){
	settingsValidation();
	readEeprom(modConfig.byteConfig, 0xBC0, sizeof(modConfig));
}

void saveModSettings(){
	saveEeprom(modConfig.byteConfig, 0xBC0, sizeof(modConfig));
	someSPImethod();
}


const uint8_t* getSensorName(int sensor){
	if ( sensor >= 0 && sensor <= 255 )
	{
		if(sensor <= 20) return SENSOR_00 + (sensor << 3);
	    if (sensor == 65) return SENSOR_41;
	    if ( sensor < 124 ) return (const uint8_t*)UNKNOWN_SENSOR;

	    if ( sensor <= 127 ){
	    	sensor = sensor - 124;
	    	return SENSORS_7D + (sensor * 8);
	    }
	    if ( sensor <= 139 ){ //SHORT sensors 4 bytes
	    	sensor = sensor - 128;
	        return SENSORS_80 + (sensor * 4);
	    }
	    if ( sensor >= 249)
	    {
	    	sensor = sensor - 249;
	    	return SENSORS_F9 + (sensor * 8);
	    }
	 }
	 return (const uint8_t*)UNKNOWN_SENSOR;
}

uint32_t divMod(uint32_t val, uint32_t divisor, uint32_t* mod){
	uint32_t result = uidivmod(val, divisor);
	__asm("mov %[output], r1" : [output] "=r" (*mod));
	return result;
}


void printTimer(int32_t skipPrint){
	uint32_t now = *(uint32_t *)(0xE000E018);
	uint32_t diff = 0;
	uint32_t h = 0;
	uint32_t m = 0;
	uint32_t s = 0;
	if(longSensors[0] > 0 || isTimerActive()){
		if(timerValue == 0) timerValue = *(uint32_t *)(0xE000E018);
		else{
			diff = (now - timerValue) & 0xFFFFFF;
			if(diff > 480008){ //3529827,038475115 0x35DC63
				//0.00002083ms
				longSensors[1]++; //+100ms
				//1 tick = 0,208 us 0.02083us
				//1000 us = 1ms
				//1s = 1000000us
				//100ms = 100000us
				//1000 ms = 1s

				//100 ms = 100000 us =
				if(longSensors[1]>=10){
					longSensors[1]-=10;
					longSensors[0]++;
					h = divMod(longSensors[0], 3600, &m);
					m = divMod(m, 60, &s);
					timerValue = *(uint32_t *)(0xE000E018);
					sprintfCall((char*)timerBuffer, (const char*)timerFormat, h,m,s);
				}
			//if(diff > 0x2DC347){


			}
		}

	}
	if(timerBuffer[0]==0) strcatCall((char*)timerBuffer, (const char*)timerNull);
	if(skipPrint ==0)displayTextAt((char*)timerBuffer, 1, 0, 0);
}

uint32_t isTimerActive(){
	uint8_t channel = modConfig.cfg.timerCH;
	if(channel > 16) return 0;
	uint16_t value	= modConfig.cfg.timerStart;
	if(value > 2000) return 0;

	uint32_t address = 0x1FFFFD9C;
	if(*(uint8_t *)(address) == 0x58){
		address++;
		channel -=1;
		address += 2*channel;
		uint16_t val = (uint16_t)(*(uint8_t *)address) | ((uint16_t)(*(uint8_t *)(address+1)) << 8);
		if(val > value) return 1;
	}
	return 0;

	/*


	if(*(uint8_t *)(address) == 0x58){
		address++;
		channel -=1;
		address += 2*channel;
		uint16_t val = (uint16_t)(*(uint8_t *)address) | ((uint16_t)(*(uint8_t *)(address+1)) << 8);
		if(val > 1100){
			return 1;
		}
	}
	return 0;*/
}


/*Belongs to .s_modMenu 0xFFA0 */
void displayMenu(){
	showNavPage((const char*) extraMenu, 4, (manuEntry*)menuList);
}

void BatteryType() {
	uint32_t key = 0;
	uint8_t batteryType = modConfig.cfg.batteryType;
	uint32_t typeName;
		while (1) {
			callSetupDMAandSend();
			displayPageHeader((char*)0xFFA8);
			if(batteryType !=0){
				typeName = 0xF4A8; //NiMH
			}
			else typeName = 0xF4A0; //lipo
			displayTextAt((char*)typeName, 8, 32,0);
			LCD_updateCALL();
			key = getKeyCode();
			if(key == KEY_SHORT_UP || key == KEY_LONG_UP){
				batteryType++;
				if(batteryType > 1) batteryType = 0;
			}
			else if(key == KEY_SHORT_DOWN || key == KEY_LONG_DOWN){
				if(batteryType ==0)batteryType = 1;
				else batteryType--;
			}
			else break;

		}

	if( key == KEY_LONG_CANCEL) {
		modConfig.cfg.batteryType = batteryType;
		saveModSettings();
	}
}
uint32_t GetBatteryVoltage(){
	uint32_t voltage = 4200;
	if(!modConfig.cfg.batteryType) voltage -= 400;
	return voltage;
}


void AlarmConfig(){
	sensorAlarm alarms[3];
	uint32_t row = 0;
	uint32_t column = 0;
	uint32_t columnOffset = 0;
	uint32_t key = 0;
	uint16_t step = 1;
	uint16_t y = 0;
	sensorAlarm alarmItem;
	char buffer[32];
	for(int i = 0; i < 3; i++){
		alarms[i].operator = modConfig.cfg.alarm[i].operator;
		alarms[i].sensorID = modConfig.cfg.alarm[i].sensorID;
		alarms[i].value = modConfig.cfg.alarm[i].value;
	}
	do{
		 while ( 1 )
		 {
			 callSetupDMAandSend();
			 displayPageHeader((char*)alarm);
			 for(int i = 0; i < 3; i++){
				y = 16+i*8;
				alarmItem = alarms[i];
				displayTextAt((char*)getSensorName(alarmItem.sensorID), 9, y,0);
				buffer[0] = 0;
				buffer[1] = 0;
				if(alarmItem.operator == 0) buffer[0] = '<';
				if(alarmItem.operator == 1) buffer[0] = '>';
				displayTextAt(buffer, 56, y,0);
				buffer[0] = 0;
				formatSensorValue(buffer, alarmItem.sensorID, alarmItem.value);
				displayTextAt(buffer, 72, y,0);
			 }
 			 columnOffset = 1;
 			 if(column==1) columnOffset = 48;
 			 if(column==2) columnOffset = 64;

			 displayGFX((gfxInfo*) GFX_ARROW, columnOffset, 16 + (row*8));

			 LCD_updateCALL();
			 key = getKeyCode();
			 if(key < 20) step =1;
			 else step = 10;
			 if(key == KEY_LONG_OK || key == KEY_SHORT_OK ){
				 column++;
				 if(column >= 3) {
					 column = 0;
					 row++;
				 }
				 if(row >=3)row = 0;
			 }
			 else if(key == KEY_SHORT_UP || key == KEY_LONG_UP){
				 if(column == 2){
					 alarms[row].value += step;
				 }
				 if(column == 1) {
					 if(alarms[row].operator != 0) alarms[row].operator = 0;
					 else alarms[row].operator = 1;
				 }
				 if(column == 0){
					 alarms[row].sensorID += 1;
					 if(alarms[row].sensorID == IBUS_MEAS_TYPE_RESERV_2 + 1) alarms[row].sensorID = IBUS_MEAS_TYPE_ALT;
				 }
			 }
			 else if(key == KEY_SHORT_DOWN || key == KEY_LONG_DOWN) {
				 if(column == 0){
					 alarms[row].sensorID -= 1;
					 if(alarms[row].sensorID == IBUS_MEAS_TYPE_ALT -1) alarms[row].sensorID = IBUS_MEAS_TYPE_RESERV_2;
				 }
				 if(column == 2){
					 alarms[row].value -= step;
				 }
				// if(column == 1) alarms[row].operator ^= alarms[row].operator;
			 }
			 else  {
				 break;
			 }
		 }

	}
	while ( key != KEY_LONG_CANCEL && key != KEY_SHORT_CANCEL);

	if( key == KEY_LONG_CANCEL) {
		for(int i = 0; i < 3; i++){
			modConfig.cfg.alarm[i].operator = alarms[i].operator;
			modConfig.cfg.alarm[i].sensorID = alarms[i].sensorID;
			modConfig.cfg.alarm[i].value = alarms[i].value;
		}
		saveModSettings();
	}

}


void TimerConfig(){
	uint16_t data[3];
	uint16_t max[3];
	data[0] = (uint16_t) modConfig.cfg.timerCH;
	data[1] = modConfig.cfg.timerStart;
	data[2] = modConfig.cfg.timerAlarm;
	max[0] = 10;
	max[1] = 2200;
	max[2] = 0xffff;

	uint8_t navPos = 0;
	uint32_t key = 0;
	uint16_t step = 1;

	char buffer[32];
	do
	 {
	    while ( 1 )
	    {
	      callSetupDMAandSend();
	      displayPageHeader((char*)TEXT_TIMMER);
	      displayTextAt((char*)TEXT_CHANNEL, 9, 16,0);
	      displayTextAt((char*)TEXT_VALUE, 9, 24,0);
	      displayTextAt((char*)alarm, 9, 32,0);

	      for(int i = 0; i < 3; i++){
	    	  sprintfCall(buffer, (const char*) formatNumber, data[i]);
	    	  displayTextAt(buffer, 64, 16 + i*8,0);
	      }
	      displayGFX((gfxInfo*) GFX_ARROW, 1, 16 + navPos*8);

	      LCD_updateCALL();
	      key = getKeyCode();

	      step = 1;
	      if(navPos != 0) step = 10;

	      if(key == KEY_LONG_OK || key == KEY_SHORT_OK ){
	    	  navPos++;
	    	  if(navPos >=3)navPos = 0;
	      }
	      else if(key == KEY_SHORT_UP || key == KEY_LONG_UP){
	    	  data[navPos] += step;
	    	  if(data[navPos] >= max[navPos]) data[navPos] = max[navPos];
	      }
	      else if(key == KEY_SHORT_DOWN || key == KEY_LONG_DOWN) {
	    	  if(data[navPos] > step) data[navPos] -= step;
	      }
	      else  {
	    	  break;
	      }
	      key = someBeepCheck();
	      if ( key >= 2 )
	      {
	        beep(784, 15);
	        beep(0, 15);
	      }
	    }
	  }
	  while ( key != KEY_LONG_CANCEL && key != KEY_SHORT_CANCEL);
	  if( key == KEY_LONG_CANCEL) {
		modConfig.cfg.timerCH = (uint8_t) data[0];
		modConfig.cfg.timerStart = data[1];
		modConfig.cfg.timerAlarm = data[2];
		saveModSettings();
	  }
}

void parseCoord(uint32_t *deg, uint32_t *min, uint32_t *sec, uint32_t *subSec, uint32_t coord)
{
	uint32_t tmp = 0;
	*deg = divMod(coord, 10000000, &tmp);
	coord = tmp * 60;
	*min = divMod(coord, 10000000, &tmp);
	coord = tmp * 60;
	*sec = divMod(coord, 10000000, &tmp);
	*subSec = div_(tmp, 10000);
}

void formatSensorValue(char* target, int sensorID, uint16_t sensorValue) {
	const char* format = (const char*) formatNumber;
	const char* unit = 0;
	uint32_t result = (uint32_t) sensorValue;
	uint8_t negative = 0;
	uint32_t result2 = 0;
	uint32_t result3 = 0;
	uint32_t result4 = 0;
	int32_t tmp = 0;

	uint8_t sensorInfo = STD_SENSOR | UNSIGNED | MUL_001 | UNIT_NONE;

	if (sensorID >= 255) return;
	if (sensorID <= IBUS_MEAS_TYPE_RESERV_2) {
		sensorInfo = sensorDesc00[sensorID];
		if ((sensorInfo & SIGNED__) != 0 && ((int16_t) sensorValue) < 0) {
			negative = 1;
			result = (uint32_t) -((int16_t) sensorValue);
		}
	} else if (sensorID == IBUS_MEAS_TYPE_PRES) {
		if (sensorValue < 18)
			result = longSensors[sensorValue];
	} else if (sensorID >= IBUS_MEAS_TYPE_COG && sensorID <= IBUS_MEAS_TYPE_S92) {
		sensorInfo = sensorDesc80[sensorID - IBUS_MEAS_TYPE_COG];
		if (sensorID >= IBUS_MEAS_TYPE_GPS_LON && sensorValue < 18) {
			result = longSensors[sensorValue];
		}
		if ((sensorInfo & SIGNED__) != 0 && ((int32_t) result) < 0) {
			negative = 1;
			result = (uint32_t) -((int32_t) result);
		}
	} else if (sensorID >= IBUS_MEAS_TYPE_ALT) {
		sensorInfo = sensorDescF9[sensorID - IBUS_MEAS_TYPE_ALT];
		if (sensorID == IBUS_MEAS_TYPE_RSSI || sensorID == IBUS_MEAS_TYPE_NOISE) {
			negative = 1;
		}
	}
	if ((sensorInfo & MUL_100) == MUL_100) {
		format = (const char*) formatNumberFractial;
		result = divMod(result, 100, &result2);
	}

	if ((sensorInfo & CUS_SENSOR) != CUS_SENSOR) {
		if ((sensorInfo & 0xF) != UNIT_NONE) {
			unit = (const char*)(units) +unitsOffsets[(sensorInfo & 0xF)-1];
		}

	} else {
		switch (sensorID) {
		case IBUS_MEAS_TYPE_GPS_STATUS:
			result2 = result >> 8 & 0xFF;
			result &= 0xFF;
			format = (const char*) formatGPS;
			break;
		case IBUS_MEAS_TYPE_ARMED:
			if (sensorValue) {
				target[0] = 'U'; //U
				target[1] = 'n'; //n
				target += 2;
			}
			strcatCall(target, (const char *) ARMED);
			return;
		case IBUS_MEAS_TYPE_FLIGHT_MODE:
			if (result <= 9) strcatCall(target, (const char *) FLY_MODES + (flyModesOffset[result]));
			else strcatCall(target, (const char *) UNKNOWN_SENSOR);
			return;
		case IBUS_MEAS_TYPE_GPS_LON:
		case IBUS_MEAS_TYPE_GPS_LAT:
			format = (const char*) formatCoord;
			tmp = result;
			parseCoord(&result, &result2, &result3, &result4, tmp);
			break;
		}
	}
	if (negative) {
		target[0] = '-';
		target++;
	}
	sprintfCall(target, format, result, result2, result3, result4);
	while(*target!=0) target++;
	if(unit != 0) strcatCall(target, unit);
}

void displaySensors(){
	char buffer[64];
	uint32_t bufferOffset = 0;
	uint8_t batteryID = IBUS_MEAS_TYPE_INTV;
	const uint8_t* settings = (uint8_t*)((*(uint32_t *)(USED_MODEL_PTR)));
	uint32_t batteryType = (uint32_t)settings[2];
	int32_t timer = ((*(int32_t *)(TIMER_SYS_TIM)));
	int32_t telemetryUpdateTimer = ((*(int32_t *)(MEM_20000000 + 0x1C)));
	uint8_t sensorCount = 3;
	uint32_t yPos = 34;
	uint32_t xPos = 9;
	if(timer - telemetryUpdateTimer > 1000 && mainScreenIndex < 2) {

		return;
	}

	settings += 0x60; //AFHDS2
	if (mainScreenIndex < 2) {
		batteryType = batteryType << 30;
		batteryType = batteryType >> 31;
		if ((batteryType & 1) == 1) { //external
			batteryID = IBUS_MEAS_TYPE_EXTV; //external bat
		}
		formatSensorData(batteryID, 0, buffer);
		if (buffer[0] != 0x00) {
			while (buffer[bufferOffset] != 0)
				bufferOffset++;
			displayTextAt(buffer, 128 - (bufferOffset * 8), 9, 0);
		}
		formatSensorData(IBUS_MEAS_TYPE_SNR, 0, buffer);
		if (buffer[0] != 0x00)
			displayTextAt(buffer, 0, 9, 0);
	}
	if(*settings == 0) return; //check AFHDS2

	settings += 33; // settings[129]
	if (mainScreenIndex != 0) {
		if (mainScreenIndex == 1) {
			settings += 3;
		}
		else {
			settings = sensorsScreens + (6* (mainScreenIndex-2));
			sensorCount = 6;
			yPos = 1;
			xPos = 1;
		}
	}


	for (int i = 0; i < sensorCount; i++) {
		if (*settings != 0xFF) {
			strcatCall(buffer, (const char*) getSensorName(*settings));
			bufferOffset = 0;
			while (buffer[bufferOffset] != 0) bufferOffset++;
			buffer[bufferOffset++] = ' ';
			buffer[bufferOffset] = 0;
			if(*settings >= IBUS_MEAS_TYPE_GPS_LON && *settings <= IBUS_MEAS_TYPE_GPS_ALT){
				formatSensorValue(buffer + bufferOffset, *settings, *settings - 0x80);
			}
			else{
				formatSensorData(*settings, 0, buffer + bufferOffset);
			}

			while (buffer[bufferOffset] != 0) bufferOffset++;
			displayTextAt(buffer, xPos, (i * 8) + yPos, 0);
		}
		settings++;
	}

}

uint32_t mulu16(uint32_t x, uint32_t y)
{
	uint32_t a = x >> 16;
	uint32_t b = x & 0xFFFF;
	uint32_t c = y >> 16;
	uint32_t d = y & 0xFFFF;
	return ((d * b) >> 16) + (d * a) + (c * b)  + ((c * a) << 16);
}


int32_t log2fix(uint32_t x, size_t precision)
{
	int32_t b = 1U << (precision - 1);
	int32_t y = 0;

	if (precision < 1 || precision > 31) return INT32_MAX; // indicates an error
	if (x == 0)	return INT32_MIN; // represents negative infinity

	while (x < 1U << precision) {
		x <<= 1;
		y -= 1U << precision;
	}

	while (x >= 2U << precision) {
		x >>= 1;
		y += 1U << precision;
	}

	uint32_t z = x;

	for (size_t i = 0; i < precision; i++) {
		z = z * z >> precision;
		if (z >= 2U << precision) {
			z >>= 1;
			y += b;
		}
		b >>= 1;
	}

	return y;
}

int32_t logfix(uint32_t x, size_t precision)
{
	uint32_t tmp;
	int32_t logResult = log2fix(x, precision);
	uint8_t negative = 0;

	if (logResult < 0) {
		negative = 1;
		logResult = -1;
	}
	tmp = mulu16(logResult, INV_LOG2_E_Q1DOT31);
	tmp = (tmp >> 15);

	if (negative) {
		return (-1) * tmp;
	}
	return (int32_t)tmp;
}

void init(uint32_t pressure) {
	//constVal = mul(/*R_div_g*/ R_div_g_19_13, /*Tmp_24C*/ Tmp_24C_19_13, PRECISION);
	//constVal = R_div_G_MUL_AVG_TEMP_19_13;
	constVal = R_div_G_MUL_AVG_TEMP_19_13_8_C;
	initPressure = FIXED(pressure);
	initPressureRaw = pressure;
}

int16_t getALT(uint32_t pressure) {
	if(pressure == 0) return 0;

	if (initPressure == 0) {
		init(pressure);
	}

	uint32_t tmp = FIXED(pressure);
	uint8_t negative = 0;
	if (initPressureRaw > pressure) {
		//val is already shifted by PRECISION
		tmp =  div_(initPressure,pressure);
		//tmp = (int32_t)(((int64_t)initPressure << PRECISION) / ((int64_t)tmp));
	}
	else {
		tmp = div_(tmp, initPressureRaw);
		//tmp = (int32_t)(((int64_t)tmp << PRECISION) / ((int64_t)initPressure));
		negative = 1;
	}
	tmp = logfix(tmp, PRECISION);
	uint32_t res = mulu16(tmp, constVal);

	//real CD3260DD
	//this result CD32
	if (PRECISION < 16) {
		res = res << (16 - PRECISION);
	}
	else if (PRECISION > 16) {
	//	res =  res >> (PRECISION - 16);
	}
	//rounding
	//res = res + ((res & 1 << (PRECISION - 1)) << 1);
	int16_t h = res >> PRECISION;

	if (negative) h = -h;
	return h;
}
#pragma GCC optimize ("O1")

