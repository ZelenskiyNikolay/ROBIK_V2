#pragma once
#include <Arduino.h>

enum ButtonIR
{
    ButtonLeft,
    ButtonRight,
    ButtonUp,
    ButtonDown,
    ButtonOk,
    ButtonStar,
    ButtonHash,
    Button0,
    Button1,
    Button2,
    Button3,
    Button4,
    Button5,
    Button6,
    Button7,
    Button8,
    Button9,
    NOOL,
};

class IRSensor
{
public:
    static IRSensor &getInstance()
    {
        static IRSensor instance;
        return instance;
    }
    void begin(int pin);
    void update();
    ButtonIR GetSensorState();

private:
    IRSensor();
    int SensorPin;
    ButtonIR lastState = NOOL;
};