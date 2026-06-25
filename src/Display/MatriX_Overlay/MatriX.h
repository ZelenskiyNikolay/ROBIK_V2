#pragma once
#include <Arduino.h>
#include "Display/DisplayOled.h"
#include "Symbol.h"
#include "Sprites.h"
#include "Core/timer.h"
#include "Flow.h"

#define MAX_FLOWS 30
class Matrix
{
private:
    int _collFlows = 15;
    Flow F[MAX_FLOWS];
    float timer = 0;
    float timer2 = 300;
    DisplayOled *disp;

public:
    void Init(DisplayOled &display)
    {
        disp = &display;
        for (int i = 0; i < MAX_FLOWS; i++)
            F[i].Init(random(10, 50), random(1, 120), random(1, 9), display);

        disp->clear();
        disp->NeedUpdate = true;
    }
    void Reset()
    {
        _collFlows = 15;
        timer = 0;
        timer2 = 300;
        for (int i = 0; i < MAX_FLOWS; i++)
            F[i].Init(random(10, 50), random(1, 120), random(1, 9), *disp);
    }
    void Update(float dt)
    {
        timer -= dt;
        if (timer > 0)
            return;
        disp->clear();
        for (int i = 0; i < _collFlows; i++)
        {
            F[i].PrintFlow();
            if (F[i].Delay() < 0)
                F[i].MovSymbols();
            else
                F[i].DelayInc();

            if (F[i].ClipFlow(64))
            {
                F[i].FlowRefresh(random(10, 50), random(1, 120));
            }
        }
        if (timer2 > 0)
        {
            timer2 -= dt;
            if (timer2 > 200)
                disp->drawText("THE MATRIX", 0, 30, 2);
            else
                disp->drawText("Wake up,  Neo...", 0, 30, 2);
        }
        else
        {
            _collFlows = MAX_FLOWS;
        }
        disp->NeedUpdate = true;
        timer = 100;
    }
};