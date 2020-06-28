/* Config
    Defines various settings for DigitalMassBalance.
*/


#pragma once
//----LCD----//
  // Display size.
  const int LCD_ROWS = 2;
  const int LCD_COLS = 16;
  
 
//----Serial----//
  // Communication rate.
  const int REPORT_RATE = 1; // Hz
  unsigned long lastRefresh = 1;  // milliseconds
  
  // Continuous output to serial.
  bool isContinuousReport = 0;
  
  