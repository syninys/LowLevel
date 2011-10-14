//ArdD001
//ver 12 Aug 10

//(Started 12 Aug 10, 277RR, 2nd day there.)

//Created using version 0017 of the Arduino Development Environment

//Very, very simple "test" program for Sparkfun
//4 character 7 segment LED displays, serially driven
//using just the "Rx" input on the module.
//See Sparkfun.com... product codes COM-09764 - COM-09767
//(differ only in color)

//Connect one of the above modules to an Arduino with
//just 3 wires: 5v (Vcc) (or 3.3, if you are using a
//low voltage system. Module is good down to 2.6v)
//Also connect to "GND" (ground), and with a wire from
//the "Rx" pad of the module... the MODULE is receiving
//it's commands through this... to the pin you choose to
//TRANSMIT data out from the Arduino on.
//If you use D3, then nothing below needs a change.
//If you use a different pin, then change the....
//#define SerOutFrmArdu...
//line.

//When you run the program, you should see the following,
//one after the other...
//1234
//2340
//3400
//4000
//0000
//----
//8888
//HELO

//(That last is the best we can do for "Hello World" on
//a 4 character, 7 segment display)

//... then there will be a brief period when all the
//LEDs are off, and then the display will repeat the
//above. All that happens over and over.

//Simple! When you know how. I didn't, and it took a
//while to get everything right. I hope you come
//across this in time to save yourself the time I wasted.

#include <NewSoftSerial.h>
//Yes, NEWSoftSerial...
//I don't think SoftwareSerial has the
//"available()" function, does it? Not
//needed here, but might as well stick
//to one library for all work with
//extra serial lines.

#define SerInToArdu 2//no ; here. Not used in this demo,
//  but in defining mySerialPort, we to designate some pin
//  for the serial data into the Arduino
#define SerOutFrmArdu 3//no ; The pin that serial data
//  will go out from the Arduino on. Beware "Rx"/"Tx"
//  "gotcha": The Arduino's "Tx" (transmit) will go to
//  the connected device's "Rx" (receive)

#define wDelay 600//no ; here. Sets how long each "message" appears

NewSoftSerial mySerialPort(SerInToArdu,SerOutFrmArdu);
//  The above creates the serial channel we will use.

void setup(){

pinMode(SerOutFrmArdu,OUTPUT);
pinMode(SerInToArdu,INPUT);//Not actually needed... put in
   //to be explicit as to data direction over serial lines

mySerialPort.begin(9600);

mySerialPort.print("v"); //To reset display module
//See next demo program for more details, and text
//in "Special Codes" section of....
//http://www.arunet.co.uk/tkboyd/ec/ec1led4x7ser.htm

};//end "setup()"

void loop(){
  mySerialPort.print("1234");
  delay(wDelay);
  mySerialPort.print("2340");
  delay(wDelay);
  mySerialPort.print("3400");
  delay(wDelay);
  mySerialPort.print("4000");
  delay(wDelay);
  mySerialPort.print("0000");
  delay(wDelay);
  mySerialPort.print("----");
  delay(wDelay);
  mySerialPort.print("8888");
  delay(wDelay);
  mySerialPort.print("HEL0");
  delay(wDelay);
  delay(wDelay);
  delay(wDelay);
  mySerialPort.print("xxxx");//Send an "x" to turn a digit off
  delay(wDelay);
  delay(wDelay);
};
