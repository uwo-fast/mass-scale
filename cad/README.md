# CAD

The printed parts of the open-source lab-grade scale, drawn in FreeCAD 0.18 by
Benjamin Hubbard and published on the [OSF project](https://osf.io/me9a8).
Every part locates by features of the prints themselves — bosses and snap
joints — so the only fasteners are the ones that hold the load cell.

| Path | What it is | Source |
| --- | --- | --- |
| [`Mass Balance.FCStd`](Mass%20Balance.FCStd) | the scale, one document with four PartDesign bodies: **Base**, **Top**, **Bed** and **Cover**, driven by a `param` spreadsheet (2020-02-25) | FreeCAD 0.18 |
| [`old/Base.FCStd`](old/Base.FCStd), [`old/Bed.FCStd`](old/Bed.FCStd), [`old/Face.FCStd`](old/Face.FCStd) | the earlier design as three separate documents (2020-01-14): a base holding the electronics and load cell, a bed, and a face carrying the tare button and the LCD | FreeCAD 0.18 |
| [`stl/`](stl/) | the four STLs published with the design (2020-06-29): `Base.stl`, `Top.stl`, `Bed.stl`, `Cover.stl` | exported by the author |

## The bodies

The `param` spreadsheet in `Mass Balance.FCStd` sets the body at
110 × 155 × 30 mm with a 2 mm shell and a 45 mm corner radius, the 40 × 60 mm
solder breadboard's seat, the 72 × 27 mm screen cutout, the 6 mm push-button
opening, the Nano's USB slot, and the boss height and screw spacing for each of
the two load cells (TAL220: 15 mm spacing, 62.5 mm from the bed centre; TAL221:
6 mm spacing, 40 mm from the centre). Change a value there and the four bodies
follow.

By their dimensions, the published STLs are:

| STL | Bounding box (mm) | Part |
| --- | --- | --- |
| `Base.stl` | 110 × 155 × 30 | the base: houses the circuit board and the LCD, and carries the two load-cell bosses |
| `Top.stl` | 110 × 125 × 7 | the lid that snaps onto the base and encloses the electronics ("cover" in the paper's Table 1) |
| `Bed.stl` | 100 × 80 × 14 | the weighing bed, bolted to the free end of the load cell |
| `Cover.stl` | 107 × 87 × 22 | the optional cover that sits over the bed to shield it from air currents, printed in translucent PETG in the paper |

`just check` confirms each document is an intact FreeCAD archive holding the body
[`parts.tsv`](parts.tsv) names, and that each STL encloses a positive volume
(`just stl-stats` prints them). The STLs have not been re-exported here: no
headless FreeCAD is available in the build environment, so they remain the
author's exports. Re-export from FreeCAD (0.18 or later) by selecting a body and
using File → Export.

The paper prints the parts in PLA on a LulzBot TAZ 6 at 0.14 mm layers with 20 %
cubic infill (Table 2 of the paper), and notes that all of them can be printed
without support depending on the finish wanted.
