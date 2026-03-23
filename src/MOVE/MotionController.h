#pragma once
#include "Motor.h"
//#include "Sensors/Compass.h"
// #include "MotorModule.h"
// #include "Sensors/RPM_sensor.h"

#define STEPS_90 2245

#define STEPS_PER_TURN 2048
#define STEPS_PER_REV 4096 // 28BYJ-48 (half-step)

#define EN_PIN 10

//#define TIME_STEP 1

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

public:
    MotionController();
    void begin();
    int update(float dt);
    bool isBusy() const;
    void NewMov(MotionState Command, float Left = 0, float Right = 0);
    void MoveSpeed(bool HiSpeed = true);
};