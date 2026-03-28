#pragma once
#include "Motor.h"
#include "MOVE/Motion.h"
//#include "Sensors/Compass.h"
// #include "MotorModule.h"
// #include "Sensors/RPM_sensor.h"

#define STEPS_90 1095

#define STEPS_PER_REV 2048 // 28BYJ-48 (half-step)

#define EN_PIN 10


class MotionController
{
private:
    Motor motorL;
    Motor motorR;

    MotionCommand command;

    int TIME_STEP = 1;

    float timer = TIME_STEP;

    float startHeading = 0;
    float targetHeading = 0;

    void ForwardMov();
    void BackwardMov();
    void Turn90Left();
    void Turn90Right();
    void TurnLeft();
    void TurnRight();

public:
    MotionController();
    void begin();
    int update(float dt);
    bool isBusy() const;
    void NewMov(MotionState Command, float Left = 0, float Right = 0);
    void MoveSpeed(bool HiSpeed = true);
};