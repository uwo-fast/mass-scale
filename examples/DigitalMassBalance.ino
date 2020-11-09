/*  DigitalMassBalance uses a load cell to measure and report an object's mass.
      This firmware is designed to meet SMA SCP 0499 Level #2 for scale serial 
      communication. The command and response formats for serial communication 
      are documented in included files.
      
      The scale was designed by researchers in Michigan Technological 
      University's MOST group <https://www.appropedia.org/Category:MOST>
      
      REVISIONS:
      1.0.0 : Initial release - function scale with serial reporting.
      2.0.0 : First release up to SMA standards. Work to do on data filtering.
      
    Copyright (C) 2020 Benjamin Hubbard
      ! Note that external libraries included with this software are 
        subject to their own licenses, included within their respective 
        folders.
        
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see
    <https://www.gnu.org/licenses/>.
*/


// Headers including a variety of definitions for the scale. These are separated
// to help reduce the length of this script.
#include "src\MOST_MassBalance.h"
#include "src\OLED.h"
#include "src\LCD.h"

// HX711 Pins.
// Data pin.
#define mHX_DT  2
// Clock pin.
#define mHX_SCK 3
// Vcc pin - HX711 requires 3-5V @ 1.5 mA, Nano supplies 5V @ 20 mA.
#define mHX_VCC 4

// OLED power.
#define mOLED_VCC 6
// Display size
#define mOLED_HEIGHT 64
#define mOLED_WIDTH 128

// I2C Address (located on the back of your display).
#define mOLED_ADDR 0x3C


#define mLCD_RS A0

// Enable pin.
#define mLCD_EN A1

// Data pins.
#define mLCD_D4 A2
#define mLCD_D5 A3
#define mLCD_D6 A4
#define mLCD_D7 A5

// Power supply. allaboutcircuits.com suggests an LCD requires 5V @ 3mA.
#define mLCD_VCC 5

#define mLCD_ROWS 2
#define mLCD_COLS 6

// LCD Display. Pins defined in Pinouts.hpp.
LiquidCrystal lcd(mLCD_RS, mLCD_EN, mLCD_D4, mLCD_D5, mLCD_D6, mLCD_D7);
LCD display(&lcd, mLCD_ROWS, mLCD_COLS, mLCD_VCC);
MOST_MassBalance myBalance(10, 8, &display);

// Adafruit_SSD1306 oled(mOLED_WIDTH, mOLED_HEIGHT, &Wire); // < might need.
// OLED display(&oled, mOLED_WIDTH, mOLED_HEIGHT, mOLED_ADDR, mOLED_VCC);
// MOST_MassBalance myBalance(&display);
  // MOST_MassBalance myBalance(nullptr);

void setup() {
  myBalance.begin();
}


void loop() {
  myBalance.measureListenReportAtRate(1);
}
