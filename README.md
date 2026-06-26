# Mass Scale — 3D-Printable Digital Balance

Firmware and design files for a digital mass balance with 3D printable components.
([Project Page](https://www.appropedia.org/3-D_Printable_Digital_Balance))

This balance is powered and controlled by an Arduino Nano.
Using the digital I/O pins, the Nano powers and reads information from an HX711
32-bit load cell amplifier.
The readout from this loadcell can be calibrated using a standard mass.
Calibration is performed over the serial connection (see [Calibrate](#calibrate)).
The calibrated value is distributed via two media: Serial and an LCD display.
When connected to the Nano over serial (115200 baud), the raw (tared) and calibrated
readout are displayed.
Depending on the serial client in use, this data can be logged and plotted -
primarily for scientific uses.
When an LCD is included in the system, the calibrated value is displayed with units.

A rewrite of [brhubbar / OS_Nano_Balance](https://github.com/brhubbar/OS_Nano_Balance)
with a serial-driven calibration workflow.

## Power

The entire system is powered by the USB input to the Nano.
This can be supplied by a computer or a 5V wall adapter.

- The HX711 and LCD are powered by digital I/O pins, meaning they can be independently turned on and off if desired.
- This works because the HX711 requires a maximum of 1.5 mA @ 5V (see data sheet in `doc`).
- Similarly, the LCD requires around 2.5 mA @ 5V (see data sheet in `doc`).
- Arduino I/O pins can supply 40 mA @ 5V (recommended max is 20 mA continuous)
  (see [Arduino Forum](https://forum.arduino.cc/index.php?topic=121675.0)

## Wishlist

- Send a calibration value over serial, or offer a selection on the display (select with Tare)
- Improve readout on the display to offer more information and work with calibration
- Condense readout commands into functions to make the code
  [DRY](https://pragprog.com/the-pragmatic-programmer/extracts/tips)
- Improved averaging algorithm to improve Signal-to-Noise-Ratio
- Add compatibility for different types of displays
- Update readout resolution (number of decimals) to match the resolution of the load cell.
  This could be adaptive baseed on the calibration value.
  (eg a 5000g load cell offers a different level of precision than a 50kg load cell).

## Printable Parts

Two options exist: OpenSCAD and FreeCAD. Both are designed to be parametric such that they can adapt to various load cells and setups.

### OpenSCAD

Use the customizer window to alter the parameters.

Dependancies:

- [CameronBrooks11 / load-cell-scad](https://github.com/CameronBrooks11/load-cell-scad.git)

### FreeCAD

Open the spreadsheet to alter the parameters.

## Wiring

Wiring has not yet been cleanly drawn up.
The LCD wiring is based on the
[Hello World](https://www.arduino.cc/en/Tutorial/HelloWorld)
example.
Pin locations can be determined by looking at the .ino source code.

**Load Cell --> HX711:**

- red --> E+
- blk --> E-
- grn --> A+
- wht --> A-

**HX711 --> Arduino:**  
The HX711 is powered by the Arduino's digital pin.
This is possible because the Arduino can supply 20 mA of current, while the
HX711 only requires 1.5 mA.

- GND --> GND
- DT --> D2
- SCK --> D3
- VCC --> D4

**Button --> Arduino:**

- GND --> Tare --> D8

Calibration no longer uses a button; it is driven over serial (see
[Calibrate](#calibrate)).

## Firmware

The sketch is in `src/mass-scale/`. It sends a readout to the Serial Monitor at
115200 baud and accepts single-line commands; Arduino's Serial Plotter can be
used to get a live readout of changing mass on the balance.

### Dependencies

- [bogde/HX711](https://github.com/bogde/HX711) — install via the Arduino
  Library Manager (or `lib_deps` in PlatformIO).
- `LiquidCrystal` and `EEPROM` ship with the Arduino AVR core.

### Tare

To tare (zero the output), press the tare button (on pin 8) or send `t` over
serial.

### Calibrate

Calibration is interactive over serial:

1. Send `c` to enter calibration.
2. With nothing on the platter, send `u00` to zero it.
3. Place a known mass on the platter and send `aXY`, where `XY` is the mass in
   grams (e.g. `a50` for 50 g).

The firmware computes a new sensitivity from the raw reading and the known mass,
applies it, and stores it in EEPROM so it persists across power cycles.

## Data Logging

To log data, use a third-party client such as PuTTY, which offers session-logging
capabilities.
