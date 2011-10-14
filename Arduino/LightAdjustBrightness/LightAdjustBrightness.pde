
#include <NewSoftSerial.h>

NewSoftSerial led(2, 3);

#define MIN_BRIGHT 254
#define MAX_BRIGHT 15

int lightsensor = A0;
int currentBrightness;

void setup() {
  pinMode(3, OUTPUT);
  pinMode(2, INPUT);
  pinMode(lightsensor, INPUT);
  led.begin(9600);
  led.print("v"); // reset it
  led.print("w"); led.print(B00010000,BYTE); // colon on
  led.print("1234"); // Sample text
//  Serial.begin(9600);
  currentBrightness = MIN_BRIGHT; 
}

void loop() {
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
  delay(1000);
}
