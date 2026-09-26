# Sources

Where everything in this repository came from, and what else exists about the
scale that is not copied here. Collected in September 2026.

## The three generations

| Generation | Where | In this repository |
| --- | --- | --- |
| **2019, OS Nano Balance**, Benjamin Hubbard | [github.com/brhubbar/OS_Nano_Balance](https://github.com/brhubbar/OS_Nano_Balance) (`GPL-3.0`), documented on Appropedia as [3D printed digital balance](https://www.appropedia.org/3D_printed_digital_balance) | full history, tag `mtu-nanobalance-2019`; the sketch in [`../firmware/os-nano-balance-2019/`](../firmware/os-nano-balance-2019/), the FreeCAD parts in [`../cad/freecad-2019/`](../cad/freecad-2019/), the OpenSCAD parts in [`../cad/openscad-2019/`](../cad/openscad-2019/) |
| **2020, the MOST lab-grade scale**, Benjamin Hubbard and Joshua Pearce | the paper, OSF, GitLab and Appropedia listed under [Primary](#primary) below | full history of the firmware library, tags `mtu-instruments-2020`, `mtu-final-2021` and `mtu-osf-2020`; the design in [`../cad/freecad-2020/`](../cad/freecad-2020/), the firmware in [`../firmware/most-massbalance-2020/`](../firmware/most-massbalance-2020/) |
| **2025–26, the revival**, Cameron Brooks, FAST research group | [github.com/uwo-fast/mass-scale](https://github.com/uwo-fast/mass-scale), the repository this one continues | full history, tag `fast-revival-2026`; the designs in [`../cad/openscad/`](../cad/openscad/), [`../cad/openscad-v3/`](../cad/openscad-v3/) and [`../cad/openscad-v1/`](../cad/openscad-v1/), the firmware in [`../firmware/mass-scale/`](../firmware/mass-scale/) |

## Primary

| What | Where |
| --- | --- |
| Paper, 2020 | [doi:10.3390/instruments4030018](https://doi.org/10.3390/instruments4030018), open access, `CC-BY-4.0` · in [`paper/`](paper/) · a [preprint](https://www.preprints.org/manuscript/202004.0472/v1) and an [author copy](https://digitalcommons.mtu.edu/michigantech-p/2052) also exist |
| Firmware library, with history | [gitlab.com/mtu-most/most_massbalance](https://gitlab.com/mtu-most/most_massbalance), included here (tags `mtu-instruments-2020`, `mtu-final-2021`, and upstream's own `v3.0.0` to `v3.4.0`) |
| CAD, the paper's sketch, circuit, datasheets and test data | [OSF me9a8](https://osf.io/me9a8), "Open source laboratory logging digital scale" (DOI 10.17605/OSF.IO/ME9A8), imported unchanged in the commit tagged `mtu-osf-2020` |
| Project page, with the breadboard assembly guide | [Appropedia: Open Source Digitally Replicable Lab-Grade Scales](https://www.appropedia.org/Open_Source_Digitally_Replicable_Lab-Grade_Scales), adapted in [`electronics-assembly.md`](electronics-assembly.md) |
| STLs, mirrored | [MyMiniFactory 126135](https://www.myminifactory.com/object/3d-print-open-source-digitally-replicable-lab-grade-scale-126135), linked from Appropedia as the "quick download" (not fetched: the site sits behind a browser check) |
| Use video | "MOST OS Balance.mpeg" on OSF, the paper's Video S1; attached to this repository's first release (see below) |
| The 2019 project's page | [Appropedia: 3D printed digital balance](https://www.appropedia.org/3D_printed_digital_balance) (its older title, "3-D Printable Digital Balance", redirects there) |

Libraries the firmware uses, with their upstreams: the
[HX711 library](https://github.com/bogde/HX711) by Bogdan Necula (`MIT`),
bundled with the 2020 firmware at 0.7.4 and fetched at 0.7.5 for the current
firmware by `just firmware-libs`; Arduino's
[LiquidCrystal](https://github.com/arduino-libraries/LiquidCrystal)
(`LGPL-2.1-or-later`), bundled with the 2020 sketch at 1.0.7 and fetched at
the same version for the current firmware. The 2020 library's OLED support
uses Adafruit's [SSD1306](https://github.com/adafruit/Adafruit_SSD1306),
[GFX](https://github.com/adafruit/Adafruit-GFX-Library) and
[BusIO](https://github.com/adafruit/Adafruit_BusIO) libraries, installed by
`just firmware-deps` rather than bundled.

The v3 design's load-cell and amplifier models come from
[CameronBrooks11/load-cell-scad](https://github.com/CameronBrooks11/load-cell-scad)
(`GPL-3.0`), copied at commit `c8f224a` (2026-07-01) into
[`../cad/openscad-v3/lib/load-cell-scad/`](../cad/openscad-v3/lib/load-cell-scad/README.md).

The 2020 firmware's serial interface implements the Scale Manufacturers
Association's SCP-0499 protocol; the standard and the association's load-cell
test guideline are linked from [`datasheets/`](datasheets/README.md).

## Related

- **The 2020 sketch's README** is the 2019 project's README with a few items
  struck through, and the revival's README was the same document updated for
  the serial-driven firmware; the 2019 original is kept at
  [`../firmware/os-nano-balance-2019/README.md`](../firmware/os-nano-balance-2019/README.md).
- **The upstream repository's earlier home.** The library's history starts on
  GitHub in January 2020 (its merge commits name pull requests there) and moved
  to the MOST group's GitLab in 2020; the GitHub copy no longer exists.

## Not included

- **The use video**, `Documentation/MOST OS Balance.mpeg` on OSF: 370,255,096
  bytes, MPEG-2 program stream, MD5 `66f02c4be425f369919c497e4ba0343c`. Too
  large for the repository; attached to its first release instead.
- **The Denver Instruments manuals** for the A-160 analytical balance and the
  APX402 balance the paper compared against (`Denver A-160 Operators
  Manual.pdf`, MD5 `16764d6581afa8f9837330ee14bca2db`; `Denver APX402
  Operators Manual.pdf`, MD5 `1d14f42e87e6531837753cf2fd0bd2c6`): third-party
  documents about instruments that are not part of the design. The paper cites
  them at denverinstrument.com (references 90 and 91). Reachable through the
  `mtu-osf-2020` tag.
- **The SMA protocol tester**, `SCPTesterProgram.zip` (a 2004 Windows
  installer, MD5 `8c11f268e9889f2dac8af41f9a3bb681`) and
  `SCPTesterDocumentation.pdf` (MD5 `549957656531bd4c33909fb9253603c0`): the
  Scale Manufacturers Association's own test program, used to check the
  firmware's compliance. Reachable through the `mtu-osf-2020` tag.
- **The SMA standard and test guideline**, `ScaleCommProtocol5199M1.pdf` and
  `loadcellapplicationtestguidelineapril2010.pdf`: the association's
  publications, linked from [`datasheets/`](datasheets/README.md) instead.
  Reachable through the `mtu-osf-2020` tag.
- **Duplicates.** The OSF copies of the Fritzing circuit and the two protocol
  notes, byte for byte the upstream repository's (after line-ending
  normalisation for the notes), and the STL zip, whose four files are stored
  unpacked. The revival's copies of the 2019 FreeCAD parts and of the 2020
  document, and the 2019 and revival copies of the datasheets, byte for byte
  the files kept. Reachable through the `mtu-osf-2020`, `mtu-nanobalance-2019`
  and `fast-revival-2026` tags.
- **Git metadata** of the imports: the 2019 sketch's HX711 submodule (the
  library is a build dependency instead), the sketch's stale `.gitmodules`
  and `.gitignore` on OSF, and the 2019 and revival `LICENSE`, `.gitignore`
  and `.gitattributes` files, which the repository-level files replace.

## Upstream problems found while importing

Fixed, so that the firmware builds on every platform (see
[`../CHANGELOG.md`](../CHANGELOG.md)):

- Both 2020 sketches included their headers with backslashes, and the 2020
  sketch included its bundled libraries at submodule paths that the OSF upload
  does not have.

Documented but not fixed: see the repository's issues and the README's known
problems. The 2019 sketch is kept as published; it includes its libraries at
the submodule paths of its own repository, with backslashes, and is not built
here.
