# FlySky I6 
Project description:

Firmware modification for the FlySky i6 based on modified firware developed by dave borthwick.

Implemted features:

* Configurable flight timer

* Configurable telemetry alarms

* Configurable battery 

* Basic telemetry from RX (RSSI, Nosie, SNR)

* Advanced Telemetry

* Configurable output of RX (PPM/PWM/SBUS/IBUS) 

* 3 positions Switch B

* Variable in place of Switch C

* Using PPM input for custom channels

* Refefined main screen

* Channels 12-14 sendinng PPM values 6,5,4

* RSSI (Error) send on channel 11

* Handling AC frames (up to 28 bytes)

* ...

Supported telemetry:

*Internal Voltage

*Temperature

*RPM

*External Voltage

*Avg Cell voltage

*Battery current A * 100

*Remaining battery percentage / mah drawn otherwise or fuel level no unit!

*Throttle value / battery capacity

*Heading  0..360 deg, 0=north 

*Climb rate

*Course over ground(NOT heading, but direction of movement) in degrees * 100, 0.0..359.99 degrees. unknown max uint

*GPS status

*ACC X

*ACC Y

*ACC Z

*Roll

*Pitch

*Yaw

*Vertical speed

*Ground speed

*GPS distnace

*Armed 

*Flight mode

*Pressure

*Odometer1

*Odometer2

*Speed

*TX Voltage

*Latitude

*Longitude

*Altitude 

*GPS Altitude

*4 byte sensor s84

*4 byte sensor s85

*4 byte sensor s86

*4 byte sensor s87

*4 byte sensor s88

*4 byte sensor s89

*4 byte sensor s8a

*SNR

*Noise

*RSSI

*Error rate
  



