# Changelog

All notable changes to this project are documented in this file.

The format loosely follows [Keep a Changelog](https://keepachangelog.com/en/1.1.0/)
and [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### The three generations joined (2026-09)

The repository, now `mass-scale`, carries the full histories of
Benjamin Hubbard's 2019 [OS Nano Balance](https://github.com/brhubbar/OS_Nano_Balance)
(tag `mtu-nanobalance-2019`) and of the FAST research group's 2025–26 revival,
[uwo-fast/mass-scale](https://github.com/uwo-fast/mass-scale) (tag
`fast-revival-2026`), beside the 2020 history already here. Each was merged
unchanged under a staging folder and then sorted into place in commits that
only move files, so `git log --follow` reaches every file's origin. Nothing
was rewritten.

#### Added

- `cad/openscad/`: the revival's OpenSCAD v2 enclosure, platter and assembly,
  the current design; `cad/openscad-v3/`: the v3 revision in progress, with
  the four files of [load-cell-scad](https://github.com/CameronBrooks11/load-cell-scad)
  it includes vendored under `lib/` at commit `c8f224a` (`GPL-3.0`, with a
  provenance README) and its includes pointed there, so it renders from a
  plain clone; `cad/openscad-v1/`: the revival's reformatting of the 2019
  OpenSCAD parts.
- `cad/freecad-2019/`: Hubbard's three FreeCAD parts and `Parameters.csv`,
  with the OSF revision of the base kept beside his as `Base-osf.FCStd` and a
  README saying which is which; `cad/openscad-2019/`: his original OpenSCAD
  parts.
- `firmware/mass-scale/`: the revival's rewritten firmware, the current one,
  with its README made plain Markdown (it was wrapped in a code fence).
- `firmware/os-nano-balance-2019/`: Hubbard's 2019 sketch and project README.
- `docs/datasheets/1602LCD.pdf`, the 1602A LCD reference the 2019 project
  carried.
- `just check-scad` (and `cad/scad-parts.tsv`): renders the v2 and v3 parts
  with `--hardwarnings`, `OPENSCAD` naming the command and `OPENSCAD_FLAGS`
  adding flags; `just render` writes them to `build/cad/openscad/`.
- `just firmware-libs`: fetches bogde/HX711 0.7.5 and
  arduino-libraries/LiquidCrystal 1.0.7 as release tarballs, checks their
  SHA-256s, and `check-firmware` and `firmware` build `firmware/mass-scale`
  against them with `--library`, ahead of any library of the same name
  installed for the user.
- The CI workflow installs OpenSCAD and can be started by hand.
- A History section in the README, a `cad/freecad-2019/README.md`, a new
  `firmware/README.md` covering all three generations with the shared wiring,
  and `CITATION.cff` references for the 2019 and revival work.

#### Changed

- The 2020 design moved to `cad/freecad-2020/` (document and `stl/`), the
  2020 firmware to `firmware/most-massbalance-2020/`; `cad/parts.tsv` and the
  guides that name them follow. These commits only move files.
- `LICENSING.md` covers the 2019 files, the revival's firmware and designs and
  the vendored library, and reads Hubbard's bare GPL-3.0 licence file as
  or-later under the standing practice already applied to the 2020 files.
- Every self reference is `mass-scale`.

#### Removed

- The Scale Manufacturers Association's SCP-0499 standard and Load Cell
  Application and Test Guideline from `docs/datasheets/`: the association's
  publications, linked from the datasheets README instead (its site and the
  Wayback Machine). Both remain in the history.
- Duplicates, byte for byte the files kept: the revival's copies of the 2019
  FreeCAD parts and of the 2020 document, the 2019 and revival copies of the
  datasheets, and the OSF copies of the 2019 `Bed.FCStd` and `Face.FCStd`.
- Files the merged tree supersedes: the 2019 and revival `LICENSE`,
  `.gitignore` and `.gitattributes`, the revival's project README (its
  wiring and serial notes moved into `firmware/README.md`), the 2019 sketch's
  HX711 submodule, and two scratch files of the v2 `lib/` (one empty, one
  written against a load-cell-scad API that no longer exists).

### Imported from MOST (2026-09)

Imported into uwo-fast from
[mtu-most/most_massbalance](https://gitlab.com/mtu-most/most_massbalance), with
its full history (the firmware the paper describes is tagged
`mtu-instruments-2020`, upstream's last commit `mtu-final-2021`, and upstream's
own `v3.0.0` to `v3.4.0` tags are kept), and the design files from
[OSF me9a8](https://osf.io/me9a8), imported unchanged in the commit tagged
`mtu-osf-2020`. Paths below are as they were at the time; the CAD is now under
`cad/freecad-2020/` and the firmware under `firmware/most-massbalance-2020/`.

#### Added

- `cad/`: the FreeCAD assembly with its four bodies, the three earlier part
  files, and the four published STLs, from OSF.
- `firmware/DigitalMassBalance/`: the 2.0.1 sketch the paper describes, with
  the HX711 and LiquidCrystal libraries it bundles, from OSF.
- `firmware/MOST_MassBalance/src/HX711/`: the HX711 library at the commit the
  submodule pinned (0.7.4, `MIT`), in place of the submodule.
- `electronics/`: the Fritzing renderings from OSF beside the Fritzing sketch
  from upstream.
- `docs/`: the component datasheets and the SMA protocol, the test logs,
  spreadsheet and plots, the paper (`CC-BY-4.0`), an operation guide adapted
  from it, the breadboard assembly guide adapted from Appropedia with its
  photographs, and a sources page; READMEs for `cad/`, `firmware/`,
  `electronics/` and `bom/`, with the paper's bill of materials as tables.
- `justfile` with `check` (verify every FreeCAD document and published STL,
  build both firmware targets), `firmware-deps`, `firmware` and `stl-stats`,
  and a CI workflow running `just check`.
- `cad/parts.tsv`, the list of bodies and their published STLs, and
  `tools/stlcmp.py`, which reports an STL's volume and bounding box.
- `tools/export_freecad.py` and `just compare-freecad`, which re-export every
  body from `Mass Balance.FCStd` under `freecadcmd` and compare the exports
  with the published STLs. Top, Bed and Cover match; the published `Base.stl`
  is 1.25 % smaller than the document's base and predates its last save
  (`cad/README.md`).
- `README.md`, `LICENSING.md` with the licence texts, `CITATION.cff`, and the
  FAST template's editor and line-ending configuration.

#### Changed

- The library moved to `firmware/MOST_MassBalance/`, its Fritzing circuit to
  `electronics/` and its protocol notes to `docs/serial-protocol/`; the OSF
  files sorted into `cad/`, `firmware/`, `electronics/` and `docs/`. These
  commits only move files.
- The STL archive is stored as its four files.
- Line endings normalized to LF.

#### Fixed

- Both sketches included their headers with backslashes, so they compiled only
  on Windows. With forward slashes they build everywhere.
- The 2020 sketch included its bundled libraries at the paths its git
  submodules had (`HX711/src/HX711.h`), which the OSF upload does not have.
  The includes now point at the files as published.

With those changes, the 2020 sketch and the library's example both build for an
Arduino Nano with arduino-cli.

#### Removed

- The OSF copies of the Fritzing circuit and the two protocol notes, which
  duplicate the upstream files byte for byte, and the sketch's stale
  `.gitmodules` and `.gitignore`.
- The operator manuals of the two Denver Instruments balances the paper
  compared against, and the Scale Manufacturers Association's protocol tester
  program: third-party material that is not part of the design.
- The 370 MB use video was never committed; it is attached to the first
  release instead.
