#ifndef __RGB_NCP5623_TYPES_HPP__
#define __RGB_NCP5623_TYPES_HPP__

#include <Wire.h>
#include <Arduino.h>

#define NCP5623B_I2C_ADDR 0x38

#define NCP5623_LED_CURRENT 0x20  /**< Current register */
#define NCP5623_LED_PWM0    0x40  /**< pwm0 register */
#define NCP5623_LED_PWM1    0x60  /**< pwm1 register */
#define NCP5623_LED_PWM2    0x80  /**< pwm2 register */

#define NCP5623_LED_BRIGHT  0x1f  /**< full brightness */
#define NCP5623_LED_OFF     0x00  /**< off */

struct led_control_s {

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
	static constexpr uint8_t COLOR_AMBER = 6;
	static constexpr uint8_t COLOR_CYAN = 7;
	static constexpr uint8_t COLOR_WHITE = 8;
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