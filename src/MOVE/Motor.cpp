#include "Motor.h"

Motor::Motor(uint8_t step_pin, uint8_t dir_pin)
{
    step = step_pin;
    dir = dir_pin;
}

void Motor::begin()
{
    pinMode(step, OUTPUT);
    pinMode(dir, OUTPUT);
    digitalWrite(step, LOW);
    digitalWrite(dir, LOW);
}

void Motor::Step(bool stip_dir)
{
    digitalWrite(dir, stip_dir);
    digitalWrite(step, HIGH);
    timer = step_time;
    idel = false;
}
void Motor::update(float dt)
{
    timer -= dt;
    if (timer < 0)
    {
        if (!phaseTwo)
        {
            digitalWrite(step, LOW);
            timer = step_time;
            phaseTwo = !phaseTwo;
        }
        else
        {
            phaseTwo = !phaseTwo;
            idel = true;
        }
    }
}
void Motor::SetSpeed(bool Speed)
{
    if(Speed)
        step_time = MAX_SPEED;
    else
        step_time = SLOW_SPEED;
}
bool Motor::Idel()
{
    return idel;
}
