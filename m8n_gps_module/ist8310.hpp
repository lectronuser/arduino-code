#ifndef __IST8310_HPP__
#define __IST8310_HPP__

#include "ist8310_registers.hpp"

using namespace iSentek_IST8310;

class IST8310
{
public:
    IST8310(TwoWire &wire = Wire, uint8_t address = I2C_ADDRESS_DEFAULT);
    bool begin();
    uint8_t whoAmI();
    bool readMagnetometer(int16_t &mx, int16_t &my, int16_t &mz);
    bool readTemperature(int16_t &temp);
    void writeRegister(uint8_t reg, uint8_t value);
    uint8_t readRegister(uint8_t reg);
    bool readRegisters(uint8_t reg, uint8_t *buffer, size_t length);
    void setSingleMeasurementMode();
    void softReset();
    bool isDataReady();
    void setI2CAddress(uint8_t address);
    void setOutputDataRate(uint8_t odr);
    void setResolution(uint8_t xRes, uint8_t yRes, uint8_t zRes);
    void enableContinuousMeasurementMode();
    int getYawAngle();
private:
    TwoWire &_wire;
    uint8_t _address;
};

#endif