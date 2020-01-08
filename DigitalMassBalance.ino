/*  DigitalMassBalance uses a load cell to measure and report an object's mass.
    Copyright (C) 2020 Benjamin Hubbard

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


#include "src\Libraries.hpp"
#include "src\Pinouts.hpp"
#include "src\Config.hpp" 


// Hardware objects.
HX711 loadcell;
LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

// Sensitivity.
double sensitivity = 1;
  

void setup() {
  initSerial();
  initLoadCell();
  initLCD();
  getSensitivity();
  initQueue();
  pinMode(BTN_TARE, INPUT_PULLUP);
}


void loop() {
  double mass = getMassAveraged();
  
  reportMass(mass);
  listenForInput();
}


//----Support methods----//
  void initSerial() {
    // Initialize the serial connection; wait until it's running.
    
    Serial.begin(115200);
    
    while(!Serial) {
      // Wait for serial to initialize.
    } // Serial initialized.
    
    return;
  }
  
  
  void initLoadCell() {
    // Set up the HX711 for use. Turns on the device, then verifies the 
    // calibration value.
    
    Serial.println("Initializing HX711...");
    
    // Turn on the HX711 power supply.
    pinMode(HX_VCC, OUTPUT);
    digitalWrite(HX_VCC, HIGH);
    
    // Give it time to power on.
    delay(500);
    
    // Initialize the HX711.
    loadcell.begin(HX_DT, HX_SCK);
    
    // Wait until it's ready.
    bool is_ready = false;
    int num_retries = 3;
    int wait_delay = 200;
    while (!is_ready) {
      // Give some indication that it's thinking.
      Serial.print("...");
      is_ready = loadcell.wait_ready_retry(num_retries, wait_delay);
    }
    
    // Give the HX711 a chance to finish initializing.
    delay(2000);
    
    // Tare the thing.
    tare();
    
    Serial.println("HX711 Initialized!");
    Serial.println();
    
    return;
  }
  
  
  void initLCD() {
    // Runs all the necessary startup for the LCD.
    
    if (!isLCD) {
      return;
    }
    
    Serial.println("Initializing LCD...");
    
    // Turn on the LCD power supply.
    pinMode(LCD_VCC, OUTPUT);
    digitalWrite(LCD_VCC, HIGH);
    
    // Give it time to power on.
    delay(500);
    
    // Initialize the display.
    lcd.begin(LCD_COLS, LCD_ROWS);
    // Clear the display.
    lcd.clear();
    // Home the cursor.
    lcd.home();
    // Hide the cursor.
    lcd.noCursor();
    // Ensure the display is on. (lcd.noDisplay() turns off the display).
    lcd.display();
    
    // Test the display.
    Serial.println("Testing the display...");
    // Print an 8 to each character in the display.
    for (int i = 0; i < LCD_ROWS * LCD_COLS; i++) {
      lcd.print("8");
      if (i == LCD_COLS - 1) {
        lcd.setCursor(0, 1);
      }
    }
    delay(200);
    lcd.clear();
    
    Serial.println("LCD initialized!");
    Serial.println();
    
    return;
  }
  
  
  void initQueue() {
    for (int i = 0; i < QUEUE_SIZE; i++) {
      hxQueue[i] = 0.00;
    }
  }
  
  
  void tare() {
    // Bundles up the output and the action for taring.
    Serial.println("Tare...");
    loadcell.tare(HX_NUM_AVGS);
    
    return;
  }
  
  
  void clearDisplay() {
    lcd.clear();
  }
  
  
  void printToDisplay(String output, int row, int col) {
    lcd.setCursor(col, row);
    lcd.print(output);
    
    return;
  }
  
  
  void printToSerialAndDisplay(String output, int row, int col) {
    // Serial.
    Serial.println(output);
    
    // LCD.
    printToDisplay(output, row, col);
    
    return;
  }
  
  
  double getHxReadout() {
    // Read the raw (tared) value from the loadcell amplifier.
    return loadcell.get_value(HX_NUM_AVGS);
  }
  
  
  double getHxReadoutAveraged() {
    // Shift the queue.
    for (int i = QUEUE_SIZE - 1; i > 0; i--) {
      hxQueue[i] = hxQueue[i-1];
    }
    
    // Place the current mass (scaled by sensitivity) in the queue.
    hxQueue[0] = getHxReadout();
    
    // Return the average value from the queue.
    double sum = 0;
    for (int i = 0; i < QUEUE_SIZE; i++) {
      sum += hxQueue[i];
    }
    
    return sum / QUEUE_SIZE;
  }
  
    
  double getMass() {
    return getHxReadout() / sensitivity;
  }
  
  
  double getMassAveraged() {
    return getHxReadoutAveraged() / sensitivity;
  }
  
  
  void reportMass(double mass) {
    clearDisplay();
    printToSerialAndDisplay(String(mass, NUM_DIGITS) + " " + units, 0, 0);
    
    return;
  }
  
  
  void listenForInput() {
    switch (digitalRead(BTN_TARE)) {
      case 0: {
        unsigned long time_of_press = millis();
        printToSerialAndDisplay(".", LCD_ROWS - 1, LCD_COLS - 1);
        
        while (digitalRead(BTN_TARE) == 0) {
          // Wait for button release.
        } // Button released.
        
        printToDisplay(" ", LCD_ROWS - 1, LCD_COLS - 1);
        
        unsigned long time_of_release = millis();
        
        if (time_of_release - time_of_press < CAL_WAIT) {
          tare();
        } else {
          calibrate();
        }
        
        break;
      }        
      default: {
        // Buttons are active LOW; do nothing if button isn't pressed.
        break;
      }
    }
    
    return;
  }
  
  
  void calibrate() {
    // Tell the user what mass to use.
    printToSerialAndDisplay("Cal w/ " + String(CAL_STANDARD_MASS, NUM_DIGITS) + 
      " " + units, 0, 0);
      
    tare();
    
    // Wait for the mass to get added.
    printToSerialAndDisplay("Add mass...", 1, 0);
    while (getHxReadout() < CAL_THRESHOLD) {
      // Wait for obvious addition of mass.
    } // Mass apparently added.
    
    delay(1000);
    
    initQueue();
    double hxReadout;
    
    for (int i = QUEUE_SIZE; i>0; i--) {
      // Add extra space to account for change in number of digits displayed.
      printToSerialAndDisplay("Avg rem: " + String(i) + " ", 1, 0);
      hxReadout = getHxReadoutAveraged();
      delay(1000);
    }
    
    sensitivity = hxReadout / CAL_STANDARD_MASS;
    
    setSensitivity();
  }
  
  
  void getSensitivity() {
    // Fetch the stored sensitivity value.
    
    Serial.println("Reading sensitivity from memory...");
    
    char cal_check;
    EEPROM.get(CAL_SIGNATURE_ADDR, cal_check);
    
    if (cal_check != CAL_SIGNATURE) {
      Serial.println("No sensitivity stored in memory.");
    } else {
      EEPROM.get(CAL_VALUE_ADDR, sensitivity);
    }
    
    reportSensitivity();
  }
  
  
  void setSensitivity() {
    // Send the current sensitivity to memory.
    
    Serial.println("Writing sensitivity to memory...");
    reportSensitivity();
    
    EEPROM.put(CAL_SIGNATURE_ADDR, CAL_SIGNATURE);
    EEPROM.put(CAL_VALUE_ADDR, sensitivity);
    
    Serial.println("Sensitivity stored.");
    return;
  }
  
  
  void reportSensitivity() {
    Serial.println("Sensitivity: " + String(sensitivity, NUM_DIGITS) + 
      " div/" + units);
      
    return;
  }