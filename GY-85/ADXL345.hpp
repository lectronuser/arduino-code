#ifndef ADXL345_HPP
#define ADXL345_HPP

#include <Wire.h>
#include <Arduino.h>
#include <stdint.h>

struct ADXL345Data
{
    float x;
    float y;
    float z;
};

enum class ADXL345Range : uint8_t
{
    ADXL345_RANGE_2G = 0X00,
    ADXL345_RANGE_4G = 0X01,
    ADXL345_RANGE_8G = 0X02,
    ADXL345_RANGE_16G = 0X03
};

enum class ADXL345DataRate : uint8_t
{
    ADXL345_DATARATE_3200HZ = 0X0F,
    ADXL345_DATARATE_1600HZ = 0X0E,
    ADXL345_DATARATE_800HZ = 0X0D,
    ADXL345_DATARATE_400HZ = 0X0C,
    ADXL345_DATARATE_200HZ = 0X0B,
    ADXL345_DATARATE_100HZ = 0X0A,
    ADXL345_DATARATE_50HZ = 0X09,
    ADXL345_DATARATE_25HZ = 0X08,
    ADXL345_DATARATE_12HZ5 = 0X07,
    ADXL345_DATARATE_6HZ25 = 0X06,
};

enum class ADXL345PowerMode
{
    ADXL345_NORMAL_POWER,
    ADXL345_LOW_POWER
};

class ADXL345
{
private:
    enum class Register : uint8_t
    {
        DEVID = 0x00,
        THRESH_TAP = 0x1D,
        OFSX = 0x1E,
        OFSY = 0x1F,
        OFSZ = 0x20,
        DUR = 0x21,
        LATENT = 0x22,
        WINDOW = 0x23,
        THRESH_ACT = 0x24,
        THRESH_INACT = 0x25,
        TIME_INACT = 0x26,
        ACT_INACT_CTL = 0x27,
        THRESH_FF = 0x28,
        TIME_FF = 0x29,
        TAP_AXES = 0x2A,
        ACT_TAP_STATUS = 0x2B,
        BW_RATE = 0x2C,
        POWER_CTL = 0x2D,
        INT_ENABLE = 0x2E,
        INT_MAP = 0x2F,
        INT_SOURCE = 0x30,
        DATA_FORMAT = 0x31,
        DATAX0 = 0x32,
        DATAX1 = 0x33,
        DATAY0 = 0x34,
        DATAY1 = 0x35,
        DATAZ0 = 0x36,
        DATAZ1 = 0x37,
        FIFO_CTL = 0x38,
        FIFO_STATUS = 0x39
    };

    uint8_t address_;

    ADXL345Data cal_offset_ = {0, 0, 0};
    bool calibrated_ = false;

    ADXL345PowerMode power_mode_ = ADXL345PowerMode::ADXL345_NORMAL_POWER;
    ADXL345Range current_range_ = ADXL345Range::ADXL345_RANGE_16G;
    ADXL345DataRate current_rate_ = ADXL345DataRate::ADXL345_DATARATE_200HZ;

    constexpr static uint8_t ADXL345_WHOAMI = 0xE5;
    constexpr static int CALIBRATION_SAMPLE_COUNT = 100;
    constexpr static uint8_t ADXL345_INT_DATA_READY = 0x80;
    constexpr static uint8_t ADXL345_INT_ACTIVITY = 0x10;
    constexpr static uint8_t ADXL345_INT_SOURCE = 0x30;
    constexpr static uint8_t ADXL345_INT_ENABLE = 0x2E;
    constexpr static uint8_t ADXL345_INT_MAP = 0x2F;
    float SENSITIVITY_ = 0.0312f; // g/LSB
    constexpr static float GRAVITY = 9.80665f; // m/s^2
    uint8_t raw_data[6] = {0, 0, 0, 0, 0, 0};

public:
    ADXL345(uint8_t address = 0x53) : address_(address) 
    {
        Wire.begin();
    }

    ~ADXL345() = default;
    
    void init();
    void init(ADXL345Range range, ADXL345DataRate rate, ADXL345PowerMode power_mode = ADXL345PowerMode::ADXL345_NORMAL_POWER);
    bool whoAmI() const;
    bool isConnected() const { return whoAmI(); }
    void readData();
    void enableReadingInterrupt();
    bool isIntDataReady();
    void setRange(ADXL345Range range);
    void setDataRate(ADXL345DataRate rate);
    void setPowerMode(ADXL345PowerMode mode);
    void setLFullResolution(bool enable);
    ADXL345Range getRange() const { return current_range_; }
    ADXL345DataRate getDataRate() const { return current_rate_; }
    ADXL345PowerMode getPowerMode() const { return power_mode_; }
    void calibrate();
    void adjustSensitivity();
    void readAccel(ADXL345Data &data);
    int16_t getRawX() const;
    int16_t getRawY() const;
    int16_t getRawZ() const;
    float getX() const;
    float getY() const;
    float getZ() const;

private:
    void writeRegister(Register reg, uint8_t value) const;
    uint8_t readRegister(Register reg) const;
};

#endif