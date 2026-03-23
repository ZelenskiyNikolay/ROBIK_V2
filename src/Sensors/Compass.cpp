#include "Compass.h"
#include <math.h>

Compass::Compass() {}

Compass &Compass::getInstance()
{
    static Compass instance;
    return instance;
}

bool Compass::begin()
{
    if (!qmc.begin())
    {
        return false;
    }

    qmc.setMode(QMC5883P_MODE_NORMAL);
    qmc.setRange(QMC5883P_RANGE_8G);
    qmc.setODR(QMC5883P_ODR_200HZ);

    delay(10);

    return true;
}
void Compass::update(float dt)
{
    timer -= dt;
    if(timer<0)
    if (qmc.isDataReady())
    {
        qmc.getRawMagnetic(&x, &y, &z);

        float xf = (float)x - x_offset;
        float yf = (float)y - y_offset;

        // SCALE CORRECTION
        xf *= avg_scale / x_scale;
        yf *= avg_scale / y_scale;

        float temp = atan2(yf, xf) * 180.0f / PI;
        if (temp < 0)
            temp += 360.0f;

        // Вычисляем кратчайшую разницу между новым и старым углом
        float diff = temp - heading;
        if (diff > 180)
            diff -= 360;
        if (diff < -180)
            diff += 360;

        // Твоя идея усреднения (пополам), но примененная к разнице
        heading += diff / 2;

        // Возвращаем в диапазон 0-360
        if (heading >= 360)
            heading -= 360;
        if (heading < 0)
            heading += 360;

        timer = COMPASS_UPDATE_TIME;
    }
}

float Compass::getHeading()
{
    return heading;
}
