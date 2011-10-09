/** Implementation of a micro controled Radio class for the Phillips TEA5767.
    This talks via I2C, mostly becasue it's easier, and there's really no speed problem 
    for this purpose.
    */
    
#define I2CADDRESS 0x60;

#include <Radio.h>

Radio::Radio() {
   frequencyH = 0; 
   frequencyL = 0; 
}

void Radio::begin() {
  Wire.begin();
}

void Radio::tuneTo(double freq) {
   setFrequency(freq);
  Wire.beginTransmission(ADDRESS);   //writing TEA5767
  Wire.send(frequencyH);
  Wire.send(frequencyL);
  Wire.send(0xB0);  // FIXME: Magic!
  Wire.send(0x10);  // FIXME: Magic!
  Wire.send(0x00);  // FIXME: Magic!
  Wire.endTransmission();
  delay(100);
  
}

double Radio::getFrequency() { // FIXME: Should read from the radio
  return frequencyToDouble(this.frequencyH, this.frequencyL);  
}



/** Converts from the machine representation to the double.
    Note that this, and it's inverse, setFrequency() assume a 32kHz crystal, whilst the
    chip supports other options.  Ideally, this would base it's sums of checkin what was
    actually there! */
double Radio::frequencyToDouble(uint8_t freqH, uint8_t freqL) {
  return (freqH<<8)+freqL)*32768/4-225000;
}

/** Places the machine equivilents of the doulbe into FrequencyH and freqeuencyL */
void Radio::setFrequency(double freq) {
  uint16_t f = 4*(freq*1000000+225000)/32768; //XXX: Makes assumptions about the crystal
  this.frequencyH = f>>8;
  this.frequencyL = f & 0xFF;
}

