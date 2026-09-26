/**
 * @file amplifier_boards.scad
 * @brief Data table of load-cell amplifier board variants.
 *
 * (c) 2021 fnh, hendrich@informatik.uni-hamburg.de
 * (c) 2025 ckb, cbrook49@uwo.ca
 *
 * Variant data. Each board is a named row, collected into the amplifier_boards
 * list. The drawing module lives in amplifier_board.scad (used at the bottom).
 *
 * Row format:
 *   [ name,
 *     pcb_dims,     // [ length, width, height ]
 *     colour,       // PCB substrate colour
 *     chip_dims,    // [ length, width, height ]
 *     headers,      // list of header blocks, each [ x, n_pins, block_height ]
 *     mount_holes ] // list of mounting holes, each [ x, y, diameter ]
 */

//                        name              pcb_dims             colour        chip_dims      headers                                  mount_holes
// Generic HX711 breakout: 6-pin + 4-pin headers, two M3 mounting bores.
AMP_HX711_generic  = [ "HX711 generic",  [ 33.5, 20.5, 1.5 ], "darkgreen", [ 4.5, 10, 1 ], [ [ -13.75, 6, 2 ], [ 12.25, 4, 2 ] ], [ [ 12.175, 7.675, 3.15 ], [ 12.175, -7.675, 3.15 ] ] ];

// SparkFun HX711 breakout: two symmetric 5-pin headers, no mounting holes.
AMP_HX711_sparkfun = [ "HX711 SparkFun", [ 31.2, 23.4, 1.6 ], "darkred",   [ 4.5, 10, 1 ], [ [ -14.33, 5, 5.05 ], [ 14.33, 5, 5.05 ] ], [] ];

amplifier_boards = [ AMP_HX711_generic, AMP_HX711_sparkfun ];

use <amplifier_board.scad>
