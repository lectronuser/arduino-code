#ifndef ITG3205_HPP
#define ITG3205_HPP

#include <Wire.h>
#include <Arduino.h>
#include <stdint.h>
#include <math.h>

struct ITG3205Data
{
    float x;
    float y;
    float z;
    float temp;
};

enum class ITG3205DLPFConfig : uint8_t
{
    DLPF_256HZ = 0,
    DLPF_188HZ = 1,
    DLPF_98HZ = 2,
    DLPF_42HZ = 3,
    DLPF_20HZ = 4,
    DLPF_10HZ = 5,
    DLPF_5HZ = 6
};

enum class ITG3205ClockSource : uint8_t
{
    INTERNAL_ = 0x00,
    PLL_X_GYRO = 0x01,
    PLL_Y_GYRO = 0x02,
    PLL_Z_GYRO = 0x03,
    PLL_EXTERNAL_32KHZ = 0x04,
    PLL_EXTERNAL_19MHZ = 0x05
};

enum class ITG3205Frequency
{
    FREQ_100HZ = 9,
    FREQ_200HZ = 4,
    FREQ_250HZ = 3,
    FREQ_400HZ = 2,
    FREQ_500HZ = 1,
    FREQ_1000HZ = 0
};

class ITG3205
{
public:
    ITG3205(uint8_t address = DEFAULT_ADDRESS);
    ~ITG3205();

    void init(ITG3205DLPFConfig dlpf = ITG3205DLPFConfig::DLPF_42HZ,
              ITG3205Frequency sample_rate_divider = ITG3205Frequency::FREQ_200HZ);

    bool isConnected() const;
    
    void readData();

    void setDLPF(ITG3205DLPFConfig dlpf);
    
    void setClockSource(ITG3205ClockSource source);
    
    void setSampleRateDivider(ITG3205Frequency divider);
    
    void setFullScaleRange();

    ITG3205DLPFConfig getDLPFConfig() const { return current_dlpf_; }

    ITG3205Frequency getSampleRateDivider() const { return sample_rate_divider_; }

    ITG3205ClockSource getClockSource() const { return clock_source_; }

    void calibrate();

    void readGyro(ITG3205Data &data);

    int16_t getRawTemp() const;

    int16_t getRawX() const;

    int16_t getRawY() const;

    int16_t getRawZ() const;

    float getTemperature() const;

    float getX() const;

    float getY() const;

    float getZ() const;

private:
    enum class Register : uint8_t
    {
        WHO_AM_I = 0x00,
        SMPLRT_DIV = 0x15,
        DLPF_FS = 0x16,
        INT_CFG = 0x17,
        INT_STATUS = 0x1A,
        TEMP_OUT_H = 0x1B,
        TEMP_OUT_L = 0x1C,
        GYRO_XOUT_H = 0x1D,
        GYRO_XOUT_L = 0x1E,
        GYRO_YOUT_H = 0x1F,
        GYRO_YOUT_L = 0x20,
        GYRO_ZOUT_H = 0x21,
        GYRO_ZOUT_L = 0x22,
        PWR_MGM = 0x3E
    };

    uint8_t address_;
    uint8_t raw_data[8];

    constexpr static float SENSITIVITY_ = 14.375f;

    ITG3205Data cal_offset_;
    constexpr static int CALIBRATION_SAMPLE_COUNT = 100;
    bool calibrated_ = false;

    static constexpr uint8_t DEFAULT_ADDRESS = 0x68;
    static constexpr uint8_t FS_SEL_2000DPS = 0x03 << 3; // Full scale range (2000 degrees/s)

    ITG3205DLPFConfig current_dlpf_;
    ITG3205ClockSource clock_source_;
    ITG3205Frequency sample_rate_divider_;

    void writeRegister(Register reg, uint8_t value) const;
    uint8_t readRegister(Register reg) const;
};

#endif