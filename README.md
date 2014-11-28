midistatebroadcast
==================

A small program that captures MIDI information and makes it available to other processes, using shared memory.

To compile:
- have RtMidi http://www.music.mcgill.ca/~gary/rtmidi/ instaliled
  (I used 2.1)
- g++ -o midistatebroadcast midistatebroadcast.cpp -lrtmidi

To use:
run the executable.  By default it will listen on the first MIDI port it finds,
but it will also print all the names of other ports.  If you want it to listen
to a different port, quit the program (press enter or Ctrl-C) then start
it again with the desired port as the only argument.

The program listens in "OMNI" mode, that is, it ignores the channel number.
This is easily changed in the code if you so desire.
