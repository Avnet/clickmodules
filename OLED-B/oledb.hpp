/**
* copyright (c) 2018, James Flynn
* SPDX-License-Identifier: MIT
*/

/**
* @file   oledb_ssd1306.hpp
* @brief  Class for managing a MicroE OLEDB Click board
*         the class simply pulls the 'C' functions to create a single object.

* @author James Flynn
*
* @date   28-Feb-2019
*/

#ifndef __OLEDB_HPP__
#define __OLEDB_HPP__

#include <stdio.h>
#include <unistd.h>

#include "spi.hpp"
#include "oledb_ssd1306.h"
#include "Avnet_GFX.h"

extern "C" {
#include <hwlib/hwlib.h>
}

#define _delay(x)	(usleep(x*1000))

class OLEDB {
  private:
    OLEDB_SSD1306* _fd;
    static SPI*    _spi;
    static gpio_handle_t dcPin;
    static gpio_handle_t rstPin;

    static int spiwrite( uint16_t cmd, uint8_t *b, int siz );
    static void hw_init(void);
    static int hw_reset(void);

  public:

    OLEDB(void);
    ~OLEDB(void);
    
    OLEDB_SSD1306* OLED_handle(void) {return _fd; }
    void    clearDisplay(void) { oledb_clrDispBuff(_fd); }
    int     getPixel(OLEDB_SSD1306*p, int16_t x, int16_t y) { return oledb_getPixel(p, x, y); }
    void    display(OLEDB_SSD1306* p, int  reverse) { oledb_display(p, reverse); }
    void    invertDisplay(OLEDB_SSD1306*p, int  i) { oledb_invertDisplay(p,  i); }
    void    dim(OLEDB_SSD1306*p,  int dim) { oledb_dim(p,  dim); }
    void    startscrollright(OLEDB_SSD1306*p, uint8_t start, uint8_t stop)
            { oledb_startscrollright(p, start, stop); }
    void    startscrollleft(OLEDB_SSD1306*p, uint8_t start, uint8_t stop)
            { oledb_startscrollleft(p, start, stop); }
    void    startscrolldiagright(OLEDB_SSD1306*p, uint8_t start, uint8_t stop)
            { oledb_startscrolldiagright(p, start, stop); }
    void    startscrolldiagleft(OLEDB_SSD1306*p, uint8_t start, uint8_t stop)
            { oledb_startscrolldiagleft(p, start, stop); }
    void    stopscroll(OLEDB_SSD1306*p) { oledb_stopscroll(p); }

    void    writePixel(int16_t x, int16_t y, uint16_t color) { AvnetGFX_writePixel(x, y, color); }
    void    fillScreen(uint16_t color) { AvnetGFX_fillScreen(color); }
    void    drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) 
            { AvnetGFX_drawLine(x0, y0, x1, y1, color); }

    void    drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) 
            { AvnetGFX_drawRect(x, y, w, h, color); }
    void    fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) 
            { AvnetGFX_fillRect(x, y, w, h, color); }

    void    drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color) 
            { AvnetGFX_drawRoundRect(x, y, w, h, r, color); }
    void    fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color) 
            { AvnetGFX_fillRoundRect(x, y, w, h, r, color); }

    void    drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) { AvnetGFX_drawCircle(x0, y0, r, color); }
    void    fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) { AvnetGFX_fillCircle(x0, y0, r, color); }

    void    drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) 
            { AvnetGFX_drawTriangle(x0, y0, x1, y1, x2, y2, color); }
    void    fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) 
            { AvnetGFX_fillTriangle(x0, y0, x1, y1, x2, y2, color); }
    void    drawBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color) 
            { AvnetGFX_drawBitmap(x, y, bitmap, w, h, color); }

    size_t  printText(const char *fmt, ...) { 
        char buff[255];
        va_list ap;
        va_start(ap, fmt);
        vsprintf (buff,fmt, ap);
        for( size_t i=0; i<strlen(buff); i++ )
            AvnetGFX_write(buff[i]);
        va_end(ap);
        return strlen(buff);
        }

    void    setCursor(int16_t x, int16_t y) { return AvnetGFX_setCursor(x, y); }
    int16_t getCursorX(void) { return AvnetGFX_getCursorX(); } 
    int16_t getCursorY(void) { return AvnetGFX_getCursorY(); } 

    void    setTextSize(uint8_t s) { AvnetGFX_setTextSize(s); }
    void    setTextColor(uint16_t c, uint16_t b) { AvnetGFX_setTextColor(c, b); }
    void    setTextWrap(int w) { AvnetGFX_setTextWrap(w); }
    void    charSize(char c, int16_t *x, int16_t *y, int16_t *minx, int16_t *miny, int16_t *maxx, int16_t *maxy) 
            { AvnetGFX_charBounds(c, x, y, minx, miny, maxx, maxy); }
    void    getStrBounds(char *str, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h)  
            { AvnetGFX_getTextBounds(str, x, y, x1, y1, w, h); }

    int16_t display_width(void) { return AvnetGFX_width(); }
    int16_t display_height(void) { return AvnetGFX_height(); }

    void    setRotation(uint8_t x) { AvnetGFX_setRotation(x); }
    uint8_t getRotation(void) { return AvnetGFX_getRotation(); }
};

#endif // __OLEDB_HPP__

