#ifndef EXTERNAL_IO_HPP 
#define EXTERNAL_IO_HPP

#include "stand_type.hpp"

class ExternalIO
{
public:
    ExternalIO();
    void setBuzzer(State state);
    void set3V3(State state);
    void setLED(State state);
    int getSwitch() const;

private:
    void init();
    const uint8_t buzzer_pin{7};
    const uint8_t v3_3_pin{6};
    const uint8_t led_pin{5};
    const uint8_t switch_pin{4};
};

#endif