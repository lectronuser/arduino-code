#ifndef __IST8310_REGISTER_HPP__
#define __IST8310_REGISTER_HPP__

#include <Wire.h>
#include <Arduino.h>

static constexpr uint8_t Bit0 = (1 << 0);
static constexpr uint8_t Bit1 = (1 << 1);
static constexpr uint8_t Bit2 = (1 << 2);
static constexpr uint8_t Bit3 = (1 << 3);
static constexpr uint8_t Bit4 = (1 << 4);
static constexpr uint8_t Bit5 = (1 << 5);
static constexpr uint8_t Bit6 = (1 << 6);
static constexpr uint8_t Bit7 = (1 << 7);

namespace iSentek_IST8310
{
    static constexpr uint32_t I2C_SPEED = 400 * 1000; // 400 kHz I2C serial interface
    static constexpr uint8_t I2C_ADDRESS_DEFAULT = 0x0E;

    static constexpr uint8_t Device_ID = 0x10;

    enum Register : uint8_t
    {
        WAI = 0x00, // Who Am I Register

        STAT1 = 0x02, // Status Register 1
        DATAXL = 0x03,
        DATAXH = 0x04,
        DATAYL = 0x05,
        DATAYH = 0x06,
        DATAZL = 0x07,
        DATAZH = 0x08,
        STAT2 = 0x09, // Status Register 2
        CNTL1 = 0x0A, // Control Setting Register 1
        CNTL2 = 0x0B, // Control Setting Register 2
        STR = 0x0C,   // Self-Test Register
        CNTL3 = 0x0D, // Control Setting Register 3

        TEMPL = 0x1C, // Temperature Sensor Output Register (Low Byte)
        TEMPH = 0x1D, // Temperature Sensor Output Register (High Byte)

        TCCNTL = 0x40,  // Temperature Compensation Control register
        AVGCNTL = 0x41, // Average Control Register
        PDCNTL = 0x42,  // Pulse Duration Control Register

        XX_CROSS_L = 0x9C, // cross axis xx low byte
        XX_CROSS_H = 0x9D, // cross axis xx high byte
        XY_CROSS_L = 0x9E, // cross axis xy low byte
        XY_CROSS_H = 0x9F, // cross axis xy high byte
        XZ_CROSS_L = 0xA0, // cross axis xz low byte
        XZ_CROSS_H = 0xA1, // cross axis xz high byte
        YX_CROSS_L = 0xA2, // cross axis yx low byte
        YX_CROSS_H = 0xA3, // cross axis yx high byte
        YY_CROSS_L = 0xA4, // cross axis yy low byte
        YY_CROSS_H = 0xA5, // cross axis yy high byte
        YZ_CROSS_L = 0xA6, // cross axis yz low byte
        YZ_CROSS_H = 0xA7, // cross axis yz high byte
        ZX_CROSS_L = 0xA8, // cross axis zx low byte
        ZX_CROSS_H = 0xA9, // cross axis zx high byte
        ZY_CROSS_L = 0xAA, // cross axis zy low byte
        ZY_CROSS_H = 0xAB, // cross axis zy high byte
        ZZ_CROSS_L = 0xAC, // cross axis zz low byte
        ZZ_CROSS_H = 0xAD, // cross axis zz high byte
    };

    // STAT1
    enum STAT1_BIT : uint8_t
    {
        DOR = Bit1,  // Data overrun bit
        DRDY = Bit0, // Data ready
    };

    // CNTL1
    enum CNTL1_BIT : uint8_t
    {
        // 3:0 Mode: Operating mode setting
        MODE_SINGLE_MEASUREMENT = Bit0,
    };

    // CNTL2
    enum CNTL2_BIT : uint8_t
    {
        SRST = Bit0, // Soft reset, perform the same routine as POR
    };

    // STR
    enum STR_BIT : uint8_t
    {
        SELF_TEST = Bit6,
    };

    // CNTL3
    enum CNTL3_BIT : uint8_t
    {
        Z_16BIT = Bit6, // Sensor output resolution adjustment for Z axis: 16-bit (Sensitivity: 1320 LSB/Gauss)
        Y_16BIT = Bit5, // Sensor output resolution adjustment for Y axis: 16-bit (Sensitivity: 1320 LSB/Gauss)
        X_16BIT = Bit4, // Sensor output resolution adjustment for X axis: 16-bit (Sensitivity: 1320 LSB/Gauss)
    };

    // AVGCNTL
    enum AVGCNTL_BIT : uint8_t
    {
        // 5:3 Average times for y sensor data. Times of average will be done before switch to next channel
        Y_16TIMES_SET = Bit5, // 3’b100 average by 16 times (ODRmax=166Hz)
        Y_16TIMES_CLEAR = Bit4 | Bit3,

        // 2:0 Average times for x & z sensor data. Times of average will be done before switch to next channel
        XZ_16TIMES_SET = Bit2, // average by 16 times (ODRmax=166Hz)
        XZ_16TIMES_CLEAR = Bit1 | Bit0,
    };

    // PDCNTL
    enum PDCNTL_BIT : uint8_t
    {
        // 7:6 Pulse duration
        PULSE_NORMAL = Bit7 | Bit6, // Normal (please use this setting)
    };

}
#endif