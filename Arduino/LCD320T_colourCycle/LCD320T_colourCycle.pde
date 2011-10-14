
#define LCD_RS 19 
#define LCD_REST 16 
#define LCD_WR 18 
#define LCD_CS 17 

// LCD colour cycle

void main_Write_COM(int DH) 
{ 
unsigned char i; 
int temp; 
digitalWrite(LCD_RS,LOW); 
digitalWrite(LCD_CS,LOW); 
for(i=0;i<16;i++) 
{ 
temp=(DH&0x01); 
if(temp) 
digitalWrite(i,HIGH);
else 
digitalWrite(i,LOW); 
DH=DH>>1; 
} 
digitalWrite(LCD_WR,LOW); 
digitalWrite(LCD_WR,HIGH); 
digitalWrite(LCD_CS,HIGH); 
} 
void main_Write_DATA(int DH) 
{ 
unsigned char i; 
int temp; 
digitalWrite(LCD_RS,HIGH); 
digitalWrite(LCD_CS,LOW); 
for(i=0;i<16;i++) 
{ 
temp=(DH&0x01); 
if(temp) 
digitalWrite(i,HIGH); 
else 
digitalWrite(i,LOW); 
DH=DH>>1; 
} 
digitalWrite(LCD_WR,LOW); 
digitalWrite(LCD_WR,HIGH); 
digitalWrite(LCD_CS,HIGH); 
} 
void main_W_com_data(int com1,int dat1) 
{ 
main_Write_COM(com1); 
main_Write_DATA(dat1); 
} 
void address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2) 
{ 
main_W_com_data(0x0002,x1>>8); // Column address start2 
main_W_com_data(0x0003,x1); // Column address start1 
main_W_com_data(0x0004,x2>>8); // Column address end2 
main_W_com_data(0x0005,x2); // Column address end1 
main_W_com_data(0x0006,y1>>8); // Row address start2 
main_W_com_data(0x0007,y1); // Row address start1 
main_W_com_data(0x0008,y2>>8); // Row address end2
main_W_com_data(0x0009,y2); // Row address end1 
main_Write_COM(0x0022); 
} 
void main_init(void) 
{ 
digitalWrite(LCD_REST,HIGH); 
delay(5); 
digitalWrite(LCD_REST,LOW); 
delay(10); 
digitalWrite(LCD_REST,HIGH); 
delay(20); 
// VENDOR 
main_W_com_data(0x0046,0x00A4); 
main_W_com_data(0x0047,0x0053); 
main_W_com_data(0x0048,0x0000); 
main_W_com_data(0x0049,0x0044); 
main_W_com_data(0x004a,0x0004); 
main_W_com_data(0x004b,0x0067); 
main_W_com_data(0x004c,0x0033); 
main_W_com_data(0x004d,0x0077); 
main_W_com_data(0x004e,0x0012); 
main_W_com_data(0x004f,0x004C); 
main_W_com_data(0x0050,0x0046); 
main_W_com_data(0x0051,0x0044); 
//240x320 window setting 
main_W_com_data(0x0002,0x0000); // Column address start2 
main_W_com_data(0x0003,0x0000); // Column address start1 
main_W_com_data(0x0004,0x0000); // Column address end2 
main_W_com_data(0x0005,0x00ef); // Column address end1 
main_W_com_data(0x0006,0x0000); // Row address start2 
main_W_com_data(0x0007,0x0000); // Row address start1 
main_W_com_data(0x0008,0x0001); // Row address end2 
main_W_com_data(0x0009,0x003f); // Row address end1 
// Display Setting 
main_W_com_data(0x0001,0x0006); // IDMON=0, INVON=1, NORON=1, PTLON=0 
main_W_com_data(0x0016,0x00C8); // MY=0, MX=0, MV=0, ML=1, BGR=0, TEON=0 0048 
main_W_com_data(0x0023,0x0095); // N_DC=1001 0101
main_W_com_data(0x0024,0x0095); // PI_DC=1001 0101 
main_W_com_data(0x0025,0x00FF); // I_DC=1111 1111 
main_W_com_data(0x0027,0x0002); // N_BP=0000 0010 
main_W_com_data(0x0028,0x0002); // N_FP=0000 0010 
main_W_com_data(0x0029,0x0002); // PI_BP=0000 0010 
main_W_com_data(0x002a,0x0002); // PI_FP=0000 0010 
main_W_com_data(0x002C,0x0002); // I_BP=0000 0010 
main_W_com_data(0x002d,0x0002); // I_FP=0000 0010 
main_W_com_data(0x003a,0x0001); // N_RTN=0000, N_NW=001 0001 
main_W_com_data(0x003b,0x0000); // P_RTN=0000, P_NW=001 
main_W_com_data(0x003c,0x00f0); // I_RTN=1111, I_NW=000 
main_W_com_data(0x003d,0x0000); // DIV=00 
delay(1); 
main_W_com_data(0x0035,0x0038); // EQS=38h 
main_W_com_data(0x0036,0x0078); // EQP=78h 
main_W_com_data(0x003E,0x0038); // SON=38h 
main_W_com_data(0x0040,0x000F); // GDON=0Fh 
main_W_com_data(0x0041,0x00F0); // GDOFF 
// Power Supply Setting 
main_W_com_data(0x0019,0x0049); // CADJ=0100, CUADJ=100, OSD_EN=1 ,60Hz 
main_W_com_data(0x0093,0x000F); // RADJ=1111, 100% 
delay(1); 
main_W_com_data(0x0020,0x0040); // BT=0100 
main_W_com_data(0x001D,0x0007); // VC1=111 0007 
main_W_com_data(0x001E,0x0000); // VC3=000 
main_W_com_data(0x001F,0x0004); // VRH=0011 
//VCOM SETTING 
main_W_com_data(0x0044,0x004D); // VCM=101 0000 4D 
main_W_com_data(0x0045,0x000E); // VDV=1 0001 0011 
delay(1); 
main_W_com_data(0x001C,0x0004); // AP=100 
delay(2); 
main_W_com_data(0x001B,0x0018); // GASENB=0, PON=0, DK=1, XDK=0, VLCD_TRI=0, STB=0 
delay(1); 
main_W_com_data(0x001B,0x0010); // GASENB=0, PON=1, DK=0, XDK=0, VLCD_TRI=0, STB=0 
delay(1); 
main_W_com_data(0x0043,0x0080); //set VCOMG=1
delay(2); 
// Display ON Setting 
main_W_com_data(0x0090,0x007F); // SAP=0111 1111 
main_W_com_data(0x0026,0x0004); //GON=0, DTE=0, D=01 
delay(1); 
main_W_com_data(0x0026,0x0024); //GON=1, DTE=0, D=01 
main_W_com_data(0x0026,0x002C); //GON=1, DTE=0, D=11 
delay(1); 
main_W_com_data(0x0026,0x003C); //GON=1, DTE=1, D=11 
// INTERNAL REGISTER SETTING 
main_W_com_data(0x0057,0x0002); // TEST_Mode=1: into TEST mode 
main_W_com_data(0x0095,0x0001); // SET DISPLAY CLOCK AND PUMPING CLOCK  TO SYNCHRONIZE 
main_W_com_data(0x0057,0x0000); // TEST_Mode=0: exit TEST mode 
//main_W_com_data(0x0021,0x0000); 
main_Write_COM(0x0022); 
} 
void Pant(unsigned int color) 
{ 
int i,j; 
address_set(0,0,239,319); 
for(i=0;i<320;i++) 
{ 
for (j=0;j<240;j++) 
{ 
main_Write_DATA(color); 
} 
} 
} 
void setup() 
{ 
unsigned char p; 
for(p=0;p<20;p++) 
{ 
pinMode(p,OUTPUT); 
} 
main_init();
} 
void loop() 
{ 
Pant(0xf800); //Red 
delay(1000); 
Pant(0X07E0); //Green 
delay(1000); 
Pant(0x001f); //Blue 
delay(1000); 
} 
