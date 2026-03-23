#include "Motor.h"

Motor::Motor(uint8_t p1, uint8_t p2, uint8_t p3, uint8_t p4)
{
    pins[0] = p1;
    pins[1] = p2;
    pins[2] = p3;
    pins[3] = p4;
}

void Motor::begin()
{
    for (int i = 0; i < 4; i++)
    {
        pinMode(pins[i], OUTPUT);
        digitalWrite(pins[i], LOW);
    }
}

void Motor::step(int dir)
{
    if (!idel)
        stepIndex += dir;

    if (stepIndex > 7)
        stepIndex = 0;
    if (stepIndex < 0)
        stepIndex = 7;

    setStep(stepIndex);
}
void Motor::setStep(uint8_t s)
{
    idel = false;
    for (int i = 0; i < 4; i++)
        digitalWrite(pins[i], steps[s][i]);
}
void Motor::Idel()
{
    if (!idel)
    {
        idel = true;
        for (int i = 0; i < 4; i++)
            digitalWrite(pins[i], LOW);
    }
}
