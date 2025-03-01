#include "rgb_led.hpp"

using namespace RGB_LED_NCP5623B;
using namespace RGB_LED_IS31fl3195;

void RGB::setLedReference(uint8_t red, uint8_t green, uint8_t blue)
{
    _red = red;
    _green = green;
    _blue = blue;
}

void RGB::updateLedColor(led_control_s led)
{
    switch (led.color)
    {
    case led_control_s::COLOR_RED:
        _r = _color_brightness;
        _g = 0;
        _b = 0;
        sendLedEnable(true);
        break;
    case led_control_s::COLOR_GREEN:
        _r = 0;
        _g = _color_brightness;
        _b = 0;
        sendLedEnable(true);
        break;
    case led_control_s::COLOR_BLUE:
        _r = 0;
        _g = 0;
        _b = _color_brightness;
        sendLedEnable(true);
        break;
    case led_control_s::COLOR_YELLOW:
        _r = _color_brightness;
        _g = _color_brightness;
        _b = 0;
        sendLedEnable(true);
        break;
    case led_control_s::COLOR_PURPLE:
        _r = _color_brightness;
        _g = 0;
        _b = _color_brightness;
        sendLedEnable(true);
        break;
    case led_control_s::COLOR_CYAN:
        _r = 0;
        _g = _color_brightness;
        _b = _color_brightness;
        sendLedEnable(true);
        break;
    case led_control_s::COLOR_WHITE:
        _r = _color_brightness;
        _g = _color_brightness;
        _b = _color_brightness;
        sendLedEnable(true);
        break;
    default:
        _r = 0;
        _g = 0;
        _b = 0;
        sendLedEnable(false);
        break;
    }
    sendLedRgb();
}

void RGB::red()
{
    led_control.color = led_control_s::COLOR_RED;
    updateLedColor(led_control);
}

void RGB::blue()
{
    led_control.color = led_control_s::COLOR_BLUE;
    updateLedColor(led_control);
}

void RGB::green()
{
    led_control.color = led_control_s::COLOR_GREEN;
    updateLedColor(led_control);
}

void RGB::yellow()
{
    led_control.color = led_control_s::COLOR_YELLOW;
    updateLedColor(led_control);
}

void RGB::purple()
{
    led_control.color = led_control_s::COLOR_PURPLE;
    updateLedColor(led_control);
}

void RGB::cyan()
{
    led_control.color = led_control_s::COLOR_CYAN;
    updateLedColor(led_control);
}

void RGB::white()
{
    led_control.color = led_control_s::COLOR_WHITE;
    updateLedColor(led_control);
}

void RGB::ledOff()
{
    led_control.color = led_control_s::COLOR_OFF;
    updateLedColor(led_control);
}

RGB_NCP5623::RGB_NCP5623(uint8_t address)
{
    setColorBrightness(NCP5623_LED_BRIGHT);
    setLedReference(NCP5623_LED_PWM0, NCP5623_LED_PWM1, NCP5623_LED_PWM2);
    _address = address;
}

int RGB_NCP5623::sendLedRgb()
{
    uint8_t msg[7] = {0x20, 0x70, 0x40, 0x70, 0x60, 0x70, 0x80};
    uint8_t brightness = UINT8_MAX;

    msg[0] = NCP5623_LED_CURRENT | (brightness & 0x1f);
    msg[2] = _red | (uint8_t(_r * _brightness) & 0x1f);
    msg[4] = _green | (uint8_t(_g * _brightness) & 0x1f);
    msg[6] = _blue | (uint8_t(_b * _brightness) & 0x1f);

    Wire.beginTransmission(NCP5623B_ADDR);
    for (int i = 0; i < 7; i++)
    {
        Wire.write(msg[i]);
    }
    return Wire.endTransmission();
}

RGB_IS31fl3195::RGB_IS31fl3195(uint8_t address)
{
    setColorBrightness(IS31fl3195_LED_BRIGHT);
    setLedReference(OUT_CURRENT1, OUT_CURRENT2, OUT_CURRENT3);
    _address = address;
    sendLedEnable(false);
}

int RGB_IS31fl3195::sendLedRgb()
{
    int ret = 0;
    const uint8_t leds[4][2] = {
        {_red, static_cast<uint8_t>(_r * _brightness)},
        {_green, static_cast<uint8_t>(_g * _brightness)},
        {_blue, static_cast<uint8_t>(_b * _brightness)},
        {IS31fl3195_COLOR_UPDATE, IS31fl3195_COLOR_UPDATE_KEY},
    };

    for (unsigned i = 0; i < sizeof(leds) / sizeof(leds[0]); i++) {
        ret = i2cWrite(leds[i][0], leds[i][1]);
  
        if (ret < 0) {
          break;
        }
    }

    return ret;
}

int RGB_IS31fl3195::sendLedEnable(bool enable)
{
    int ret = 0;

    if ((_leds_enabled ^ enable) == 0)
    {
        return ret;
    }

    _leds_enabled = enable;
    uint8_t shutdown_ctrl = RUN_MODE;

    if (enable)
    {
        shutdown_ctrl |= L1_L3_EN;
    }

    const uint8_t mode = MODE_CONFIG_LM_SINGLE |
                         MODE_CONFIG_OUT1_MODE(MODE_CONFIG_OUT_MODE_CURRENT) |
                         MODE_CONFIG_OUT2_MODE(MODE_CONFIG_OUT_MODE_CURRENT) |
                         MODE_CONFIG_OUT3_MODE(MODE_CONFIG_OUT_MODE_CURRENT);

    const uint8_t cb = CURRENT_BAND_CB(1, _current_band) |
                       CURRENT_BAND_CB(2, _current_band) |
                       CURRENT_BAND_CB(3, _current_band);

    const uint8_t init[][2] = {
        {SHUTDOWN_CTRL, shutdown_ctrl},
        {MODE_CONFIG, mode},
        {CHARGE_PUMP1, CHARGE_PUMP1_DEFAULT | CHARGE_PUMP1_CPM_1P5X},
        {CHARGE_PUMP2, CHARGE_PUMP2_HRT_150MV | CHARGE_PUMP2_CPDE(4, CHARGE_PUMP2_CPDE_DISABLE)},
        {CURRENT_BAND, cb},
    };

    for (unsigned i = 0; i < sizeof(init) / sizeof(init[0]); i++)
    {
        ret = i2cWrite(init[i][0], init[i][1]);

        if (ret < 0)
        {
            break;
        }
    }

    return ret;
}

int RGB_IS31fl3195::i2cWrite(uint8_t reg, uint8_t value)
{
    Wire.beginTransmission(_address);
    Wire.write(reg);
    Wire.write(value);
    return Wire.endTransmission();
}
