/*  LCD is a supporting class for MOST_MassBalance.
      It acts as an interface with Liquid Crystal Displays by 
      implementing the abstract class Display, defined in 
      DisplayInterface.h.
      
      The scale was designed by researchers in Michigan Technological 
      University's MOST group <https://www.appropedia.org/Category:MOST>
      
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

#ifndef LCD_h
#define LCD_h


#include <Arduino.h>
#include "DisplayInterface.h"
#include "LiquidCrystal\src\LiquidCrystal.h"


class LCD : virtual public Display {
  public:
    LiquidCrystal *lcd;
    LCD(LiquidCrystal *_lcd,
        uint8_t _LCD_ROWS=2,
        uint8_t _LCD_COLS=16);
    ~LCD();
    void init(uint8_t vccPin);
    void shutdown(uint8_t vccPin);
    void print(String output, uint8_t row, uint8_t col);  
    void clear();
    
  private:
    uint8_t LCD_ROWS;
    uint8_t LCD_COLS;
};


#endif
