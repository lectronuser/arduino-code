#include "m8n_io_control.hpp"

M8N::M8N()
{
    init();
}

void M8N::init()
{
    pinMode(BZZR_PIN, OUTPUT);
    pinMode(V3_3_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);
    pinMode(SWITCH_PIN, INPUT);
    
    set3V3(State::ON);
    setBuzzer(State::OFF);
    setLED(State::OFF);
}

void M8N::setBuzzer(State state)
{
    digitalWrite(BZZR_PIN, (state == State::ON ? HIGH : LOW));
}

void M8N::set3V3(State state)
{
    digitalWrite(V3_3_PIN, (state == State::ON ? HIGH : LOW));
}

void M8N::setLED(State state)
{
    if(state == State::ON)
    {
        set3V3(State::ON);
        digitalWrite(LED_PIN, LOW);
    }
    else
    {
        digitalWrite(LED_PIN, HIGH);
    }
}

int M8N::getSwitch() const
{
    return !digitalRead(SWITCH_PIN);
}