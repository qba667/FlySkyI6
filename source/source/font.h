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

#ifndef FONT_H_
#define FONT_H_

#include <stdint.h>

// font indices
#define FONT_LENGTH       0
#define FONT_FIXED_WIDTH  2
#define FONT_HEIGHT       3
#define FONT_FIRST_CHAR   4
#define FONT_CHAR_COUNT   5
#define FONT_WIDTH_TABLE  6

// helpers
#define font_is_fixed_width(_f)  ((_f[FONT_LENGTH] == 0) && (_f[FONT_LENGTH+1] < 2))
#define font_is_nopad_fixed_font(_f)  ((_f[FONT_LENGTH] == 0) && (_f[FONT_LENGTH+1] == 1))

extern __attribute__((section (".TestCODE"))) const uint8_t font_system5x7[];
extern const uint8_t font_tomthumb3x5[];
extern const uint8_t font_metric15x26[];
extern const uint8_t font_metric7x12[];
// Font data for Lucida Console 8pt
extern const uint8_t lucidaConsole_8ptBitmaps[];
extern const uint8_t font_arial[];

#endif  // FONT_H_
