/*  DataFilter is a supporting class for MOST_MassBalance.
      It facilitates the management of data to be filtered, whether by
      averaging, a low-pass filter, or some other method.
      
      This code was designed by researchers in Michigan Technological 
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

#include "DataFilter.h"


DataFilter::DataFilter(uint8_t _n_taps) {
  // Create the data queue.
  n_taps = _n_taps;
  data_queue = new double[n_taps];
  // Set to zeros.
  fill(0);
}


DataFilter::~DataFilter() {
  // Delete allocated memory.
  delete data_queue;
}


void DataFilter::push(double val) {
  // Replace the oldest value with the newest value.
  data_queue[queue_idx] = val;
  queue_idx++;
  if (queue_idx >= n_taps) {
    queue_idx = 0;
  }
}


double DataFilter::getAverage() {
  double sum = 0;
  Serial.print("[");
  for (uint8_t i=0; i < n_taps; i++) {
    sum += data_queue[i];
    Serial.print(data_queue[i]);
    Serial.print(", ");
  }
  Serial.println("]");
  return sum / n_taps;
}


void DataFilter::fill(double val) {
  for (uint8_t i=0; i < n_taps; i++) {
    data_queue[i] = val;
  }
}
