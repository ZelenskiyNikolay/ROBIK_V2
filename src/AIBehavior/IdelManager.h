#pragma once
#include <Arduino.h>
#include "MOVE/MovementModule.h"
#include "Sound/SoundManager.h"

#define TIMER_MOV 15000
#define TIMER_SPEAK 20000
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
    bool idelMov = false;
    float timer1;
    float timer2;
};