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

#ifndef CONSOLE_H_
#define CONSOLE_H_

#include <stdint.h>
#include "font.h"

// rendering color for text, background is the inverse (allowed: 0,1)
#define CONSOLE_TEXTCOLOR 1

__attribute__((section (".TestCODE"))) void console_init(void);
__attribute__((section (".TestCODE"))) void console_clear(void);

__attribute__((section (".TestCODE"))) static void console_render_str(uint8_t line, uint8_t color, uint8_t *str);
__attribute__((section (".TestCODE"))) void console_puts(uint8_t *str);
__attribute__((section (".TestCODE"))) void console_putc(uint8_t c);
__attribute__((section (".TestCODE"))) void console_render(void);

// you can define the console font here. make sure to use FIXED WIDTH fonts!
// make sure to set width and height properly
// #define CONSOLE_FONT font_system5x7
//#define CONSOLE_FONT font_system5x7
#define CONSOLE_FONT font_system5x7
//#define CONSOLE_FONT lucidaConsole_8ptBitmaps
#define CONSOLE_FONT_WIDTH  5
#define CONSOLE_FONT_HEIGHT 7

// calculate how many chars can be printed on the console
#define CONSOLE_BUFFER_SIZE_X (LCD_WIDTH  / (CONSOLE_FONT_WIDTH+1))
#define CONSOLE_BUFFER_SIZE_Y ((LCD_HEIGHT / (CONSOLE_FONT_HEIGHT+1))+1)

#endif  // CONSOLE_H_

