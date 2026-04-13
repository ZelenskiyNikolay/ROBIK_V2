#pragma once
#include <Arduino.h>
#include "MOVE/MovementModule.h"

#define TIMER_MOV 30000
#define TIMER_SPEAK 100000
class IdelManager
{
public:
    static IdelManager &getInstance()
    {
        static IdelManager instance;
        return instance;
    }

    void begin();
    void Update(float dt);


private:
    IdelManager();
    bool IsIdel();
    float RND_DIST();
    float timer1;
    float timer2;
};