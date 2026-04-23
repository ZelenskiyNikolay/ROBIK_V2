#include "SafetyModule.h"

SafetyModule::SafetyModule()
    : sensorLeft(11), sensorRight(12), sensorBack(24), motion()
{
    CheckSensors();
    motion.begin();
}

void SafetyModule::MoveSpeed(bool HiSpeed) { motion.MoveSpeed(HiSpeed); }
int Time = 1000;
float timer = 0;
void SafetyModule::update(float dt)
{
    timer -= dt;
    if (timer < 0)
    {
        Serial.print("ЕdgeAlign: ");
        Serial.println(edgeAlign);
        timer = Time;
    }
    if (!edgeAlign)
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
                    moveBridge.New_Command = false;
                    break;
                case EDGE_ALIG:
                    edgeAlign = true;
                    NewMov(moveBridge.Command);
                    moveBridge.IsBusy = true;
                    moveBridge.New_Command = false;
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
    else
    {
        if (EdgeAlignment(dt))
        {
            edgeAlign = false;
        }
    }
}

void SafetyModule::StopMov() { motion.Stop(); }

bool leftHit = false;
bool rightHit = false;
float timer1 = 0;
bool SafetyModule::EdgeAlignment(float dt)
{
    bool Left = sensorLeft.GetSensorState();
    bool Right = sensorRight.GetSensorState();

    timer1 -= dt;
    if (timer1 < 0)
    {
        Serial.print("leftHit: ");
        Serial.print(leftHit);
        Serial.print("rightHit: ");
        Serial.print(rightHit);
        timer1 = Time;
    }
    motion.update(dt);

    if (!Left)
    {
        leftHit = true;
        motion.StopLeft();
    }
    if (!Right)
    {
        rightHit = true;
        motion.StopRight();
    }
    if (leftHit && rightHit)
    {
        motion.Stop();
        leftHit = false;
        rightHit = false;
        return true;
    }
    return false;
}
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

    if (Left && Right && Back)
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
