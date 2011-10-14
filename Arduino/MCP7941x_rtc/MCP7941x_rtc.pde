// Wire Master Reader
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Reads data from an I2C/TWI slave device
// Refer to the "Wire Slave Sender" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>


#define ADDRESS 0x6f


void setup()
{
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output
  
  // Start teh oscilator
  initRTC();
}

void initRTC() {
 Wire.beginTransmission(ADDRESS);
 Wire.send(0);
 Wire.endTransmission();
 
 Wire.requestFrom(ADDRESS, 1);
 if(Wire.receive() & 0x80) {
   // Clock is running
   return;
 }
 //Otherwise start it
 Wire.beginTransmission(ADDRESS);
 Wire.send(0);
 Wire.send(0x80); // Set the high bit, and wipe the current seconds..
 Wire.endTransmission();
}

void loop()
{
 Wire.beginTransmission(ADDRESS);
 Wire.send(0);
 Wire.endTransmission();
 
 Wire.requestFrom(ADDRESS, 8);
 Serial.print(Wire.receive(), DEC);
 Serial.print(" ");
 Serial.print(Wire.receive(), DEC);
 Serial.print(" ");
 Serial.print(Wire.receive(), DEC);
 Serial.print(" ");
 Serial.print(Wire.receive(), DEC);
 Serial.print(" ");
  Serial.print(Wire.receive(), DEC);
 Serial.print(" "); Serial.print(Wire.receive(), DEC);
 Serial.print(" "); Serial.print(Wire.receive(), DEC);
 Serial.print(" ");
 Serial.print(Wire.receive(), DEC);
 Serial.println(" ");
 
 delay(1000);
}
