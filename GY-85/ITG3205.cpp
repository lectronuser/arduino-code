#include "ITG3205.hpp"

ITG3205::ITG3205(uint8_t address) : 
    address_(address),
    current_dlpf_(ITG3205DLPFConfig::DLPF_42HZ),
    clock_source_(ITG3205ClockSource::INTERNAL_),
    sample_rate_divider_(ITG3205Frequency::FREQ_200HZ)
{
    Wire.begin();
}

ITG3205::~ITG3205()
{
}

void ITG3205::init(ITG3205DLPFConfig dlpf, ITG3205Frequency sample_rate_divider)
{
    if (!isConnected()) {
        Serial.println("ITG3205 not found at address 0x" + String(address_, HEX));
        return;
    }

    setClockSource(ITG3205ClockSource::INTERNAL_);
    setDLPF(dlpf);
    setSampleRateDivider(sample_rate_divider);
    setFullScaleRange();
}

bool ITG3205::isConnected() const
{
    return readRegister(Register::WHO_AM_I) == DEFAULT_ADDRESS;
}

void ITG3205::readData()
{
    Wire.beginTransmission(address_);
    Wire.write(static_cast<uint8_t>(Register::TEMP_OUT_H));
    Wire.endTransmission(false);
    
    Wire.requestFrom(address_, (uint8_t)8);
    for (int i = 0; i < 8; i++) {
        raw_data[i] = Wire.read();
    }
}

void ITG3205::setDLPF(ITG3205DLPFConfig dlpf)
{
    uint8_t val = readRegister(Register::DLPF_FS);
    val &= ~0x07; // Clear DLPF bits
    val |= static_cast<uint8_t>(dlpf);
    writeRegister(Register::DLPF_FS, val);
    current_dlpf_ = dlpf;
}

void ITG3205::setClockSource(ITG3205ClockSource source)
{
    uint8_t val = readRegister(Register::PWR_MGM);
    val &= ~0x07; // Clear clock source bits
    val |= static_cast<uint8_t>(source);
    writeRegister(Register::PWR_MGM, val);
    clock_source_ = source;
}

void ITG3205::setSampleRateDivider(ITG3205Frequency divider)
{
    if (current_dlpf_ == ITG3205DLPFConfig::DLPF_256HZ)
    {
        // in this case the INTERNAL_ sample rate is 8KHz so ITG3205Frequency members will not match
        // we just adjust sensor rate 200Hz.
        writeRegister(Register::SMPLRT_DIV, 39); // 8KHz / 200Hz = 40 - 1
        sample_rate_divider_ = ITG3205Frequency::FREQ_200HZ;
        return;
    }

    writeRegister(Register::SMPLRT_DIV, static_cast<uint8_t>(divider));
    sample_rate_divider_ = divider;
}

void ITG3205::setFullScaleRange()
{
    uint8_t val = readRegister(Register::DLPF_FS);
    val &= ~0x18; // Clear FS_SEL bits
    val |= FS_SEL_2000DPS;
    writeRegister(Register::DLPF_FS, val);
}

void ITG3205::calibrate()
{
    ITG3205Data sum = {0, 0, 0, 0};

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
    cal_offset_.z = sum.z / CALIBRATION_SAMPLE_COUNT;

    calibrated_ = true;
}

void ITG3205::readGyro(ITG3205Data &data)
{
    readData();

    if (calibrated_)
    {
        data.x = (getX() - cal_offset_.x) * (M_PI / 180.0F); // Convert to radians
        data.y = (getY() - cal_offset_.y) * (M_PI / 180.0F);
        data.z = (getZ() - cal_offset_.z) * (M_PI / 180.0F);
        return;
    }

    data.x = getX() * (M_PI / 180.0F); // Convert to radians
    data.y = getY() * (M_PI / 180.0F);
    data.z = getZ() * (M_PI / 180.0F);
}

int16_t ITG3205::getRawTemp() const
{
    return static_cast<int16_t>((raw_data[0] << 8) | raw_data[1]);
}

int16_t ITG3205::getRawX() const
{
    return static_cast<int16_t>((raw_data[2] << 8) | raw_data[3]);
}

int16_t ITG3205::getRawY() const
{
    return static_cast<int16_t>((raw_data[4] << 8) | raw_data[5]);
}

int16_t ITG3205::getRawZ() const
{
    return static_cast<int16_t>((raw_data[6] << 8) | raw_data[7]);
}

float ITG3205::getTemperature() const
{
    return static_cast<float>(getRawTemp()) / 340.0f + 35.0f; // Convert to degrees Celsius
}

float ITG3205::getX() const
{
    return static_cast<float>(getRawX()) / SENSITIVITY_;
}

float ITG3205::getY() const
{
    return static_cast<float>(getRawY()) / SENSITIVITY_;
}

float ITG3205::getZ() const
{
    return static_cast<float>(getRawZ()) / SENSITIVITY_;
}

void ITG3205::writeRegister(Register reg, uint8_t value) const
{
    Wire.beginTransmission(address_);
    Wire.write(static_cast<uint8_t>(reg));
    Wire.write(value);
    Wire.endTransmission();
}

uint8_t ITG3205::readRegister(Register reg) const
{
    Wire.beginTransmission(address_);
    Wire.write(static_cast<uint8_t>(reg));
    Wire.endTransmission(false);
    
    Wire.requestFrom(address_, (uint8_t)1);
    return Wire.read();
}