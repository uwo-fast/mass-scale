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
    virtual ~Display();
    virtual void init(int vccPin);
    virtual void shutdown(int vccPin);
    virtual void print(String output, int row, int col);  
    virtual void clear();
};


#endif
