arduinoPressureDiff
===================

Arduino Well depth via pressure differential.

I use INO to build this.  http://inotool.org

This program uses a MPX5100DP to measure the depth of my well.

The MPX5100DP uses a 3 wire interface.  Pin 1 is identifiable by a notch on the
pin.  Pin 1 is Vout.  Pin 2 is Ground.  Pin 3 is Vs.

Uses the ProtoThreads library from here:
https://code.google.com/p/arduinode/downloads/detail?name=pt.zip
