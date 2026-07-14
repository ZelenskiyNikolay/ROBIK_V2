#include <Arduino.h>
#include "includes.h"

void MoveSpeed(bool HiSpeed)
{
    moveBridge.Command = SET_SPEED;
    moveBridge.HiSpeed = HiSpeed;
    moveBridge.New_Command = true;
}
void NewMov(MotionState Command, float Left = 0, float Right = 0);
void NewMov(MotionState Command, float Left, float Right)
{
    moveBridge.Command = Command;
    moveBridge.Left = Left;
    moveBridge.Right = Right;
    moveBridge.New_Command = true;
}
void IrLogic();
void setup()
{
    Serial.begin(9600);
    delay(2000);

    IRSensor::getInstance().begin(29);
    ConnectionManager::getInstance().Init(0,1);

    SafetyModule::getInstance().NewMov(MotionState::FORWARD, 0.01f, 0.01f);
}

void loop()
{
    float dt = getDeltaTime();

    // IRSensor::getInstance().update();

    // IrLogic();
    if(ConnectionManager::getInstance().IsNewPack())
        ConnectionManager::getInstance().GetCommannd(&moveBridge);

    SafetyModule::getInstance().update(dt);

    FpsCount(dt);
    tight_loop_contents();
}
void IrLogic()
{
    ButtonIR tmp = IRSensor::getInstance().GetSensorState();

    if (tmp == NOOL)
        return;

    switch (tmp)
    {
    case ButtonIR::ButtonUp:
        NewMov(MotionState::FORWARD, 0.5f, 0.5f);
        break;
    case ButtonDown:
        NewMov(MotionState::BACKWARD, 0.5f, 0.5f);
        break;
    case ButtonLeft:
        NewMov(MotionState::TURN_LEFT90);
        break;
    case ButtonRight:
        NewMov(MotionState::TURN_RIGHT90);
        break;
    default:
        break;
    }
}
void setup1()
{
}
void loop1()
{
    float dt1 = getDeltaTime1();

    FpsCount1(dt1);
    tight_loop_contents();
}