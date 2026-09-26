# Firmware

Three generations of the scale's firmware, all for an Arduino Nano (ATmega328P)
reading an HX711 load-cell amplifier, with an optional 16 × 2 LCD. The current
one is [`mass-scale/`](mass-scale/); the other two are kept as published.

| Directory | What it is | By | Built by `just check` |
| --- | --- | --- | --- |
| [`mass-scale/`](mass-scale/) | **The current firmware (2025–26):** a rewrite of the 2019 sketch with calibration driven over serial (`c`, `u00`, `a<grams>`), the sensitivity stored in EEPROM, tare from the button or `t`, a debounced button, a non-blocking 100 ms readout of the raw and calibrated values, and guards against a missing HX711 or a corrupt calibration ([details](mass-scale/README.md)) | Cameron K. Brooks, derived from the 2019 sketch | yes, against bogde/HX711 0.7.5 and LiquidCrystal 1.0.7 fetched by `just firmware-libs` |
| [`most-massbalance-2020/`](most-massbalance-2020/) | **The paper's firmware (2020):** `DigitalMassBalance`, the 2.0.1 sketch the paper describes, and `MOST_MassBalance`, its later library form (3.4.0) with LCD and OLED display drivers and an averaging filter. Both implement the Scale Manufacturers Association's SCP-0499 serial protocol to Level 2 ([details](most-massbalance-2020/README.md)) | Benjamin Hubbard | yes, both |
| [`os-nano-balance-2019/`](os-nano-balance-2019/) | **The 2019 sketch:** `OS_Nano_Balance.ino`, with a calibrate button (D7) as well as tare (D8), a fixed 235.9 g calibration mass and readings in kilograms; kept with the project's [README](os-nano-balance-2019/README.md) of the time | Benjamin Hubbard | no: it includes the HX711 and LiquidCrystal libraries at the paths of its own repository's git submodules (`src\HX711\src\HX711.h`), which are not here |

## Wiring

All three use the same pins, so a scale wired for one runs any of them.

| Function | Pin | Notes |
| --- | --- | --- |
| HX711 DT | D2 | |
| HX711 SCK | D3 | |
| HX711 VCC | D4 | the HX711 (1.5 mA) is powered from a digital pin, so it can be switched off |
| LCD power | D5 | as above (the LCD draws about 2.5 mA) |
| Tare button | D8 | to ground; internal pull-up |
| Calibrate button | D7 | 2019 sketch only; the 2020 sketch defines it but does not use it |
| LCD RS, E, D4–D7 | A0, A1, A2–A5 | 4-bit mode, as Arduino's LiquidCrystal "Hello World" |

Load cell to HX711: red to E+, black to E-, green to A+, white to A-. The
2020 design's Fritzing circuit and breadboard photographs are in
[`../electronics/`](../electronics/README.md) and
[`../docs/electronics-assembly.md`](../docs/electronics-assembly.md).

## Building and flashing the current firmware

```sh
just firmware-deps   # once: the pinned AVR core and libraries
just firmware        # build/firmware/mass-scale/mass-scale.ino.hex, and the 2020 targets
arduino-cli upload --fqbn arduino:avr:nano -p /dev/ttyUSB0 --input-dir build/firmware/mass-scale
```

Or compile and upload the sketch directly, naming the fetched libraries so
they take priority over any of the same name installed for the user:

```sh
arduino-cli compile --fqbn arduino:avr:nano \
  --library build/deps/libraries/HX711 --library build/deps/libraries/LiquidCrystal \
  firmware/mass-scale
arduino-cli upload --fqbn arduino:avr:nano -p /dev/ttyUSB0 firmware/mass-scale
```

Clones with the older bootloader need `--fqbn arduino:avr:nano:cpu=atmega328old`.
In the Arduino IDE, install "HX711 Arduino Library" (Bogdan Necula) and
"LiquidCrystal" from the Library Manager and open `mass-scale/mass-scale.ino`.

Then open a serial monitor at 115200 baud with newline line endings: the scale
tares itself at start-up, prints `Raw: …, Mass: … g` ten times a second, tares
on `t` or the button, and calibrates on `c` followed by `u00` with the platter
empty and `a<grams>` with a known mass on it (`x` aborts). The result is
stored in EEPROM. Any serial client that logs, such as PuTTY, records the
readings; Arduino's Serial Plotter shows them live.

## What the current firmware does not do yet

Compared with the 2020 firmware it replaces: it does not speak the SMA
SCP-0499 protocol (it has its own single-letter commands), has no OLED
driver, no configurable averaging filter and no logging beyond what a serial
client captures. These are tracked as issues.
