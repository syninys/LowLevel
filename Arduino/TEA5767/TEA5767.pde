#include "Radio.h"
#include <Wire.h>

Radio radio;

void setup()   { 

  radio.begin();
  Serial.begin(9600);

  radio.tuneTo(87.5);

  delay(100);
  
  radio.searchUp();

}

void loop()
{

  radio.readState();
  if(Serial.available()) {
    switch(Serial.read()) {
      case '+':
        radio.tuneTo(radio.getFrequency() + 0.1);
        break;
      case '-':
        radio.tuneTo(radio.getFrequency() - 0.1);
        break;
      case '\n':
        // Print status here:
        Serial.print(radio.getFrequency());
        Serial.print(" MHz ");
        if(radio.in.parsed.stereo) {
          Serial.print("S ");
        } else {
          Serial.print("M ");
        }
        Serial.println("");
        break;
      default:
        // do nothing
        break;
    }
  }
  
  delay(1000);
}
