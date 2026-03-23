#include "SurfaceSensor.h"

SurfaceSensor::SurfaceSensor(int pin) : SensorPin(pin)
{
    pinMode(SensorPin, INPUT);

    lastState = digitalRead(SensorPin);
}

void SurfaceSensor::update()
{
    bool motion = (bool)digitalRead(SensorPin);
}
bool SurfaceSensor::GetSensorState()
{
    return !digitalRead(SensorPin);
}