#ifndef __LCDMINI_H__
#define __LCDMINI_H__

#include <stdint.h>

// LCD commands
#define LCD_CLEARDISPLAY        0x01
#define LCD_RETURNHOME          0x02
#define LCD_ENTRYMODESET        0x04
#define LCD_DISPLAYCONTROL      0x08
#define LCD_CURSORSHIFT         0x10
#define LCD_FUNCTIONSET         0x20
#define LCD_SETCGRAMADDR        0x40
#define LCD_SETDDRAMADDR        0x80
#define LCD_MOVERIGHT           0x04
#define LCD_DISPLAYMOVE         0x08
#define LCD_CURSORMOVE          0x00
#define LCD_MOVELEFT            0x00

// LCD Operation controls
#define LCD_ENTRYRIGHT          0x00
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYLEFT           0x02
#define LCD_ENTRYSHIFTDECREMENT 0x00

// LCD Display controls
#define LCD_BLINKON             0x01
#define LCD_CURSORON            0x02
#define LCD_DISPLAYON           0x04
#define LCD_DISPLAYOFF          0x00
#define LCD_CURSOROFF           0x00
#define LCD_BLINKOFF            0x00

// LCD Function settings
#define LCD_5x10DOTS            0x04
#define LCD_2LINE               0x08
#define LCD_8BITMODE            0x10
#define LCD_4BITMODE            0x00
#define LCD_1LINE               0x00
#define LCD_5x8DOTS             0x00

// LCD dimensions for the LCD mini click board
#define LCD_COLUMNS             16
#define LCD_LINES               2
#define LCD_DOTSIZE             LCD_5x8DOTS


int  open_lcdmini( void *(init)(int), void (*cs1)(int), void (*cs2)(int), void (*rst)(int), 
                   void (*fpwm)(int), void (*spi_tx)uint16_t, uint8_t*, int);
void close_lcdmini( void );

void setBacklight(uint16_t bl);
void display(int on);
void cursor(int on);
void blink(int on);

void scrollDisplayLeft(void) ;
void scrollDisplayRight(void) ;
void textLeft2Right(void) ;
void textRight2Left(void) ;
void autoscroll(void) ;
void noAutoscroll(void) ;
void createChar(uint8_t location, uint8_t charmap[]) ;
void clearDisplay();
void home();
void setLCDdimensions(int row0, int row1, int row2, int row3);
void setCursor(uint8_t col, uint8_t row);
void setContrast(uint8_t contrast);

#endif  //__LCDMINI_H__

