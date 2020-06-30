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

#ifndef DATA_FILTER_h
#define DATA_FILTER_h


#include <Arduino.h>


class DataFilter {
  public:
    DataFilter(uint8_t _n_taps=10);
    ~DataFilter();
    void push(double val);
    double getAverage();
    void fill(double val=0);
  private:
    double *data_queue;
    uint8_t n_taps;
    uint8_t queue_idx;
};


#endif
