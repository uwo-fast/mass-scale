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


#include <Arduino.h>


// Configurable variables (defined in external code).
extern double mass;
extern double tareWeight;


// Non-configurable variables.
// Baud rate defined by SMA SCP 0499.
const double BAUD = 9600;


class MOST_MassBalance {
  public:
    void initSerial();
    //void initLoadCell();
    
    // Tare.
    void tareSilent();
    void clearTareSilent();
    
    // Helpers.
    String rightJustify(String str, int width);
    void softReset();

};
    
    
#endif
