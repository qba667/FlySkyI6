# FlySky I6 
Project description:

Firmware modification for the FlySky i6 based on modified firware developed by dave borthwick.

Latest version:
386
https://github.com/qba667/FlySkyI6/blob/master/release/flysky-updater_386.zip
x64
https://github.com/qba667/FlySkyI6/blob/master/release/flysky-updater_x64.zip

Oryginal multiplatform updater by mhils:

https://github.com/mhils/flysky-updater

Improved version is handling file size correctly, autodetection serial ports and FW files.
New FW can be copied to update directory - updater will detect it automatically.

![alt text](https://github.com/qba667/FlySkyI6/blob/master/update.png)

Steps to update:
1.Download, extract latest revision of the mod
2.Enter firmware update mode - frome menu or by moving 2 trims to center
3.Run Updater - select serial port if nescesary 

As adviced by povlhp in case if you want to support the project and reward me the time I spent on the FW please click this affiliate link when shopping at Banggood - It is free for you, but I will get reward from Banggood: 

https://www.banggood.com/?p=SB06021048261201501J

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

* Internal Voltage

* Temperature

* RPM

* External Voltage

* Avg Cell voltage

* Battery current A * 100

* Remaining battery percentage / mah drawn otherwise or fuel level no unit!

* Throttle value / battery capacity

* Heading  0..360 deg, 0=north 

* Climb rate

* Course over ground(NOT heading, but direction of movement) in degrees * 100, 0.0..359.99 degrees. unknown max uint

* GPS status

* ACC X

* ACC Y

* ACC Z

* Roll

* Pitch

* Yaw

* Vertical speed

* Ground speed

* GPS distnace

* Armed 

* Flight mode

* Pressure

* Odometer1

* Odometer2

* Speed

* TX Voltage

* Latitude

* Longitude

* Altitude 

* GPS Altitude

* 4 byte sensor s84

* 4 byte sensor s85

* 4 byte sensor s86

* 4 byte sensor s87

* 4 byte sensor s88

* 4 byte sensor s89

* 4 byte sensor s8a

* SNR

* Noise

* RSSI

* Error rate
