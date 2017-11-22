/*
 * ibustelemetry.h
 *
 *  Created on: 19 lut 2017
 *      Author: Kuba
 */


#ifndef SOURCE_IBUSTELEMETRY_H_
#define SOURCE_IBUSTELEMETRY_H_

#define IBUS_MEAS_TYPE_INTV				0x00    // Internal Voltage
#define IBUS_MEAS_TYPE_TEM				0x01    // Temperature
#define IBUS_MEAS_TYPE_MOT				0x02    // RPM
#define IBUS_MEAS_TYPE_EXTV				0x03    // External Voltage
#define IBUS_MEAS_TYPE_CELL				0x04    // Avg Cell voltage
#define IBUS_MEAS_TYPE_BAT_CURR			0x05    // battery current A * 100
#define IBUS_MEAS_TYPE_FUEL				0x06	// remaining battery percentage / mah drawn otherwise or fuel level no unit!
#define IBUS_MEAS_TYPE_RPM				0x07	// throttle value / battery capacity
#define IBUS_MEAS_TYPE_CMP_HEAD			0x08 	//Heading  0..360 deg, 0=north 2bytes
#define IBUS_MEAS_TYPE_CLIMB_RATE 		0x09  	//2 bytes m/s *100
#define IBUS_MEAS_TYPE_COG				0x0a 	//2 bytes  Course over ground(NOT heading, but direction of movement) in degrees * 100, 0.0..359.99 degrees. unknown max uint
#define IBUS_MEAS_TYPE_GPS_STATUS		0x0b 	//2 bytes
#define IBUS_MEAS_TYPE_ACC_X			0x0c 	//2 bytes m/s *100 signed
#define IBUS_MEAS_TYPE_ACC_Y			0x0d 	//2 bytes m/s *100 signed
#define IBUS_MEAS_TYPE_ACC_Z			0x0e 	//2 bytes m/s *100 signed
#define IBUS_MEAS_TYPE_ROLL				0x0f 	//2 bytes deg *100 signed
#define IBUS_MEAS_TYPE_PITCH			0x10 	//2 bytes deg *100 signed
#define IBUS_MEAS_TYPE_YAW				0x11 	//2 bytes deg *100 signed
#define IBUS_MEAS_TYPE_VERTICAL_SPEED	0x12 	//2 bytes m/s *100
#define IBUS_MEAS_TYPE_GROUND_SPEED		0x13 	//2 bytes m/s *100 different unit than build-in sensor
#define IBUS_MEAS_TYPE_GPS_DIST			0x14 	//2 bytes dist from home m unsigned
#define IBUS_MEAS_TYPE_ARMED			0x15 	//2 bytes
#define IBUS_MEAS_TYPE_FLIGHT_MODE		0x16 	//2 bytes simple index listed below


#define IBUS_MEAS_TYPE_PRES				0x41    // Pressure
#define IBUS_MEAS_TYPE_ODO1				0x7c    // Odometer1
#define IBUS_MEAS_TYPE_ODO2				0x7d    // Odometer2
#define IBUS_MEAS_TYPE_SPE				0x7e    // Speed			//2byte km/h
#define IBUS_MEAS_TYPE_TX_V				0x7f    // TX Voltage


//4 byte sensors
#define IBUS_MEAS_TYPE_GPS_LAT			0x80 //4bytes signed WGS84 in degrees * 1E7
#define IBUS_MEAS_TYPE_GPS_LON			0x81 //4bytes signed WGS84 in degrees * 1E7
#define IBUS_MEAS_TYPE_GPS_ALT			0x82 //4bytes signed!!! GPS alt m*100
#define IBUS_MEAS_TYPE_ALT				0x83 //4bytes signed!!! Alt m*100
#define IBUS_MEAS_TYPE_ALT_MAX			0x84 //4bytes signed MaxAlt m*100
#define IBUS_MEAS_TYPE_S85				0x85
#define IBUS_MEAS_TYPE_S86				0x86
#define IBUS_MEAS_TYPE_S87				0x87
#define IBUS_MEAS_TYPE_S88				0x88
#define IBUS_MEAS_TYPE_S89				0x89
#define IBUS_MEAS_TYPE_S8a				0x8a


//#define IBUS_MEAS_TYPE_ALT_FLYSKY		0xf9    // Altitude			//2 bytes signed in m
#define IBUS_MEAS_TYPE_SNR				0xfa    // SNR
#define IBUS_MEAS_TYPE_NOISE			0xfb    // Noise
#define IBUS_MEAS_TYPE_RSSI				0xfc    // RSSI
#define IBUS_MEAS_TYPE_ERR				0xfe    // Error rate
#define IBUS_MEAS_TYPE_UNKNOWN			0xff


#define IBUS_MEAS_TYPE_GPS_FULL			0xfd
#define IBUS_MEAS_TYPE_VOLT_FULL		0xf0
#define IBUS_MEAS_TYPE_ACC_FULL			0xef


const uint8_t FULL_GPS_IDS[] = {
	IBUS_MEAS_TYPE_GPS_STATUS,
	IBUS_MEAS_TYPE_GPS_LAT,
	IBUS_MEAS_TYPE_GPS_LON,
	IBUS_MEAS_TYPE_GPS_ALT
};

const uint8_t FULL_VOLT_IDS[] = {
	IBUS_MEAS_TYPE_EXTV,
	IBUS_MEAS_TYPE_CELL,
	IBUS_MEAS_TYPE_BAT_CURR,
	IBUS_MEAS_TYPE_FUEL,
	IBUS_MEAS_TYPE_RPM,
};

const uint8_t FULL_ACC_IDS[] = {
	IBUS_MEAS_TYPE_ACC_X,
	IBUS_MEAS_TYPE_ACC_Y,
	IBUS_MEAS_TYPE_ACC_Z,
	IBUS_MEAS_TYPE_ROLL,
	IBUS_MEAS_TYPE_PITCH,
	IBUS_MEAS_TYPE_YAW
};

#endif /* SOURCE_IBUSTELEMETRY_H_ */
