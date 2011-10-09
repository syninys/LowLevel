
#include <MP3Player.h>
#include <SPI.h>
#include <SD.h>

#define BUFSIZE 255

#define SDCARD_CS 4

// For get_free_memory
extern unsigned int __bss_end;
extern void *__brkval;

Sd2Card card;
SdVolume volume;
SdFile root;
SdFile cwd;
MP3Player mp3Player(9, 2, 3); // XCS, XDCS, DREQ

char buffer[BUFSIZE];
int bufpos;

void setup() {
  pinMode(10, OUTPUT); // Ensure it's set as output, else SPI lib might put us in slave mode.  Other pins are setup by thier libs
  Serial.begin(115200);
  mp3Player.begin();

  delay(500);

  if (!card.init(SPI_HALF_SPEED, SDCARD_CS)) haltSD("card init");
  if (!volume.init(card)) haltSD("volume init");
  if (!root.openRoot(&volume)) haltSD("openRoot");
  cwd = root;
  bufpos = 0;
  
  Serial.println("> ");
}

void loop() {
  if(Serial.available()) {
   // Read into the buffer, next position, then decide what to do.
   switch(buffer[bufpos] = Serial.read()) {
     case '\n':
     case '\r':
       // Command complete, process
       Serial.print('\n');
       buffer[bufpos] = '\0';
       bufpos = 0; // Read next command
       processCommand();
       break;
     default:
       Serial.print(buffer[bufpos]);
       bufpos++;
       if(bufpos >= BUFSIZE) {
          Serial.println("\nLine too long");
          bufpos = 0;
       }
       // wait for next char.
       break;
     // special characters could be trapped here for single character commands
   }
  }  
}

#define READ_BUF_SIZE 16

void processCommand() {
  SdFile tmp;
  char *filename; 
  int result;
  char * buf[READ_BUF_SIZE];
  
  Serial.print("\nP: ");
  Serial.println(buffer);
  // Giant lookup table on first character, then more subtle procesing if needed
  switch(buffer[0]) {
    case 'l':
       // ls
       cwd.ls();
       break;
     case 'c':
       //cd
       // FIXME: Check file exists!
       filename = findNextArg(buffer + 1);
       if(filename[0] == '/') {
          Serial.println("root");
          cwd = root;
          break;
       }
       result = tmp.open(cwd, filename, O_RDONLY);
       if(!result) {
           Serial.print("Cant open dir: ");
           Serial.print(filename);
       } else {
           cwd = tmp;
       }
       break;
     case 'h':
       // head
       filename = findNextArg(buffer + 1);
       result = tmp.open(cwd, filename, O_RDONLY);
       if(!result) {
           Serial.println("Can't open file: ");
           Serial.print(filename);
       } else {
          tmp.rewind();
          tmp.read(&buf, READ_BUF_SIZE);
          hexDump(buf[0], READ_BUF_SIZE);
          tmp.close();
       }
       break;
     case 'r':
       // root:
       cwd = root;
       break;
     case 'm':
       // mem
       Serial.print("mem = ");
       Serial.println(get_free_memory());
       break;
  }
  // Print the prompt after processing any comments
  Serial.print("> ");
  
}

void hexDump(char * startAddress, unsigned int bytes) {
  char textString[8];
  for(int i = 0; i < bytes; i++) {
    sprintf(textString, "%02X ", startAddress[i]);
    Serial.print(textString);
  }
  Serial.println("");
  for(int i = 0; i < bytes; i++) {
    char thing = startAddress[i];
    if(thing > 0x20 && thing < 0x7F) {
      Serial.print(thing);
    } else {
      Serial.print(".");
    }
  Serial.print("  ");
  }
  Serial.println("");
}

int get_free_memory()
{
  int free_memory;

  if((int)__brkval == 0)
    free_memory = ((int)&free_memory) - ((int)&__bss_end);
  else
    free_memory = ((int)&free_memory) - ((int)__brkval);

  return free_memory;
}


/** Gives the location of the first char of the next word in teh buffer, or NULL if there is none. */
char * findNextArg(char * loc) {
  return scanToChar(scanToSpace(loc));
} 

char * scanToSpace(char * loc) {
  while(true) {
    if(NULL == loc || NULL == *loc) {
      return NULL;
    }
    if(*loc == ' ') return loc;
    loc++;
  }
  return loc;
}

char * scanToChar(char * loc) {
  while(true) {
    if(NULL == loc || NULL == *loc) {
      return NULL;
    }
    if(*loc > 33) return loc; // 33 is first noncontrol, nonswhitespace character
    loc++;
  }
  return loc;
}

void haltSD(char * message) {
  Serial.println(message);
  Serial.print("SD error code: ");
  Serial.print(card.errorCode(), HEX);
  Serial.print(" , ");
  Serial.println(card.errorData(), HEX);
  while(1) {
    delay(1000);
  }
  // And it's reset button time
}

