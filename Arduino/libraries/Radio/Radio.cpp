/** Implementation of a micro controled Radio class for the Phillips TEA5767.
    This talks via I2C, mostly becasue it's easier, and there's really no speed problem 
    for this purpose.
    */
    
#define I2CADDRESS 0x60


#include "WProgram.h"
#include "Radio.h"
#include <Wire.h>

Radio::Radio() {
  // FIXME: Bit too much magic here!
  // Set up some defaults
  this->out.buffer[0] = 0x00;
  this->out.buffer[1] = 0x00;
  this->out.buffer[2] = 0xB0;
  this->out.buffer[3] = 0x10;
  this->out.buffer[4] = 0x00;
}

void Radio::begin() {
  Wire.begin();
}

void Radio::tuneTo(double freq) {
  setFrequency(freq);
  this->out.parsed.searchMode = 0; // Turn search off.
  writeState();
}

double Radio::getFrequency() {
  readState();
  return frequencyToDouble(this->in.parsed.pllH, this->in.parsed.pllL);  
}

/** Converts from the machine representation to the double.
    Note that this, and it's inverse, setFrequency() assume a 32kHz crystal, whilst the
    chip supports other options.  Ideally, this would base it's sums of checkin what was
    actually there! */
double Radio::frequencyToDouble(unsigned int freqH, unsigned int freqL) {
  return (( freqH<<8 + freqL) * 32768 / 4 - 225000) / 1000000; // in MHz
}

/** Places the machine equivilents of the doulbe into FrequencyH and freqeuencyL */
void Radio::setFrequency(double freq) {
  uint16_t f = 4*(freq*1000000+225000)/32768; //XXX: Makes assumptions about the crystal
  this->out.parsed.pllH = f>>8;
  this->out.parsed.pllL = f & 0xFF;
}

void Radio::searchUp() {
   this->out.parsed.searchStopLevel = 3; // Strongest stations only
   this->out.parsed.searchUp = 1;
   this->out.parsed.searchMode = 1;
   writeState();
}

void Radio::searchDown() {
   this->out.parsed.searchStopLevel = 3; // Strongest stations only
   this->out.parsed.searchUp = 0; // Going ... down!
   this->out.parsed.searchMode = 1;
   writeState();
}


/** Reads full state from chip, and places it in the in.buffer. */
// FIXME: Should update outstate for things that are relevent!
void Radio::readState() {
  Wire.requestFrom(0x60,5); //reading TEA5767
  for (int i=0; i<5; i++) {
    this->in.buffer[i] = Wire.receive();
  }
}

/** Writes the state in the out buffer to the radio. */
void Radio::writeState() {
  Wire.beginTransmission(I2CADDRESS);   //writing TEA5767
  Wire.send(out.buffer[0]);
  Wire.send(out.buffer[1]);
  Wire.send(out.buffer[2]);
  Wire.send(out.buffer[3]);
  Wire.send(out.buffer[4]);
  Wire.endTransmission();
  delay(100);  
}
