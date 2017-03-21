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

#ifndef SCREEN_H_
#define SCREEN_H_

#include <stdint.h>

#include "lcd.h"

#define SCREEN_BUFFER_SIZE ((LCD_WIDTH * LCD_HEIGHT) / 8)
uint8_t __attribute__((section (".screenBuffer"))) screen_buffer[SCREEN_BUFFER_SIZE];
//unsigned uint8_t __attribute__((section (".GFX_ARRAY"))) screen_buffer[SCREEN_BUFFER_SIZE];

void screen_init(void);
void screen_clear(void);
__attribute__((section (".TestCODE")))  void screen_update(void);
void screen_test(void);

void screen_fill_round_rect(uint8_t x, uint8_t y, uint8_t width, \
                            uint8_t height, uint8_t radius, uint8_t color);
void screen_fill_rect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color);
void screen_draw_round_rect(uint8_t x, uint8_t y, uint8_t width, \
                            uint8_t height, uint8_t radius, uint8_t color);
void screen_draw_rect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color);
void screen_draw_hline(uint8_t x, uint8_t y, uint8_t width, uint8_t color);
void screen_draw_round_rect(uint8_t x, uint8_t y, uint8_t width, \
                            uint8_t height, uint8_t radius, uint8_t color);
void screen_draw_rect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color);
void screen_draw_hline(uint8_t x, uint8_t y, uint8_t width, uint8_t color);
void screen_draw_vline(uint8_t x, uint8_t y, uint8_t height, uint8_t color);
void screen_set_pixels(uint8_t x, uint8_t y, uint8_t x2, uint8_t y2, uint8_t color);
void screen_draw_line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color);


uint32_t screen_strlen(uint8_t *str);

__attribute__((section (".TestCODE")))  void screen_set_font(const uint8_t *font);
__attribute__((section (".TestCODE")))  void screen_puts_xy(uint8_t x, uint8_t y, uint8_t color, uint8_t *str);
void screen_puts_xy_centered(uint8_t y, uint8_t x, uint8_t color, uint8_t *str);
void screen_puts_centered(uint8_t y, uint8_t color, uint8_t *str);
void screen_put_int8(uint8_t x, uint8_t y, uint8_t color, int8_t c);
void screen_put_uint8(uint8_t x, uint8_t y, uint8_t color, uint8_t c);
void screen_put_uint8_2dec(uint8_t x, uint8_t y, uint8_t color, uint8_t c);
void screen_put_uint8_1dec(uint8_t x, uint8_t y, uint8_t color, uint8_t c);
void screen_put_time(uint8_t x, uint8_t y, uint8_t color, int16_t time);
void screen_put_fixed2_1digit(uint8_t x, uint8_t y, uint8_t color, uint32_t v);
void screen_put_uint14(uint8_t x, uint8_t y, uint8_t color, uint16_t c);
void screen_put_hex16(uint8_t x, uint8_t y, uint8_t color, uint16_t val);
void screen_put_hex8(uint8_t x, uint8_t y, uint8_t color, uint8_t val);
void screen_put_fixed2(uint8_t x, uint8_t y, uint8_t color, uint16_t c);
__attribute__((section (".TestCODE")))  void screen_fill(uint8_t color);

#define screen_buffer_read(_addr) (screen_buffer[_addr])
#define screen_buffer_write(_addr, _val) {\
    if (_addr >= SCREEN_BUFFER_SIZE) { \
        /*Serial.write("ERROR: "); Serial.print(_addr); Serial.write("\r\n");*/ \
    } else { \
        screen_buffer[_addr] = (uint8_t)_val; \
    } \
}

#define _screen_absDiff(x, y) (((x) > (y)) ?  ((x)-(y)) : ((y)-(x)))
#define _screen_swap(a, b) {uint8_t t; t = (a); a = (b);  b = t;}

#define screen_set_dot(x, y, color) { \
  if (((x) >= LCD_WIDTH) || ((y) >= LCD_HEIGHT)) { return; } \
  if (color) { \
    screen_buffer[((y)/8)*128 + (x)] |= (1 << ((y) % 8)); \
  } else { \
    screen_buffer[((y)/8)*128 + (x)] &= ~(1 << ((y) % 8)); \
  } \
}

#endif  // SCREEN_H_

