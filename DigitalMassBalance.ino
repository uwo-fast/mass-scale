/*  DigitalMassBalance uses a load cell to measure and report an object's mass.
      This firmware is designed to meet SMA SCP 0499 Level #2 for scale serial 
      communication. The command and response formats for serial communication 
      are documented in included files.
      
      The scale was designed by researchers ing Michigan Technological 
      University's MOST group <https://www.appropedia.org/Category:MOST>
      
      REVISIONS:
      1.0.0 : Initial release - function scale with serial reporting.
      2.0.0 : First release up to SMA standards. Work to do on data filtering.
      
    Copyright (C) 2020 Benjamin Hubbard
      ! Note that external libraries included with this software are subject to
        their own licenses, included within their respective folders.
        
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/


// Headers including a variety of definitions for the scale. These are separated
// to help reduce the length of this script.
#include "src\MOST_MassBalance.h"
#include "src\LCD.h"

// HX711 Pins.
// Data pin.
const int HX_DT  = 2;
// Clock pin.
const int HX_SCK = 3;
// Vcc pin - HX711 requires 3-5V @ 1.5 mA, Nano supplies 5V @ 20 mA.
const int HX_VCC = 4;


// LCD Pins.
// Reset pin.
const int LCD_RS = A0;
// Enable pin.
const int LCD_EN = A1;
// Data pins.
const int LCD_D4 = A2;
const int LCD_D5 = A3;
const int LCD_D6 = A4;
const int LCD_D7 = A5;
// Power supply. allaboutcircuits.com suggests an LCD requires 5V @ 3mA.
const int LCD_VCC = 5;
// LCD Size.
const int LCD_ROWS = 2;
const int LCD_COLS = 16;
 
// Tare pin.
const int BTN_TARE = 8;


LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
LCD display(&lcd, LCD_ROWS, LCD_COLS);
MOST_MassBalance myBalance(&display, LCD_VCC, BTN_TARE);
  

void setup() {
  myBalance.begin();
}


void loop() {
  myBalance.measureListenReportAtRate(1);
}
