
// MSFTime
// Jarkman, 01/2011
// http://www.jarkman.co.uk/catalog/robots/msftime.htm

// Decodes MSF time signals from a receiver like this:
//  http://www.pvelectronics.co.uk/index.php?main_page=product_info&cPath=9&products_id=2
// and integrates with the Time library to provide a real-time clock

// The algorithm used here is vulnerable to noise pulses, which will cause it to give up decoding the current signal.
// It needs a whole minute of good signal to get a time fix. I've found that with my receiver, at some times of day and in some places,
// it never gets a fix at all, because each minute gets at least one noise pulse.

// Evenings work best.

// It would be possible to rework the algorithm to use an averaged signal, 
// which would be much more immune to noise, but this is good enough for my purposes.

// Turn these on to see more of what we are receiving.
//#define DEBUG 
//#define EXTRA_DEBUG

#define USE_AVR_INTERRUPTS // define this to use AVR interrupts, with the module wired to analogue pin 0
                           // Or undefine it to use Arduino interrupts, with the module wired to in digital pin 2


#include <wiring.h>

#ifdef DEBUG
#include <HardwareSerial.h> // for Serial for logging
#endif

#include <Time.h>  // from http://www.arduino.cc/playground/Code/Time

#ifdef USE_AVR_INTERRUPTS
#include <avr/interrupt.h>
#endif

#include "MSFTime.h"



#define PULSE_MARGIN 30     // The leeway we allow our pulse lengths

#define PULSE_OFF_OFFSET 20 // arbitrary offset to compensate for assymetric behaviour of my receiver,
                            // which tends to deliver an 'off' period that is too short
                            // adjust this so the short off pulses measure about 100mS in the stateChange() debug logging

#define PULSE_IGNORE_WIDTH 30 // just ignore very short pulses

MSFTime::MSFTime()
{


}

MSFTime *sMSF = 0;



void MSFTime::init(byte ledPin)
{
  mLedPin = ledPin;


  mOffStartMillis = 0;
  mOnStartMillis = 0;
  mPrevOffStartMillis = 0;
  mPrevOnStartMillis = 0;
  mOnWidth = 0;
  mFixMillis = 0;
  mIsReading = false;
  mBitIndex = 0;
  mGoodPulses = 0;

  for( int i = 0; i < 7; i ++ )
  {
    mABits[i] = 0;
    mBBits[i] = 0;
  }


  if( ledPin != 255 )
    pinMode(mLedPin, OUTPUT);


  sMSF = this;

  #ifdef USE_AVR_INTERRUPTS
    // see
    // http://www.me.ucsb.edu/~me170c/Code/How_to_Enable_Interrupts_on_ANY_pin.pdf
    // for a good descripton of the AVR pin interrupt malarkey
    // Switching this stuff to a different pin requires reading the table there quite carefully
    // You will need to change the block below, and also the ISR(PCINT?_vect) line below.
    // I chose digital 8 as my input because it is one of the few pins that the LOLShield does not use.

    // for analogue pin 0:
    mInputPin = 14;
    pinMode(mInputPin, INPUT);

    PCICR |= (1<<PCIE1);
    PCMSK1 |= (1<<PCINT8);
    MCUCR = (1<<ISC01) | (1<<ISC00);
    /*
    // for digital pin 7:
    mInputPin = 7;
    pinMode(mInputPin, INPUT);

    PCICR |= (1<<PCIE2);
    PCMSK2 |= (1<<PCINT23);
    MCUCR = (1<<ISC01) | (1<<ISC00);
  */
    // enable interrupts
    interrupts();
  #else
    mInputPin = 2;
    pinMode(mInputPin, INPUT);
    interruptIndex = 0; // the Arduino interrupt for pin 2
    attachInterrupt(interruptIndex, sStateChange, CHANGE);

  #endif
}


#ifdef USE_AVR_INTERRUPTS
ISR(PCINT1_vect)  // for analogue 0 (digital 14)
//ISR(PCINT0_vect)  // for digital pin 8
//ISR(PCINT2_vect)  // for digital pin 7
{
  //todo - check our pin changed
  if( sMSF )
    sMSF->stateChange();
}
#else
void sStateChange() // static fn for the interrupt handler
{
  if( sMSF )
    sMSF->stateChange();
}
#endif

byte oldVal = 3;

void MSFTime::stateChange()  // interrupt routine called on every state change
{

   byte val = digitalRead(mInputPin);


  if( val == oldVal )
    return;
    
  oldVal = val;

  long millisNow = millis();

  if( mLedPin != 255 )
    digitalWrite(mLedPin, val);

  // see here:
  // http://www.pvelectronics.co.uk/rftime/msf/MSF_Time_Date_Code.pdf
  // for an explanation of the format

  // Carrier goes off for 100, 200, 300, or 500 mS during every second

  // Our input is inverted by our receiver, so val != 0 when carrier is off

    if (val != 0) // carrier is off, start timing
    {
      
      if( millisNow - mOnStartMillis < PULSE_IGNORE_WIDTH)
      {
        // ignore this transition plus the previous one
         #ifdef EXTRA_DEBUG
        Serial.print("Ignoring on pulse len ");
        Serial.println(millisNow - mOnStartMillis);
        #endif
        
        mOnStartMillis = mPrevOnStartMillis;
       
        return;
      }
      
      mPrevOffStartMillis = mOffStartMillis;
      mOffStartMillis = millisNow;

      mOnWidth = mOffStartMillis - mOnStartMillis;
      return; 
    }

    
    if( millisNow - mOffStartMillis < PULSE_IGNORE_WIDTH)
    {
       #ifdef EXTRA_DEBUG
        Serial.print("Ignoring off pulse len ");
        Serial.println(millisNow - mOffStartMillis);
        #endif
      // ignore this transition plus the previous one
      mOffStartMillis = mPrevOffStartMillis;
      return;
    }
    
        mPrevOnStartMillis = mOnStartMillis;
    mOnStartMillis = millisNow;
  
    long offWidth = millisNow - mOffStartMillis - PULSE_OFF_OFFSET;


    /* check the width of the off-pulse; according to the specifications, a
     * pulse must be 0.1 or 0.2 or 0.3 or 0.5 seconds
     */

    boolean is500 = abs(offWidth-500) < PULSE_MARGIN;
    boolean is300 = abs(offWidth-300) < PULSE_MARGIN;
    boolean is200 = abs(offWidth-200) < PULSE_MARGIN;
    boolean is100 = abs(offWidth-100) < PULSE_MARGIN;

    long onWidth = mOnWidth;

    boolean onWas100 =  (onWidth > 5) && (onWidth < 200);

    boolean onWasNormal = (onWidth > 400) && (onWidth < (900 + PULSE_MARGIN));

    
    #ifdef EXTRA_DEBUG
    Serial.print("Sum ");
    Serial.print(offWidth + onWidth);
    Serial.print("  offWidth ");
    Serial.print(offWidth);
    Serial.print("  onWidth ");
    Serial.print(onWidth);

    Serial.print("  mBitIndex ");
    Serial.println((int)mBitIndex);
    #endif


    if( (onWasNormal || onWas100) && (is100 || is200 || is300 || is500 ))
    {
      mGoodPulses++;
    }
    else
    {
      #ifdef EXTRA_DEBUG
      Serial.println("Bad pulse!!!!!! ");
      #endif
      mGoodPulses = 0;
    }
    
    /*
    Cases:
    a 500mS carrier-off marks the start of a minute
    a 300mS carrier-off means bits 1 1
    a 200mS carrier-off means bits 1 0
    a 100mS carrier-off followed by a 900mS carrier-on means bits 0 0
    a 100mS carrier-off followed by a 100mS carrier-on followed by a 100mS carrier-off means bits 0 1
    */

    if( is500 ) // minute marker
    {
      if( mIsReading )
        doDecode();

      mIsReading = true; // and get ready to read the next minute's worth
      mBitIndex = 1;  // the NPL docs number bits from 1, so we will too
    }
    else
    if( mIsReading )
    {
      if( mBitIndex < 60 && onWasNormal && (is100 || is200 || is300 )) // we got a sensible pair of bits, 00 or 01 or 11
      {
        if( is100 )
        {
           setBit( mABits, mBitIndex, 0 );
           setBit( mBBits, mBitIndex++, 0 );
        }
        if( is200 )
        {
           setBit( mABits, mBitIndex, 1 );
           setBit( mBBits, mBitIndex++, 0 );
        }
        if( is300 )
        {
           setBit( mABits, mBitIndex, 1 );
           setBit( mBBits, mBitIndex++, 1 );
        }

        #ifdef EXTRA_DEBUG
          if( getBit( mABits, mBitIndex - 1 ))
             Serial.println("  A = 1");
          else
             Serial.println("  A = 0");

          if( getBit( mBBits, mBitIndex - 1 ))
             Serial.println("  B = 1");
          else
             Serial.println("  B = 0");
         #endif


      }
      else if( mBitIndex < 60 && onWas100 && is100 && mBitIndex > 0 ) // tricky - we got a second bit for the preceding pair
      {
        setBit( mBBits, mBitIndex - 1, 1 );

        #ifdef EXTRA_DEBUG
            if( getBit( mBBits, mBitIndex - 1 ))
             Serial.println("  B = 1");
          else
             Serial.println("  B = 0");
         #endif
       }
      else // bad pulse, give up
      {
        #ifdef DEBUG
        if( mIsReading || ! (is100 || is200 || is300 || is500))
        {
          Serial.println("Bad pulse len");
          Serial.print("  offWidth ");
          Serial.println(offWidth);
          Serial.print("  onWidth ");
          Serial.println(onWidth);
          Serial.print("  mBitIndex ");
          Serial.println((int)mBitIndex);
        }
        #endif

        mIsReading = false;
        mBitIndex = 0;
      }
    }
}

void MSFTime::doDecode()
{

    if( mBitIndex != 60 ) // there are always 59 bits, barring leap-seconds
    {
      #ifdef DEBUG
        Serial.println("Wrong number of bits ");
      #endif
      return;
    }

    if( ! checkValid())
    {
      #ifdef DEBUG
        Serial.println("Not valid");
      #endif
      return;
    }



    mFixMillis = millis() - 500L;

    mFixYear = decodeBCD( mABits, 24, 17 );    // 0-99
    mFixMonth = decodeBCD( mABits, 29, 25 );   // 1-12
    mFixDayOfMonth = decodeBCD( mABits, 35, 30 );  // 1-31
    mFixDayOfWeek = decodeBCD( mABits, 38, 36 );
    mFixHour = decodeBCD( mABits, 44, 39 );  // 0-23
    mFixMinute = decodeBCD( mABits, 51, 45 );  // 0-59

    #ifdef DEBUG
    Serial.println("Decoded");
    Serial.print(2000+(int)mFixYear);
    Serial.print("/");

    Serial.print((int)mFixMonth);

    Serial.print("/");
    Serial.println((int)mFixDayOfMonth);
    //Serial.println((int)mFixDayOfWeek);
    Serial.print((int)mFixHour);
    Serial.print(":");
    Serial.println((int)mFixMinute);
    #endif
}

boolean MSFTime::checkValid()
{
  boolean result = true;

  if( getBit( mABits, 52 ))
    {
      #ifdef EXTRA_DEBUG
      Serial.println("bit 52A not 0!");
      #endif
      result = false;
    }
  if( ! getBit( mABits, 53 ))
    {
      #ifdef EXTRA_DEBUG
      Serial.println("bit 53A not 1!");
      #endif
      result = false;
    }
    if( ! getBit( mABits, 54 ))
    {
      #ifdef EXTRA_DEBUG
      Serial.println("bit 54A not 1!");
      #endif
      result = false;
    }
    if( ! getBit( mABits, 55 ))
    {
      #ifdef EXTRA_DEBUG
      Serial.println("bit 55A not 1!");
      #endif
      result = false;
    }
    if( ! getBit( mABits, 56 ))
    {
      #ifdef EXTRA_DEBUG
      Serial.println("bit 56A not 1!");
      #endif
      result = false;
    }
    if( ! getBit( mABits, 57 ))
    {
      #ifdef EXTRA_DEBUG
      Serial.println("bit 57A not 1!");
      #endif
      result = false;
    }
    if( ! getBit( mABits, 58 ))
    {
      #ifdef EXTRA_DEBUG
      Serial.println("bit 58A not 1!");
      #endif
      result = false;
    }

    if( getBit( mABits, 59 ))
    {
      #ifdef EXTRA_DEBUG
      Serial.println("bit 59A not 0!");
      #endif
      result = false;
    }

    if( ! checkParity( mABits, 17, 24, getBit( mBBits, 54 )))
      result = false;

    if( ! checkParity( mABits, 25,35, getBit( mBBits, 55 )))
      result = false;

    if( ! checkParity( mABits, 36,38, getBit( mBBits, 56 )))
      result = false;

    if( ! checkParity( mABits, 39, 51, getBit( mBBits, 57 )))
      result = false;
    return result;
}

boolean MSFTime::checkParity( byte *bits, int from, int to, boolean p )
{
  int set = 0;
  int b;
  for( b = from; b <= to; b ++ )
    if( getBit( bits, b ))
     set++;

  if( p )
   set ++;

  if( set & 0x01 ) // must be an odd number of set bits
   return true;

  #ifdef EXTRA_DEBUG
    Serial.print("Failed parity for bits ");
    Serial.print(from);
    Serial.print("->");
    Serial.println(to);
  #endif

}

byte MSFTime::getProgess()
{
  if( mIsReading )
    return mBitIndex;
  else
    return mGoodPulses;
}

long  MSFTime::getFixAge()
{
  if( mFixMillis == 0 )
    return 0;
    
  return millis() - mFixMillis;
}

byte MSFTime::getStatus()
{
  byte result = 0;
  
  if( (millis() - mOffStartMillis) < 5000L)
    result = result |  MSF_STATUS_CARRIER; // got radio activity of some sort
   
   if( mBitIndex > 1 )
     result = result | MSF_STATUS_READING;
   else
     if( result & MSF_STATUS_CARRIER )
       result = result | MSF_STATUS_WAITING;
       
   if( (millis() - mFixMillis) < 60L * 60000L )
     result = result | MSF_STATUS_FIX;      // got a fix that's less than an hour old
      
    if( (millis() - mFixMillis) < 62000L )
      result = result | MSF_STATUS_FRESH_FIX;    // got a fix on our last cycle
    
    return result;
}


time_t MSFTime::getTime()
{
  tmElements_t tm;

  if( mFixMillis == 0 )
  {
    #ifdef DEBUG
    //Serial.println("getTime - no fix");
    #endif
    return (time_t) 0; // not got a fix
  }

  tm.Year = mFixYear + 2000 - 1970;  // convert from MSF's years since 2000 into Time's years since 1970
  tm.Month = mFixMonth;              // 1-12
  tm.Day = mFixDayOfMonth;           // 1-31
  tm.Hour = mFixHour;                // 0-23
  tm.Minute = mFixMinute;            // 0-59
  tm.Second = 0;                     // 0-59

  time_t time = makeTime(tm) + (millis() - mFixMillis) / 1000; // add the time at the last fix to the interval since the last fix

  #ifdef DEBUG
  Serial.println("getTime has time");
  Serial.println(time);
  #endif

  return time;
}



void MSFTime::setBit( byte*bits, int bitIndex, byte value )
{
  byte mask = 1 << (bitIndex & 0x7);

  if( value )
    bits[bitIndex>>3] = bits[bitIndex>>3] | mask;
  else
    bits[bitIndex>>3] = bits[bitIndex>>3] & ( ~ mask );

}

boolean MSFTime::getBit( byte*bits, int bitIndex )
{
  byte mask = 1 << (bitIndex & 0x7);

  return (bits[bitIndex>>3] & mask) != 0 ;

}

byte BCD[] = { 1,2,4,8,10,20,40,80 };

byte MSFTime::decodeBCD( byte *bits, byte lsb, byte msb )
{
  byte result = 0;

  byte b = lsb;
  byte d = 0;

  for( ; b >= msb; b --, d ++ )
  {
    if( getBit( bits, b ))
      result += BCD[ d ];
  }

  return result;
}



