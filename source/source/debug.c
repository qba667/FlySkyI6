/*
    Copyright 2016 fishpepper <AT> gmail.com

    This program is free software: you can redistribute it and/ or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http:// www.gnu.org/licenses/>.

    author: fishpepper <AT> gmail.com
*/

#include "debug.h"
#include "delay.h"
#include "console.h"
#include "screen.h"
#include <stdint.h>

static uint8_t debug_init_done;

void debug_init(void) {
    debug_init_done = 1;

    debug_put_newline();
    //     #####################
    debug("###  OpenGround  ###\n");
    debug(" (c) by fishpepper  \n\n");
    debug("debug: init done\n");

    debug("debug: ");

    debug("FLYSKY/TGY I6");

    debug("\n");
}

void debug_putc(uint8_t ch) {
    // add \r to newlines
    // if (ch == '\n') debug_putc('\r');
    console_putc(ch);
}

void debug_flush(void) {
    if (!debug_init_done) {
        return;
    }
    console_render();
    screen_update();
    /*
    if (!gui_running()) {
        // if gui is not yet active, render console now
        if (adc_get_channel_rescaled(CHANNEL_ID_CH3) < 0) {
            // show console on switch down
            console_render();
            screen_update();
        } else {
            lcd_show_logo();
        }
    }*/
}


void debug(uint8_t *data) {
    uint8_t c = *data++;
    while (c) {
        debug_putc(c);
        c = *data++;
    }
}


// put hexadecimal number to debug out.
void debug_put_hex8(uint8_t val) {
    uint8_t lo = val&0x0F;
    uint8_t hi = val>>4;
    if (hi < 0x0A) {
        hi = '0' + hi;
    } else {
        hi = 'A' - 0x0A + hi;
    }

    if (lo < 0x0A) {
        lo = '0' + lo;
    } else {
        lo = 'A' - 0x0A + lo;
    }
    debug_putc(hi);
    debug_putc(lo);
}

// put 16bit hexadecimal number to debug out
void debug_put_hex16(uint16_t val) {
    debug_put_hex8(val>>8);
    debug_put_hex8(val & 0xFF);
}

// put 32bit hexadecimal number to debug out
void debug_put_hex32(uint32_t val) {
    debug_put_hex8(val>>24);
    debug_put_hex8(val>>16);
    debug_put_hex8(val>> 8);
    debug_put_hex8(val & 0xFF);
}

// output a signed 8-bit number to uart
void debug_put_int8(int8_t c) {
    uint8_t tmp;
    uint8_t mul;
    uint8_t l;
    uint8_t uint_s;

    if (c < 0) {
        debug_putc('-');
        uint_s = -c;
    } else {
        uint_s = c;
    }

    l = 0;
    for (mul = 100; mul > 0; mul = mul/ 10) {
        tmp = '0';
        while (uint_s >= mul) {
            uint_s -= mul;
            tmp++;
            l = 1;
        }
        if ((l == 0) && (tmp == '0') && (mul != 1)) {
            // dont print spacer
            // debug_putc(' ');
        } else {
            debug_putc(tmp);
        }
    }
}

// output an unsigned 8-bit number to uart
void debug_put_uint8(uint8_t c) {
    uint8_t tmp;
    uint8_t mul;
    uint8_t l;

    l = 0;
    for (mul = 100; mul >0 ; mul = mul/ 10) {
        tmp = '0';
        while (c >= mul) {
            c -= mul;
            tmp++;
            l = 1;
        }
        if ((l == 0) && (tmp == '0') && (mul != 1)) {
            // dont print spacer
            // debug_putc(' ');
        } else {
            debug_putc(tmp);
        }
    }
}

// output an unsigned 16-bit number to uart
void debug_put_uint16(uint16_t c) {
    uint8_t tmp;
    uint8_t l = 0;
    // loop unrolling is better(no int16 arithmetic)
    /*for (mul = 10000; mul>0; mul = mul/ 10) {
        uint16_t mul;

        l = 0;
                tmp = '0';
                while (c>=mul) {
                        c -= mul;
                        tmp++;
                        l = 1;
                }
                if ((l == 0) && (tmp == '0') && (mul!=1)) {
                        // debug_putc(' ');
                } else {
                        debug_putc(tmp);
                }
        }*/
    tmp = 0;
    while (c >= 10000L) {
        c -= 10000L;
        tmp++;
        l = 1;
    }
    if (tmp != 0) debug_putc('0' + tmp);

    tmp = 0;
    while (c >= 1000L) {
        c -= 1000L;
        tmp++;
        l = 1;
    }
    if (l || (tmp != 0)) debug_putc('0' + tmp);

    tmp = 0;
    while (c >= 100) {
        c -= 100;
        tmp++;
        l = 1;
    }
    if (l || (tmp != 0)) debug_putc('0' + tmp);

    tmp = 0;
    while (c >= 10) {
        c -= 10;
        tmp++;
        l = 1;
    }
    if (l || (tmp != 0)) debug_putc('0' + tmp);

    debug_putc('0' + (uint8_t)c);
}

void debug_put_fixed2(uint16_t c) {
    uint8_t tmp;
    uint8_t l = 0;
    tmp = 0;
    while (c >= 10000L) {
        c -= 10000L;
        tmp++;
        l = 1;
    }
    if (tmp != 0) debug_putc('0' + tmp);

    tmp = 0;
    while (c >= 1000L) {
        c -= 1000L;
        tmp++;
        l = 1;
    }
    if (l || (tmp != 0)) debug_putc('0' + tmp);

    tmp = 0;
    while (c >= 100) {
        c -= 100;
        tmp++;
        l = 1;
    }
    if (l || (tmp != 0)) debug_putc('0' + tmp);

    debug_putc('.');

    tmp = 0;
    while (c >= 10) {
        c -= 10;
        tmp++;
        l = 1;
    }
    if (l || (tmp != 0)) debug_putc('0' + tmp);

    debug_putc('0' + (uint8_t)c);
}

void debug_put_newline(void) {
    debug_putc('\n');
}

uint32_t debug_is_initialized(void) {
    return debug_init_done;
}
