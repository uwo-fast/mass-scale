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
// Averaging window. 
// TODO: implement indexed queue to save time lost moving numbers around.
// Averaging window (applies only to calibrated value, not raw).
double hxQueue[QUEUE_SIZE];

// Response characteristics.
// Number of digits after the decimal.
int precision = 3;
// About index.
int aboutIdx = 4;
// Scale range to report (this scale is single-range).
int range = 1;


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


// TODO: replace averaging queue with a low pass filter.
void MOST_MassBalance::initQueue() {
  for (int i = 0; i < QUEUE_SIZE; i++) {
    hxQueue[i] = 0.00;
  }
}
    

// ZERO Functions //
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


// TARE Functions //
void MOST_MassBalance::tare() {
  // Sets the tareWeight to the current measured weight 
  // (accounting for current tare).
  tareSilent();
  // Report instantaneous mass.
  reportMass();
}


void MOST_MassBalance::tareSilent() {
  // Silently change the tare (no serial output).
  tareWeight += mass;
}


void MOST_MassBalance::clearTare() {
  // Reset the tare.
  tareWeight = 0.0;
  // Report instantaneous mass.
  reportMass();
}


void MOST_MassBalance::clearTareSilent() {
  // Silently reset the tare.
  tareWeight = 0.0;
}


// MASS Functions //
// TODO: Does 'mass' need to be a global variable?
// TODO: Check for overload.
double MOST_MassBalance::getHxReadout() {
  // Read the raw (zeroed) value from the loadcell amplifier.
  return loadcell.get_value(HX_NUM_AVGS);
}


double MOST_MassBalance::getHxReadoutAveraged() {
  // Uses an array to read an average reading from the HX711 (not scaled for
  // loadcell sensitivity).
  // Shift the queue.
  for (int i = QUEUE_SIZE - 1; i > 0; i--) {
    hxQueue[i] = hxQueue[i-1];
  }
  
  // Place the current mass (24-bit unscaled number) in the queue.
  hxQueue[0] = getHxReadout();
  
  // Return the average value from the queue.
  double sum = 0;
  for (int i = 0; i < QUEUE_SIZE; i++) {
    sum += hxQueue[i];
  }
  
  return sum / QUEUE_SIZE;
}


double MOST_MassBalance::getMass() {
  // Reads instantaneous, tared mass.
  return getHxReadout() / sensitivity - tareWeight;
}


double MOST_MassBalance::getMassAveraged() {
  // Reads averaged, tared mass.
  return getHxReadoutAveraged() / sensitivity - tareWeight;
}


// OUTPUT Functions //
void MOST_MassBalance::reportTare() {
  // Report the tare weight over serial (response to 'M').
  reportSmaFormat(tareWeight, "T");
}


void MOST_MassBalance::reportMass() {
  // Reports the instantaneous mass over serial. Used for 'T', 'Z', 'XC'.
  reportSmaFormat(getMass(), getNetOrGross());
}


void MOST_MassBalance::reportMassAveraged() {
  // Reports averaged/filtered mass over serial. Used for 'W' and 'R'
  reportSmaFormat(getMassAveraged(), getNetOrGross());
}


void MOST_MassBalance::reportSmaFormat(double _mass,
                                       String gross_status) {
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
int MOST_MassBalance::findInArray(int *array,
                                  int query,
                                  int startSearch,
                                  int endSearch) {
  // Finds a character in an integer array. Used to find <CR> and <LF>.
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