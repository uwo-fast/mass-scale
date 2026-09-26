/**
 * @file load_cell.scad
 * @brief Parametric model of a strain-gauge load cell.
 *
 * (c) 2021 fnh, hendrich@informatik.uni-hamburg.de
 * (c) 2025 ckb, cbrook49@uwo.ca
 *
 * Accessors and drawing module. Variant data is in load_cells.scad.
 * Draw a cell by passing a row: load_cell(LC_TAL220).
 */

//
//! Simple geometric model of a bar strain-gauge load cell: metal body with a
//! strain gauge on the top and bottom faces, cylindrical (and optional
//! rectangular) center cutouts, optional side cuts, and one or more screw-hole
//! groups. The model origin is at the center of the bar, oriented along x.
//

// --- property accessors ------------------------------------------------------
// Each `type` row is:
//   [ name, cell_dims, sensor_dims, center_cutout, screw_holes, side_cuts ]
function lc_name(type)          = type[0]; //! Part name, e.g. "TAL220"
function lc_cell_dims(type)     = type[1]; //! Body dimensions [ length, width, height ]
function lc_sensor_dims(type)   = type[2]; //! Strain-gauge dimensions [ length, width, thickness ]
function lc_center_cutout(type) = type[3]; //! Center cutout [ diameter, [x1,y1], [x2,y2] (, rect_height) ]
function lc_screw_holes(type)   = type[4]; //! List of screw groups, each [ diameter, [Ax,Ay], [Bx,By] ]
function lc_side_cuts(type)     = type[5]; //! Optional side cuts [ length, width ], or undef

module load_cell(type, load_cell_color = "silver", sensor_color = "white")
{ //! Draw the load cell described by `type`
    cell_dims     = lc_cell_dims(type);
    sensor_dims   = lc_sensor_dims(type);
    center_cutout = lc_center_cutout(type);
    screw_holes   = lc_screw_holes(type);
    side_cuts     = lc_side_cuts(type);

    difference()
    {
        union()
        {
            // main body
            color(load_cell_color) cube([ cell_dims[0], cell_dims[1], cell_dims[2] ], center = true);

            // strain gages
            color(
                sensor_color) for (dz =
                                       [
                                           -cell_dims[2] / 2 - sensor_dims[2] / 2, cell_dims[2] / 2 + sensor_dims[2] / 2
                                       ]) translate([ 0, 0, dz ])
                cube([ sensor_dims[0], sensor_dims[1], sensor_dims[2] ], center = true);
        }

        // optional side cutouts
        if (!is_undef(side_cuts[0]))
        {
            for (dy = [ -cell_dims[1] / 2, cell_dims[1] / 2 ])
                hull()
                {
                    translate([ -side_cuts[0] / 2 + 2, dy, 0 ])
                        cylinder(d = side_cuts[1] * 2, h = cell_dims[2] + 1, center = true, $fn = 15);
                    translate([ +side_cuts[0] / 2 - 2, dy, 0 ])
                        cylinder(d = side_cuts[1] * 2, h = cell_dims[2] + 1, center = true, $fn = 15);
                }
        }

        // center cylindrical cutouts
        for (dx = [ center_cutout[1][0], center_cutout[2][0] ])
            translate([ dx, 0, 0 ]) rotate([ 90, 0, 0 ])
                cylinder(d = center_cutout[0], h = cell_dims[1] + 1, center = true, $fn = 50);

        // optional rectangular center cutout
        if (!is_undef(center_cutout[3]))
            cube([ abs(center_cutout[2][0]) + abs(center_cutout[1][0]), cell_dims[0] + 1, center_cutout[3] ],
                 center = true);

        // screw holes
        for (i = [0:len(screw_holes) - 1])
        {
            hole = screw_holes[i];
            translate([ hole[1][0], hole[1][1], 0 ])
                cylinder(d = hole[0], h = 1 + cell_dims[1] + 1, center = true, $fn = 50);
            translate([ hole[2][0], hole[2][1], 0 ])
                cylinder(d = hole[0], h = 1 + cell_dims[1] + 1, center = true, $fn = 50);
        }
    }
}
