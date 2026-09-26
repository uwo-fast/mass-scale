
include <_settings.scad>;

use <load-cell-scad/amplifier_boards.scad>;
use <load-cell-scad/load_cell.scad>;

// spacing between objects
spacing = 25;

translate([50, 50, 0]) {
  // load cells
  translate([0, 0, 0]) load_cell("TAL221");
  translate([0, spacing, 0]) load_cell("TAL220B");
  translate([0, spacing * 2, 0]) load_cell("TAL220");
  translate([0, spacing * 3, 0]) load_cell("komputer_10kg");
  translate([0, spacing * 4, 0]) load_cell("komputer_5kg");

  // amplifier boards
  translate([spacing * 2, 0, 0]) generic_hx711_load_cell_amp();
  translate([spacing * 2, spacing, 0]) sparkfun_hx711_load_cell_amp();
}

// TAL221 screw holes
tal221_screw_holes = get_load_cell_screw_holes("TAL221");

//[ [3, [-20, -3], [-20, 3]], [3.2, [20, -3], [20, 3]] ]
echo(tal221_screw_holes[1][0]);
echo();

// REDO THIS AS A CHILD MODULE IN THE LIB
translate([0, tal221_screw_holes[0][1][1], 0]) cylinder(h=10, d=tal221_screw_holes[0][0], center=false);
translate([0, tal221_screw_holes[0][2][1], 0]) cylinder(h=10, d=tal221_screw_holes[0][0], center=false);

translate([10, tal221_screw_holes[1][1][1], 0]) cylinder(h=10, d=tal221_screw_holes[1][0], center=false);
translate([10, tal221_screw_holes[1][2][1], 0]) cylinder(h=10, d=tal221_screw_holes[1][0], center=false);
