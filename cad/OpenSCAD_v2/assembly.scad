include <_settings.scad>;

use <enclosure.scad>;
use <platter.scad>;

// dimensions of the main body
test_length = 150;
test_width = 100;
test_height = 30;

// radius of the corners
test_corner_radius = 15;

// thickness of the walls and top
test_wall_thickness = 3;
test_top_thickness = 3;

// angle of the front bevel
test_bevel_angle = 35;

// dimensions of the platter
test_height_platter = 4;
test_wall_thickness_platter = 3;
length_platter = test_length + test_wall_thickness_platter * 2;
width_platter = test_width + test_wall_thickness_platter * 2;
test_allowance_platter = 0.2;

// Show the platter and enclosure
show_platter = true;
show_enclosure = true;

// Show cross section view for testing
show_platter_cross_section = false;
show_enclosure_cross_section = false;

// ---------------------------------------------------------------

if (show_platter)
  cross_section(show_platter_cross_section)
    platter(
      length=length_platter + test_allowance_platter * 2, width=width_platter + test_allowance_platter * 2, height=test_height_platter, corner_radius=test_corner_radius,
      wall_thickness=test_wall_thickness_platter, top_thickness=test_top_thickness
    );

if (show_enclosure)
  translate([test_wall_thickness_platter + test_allowance_platter, test_wall_thickness_platter + test_allowance_platter, test_wall_thickness_platter + test_allowance_platter])
    cross_section(show_enclosure_cross_section)
      enclosure(
        length=test_length, width=test_width, height=test_height, corner_radius=test_corner_radius,
        wall_thickness=test_wall_thickness, top_thickness=test_top_thickness, bevel_angle=test_bevel_angle
      );

// ---------------------------------------------------------------

// Helper module to show cross section
module cross_section(show) {
  // Cross section view
  if (show) {
    difference() {
      children();

      translate([-z_fight, -z_fight, -250])
        cube([test_length / 2, test_width / 2, 500]);
    }
  } else {
    children();
  }
}
