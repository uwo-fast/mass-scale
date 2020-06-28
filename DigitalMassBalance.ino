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
#include "src\Pinouts.hpp"
#include "src\MOST_MassBalance.h"
#include "src\LCD.h"


// LCD Display. Pins defined in Pinouts.hpp.
const int LCD_ROWS = 2;
const int LCD_COLS = 16;
  
LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
LCD display(&lcd, LCD_ROWS, LCD_COLS);
MOST_MassBalance myBalance(&display, LCD_VCC, BTN_TARE);
  

void setup() {
  myBalance.begin();
}


void loop() {
  myBalance.measureListenReportAtRate(1);
}
