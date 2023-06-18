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
#include "sfe_pca9846.h"
#include "sfe_bus.h"
#include <Wire.h>

class SparkFun_PCA9846 : public QwDevPCA9846
{

public:
    SparkFun_PCA9846(){};

    ///////////////////////////////////////////////////////////////////////
    // begin()
    //
    // This method is called to initialize the PCA9846 library and connect to
    // the PCA9846 device. This method must be called before calling any other method
    // that interacts with the device.
    //
    // This method follows the standard startup pattern in SparkFun Arduino
    // libraries.
    //
    //  Parameter   Description
    //  ---------   ----------------------------
    //  wirePort    optional. The Wire port. If not provided, the default port is used
    //  address     optional. I2C Address. If not provided, the default address is used.
    //  retval      true on success, false on startup failure
    //
    // This methond is overridden, implementing two versions.
    //
    // Version 1:
    // User skips passing in an I2C object which then defaults to Wire.
    bool begin(uint8_t deviceAddress = SFE_PCA9846_MUX_DEFAULT_ADDRESS)
    {
        // Setup  I2C object and pass into the superclass
        setCommunicationBus(_i2cBus, deviceAddress);

        // Initialize the I2C buss class i.e. setup default Wire port
        _i2cBus.init();

        // Initialize the system - return results
        return this->QwDevPCA9846::init();
    }

    // Version 2:
    // User passes in an I2C object and an address (optional).
    bool begin(TwoWire &wirePort, uint8_t deviceAddress = SFE_PCA9846_MUX_DEFAULT_ADDRESS)
    {
        // Setup  I2C object and pass into the superclass
        setCommunicationBus(_i2cBus, deviceAddress);

        // Give the I2C port provided by the user to the I2C bus class.
        _i2cBus.init(wirePort, true);

        // Initialize the system - return results
        return this->QwDevPCA9846::init();
    }

private:
    // I2C bus class
    sfe_PCA9846::QwI2C _i2cBus;
};
