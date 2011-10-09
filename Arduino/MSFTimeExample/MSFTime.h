
// MSFTime
// Jarkman, 01/2011
// http://www.jarkman.co.uk/catalog/robots/msftime.htm

// Decodes MSF time signals from a receiver like this:
//  http://www.pvelectronics.co.uk/index.php?main_page=product_info&cPath=9&products_id=2
// and integrates with the Time library to provide a real-time clock

#ifndef MSFTime_h 
#define MSFTime_h

#include <inttypes.h>
typedef uint8_t byte;
typedef uint8_t boolean;

#define MSF_STATUS_CARRIER 1 // got radio activity of some sort

#define MSF_STATUS_WAITING 2 // waiting for a sync marker
   
#define MSF_STATUS_READING 4 // currently reading in a fix

#define MSF_STATUS_FIX 8      // got a fix that's less than an hour old
      
#define MSF_STATUS_FRESH_FIX 16 // got a fix on our last cycle

class MSFTime
{
private:	

  boolean mIsReading;
  
  byte mInputPin;
  byte mLedPin;

  volatile long mOffStartMillis;
  volatile long mOnStartMillis;
  volatile long mPrevOffStartMillis;
  volatile long mPrevOnStartMillis;
  long mOnWidth;

  // the fix we're reading in  
  byte mABits[8];
  byte mBBits[8];
  volatile byte mBitIndex; 
  volatile byte mGoodPulses;

  
  void doDecode();
  boolean checkValid();
  boolean checkParity( byte *bits, int from, int to, boolean p );

  void setBit( byte*bits, int bitIndex, byte value );
  boolean getBit( byte*bits, int bitIndex );
  byte decodeBCD( byte *bits, byte lsb, byte msb );


public:		
  
  volatile long mFixMillis; // value of millis() at last fix
    
  volatile byte mFixYear;    // 0-99
  volatile byte mFixMonth;   // 1-12
  volatile byte mFixDayOfMonth;  // 1-31
  volatile byte mFixDayOfWeek; 
  volatile byte mFixHour;  // 0-23
  volatile byte mFixMinute;  // 0-59
    

  
  MSFTime();
  void stateChange();
  void init(byte ledPin); // initialise with radio signal mirrored on given pin
  time_t getTime();
  byte getStatus();
  byte getProgess();
  long getFixAge();
};

#endif

