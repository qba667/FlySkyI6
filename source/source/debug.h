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

#ifndef DEBUG_H_
#define DEBUG_H_

#include <stdint.h>

uint32_t debug_is_initialized(void);
void debug_init(void);
void debug_putc(uint8_t ch);
void debug_flush(void);
void debug(uint8_t *data);
void debug_put_hex8(uint8_t val);
void debug_put_hex16(uint16_t val);
void debug_put_hex32(uint32_t val);
void debug_put_uint8(uint8_t c);
void debug_put_int8(int8_t c);
void debug_put_uint16(uint16_t c);
void debug_put_newline(void);
void debug_put_fixed2(uint16_t c);

#endif  // DEBUG_H_
