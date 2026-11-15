# Files left out of the imports

What the 2019, 2020 and revival sources held that this repository does not
keep, and why. [`docs/sources.md`](../docs/sources.md) says where each source
is.

- **The use video**, `Documentation/MOST OS Balance.mpeg` on OSF: 370,255,096
  bytes, MPEG-2 program stream, MD5 `66f02c4be425f369919c497e4ba0343c`. Too
  large for the repository; attached to the `mtu-osf-2020` release instead:
  <https://github.com/uwo-fast/mass-scale/releases/tag/mtu-osf-2020>.
- **The Denver Instruments manuals** for the A-160 analytical balance and the
  APX402 balance the paper compared against (`Denver A-160 Operators
  Manual.pdf`, MD5 `16764d6581afa8f9837330ee14bca2db`; `Denver APX402
  Operators Manual.pdf`, MD5 `1d14f42e87e6531837753cf2fd0bd2c6`): third-party
  documents about instruments that are not part of the design. The paper cites
  them at denverinstrument.com (references 90 and 91). Reachable through the
  `mtu-osf-2020` tag.
- **The SMA protocol tester**, `SCPTesterProgram.zip` (a 2004 Windows
  installer, MD5 `8c11f268e9889f2dac8af41f9a3bb681`) and
  `SCPTesterDocumentation.pdf` (MD5 `549957656531bd4c33909fb9253603c0`): the
  Scale Manufacturers Association's own test program, used to check the
  firmware's compliance. Reachable through the `mtu-osf-2020` tag.
- **The SMA standard and test guideline**, `ScaleCommProtocol5199M1.pdf` and
  `loadcellapplicationtestguidelineapril2010.pdf`: the association's
  publications, linked from
  [`docs/datasheets/`](../docs/datasheets/README.md) instead. Reachable through the `mtu-osf-2020` tag.
- **Duplicates.** The OSF copies of the Fritzing circuit and the two protocol
  notes, byte for byte the upstream repository's (after line-ending
  normalisation for the notes), and the STL zip, whose four files are stored
  unpacked. The revival's copies of the 2019 FreeCAD parts and of the 2020
  document, and the 2019 and revival copies of the datasheets, byte for byte
  the files kept. Reachable through the `mtu-osf-2020`, `mtu-nanobalance-2019`
  and `fast-revival-2026` tags.
- **Git metadata** of the imports: the 2019 sketch's HX711 submodule (the
  library is a build dependency instead), the sketch's stale `.gitmodules`
  and `.gitignore` on OSF, and the 2019 and revival `LICENSE`, `.gitignore`
  and `.gitattributes` files, which the repository-level files replace.
