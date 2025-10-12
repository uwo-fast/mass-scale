include <_settings.scad>;

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
module enclosure(length, width, height, corner_radius, wall_thickness, top_thickness, bevel_angle) {

  minkowskiDim = [corner_radius, corner_radius, 0];
  oDim = [length, width, height];
  iDim = oDim - [wall_thickness * 2, wall_thickness * 2, 0];

  union() {
    // main body
    difference() {

      translate([minkowskiDim[0], minkowskiDim[1], -top_thickness]) difference() {

          minkowski() {
            translate([0, 0, top_thickness]) cube(oDim - minkowskiDim * 2);
            cylinder(r=minkowskiDim[0], h=1);
          }

          color("lightgrey")
            minkowski() {
              translate([wall_thickness, wall_thickness, 0]) cube(iDim - minkowskiDim * 2);
              cylinder(r=minkowskiDim[0], h=1);
            }
        }
      // Front bevel cut off
      translate([0, -z_fight / 2, wall_thickness * 2]) rotate([0, -bevel_angle, 0])
          cube([oDim[2] * 2, oDim[1] + z_fight, oDim[2] * 2]);
    }

    // front bevel
    difference() {
      intersection() {
        translate([minkowskiDim[0], minkowskiDim[1], 0])
          minkowski() {
            cube(oDim - minkowskiDim * 2);
            cylinder(r=minkowskiDim[0], h=1);
          }

        rotate([0, -bevel_angle, 0]) cube([oDim[2] * 2, oDim[1], wall_thickness * 2]);
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
}
