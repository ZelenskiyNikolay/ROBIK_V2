#pragma once
#include <Adafruit_QMC5883P.h>
#define COMPASS_UPDATE_TIME 200

class Compass
{

public:
    static Compass &getInstance();

    bool begin();
    void update(float dt);
    float getHeading();

private:
    Compass();

    Adafruit_QMC5883P qmc;

    volatile float heading = 0;

    int16_t x, y, z;

    float timer =  COMPASS_UPDATE_TIME;
    // offsets после калибровки
    float x_offset = -230.5f;
    float y_offset = 243.5f;

    float x_scale = 890.5f;
    float y_scale = 899.5f;
    float avg_scale = 895.0f;
};