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
    /**
     * Constructor
     *
     * @param *_lcd pointer to the object driving the display.
     * @param _LCD_ROWS, _LCD_COLS number of rows of characters (used by
     * print)
     */
    LCD(LiquidCrystal *_lcd,
        uint8_t _LCD_ROWS=2,
        uint8_t _LCD_COLS=16);
    /// Free Allocated memory.
    ~LCD();
    /**
     * Turn on the display and initialize its driver.
     *
     * @param vccPin the pin used to power the display
     */
    void init(uint8_t vccPin);
    /**
     * Turn off the display.
     *
     * @param vccPin the pin used to power the display
     */
    void shutdown(uint8_t vccPin);
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
    void print(String output, uint8_t row, uint8_t col);  
    /// Clear the display.
    void clear();
    
  private:
    LiquidCrystal *lcd;
    uint8_t LCD_ROWS;
    uint8_t LCD_COLS;
};


#endif
