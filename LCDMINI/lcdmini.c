
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
#define LCD_E                   0x08  //corresponds to GPB3 on MCP23S17

//    MCP23S17-GPB3 => E   (enable)
//    MCP23S17-GPB2 => RS  (register select)

static void (*cs1_cntrl)(int isSet);                       //controls the MCP23S17 CS pin
static void (*cs2_cntrl)(int isSet);                       //controls the DigiPOT CS pin
static void (*rst_cntrl)(int isSet);                       //controls the MCP23S17 RST pin
static void (*pwm_cntrl)(int dcycle);                      //control the backlight 
static void (*spiTX)( uint16_t cmd, uint8_t *b, int siz ); //sends command out the SPI bus

static uint8_t _displayFunction;
static uint8_t _displayOperation;
static uint8_t _displayState;

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
//   * sets the function to be called for I/O control of cs1, cs2, rst
//   * sets the function to be called for the SPI read/write function
//   * sets the function to be called for controlling the PWM pin
// It then configures the expander, digipot, and LCD itself
//

LCDMINI* open_lcdmini( 
    void *(init)(int), 
    void (*cs1)(int), 
    void (*cs2)(int), 
    void (*rst)(int), 
    void (*fpwm)(int), 
    void (*spi_tx)(uint8_t*, int) )
{
    cs1_ctrl  =cs1;
    cs2_ctrl  =cs2;
    rst_ctrl  =rst;
    pwm_ctrl  =fpwm;
    spiTX_ctrl=spi_tx;

    init();  //call the users init() function to initialize all I/O lines and interfaces

    MCP23S17_setup();
    MCP4161_setup();

    _delay(50); //give everything 50ms for power to stabilize 
//
// the LCD is connected using the following MCP23S17 to LCD connections:
//    MCP23S17-GPB7 => D7
//    MCP23S17-GPB6 => D6
//    MCP23S17-GPB5 => D5
//    MCP23S17-GPB4 => D4
//    MCP23S17-GPB3 => E   (enable)
//    MCP23S17-GPB2 => RS  (register select)
// so we need to set the OLATB register on the MCP23S17 to latch GPBx outputs 

    
     // establish the display state, function and operation
    _displayState    = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;  
    _displayFunction = LCD_4BITMODE | LCD_5x8DOTS |  LCD_2LINE;
    _displayOperation= LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;

    setLCDdimensions(0x00, 0x40, 0x00 + LCD_COLUMNS, 0x40 + LCD_COLUMNS);  
    LCDCommand(LCD_FUNCTIONSET  | _displayFunction);  
    LCDCommand(LCD_ENTRYMODESET | _displayOperation);
    display(1);
    clearDisplay();
}





































void close_lcdmini( void )
{
    display(0);
}

void setBacklight(uint16_t bl)
{
   if(bl >0 && bl < 1023 ) 
       analogWrite(_pwmpin, bl);
}

void display(int on)     // Turn the display on(1) or Off(0)
{
  if( on )
      _displayState |= LCD_DISPLAYON;
  else
      _displayState &= ~LCD_DISPLAYON;
  LCDCommand(LCD_DISPLAYCONTROL | _displayState);
}

void cursor(int on)
{
    if( on )
        _displayState |= LCD_CURSORON;
    else
        _displayState &= ~LCD_CURSORON;
  LCDCommand(LCD_DISPLAYCONTROL | _displayState);
}

void blink(int on)
{
    if( on )
        _displayState |= LCD_BLINKON;
    else
        _displayState &= ~LCD_BLINKON;
  LCDCommand(LCD_DISPLAYCONTROL | _displayState);
}

void scrollDisplayLeft(void) {
  LCDCommand(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}

void scrollDisplayRight(void) {
  LCDCommand(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void textLeft2Right(void) {
  _displayOperation |= LCD_ENTRYLEFT;
  LCDCommand(LCD_ENTRYMODESET | _displayOperation);
}

// This is for text that flows Right to Left
void textRight2Left(void) {
  _displayOperation &= ~LCD_ENTRYLEFT;
  LCDCommand(LCD_ENTRYMODESET | _displayOperation);
}

// This will 'right justify' text from the cursorOn
void autoscroll(void) {
  _displayOperation |= LCD_ENTRYSHIFTINCREMENT;
  LCDCommand(LCD_ENTRYMODESET | _displayOperation);
}

// This will 'left justify' text from the cursorOn
void noAutoscroll(void) {
  _displayOperation &= ~LCD_ENTRYSHIFTINCREMENT;
  LCDCommand(LCD_ENTRYMODESET | _displayOperation);
}

// Fill the first 8 CGRAM locations
// with custom characters
void createChar(uint8_t location, uint8_t charmap[]) {
  location &= 0x7; // we only have 8 locations 0-7
  LCDCommand(LCD_SETCGRAMADDR | (location << 3));
  for (int i=0; i<8; i++) {
    write(charmap[i]);
  }
}

void clearDisplay()
{
  LCDCommand(LCD_CLEARDISPLAY);  // clear displayOn, set cursorOn position to zero
  _delay(2);                  // and give it 2ms to complete
}

void home()
{
  LCDCommand(LCD_RETURNHOME);    // set cursorOn position to zero
  _delay(2);                  // and give it 2ms to complete
}

void setLCDdimensions(int row0, int row1, int row2, int row3)
{
  _row_offsets[0] = row0;
  _row_offsets[1] = row1;
  _row_offsets[2] = row2;
  _row_offsets[3] = row3;
}

void setCursor(uint8_t col, uint8_t row)
{
  const size_t max_lines = sizeof(_row_offsets) / sizeof(*_row_offsets);
  if ( row >= max_lines ) {
    row = max_lines - 1;    // we count rows starting w/0
  }
  LCDCommand(LCD_SETDDRAMADDR | (col + _row_offsets[row]));
}

void setContrast(uint8_t contrast)
{
   uint8_t buff[2];

   buff[0] = MCP4161_WIPER;
   buff[1] = contrast;
   cs2_cntrl(0);          //select the MCP4161 SPI Digital POT
   spiTX_cntl(buff, 2);   //set Volatile Wiper 0 to the desired contract
   cs2_cntrl(1);          //de-select the MCP4161 
}  


/********** high level commands, for the user! */

/*********** mid level commands, for sending data/cmds */

inline void LCDCommand(uint8_t value) 
{

  write8bits(value, LOW);

    uint8_t HighNibble = value >> 4;
    uint8_t LowNibble  = value & 0xF;

    MCP23S17_TxByte(MCP23S17_GPIOB, HighNibble);                 //set data into output latch
    MCP23S17_TxByte(MCP23S17_GPIOB, HighNibble| LCD_ENABLE);     //then set the LCD enable bit
    delay(1);
    MCP23S17_TxByte(MCP23S17_GPIOB, HighNibble&(~LCD_ENABLE));     //then set the LCD enable bit

    MCP23S17_TxByte(MCP23S17_GPIOB, LowNibble);                  //set data into output latch
    MCP23S17_TxByte(MCP23S17_GPIOB, LowNibble | LCD_ENABLE);     //followed by the low nibble
    delay(1);
    MCP23S17_TxByte(MCP23S17_GPIOB, LowNibble &(~LCD_ENABLE));   //then disable LCD enable bit
}

//
// we can only write 4-bits at a time to the LCD... but this is only needed
// when writing user defined characters to the display.
//
inline void write(uint8_t value) 
{
  write8bits(value, HIGH);
}

/************ low level data pushing commands **********/

void write8bits(uint8_t value, uint8_t RSbit) {
    uint8_t nibbleHigh = value >> 4;
    uint8_t nibbleLow = value & 0xF;
    uint8_t packetHigh = (nibbleHigh << 4) | (RSbit << 2);
    uint8_t packetLow = (nibbleLow << 4) | (RSbit << 2);
    // EN = 0
    MCP23S17_setOutput(packetHigh);
    delayMicroseconds(10);
    // EN = 1
    MCP23S17_setOutput(packetHigh | (1<<3));
    delayMicroseconds(10);
    // EN = 0
    MCP23S17_setOutput(packetHigh);
    delayMicroseconds(10);

    // EN = 0
    MCP23S17_setOutput(packetLow);
    delayMicroseconds(10);
    // EN = 1
    MCP23S17_setOutput(packetLow | (1<<3));
    delayMicroseconds(10);
    // EN = 0
    MCP23S17_setOutput(packetLow);
    delayMicroseconds(40);

}

// This function confugires the MCP23S17 port expander
void MCP23S17_setup(void)
{
    rst_cntrl(1)
    cs1_cntrl(1);
    MCP23S17_TxByte(MCP23S17_IOCON, 0b00001000); // Set MCP23S17 address (A1/A2/A3) to match address pins (000)

    pinMode(_rst, OUTPUT);
    pinMode(_cs, OUTPUT);
    digitalWrite(_rst, 1);
    digitalWrite(_cs, 1); 
    SPI.begin();                          // 2. Start SPI
    digitalWrite(_rst, 0);                // 3. briefly flash the reset pin
    delayMicroseconds(100);
    digitalWrite(_rst, 1);
    delayMicroseconds(100); 	

    MCP23S17_TxByte(MCP23S17_IOCON, 0b00001000); // Set the MCP23S17 address (A1/A2/A3) to match address pins (000)
    delayMicroseconds(50);
    MCP23S17_TxByte(MCP23S17_IODIRB, 0);         // configure LCD port direction as output
    MCP23S17_TxByte(MCP23S17_GPIOB, 0);	  // set LCD port as 0
    delayMicroseconds(50);
}

void MCP4161_setup(void){
    cs2_cntl(1);
}


// Writes to MCP23S17
void MCP23S17_TxByte(uint8_t addr, uint8_t tbyte)
{
    uint8_t buff[3];
    buff[0] = MCP23S17_OPCODE;
    buff[1] = addr;
    buff[2] = tbyte;

    cs1_cntrl(0);
    spiTX_cntl(buff, 3);
    cs1_cntrl(1);
}

// Updates the status of MCP23S17 port
void MCP23S17_setOutput(uint8_t output){
    MCP23S17_TxByte(MCP23S17_GPIOB, output);
}
