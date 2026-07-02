/*  mass-scale: firmware for a 3D-printable digital mass balance.
    Copyright (C) 2025 Cameron K. Brooks

    Derived from OS_Nano_Balance, Copyright (C) 2019 Benjamin Hubbard
    (https://github.com/brhubbar/OS_Nano_Balance).

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

#include <HX711.h>
#include <EEPROM.h>
#include <LiquidCrystal.h>

#include "defaults.h"

// HX711 pins and setup
const int hx_dt = 2;
const int hx_sck = 3;
const int hx_vcc = 4;
const int hx_num_avgs = 3;
const int hx_cal_num_avgs = 15;
HX711 loadcell;

// LCD setup
bool isLCD = true;
const int lcd_rs = A0, lcd_en = A1, lcd_d4 = A2, lcd_d5 = A3, lcd_d6 = A4, lcd_d7 = A5;
const int lcd_rows = 2, lcd_cols = 16, lcd_vcc = 5;
LiquidCrystal lcd(lcd_rs, lcd_en, lcd_d4, lcd_d5, lcd_d6, lcd_d7);

// Calibration and EEPROM setup
char cal_sig = 'C', cal_check;
int cal_sig_addr = 0, cal_val_addr = sizeof(char);
float cal_val = 0.00f;
String units = "g";
float sensitivity;

// Input pins
const int btn_tare = 8;

// Tare button debounce (active-low with internal pull-up)
const unsigned long btn_debounce_ms = 50;
int btn_reading = HIGH;	  // last raw sample
int btn_state = HIGH;	  // last debounced (stable) state
unsigned long btn_changed = 0; // millis() of last raw change

// Readouts
double val;
float mass;
int num_digits = 6;	   // precision for printing the calibration value
const int disp_decimals = 2; // decimal places shown for the mass readout
// TODO (wishlist): adapt disp_decimals to the load cell resolution / scale.

// Non-blocking timing for events
unsigned long previousMillis = 0;
const long interval = 100; // Update interval (in ms)

// Initialization methods
void initLoadCell()
{
	Serial.println("Initializing HX711...");
	pinMode(hx_vcc, OUTPUT);
	digitalWrite(hx_vcc, HIGH);
	delay(500);

	loadcell.begin(hx_dt, hx_sck);

	// Wait for the HX711 to be ready; the first reads after power-on are
	// otherwise garbage.
	while (!loadcell.wait_ready_retry(3, 200))
		Serial.println("Waiting for HX711...");
	Serial.println("HX711 Initialized!");

	// Load the calibration (sensitivity) from EEPROM if a valid signature is
	// present, otherwise fall back to the compiled-in default.
	Serial.println("Loading calibration...");
	EEPROM.get(cal_sig_addr, cal_check);
	if (cal_check == cal_sig)
	{
		EEPROM.get(cal_val_addr, cal_val);
		Serial.print("Stored calibration: ");
	}
	else
	{
		cal_val = default_scale;
		Serial.print("No stored calibration, default: ");
	}
	Serial.print(cal_val, num_digits);
	Serial.println(" div/" + units);
	loadcell.set_scale(cal_val);

	// Zero the scale at boot. This also captures the platter as tare weight.
	loadcell.tare(20);
	delay(500);
}

void initLCD()
{
	Serial.println("Initializing LCD...");
	pinMode(lcd_vcc, OUTPUT);
	digitalWrite(lcd_vcc, HIGH);
	delay(500);
	lcd.begin(lcd_cols, lcd_rows);
	lcd.clear();
	lcd.noCursor();
	lcd.display();
}

void setup()
{
	Serial.begin(115200);
	while (!Serial)
	{
	}

	initLoadCell();
	if (isLCD)
		initLCD();

	pinMode(btn_tare, INPUT_PULLUP);

	Serial.println("Ready! Send 't' to tare, 'c' to calibrate.");
}

void loop()
{
	// Poll serial commands every iteration so they stay responsive.
	if (Serial.available())
	{
		String input = Serial.readStringUntil('\n');
		input.trim();

		if (input == "t")
		{
			// Tare
			Serial.println("Taring...");
			loadcell.tare(20);
		}
		else if (input == "c")
		{
			// Start calibration
			Serial.println("Calibration started. Send 'u00' when unloaded, or 'x' to abort.");
			startCalibrationLoop();
		}
	}

	// Poll the tare button every iteration, debounced. Act only on the
	// press edge (HIGH->LOW) so holding the button does not re-tare.
	int reading = digitalRead(btn_tare);
	if (reading != btn_reading)
	{
		btn_reading = reading;
		btn_changed = millis();
	}
	if (millis() - btn_changed > btn_debounce_ms && reading != btn_state)
	{
		btn_state = reading;
		if (btn_state == LOW)
		{
			Serial.println("Taring...");
			loadcell.tare(20);
		}
	}

	// Throttle the measurement and display to `interval` ms.
	unsigned long currentMillis = millis();
	if (currentMillis - previousMillis >= interval)
	{
		previousMillis = currentMillis;
		updateReadout();
	}
}

// Read once and report both the raw (tare-subtracted) value and the calibrated
// mass, on serial and on the LCD if present.
void updateReadout()
{
	val = loadcell.get_value(hx_num_avgs);	// raw, tare-subtracted
	mass = (float)(val / loadcell.get_scale()); // calibrated mass

	Serial.print("Raw: ");
	Serial.print(val);
	Serial.print(", Mass: ");
	Serial.print(mass, disp_decimals);
	Serial.println(" " + units);

	if (isLCD)
	{
		// Overwrite a fixed-width line instead of lcd.clear() to avoid flicker.
		String line = String(mass, disp_decimals) + " " + units;
		while (line.length() < (unsigned int)lcd_cols)
			line += " ";
		lcd.setCursor(0, 0);
		lcd.print(line);
	}
}

void startCalibrationLoop()
{
	while (true)
	{
		if (Serial.available())
		{
			String userInput = Serial.readStringUntil('\n');
			userInput.trim();

			if (userInput == "x")
			{
				// Abort without changing the stored calibration.
				Serial.println("Calibration aborted.");
				return;
			}
			else if (userInput == "u00")
			{
				// Unloaded at 0g
				loadcell.tare(20);
				Serial.println("Unloaded. Now place a known weight and send 'aXY' (e.g., 'a50' for 50g). Send 'x' to abort.");
			}
			else if (userInput.startsWith("a"))
			{
				// Known weight added
				userInput.remove(0, 1);
				float weight = userInput.toFloat();

				if (weight > 0)
				{
					// Use the raw, tare-subtracted reading (not get_units, which
					// would divide by the previous scale) so the new sensitivity
					// is computed from counts per gram.
					float rawValueAtWeight = loadcell.get_value(hx_cal_num_avgs);
					sensitivity = rawValueAtWeight / weight;

					// Reject a non-finite or near-zero sensitivity, which
					// happens if no weight was actually placed or the cell is
					// disconnected. A zero scale would later divide the mass
					// reading to infinity.
					if (!isfinite(sensitivity) || fabs(sensitivity) < 1e-6)
					{
						Serial.println("Reading too small; is the weight on the scale? Try again.");
						continue;
					}

					Serial.print("Calculated sensitivity: ");
					Serial.println(sensitivity, num_digits);

					// Save calibration
					loadcell.set_scale(sensitivity);
					EEPROM.put(cal_sig_addr, cal_sig);
					EEPROM.put(cal_val_addr, sensitivity);
					Serial.println("Calibration complete!");

					return;
				}
				else
				{
					Serial.println("Invalid weight entered. Try again.");
				}
			}
		}
	}
}