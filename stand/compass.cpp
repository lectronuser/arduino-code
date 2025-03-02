#include "compass.hpp"

Compass::Compass(TwoWire &wire, uint8_t address) : _wire(wire),
                                                   _address(address)
{
}

void Compass::begin()
{
    _wire.begin();
    _wire.setClock(I2C_SPEED);
    is_ready = (whoAmI() == Device_ID);
}

uint8_t Compass::whoAmI()
{
    return readRegister(Register::WAI);
}

bool Compass::readMagnetometer(int16_t &mx, int16_t &my, int16_t &mz)
{
    uint8_t buffer[6];
    bool success = readRegisters(Register::DATAXL, buffer, 6);

    if (!success)
    {
        return false;
    }

    mx = (int16_t(buffer[1]) << 8) | buffer[0];
    my = (int16_t(buffer[3]) << 8) | buffer[2];
    mz = (int16_t(buffer[5]) << 8) | buffer[4];

    return true;
}

bool Compass::readTemperature(int16_t &temp)
{
    uint8_t buffer[2];
    bool success = readRegisters(Register::TEMPL, buffer, 2);

    if (!success)
    {
        return false;
    }

    temp = (int16_t(buffer[1]) << 8) | buffer[0];

    return true;
}

void Compass::writeRegister(uint8_t reg, uint8_t value)
{
    _wire.beginTransmission(_address);
    _wire.write(reg);
    _wire.write(value);
    _wire.endTransmission();
}

uint8_t Compass::readRegister(uint8_t reg)
{
    _wire.beginTransmission(_address);
    _wire.write(reg);
    _wire.endTransmission(false);
    _wire.requestFrom(_address, uint8_t(1));
    return _wire.available() ? _wire.read() : 0;
}

bool Compass::readRegisters(uint8_t reg, uint8_t *buffer, size_t length)
{
    _wire.beginTransmission(_address);
    _wire.write(reg);
    if (_wire.endTransmission(false) != 0)
    {
        return false;
    }

    _wire.requestFrom(_address, length);
    for (size_t i = 0; i < length && _wire.available(); i++)
    {
        buffer[i] = _wire.read();
    }

    return true;
}

void Compass::setSingleMeasurementMode()
{
    writeRegister(Register::CNTL1, CNTL1_BIT::MODE_SINGLE_MEASUREMENT);
}

void Compass::softReset()
{
    writeRegister(Register::CNTL2, CNTL2_BIT::SRST);
    delay(100);
}

bool Compass::isDataReady()
{
    uint8_t status = readRegister(Register::STAT1);
    return (status & STAT1_BIT::DRDY);
}

void Compass::setI2CAddress(uint8_t address)
{
    _address = address;
}

void Compass::setOutputDataRate(uint8_t odr)
{
    writeRegister(Register::CNTL1, odr);
}

void Compass::setResolution(uint8_t xRes, uint8_t yRes, uint8_t zRes)
{
    uint8_t cntl3Val = 0;
    if (xRes == 16)
        cntl3Val |= CNTL3_BIT::X_16BIT;
    if (yRes == 16)
        cntl3Val |= CNTL3_BIT::Y_16BIT;
    if (zRes == 16)
        cntl3Val |= CNTL3_BIT::Z_16BIT;

    writeRegister(Register::CNTL3, cntl3Val);
}

void Compass::enableContinuousMeasurementMode()
{
    writeRegister(Register::CNTL1, CNTL1_BIT::MODE_SINGLE_MEASUREMENT);
    writeRegister(Register::CNTL2, 0x01);
}

int Compass::getYawAngle()
{
    int16_t mx, my, mz;
    writeRegister(CNTL1, MODE_SINGLE_MEASUREMENT);

    delay(20);

    if (readMagnetometer(mx, my, mz))
    {
        float yaw = atan2((float)my, (float)mx) * 180.0 / PI;

        if (yaw < 0)
        {
            yaw += 360;
        }

        return static_cast<int>(yaw);
    }
    else
    {
        return -1;
    }
}
