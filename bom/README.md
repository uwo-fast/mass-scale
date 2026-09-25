# Bill of materials

From Table 1 of the paper (Hubbard and Pearce 2020, `CC-BY-4.0`), whose prices
are April 2020 US retail; the paper's reference numbers are kept so the sources
can be found in it. The paper prices the whole scale at USD 51.36 without an
LCD and USD 65.55 with one, or USD 40.75 and 47.01 counting only the parts used
out of bulk packs (Table 5), plus USD 4.00 for a wall supply.

## 3-D printed parts

All in PLA except the optional cover; see [`../cad/`](../cad/README.md).

| Part | Notes |
| --- | --- |
| Base | print in the normal orientation; no supports needed |
| Cover (lid, `Top.stl`) | print in either orientation; without support at the cost of the top finish |
| Bed | print in either orientation; without support at the cost of the top finish |
| Cover (optional, `Cover.stl`) | print upside-down without supports; reduces air currents over the bed |

The three PLA parts weigh 117.40 g together (18.74 + 33.22 + 65.44 g), USD 2.46
in filament and electricity by the paper's estimate.

## Electronic components

| Component | Qty | Price (USD) | Paper ref. |
| --- | --- | --- | --- |
| Arduino Nano | 1 | 20.70 (a derivative with cable, 5.72) | [66], [67] |
| USB-A to mini-B cable (or whichever the Nano in use takes) | 1 | 5.26 | [68] |
| 5 V USB power block (optional; a computer's USB port also powers the scale) | 1 | 4.00 | [69] |
| HX711 load-cell amplifier | 1 | 8.50, sold with a TAL220 | [70] |
| Push-button, normally open, momentary | 1 | 2.50 for 20 | [71] |
| Jumper wires | — | — | — |
| Breadboard: solderless (stays outside the scale), or a 40 × 60 mm solder breadboard with solder and iron | 1 | 5.95, or 5.99 for 40 | [72], [73] |

With an LCD (optional):

| Component | Qty | Price (USD) | Paper ref. |
| --- | --- | --- | --- |
| 16 × 2 LCD | 1 | 5.99 | [74] |
| 10 kΩ potentiometer (contrast) | 1 | 0.25 | [75] |
| 220 Ω resistor (backlight) | 1 | 7.95 for 500 | [76] |

With a solder breadboard, in addition:

| Component | Qty | Price (USD) | Paper ref. |
| --- | --- | --- | --- |
| Female header pins, 1 × 4 | 2 | 0.95 | [77] |
| Female header pins, 1 × 8 | 1 | | [77] |
| Female header pins, 1 × 9 | 1 | | [77] |
| Male header pins, 1 × 12 | 1 | 3.00 for five 1 × 16 strips | [77] |
| Male header pins, 1 × 4 | 1 | | [77] |

The header counts in [`../docs/electronics-assembly.md`](../docs/electronics-assembly.md),
written for the third scale built, differ from these (two 1 × 15 and one each of
1 × 6 and 1 × 4 female, one 1 × 4 male); follow whichever layout you build.

## Load cell, one of

| Configuration | Parts | Price (USD) | Paper ref. |
| --- | --- | --- | --- |
| TAL220 (3 to 200 kg ranges; the paper tests 5 kg) | the load cell, wires terminated in a female 4-pin connector (red, black, white, green); two M4 × 25 mm cap screws (3 mm hex key); two M5 × 25 mm cap screws (4 mm hex key) | included with the HX711 above; under 2.00 for the screws | [70], [78] |
| TAL221 (100 to 1500 g ranges; the paper tests 100 g and 500 g) | the load cell, wires terminated as above; four M3 × 20 mm cap screws (2.5 mm hex key); two M3 nuts (5.5 mm socket) | 8.95 for the 100 g cell; under 2.00 for the screws | [79], [80] |

The datasheets of both load cells and of the HX711 and LCD are in
[`../docs/datasheets/`](../docs/datasheets/README.md).
