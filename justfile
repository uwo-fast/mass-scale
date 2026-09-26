# mass-scale — CAD and firmware tasks
# Requires: openscad, arduino-cli with the arduino:avr core and the libraries
# that firmware-deps installs, curl, python3, unzip. FreeCAD (freecadcmd) only
# for compare-freecad; check does not need it.

build := "build"
fqbn := "arduino:avr:nano"
lib := "firmware/most-massbalance-2020/MOST_MassBalance"
# OPENSCAD names the OpenSCAD command when it is not `openscad` on PATH;
# OPENSCAD_FLAGS adds flags to every render (for example a backend choice).
openscad := env("OPENSCAD", "openscad")
openscad_flags := env("OPENSCAD_FLAGS", "")
# The libraries firmware/mass-scale needs, fetched as release tarballs into
# build/deps/libraries/ and checked against these SHA-256s (firmware-libs).
hx711_version := "0.7.5"
hx711_sha256 := "a6fb961ddb29679bf8ee5ed51cfd9e4fed1fd9f274aabb1b531e4996c3dfb6c1"
liquidcrystal_version := "1.0.7"
liquidcrystal_sha256 := "3641e8f4175ee8c66bbbc7e93961cd8ef333b9cd3689aa83c2512a23ba68200c"

# Every FreeCAD body, with the STL published for it, is listed in
# cad/parts.tsv; every OpenSCAD part to render in cad/scad-parts.tsv.

default:
    @just --list

# Check every FreeCAD document and published STL, render every OpenSCAD part, and build every firmware target. CI gate.
check: check-cad check-scad check-firmware

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
      if ! out=$(python3 tools/stlcmp.py --stats "cad/$stl" 2>&1); then
        echo "FAIL ($stl)"; head -1 <<<"$out" | sed 's/^/      /'; fail=1; continue
      fi
      vol=${out%% mm3*}
      if ! awk -v v="$vol" 'BEGIN { exit !(v > 0) }'; then
        echo "FAIL ($stl encloses no volume)"; fail=1; continue
      fi
      echo "ok  $stl: $out"
    done < <(grep -v '^#' cad/parts.tsv)
    exit $fail

# Render every OpenSCAD part in cad/scad-parts.tsv, failing on any warning, error or deprecation, or an empty result.
check-scad:
    #!/usr/bin/env bash
    # The exit code alone is not a gate: OpenSCAD exits 0 on DEPRECATED, so the
    # output is grepped as well. --hardwarnings makes a WARNING exit non-zero.
    set -uo pipefail
    tmp=$(mktemp -d); trap 'rm -rf "$tmp"' EXIT
    fail=0
    # A part may name one warning it is known to produce (scad-parts.tsv, last
    # column); that warning alone is tolerated, and --hardwarnings is dropped
    # for it so the warning does not abort the render.
    while IFS=$'\t' read -r dir file defs part allow; do
      args=(); [ "$defs" != - ] && for d in ${defs//,/ }; do args+=(-D "$d"); done
      hard=(--hardwarnings); [ "${allow:--}" != - ] && hard=()
      printf '  %-44s ' "$dir/$part"
      out=$(cd "cad/$dir" && {{openscad}} {{openscad_flags}} "${hard[@]}" "${args[@]}" -o "$tmp/out.stl" "$file" 2>&1); rc=$?
      diag=$(grep -E 'ERROR:|WARNING:|DEPRECATED:' <<<"$out" || true)
      [ "${allow:--}" != - ] && diag=$(grep -vE "$allow" <<<"$diag" || true)
      if [ "$rc" -ne 0 ] || [ -n "$diag" ] || [ ! -s "$tmp/out.stl" ]; then
        echo FAIL; head -3 <<<"$diag" | sed 's/^/      /'; fail=1
      elif ! stats=$(python3 tools/stlcmp.py --stats "$tmp/out.stl" 2>&1); then
        echo "FAIL ($stats)"; fail=1
      else
        echo "ok  $stats"
      fi
      rm -f "$tmp/out.stl"
    done < <(grep -v '^#' cad/scad-parts.tsv)
    exit $fail

# Build the current firmware (firmware/mass-scale) against its pinned libraries, and the paper's DigitalMassBalance sketch and the MOST_MassBalance library's example, all for an Arduino Nano.
check-firmware: firmware-libs
    #!/usr/bin/env bash
    set -uo pipefail
    tmp=$(mktemp -d); trap 'rm -rf "$tmp"' EXIT
    libs="$PWD/{{build}}/deps/libraries"
    fail=0
    build() {  # label, sketch directory, extra arduino-cli arguments
      label=$1; sketch=$2; shift 2
      printf '  %-44s ' "$label"
      if out=$(arduino-cli compile --fqbn {{fqbn}} --build-path "$tmp/build-$label" "$@" "$sketch" 2>&1); then
        echo ok; grep -E 'Sketch uses|Global variables' <<<"$out" | sed 's/^/      /'
      else
        echo FAIL; grep -E 'error' <<<"$out" | head -3 | sed 's/^/      /'; fail=1
      fi
    }
    # --library gives the pinned copies priority over any library of the same
    # name installed for the user.
    build mass-scale firmware/mass-scale --library "$libs/HX711" --library "$libs/LiquidCrystal"
    build DigitalMassBalance firmware/most-massbalance-2020/DigitalMassBalance
    # The library builds the way its README describes: its directory is the
    # sketch folder, with the example copied in as a sketch named after it.
    cp -r {{lib}} "$tmp/MOST_MassBalance"
    cp {{lib}}/examples/DigitalMassBalance/DigitalMassBalance.ino "$tmp/MOST_MassBalance/MOST_MassBalance.ino"
    build MOST_MassBalance "$tmp/MOST_MassBalance"
    exit $fail

# Fetch the pinned HX711 and LiquidCrystal releases that firmware/mass-scale builds against into build/deps/libraries/, checking each tarball's SHA-256.
firmware-libs:
    #!/usr/bin/env bash
    set -euo pipefail
    dest="$PWD/{{build}}/deps/libraries"
    fetch() {  # folder, GitHub repository, tag, sha256
      [ -f "$dest/$1/library.properties" ] && return 0
      mkdir -p "$dest"
      curl -sfL -o "$dest/$1.tar.gz" "https://github.com/$2/archive/refs/tags/$3.tar.gz"
      echo "$4  $dest/$1.tar.gz" | sha256sum -c --quiet
      rm -rf "$dest/$1"
      tar -xzf "$dest/$1.tar.gz" -C "$dest"
      mv "$dest/${2#*/}-$3" "$dest/$1"
      echo "  $2 $3 -> build/deps/libraries/$1"
    }
    fetch HX711 bogde/HX711 {{hx711_version}} {{hx711_sha256}}
    fetch LiquidCrystal arduino-libraries/LiquidCrystal {{liquidcrystal_version}} {{liquidcrystal_sha256}}

# Install the pinned Arduino core and the display libraries the MOST_MassBalance library needs, and fetch the libraries firmware/mass-scale builds against.
firmware-deps: firmware-libs
    arduino-cli core install arduino:avr@1.8.6
    arduino-cli lib install "LiquidCrystal@1.0.7" "Adafruit SSD1306@2.5.17" "Adafruit GFX Library@1.12.6" "Adafruit BusIO@1.17.4"

# Build every firmware target and leave the .hex files under build/firmware/.
firmware: firmware-libs
    #!/usr/bin/env bash
    set -euo pipefail
    tmp=$(mktemp -d); trap 'rm -rf "$tmp"' EXIT
    libs="$PWD/{{build}}/deps/libraries"
    mkdir -p {{build}}/firmware
    arduino-cli compile --fqbn {{fqbn}} --build-path "$tmp/ms" --output-dir {{build}}/firmware/mass-scale --library "$libs/HX711" --library "$libs/LiquidCrystal" firmware/mass-scale
    arduino-cli compile --fqbn {{fqbn}} --build-path "$tmp/dmb" --output-dir {{build}}/firmware/DigitalMassBalance firmware/most-massbalance-2020/DigitalMassBalance
    cp -r {{lib}} "$tmp/MOST_MassBalance"
    cp {{lib}}/examples/DigitalMassBalance/DigitalMassBalance.ino "$tmp/MOST_MassBalance/MOST_MassBalance.ino"
    arduino-cli compile --fqbn {{fqbn}} --build-path "$tmp/mmb" --output-dir {{build}}/firmware/MOST_MassBalance "$tmp/MOST_MassBalance"
    ls {{build}}/firmware/*/*.hex

# Render every OpenSCAD part in cad/scad-parts.tsv to build/cad/openscad/<part>.stl.
render:
    #!/usr/bin/env bash
    set -euo pipefail
    mkdir -p {{build}}/cad/openscad
    while IFS=$'\t' read -r dir file defs part allow; do
      args=(); [ "$defs" != - ] && for d in ${defs//,/ }; do args+=(-D "$d"); done
      printf '  %-44s ' "$dir/$part"
      (cd "cad/$dir" && {{openscad}} {{openscad_flags}} "${args[@]}" -o "$OLDPWD/{{build}}/cad/openscad/$part.stl" "$file" 2>&1 | grep -E 'ERROR:|WARNING:|DEPRECATED:' || true)
      python3 tools/stlcmp.py --stats "{{build}}/cad/openscad/$part.stl"
    done < <(grep -v '^#' cad/scad-parts.tsv)

# Re-export every body from its FreeCAD document to build/cad/freecad/ and compare each with its published STL. Needs freecadcmd (FREECADCMD overrides the command); not part of check.
compare-freecad:
    #!/usr/bin/env bash
    set -uo pipefail
    {{env("FREECADCMD", "freecadcmd")}} tools/export_freecad.py; fail=$?
    while IFS=$'\t' read -r fcstd body stl; do
      [ "$stl" = - ] && continue
      n=$(basename "$stl"); printf '  %-8s ' "${n%.stl}"
      python3 tools/stlcmp.py "{{build}}/cad/freecad/$n" "cad/$stl" || fail=1
    done < <(grep -v '^#' cad/parts.tsv)
    exit $fail

# Print volume, bounding box and triangle count of every published STL.
stl-stats:
    #!/usr/bin/env bash
    set -euo pipefail
    for f in cad/freecad-2020/stl/*.stl; do printf '  %-12s ' "$(basename "$f")"; python3 tools/stlcmp.py --stats "$f"; done

clean:
    rm -rf {{build}}
