#pragma once
#include <Arduino.h>

#define MAX_SPEED 0.1f
#define SLOW_SPEED 1.0f

class Motor
{
public:
    Motor(uint8_t step_pin, uint8_t dir_pin);

    void begin();
    void Step(bool stip_dir);
    bool Idel();
    void update(float dt);
    void SetSpeed(bool Speed);
private:
    uint8_t step;
    uint8_t dir;
    float step_time = MAX_SPEED;
    bool idel = true;

    bool phaseTwo = false;
    float timer = step_time;   
};
