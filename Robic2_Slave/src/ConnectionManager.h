#pragma once
#include <Arduino.h>
#include "MOVE/Motion.h"
#include "MOVE/MovementBridge.h"
// #include "hardware/uart.h"

struct __attribute__((__packed__)) MOVEPacket
{
    byte command;
    float Left;
    float Right;
    bool HiSpeed;
};

class ConnectionManager
{
public:
    static ConnectionManager &getInstance()
    {
        static ConnectionManager instance;
        return instance;
    }
    void Init(int TXpin, int RXpin)
    {
        Serial1.setTX(TXpin);
        Serial1.setRX(RXpin);
        Serial1.begin(115200);
    }
    bool IsNewPack() { return (Serial1.available() >= sizeof(MOVEPacket)); }
    void GetCommannd(MovementCommand *mov)
    {
        Serial.println("Принят пакет в слейв");
        MOVEPacket tmp;

        Serial1.readBytes((byte *)&tmp, sizeof(tmp));

        Serial.print("Команда: ");
        Serial.print(tmp.command);
        Serial.print(" STEEPS: ");
        Serial.println(tmp.Left);
        mov->Command = (MotionState)(tmp.command);
        mov->HiSpeed = tmp.HiSpeed;
        mov->Left = tmp.Left;
        mov->Right = tmp.Right;
        mov->New_Command = true;
    }

private:
    ConnectionManager() {}
};