

// MSFTime samples program
// Jarkman, 01/2011
// http://www.jarkman.co.uk/catalog/robots/msftime.htm

// Prerequisites:
// An MSF time receiver, wired to analogue pin 0, like this: http://www.pvelectronics.co.uk/index.php?main_page=product_info&cPath=9&products_id=2
// Time library:  http://www.arduino.cc/playground/Code/Time
// LOLShield library: http://code.google.com/p/lolshield/source/browse/#svn%2Ftrunk%2Flib%253Fstate%253Dclosed


#include <Time.h>  // from http://www.arduino.cc/playground/Code/Time
#include "MSFTime.h" 


MSFTime MSF;// = MSFTime();

time_t prevDisplay = 0; // when the digital clock was displayed
byte prevStatus = 255;

time_t msfTimeSync();

void setup()
{
  
  
   Serial.begin(9600);
   
   
   MSF.init( 255 ); // LED pin for status - pass 13 for the built-in LED on the Arduino, or 255 for no led at all
                   // For reasons I do not understand, you cannot use this when running with USE_AVR_INTERRUPTS
   
     Serial.println("Waiting for MSF time ... ");
    setSyncProvider(msfTimeSync);  // tell the Time library to ask for a new time value 
 
}
  
void loop()
{

  byte currStatus = MSF.getStatus();
  
  if(currStatus != prevStatus || currStatus & MSF_STATUS_FIX) 
  {
    if( currStatus != prevStatus )
    {
     if( currStatus & MSF_STATUS_CARRIER)
       Serial.println("Got carrier");
     if( (currStatus & MSF_STATUS_WAITING))
       Serial.println("Waiting for minute sync");
     if( (currStatus & MSF_STATUS_READING))
       Serial.println("Reading fix"); 

     prevStatus = currStatus;
    }
    
   now();
    
    if( timeStatus()!= timeNotSet )
    {
     if( now() != prevDisplay) //update the display only if the time has changed
     {
       prevDisplay = now();
       digitalClockDisplay();  
     }
    }
  }	 
}

void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

void digitalClockDisplay(){
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year()); 
  Serial.println(); 
}


time_t msfTimeSync() // called periodically by Time library to syncronise itself
{
   return MSF.getTime();
}

/***************************************************************************************/


