#include "external_io.hpp"

ExternalIO::ExternalIO()
{
    init();
}

void ExternalIO::init()
{
    pinMode(buzzer_pin, OUTPUT);
    pinMode(v3_3_pin, OUTPUT);
    pinMode(led_pin, OUTPUT);
    pinMode(switch_pin, INPUT);

    set3V3(State::ON);
    setBuzzer(State::OFF);
    setLED(State::OFF);
}

void ExternalIO::setBuzzer(State state)
{
    digitalWrite(buzzer_pin, (state == State::ON ? HIGH : LOW));
}

void ExternalIO::set3V3(State state)
{
    digitalWrite(v3_3_pin, (state == State::ON ? HIGH : LOW));
}

void ExternalIO::setLED(State state)
{
    if (state == State::ON)
    {
        set3V3(State::ON);
        digitalWrite(led_pin, LOW);
    }
    else
    {
        digitalWrite(led_pin, HIGH);
    }
}

int ExternalIO::getSwitch() const
{
    return !digitalRead(switch_pin);
}