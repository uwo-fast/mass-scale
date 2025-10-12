include <_settings.scad>;

module platter(length, width, height, corner_radius, wall_thickness, top_thickness) {
  minkowskiDim = [corner_radius, corner_radius, 0];
  oDim = [length, width, height];
  iDim = oDim - [wall_thickness * 2, wall_thickness * 2, 0];

  union() {
    translate([minkowskiDim[0], minkowskiDim[1], 0])
      difference() {
        minkowski() {
          cube(oDim - minkowskiDim * 2);
          cylinder(r=minkowskiDim[0], h=1);
        }
        color("lightgrey")
          minkowski() {
            translate([wall_thickness, wall_thickness, top_thickness])
              cube(iDim - minkowskiDim * 2);
            cylinder(r=minkowskiDim[0], h=1);
          }
      }
  }
}
