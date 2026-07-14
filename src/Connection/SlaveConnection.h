#pragma once
#include <Arduino.h>
#include"MOVE/Motion.h"
#include "hardware/uart.h"

struct __attribute__((__packed__)) MOVEPacket {
    byte command;
    float Left;
    float Right;
    bool HiSpeed;
};

class SlaveConnection
{
public:
    static SlaveConnection &getInstance()
    {
        static SlaveConnection instance;
        return instance;
    }
    void Init();
    void NewMov(MotionState Command, float Left = (0.0F), float Right = (0.0F));
    void MoveSpeed(bool HiSpeed = true);
private:
    SlaveConnection() {}
};