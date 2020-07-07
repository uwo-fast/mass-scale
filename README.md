# MOST_MassBalance
This is the firmware behind 
[DigitalMassBalance](https://github.com/brhubbar/DigitalMassBalance)
which was developed as a serially enabled, lab-grade digital scale. Work
on this scale has been published on the 
[Open Science Framework (OSF)](https://osf.io/me9a8/)
and by MDPI in their 
[Instruments Journal](https://www.mdpi.com/2410-390X/4/3/18)
This builds on the 
[OS Nano Balance](https://www.appropedia.org/3-D_Printable_Digital_Balance)
project and is briefed in a new page on 
[Appropedia](https://www.appropedia.org/Open_Source_Digitally_Replicable_Lab-Grade_Scales).

Since publishing in MDPI, the firmware has been reworked to move all scale
functionality to a library, facilitating use of this functionality in
implementations beyond a simple scale without sacrificing access to future
improvements.

To install, simply clone or pull this repository. The MDPI paper contains 
instructions on how to build the scale, documents initial testing completed
on the scale (including the concluded precision of the scale). The OSF
repository includes all of the necessary design files, in editable 
(e.g. FreeCAD) and published (e.g. STL) formats.

This was developed by researchers at Michigan Technological University in the
[Michigan Open Sustainability Technology (MOST)](https://www.appropedia.org/MOST)
group.

## Use Notes
Due to the SRAM demands to run an OLED display, a microcontroller with 
more than 2 MB of memory is required when using an OLED (i.e. you must use a 
Nano Every or a Mega). If using a Nano Every, install the Arduino megaAVR 
boards library from the boards manager. As of version 1.8.6, the core library 
has a bug that prevents the use of the LCD library. To remedy this bug, follow 
the instructions from 
[this post by pert on the Arduino Forum:](https://forum.arduino.cc/index.php?topic=678338.0)
> This bug has been fixed:
> https://github.com/arduino/ArduinoCore-API/commit/7f8de5869ee3de7a915bf1db64313c08595b3ac3
> 
> A new release hasn't happened since that fix. You can fix it by doing this:
> 
> (In the Arduino IDE) File > Preferences
> 
> Click the link at the line following More preferences can be edited directly in the file.
> 
> Open the packages/arduino/hardware/megaavr/1.8.6/cores/arduino/api subfolder.
> 
> Open the file Print.h in a text editor.
> 
> Add the following line to the end of the file:
> ~~~
> using namespace arduino;
> ~~~
> Save the file.
> 
> You will now be able to use the LiquidCrystal library with your Nano Every.
> 
> Please let me know if you run into any problems or questions while following those instructions.

## Known Bugs
* Using the command 'XL' to turn an OLED off, then immediately back on, can 
  cause the program to stall. Waiting for at least 10 seconds tends to prevents
  this issue.
