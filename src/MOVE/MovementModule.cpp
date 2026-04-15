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
bool MovementModule::IsBusyQueue() { return isBusyQueue; }

void MovementModule::MoveSpeed(bool HiSpeed)
{
    moveBridge.Command = SET_SPEED;
    moveBridge.HiSpeed = HiSpeed;
    moveBridge.New_Command = true;
}

void MovementModule::StopMov()
{
    NewMov(MotionState::IDLE);
    // moveBridge.Command = IDLE;
    // moveBridge.New_Command = true;
}

void MovementModule::update(float dt)
{
    if (weit_timer > 0)
        weit_timer -= dt;
    if (weit_timer <= 0)
        if (!isBusy())
            if (bottom_queue != top_queue)
                POPQueue();
}
void MovementModule::ResetQueue()
{
    bottom_queue = top_queue;
    StopMov();
}

void MovementModule::AddMovQueue(MotionState Command, float Left, float Right)
{
    _queue[top_queue].type = Command;
    _queue[top_queue].targetLeft = Left;
    _queue[top_queue].targetRight = Right;

    if (top_queue < QUEUE_SIZE - 1)
    {
        top_queue++;
    }
    else
    {
        top_queue = 0;
    }

    Serial.print("Added to index: ");
    Serial.println(top_queue);
}
void MovementModule::POPQueue()
{
    if (bottom_queue != top_queue)
    {
        MotionCommand cmd = _queue[bottom_queue];
        if (cmd.type == SET_SPEED)
            MoveSpeed((bool)cmd.targetLeft);
        else if (cmd.type == WEIT)
        {
            weit_timer = cmd.targetLeft;
        }
        else
        {
            isBusyQueue = true;
            NewMov(cmd.type, cmd.targetLeft, cmd.targetRight);
        }

        if (bottom_queue < QUEUE_SIZE - 1)
        {
            bottom_queue++;
        }
        else
        {
            bottom_queue = 0;
        }

        Serial.println("Next command from queue started!");
    }
    else
    isBusyQueue = false;
}