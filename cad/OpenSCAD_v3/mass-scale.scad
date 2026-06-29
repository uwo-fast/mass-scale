

$fn = $preview ? 32 : 128;
z_fight = $preview ? 0.05 : 0;

// dimensions of the main body
test_length = 150;
test_width = 100;
test_height = 30;

// radius of the corners
test_corner_radius = 15;

// thickness of the walls and top
test_wall_thickness = 3;

// angle of the front bevel
test_bevel_angle = 35;

// Show the platter and enclosure
show_enclosure = true;

// Show cross section view for testing
show_enclosure_cross_section = false;

// ---------------------------------------------------------------

if (show_enclosure)
  cross_section(show_enclosure_cross_section)
    enclosure(
      length=test_length, width=test_width, height=test_height, corner_radius=test_corner_radius,
      wall_thickness=test_wall_thickness, bevel_angle=test_bevel_angle
    );

// ---------------------------------------------------------------

// TODO: make these parameters through a look up table scheme or similar for different LCD sizes
// LCD dimensions and hole placements
lcd_height = 24.5;
lcd_width = 71.5;
lcd_backlight_depth = 3;
lcd_backlight_width = 8;
lcd_backlight_height = 18;
lcd_holes_height_dist = 30;
lcd_holes_width_dist = 74.5;
lcd_hole_diameter = 3.1;

module enclosure(length, width, height, corner_radius, wall_thickness, bevel_angle) {

  minkowskiDim = [corner_radius, corner_radius, 0];
  oDim = [length, width, height];
  iDim = oDim - [wall_thickness * 2, wall_thickness * 2, 0];

  // main body
  body(minkowskiDim, oDim, iDim, wall_thickness, bevel_angle);

  // front bevel
  //front_bevel(minkowskiDim, oDim, wall_thickness, bevel_angle);
}

lip_gap = 0.8;

module body(minkowskiDim, oDim, iDim, wall_thickness, bevel_angle) {

  union() {
    difference() {

      difference() {

        // Main body
        minkowski() {
          translate([0, 0, oDim[2] / 2])
            cube(oDim - minkowskiDim * 2, center=true);
          cylinder(r=minkowskiDim[0], h=1);
        }

        // Hollow out the inside
        minkowski() {
          translate([0, 0, wall_thickness + oDim[2] / 2])
            cube(iDim - minkowskiDim * 2, center=true);
          cylinder(r=minkowskiDim[0], h=1);
        }
      }

      // Slice off top for a consistent height post-minkowski
      translate([-z_fight, -z_fight, oDim[2] * 1.5])
        cube([oDim[0] * 2, oDim[1] * 2, oDim[2]], center=true);

      // Front bevel cut off
      translate([-oDim[0] / 2, 0, wall_thickness])
        rotate([0, -bevel_angle, 0])
          translate([oDim[2], 0, oDim[2]])
            cube([oDim[2] * 2, oDim[1] + z_fight, oDim[2] * 2], center=true);
    }
    for (i = [0, 1])
      mirror([0, i, 0]) {
        translate([-oDim[1] / 3, wall_thickness - oDim[1] / 2, oDim[2] - wall_thickness - lip_gap])
          rotate([0, 90, 0])
            linear_extrude(height=oDim[1] * 2 / 3)
              polygon(points=[[0, 0], [wall_thickness, 0], [0, wall_thickness]]);

        translate([oDim[1] / 3, wall_thickness - oDim[1] / 2, oDim[2] - wall_thickness])
          rotate([0, 270, 0])
            linear_extrude(height=oDim[1] * 2 / 3)
              polygon(points=[[0, 0], [wall_thickness, 0], [0, wall_thickness]]);
      }
  }
}

module front_bevel(minkowskiDim, oDim, wall_thickness, bevel_angle) {

  difference() {

    // Front bevel face
    intersection() {
      translate([minkowskiDim[0], minkowskiDim[1], 0])
        minkowski() {
          cube(oDim - minkowskiDim * 2);
          cylinder(r=minkowskiDim[0], h=1);
        }

      rotate([0, -bevel_angle, 0])
        translate([0, 0, wall_thickness * cos(bevel_angle)])
          cube([oDim[2] * 2, oDim[1], wall_thickness]);
    }

    // LCD cut out on bevel
    translate([lcd_height / 2, (oDim[1] - lcd_width) / 2, wall_thickness * 2])
      rotate([0, -bevel_angle, 0])
        cube([lcd_height, lcd_width, oDim[2]]);

    // LCD cut out on bevel
    translate([lcd_backlight_height * 3 / 4, (oDim[1] - lcd_width - lcd_backlight_width) / 2, wall_thickness + lcd_backlight_height / 4])
      rotate([0, -bevel_angle, 0])
        cube([lcd_backlight_height, lcd_width + lcd_backlight_width, wall_thickness + 0.4 + lcd_backlight_depth]);

    // LCD cut out on bevel
    translate([lcd_height / 2, (oDim[1] - lcd_width) / 2, wall_thickness * 2])
      rotate([0, -bevel_angle, 0])for (x = [-1, 1], y = [-1, 1]) {
        translate([lcd_height / 2 + x * lcd_holes_height_dist / 2, lcd_width / 2 + y * lcd_holes_width_dist / 2, wall_thickness * 0])
          cylinder(h=oDim[2] * 2, r=lcd_hole_diameter / 2, $fn=16);
      }
  }
}

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
