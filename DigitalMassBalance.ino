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


const String REV = "0.0.1";

// Hardware objects.
HX711 loadcell;
LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

// Weight variables.
double sensitivity = 1.0;
double tareWeight = 0.0;
double mass;
  

void setup() {
  initSerial();
  initLoadCell();
  initLCD();
  getSensitivity();
  initQueue();
  pinMode(BTN_TARE, INPUT_PULLUP);
}


void loop() {
  // Get the averaged, tared mass.
  mass = getMassAveraged();
  
  // enforce report rate without hampering sample rate.
  if (millis() - lastRefresh > 1/REPORT_RATE * 1000) {
    lastRefresh = millis();
    
    // Listen for input over serial.
    // When using Arduino Serial Monitor, switch to 'Both NL & CR' in bottom 
    // right.
    // When using Putty, use Ctrl+J for LF, followed by command, followed by 
    // Ctrl+M for CR.
    if (Serial.available() > 2) { // Minimum cmd length is 3 characters, <LF>c<CR>
      doSerial();
    }
    
    if (isContinuousReport) {
      reportMass();
    }
    
    // Simple scale functionality.
    displayMass(mass);
    listenForButtonInput();
  }
}


//----Support methods----//
  //----Initialize----//
    void initSerial() {
      // Initialize the serial connection; wait until it's running.
      Serial.begin(BAUD);
      
      while(!Serial) {
        // Wait for serial to initialize.
      } // Serial initialized.
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
      
      // Zero the scale (set the offset on data returned by the HX711).
      zeroSilent();
      
      Serial.println("HX711 Initialized!");
      Serial.println();
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
    }
    
    // TODO: replace averaging queue with a low pass filter.
    void initQueue() {
      for (int i = 0; i < QUEUE_SIZE; i++) {
        hxQueue[i] = 0.00;
      }
    }
    
    
  //----Serial----//
    void doSerial() {
      // Give everything a chance to transmit over serial.
      delay(200);
      // Make sure the buffer is settled.
      Serial.flush();

      // Determine how many characters are waiting.
      int len = Serial.available();
      
      // Initialize an array to hold the command.
      int cmd[len];

      // Fill up cmd.
      receiveCommand(cmd, len);
      
      // Parse the command for the <LF> and <CR>. The command starts one char
      // beyond the LF, and ends with the CR.
      int startIdx = findInArray(cmd, LF, 0, len) + 1;
      int endIdx = findInArray(cmd, CR, startIdx, len);
      
      // Check for errors. Since we start after the LF, minimum index is 1.
      if (startIdx < 1 || endIdx < 1) {
        Serial.println("Did not find LF or CR");
        return;
      }
      
      // Execute the command.
      doCommand(cmd, startIdx, endIdx);
    }

    // Note that arrays are pointers, so just pass in the array's variable.
    // TODO: make this accommodate commands that come character by character.
    //        This can be accomplished by clearing command in when receiving
    //        a LF, but requires preallocating potentially too much memory for a 
    //        command. Can be accomplished by peeking for a <CR>. Could have
    //        timing issues.
    void receiveCommand(int *cmd, int len) {  
      // Read everything but the last character into the cmd array.
      // The last character could be an LF which would lead the next command.
      for (int i = 0; i < len-1; i++) {
        cmd[i] = Serial.read();
      }

      // If the next guy is <LF>, leave 'er alone (this accommodates Arduino
      // Serial Monitor behavior).
      char last = Serial.peek();
      // If not, add it to the cmd array (could be a CR from putty, or a 
      // single character from terminal without local echo/line editing on.
      if (last != LF){
        // Chuck it at the end of the array.
        cmd[len-1] = Serial.read();
      }
    }
   
    
    int findInArray(int *array, int query, int startSearch, int endSearch) {
      // Finds a character in an integer array. Used to find <CR> and <LF>.
      // Account for incrementing i at top of loop.
      int i = startSearch - 1;
      int c;  // Character being checked.
      do {
        // Increment i.
        i++;
        // Read a character from the array.
        c = array[i];
        
        if (i > endSearch) {
          return -1;
        }
      } while (c != query);
      
      return i;
    }
    
    
    void doCommand(int *cmd, int startIdx, int endIdx) {
      // By the SMA standard, the first character indicates the function. 
      // The scale should not accept a command if it doesn't match the syntax 
      // exactly. (eg <LF> wa <CR> should not execute the <LF> w <CR> command.)
      // To combat this, interpret commands by length.
      // First, cancel continuous reporting.
      isContinuousReport = 0;
      switch (endIdx - startIdx) {
        case 1:   // Single character commands.
          switch ((char) cmd[startIdx]) {
            case 'w':
            case 'W':
              reportMass();

              break;
            case 'z':
            case 'Z':
              zero();

              break;
            case 'd':
            case 'D':
              // TODO: Implement actual diagnostics.
              Serial.print("\n    \r");

              break;
            case 'a':
            case 'A':
              aboutIdx = 0;
              Serial.print("\nSMA:2/1.0\r");
              break;
            case 'b':
            case 'B':
              switch (aboutIdx) {
                case 0:
                  Serial.print("\nMFG:Michigan Tech MOST\r");
                  break;
                case 1:
                  Serial.print("\nMOD:Digital Mass Balance\r");
                  break;
                case 2:
                  Serial.print("\nREV:" + REV + "\r");
                  break;
                case 3:
                  Serial.print("\nEND:\r");
                  break;
                default:
                  Serial.print("\n?\r");
                  break;
              }
              
              aboutIdx++;
              
              break;
            case ESC:
              softReset();
              
              break;
            case 'r':
            case 'R':
              isContinuousReport = 1;

              break;
            case 't':
            case 'T':
              tare();

              break;
            case 'c':
            case 'C':
              clearTare();

              break;
            case 'm':
            case 'M':
              reportTare();

              break;
            default:
              Serial.print("\n?\r");
              
              break;
          }
          
          break;
        case 2:
          // Command will be an x followed by a character.
          switch ((char) cmd[startIdx]) {
            case 'x':
            case 'X':
              switch ((char) cmd[startIdx + 1]) {
                case 'c':
                case 'C':
                  calibrate();
                  
                  break;
                default:
                  Serial.print("\n?\r");
              }
              
              break;
            default:
              Serial.print("\n?\r");
              
              break;
          }
          
          // char data[2];
          // sprintf(data, "%c%c", cmd[startIdx], cmd[startIdx+1]);
          // Serial.println(data);
          
          break;
        case 12:
          switch ((char) cmd[startIdx]) {
            case 'x':
            case 'X':
              switch ((char) cmd[startIdx + 1]) {
                case 'c':
                case 'C':
                  // The calibration weight is submitted with 10 characters of 
                  // the value, plus 3 characters of units.
                  // TODO: handle multiple options for units.
                  String calStandard = "";
                  for (int i = 0; i < 10; i++) {
                    calStandard += String((char) cmd[startIdx + 2 + i]);
                  }
                  // BUG: there seems to be an overflow issue for large inputs.
                  // BUG: toDouble() only returns two decimal points of 
                  //      precision.
                  cal_standard_mass = calStandard.toDouble();
                  calibrate();
                  break;
                default:
                  Serial.print("\n?\r");
              }
              break;
              
          }
          break;
        default:
          Serial.print("\n?\r");
          
          break;
      }
    }

  
  //----Other----//
    void zero() {
      // Uses the HX711 built in tare() command to set the zero (empty bed).
      loadcell.tare(HX_NUM_AVGS);
      
      clearTareSilent();
      
      mass = getMassAveraged();
      // SMA formatted response.
      String response = "\n";     // <LF>
      response += "Z";            // <s>
      response += String(range);  // <r>
      response += String(netOrGross());            // <n>
      response += " ";            // <m>
      response += " ";            // <f>
      response += rightJustify(String(mass, PRECISION), WT_WIDTH);  // <xxxxxx.xxx>
      response += units;          // <uuu>
      response += "\r";           // <CR>
      Serial.print(response);
    }
    
    
    void zeroSilent() {
      loadcell.tare(HX_NUM_AVGS);
      clearTareSilent();
    }
    
    void tare() {
      // Sets the tareWeight to the current measured weight 
      // (accounting for current tare).
      tareWeight += mass;
      
      reportMass();
    }
    
    
    void tareSilent() {
      tareWeight += mass;
    }
    
    
    void clearTare() {
      tareWeight = 0.0;
      
      reportMass();
    }
    
    
    void clearTareSilent() {
      tareWeight = 0.0;
    }
    
    
    void reportTare() {
      String tareStr = rightJustify(String(tareWeight, PRECISION), WT_WIDTH);
      // SMA formatted response.
      String response = "\n";     // <LF>
      response += " ";            // <s>
      response += String(range);  // <r>
      response += "T";            // <n>
      response += " ";            // <m>
      response += " ";            // <f>
      response += tareStr;        // <xxxxxx.xxx>
      response += units;          // <uuu>
      response += "\r";           // <CR>
      Serial.print(response);
    }
    
    
    String rightJustify(String str, int width) {
      int numWhtSpc = width - str.length();
      for (int i = 0; i < numWhtSpc; i++) {
        str = " " + str;
      }
      
      return str;
    }
    
    
    String netOrGross() {
      // Net/Gross status. Net if tared, gross if tare = 0.
      if (tareWeight == 0) {
        return "G";
      } else {
        return "N";
      }
    }
    
    
    void reportMass() {
      mass = getMassAveraged();
      String massStr = rightJustify(String(mass, PRECISION), WT_WIDTH);
      
      // SMA formatted response.
      String response = "\n";           // <LF>
      response += " ";                  // <s>
      response += String(range);        // <r>
      response += String(netOrGross()); // <n>
      response += " ";                  // <m>
      response += " ";                  // <f>
      response += massStr;              // <xxxxxx.xxx>
      response += units;                // <uuu>
      response += "\r";                 // <CR>
      Serial.print(response);
    }
    
    
    void clearDisplay() {
      lcd.clear();
    }
    
    
    void printToDisplay(String output, int row, int col) {
      lcd.setCursor(col, row);
      lcd.print(output);
    }
    
    
    // TODO: Check for overload.
    double getHxReadout() {
      // Read the raw (zeroed) value from the loadcell amplifier.
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
      return getHxReadout() / sensitivity - tareWeight;
    }
    
    
    double getMassAveraged() {
      return getHxReadoutAveraged() / sensitivity - tareWeight;
    }
    
    
    void displayMass(double mass) {
      clearDisplay();
      String massStr = rightJustify(String(mass, PRECISION), WT_WIDTH);
      printToDisplay(massStr + " " + units, 0, 0);
    }
    
    
    void listenForButtonInput() {
      switch (digitalRead(BTN_TARE)) {
        case 0: {
          unsigned long time_of_press = millis();
          printToDisplay(".", LCD_ROWS - 1, LCD_COLS - 1);
          
          while (digitalRead(BTN_TARE) == 0) {
            // Wait for button release.
          } // Button released.
          
          printToDisplay(" ", LCD_ROWS - 1, LCD_COLS - 1);
          
          unsigned long time_of_release = millis();
          
          if (time_of_release - time_of_press < CAL_WAIT) {
            zeroSilent();
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
    }
    
    
    void calibrate() {
      // Tell the user what mass to use.
      printToDisplay("Cal w/ " + String(cal_standard_mass, PRECISION) + 
        " " + units, 0, 0);
        
      zeroSilent();
      
      // Wait for the mass to get added.
      printToDisplay("Add mass...", 1, 0);
      while (getHxReadout() < CAL_THRESHOLD) {
        // Wait for obvious addition of mass.
      } // Mass apparently added.
      
      delay(1000);
      
      initQueue();
      double hxReadout;
      
      for (int i = QUEUE_SIZE; i>0; i--) {
        // Add extra space to account for change in number of digits displayed.
        printToDisplay("Avg rem: " + String(i) + " ", 1, 0);
        hxReadout = getHxReadoutAveraged();
        delay(1000);
      }
      
      sensitivity = hxReadout / cal_standard_mass;
      
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
    }
    
    
    void reportSensitivity() {
      Serial.println("Sensitivity: " + String(sensitivity, PRECISION) + 
        " div/" + units);
    }
    
    
    void softReset() {
      asm volatile (" jmp 0");
    }