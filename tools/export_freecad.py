#!/usr/bin/env freecadcmd
"""Re-export every body cad/parts.tsv names from its FreeCAD document, so the
exports can be compared with the published STLs.

Run from the repository with FreeCAD's headless interpreter:

    freecadcmd tools/export_freecad.py

Each document is opened and recomputed; any object that does not reach the
Up-to-date state is reported. Each body with a published STL is then
tessellated with MeshPart.meshFromShape at a 0.01 mm linear deflection and
written as binary STL to build/cad/freecad/<stl file name>. Exits 1 if a document did
not recompute cleanly, after writing every export it could.
"""
import os
import sys

import FreeCAD
import MeshPart

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
OUT = os.path.join(ROOT, "build", "cad", "freecad")

LINEAR_DEFLECTION = 0.01  # mm
ANGULAR_DEFLECTION = 0.1  # rad


def parts():
    """(document, body label, published STL) rows of cad/parts.tsv with an STL."""
    with open(os.path.join(ROOT, "cad", "parts.tsv")) as f:
        for line in f:
            if line.startswith("#") or not line.strip():
                continue
            fcstd, body, stl = line.rstrip("\n").split("\t")
            if stl != "-":
                yield fcstd, body, stl


def export(doc, label, stl):
    bodies = [o for o in doc.Objects if o.TypeId == "PartDesign::Body" and o.Label == label]
    if len(bodies) != 1:
        print("  no single body labelled %s" % label)
        return False
    shape = bodies[0].Shape
    mesh = MeshPart.meshFromShape(Shape=shape, LinearDeflection=LINEAR_DEFLECTION,
                                  AngularDeflection=ANGULAR_DEFLECTION, Relative=False)
    name = os.path.basename(stl)
    mesh.write(os.path.join(OUT, name))
    bb = mesh.BoundBox
    print("  %-8s %9.1f mm3  bbox %.2f x %.2f x %.2f  tris %d  -> build/cad/freecad/%s" % (
        label, shape.Volume, bb.XLength, bb.YLength, bb.ZLength, mesh.CountFacets, name))
    return True


os.makedirs(OUT, exist_ok=True)
ok = True
docs = {}
for fcstd, body, stl in parts():
    if fcstd not in docs:
        doc = FreeCAD.openDocument(os.path.join(ROOT, "cad", fcstd))
        doc.recompute()
        invalid = [o for o in doc.Objects if "Invalid" in o.State]
        touched = [o for o in doc.Objects if "Touched" in o.State and "Invalid" not in o.State]
        for o in invalid:
            print("  %s: %s %s failed to recompute" % (fcstd, o.TypeId, o.Name))
        if touched:
            print("  %s: %d objects were not recomputed and keep their saved shape"
                  % (fcstd, len(touched)))
        ok = ok and not (invalid or touched)
        docs[fcstd] = doc
    ok = export(docs[fcstd], body, stl) and ok
for doc in docs.values():
    FreeCAD.closeDocument(doc.Name)
sys.stdout.flush()
sys.exit(0 if ok else 1)
