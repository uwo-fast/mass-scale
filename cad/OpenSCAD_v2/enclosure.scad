include <_settings.scad>;
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
      // Front bevel cutout.
      translate([0, -z_fight / 2, wall_thickness * 2]) rotate([0, -bevel_angle, 0])
          cube([oDim[2] * 2, oDim[1] + z_fight, oDim[2] * 2]);
    }

    // front bevel
    intersection() {
      translate([minkowskiDim[0], minkowskiDim[1], 0]) minkowski() {
          cube(oDim - minkowskiDim * 2);
          cylinder(r=minkowskiDim[0], h=1);
        }

      rotate([0, -bevel_angle, 0]) cube([oDim[2] * 2, oDim[1], wall_thickness * 2]);
    }
  }
}
