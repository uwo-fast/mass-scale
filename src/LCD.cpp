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

#include "LCD.h"


LCD::LCD(LiquidCrystal *_lcd, uint8_t _LCD_ROWS, uint8_t _LCD_COLS) {
  // Store objects provided by the user.
  // The display object is initialized externally (requires pins).
  lcd = _lcd;
  LCD_ROWS = _LCD_ROWS;
  LCD_COLS = _LCD_COLS;
}

LCD::~LCD() {
  // Remove the pointer from memory.
  delete lcd;
}


void LCD::init(uint8_t vccPin) {
  // Run all the necessary startup for the LCD.  
  Serial.print(F("\nInitializing LCD..."));
  
  // Turn on the LCD power supply.
  pinMode(vccPin, OUTPUT);
  digitalWrite(vccPin, HIGH);
  
  // Give it time to power on.
  delay(500);
  
  // Initialize the display.
  lcd->begin(LCD_COLS, LCD_ROWS);
  // Clear the display.
  lcd->clear();
  // Home the cursor.
  lcd->home();
  // Hide the cursor.
  lcd->noCursor();
  // Ensure the display is on. (lcd.noDisplay() turns off the display).
  lcd->display();
  
  // Test the display.
  Serial.print(F("Testing the display..."));
  // Print an 8 to each character in the display.
  for (uint8_t i = 0; i < LCD_ROWS * LCD_COLS; i++) {
    lcd->print("8");
    if (i == LCD_COLS - 1) {
      lcd->setCursor(0, 1);
    }
  }
  delay(200);
  lcd->clear();
  
  Serial.print(F("LCD initialized!\r\n\r"));
}


void LCD::shutdown(uint8_t vccPin) {
  lcd->noDisplay();
  digitalWrite(vccPin, LOW);
}


void LCD::print(String output, uint8_t row, uint8_t col) {
  // Sends a formatted string to the LCD, starting at the requested 
  // location.
  lcd->setCursor(col, row);
  lcd->print(output);
}


void LCD::clear() {
  // Remove all information from the LCD.
  lcd->clear();
}
