midistatebroadcast
==================

A small program that captures MIDI information and makes it available to other processes, using shared memory.

## Compiling

midistatebroadcast relies on RtMidi for MIDI handling the ports.  Thus, you need to first install RtMidi http://www.music.mcgill.ca/~gary/rtmidi/ as appropiate for your operating system.  I use version 3.0.

To compile on Linux/MacOS X:
- g++ -o midistatebroadcast midistatebroadcast.cpp -lrtmidi

Alternatively, you might need to use
- g++ -o midistatebroadcast midistatebroadcast.cpp -lrtmidi -lasound -pthread

or possibly tell g++ where to find librtmidi (using "-L." for example if you copy librtmidi.a into the
same directory as midistatebroadcast.cpp.  On fact, by just running "./configure; make" in
rtmidi, you don't need to install it system-wide, just copy librtmidi and RtMidi.h into the
midistatebroadcast directory, and use "-L.".

To compile on Windows:

- TBD.  I'll try to make a statically linked executable available.

To use:
run the executable.  By default it will listen on the first MIDI port it finds,
but it will also print all the names of other ports.  If you want it to listen
to a different port, quit the program (press enter or Ctrl-C) then start
it again with the desired port as the only argument.

The program listens in "OMNI" mode, that is, it ignores the channel number.
This is easily changed in the code if you so desire.
