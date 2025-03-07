#ifndef COMPASS_HPP
#define COMPASS_HPP

#include "stand_type.hpp"

using namespace IST8310;

// IST8310 compass
class Compass
{
public:
    Compass(TwoWire &wire = Wire, uint8_t address = IST8310_ADDR);
    void begin();
    uint8_t whoAmI();
    bool readMagnetometer(int16_t &mx, int16_t &my, int16_t &mz);
    bool readTemperature(int16_t &temp);
    void setSingleMeasurementMode();
    void softReset();
    bool isDataReady();
    void setI2CAddress(uint8_t address);
    void setOutputDataRate(uint8_t odr);
    void setResolution(uint8_t xRes, uint8_t yRes, uint8_t zRes);
    void enableContinuousMeasurementMode();
    float getYawAngle();
    bool getIsReady() const { return is_ready; }

private:
    void writeRegister(uint8_t reg, uint8_t value);
    uint8_t readRegister(uint8_t reg);
    bool readRegisters(uint8_t reg, uint8_t *buffer, size_t length);

private:
    TwoWire &_wire;
    uint8_t _address;
    bool is_ready = false;
};

#endif