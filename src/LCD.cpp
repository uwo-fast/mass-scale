/*  DigitalMassBalance uses a load cell to measure and report an object's mass.
      This firmware is designed to meet SMA SCP 0499 Level #2 for scale serial 
      communication. The command and response formats for serial communication 
      are documented in included files.
      
      The scale was designed by researchers ing Michigan Technological 
      University's MOST group <https://www.appropedia.org/Category:MOST>
      
      REVISIONS:
      1.0.0 : Initial release - function scale with serial reporting.
      2.0.0 : First release up to SMA standards. Work to do on data filtering.
      
    Copyright (C) 2020 Benjamin Hubbard
      ! Note that external libraries included with this software are subject to
        their own licenses, included within their respective folders.
        
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "LCD.h"


LCD::LCD(LiquidCrystal *_lcd, int _LCD_ROWS, int _LCD_COLS) {
  // The display object is initialized externally (requires pins).
  lcd = _lcd;
  LCD_ROWS = _LCD_ROWS;
  LCD_COLS = _LCD_COLS;
}

LCD::~LCD() {
  // Remove the pointer from memory.
  delete lcd;
}


void LCD::init(int vccPin) {
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
  for (int i = 0; i < LCD_ROWS * LCD_COLS; i++) {
    lcd->print("8");
    if (i == LCD_COLS - 1) {
      lcd->setCursor(0, 1);
    }
  }
  delay(200);
  lcd->clear();
  
  Serial.print(F("LCD initialized!\r\n\r"));
}


void LCD::print(String output, int row, int col) {
  // Sends a formatted string to the LCD, starting at the requested 
  // location.
  lcd->setCursor(col, row);
  lcd->print(output);
}


void LCD::clear() {
  // Remove all information from the LCD.
  lcd->clear();
}
