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


const String REV = "2.0.1";


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
  Serial.print("\nUse <LF>X?<CR> to view serial commands\r");
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
      doSerial();
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


//----Support methods----// 
  //----Serial----//
    void doSerial() {
      // Give everything a chance to transmit over serial.
      delay(200);
      // Make sure the buffer is settled.
      Serial.flush();

      // Determine how many characters are waiting.
      int len = Serial.available();
      
      // Initialize an array to hold the command.
      int cmd[len];

      // Fill up cmd.
      receiveCommand(cmd, len);
      
      // Check for abort command.
      int escIdx = myBalance.findInArray(cmd, ESC, 0, len);
      if (escIdx >= 0) {  // If there's an escape character, reset.
        myBalance.softReset();
      }
      
      // Parse the command for the <LF> and <CR>. The command starts one char
      // beyond the LF, and ends with the CR.
      int startIdx = myBalance.findInArray(cmd, LF, 0, len) + 1;
      int endIdx = myBalance.findInArray(cmd, CR, startIdx, len);
      
      // Check for errors. Since we start after the LF, minimum index is 1. 
      // Note that findInArray returns -1 if it cannot find the character.
      if (startIdx < 1 || endIdx < 1) {
        Serial.println("\n?\r");
        return;
      }
      
      // Execute the command.
      doCommand(cmd, startIdx, endIdx);
    }

    // Note that arrays are pointers, so just pass in the array's variable.
    // TODO: make this accommodate commands that come character by character.
    //        This can be accomplished by clearing command in when receiving
    //        a LF, but requires preallocating potentially too much memory for a 
    //        command. Can be accomplished by peeking for a <CR>. Could have
    //        timing issues. Maybe watch the change in Serial.available().
    void receiveCommand(int *cmd, int len) {  
      // Read everything but the last character into the cmd array.
      // The last character could be an LF which would lead the next command.
      for (int i = 0; i < len-1; i++) {
        cmd[i] = Serial.read();
      }

      // If the next guy is <LF>, leave 'er alone (this accommodates Arduino
      // Serial Monitor behavior).
      // If not, add it to the cmd array (could be a CR from PuTTY, or a 
      // single character from terminal without local echo/line editing on.
      char last = Serial.peek();
      if (last != LF){
        // Chuck it at the end of the array.
        cmd[len-1] = Serial.read();
      }
    }
    
    
    void doCommand(int *cmd, int startIdx, int endIdx) {
      // By the SMA standard, the first character indicates the function. 
      // The scale should not accept a command if it doesn't match the syntax 
      // exactly. (eg <LF> wa <CR> should not execute the <LF> w <CR> command.)
      // To combat this, interpret commands by length.
      // First, cancel continuous reporting.
      isContinuousReport = 0;
      
      switch (endIdx - startIdx) {
        case 1:   // Single character commands.
          switch ((char) cmd[startIdx]) {
            case 'w': // Report weight.
            case 'W':
              myBalance.reportMassAveraged();
              break;
              
            case 'z': // Zero request.
            case 'Z':
              myBalance.zero();
              break;
              
            case 'd': // Run diagnostics.
            case 'D':
              // TODO: Implement actual diagnostics.
              Serial.print("\n    \r");
              break;
              
            case 'a': // About (first row).
            case 'A':
              aboutIdx = 0;
              Serial.print("\nSMA:2/1.0\r");
              break;
              
            case 'b': // aBout (scrolling).
            case 'B':
              switch (aboutIdx) {
                case 0:
                  Serial.print("\nMFG:Michigan Tech MOST\r");
                  break;
                case 1:
                  Serial.print("\nMOD:Digital Mass Balance\r");
                  break;
                case 2:
                  Serial.print("\nREV:" + REV + "\r");
                  break;
                case 3:
                  Serial.print("\nEND:\r");
                  break;
                default:
                  Serial.print("\n?\r");
                  break;
              }
              aboutIdx++;
              break;
              
            case 'r': // Continuous reporting request.
            case 'R':
              isContinuousReport = 1;
              break;
              
            case 't': // Tare request.
            case 'T':
              myBalance.tare();
              break;
              
            case 'c': // Clear tare.
            case 'C':
              myBalance.clearTare();
              break;
              
            case 'm': // Return the current tare weight.
            case 'M':
              myBalance.reportTare();
              break;
              
            default:  // Unknown command.
              Serial.print("\n?\r");
              break;
          } // End of single character commands.
          break;
          
        case 2: // 2 character commands.
          // Command will be an X followed by a character.
          switch ((char) cmd[startIdx]) {
            case 'x': // Custom commands.
            case 'X':
              switch ((char) cmd[startIdx + 1]) {
                case 'c': // Calibrate request (using hard-coded standard mass).
                case 'C':
                  // Calibration occurs with no tare.
                  myBalance.clearTareSilent();
                  myBalance.reportCalibrationMass();
                  myBalance.calibrate();
                  break;
                
                case 'l': // Toggle LCD power.
                case 'L':
                  if (isLCD) {
                    isLCD = false;
                    lcd.noDisplay();
                    digitalWrite(LCD_VCC, LOW);
                  } else if (!isLCD) {
                    isLCD = true;
                    myBalance.initDisplay();
                  }
                  break;
                  
                case 'p': // Toggle output precision.
                case 'P':
                  precision += 1;
                  if (precision > 4) {
                    precision = 0;
                  }
                  break;
                  
                case '?': // Tell the user what commands are available.
                  Serial.print("\nSMA SCP 0499 Serial Protocol\r");
                  Serial.print("\nAll cmds: <LF>cmd<CR>\r");
                  Serial.print("\nw           : averaged Weight\r");
                  Serial.print("\nz           : Zero scale\r");
                  Serial.print("\nd           : run Diagnostics\r");
                  Serial.print("\na           : About, first row\r");
                  Serial.print("\nb           : aBout, scroll\r");
                  Serial.print("\nr           : continuous Report\r");
                  Serial.print("\nt           : Tare scale\r");
                  Serial.print("\nc           : Clear tare\r");
                  Serial.print("\nm           : report tare weight\r");
                  Serial.print("\nxc          : enter Calibration mode\r");
                  Serial.print("\nxl          : toggle Lcd power\r");
                  Serial.print("\nxp          : scroll output Precision\r");
                  Serial.print("\nx?          : list all commands\r");
                  Serial.print("\nxc######.###: Calibrate with mass (needs 10 digits)\r");
                  // Give time for everything to send.
                  Serial.flush();
                  break;  
                  
                default:  // Unrecognized custom command.
                  Serial.print("\n?\r");
                  break;
              } // End of custom commands.
              break;
              
              // TODO: Add power saving commands/methods.
            default:  // Unrecognized 2 character command.
              Serial.print("\n?\r");
              break;
          } // End of 2 character commands.          
          break;
          
        case 12:  // Command with numeric input.
          // Commands will be an x, character, then 10 character number (with
          // leading whitespace).
          switch ((char) cmd[startIdx]) {
            case 'x': // Custom command.
            case 'X':
              switch ((char) cmd[startIdx + 1]) {
                case 'c': // Calibrate request.
                case 'C': { // Use brackets to prevent fall-through warnings.
                  // The calibration weight is submitted with 10 characters of 
                  // the value, plus 3 characters of units.
                  // TODO: handle multiple options for units.
                  myBalance.clearTareSilent();
                  
                  // Read the requested calibration mass.
                  String calStandard = "";
                  for (int i = 0; i < 10; i++) {
                    calStandard += String((char) cmd[startIdx + 2 + i]);
                  }
                  // BUG: there seems to be an overflow issue for large inputs.
                  // BUG: toDouble() only returns two decimal points of 
                  //      precision.
                  cal_standard_mass = calStandard.toDouble();
                  
                  myBalance.reportCalibrationMass();
                  myBalance.calibrate();
                  break;
                }
                
                default:  // Unrecognized custom, numeric input command.
                  Serial.print("\n?\r");
              } // End of custom commands.
              break;
          } // End of 12 character commands.
          break;
          
        default:  // Unrecognized command length.
          Serial.print("\n?\r");
          break;
      } // End of command interpretation.
    }
    
  
