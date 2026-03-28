#include "MOVE/MovementModule.h"

MovementCommand moveBridge;

MovementModule::MovementModule(){}

void MovementModule::Init(){}

void MovementModule::NewMov(MotionState Command, float Left, float Right)
{
    moveBridge.Command = Command;
    moveBridge.Left = Left;
    moveBridge.Right = Right;
    moveBridge.HiSpeed = true;
    moveBridge.New_Command = true;
}

bool MovementModule::isBusy(){return moveBridge.IsBusy;}