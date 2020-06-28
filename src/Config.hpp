/* Config
    Defines various settings for DigitalMassBalance.
*/


#pragma once


//-----HX711----//
  // TODO: Remove this variable after transfer.
  extern const int HX_NUM_AVGS;
  extern const int QUEUE_SIZE;


//----LCD----//
  // Is there an LCD?
  bool isLCD = true;

  // Display size.
  const int LCD_ROWS = 2;
  const int LCD_COLS = 16;


//----Calibration/Sensitivity----//
  // Mass used to calibrate (default: 1 US cup of water);
  double cal_standard_mass = 235.9;

  // Mass units.
  String units = "  g"; 
  
 
//----Serial----//
  // Communication rate.
  const int REPORT_RATE = 1; // Hz
  unsigned long lastRefresh = 1;  // milliseconds
  
  // Continuous output to serial.
  bool isContinuousReport = 0;
  
  