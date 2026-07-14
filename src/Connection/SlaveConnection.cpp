#include "SlaveConnection.h"

void SlaveConnection::Init()
{
    Serial2.end();
    Serial2.setTX(8);
    Serial2.setRX(9);
    Serial2.begin(115200);
}
void SlaveConnection::NewMov(MotionState Command, float Left, float Right)
{
    MOVEPacket pac;
    pac.command = Command;
    pac.Left = Left;
    pac.Right = Right;

    Serial2.write((byte *)&pac, sizeof(pac));
}
void SlaveConnection::MoveSpeed(bool HiSpeed)
{
    MOVEPacket pac;
    pac.command = SET_SPEED;
    pac.HiSpeed = HiSpeed;

    Serial2.write((byte *)&pac, sizeof(pac));
}