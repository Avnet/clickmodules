
#include <Avnet_GFX.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include "oledb_ssd1306.h"

// SOME DEFINES AND STATIC VARIABLES USED INTERNALLY -----------------------

#ifndef _swap
#define _swap(a, b)  (((a) ^= (b)), ((b) ^= (a)), ((a) ^= (b))) 
#endif

static OLEDB_SSD1306* oled_dev;

// Open and Close functions ------------------------------------------------

OLEDB_SSD1306* open_oled( void (*init)(void), int (*rst_func)(void), int (*wr_func)( uint16_t cmd, uint8_t *b, int siz ) )
{
    oled_dev = (OLEDB_SSD1306*) malloc(sizeof(OLEDB_SSD1306));
    if( oled_dev == NULL )
        return NULL;
    oled_dev->buffer = (uint8_t *)malloc(SSD1306_LCDWIDTH * ((SSD1306_LCDHEIGHT + 7) / 8));
    if( oled_dev->buffer == NULL )
        return NULL;

    oled_dev->rst = rst_func;    //this function performs a reset of the SSD1306
    oled_dev->wr  = wr_func;     //this functioin writes a buffer of data out 
    init();
    oledb_init(oled_dev,1);
    AvnetGFX_init(oled_dev, SSD1306_LCDWIDTH, SSD1306_LCDHEIGHT);
    return oled_dev;
}

void close_oled( OLEDB_SSD1306* ptr )
{
    free(oled_dev->buffer);
    free(oled_dev);
}


static const uint8_t init_seq[] = {             // Init sequence
    SSD1306_DISPLAYOFF,                   // 0xAE
    SSD1306_SETDISPLAYCLOCKDIV,           // 0xD5
    0x80,                                 // the suggested ratio 0x80
    SSD1306_SETMULTIPLEX,                 // 0xA8
    (SSD1306_LCDHEIGHT - 1),
    SSD1306_SETDISPLAYOFFSET,             // 0xD3
    0x0,                                  // no offset
    (SSD1306_SETSTARTLINE | 0x0),         // 0x40 | line #0 
    SSD1306_CHARGEPUMP,                   // 0x8D
    0x14,

    SSD1306_MEMORYMODE,                   // 0x20
    0x00,                                 // use Horizontal addressing mode
    SSD1306_COMSCANDEC,                   // 0xC8
    SSD1306_SETCOMPINS,                   // 0xDA
    0x12,                                 // ada x12
    SSD1306_SETCONTRAST,                  // 0x81
    0xAF,
    SSD1306_SETPRECHARGE,                 // 0xd9
    0x25,
    SSD1306_SETVCOMDETECT,                // 0xDB
    0x20,
    SSD1306_DISPLAYALLON_RESUME,          // 0xA4
    SSD1306_NORMALDISPLAY,                // 0xA6
    SSD1306_DISPLAYON                     // Main screen turn on
    };

int oledb_init(OLEDB_SSD1306* ptr, int reset )
{
    int x;
    uint8_t clearb = 0x00;
    if( reset ) ptr->rst();
    x = ptr->wr(  SSD1306_COMMAND,  (uint8_t *)init_seq, sizeof(init_seq) );

    for( int i=0; i< (128 * ((96+7)/8)); i++) //clear all display RAM in the SSD1306   
        ptr->wr(SSD1306_DATA, &clearb, 1);
    oledb_clrDispBuff(ptr);
    oledb_display(ptr,FALSE);

    return x;
}

// LCD COMMAND FUNCTIONS  --------------------------------------------------

// -------------------------------------------------------------------------
static const uint8_t disprev[] = {
        (SSD1306_SEGREMAP | 1),    // 0 - 0xA1 reverse line scan   
        SSD1306_PAGEADDR,          // 1 - 0x22
        0,                         // 2 - Page start address
        (0xFF),                    // 3 - Page end (not really, but this works)
        SSD1306_COLUMNADDR,        // 4 - 0x21
        (127-(SSD1306_LCDWIDTH-1)),// 5 - Column start address if reversed
        (127)                      // 6 - Column end address
        };

static const uint8_t dispnorm[] = {
        (SSD1306_SEGREMAP),        // 0 - 0xA0 normal line scan   
        SSD1306_PAGEADDR,          // 1 - 0x22
        0,                         // 2 - Page start address
        (0xFF),                    // 3 - Page end (not really, but this works)
        SSD1306_COLUMNADDR,        // 4 - 0x21
        0,                         // 5 - Column start address
        (SSD1306_LCDWIDTH-1)       // 6 - Column end address
        };

void oledb_display(OLEDB_SSD1306* ptr, int reverse_linescan) 
{
    if( reverse_linescan ) 
        ptr->wr(  SSD1306_COMMAND,  (uint8_t*)disprev, sizeof(disprev) );
    else
        ptr->wr(  SSD1306_COMMAND,  (uint8_t*)dispnorm, sizeof(dispnorm) );

    uint16_t count = SSD1306_LCDWIDTH * ((SSD1306_LCDHEIGHT + 7) / 8);
    ptr->wr( SSD1306_DATA,  ptr->buffer, count );
}


// -------------------------------------------------------------------------
static uint8_t scrollright[] = {
    SSD1306_RIGHT_HORIZONTAL_SCROLL,  //0x26
    0X00,                             
    0x00,                             //2 - start - start
    0X00,                             
    0x00,                             //4 - stop
    0X00,
    0XFF,
    SSD1306_ACTIVATE_SCROLL 
    };

void oledb_startscrollright(OLEDB_SSD1306* ptr, uint8_t start, uint8_t stop) 
{
    scrollright[2] = start;
    scrollright[4] = stop;
    ptr->wr( SSD1306_COMMAND,  scrollright, sizeof(scrollright) );
}

// -------------------------------------------------------------------------
static uint8_t scrollleft[] = {
    SSD1306_LEFT_HORIZONTAL_SCROLL, //0x27
    0X00,
    0x00,                           //2 - start
    0X00,
    0x00,                           //4 - stop
    0X00,
    0XFF,
    SSD1306_ACTIVATE_SCROLL 
    };

void oledb_startscrollleft(OLEDB_SSD1306* ptr, uint8_t start, uint8_t stop) 
{
    scrollleft[2] = start;
    scrollleft[4] = stop;
    ptr->wr( SSD1306_COMMAND,  scrollleft, sizeof(scrollleft) );
}

// -------------------------------------------------------------------------
static uint8_t scrolldiagright[] = {
    SSD1306_SET_VERTICAL_SCROLL_AREA,              //0xA3
    0X00,
    SSD1306_LCDHEIGHT,
    SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL,  //0x29
    0X00,
    0x00,                                          //5 - start
    0X00,
    0x00,                                          //7 - stop
    0X01,
    SSD1306_ACTIVATE_SCROLL 
    };

void oledb_startscrolldiagright(OLEDB_SSD1306* ptr, uint8_t start, uint8_t stop) 
{
    scrolldiagright[5] = start;
    scrolldiagright[7] = stop;
    ptr->wr( SSD1306_COMMAND,  scrolldiagright, sizeof(scrolldiagright) );
}

// -------------------------------------------------------------------------
static uint8_t scrolldiagleft[] = {
    SSD1306_SET_VERTICAL_SCROLL_AREA,             //0xA3
    0X00,
    SSD1306_LCDHEIGHT,
    SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL,  //0x2A
    0X00,
    0x00,                                         //5 - start
    0X00,
    0x00,                                         //7 - stop
    0X01,
    SSD1306_ACTIVATE_SCROLL 
    };

void oledb_startscrolldiagleft(OLEDB_SSD1306* ptr, uint8_t start, uint8_t stop) 
{
    scrolldiagleft[5] = start;
    scrolldiagleft[7] = stop;
    ptr->wr( SSD1306_COMMAND,  scrolldiagleft, sizeof(scrolldiagleft) );
}

// -------------------------------------------------------------------------
void oledb_stopscroll(OLEDB_SSD1306* ptr) 
{
    uint8_t cmd = SSD1306_DEACTIVATE_SCROLL;
    ptr->wr( SSD1306_COMMAND, &cmd, 1);
}

void oledb_invertDisplay(OLEDB_SSD1306* ptr, int i) {
    uint8_t cmd = i ? SSD1306_INVERTDISPLAY : SSD1306_NORMALDISPLAY;
    ptr->wr( SSD1306_COMMAND, &cmd, 1);
}

void oledb_dim(OLEDB_SSD1306* ptr, int dim) 
{
    uint8_t cmd = (dim)? 0:0xCF;
    uint8_t cntrast = SSD1306_SETCONTRAST;

    ptr->wr( SSD1306_COMMAND, &cntrast, 1 );
    ptr->wr( SSD1306_COMMAND, &cmd, 1 );
}


// DRAWING FUNCTIONS -------------------------------------------------------

void oledb_clrDispBuff(OLEDB_SSD1306* ptr) 
{
  memset(ptr->buffer, 0, SSD1306_LCDWIDTH * ((SSD1306_LCDHEIGHT + 7) / 8));
}


void oledb_drawPixel(OLEDB_SSD1306* ptr, int16_t x, int16_t y, uint16_t color) 
{
    if((x >= 0) && (x < AvnetGFX_width()) && (y >= 0) && (y < AvnetGFX_height())) { // Rotate coordinates if needed.
        switch(AvnetGFX_getRotation()) {
           case 1:
              _swap(x, y);
              x = SSD1306_LCDWIDTH - x - 1;
              break;
           case 2:
              x = SSD1306_LCDWIDTH  - x - 1;
              y = SSD1306_LCDHEIGHT - y - 1;
              break;
           case 3:
              _swap(x, y);
              y = SSD1306_LCDHEIGHT - y - 1;
              break;
           }
        uint8_t *byte = &ptr->buffer[x + (y/8)*SSD1306_LCDWIDTH]; // get the buffer byte needed
        switch(color) {
           case WHITE:   *byte  |=  (1 << (y&7)); break;     //set the bit within the byte
           case BLACK:   *byte  &= ~(1 << (y&7)); break;     //clr the bit within the byte
           case INVERSE: *byte  ^=  (1 << (y&7)); break;     //invert the bit within the byte
           }
        }
}

void oledb_drawFastHLine( OLEDB_SSD1306* ptr, int16_t x, int16_t y, int16_t w, uint16_t color) 
{
    int bSwap = FALSE;
    switch(AvnetGFX_getRotation()) {
        case 1: // 90 degree rotation, swap x & y for rotation, then invert x
            bSwap = TRUE;
            _swap(x, y);
            x = SSD1306_LCDWIDTH - x - 1;
            break;
        case 2: // 180 degree rotation, invert x and y, then shift y around for height.
            x  = SSD1306_LCDWIDTH  - x - 1;
            y  = SSD1306_LCDHEIGHT - y - 1;
            x -= (w-1);
            break;
        case 3: // 270 degree rotation, swap x & y for rotation, then invert y and adjust for w (not to become h)
            bSwap = TRUE;
            _swap(x, y);
            y  = SSD1306_LCDHEIGHT - y - 1;
            y -= (w-1);
            break;
    }

    if(bSwap) 
        oledb_drawFastVLineInternal(ptr, x, y, w, color);
    else      
        oledb_drawFastHLineInternal(ptr, x, y, w, color);
}

void oledb_drawFastHLineInternal(OLEDB_SSD1306* ptr,int16_t x, int16_t y, int16_t w, uint16_t color) 
{
    if((y >= 0) && (y < SSD1306_LCDHEIGHT)) { // Y coord in bounds?
        if(x < 0) { // Clip left
            w += x;
            x  = 0;
            }

        if((x + w) > SSD1306_LCDWIDTH) { // Clip right
            w = (SSD1306_LCDWIDTH - x);
            }

        if(w > 0) { // Proceed only if width is positive
            uint8_t *pBuf = &ptr->buffer[(y / 8) * SSD1306_LCDWIDTH + x];
            uint8_t mask = 1 << (y & 7);
            switch(color) {
                case INVERSE:  while(w--) { *pBuf++ ^= mask; }; break;
                case WHITE:    while(w--) { *pBuf++ |= mask; }; break;
                case BLACK:    mask = ~mask; while(w--) { *pBuf++ &= mask; }; break;
                }
            }
        }
}

void oledb_drawFastVLine(OLEDB_SSD1306* ptr,int16_t x, int16_t y, int16_t h, uint16_t color) 
{
    int bSwap = FALSE;
    switch(AvnetGFX_getRotation()) {
        case 1: // for 90 degree rotation, swap x & y, then invert x and adjust for h (to become w)
            bSwap = TRUE;
            _swap(x, y);
            x  = SSD1306_LCDWIDTH - x - 1;
            x -= (h-1);
            break;
        case 2: // for 180 degree rotation, invert x and y, then shift y around for height.
            x = SSD1306_LCDWIDTH  - x - 1;
            y = SSD1306_LCDHEIGHT - y - 1;
            y -= (h-1);
            break;
        case 3: // for 270 degree rotation, swap x & y for rotation, then invert y
            bSwap = TRUE;
            _swap(x, y);
            y = SSD1306_LCDHEIGHT - y - 1;
            break;
        }

    if(bSwap) 
        oledb_drawFastHLineInternal(ptr, x, y, h, color);
    else      
        oledb_drawFastVLineInternal(ptr, x, y, h, color);
}

void oledb_drawFastVLineInternal(OLEDB_SSD1306* ptr,int16_t x, int16_t __y, int16_t __h, uint16_t color) 
{
    if((x >= 0) && (x < SSD1306_LCDWIDTH)) { // X coord in bounds?
        if(__y < 0) { // Clip top
            __h += __y;
            __y = 0;
            }
        if((__y + __h) > SSD1306_LCDHEIGHT) { // Clip bottom
            __h = (SSD1306_LCDHEIGHT - __y);
            }
        if(__h > 0) { // Proceed only if height is now positive
            uint8_t  y = __y, h = __h;
            uint8_t *pBuf = &ptr->buffer[(y / 8) * SSD1306_LCDWIDTH + x];

            // do the first partial byte, if necessary - this requires some masking
            uint8_t mod = (y & 7);
            if(mod) { // mask off the high n bits we want to set
                mod = 8 - mod;
                uint8_t mask = ~(0xFF >> mod);

                switch(color) {
                    case WHITE:   *pBuf |=  mask; break;
                    case BLACK:   *pBuf &= ~mask; break;
                    case INVERSE: *pBuf ^=  mask; break;
                    }
                pBuf += SSD1306_LCDWIDTH;
                }

            if(h >= mod) { // More to go?
                h -= mod;
                // Write solid bytes while we can - effectively 8 rows at a time
                if(h >= 8) {
                    if(color == INVERSE) {
                        // separate copy of the code so we don't impact performance of
                        // black/white write version with an extra comparison per loop
                        do {
                            *pBuf ^= 0xFF;  // Invert byte
                            pBuf  += SSD1306_LCDWIDTH; // Advance pointer 8 rows
                            h     -= 8;     // Subtract 8 rows from height
                            } 
                        while(h >= 8);
                        } 
                    else{
                        // store a local value to work with
                        uint8_t val = (color != BLACK) ? 255 : 0;
                        do {
                            *pBuf = val;    // Set byte
                            pBuf += SSD1306_LCDWIDTH;  // Advance pointer 8 rows
                            h    -= 8;      // Subtract 8 rows from height
                            } 
                        while(h >= 8);
                        }
                    }

                if(h){ // Do the final partial byte, if necessary
                    mod = h & 7;
                    uint8_t mask = (1 << mod) - 1;
                    switch(color) {
                        case WHITE:   *pBuf |=  mask; break;
                        case BLACK:   *pBuf &= ~mask; break;
                        case INVERSE: *pBuf ^=  mask; break;
                        }
                    }
                }
            } // endif positive height
        } // endif x in bounds
}

int oledb_getPixel(OLEDB_SSD1306* ptr,int16_t x, int16_t y) 
{
    if((x >= 0) && (x < AvnetGFX_width()) && (y >= 0) && (y < AvnetGFX_height())) {
        // Pixel is in-bounds. Rotate coordinates if needed.
        switch(AvnetGFX_getRotation()) {
         case 1:
             _swap(x, y);
             x = SSD1306_LCDWIDTH - x - 1;
             break;
         case 2:
             x = SSD1306_LCDWIDTH  - x - 1;
             y = SSD1306_LCDHEIGHT - y - 1;
             break;
         case 3:
             _swap(x, y);
             y = SSD1306_LCDHEIGHT - y - 1;
             break;
             }
        return (ptr->buffer[x + (y / 8) * SSD1306_LCDWIDTH] & (1 << (y & 7)));
        }
    return FALSE; // Pixel out of bounds
}

uint8_t *oledb_getBuffer(OLEDB_SSD1306* ptr) 
{
    return ptr->buffer;
}





