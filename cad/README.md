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
(`just stl-stats` prints them). The STLs are the author's exports; CI does not
re-export them, because it has no FreeCAD.

## How the published STLs compare with the source

`just compare-freecad` runs [`../tools/export_freecad.py`](../tools/export_freecad.py)
under `freecadcmd`, FreeCAD's headless interpreter: it opens
`Mass Balance.FCStd`, recomputes it, reports any object that does not reach the
up-to-date state, writes each body to `build/cad/freecad/` as binary STL with
`MeshPart.meshFromShape` at a 0.01 mm linear deflection, and compares each
export with the published file by volume and bounding box
(`tools/stlcmp.py`). It needs FreeCAD on the machine (`FREECADCMD` names the
interpreter when it is not on `PATH`); tessellations differ between exports,
so a re-export is compared by volume and bounding box, not by hash.

With FreeCAD 1.1.3, the document (saved by 0.18R4) opens with one deprecation
notice, that the feature on `Sketch001` sets the old `Midplane` property in
place of `SideType`, and a full recompute leaves every object up to date. The
recomputed bodies have the same volume as the shapes saved in the file, to four
decimals of a percent, so what follows compares the saved design with the
published STLs:

| Body | Published STL | Export | Published | Difference |
| --- | --- | --- | --- | --- |
| Base | `Base.stl` | 63300.7 mm³, 110 × 155 × 30 mm | 62517.0 mm³, 110 × 155 × 30 mm | +1.25 % |
| Top | `Top.stl` | 28050.8 mm³, 110 × 125.02 × 7 mm | 28048.5 mm³, 110 × 125.02 × 7 mm | +0.01 % |
| Bed | `Bed.stl` | 28167.5 mm³, 100 × 80 × 14 mm | 28166.1 mm³, 99.98 × 80 × 14 mm | +0.01 % |
| Cover | `Cover.stl` | 14129.6 mm³, 107 × 87 × 22 mm | 14128.0 mm³, 107 × 87 × 22 mm | +0.01 % |

Top, Bed and Cover match to within tessellation. The published `Base.stl` is
784 mm³ smaller than the document's Base body: it carries a 2020-02-18
timestamp in the OSF archive, two days before the document's last save
(2020-02-20, by its own metadata), and the Base body's last four features (the
screen holder, two PCB retainers and the cable-management pad) add 915 mm³
after the USB slot, where the body is 62388 mm³. No single feature boundary reproduces 62517 mm³, so the published
file is an export of an intermediate state of the base, and the document
holds the later design. The bounding boxes are identical, so the difference is
inside the shell.

The paper prints the parts in PLA on a LulzBot TAZ 6 at 0.14 mm layers with 20 %
cubic infill (Table 2 of the paper), and notes that all of them can be printed
without support depending on the finish wanted.
