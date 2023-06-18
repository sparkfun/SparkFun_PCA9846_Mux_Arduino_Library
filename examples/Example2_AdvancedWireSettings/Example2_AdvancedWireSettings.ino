/*
  Use the PCA9846 Qwiic Mux to access multiple I2C devices on seperate busses.
  By: Paul Clark @ SparkFun Electronics
  Date: June 18th, 2023

  This library supports different Wire ports and different I2C addresses
  of the mux itself. By sending the mux address to the library you can
  have up to 8 mux boards connected with 32 identical devices behind them!

  This example shows how to start the lib with a different Wire port and 
  a different I2C address on the mux. It will fail on the Uno because the Uno
  has only one Wire port.

  The PCA9846 appears on two I2C addresses:
  * The mux address defined by the A0-A4 jumpers
  * Address 0x7C - which is used to read the Device ID

  Jumper Address Selection (Unshifted):
  Address A4     A3     A2     A1     A0
  0x70    Open   Open   Open   Open   Closed
  0x71    Open   Open   Open   Closed Open
  0x72    Open   Open   Closed Open   Open
  0x73    Open   Closed Open   Open   Open
  0x74    Closed Open   Open   Open   Closed
  0x75    Closed Open   Open   Closed Open
  0x76    Closed Open   Closed Open   Open
  0x77    Closed Closed Open   Open   Open

  The library can read the 24-bit Device ID using the special command getUniqueId

  Hardware Connections:
  Close the A4 jumper on the Mux board to change the address to 0x74.
  Attach the Qwiic Mux to your RedBoard or Uno.
  Serial.print it out at 115200 baud to serial monitor.

  SparkFun labored with love to create this code. Feel like supporting open
  source? Buy a board from SparkFun!
  https://www.sparkfun.com/products/22362
*/

#include <Wire.h>

#include <SparkFun_PCA9846.h> //Click here to get the library: http://librarymanager/All#SparkFun_PCA9846_Mux
SparkFun_PCA9846 myMux;

#define myWire Wire1 // Use Wire1. Change this if needed

void setup()
{
  delay(1000);

  Serial.begin(115200);
  Serial.println("PCA9846 Qwiic Mux Read Example");

  myWire.begin(); //This line will fail to compile on an Uno. Use a dev platform with multiple Wire ports

  //Setup the mux to use myWire (Wire1). If you have multiple muxes, pass the address as the second argument.
  //The A4 jumper must be closed with solder to use address 0x74. The default jumper configuration sets the address to 0x70.
  if (myMux.begin(myWire, 0x74) == false)
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