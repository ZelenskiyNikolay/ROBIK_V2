#include "MOVE/MovementModule.h"

MovementCommand moveBridge;

MovementModule::MovementModule() {}

void MovementModule::Init() {}

void MovementModule::NewMov(MotionState Command, float Left, float Right)
{
    moveBridge.Command = Command;
    moveBridge.Left = Left;
    moveBridge.Right = Right;
    moveBridge.New_Command = true;
}

bool MovementModule::isBusy() { return moveBridge.IsBusy; }

void MovementModule::MoveSpeed(bool HiSpeed)
{
    moveBridge.Command = SET_SPEED;
    moveBridge.HiSpeed = HiSpeed;
    moveBridge.New_Command = true;
}

void StopMov(){moveBridge.Command = IDLE; moveBridge.New_Command = true;}