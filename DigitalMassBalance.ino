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
#include "src\Config.hpp" 
#include "src\MOST_MassBalance.h"
#include "src\LCD.h"


// Hardware objects (uses external libraries).
// LCD Display. Pins defined in Pinouts.hpp.
LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
LCD display(&lcd, LCD_ROWS, LCD_COLS);
MOST_MassBalance myBalance(&display, LCD_VCC, BTN_TARE);

// Variables used during data collection.
// Sensitivity is read from memory - this is here as a default.
extern double sensitivity;
// Used as an offset from zero (ie for a container). Implemented in this script,
// while zero is implemented within the HX711 library.
extern double tareWeight;
// Measured mass (can be assigned an averaged/filtered value or an instantaneous
// value.
extern double mass;
  

void setup() {
  // Initialization methods have self-explanatory names. Initializers as of REV
  // 1.0.0 report non-standard information over serial. Those at startup are the
  // only non-standard serial communications this scale produces.
  myBalance.initSerial();
  myBalance.initLoadCell();
  myBalance.initDisplay();
  myBalance.getSensitivity();
  myBalance.initQueue();
  pinMode(BTN_TARE, INPUT_PULLUP);
  Serial.print(F("\nUse <LF>X?<CR> to view serial commands\r"));
}


void loop() {
  // Get the averaged, tared mass.
  mass = myBalance.getMassAveraged();
  
  // Enforce report rate without hampering sample rate (sample rate depends on
  // how much processing is done during each iteration of loop().
  // TODO: add a command to change the report rate.
  if (millis() - lastRefresh > 1/REPORT_RATE * 1000) {
    // Reset the time. Last refresh is initialized in Config.hpp.
    lastRefresh = millis();
    
    // Listen for input over serial.
    // When using Arduino Serial Monitor, switch to 'Both NL & CR' in bottom 
    // right. When the scale first starts up, hit enter once to queue up a <LF>
    // character, otherwise the first command will not meet com standards and 
    // return a ?
    // When using Putty, use Ctrl+J for LF, followed by command, followed by 
    // Ctrl+M or simply Enter for CR.
    if (Serial.available() > 2) { // Minimum cmd length is 3 characters<LF>c<CR>
      myBalance.doSerial();
    }
    
    if (isContinuousReport) {
      myBalance.reportMassAveraged();
    }
    
    // Simple scale functionality. Note that placement of button listener 
    // requires an extended button press. Assuming a report rate of at least
    // 1 Hz, this should not be an issue.
    myBalance.displayMass(mass);
    myBalance.listenForButtonInput();
  }
}
