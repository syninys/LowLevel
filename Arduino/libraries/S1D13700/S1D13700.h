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
  S1D13700.h - A library for operating 320x240
  monochrome graphics displays based on the Epson
  S1D13700 LCD Controller.
  Library by Matt Bommicino
*/
#ifndef S1D13700_h
#define S1D13700_h

#include "WProgram.h"

/*uncomment the line below if you need to use non-default pins
	for your data lines */

//#define S1D13700_CUSTOM_DATA_PINS

#define FIXED_DIR DDRD
#define FIXED_PORT PORTD
#define FIXED_PIN PIND

class S1D13700
{
  public:
    S1D13700(void);
    void writeCommand(unsigned char command);
    void writeData(unsigned char data);
    unsigned char readData(void);
    void initLCD(void);
    
    void writeText(char * text);
    void setPixel(unsigned int x,unsigned int y, unsigned char state);
    void setCursorAddress(unsigned int address);
    void textGoTo(unsigned char x, unsigned char y);
    void graphicGoTo(unsigned int x, unsigned int y);
    void clearText(void);
    void clearGraphic(void);
    
    void drawBox(int x0, int y0, int x1, int y1);
    void drawCircle(int x0, int y0, int radius);
    void drawLine(int x0, int y0, int x1, int y1);
	
	struct pin_type {
		unsigned char d0;
		unsigned char d1;
		unsigned char d2;
		unsigned char d3;
		unsigned char d4;
		unsigned char d5;
		unsigned char d6;
		unsigned char d7;
		unsigned char a0;
		unsigned char wr;
		unsigned char rd;
		unsigned char cs;
		unsigned char rst;
	} pins;
    
    
    
  private:
    void hardReset(void);
};

#define READDELAY() asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop")

/*definitions for specific LCD settings as well as the
  command codes for various function */

#define 	S1D13700_SYSTEM_SET   	0x40
#define 	S1D13700_SLEEP_IN   		0x53
#define 	S1D13700_DISP_OFF   		0x58
#define 	S1D13700_DISP_ON   		0x59
#define 	S1D13700_SCROLL   		0x44
#define 	S1D13700_CSRFORM   		0x5d
#define 	S1D13700_CGRAM_ADR   	0x5c
#define 	S1D13700_CSRDIR_U   		0x4e
#define 	S1D13700_CSRDIR_D   		0x4f
#define 	S1D13700_CSRDIR_L   		0x4d
#define 	S1D13700_CSRDIR_R   		0x4c
#define 	S1D13700_HDOT_SCR   		0x5a
#define 	S1D13700_OVLAY   		0x5b
#define 	S1D13700_CSRW   			0x46
#define 	S1D13700_CSRR   			0x47
#define 	S1D13700_MWRITE   		0x42
#define 	S1D13700_MREAD   		0x43
#define 	S1D13700_SCR_WIDTH   	319
#define 	S1D13700_M0   			0
#define 	S1D13700_M1   			0
#define 	S1D13700_M2   			0
#define 	S1D13700_WS   			0
#define 	S1D13700_IV   			1
#define 	S1D13700_FX   			7
#define 	S1D13700_FY   			7
#define 	S1D13700_WF   			1
#define 	S1D13700_CR   			39
#define 	S1D13700_TCR   			90
#define 	S1D13700_LF   			239
#define 	S1D13700_APL   			40
#define 	S1D13700_APH   			0
#define 	S1D13700_LINES   		30
#define 	S1D13700_SAD1L   		0
#define 	S1D13700_SAD1H   		0
#define 	S1D13700_SL1   			0xEF
#define 	S1D13700_SAD2L   		0xB0
#define 	S1D13700_SAD2H   		0x04
#define 	S1D13700_SL2   			0xEF
#define 	S1D13700_SAD3L   		0
#define 	S1D13700_SAD3H  			0
#define 	S1D13700_SAD4L   		0
#define 	S1D13700_SAD4H   		0
#define 	S1D13700_CRX   			0x04
#define 	S1D13700_CRY   			0x07
#define 	S1D13700_CM   			0
#define 	S1D13700_MX0   			1
#define 	S1D13700_MX1   			0
#define 	S1D13700_DM1   			0
#define 	S1D13700_DM2   			0
#define 	S1D13700_OV   			0
#define 	S1D13700_SAGL   			0
#define 	S1D13700_SAGH   			0x70
#define 	S1D13700_SCRD   			0
#define 	S1D13700_FLASH   		0x16
#define 	S1D13700_TEXTSIZE   		((S1D13700_SAD2H << 8) + S1D13700_SAD2L)
#define		S1D13700_GRAPHICSTART	((S1D13700_SAD2H << 8) + S1D13700_SAD2L)
#define 	S1D13700_GRAPHICSIZE   	((S1D13700_SL2+1) * (S1D13700_SCR_WIDTH+1))>>3
#define 	S1D13700_MEM_END   		10800
#define 	S1D13700_SYS_P1   		0x10 | (S1D13700_IV << 5) | (S1D13700_WS << 3) | (S1D13700_M2 << 2) | (S1D13700_M1 << 1) | S1D13700_M0
#define 	S1D13700_SYS_P2   		0x00 | (S1D13700_WF << 7) | S1D13700_FX
#define 	S1D13700_CSRF_P2   		0x00 | (S1D13700_CM << 7) | S1D13700_CRY
#define 	S1D13700_OVLAY_P1   		0x00 | (S1D13700_OV << 4) | (S1D13700_DM2 << 3) | (S1D13700_DM1 << 2) | (S1D13700_MX1 << 1) | S1D13700_MX0

#endif
