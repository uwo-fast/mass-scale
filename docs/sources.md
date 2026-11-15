# Sources

Where everything in this repository came from, and what else exists about the
scale that is not copied here. Collected in September 2026. The README's
[History](../README.md#history) says which folders and tags hold each
generation;
[`CHANGELOG.md`](https://github.com/uwo-fast/mass-scale/blob/main/CHANGELOG.md)
says what changed on import, and
[`dev/sources-inventory.md`](https://github.com/uwo-fast/mass-scale/blob/main/dev/sources-inventory.md)
lists the files left out, with checksums where they had them.

## Primary

| What | Where |
| --- | --- |
| Paper, 2020 | [doi:10.3390/instruments4030018](https://doi.org/10.3390/instruments4030018), open access, `CC-BY-4.0`; in [`paper/`](https://github.com/uwo-fast/mass-scale/tree/main/docs/paper); a [preprint](https://www.preprints.org/manuscript/202004.0472/v1) and an [author copy](https://digitalcommons.mtu.edu/michigantech-p/2052) also exist |
| Firmware library, with history | [gitlab.com/mtu-most/most_massbalance](https://gitlab.com/mtu-most/most_massbalance), included here (tags `mtu-instruments-2020`, `mtu-final-2021`, and upstream's own `v3.0.0` to `v3.4.0`) |
| CAD, the paper's sketch, circuit, datasheets and test data | [OSF me9a8](https://osf.io/me9a8), "Open source laboratory logging digital scale" (DOI 10.17605/OSF.IO/ME9A8), imported unchanged in the commit tagged `mtu-osf-2020` |
| Project page, with the breadboard assembly guide | [Appropedia: Open Source Digitally Replicable Lab-Grade Scales](https://www.appropedia.org/Open_Source_Digitally_Replicable_Lab-Grade_Scales), adapted in [`electronics-assembly.md`](electronics-assembly.md) |
| STLs, mirrored | [MyMiniFactory 126135](https://www.myminifactory.com/object/3d-print-open-source-digitally-replicable-lab-grade-scale-126135), linked from Appropedia as the "quick download" (not fetched: the site sits behind a browser check) |
| Use video | "MOST OS Balance.mpeg" on OSF, the paper's Video S1; attached to the [`mtu-osf-2020` release](https://github.com/uwo-fast/mass-scale/releases/tag/mtu-osf-2020) |
| The 2019 project's page | [Appropedia: 3D printed digital balance](https://www.appropedia.org/3D_printed_digital_balance) (its older title, "3-D Printable Digital Balance", redirects there) |

Libraries the firmware uses, with their upstreams: the
[HX711 library](https://github.com/bogde/HX711) by Bogdan Necula (`MIT`),
bundled with the 2020 firmware at 0.7.4 and fetched at 0.7.5 for the current
firmware by `just firmware-libs`; Arduino's
[LiquidCrystal](https://github.com/arduino-libraries/LiquidCrystal)
(`LGPL-2.1-or-later`), bundled with the 2020 sketch at 1.0.7 and fetched at
the same version for the current firmware. The 2020 library's OLED support
uses Adafruit's [SSD1306](https://github.com/adafruit/Adafruit_SSD1306),
[GFX](https://github.com/adafruit/Adafruit-GFX-Library) and
[BusIO](https://github.com/adafruit/Adafruit_BusIO) libraries, installed by
`just firmware-deps` rather than bundled.

The v3 design's load-cell and amplifier models come from
[CameronBrooks11/load-cell-scad](https://github.com/CameronBrooks11/load-cell-scad)
(`GPL-3.0`), copied at commit `c8f224a` (2026-07-01) into
[`cad/openscad-v3/lib/load-cell-scad/`](https://github.com/uwo-fast/mass-scale/blob/main/cad/openscad-v3/lib/load-cell-scad/README.md).

The 2020 firmware's serial interface implements the Scale Manufacturers
Association's SCP-0499 protocol; the standard and the association's load-cell
test guideline are linked from [`datasheets/`](datasheets/README.md).

## Related

- **The 2020 sketch's README** is the 2019 project's README with a few items
  struck through, and the revival's README was the same document updated for the
  serial-driven firmware; the 2019 original is kept at
  [`firmware/os-nano-balance-2019/README.md`](https://github.com/uwo-fast/mass-scale/blob/main/firmware/os-nano-balance-2019/README.md).
- **The upstream repository's earlier home.** The library's history starts on
  GitHub in January 2020 (its merge commits name pull requests there) and moved
  to the MOST group's GitLab in 2020; the GitHub copy no longer exists.
