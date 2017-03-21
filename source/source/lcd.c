
#include "lcd.h"
#include "fsl_port.h"
#include "delay.h"




static void lcd_init_rcc(void) {
	CLOCK_EnableClock(kCLOCK_PortA);
	CLOCK_EnableClock(kCLOCK_PortC);
	CLOCK_EnableClock(kCLOCK_PortD);
	CLOCK_EnableClock(kCLOCK_PortE);
}

static void lcd_data_set(uint8_t data){
	GPIOE->PDOR = (GPIOE->PDOR & 0xFFC3FFFF) | ((data << 18) & 0x3FFFFF);// port e
	GPIOC->PDOR = (GPIOC->PDOR & 0xFFFFF0FF) | ((data >> 4 << 8)); // portc
}


static void lcd_write_command(uint8_t data, uint8_t isGFX ) {
	//GPIOA13 A0
	if(isGFX) GPIOA->PSOR = 0x2000;
	else GPIOA->PCOR = 0x2000;
	GPIOE->PCOR = 0x80000000; //RW
	GPIOA->PCOR = 0x10;		  //CS
    lcd_data_set(data);
    GPIOE->PSOR = 0x40000000;	//RD
    GPIOE->PCOR = 0x40000000;	//RD
    GPIOA->PSOR = 0x10;			//CS
}

static void lcd_reset(void) {
    // wait for voltages to be stable
	_delay_ms(100);
    // trigger hw reset
    LCD_RST_LO();
    delay_us(20);  // at least 5us
    LCD_RST_HI();
    delay_us(20);  // at least 5us?

    lcd_write_command(0xE2, 0);
    lcd_write_command(0xAE, 0);
    lcd_write_command(0xA1, 0);
    lcd_write_command(0xA6, 0);
    lcd_write_command(0xA4, 0);
    lcd_write_command(0xA2, 0);
    lcd_write_command(0xC0, 0);
    lcd_write_command(0x2F, 0);
    lcd_write_command(0x24, 0);
    lcd_write_command(0x81, 0);
    lcd_write_command(0x28, 0);
    lcd_write_command(0xAF, 0);

    int index = 0;
    int page = 0;
      do
      {
    	lcd_write_command(page | 0xB0, 0);
    	lcd_write_command(4u, 0);
    	lcd_write_command(0x10u, 0);
        int line = 4;

        do
        {
        	lcd_write_command(0, 1);
          ++line;
        }
        while ( line < 132 );
        ++page;
      }
      while ( page < 8 );

/*
    // send software reset command
    lcd_write_command(LCD_CMD_RESET);

    // switch display off
    lcd_write_command(LCD_CMD_DISPLAY_OFF);

    // do not set all pixels to black(..._ALLBLACK)
    lcd_write_command(LCD_CMD_MODE_RAM);

    // set to 1/ 7 bias:
    lcd_write_command(LCD_CMD_BIAS_1_7);  // opentx uses LCD_CMD_BIAS_1_9

    // set seg and com directions
    lcd_write_command(LCD_CMD_COM_NORMAL);  // INVERSE);
    // normal or inverted?
    lcd_write_command(LCD_CMD_SEG_INVERSE);  // NORMAL);

    // power control -> all boosters on
    lcd_write_command(LCD_CMD_POWERCTRL_ALL_ON);

    // regulator ratio
    lcd_write_command(LCD_CMD_REG_RATIO_011);  // opentx uses LCD_CMD_REG_RATIO_101 ?

    // set EV(contrast?), this is  2byte command
    lcd_write_command(LCD_CMD_EV);
    lcd_write_command(40);  // dynamic contrast 0...63

    // set startline to 0
    lcd_write_command(LCD_CMD_SET_STARTLINE + 0);
    // set start page to 0
    lcd_write_command(LCD_CMD_SET_PAGESTART + 0);

    // set col address of ram to 0
    lcd_write_command(LCD_CMD_SET_COL_LO + 0);
    lcd_write_command(LCD_CMD_SET_COL_HI + 0);

    // switch display on
    lcd_write_command(LCD_CMD_DISPLAY_ON);
    */
}




static void lcd_init_gpio(void) {
	//A4
	//v40049010 = (v40049010 & 0xFFFFF8F8) + 256; CS_PIN
	PORT_SetPinMux(PORTA, 4, kPORT_MuxAsGpio);
	//D7
	//v4004C01C = (v4004C01C & 0xFFFFF8F8) + 256; PIN_RST
	PORT_SetPinMux(PORTD, 7, kPORT_MuxAsGpio);
	//A13
	//v40049034 = (v40049034 & 0xFFFFF8F8) + 256; RS_PIN?
	PORT_SetPinMux(PORTA, 13, kPORT_MuxAsGpio);
	//E31
	//v4004D07C = (v4004D07C & 0xFFFFF8F8) + 256; RW_PIN
	PORT_SetPinMux(PORTE, 31, kPORT_MuxAsGpio);
	//E30
	//v4004D078 = (v4004D078 & 0xFFFFF8F8) + 256; RD_PIN
	PORT_SetPinMux(PORTE, 30, kPORT_MuxAsGpio);
	//E18 19 20 21
	/*
	 v4004D048 = (v4004D048 & 0xFFFFF8F8) + 256;
  	 v4004D04C = (v4004D04C & 0xFFFFF8F8) + 256;
  	 v4004D050 = (v4004D050 & 0xFFFFF8F8) + 256;
  	 v4004D054 = (v4004D054 & 0xFFFFF8F8) + 256;
	 */
	PORT_SetPinMux(PORTE, 18, kPORT_MuxAsGpio);
	PORT_SetPinMux(PORTE, 19, kPORT_MuxAsGpio);
	PORT_SetPinMux(PORTE, 20, kPORT_MuxAsGpio);
	PORT_SetPinMux(PORTE, 21, kPORT_MuxAsGpio);

	//C8 C9 C10 C11
	/*
	 v4004B020 = (v4004B020 & 0xFFFFF8F8) + 256;
	 v4004B024 = (v4004B024 & 0xFFFFF8F8) + 256;
	 v4004B028 = (v4004B028 & 0xFFFFF8F8) + 256;
	 v4004B02C = (v4004B02C & 0xFFFFF8F8) + 256;
	*/
	PORT_SetPinMux(PORTC, 8, kPORT_MuxAsGpio);
	PORT_SetPinMux(PORTC, 9, kPORT_MuxAsGpio);
	PORT_SetPinMux(PORTC, 10, kPORT_MuxAsGpio);
	PORT_SetPinMux(PORTC, 11, kPORT_MuxAsGpio);

	//GPIOA_PDDR
	GPIOA->PDDR |= 0x2010u;
	//GPIOD_PDDR
	GPIOD->PDDR |= 0x80u;
	//GPIOE_PDDR
	GPIOE->PDDR |= 0xC03C0000u;
	//GPIOC_PDDR
	GPIOC->PDDR |= 0xF00u;
	//clear a0 a13
	GPIOA->PCOR = 0x2010;
	//clear d7
	GPIOD->PCOR = 0x80;
	//clear e
	GPIOE->PCOR = 0xC0000000;
}

void lcd_init(void) {
	lcd_init_rcc();
    lcd_init_gpio();
    lcd_reset();
}


void lcd_powerdown(void) {
    // switch display off
    lcd_write_command(LCD_CMD_DISPLAY_OFF, 0);

    // all pixels on
    lcd_write_command(LCD_CMD_MODE_ALLBLACK, 0);

    // wait
    // wdt_reset();
    _delay_ms(100);
    //wdt_reset();
    _delay_ms(100);
    //wdt_reset();
    _delay_ms(100);
}

void lcd_send_data(const uint8_t *buf) {
	int page = 0;
	do {
		lcd_write_command(page | 0xB0, 0);
		lcd_write_command(4u, 0);
		lcd_write_command(0x10u, 0);

		int line = 4;
		do {
			lcd_write_command(*buf++, 1);
			++line;
		} while (line < 132);
		++page;
	} while (page < 8);

}

/*
void lcd_send_data(const uint8_t *buf) {
    uint32_t x, y;


    // set start to 0,0
    lcd_write_command(LCD_CMD_SET_STARTLINE + 0,0);
    lcd_write_command(LCD_CMD_SET_PAGESTART + 2,0);

    // set col address of ram to 0
    lcd_write_command(LCD_CMD_SET_COL_LO + 0,0);
    lcd_write_command(LCD_CMD_SET_COL_HI + 0,0);



    for (y = 0; y < 8; y++) {
        // start on col 0
        lcd_write_command(LCD_CMD_SET_COL_LO + 0,0);
        lcd_write_command(LCD_CMD_SET_COL_HI + 0,0);
        // page start
        lcd_write_command(LCD_CMD_SET_PAGESTART + y,0);

        LCD_CS_LO();
        LCD_RS_HI();
        LCD_RW_LO();

        // send 4 dummy bytes(132-128)
        for (x = 4; x > 0; --x) {
            lcd_data_set(0x00);
            LCD_RD_HI();
            LCD_RD_LO();
        }

        for (x = 128; x > 0; --x) {
            lcd_data_set(*buf++);
            // execute write
            LCD_RD_HI();
            LCD_RD_LO();
        }
    }

    LCD_RD_HI();

    // deselect device
    LCD_CS_HI();
    LCD_RW_HI();
}*/

void lcd_show_logo(void) {
    //lcd_send_data(logo_data);
}
