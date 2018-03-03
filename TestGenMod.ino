/*
   Time_NTP.pde
   Example showing time sync to NTP time source

   This sketch uses the Ethernet library
*/


#include <JTEncode.h>
#include <int.h>
#include <TimeLib.h>
#include <WiFi.h>
#include <WiFiUdp.h>

#include "si5351.h"
#include "Wire.h"


Si5351 si5351;

const byte numChars = 32;
char receivedChars[numChars]; // an array to store the received data

boolean newData = false;

unsigned long freq =  1500000000ULL;              // Change this

uint32_t MHz, KHz, Hz, mHz;




void setup()
{
  bool i2c_found;
  
  Serial.begin(9600);

  while (!Serial) ; // Needed for Leonardo only
  delay(250);

  // Initialize the Si5351

 i2c_found = si5351.init(SI5351_CRYSTAL_LOAD_8PF, 0, 0);
  if(!i2c_found)
  {
    Serial.println("Device not found on I2C bus!");
 //   while (1);
  }

  Serial.println("OK!");

  si5351.drive_strength(SI5351_CLK0, SI5351_DRIVE_8MA);
    si5351.set_correction(15560, SI5351_PLL_INPUT_XO); //correction in ppb, positive if Xtal higher than nominal

 si5351.set_freq(freq, SI5351_CLK0);
   si5351.update_status();

  
  delay(500);
   si5351.set_freq(freq, SI5351_CLK0);
   si5351.update_status();
  si5351.output_enable(SI5351_CLK0, 1);
   PrintFreq();



}


void loop()
{

  recvWithEndMarker();
 showNewData();


   delay(500);

   
 si5351.set_freq(freq * 2, SI5351_CLK0);
   si5351.update_status();

   
 
}


void recvWithEndMarker() {
 static byte ndx = 0;
 char endMarker = '\n';
 char rc;
 
 // if (Serial.available() > 0) {
           while (Serial.available() > 0 && newData == false) {
 rc = Serial.read();

 if (rc != endMarker) {
 receivedChars[ndx] = rc;
 ndx++;
 if (ndx >= numChars) {
 ndx = numChars - 1;
 }
 }
 else {
 receivedChars[ndx] = '\0'; // terminate the string
 ndx = 0;
 newData = true;
 }
 }
}

void PrintFreq(void)
{
   MHz = freq / 100000000ULL;
 KHz = (freq - MHz*100000000)/100000;
 Hz =  (freq - MHz*100000000 - KHz * 100000)/100;
 mHz = (freq - MHz*100000000 - KHz * 100000 - Hz *100);
 Serial.print("MHz ");
  Serial.print(MHz);
   Serial.print(" KHz ");
  Serial.print(KHz);
   Serial.print(" Hz ");
  Serial.println(Hz);
   Serial.print("Frequency ");
  Serial.println(freq);
}

void showNewData() {
 if (newData == true) {
 Serial.print("This just in ... ");
 Serial.println(receivedChars);
 if (receivedChars[0] == 'M')
 {
  freq += 100000000;
 }
 if (receivedChars[0] == 'm')
 {
  freq -= 100000000; 
 }
  if (receivedChars[0] == 'K')
 {
  freq += 100000;
 }
 if (receivedChars[0] == 'k')
 {
  freq -= 100000; 
 }
   if (receivedChars[0] == 'H')
 {
  freq += 100;
 }
 if (receivedChars[0] == 'h')
 {
  freq -= 100; 
 }
 PrintFreq();
 newData = false;
 }
}


