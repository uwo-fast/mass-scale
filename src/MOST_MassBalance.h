/*  MOST_MassBalance is a library of drivers for a digital mass balance
      
      This firmware is designed to meet SMA SCP 0499 Level #2 for scale
      serial communication. The command and response formats for serial communication are documented in included files.
      
      The scale was designed by researchers ing Michigan Technological 
      University's MOST group <https://www.appropedia.org/Category:MOST>
      
      REVISIONS:
      
      
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
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef MOST_MASS_BALANCE_h
#define MOST_MASS_BALANCE_h


// Main Arduino library needs to be explicitly included for libraries.
#include <Arduino.h> 
// Hard memory read/write. 
#include <EEPROM.h>
// Load cell amplifier.
#include "HX711/src/HX711.h"
// Display.
#include "DisplayInterface.h"


// TODO: Move these into class scope after transfer (restrict external access).
// Configurable variables (defined in external code).
// Number of digits after the decimal.
extern int precision;
// Unit of mass.
extern String units;
// Calibration standard.
extern double cal_standard_mass;

// Display VCC pin.
extern int displayVccPin;


// Non-configurable variables.
// Baud rate defined by SMA SCP 0499.
const double BAUD = 9600;

// Signature to store in the memory when calibrating.
const char CAL_SIGNATURE = 'C';
// Address of calibration signature.
const int CAL_SIGNATURE_ADDR = 0;
// Address of the stored calibration value.
const int CAL_VALUE_ADDR = CAL_SIGNATURE_ADDR + sizeof(char);
// Time (ms) that the push button is held to enter calibration mode.
const int CAL_WAIT = 3000;

// Number of averages completed by HX711 library.
const int HX_NUM_AVGS = 1;
// Number of averages completed internally.
const int QUEUE_SIZE = 10;

// Response block width for a weight report.
const int WT_WIDTH = 10;


// Internal Variables.
extern HX711 loadcell;

extern double mass;
extern double tareWeight;
extern double sensitivity;
extern double hxQueue[QUEUE_SIZE];

// Response characteristics.
extern int precision;
extern int aboutIdx;
extern int range;


class MOST_MassBalance {
  public:
    MOST_MassBalance(Display *_display=NULL,
                     int display_vcc=5,
                     int btn_tare=8);
    void initSerial();
    void initLoadCell(int HX_VCC=4, int HX_DT=2, int HX_SCK=3);
    void initDisplay();
    void initQueue();
    
    // Zero.
    void zero();
    void zeroSilent();
    
    // Tare.
    void tare();
    void tareSilent();
    void clearTare();
    void clearTareSilent();
    
    // Mass.
    double getHxReadout();
    double getHxReadoutAveraged();
    double getMass();
    double getMassAveraged();
    
    // Input.
    void listenForButtonInput();
    
    // Output.
    void reportTare();
    void reportMass();
    void reportMassAveraged();
    void reportSmaFormat(double _mass, String gross_status);
    void displayMass(double _mass);
    void reportCalibrationMass();
    
    // Sensitivity.
    void getSensitivity();
    void setSensitivity();
    void reportSensitivity();
    void calibrate();
    
    // Helpers.
    int findInArray(int *array, int query, int startSearch, int endSearch);
    String rightJustify(String str, int width);
    String getNetOrGross();
    void softReset();
    
  private:
    Display *display;
    int DISPLAY_VCC;
    int BTN_TARE;
    // Threshold to begin calibration (to prevent premature measuring).
    const double CAL_THRESHOLD = 20000;

};
    
    
#endif
