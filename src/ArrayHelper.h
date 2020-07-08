/*  ArrayHelper is a library of functions that interact with arrays.

      Specifically designed for use by the MOST_MassBalance library.
      
      The software was designed by researchers in Michigan Technological 
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

#ifndef ARRAY_HELPER_h
#define ARRAY_HELPER_h

// Main Arduino library needs to be explicitly included for libraries.
#include <Arduino.h> 


namespace ArrayHelper {
    /**
     * Find a number or character within a range of memory.
     *
     * @param *array pointer to the array
     * @param query the character/number to look for
     * @param startSearch the index in the array to start at
     * @param endSearch the index in the array to quit looking
     * @returns the index of the query. -1 if not found.
     */
    int findInArray(int *array, 
                           int query,
                           int startSearch,
                           int endSearch);
    
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
};
    
    
#endif
