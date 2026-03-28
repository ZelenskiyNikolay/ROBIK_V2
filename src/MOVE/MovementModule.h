#pragma once
#include <Arduino.h>
#include "MOVE/Motion.h"
#include "MOVE/MovementBridge.h"

class MovementModule
{
public:
    static MovementModule &getInstance()
    {
        static MovementModule instance;
        return instance;
    }
    void Init();
    void update(float dt);
    bool isBusy();
    void NewMov(MotionState Command, float Left = 0, float Right = 0);
    void MoveSpeed(bool HiSpeed = true);
    void StopMov();
private:
    MovementModule();
};