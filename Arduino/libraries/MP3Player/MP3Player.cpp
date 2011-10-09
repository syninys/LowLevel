/** Implementation of a microcontroller controlled MP3Player, with a VS1053 */

#include "WProgram.h"
#include "MP3Player.h"

MP3Player::MP3Player(int xcs, int xdcs, int dreq) {
  this->pinXCS= xcs;
  this->pinXDCS = xdcs;
  this->pinDREQ = dreq;
}

void MP3Player::begin() {
  // Nothing for hte moment
}

/** Blocking method that playst shte given file to the end, and then returns.
    The file shoudl be opened, and positioned at the start of the data, with the end of the data
    being at the end of the file.
    It's not inteneded as the best interface, merely a stepping stone with some applications. */

