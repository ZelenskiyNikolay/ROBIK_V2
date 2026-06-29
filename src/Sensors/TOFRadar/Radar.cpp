#include "Radar.h"

Radar::Radar() : myServo(), lox()
{
}

void Radar::begin(uint pin)
{
    myServo.attach(pin, Servo0, Servo180);
    myServo.write_mic(servoAngleTable[SERVO_90]);
    while (true)
    {
        myServo.UpdateServo(0.001f);
        if (myServo.IsRotateComplite())
            break;
    }

    lox.begin(41U,false,&Wire1);
}
void Radar::scan()
{
    steep = 0;
    _isScan = true;
    myServo.play();
    myServo.write_mic(servoAngleTable[0]);
}
inline void sort(int *d, int size)
{
    for (int i = 1; i < size; i++)
    {
        uint16_t key = d[i];
        int j = i - 1;
        while (j >= 0 && d[j] > key)
        {
            d[j + 1] = d[j];
            j--;
        }
        d[j + 1] = key;
    }
}
void Radar::update(float dt)
{
    if (_IsOne)
    {
        switch (OneСornerSteep)
        {
        case SCAN:
            OneСornerSteep = MOV;
            myServo.play();
            myServo.write_mic(servoAngleTable[OneMeasureСorner]);
            break;
        case MOV:
            if (!myServo.IsRotateComplite())
                myServo.UpdateServo(dt);
            else
                OneСornerSteep = MEASURE;
            break;
        case MEASURE:
            if (myServo.IsRotateComplite())
            {
                lox.rangingTest(&measure,false);
                if (measure.RangeStatus != 4)
                {
                    Measure = (measure.RangeMilliMeter-25);
                    _IsOne = false;
                    IsOneСornerMeasureRedy = true;
                    OneСornerSteep = SCAN;
                }
                else
                {
                    Measure = -1; // measure.RangeMilliMeter;
                    _IsOne = false;
                    IsOneСornerMeasureRedy = true;
                    OneСornerSteep = SCAN;
                }
            }
            break;
        default:
            break;
        }
        // myServo.UpdateServo(dt);
    }
    if (!_isScan)
    {
        myServo.UpdateServo(dt);
        return;
    }
    if (myServo.IsRotateComplite())
    {
        lox.rangingTest(&measure);
        if (measure.RangeStatus != 4)
            data[steep] = (measure.RangeMilliMeter-25);
        else
        {
            lox.rangingTest(&measure);
            if (measure.RangeStatus != 4)
                data[steep] = (measure.RangeMilliMeter-25);
            else
                data[steep] = -1;
        }

        if (steep < 4)
        {
            steep++;
            myServo.play();
            myServo.write_mic(servoAngleTable[steep]);
        }
        else
        {
            myServo.play();
            myServo.write_mic(servoAngleTable[SERVO_90]);
            _isScan = false;
        }
    }
    else
        myServo.UpdateServo(dt);
}
void Radar::OneСornerMeasure(int ang)
{
    _IsOne = true;
    IsOneСornerMeasureRedy = false;
    OneMeasureСorner = ang;
    OneСornerSteep = SCAN;
}

// {
//     switch (OneСornerSteep)
//     {
//     case SCAN:
//         OneСornerSteep = MOV;
//         myServo.play();
//         myServo.write_mic(servoAngleTable[ang]);
//         break;
//     case MOV:
//         if (!myServo.IsRotateComplite())
//             myServo.UpdateServo(dt);
//         else
//             OneСornerSteep = MEASURE;
//         break;
//     default:
//         break;
//     }
// }