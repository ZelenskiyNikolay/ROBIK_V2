#pragma once
enum MotionState
{
    IDLE,
    FORWARD,
    BACKWARD,
    TURN_LEFT,
    TURN_RIGHT,
    TURN_LEFT90,
    TURN_RIGHT90
};

struct MotionCommand
{
    MotionState type;
    float targetLeft;
    float targetRight;
};