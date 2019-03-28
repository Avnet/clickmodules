/*
  MIKROE LCD mini click - Blink
  
 https://www.mikroe.com/lcd-mini-click

 This sketch prints "Hello World!" to the LCD and makes the
 cursor block blink.

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe
 modified 7 Nov 2016
 by Arturo Guadalupi
 modified 21 Feb 2019
 by Teodor Costachioiu (https://electronza.com)

 This example code is in the public domain.

 https://electronza.com/arduino-mikroe-lcdmini-library

*/

// include the library code:
#include <MIKROE_LCDmini.h>

/* The LCD library is initialized as follows:
 * MCP23S17_LCD(uint8_t rst, uint8_t cs, uint8_t cs2, uint8_t pwmpin);
 * rst  - RST pin for MCP23S17
 * cs   - CS pin for MCP23S17 
 * cs2  - CS pin for MCP4161
 * pwmpin - lcd backlight control pin
 * 
 * Examples, when using Arduino Uno and Arduino Uno click shield
 * https://www.mikroe.com/arduino-uno-click-shield
 * for mikroBUS socket #1
 * MCP23S17_LCD lcd(A3, 10, A0, 6);
 * for mikroBUS socket #2
 * MCP23S17_LCD lcd(A2, 9, A1, 5);
*/

MIKROE_LCDmini lcd(A3, 10, A0, 6);

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin();
  lcd.setContrast(30);
  // Print a message to the LCD.
  lcd.print("hello, world!");
  //lcd.setBacklight(600);   
}

void loop() {
  // Turn off the blinking cursor:
  lcd.noBlink();
  delay(3000);
  // Turn on the blinking cursor:
  lcd.blink();
  delay(3000);
}


