
#ifndef MP3Player_h
#define MP3Player_h

#include "WProgram.h"
#include <inttypes.h>

#include <SPI.h>
#include <SD.h>

/** Control Chip Select Pin (for accessing SPI Control/Status registers) */
#define MP3_XCS 9

/** Data Chip Select / BSYNC Pin */
#define MP3_XDCS 2

//** Data Request Pin: Player asks for more data */
#define MP3_DREQ 3

struct SCIRegisters {
  uint16_t mode;   // Mode control
  uint16_t status; // Status of VS1053b
  uint16_t bass;   // Built-in bass/treble control
  uint16_t clockf; // Clock freq + multiplier
  uint16_t decodeTime; // decode time in seconds
  uint16_t audata; // misc audio data
  uint16_t wRam;   // RAM write/read
  uint16_t wRamAddr; // base address for ram write/read
  uint16_t hDat0;  // stream header data 0
  uint16_t hDat1;  // stream header data 1 
  uint16_t aiaddr; // start address of appplication
  uint16_t vol;    // Volumne control
  uint16_t aictrl0; // Application control register0
  uint16_t aictrl1; // Application control register1
  uint16_t aictrl2; // Application control register2
  uint16_t aictrl3; // Application control register3
};

struct SCIBitFields { // Bit field decomposition of the registers
  // XXX: Might need to reverse this by 8 bit sections? Not clear, so do it live.
  // SCI_MODE
  unsigned int differential : 1; // 0 = normal, 1 = left inverted
  unsigned int allowLayer12 : 1; // 0 = no, 1 = yes.  0 by default for patent reasons
  unsigned int reset : 1;        // 0 = normal, 1 = reset
  unsigned int cancel : 1;       // 0 = no, 1 = cancel decoding current file
  unsigned int earSpeakerLo : 1; // 0 = off, 1 = active
  unsigned int sdiTests : 1;     // 0 = not allowed, 1 = allowed
  unsigned int streamMode : 1;   // 0 = no, 1 = yes
  unsigned int earSpeakerHi : 1; // 0 = off, 1 = active
  unsigned int dclkEdge : 1;     // 0 = rising, 1 = falling
  unsigned int sdiBitOrder : 1;  // 0 = MSb first, 1 = MSb last
  unsigned int shareSpiCS : 1;   // 0 = no, 1 = yes
  unsigned int newSpiMode : 1;   // 0 = no, 1 = yes (default)
  unsigned int adpcmRecord : 1;  // 0 = not active, 1 = active
  unsigned int unused : 1;       // 0 = right, 1 = wrong 
  unsigned int micLineSelctor : 1; // 0 = micp, 1 = line1
  unsigned int inputClock : 1;   // 0 = 12..13 MHz, 1 = 24..26 MHz
  
  // SCI_STATUS
  unsigned int referenceVoltage : 1; // 0 = 1.23V, 1 = 1.65V
  unsigned int analogInternalPowerDown : 1;
  unsigned int analogDriverPowerDown : 1;
  unsigned int version : 4;      // 4 for VS1053/VS8053.
  unsigned int reserved : 2;
  unsigned int vcmDisable : 1;   // 0 = GBUF overload detection active, 1 = disabled
  unsigned int vcmOverload : 1;  // 1 = GBUF overload
  unsigned int swing : 3;        // 0 = 0dB, 1 = +0.5dB, 2 = 1dB.  Other values overdrive the dac, and should not be used.
  unsigned int doNotJump : 1;    // 1 if in  decoding headers, and jump forbidden

  // SCI_BASS
  unsigned int bassFreq : 4;         // 10 Hz steps, 2-15
  unsigned int bassAmplitude : 4;    // 1dB steps, 1-15, 0 = off
  unsigned int trebleFreq : 4;       // 1000 Hz steps, 1-15
  unsigned int trebleAmplitude : 4;  // 1.5 dB steps, -8 to 7, 0 = off
  
  // SCI_CLOCK
  unsigned int clockStuff : 16;      // TODO
  
  // SCI_DECODE_TIME
  uint16_t decodeTime; // Current decoded time in seconds.
  
  // SCI_AUDATA - current track metadata
  unsigned int stereo : 1;  // 0 == mono, 1 for stereo
  unsigned int sampleRate : 15; // Sample rate divided by 2

  uint16_t wRam;   // RAM write/read
  uint16_t wRamAddr; // base address for ram write/read
  uint16_t hDat0;  // stream header data 0
  uint16_t hDat1;  // stream header data 1 
  uint16_t aiaddr; // start address of appplication
  
  // SCI_VOL
  uint8_t leftVol;   // Attenuation, so 0 is loud, and 0xFE is silence
  uint8_t rightVol;  

  // and the 4 SCI_AICTRL registers
  uint16_t aiCtrl[4];
};


class MP3Player {
  public:
//  private:
  int pinXCS;  // Control Chip Select Pin (for accessing SPI Control/Status registers)
  int pinXDCS; // Data Chip Select Pin.
  int pinDREQ; // Data Request Pin: Player asks for more data

//  public:
  MP3Player(int xcs, int xdcs, int dreq);
  void begin();

  /** Play the file from the SD library.  Assumes that the file is opened, 
      positioned at the start of the file, and should be played until the end of the file.
      This method will block until the song is played. 
  void playFromSDCard(File file); */
  
};

#endif MP3Player_h
