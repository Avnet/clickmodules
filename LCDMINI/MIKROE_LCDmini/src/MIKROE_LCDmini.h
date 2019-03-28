#ifndef MIKROE_LCDmini_h
#define MIKROE_LCDmini_h

#include <inttypes.h>
// MCP23S17 uses SPI
#include <SPI.h>
#include "Print.h"

// defines for MCP23S17
#define _port                   0x15
#define _iodir                  0x01
#define IOCON                   0x0A 
// Control byte and configuration register information - Control Byte: "0100 A2 A1 A0 R/W" -- W=0
#define    OPCODEW              0x40  // Opcode for MCP23S17, LSB (bit0) set to write (0)
#define    OPCODER              0x41  // Opcode for MCP23S17, LSB (bit0) set to read (1)

// commands
#define LCD_CLEARDISPLAY        0x01
#define LCD_RETURNHOME          0x02
#define LCD_ENTRYMODESET        0x04
#define LCD_DISPLAYCONTROL      0x08
#define LCD_CURSORSHIFT         0x10
#define LCD_FUNCTIONSET         0x20
#define LCD_SETCGRAMADDR        0x40
#define LCD_SETDDRAMADDR        0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT          0x00
#define LCD_ENTRYLEFT           0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON           0x04
#define LCD_DISPLAYOFF          0x00
#define LCD_CURSORON            0x02
#define LCD_CURSOROFF           0x00
#define LCD_BLINKON             0x01
#define LCD_BLINKOFF            0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE         0x08
#define LCD_CURSORMOVE          0x00
#define LCD_MOVERIGHT           0x04
#define LCD_MOVELEFT            0x00

// flags for function set
#define LCD_8BITMODE            0x10
#define LCD_4BITMODE            0x00
#define LCD_2LINE               0x08
#define LCD_1LINE               0x00
#define LCD_5x10DOTS            0x04
#define LCD_5x8DOTS             0x00

// According to the LCD used by the LCD mini click board
#define cols                    16
#define lines                   2
#define dotsize                 LCD_5x8DOTS


class MIKROE_LCDmini : public Print {
public:
  
  MIKROE_LCDmini(uint8_t rst, uint8_t cs, uint8_t cs2, uint8_t pwmpin);

  void init(uint8_t rst, uint8_t cs, uint8_t cs2, uint8_t pwmpin);
    
  void begin();

  void clear();
  void home();

  void noDisplay();
  void display();
  void noBlink();
  void blink();
  void noCursor();
  void cursor();
  void scrollDisplayLeft();
  void scrollDisplayRight();
  void leftToRight();
  void rightToLeft();
  void autoscroll();
  void noAutoscroll();

  void setRowOffsets(int row1, int row2, int row3, int row4);
  void createChar(uint8_t, uint8_t[]);
  void setCursor(uint8_t, uint8_t);
  void setContrast(uint8_t);
  void setBacklight(uint16_t);
  virtual size_t write(uint8_t);
  void command(uint8_t);
  
  using Print::write;

private:
  void send(uint8_t, uint8_t);
  void write4bits(uint8_t, bool);
  void write8bits(uint8_t, bool);
  void expander_setup();
  void digipot_setup();
  void expander_sendByte(uint8_t, uint8_t);
  void expander_setOutput(uint8_t);

  // uint8_t _rs_pin; // LOW: command.  HIGH: character.
  // uint8_t _rw_pin; // LOW: write to LCD.  HIGH: read from LCD.
  // uint8_t _enable_pin; // activated by a HIGH pulse.
  // uint8_t _data_pins[8];
  
  uint8_t _cs;
  uint8_t _rst;
  uint8_t _cs2;
  uint8_t _pwmpin;

  uint8_t _displayfunction;
  uint8_t _displaycontrol;
  uint8_t _displaymode;

  uint8_t _initialized;

  uint8_t _numlines;
  uint8_t _row_offsets[4];

};

#endif
