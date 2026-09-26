# Firmware

Two generations of the scale's firmware, both by Benjamin Hubbard, both for an
Arduino Nano (ATmega328P) reading an HX711 load-cell amplifier, with an optional
16 × 2 LCD. Both build with `just check`.

| Directory | What it is | From |
| --- | --- | --- |
| [`DigitalMassBalance/`](DigitalMassBalance/) | **DigitalMassBalance 2.0.1**, the sketch the paper describes: one `.ino` plus `Config.hpp`, `Pinouts.hpp` and `Libraries.hpp` under `src/`, with the HX711 (0.7.4) and LiquidCrystal (1.0.7) libraries bundled beside them | OSF, uploaded 2020-04-15; the same files as upstream commit `1085674` (tag `mtu-instruments-2020`) |
| [`MOST_MassBalance/`](MOST_MassBalance/) | **MOST_MassBalance 3.4.0**, the rework of that sketch into a library after the paper: a `MOST_MassBalance` class, a `DataFilter` averaging queue, a `DisplayInterface` with `LCD` and `OLED` implementations, and an example sketch | [gitlab.com/mtu-most/most_massbalance](https://gitlab.com/mtu-most/most_massbalance), history included (tag `mtu-final-2021`) |

## DigitalMassBalance (the paper's firmware)

`DigitalMassBalance.ino` is a self-contained sketch. `Pinouts.hpp` sets the
pins (HX711 data on D2, clock on D3 and power on D4; LCD on A0–A5 with power on
D5; the tare button on D8, with a separate calibrate input defined on D7),
`Config.hpp` the options (whether an
LCD is present, the 10-sample averaging window, the 3 s button hold that starts
calibration, the default 235.9 g calibration mass, and the 9600 baud serial
rate), and `Libraries.hpp` the includes. Its serial interface implements the
Scale Manufacturers Association's SCP-0499 Level 2 command set; see
[`../docs/operation.md`](../docs/operation.md) and
[`../docs/serial-protocol/`](../docs/serial-protocol/).

Open the folder in the Arduino IDE, or build and upload with arduino-cli:

```sh
arduino-cli compile --fqbn arduino:avr:nano firmware/DigitalMassBalance
arduino-cli upload  --fqbn arduino:avr:nano -p /dev/ttyUSB0 firmware/DigitalMassBalance
```

Clones with the older bootloader need `--fqbn arduino:avr:nano:cpu=atmega328old`.

## MOST_MassBalance (the library)

The library keeps every scale function in `src/` and ships one example,
`examples/DigitalMassBalance/DigitalMassBalance.ino`, which wires an LCD and
constructs a `MOST_MassBalance` with a 10-sample averaging queue and the tare
button on D8; `begin()` takes the HX711 pins, by default power on D4, data on D2
and clock on D3, the same as the 2020 sketch. Its `README.md` explains
the intended use: the library directory itself is the sketch folder, and the
example is copied into it as `MOST_MassBalance.ino` (a name its `.gitignore`
deliberately ignores), so `#include "src/..."` resolves. `just check` builds it
exactly that way, in a temporary copy.

It needs the LiquidCrystal library and, because `OLED.h` is included by the
example, Adafruit's SSD1306, GFX and BusIO libraries from the Library Manager;
`just firmware-deps` installs the pinned versions. The HX711 library it includes
as `HX711/src/HX711.h` is vendored under `src/HX711/`
([its README](MOST_MassBalance/src/HX711/README.md)) in place of the git
submodule upstream used. Upstream's README notes that an OLED needs more RAM
than a Nano has, so an OLED build wants a Nano Every or a Mega.

`beginBackground()` and `getMassAveraged()` let another program use the library
as a load-cell handler without the serial front end (added in 3.4.0).

## What changed here

Only include paths, one commit each, so the sketches compile on Linux and macOS
as well as Windows: the three `src\...` includes in each sketch use forward
slashes, and `Libraries.hpp` points at the bundled libraries where the OSF
upload placed them. See [`../CHANGELOG.md`](../CHANGELOG.md).
