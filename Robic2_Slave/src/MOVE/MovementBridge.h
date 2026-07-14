#pragma once
#include <Arduino.h>
#include "MOVE/Motion.h"
#include "MOVE/Tics.h"

struct MovementCommand {
    volatile bool New_Command = false;
    float Left = 0;
    float Right = 0;
    bool HiSpeed;
    volatile bool IsBusy;
    MotionState Command;
    volatile bool Odonometry_Apdeted = false;
    Odonometry odonometry;
};

extern  MovementCommand moveBridge;