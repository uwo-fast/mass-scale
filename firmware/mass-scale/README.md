# mass-scale

Arduino firmware for a 3D-printable digital mass balance using an HX711 load-cell amplifier and optional 16×2 LCD.

The firmware provides:

- Live raw and calibrated mass readings
- Serial and push-button tare
- Serial-guided calibration
- EEPROM calibration storage
- Automatic zeroing at startup

## Hardware

- Arduino-compatible board
- HX711 load-cell amplifier
- Load cell
- Optional HD44780-compatible 16×2 LCD
- Optional momentary tare button

## Pin Configuration

| Function    |   Pin |
| ----------- | ----: |
| HX711 DT    |     2 |
| HX711 SCK   |     3 |
| HX711 power |     4 |
| LCD power   |     5 |
| Tare button |     8 |
| LCD RS      |    A0 |
| LCD Enable  |    A1 |
| LCD D4–D7   | A2–A5 |

The tare button connects pin 8 to ground and uses the internal pull-up resistor.

## Dependencies

- `HX711` — the "HX711 Arduino Library" by Bogdan Necula
  ([bogde/HX711](https://github.com/bogde/HX711)), 0.7.5
- `LiquidCrystal` ([arduino-libraries/LiquidCrystal](https://github.com/arduino-libraries/LiquidCrystal)), 1.0.7

`EEPROM` is provided by the Arduino core. From the repository, `just firmware-libs`
fetches both libraries at those versions into `build/deps/libraries/` and
`just check-firmware` and `just firmware` build against them; in the Arduino
IDE, install them from the Library Manager.

## Configuration

Set the fallback calibration factor in `defaults.h`:

```cpp
#define default_scale 0.016016
```

Set `isLCD` to `false` when no LCD is connected.

## Usage

Flash the firmware, then open the serial monitor at:

- **115200 baud**
- **Newline** line ending

The scale automatically tares during startup. The attached platter must therefore be installed and unloaded when the device powers on.

### Serial Commands

| Command | Action            |
| ------- | ----------------- |
| `t`     | Tare the scale    |
| `c`     | Start calibration |

The tare button performs the same operation as the `t` command.

### Calibration

1. Remove all removable weight while leaving the permanent platter installed.
2. Send `c`.
3. Send `u00` when the scale is unloaded.
4. Place a known mass on the scale.
5. Send `a` followed by the mass in grams (or `x` to abort).

For a 500 g calibration mass:

```text
c
u00
a500
```

The calculated sensitivity is stored in EEPROM and restored automatically after restart.

For best results, use a calibration mass near the normal operating range and wait for the load-cell reading to stabilize before entering its value.

## Output

The serial monitor reports the tare-subtracted raw HX711 reading and calibrated mass:

```text
Raw: 617283.00, Mass: 500.00 g
```

The LCD, when enabled, displays the mass in grams.

## License

Copyright © 2025 Cameron K. Brooks.

This project is licensed under the GNU General Public License, version 3 or later.

It is derived from `OS_Nano_Balance`, Copyright © 2019 Benjamin Hubbard.
