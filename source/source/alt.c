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
#include "fsl_gpio.h"
#include "fsl_port.h"
#define OPERATOR_LT 0
#define OPERATOR_GT 1

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
	int offset = -1;

	do{
		//ID|Index|Length|Data
		offset = -1;
		sensorIndex = rxBuffer[index +1];
		sensorData = (rxBuffer[index+6] << 24 | rxBuffer[index+5] << 16 | rxBuffer[index+4] << 8 | rxBuffer[index+3]);

		if(rxBuffer[index]==IBUS_MEAS_TYPE_GPS_FULL){
			add2ByteSensor(IBUS_MEAS_TYPE_GPS_STATUS, sensorIndex, rxBuffer[index+4] << 8 | rxBuffer[index+3]);
			tmp = index+5;
			for(i=0; i <3; i++){
				sensorData = rxBuffer[tmp] | rxBuffer[tmp+1] << 8 | rxBuffer[tmp+2] << 16 | rxBuffer[tmp+3] << 24;
				longSensors[i] = sensorData;
				add2ByteSensor(IBUS_MEAS_TYPE_GPS_LAT + i, sensorIndex, i);
				tmp+=4;
			}
		}
		else if(rxBuffer[index]==IBUS_MEAS_TYPE_VOLT_FULL){
			tmp = index+3;
			for(i=IBUS_MEAS_TYPE_EXTV; i <=IBUS_MEAS_TYPE_RPM; i++){
				add2ByteSensor(i, sensorIndex, rxBuffer[tmp+1] << 8 | rxBuffer[tmp]);
				tmp+=2;
			}
		}
		else if(rxBuffer[index]==IBUS_MEAS_TYPE_ACC_FULL){
			tmp = index+3;
			for(i=IBUS_MEAS_TYPE_ACC_X; i <=IBUS_MEAS_TYPE_YAW; i++){
				add2ByteSensor(i, sensorIndex, rxBuffer[tmp+1] << 8 | rxBuffer[tmp]);
				tmp+=2;
			}
		}
		else if(rxBuffer[index]==IBUS_MEAS_TYPE_PRES){
			offset = SENSORS_ARRAY_LENGTH - 1;
		}
		else if(rxBuffer[index] >= IBUS_MEAS_TYPE_GPS_LAT && rxBuffer[index] <= IBUS_MEAS_TYPE_S8a){
			offset = rxBuffer[index] - IBUS_MEAS_TYPE_GPS_LAT;
		}
		if(offset >= 0){
			longSensors[offset] = sensorData;
			add2ByteSensor(rxBuffer[index], sensorIndex, offset);
		}
		index+=rxBuffer[index + 2] + 3;
	}
	while (index <= 28 );
}

void loadModSettings(){
	settingsValidation();
	readEeprom((uint8_t *)&modConfig, 0xBC0, sizeof(modConfig));
}

void saveModSettings(){
	saveEeprom((uint8_t *)&modConfig, 0xBC0, sizeof(modConfig));
	someSPImethod();
}


const uint8_t* getSensorName(int sensor){
	if ( sensor >= IBUS_MEAS_TYPE_INTV && sensor <= IBUS_MEAS_TYPE_UNKNOWN )
	{
		if(sensor <= IBUS_MEAS_TYPE_FLIGHT_MODE) return SENSOR_00 + (sensor * 5);
	    if (sensor == IBUS_MEAS_TYPE_PRES) return SENSOR_41;
	    if ( sensor < IBUS_MEAS_TYPE_ODO1 ) return (const uint8_t*)UNKNOWN_SENSOR;

	    if ( sensor <= IBUS_MEAS_TYPE_TX_V ){
	    	sensor = sensor - IBUS_MEAS_TYPE_ODO1;
	    	return SENSORS_7D + (sensor * 5);
	    }
	    if (sensor <= IBUS_MEAS_TYPE_S8a ){
	    	sensor = sensor - IBUS_MEAS_TYPE_GPS_LAT;
	        return SENSORS_80 + (sensor * 5);
	    }
	    if ( sensor >= IBUS_MEAS_TYPE_SNR)
	    {
	    	sensor = sensor - IBUS_MEAS_TYPE_SNR;
	    	return SENSORS_FA + (sensor * 5);
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
	if(timerValue > 0 || isTimerActive()){
		if(lastTimerUpdate == 0) lastTimerUpdate = *(uint32_t *)(0xE000E018);
		else{
			diff = (now - lastTimerUpdate) & 0xFFFFFF;
			if(diff > 480008){ //3529827,038475115 0x35DC63
				//0.00002083ms
				ticks100MS++; //+100ms
				//1 tick = 0,208 us 0.02083us
				//1000 us = 1ms
				//1s = 1000000us
				//100ms = 100000us
				//1000 ms = 1s

				//100 ms = 100000 us =
				if(ticks100MS>=10){
					ticks100MS-=10;
					timerValue++;
					h = divMod(timerValue, 3600, &m);
					m = divMod(m, 60, &s);
					lastTimerUpdate = *(uint32_t *)(0xE000E018);
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
	uint8_t channel = modConfig.timerCH;
	if(channel > 16) return 0;
	uint16_t value	= modConfig.timerStart;
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
#define SW_A 0u
#define SW_B 1u
#define SW_C 2u
#define SW_D 3u
#define SW_E 6u
int getAuxChannel(uint32_t request){
	int sw1 = 0;
	int sw2 = 0;
	uint8_t swB_3pos = modConfig2.swConfig & 1;
	uint8_t varC = modConfig2.swConfig & 2;
	if(request == 0) return 0;
	if(request == 15){
		return getSWState(SW_E);
	}
	if(request > 11){
		//12 6 -> result 	0
		//13 5				4
		//14 4				8
		//15 3				12
		//16 2				16
		//17 1				20
		request -= 12;
		request *=4;
		return  *(int32_t *)(PPM_IN_BUFFER_CH6 - request);
	}
	if(request == 11){ //error
		sw1 = getSensorValue(254, 0,0);
		if(sw1 == 0x8000) sw1 = -10000;
		else if(sw1 > 50) sw1 = -200* (sw1 - 50);
		else sw1 = 200 * (50 - sw1);
		return sw1;
	}
	else if(request == 7){ // A+B
		if(swB_3pos != 0){
			sw1 = SW_B;
			sw2 = SW_A;
			goto TreePos;
		}
		else{
			sw1 = SW_A;
			sw2 = SW_B;
			goto TwoPos;
		}
	}
	else if(request == 10){ // A+D or E
		/*
		 uint32_t portD = ((*(uint32_t *)(0x400FF0D0)));
		if ( !(portD & 0x4))  return 10000;
		return -10000;
		 */
		sw1 = SW_A;
		sw2 = SW_D;
		goto TwoPos;
	}
	else if(request == 8){ // B+C
		sw1 = SW_C;
		sw2 = SW_B;
		if(swB_3pos != 0) goto NinePos;
		goto TreePos;
	}
	else if(request == 9){ // C+D
		sw1 = SW_C;
		sw2 = SW_D;
		goto TreePos;
	}
	else if ( request > 2 ){
		//swA = 3  swB = 4 swC = 5 swD = 6
		if(request == 5 && varC){
			sw1 = ADC_SW_C - ADC_VAR_A;
			goto VarCalc;
		}
      return getSWState(request - 3);
	}
	else { //VAR A VAR B
		sw1 = (request -1) << 2;
		VarCalc:
		sw1 = *(int32_t *)(ADC_VAR_A + sw1);
		sw1 *= 10000;
		sw1 = sw1 >> 11;
		sw1 -= 10000;
		return sw1;
	}

	TwoPos:
	sw1 = getSWState(sw1);
	sw2 = getSWState(sw2) *2;
	return div_(sw1 + sw2, 3);
	TreePos:
	sw1 = getSWState(sw1) *2;
	sw2 = getSWState(sw2) *3;
	return div_(sw1 + sw2, 5);
	NinePos:
	sw1 = getSWState(sw1);
	sw2 = getSWState(sw2) *3;
	return div_(sw1 + sw2, 4);
	return sw2;
}

int getSWState(uint32_t swIndex){
	int result = 0;
	uint32_t states = *((uint32_t *)INPUT_STATES);
	if ( (1 << (swIndex + 16)) & states )
	{
	    result = 10000;
	}
	else{
		switch(swIndex){
			case SW_C:
				if(states & 0x100000u) result = -10000;
				break;
			case SW_B:
				if(states & 0x200000u) result = -10000;
				break;
			default:
				result = -10000;
		}
	}
	return result;
}




/*Belongs to .s_modMenu 0xFFA0 */
void displayMenu(){
	showNavPage((const char*) extraMenu, 4, (manuEntry*)menuList);
}

void BatteryType() {
	uint32_t key = 0;
	uint16_t batteryVolt = modConfig.batteryVoltage;
	char buffer[32];
		while (1) {
			callSetupDMAandSend();
			if(batteryVolt > 900) batteryVolt = 0;
			displayPageHeader((char*)0xFFA8);
			displayTextAt((char*)0xFF98, 8, 24,0);
			formatSensorValue(buffer, IBUS_MEAS_TYPE_INTV, batteryVolt);
			displayTextAt((char*)buffer, 8, 32,0);
			LCD_updateCALL();
			key = getKeyCode();
			if(key == KEY_SHORT_UP) batteryVolt++;
			else if(key == KEY_LONG_UP)	batteryVolt+=50;
			else if(key == KEY_SHORT_DOWN) batteryVolt--;
			else if(key == KEY_LONG_DOWN)batteryVolt-=50;
			else break;

		}

	if( key == KEY_LONG_CANCEL) {
		modConfig.batteryVoltage = batteryVolt;
		saveModSettings();
	}
}

 void ChackCustomAlarms(){
	 int32_t timer = ((*(int32_t *)(TIMER_SYS_TIM)));
	 int32_t lastAlarm = *((int32_t *)LAST_ALARM_TIMER);
	 int32_t lastTelemetryUpdate = *((int32_t *)TELEMETRY_UPDATE_TIMER);
	 uint8_t beepCount = 0;
	 uint8_t sensorID = 0;

	 if(timer < 100 ) return;

	 if((uint32_t)modConfig.timerAlarm != 0 && timerValue > (uint32_t)modConfig.timerAlarm) {
		 beepCount = 1;
	 }

	 if(timer - lastTelemetryUpdate < 1000){
		 for(int i = 0; i < 3; i++){
			sensorID = modConfig.alarm[i].sensorID;
			if(sensorID == 0xff) continue;
			int32_t sensorValue = getSensorValue(sensorID, 0, 0);
			if(sensorID >= IBUS_MEAS_TYPE_GPS_LAT && sensorID <= IBUS_MEAS_TYPE_S8a && sensorValue < SENSORS_ARRAY_LENGTH){
				sensorValue = longSensors[sensorValue];
			}
			if(modConfig.alarm[i].operator == OPERATOR_GT){
				if(sensorValue > modConfig.alarm[i].value){
					beepCount = i+2;
					break;
				}
			}
			if(modConfig.alarm[i].operator == OPERATOR_LT){
				if(sensorValue < modConfig.alarm[i].value){
					beepCount = i+2;
					break;
				}
			}

		 	//modConfig.alarms[i].operator = modConfig.alarm[i].operator;
		 	//modConfig.alarms[i].sensorID = modConfig.alarm[i].sensorID;
		 	//modConfig.alarms[i].value = modConfig.alarm[i].value;
		 }
	 }

	 if(beepCount > 0 && ((timer - lastAlarm) >= 2000)){
		 *((int32_t *)LAST_ALARM_TIMER) = timer;
		 while(beepCount > 0){
			 beepCount --;
		 if(someBeepCheck() >= 2){
			beep(700,100);
			beep(0,100);
		 }}
		//if(someBeepCheck >= 2){
		//	beep(1000, 350);
		//	beep(500, 350);
		//}
	 }


	 CheckAlarmsCall();
 }

void SwBConfig() {
	uint32_t key = 0;
	uint8_t swConfig = modConfig2.swConfig;
	uint8_t modeSWB = 2;

	char buffer[32];
	while (1) {
		callSetupDMAandSend();
		displayPageHeader((char*) 0xd075); //Switches
		displayTextAt((char*)0xEDC7, 8, 24, 0); //SwB
		if((swConfig & 1) == 0) modeSWB = 2;
		else modeSWB = 3;

		sprintfCall(buffer, (const char*) formatNumber, modeSWB);
		displayTextAt((char*) buffer, 48, 24, 0);

		displayTextAt((char*)0xEDCB, 8, 32, 0); //SwC

		if((swConfig & 2) == 0){
			sprintfCall(buffer, (const char*) formatNumber, 3);
		}
		else {
			buffer[0] = 'V';
			buffer[1] = 'a';
			buffer[2] = 'r';
			buffer[3] = 0;
		}
		displayTextAt((char*) buffer, 48, 32, 0);

		LCD_updateCALL();
		key = getKeyCode();
		if (key == KEY_SHORT_UP || key == KEY_LONG_UP)
		{
			if((swConfig & 1) == 0)swConfig |= 1 << 0;
			else swConfig &= ~(1 << 0);
		}
		else if(key == KEY_SHORT_DOWN || key == KEY_LONG_DOWN){
			if((swConfig & 2) == 0)swConfig |= 2;
			else swConfig &= ~(2);
		}
		else break;

	}

	if (key == KEY_LONG_CANCEL) {
		modConfig2.swConfig = swConfig;
		//saveModSettings();
	}

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
		alarms[i].operator = modConfig.alarm[i].operator;
		alarms[i].sensorID = modConfig.alarm[i].sensorID;
		alarms[i].value = modConfig.alarm[i].value;
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
				if(alarmItem.operator == OPERATOR_LT) buffer[0] = '<';
				if(alarmItem.operator == OPERATOR_GT) buffer[0] = '>';
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
					 alarms[row].value = 0;
					 alarms[row].sensorID += 1;
					 if(alarms[row].sensorID == IBUS_MEAS_TYPE_FLIGHT_MODE + 1) alarms[row].sensorID = IBUS_MEAS_TYPE_GPS_LAT;
					 if(alarms[row].sensorID == IBUS_MEAS_TYPE_S8a + 1) alarms[row].sensorID = IBUS_MEAS_TYPE_SNR;
				 }
			 }
			 else if(key == KEY_SHORT_DOWN || key == KEY_LONG_DOWN) {
				 if(column == 0){
					 alarms[row].value = 0;
					 alarms[row].sensorID -= 1;
					 if(alarms[row].sensorID == IBUS_MEAS_TYPE_SNR -1) alarms[row].sensorID = IBUS_MEAS_TYPE_S8a;
					 if(alarms[row].sensorID == IBUS_MEAS_TYPE_GPS_LAT -1) alarms[row].sensorID = IBUS_MEAS_TYPE_FLIGHT_MODE;
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
			modConfig.alarm[i].operator = alarms[i].operator;
			modConfig.alarm[i].sensorID = alarms[i].sensorID;
			modConfig.alarm[i].value = alarms[i].value;
		}
		saveModSettings();
	}

}


void TimerConfig(){
	uint16_t data[3];
	uint16_t max[3];
	data[0] = (uint16_t) modConfig.timerCH;
	data[1] = modConfig.timerStart;
	data[2] = modConfig.timerAlarm;
	max[0] = 10;
	max[1] = 2200;
	max[2] = 0xffff;
	uint32_t h = 0;
	uint32_t m = 0;
	uint32_t s = 0;
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
	    	  if(i==2){
	    		  h = divMod(data[i], 3600, &m);
	    		  m = divMod(m, 60, &s);
	    		  sprintfCall(buffer, (const char*) timerFormat, h,m,s);
	    	  }
	    	  displayTextAt(buffer, 64, 16 + i*8,0);
	      }
	      displayGFX((gfxInfo*) GFX_ARROW, 1, 16 + navPos*8);

	      LCD_updateCALL();
	      key = getKeyCode();

	      step = 1;
	      if(navPos != 0) step = 10;

	      if(key == KEY_LONG_OK){
	    	  data[navPos] = 0;
	      }
	      else if(key == KEY_SHORT_OK){
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
		modConfig.timerCH = (uint8_t) data[0];
		modConfig.timerStart = data[1];
		modConfig.timerAlarm = data[2];
		saveModSettings();
	  }
}

void parseCoord(uint32_t *deg, uint32_t *min, uint32_t *sec, uint32_t *subSec, uint32_t coord)
{
	/*uint32_t tmp = 0;
	*deg = divMod(coord, 10000000, &tmp);
	coord = tmp * 60;
	*min = divMod(coord, 10000000, &tmp);
	coord = tmp * 60;
	*sec = divMod(coord, 10000000, &tmp);*/
}



void formatSensorValue(char* target, int sensorID, uint16_t sensorValue) {
	const char* format = (const char*) formatNumber;
	const char* unit = 0;
	uint32_t result = (uint32_t) sensorValue;
	uint8_t negative = 0;
	uint32_t result2 = 0;
	uint32_t result3 = 0;
	uint32_t result4 = 0;

	uint8_t sensorInfo = STD_SENSOR | UNSIGNED | MUL_001 | UNIT_NONE;

	if (sensorID > 255) return;
	if (sensorID <= IBUS_MEAS_TYPE_FLIGHT_MODE) {
		sensorInfo = sensorDesc00[sensorID];
		if ((sensorInfo & SIGNED__) != 0 && ((int16_t) sensorValue) < 0) {
			negative = 1;
			result = (uint32_t) -((int16_t) sensorValue);
		}
	} else if (sensorID == IBUS_MEAS_TYPE_PRES) {
		if (sensorValue < SENSORS_ARRAY_LENGTH) result = longSensors[sensorValue];

	} else if (sensorID >= IBUS_MEAS_TYPE_GPS_LAT && sensorID <= IBUS_MEAS_TYPE_S8a) {
		sensorInfo = sensorDesc80[sensorID - IBUS_MEAS_TYPE_GPS_LAT];
		if (sensorValue < SENSORS_ARRAY_LENGTH) {
			result = longSensors[sensorValue];
		}
		if ((sensorInfo & SIGNED__) != 0 && ((int32_t) result) < 0) {
			negative = 1;
			result = (uint32_t) -((int32_t) result);
		}
	} else if (sensorID >= IBUS_MEAS_TYPE_SNR) {
		sensorInfo = sensorDescFA[sensorID - IBUS_MEAS_TYPE_SNR];
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
			if (!sensorValue) {
				target[0] = 'U'; //U
				target[1] = 'n'; //n
				target += 2;
			}
			strcatCall(target, (const char *) ARMED);
			return;
		case IBUS_MEAS_TYPE_FLIGHT_MODE:
			if (result <= 9){
				strcatCall(target, (const char *) FLY_MODES + (flyModesOffset[result]));
				return;
			}
			else{
				format = (const char*) formatNumber;
			}
			return;
		case IBUS_MEAS_TYPE_GPS_LON:
		case IBUS_MEAS_TYPE_GPS_LAT:
			format = (const char*) formatCoord;
			result = divMod(result, 10000000, &result2);
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

void configurePINS2(){
	configurePINs();
	PORT_SetPinMux(PORTD, 2u, kPORT_MuxAsGpio);
	GPIOD->PDDR &= ~(1U << 2); //input
}

//return non 0 when switch MAX value
void swBasADC(){
	uint32_t swBVal =((*(uint32_t *)(ADC_SW_B)));
	uint32_t *tempInputs = (uint32_t *)TEMP_INPUT_STATES;
	if (swBVal < 500u)	*tempInputs |= 0x200000u;
	if(swBVal > 3800u) *tempInputs |= 0x20000u;
}

void swEHandling(){
	uint32_t *tempInputs = (uint32_t *)TEMP_INPUT_STATES;
	uint32_t portD = ((*(uint32_t *)(0x400FF0D0)));
	if ( !(portD & 0x4)) *tempInputs |= 1 << 22;
}

void displaySensors(){
	char buffer[64];
	uint32_t bufferOffset = 0;
	uint8_t batteryID = IBUS_MEAS_TYPE_INTV;
	const uint8_t* settings = (uint8_t*)((*(uint32_t *)(USED_MODEL_PTR)));
	uint32_t batteryType = (uint32_t)settings[98];
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
			sensorCount = SENSORS_PER_PAGE;
			settings = sensorsScreens + (SENSORS_PER_PAGE* (mainScreenIndex-2));
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
			if(*settings >= IBUS_MEAS_TYPE_GPS_LAT && *settings <= IBUS_MEAS_TYPE_GPS_ALT){
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

