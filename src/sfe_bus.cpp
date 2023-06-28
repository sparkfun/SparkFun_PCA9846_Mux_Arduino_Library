// sfe_bus.cpp
//
// SparkFun code, firmware, and software is released under the MIT
// License(http://opensource.org/licenses/MIT).
//
// SPDX-License-Identifier: MIT
//
//    The MIT License (MIT)
//
//    Copyright (c) 2023 SparkFun Electronics
//    Permission is hereby granted, free of charge, to any person obtaining a
//    copy of this software and associated documentation files (the "Software"),
//    to deal in the Software without restriction, including without limitation
//    the rights to use, copy, modify, merge, publish, distribute, sublicense,
//    and/or sell copies of the Software, and to permit persons to whom the
//    Software is furnished to do so, subject to the following conditions: The
//    above copyright notice and this permission notice shall be included in all
//    copies or substantial portions of the Software. THE SOFTWARE IS PROVIDED
//    "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
//    NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
//    PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
//    ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
//    CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

// The following classes specify the behavior for communicating over
// the Inter-Integrated Circuit (I2C) bus. For ease of implementation
// an abstract interface (QwIDeviceBus) is used.

#include "sfe_bus.h"
#include <Arduino.h>

//////////////////////////////////////////////////////////////////////////////////////////////////
// Constructor
//

namespace sfe_PCA9846
{

    QwI2C::QwI2C(void) : _i2cPort{nullptr}
    {
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////
    // I2C init()
    //
    // Methods to init/setup this device. The caller can provide a Wire Port, or this class
    // will use the default

    bool QwI2C::init(TwoWire &wirePort, bool bInit)
    {

        // if we don't have a wire port already
        if (!_i2cPort)
        {
            _i2cPort = &wirePort;

            if (bInit)
                _i2cPort->begin();
        }

        return true;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////
    // I2C init()
    //
    // Methods to init/setup this device. The caller can provide a Wire Port, or this class
    // will use the default
    bool QwI2C::init()
    {
        if (!_i2cPort)
            return init(Wire);
        else
            return false;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////
    // ping()
    //
    // Is a device connected?
    bool QwI2C::ping(uint8_t i2c_address)
    {

        if (!_i2cPort)
            return false;

        _i2cPort->beginTransmission(i2c_address);
        return _i2cPort->endTransmission() == 0;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////
    // write()
    //
    // Write a byte to the device

    bool QwI2C::write(uint8_t i2c_address, uint8_t dataToWrite)
    {

        if (!_i2cPort)
            return false;

        _i2cPort->beginTransmission(i2c_address);
        _i2cPort->write(dataToWrite);
        return _i2cPort->endTransmission() == 0;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////
    // writeRegisterByte()
    //
    // Write a byte to a register

    bool QwI2C::writeRegisterByte(uint8_t i2c_address, uint8_t offset, uint8_t dataToWrite)
    {

        if (!_i2cPort)
            return false;

        _i2cPort->beginTransmission(i2c_address);
        _i2cPort->write(offset);
        _i2cPort->write(dataToWrite);
        return _i2cPort->endTransmission() == 0;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////
    // writeRegisterRegion()
    //
    // Write a block of data to a device.

    bool QwI2C::writeRegisterRegion(uint8_t i2c_address, uint8_t offset, const uint8_t *data, uint16_t length)
    {
        if (!_i2cPort)
            return false;

        _i2cPort->beginTransmission(i2c_address);
        _i2cPort->write(offset);
        _i2cPort->write(data, (int)length);

        return _i2cPort->endTransmission() == 0;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // read()
    //
    // Reads a single byte from the device.
    //
    //
    bool QwI2C::read(uint8_t address, uint8_t *data)
    {
        if (!_i2cPort)
            return false;

        uint8_t nReturned = _i2cPort->requestFrom((int)address, (int)1, (int)true);

        // Check we received the correct number of bytes
        if (nReturned != 1)
            return false; // error

        // Copy the retrieved data to the data segment
        *data = _i2cPort->read();

        return true; // Success
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // readRegisterRegion()
    //
    // Reads a small block of data from an i2c register on the devices.
    //
    //
    bool QwI2C::readRegisterRegion(uint8_t address, uint8_t offset, uint8_t *data, uint8_t length)
    {
        if (!_i2cPort)
            return false;

        _i2cPort->beginTransmission(address);
        _i2cPort->write(offset);
        if (_i2cPort->endTransmission(false) != 0) // Do a restart
            return false; // error with the end transmission

        uint8_t nReturned = _i2cPort->requestFrom((int)address, (int)length, (int)true);

        // Check we received the correct number of bytes
        if (nReturned != length)
            return false; // error

        // Copy the retrieved data to the data segment
        for (uint8_t i = 0; i < nReturned; i++)
            *data++ = _i2cPort->read();

        return true; // Success
    }

}
