#include "rgbled_ncp5623.hpp"

RGBLED_NCP5623::RGBLED_NCP5623(TwoWire &wire, uint8_t address) : _wire(wire),
                                                                 _address(address)
{
}

void RGBLED_NCP5623::ledControl(led_control_s led)
{
    switch (led.color)
    {
    case led_control_s::COLOR_RED:
        _r = NCP5623_LED_BRIGHT;
        _g = 0;
        _b = 0;
        break;
    case led_control_s::COLOR_GREEN:
        _r = 0;
        _g = NCP5623_LED_BRIGHT;
        _b = 0;
        break;
    case led_control_s::COLOR_BLUE:
        _r = 0;
        _g = 0;
        _b = NCP5623_LED_BRIGHT;
        break;
    case led_control_s::COLOR_YELLOW:
        _r = NCP5623_LED_BRIGHT;
        _g = NCP5623_LED_BRIGHT;
        _b = 0;
        break;
    case led_control_s::COLOR_PURPLE:
        _r = NCP5623_LED_BRIGHT;
        _g = 0;
        _b = NCP5623_LED_BRIGHT;
        break;
    case led_control_s::COLOR_CYAN:
        _r = 0;
        _g = NCP5623_LED_BRIGHT;
        _b = NCP5623_LED_BRIGHT;
        break;
    case led_control_s::COLOR_WHITE:
        _r = NCP5623_LED_BRIGHT;
        _g = NCP5623_LED_BRIGHT;
        _b = NCP5623_LED_BRIGHT;
        break;
    default:
        _r = 0;
        _g = 0;
        _b = 0;
        break;
    }
    sendData();
}

void RGBLED_NCP5623::sendData()
{
    uint8_t msg[7] = {0x20, 0x70, 0x40, 0x70, 0x60, 0x70, 0x80};
	uint8_t brightness = UINT8_MAX;

	msg[0] = NCP5623_LED_CURRENT | (brightness & 0x1f);
	msg[2] = _red | (uint8_t(_r * _brightness) & 0x1f);
	msg[4] = _green | (uint8_t(_g * _brightness) & 0x1f);
	msg[6] = _blue | (uint8_t(_b * _brightness) & 0x1f);

    Wire.beginTransmission(NCP5623B_I2C_ADDR);
    for(int i = 0; i < 7; i++)
    {
        Wire.write(msg[i]);
    }
    Wire.endTransmission();
}

void RGBLED_NCP5623::setBrightness(int brightness)
{
    if(brightness <= 255)
    {
        _brightness = brightness / 255.f;
    }
}