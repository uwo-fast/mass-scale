# Changelog

All notable changes to this project are documented in this file.

The format loosely follows [Keep a Changelog](https://keepachangelog.com/en/1.1.0/)
and [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

Imported into uwo-fast from
[mtu-most/most_massbalance](https://gitlab.com/mtu-most/most_massbalance), with
its full history (the firmware the paper describes is tagged
`mtu-instruments-2020`, upstream's last commit `mtu-final-2021`, and upstream's
own `v3.0.0` to `v3.4.0` tags are kept), and the design files from
[OSF me9a8](https://osf.io/me9a8), imported unchanged in the commit tagged
`mtu-osf-2020`.

### Added

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

### Changed

- The library moved to `firmware/MOST_MassBalance/`, its Fritzing circuit to
  `electronics/` and its protocol notes to `docs/serial-protocol/`; the OSF
  files sorted into `cad/`, `firmware/`, `electronics/` and `docs/`. These
  commits only move files.
- The STL archive is stored as its four files.
- Line endings normalized to LF.

### Fixed

- Both sketches included their headers with backslashes, so they compiled only
  on Windows. With forward slashes they build everywhere.
- The 2020 sketch included its bundled libraries at the paths its git
  submodules had (`HX711/src/HX711.h`), which the OSF upload does not have.
  The includes now point at the files as published.

With those changes, the 2020 sketch and the library's example both build for an
Arduino Nano with arduino-cli.

### Removed

- The OSF copies of the Fritzing circuit and the two protocol notes, which
  duplicate the upstream files byte for byte, and the sketch's stale
  `.gitmodules` and `.gitignore`.
- The operator manuals of the two Denver Instruments balances the paper
  compared against, and the Scale Manufacturers Association's protocol tester
  program: third-party material that is not part of the design.
- The 370 MB use video was never committed; it is attached to the first
  release instead.
