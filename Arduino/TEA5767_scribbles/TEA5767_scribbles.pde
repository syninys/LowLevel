#include <Wire.h>

#define ADDRESS 0x60

unsigned char search_mode=0;

int b=0;
int c=0;

#define Button_next 30
#define Button_prev 31

void setup()   { 

  Wire.begin();
  Serial.begin(9600);

  tuneTo(97.4);

  delay(100);

}

unsigned int calcFrequency(double freq) {
   return  4*(freq*1000000+225000)/32768; //calculating PLL word
}

void tuneTo(double freq) {
  unsigned int frequencyB=calcFrequency(freq);  
  unsigned char frequencyH=frequencyB>>8;
  unsigned char frequencyL=frequencyB&0XFF;
  
  Wire.beginTransmission(ADDRESS);   //writing TEA5767
  Wire.send(frequencyH);
  Wire.send(frequencyL);
  Wire.send(0xB0);
  Wire.send(0x10);
  Wire.send(0x00);
  Wire.endTransmission();
  delay(100);
}

void loop()
{

  unsigned char buffer[5];

  Wire.requestFrom(0x60,5); //reading TEA5767

  if (Wire.available()) 

  {
    for (int i=0; i<5; i++) {

      buffer[i]= Wire.receive();
    }

    double freq_available=(((buffer[0]&0x3F)<<8)+buffer[1])*32768/4-225000;

    Serial.print("FM ");

    Serial.print((freq_available/1000000));

    unsigned char frequencyH=((buffer[0]&0x3F));
    unsigned char frequencyL=buffer[1];

    if (search_mode) {

      if(buffer[0]&0x80) search_mode=0;

    }

    if (search_mode==1) Serial.print(" SCAN");
    else {
      Serial.print("       ");
    }

    Serial.print("Level: ");
    Serial.print((buffer[3]>>4));
    Serial.print("/16 ");

    if (buffer[2]&0x80) Serial.print("STEREO   ");
    else Serial.print("MONO   ");

    Serial.println("");
  }
  delay(1000);
}
