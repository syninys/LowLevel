
#ifndef Radio_h
#define Radio_h

#include <inttypes.h>

/** Matches size with the 5 bute out buffer, but with approriate labels. */
struct tea5767WriteBuffer {
  unsigned int pllH : 6;
  unsigned int searchMode : 1;
  unsigned int mute : 1;

  unsigned int pllL : 8;
  
  unsigned int swpOne : 1;
  unsigned int muteLeft : 1;
  unsigned int muteRight : 1;
  unsigned int forceMono : 1;
  unsigned int hlsi : 1;
  unsigned int searchStopLevel : 2;
  unsigned int searchUp : 1;

  unsigned int searchIndicator : 1;
  unsigned int steroNoiseControl : 1;
  unsigned int HighCutControl : 1;
  unsigned int softMute : 1;
  unsigned int xtal : 1;
  unsigned int bandLimits : 1;
  unsigned int standby : 1;
  unsigned int swpTwo : 1;

  unsigned int unused : 6;
  unsigned int deemphTimeConstant : 1;
  unsigned int pllRef : 1;
};

struct tea5767ReadBuffer {
  unsigned int pllH : 6;
  unsigned int bandLimitFlag : 1;
  unsigned int readyFlag : 1;

  unsigned int pllL : 8;

  unsigned int pllCounter : 7;
  unsigned int stereo : 1;

  unsigned int alwaysZero : 1;
  unsigned int chipId : 3;
  unsigned int level : 4;

  unsigned int unused : 8; 
};

class Radio {
  public:
    union in {
      uint8_t buffer[5];
      struct tea5767ReadBuffer parsed;
    } in;
    union out {
      uint8_t buffer[5];
      struct tea5767WriteBuffer parsed;
    } out;
  
    double frequencyToDouble(unsigned int freqH, unsigned int freqL);
    void setFrequency(double freq);
    /** Reads the current state from the chip to the buffer */
    void readState();
    /** Writes the current state from the buffer to the chip */
    void writeState();
  
//  public:
    Radio();
    void begin();
  
    /** Sets the current tuning to the passed frequency.  This may give the radio the instruction, 
        but return before the tuning operation is complete. */
    void tuneTo(double frequency);
    /** Scans up to find a station */
    void searchUp();
    /** And the counterpart, scan downwards */
    void searchDown();
 
    double getFrequency();   
};


#endif
