#ifndef __M8N_IO_CONTROL_HPP__ 
#define __M8N_IO_CONTROL_HPP__

#include <Arduino.h>

#define BZZR_PIN 7
#define V3_3_PIN 6
#define LED_PIN 5
#define SWITCH_PIN 4

enum class State{
    OFF = 0,
    ON = 1
};

class M8N
{
public:
    M8N();
    void setBuzzer(State state);
    void set3V3(State state);
    void setLED(State state);
    int getSwitch() const;
private:
    void init();
};

#endif