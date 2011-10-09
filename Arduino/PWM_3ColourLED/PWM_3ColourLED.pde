#include <math.h>

#define RED   9
#define GREEN 10
#define BLUE  11

#define BRIGHTNESS  0.8

#define RED_BRIGHT   1.0
#define GREEN_BRIGHT 0.8
#define BLUE_BRIGHT  1.0

#define RED_PHASE     0.0
#define GREEN_PHASE   60.0
#define BLUE_PHASE    120.0

void opPin(int pin, float brightness, float adjust) {
   analogWrite(pin, int(brightness * adjust * 255 * BRIGHTNESS) );
}

void setRed(float brightness) { opPin(RED, brightness, RED_BRIGHT); }
void setGreen(float brightness) { opPin(GREEN, brightness, GREEN_BRIGHT); }
void setBlue(float brightness) { opPin(BLUE, brightness, BLUE_BRIGHT); }

void setColour(byte r, byte g, byte b) {
  analogWrite(RED, r);
  analogWrite(GREEN, g);
  analogWrite(BLUE, b);
}

char mode = 'p'; // p for pulse, s for serial

void setup() {
    pinMode(RED, OUTPUT);
    pinMode(GREEN, OUTPUT);
    pinMode(BLUE, OUTPUT);
   
    setRed(0.5);
    delay(100);
    setRed(0);
    setGreen(0.5);
    delay(100);
    setGreen(0);
    setBlue(0.5);
    delay(100);
    setBlue(0);
    
    Serial.begin(9600);
}

float deg = 0;
float twoPI = (2.0 * PI);

void pulseLoop() {
 setRed(sin((deg + RED_PHASE)   / twoPI) * 0.5 + 0.5); 
 setGreen(sin((deg + GREEN_PHASE) / twoPI) * 0.5 + 0.5); 
 setBlue(sin((deg + BLUE_PHASE) / twoPI) * 0.5 + 0.5); 
 delay(150);
 if(Serial.available()) {
   mode = 's';
 }
 deg = deg + 1.0;
 if(deg > 360) { deg = deg - 360; }
}

void sendColour(byte r, byte g, byte b) {
    Serial.print((int)r);
    Serial.print(", ");
    Serial.print((int)g);
    Serial.print(", ");
    Serial.println((int)b);   
}

char current = 'r';

byte r = 0;
byte g = 0;
byte b = 0;

void serialLoop() {
  
  if (Serial.available()) {
    switch(current) {
      case 'r':
        r = Serial.read();
        current = 'g';
        break;
      case 'g':
        g = Serial.read();
        current = 'b';
        break;
      case 'b':
        b = Serial.read();
        if( r == 1 && g == 2 && b == 3) {
           mode = 'p';
           current = 'r';
           break;
        }
        setColour(r,g,b);
        sendColour(r,g,b);
        current = 'r';
        break;
    }
  } 
}

void loop() {
  switch(mode) {
     case 'p':
      pulseLoop();
      break;
     case 's':
      serialLoop();
      break;
  }
}
