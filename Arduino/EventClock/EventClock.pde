
#include <NewSoftSerial.h>
#include <Wire.h>
#include <RTClib.h>

// Pin definitions

#define LIGHTSENSOR   A1
#define DISPLAY_IN    2
#define DISPLAY_OUT   3

// Parameters

#define MIN_BRIGHT 254
#define MAX_BRIGHT 10

// Comms objects

NewSoftSerial sevenSeg(DISPLAY_IN, DISPLAY_OUT);
RTC_DS1307 rtc;

// flags
boolean rtcRunning;

// Data model

struct State {
  int brightness;
  DateTime now;
};


struct State current;
struct State target;

void setup()
{
  pinMode(LIGHTSENSOR, INPUT);

  pinMode(DISPLAY_OUT, OUTPUT);
  pinMode(DISPLAY_IN, INPUT);
  sevenSeg.begin(9600);
  sevenSeg.print("v");
  colonOn();

  Wire.begin();
  rtc.begin();

  rtcRunning = true;
  if (! rtc.isrunning()) {
    rtcRunning = false;
  }

  target.brightness = MIN_BRIGHT;
  readTime();
  current.now = target.now;
  current.brightness  = target.brightness;

  show(target.now.hour(), target.now.minute());
}

void show(int hours, int mins) {
  // BCD style time:
  int unit = mins % 10;
  int ten  = (mins % 100) / 10;
  int hun  = hours % 10;
  int thou = (hours % 100) / 10;
  sevenSeg.print(thou, DEC);
  sevenSeg.print(hun, DEC);
  sevenSeg.print(ten, DEC);
  sevenSeg.print(unit, DEC);
}

void colonOn() {
  sevenSeg.print("w");
  sevenSeg.print(B00010000,BYTE);
}

void loop() {
  readTime();
  readBrightness();

  checkAlarms();

  updateTimeDisplay();
  updateBrightness();
}

void readTime() {
  target.now = rtc.now();
}

void readBrightness() {
  int sense = analogRead(LIGHTSENSOR);
  int bright = (sense - 200) / 2;
  if(bright < MAX_BRIGHT) bright = MAX_BRIGHT; // Yep, it's backwards!
  if(bright > MIN_BRIGHT) bright = MIN_BRIGHT;
  target.brightness = bright;
}


void checkAlarms() {
  // Implement soft alarm functionality here
}

void updateTimeDisplay() {
  if(target.now.second() != current.now.second()) {
    // rate limit updates a bit
    if(target.brightness > current.brightness) {
      current.brightness = target.brightness;
      sevenSeg.print(0x7A, BYTE); 
      sevenSeg.print(current.brightness, BYTE); // Set bightness
    } else if(target.brightness < current.brightness) {
      current.brightness = target.brightness;
      sevenSeg.print(0x7A, BYTE); 
      sevenSeg.print(current.brightness, BYTE); // Set bightness
    }
    show(target.now.hour(), target.now.minute()); 
  }
  current.now = target.now;
}

void updateBrightness() {
}

