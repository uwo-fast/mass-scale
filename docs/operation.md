# Operating the scale

Adapted from Sections 2.4 and 2.5 of the paper in [`paper/`](paper/):
Hubbard BR, Pearce JM (2020). *Open-Source Digitally Replicable Lab-Grade
Scales.* Instruments 4(3): 18 (`CC-BY-4.0`), and from the protocol notes in
[`serial-protocol/`](serial-protocol/). It describes the
[DigitalMassBalance 2.0.1](../firmware/most-massbalance-2020/DigitalMassBalance/) firmware the paper
was written for; the later [MOST_MassBalance](../firmware/most-massbalance-2020/MOST_MassBalance/)
library keeps the same commands. Text marked **Note** is added here.

![The assembled scale](images/Oscale.png)

The scale works in two ways: as a simple scale with an LCD and a push-button,
and as a lab scale with a serial interface for data logging and control. A
single firmware does both; `Config.hpp` tells it whether an LCD is fitted, and
the serial interface always listens.

## Start-up

On power, over USB from a computer or a 5 V power block, the firmware:

1. opens a 9600 baud serial connection (it waits for serial to initialise, which
   does not noticeably delay start-up on a plain power supply);
2. powers up the HX711 and checks it is talking, reporting progress over serial;
3. powers up the LCD if one is configured, flashing every digit as `8` for a
   moment;
4. reads the saved calibration sensitivity from EEPROM, if a calibration has
   ever been run, and reports it; otherwise the sensitivity is 1 and the
   readout is the raw HX711 count;
5. clears the averaging queue (10 readings by default, set in `Config.hpp`),
   which sets the scale's response time;
6. sets the tare button as an input with the internal pull-up, so it is active
   low.

After that the scale reads the load cell as fast as it can, keeps a sliding
average, updates the LCD, watches the button, and listens for serial commands,
the last three at the report rate set in `Config.hpp` (1 Hz).

## As a simple scale

- **Tare.** Press and release the button on the front. A dot in the lower right
  corner of the LCD acknowledges the press.
- **Calibrate.** Hold the button for at least 3 s (`CAL_WAIT` in
  `Config.hpp`). The scale tares, then shows the mass to put on the bed (the
  calibration mass in `Config.hpp`, 235.9 g by default, one US cup of water).
  When it detects the added mass it averages 10 readings, works out the new
  sensitivity and saves it to EEPROM.

## As a lab scale over serial

The serial interface follows the Scale Manufacturers Association's SCP-0499
protocol to Level 2: the six Level 1 commands plus extensions. Open the port at
9600 baud. Every command is a character between a line feed and a carriage
return, `<LF>c<CR>`; the `<ESC>` reset is the one exception. The responses,
`<LF><s><r><n><m><f><xxxxxx.xxx><uuu><CR>`, are described in
[`serial-protocol/Response Formats.txt`](serial-protocol/Response%20Formats.txt).

| Command | Does |
| --- | --- |
| `W` | return the displayed weight |
| `Z` | zero the scale (with nothing on the bed) |
| `T` | tare: subtract the mass now on the bed, for instance a container |
| `T<xxxxxx.xxx>` | set the tare weight to the value given |
| `M` | return the stored tare weight |
| `C` | clear the tare, so readings refer to the zero set by `Z` |
| `D` | run the scale diagnostics |
| `A`, `B` | the "about" lines: protocol level, then firmware information |
| `R` | repeat the weight continuously at the report rate, until another command arrives |
| `<ESC>` | software reset; wait a few seconds, then send `A` to confirm the scale is back |
| `XC` | calibrate with the default calibration mass |
| `XC<xxxxxx.xxx>` | calibrate with the mass given; the scale zeroes, echoes the expected mass, measures for two minutes, then reports the new reading |
| `XL` | toggle power to the LCD, for power saving when the display is not needed |
| `XP` | scroll the readout precision between 0 and 4 decimal places |
| `X?` | list the commands (the leading `X` of the extended commands is also announced at the end of start-up) |

**Note:** the commands are single characters after the `<LF>`, so a terminal
program has to send the line feed first; PuTTY with its session logging is what
the paper used to capture data (see [`test-data/`](test-data/README.md)).

## Calibrating for measurements

For the paper's tests the scale was calibrated against a known mass measured on
a laboratory balance, or against a standard mass: the 100 g TAL221 with a 20 g
mass, the 500 g TAL221 and 5 kg TAL220 with 100 g. The electronics were left on
for at least an hour before calibrating and measuring; the paper observes that
the reading drifts noticeably after power-on, largely with temperature, and
that the comparison balances' manuals recommend 15 to 60 minutes of warm-up. A
cover over the bed (`Cover.stl`) reduces the effect of air currents.

## Mounting the load cell

The load cell must be mounted with its wires running toward the end fixed to
the base; wires leaving the floating end interfere with the measurement.

- **TAL220:** bolt the M5 end (the wires run to it) to the TAL220 boss in the
  base, connect the cell to the HX711 (red E+, black E−, white A−, green A+),
  snap the lid onto the base, then bolt the bed to the free end with the two
  M4 screws.
- **TAL221:** the lid is sandwiched between the bed and the load cell. Screw
  the untapped end of the cell (the wires do not run to it) to the bed with M3
  screws and nuts, connect the cell to the HX711 as above, then screw the
  tapped M3 end to the TAL221 boss in the base, by feel, with the lid snapped
  on.

Section 2.3 of the paper has photographs of each step.
