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

// **********************************************************
// these lcd drawing functions are based on
// openglcd: https:// bitbucket.org/bperrybap/openglcd/src
// **********************************************************

#include "screen.h"
#include "font.h"
#include "delay.h"

uint8_t screen_buffer[SCREEN_BUFFER_SIZE];
static const uint8_t *screen_font_ptr;
static uint32_t screen_font_x;
static uint32_t screen_font_y;
static uint8_t  screen_font_color;

void screen_init(void) {
    screen_clear();
    led_backlight_on();
}

void screen_clear(void) {
    screen_fill(0);
    screen_update();
}

void screen_update(void) {
    lcd_send_data(screen_buffer);
}

void screen_test(void) {
    uint32_t x, y;
    while (1) {
        for (x = 0; x < 64; x++) {
            screen_clear();
            screen_puts_xy(0, x+0, 1, "0 ABC");
            screen_puts_xy(30, x+1, 1, "1 ABC");
            screen_puts_xy(60, x+2, 1, "2 ABC");
            screen_puts_xy(90, x+3, 1, "3 ABC");
            screen_update();

            delay_ms(100);
        }
    }
}
void screen_draw_line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color) {
    uint8_t deltax, deltay, x, y, steep;
    int8_t error, ystep;

    steep = _screen_absDiff(y1, y2) > _screen_absDiff(x1, x2);

    if (steep) {
        _screen_swap(x1, y1);
        _screen_swap(x2, y2);
    }

    if (x1 > x2) {
        _screen_swap(x1, x2);
        _screen_swap(y1, y2);
    }

    deltax = x2 - x1;
    deltay = _screen_absDiff(y2, y1);
    error = deltax / 2;
    y = y1;

    if (y1 < y2) {
        ystep = 1;
    } else {
        ystep = -1;
    }

    for (x = x1; x <= x2; x++) {
        if (steep) {
            screen_set_dot(y, x, color);
        } else {
            screen_set_dot(x, y, color);
        }
        error = error - deltay;

        if (error < 0) {
            y = y + ystep;
            error = error + deltax;
        }
    }
}


// set pixels from upper left edge x,y to lower right edge x1,y1 to the given color
// the width of the region is x1-x + 1, height is y1-y+1
void screen_set_pixels(uint8_t x, uint8_t y, uint8_t x2, uint8_t y2, uint8_t color) {
    uint8_t mask, pageOffset, h, i, data;
    uint8_t height = y2-y+1;
    uint8_t width = x2-x+1;
    uint16_t dpos = 0;

    pageOffset = y%8;
    y -= pageOffset;
    mask = 0xFF;
    if (height < 8-pageOffset) {
        mask >>= (8-height);
        h = height;
    } else {
        h = 8-pageOffset;
    }
    mask <<= pageOffset;

    dpos = (y/ 8)*128 + x;
    for (i = 0; i < width; i++) {
        if (color) {
            screen_buffer[dpos] |= mask;
        } else {
            screen_buffer[dpos] &= ~mask;
        }
        dpos++;
    }

    while (h + 8 <= height) {
        h += 8;
        y += 8;
        dpos = (y/ 8)*128 + x;
        for (i = 0; i < width; i++) {
            if (color) {
                screen_buffer[dpos] = 0xFF;
            } else {
                screen_buffer[dpos] = 0x00;
            }
            dpos++;
        }
    }

    if (h < height) {
        mask = ~(0xFF << (height-h));
        dpos = (y/ 8+1)*128 + x;
        for (i = 0; i < width; i++) {
            if (color) {
                screen_buffer[dpos] |= mask;
            } else {
                screen_buffer[dpos] &= ~mask;
            }
        dpos++;
        }
    }
}

void screen_draw_vline(uint8_t x, uint8_t y, uint8_t height, uint8_t color) {
    screen_set_pixels(x, y, x, y+height-1, color);
}

void screen_draw_hline(uint8_t x, uint8_t y, uint8_t width, uint8_t color) {
    screen_set_pixels(x, y, x+width-1, y, color);
}

void screen_draw_rect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color) {
    // top
    screen_draw_hline(x, y, width, color);
    // bottom
    screen_draw_hline(x, y+height-1, width, color);
    // left
    screen_draw_vline(x, y, height, color);
    // right
    screen_draw_vline(x+width-1, y, height, color);   // right
}

void screen_draw_round_rect(uint8_t x, uint8_t y, uint8_t width, uint8_t height,
                            uint8_t radius, uint8_t color) {
    int16_t tSwitch;
    uint8_t x1 = 0, y1 = radius;
    tSwitch = 3 - 2 * radius;

    while (x1 <= y1) {
        // upper left corner
        screen_set_dot(x+radius - x1, y+radius - y1, color);  // upper half
        screen_set_dot(x+radius - y1, y+radius - x1, color);  // lower half

        // upper right corner
        screen_set_dot(x+width-radius-1 + x1, y+radius - y1, color);  // upper half
        screen_set_dot(x+width-radius-1 + y1, y+radius - x1, color);  // lower half

        // lower right corner
        screen_set_dot(x+width-radius-1 + x1, y+height-radius-1 + y1, color);  // lower half
        screen_set_dot(x+width-radius-1 + y1, y+height-radius-1 + x1, color);  // upper half

        // lower left corner
        screen_set_dot(x+radius - x1, y+height-radius-1 + y1, color);  // lower half
        screen_set_dot(x+radius - y1, y+height-radius-1 + x1, color);  // upper half

        if (tSwitch < 0) {
            tSwitch += (4 * x1 + 6);
        } else {
            tSwitch += (4 * (x1 - y1) + 10);
            y1--;
        }
        x1++;
    }

    screen_draw_hline(x+radius, y, width-(2*radius), color);      // top
    screen_draw_hline(x+radius, y+height-1, width-(2*radius), color);  // bottom
    screen_draw_vline(x, y+radius, height-(2*radius), color);     // left
    screen_draw_vline(x+width-1, y+radius, height-(2*radius), color);  // right
}

void screen_fill_rect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color) {
    screen_set_pixels(x, y, x+width-1, y+height-1, color);
}

void screen_fill_round_rect(uint8_t x, uint8_t y, uint8_t width,
                            uint8_t height, uint8_t radius, uint8_t color) {
    int16_t tSwitch;
    uint8_t x1 = 0, y1 = radius;
    tSwitch = 3 - 2 * radius;

    // center block
    // filling center block first makes it apear to fill faster
    screen_fill_rect(x+radius, y, width-2*radius, height, color);

    while (x1 <= y1) {
        // left side
        screen_draw_line(
            x+radius - x1, y+radius - y1,           // upper left corner upper half
            x+radius - x1, y+height-radius-1 + y1,  // lower left corner lower half
            color);
        screen_draw_line(
            x+radius - y1, y+radius - x1,           // upper left corner lower half
            x+radius - y1, y+height-radius-1 + x1,  // lower left corner upper half
            color);

        // right side
        screen_draw_line(
            x+width-radius-1 + x1, y+radius - y1,           // upper right corner upper half
            x+width-radius-1 + x1, y+height-radius-1 + y1,  // lower right corner lower half
            color);
        screen_draw_line(
            x+width-radius-1 + y1, y+radius - x1,           // upper right corner lower half
            x+width-radius-1 + y1, y+height-radius-1 + x1,  // lower right corner upper half
            color);

        if (tSwitch < 0) {
            tSwitch += (4 * x1 + 6);
        } else {
            tSwitch += (4 * (x1 - y1) + 10);
            y1--;
        }
        x1++;
    }
}

uint8_t screen_put_char(uint8_t c) {
    uint8_t thielefont = 0;
    uint8_t width      = 0;
    uint8_t height     = screen_font_ptr[FONT_HEIGHT];
    uint8_t bytes      = (height+7)/ 8;  // calculates height in rounded up bytes
    uint8_t firstChar  = screen_font_ptr[FONT_FIRST_CHAR];
    uint8_t charCount  = screen_font_ptr[FONT_CHAR_COUNT];
    uint32_t index     = 0;
    uint32_t i;

    if (c < firstChar || c >= (firstChar+charCount)) {
        return 0;  // invalid char
    }
    c-= firstChar;


    if (font_is_fixed_width(screen_font_ptr)) {
        thielefont = 0;
        width = screen_font_ptr[FONT_FIXED_WIDTH];
        index = c*bytes*width+FONT_WIDTH_TABLE;
    } else {
        // variable width font, read width data, to get the index
        thielefont = 1;
        /*
        * Because there is no table for the offset of where the data
        * for each character glyph starts, run the table and add up all the
        * widths of all the characters prior to the character we
        * need to locate.
        */
        for (i = 0; i < c; i++) {
            index += screen_font_ptr[FONT_WIDTH_TABLE+i];
        }

        /*
        * Calculate the offset of where the font data
        * for our character starts.
        * The index value from above has to be adjusted because
        * there is potentialy more than 1 byte per column in the glyph,
        * when the characgter is taller than 8 bits.
        * To account for this, index has to be multiplied
        * by the height in bytes because there is one byte of font
        * data for each vertical 8 pixels.
        * The index is then adjusted to skip over the font width data
        * and the font header information.
        */
        index = index*bytes+charCount+FONT_WIDTH_TABLE;

        /*
        * Finally, fetch the width of our character
        */
        width = screen_font_ptr[FONT_WIDTH_TABLE+c];
    }


    if (!width) {
        // no glyph definition for character?
        return(0);
    }

    // last but not least, draw the character

    /*
    * Paint font data bits and write them to LCD memory 1 LCD page at a time.
    * This is very different from simply reading 1 byte of font data
    * and writing all 8 bits to LCD memory and expecting the write data routine
    * to fragement the 8 bits across LCD 2 memory pages when necessary.
    * That method(really doesn't work) and reads and writes the same LCD page
    * more than once as well as not do sequential writes to memory.
    *
    * This method of rendering while much more complicated, somewhat scrambles the font
    * data reads to ensure that all writes to LCD pages are always sequential and a given LCD
    * memory page is never read or written more than once.
    * And reads of LCD pages are only done at the top or bottom of the font data rendering
    * when necessary.
    * i.e it ensures the absolute minimum number of LCD page accesses
    * as well as does the sequential writes as much as possible.
    *
    */

    uint32_t pixels = height;
    uint32_t p;
    uint32_t dy;
    uint32_t tfp;
    uint32_t dp;
    uint32_t dbyte;
    uint8_t fdata;
    uint32_t j;

    if (!font_is_nopad_fixed_font(screen_font_ptr)) {
        pixels++;  // extra pixel on bottom for spacing on all fonts but NoPadFixed fonts
    }


    for (p = 0; p < pixels;) {
        dy = screen_font_y + p;

        // align to proper Column and page in LCD memory
        uint32_t screen_dpos = ((dy & ~7)/ 8)*128 + screen_font_x;

        uint32_t page = p/8 * width;  // page must be 16 bit to prevent overflow

        // each column of font data
        for (j = 0; j < width; j++) {
            /*
            * Fetch proper byte of font data.
            * Note:
            * This code "cheats" to add the horizontal space/ pixel row
            * below the font.
            * It essentially creates a font pixel of 0/ off when the pixels are
            * out of the defined pixel map.
            *
            * fake a fondata read read when we are on the very last
            * bottom "pixel". This lets the loop logic continue to run
            * with the extra fake pixel. If the loop is not the
            * the last pixel the pixel will come from the actual
            * font data, but that is ok as it is 0 padded.
            *
            */

            if (p >= height) {
                /*
                * fake a font data read for padding below character.
                */
                fdata = 0;
            } else {
                fdata = screen_font_ptr[index + page + j];
                /*
                * Have to shift font data because Thiele shifted residual
                * font bits the wrong direction for LCD memory.
                *
                * The real solution to this is to fix the variable width font format to
                * not shift the residual bits the wrong direction!!!!
                */
                if (thielefont && (height - (p&~7)) < 8) {
                    fdata >>= 8 - (height & 7);
                }
            }

            if (!screen_font_color) {
                fdata ^= 0xff;  /* inverted data for "white" font color */
            }

            /*
            * Check to see if a quick full byte write of font
            * data can be done.
            */
            if (!(dy & 7) && !(p & 7) && ((pixels -p) >= 8)) {
                /*
                * destination pixel is on a page boundary
                * Font data is on byte boundary
                * And there are 8 or more pixels left
                * to paint so a full byte write can be done.
                */
                screen_buffer_write(screen_dpos, fdata);
                screen_dpos++;
                continue;
            } else {
                /*
                * No, so must fetch byte from LCD memory.
                */
                dbyte = screen_buffer_read(screen_dpos);
            }

            /*
            * At this point there is either not a full page of data
            * left to be painted  or the font data spans multiple font
            * data bytes. (or both) So, the font data bits will be painted
            * into a byte and then written to the LCD memory.page.
            */


            tfp = p;    /* font pixel bit position    */
            dp = dy & 7;  /* data byte pixel bit position */

            /*
            * paint bits until we hit bottom of page/ byte
            * or run out of pixels to paint.
            */
            while ((dp <= 7) && (tfp) < pixels) {
                if (fdata & (1 << (tfp & 7))) {
                    dbyte |= (1 << dp);
                } else {
                    dbyte &= ~(1 << dp);
                }

                /*
                * Check for crossing font data bytes
                */
                if ((tfp & 7)== 7) {
                    fdata = screen_font_ptr[index + page + j + width];

                    /*
                    * Have to shift font data because Thiele shifted residual
                    * font bits the wrong direction for LCD memory.
                    *
                    * Note: the 8+1 is there vs 8 because we are fetching the font
                    * data byte for the next pixel, but tfp has not yet incremented yet
                    * so it is one less then it should be. We add 1 to the 8
                    * to account for this.
                    */

                    if ((thielefont) && ((height - tfp) < (8+1))) {
                        fdata >>= (8 - (height & 7));
                    }

                    if (!screen_font_color) {
                        fdata ^= 0xff;  /* inverted data for "white" color  */
                    }
                }
                tfp++;
                dp++;
            }
            /*
            * Now flush out the painted byte.
            */
            screen_buffer_write(screen_dpos, dbyte);
            screen_dpos++;
        }

        /*
        * now create a horizontal gap(vertical line of pixels) between characters.
        * Since this gap is "white space", the pixels painted are oposite of the
        * font color.
        *
        * Since full LCD pages are being written, there are 4 combinations of filling
        * in the this gap page.
        *  - pixels start at bit 0 and go down less than 8 bits
        *  - pixels don't start at 0 but go down through bit 7
        *  - pixels don't start at 0 and don't go down through bit 7(fonts shorter than 6 hi)
        *  - pixels start at bit 0 and go down through bit 7(full byte)
        *
        * The code below creates a mask of the bits that should not be painted.
        *
        * Then it is easy to paint the desired bits since if the color is WHITE,
        * the paint bits are set, and if the coloer is not WHITE the paint bits are stripped.
        * and the paint bits are the inverse of the desired bits mask.
        */


        if (!font_is_nopad_fixed_font(screen_font_ptr)) {
            // extra pixel on right for spacing on all fonts but NoPadFixed fonts
            if ((dy & 7) || (pixels - p < 8)) {
                uint8_t mask = 0;
                dbyte =  screen_buffer_read(screen_dpos);

                if (dy & 7) {
                    mask |= (1 << (dy & 7)) -1;
                }

                if ((pixels-p) < 8) {
                    mask |= ~((1 << (pixels - p)) -1);
                }

                if (!screen_font_color) {
                    dbyte |= ~mask;
                } else {
                    dbyte &= mask;
                }
            } else {
                if (!screen_font_color) {
                    dbyte = 0xff;
                } else {
                    dbyte = 0;
                }
            }

            // does not work with 3x5 font?!
            if (width != 3) screen_buffer_write (screen_dpos, dbyte);
        }
        /*
        * advance the font pixel for the pixels
        * just painted.
        */
        p += 8 - (dy & 7);
    }

    /*
    * Since this rendering code always starts off with a GotoXY() it really isn't necessary
    * to do a real GotoXY() to set the h/ w location after rendering a character.
    * We can get away with only setting the s/ w version of X & Y.
    *
    * Since y didn't change while rendering, it is still correct.
    * But update x for the pixels rendered.
    *
    */
    screen_font_x += width;  // pixels rendered in character glyph

    if (!font_is_nopad_fixed_font(screen_font_ptr)) {
        screen_font_x++;  // skip over pad pixel we rendered
    }

    return 1;  // valid char
}

void screen_puts_xy(uint8_t x, uint8_t y, uint8_t color, uint8_t *str) {
    screen_font_x = x;
    screen_font_y = y;
    screen_font_color = color;

    while (*str) {
        screen_put_char(*str);
        str++;
    }
}

uint32_t screen_strlen(uint8_t *str) {
    uint32_t len = 0;
    while (*str++) {
        len++;
        if (len > 50) {
            // just in case... we do not handle longer strings
            break;
        }
    }
    return (screen_font_ptr[FONT_FIXED_WIDTH] + 1) * len;
}

void screen_puts_xy_centered(uint8_t x, uint8_t y, uint8_t color, uint8_t *str) {
    uint32_t font_w = screen_font_ptr[FONT_FIXED_WIDTH];
    uint32_t font_h = screen_font_ptr[FONT_HEIGHT];

    uint32_t len = screen_strlen(str);
    uint32_t sx = x - (len) / 2;
    uint32_t sy = y - font_h / 2;
    screen_puts_xy(sx, sy, color, str);
}

void screen_puts_centered(uint8_t y, uint8_t color, uint8_t *str) {
    screen_puts_xy_centered(LCD_WIDTH/2, y, color, str);
}



// output a signed 8-bit number
void screen_put_int8(uint8_t x, uint8_t y, uint8_t color, int8_t c) {
    screen_font_x = x;
    screen_font_y = y;
    screen_font_color = color;

    uint8_t tmp;
    uint8_t mul;
    uint8_t l;
    uint8_t uint_s;

    if (c < 0) {
        screen_put_char('-');
        uint_s = -c;
    } else {
        screen_put_char(' ');
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
            screen_put_char(' ');
        } else {
            screen_put_char(tmp);
        }
    }
}

void screen_put_time(uint8_t x, uint8_t y, uint8_t c, int16_t time) {
    // print time -00:00 on screen
    uint32_t color = c;
    uint32_t negative = 0;

    if (time < 0) {
        time = -time;
        // render '-' char
        screen_fill_rect(x, y + screen_font_ptr[FONT_HEIGHT]/2 - 1,
                         screen_font_ptr[FONT_FIXED_WIDTH]/2, 3, color);
    }

    int16_t minutes = time / 60;
    int16_t seconds = time % 60;

    // put minutes
    x = x + (screen_font_ptr[FONT_FIXED_WIDTH]/2 + 2);
    screen_put_uint8_2dec(x, y, color, minutes);
    x = x + (screen_font_ptr[FONT_FIXED_WIDTH] + 1) * 2;

    // render colon
    screen_fill_rect(x, y + screen_font_ptr[FONT_HEIGHT]*3/8, 2, 2, color);
    screen_fill_rect(x, y + screen_font_ptr[FONT_HEIGHT]*5/8, 2, 2, color);

    // put seconds
    x = x + 3;
    screen_put_uint8_2dec(x, y, color, seconds);
}


void screen_put_fixed2_1digit(uint8_t x, uint8_t y, uint8_t color, uint32_t v) {
    int16_t full = v / 100;
    int16_t frac = (v % 100)/10;  // keep only one digit

    // put v
    screen_put_uint8_2dec(x, y, color, full);
    x = x + (screen_font_ptr[FONT_FIXED_WIDTH] + 1) * 2;

    // render point
    screen_fill_rect(x, y + screen_font_ptr[FONT_HEIGHT]*7/8, 2, 2, color);

    // put frac
    x = x + 3;
    screen_put_uint8_1dec(x, y, color, frac);
}


// output a unsigned 8-bit, only two decimals
void screen_put_uint8_2dec(uint8_t x, uint8_t y, uint8_t color, uint8_t c) {
    screen_font_x = x;
    screen_font_y = y;
    screen_font_color = color;

    uint8_t tmp;
    uint8_t mul;
    uint8_t l;

    // this should not happen
    if (c >= 100) {
        return;
    }

    l = 0;
    for (mul = 10; mul > 0; mul = mul/ 10) {
        tmp = '0';
        while (c >= mul) {
            c -= mul;
            tmp++;
            l = 1;
        }
        screen_put_char(tmp);
    }
}

void screen_put_uint8_1dec(uint8_t x, uint8_t y, uint8_t color, uint8_t c) {
    screen_font_x = x;
    screen_font_y = y;
    screen_font_color = color;

    // this should not happen
    if (c >= 10) {
        return;
    }

    screen_put_char('0' + c);
}

// output a unsigned 8-bit
void screen_put_uint8(uint8_t x, uint8_t y, uint8_t color, uint8_t c) {
    screen_font_x = x;
    screen_font_y = y;
    screen_font_color = color;

    uint8_t tmp;
    uint8_t mul;
    uint8_t l;

    l = 0;
    for (mul = 100; mul > 0; mul = mul/ 10) {
        tmp = '0';
        while (c >= mul) {
            c -= mul;
            tmp++;
            l = 1;
        }
        if ((l == 0) && (tmp == '0') && (mul != 1)) {
            screen_put_char(' ');
        } else {
            screen_put_char(tmp);
        }
    }
}

// output an unsigned 14-bit number
void screen_put_uint14(uint8_t x, uint8_t y, uint8_t color, uint16_t c) {
    screen_font_x = x;
    screen_font_y = y;
    screen_font_color = color;

    uint8_t tmp;
    uint8_t l = 0;
    tmp = 0;
    while (c >= 10000L) {
        c -= 10000L;
        tmp++;
        l = 1;
    }
    // if (tmp != 0) screen_put_char('0' + tmp);

    tmp = 0;
    while (c >= 1000L) {
        c -= 1000L;
        tmp++;
        l = 1;
    }
    if (l || (tmp != 0)) {
        screen_put_char('0' + tmp);
    } else {
        screen_put_char(' ');
    }

    tmp = 0;
    while (c >= 100) {
        c -= 100;
        tmp++;
        l = 1;
    }
    if (l || (tmp != 0)) {
        screen_put_char('0' + tmp);
    } else {
        screen_put_char(' ');
    }

    tmp = 0;
    while (c >= 10) {
        c -= 10;
        tmp++;
        l = 1;
    }
    if (l || (tmp != 0)) {
        screen_put_char('0' + tmp);
    } else {
        screen_put_char(' ');
    }

    screen_put_char('0' + (uint8_t)c);
}

// put hexadecimal number to debug out.
void screen_put_hex8(uint8_t x, uint8_t y, uint8_t color, uint8_t val) {
    screen_font_x = x;
    screen_font_y = y;
    screen_font_color = color;

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
    screen_put_char(hi);
    screen_put_char(lo);
}

// put 16bit hexadecimal number to debug out
void screen_put_hex16(uint8_t x, uint8_t y, uint8_t color, uint16_t val) {
    screen_put_hex8(x, y, color, val>>8);
    screen_put_hex8(x, y, color, val & 0xFF);
}

void screen_put_fixed2(uint8_t x, uint8_t y, uint8_t color, uint16_t c) {
    uint8_t tmp;
    uint8_t l = 0;
    tmp = 0;
    screen_font_x = x;
    screen_font_y = y;
    screen_font_color = color;

    while (c >= 10000L) {
        c -= 10000L;
        tmp++;
        l = 1;
    }
    if (tmp != 0) screen_put_char('0' + tmp);

    tmp = 0;
    while (c >= 1000L) {
        c -= 1000L;
        tmp++;
        l = 1;
    }
    if (l || (tmp != 0)) screen_put_char('0' + tmp);

    tmp = 0;
    while (c >= 100) {
        c -= 100;
        tmp++;
        l = 1;
    }
    if (l || (tmp != 0)) screen_put_char('0' + tmp);

    screen_put_char('.');

    tmp = 0;
    while (c >= 10) {
        c -= 10;
        tmp++;
        l = 1;
    }
    if (l || (tmp != 0)) screen_put_char('0' + tmp);

    screen_put_char('0' + (uint8_t)c);
}

void screen_set_font(const uint8_t *font) {
    screen_font_ptr = font;
}

void screen_fill(uint8_t color) {
    uint32_t i;
    // this is optimized for runtime, do not move the if into the for loop!
    if (color) {
        for (i = 0; i < SCREEN_BUFFER_SIZE; i++) {
            screen_buffer[i] = 0xFF;
        }
    } else {
        for (i = 0; i < SCREEN_BUFFER_SIZE; i++) {
            screen_buffer[i] = 0;
        }
    }
}
