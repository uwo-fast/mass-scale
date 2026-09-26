// Default load-cell calibration used until a value is stored in EEPROM.
// default_scale is the sensitivity in HX711 divisions per gram. This is just a
// placeholder inherited from the original hardware; calibrate with the 'c'
// serial command, which saves the result to EEPROM (it survives power cycles).
// The offset (zero point) is not stored: the firmware tares at boot instead.

#define default_scale 0.016016