
#include <NewSoftSerial.h>

#include <Wire.h>
#include "RTClib.h"

#define MIN_BRIGHT 254
#define MAX_BRIGHT 15

int lightsensor = A0;
int currentBrightness;

RTC_DS1307 RTC; // SDC on A5, SDA on A4, due to Hardware

NewSoftSerial led(2, 3);

boolean isRunning;  // RTC status
void setup()  
{
  pinMode(3, OUTPUT);
  pinMode(2, INPUT);
  pinMode(lightsensor, INPUT);
  led.begin(9600);
  led.print("v"); // reset it
  colonOn();
    
  Wire.begin();
  RTC.begin();

  isRunning = true;
  if (! RTC.isrunning()) {
    isRunning = false;
  }
  currentBrightness = MIN_BRIGHT;  
}

void show(int hours, int mins) {
  // BCD style time:
 int unit = mins % 10;
 int ten  = (mins % 100) / 10;
 int hun  = hours % 10;
 int thou = (hours % 100) / 10;
 led.print(thou, DEC);
 led.print(hun, DEC);
 led.print(ten, DEC);
 led.print(unit, DEC);
}

void colonOn() {
    led.print("w");
  led.print(B00010000,BYTE);
}


/** Called when the closk is stalled, in lieu of loop(). */
void stalledLoop() {
  // FIXME: output something here...
 // 00:00 flasher? 
}
  
void loop()
{
  if(!isRunning) {
    stalledLoop();
    return;
  }
  
  DateTime now = RTC.now();
  show(now.hour(), now.minute());  
  
  int sense = analogRead(lightsensor);
//  Serial.print(sense);
//  Serial.print(" -> ");
  int bright = (sense - 200) / 2;
  if(bright < MAX_BRIGHT) bright = MAX_BRIGHT; // Yep, it's backwards!
  if(bright > MIN_BRIGHT) bright = MIN_BRIGHT;
//  Serial.print(bright);
//  Serial.print(" limited to ");
  // Limit change size
  if((currentBrightness - bright) < -30) bright = currentBrightness + 30;
  if((currentBrightness - bright) > 20) bright = currentBrightness - 20;
//  Serial.println(bright);
  led.print(0x7A, BYTE); led.print(bright, BYTE); // Set bightness
  currentBrightness = bright;
  
  delay(500); // sleep for a bit.
}
