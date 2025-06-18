#include "ADXL345.hpp"

void ADXL345::init()
{
    init(ADXL345Range::ADXL345_RANGE_16G, ADXL345DataRate::ADXL345_DATARATE_200HZ);
}

void ADXL345::init(ADXL345Range range, ADXL345DataRate rate, ADXL345PowerMode power_mode)
{
    if (!whoAmI())
    {
        Serial.println("ADXL345 not found at address 0x" + String(address_, HEX));
        return;
    }

    setPowerMode(power_mode);
    setRange(range);
    setDataRate(rate);
    setLFullResolution(false);

    writeRegister(Register::THRESH_ACT, 0x05); // Set activity threshold
    writeRegister(Register::ACT_INACT_CTL, 0xFF); // Enable activity detection on all axes
}

bool ADXL345::whoAmI() const
{
    return readRegister(Register::DEVID) == ADXL345_WHOAMI;
}

void ADXL345::readData()
{
    Wire.beginTransmission(address_);
    Wire.write(static_cast<uint8_t>(Register::DATAX0));
    Wire.endTransmission(false);
    
    Wire.requestFrom(address_, (uint8_t)6);
    for (int i = 0; i < 6; i++) {
        raw_data[i] = Wire.read();
    }
}

void ADXL345::enableReadingInterrupt() {
    // Enable DATA_READY interrupt
    writeRegister(Register::INT_ENABLE, ADXL345_INT_DATA_READY);
    
    // Map interrupts to INT1 pin
    writeRegister(Register::INT_MAP, 0x00); // All interrupts to INT1

    readRegister(Register::INT_SOURCE);
}

bool ADXL345::isIntDataReady() {
    uint8_t int_source = readRegister(Register::INT_SOURCE);
    return (int_source & ADXL345_INT_DATA_READY);
}

void ADXL345::setRange(ADXL345Range range)
{
    uint8_t data = readRegister(Register::DATA_FORMAT);
    data &= ~0x03;
    data |= static_cast<uint8_t>(range);
    writeRegister(Register::DATA_FORMAT, data);
    current_range_ = range;

    adjustSensitivity();
}

void ADXL345::setDataRate(ADXL345DataRate rate)
{
    uint8_t data = static_cast<uint8_t>(rate);
    if (power_mode_ == ADXL345PowerMode::ADXL345_LOW_POWER)
    {
        data |= 0x10;
    }
    writeRegister(Register::BW_RATE, data);
    current_rate_ = rate;
}

void ADXL345::setPowerMode(ADXL345PowerMode mode)
{
    power_mode_ = mode;

    if (mode == ADXL345PowerMode::ADXL345_LOW_POWER)
    {
        writeRegister(Register::POWER_CTL, 0x04);
    }
    else
    {
        writeRegister(Register::POWER_CTL, 0x00);
        writeRegister(Register::POWER_CTL, 0x10);
        writeRegister(Register::POWER_CTL, 0x08);
    }
}

void ADXL345::setLFullResolution(bool enable)
{
    uint8_t data = readRegister(Register::DATA_FORMAT);
    if (enable)
    {
        data |= 0x08;
    }
    else
    {
        data &= ~0x08;
    }
    writeRegister(Register::DATA_FORMAT, data);
}

void ADXL345::calibrate()
{
    ADXL345Data sum = {0, 0, 0};
    for (size_t i = 0; i < CALIBRATION_SAMPLE_COUNT; i++)
    {
        readData();
        sum.x += getX();
        sum.y += getY();
        sum.z += getZ();
        delay(20);
    }

    cal_offset_.x = sum.x / CALIBRATION_SAMPLE_COUNT;
    cal_offset_.y = sum.y / CALIBRATION_SAMPLE_COUNT;
    cal_offset_.z = (sum.z / CALIBRATION_SAMPLE_COUNT) - 1.0f; // Adjust for gravity

    calibrated_ = true;
}

void ADXL345::writeRegister(Register reg, uint8_t value) const
{
    Wire.beginTransmission(address_);
    Wire.write(static_cast<uint8_t>(reg));
    Wire.write(value);
    Wire.endTransmission();
}

uint8_t ADXL345::readRegister(Register reg) const
{
    Wire.beginTransmission(address_);
    Wire.write(static_cast<uint8_t>(reg));
    Wire.endTransmission(false);
    
    Wire.requestFrom(address_, (uint8_t)1);
    return Wire.read();
}

void ADXL345::adjustSensitivity()
{
    if (current_range_ == ADXL345Range::ADXL345_RANGE_2G)
    {
        SENSITIVITY_ = 0.0039f;
    }
    else if (current_range_ == ADXL345Range::ADXL345_RANGE_4G)
    {
        SENSITIVITY_ = 0.0078f;
    }
    else if (current_range_ == ADXL345Range::ADXL345_RANGE_8G)
    {
        SENSITIVITY_ = 0.0156f;
    }
    else
    {
        SENSITIVITY_ = 0.0312f;
    }
}

void ADXL345::readAccel(ADXL345Data &data)
{
    readData();

    if (calibrated_)
    {
        data.x = (getX() - cal_offset_.x) * GRAVITY;
        data.y = (getY() - cal_offset_.y) * GRAVITY;
        data.z = (getZ() - cal_offset_.z) * GRAVITY;
        return;
    }

    data.x = getX() * GRAVITY;
    data.y = getY() * GRAVITY;
    data.z = getZ() * GRAVITY;
}

int16_t ADXL345::getRawX() const
{
    return static_cast<int16_t>(raw_data[0] | (raw_data[1] << 8));
}

int16_t ADXL345::getRawY() const
{
    return static_cast<int16_t>(raw_data[2] | (raw_data[3] << 8));
}

int16_t ADXL345::getRawZ() const
{
    return static_cast<int16_t>(raw_data[4] | (raw_data[5] << 8));
}

float ADXL345::getX() const
{
    return static_cast<float>(getRawX()) * SENSITIVITY_;
}

float ADXL345::getY() const
{
    return static_cast<float>(getRawY()) * SENSITIVITY_;
}

float ADXL345::getZ() const
{
    return static_cast<float>(getRawZ()) * SENSITIVITY_;
}