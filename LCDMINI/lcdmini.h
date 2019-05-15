#ifndef __LCDMINI_H__
#define __LCDMINI_H__

#include <stdint.h>
#include <stdarg.h>
#include <unistd.h>

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

#define ON                      1
#define OFF                     0
#define RIGHT2LEFT              1
#define GO_LEFT                 0
#define GO_RIGHT                1
#define LEFT2RIGHT              0

#define _delay(x) (usleep(x*1000))   //macro to provide ms pauses

int    open_lcdmini(void (*init)(),void (*cs1)(),void (*cs2)(),void (*rst)(),void (*fpwm)(),void (*spi_tx)(uint8_t*,int));
void   close_lcdmini( void );
int    lcd_setCursor(uint8_t col, uint8_t row);
void   lcd_setBacklight(uint16_t inten);
void   lcd_display(int on);
void   lcd_cursor(int on);
void   lcd_blink(int on);
void   lcd_clearDisplay(void);
void   lcd_home(void);
void   lcd_autoscroll(int on);
void   lcd_scrollDisplay(int left);
void   lcd_textOutputDirection(int goRight) ;
void   lcd_setContrast(uint8_t contrast);
void   lcd_createChar(uint8_t location, uint8_t charmap[]);

size_t lcd_printf(const char *fmt, ...);
int    lcd_puts(const char *);
int    lcd_putchar(uint8_t);

#endif  //__LCDMINI_H__

