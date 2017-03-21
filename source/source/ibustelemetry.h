/*
 * ibustelemetry.h
 *
 *  Created on: 19 lut 2017
 *      Author: Kuba
 */


#ifndef SOURCE_IBUSTELEMETRY_H_
#define SOURCE_IBUSTELEMETRY_H_

#define IBUS_MEAS_TYPE_INTV		0x00    // Internal Voltage
#define IBUS_MEAS_TYPE_TEM		0x01    // Temperature
#define IBUS_MEAS_TYPE_MOT		0x02    // RPM
#define IBUS_MEAS_TYPE_EXTV		0x03    // External Voltage
#define IBUS_MEAS_TYPE_CMP_HEAD	0x04 //Heading  0..360 deg, 0=north 2bytes
#define IBUS_MEAS_TYPE_BAT_CURR	0x05 //battery current  10*milliamperes (1 = 10 milliampere) 2byts
#define IBUS_MEAS_TYPE_CLIMB_RATE 0x06 //2bytes m/s *100
#define IBUS_MEAS_TYPE_ACC_X	0x09 //pitch 2bytes	deg * 100.0 signed!!!
#define IBUS_MEAS_TYPE_ACC_Y	0x08 //roll 2bytes deg * 100.0 	signed!!!
#define IBUS_MEAS_TYPE_ACC_Z	0x07 //yaw 2bytes deg * 100.0	signed!!!
#define IBUS_MEAS_TYPE_VERTICAL_SPEED	0x0a //2byte m/s *100
#define IBUS_MEAS_TYPE_GROUND_SPEED	0x0b //2byte m/s *100 different unit than build-in sensor
#define IBUS_MEAS_TYPE_GPS_DIST	0x0c //calc?
#define IBUS_MEAS_TYPE_ARMED	0x0d //2 bytes
#define IBUS_MEAS_TYPE_FLIGHT_MODE	0x0e //2 bytes simple index listed below
#define IBUS_MEAS_TYPE_TIMER	0x0f
#define IBUS_MEAS_TYPE_MODEL	0x10
#define IBUS_MEAS_TYPE_RESERV_1	0x11
#define IBUS_MEAS_TYPE_RESERV_2	0x12

#define IBUS_MEAS_TYPE_PRES		0x41    // Pressure
#define IBUS_MEAS_TYPE_ODO1		0x7c    // Odometer1
#define IBUS_MEAS_TYPE_ODO2		0x7d    // Odometer2
#define IBUS_MEAS_TYPE_SPE		0x7e    // Speed			//2byte km/h
#define IBUS_MEAS_TYPE_TX_V		0x7f    // Speed			//2byte km/h

#define IBUS_MEAS_TYPE_COG				0x80 //2bytes  Course over ground(NOT heading, but direction of movement) in degrees * 100, 0.0..359.99 degrees. unknown max uint
#define IBUS_MEAS_TYPE_GPS_STATUS		0x81 //2bytes
//4 byte sensors
#define IBUS_MEAS_TYPE_GPS_LON			0x82 //4bytes signed WGS84 in degrees * 1E7
#define IBUS_MEAS_TYPE_GPS_LAT			0x83 //4bytes signed WGS84 in degrees * 1E7
#define IBUS_MEAS_TYPE_GPS_ALT			0x84 //2bytes signed!!! GPS alt
#define IBUS_MEAS_TYPE_S85				0x85
#define IBUS_MEAS_TYPE_S86				0x86
#define IBUS_MEAS_TYPE_S87				0x87
#define IBUS_MEAS_TYPE_S88				0x88
#define IBUS_MEAS_TYPE_S89				0x89
#define IBUS_MEAS_TYPE_S8A				0x8A
#define IBUS_MEAS_TYPE_S8B				0x8B
#define IBUS_MEAS_TYPE_S8c				0x8C
#define IBUS_MEAS_TYPE_S8d				0x8D
#define IBUS_MEAS_TYPE_S8e				0x8E
#define IBUS_MEAS_TYPE_S8f				0x8F
#define IBUS_MEAS_TYPE_S90				0x90
#define IBUS_MEAS_TYPE_S91				0x91
#define IBUS_MEAS_TYPE_S92				0x92

#define IBUS_MEAS_TYPE_ALT		0xf9    // Altitude			//2 bytes signed in m
#define IBUS_MEAS_TYPE_SNR		0xfa    // SNR
#define IBUS_MEAS_TYPE_NOISE	0xfb    // Noise
#define IBUS_MEAS_TYPE_RSSI		0xfc    // RSSI
#define IBUS_MEAS_TYPE_ERR		0xfe    // Error rate

#define IBUS_MEAS_TYPE_GPS_FULL	0xfd //1byte fix 1 byte satellites 4 bytes LAT 4 bytes LON 4bytes alt



#endif /* SOURCE_IBUSTELEMETRY_H_ */
