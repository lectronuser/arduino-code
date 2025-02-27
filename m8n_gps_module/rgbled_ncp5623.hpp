#ifndef __RGBLED_NCP5623_HPP__
#define __RGBLED_NCP5623_HPP__

#include "rgbled_ncp5623_types.hpp"

class RGBLED_NCP5623
{
public:
    RGBLED_NCP5623(TwoWire &wire = Wire, uint8_t address = NCP5623B_I2C_ADDR);
    void ledControl(led_control_s led);
    void setBrightness(int brightness);

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

    float _brightness{0.50f};
};

#endif