/*
    Copyright 2016 fishpepper <AT> gmail.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    author: fishpepper <AT> gmail.com
*/

#ifndef LCD_H_
#define LCD_H_

#include <stdint.h>
#include "MKL16Z4.h"
#include "fsl_gpio.h"

#define LCD_WIDTH   128
#define LCD_HEIGHT   64

__attribute__((section (".TestCODE"))) void lcd_init(void);
__attribute__((section (".TestCODE"))) void lcd_send_data(const uint8_t *buf);
__attribute__((section (".TestCODE"))) void lcd_powerdown(void);
__attribute__((section (".TestCODE"))) void lcd_show_logo(void);

#define LCD_RW_HI()   { GPIOE->PSOR = 0x8000; }
#define LCD_RW_LO()   { GPIOE->PCOR = 0x8000; }
#define LCD_RST_HI()  { GPIOD->PSOR = 0x80; }
#define LCD_RST_LO()  { GPIOD->PCOR = 0x80; }
#define LCD_RD_HI()   { GPIOE->PSOR = 0x4000; }
#define LCD_RD_LO()   { GPIOE->PCOR = 0x4000; }
#define LCD_RS_HI()   { GPIOA->PSOR = 0x4000; }
#define LCD_RS_LO()   { GPIOA->PCOR = 0x4000;}
#define LCD_CS_HI()   { GPIOA->PSOR = 0x10; }
#define LCD_CS_LO()   { GPIOA->PCOR = 0x10; }



#define LCD_CMD_RESET            0xE2
#define LCD_CMD_BIAS_1_9         0xA2  // 1/9 bias
#define LCD_CMD_BIAS_1_7         0xA3  // 1/7 bias

#define LCD_CMD_SEG_NORMAL       0xA0
#define LCD_CMD_SEG_INVERSE      0xA1

#define LCD_CMD_COM_NORMAL       0xC0
#define LCD_CMD_COM_INVERSE      0xC8

#define LCD_CMD_REG_RATIO_000    0x20
#define LCD_CMD_REG_RATIO_001    0x21
#define LCD_CMD_REG_RATIO_010    0x22
#define LCD_CMD_REG_RATIO_011    0x23
#define LCD_CMD_REG_RATIO_100    0x24
#define LCD_CMD_REG_RATIO_101    0x25
#define LCD_CMD_REG_RATIO_110    0x26
#define LCD_CMD_REG_RATIO_111    0x27

#define LCD_CMD_EV               0x81
#define LCD_CMD_POWERCTRL_ALL_ON 0x2F

#define LCD_CMD_SET_STARTLINE    0x40
#define LCD_CMD_SET_PAGESTART    0xB0

#define LCD_CMD_SET_COL_LO       0x00
#define LCD_CMD_SET_COL_HI       0x10

#define LCD_CMD_DISPLAY_OFF      0xAE
#define LCD_CMD_DISPLAY_ON       0xAF

#define LCD_CMD_MODE_RAM         0xA4
#define LCD_CMD_MODE_ALLBLACK    0xA5

#endif  // LCD_H_

