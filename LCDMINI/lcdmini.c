
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "lcdmini.h"


static void (*cs1_cntrl)(int isSet);                       //controls the MCP23S17 CS pin
static void (*cs2_cntrl)(int isSet);                       //controls the DigiPOT CS pin
static void (*rst_cntrl)(int isSet);                       //controls the MCP23S17 RST pin
static void (*pwm_cntrl)(int dcycle);                      //control the backlight 
static void (*spiTX)( uint8_t *b, int siz );               //sends command out the SPI bus
static void (*Delay_us)(uint16_t);                         //makes delay in order of microseconds 

static uint8_t _displayFunction = 0;
static uint8_t _displayOperation = 0;
static uint8_t _displayState = 0;

static void _LCDCommand(uint8_t value);
static void _MCP23S17_setup(void);
static void _MCP4161_setup(void);
static void _MCP23S17_TxByte(uint8_t addr, uint8_t tbyte);
static void _MCP23S17_putchar(uint8_t output);

void write4bits(uint8_t value, _Bool RSbit);

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

/**
  * @brief  performs the following assignments: sets the functions to be called for I/O control of cs1, cs2, rst; 
  *         sets the function to be called for the SPI read/write function and sets the function to be called for 
  *         controlling the PWM pin.
  * @param  init(): function where I/O and interfaces are initialized.
  * @param  cs1: function that controls CS1 pin. 
  * @param  cs2: function that controls CS2 pin. 
  * @param  rst: function that controls Reset pin. 
  * @param  fpwm: function that controls PWM pin. 
  * @param  spi_tx: function that controls SPI write.
  * @param  delay_us: function that makes delays of micro seconds.
  * @retval Positive if was unsuccefully, zero if was succefully.
  */
int open_lcdmini( 
    void (*init)(void), 
    void (*cs1)(int), 
    void (*cs2)(int), 
    void (*rst)(int), 
    void (*fpwm)(int), 
    void (*spi_tx)(uint8_t*, int),
	void (*delay_us)(uint16_t))
{
	
	// Call the users init() function to initialize all I/O lines and interfaces
    init();  

	// Assign I/O functions
    cs1_cntrl  =cs1;
    cs2_cntrl  =cs2;
    rst_cntrl  =rst;
    pwm_cntrl =fpwm;
    spiTX     =spi_tx;
	Delay_us = delay_us;

	// Setup the IO expander
    _MCP23S17_setup();   
    //_MCP4161_setup();    //setup Digi-pot

    // Give everything 5ms to stabilize 
	Delay_us(5000);

    // Establish the display state, function and operation
	// Send 8 bits command
	write4bits(0x03, 0);
	Delay_us(5000);

	// Send 8 bits command
	write4bits(0x03, 0);
	Delay_us(5000);
	
	// Send 8 bits command
	write4bits(0x03, 0);
	Delay_us(150);
	
	// Change to 4 bits mode
	write4bits(0x02, 0);

	// Configure LCD desired settings		
	_displayFunction = LCD_4BITMODE | LCD_5x8DOTS | LCD_2LINE;
	_LCDCommand(LCD_FUNCTIONSET | _displayFunction);

	Delay_us(150);

	// Turn on the display
	lcd_display(ON);

	Delay_us(1000);

	// Clear the display
	lcd_clearDisplay();

	// Configure LCD aditional settings	
	_displayOperation = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
    _LCDCommand(LCD_ENTRYMODESET | _displayOperation);

	// return zero, we can't read LCD because R/W pin is configured to Write by HW, so we can't
	// test if we have written correctly
    return 0;
}

//TODO: Close SPI amd I/Os
/*! --------------------------------------------------------------------------------
* @brief close_lcdmini() - turn off the LCD Mini click by simply turning the LCD off.
*        this is the location to close any opened I/O or free malloc'd memory.
* @param None.
* @retval None.
*/
void close_lcdmini( void )
{
    lcd_display(OFF);
}

/*! --------------------------------------------------------------------------------
* @brief setCursor(column,row)  - positions the active cursor to the requested column/row.  
* @param  col: column of active cursor, must be value 0-15
* @param  row: row of active cursor, must be value of 0 or 1
# @retval -1 if an error in parameters was detected or 0 if no error
*/
int lcd_setCursor(uint8_t col, uint8_t row)
{
	if (row > 1 || col > 15)  //we only have 2 rows (0 & 1) and 16 columns (0-15)
	{
		return -1;
	}
    size_t ddram_row = (!row)? 0x40 : 0;
    _LCDCommand(LCD_SETDDRAMADDR | (col + ddram_row));
    return 0;
}


/*! --------------------------------------------------------------------------------
* @brief setBacklight(intensity)  - the intensity of the backlight is controlled by 
*        pulse-width-modulating a bit to the digi-pot.  The PWM function is managed
*        by the users pwm_cntrl function which was setup when the lcdmini was opened.
* @param inten is the desired intensity, a value from 0 to 100 (corresponds to %)
* @retval None.
*/
void lcd_setBacklight(uint16_t inten)
{
	if (inten > 100)
	{
		inten = 100;
	}
    pwm_cntrl(inten);
}

/*! --------------------------------------------------------------------------------
* @brief display(int) - turn the display on or off
* @param on:  1=on, 0=off
* @retval None.
*/
void lcd_display(int on)     // Turn the display on(1) or Off(0)
{
	if (on)
	{
		_displayState |= LCD_DISPLAYON;
	}
	else
	{
		_displayState &= ~LCD_DISPLAYON;
	}
	_LCDCommand(LCD_DISPLAYCONTROL | _displayState);
}

/*! --------------------------------------------------------------------------------
* @brief cursor(int) - turn the cursor on or off
* @param on:  1=on, 0=off
* @retval None.
*/
void lcd_cursor(int on)
{
	if (on)
	{
		_displayState |= LCD_CURSORON;
	}
	else
	{
		_displayState &= ~LCD_CURSORON;
	}
	_LCDCommand(LCD_DISPLAYCONTROL | _displayState);
}

/*! --------------------------------------------------------------------------------
* @brief blink(int) - turn on/off blinking of the cursor 
* @param on:  1=on, 0=off.
* @retval None.
*/
void lcd_blink(int on)
{
	if (on)
	{
		_displayState |= LCD_BLINKON;
	}
	else
	{
		_displayState &= ~LCD_BLINKON;
	}
	_LCDCommand(LCD_DISPLAYCONTROL | _displayState);
}

/*! --------------------------------------------------------------------------------
* @brief clearDisplay() - clears the display and sets the curor positon to 0,0.
* @param None.
* @retval None.
*/
void lcd_clearDisplay()
{
	// Clear displayOn, set cursorOn position to zero
	_LCDCommand(LCD_CLEARDISPLAY);  
	Delay_us(1000);
	
}

/*! --------------------------------------------------------------------------------
* @brief home() - positions the cursor to 0,0.
* @param None.
* @retval None.
*/
void lcd_home()
{
	// Set cursorOn position to zero
	_LCDCommand(LCD_RETURNHOME);    
	Delay_us(2000);
}


/*! --------------------------------------------------------------------------------
* @brief autoscroll() - sets the LCD to auto scroll text as it is sent.
* @param  on is set to 1 to enable autoscroll, set to 0 to disable autoscroll.
* @retval None.
*/
void lcd_autoscroll(int on)
{
	if (on)
	{
		_displayOperation |= LCD_ENTRYSHIFTINCREMENT;
	}
	else
	{
		_displayOperation &= ~LCD_ENTRYSHIFTINCREMENT;
	}

    _LCDCommand(LCD_ENTRYMODESET | _displayOperation);
}


/*! --------------------------------------------------------------------------------
* @brief scrollDisplay() - moves the contents of the display either left or right.
* @param  left is set to 1 to move the contents to the left, 0 to move contents to the right.
* @retval None.
*/
void lcd_scrollDisplay(int left)
{
    _LCDCommand(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | (left? LCD_MOVELEFT: LCD_MOVERIGHT));
}


/*! --------------------------------------------------------------------------------
* @brief textOutputDirection() - as text is printed to the display, it goes either
*        left to right, or right to left.  This function sets that direction for output.
* @param  goRight: 0 goto the left, 1 go to the right
* @retval None.
*/
void lcd_textOutputDirection(int goRight) 
{
	if (goRight == RIGHT2LEFT)
	{
		_displayOperation |= LCD_ENTRYLEFT;
	}
	else
	{
		_displayOperation &= ~LCD_ENTRYLEFT;
	}
    _LCDCommand(LCD_ENTRYMODESET | _displayOperation);
}


/*! --------------------------------------------------------------------------------
* @brief lcd_setContrast() - set the contrast of the LCD
* @param contrast: 0 to 100 % contrast level
* @retval None.
*/
void lcd_setContrast(uint8_t contrast)
{
	uint8_t buff[2];

	buff[0] = MCP4161_WIPER;
	buff[1] = contrast;
	// Select the MCP4161 SPI Digital POT
	cs2_cntrl(0);   
	// Set Volatile Wiper 0 to the desired contract
	spiTX(buff, 2);  
	// De-select the MCP4161 
	cs2_cntrl(1);          
}  

/*! --------------------------------------------------------------------------------
* @brief lcd_createChar() - fill the first 8 CGRAM locations with custom characters.
* @param location: location to write the character.
* @param charmap[]: map of the character.
* @retval None.
*/
void lcd_createChar(uint8_t location, uint8_t charmap[]) 
{
	uint8_t HighNibble;
	uint8_t LowNibble;
	location &= 0x7; // we only have 8 locations 0-7
	_LCDCommand(LCD_SETCGRAMADDR | (location << 3));
	for (int i = 0; i < 8; i++) 
	{
		HighNibble = (charmap[i] & 0xf0) | 0x04;                      //set the RSBIT for each nibble
		LowNibble  = (charmap[i] << 4)   | 0x04;

		_MCP23S17_TxByte(MCP23S17_GPIOB, HighNibble);                 //then write data to output latch
		_MCP23S17_TxByte(MCP23S17_GPIOB, HighNibble| LCD_ENABLE);     //set the LCD enable bit
		_MCP23S17_TxByte(MCP23S17_GPIOB, HighNibble&(~LCD_ENABLE));   //and clear the LCD enable bit

		_MCP23S17_TxByte(MCP23S17_GPIOB, LowNibble);                 //write low nibble to output latch
		_MCP23S17_TxByte(MCP23S17_GPIOB, LowNibble| LCD_ENABLE);     //set the LCD enable bit
		_MCP23S17_TxByte(MCP23S17_GPIOB, LowNibble&(~LCD_ENABLE));   //and clear the LCD enable bit
	}
}

/*! --------------------------------------------------------------------------------
* @brief lcd_putchar() - write a char on the LCD.
* @param c: character to write.
* @retval None.
*/
void lcd_putchar(uint8_t c)
{
    _MCP23S17_putchar(c);
}

/*! --------------------------------------------------------------------------------
* @brief lcd_puts() - write a string on the LCD.
* @param s: pointer to the begining of string.
* @retval None.
*/
void lcd_puts(const char *s)
{
	for (int i = 0; i < strlen(s); i++)
	{
		_MCP23S17_putchar(s[i]);
	}
}

/*! --------------------------------------------------------------------------------
* @brief lcd_printf() - similar to standard printf function.
* @param fmt: pointer to the begining of string and formats.
* @param ...: variable to be printed.
* @retval strlen: size of the written buffer.
*/
size_t lcd_printf(const char *fmt, ...)
{
    char buff[255];
    va_list ap;
    va_start(ap, fmt);
    vsprintf (buff,fmt, ap);
	for (size_t i = 0; i < strlen(buff); i++)
	{
		_MCP23S17_putchar(buff[i]);
	}
    va_end(ap);
    return strlen(buff);
}


/*! --------------------------------------------------------------------------------
* @brief _LCDCommand() - write a command to the LCD.
* @param value: value of the command.
* @retval None.
*/
static void _LCDCommand(uint8_t value) 
{
    uint8_t HighNibble = (value >> 4) & 0xF;
    uint8_t LowNibble  = value & 0xF;

	HighNibble <<= 4;
	LowNibble <<= 4;

    _MCP23S17_TxByte(MCP23S17_GPIOB, HighNibble);                 //set data into output latch
	Delay_us(2);
    _MCP23S17_TxByte(MCP23S17_GPIOB, (HighNibble) | LCD_ENABLE);     //then set the LCD enable bit
	Delay_us(2);
    _MCP23S17_TxByte(MCP23S17_GPIOB, (HighNibble) &(~LCD_ENABLE));   //then clear the LCD enable bit
	Delay_us(2);

    _MCP23S17_TxByte(MCP23S17_GPIOB, (LowNibble));                  //set data into output latch
	Delay_us(2);
    _MCP23S17_TxByte(MCP23S17_GPIOB, (LowNibble) | LCD_ENABLE);     //followed by the low nibble
	Delay_us(2);
    _MCP23S17_TxByte(MCP23S17_GPIOB, (LowNibble) &(~LCD_ENABLE));   //then clear LCD enable bit
}

/*! --------------------------------------------------------------------------------
* @brief _MCP23S17_setup() - initialize MCP23S17 port expander. This function configures the MCP23S17 
*        port expander which is connected to the LCD (see lcd-mini-click-schematic-v101.pdf).  The LCD is 
*        connected using the following MCP23S17 pins:
*        MCP23S17    LCD
*         GPB7   => D7
*         GPB6   => D6
*         GPB5   => D5
*         GPB4   => D4
*         GPB3   => E   (enable)
*         GPB2   => RS  (register select)
*       set the OLATB register on the MCP23S17 to latch GPBx outputs that drive the LCD
* @param None.
* @retval None.
*/
static void _MCP23S17_setup(void)
{
	rst_cntrl(0);
	Delay_us(10);
    rst_cntrl(1);
    cs1_cntrl(1);
	
	Delay_us(10);

	// Set MCP23S17 address (A1/A2/A3) to match address pins (000)
	_MCP23S17_TxByte(MCP23S17_IOCON, 0b00101000); 
	// Configure LCD port direction as output
    _MCP23S17_TxByte(MCP23S17_IODIRB, 0);     
	// Set LCD port as 0
    _MCP23S17_TxByte(MCP23S17_GPIOB,  0);         
}

/*! --------------------------------------------------------------------------------
* @brief _MCP4161_setup() - initialize MCP4161 port expander.
* @param None.
* @retval None.
*/
static void _MCP4161_setup(void)
{
    cs2_cntrl(1);
}

// Writes to MCP23S17
/*! --------------------------------------------------------------------------------
* @brief _MCP23S17_TxByte() - write to MCP23S17 register.
* @param addr: address of the register to write.
* @param tbyte: value to write.
* @retval None.
*/
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

/*! --------------------------------------------------------------------------------
* @brief _LCDData() - write text to the LCD.
* @param value: character to write.
* @retval None.
*/
static void _LCDData(uint8_t value)
{
	uint8_t HighNibble = (value >> 4) & 0xF;
	uint8_t LowNibble = value & 0xF;
	uint8_t rs = 0x04;

	HighNibble <<= 4;
	LowNibble <<= 4;

	// Set RS and Enable pins
	_MCP23S17_TxByte(MCP23S17_GPIOB, LCD_ENABLE | rs) ;                 
	Delay_us(2);
	// Set the high nibble data
	_MCP23S17_TxByte(MCP23S17_GPIOB, (HighNibble) | LCD_ENABLE | rs);     
	Delay_us(2);
	// Then clear the LCD enable bit
	_MCP23S17_TxByte(MCP23S17_GPIOB, (HighNibble) &(~LCD_ENABLE) | rs);   
	Delay_us(2);
	// Set low nibble data into output
	_MCP23S17_TxByte(MCP23S17_GPIOB, (LowNibble) | rs);                  
	Delay_us(2);
	// Set LCD enable bit
	_MCP23S17_TxByte(MCP23S17_GPIOB, (LowNibble) | LCD_ENABLE | rs);
	Delay_us(2);
	// Then clear LCD enable bit
	_MCP23S17_TxByte(MCP23S17_GPIOB, (LowNibble &(~LCD_ENABLE)) | rs);   
	Delay_us(2);
	// Clear RS bit
	_MCP23S17_TxByte(MCP23S17_GPIOB, (LowNibble) &(~LCD_ENABLE) & (~rs));
	Delay_us(2);
}

/*! --------------------------------------------------------------------------------
* @brief _MCP23S17_putchar() - wrapper to write a character on LCD.
* @param output: character to write.
* @retval None.
*/
static void _MCP23S17_putchar(uint8_t output)
{
	_LCDData(output);
}

/*! --------------------------------------------------------------------------------
* @brief write4bits() - write command of 4 bits.
* @param value: value.
* @param RSbit: RS bit to tell the LCD if data sent is data or command.
* @retval None.
*/
void write4bits(uint8_t value, _Bool RSbit)
{
	uint8_t packet = (value << 4) | (RSbit << 2);
	// EN = 0
	_MCP23S17_TxByte(MCP23S17_GPIOB, packet);
	Delay_us(5);
	// EN = 1
	_MCP23S17_TxByte(MCP23S17_GPIOB, packet | (1 << 3));
	Delay_us(5);
	// EN = 0
	_MCP23S17_TxByte(MCP23S17_GPIOB, packet);
	Delay_us(40);
}

