#pragma once
#include <Arduino.h>
#include "MOVE/Motion.h"

struct MovementCommand {
    volatile bool New_Command = false;
    float Left = 0;
    float Right = 0;
    bool HiSpeed;
    volatile bool IsBusy;
    MotionState Command;
};

extern  MovementCommand moveBridge;