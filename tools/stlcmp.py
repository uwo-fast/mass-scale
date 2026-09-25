#!/usr/bin/env python3
"""Compare STL files by enclosed volume and bounding box.

usage: stlcmp.py A.stl B.stl        -> one comparison line
       stlcmp.py --stats X.stl      -> volume, bbox, triangle count
"""
import struct
import sys


def triangles(path):
    data = open(path, "rb").read()
    if len(data) >= 84:
        n = struct.unpack_from("<I", data, 80)[0]
        if 84 + 50 * n == len(data):
            for i in range(n):
                v = struct.unpack_from("<12f", data, 84 + 50 * i)
                yield v[3:6], v[6:9], v[9:12]
            return
    verts = []
    for line in data.decode("latin-1").splitlines():
        parts = line.split()
        if parts[:1] == ["vertex"]:
            verts.append(tuple(float(x) for x in parts[1:4]))
            if len(verts) == 3:
                yield tuple(verts)
                verts = []


def stats(path):
    vol = 0.0
    n = 0
    lo = [float("inf")] * 3
    hi = [float("-inf")] * 3
    for a, b, c in triangles(path):
        n += 1
        vol += (a[0] * (b[1] * c[2] - b[2] * c[1])
                - a[1] * (b[0] * c[2] - b[2] * c[0])
                + a[2] * (b[0] * c[1] - b[1] * c[0])) / 6.0
        for p in (a, b, c):
            for k in range(3):
                lo[k] = min(lo[k], p[k])
                hi[k] = max(hi[k], p[k])
    size = [hi[k] - lo[k] for k in range(3)]
    return abs(vol), size, n


def main():
    if sys.argv[1] == "--stats":
        v, s, n = stats(sys.argv[2])
        print(f"{v:12.1f} mm3  bbox {s[0]:.2f} x {s[1]:.2f} x {s[2]:.2f}  tris {n}")
        return
    va, sa, _ = stats(sys.argv[1])
    vb, sb, _ = stats(sys.argv[2])
    dv = (va - vb) / vb * 100 if vb else float("nan")
    bbox = "same" if all(abs(sorted(sa)[k] - sorted(sb)[k]) < 0.05 for k in range(3)) else \
        f"{sa[0]:.1f}x{sa[1]:.1f}x{sa[2]:.1f} vs {sb[0]:.1f}x{sb[1]:.1f}x{sb[2]:.1f}"
    print(f"vol {va:10.1f} vs {vb:10.1f} ({dv:+.2f} %)  bbox {bbox}")


if __name__ == "__main__":
    main()
