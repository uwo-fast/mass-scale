# Electronics

There is no printed circuit board. The scale's electronics are an Arduino Nano,
an HX711 load-cell amplifier breakout, a push-button, and optionally a 16 × 2
character LCD with a 10 kΩ contrast trimmer and a 220 Ω backlight resistor,
wired on a solderless breadboard or, as built in the paper, on a 40 × 60 mm
solder breadboard with header pins for the Nano, the HX711, the load cell and
the LCD.

| Path | What it is |
| --- | --- |
| [`Circuit Diagram.fzz`](Circuit%20Diagram.fzz) | the Fritzing sketch (Benjamin Hubbard, 2020-04-09); the upstream repository's copy, byte for byte the one published on OSF |
| [`Circuit Diagram_bb.png`](Circuit%20Diagram_bb.png) | the breadboard view, the paper's Figure 1 |
| [`Circuit Diagram_schem.png`](Circuit%20Diagram_schem.png) | the schematic view, the paper's Figure 2 |

The HX711 and the LCD are powered from Nano digital pins (D4 and D5), which the
firmware switches, rather than from the 5 V rail: the HX711 draws at most
1.5 mA and the LCD a few mA, within what a pin supplies. The load cell connects
to the HX711 as red → E+, black → E−, white → A−, green → A+. The Nano's pin
assignments are in
[`../firmware/DigitalMassBalance/src/Pinouts.hpp`](../firmware/DigitalMassBalance/src/Pinouts.hpp).

The parts are listed in [`../bom/`](../bom/README.md);
[`../docs/electronics-assembly.md`](../docs/electronics-assembly.md) walks
through wiring the solder breadboard.
