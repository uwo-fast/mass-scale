/*  OLED is a supporting class for MOST_MassBalance.
      It acts as an interface with Adafruits graphics library,
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
    /**
     * Constructor
     *
     * @param *_oled pointer to the object driving the display.
     * @param _OLED_WIDTH, _OLED_HEIGHT number of rows of characters
     *    (used by print)
     * @param _OLED_ADDR address for I2C communication.
     * @param vccPin specifies the pin used to power the display.
     */
    OLED(Adafruit_SSD1306 *_oled,
         uint8_t _OLED_WIDTH=128,
         uint8_t _OLED_HEIGHT=64,
         uint8_t _OLED_ADDR=0x3C,
         uint8_t vccPin=-1);
        
    /// Free Allocated memory.
    ~OLED();
    
    /**
     * Turn on the display and initialize its driver.
     *
     * @param vccPin the pin used to power the display
     */
    void init(uint8_t _vccPin) override;
        
    /// Initialize using a known vccPin.
    virtual void init() override;
    
    /**
     * Turn off the display.
     *
     * @param vccPin the pin used to power the display
     */
    void shutdown(uint8_t _vccPin) override;
        
    /// Shutdown using a known vccPin.
    virtual void shutdown() override;
    
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
    void print(String output, uint8_t row, uint8_t col) override;
    
    /// Clear the display.
    void clear() override;
    
    /**
     * Set the vccPin.
     *
     * Implemented for backwards compatibility of MOST_MassBalance.
     *
     * @param _vccPin the pin used to power the display
     */
    void setVccPin(uint8_t _vccPin) override;
    
  private:
    Adafruit_SSD1306 *oled;
    uint8_t OLED_WIDTH;
    uint8_t OLED_HEIGHT;
    uint8_t OLED_ADDR;
    
    #define OLED_CHAR_WIDTH 6
    #define OLED_CHAR_HEIGHT 8
};


#endif
