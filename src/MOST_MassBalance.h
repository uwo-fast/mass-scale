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
// Data handler.
#include "DataFilter.h"


class MOST_MassBalance {
  public:
    /**
     * Contstructor
     *
     * @param _display instance of a DisplayInterface child, used to
     *    manage the display output
     * @param display_vcc the p in used to apply power to the display
     * @param btn_tare the pin in pullup mode to measure tare button
     * @param _cal_standard_mass default mass used for calibration
     * @param _cal_standard_units default units for calibration mass
     */
    MOST_MassBalance(Display *_display=nullptr,
                     int display_vcc=5,
                     int btn_tare=8,
                     double _cal_standard_mass=100,
                     String _cal_standard_units="g");
    /// Free allocated memory.
    ~MOST_MassBalance();
    
    /**
     * Read data at top speed, read and write to Serial at fixed rate.
     *
     * @param report_rate rate in Hz at which to check the Serial buffer
     *    for incoming commands and send output to the display and 
     *    Serial output
     */
     
    void measureListenReportAtRate(double report_rate=1.0);
    
    /// Run a sequence of initializers for the mass balance.
    void begin();
    
    /**
     * Run getHxReadoutAveraged and scale it to mass using sensitivity.
     *
     * @returns the average mass computed by the data handler
     */
    double getMassAveraged();
    
  private:
    //-------------Values---------------------------------------------//
    const String REV = "3.0.1";
    
    // Configurable variables.
    Display *display;
    uint8_t DISPLAY_VCC;
    uint8_t BTN_TARE;
    // Calibration standard.
    double cal_standard_mass;
    // Calibration standard units.
    String cal_standard_units;
    
    
    // Response characteristics.
    // Number of digits after the decimal.
    uint8_t precision = 3;
    // About index.
    uint8_t aboutIdx = 4;
    // Scale range to report (this scale is single-range).
    uint8_t range = 1;
    
    
    // Non-configurable variables.
    // Baud rate defined by SMA SCP 0499.
    #define BAUD 9600

    // Signature to store in the memory when calibrating.
    #define CAL_SIGNATURE 'C'
    // Address of calibration signature.
    #define CAL_SIGNATURE_ADDR 0
    // Address of the stored calibration value.
    #define CAL_VALUE_ADDR CAL_SIGNATURE_ADDR + sizeof(char)
    // Address of units values.
    #define CAL_UNITS_ADDR CAL_VALUE_ADDR + sizeof(double)
    // Time (ms) that the push button is held to enter calibration mode.
    #define CAL_WAIT 3000
    // Threshold to begin calibration (to prevent premature measuring).
    #define CAL_THRESHOLD 20000
    
    // Number of averages completed by HX711 library.
    #define HX_NUM_AVGS 1
    // Number of averages completed internally.
    #define QUEUE_SIZE 10

    // Response block width for a weight report.
    #define WT_WIDTH 10
    // Response block width for a unit.
    #define UNIT_WIDTH 3
    
    
    // Internal Variables.
    HX711 loadcell;
    DataFilter *hxQueue;

    // Used as an offset from zero (ie for a container). Tare is done in
    // this script, while zero is implemented within the HX711 library.
    double tareWeight = 0.0;
    // Load cell sensitivity - will update from memory.
    double sensitivity = 1.0;
    // Measured mass units - will update from memory.
    String units = "  g";
    bool isContinuousReport = 0;
    bool isDisplayOn = 1;
    
    // Non-printable ASCII characters.
    #define LF 0x0A
    #define CR 0x0D
    #define ESC 0x1B
    #define SPACE 0x20
    
    
    //-------------Functions------------------------------------------//
    // Kept private because the serial API serves as the interface.
    // Initialization.
    /// Initialize serial.
    void initSerial();
    /** 
     * Initialize HX711
     *
     * @param HX_VCC pin used to provide 5V power to the amplifier
     * @param HX_DT pin connected to HX711 DT pin
     * @param HX_SCK pin connected to HX711 SCK pin
     */    
    void initLoadCell(int HX_VCC=4, int HX_DT=2, int HX_SCK=3);
    /// Call the display's initialization sequence.
    void initDisplay();
    /// Reset the data handler's queue to all zeros.
    void initQueue();
    
    /// Zero the HX711 object and report new mass over Serial.
    void zero();
    /// Zero the HX711 without any Serial output. 
    void zeroSilent();
    
    /// Run tareSilent and report new mass over Serial.
    void tare();
    /// Apply current mass readout to the locally managed tareWeight.
    void tareSilent();
    /// Run clearTareSilent and report the new mass.
    void clearTare();
    /// Reset tareWeight to zero .
    void clearTareSilent();
    
    // Mass.
    /**
     * Read the current measurement from the HX711.
     *
     * Does not interact with the data handler (the value is not added 
     * to the queue).
     *
     * @returns 24-bit readout from the HX711
     */
    double getHxReadout();
    /**
     * Apply the value returned by getHxReadout to the data handler.
     *
     * @returns the average value calculated by the data handler
     */
    double getHxReadoutAveraged();
    /**
     * Run getHxReadout and scale it to mass using sensitivity.
     *
     * @returns the instantaneous mass from the HX711
     */
    double getMass();
    
    // Input.
    /** 
     * Check for a button press on btn_tare and respond to it
     * 
     * Button presses initiate a tare action. Button holds initiate a
     * calibration sequence.
     */
    void listenForButtonInput();
    
    // Output.
    /// Report the current tareWeight over Serial.
    void reportTare();
    /** 
     * Report the instantaneous mass (getMass) over Serial.
     *
     * @returns the mass to allow the same value to be displayed
     */
    double reportMass();
    /**
     * Report the averaged mass (getMassAveraged) over Serial.
     *
     * @returns the mass to allow the same value to be displayed
     */
    double reportMassAveraged();
    /**
     * Format the Serial output to meet SMA SCP 0499 (standard).
     *
     * @param _mass the mass to be reported
     * @param _units the units to report
     * @param gross_status character to place in the gross/net space
     */
    void reportSmaFormat(double _mass,
                         String _units,
                         String gross_status);
    /**
     * Send the mass to the display.
     *
     * @param _mass the mass to be printed on the display
     */
    void displayMass(double _mass);
    /**
     * Print a string to the display, starting at a specific location.
     *
     * @param output the string to print
     * @param row the vertical position to start at, measured by 
     *    character (i.e. row 2 is shifted down the size of one mono-
     *    spaced character from row 1).
     * @param col the horizontal position to start at, measured in the
     *    same manner as row.
     */
    void printToDisplay(String output, int row, int col);
    /// Clear the display.
    void clearDisplay();
    /// Remove power from the display.
    void shutdownDisplay();
    /// Report the mass to be used for calibration over Serial.
    void reportCalibrationMass();
    
    // Sensitivity.
    /// Read the sensitivity from EEPROM (hard memory).
    void getSensitivity();
    /// Write the sensitivity to EEPROM (hard memory).
    void setSensitivity();
    /// Report the sensitivity over Serial.
    void reportSensitivity();
    /// Run through a series of steps to calibrate the load cell.
    void calibrate();
    /// Set the units used on the display and write them to EEPROM.
    void setUnits(String units);
    
    // Helpers.
    /**
     * Find a number or character within a range of memory.
     *
     * @param *array pointer to the array
     * @param query the character/number to look for
     * @param startSearch the index in the array to start at
     * @param endSearch the index in the array to quit looking
     * @returns the index of the query. -1 if not found.
     */
    int findInArray(int *array, int query, int startSearch, int endSearch);
    /**
     * Right justify a string within a window; clip if necessary.
     *
     * Clip favors the end of the string.
     *
     * @param str the string to right-justify
     * @param width the final length of the string
     * @returns the right-justified, clipped string
     */
    String rightJustify(String str, int width);
    /**
     * Check if the mass being reported is net or gross.
     *
     * @returns 'N' for Net if tareWeight is non-zero. 'G' for Gross if
     *    there is no tare weight
     */
    String getNetOrGross();
    /// Send the Arduino back to setup.
    void softReset();
    
    // Serial.
    /// Run the serial command receive/response sequence.
    void doSerial();
    /**
     * Read a command in from the Serial buffer.
     *
     * @param *cmd pointer to the array to fill with the command
     * @param len the length of the incoming command
     */
    void receiveCommand(int *cmd, int len);
    /**
     * Interpret the command and do as it asks.
     *
     * @param *cmd the array of Serial inputs
     * @param startIdx the beginning of the command within *cmd
     * @param endIdx the end of the command within *cmd
     */
    void doCommand(int *cmd, int startIdx, int endIdx);

};
    
    
#endif
