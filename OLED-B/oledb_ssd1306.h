/*!
 * @file oledb_ssd1306.h
 *
 */

#ifndef _OLEDB_SSD1306_h_
#define _OLEDB_SSD1306_h_

#include <stdint.h>
#include <stddef.h>

#define TRUE                                            1 ///< for general usage
#define FALSE                                           0 ///< for general usage

#define SSD1306_LCDWIDTH                               96 ///< MicroE OLED-B display width in pixels
#define SSD1306_LCDHEIGHT                              39 ///< MicroE OLED-B display height in pixels

#define BLACK                                           0 ///< Draw 'off' pixel
#define WHITE                                           1 ///< Draw 'on' pixel
#define INVERSE                                         2 ///< Invert pixel

#define SSD1306_MEMORYMODE                           0x20 ///< See datasheet
#define SSD1306_COLUMNADDR                           0x21 ///< See datasheet
#define SSD1306_PAGEADDR                             0x22 ///< See datasheet
#define SSD1306_SETCONTRAST                          0x81 ///< See datasheet
#define SSD1306_CHARGEPUMP                           0x8D ///< See datasheet
#define SSD1306_SEGREMAP                             0xA0 ///< See datasheet
#define SSD1306_DISPLAYALLON_RESUME                  0xA4 ///< See datasheet
#define SSD1306_DISPLAYALLON                         0xA5 ///< See datasheet
#define SSD1306_NORMALDISPLAY                        0xA6 ///< See datasheet
#define SSD1306_INVERTDISPLAY                        0xA7 ///< See datasheet
#define SSD1306_SETMULTIPLEX                         0xA8 ///< See datasheet
#define SSD1306_DISPLAYOFF                           0xAE ///< See datasheet
#define SSD1306_DISPLAYON                            0xAF ///< See datasheet
#define SSD1306_COMSCANINC                           0xC0 ///< See datasheet
#define SSD1306_COMSCANDEC                           0xC8 ///< See datasheet
#define SSD1306_SETDISPLAYOFFSET                     0xD3 ///< See datasheet
#define SSD1306_SETDISPLAYCLOCKDIV                   0xD5 ///< See datasheet
#define SSD1306_SETPRECHARGE                         0xD9 ///< See datasheet
#define SSD1306_SETCOMPINS                           0xDA ///< See datasheet
#define SSD1306_SETVCOMDETECT                        0xDB ///< See datasheet

#define SSD1306_SETLOWCOLUMN                         0x00 ///< Not currently used
#define SSD1306_SETHIGHCOLUMN                        0x10 ///< Not currently used
#define SSD1306_SETSTARTLINE                         0x40 ///< See datasheet

#define SSD1306_EXTERNALVCC                          0x01 ///< External display voltage source
#define SSD1306_SWITCHCAPVCC                         0x02 ///< Gen. display voltage from 3.3V

#define SSD1306_RIGHT_HORIZONTAL_SCROLL              0x26 ///< Init rt scroll
#define SSD1306_LEFT_HORIZONTAL_SCROLL               0x27 ///< Init left scroll
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29 ///< Init diag scroll
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL  0x2A ///< Init diag scroll
#define SSD1306_DEACTIVATE_SCROLL                    0x2E ///< Stop scroll
#define SSD1306_ACTIVATE_SCROLL                      0x2F ///< Start scroll
#define SSD1306_SET_VERTICAL_SCROLL_AREA             0xA3 ///< Set scroll range

#define SSD1306_COMMAND                              0x01 ///<when sending commands
#define SSD1306_DATA                                 0x00 ///<when sending data

#ifdef __cplusplus
extern "C" {
#endif

typedef struct oled_t {
    uint8_t* buffer;
    int  (*rst)(void);
    void (*dc)(int cmd);
    int (*wr)( uint16_t cmd, uint8_t *b, int siz );
    } OLEDB_SSD1306;

OLEDB_SSD1306* open_oled( void (*v)(void), int (*rst_func)(void), int (*wr_func)( uint16_t cmd, uint8_t *b, int siz ) );
void           close_oled(OLEDB_SSD1306*);

int            oledb_init(OLEDB_SSD1306* ptr, int reset );
void           oledb_display(OLEDB_SSD1306*, int  reverse);
void           oledb_clrDispBuff(OLEDB_SSD1306*);
void           oledb_invertDisplay(OLEDB_SSD1306*, int  i);
void           oledb_dim(OLEDB_SSD1306*, int  dim);
void           oledb_drawPixel(OLEDB_SSD1306*, int16_t x, int16_t y, uint16_t color);
int            oledb_getPixel(OLEDB_SSD1306*, int16_t x, int16_t y);
void           oledb_startscrollright(OLEDB_SSD1306*, uint8_t start, uint8_t stop);
void           oledb_startscrollleft(OLEDB_SSD1306*, uint8_t start, uint8_t stop);
void           oledb_startscrolldiagright(OLEDB_SSD1306*, uint8_t start, uint8_t stop);
void           oledb_startscrolldiagleft(OLEDB_SSD1306*, uint8_t start, uint8_t stop);
void           oledb_stopscroll(OLEDB_SSD1306*);
uint8_t*       oledb_getBuffer(OLEDB_SSD1306*);

void           oledb_drawFastHLineInternal(OLEDB_SSD1306*, int16_t x, int16_t y, int16_t w, uint16_t color);
void           oledb_drawFastVLineInternal(OLEDB_SSD1306*, int16_t x, int16_t y, int16_t h, uint16_t color);

#ifdef __cplusplus
}
#endif

#endif // _OLEDB_SSD1306_h_

