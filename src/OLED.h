/*  LCD is a supporting class for MOST_MassBalance.
      It acts as an interface with Liquid Crystal Displays by 
      implementing the abstract class Display, defined in 
      DisplayInterface.h.
      
      Dependencies: Install Adafruit SSD1306 (and its dependencies,
                    GFX-Library and Bus_IO) using the Arduino library
                    manager.
      
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

#ifndef OLED_h
#define OLED_h


#include <Arduino.h>
#include "DisplayInterface.h"
#include <Adafruit_SSD1306.h>


class OLED : virtual public Display {
  public:
    Adafruit_SSD1306 *oled;
    OLED(Adafruit_SSD1306 *_oled,
         uint8_t _OLED_WIDTH=128,
         uint8_t _OLED_HEIGHT=64,
         uint8_t _OLED_ADDR=0x3C);
    ~OLED();
    void init(uint8_t vccPin);
    void shutdown(uint8_t vccPin);
    void print(String output, uint8_t row, uint8_t col);  
    void clear();
    
  private:
    uint8_t OLED_WIDTH;
    uint8_t OLED_HEIGHT;
    uint8_t OLED_ADDR;
    
    #define OLED_CHAR_WIDTH 6
    #define OLED_CHAR_HEIGHT 8
};


#endif
