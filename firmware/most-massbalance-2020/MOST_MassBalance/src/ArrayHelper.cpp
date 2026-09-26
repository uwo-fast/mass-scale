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

#include "ArrayHelper.h"


int ArrayHelper::findInArray(int *array,
                             int query,
                             int startSearch,
                             int endSearch) {
  // Find a character in an integer array (used to find <CR> and <LF>).
  // Account for incrementing i at top of loop.
  int i = startSearch - 1;
  int c;  // Character being checked.
  do {
    // Increment i.
    i++;
    // Read a character from the array.
    c = array[i];

    // Don't go looking where there is nothing to be found.
    if (i > endSearch) {
      return -1;
    }
  } while (c != query);

  return i;
}


String ArrayHelper::rightJustify(String str, int width) {
  // Set a string right justified within a window. Used for formatting
  // numbers to SMA specification on serial output.
  int numWhtSpc = width - str.length();
  if (numWhtSpc < 0) {  // The string is longer than the width.
    // Return the very end of the string.
    return str.substring(-numWhtSpc);
  }
  for (int i = 0; i < numWhtSpc; i++) {
    str = " " + str;
  }

  return str;
}
