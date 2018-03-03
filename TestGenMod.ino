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



unsigned long freq =  150000000ULL;              // Change this




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
    while (1);
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



}


void loop()
{
 Serial.print(minute());
  Serial.print(":");


  Serial.println(second());
   Serial.print(":");
  Serial.println(timeSet);


 si5351.set_freq(freq, SI5351_CLK0);
   si5351.update_status();


   delay(500);

   
 si5351.set_freq(freq * 2, SI5351_CLK0);
   si5351.update_status();

   delay(500);
    si5351.output_enable(SI5351_CLK0, 0);
    
   delay(500);
     si5351.output_enable(SI5351_CLK0, 1);
     
   delay(500);
 
}


