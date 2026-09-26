# The 2019 FreeCAD parts

Benjamin Hubbard's OS Nano Balance as three FreeCAD 0.18 documents, one per
printed part, driven by the values in `Parameters.csv`: a base holding the
electronics and the load cell, a bed, and a face carrying the tare button and
the LCD. This is the design the 2020 document (`../freecad-2020/`) grew out of.

Two revisions of the base exist, and both are kept:

| File | Revision | From |
| --- | --- | --- |
| `Base.FCStd` | last saved 2019-12-04 | [OS Nano Balance](https://github.com/brhubbar/OS_Nano_Balance), commit `082e950` "Finalize bed and base cad" (tag `mtu-nanobalance-2019`) |
| `Base-osf.FCStd` | last saved 2020-01-14, a later revision of the same document | the "old" parts on [OSF me9a8](https://osf.io/me9a8) (tag `mtu-osf-2020`) |
| `Bed.FCStd`, `Face.FCStd` | last saved 2019-12-04 | both sources, byte for byte the same file |
| `Parameters.csv` | 2019-12-04 | OS Nano Balance; the values the three documents were drawn to |

The dates are the documents' own `LastModifiedDate`. `just check` confirms
each document is an intact archive holding the body named in
[`../parts.tsv`](../parts.tsv).
