
#define WE 0
#define OE 1
#define CE 2

#define ADDR0 3
#define ADDR1 4
#define ADDR2 5

#define DQ0 6
#define DQ1 7
#define DQ2 8

void setup() {
  pinMode(OUTPUT, WE);
  pinMode(OUTPUT, OE);
  pinMode(OUTPUT, CE);
  pinMode(OUTPUT, 13);
 
  pinMode(OUTPUT, ADDR0); 
  pinMode(OUTPUT, ADDR1); 
  pinMode(OUTPUT, ADDR2); 
  
  pinMode(OUTPUT, DQ0);
  pinMode(OUTPUT, DQ1);
  pinMode(OUTPUT, DQ2);
  
  digitalWrite(WE, HIGH);
  digitalWrite(OE, HIGH);  // Into the do nothing statsa
  delay(5);
  digitalWrite(CE, LOW); // And finally turn on the SRAM
  delay(5);
  for(int i = 0; i++; i <= 7) {
    writeVal(i, i);
  }
  
  // Disconnect the data pins
  pinMode(INPUT, DQ0);
  pinMode(INPUT, DQ1);
  pinMode(INPUT, DQ2);
}

// Call this with CE active, and WE and OE high
void writeVal(int addr, int val) {
  dWrite(ADDR0, addr & 0x1);
  dWrite(ADDR1, addr & 0x2);
  dWrite(ADDR2, addr & 0x4);
  dWrite(DQ0, val & 0x1);
  dWrite(DQ1, val & 0x2);
  dWrite(DQ2, val & 0x3);
  digitalWrite(WE, LOW);
  delay(1);
  digitalWrite(WE, HIGH);
}

// Call this with CE active, and WE high
int readVal(int addr) {
  digitalWrite(OE, HIGH);
  dWrite(ADDR0, addr & 0x1);
  dWrite(ADDR1, addr & 0x2);
  dWrite(ADDR2, addr & 0x4);
  digitalWrite(OE, LOW);  
}

int dWrite(int pin, boolean val) {
  if(val) 
    digitalWrite(pin, HIGH);
  else
    digitalWrite(pin, LOW);
}

int count = 0;
void loop() {
  readVal(count);
  count++;
  if(count > 7) count = 0;
  delay(500);
}
