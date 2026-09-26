/**
 * @file load_cells.scad
 * @brief Data table of standard load-cell variants.
 *
 * (c) 2021 fnh, hendrich@informatik.uni-hamburg.de
 * (c) 2025 ckb, cbrook49@uwo.ca
 *
 * Variant data. Each variant is a named row, collected into the load_cells
 * list. The drawing module lives in load_cell.scad (used at the bottom).
 *
 * Row format:
 *   [ name,
 *     cell_dims,      // [ length, width, height ]
 *     sensor_dims,    // [ length, width, thickness ]
 *     center_cutout,  // [ diameter, [x1,y1], [x2,y2] ] ; optional 4th = rect-cut height between holes
 *     screw_holes,    // list of groups, each [ diameter, [Ax,Ay], [Bx,By] ]
 *     side_cuts ]     // [ length, width ], or undef
 */

//                     name              cell_dims           sensor    center_cutout             screw_holes                                              side_cuts
// komputer_10kg: same body as SparkFun, different screw positions; both sides M4 (despite naming).
LC_komputer_10kg = [ "komputer_10kg",  [ 75.2, 12.7, 12.7 ], [ 24, 12, 1 ], [ 12.0, [ 4, 0 ], [ -4, 0 ] ], [ [ 4, [ -31.6, 0 ], [ -21.6, 0 ] ], [ 4, [ 21.6, 0 ], [ 31.6, 0 ] ] ], undef ];

// komputer_5kg: left side M5, right side M4; bore spacing 5.1 | 15 | 40 | 15 | 5.1.
LC_komputer_5kg  = [ "komputer_5kg",   [ 80.2, 12.7, 12.7 ], [ 24, 12, 1 ], [ 12.0, [ 4, 0 ], [ -4, 0 ] ], [ [ 5, [ -35, 0 ], [ -20, 0 ] ], [ 4, [ 20, 0 ], [ 35, 0 ] ] ],       undef ];

// TAL220 (SparkFun 10kg): left side M5, right side M4.
LC_TAL220        = [ "TAL220",         [ 80, 12.7, 12.7 ],   [ 24, 12, 1 ], [ 12.0, [ 4, 0 ], [ -4, 0 ] ], [ [ 5, [ -35, 0 ], [ -25, 0 ] ], [ 4, [ 25, 0 ], [ 35, 0 ] ] ],       undef ];

// TAL220B: single M5 pair at +/-20.
LC_TAL220B       = [ "TAL220B",        [ 55, 12.7, 12.7 ],   [ 24, 12, 1 ], [ 12.0, [ 4, 0 ], [ -4, 0 ] ], [ [ 5, [ -20, 0 ], [ 20, 0 ] ] ],                                    undef ];

// TAL221 (SparkFun 100g): rectangular center cut (4th cutout field) + side cuts; left M3 through, right M3.2 through.
LC_TAL221        = [ "TAL221",         [ 47, 12, 6 ],        [ 30, 6, 1 ],  [ 5.3, [ 10, 0 ], [ -10, 0 ], 3 ], [ [ 3.0, [ -20, -3 ], [ -20, 3 ] ], [ 3.2, [ 20, -3 ], [ 20, 3 ] ] ], [ 33, 2 ] ];

load_cells = [ LC_komputer_10kg, LC_komputer_5kg, LC_TAL220, LC_TAL220B, LC_TAL221 ];

use <load_cell.scad>
