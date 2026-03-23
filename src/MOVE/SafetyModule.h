#pragma once
//#include "MotorModule.h"
//#include "MovementRequest.h"
#include "Sensors/SurfaceSensor.h"
//#include "Sensors/UltrasonicSensor.h"
//#include "Sensors/RPM_sensor.h"
#include "MotionController.h"
#include "SafetyModule.h"


enum SafetyTriger
{
    NONE,
    SENSOR_LEFT,
    SENSOR_RIGHT,
    SENSOR_BACK,
    SENSOR_BACK_FLOW,
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
    void TriggerUltrasonic();

private:
    SafetyModule();
    MotionController motion;
    SurfaceSensor sensorLeft, sensorRight;
   // SurfaceSensor sensorBack, sensorBackFlow;

    SafetyTriger sensorTrigger = SafetyTriger::NONE;

    //UltrasonicModule ultrasonic; //(16,17);

    bool corection = false;

    bool CheckSensors();
    void Corection();
};