/*  MOST_MassBalance is a library of drivers for a digital mass balance
      
      This firmware is designed to meet SMA SCP 0499 Level #2 for scale
      serial communication. The command and response formats for serial communication are documented in included files.
      
      The scale was designed by researchers in Michigan Technological 
      University's MOST group <https://www.appropedia.org/Category:MOST>
      
      REVISIONS:
      1.0.0 : Initial release - function scale with serial reporting.
      2.0.0 : First release up to SMA standards. Work to do on data
              filtering.      
      3.0.0 : Refactor all functionality into a library.

    A NOTE ON SERIAL COMMUNICATION:
    - All commands are straddled by a newline \n and carriage return \r.
    - When using Arduino Serial Monitor, switch to 'Both NL & CR' in
      bottom right. When the scale first starts up, hit enter once to 
      queue up a <LF> character, otherwise the first command will not 
      meet com standards and return a ?
    - When using Putty, use Ctrl+J for LF, followed by command, followed
      by Ctrl+M or simply Enter for CR.
    
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

#include "MOST_MassBalance.h"


HX711 loadcell;

// Used as an offset from zero (ie for a container). Implemented in this
// script, while zero is implemented within the HX711 library.
double tareWeight = 0.0;
// Sensitivity is read from memory - this is here as a default.
double sensitivity = 1.0;


MOST_MassBalance::MOST_MassBalance(Display *_display,
                                   int display_vcc,
                                   int btn_tare,
                                   double _cal_standard_mass,
                                   String _units){
  // Assign configurable variables.
  display = _display;
  DISPLAY_VCC = display_vcc;
  BTN_TARE = btn_tare;
  cal_standard_mass = _cal_standard_mass;
  setUnits(_units);
}


void MOST_MassBalance::measureListenReportAtRate(double report_rate) {
  // Continuously update mass averaging window, slow down all else.
  // Initialize time tracker.
  static unsigned long lastRefresh = 1;
  
  // Keep data moving through the averaging filter as fast as possible. 
  double mass = getMassAveraged();
  
  // Enforce report rate without hampering sample rate (sample rate 
  // depends on how much processing is done between each call.
  if (millis() - lastRefresh > 1/report_rate * 1000) {
    // Reset the time.
    lastRefresh = millis();
    
    // Listen for input over serial.
    if (Serial.available() > 2) { // Minimum cmd length is 3
      doSerial();                 // characters <LF>c<CR>
    }
    
    if (isContinuousReport) {
      // Report to serial and update 'mass' so the display matches.
      mass = reportMassAveraged();
    }
    
    // Simple scale functionality. Note that placement of button
    // listener requires an extended button press. Assuming a report 
    // rate of at least 1 Hz, this should not be an issue.
    displayMass(mass);
    listenForButtonInput();
  }
}


//-----------------INITIALIZATION functions---------------------------//
void MOST_MassBalance::begin(){
  // Run all initialization functions.
  // Initialization serial output is the only non-standard output in 
  // this library.
  initSerial();
  initLoadCell();
  initDisplay();
  getSensitivity();
  initQueue();
  pinMode(BTN_TARE, INPUT_PULLUP);
  Serial.print(F("\nUse <LF>X?<CR> to view serial commands\r"));
}


void MOST_MassBalance::initSerial() {
  // Initialize the serial connection.
  Serial.begin(BAUD);
  
  while(!Serial) {
    // Wait for serial to initialize.
  } // Serial initialized.
}


void MOST_MassBalance::initLoadCell(int HX_VCC, int HX_DT, int HX_SCK) {
  // Turn on HX711 and read sensitivity from memory.
  Serial.print(F("\nInitializing HX711..."));
  
  // Turn on the HX711 power supply.
  pinMode(HX_VCC, OUTPUT);
  digitalWrite(HX_VCC, HIGH);
  
  // Give it time to power on.
  delay(500);
  
  // Initialize the HX711.
  loadcell.begin(HX_DT, HX_SCK);
  
  // Wait until it's ready.
  bool is_ready = false;
  int num_retries = 3;
  int wait_delay = 200;
  while (!is_ready) {
    // Give some indication that it's thinking.
    Serial.print(F("..."));
    is_ready = loadcell.wait_ready_retry(num_retries, wait_delay);
  }
  
  // Give the HX711 a chance to finish initializing.
  delay(2000);
  
  // Zero the scale (set the offset on data returned by the HX711).
  zeroSilent();
  
  Serial.print(F("HX711 Initialized!\r\n\r"));
}


void MOST_MassBalance::initDisplay(){
  display->init(DISPLAY_VCC);
}


// TODO: replace averaging queue with a low pass filter.
void MOST_MassBalance::initQueue() {
  // Set averaging window size and fill with zeros.
  hxQueue = new double[QUEUE_SIZE];
  for (int i = 0; i < QUEUE_SIZE; i++) {
    hxQueue[i] = 0.00;
  }
}
    

//-----------------ZERO Functions-------------------------------------//
void MOST_MassBalance::zero() {
  // Uses the HX711 built in tare() command to set the zero (empty bed).
  zeroSilent();
  reportMass();
}


void MOST_MassBalance::zeroSilent() {
  // Zero without serial response. Used for button-press and calibrate.
  loadcell.tare(HX_NUM_AVGS);
  clearTareSilent();
}


//-----------------TARE Functions-------------------------------------//
void MOST_MassBalance::tare() {
  // Update tareWeight to include current measured weight.
  tareSilent();
  // Report instantaneous mass.
  reportMass();
}


void MOST_MassBalance::tareSilent() {
  // Silently change the tare (no serial output).
  tareWeight += getMassAveraged();
}


void MOST_MassBalance::clearTare() {
  // Reset the tare.
  clearTareSilent();
  // Report instantaneous mass.
  reportMass();
}


void MOST_MassBalance::clearTareSilent() {
  // Silently reset the tare.
  tareWeight = 0.0;
}


//-----------------MASS Functions-------------------------------------//
// TODO: Does 'mass' need to be a global variable?
// TODO: Check for overload.
double MOST_MassBalance::getHxReadout() {
  // Read the raw (zeroed) value from the loadcell amplifier.
  return loadcell.get_value(HX_NUM_AVGS);
}


double MOST_MassBalance::getHxReadoutAveraged() {
  // Use an array to return an averaged raw reading from the HX711.
  static uint16_t queueIndex = 0;
  
  // Place the current mass (24-bit unscaled number) in the queue.
  hxQueue[queueIndex] = getHxReadout();
  
  // Increment the queue index.
  queueIndex++;
  if (queueIndex >= QUEUE_SIZE) {
    queueIndex = 0;
  }
  
  // Return the average value from the queue.
  double sum = 0;
  for (int i = 0; i < QUEUE_SIZE; i++) {
    sum += hxQueue[i];
  }
  
  return sum / QUEUE_SIZE;
}


double MOST_MassBalance::getMass() {
  // Read the instantaneous, tared, calibrated mass.
  return getHxReadout() / sensitivity - tareWeight;
}


double MOST_MassBalance::getMassAveraged() {
  // Read the averaged, tared, calibrated mass.
  return getHxReadoutAveraged() / sensitivity - tareWeight;
}


//-----------------INPUT Functions------------------------------------//
void MOST_MassBalance::listenForButtonInput() {
  // Check for a press of the tare button, allowing zero or calibration.
  switch (digitalRead(BTN_TARE)) {  // Is the button pressed?
    case 0: { // Yes. (Use brackets to prevent fall-through warnings)
      // Prepare to measure how long the button is held.
      unsigned long time_of_press = millis();
      // Give the user an indication of detection on the display.
      // TODO: Make these numbers mean something.
      display->print(".", 1, 15);
      
      while (digitalRead(BTN_TARE) == 0) {  // Button is pressed.
        // Wait for button release.
      } // Button is released.
      
      // Clear the '.' indicator once the button is released.
      display->print(" ", 1, 15);
      
      // Check when the button was released.
      unsigned long time_of_release = millis();
      
      // Behavior is determined by length of button press.
      if (time_of_release - time_of_press < CAL_WAIT) { 
        // Button was 'pressed', requesting a tare.
        // Use zero to simulate tare because there is no way to clear
        // tare with the button.
        zeroSilent();
      } else {
        // Button was 'held', requesting calibration.
        calibrate();
      }
      break; // End of button pressed response.
    }
    default: // No.
      // Buttons are active LOW; do nothing if button isn't pressed.
      break;
  } // End of button-press check.
}


//-----------------OUTPUT Functions-----------------------------------//
void MOST_MassBalance::reportTare() {
  // Report the tare weight over serial (response to 'M').
  reportSmaFormat(tareWeight, "T");
}


double MOST_MassBalance::reportMass() {
  // Report the instantaneous mass over serial ('T', 'Z', 'XC').
  double mass = getMass();
  reportSmaFormat(mass, getNetOrGross());
  return mass;
}


double MOST_MassBalance::reportMassAveraged() {
  // Report averaged/filtered mass over serial ('W' and 'R').
  double mass = getMassAveraged();
  reportSmaFormat(mass, getNetOrGross());
  return mass;
}


void MOST_MassBalance::reportSmaFormat(double _mass,
                                       String gross_status) {
  // Report a value over serial, adhering to SMA SCP 0499 format specs.
  // Check for scale status.
  String scale_status;
  if (_mass == 0.0) {
    scale_status = "Z";
    // TODO: add cases for over-weight, under-weight.
  } else {
    scale_status = " ";
  }
  
  // Size and string-ify the mass to report.
  String massStr = rightJustify(String(_mass, precision), WT_WIDTH);
  
  // SMA formatted response.
  String response = "\n";           // <LF>
  response += scale_status;         // <s>
  response += String(range);        // <r>
  response += gross_status;         // <n>
  response += " ";                  // <m>
  response += " ";                  // <f>
  response += massStr;              // <xxxxxx.xxx>
  response += units;                // <uuu>
  response += "\r";                 // <CR>
  Serial.print(response);
}


void MOST_MassBalance::displayMass(double _mass) {
  // Show the mass (whether it is instantaneous or averaged) on the LCD.
  String massStr = rightJustify(String(_mass, precision), WT_WIDTH);
  display->print(massStr + units, 0, 0);
}


void MOST_MassBalance::reportCalibrationMass() {
  // Report calibration mass over serial ('XC' and 'XCxxxxxxx.xx')
  reportSmaFormat(cal_standard_mass, "C");
}


//-----------------SENSITIVITY Functions------------------------------//
void MOST_MassBalance::getSensitivity() {
  // Fetch the stored sensitivity value from memory.
  Serial.print(F("\nReading sensitivity from memory..."));
  
  // The calibration value is stored with an indicator character - check
  // for it.
  char cal_check;
  EEPROM.get(CAL_SIGNATURE_ADDR, cal_check);
  
  if (cal_check != CAL_SIGNATURE) {
    // The expected character is not there.
    Serial.print(F("No sensitivity stored in memory.\r"));
  } else {
    // The expected character is there.
    EEPROM.get(CAL_VALUE_ADDR, sensitivity);
  }
  
  reportSensitivity();
}


void MOST_MassBalance::setSensitivity() {
  // Send the current sensitivity to memory silently.
  EEPROM.put(CAL_SIGNATURE_ADDR, CAL_SIGNATURE);
  EEPROM.put(CAL_VALUE_ADDR, sensitivity);
}


void MOST_MassBalance::reportSensitivity() {
  // Report sensitivity over serial (only done during startup).
  Serial.print("\nSensitivity: " + String(sensitivity, precision) + 
    " div/" + units + "\r\n\r");
}


void MOST_MassBalance::calibrate() {
  // Calibration sequence.
  // Tell the user what mass to use.
  display->print("Cal w/ " + String(cal_standard_mass, precision)
    + units, 0, 0);
    
  // Ensure the scale is zeroed.
  zeroSilent();
  
  // Wait for the mass to get added (no use averaging with no weight on
  // the scale).
  display->print("Add mass...", 1, 0);
  while (getHxReadout() < CAL_THRESHOLD) {
    // Wait for obvious addition of mass.
    if (Serial.available() > 2 || digitalRead(BTN_TARE) == 0) {
      // User interrupted via button or serial command - abort.
      display->clear();
      return;
    }
  } // Mass apparently added.
  
  // Allow the loadcell to settle.
  delay(1000);
  
  // Clear the averaging queue.
  initQueue();
  double hxReadout = 0;
  
  // Fill the averaging queue.
  for (int i = QUEUE_SIZE; i>0; i--) {
    // Add extra space to overwrite trailing digit when a place 
    // disappears (e.g. 10 --> 9, the 0 would be left on screen).
    display->print("Avg rem: " + String(i) + " ", 1, 0);
    hxReadout = getHxReadoutAveraged();
    delay(1000);
  }
  
  // Determine the new sensitivity.
  sensitivity = hxReadout / cal_standard_mass;
  
  // Save the sensitivity to hard memory for next time.
  setSensitivity();
  
  // Report an instantaneous mass so the user can see if the calibration
  // was successful.
  display->clear();
  reportMass();
}


void MOST_MassBalance::setUnits(String _units) {
  // Change the units string, enforcing right-justified 3-char wide.
  units = rightJustify(_units, 3);
}


//-----------------HELPER Functions-----------------------------------//
int MOST_MassBalance::findInArray(int *array,
                                  int query,
                                  int startSearch,
                                  int endSearch) {
  // Find a character in an integer array (used to find <CR> and <LF>).
  // Account for incrementing i at top of loop.
  int i = startSearch - 1;
  int c;  // Character being checked.
  do {
    // Increment i.
    i++;
    // Read a character from the array.
    c = array[i];
    
    // Don't go looking where there is nothing to be found.
    if (i > endSearch) {
      return -1;
    }
  } while (c != query);
  
  return i;
}


// TODO: Implement clipping.
String MOST_MassBalance::rightJustify(String str, int width) {
  // Set a string right justified within a window. Used for formatting
  // numbers to SMA specification on serial output.
  int numWhtSpc = width - str.length();
  for (int i = 0; i < numWhtSpc; i++) {
    str = " " + str;
  }
  
  return str;
}


String MOST_MassBalance::getNetOrGross() {
  // Return Net/Gross status. Net if tared, Gross if tare = 0.
  if (tareWeight == 0) {
    return "G";
  } else {
    return "N";
  }
}


void MOST_MassBalance::softReset() {
  // Reset the software. 
  // This seems to jump back to setup(), but not completely reset the
  // Arduino.
  asm volatile (" jmp 0");
}


//-----------------SERIAL Functions-----------------------------------//
void MOST_MassBalance::doSerial() {
  // Run through sequence of serial command interpretation.
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
  int escIdx = findInArray(cmd, ESC, 0, len);
  if (escIdx >= 0) {  // If there's an escape character, reset.
    softReset();
  }
  
  // Parse the command for the <LF> and <CR>. The command starts one
  // char beyond the LF, and ends with the CR.
  int startIdx = findInArray(cmd, LF, 0, len) + 1;
  int endIdx = findInArray(cmd, CR, startIdx, len);
  
  // Check for errors. Since we start after the LF, minimum index is 1. 
  // Note that findInArray returns -1 if it cannot find the character.
  if (startIdx < 1 || endIdx < 1) {
    Serial.println(F("\n?\r"));
    return;
  }
  
  // Execute the command.
  doCommand(cmd, startIdx, endIdx);
}


// Note that arrays are pointers, so just pass in the array's variable.
// TODO: make this accommodate commands that come character by character.
//       This can be accomplished by clearing command in when receiving
//       a LF, but requires preallocating potentially too much memory for a 
//       command. Can be accomplished by peeking for a <CR>. Could have
//       timing issues. Maybe watch the change in Serial.available().
void MOST_MassBalance::receiveCommand(int *cmd, int len) {  
  // Read everything but the last character into the cmd array.
  // The last character may be an LF, which would lead the next command.
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


void MOST_MassBalance::doCommand(int *cmd, int startIdx, int endIdx) {
  // Interpret the requested action and execute.
  // By the SMA standard, the first character indicates the function. 
  // The scale should not accept a command if it doesn't match the
  // syntax exactly. (e.g. <LF> wa <CR> should not execute the 
  // <LF> w <CR> command.)
  // To combat this, interpret commands by length.
  // First, cancel continuous reporting.
  isContinuousReport = 0;
  
  switch (endIdx - startIdx) {
    case 1:   // Single character commands.
      switch ((char) cmd[startIdx]) {
        case 'w': // Report weight.
        case 'W':
          reportMassAveraged();
          break;
          
        case 'z': // Zero request.
        case 'Z':
          zero();
          break;
          
        case 'd': // Run diagnostics.
        case 'D':
          // TODO: Implement actual diagnostics.
          Serial.print(F("\n    \r"));
          break;
          
        case 'a': // About (first row).
        case 'A':
          aboutIdx = 0;
          Serial.print(F("\nSMA:2/1.0\r"));
          break;
          
        case 'b': // aBout (scrolling).
        case 'B':
          switch (aboutIdx) {
            case 0:
              Serial.print(F("\nMFG:Michigan Tech MOST\r"));
              break;
            case 1:
              Serial.print(F("\nMOD:Digital Mass Balance\r"));
              break;
            case 2:
              Serial.print("\nREV:" + REV + "\r");
              break;
            case 3:
              Serial.print(F("\nEND:\r"));
              break;
            default:
              Serial.print(F("\n?\r"));
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
          tare();
          break;
          
        case 'c': // Clear tare.
        case 'C':
          clearTare();
          break;
          
        case 'm': // Return the current tare weight.
        case 'M':
          reportTare();
          break;
          
        default:  // Unknown command.
          Serial.print(F("\n?\r"));
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
              clearTareSilent();
              reportCalibrationMass();
              calibrate();
              break;
            
            case 'l': // Toggle LCD power.
            case 'L':
              // TODO: Implement Power toggle.
              break;
              
            case 'p': // Toggle output precision.
            case 'P':
              precision += 1;
              if (precision > 4) {
                precision = 0;
              }
              break;
              
            case '?': // Tell the user what commands are available.
              Serial.print(F("\nSMA SCP 0499 Serial Protocol\r"));
              Serial.print(F("\nAll cmds: <LF>cmd<CR>\r"));
              Serial.print(F("\nw           : averaged Weight\r"));
              Serial.print(F("\nz           : Zero scale\r"));
              Serial.print(F("\nd           : run Diagnostics\r"));
              Serial.print(F("\na           : About, first row\r"));
              Serial.print(F("\nb           : aBout, scroll\r"));
              Serial.print(F("\nr           : continuous Report\r"));
              Serial.print(F("\nt           : Tare scale\r"));
              Serial.print(F("\nc           : Clear tare\r"));
              Serial.print(F("\nm           : report tare weight\r"));
              Serial.print(F("\nxc          : enter Calibration mode\r"));
              Serial.print(F("\nxl          : toggle Lcd power\r"));
              Serial.print(F("\nxp          : scroll output Precision\r"));
              Serial.print(F("\nx?          : list all commands\r"));
              Serial.print(F("\nxc######.###: Calibrate with mass (needs 10 digits)\r"));
              // Give time for everything to send.
              Serial.flush();
              break;  
              
            default:  // Unrecognized custom command.
              Serial.print(F("\n?\r"));
              break;
          } // End of custom commands.
          break;

        default:  // Unrecognized 2 character command.
          Serial.print(F("\n?\r"));
          break;
      } // End of 2 character commands.          
      break;
      
    case 12:  // Command with numeric input.
      // Commands will be an x, character, then 10 character number
      // (with leading whitespace).
      switch ((char) cmd[startIdx]) {
        case 'x': // Custom command.
        case 'X':
          switch ((char) cmd[startIdx + 1]) {
            case 'c': // Calibrate request.
            case 'C': { // Brackets prevent fall-through warnings.
              // The calibration weight is submitted with 10 characters
              // of the value, plus 3 characters of units.
              // TODO: handle multiple options for units.
              // TODO: accept cal_std with or without units.
              clearTareSilent();
              
              // Read the requested calibration mass.
              String calStandard = "";
              for (int i = 0; i < 10; i++) {
                calStandard += String((char) cmd[startIdx + 2 + i]);
              }
              // BUG: there seems to be an overflow issue for large inputs.
              // BUG: toDouble() only returns two decimal points of 
              //      precision.
              cal_standard_mass = calStandard.toDouble();
              
              reportCalibrationMass();
              calibrate();
              break;
            }
            
            default:  // Unrecognized custom, numeric input command.
              Serial.print(F("\n?\r"));
          } // End of custom commands.
          break;
      } // End of 12 character commands.
      break;
      
    default:  // Unrecognized command length.
      Serial.print(F("\n?\r"));
      break;
  } // End of command interpretation.
}
