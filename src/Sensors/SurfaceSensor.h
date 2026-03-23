#pragma once
#include <Arduino.h>

class SurfaceSensor {
public:
    SurfaceSensor(int pin);
    void update();
    bool GetSensorState();
private:
    int SensorPin;
    bool lastState = false;
};