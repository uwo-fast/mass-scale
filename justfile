# lab-grade-scale — CAD and firmware tasks
# Requires: arduino-cli with the arduino:avr core and the libraries that
# firmware-deps installs, python3, unzip. FreeCAD is not needed for check.

build := "build"
fqbn := "arduino:avr:nano"
lib := "firmware/MOST_MassBalance"

# Every FreeCAD body, with the STL published for it, is listed in cad/parts.tsv.

default:
    @just --list

# Check every FreeCAD document and published STL, and build every firmware target. CI gate.
check: check-cad check-firmware

# Check that each FreeCAD document is an intact archive holding the body cad/parts.tsv names, and that each published STL parses with a positive volume.
check-cad:
    #!/usr/bin/env bash
    set -uo pipefail
    fail=0
    while IFS=$'\t' read -r fcstd body stl; do
      printf '  %-44s ' "$fcstd [$body]"
      if ! unzip -tq "cad/$fcstd" >/dev/null 2>&1; then
        echo "FAIL (not an intact FreeCAD archive)"; fail=1; continue
      fi
      if ! grep -q "<String value=\"$body\"" <(unzip -p "cad/$fcstd" Document.xml 2>/dev/null); then
        echo "FAIL (no body labelled $body)"; fail=1; continue
      fi
      if [ "$stl" = - ]; then echo ok; continue; fi
      if ! out=$(python3 tools/stlcmp.py --stats "cad/stl/$stl" 2>&1); then
        echo "FAIL ($stl)"; head -1 <<<"$out" | sed 's/^/      /'; fail=1; continue
      fi
      vol=${out%% mm3*}
      if ! awk -v v="$vol" 'BEGIN { exit !(v > 0) }'; then
        echo "FAIL ($stl encloses no volume)"; fail=1; continue
      fi
      echo "ok  $stl: $out"
    done < <(grep -v '^#' cad/parts.tsv)
    exit $fail

# Build the paper's DigitalMassBalance sketch and the MOST_MassBalance library's example for an Arduino Nano.
check-firmware:
    #!/usr/bin/env bash
    set -uo pipefail
    tmp=$(mktemp -d); trap 'rm -rf "$tmp"' EXIT
    fail=0
    build() {  # label, sketch directory
      printf '  %-44s ' "$1"
      if out=$(arduino-cli compile --fqbn {{fqbn}} --build-path "$tmp/build-$1" "$2" 2>&1); then
        echo ok; grep -E 'Sketch uses|Global variables' <<<"$out" | sed 's/^/      /'
      else
        echo FAIL; grep -E 'error' <<<"$out" | head -3 | sed 's/^/      /'; fail=1
      fi
    }
    build DigitalMassBalance firmware/DigitalMassBalance
    # The library builds the way its README describes: its directory is the
    # sketch folder, with the example copied in as a sketch named after it.
    cp -r {{lib}} "$tmp/MOST_MassBalance"
    cp {{lib}}/examples/DigitalMassBalance/DigitalMassBalance.ino "$tmp/MOST_MassBalance/MOST_MassBalance.ino"
    build MOST_MassBalance "$tmp/MOST_MassBalance"
    exit $fail

# Install the pinned Arduino core and the display libraries the MOST_MassBalance library needs.
firmware-deps:
    arduino-cli core install arduino:avr@1.8.6
    arduino-cli lib install "LiquidCrystal@1.0.7" "Adafruit SSD1306@2.5.17" "Adafruit GFX Library@1.12.6" "Adafruit BusIO@1.17.4"

# Build both firmware targets and leave the .hex files under build/firmware/.
firmware:
    #!/usr/bin/env bash
    set -euo pipefail
    tmp=$(mktemp -d); trap 'rm -rf "$tmp"' EXIT
    mkdir -p {{build}}/firmware
    arduino-cli compile --fqbn {{fqbn}} --build-path "$tmp/dmb" --output-dir {{build}}/firmware/DigitalMassBalance firmware/DigitalMassBalance
    cp -r {{lib}} "$tmp/MOST_MassBalance"
    cp {{lib}}/examples/DigitalMassBalance/DigitalMassBalance.ino "$tmp/MOST_MassBalance/MOST_MassBalance.ino"
    arduino-cli compile --fqbn {{fqbn}} --build-path "$tmp/mmb" --output-dir {{build}}/firmware/MOST_MassBalance "$tmp/MOST_MassBalance"
    ls {{build}}/firmware/*/*.hex

# Print volume, bounding box and triangle count of every published STL.
stl-stats:
    #!/usr/bin/env bash
    set -euo pipefail
    for f in cad/stl/*.stl; do printf '  %-12s ' "$(basename "$f")"; python3 tools/stlcmp.py --stats "$f"; done

clean:
    rm -rf {{build}}
