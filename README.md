# FlySky I6 
Project description:

Firmware modification for the FlySky i6 based on modified firware developed by dave borthwick.

Latest version:

 * 386
 
https://github.com/qba667/FlySkyI6/blob/master/release/flysky-updater_386.zip
 * x64
 
https://github.com/qba667/FlySkyI6/blob/master/release/flysky-updater_x64.zip

Oryginal multiplatform updater by mhils:

https://github.com/mhils/flysky-updater

Improved version:
* Handling file size correctly
* Autodetection of serial ports
* Autodetection of FW files

New FW can be copied to update directory - updater will detect it automatically.

![alt text](https://github.com/qba667/FlySkyI6/blob/master/update.png)

Steps to update:
1. Download, extract latest revision of the mod
2. Enter firmware update mode - from menu or by moving 2 trims to center
3. Run Updater - select serial port if necessary


In case if you want to support the project and reward me the time I spent on the FW please click this affiliate link when shopping at Banggood - It is free for you, but I will get reward from Banggood: 

https://www.banggood.com/?p=SB06021048261201501J

The project is Free Software and is made available free of charge. Your donation, which is purely optional.
If you like the software, you can also consider a donation.
All donations are greatly appreciated!

[<img src="https://www.paypalobjects.com/en_US/GB/i/btn/btn_donateCC_LG.gif">](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=VSM36U6F7EN68)


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

| 2 Bytes          | 4 Bytes       | RX specific      |
| ---------------- | ------------- | ---------------- |
| Temperature      | Pressure      | Internal Voltage |
| RPM              | Latitude      | SNR              |
| External Voltage | Longitude     | Noise            |
| Avg Cell Voltage | Altitude      | RSSI             |
| Battery current  | GPS Altitude  | Error rate       |
| Remaining batt.  | s84           |                  |
| Throttle         | s85           |                  |
| Heading          | s86           |                  |
| Climb rate       | s87           |                  |
| CoG              | s88           |                  |
| GPS status       | s89           |                  |
| ACC X            | s8a           |                  |
| ACC Y            |               |                  |
| ACC Z            |               |                  |
| Roll             |               |                  |
| Pitch            |               |                  |
| Yaw              |               |                  |
| Vertical speed   |               |                  |
| Ground speed     |               |                  |
| GPS distnace     |               |                  |
| Armed            |               |                  |
| Flight mode      |               |                  |
| Odometer1        |               |                  |
| Odometer2        |               |                  |
| Speed            |               |                  |
| TX Voltage       |               |                  |

