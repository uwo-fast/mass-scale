# Licensing

This repository holds three generations of an open-source digital scale: the
2019 OS Nano Balance by Benjamin Hubbard, the lab-grade scale made at Michigan
Technological University's Open Sustainability Technology (MOST) lab in
2019–2021, and the FAST research group's 2025–26 revival, each under the terms
it was published with, plus what the FAST research group has added since.
[What applies where](#what-applies-where) is the operative section: it is how
these files are made available. The rest of this document is commentary. The
binding terms are the full texts in [`LICENSES/`](LICENSES/).

There is deliberately no root `LICENSE` file. GitHub would label the whole
repository with whatever licence it found there, and no single licence covers it.

## What applies where

"As stated" means the licence the file, its repository or its host declared when
it was published. Apart from the files [notes 4 and 5](#notes) cover, nothing
here has been relicensed.

### The current design and firmware (2025–26)

| Path | Licence | Source of the statement |
| --- | --- | --- |
| `firmware/mass-scale/` | `GPL-3.0-or-later` | the header of `mass-scale.ino`, "either version 3 of the License, or (at your option) any later version", © 2025 Cameron K. Brooks, derived from OS Nano Balance © 2019 Benjamin Hubbard; its README says the same |
| `cad/openscad/`, `cad/openscad-v3/` except `lib/load-cell-scad/` | `GPL-3.0-or-later` **and** `CERN-OHL-S-2.0`, at your option | the lab's parametric CAD source, by the [organization licensing policy](https://github.com/uwo-fast/.github/blob/main/LICENSING.md); FAST research group and contributors (`lib/trapezium.scad` is © 2025 Cameron K. Brooks, copied from the lab's [open-source-bioreactor](https://github.com/uwo-fast/open-source-bioreactor)) |
| `cad/openscad-v1/` | `GPL-3.0-or-later` | a reformatting of the 2019 OpenSCAD parts, whose headers it keeps: "either version 3 of the License, or (at your option) any later version", © 2019 Benjamin Hubbard; a modification keeps the file's licence |
| `cad/openscad-v3/lib/load-cell-scad/` | `GPL-3.0` | the `LICENSE` beside them and the README of [CameronBrooks11/load-cell-scad](https://github.com/CameronBrooks11/load-cell-scad) at commit `c8f224a`; the file headers credit © 2021 Norman Hendrich and © 2025 Cameron K. Brooks — see [its README](cad/openscad-v3/lib/load-cell-scad/README.md) |

### The 2019 files

| Path | Licence | Source of the statement |
| --- | --- | --- |
| `firmware/os-nano-balance-2019/OS_Nano_Balance.ino`, `cad/openscad-2019/` | `GPL-3.0-or-later` | the header of each file, "either version 3 of the License, or (at your option) any later version", © 2019 Benjamin Hubbard |
| `firmware/os-nano-balance-2019/README.md`, `cad/freecad-2019/` (`Base.FCStd`, `Bed.FCStd`, `Face.FCStd`, `Parameters.csv`) | `GPL-3.0-or-later` — see [note 5](#notes) | the `LICENSE` of [brhubbar/OS_Nano_Balance](https://github.com/brhubbar/OS_Nano_Balance), the GPL-3.0 text with no "or later" statement for these files; read as or-later |
| `cad/freecad-2019/Base-osf.FCStd` | `GPL-3.0-or-later` — see [notes 1 and 4](#notes) | licence of the [OSF project](https://osf.io/me9a8), "GNU General Public License (GPL) 3.0"; the same document as Hubbard's `Base.FCStd`, saved a month later |

### The 2020 files

| Path | Licence | Source of the statement |
| --- | --- | --- |
| `firmware/most-massbalance-2020/MOST_MassBalance/`, except `src/HX711/` | `GPL-3.0-or-later` | the header of every source file, "either version 3 of the License, or (at your option) any later version", © 2020 Benjamin Hubbard; the `LICENSE` beside them is the GPL-3.0 text |
| `firmware/most-massbalance-2020/MOST_MassBalance/src/HX711/` | `MIT` | its `LICENSE` and headers, © 2018 Bogdan Necula ([bogde/HX711](https://github.com/bogde/HX711) 0.7.4) |
| `firmware/most-massbalance-2020/DigitalMassBalance/`, except `src/HX711/` and `src/LiquidCrystal/` | `GPL-3.0-or-later` | the header of `DigitalMassBalance.ino`, © 2020 Benjamin Hubbard; the `LICENSE` beside it is the GPL-3.0 text |
| `firmware/most-massbalance-2020/DigitalMassBalance/src/HX711/` | `MIT` | its headers, © 2018 Bogdan Necula |
| `firmware/most-massbalance-2020/DigitalMassBalance/src/LiquidCrystal/` | `LGPL-2.1-or-later` — see [note 2](#notes) | the README of [arduino-libraries/LiquidCrystal](https://github.com/arduino-libraries/LiquidCrystal) at the commit these two files match (1.0.7), © 2006–2008 Hans-Christoph Steiner, © 2010 Arduino LLC; the files themselves carry no header |
| `electronics/Circuit Diagram.fzz`, `docs/serial-protocol/` | `GPL-3.0-or-later` — see [notes 1 and 4](#notes) | the `LICENSE` of [mtu-most/most_massbalance](https://gitlab.com/mtu-most/most_massbalance), the GPL-3.0 text with no "or later" statement for these files, and the OSF project's "GNU General Public License (GPL) 3.0" for the same files; both read as or-later |
| `cad/freecad-2020/`, `electronics/*.png`, `docs/test-data/` | `GPL-3.0-or-later` — see [notes 1 and 4](#notes) | licence of the [OSF project](https://osf.io/me9a8), "GNU General Public License (GPL) 3.0", 2020, and the paper: "released under a GNU General Public License (GPL) 3.0"; read as or-later |
| `docs/paper/instruments-04-00018.pdf` | `CC-BY-4.0` | the article, © 2020 by the authors, licensee MDPI |
| `bom/README.md`, `docs/operation.md` | `CC-BY-4.0` | adapted from the paper, which they credit |
| `docs/electronics-assembly.md` | `CC-BY-SA-4.0` | adapted from the Appropedia page it names; Appropedia's site licence |
| `docs/images/` | `CC-BY-SA-4.0` — see [note 4](#notes) | the Appropedia file page of each image: two state `CC-BY-SA-4.0`, and `Oscale.png` states `GPL` with no version, read as `CC-BY-SA-4.0` under note 4; per file in [`docs/images/README.md`](docs/images/README.md) |

### Third-party documents, and what FAST added

| Path | Licence | Source of the statement |
| --- | --- | --- |
| `docs/datasheets/` | none stated — see [open questions](#open-questions) | third-party documents (HTC-Sensor, Avia Semiconductor, Xiamen Ocular, and the unnamed maker of the 1602A module) as uploaded to OSF and as carried by the 2019 project; their publishers hold the copyright. The two Scale Manufacturers Association documents that were here are linked instead ([`docs/datasheets/README.md`](docs/datasheets/README.md)) and remain in the history |
| `justfile`, `tools/`, `cad/parts.tsv`, `cad/scad-parts.tsv`, `.github/`, and other build tooling added by FAST | `GPL-3.0-or-later` | FAST research group and contributors |
| `README.md`, `LICENSING.md`, `CHANGELOG.md`, `CITATION.cff`, `docs/sources.md`, and the `README.md` in each directory except `firmware/mass-scale/`, `firmware/os-nano-balance-2019/`, `firmware/most-massbalance-2020/MOST_MassBalance/`, `firmware/most-massbalance-2020/DigitalMassBalance/` and `cad/openscad-v3/lib/load-cell-scad/` | `GPL-3.0-or-later` **and** `CERN-OHL-S-2.0`, at your option | FAST research group and contributors |

Modifications made here to a file keep that file's licence. The FAST changes to
the three 2020 sketch and header files (include paths only), to
`cad/openscad-v3/test_load_cells.scad` (include paths only) and to
`firmware/mass-scale/README.md` (formatting) are licensed like the files they
change; see the git history.

New FAST work that is not a modification of the files above follows the
[organization licensing policy](https://github.com/uwo-fast/.github/blob/main/LICENSING.md):
`GPL-3.0-or-later` for software and parametric CAD source, `CERN-OHL-S-2.0` for
hardware design files and the outputs generated from them. Add a row to the table
above for each new directory.

### Notes

1. **The GPL version.** OSF's licence picker records "GPL 3.0" with copyright year
   2020 and no holder, and does not say whether later versions are allowed. The
   upstream repository's `LICENSE` is the GPL-3.0 text; its source files add
   "or (at your option) any later version" in their headers, but the Fritzing
   circuit and the two protocol notes have no header of their own. Both
   statements are read as `GPL-3.0-or-later` (note 4).
2. **LiquidCrystal.** The two files bundled with the 2020 sketch match
   `src/LiquidCrystal.cpp` and `.h` of arduino-libraries/LiquidCrystal at tag
   1.0.7 byte for byte, the commit the sketch's original git submodule pinned.
   That repository states its licence only in its README: LGPL "either version
   2.1 of the License, or (at your option) any later version". The paper says
   the library is "used under the GNU Lesser General Public License". The
   current firmware uses the same library, fetched at the same version by
   `just firmware-libs` rather than bundled, together with bogde/HX711 0.7.5
   (`MIT`); neither is in the tree.
3. **The protocol notes.** `docs/serial-protocol/` quotes command and response
   definitions from the Scale Manufacturers Association's SCP-0499 standard
   (the "4.x" and "5.x" section numbers are the standard's) and adds the
   scale's own extended commands. The standard itself is linked from
   [`docs/datasheets/README.md`](docs/datasheets/README.md).
4. **Unversioned GPL statements.** GPL statements that give no version
   qualifier — OSF's "GPL 3.0", the paper's, and the upstream `LICENSE` for
   the two files without a header — follow the MOST lab's standing practice:
   `GPL-3.0-or-later` for software and `CERN-OHL-S-2.0` for hardware, applied
   on 2026-09-25 by the FAST research group, which maintains this repository
   and continues the lab's work. Where a paper and its OSF project state
   different licences, the paper's statement is used; here they agree, and
   every file they cover is under the GPL, so nothing has moved to
   `CERN-OHL-S-2.0`. Images the lab published with no licence, or with an
   unversioned GFDL or GPL, are `CC-BY-SA-4.0`; here that is
   `docs/images/Oscale.png`, applied on 2026-09-26. Michigan Technological
   University holds the copyright in work its staff and students made there.
5. **The 2019 licence file.** OS Nano Balance's `LICENSE` is the GPL-3.0 text
   with no "or later" statement, and the repository declares `GPL-3.0`. Its
   sketch and OpenSCAD files carry or-later headers of their own; the FreeCAD
   parts, the parameter sheet and the README do not. For those, the bare
   licence file is read as `GPL-3.0-or-later` under the same standing practice
   as note 4, which the MOST lab applies to its students' work: the project was
   made at the lab in 2019 by the author of the 2020 files, whose own headers
   are or-later. Applied on 2026-09-26 by the FAST research group.

## Open questions

These have to be settled with the copyright holders before the repository is
made public:

- **The datasheets** in `docs/datasheets/` are third-party documents. They
  were uploaded to OSF under the project's GPL-3.0 statement, and carried by
  the 2019 project under its, which their publishers never made. Either keep
  them with their publishers' permission, or replace them with the links in
  [`docs/sources.md`](docs/sources.md). The two SMA documents have already
  been replaced with links; they remain in the history.
- **LiquidCrystal's licence** is inferred from its upstream README (note 2).
- **load-cell-scad's headers** credit © 2021 Norman Hendrich; the repository
  states `GPL-3.0` without saying what licence the 2021 work was taken under.

## Contributions

"Contribution" means any work of authorship intentionally submitted for inclusion
in this project — a pull request, patch, commit, issue, or other communication —
excluding anything conspicuously marked "Not a Contribution".

Unless you state otherwise, contributions you submit are licensed under whichever
of the licences above covers the files they touch, with no additional terms. By
submitting a contribution you represent that you have the right to license it on
those terms.

## Patents

GPL-3.0 (section 11) and CERN-OHL-S-2.0 (section 7) contain express patent
provisions. Review them before using, modifying, or distributing this project.

## Disclaimer of warranty and liability

A summary of terms in the licences themselves — GPL-3.0 sections 15–17,
LGPL-2.1 sections 15–16, the MIT licence, CERN-OHL-S-2.0 section 6, and
CC-BY-4.0 and CC-BY-SA-4.0 section 5.

THIS PROJECT, INCLUDING ALL SOFTWARE AND HARDWARE DESIGNS, IS PROVIDED "AS IS"
WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND
NON-INFRINGEMENT.

TO THE EXTENT PERMITTED BY APPLICABLE LAW, IN NO EVENT SHALL THE AUTHORS,
CONTRIBUTORS, OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES, OR OTHER
LIABILITY ARISING FROM, OUT OF, OR IN CONNECTION WITH THIS PROJECT.

### Additional disclaimer

The following is not a summary of any licence. It is an additional disclaimer
offered alongside them:

THE DESIGNS HERE HAVE NOT BEEN INDEPENDENTLY VERIFIED OR CERTIFIED FOR SAFETY,
METROLOGICAL OR REGULATORY COMPLIANCE, AND THE SCALE IS NOT A LEGAL-FOR-TRADE
INSTRUMENT. THE 2019 AND 2020 DESIGNS HAVE NOT BEEN REBUILT OR RE-TESTED SINCE
THEIR ORIGINAL PUBLICATION, AND THE CURRENT DESIGN HAS NOT BEEN CHARACTERISED.
ANY PHYSICAL PRODUCT MANUFACTURED FROM THEM IS PRODUCED AND OPERATED ENTIRELY
AT YOUR OWN RISK. TO THE EXTENT PERMITTED BY APPLICABLE LAW, THE AUTHORS AND
CONTRIBUTORS ARE NOT RESPONSIBLE FOR ANY PERSONAL INJURY, PROPERTY DAMAGE, OR
OTHER HARM RESULTING FROM THE USE OR MANUFACTURE OF PRODUCTS BASED ON THEM.

## Trademarks

No licence here grants trademark rights. This project does not grant permission to
use the names, trademarks, or logos of Michigan Technological University, the MOST
lab, Arduino, the Scale Manufacturers Association, the FAST research group, or
the project's contributors, except as needed to describe the project's origin.

## Precedence

Where the commentary in this document and the full licence texts conflict, the
full texts prevail. The tables under [What applies where](#what-applies-where)
are not commentary: they state which files are made available under which terms.
