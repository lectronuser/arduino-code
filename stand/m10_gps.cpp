#include "m10_gps.hpp"

/********************************
    Compass (IST8310)
*********************************/
Compass_IST8310::Compass_IST8310(TwoWire &wire, uint8_t address) : _wire(wire),
                                                                   _address(address)
{
}

bool Compass_IST8310::begin()
{
    _wire.begin();
    _wire.setClock(I2C_SPEED);
    return whoAmI() == Device_ID;
}

uint8_t Compass_IST8310::whoAmI()
{
    return readRegister(Register::WAI);
}

bool Compass_IST8310::readMagnetometer(int16_t &mx, int16_t &my, int16_t &mz)
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

bool Compass_IST8310::readTemperature(int16_t &temp)
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

void Compass_IST8310::writeRegister(uint8_t reg, uint8_t value)
{
    _wire.beginTransmission(_address);
    _wire.write(reg);
    _wire.write(value);
    _wire.endTransmission();
}

uint8_t Compass_IST8310::readRegister(uint8_t reg)
{
    _wire.beginTransmission(_address);
    _wire.write(reg);
    _wire.endTransmission(false);
    _wire.requestFrom(_address, uint8_t(1));
    return _wire.available() ? _wire.read() : 0;
}

bool Compass_IST8310::readRegisters(uint8_t reg, uint8_t *buffer, size_t length)
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

void Compass_IST8310::setSingleMeasurementMode()
{
    writeRegister(Register::CNTL1, CNTL1_BIT::MODE_SINGLE_MEASUREMENT);
}

void Compass_IST8310::softReset()
{
    writeRegister(Register::CNTL2, CNTL2_BIT::SRST);
    delay(100);
}

bool Compass_IST8310::isDataReady()
{
    uint8_t status = readRegister(Register::STAT1);
    return (status & STAT1_BIT::DRDY);
}

void Compass_IST8310::setI2CAddress(uint8_t address)
{
    _address = address;
}

void Compass_IST8310::setOutputDataRate(uint8_t odr)
{
    writeRegister(Register::CNTL1, odr);
}

void Compass_IST8310::setResolution(uint8_t xRes, uint8_t yRes, uint8_t zRes)
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

void Compass_IST8310::enableContinuousMeasurementMode()
{
    writeRegister(Register::CNTL1, CNTL1_BIT::MODE_SINGLE_MEASUREMENT);
    writeRegister(Register::CNTL2, 0x01);
}

int Compass_IST8310::getYawAngle()
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

/********************************
    RGB LED (NCP5623)
*********************************/
RGB_NCP5623::RGB_NCP5623(TwoWire &wire, uint8_t address) : _wire(wire),
                                                           _address(address)
{
}

void RGB_NCP5623::ledControl(led_control_s led)
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

void RGB_NCP5623::sendData()
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
    Wire.endTransmission();
}

void RGB_NCP5623::setBrightness(uint8_t brightness)
{
    if (brightness <= 255)
    {
        _brightness = brightness / 255.f;
    }
}

void RGB_NCP5623::red()
{
    led.color = led_control_s::COLOR_RED;
    ledControl(led);
}

void RGB_NCP5623::blue()
{
    led.color = led_control_s::COLOR_BLUE;
    ledControl(led);
}

void RGB_NCP5623::green()
{
    led.color = led_control_s::COLOR_GREEN;
    ledControl(led);
}

void RGB_NCP5623::yellow()
{
    led.color = led_control_s::COLOR_YELLOW;
    ledControl(led);
}

void RGB_NCP5623::purple()
{
    led.color = led_control_s::COLOR_PURPLE;
    ledControl(led);
}

void RGB_NCP5623::cyan()
{
    led.color = led_control_s::COLOR_CYAN;
    ledControl(led);
}

void RGB_NCP5623::white()
{
    led.color = led_control_s::COLOR_WHITE;
    ledControl(led);
}

void RGB_NCP5623::ledOff()
{
    led.color = led_control_s::COLOR_OFF;
    ledControl(led);
}

/********************************
    RGB LED (IS31fl3195)
*********************************/

RGB_IS31fl3195::RGB_IS31fl3195(TwoWire &wire, uint8_t address) : _wire(wire),
                                                                 _address(address)
{
}

void RGB_IS31fl3195::ledControl(led_control_s led)
{
    switch (led.color)
    {
    case led_control_s::COLOR_RED:
        _r = IS31fl3195_LED_BRIGHT;
        _g = 0;
        _b = 0;
        break;
    case led_control_s::COLOR_GREEN:
        _r = 0;
        _g = IS31fl3195_LED_BRIGHT;
        _b = 0;
        break;
    case led_control_s::COLOR_BLUE:
        _r = 0;
        _g = 0;
        _b = IS31fl3195_LED_BRIGHT;
        break;
    case led_control_s::COLOR_YELLOW:
        _r = IS31fl3195_LED_BRIGHT;
        _g = IS31fl3195_LED_BRIGHT;
        _b = 0;
        break;
    case led_control_s::COLOR_PURPLE:
        _r = IS31fl3195_LED_BRIGHT;
        _g = 0;
        _b = IS31fl3195_LED_BRIGHT;
        break;
    case led_control_s::COLOR_CYAN:
        _r = 0;
        _g = IS31fl3195_LED_BRIGHT;
        _b = IS31fl3195_LED_BRIGHT;
        break;
    case led_control_s::COLOR_WHITE:
        _r = IS31fl3195_LED_BRIGHT;
        _g = IS31fl3195_LED_BRIGHT;
        _b = IS31fl3195_LED_BRIGHT;
        break;
    default:
        _r = 0;
        _g = 0;
        _b = 0;
        break;
    }
    sendData();
}

void RGB_IS31fl3195::sendData()
{
    const uint8_t leds[][2] = {
        {_red, static_cast<uint8_t>(_r * _brightness)},
        {_green, static_cast<uint8_t>(_g * _brightness)},
        {_blue, static_cast<uint8_t>(_b * _brightness)},
        {IS31fl3195_COLOR_UPDATE, IS31fl3195_COLOR_UPDATE_KEY},
    };

    Wire.beginTransmission(NCP5623B_ADDR);
    for (int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 2; j++)
        {
            Wire.write(leds[i][j]);
        }
    }
    Wire.endTransmission();
}

void RGB_IS31fl3195::setBrightness(uint8_t brightness)
{
    if (brightness <= 255)
    {
        _brightness = brightness / 255.f;
    }
}

void RGB_IS31fl3195::red()
{
    led.color = led_control_s::COLOR_RED;
    ledControl(led);
}

void RGB_IS31fl3195::blue()
{
    led.color = led_control_s::COLOR_BLUE;
    ledControl(led);
}

void RGB_IS31fl3195::green()
{
    led.color = led_control_s::COLOR_GREEN;
    ledControl(led);
}

void RGB_IS31fl3195::yellow()
{
    led.color = led_control_s::COLOR_YELLOW;
    ledControl(led);
}

void RGB_IS31fl3195::purple()
{
    led.color = led_control_s::COLOR_PURPLE;
    ledControl(led);
}

void RGB_IS31fl3195::cyan()
{
    led.color = led_control_s::COLOR_CYAN;
    ledControl(led);
}

void RGB_IS31fl3195::white()
{
    led.color = led_control_s::COLOR_WHITE;
    ledControl(led);
}

void RGB_IS31fl3195::ledOff()
{
    led.color = led_control_s::COLOR_OFF;
    ledControl(led);
}

/********************************
    Buzzer, RED Led and Switch
*********************************/
ExternalIO::ExternalIO()
{
    init();
}

void ExternalIO::init()
{
    pinMode(buzzer_pin, OUTPUT);
    pinMode(v3_3_pin, OUTPUT);
    pinMode(led_pin, OUTPUT);
    pinMode(switch_pin, INPUT);

    set3V3(State::ON);
    setBuzzer(State::OFF);
    setLED(State::OFF);
}

void ExternalIO::setBuzzer(State state)
{
    digitalWrite(buzzer_pin, (state == State::ON ? HIGH : LOW));
}

void ExternalIO::set3V3(State state)
{
    digitalWrite(v3_3_pin, (state == State::ON ? HIGH : LOW));
}

void ExternalIO::setLED(State state)
{
    if (state == State::ON)
    {
        set3V3(State::ON);
        digitalWrite(led_pin, LOW);
    }
    else
    {
        digitalWrite(led_pin, HIGH);
    }
}

int ExternalIO::getSwitch() const
{
    return !digitalRead(switch_pin);
}