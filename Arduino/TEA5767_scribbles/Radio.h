
#ifndef Radio_h
#define Radio_h

#include <inttypes.h>

class Radio {
  private:
    uint8_t frequencyL; // In machine units
    uint8_t frequencyH; // In machine units
  
    double frequencyToDouble(uint8_t freqH, uint8_t freqL);
    void setFrequency(double freq);
  
  public:
    Radio();
    void begin();
  
    /** Sets the current tuning to the passed frequency.  This may give the radio the instruction, 
        but return before the tuning operation is complete. */
    void tuneTo(double frequency);
 
    double getFrequency();   
}


#endif
