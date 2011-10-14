
#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 RTC;

void setup () {
    Serial.begin(57600);
    Wire.begin();
    RTC.begin();
    RTC.adjust(DateTime(__DATE__, __TIME__));
}

void loop() {
  
}
