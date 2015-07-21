midistatebroadcast
==================

A small program that captures MIDI information and makes it available to other processes, using shared memory.

To compile:
- have RtMidi http://www.music.mcgill.ca/~gary/rtmidi/ installed
  (I used 2.1)
- g++ -o midistatebroadcast midistatebroadcast.cpp -lrtmidi

Depending on your system, you might also need to use

- g++ -o midistatebroadcast midistatebroadcast.cpp -lrtmidi -lasound -pthread

or possibly tell g++ where to find librtmidi (using "-L." for example if you copy librtmidi.a into the
same directory as midistatebroadcast.cpp.  On fact, by just running "./configure; make" in
rtmidi, you don't need to install it system-wide, just copy librtmidi and RtMidi.h into the
midistatebroadcast directory, and use "-L.".

To use:
run the executable.  By default it will listen on the first MIDI port it finds,
but it will also print all the names of other ports.  If you want it to listen
to a different port, quit the program (press enter or Ctrl-C) then start
it again with the desired port as the only argument.

The program listens in "OMNI" mode, that is, it ignores the channel number.
This is easily changed in the code if you so desire.
