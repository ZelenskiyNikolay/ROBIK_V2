#pragma once
#include <Arduino.h>

enum Target
{
    NO_TARGET,
    EXPLORE,
    JOKE,
    BASE,
    MOV_TO_POINT,
};
struct Needs
{
    float energy = 100.0f; // Зависит от BatteryModule
    float boredom = 0;     // скука
    float curiosity = 0;   // любопытство
    float Safety = 100.0f; //(Безопасность / Тревожность)
    Target Nov_Target = NO_TARGET; //Текущая цель
};

class Brain
{
public:
    static Brain &getInstance()
    {
        static Brain instance;
        return instance;
    }

    void update(float dt);

private:
    Brain();
};