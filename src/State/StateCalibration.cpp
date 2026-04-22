#include "StateCalibration.h"

StateCalibration::StateCalibration(DisplayOled &disp)
    : display(&disp), sprite(&disp)
{
}

void StateCalibration::enter()
{
    display->clear();
    display->drawText("Calibration:", 0, 0, 1);
    timer = 0;
}

void StateCalibration::update(float dt)
{
    MovementModule::getInstance().update(dt);
    ProcessCalibration(dt);
    Draw(dt);
}

void StateCalibration::ProcessCalibration(float dt)
{
    if (steep_calibration == 0)
    {
        MovementModule::getInstance().MoveSpeed();
        MovementModule::getInstance().NewMov(MotionState::FORWARD, 5, 5);
        steep_calibration++;
        return;
    }
    if (steep_calibration == 1)
    {
        if (!MovementModule::getInstance().isBusy())
        {
            MovementModule::getInstance().NewMov(MotionState::BACKWARD, 0.2, 0.2);
            steep_calibration++;
            timer1 = 1000;
            return;
        }
    }
    if (steep_calibration == 2)
    {
        timer1 -= dt;
        if (timer1 < 0 && !MovementModule::getInstance().isBusy())
        {
            float heading = Compass::getInstance().getHeading();
            MovementModule::getInstance().SouthAngle = (int)heading;
            steep_calibration++;
        }
    }

    if (steep_calibration == 3)
    {
        if (!MovementModule::getInstance().isBusy())
        {
            MovementModule::getInstance().NewMov(MotionState::TURN_LEFT90);
            steep_calibration++;
            timer1 = 4000;
            return;
        }
    }
    if (steep_calibration == 4)
    {
        timer1 -= dt;
        if (timer1 < 0)
        {
            float temp = MovementModule::getInstance().GetDistance();
            if (temp > 0 && temp < 300)
            {
                distance = temp;
            }

            if (distance < 40 || distance > 70)
            {
                steep_calibration = 4;
                return;
            }
            else
            {
                MovementModule::getInstance().EastDistanse = (int)distance;
                steep_calibration++;
                timer1 = 500;
                return;
            }
        }
    }
    if (steep_calibration == 5)
    {
        if (!MovementModule::getInstance().isBusy())
        {
            MovementModule::getInstance().NewMov(MotionState::TURN_RIGHT, 0.15, 0.15);
            steep_calibration++;
            return;
        }
    }
    if (steep_calibration == 6)
    {
        if (!MovementModule::getInstance().isBusy())
        {
            MovementModule::getInstance().NewMov(MotionState::BACKWARD, 0.5, 0.5);
            steep_calibration++;
            return;
        }
    }
    if (steep_calibration > 6)
    {
        if (!MovementModule::getInstance().isBusy())
            EventBus::push({EVENT_CHANGE_STATE, STATE_NORMAL});
    }
}

void drawCompassIcon(int16_t x, int16_t y, DisplayOled &display) {
    // 1. Рисуем круг (радиус 10 пикселей)
    display.drawCircle(x, y, 10, SSD1306_WHITE);
    
    // 2. Рисуем "Север" (закрашенный треугольник)
    // Координаты: (верхушка, левый угол, правый угол)
    display.fillTriangle(x, y - 8, x - 4, y, x + 4, y, SSD1306_WHITE);
    
    // 3. Рисуем "Юг" (пустой треугольник)
    display.drawTriangle(x, y + 8, x - 4, y, x + 4, y, SSD1306_WHITE);
    
    
    display.drawText("N", x - 2, y - 18, 1);
}
void StateCalibration::Draw(float dt)
{
    timer -= dt;
    if (timer < 0)
    {
        display->clear();
        display->drawText("Calibration:", 0, 0, 1);
        drawCompassIcon(110, 25,*display);
        char buffer[32];
        sprintf(buffer, "Аng: %d cm", MovementModule::getInstance().SouthAngle);
        display->drawText(buffer, 0, 55, 1);
        sprintf(buffer, "Dis: %d cm", (int)distance);
        display->drawText(buffer, 0, 35, 1);

        timer = 500;
    }
}