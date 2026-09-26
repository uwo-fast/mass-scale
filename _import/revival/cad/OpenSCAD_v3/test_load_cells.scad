// Example of using load-cell-scad library
spacing = 25;

// Load cells

include <load-cell-scad/load_cells.scad>;

translate([0, spacing * 0, 0]) load_cell(LC_TAL221);
translate([0, spacing * 1, 0]) load_cell(LC_TAL220B);
translate([0, spacing * 2, 0]) load_cell(LC_TAL220);
translate([0, spacing * 3, 0]) load_cell(LC_komputer_10kg);
translate([0, spacing * 4, 0]) load_cell(LC_komputer_5kg);

// Amplifier boards

include <load-cell-scad/amplifier_boards.scad>;

translate([0, -spacing, 0]) amplifier_board(AMP_HX711_generic);
translate([0, -spacing * 2, 0]) amplifier_board(AMP_HX711_sparkfun);
