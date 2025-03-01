#ifndef __STAND_TYPES_HPP__
#define __STAND_TYPES_HPP__

#include <Wire.h>
#include <Arduino.h>

#define I2C_SPEED (400 * 1000) // 400 kHz
#define SHUTDOWN_CTRL 0x01
#define SHUTDOWN_CTRL_SSD_SHUTDOWN 0x00
#define SHUTDOWN_CTRL_SSD_NORMAL 0x01

#define SHUTDOWN_CTRL_SLE_SHIFT 1
#define SHUTDOWN_CTRL_SLE_MASK 0x03
#define SHUTDOWN_CTRL_SLE_DISABLE (0 << SHUTDOWN_CTRL_SLE_SHIFT)
#define SHUTDOWN_CTRL_SLE_SLEEP1 (1 << SHUTDOWN_CTRL_SLE_SHIFT)
#define SHUTDOWN_CTRL_SLE_SLEEP2 (2 << SHUTDOWN_CTRL_SLE_SHIFT)

#define SHUTDOWN_CTRL_CPPM_SHIFT 3
#define SHUTDOWN_CTRL_CPPM_1X (0 << SHUTDOWN_CTRL_CPPM_SHIFT)
#define SHUTDOWN_CTRL_CPPM_1P5X (1 << SHUTDOWN_CTRL_CPPM_SHIFT)

#define   SHUTDOWN_CTRL_CPPM_SHIFT        3
#define   SHUTDOWN_CTRL_CPPM_1X           (0 << SHUTDOWN_CTRL_CPPM_SHIFT)
#define   SHUTDOWN_CTRL_CPPM_1P5X         (1 << SHUTDOWN_CTRL_CPPM_SHIFT)
#define   SHUTDOWN_CTRL_EN_SHIFT          4
#define   SHUTDOWN_CTRL_EN_MASK           0xf
#define   SHUTDOWN_CTRL_EN1               (1 << SHUTDOWN_CTRL_EN_SHIFT)
#define   SHUTDOWN_CTRL_EN2               (2 << SHUTDOWN_CTRL_EN_SHIFT)
#define   SHUTDOWN_CTRL_EN3               (4 << SHUTDOWN_CTRL_EN_SHIFT)
#define   SHUTDOWN_CTRL_EN4               (8 << SHUTDOWN_CTRL_EN_SHIFT)

#define MODE_CONFIG 0x02
#define MODE_CONFIG_LM_SHIFT 0
#define MODE_CONFIG_LM_MASK 0x3
#define MODE_CONFIG_LM_SINGLE (0 << MODE_CONFIG_LM_SHIFT)
#define MODE_CONFIG_LM_RGBW (1 << MODE_CONFIG_LM_SHIFT)
#define MODE_CONFIG_LM_SRGBY (2 << MODE_CONFIG_LM_SHIFT)

#define MODE_CONFIG_OUT_MODE_SHIFT 4
#define MODE_CONFIG_OUT_MODE_MASK 0xf
#define MODE_CONFIG_OUT_MODE_CURRENT 0
#define MODE_CONFIG_OUT_MODE_PATTERN 1
#define MODE_CONFIG_OUT_MODE(n, m) ((m) << ((n - 1) + MODE_CONFIG_OUT_MODE_SHIFT))
#define MODE_CONFIG_OUT1_MODE(m) MODE_CONFIG_OUT_MODE(1, (m))
#define MODE_CONFIG_OUT2_MODE(m) MODE_CONFIG_OUT_MODE(2, (m))
#define MODE_CONFIG_OUT3_MODE(m) MODE_CONFIG_OUT_MODE(3, (m))
#define MODE_CONFIG_OUT4_MODE(m) MODE_CONFIG_OUT_MODE(4, (m))

#define CHARGE_PUMP1                      0x3
#define     CHARGE_PUMP1_CPM_SHIFT        0
#define     CHARGE_PUMP1_CPM_MASK         0x3
#define     CHARGE_PUMP1_CPM_AUTO         (0 << CHARGE_PUMP1_CPM_SHIFT)
#define     CHARGE_PUMP1_CPM_1X           (1 << CHARGE_PUMP1_CPM_SHIFT)
#define     CHARGE_PUMP1_CPM_1P5X         (2 << CHARGE_PUMP1_CPM_SHIFT)
#define     CHARGE_PUMP1_DEFAULT          (8 << 2)

#define CHARGE_PUMP2                      0x4
#define     CHARGE_PUMP2_CPDE_SHIFT       0
#define     CHARGE_PUMP2_CPDE_MASK        0xf
#define     CHARGE_PUMP2_CPDE_ENABLE      0
#define     CHARGE_PUMP2_CPDE_DISABLE     1
#define     CHARGE_PUMP2_CPDE(n,m)        ((m) << ((n-1) + CHARGE_PUMP2_CPDE_SHIFT))

#define     CHARGE_PUMP2_HRT_SHIFT        4
#define     CHARGE_PUMP2_MASK             0x3
#define     CHARGE_PUMP2_HRT(m)           ((m) << + CHARGE_PUMP2_HRT_SHIFT)
#define     CHARGE_PUMP2_HRT_50MV         CHARGE_PUMP2_HRT(0)
#define     CHARGE_PUMP2_HRT_100MV        CHARGE_PUMP2_HRT(1)
#define     CHARGE_PUMP2_HRT_125MV        CHARGE_PUMP2_HRT(2)
#define     CHARGE_PUMP2_HRT_150MV        CHARGE_PUMP2_HRT(3)
#define     CHARGE_PUMP2_HRT_175MV        CHARGE_PUMP2_HRT(4)
#define     CHARGE_PUMP2_HRT_200MV        CHARGE_PUMP2_HRT(5)
#define     CHARGE_PUMP2_HRT_250MV        CHARGE_PUMP2_HRT(6)
#define     CHARGE_PUMP2_HRT_300MV        CHARGE_PUMP2_HRT(7)

#define RUN_MODE (SHUTDOWN_CTRL_SSD_NORMAL |  \
                  SHUTDOWN_CTRL_SLE_DISABLE | \
                  SHUTDOWN_CTRL_CPPM_1P5X)

#define L1_L3_EN (SHUTDOWN_CTRL_EN1 | SHUTDOWN_CTRL_EN2 | SHUTDOWN_CTRL_EN3)

#define CURRENT_BAND 0x5
#define CURRENT_BAND_CB_SHIFT 0
#define CURRENT_BAND_CB_MASK_ 0x3
#define CURRENT_BAND_CB2_SHIFT 2

#define CURRENT_BAND_CB_P25 0
#define CURRENT_BAND_CB_P5 1
#define CURRENT_BAND_CB_P75 2
#define CURRENT_BAND_CB_1P0 3
#define CURRENT_BAND_CB(n, m) (m) << ((n - 1) * CURRENT_BAND_CB2_SHIFT)

static constexpr uint8_t Bit0 = (1 << 0);
static constexpr uint8_t Bit1 = (1 << 1);
static constexpr uint8_t Bit2 = (1 << 2);
static constexpr uint8_t Bit3 = (1 << 3);
static constexpr uint8_t Bit4 = (1 << 4);
static constexpr uint8_t Bit5 = (1 << 5);
static constexpr uint8_t Bit6 = (1 << 6);
static constexpr uint8_t Bit7 = (1 << 7);

enum class State
{
    OFF = 0,
    ON = 1
};

enum Result_e {
    ERROR = -1,
    OK = 0
};

namespace IST8310
{
    static constexpr uint8_t IST8310_ADDR = 0x0E;
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

    enum STAT1_BIT : uint8_t
    {
        DOR = Bit1,  // Data overrun bit
        DRDY = Bit0, // Data ready
    };

    enum CNTL1_BIT : uint8_t
    {
        // 3:0 Mode: Operating mode setting
        MODE_SINGLE_MEASUREMENT = Bit0,
    };

    enum CNTL2_BIT : uint8_t
    {
        SRST = Bit0, // Soft reset, perform the same routine as POR
    };

    enum STR_BIT : uint8_t
    {
        SELF_TEST = Bit6,
    };

    enum CNTL3_BIT : uint8_t
    {
        Z_16BIT = Bit6, // Sensor output resolution adjustment for Z axis: 16-bit (Sensitivity: 1320 LSB/Gauss)
        Y_16BIT = Bit5, // Sensor output resolution adjustment for Y axis: 16-bit (Sensitivity: 1320 LSB/Gauss)
        X_16BIT = Bit4, // Sensor output resolution adjustment for X axis: 16-bit (Sensitivity: 1320 LSB/Gauss)
    };

    enum AVGCNTL_BIT : uint8_t
    {
        // 5:3 Average times for y sensor data. Times of average will be done before switch to next channel
        Y_16TIMES_SET = Bit5, // 3’b100 average by 16 times (ODRmax=166Hz)
        Y_16TIMES_CLEAR = Bit4 | Bit3,

        // 2:0 Average times for x & z sensor data. Times of average will be done before switch to next channel
        XZ_16TIMES_SET = Bit2, // average by 16 times (ODRmax=166Hz)
        XZ_16TIMES_CLEAR = Bit1 | Bit0,
    };

    enum PDCNTL_BIT : uint8_t
    {
        // 7:6 Pulse duration
        PULSE_NORMAL = Bit7 | Bit6, // Normal (please use this setting)
    };
}

namespace RGB_LED_NCP5623B
{
    static constexpr uint8_t NCP5623B_ADDR = 0x38;
    static constexpr uint8_t NCP5623_LED_CURRENT = 0x20;
    static constexpr uint8_t NCP5623_LED_PWM0 = 0x40;
    static constexpr uint8_t NCP5623_LED_PWM1 = 0x60;
    static constexpr uint8_t NCP5623_LED_PWM2 = 0x80;
    static constexpr uint8_t NCP5623_LED_BRIGHT = 0x1F;
    static constexpr uint8_t NCP5623_LED_OFF = 0x00;
}

namespace RGB_LED_IS31fl3195
{                      
    static constexpr uint8_t IS31fl3195_ADDR = 0x54;
    static constexpr uint8_t PRODUCT_ID = 0x00;
    static constexpr uint8_t IS31fl3195_COLOR_UPDATE = 0x50;
    static constexpr uint8_t IS31fl3195_COLOR_UPDATE_KEY = 0xC5;
    static constexpr uint8_t OUT_CURRENT1 = 0x10;
    static constexpr uint8_t OUT_CURRENT2 = 0x21;
    static constexpr uint8_t OUT_CURRENT3 = 0x32;
    static constexpr uint8_t OUT_CURRENT4 = 0x40;
    static constexpr uint8_t IS31fl3195_LED_BRIGHT = 0xFF;
}

struct led_control_s
{
    uint64_t timestamp;
    uint8_t led_mask;
    uint8_t color;
    uint8_t mode;
    uint8_t num_blinks;
    uint8_t priority;
    uint8_t _padding0[3];
    static constexpr uint8_t COLOR_OFF = 0;
    static constexpr uint8_t COLOR_RED = 1;
    static constexpr uint8_t COLOR_GREEN = 2;
    static constexpr uint8_t COLOR_BLUE = 3;
    static constexpr uint8_t COLOR_YELLOW = 4;
    static constexpr uint8_t COLOR_PURPLE = 5;
    static constexpr uint8_t COLOR_CYAN = 6;
    static constexpr uint8_t COLOR_WHITE = 7;
    static constexpr uint8_t MODE_OFF = 0;
    static constexpr uint8_t MODE_ON = 1;
    static constexpr uint8_t MODE_DISABLED = 2;
    static constexpr uint8_t MODE_BLINK_SLOW = 3;
    static constexpr uint8_t MODE_BLINK_NORMAL = 4;
    static constexpr uint8_t MODE_BLINK_FAST = 5;
    static constexpr uint8_t MODE_BREATHE = 6;
    static constexpr uint8_t MODE_FLASH = 7;
    static constexpr uint8_t MAX_PRIORITY = 2;
    static constexpr uint8_t ORB_QUEUE_LENGTH = 8;
};

#endif