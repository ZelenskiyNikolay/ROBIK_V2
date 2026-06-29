#include <Arduino.h>
#include "MyServo.h"
#include "Adafruit_VL53L0X.h"

class Radar
{
private:
    MyServo myServo;
    Adafruit_VL53L0X lox;
    VL53L0X_RangingMeasurementData_t measure;

    int forward;
    int steep;
    bool _isScan = false;
    bool _IsOne = false;
    

public:
    Radar();
    uint32_t Servo0 =  700;
    uint32_t Servo180 = 2800;
    
    uint16_t servoAngleTable[5] = { 800, 1300, 1800, 2300, 2800 };
    int data[5];

    void begin(uint pin);
    void scan();
    void update(float dt);
    bool IsScan(){return _isScan;}
    const int* GetData(){return data;}
    const int GetData(int i){return data[i];}
    void SetAngServo(int ang){myServo.play();myServo.write(ang);}
    void SetMicrosServo(int micros){myServo.play();myServo.write_mic(micros);}
    void OneСornerMeasure(int ang = SERVO_90);
    bool IsOneСornerMeasureRedy = false;
    int OneMeasureСorner = 0;
    int Measure = 0;
    enum ServoPoint
    {
        SERVO_0,
        SERVO_45,
        SERVO_90,
        SERVO_135,
        SERVO_180
    };
    enum Steeps {
        SCAN,
        MOV,
        MEASURE
    };
    Steeps OneСornerSteep = SCAN;
};