//*****************************************//
//  midistatebroadcast.cpp by
//               Joachim Thiemann 2014
//  modified originally from
//  midicctmp.cc
//  which in turn was derived from
//  qmidiin.cpp by Gary Scavone, 2003-2004.
//
//  read MIDI queue and create a mmap()-ed
//  file in /tmp with the current state of
//  the keys and the controllers.
//
//*****************************************//

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <iostream>
#include <cstdlib>
#include "RtMidi.h"

#include <unistd.h>
#include <signal.h>
#define SLEEP( milliseconds ) usleep( (unsigned long) (milliseconds * 1000.0) )

#include <vector>

bool done;
static void finish( int ignore ){ exit(0); }
#define SHMEMSIZE 256
unsigned char *memarea;

void usage( void ) {
  // Error function in case of incorrect command-line
  // argument specifications.
  std::cout << "\nusage: midicctmp \n";
  std::cout << "    where port = the device to use (default = 0).\n\n";
  exit( 0 );
}

void getMidiData( double deltatime, std::vector< unsigned char > *mesg, void *)
{
  unsigned int nBytes = mesg->size();
  // debug
  // for( int n; n<nBytes; n++) printf("%02x ", mesg->at(n));
  // parse the message, place data into shared mem
  if (nBytes==3) {
    unsigned int type = mesg->at(0) & 0xf0; // ignore channel number ("OMNI" mode)
    unsigned int num = mesg->at(1);         // key or controller number
    unsigned int val = mesg->at(2);         // key velocity ot control value
    if (type==0x80) memarea[num] = 0;       // note off
    if (type==0x90) memarea[num] = val;     // note on (and off if 0)
    if (type==0xb0) memarea[128+num] = val; // control change
  }
}

int main( int argc, char *argv[] )
{
  RtMidiIn *midiin = 0;
  std::vector<unsigned char> message;
  int nBytes;
  double stamp;
  int f;

  // Minimal command-line check.
  if ( argc > 2 ) usage();

  // try to open file in /tmp for the shared memory
  f = open( "/tmp/midibroadcast", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR );
  lseek( f, SHMEMSIZE-1, SEEK_SET );
  write( f, "", 1 );
  lseek( f, 0, SEEK_SET );
  memarea = (unsigned char *)mmap( NULL, SHMEMSIZE, PROT_READ|PROT_WRITE, MAP_SHARED, f, 0);
  if (MAP_FAILED == memarea) {
    perror( "Shared memory mmap" );
  }
  close( f );

  // RtMidiIn constructor
  try {
    midiin = new RtMidiIn();
  }
  catch ( RtMidiError &error ) {
    error.printMessage();
    exit( EXIT_FAILURE );
  }

  // Check available ports and print the names
  unsigned int port = 0;
  unsigned int nPorts = midiin->getPortCount();
  std::cout << "Found " << nPorts << " ports.\n";
  for (; port<nPorts; port++)
    std::cout << port << ": " << midiin->getPortName( port ) << "\n";
  port = 0;
  if ( argc == 2 ) port = (unsigned int) atoi( argv[1] );
  if ( port >= nPorts ) {
    delete midiin;
    std::cout << "Invalid port specifier!\n";
    usage();
  }

  // set callback
  try {
    midiin->setCallback( &getMidiData );
  }
  catch ( RtMidiError &error ) {
    error.printMessage();
    goto cleanup;
  }
  
  // Ignore sysex, timing, or active sensing messages.
  midiin->ignoreTypes( true, true, true );

  // open the port
  try {
    midiin->openPort( port );
  }
  catch ( RtMidiError &error ) {
    error.printMessage();
    goto cleanup;
  }
  printf( "Reading from port %d.\n", port );

  // Install an interrupt handler function.
  done = false;
  (void) signal(SIGINT, finish);

  std::cout << "\nReading MIDI input ... press <enter> to quit.\n";
  char input;
  std::cin.get(input);

  // Clean up
 cleanup:
  delete midiin;

  return 0;
}

