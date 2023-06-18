// sfe_bus.h
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

#pragma once

#include "Arduino.h"
#include <Wire.h>

namespace sfe_PCA9846
{

    // The following abstract class is used an interface for upstream implementation.
    class QwIDeviceBus
    {
    public:
        virtual bool ping(uint8_t address) = 0;

        virtual bool write(uint8_t address, uint8_t data) = 0;

        virtual bool writeRegisterByte(uint8_t address, uint8_t offset, uint8_t data) = 0;

        virtual int writeRegisterRegion(uint8_t address, uint8_t offset, const uint8_t *data, uint16_t length) = 0;

        virtual int read(uint8_t address, uint8_t *data) = 0;

        virtual int readRegisterRegion(uint8_t address, uint8_t offset, uint8_t *data, uint8_t length) = 0;
    };

    // The QwI2C device defines behavior for I2C implementation based around the TwoWire class (Wire).
    // This is Arduino specific.
    class QwI2C : public QwIDeviceBus
    {
    public:
        QwI2C(void);

        bool init();

        bool init(TwoWire &wirePort, bool bInit = false);

        bool ping(uint8_t address);

        bool write(uint8_t address, uint8_t data);

        bool writeRegisterByte(uint8_t address, uint8_t offset, uint8_t data);

        int writeRegisterRegion(uint8_t address, uint8_t offset, const uint8_t *data, uint16_t length);

        int read(uint8_t address, uint8_t *data);

        int readRegisterRegion(uint8_t address, uint8_t offset, uint8_t *data, uint8_t length);

    private:
        TwoWire *_i2cPort;
    };

};
