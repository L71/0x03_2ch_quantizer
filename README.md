0x03 2ch Quantizer
==================

This thing is a two channel CV quantizer for Eurorack, intended to make sure
some oscillators played by an analog sequencer are actually playing in tune :)

* two CV input channels, each input 0-5V is mapped to output 0-4.096V, four octaves @ 1V/oct.
* transpose knob per channel, +/- 1 octave in semitone steps
* The green LEDs will be brighter when transpose is set to -12/0/+12 half notes
* Red LED lights up if any channel tries to go off limits

Prototype build for Eurorack (complete with panel) was OK :) 

The microcontroller source code is released under the GPL v3.0 license. 
The PCBs and front panel drawings are released under the Creative Commons CC-BY-SA 4.0 license.

Tools used, all on MacOS 10.10:
C compiler: avr-gcc 4.3.3, http://www.obdev.at/products/crosspack/index.html
PCB: Eagle 7.1, www.cadsoftusa.com
Panel: Inkscape 0.91, inkscape.org 

