
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "lcdmini.h"

// defines for MCP4161
#define MCP4161_WIPER           0x00

// defines for MCP23S17; the lcd-mini click uses Bank #0
#define MCP23S17_OLATB          0x15
#define MCP23S17_GPIOB          0x13
#define MCP23S17_IODIRB         0x01
#define MCP23S17_IOCON          0x0A  // Shared Register 
#define MCP23S17_OPCODE         0x40  // Control Byte: "0100 A2 A1 A0 R/W" -- W=0

#define LCD_RST                 0x04  //corresponds to GPB2 on MCP23S17
#define LCD_ENABLE              0x08  //corresponds to GPB3 on MCP23S17

//    MCP23S17-GPB3 => E   (enable)
//    MCP23S17-GPB2 => RS  (register select)

static void (*cs1_cntrl)(int isSet);                       //controls the MCP23S17 CS pin
static void (*cs2_cntrl)(int isSet);                       //controls the DigiPOT CS pin
static void (*rst_cntrl)(int isSet);                       //controls the MCP23S17 RST pin
static void (*pwm_cntrl)(int dcycle);                      //control the backlight 
static void (*spiTX)( uint8_t *b, int siz );               //sends command out the SPI bus

static uint8_t _displayFunction;
static uint8_t _displayOperation;
static uint8_t _displayState;

static void _LCDCommand(uint8_t value);
static void _MCP23S17_setup(void);
static void _MCP4161_setup(void);
static void _MCP23S17_TxByte(uint8_t addr, uint8_t tbyte);
static void _MCP23S17_putchar(uint8_t output);

// When the displayOn powers up, configure as follows:
//
// 1. Display clear
// 2. Function set: 
//    DL = 1; 8-bit interface data 
//    N = 0; 1-line displayOn 
//    F = 0; 5x8 dot character font 
// 3. Display on/off control: 
//    D = 0; Display off 
//    C = 0; Cursor off 
//    B = 0; Blinking off 
// 4. Entry mode set: 
//    I/D = 1; Increment by 1 
//    S = 0; No shift 
//

// The users init() function must:
//   1. setup the SPI interface for 8-bit, MODE0.0, 100000
//   2. setup gpio managment pins

// open_lcdmini performs the following assignments:
//   * sets the functions to be called for I/O control of cs1, cs2, rst
//   * sets the function to be called for the SPI read/write function
//   * sets the function to be called for controlling the PWM pin
// It then configures the expander, digipot, and LCD itself
//

int open_lcdmini( 
    void (*init)(void), 
    void (*cs1)(int), 
    void (*cs2)(int), 
    void (*rst)(int), 
    void (*fpwm)(int), 
    void (*spi_tx)(uint8_t*, int) )
{
    init();  //call the users init() function to initialize all I/O lines and interfaces

    cs1_cntrl  =cs1;
    cs2_cntrl  =cs2;
    rst_cntrl  =rst;
    pwm_cntrl =fpwm;
    spiTX     =spi_tx;

    _MCP23S17_setup();   //setup the IO expander
    _MCP4161_setup();    //setup Digi-pot

    _delay(5); //give everything 5ms to stabilize 
    
     // establish the display state, function and operation
    _displayState    = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;  
    _displayFunction = LCD_4BITMODE | LCD_5x8DOTS |  LCD_2LINE;
    _displayOperation= LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;

    _LCDCommand(LCD_FUNCTIONSET  | _displayFunction);  
    _LCDCommand(LCD_ENTRYMODESET | _displayOperation);
    lcd_display(ON);
    lcd_clearDisplay();
    return 1;
}


/*! --------------------------------------------------------------------------------
* @brief close_lcdmini() - turn off the LCD Mini click by simply turning the LCD off.
*    this is the location to close any opened I/O or free malloc'd memory.
*/
void close_lcdmini( void )
{
    lcd_display(OFF);
}


/*! --------------------------------------------------------------------------------
* @brief setCursor(column,row)  - positions the active cursor to the requested column/row.  
*
* @param  col column of active cursor, must be value 0-15
*         row row of active cursor, must be value of 0 or 1
# @return -1 if an error in parameters was detected or 0 if no erro
*/
int lcd_setCursor(uint8_t col, uint8_t row)
{
    if( row > 1 || col > 15 )  //we only have 2 rows (0 & 1) and 16 columns (0-15)
        return -1;
    size_t ddram_row = (!row)? 0x40 : 0;
    _LCDCommand(LCD_SETDDRAMADDR | (col + ddram_row));
    return 0;
}


/*! --------------------------------------------------------------------------------
* @brief setBacklight(intensity)  - the intensity of the backlight is controlled by 
*        pulse-width-modulating a bit to the digi-pot.  The PWM function is managed
*        by the users pwm_cntrl function which was setup when the lcdmini was opened.
* @param inten is the desired intensity, a value from 0 to 100 (corresponds to %)
*/
void lcd_setBacklight(uint16_t inten)
{
    if( inten > 100 )
        inten = 100;
    pwm_cntrl(inten);
}

/*! --------------------------------------------------------------------------------
* @brief display(int) - turn the display on or off
* @param on,  1=on, 0=off
*/
void lcd_display(int on)     // Turn the display on(1) or Off(0)
{
  if( on )
      _displayState |= LCD_DISPLAYON;
  else
      _displayState &= ~LCD_DISPLAYON;
  _LCDCommand(LCD_DISPLAYCONTROL | _displayState);
}

/*! --------------------------------------------------------------------------------
* @brief cursor(int) - turn the cursor on or off
* @param on,  1=on, 0=off
*/
void lcd_cursor(int on)
{
    if( on )
        _displayState |= LCD_CURSORON;
    else
        _displayState &= ~LCD_CURSORON;
  _LCDCommand(LCD_DISPLAYCONTROL | _displayState);
}

/*! --------------------------------------------------------------------------------
* @brief blink(int) - turn on/off blinking of the cursor 
* @param on,  1=on, 0=off
*/
void lcd_blink(int on)
{
    if( on )
        _displayState |= LCD_BLINKON;
    else
        _displayState &= ~LCD_BLINKON;
  _LCDCommand(LCD_DISPLAYCONTROL | _displayState);
}

/*! --------------------------------------------------------------------------------
* @brief clearDisplay() - clears the display and sets the curor positon to 0,0
*/
void lcd_clearDisplay()
{
  _LCDCommand(LCD_CLEARDISPLAY);  // clear displayOn, set cursorOn position to zero
  _delay(2);                  
}

/*! --------------------------------------------------------------------------------
* @brief home() - positions the cursor to 0,0
*/
void lcd_home()
{
  _LCDCommand(LCD_RETURNHOME);    // set cursorOn position to zero
  _delay(2);                  
}


/*! --------------------------------------------------------------------------------
* @brief autoscroll() - sets the LCD to auto scroll text as it is sent
*
* @param  on is set to 1 to enable autoscroll, set to 0 to disable autoscroll
*/
void lcd_autoscroll(int on)
{
    if( on )
        _displayOperation |= LCD_ENTRYSHIFTINCREMENT;
    else
        _displayOperation &= ~LCD_ENTRYSHIFTINCREMENT;

    _LCDCommand(LCD_ENTRYMODESET | _displayOperation);
}


/*! --------------------------------------------------------------------------------
* @brief scrollDisplay() - moves the contents of the display either left or right
*
* @param  left is set to 1 to move the contents to the left, 0 to move contents to the right
*/
void lcd_scrollDisplay(int left)
{
    _LCDCommand(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | (left? LCD_MOVELEFT: LCD_MOVERIGHT));
}


/*! --------------------------------------------------------------------------------
* @brief textOutputDirection() - as text is printed to the display, it goes either
*     left to right, or right to left.  This function sets that direction for output
*
* @param  goRight
*/
void lcd_textOutputDirection(int goRight) 
{
    if( goRight == RIGHT2LEFT)
        _displayOperation |= LCD_ENTRYLEFT;
    else
        _displayOperation &= ~LCD_ENTRYLEFT;
    _LCDCommand(LCD_ENTRYMODESET | _displayOperation);
}



void lcd_setContrast(uint8_t contrast)
{
   uint8_t buff[2];

   buff[0] = MCP4161_WIPER;
   buff[1] = contrast;
   cs2_cntrl(0);          //select the MCP4161 SPI Digital POT
   spiTX(buff, 2);   //set Volatile Wiper 0 to the desired contract
   cs2_cntrl(1);          //de-select the MCP4161 
}  

// Fill the first 8 CGRAM locations
// with custom characters
void lcd_createChar(uint8_t location, uint8_t charmap[]) {
  location &= 0x7; // we only have 8 locations 0-7
  _LCDCommand(LCD_SETCGRAMADDR | (location << 3));
  for (int i=0; i<8; i++) {
    uint8_t HighNibble = (charmap[i] & 0xf0) | 0x04;             //set the RSBIT for each nibble
    uint8_t LowNibble  = (charmap[i] << 4)   | 0x04;

    _MCP23S17_TxByte(MCP23S17_GPIOB, HighNibble);                 //then write data to output latch
    _MCP23S17_TxByte(MCP23S17_GPIOB, HighNibble| LCD_ENABLE);     //set the LCD enable bit
    _MCP23S17_TxByte(MCP23S17_GPIOB, HighNibble&(~LCD_ENABLE));   //and clear the LCD enable bit

    _MCP23S17_TxByte(MCP23S17_GPIOB, LowNibble);                 //write low nibble to output latch
    _MCP23S17_TxByte(MCP23S17_GPIOB, LowNibble| LCD_ENABLE);     //set the LCD enable bit
    _MCP23S17_TxByte(MCP23S17_GPIOB, LowNibble&(~LCD_ENABLE));   //and clear the LCD enable bit
    }
}

int  lcd_putchar(uint8_t c)
{
    _MCP23S17_putchar(c);
    return 0;
}


int  lcd_puts(const char *s)
{
    for(int i=0; i<strlen(s); i++)
        _MCP23S17_putchar(s[i]);
    _MCP23S17_putchar('\n');
    return 0;
}


size_t lcd_printf(const char *fmt, ...)
{
    char buff[255];
    va_list ap;
    va_start(ap, fmt);
    vsprintf (buff,fmt, ap);
    for( size_t i=0; i<strlen(buff); i++ )
        _MCP23S17_putchar(buff[i]);
    va_end(ap);
    return strlen(buff);
}




static void _LCDCommand(uint8_t value) 
{
    uint8_t HighNibble = (value >> 4) & 0xF;
    uint8_t LowNibble  = value & 0xF;

    _MCP23S17_TxByte(MCP23S17_GPIOB, HighNibble);                 //set data into output latch
    _MCP23S17_TxByte(MCP23S17_GPIOB, HighNibble| LCD_ENABLE);     //then set the LCD enable bit
    _MCP23S17_TxByte(MCP23S17_GPIOB, HighNibble&(~LCD_ENABLE));   //then clear the LCD enable bit

    _MCP23S17_TxByte(MCP23S17_GPIOB, LowNibble);                  //set data into output latch
    _MCP23S17_TxByte(MCP23S17_GPIOB, LowNibble | LCD_ENABLE);     //followed by the low nibble
    _MCP23S17_TxByte(MCP23S17_GPIOB, LowNibble &(~LCD_ENABLE));   //then clear LCD enable bit
}

// This function confugires the MCP23S17 port expander which is connected to the LCD (see
// lcd-mini-click-schematic-v101.pdf).  The LCD is connected using the following MCP23S17 
// pins:
//    MCP23S17    LCD
//      GPB7   => D7
//      GPB6   => D6
//      GPB5   => D5
//      GPB4   => D4
//      GPB3   => E   (enable)
//      GPB2   => RS  (register select)
//
// set the OLATB register on the MCP23S17 to latch GPBx outputs that drive the LCD

static void _MCP23S17_setup(void)
{
    rst_cntrl(1);
    cs1_cntrl(1);
    _MCP23S17_TxByte(MCP23S17_IOCON, 0b00001000); // Set MCP23S17 address (A1/A2/A3) to match address pins (000)
    _MCP23S17_TxByte(MCP23S17_IODIRB, 0);         // configure LCD port direction as output
    _MCP23S17_TxByte(MCP23S17_GPIOB,  0);         // set LCD port as 0
}

static void _MCP4161_setup(void)
{
    cs2_cntrl(1);
}


// Writes to MCP23S17
static void _MCP23S17_TxByte(uint8_t addr, uint8_t tbyte)
{
    uint8_t buff[3];
    buff[0] = MCP23S17_OPCODE;
    buff[1] = addr;
    buff[2] = tbyte;

    cs1_cntrl(0);
    spiTX(buff, 3);
    cs1_cntrl(1);
}

// Updates the status of MCP23S17 port
static void _MCP23S17_putchar(uint8_t output)
{
    _MCP23S17_TxByte(MCP23S17_GPIOB, output);
}

