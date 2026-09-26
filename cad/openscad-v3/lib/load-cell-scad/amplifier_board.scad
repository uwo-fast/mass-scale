/**
 * @file amplifier_board.scad
 * @brief Parametric model of an HX711-style load-cell amplifier board.
 *
 * (c) 2021 fnh, hendrich@informatik.uni-hamburg.de
 * (c) 2025 ckb, cbrook49@uwo.ca
 *
 * Accessors and drawing module. Variant data is in amplifier_boards.scad.
 * Draw a board by passing a row: amplifier_board(AMP_HX711_generic).
 */

//
//! Generic geometric model of a small load-cell amplifier PCB (e.g. HX711
//! breakout): PCB substrate, chip, one or more pin-header blocks, and optional
//! mounting holes. Board is centered on the origin, lying in the xy-plane.
//

// --- property accessors ------------------------------------------------------
// Each `type` row is:
//   [ name, pcb_dims, colour, chip_dims, headers, mount_holes ]
function amp_name(type)        = type[0]; //! Part name
function amp_pcb(type)         = type[1]; //! PCB dimensions [ length, width, height ]
function amp_colour(type)      = type[2]; //! PCB substrate colour
function amp_chip(type)        = type[3]; //! Chip dimensions [ length, width, height ]
function amp_headers(type)     = type[4]; //! List of header blocks, each [ x, n_pins, block_height ]
function amp_mount_holes(type) = type[5]; //! List of mounting holes, each [ x, y, diameter ]

// header geometry constants (2.54 mm 0.1" pin headers)
pin_pitch  = 2.54;
pin_hole_d = 1.0;
pin_d      = 0.8;

module amplifier_board(type, headers = true, header_pin_length = 8)
{ //! Draw the amplifier board described by `type`
    pcb    = amp_pcb(type);
    chip   = amp_chip(type);
    hdrs   = amp_headers(type);
    mounts = amp_mount_holes(type);
    l = pcb[0];
    w = pcb[1];
    h = pcb[2];

    difference()
    {
        union()
        {
            // main pcb
            color(amp_colour(type)) translate([ 0, 0, h / 2 ]) cube([ l, w, h ], center = true);
            // chip
            color("black") translate([ 0, 0, h + chip[2] / 2 ]) cube(chip, center = true);
            // header blocks
            if (headers)
                for (hd = hdrs)
                    color("black") translate([ hd[0], 0, h + hd[2] / 2 ])
                        cube([ pin_pitch, hd[1] * pin_pitch, hd[2] ], center = true);
        }

        // header pin holes
        for (hd = hdrs)
            for (i = [0:hd[1] - 1])
            {
                dy = (i - (hd[1] - 1) / 2) * pin_pitch;
                translate([ hd[0], dy, -0.5 ]) cylinder(d = pin_hole_d, h = h + hd[2] + 1, center = false, $fn = 20);
            }

        // mounting holes
        for (m = mounts)
            translate([ m[0], m[1], -0.1 ]) cylinder(d = m[2], h = h + 0.2, center = false, $fn = 20);
    } // difference

    // header pins
    if (header_pin_length > 0)
        for (hd = hdrs)
            for (i = [0:hd[1] - 1])
            {
                dy = (i - (hd[1] - 1) / 2) * pin_pitch;
                translate([ hd[0], dy, -0.2 ]) cylinder(d = pin_d, h = header_pin_length, center = false, $fn = 20);
            }
}
