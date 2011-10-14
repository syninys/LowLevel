
#include <NewSoftSerial.h>

NewSoftSerial led(2, 3);

int jitter = 0; // How far off 1 second we were
long last;

void setup()  
{
  pinMode(3, OUTPUT);
  pinMode(2, INPUT);
  // set the data rate for the NewSoftSerial port
  led.begin(9600);
  led.print("v"); // reset it
//  led.print(0x7a, BYTE); led.print(254, BYTE); // Set brightness
  colonOn();
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
 last = 0;
}

void colonOn() {
    led.print("w");
  led.print(B00010000,BYTE);
}

void loop()
{
  // millis is a long, and will overflow after about 50 days.
  // Sucks for a clock, but works for this test
  // Only adjust by a fraction of jitter - as we want to damp things down.
  long ms = millis();
  if(last != 0) {
     // Probbly want to put some mixinf of previous in here too, as damping
     jitter = (ms - last) - 1000;
  }
  int seconds = ms / 1000;
  int minutes = seconds / 60;
  int hours = minutes / 60;
  show(minutes % 60, seconds % 60);
  delay(1000 - jitter);
  last = ms;
}
