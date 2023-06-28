/*
  This is an Arduino library written for the PCA9846 4-port multiplexer.
  By Paul Clark @ SparkFun Electronics, June 18th, 2023

  The PCA9846 allows up to 4 devices to be attached to a single
  I2C bus. This is helpful for I2C devices that have a single I2C address.

  https://github.com/sparkfun/SparkFun_PCA9846_Mux_Arduino_Library

  SparkFun labored with love to create this code. Feel like supporting open
  source? Buy a board from SparkFun!
  https://www.sparkfun.com/products/22362
*/

#pragma once

#include "sfe_bus.h"

#define SFE_PCA9846_MUX_DEFAULT_ADDRESS 0x70   // A0 = SCL, A1 = GND, Unshifted
#define SFE_PCA9846_MUX_DEVICE_ID_ADDRESS 0x7C // Unshifted
#define SFE_PCA9846_MUX_DEVICE_ID 0x000858

class QwDevPCA9846
{
public:
    QwDevPCA9846() : _i2cAddress{SFE_PCA9846_MUX_DEFAULT_ADDRESS} {};

    ///////////////////////////////////////////////////////////////////////
    // init()
    //
    // Called to init the system. Connects to the device and sets it up for
    // operation

    bool init();

    ///////////////////////////////////////////////////////////////////////
    // isConnected()
    //
    //
    //  Parameter   Description
    //  ---------   -----------------------------
    //  retval      true if device is connected, false if not connected

    bool isConnected(); // Checks if sensor ack's the I2C request

    //////////////////////////////////////////////////////////////////////////////////
    // write()
    //
    //
    //  Parameter    Description
    //  ---------    -----------------------------
    //  data         The data to be written
    //  retval       false = error, true = success

    bool write(uint8_t data);

    //////////////////////////////////////////////////////////////////////////////////
    // writeRegisterRegion()
    //
    //
    //  Parameter    Description
    //  ---------    -----------------------------
    //  reg          register to write to
    //  data         Array containing the data to be written
    //  length       Length of the data to written
    //  retval       false = error, true = success

    bool writeRegisterRegion(uint8_t reg, uint8_t *data, uint16_t length);

    //////////////////////////////////////////////////////////////////////////////////
    // read()
    //
    //
    //  Parameter    Description
    //  ---------    -----------------------------
    //  data         Array to store data in
    //  retval       false = error, true = success

    bool read(uint8_t *data);

    //////////////////////////////////////////////////////////////////////////////////
    // readRegisterRegion()
    //
    //
    //  Parameter    Description
    //  ---------    -----------------------------
    //  reg          register to read from
    //  data         Array to store data in
    //  length       Length of the data to read
    //  retval       false = error, true = success

    bool readRegisterRegion(uint8_t reg, uint8_t *data, uint16_t length);

    //////////////////////////////////////////////////////////////////////////////////
    // setCommunicationBus()
    //
    // Called to set the Communication Bus object to use
    //
    //  Parameter    Description
    //  ---------    -----------------------------
    //  theBus       The Bus object to use
    //  idBus        The bus ID for the target device.
    //
    void setCommunicationBus(sfe_PCA9846::QwIDeviceBus &theBus, uint8_t i2cAddress);
    void setCommunicationBus(sfe_PCA9846::QwIDeviceBus &theBus);

    uint32_t getUniqueId();

    bool setPort(uint8_t portNumber);     // Enable a single port. All other ports disabled.
    bool setPortState(uint8_t portBits);  // Overwrite port register with all 4 bits. Allows multiple bit writing in one call.
    uint8_t getPort();                    // Returns the bit position of the first enabled port. Useful for IDing which port number is enabled.
    uint8_t getPortState();               // Returns current 4-bit wide state. May have multiple bits set in 4-bit field.
    bool enablePort(uint8_t portNumber);  // Enable a single port without affecting other bits
    bool disablePort(uint8_t portNumber); // Disable a single port without affecting other bits

private:
    sfe_PCA9846::QwIDeviceBus *_sfeBus;
    uint8_t _i2cAddress;
};
