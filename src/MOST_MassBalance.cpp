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

#include "MOST_MassBalance.h"


HX711 loadcell;

// Measured mass (can be assigned an averaged/filtered value or an 
// instantaneous value.
double mass;
// Used as an offset from zero (ie for a container). Implemented in this
// script, while zero is implemented within the HX711 library.
double tareWeight = 0.0;
// Sensitivity is read from memory - this is here as a default.
double sensitivity = 1.0;


// INITIALIZATION functions //
void MOST_MassBalance::initSerial() {
  // Initialize the serial connection.
  Serial.begin(BAUD);
  
  while(!Serial) {
    // Wait for serial to initialize.
  } // Serial initialized.
}


void MOST_MassBalance::initLoadCell(int HX_VCC, int HX_DT, int HX_SCK) {
  // Set up the HX711 for use. Turns on the device, then verifies the 
  // calibration value.
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


// ZERO Functions //
void MOST_MassBalance::zeroSilent() {
  // Zero without serial response. Used for button-press and calibrate.
  loadcell.tare(HX_NUM_AVGS);
  clearTareSilent();
}


// TARE Functions //
void MOST_MassBalance::tareSilent() {
  // Silently change the tare (no serial output).
  tareWeight += mass;
}


void MOST_MassBalance::clearTareSilent() {
  // Silently reset the tare.
  tareWeight = 0.0;
}


// SENSITIVITY Functions //
void MOST_MassBalance::getSensitivity() {
  // Fetch the stored sensitivity value from memory.
  Serial.print(F("\nReading sensitivity from memory..."));
  
  char cal_check;
  EEPROM.get(CAL_SIGNATURE_ADDR, cal_check);
  
  if (cal_check != CAL_SIGNATURE) {
    Serial.print(F("No sensitivity stored in memory.\r"));
  } else {
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
  Serial.print("\nSensitivity: " + String(sensitivity, precision) + 
    " div/" + units + "\r\n\r");
}


// HELPER Functions //
String MOST_MassBalance::rightJustify(String str, int width) {
  // Sets a string right justified within a window. Used for formatting
  // numbers to SMA specification on serial output.
  int numWhtSpc = width - str.length();
  for (int i = 0; i < numWhtSpc; i++) {
    str = " " + str;
  }
  
  return str;
}


String MOST_MassBalance::getNetOrGross() {
  // Net/Gross status. Net if tared, Gross if tare = 0.
  if (tareWeight == 0) {
    return "G";
  } else {
    return "N";
  }
}


void MOST_MassBalance::softReset() {
  // Reset the software. This seems to jump back to setup(), but not 
  // completely reset the Arduino.
  asm volatile (" jmp 0");
}