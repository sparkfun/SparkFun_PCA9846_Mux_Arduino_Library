/*
  Use the PCA9846 Qwiic Mux to access multiple I2C devices on seperate busses.
  By: Paul Clark @ SparkFun Electronics
  Date: June 18th, 2023

  This is the code we use to test the Qwiic Mux PCS9846 during production.

  SparkFun labored with love to create this code. Feel like supporting open
  source? Buy a board from SparkFun!
  https://www.sparkfun.com/products/22362
*/

#include <Wire.h>

#include <SparkFun_PCA9846.h> //Click here to get the library: http://librarymanager/All#SparkFun_PCA9846_Mux

SparkFun_PCA9846 myMux;

void setup()
{
  delay(1000);

  Serial.begin(115200);
  Serial.println();
  Serial.println("PCA9846 Qwiic Mux Production Test");

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop()
{
  delay(250); // Try every 0.25 seconds

  Wire.begin();

  Wire.beginTransmission(0x70); // Detect mux on address 0x70
  if (Wire.endTransmission() != 0)
  {
    digitalWrite(LED_BUILTIN, LOW);
    Wire.end();
    Serial.println(F("Error: nothing detected on address 0x70"));
    return;
  }
  
  //Initialize mux - reads the "Who Am I" Device ID from address 0x7C
  if (myMux.begin() == false)
  {
    digitalWrite(LED_BUILTIN, LOW);
    Wire.end();
    Serial.println(F("Error: mux .begin failed"));
    return;
  }

  //Enable all four ports, then read back the port configuration
  //This should check for shorts on the multiplexed ports
  if (myMux.setPortState(0x0F) == false)
  {
    digitalWrite(LED_BUILTIN, LOW);
    Wire.end();
    Serial.println(F("Error: mux .setPortState failed"));
    return;
  }
  if (myMux.getPortState() != 0x0F)
  {
    digitalWrite(LED_BUILTIN, LOW);
    Wire.end();
    Serial.println(F("Error: mux .getPortState failed"));
    return;
  }

  // All good!
  Serial.println(F("Test passed!"));
  digitalWrite(LED_BUILTIN, HIGH);
  Wire.end();
}