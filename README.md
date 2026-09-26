# Mass Scale

> A 3-D printed digital scale built around a strain-gauge load cell, an HX711
> amplifier and an Arduino Nano, read over serial for data logging. Three
> generations in one repository: Benjamin Hubbard's 2019 OS Nano Balance, the
> lab-grade scale Michigan Tech's MOST lab published in *Instruments* in 2020,
> and the FAST research group's 2025–26 revival, which is the current design.

[![Paper](https://img.shields.io/badge/Paper-Instruments_2020-blue.svg)](https://doi.org/10.3390/instruments4030018)
[![Appropedia](https://img.shields.io/badge/Appropedia-project_page-lightblue.svg)](https://www.appropedia.org/Open_Source_Digitally_Replicable_Lab-Grade_Scales)
[![Licensing](https://img.shields.io/badge/Licensing-by_component-lightgrey.svg)](LICENSING.md)

![The assembled 2020 scale: the printed housing with its yellow weighing platform and the LCD](docs/images/Oscale.png)

## Overview

The scale is a printed housing that holds a single-point parallel-beam load
cell (a TAL220 for kilogram ranges or a TAL221 for gram ranges), an HX711
24-bit load-cell amplifier, an Arduino Nano, a push-button, and optionally a
16 × 2 LCD. Everything runs from 5 V over USB, so a phone charger or a
computer's USB port powers it. The button tares the scale; over serial, a
computer reads it continuously and calibrates it against a known mass.

**The current design** is the revival's:

- [`cad/openscad/`](cad/README.md#the-current-design) — the OpenSCAD v2
  enclosure, with a bevelled front face for the LCD, and the platter that
  fits over it. Parametric: the dimensions are variables at the top of
  `assembly.scad`. [`cad/openscad-v3/`](cad/README.md) is the next revision,
  in progress, with a sliding, latching lid.
- [`firmware/mass-scale/`](firmware/README.md) — the rewritten firmware:
  calibration over serial, stored in EEPROM; tare from the button or serial;
  a non-blocking readout of raw and calibrated values ten times a second;
  guards against a missing amplifier or a corrupt calibration.

The 2020 design and firmware are kept in full and still checked, and the 2019
files are kept for reference and attribution; see [History](#history) and
[`docs/sources.md`](docs/sources.md).

The scale is described in:

> Hubbard BR, Pearce JM (2020). *Open-Source Digitally Replicable Lab-Grade
> Scales.* Instruments 4(3): 18.
> [doi:10.3390/instruments4030018](https://doi.org/10.3390/instruments4030018)

The paper tests three load cells, a 5 kg TAL220 and 500 g and 100 g TAL221s,
against two commercial laboratory balances and against standard masses. It
reports repeatability within 0.05 g for all three, down to a standard deviation
of about 0.005 g for the 100 g cell, readings that track the laboratory balances
linearly, and a parts cost of about USD 51 without an LCD or USD 66 with one
(USD 41 and 47 counting only the parts used out of bulk packs), against USD 110
to 289 for commercial scales with a serial interface.

**Status.** The v2 enclosure and platter have been printed, and the firmware
builds and has had its calibration and readout reworked since; neither has
been characterised the way the paper characterised the 2020 scale. The v2 parts do not yet model the load cell's
mounting or the electronics' seats, and v3 is unfinished; the firmware does
not yet have the 2020 firmware's SMA protocol, OLED support or averaging
filter. See [Known problems](#known-problems) and the issues.

## Repository layout

- `cad/` — every generation of the printed parts: `openscad/` (current),
  `openscad-v3/` (in progress), `openscad-v1/`, `freecad-2020/` with the
  published STLs, `freecad-2019/`, `openscad-2019/`
  ([details](cad/README.md))
- `firmware/` — `mass-scale/` (current), `most-massbalance-2020/` (the paper's
  sketch and the library form of it), `os-nano-balance-2019/`
  ([details](firmware/README.md))
- `electronics/` — the 2020 Fritzing circuit and its breadboard and schematic
  views ([details](electronics/README.md)); the pins are the same in every
  generation
- `bom/` — the paper's bill of materials ([as tables](bom/README.md))
- `docs/` — the 2020 [operation guide](docs/operation.md), the
  [breadboard assembly guide](docs/electronics-assembly.md),
  [the paper](docs/paper/), the [datasheets](docs/datasheets/README.md), the
  [test data](docs/test-data/README.md), the
  [serial protocol notes](docs/serial-protocol/README.md), and
  [sources](docs/sources.md) for everything else
- `tools/` — the STL and FreeCAD comparison scripts the checks use

## Getting started

You need [OpenSCAD](https://openscad.org/),
[arduino-cli](https://arduino.github.io/arduino-cli/), Python 3, `curl`,
`unzip` and [just](https://github.com/casey/just). FreeCAD 0.18 or later
opens the 2020 and 2019 CAD; it is not needed for the checks.

```sh
just firmware-deps   # install the pinned AVR core and libraries; fetch the pinned HX711 and LiquidCrystal
just check           # check the FreeCAD files and STLs, render every OpenSCAD part, build every firmware target (what CI runs)
just render          # write every OpenSCAD part to build/cad/openscad/
just firmware        # build every firmware target and leave the .hex files under build/firmware/
```

`OPENSCAD` names the OpenSCAD command when it is not on `PATH`, and
`OPENSCAD_FLAGS` adds flags to every render.

To build one: render `enclosure` and `platter` from `cad/openscad/`
(`just render`, or the customizer in OpenSCAD's GUI with `assembly.scad`), buy
the parts in [`bom/`](bom/README.md) (the electronics are the same in every
generation), wire them as in [`firmware/README.md`](firmware/README.md#wiring)
or the [breadboard guide](docs/electronics-assembly.md), and flash the
firmware:

```sh
just firmware
arduino-cli upload --fqbn arduino:avr:nano -p /dev/ttyUSB0 --input-dir build/firmware/mass-scale
```

Then open a serial monitor at 115200 baud (newline line endings). The scale
tares itself at start-up and prints `Raw: …, Mass: … g`; `t` or the button
tares it, and `c`, `u00` with the platter empty, then `a<grams>` with a known
mass on it, calibrates it and stores the result in EEPROM. Any serial client
that logs, such as PuTTY, records the readings.
[`firmware/mass-scale/README.md`](firmware/mass-scale/README.md) has the
details; [`docs/operation.md`](docs/operation.md) describes the 2020 firmware
and its SCP-0499 commands.

## Known problems

Carried over from the published designs, or opened by the revival, and not
fixed here; each is an issue:

- **Drift after power-on.** The paper observes that the reading drifts
  noticeably when the scale is first switched on, largely with temperature,
  and recommends a warm-up; no firmware here compensates for temperature.
- **The current firmware lacks features of the 2020 firmware:** the SMA
  SCP-0499 serial protocol, the OLED display driver, the configurable
  averaging filter, and the library form that other sketches can use.
- **The v2 enclosure has no load-cell mount or electronics seats**, and v3 is
  unfinished; the 2020 FreeCAD design has both.
- **The OLED display is unstable on small boards** (2020 library). Upstream's
  README says an OLED needs more RAM than a Nano has (use a Nano Every or a
  Mega), and that turning an OLED off and straight back on with `XL` can stall
  the firmware.
- **The housing is light.** The paper notes that the printed housing would
  deform under heavy loads, which side-loads the load cell; higher-capacity
  cells want a stiffer housing.
- **The published 2020 `Base.stl` predates the FreeCAD document.** It is
  1.25 % smaller than the base the document builds, with the same outline; the
  other three STLs match the document (see [`cad/README.md`](cad/README.md)).
- **The paper's sketch was Windows-only as published**: its includes used
  backslashes, fixed here in three one-line commits (see
  [`CHANGELOG.md`](CHANGELOG.md)). The 2019 sketch is kept as published and
  is not built.
- **Third-party datasheets** are redistributed without a licence statement;
  see [`LICENSING.md`](LICENSING.md).

## History

The repository carries all three histories, joined without rewriting any of
them. An annotated tag marks each source as it was:

| Generation | What | Tags |
| --- | --- | --- |
| **2019 — OS Nano Balance**, Benjamin Hubbard | The first version: an Arduino Nano, an HX711 and a TAL220 or TAL221 in printed parts drawn first in OpenSCAD and then in FreeCAD, with a sketch that calibrates from a button against a fixed mass. Published at [brhubbar/OS_Nano_Balance](https://github.com/brhubbar/OS_Nano_Balance) and on Appropedia as [3D printed digital balance](https://www.appropedia.org/3D_printed_digital_balance). Here: `firmware/os-nano-balance-2019/`, `cad/freecad-2019/`, `cad/openscad-2019/` | `mtu-nanobalance-2019` |
| **2020 — the MOST lab-grade scale**, Benjamin Hubbard and Joshua Pearce | The scale the paper describes: one parametric FreeCAD document, the DigitalMassBalance 2.0.1 sketch implementing the SMA SCP-0499 serial protocol, and its later library form MOST_MassBalance 3.4.0. Published in [*Instruments*](https://doi.org/10.3390/instruments4030018), on [OSF me9a8](https://osf.io/me9a8), at [gitlab.com/mtu-most/most_massbalance](https://gitlab.com/mtu-most/most_massbalance) and on [Appropedia](https://www.appropedia.org/Open_Source_Digitally_Replicable_Lab-Grade_Scales). Here: `cad/freecad-2020/`, `firmware/most-massbalance-2020/`, `electronics/`, `bom/`, `docs/` | `mtu-instruments-2020` (the firmware as published with the paper), `mtu-final-2021` (upstream's last commit), `mtu-osf-2020` (the OSF files as published), and upstream's own `v3.0.0` to `v3.4.0` |
| **2025–26 — the FAST revival**, Cameron Brooks | A new OpenSCAD enclosure and platter (v2), a next revision in progress (v3) using the [load-cell-scad](https://github.com/CameronBrooks11/load-cell-scad) library, and a rewrite of the 2019 firmware with calibration over serial. Published at [uwo-fast/mass-scale](https://github.com/uwo-fast/mass-scale), which this repository continues. Here: `cad/openscad/`, `cad/openscad-v3/`, `cad/openscad-v1/`, `firmware/mass-scale/` | `fast-revival-2026` |

[`CHANGELOG.md`](CHANGELOG.md) lists what changed when the histories were
joined and when the 2020 files were imported; [`docs/sources.md`](docs/sources.md)
says where every file came from and what was left out.

## Citation

If you use this scale, please cite the paper above. GitHub's **Cite this
repository** button gives it, from [`CITATION.cff`](CITATION.cff), which also
references the 2019 and revival work.

## License

This repository is licensed by component: the current firmware and the 2019
and 2020 firmware and design files are `GPL-3.0-or-later`; the revival's
OpenSCAD designs are `GPL-3.0-or-later` and `CERN-OHL-S-2.0`, at your option;
the bundled libraries are `MIT` and `LGPL-2.1-or-later` and the vendored
load-cell-scad `GPL-3.0`; the paper and the guides adapted from it are
`CC-BY-4.0`, the Appropedia material `CC-BY-SA-4.0`, and the datasheets state
none. [`LICENSING.md`](LICENSING.md) states what applies where, including the
questions still open.

## Contact

Maintained by the [FAST research group](https://uwo-fast.github.io/). For
research collaboration inquiries, contact Dr. Joshua Pearce
(<joshua.pearce@uwo.ca>).
