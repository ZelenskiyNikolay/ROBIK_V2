#include "SafetyModule.h"

SafetyModule::SafetyModule()
    : sensorLeft(11), sensorRight(12), sensorBack(24), motion()
                                                // ultrasonic(28, 3), motion()
{
    // ultrasonic.begin();
    CheckSensors();
    motion.begin();
}


void SafetyModule::MoveSpeed(bool HiSpeed) { motion.MoveSpeed(HiSpeed); }

void SafetyModule::update(float dt)
{
    if (!isBusy())
    {
        if (moveBridge.New_Command)
        {
            switch (moveBridge.Command)
            {
            case SET_SPEED:
                MoveSpeed(moveBridge.HiSpeed);
                moveBridge.New_Command = false;
                break;
            case IDLE:
                StopMov();
            break;
            default:
                MoveSpeed(moveBridge.HiSpeed);
                NewMov(moveBridge.Command, moveBridge.Left, moveBridge.Right);
                moveBridge.IsBusy = true;
                moveBridge.New_Command = false;
                break;
            }
        }
        else
            moveBridge.IsBusy = false;
    }

    if (corection)
    {
        CheckSensors();
        Corection();
        motion.update(dt);
        return;
    }
    if (CheckSensors())
    {
        corection = true;
    }
    else
    {
        motion.update(dt);
    }
}

void SafetyModule::StopMov() {}
// bool SafetyModule::EdgeAlignment()
// {
//     bool Left = sensorLeft.GetSensorState();
//     bool Right = sensorRight.GetSensorState();

//     if (Left && Right)
//     {
//         motion.Forward();
//     }
//     if (!Left)
//     {
//         motion.LeftStop();
//     }
//     if (!Right)
//     {
//         motion.RightStop();
//     }
//     if (!Left && !Right)
//     {
//         motion.SafatyStop();
//         return true;
//     }
//     return false;
// }
void SafetyModule::Corection()
{
    switch (sensorTrigger)
    {
    case SafetyTriger::SENSOR_LEFT:
        NewMov(MotionState::BACKWARD, 0.1, 0.1);
        break;
    case SafetyTriger::SENSOR_RIGHT:
        NewMov(MotionState::BACKWARD, 0.1, 0.1);
        break;
    case SafetyTriger::SENSOR_BACK:
        NewMov(MotionState::FORWARD, 0.1, 0.1);
        break;
    case SafetyTriger::NONE:
        corection = false;
        break;
    default:
        break;
    }
}

bool SafetyModule::CheckSensors()
{
    bool Left = sensorLeft.GetSensorState();
    bool Right = sensorRight.GetSensorState();
    bool Back = sensorBack.GetSensorState();

    if (Left && Right &&  Back)
        sensorTrigger = SafetyTriger::NONE;
    else if (!Left)
        sensorTrigger = SafetyTriger::SENSOR_LEFT;
    else if (!Right)
        sensorTrigger = SafetyTriger::SENSOR_RIGHT;
    else if (!Back)
        sensorTrigger = SafetyTriger::SENSOR_BACK;

    return (!Left || !Right || !Back);
}

bool SafetyModule::isBusy() const
{
    return motion.isBusy();
}
void SafetyModule::NewMov(MotionState Command, float Left, float Right)
{
    motion.NewMov(Command, Left, Right);
}
// long SafetyModule::GetTics(bool left) { return motion.GetTics(left); }
// void SafetyModule::StopMov(){ motion.SafatyStop();}
