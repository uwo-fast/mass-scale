# Licensing

This repository holds the open-source lab-grade digital scale made at Michigan
Technological University's Open Sustainability Technology (MOST) lab in
2019–2021, under the terms it was published with, plus what the FAST research
group has added since. [What applies where](#what-applies-where) is the
operative section: it is how these files are made available. The rest of this
document is commentary. The binding terms are the full texts in
[`LICENSES/`](LICENSES/).

There is deliberately no root `LICENSE` file. GitHub would label the whole
repository with whatever licence it found there, and no single licence covers it.

## What applies where

"As stated" means the licence the file, its repository or its host declared when
it was published. Apart from the files [note 4](#notes) covers, nothing here
has been relicensed.

| Path | Licence | Source of the statement |
| --- | --- | --- |
| `firmware/MOST_MassBalance/`, except `src/HX711/` | `GPL-3.0-or-later` | the header of every source file, "either version 3 of the License, or (at your option) any later version", © 2020 Benjamin Hubbard; the `LICENSE` beside them is the GPL-3.0 text |
| `firmware/MOST_MassBalance/src/HX711/` | `MIT` | its `LICENSE` and headers, © 2018 Bogdan Necula ([bogde/HX711](https://github.com/bogde/HX711) 0.7.4) |
| `firmware/DigitalMassBalance/`, except `src/HX711/` and `src/LiquidCrystal/` | `GPL-3.0-or-later` | the header of `DigitalMassBalance.ino`, © 2020 Benjamin Hubbard; the `LICENSE` beside it is the GPL-3.0 text |
| `firmware/DigitalMassBalance/src/HX711/` | `MIT` | its headers, © 2018 Bogdan Necula |
| `firmware/DigitalMassBalance/src/LiquidCrystal/` | `LGPL-2.1-or-later` — see [note 2](#notes) | the README of [arduino-libraries/LiquidCrystal](https://github.com/arduino-libraries/LiquidCrystal) at the commit these two files match (1.0.7), © 2006–2008 Hans-Christoph Steiner, © 2010 Arduino LLC; the files themselves carry no header |
| `electronics/Circuit Diagram.fzz`, `docs/serial-protocol/` | `GPL-3.0-or-later` — see [notes 1 and 4](#notes) | the `LICENSE` of [mtu-most/most_massbalance](https://gitlab.com/mtu-most/most_massbalance), the GPL-3.0 text with no "or later" statement for these files, and the OSF project's "GNU General Public License (GPL) 3.0" for the same files; both read as or-later |
| `cad/`, `electronics/*.png`, `docs/test-data/` | `GPL-3.0-or-later` — see [notes 1 and 4](#notes) | licence of the [OSF project](https://osf.io/me9a8), "GNU General Public License (GPL) 3.0", 2020, and the paper: "released under a GNU General Public License (GPL) 3.0"; read as or-later |
| `docs/datasheets/` | none stated — see [open questions](#open-questions) | third-party documents (HTC-Sensor, Avia Semiconductor, Xiamen Ocular, the Scale Manufacturers Association) as uploaded to OSF; their publishers hold the copyright |
| `docs/paper/instruments-04-00018.pdf` | `CC-BY-4.0` | the article, © 2020 by the authors, licensee MDPI |
| `bom/README.md`, `docs/operation.md` | `CC-BY-4.0` | adapted from the paper, which they credit |
| `docs/electronics-assembly.md` | `CC-BY-SA-4.0` | adapted from the Appropedia page it names; Appropedia's site licence |
| `docs/images/` | `CC-BY-SA-4.0` — see [note 4](#notes) | the Appropedia file page of each image: two state `CC-BY-SA-4.0`, and `Oscale.png` states `GPL` with no version, read as `CC-BY-SA-4.0` under note 4; per file in [`docs/images/README.md`](docs/images/README.md) |
| `justfile`, `tools/`, `cad/parts.tsv`, `.github/`, and other build tooling added by FAST | `GPL-3.0-or-later` | FAST research group and contributors |
| `README.md`, `LICENSING.md`, `CHANGELOG.md`, `CITATION.cff`, `docs/sources.md`, and the `README.md` in each directory except `firmware/MOST_MassBalance/` and `firmware/DigitalMassBalance/` | `GPL-3.0-or-later` **and** `CERN-OHL-S-2.0`, at your option | FAST research group and contributors |

Modifications made here to a file keep that file's licence. The FAST changes to
the three sketch and header files (include paths only, see the git history) are
licensed like the files they change.

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
   the library is "used under the GNU Lesser General Public License".
3. **The protocol notes.** `docs/serial-protocol/` quotes command and response
   definitions from the Scale Manufacturers Association's SCP-0499 standard
   (the "4.x" and "5.x" section numbers are the standard's) and adds the
   scale's own extended commands. The standard itself is in
   `docs/datasheets/ScaleCommProtocol5199M1.pdf`.
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

## Open questions

These have to be settled with the copyright holders before the repository is
made public:

- **The datasheets and the SMA standard** in `docs/datasheets/` are third-party
  documents. They were uploaded to OSF under the project's GPL-3.0 statement,
  which their publishers never made. Either keep them with their publishers'
  permission, or replace them with the links in
  [`docs/sources.md`](docs/sources.md).
- **LiquidCrystal's licence** is inferred from its upstream README (note 2).

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
INSTRUMENT. THEY HAVE NOT BEEN REBUILT OR RE-TESTED SINCE THEIR ORIGINAL
PUBLICATION. ANY PHYSICAL PRODUCT MANUFACTURED FROM THEM IS PRODUCED AND
OPERATED ENTIRELY AT YOUR OWN RISK. TO THE EXTENT PERMITTED BY APPLICABLE LAW,
THE AUTHORS AND CONTRIBUTORS ARE NOT RESPONSIBLE FOR ANY PERSONAL INJURY,
PROPERTY DAMAGE, OR OTHER HARM RESULTING FROM THE USE OR MANUFACTURE OF
PRODUCTS BASED ON THEM.

## Trademarks

No licence here grants trademark rights. This project does not grant permission to
use the names, trademarks, or logos of Michigan Technological University, the MOST
lab, Arduino, the Scale Manufacturers Association, the FAST research group, or
the project's contributors, except as needed to describe the project's origin.

## Precedence

Where the commentary in this document and the full licence texts conflict, the
full texts prevail. The table under [What applies where](#what-applies-where) is
not commentary: it states which files are made available under which terms.
