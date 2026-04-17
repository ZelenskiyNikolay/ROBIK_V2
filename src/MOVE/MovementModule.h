#pragma once
#include <Arduino.h>
#include "MOVE/Motion.h"
#include "MOVE/MovementBridge.h"
#include "Sensors/Ultrasonic.h"

#define QUEUE_SIZE 30

class MovementModule
{
public:
    static MovementModule &getInstance()
    {
        static MovementModule instance;
        return instance;
    }
    void Init();
    void update(float dt);
    bool isBusy();
    void NewMov(MotionState Command, float Left = 0, float Right = 0);
    void AddMovQueue(MotionState Command, float Left = 0, float Right = 0);
    void MoveSpeed(bool HiSpeed = true);
    void StopMov();

    void ResetQueue();
    bool IsBusyQueue();

    float GetDistance();
    void TriggerUltrasonic();
private:
    MovementModule();
    Ultrasonic ultrasonic; //(0,1);

    MotionCommand _queue[QUEUE_SIZE];
    int bottom_queue = 0;
    int top_queue = 0;
    float weit_timer = 0;

    void POPQueue();
    bool isBusyQueue = false;
};