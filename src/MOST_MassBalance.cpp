/*  MOST_MassBalance is a library of drivers for a digital mass balance
      
      This firmware is designed to meet SMA SCP 0499 Level #2 for scale
      serial communication. The command and response formats for serial 
      communication are documented in included files.
      
      The scale was designed by researchers in Michigan Technological 
      University's MOST group <https://www.appropedia.org/Category:MOST>
      
      REVISIONS:
      1.0.0 : Initial release - function scale with serial reporting.
      2.0.0 : First release up to SMA standards. Work to do on data
              filtering.      
      3.0.0 : Refactor all functionality into a library.
      3.0.1 : Move data handling into a separate class.
      3.1.0 : Add OLED interface.
      3.1.1 : Require installed libraries for displays.
      3.1.2 : Make getMass() public.

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



MOST_MassBalance::MOST_MassBalance(Display *_display,
                                   int display_vcc,
                                   int btn_tare,
                                   double _cal_standard_mass,
                                   String _cal_standard_units){
  // Assign configurable variables.
  display = _display;
  DISPLAY_VCC = display_vcc;
  BTN_TARE = btn_tare;
  cal_standard_mass = _cal_standard_mass;
  cal_standard_units = ArrayHelper::rightJustify(_cal_standard_units,
                                                 UNIT_WIDTH);
  hxQueue = new DataFilter(QUEUE_SIZE);
}


MOST_MassBalance::~MOST_MassBalance() {
  delete hxQueue;
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
  if (display) {
    initDisplay();
  }
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
  if (display && !isDisplayOn) { // display is not a NULL pointer.
    display->init(DISPLAY_VCC);
    isDisplayOn = 1;
  }
}


void MOST_MassBalance::initQueue() {
  // Set averaging window size and fill with zeros.
  hxQueue->fill(0);
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
double MOST_MassBalance::getHxReadout() {
  // Read the raw (zeroed) value from the loadcell amplifier.
  return loadcell.get_value(HX_NUM_AVGS);
}


double MOST_MassBalance::getHxReadoutAveraged() {
  // Interface with the data handler and return an average raw value.
  // Place the current mass (24-bit unscaled number) in the queue.
  hxQueue->push(getHxReadout());
  
  return hxQueue->getAverage();
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
      printToDisplay(".", 1, 15);
      
      while (digitalRead(BTN_TARE) == 0) {  // Button is pressed.
        // Wait for button release.
      } // Button is released.
      
      // Clear the '.' indicator once the button is released.
      printToDisplay(" ", 1, 15);
      
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
  reportSmaFormat(tareWeight, units, "T");
}


double MOST_MassBalance::reportMass() {
  // Report the instantaneous mass over serial ('T', 'Z', 'XC').
  double mass = getMass();
  reportSmaFormat(mass, units, getNetOrGross());
  return mass;
}


double MOST_MassBalance::reportMassAveraged() {
  // Report averaged/filtered mass over serial ('W' and 'R').
  double mass = getMassAveraged();
  reportSmaFormat(mass, units, getNetOrGross());
  return mass;
}


void MOST_MassBalance::reportSmaFormat(double _mass,
                                       String _units,
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
  String massStr = ArrayHelper::rightJustify(String(_mass, precision),
                                             WT_WIDTH);
  
  // SMA formatted response.
  String response = "\n";           // <LF>
  response += scale_status;         // <s>
  response += String(range);        // <r>
  response += gross_status;         // <n>
  response += " ";                  // <m>
  response += " ";                  // <f>
  response += massStr;              // <xxxxxx.xxx>
  response += _units;               // <uuu>
  response += "\r";                 // <CR>
  Serial.print(response);
}


void MOST_MassBalance::displayMass(double _mass) {
  // Show the mass (whether it is instantaneous or averaged) on the LCD
  String massStr = ArrayHelper::rightJustify(String(_mass, precision),
                                             WT_WIDTH);
  printToDisplay(massStr + units, 0, 0);
}


void MOST_MassBalance::printToDisplay(String output, int row, int col) {
  // Print to the display (if there is one).
  if (display && isDisplayOn) { // display is not a NULL pointer.
    display->print(output, row, col);
  }
}


void MOST_MassBalance::clearDisplay() {
  // Clear the display (if one exists).
  if (display && isDisplayOn) { // display is not a NULL pointer.
    display->clear();
  }
}


void MOST_MassBalance::shutdownDisplay() {
  // Turn the display off to save power (if there is a display).
  if (display && isDisplayOn) { // display is not a NULL pointer.
    display->shutdown(DISPLAY_VCC);
    isDisplayOn = 0;
  }
}


void MOST_MassBalance::reportCalibrationMass() {
  // Report calibration mass over serial ('XC' and 'XCxxxxxxx.xx')
  reportSmaFormat(cal_standard_mass, cal_standard_units, "C");
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
    for (int i = 0; i < UNIT_WIDTH; i++) {
      EEPROM.get(CAL_UNITS_ADDR + sizeof(char)*i, units[i]);
    }
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
  printToDisplay("Cal w/ " + String(cal_standard_mass, precision)
    + cal_standard_units, 0, 0);
    
  // Ensure the scale is zeroed.
  clearTareSilent();
  zeroSilent();
  
  // Wait for the mass to get added (no use averaging with no weight on
  // the scale).
  printToDisplay("Add mass...", 1, 0);
  while (getHxReadout() < CAL_THRESHOLD) {
    // Wait for obvious addition of mass.
    if (Serial.available() > 2 || digitalRead(BTN_TARE) == 0) {
      // User interrupted via button or serial command - abort.
      clearDisplay();
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
    printToDisplay("Avg rem: " + String(i) + " ", 1, 0);
    hxReadout = getHxReadoutAveraged();
    delay(1000);
  }
  
  // Determine the new sensitivity.
  sensitivity = hxReadout / cal_standard_mass;
  
  // Save the sensitivity to hard memory for next time.
  setSensitivity();
  
  // Overwrite units with the calibration standard units.
  setUnits(cal_standard_units);
  
  // Report an instantaneous mass so the user can see if the calibration
  // was successful.
  clearDisplay();
  reportMass();
}


void MOST_MassBalance::setUnits(String _units) {
  // Change the units string, enforcing right-justified 3-char wide.
  units = ArrayHelper::rightJustify(_units, UNIT_WIDTH);
  for (int i = 0; i < UNIT_WIDTH; i++) {
    EEPROM.put(CAL_UNITS_ADDR + sizeof(char)*i, units[i]);
  }
}


//-----------------HELPER Functions-----------------------------------//
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
  int escIdx = ArrayHelper::findInArray(cmd, ESC, 0, len);
  if (escIdx >= 0) {  // If there's an escape character, reset.
    softReset();
  }
  
  // Parse the command for the <LF> and <CR>. The command starts one
  // char beyond the LF, and ends with the CR.
  int startIdx = ArrayHelper::findInArray(cmd, LF, 0, len) + 1;
  int endIdx = ArrayHelper::findInArray(cmd, CR, startIdx, len);
  
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
  int cmdLength = endIdx - startIdx;
  
  switch (cmdLength) {
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
              reportCalibrationMass();
              calibrate();
              break;
            
            case 'l': // Toggle LCD power.
            case 'L':
              if (isDisplayOn) {
                shutdownDisplay();
              } else {
                initDisplay();
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
              Serial.print(F("\nxc######.###uuu : Calibrate with with mass and new units (needs 3 characters)\r"));
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
      
    case 12:  // Command with numeric input (10 digits).
    case 15:  // Command with numeric input and units (3 add'l digits).
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
              // Read the requested calibration mass.
              String calStandard = "";
              for (int i = 0; i < WT_WIDTH; i++) {
                calStandard += String((char) cmd[startIdx + 2 + i]);
              }
              
              // Read the units, if provided.
              String _units = "";
              for (int i = 0; i < cmdLength-WT_WIDTH-2; i++) {
                _units += String((char) cmd[startIdx + 2 + WT_WIDTH + i]);
              }
              
              if (_units != "") {
                cal_standard_units = ArrayHelper::rightJustify(_units, 3);
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
      } // End of 12/15 character commands.
      break;
    // TODO: Toggle units and apply scaling factors.
    default:  // Unrecognized command length.
      Serial.print(F("\n?\r"));
      break;
  } // End of command interpretation.
}
