# Test data

The measurements behind the paper's Section 3, as published on the
[OSF project](https://osf.io/me9a8) (`Tests/`, `Logs/` and the plots in
`Documentation/`). Section 2.6 of the paper describes the procedures.

## Serial logs

PuTTY captures of the scale's serial output, kept byte for byte (they mix CRLF
and bare CR line endings). Each begins with the scale's start-up messages and
the sensitivity read from EEPROM, then the weight reports.

| File | Capture | Test |
| --- | --- | --- |
| `200623 - 100g.log` | 2020-06-23 | standard masses, 1 g to 100 g, on the 100 g TAL221 |
| `200623 - 500g.log` | 2020-06-23 | standard masses on the 500 g TAL221 |
| `200623 - 5000g.log` | 2020-06-23 | standard masses on the 5 kg TAL220 |
| `200624 - 5000g (PLA).log` | 2020-06-24 | the 5 kg TAL220 in the printed PLA housing |
| `200624 - 5000g (wood).log` | 2020-06-24 | the 5 kg TAL220 on a pine frame, for comparison with the housing |
| `20200121.log` | 2020-01-21 | an earlier session, with the firmware before the SMA serial interface |
| `20200221.log` | 2020-02-21 | an earlier session; the spreadsheet's comparison sheets carry the same date |

The standard-mass captures are the "second set of tests" in the paper, logged
at 1 Hz in continuous-report mode (`<R>`) with a 10-value sliding average, each
mass left on the bed for about 30 s, after an hour's warm-up.

## Spreadsheet

`Balance Shootout.xlsx` (last changed 2020-06-25) holds the processed data, one
sheet per test: the standard-mass tests above (`200623 - 100g Standard Mass`,
`200623 - 500g Standard Mass`, `200623 - 5000g Standards PLA`,
`200624 - 5000g Standards Wood`), the comparisons against the Denver A-160
analytical balance (`200221 - 5000g vs A-160 LC1`, `200221 - 500g vs A-160 LC1`
and `LC2`, `200323 - 500g vs A-160`), the self-calibration of the 100 g cell
against the 500 g cell (`200414 - 100g vs WAOAW and 500g`), and `200121`.

## Plots

`plots/` holds the six figures of the comparison tests (2020-04-15): for each
load cell, the standard deviation of five repeated readings at each mass
(`* Std.png`) and the absolute difference from the reference balance
(`* Avg Diff.png`): the quantities the paper plots in Figures 10 to 12.

| Load cell | Files | Result in the paper |
| --- | --- | --- |
| 5 kg TAL220 | `5kg Std.png`, `5kg Avg Diff.png` | σ 0.0163 g on average, 0.0363 g from the A-160 |
| 500 g TAL221 | `500g Std.png`, `500g Avg Diff.png` | σ 0.0207 g, 0.0142 g from the A-160 |
| 100 g TAL221 | `100g Std.png`, `100g Avg Diff.png` | σ 0.005 g, 0.0198 g from the 500 g TAL221 it was calibrated against |
