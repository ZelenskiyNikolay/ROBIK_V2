#pragma once
//#include "MotorModule.h"
//#include "MovementRequest.h"
#include "Sensors/SurfaceSensor.h"
//#include "Sensors/RPM_sensor.h"
#include "MotionController.h"
#include "SafetyModule.h"
#include "MOVE/MovementBridge.h"

enum SafetyTriger
{
    NONE,
    SENSOR_LEFT,
    SENSOR_RIGHT,
    SENSOR_BACK,
    MENY_SENSORS
};

class SafetyModule
{
public:
    static SafetyModule &getInstance()
    {
        static SafetyModule instance;
        return instance;
    }
    long GetTics(bool left=true);
    void update(float dt);
    bool isBusy() const;
    void NewMov(MotionState Command, float Left = 0, float Right = 0);
    void MoveSpeed(bool HiSpeed = true);
    void StopMov();
    bool EdgeAlignment();

    float GetDistance();

private:
    SafetyModule();
    MotionController motion;
    SurfaceSensor sensorLeft, sensorRight;
    SurfaceSensor sensorBack;

    SafetyTriger sensorTrigger = SafetyTriger::NONE;

    bool corection = false;

    bool CheckSensors();
    void Corection();
};