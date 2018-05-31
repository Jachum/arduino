/*
  Tips for Using Plantower PMS7003 air quality sensor to a Particle IoT microcontroller
  Made by 2016 winduino 
  http://wemakers.tistory.com  
    
*/
#include <MyDisplay.hpp>
#include <SoftwareSerial.h> 
// #define DEBUG  
#define  MEAN_NUMBER  10
#define  MAX_PM   0
#define  MIN_PM   32767
SoftwareSerial serial_to_device(9, 6); // RX, TX
int incomingByte = 0; // for incoming serial data
const int MAX_FRAME_LEN = 64;
char frameBuf[MAX_FRAME_LEN];
int detectOff = 0;
int frameLen = MAX_FRAME_LEN;
bool inFrame = false;
char printbuf[256];
unsigned int calcChecksum = 0;
unsigned int pm1_0=0, pm2_5=0, pm10_0=0;
unsigned int tmp_max_pm1_0, tmp_max_pm2_5, tmp_max_pm10_0; 
unsigned int tmp_min_pm1_0, tmp_min_pm2_5, tmp_min_pm10_0; 
byte i=0;
MyDisplay disp;

struct PMS7003_framestruct {
    byte  frameHeader[2];
    unsigned int  frameLen = MAX_FRAME_LEN;
    unsigned int  concPM1_0_CF1;
    unsigned int  concPM2_5_CF1;
    unsigned int  concPM10_0_CF1;
    unsigned int  checksum;
} thisFrame;

void setup() {
    disp.print(8888);
    disp.refresh();
    
    Serial.begin(57600);
    delay(1000);
    Serial.println("-- Initializing...");
}


bool pms7003_read() {
#ifdef DEBUG  
    Serial.println("----- Reading PMS7003 -----");
#endif
    serial_to_device.begin(9600);
    bool packetReceived = false;
    calcChecksum = 0;
    while (!packetReceived) {
        disp.refresh();
        if (serial_to_device.available() > 32) {
            int drain = serial_to_device.available();
#ifdef DEBUG
                Serial.print("----- Draining buffer: -----");
                Serial.println(serial_to_device.available(), DEC);
#endif
            for (int i = drain; i > 0; i--) {
                serial_to_device.read();
            }
        }
        if (serial_to_device.available() > 0) {
#ifdef DEBUG
                Serial.print("----- Available: -----");
                Serial.println(serial_to_device.available(), DEC);
#endif
            incomingByte = serial_to_device.read();
#ifdef DEBUG
                Serial.print("----- READ: -----");
                Serial.println(incomingByte, HEX);
#endif
            if (!inFrame) {
                if (incomingByte == 0x42 && detectOff == 0) {
                    frameBuf[detectOff] = incomingByte;
                    thisFrame.frameHeader[0] = incomingByte;
                    calcChecksum = incomingByte; // Checksum init!
                    detectOff++;
                }
                else if (incomingByte == 0x4D && detectOff == 1) {
                    frameBuf[detectOff] = incomingByte;
                    thisFrame.frameHeader[1] = incomingByte;
                    calcChecksum += incomingByte;
                    inFrame = true;
                    detectOff++;
                }
                else {
                    Serial.print("----- Frame syncing... -----");
                    Serial.print(incomingByte, HEX);
                    Serial.println();
                }
            }
            else {
                frameBuf[detectOff] = incomingByte;
                calcChecksum += incomingByte;
                detectOff++;
                unsigned int  val = (frameBuf[detectOff-1]&0xff)+(frameBuf[detectOff-2]<<8);
                switch (detectOff) {
                    case 4:
                        thisFrame.frameLen = val;
                        frameLen = val + detectOff;
                        break;
                    case 6:
                        thisFrame.concPM1_0_CF1 = val;
                        break;
                    case 8:
                        thisFrame.concPM2_5_CF1 = val;
                        break;
                    case 10:
                        thisFrame.concPM10_0_CF1 = val;
                        break;
                    case 32:
                        thisFrame.checksum = val;
                        calcChecksum -= ((val>>8)+(val&0xFF));
                        break;
                    default:
                        break;
                }
                if (detectOff >= frameLen) {
#ifdef DEBUG          
                    sprintf(printbuf, "PMS7003 ");
                    sprintf(printbuf, "%s[%02x %02x] (%04x) ", printbuf,
                        thisFrame.frameHeader[0], thisFrame.frameHeader[1], thisFrame.frameLen);
                    sprintf(printbuf, "%sCF1=[%04x %04x %04x] ", printbuf,
                        thisFrame.concPM1_0_CF1, thisFrame.concPM2_5_CF1, thisFrame.concPM10_0_CF1);
                    sprintf(printbuf, "%scsum=%04x %s xsum=%04x", printbuf,
                        thisFrame.checksum, (calcChecksum == thisFrame.checksum ? "==" : "!="), calcChecksum);
                    Serial.println(printbuf);
#endif        
                    packetReceived = true;
                    detectOff = 0;
                    inFrame = false;
                }
            }
        }
    }
    serial_to_device.end();
    return (calcChecksum == thisFrame.checksum);
}
void loop () {
  if(i==0) { 
    tmp_max_pm1_0  = MAX_PM;
    tmp_max_pm2_5  = MAX_PM;
    tmp_max_pm10_0 = MAX_PM;
    tmp_min_pm1_0  = MIN_PM;
    tmp_min_pm2_5  = MIN_PM;
    tmp_min_pm10_0 = MIN_PM;
  }
  if (pms7003_read()) {
    tmp_max_pm1_0  = max(thisFrame.concPM1_0_CF1, tmp_max_pm1_0);
    tmp_max_pm2_5  = max(thisFrame.concPM2_5_CF1, tmp_max_pm2_5);
    tmp_max_pm10_0 = max(thisFrame.concPM10_0_CF1, tmp_max_pm10_0);
    tmp_min_pm1_0  = min(thisFrame.concPM1_0_CF1, tmp_min_pm1_0);
    tmp_min_pm2_5  = min(thisFrame.concPM2_5_CF1, tmp_min_pm2_5);
    tmp_min_pm10_0 = min(thisFrame.concPM10_0_CF1, tmp_min_pm10_0);
    pm1_0 += thisFrame.concPM1_0_CF1;
    pm2_5 += thisFrame.concPM2_5_CF1;
    pm10_0 += thisFrame.concPM10_0_CF1;
    i++;
    Serial.print("O");
  }
  else {
    Serial.print("*");
  }
  
  if(i==MEAN_NUMBER) {
  sprintf(printbuf, "[Checksum OK]");
  sprintf(printbuf, "%s PM1.0 = %02x, PM2.5 = %02x, PM10 = %02x", printbuf, 
    (pm1_0-tmp_max_pm1_0-tmp_min_pm1_0)/(MEAN_NUMBER-2), 
    (pm2_5-tmp_max_pm2_5-tmp_min_pm2_5)/(MEAN_NUMBER-2), 
    (pm10_0-tmp_max_pm10_0-tmp_min_pm10_0)/(MEAN_NUMBER-2));
  sprintf(printbuf, "%s [max =%02x,%02x,%02x, min = %02x,%02x,%02x]", printbuf,
    tmp_max_pm1_0, tmp_max_pm2_5, tmp_max_pm10_0,
    tmp_min_pm1_0, tmp_min_pm2_5, tmp_min_pm10_0);  
  Serial.println();
  Serial.println(printbuf);

  u16 value_to_display = (pm2_5-tmp_max_pm2_5-tmp_min_pm2_5)/(MEAN_NUMBER-2); 
  disp.print(value_to_display);
 
  pm1_0=pm2_5=pm10_0=i=0;
//  delay(20000);
  }
  
  //disp.refresh();
}
