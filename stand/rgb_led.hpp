#ifndef __RGB_LED_HPP__
#define __RGB_LED_HPP__

#include "stand_type.hpp"

class RGB
{
private:
    uint8_t _color_brightness{0};

protected:
    TwoWire &_wire;
    led_control_s led_control;

    uint8_t _address;
    float _brightness{0.50f};
    uint8_t _r{0};
    uint8_t _g{0};
    uint8_t _b{0};
    uint8_t _red;
    uint8_t _green;
    uint8_t _blue;

protected:
    RGB(TwoWire &wire = Wire) : _wire(wire) {}
    virtual void updateLedColor(led_control_s led);
    virtual int sendLedRgb() = 0;
    virtual int sendLedEnable(bool enable) = 0;
    virtual void setColorBrightness(uint8_t brightness) { _color_brightness = brightness; }
    virtual void setLedReference(uint8_t red, uint8_t green, uint8_t blue);

public:
    virtual void setBrightness(uint8_t brightness) { _brightness = brightness / 255.f; }
    virtual void red();
    virtual void blue();
    virtual void green();
    virtual void yellow();
    virtual void purple();
    virtual void cyan();
    virtual void white();
    virtual void ledOff();
};

class RGB_NCP5623 : public RGB
{
public:
    RGB_NCP5623(uint8_t address = RGB_LED_NCP5623B::NCP5623B_ADDR);

private:
    int sendLedRgb() override;
    int sendLedEnable(bool enable) { return OK; }
};

class RGB_IS31fl3195 : public RGB
{
public:
    RGB_IS31fl3195(uint8_t address = RGB_LED_IS31fl3195::IS31fl3195_ADDR);

private:
    bool _leds_enabled{false};
    int sendLedRgb() override;
    uint8_t _current_band{CURRENT_BAND_CB_P5};
    int sendLedEnable(bool enable);
    int i2cWrite(uint8_t reg, uint8_t value);
};

#endif