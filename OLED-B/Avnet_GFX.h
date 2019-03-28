
#ifndef _AVNET_GFX_H
#define _AVNET_GFX_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "oledb_ssd1306.h"

#ifndef _swap
#define _swap(a, b)  (((a) ^= (b)), ((b) ^= (a)), ((a) ^= (b))) 
#endif

#define PIXOFF     0
#define PIXON      1
#define PIXINVERSE 2

#ifdef __cplusplus
extern "C" {
#endif

void    AvnetGFX_init(OLEDB_SSD1306* ptr, int16_t w, int16_t h);
void    AvnetGFX_writePixel(int16_t x, int16_t y, uint16_t color) ;
void    AvnetGFX_fillScreen(uint16_t color) ;
void    AvnetGFX_writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) ;
void    AvnetGFX_writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) ;
void    AvnetGFX_writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) ;
void    AvnetGFX_drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) ;

void    AvnetGFX_drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) ;
void    AvnetGFX_drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color) ;
void    AvnetGFX_fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color) ;
void    AvnetGFX_fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) ;

void    AvnetGFX_drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) ;
void    AvnetGFX_drawCircleHelper( int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color) ;
void    AvnetGFX_fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) ;
void    AvnetGFX_fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t corners, int16_t delta, uint16_t color) ;

void    AvnetGFX_drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) ;
void    AvnetGFX_fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) ;

void    AvnetGFX_drawBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color) ;

void    AvnetGFX_drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size) ;
size_t  AvnetGFX_write(uint8_t c) ;
size_t  AvnetGFX_printText(const char *fmt, ...);

void    AvnetGFX_setCursor(int16_t x, int16_t y) ;
int16_t AvnetGFX_getCursorX(void) ;
int16_t AvnetGFX_getCursorY(void) ;

void    AvnetGFX_setTextSize(uint8_t s) ;
void    AvnetGFX_setTextColor(uint16_t c, uint16_t b) ;
void    AvnetGFX_setTextWrap(int w) ;
void    AvnetGFX_charBounds(char c, int16_t *x, int16_t *y, int16_t *minx, int16_t *miny, int16_t *maxx, int16_t *maxy) ;
void    AvnetGFX_getTextBounds(char *str, int16_t x, int16_t y, int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h) ;

int16_t AvnetGFX_width(void) ;
int16_t AvnetGFX_height(void) ;

void    AvnetGFX_setRotation(uint8_t x) ;
uint8_t AvnetGFX_getRotation(void) ;

#ifdef __cplusplus
}
#endif

#endif // _AVNET_GFX_H

