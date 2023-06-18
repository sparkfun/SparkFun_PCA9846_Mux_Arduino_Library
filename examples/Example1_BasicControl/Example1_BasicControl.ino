/*
  Use the PCA9846 Qwiic Mux to access multiple I2C devices on seperate busses.
  By: Paul Clark @ SparkFun Electronics
  Date: June 18th, 2023

  Some I2C devices respond to only one I2C address. This can be a problem
  when you want to hook multiple of a device to the I2C bus. An I2C Mux
  solves this issue by allowing you to change the 'channel' or port that
  the master is talking to.

  This example shows how to connect to different ports.
  The PCA9846 is a mux. This means when you setPort(1) then the SDA and SCL lines of the Arduino I2C port
  are connected to port 1 (only). All I2C traffic will be passed to/from to whatever sensor you have on port 1.

  Hardware Connections:
  Attach the PCA9846 Qwiic Mux to your RedBoard or Uno.
  Plug a device into port 1
  Serial.print it out at 115200 baud to serial monitor.

  SparkFun labored with love to create this code. Feel like supporting open
  source? Buy a board from SparkFun!
  https://www.sparkfun.com/products/14685
*/

#include <Wire.h>

#include <SparkFun_PCA9846.h> //Click here to get the library: http://librarymanager/All#SparkFun_PCA9846_Mux
SparkFun_PCA9846 myMux;

void setup()
{
  delay(1000);

  Serial.begin(115200);
  Serial.println();
  Serial.println("PCA9846 Qwiic Mux Read Example");

  Wire.begin();

  if (myMux.begin() == false)
  {
    Serial.println("Mux not detected. Freezing...");
    while (1)
      ;
  }
  Serial.println("Mux detected");
}

void loop()
{
  Serial.println();

  // Scan each port (0-3) in turn
  static uint8_t port = 3;
  port++;
  if (port == 4)
    port = 0;

  myMux.setPort(port); //Connect I2C bus to port

  uint8_t currentPortNumber = myMux.getPort();

  Serial.print("Scanning for I2C devices on main bus and port ");
  Serial.println(currentPortNumber);

  int nDevices = 0;
  for (uint8_t address = 1; address < 127; address++)
  {
    Wire.beginTransmission(address);
    int error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address < 0x10)
        Serial.print("0");
      Serial.print(address, HEX);
      Serial.println();

      nDevices++;
    }
    else if (error == 4)
    {
      Serial.print("Unknown error at address 0x");
      if (address < 0x10)
        Serial.print("0");
      Serial.println(address, HEX);
    }
  }

  if (nDevices == 0)
    Serial.println("No I2C devices found");
  else
    Serial.println("Done");

  delay(1000);
}