#include <stdint.h>
#include <stddef.h>
#define ADC_VAL 0x20000F00
#define ADC_READING 0x20000F24
#define RX_BUFFER 0x20000EE4
#define RX_BUFFER_LENGTH 28
#define TX_BUFFER 0x1FFFFDBA
#define TX_BUFFER_LENGTH 38
#define MODEL_SETTINGS 0x200002A4
#define ADC_VAR_A 0x20000F10
#define ADC_SW_C 0x20000F1C
#define ADC_SW_B 0x20000F20
#define ADC_CH1 0x20000F00
#define ADC_CH2 0x20000F04
#define ADC_CH3 0x20000F08
#define ADC_CH4 0x20000F0C
#define SENSOR_ARRAY 0x200001A4
#define CHANNEL_VALUE 0x1FFFFDE0 // signed int -10k ... +10k
#define CHANNEL_VALUE_RADIO 0x1FFFFD54
#define PPM_IN_BUFFER 0x1FFFFE18 //6 * 32bit signed
#define PPM_IN_BUFFER_CH6 0x1FFFFCD4
#define GFX_ARRAY 0x1FFFF8C0
#define INPUT_STATES 0x1FFFF89C  //uint bits
#define TEMP_INPUT_STATES 0x1FFFF898 //uint bits
#define USED_MODEL_PTR 0x2000002C //PTR to current model
#define CURRENT_MODEL_INDEX 0x200002AD
#define ARROW_GFX_INFO 0xCBF8
#define SW_B_C 0xf63e

#define AUX_CH_TEXT 0xcc5a
#define SOURCE_STRING 0xCAEE
#define MEM_20000000 0x20000000
#define TIMER_SYS_TIM 0x1FFFF8B0 //SYS_TIM
#define LAST_ALARM_TIMER 0x2000003C
#define TELEMETRY_UPDATE_TIMER 0x2000001C

#define GFX_ARROW 0xCBF8
#define TEXT_VALUE 0xCAE1
#define TEXT_CHANNEL 0x9650
#define TEXT_OFF	0xCA87
#define TEXT_TIMMER	0xDCB7
#define TEXT_END_POINTS 0xCCE0
#define TEXT_HOLD 0xCAB3
#define TEXT_MIX 0xCA93
#define TEXT_ADJ 0xDBA1
#define FORMAT_TEMP 0x5530

#define KEY_SHORT_UP 9
#define KEY_SHORT_DOWN  8

#define KEY_SHORT_OK 10
#define KEY_SHORT_CANCEL 11

#define KEY_LONG_UP 23
#define KEY_LONG_DOWN 24

#define KEY_LONG_OK 22
#define KEY_LONG_CANCEL 25

#define ALARM_GT 1
#define ALARM_LT 0


typedef struct gfxInfo
{
   int width;
   int height;
   int address;
} gfxInfo;

typedef struct manuEntry{
	const char* namePtr;
	void* thumbAddress;
} manuEntry;


typedef void* (*memcpyFun)(void *dest, const void *source, size_t n);
typedef void* (*memcpyFun2)(void *source, void *target, uint32_t n);
typedef int (*sprintfFun)(char* buffer, const char* format, ...);
typedef void (*voidFun)(void);
typedef size_t (*strLenFun)(const char * str);
typedef char* (*strcatFun)(char * str, const char* source);
typedef void * (*memSetFun)(void *str, int c, size_t n);
typedef uint32_t (*divFun)(uint32_t a, uint32_t b);
typedef int32_t (*divSignedFun)(int32_t a, int32_t b);
typedef uint32_t (*divBy10Fun)(uint32_t a);
typedef int (*intVoidFun)(void);
typedef int (*eepromFun)(uint8_t * target, int16_t eepromOffset, int length);
typedef int (*gfxFun)(gfxInfo* gfx, int x, int y);
typedef int (*drawFun)(int x1, int y1, int x2, int y2, int value);
typedef int (*pixelFun)(int x1, int y1, int value);
typedef int (*displayText)(char* text, int x, int y, int val);
typedef int (*displayHeader)(char* text);
typedef uint16_t (*sensorValue)(uint8_t sensorID, uint8_t index, int zero);
typedef void (*setSenValue)(char* packet);
typedef void (*formatSensData)(uint8_t sensorID, uint8_t sensorIndex, char* packet);
typedef void (*navPage)(const char* pageName, int numberofitems, manuEntry* menuStruct);
typedef void (*beepFun)(int freq, int duration);
typedef void (*crcFun)(char* data, int length);
typedef void (*formatNumFun)(int number, char* buffer, int digits);

typedef void (*memSetFun2)(void* targetAddress, unsigned int count, uint8_t valueToSet);


__attribute__((section (".mod_MOD_SPACE.LOADSETTINGS"))) const voidFun loadSettingsFromEeprom = (voidFun)0x6005;
__attribute__((section (".mod_MOD_SPACE.CONFIGPINS"))) const voidFun configurePINs = (voidFun)0x2C01;
__attribute__((section (".mod_MOD_SPACE.NAVPAGE"))) const navPage showNavPage = (navPage)0x6DD5;
__attribute__((section (".mod_MOD_SPACE.SETTINGSVALID"))) const voidFun settingsValidation = (voidFun)0x6005;
__attribute__((section (".mod_MOD_SPACE.SPIMETHOD"))) const voidFun someSPImethod = (voidFun)0xA339;
__attribute__((section (".mod_MOD_SPACE.MEMCPY"))) const memcpyFun strcpy_ = (memcpyFun)0x198D;
__attribute__((section (".mod_MOD_SPACE.MEMCPY2"))) const memcpyFun2 memcpy_ = (memcpyFun2)0x1CF5;
__attribute__((section (".mod_MOD_SPACE.RESET"))) const voidFun restartUnit = (voidFun)0x9929;


__attribute__((section (".mod_MOD_SPACE.SPRINTF"))) const sprintfFun sprintfCall = (sprintfFun)0x19F9;
__attribute__((section (".mod_MOD_SPACE.STRCAT"))) const strcatFun strcatCall = (strcatFun)0x1C65;
__attribute__((section (".mod_MOD_SPACE.MEM_SET"))) const memSetFun2 memsetCall = (memSetFun2)0x1E0D;
__attribute__((section (".mod_MOD_SPACE.STRLEN"))) const strLenFun strLenCall = (strLenFun)0x1CB1;
//__attribute__((section (".mod_MOD_SPACE.MEMSET"))) const memSetFun memsetCall = (memSetFun)0x3201;
__attribute__((section (".mod_MOD_SPACE.SEND"))) const voidFun sendPacketWithSysTick = (voidFun)0x9EFD;
__attribute__((section (".mod_MOD_SPACE.SEND"))) const voidFun saveModelSettingsCall = (voidFun)0xABA9;
__attribute__((section (".mod_MOD_SPACE.DIVBY10")))const divBy10Fun divBy10 = (divBy10Fun)0x2165;
__attribute__((section (".mod_MOD_SPACE.DIVMOD"))) const divFun uidivmod = (divFun)0x1E5F;
__attribute__((section (".mod_MOD_SPACE.DIV"))) const divFun div_ = (divFun)0x1E7B;
__attribute__((section (".mod_MOD_SPACE.DIV2"))) const divSignedFun div_1 = (divSignedFun)0xA179;


__attribute__((section (".mod_MOD_SPACE.GETKEY"))) const intVoidFun getKeyCode = (intVoidFun)0x2535;
__attribute__((section (".mod_MOD_SPACE.GETINPUT"))) const intVoidFun getInputStates = (intVoidFun)0x3569;
__attribute__((section (".mod_MOD_SPACE.DMASEND"))) const voidFun callSetupDMAandSend = (voidFun)0x25B5;
__attribute__((section (".mod_MOD_SPACE.EEPROMTREAD"))) const eepromFun readEeprom = (eepromFun)0x3E11;
__attribute__((section (".mod_MOD_SPACE.EEPROMWRITE"))) const eepromFun saveEeprom = (eepromFun)0x3E85;
__attribute__((section (".mod_MOD_SPACE.displayGFX"))) const gfxFun displayGFX = (gfxFun)0x4BA1;
__attribute__((section (".mod_MOD_SPACE.fillRegion"))) const drawFun fillRegion = (drawFun)0x4C71;
__attribute__((section (".mod_MOD_SPACE.drawLine"))) const drawFun drawLine = (drawFun)0x4E8D;
__attribute__((section (".mod_MOD_SPACE.drawRect"))) const drawFun drawRect = (drawFun)0x5019;
__attribute__((section (".mod_MOD_SPACE.setPixel"))) const pixelFun setPixel = (pixelFun)0x4FD9;
__attribute__((section (".mod_MOD_SPACE.displayTextAt"))) const displayText displayTextAt = (displayText)0x505D;
//__attribute__((section (".mod_MOD_SPACE.displaySmallTextAt"))) const displayText displaySmallTextAt = (displayText)0x5175;
__attribute__((section (".mod_MOD_SPACE.displayPageHeader"))) const displayHeader displayPageHeader = (displayHeader)0x6D5D;
__attribute__((section (".mod_MOD_SPACE.getSensorValue"))) const sensorValue getSensorValue = (sensorValue)0x5375;
__attribute__((section (".mod_MOD_SPACE.setSensorValue"))) const setSenValue setSensorValue = (setSenValue)0x57D5;
__attribute__((section (".mod_MOD_SPACE.formatSensorData"))) const formatSensData formatSensorData = (formatSensData)0x53CD;
__attribute__((section (".mod_MOD_SPACE.updateLCD"))) const voidFun LCD_updateCALL = (voidFun)0x25BD;

__attribute__((section (".mod_MOD_SPACE.CheckAlarms"))) const voidFun CheckAlarmsCall = (voidFun)0x61ED;

__attribute__((section (".mod_MOD_SPACE.sPrintNumer"))) const formatNumFun sprintfNumer = (formatNumFun)0xA2E4;

__attribute__((section (".mod_MOD_SPACE.showNavigationPage"))) const navPage showNavigationPage = (navPage)0x6DD5;
__attribute__((section (".mod_MOD_SPACE.beep"))) const beepFun beep = (beepFun)0x9C3D;
__attribute__((section (".mod_MOD_SPACE.beepCheck"))) const intVoidFun beepCheck = (intVoidFun)0x9BC1;
__attribute__((section (".mod_MOD_SPACE.checkCRC"))) const crcFun checkCRC = (crcFun)0xA021;
__attribute__((section (".mod_MOD_SPACE.getCurrentTicks"))) const intVoidFun getCurrentTicks = (intVoidFun)0xA1B1;
__attribute__((section (".mod_MOD_SPACE.someBeepCheck"))) const intVoidFun someBeepCheck = (intVoidFun)0x9BC1;

//volatile unsigned int *adcVal = (volatile unsigned int *)0x20000F00;
//volatile unsigned int *adcRead = (volatile unsigned int *)0x20000F20;
/*
__attribute__((section (".mod_SendPacketWithSysTick"))) void SendPacketWithSysTick(void);
__attribute__((section (".mod_strLength"))) size_t _strlen( const char * str );
__attribute__((section (".mod_sprintf")))int sprintf ( char* buffer, const char* format, ...);
__attribute__((section (".mod_memset"))) void *_memset(void *str, int c, size_t n);

__attribute__((section (".mod_uidivmod"))) uint32_t uidivmod(uint32_t a, uint32_t b);
__attribute__((section (".mod_div"))) uint32_t div(uint32_t a, uint32_t b);

__attribute__((section (".mod_getKeyCode"))) int getKeyCode();
__attribute__((section (".mod_getInputStates"))) int getInputStates();
__attribute__((section (".mod_chooseSensors"))) void chooseSensors(); //must re reimpl
__attribute__((section (".mod_callSetupDMAandSend"))) void callSetupDMAandSend();
__attribute__((section (".mod_readEeprom"))) int readEeprom(uint8_t * target, int16_t eepromOffset, int length);
__attribute__((section (".mod_saveEeprom"))) int saveEeprom(uint8_t * target, int16_t eepromOffset, int length);
__attribute__((section (".mod_displayGFX"))) int displayGFX(gfxInfo* gfx, int x, int y);
__attribute__((section (".mod_fillRegion"))) int fillRegion(int x1, int y1, int x2, int y2, int value);  //value 0 for clear 1 for fill
__attribute__((section (".mod_drawLine"))) int drawLine(int x1, int y1, int x2, int y2, int value);  //value always 0
__attribute__((section (".mod_setPixel"))) int setPixel(int x1, int y1, int value);  //value always 0
__attribute__((section (".mod_drawRect"))) signed int drawRect(int x1, signed int y1, int x2, signed int y2, int value);//value is always 0
__attribute__((section (".mod_displayTextAt"))) signed int displayTextAt(char* text, int x, int y, int val);//value is always 0
__attribute__((section (".mod_displayPageHeader"))) signed int displayPageHeader(char* text);
//inline __attribute__((section (".mod_displaySmallTextAt"))) signed int displaySmallTextAt(int x1, signed int y1, int x2, signed int y2, int value){return 0;}//value is always 0

__attribute__((section (".mod_getSensorValue"))) int getSensorValue(int sensorID, int index);//value is always 0
//s_formatSensorData
__attribute__((section (".mod_getSensorValue"))) int setSensorValue(char* sensorArray);
__attribute__((section (".mod_updateVoltageTelemetry"))) void updateVoltageTelemetry();

__attribute__((section (".mod_checkAlarms"))) void checkAlarms(); //reimplement


__attribute__((section (".mod_sensorsListMainScreen"))) void sensorsListMainScreen(); //already moved find correct lcoaotpn
__attribute__((section (".mod_showNavigationPage"))) signed int showNavigationPage(char* sensorArray, int numberofitems, manuEntry* menuStruct); //already moved find correct lcoaotpn


__attribute__((section (".mod_beep"))) void beep(int freq, int duration);
__attribute__((section (".mod_crcCheck"))) int crcCheck(char* data, int length);
__attribute__((section (".mod_getCurrentTicks"))) int getCurrentTicks();
*/



/*

*/
