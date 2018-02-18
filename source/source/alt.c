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

extern long long __mul64(long long a, long long b);


void add2ByteSensor(uint8_t sensorID, uint8_t sensorIndex, uint16_t value){
	char shortSensor[4];
	shortSensor[0] = sensorID;
	shortSensor[1] = sensorIndex;
	shortSensor[2] = value & 0xFF;
	shortSensor[3] = value >> 8 & 0xFF;
	setSensorValue(shortSensor);
}

#pragma GCC optimize ("Os")

void acData(uint8_t* rxBuffer){
	uint32_t sensorData = 0;
	uint8_t sensorIndex = 0;
	uint16_t shortSensor = 0;
	int index = 1;
	int i = 0;
	int tmp = 0;
	int offset = -1;

	do{
		//ID|Index|Length|Data
		offset = -1;
		sensorIndex = rxBuffer[index +1];
		sensorData = (rxBuffer[index+6] << 24 | rxBuffer[index+5] << 16 | rxBuffer[index+4] << 8 | rxBuffer[index+3]);
		if(rxBuffer[index]==IBUS_MEAS_TYPE_PRES){
			offset = SENSORS_ARRAY_LENGTH - 1;
			shortSensor = (uint16_t)(sensorData >> 19);
			add2ByteSensor(IBUS_MEAS_TYPE_TEM, sensorIndex, shortSensor);
			sensorData = sensorData & 0x7FFFF;
			tmp = getALT(sensorData, shortSensor);
			i = IBUS_MEAS_TYPE_ALT - IBUS_MEAS_TYPE_GPS_LAT;
			longSensors[i] = tmp;
			add2ByteSensor(IBUS_MEAS_TYPE_ALT, sensorIndex, i);
		}
		else if(rxBuffer[index]==IBUS_MEAS_TYPE_GPS_FULL){
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
/*
void loadModSettings(){
	settingsValidation();
	readEeprom((uint8_t *)&modConfig, 0xBC0, sizeof(modConfig));
}

void saveModSettings(){
	saveEeprom((uint8_t *)&modConfig, 0xBC0, sizeof(modConfig));
	someSPImethod();
}
*/

modelConfStruct* getModelModConfig(){
	uint8_t modelIndex = ((*(uint8_t *)(CURRENT_MODEL_INDEX)));
	modelConfStruct* ptr = &modConfig2.modelConfig[modelIndex];
	return ptr;
}

#define ITEMS_PER_ROW 3
#define MAX_ROWS 8
void mixConfig() {
	uint32_t key = 0;
	uint8_t row = 0;
	uint8_t col = 0;
	uint8_t rowPos = 0;

	int8_t config[MIX_CONFIG_SIZE_BYTES];
	uint8_t index = 0;
	int8_t value = 0;
	struct modelConfStruct *configPtr = getModelModConfig();
	memcpy_(config, configPtr->mix, sizeof(configPtr->mix));

	char buffer[16];
	char *bufferPtr;

	uint8_t channel = 0;

	do {
		callSetupDMAandSend();
		//displayPageHeader((char*)0xDBCB);
		channel = 7;
		for (uint8_t rowIndex= 0; rowIndex < MAX_ROWS; rowIndex++, channel++) {
			rowPos = rowIndex << 3;
			/*buffer[0] = channel < 10 ?  ' ' : '1';
			buffer[1] = '0' + (channel < 10 ?  channel : (channel - 10));
			buffer[2] = 0;
			displayTextAt(buffer, 0, rowPos, 0);
			*/
			for (uint8_t colIndex = 0; colIndex < ITEMS_PER_ROW; colIndex++) {
				int16_t val = (int16_t)config[3 * rowIndex + colIndex];
				bufferPtr = buffer;
				if(val < 0){
					val = val *-1;
					buffer[0] = '-';
					bufferPtr++;
				}
				sprintfCall(bufferPtr, (const char*)0x5564, val);
				displayTextAt(buffer, mixPos[colIndex] + 8, rowPos, 0);
			}
			displayGFX((gfxInfo*)GFX_ARROW, mixPos[col], row << 3);
		}


		LCD_updateCALL();
		key = getKeyCode();
		index = 3 * row + col;
		value = config[index];

		if (key == KEY_SHORT_OK)
		{
			col++;
			if (col >= ITEMS_PER_ROW) {
				col = 0;
				row++;
			}
			if (row >= MAX_ROWS) row = 0;
		}
		else if (key == KEY_SHORT_UP || key == KEY_LONG_UP) {
			if(value >= 127) continue;
			if(col < 2 && value >= config[index+1])continue;
			config[index] = ++value;
		}
		else if (key == KEY_SHORT_DOWN || key == KEY_LONG_DOWN) {
			if(value <= -127) continue;
			if(col > 0 && value <= config[index-1]) continue;
			config[index] = --value;
		}
	}
	while (key != KEY_LONG_CANCEL && key != KEY_SHORT_CANCEL);
	if (key == KEY_LONG_CANCEL) {
		memcpy_(configPtr->mix, config, sizeof(configPtr->mix));
	}
}

int mix(int value, int8_t min, int8_t max, int8_t subtrim){
	uint8_t input_range_fixed_q20 = 105;
	int16_t lim_p = max * 100;
	int16_t lim_n = min * 100;
	int16_t ofs = subtrim * 100;
	uint8_t neg = 0; //get from config

	if (value) {
		int16_t tmp = (value > 0) ? (lim_p - ofs) : (-lim_n + ofs);
		value = (int32_t)value * tmp;
		if (value < 0) {
			value *= -1;
			neg = !neg;
		}
		value = (int)(__mul64((uint64_t)value, (uint64_t)input_range_fixed_q20) >> 20);
		if (neg) value *= -1;
		ofs += value;
	}
	if (ofs > lim_p) ofs = lim_p;
	if (ofs < lim_n) ofs = lim_n;
	return ofs;
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
	const uint32_t now = *((uint32_t *)(0xE000E018));
	const uint32_t ms100 = 480007;
	uint32_t diff = 0;
	uint32_t h = 0;
	uint32_t m = 0;
	uint32_t s = 0;
	if(isTimerActive()){
		if(lastTimerUpdate == 0) lastTimerUpdate = now;
		else{
			diff = (now - lastTimerUpdate) & 0xFFFFFF;
			if(diff > ms100){
				ticks100MS++;
			}
			if(ticks100MS>=9){
				ticks100MS=0;
				timerValue++;
				h = divMod(timerValue, 3600, &m);
				m = divMod(m, 60, &s);
				lastTimerUpdate = now - diff;
				sprintfCall((char*)timerBuffer, (const char*)timerFormat, h,m,s);
			}
		}
	}
	if(timerBuffer[0]==0) strcatCall((char*)timerBuffer, (const char*)timerNull);
	if(skipPrint ==0)displayTextAt((char*)timerBuffer, 1, 0, 0);
}

uint32_t isTimerActive(){
	const uint32_t one_hundred = 100;
	int32_t one_thousand = one_hundred * 10;
	int32_t ten_thousands = one_thousand * 10;
	struct modelConfStruct *configPtr = getModelModConfig();
	uint8_t channel = configPtr->timerCH;
	if(timerValue > 0 && (channel & (1<<7))) return 1; //HOLD mode
	channel = channel & 0xf;
	//if(channel > (1 <<4)) return 0;
	//if(value > one_thousand*2) return 0; // > 2000
	int32_t chValue = *(((int32_t *)CHANNEL_VALUE)+(channel-1));
	int32_t configVal = ((int32_t)configPtr->timerStart - one_thousand) * 20 - ten_thousands;
	if(channel == 0) return 0;
	return chValue > configVal;
}


#define SW_A 0u
#define SW_B 1u
#define SW_C 2u
#define SW_D 3u
#define SW_E 6u


void auxChannels2(){
	uint8_t row = 0;
	uint32_t key = 0;
	uint8_t page = 0;
	uint8_t auxValue = 0;
	uint8_t index = 0;
	uint8_t modelType = 0;
	uint8_t heli = 0;
	uint32_t* namePtr = (uint32_t*)0x200000B8;
	char* tmp = 0;
	uint8_t* settings = (uint8_t*)((*(uint32_t *)(USED_MODEL_PTR)));
	uint8_t auxChannels[10];
	modelType = settings[9];
	heli = settings[55] & 8;
	settings += 60;
	auxChannels[0] = settings[0];
	auxChannels[1] = settings[1];
	extractConfigCh7_14(auxChannels +2);

	char buffer[32];
	do{
		callSetupDMAandSend();
		displayPageHeader((char*)0xCC5A);

		for(uint8_t i=0; i < 6; i++){
			index = (page * 6) + i;
			uint8_t channel = 5 + index;
			if(channel > 14) continue;
			int posY = 16 + i *8;
			displayTextAt((char*)0x9650, 8, posY, 0);
			sprintfCall(buffer, (const char*) formatNumber, channel);
			displayTextAt(buffer, 64, posY, 0);
			auxValue = auxChannels[index];
			if(auxValue <= 6){
				tmp = (char*)(*(namePtr + auxValue));
			}
			else if(auxValue <= 15){
				tmp = auxSW + (6 * (auxValue - 7));
			}

			if((channel == 6 && modelType >=2) || (channel == 5 && modelType && heli)){
				tmp = (char*) 0xCA9F;
			}

			if(tmp!=0 && tmp[0]!=0){
				displayTextAt(tmp, 88, posY, 0);
			}
		}
		displayGFX((gfxInfo*) GFX_ARROW, 0, 16 + (row*8));
		LCD_updateCALL();
		key = getKeyCode();
		index = (page * 6) + row;
		if(key == KEY_SHORT_OK) {
			row++;
			if((page == 1 && row ==4) || row == 6){
				if(++page == 2) page = 0;
				row = 0;
			}
		}
		else if(key == KEY_LONG_OK) {
			auxChannels[index] = 0;
		}
		else if(key == KEY_SHORT_UP || key == KEY_LONG_UP) {
			if(auxChannels[index] < MAX_INDEX){
				auxChannels[index]++;
			}
		}
		else if(key == KEY_SHORT_DOWN || key == KEY_LONG_DOWN) {
			if(auxChannels[index] > 0){
				auxChannels[index]--;
			}
		}
	}
	while ( key != KEY_LONG_CANCEL && key != KEY_SHORT_CANCEL);

	if(key == KEY_LONG_CANCEL){
		saveAuxCh5_14(auxChannels);
	}
}
void extractConfig(uint8_t val, uint8_t* result){
	result[0] = val & 0xF;
	result[1] = val >> 4;
}

void extractConfigCh7_14(uint8_t* result){
	struct modelConfStruct *configPtr = getModelModConfig();
	const uint8_t* settings = (uint8_t*)((*(uint32_t *)(USED_MODEL_PTR)));
	extractConfig(settings[141], result);
	extractConfig(settings[145], result+2);
	extractConfig(configPtr->ch11_12, result+4);
	extractConfig(configPtr->ch13_14, result+6);
}
void saveAuxCh5_14(uint8_t* current){
	struct modelConfStruct *configPtr = getModelModConfig();
	uint8_t* settings = (uint8_t*)((*(uint32_t *)(USED_MODEL_PTR)));
	settings+=60;
	settings[0] = current[0];
	settings[1] = current[1];
	settings+=81;
	settings[0] = current[2] | (current[3] << 4);
	settings[4] = current[4] | (current[5] << 4);
	configPtr->ch11_12 = current[6] | (current[7] << 4);
	configPtr->ch13_14 = current[8] | (current[9] << 4);
}



void createPacketCh1114(){
	int32_t value = 0;
	int32_t channel7Address = 0x1FFFFDF8;
	struct modelConfStruct *configPtr = getModelModConfig();
	struct mixConfStruct *mixPtr;
	const uint8_t* settings = (uint8_t*)((*(uint32_t *)(USED_MODEL_PTR)));
	settings += 141;
	//int32_t channel11Address = 0x1FFFFE08;
	//struct modelConfStruct *configPtr = getModelModConfig();
	uint8_t auxChannels[8];
	extractConfigCh7_14(auxChannels);

	for(uint8_t i=0; i < sizeof(auxChannels); i++){
		value = getAuxChannel(auxChannels[i]);
		mixPtr = &(configPtr->mix[i]);
		if(mixPtr->min != 0 || mixPtr->min != 0){
				value = mix(value, mixPtr->min, mixPtr->max, mixPtr->subtrim);
		}
		*((int32_t*)(channel7Address + 4*i)) = value;
	}
}
/*
int mapSNR(){
	int snr = getSensorValue(IBUS_MEAS_TYPE_SNR, 0,0);
	if(snr == 0x8000) return -10000;
	if(snr >= maxSNR) {
		maxSNR = snr;
		return 10000;
	}
	snr = maxSNR - snr;
	if(snr > 20) return -10000;
	snr = ((((10000 << 1) * (int32_t)snrMulti[snr])) >> 10) - 10000;
	return snr;
}*/
#define MAX_SNR 40
#define MIN_SNR 9


int mapSNR(){
	int snr = getSensorValue(IBUS_MEAS_TYPE_SNR, 0,0);
	if(snr == 0x8000 || snr <= MIN_SNR) return -10000;
	if(snr >= MAX_SNR) return 10000;
	snr -=9;
	snr = (snr * 645) - 10000;
	return snr;
}

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
		return  *(int32_t *)(PPM_IN_BUFFER + request);
	}
	if(request == 11){ //error
		sw1 = getSensorValue(IBUS_MEAS_TYPE_ERR, 0,0);
		if(sw1 == 0x8000) sw1 = -10000;
		return ((100 - sw1) * 200) - 10000;
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
	else if(request == 10){ // SNR
		return mapSNR();
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


/*Belongs to .mod_modMenu 0xFFA0 */
void displayMenu(){
	showNavPage((const char*) extraMenu, EXTRA_MENU_ITEMS, (manuEntry*)menuList);
}

void BatteryType() {
	uint32_t key = 0;

	uint16_t batteryVolt = modConfig2.batteryVoltage;
	char buffer[32];
		while (1) {
			callSetupDMAandSend();
			if(batteryVolt > 900) batteryVolt = 0;
			displayPageHeader((char*)(extraMenu+EXTRA_MENU_TXBAT));
			displayTextAt((char*)(extraMenu+EXTRA_MENU_ALARM), 8, 24,0);
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
		modConfig2.batteryVoltage = batteryVolt;
	}
}
void play(int freq, int duration, int pause){
	if(someBeepCheck() >= 2){
		beep(freq,duration);
		if(pause > 0) beep(0,pause);
	}
}

void beepSilent(){

}

#define BEEP_DEFAULT_FREQ (900)

#define VARIO_BASE_FREQ		1200
// gain can be 0..15, representing gains 1/8 (>> 3) to 2048 (<< 11)
#define VARIO_GAIN_OFFSET	(1 << (VARIO_MAX_GAIN_BITS - 2))
// #define VARIO_TEST_CH6_INSTEAD_OF_SENSOR	1

void CheckCustomAlarms(){
	int32_t timer = ((*(int32_t *)(TIMER_SYS_TIM)));
	int32_t lastAlarm = *((int32_t *)LAST_ALARM_TIMER);
	int32_t lastTelemetryUpdate = *((int32_t *)TELEMETRY_UPDATE_TIMER);
	uint8_t sensorID = 0;
	uint8_t active = 0, wasAlarm = 0;
	uint32_t duration = 100;
	uint32_t defFreq = 100;
	uint32_t checkTimout = defFreq *10;
	struct modelConfStruct *configPtr = getModelModConfig();
	if(timer < duration ) return;
	if(timer - lastAlarm >= 500){
		if((uint32_t)configPtr->timerAlarm != 0 && timerValue > (uint32_t)configPtr->timerAlarm) {
			*((int32_t *)LAST_ALARM_TIMER) = timer;
			play(BEEP_DEFAULT_FREQ, duration, 50);
			wasAlarm = 1;
		}
		if(timer - lastTelemetryUpdate < checkTimout){
			for(int i = 0; i < 3; i++){
				active = 0;
				sensorID = configPtr->alarm[i].sensorID;
				if(sensorID == IBUS_MEAS_TYPE_UNKNOWN) continue;
				int32_t sensorValue = getSensorValue(sensorID, 0, 0);
				if(sensorID >= IBUS_MEAS_TYPE_GPS_LAT && sensorID <= IBUS_MEAS_TYPE_S8a && sensorValue < SENSORS_ARRAY_LENGTH && sensorValue >= 0){
					sensorValue = longSensors[sensorValue];
				}
				if(configPtr->alarm[i].operator == OPERATOR_GT){
					if(sensorValue > configPtr->alarm[i].value){
						active = 1;
					}
				}
				if(configPtr->alarm[i].operator == OPERATOR_LT){
					if(sensorValue < configPtr->alarm[i].value){
						active = 1;
					}
				}
				if(active){
					*((int32_t *)LAST_ALARM_TIMER) = timer;
					play(BEEP_DEFAULT_FREQ + ((i+1)* defFreq), duration, 40);
					wasAlarm = 1;
				}
			}
#if VARIO_TEST_CH6_INSTEAD_OF_SENSOR
		}
		if (1) { // ignore the lastTelemetryUpdate
#endif

			// Vario sound feedback
			sensorID = configPtr->varioSensorID;

			if (sensorID != IBUS_MEAS_TYPE_UNKNOWN
				&& timer - varioPrevTime >= 1000
				&& !wasAlarm) {

#if VARIO_TEST_CH6_INSTEAD_OF_SENSOR
				int32_t sensorValue = *(((int32_t *)CHANNEL_VALUE)+(5));
#else

				int32_t sensorValue = (int16_t)getSensorValue(sensorID, 0, 0);

				if ((sensorID == IBUS_MEAS_TYPE_PRES || (sensorID >= IBUS_MEAS_TYPE_GPS_LAT && sensorID <= IBUS_MEAS_TYPE_S8a)) && sensorValue < SENSORS_ARRAY_LENGTH && sensorValue >= 0)
					sensorValue = longSensors[sensorValue];
#endif

				varioPrevTime = timer;
                int32_t diff = sensorValue - varioPrevValue;
                if(diff < 0) diff *=-1;
				// be silent when no change
				if (diff != 0 && diff > configPtr->varioDeadBand) {

					int32_t freq;
					int8_t gain = configPtr->varioGain;

					freq = sensorValue - varioPrevValue;

					// the range of negative values
					// is only half of the positive range,
					// so decrease the gain for them
					if (freq < 0)
						gain--;

					if (gain > VARIO_GAIN_OFFSET) {
						freq <<= gain - VARIO_GAIN_OFFSET;
					} else {
						freq >>= VARIO_GAIN_OFFSET - gain;
					}
					varioPrevValue = sensorValue;
					freq += VARIO_BASE_FREQ;

					play(VARIO_BASE_FREQ, 25, 50);

					// keep it inside one octave up/down
					if (freq < VARIO_BASE_FREQ/2)
						freq = VARIO_BASE_FREQ/2;
					else if (freq > 2*VARIO_BASE_FREQ)
						freq = 2*VARIO_BASE_FREQ;

					play(freq, 50, 50);
				}
			}
		}
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
	}

}

uint8_t nextSensorID(uint8_t sensorID)
{
	sensorID++;
	if (sensorID == IBUS_MEAS_TYPE_FLIGHT_MODE + 1)
        sensorID = IBUS_MEAS_TYPE_PRES;

    if (sensorID == IBUS_MEAS_TYPE_PRES + 1)
		sensorID = IBUS_MEAS_TYPE_ODO1;

    if (sensorID == IBUS_MEAS_TYPE_TX_V + 1)
    	sensorID = IBUS_MEAS_TYPE_GPS_LAT;

	if (sensorID == IBUS_MEAS_TYPE_S8a + 1)
		sensorID = IBUS_MEAS_TYPE_SNR;

	return sensorID;
}

uint8_t prevSensorID(uint8_t sensorID)
{
	sensorID--;

	if (sensorID == IBUS_MEAS_TYPE_SNR -1)
		sensorID = IBUS_MEAS_TYPE_S8a;

	if (sensorID == IBUS_MEAS_TYPE_GPS_LAT -1)
		sensorID = IBUS_MEAS_TYPE_TX_V;

    if (sensorID == IBUS_MEAS_TYPE_ODO1 -1)
    	sensorID = IBUS_MEAS_TYPE_PRES;

    if (sensorID == IBUS_MEAS_TYPE_PRES -1)
        sensorID = IBUS_MEAS_TYPE_FLIGHT_MODE;

	return sensorID;
}

void ASLConfig(){
	uint32_t key = 0;
	uint8_t navPos = 0;
	int y = 0;
	uint32_t values[2];
	char buffer[32];
	getInitPressure(values, &values[1]);
	do {
		callSetupDMAandSend();
		displayPageHeader((char*)(extraMenu+EXTRA_MENU_ASL)); //ASL
		//values[0] = aslConfig >> 19;
		//values[1] = aslConfig & 0x7FFFF;
		initPressure = values[0];
		initTemperature = ibusTempToK((int16_t)values[1]);
		for(uint8_t index = 0; index< 3; index++){
			y = 16+index*8;
			displayTextAt((char*)aslLabels[index], 8, y, 0);
			if(index == 0) sprintfCall(buffer, (const char*) formatNumber, values[0]);
			if(index == 1) formatSensorValue(buffer, IBUS_MEAS_TYPE_TEM, (uint16_t) (values[1]-400));
			if(index == 2) formatSensorValue(buffer, IBUS_MEAS_TYPE_ALT, IBUS_MEAS_TYPE_ALT - IBUS_MEAS_TYPE_GPS_LAT);
			displayTextAt((char*)buffer, 64, y, 0);
		}

		displayGFX((gfxInfo*) GFX_ARROW, 56, navPos ? 24 : 16);
		LCD_updateCALL();
		key = getKeyCode();

		if(key == KEY_SHORT_UP || key == KEY_LONG_UP) values[navPos] +=10;
		else if(key == KEY_SHORT_DOWN || key == KEY_LONG_DOWN) values[navPos] -=10;
		else if(key == KEY_SHORT_OK) navPos = !navPos;
		else if(key == KEY_LONG_OK) {
			values[0] = defASL & 0x7FFFF;
			values[1] = defASL >> 19;
		}
		} while ( key != KEY_LONG_CANCEL && key != KEY_SHORT_CANCEL);

		if(key == KEY_LONG_CANCEL){
			struct modelConfStruct *configPtr = getModelModConfig();
			configPtr->initAlt = values[0] | (values[1] << 19);
		}
}

void AlarmConfig(){
	struct modelConfStruct *configPtr = getModelModConfig();
	sensorAlarm alarmItem;
	sensorAlarm alarms[3];
	uint32_t row = 0;
	uint32_t column = 0;
	uint32_t columnOffset = 0;
	uint32_t key = 0;
	uint16_t step = 1;
	uint16_t y = 0;
	char buffer[32];

	memcpy_(alarms, configPtr->alarm, sizeof(alarms));

	do{
		 while ( 1 )
		 {
			 callSetupDMAandSend();
			 displayPageHeader((char*)extraMenu+EXTRA_MENU_ALARM);
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
			 if(key == KEY_LONG_OK){
				alarms[row].sensorID = IBUS_MEAS_TYPE_UNKNOWN;
				alarms[row].value = 0;
			 }
			 else if(key == KEY_SHORT_OK ){
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
					 alarms[row].sensorID = nextSensorID(alarms[row].sensorID);
				 }
			 }
			 else if(key == KEY_SHORT_DOWN || key == KEY_LONG_DOWN) {
				 if(column == 0){
					 alarms[row].value = 0;
					 alarms[row].sensorID = prevSensorID(alarms[row].sensorID);
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
		memcpy_(configPtr->alarm, alarms, sizeof(alarms));
	}

}


void TimerConfig(){
	//{ 10, 2200, 0xffff, 1 };
	uint16_t data[4];
	uint32_t h = 0;
	uint32_t m = 0;
	uint32_t s = 0;
	uint8_t navPos = 0;
	uint32_t key = 0;
	uint16_t step = 1;
	uint8_t labelPos = 9;
	char buffer[32];
	struct modelConfStruct *configPtr = getModelModConfig();
	data[0] = (uint16_t) configPtr->timerCH & 0xF;
	data[1] = configPtr->timerStart;
	data[2] = configPtr->timerAlarm;
	data[3] = (configPtr->timerCH & (1<<7)) != 0;
	do
	 {
	    while ( 1 )
	    {
	      callSetupDMAandSend();
	      displayPageHeader((char*)TEXT_TIMMER);
	      displayTextAt((char*)TEXT_CHANNEL, labelPos, 16,0);
	      displayTextAt((char*)TEXT_VALUE, labelPos, 24,0);
	      displayTextAt((char*)(extraMenu+EXTRA_MENU_ALARM), labelPos, 32,0);
		  	displayTextAt((char*)TEXT_HOLD, labelPos, 40,0);


	      for(int i = 0; i < 4; i++){
			  int yPos = 16 + i*8;
			  displayTextAt((char*)timerLabels[i], labelPos, yPos,0);
	    	  sprintfCall(buffer, (const char*) formatNumber, data[i]);
	    	  if(i==2){
	    		  h = divMod(data[i], 3600, &m);
	    		  m = divMod(m, 60, &s);
	    		  sprintfCall(buffer, (const char*) timerFormat, h,m,s);
	    	  }
	    	  displayTextAt(buffer, 64, yPos, 0);
	      }
	      displayGFX((gfxInfo*) GFX_ARROW, 1, 16 + navPos*8);

	      LCD_updateCALL();
	      key = getKeyCode();

	      step = 1;
	      if(navPos == 1 || navPos ==2) step = 10;

	      if(key == KEY_LONG_OK){
	    	  data[navPos] = 0;
	      }
	      else if(key == KEY_SHORT_OK){
	    	  navPos++;
	    	  if(navPos >=4)navPos = 0;
	      }
	      else if(key == KEY_SHORT_UP || key == KEY_LONG_UP){
	    	  data[navPos] += step;
	    	  if(data[navPos] >= timerMaxValues[navPos]) data[navPos] = timerMaxValues[navPos];
	      }
	      else if(key == KEY_SHORT_DOWN || key == KEY_LONG_DOWN) {
	    	  if(data[navPos] > step) data[navPos] -= step;
	      }
	      else  {
	    	  break;
	      }
	    }
	  }
	  while ( key != KEY_LONG_CANCEL && key != KEY_SHORT_CANCEL);
	  if( key == KEY_LONG_CANCEL) {
		configPtr->timerCH = (uint8_t) data[0] | (data[3] << 7);
		configPtr->timerStart = data[1];
		configPtr->timerAlarm = data[2];
	  }
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
		sensorInfo |= MUL_100;
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
	if ((sensorInfo & MUL_010) == MUL_010) {
		format = (const char*) formatNumberFractial;
		result = divMod(result, 10, &result2);
		result2 = result2*10;
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
#ifdef SWE
	PORT_SetPinMux(PORTD, 2u, kPORT_MuxAsGpio);
	GPIOD->PDDR &= ~(1U << 2); //input
#endif

}

void loadSettings(){
	loadSettingsFromEeprom();
	struct modelConfStruct *config;
	if(modConfig2.versionMagic != VERSION_MAGIC){
		modConfig2.batteryVoltage = 480;
		modConfig2.swConfig = 0;
		modConfig2.versionMagic = VERSION_MAGIC;
		for(uint8_t model =0; model < TOTAL_MODELS; model++){
			//set first 50 bytes
			config = &modConfig2.modelConfig[model];
			memsetCall(config, 50, 0);
			config->varioSensorID = IBUS_MEAS_TYPE_UNKNOWN;
			config->intVoltAdj = config->extVoltAdj = 10000;
			for(uint8_t mixIndex = 0; mixIndex < 8; mixIndex++){
					config->mix[mixIndex].min = -100;
					config->mix[mixIndex].max = 100;
			}
			for(uint8_t alarmIndex = 0; alarmIndex < 3; alarmIndex++)
				config->alarm[alarmIndex].sensorID = IBUS_MEAS_TYPE_UNKNOWN;
		}
		saveModelSettingsCall();
		restartUnit();
	}

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

#define VARIO_MAX_GAIN 	((1 << VARIO_MAX_GAIN_BITS) - 1)

void varioSensorSelect(){
	uint32_t key = 0;
    uint8_t row = 0;
    char buffer[8];
	struct modelConfStruct *model = getModelModConfig();
    uint8_t values[3];

    values[0] = model->varioSensorID;
    values[1] = model->varioGain;
    values[2] = model->varioDeadBand;

	do {
		callSetupDMAandSend();
		displayPageHeader((char*)varioSensor);


		displayTextAt((char*)SOURCE_STRING, 8, 24, 0);
        displayTextAt((char*)(varioSensor + GAIN_OFFSET), 8, 36, 0);
        displayTextAt((char*)(deadBand), 8, 48, 0);

		displayTextAt((char*)getSensorName(values[0]), 80, 24, 0);
		sprintfCall(buffer, (const char*) formatNumber, values[1]);
		displayTextAt(buffer, 80, 36, 0);
        sprintfCall(buffer, (const char*) formatNumber, values[2]);
		displayTextAt(buffer, 80, 48, 0);

		displayGFX((gfxInfo*) GFX_ARROW, 64, (row * 12) + 24);

		LCD_updateCALL();

		key = getKeyCode();

		if (key == KEY_LONG_CANCEL) { // Save it
			model->varioSensorID = values[0];
			model->varioGain = values[1];
            model->varioDeadBand = values[2];
		}

		if (key == KEY_SHORT_OK){
            row += 1;
            if(row == 3) row = 0;
        }


		if (key == KEY_SHORT_UP || key == KEY_LONG_UP) {
			if (row == 0) values[0] = nextSensorID(values[0]);
            else if (row == 1 && values[1] < VARIO_MAX_GAIN) values[1]++;
			else values[2]++;
		}

		if (key == KEY_SHORT_DOWN || key == KEY_LONG_DOWN) {
            if (row == 0) values[0] = prevSensorID(values[0]);
            else if (row == 1 && values[1] > 0) values[1]--;
            else values[2]--;
		}
	} while (key != KEY_LONG_CANCEL && key != KEY_SHORT_CANCEL);
}

void adjustVoltage(uint8_t* sensorArray){
	struct modelConfStruct *model = getModelModConfig();
	uint16_t adjVal = 0;
	if(sensorArray[0] == IBUS_MEAS_TYPE_INTV){
		adjVal = model->intVoltAdj;
	}
	if(sensorArray[0] == IBUS_MEAS_TYPE_EXTV){
		adjVal = model->extVoltAdj;
	}
	if(adjVal == 0 || adjVal == 10000u) return;
 	uint16_t value = sensorArray[3] << 8 | sensorArray[2];
	value = div_1(value * 10000u, adjVal);
	sensorArray[2] = value & 0xFF;
	sensorArray[3] = (value & 0xFF00) >> 8;
}

void adjustVoltageConfig(){
	uint32_t key = 0;
	struct modelConfStruct *model = getModelModConfig();
	uint16_t intVolt = model->intVoltAdj;
	uint16_t extVolt = model->extVoltAdj;

	uint16_t* adjValue = &(model->intVoltAdj);
	uint8_t index = 0;
	int32_t value = 0;
	char buffer[32];

	do {
	callSetupDMAandSend();
	displayPageHeader((char*)0xDBA1);
	uint8_t sensor = 0;
	uint8_t yPos = 0;
	for(uint8_t sensorIndex = 0; sensorIndex < 2; sensorIndex++){
		sensor = voltageSensors[sensorIndex];
		yPos = sensorIndex? 36 : 24;
		displayTextAt((char*)getSensorName(sensor), 16, yPos, 0);
		value = (uint16_t)getSensorValue(sensor, 0,0);
		if(value != 0x8000){
			formatSensorValue(buffer, sensor, value);
			displayTextAt((char*)buffer, 56, yPos,0);
		}
	}
	displayGFX((gfxInfo*) GFX_ARROW, 8, index ? 36 : 24);
	LCD_updateCALL();
	key = getKeyCode();
	if(key == KEY_SHORT_UP || key == KEY_LONG_UP) adjValue[index] -=10;
	if(key == KEY_SHORT_DOWN || key == KEY_LONG_DOWN) adjValue[index] +=10;
	else if(key == KEY_SHORT_OK) index = !index;
	else if(key == KEY_LONG_OK) adjValue[index] = 10000;
	} while ( key != KEY_LONG_CANCEL && key != KEY_SHORT_CANCEL);

	if(key != KEY_LONG_CANCEL){
		model->intVoltAdj = intVolt;
		model->extVoltAdj = extVolt;
	}
}

void getInitPressure(uint32_t* pressure, int32_t* temperature){
	struct modelConfStruct *configPtr = getModelModConfig();
	*pressure = configPtr->initAlt & 0x7FFFF;
	*temperature = configPtr->initAlt >> 19;
}

uint16_t ibusTempToK(int16_t tempertureIbus){
	return (uint16_t)(tempertureIbus - 400) + 2731;
}

int32_t log2fix(uint32_t x){
	int32_t b = 1U << (precision - 1);
	int32_t y = 0;
	while (x < 1U << precision) {
			x <<= 1;
			y -= 1U << precision;
	}

	while (x >= 2U << precision) {
			x >>= 1;
			y += 1U << precision;
	}

	uint64_t z = x;
	for (size_t i = 0; i < precision; i++) {
			z = __mul64(z, z) >> precision;
			if (z >= 2U << precision) {
					z >>= 1;
					y += b;
			}
			b >>= 1;
	}
	return y;
}


int getALT(uint32_t pressurePa, uint16_t tempertureIbus){
		if(pressurePa == 0) return 0;
    uint16_t temperatureK = ibusTempToK(tempertureIbus);
    if (initPressure <= 0) {
			getInitPressure(&initPressure, &initTemperature);
			initTemperature = ibusTempToK(initTemperature);
    }
    int temperature = (initTemperature + temperatureK) >> 1; //div 2
    bool tempNegative = temperature < 0;
    if (tempNegative)  temperature = temperature *-1;
    uint64_t helper = R_DIV_G_MUL_10_Q15;
    helper = __mul64(helper, (uint64_t)temperature);
    helper = helper >> precision;

		//div seems to take integers and return integers?!
		//so can not shift by precision because it will result in negative number

		uint32_t po_to_p = (uint32_t)(initPressure << (precision-1));
		po_to_p = div_(po_to_p, pressurePa);
		//shift missing bit
		po_to_p = po_to_p << 1;
		if(po_to_p == 0) return 0;
		uint64_t t =  __mul64(log2fix(po_to_p), INV_LOG2_E_Q1DOT31);
		int32_t ln = t >> 31;
    bool neg = ln < 0;
    if (neg) ln = ln * -1;
    helper = __mul64(helper, (uint64_t)ln);
    helper = helper >> precision;
    int result = (int)helper;

    if (neg ^ tempNegative) result = result * -1;
    return result;
}

#pragma GCC optimize ("O1")
