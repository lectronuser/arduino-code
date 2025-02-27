#include <Wire.h>

#include "ist8310.hpp"
#include "m8n_io_control.hpp"
#include "rgbled_ncp5623.hpp"

#define SER1_BAUDRATE 115200
#define SER2_BAUDRATE 38400

char my_string[128];

IST8310 ist8310;
//M8N m8n;
RGBLED_NCP5623 ncp5623;

bool is_ready_compass = false;

void setup()
{
    Wire.begin();
    Serial.begin(SER1_BAUDRATE);
    is_ready_compass = ist8310.begin();

    sprintf(my_string, "Compass (IST8310) Initialization %s", (is_ready_compass ? "Successful" : "Failed"));
    Serial.println(my_string);
    led_control_s led;
    led.color = led_control_s::COLOR_WHITE;
    ncp5623.ledControl(led);
    //m8n.setLED(State::ON);
}

void loop()
{
}