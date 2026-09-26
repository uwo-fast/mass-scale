# CAD

Every generation of the scale's printed parts, one directory each. The
current design is [`openscad/`](openscad/); the rest are kept as published,
for reference, and are still checked.

| Directory | What it is | Tool | Checked by |
| --- | --- | --- | --- |
| [`openscad/`](openscad/) | **The current design (2025–26, OpenSCAD v2):** `enclosure.scad`, a 150 × 100 × 30 mm shell with 15 mm corner radii, 3 mm walls and a 35° front bevel carrying the cutout, backlight recess and four mounting holes of a 16 × 2 LCD; `platter.scad`, the plate that fits over it with a 0.2 mm allowance; `assembly.scad`, which places both and sets the dimensions; `_settings.scad` (`$fn`, z-fighting offset); `lib/trapezium.scad`, a helper from the lab's other designs, not yet used. Printed in October 2025 with the walls thickened for a 0.8 mm nozzle | OpenSCAD | `just check-scad` renders the enclosure, the platter and the assembly |
| [`openscad-v3/`](openscad-v3/) | **In progress (2026-06):** `mass-scale.scad`, the enclosure re-centred on the origin with a sliding lid, guide channels and latch posts, and the LCD bevel; `test_load_cells.scad`, an exercise of the load-cell and amplifier-board models in `lib/load-cell-scad/` (vendored, see [its README](openscad-v3/lib/load-cell-scad/README.md)), which `mass-scale.scad` does not use yet. What is still missing is listed in the repository's issues | OpenSCAD | `just check-scad` renders the enclosure, the lid and the load-cell exercise |
| [`openscad-v1/`](openscad-v1/) | the 2019 OpenSCAD parts reformatted by the revival (2025), without `Cover.scad` and with the snap joints of the base commented out | OpenSCAD | not rendered by `just check` (they render without warnings) |
| [`freecad-2020/`](freecad-2020/) | **The paper's design (2020):** `Mass Balance.FCStd`, one document with four PartDesign bodies, **Base**, **Top**, **Bed** and **Cover**, driven by a `param` spreadsheet, and `stl/`, the four STLs published with it (2020-06-29) | FreeCAD 0.18 | `just check-cad`; `just compare-freecad` re-exports the bodies |
| [`freecad-2019/`](freecad-2019/) | Benjamin Hubbard's three FreeCAD parts (base, bed, face) with their parameter sheet, in both revisions of the base ([which is which](freecad-2019/README.md)) | FreeCAD 0.18 | `just check-cad` |
| [`openscad-2019/`](openscad-2019/) | Hubbard's original OpenSCAD parts (November 2019): `Base.scad`, `Bed.scad`, `Cover.scad`, with `LoadCellRx.scad` (receivers for the TAL220 and TAL221 with M4 or M5 screws), `TAL220.scad`, `TAL221.scad` and `snap_joint.scad`; they accommodate a TAL220 or TAL221 but no LCD | OpenSCAD | not rendered by `just check` (they render without warnings) |

`parts.tsv` lists every FreeCAD body with its published STL, and
`scad-parts.tsv` every OpenSCAD part that `just check-scad` renders and
`just render` writes to `build/cad/openscad/`. `OPENSCAD` names the OpenSCAD
command when it is not on `PATH`; `OPENSCAD_FLAGS` adds flags to every render.

## The current design

`assembly.scad` is the file to open. Its `test_*` variables set the body
(150 × 100 × 30 mm, 15 mm corner radius, 3 mm walls and top, 35° bevel) and
the platter (4 mm high, 3 mm walls, 0.2 mm allowance around the body), and
`show_platter`, `show_enclosure` and the two `*_cross_section` flags choose
what is drawn. The LCD's dimensions and hole spacing are at the top of
`enclosure.scad` (24.5 × 71.5 mm window, holes 30 × 74.5 mm apart, 3.1 mm),
with a note to turn them into a lookup table for other displays. Neither file
models the load cell's mounting or the electronics' seats, which the 2020
design has; see the repository's issues.

Render from the command line, from `cad/openscad/`:

```sh
openscad -D show_platter=false   -o enclosure.stl assembly.scad
openscad -D show_enclosure=false -o platter.stl   assembly.scad
```

or `just render` for every listed part.

## The 2020 design

The `param` spreadsheet in `freecad-2020/Mass Balance.FCStd` sets the body at
110 × 155 × 30 mm with a 2 mm shell and a 45 mm corner radius, the 40 × 60 mm
solder breadboard's seat, the 72 × 27 mm screen cutout, the 6 mm push-button
opening, the Nano's USB slot, and the boss height and screw spacing for each of
the two load cells (TAL220: 15 mm spacing, 62.5 mm from the bed centre; TAL221:
6 mm spacing, 40 mm from the centre). Change a value there and the four bodies
follow. Every part locates by features of the prints themselves, bosses and
snap joints, so the only fasteners are the ones that hold the load cell.

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

### How the published STLs compare with the source

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
after the USB slot, where the body is 62388 mm³. No single feature boundary
reproduces 62517 mm³, so the published file is an export of an intermediate
state of the base, and the document holds the later design. The bounding boxes
are identical, so the difference is inside the shell.

The paper prints the parts in PLA on a LulzBot TAZ 6 at 0.14 mm layers with 20 %
cubic infill (Table 2 of the paper), and notes that all of them can be printed
without support depending on the finish wanted.
