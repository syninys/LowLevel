/*
******************************************************************
Copyright (c) 2010, Matt Bommicino
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * Neither the name of Cafe Logic nor the names of its contributors 
	  may be used to endorse or promote products
      derived from this software without specific prior written
      permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

******************************************************************
*/
/*
  S1D13700.cpp - A library for operating 320x240
  monochrome graphics displays based on the Epson
  S1D13700 LCD Controller.
  Library by Matt Bommicino
*/

#include "WProgram.h"
#include "S1D13700.h"

/*Constructor, set pin markers to their default values */
S1D13700::S1D13700(void)
{
		this->pins.d0 = 0;
		this->pins.d1 = 1;
		this->pins.d2 = 2;
		this->pins.d3 = 3;
		this->pins.d4 = 4;
		this->pins.d5 = 5;
		this->pins.d6 = 6;
		this->pins.d7 = 7;
		this->pins.rd = 8;
		this->pins.wr = 9;
		this->pins.a0 = 10;
		this->pins.cs = 11;
		this->pins.rst = 12;
}

/* This function is specified inline for speed reasons*/
inline void __attribute__ ((always_inline)) setData(unsigned char data)
{
	#ifdef S1D13700_CUSTOM_DATA_PINS
		pinMode(this->pins.d0, OUTPUT);
		pinMode(this->pins.d1, OUTPUT);
		pinMode(this->pins.d2, OUTPUT);
		pinMode(this->pins.d3, OUTPUT);
		pinMode(this->pins.d4, OUTPUT);
		pinMode(this->pins.d5, OUTPUT);
		pinMode(this->pins.d6, OUTPUT);
		pinMode(this->pins.d7, OUTPUT);

		digitalWrite(this->pins.d0, (data & 1));
		digitalWrite(this->pins.d1, ((data & (1 << 1)) >> 1));
		digitalWrite(this->pins.d2, ((data & (1 << 2)) >> 2));
		digitalWrite(this->pins.d3, ((data & (1 << 3)) >> 3));
		digitalWrite(this->pins.d4, ((data & (1 << 4)) >> 4));
		digitalWrite(this->pins.d5, ((data & (1 << 5)) >> 5));
		digitalWrite(this->pins.d6, ((data & (1 << 6)) >> 6));
		digitalWrite(this->pins.d7, ((data & (1 << 7)) >> 7));
		
	#else
		FIXED_DIR = 0xFF;
		FIXED_PORT = data;
	#endif
	


}

/*The S1D13700 is veyr picky about how things are timed
	change these two routines at your own risk */
	
void S1D13700::writeCommand(unsigned char command)
{

	setData(command);
	digitalWrite(this->pins.wr, 0);
	digitalWrite(this->pins.cs, 0);
	digitalWrite(this->pins.wr, 1);
	digitalWrite(this->pins.cs, 1);

}

void S1D13700::writeData(unsigned char data)
{

	setData(data);
	digitalWrite(this->pins.a0, 0);
	digitalWrite(this->pins.cs, 0);
	digitalWrite(this->pins.wr, 0);
	digitalWrite(this->pins.cs, 1);
	digitalWrite(this->pins.wr, 1);
	digitalWrite(this->pins.a0, 1);
}



void S1D13700::initLCD(void)
{
	/*Set all control pins as 
	output and then set them high*/
	
	digitalWrite(this->pins.cs, 1);
	digitalWrite(this->pins.wr, 1);
	digitalWrite(this->pins.rd, 1);
	digitalWrite(this->pins.a0, 1);
	digitalWrite(this->pins.rst, 1);
	
	pinMode(this->pins.cs, OUTPUT);
	pinMode(this->pins.rst, OUTPUT);
	pinMode(this->pins.a0, OUTPUT);
	pinMode(this->pins.wr, OUTPUT);
	pinMode(this->pins.rd, OUTPUT);
	

	
	hardReset();
	
	/* Now we are going to issue the system set command
		and specify operating paramters. If these need to
		change for your application, you must study the
		S1D13700 datasheet and change the values in
		S1D13700.h */

	writeCommand(S1D13700_SYSTEM_SET); 
	delayMicroseconds(1000);
	writeData(S1D13700_SYS_P1);
	delayMicroseconds(1);	
	writeData(S1D13700_SYS_P2);	
	delayMicroseconds(1);	
	writeData(S1D13700_FY);
	delayMicroseconds(1);		
	writeData(S1D13700_CR);	
	delayMicroseconds(1);	
	writeData(S1D13700_TCR);
	delayMicroseconds(1);	
	writeData(S1D13700_LF);	
	delayMicroseconds(1);	
	writeData(S1D13700_APL);
	delayMicroseconds(1);	
	writeData(S1D13700_APH);	
		
	writeCommand(S1D13700_SCROLL);   
	writeData(S1D13700_SAD1L);		
	writeData(S1D13700_SAD1H);		
	writeData(S1D13700_SL1);		
	writeData(S1D13700_SAD2L);		
	writeData(S1D13700_SAD2H);		
	writeData(S1D13700_SL2);		
	writeData(S1D13700_SAD3L);		
	writeData(S1D13700_SAD3H); 		
	writeData(S1D13700_SAD4L);		
	writeData(S1D13700_SAD4H);		
		
	writeCommand(S1D13700_CSRFORM);
	writeData(S1D13700_CRX);		
	writeData(S1D13700_CSRF_P2);		
		
	writeCommand(S1D13700_CGRAM_ADR);       
	writeData(S1D13700_SAGL);			
	writeData(S1D13700_SAGH);				
		
	writeCommand(S1D13700_CSRDIR_R);      

	writeCommand(S1D13700_HDOT_SCR);       
	writeData(S1D13700_SCRD);			
		
	writeCommand(S1D13700_OVLAY);            
	writeData(S1D13700_OVLAY_P1);			
				
	writeCommand(S1D13700_DISP_ON);
	writeData(S1D13700_FLASH);
}

unsigned char S1D13700::readData(void)
{
	unsigned char tmp;
	digitalWrite(this->pins.cs, 0);
	#ifdef S1D13700_CUSTOM_DATA_PINS
		pinMode(this->pins.d0, INPUT);
		pinMode(this->pins.d1, INPUT);
		pinMode(this->pins.d2, INPUT);
		pinMode(this->pins.d3, INPUT);
		pinMode(this->pins.d4, INPUT);
		pinMode(this->pins.d5, INPUT);
		pinMode(this->pins.d6, INPUT);
		pinMode(this->pins.d7, INPUT);
	#else
		FIXED_DIR = 0x00;
	#endif
	digitalWrite(this->pins.rd, 0);
	/*S1D13700 takes a bit to output data after the RD pin goes low.
		We must force a delay */
	READDELAY();
	#ifdef S1D13700_CUSTOM_DATA_PINS
		tmp = (digitalRead(this->pins.d7) << 7) | (digitalRead(this->pins.d6) << 6) |
			(digitalRead(this->pins.d5) << 5) | (digitalRead(this->pins.d4) << 4) |
			(digitalRead(this->pins.d3) << 3) | (digitalRead(this->pins.d2) << 2) |
			(digitalRead(this->pins.d1) << 1) | (digitalRead(this->pins.d0)); 
	#else
		tmp =  FIXED_PIN;
	#endif
	digitalWrite(this->pins.cs, 1);
	digitalWrite(this->pins.rd, 1);
	return tmp;
}



void S1D13700::hardReset(void)
{
	digitalWrite(this->pins.rst, 0);
	delay(50);
	digitalWrite(this->pins.rst, 1);
	delay(50);
}

void S1D13700::writeText(char * text)
{
	writeCommand(S1D13700_MWRITE);
	while(*text)
	{
		writeData(*text++);
	}
}
/*Set a single pixel. We have to read a byte in, modify the appropriate bit
	then write is back out */
void S1D13700::setPixel(unsigned int x,unsigned int y, unsigned char state = 1)
{
	unsigned char tmp = 0;
	unsigned int address = S1D13700_GRAPHICSTART + (40 * y) + (x/8); 
	setCursorAddress(address);

	writeCommand(S1D13700_MREAD);
	tmp = readData();

	if(state)
	{
		tmp |= (1 << (S1D13700_FX - (x % 8)));
	}
	else
	{
		tmp &= ~(1 << (S1D13700_FX - (x % 8)));
	}

	setCursorAddress(address);
	writeCommand(S1D13700_MWRITE);
	writeData(tmp);
}

void S1D13700::setCursorAddress(unsigned int address)
{
	writeCommand(S1D13700_CSRW);
	writeData((unsigned char)(address & 0xFF));
	writeData((unsigned char)(address >> 8));
}

/*This function should be called before text is written
	each time */
void S1D13700::textGoTo(unsigned char x, unsigned char y)
{
	setCursorAddress((y * 40) + x);
}


void S1D13700::graphicGoTo(unsigned int x, unsigned int y)
{
	setCursorAddress(S1D13700_GRAPHICSTART + (y * 40) + x/8);
}


void S1D13700::clearText(void)
{
	int i;
	textGoTo(0,0);
	writeCommand(S1D13700_MWRITE);
	for(i = 0; i < 1200; i++)
	{
		writeData(' ');
	}
}


void S1D13700::clearGraphic(void)
{
	unsigned int i;
	setCursorAddress(S1D13700_GRAPHICSTART);
	writeCommand(S1D13700_MWRITE);
	for(i = 0; i < (40 * 240); i++)
	{
		writeData(0x00);
	}
}

/* 
	This is a custom adaptation of Bresenham's line algorithm
*/
void S1D13700::drawLine(int x0, int y0, int x1, int y1)
{
	int deltax;
	int deltay;
	int error;
	int ystep;
	int y;
	int x;
	unsigned char steep;
	int temp;
	steep = ((abs(y1 - y0)) > (abs(x1 - x0)));
     if (steep)
	 {
         /*swap x0, y0*/
		 temp = x0;
		 x0 = y0;
		 y0 = temp;
		 
         /*swap x1, y1*/
		 temp = x1;
		 x1 = y1;
		 y1 = temp;
	 }
     if (x0 > x1)
	 {
         /*swap x0, x1*/
		 temp = x0;
		 x0 = x1;
		 x1 = temp;
		 
         /*swap y0, y1*/
		 temp = y0;
		 y0 = y1;
		 y1 = temp;
	 }
     deltax = x1 - x0;
     deltay = abs(y1 - y0);
     error = deltax / 2;
     y = y0;
     if (y0 < y1)
	 {
		ystep = 1;
	 }
	 else 
	 {
		ystep = -1;
	 }
     for (x = x0; x <=x1; x++)
	 {
         if (steep)
		 {		
			setPixel(y,x,1);
		 } 
		 else 
		 {
		    setPixel(x,y,1);
		 }
         error = error - deltay;
         if (error < 0)
		 {
             y = y + ystep;
             error = error + deltax;
	     }
	 }


}
/*
	A simple rectangle algorithm
*/
void S1D13700::drawBox(int x0, int y0, int x1, int y1)
{
	int p;
	int tempVal;
	
	if (x0 > x1)
	{
		tempVal = x0;
		x0 = x1;
		x1 = tempVal;
	}
	
	if (y0 > y1)
	{
		tempVal = y0;
		y0 = y1;
		y1 = tempVal;
	}
	
	for (p = x0; p <= x1; p++)
	{
		setPixel(p,y0);
		setPixel(p,y1);
	}
	for (p = y0; p <= y1; p++)
	{
		setPixel(x0,p);
		setPixel(x1,p);
	}

}
/* 
	I got this circle algorithm from:
		http://en.wikipedia.org/wiki/Midpoint_circle_algorithm
	I chose it because it avoids floats
*/
void S1D13700::drawCircle(int x0, int y0, int radius)
{
	  int f = 1 - radius;
	  int ddF_x = 1;
	  int ddF_y = -2 * radius;
	  int x = 0;
	  int y = radius;
	 
	  setPixel(x0, y0 + radius);
	  setPixel(x0, y0 - radius);
	  setPixel(x0 + radius, y0);
	  setPixel(x0 - radius, y0);
	 
	  while(x < y)
	  {
		// ddF_x == 2 * x + 1;
		// ddF_y == -2 * y;
		// f == x*x + y*y - radius*radius + 2*x - y + 1;
		if(f >= 0) 
		{
		  y--;
		  ddF_y += 2;
		  f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;    
		setPixel(x0 + x, y0 + y);
		setPixel(x0 - x, y0 + y);
		setPixel(x0 + x, y0 - y);
		setPixel(x0 - x, y0 - y);
		setPixel(x0 + y, y0 + x);
		setPixel(x0 - y, y0 + x);
		setPixel(x0 + y, y0 - x);
		setPixel(x0 - y, y0 - x);
	  }

}