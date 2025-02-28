#ifndef __M10_GPS_HPP__
#define __M10_GPS_HPP__

#include "stand_type.hpp"

using namespace RGB_LED_NCP5623B;
using namespace RGB_LED_IS31fl3195;
using namespace iSentek_IST8310;

class Compass_IST8310
{
public:
    Compass_IST8310(TwoWire &wire = Wire, uint8_t address = IST8310_ADDR);
    bool begin();
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
    int getYawAngle();

private:
    void writeRegister(uint8_t reg, uint8_t value);
    uint8_t readRegister(uint8_t reg);
    bool readRegisters(uint8_t reg, uint8_t *buffer, size_t length);

private:
    TwoWire &_wire;
    uint8_t _address;
};

class RGB_NCP5623
{
public:
    RGB_NCP5623(TwoWire &wire = Wire, uint8_t address = NCP5623B_ADDR);
    void ledControl(led_control_s led);
    void setBrightness(uint8_t brightness);
    void red();
    void blue();
    void green();
    void yellow();
    void purple();
    void cyan();
    void white();
    void ledOff();

private:
    void sendData();

    TwoWire &_wire;
    uint8_t _address;

    uint8_t _r{0};
    uint8_t _g{0};
    uint8_t _b{0};
    uint8_t _red{NCP5623_LED_PWM0};
    uint8_t _green{NCP5623_LED_PWM1};
    uint8_t _blue{NCP5623_LED_PWM2};

    led_control_s led;
    float _brightness{0.50f};
};

class RGB_IS31fl3195
{
public:
    RGB_IS31fl3195(TwoWire &wire = Wire, uint8_t address = IS31fl3195_ADDR);
    void ledControl(led_control_s led);
    void setBrightness(uint8_t brightness);
    void red();
    void blue();
    void green();
    void yellow();
    void purple();
    void cyan();
    void white();
    void ledOff();

private:
    void sendData();

    TwoWire &_wire;
    uint8_t _address;

    uint8_t _r{0};
    uint8_t _g{0};
    uint8_t _b{0};

    uint8_t   _red{OUT_CURRENT1};
	uint8_t   _green{OUT_CURRENT2};
	uint8_t   _blue{OUT_CURRENT3};
	uint8_t   _current_band{CURRENT_BAND_CB_P5};

    led_control_s led;
    float _brightness{0.50f};
};

class ExternalIO
{
public:
    ExternalIO();
    void setBuzzer(State state);
    void set3V3(State state);
    void setLED(State state);
    int getSwitch() const;

private:
    void init();
    const uint8_t buzzer_pin{7};
    const uint8_t v3_3_pin{6};
    const uint8_t led_pin{5};
    const uint8_t switch_pin{4};
};

#endif