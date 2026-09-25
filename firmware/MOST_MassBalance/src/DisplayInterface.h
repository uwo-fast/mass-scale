/*  Display is an abstract interface class supporting MOST_MassBalance.
      Defines the methods that MOST_MassBalance requires access to for
      use of a display. Displays may contain other methods for added
      functionality external to MOST_MassBalance, but MOST_MassBalance
      can only access methods defined in this interface.

      Since extended functionality is allowed, the Display is
      initialized by the caller and provided to MOST_MassBalance.

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

#ifndef DISPLAY_INTERFACE_h
#define DISPLAY_INTERFACE_h


#include <Arduino.h>


class Display {
  public:
    /// Free allocated memory.
    virtual ~Display();

    /**
     * Turn on the display and initialize its driver.
     *
     * Expected to be used only once.
     *
     * @param vccPin the pin used to power the display
     */
    virtual void init(uint8_t _vccPin);

    /// Initialize using a known vccPin.
    virtual void init();

    /**
     * (Deprecated - use without input) Turn off the display.
     *
     * @param vccPin the pin used to power the display
     */
    virtual void shutdown(uint8_t _vccPin);

    /// Shutdown using a known vccPin.
    virtual void shutdown();

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
    virtual void print(String output, uint8_t row, uint8_t col);

    /// Clear the display.
    virtual void clear();

    /**
     * Set the vccPin.
     *
     * Implemented for backwards compatibility of MOST_MassBalance.
     *
     * @param _vccPin the pin used to power the display
     */
    virtual void setVccPin(uint8_t _vccPin);

  protected:
    uint8_t vccPin;
};


#endif
