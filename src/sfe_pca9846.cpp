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

#include "sfe_pca9846.h"

//////////////////////////////////////////////////////////////////////////////
// init()
//
// init the system
//
// Return Value: false on error, true on success
//

bool QwDevPCA9846::init(void)
{
    //  do we have a bus yet? is the device connected?
    if (!_sfeBus->ping(_i2cAddress))
        return false;

    // I2C ready, now check that we're using the correct sensor before moving on.
    if (getUniqueId() != SFE_PCA9846_MUX_DEVICE_ID)
        return false;

    return true;
}

///////////////////////////////////////////////////////////////////////
// isConnected()
//
// Called to determine if a ISM330DHCX device, at the provided i2c address
// is connected.
//
//  Parameter   Description
//  ---------   -----------------------------
//  retVal      true if device is connected, false if not connected

bool QwDevPCA9846::isConnected()
{
    if (getUniqueId() != SFE_PCA9846_MUX_DEVICE_ID)
        return false;

    return true;
}

//////////////////////////////////////////////////////////////////////////////
// getUniqueId()
//
// Retrieves the the device's ID: 0b 0000 0000 0000 1000 0101 1000 = 0x000858
//

uint32_t QwDevPCA9846::getUniqueId()
{

    uint8_t chipID[3] = {0};
    int retVal = _sfeBus->readRegisterRegion(SFE_PCA9846_MUX_DEVICE_ID_ADDRESS, _i2cAddress << 1, chipID, 3);

    if (retVal != 0)
        return 0;

    uint32_t ID = ((uint32_t)chipID[0]) << 16;
    ID |= ((uint32_t)chipID[1]) << 8;
    ID |= chipID[2];

    return ID;
}

////////////////////////////////////////////////////////////////////////////////////
// setCommunicationBus()
//
// Method to set the bus object that is used to communicate with the device
//
//  Parameter    Description
//  ---------    -----------------------------
//  theBus       The communication bus object
//  i2cAddress   I2C address for the 6DoF

void QwDevPCA9846::setCommunicationBus(sfe_PCA9846::QwIDeviceBus &theBus, uint8_t i2cAddress)
{
    _sfeBus = &theBus;
    _i2cAddress = i2cAddress;
}

////////////////////////////////////////////////////////////////////////////////////
// setCommunicationBus()
//
// Overloaded option for setting the data bus (theBus) object to a SPI bus object.
//
//  Parameter    Description
//  ---------    -----------------------------
//  theBus       The communication bus object
//

void QwDevPCA9846::setCommunicationBus(sfe_PCA9846::QwIDeviceBus &theBus)
{
    _sfeBus = &theBus;
}

//////////////////////////////////////////////////////////////////////////////
// write()
//
// Writes data to the device
//
//  Parameter    Description
//  ---------    -----------------------------
//  data         Data to be written

int32_t QwDevPCA9846::write(uint8_t data)
{
    return _sfeBus->write(_i2cAddress, data);
}

//////////////////////////////////////////////////////////////////////////////
// writeRegisterRegion()
//
// Writes data to the given register
//
//  Parameter    Description
//  ---------    -----------------------------
//  offset       The register to be written to
//  data         Data to be written
//  length       Number of bytes to be written

int32_t QwDevPCA9846::writeRegisterRegion(uint8_t offset, uint8_t *data, uint16_t length)
{
    return _sfeBus->writeRegisterRegion(_i2cAddress, offset, data, length);
}

//////////////////////////////////////////////////////////////////////////////
// read()
//
// Reads data from the device
//
//  Parameter    Description
//  ---------    -----------------------------
//  data         Pointer to where data will be saved

int32_t QwDevPCA9846::read(uint8_t *data)
{
    return _sfeBus->read(_i2cAddress, data);
}

//////////////////////////////////////////////////////////////////////////////
// readRegisterRegion()
//
// Reads data from the given register, auto-incrementing fore each successive read
//
//  Parameter    Description
//  ---------    -----------------------------
//  offset       The register to be read from
//  data         Pointer to where data will be saved
//  length       Number of bytes to be read

int32_t QwDevPCA9846::readRegisterRegion(uint8_t offset, uint8_t *data, uint16_t length)
{
    return _sfeBus->readRegisterRegion(_i2cAddress, offset, data, length);
}

// Enables one port. Disables all others.
// If port number if out of range, disable all ports
bool QwDevPCA9846::setPort(uint8_t portNumber)
{
    uint8_t portValue = 0;

    if (portNumber > 3)
        portValue = 0; // If port number is out of range, turn off all ports
    else
        portValue = 1 << portNumber;

    if (_sfeBus->write(_i2cAddress, portValue) != 0)
        return (false); // Device did not ACK
    return (true);
}

// Returns the first port number bit that is set
// Returns 255 if no port is enabled
// Return 254 if there is an I2C error
uint8_t QwDevPCA9846::getPort()
{
    // Read the current mux settings
    uint8_t portBits;
    int retVal = _sfeBus->read(_i2cAddress, &portBits);
    if (retVal != 0)
        return 254;

    // Search for the first set bit, then return its location
    for (uint8_t x = 0; x < 4; x++)
    {
        if (portBits & (1 << x))
            return (x);
    }
    return (255); // Return no port set
}

// Writes a 4-bit value to mux
// Overwrites any other bits
// This allows us to enable/disable multiple ports at same time
bool QwDevPCA9846::setPortState(uint8_t portBits)
{
    if (_sfeBus->write(_i2cAddress, portBits) != 0)
        return (false); // Device did not ACK
    return (true);
}

// Gets the current port state
// Returns byte that may have multiple bits set
// Return 254 if there is an I2C error
uint8_t QwDevPCA9846::getPortState()
{
    uint8_t portBits;
    if (_sfeBus->read(_i2cAddress, &portBits) != 0)
        return 254;
    return portBits;
}

// Enables a specific port number
// This allows for multiple ports to be 'turned on' at the same time. Use with caution.
bool QwDevPCA9846::enablePort(uint8_t portNumber)
{
    if (portNumber > 3)
        portNumber = 3; // Error check

    // Read the current mux settings
    uint8_t settings = getPortState();

    if (settings == 254)
        return false;

    // Set the wanted bit to enable the port
    settings |= (1 << portNumber);

    return (setPortState(settings));
}

// Disables a specific port number
bool QwDevPCA9846::disablePort(uint8_t portNumber)
{
    if (portNumber > 3)
        portNumber = 3; // Error check

    // Read the current mux settings
    uint8_t settings = getPortState();

    if (settings == 254)
        return false;

    // Clear the wanted bit to disable the port
    settings &= ~(1 << portNumber);

    return (setPortState(settings));
}
