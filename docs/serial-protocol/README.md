# Serial protocol notes

Benjamin Hubbard's working notes for the scale's serial interface, from the
upstream repository (`doc/`, 2020-04-09). They quote the command and response
definitions of the Scale Manufacturers Association's SCP-0499 standard that the
firmware implements, keeping the standard's section numbers, and add the
scale's own extended commands.

| File | Contents |
| --- | --- |
| `Command Formats.txt` | each command the scale accepts: `W` weight, `Z` zero, `T` tare, `M` tare weight, `C` clear tare, `D` diagnostics, `A`/`B` about, `R` repeat, `<ESC>` reset, and the extended `XC` calibration commands |
| `Response Formats.txt` | the response message format and its status characters, and the diagnostics and about responses |

The standard itself is `ScaleCommProtocol5199M1.pdf` in
[`../datasheets/`](../datasheets/README.md). [`../operation.md`](../operation.md)
lists the commands in use.
