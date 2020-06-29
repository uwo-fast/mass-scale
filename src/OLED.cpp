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

#include "OLED.h"


OLED::OLED(Adafruit_SSD1306 *_oled,
           uint8_t _OLED_WIDTH,
           uint8_t _OLED_HEIGHT,
           uint8_t _OLED_ADDR) {
  // Store objects provided by the user.
  // The display object is initialized externally (requires pins).
  oled = _oled;
  OLED_WIDTH = _OLED_WIDTH;
  OLED_HEIGHT = _OLED_HEIGHT;
  OLED_ADDR = _OLED_ADDR;
}

OLED::~OLED() {
  // Remove the pointer from memory.
  Serial.println("Deleting OLED");
  // delete oled;
}


void OLED::init(uint8_t vccPin) {
  // Runs all the necessary startup for the OLED.
  Serial.print(F("\nInitializing OLED..."));
  
  // Turn on the OLED power supply.
  pinMode(vccPin, OUTPUT);
  digitalWrite(vccPin, HIGH);
  
  // Give it time to power on.
  delay(500);
  
  // Initialize the display.
  while(!oled->begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)){
    Serial.print(F("..."));
  }
  
  // Splash screen.
  Serial.print(F("Testing the display..."));
  oled->display();
  delay(2000);
  // Clear buffer.
  clear();
  
  Serial.print(F("OLED initialized!\r\n\r"));
}


void OLED::shutdown(uint8_t vccPin) {
  //oled->noDisplay();
  digitalWrite(vccPin, LOW);
}


void OLED::print(String output, uint8_t row, uint8_t col) {
  // Send a formatted string to the OLED, starting at the requested 
  // location.
  oled->setTextSize(1);
  // Use built-in overtyping by setting the background to black.
  // This only works for the built-in font.
  oled->setTextColor(SSD1306_WHITE, SSD1306_BLACK);
  oled->setCursor(col * OLED_CHAR_WIDTH, row * OLED_CHAR_HEIGHT);
  oled->print(output);
  oled->display();
}


void OLED::clear() {
  // Remove all information from the OLED.
  oled->clearDisplay();
  oled->display();
}
