#include "../StateStartV2.h"

void StateStartV2::exitMenu()
{
    StateStartV2::menu->hide();
    timer = 0;
}
void StateStartV2::back()
{
    menu->back();
    timer = 0;
}

void StateStartV2::infoBat()
{
    display->clear();
    display->drawText("BATTERY SENSOR:", 0, 0, 1);
    char buffer1[16];
    int percent = BatteryModule::getInstance().getBatteryPercent();

    BatteryModule::getInstance().drawBatteryIcon(*display, 0, 10, percent);

    sprintf(buffer1, "Charge:%02d%s", percent, " %");
    display->drawText(buffer1, 0, 40, 1);

    float v = BatteryModule::getInstance().getVoltage();
    int whole = v;
    int fract = (v - whole) * 100;

    sprintf(buffer1, "Voltage:%d.%02d V", whole, fract);
    display->drawText(buffer1, 0, 50, 1);
}

void CompassIcon(int16_t x, int16_t y, DisplayOled &display)
{
    // 1. Рисуем круг (радиус 10 пикселей)
    display.drawCircle(x, y, 10, SSD1306_WHITE);

    // 2. Рисуем "Север" (закрашенный треугольник)
    // Координаты: (верхушка, левый угол, правый угол)
    display.fillTriangle(x, y - 8, x - 4, y, x + 4, y, SSD1306_WHITE);

    // 3. Рисуем "Юг" (пустой треугольник)
    display.drawTriangle(x, y + 8, x - 4, y, x + 4, y, SSD1306_WHITE);

    display.drawText("N", x - 2, y - 18, 1);
}
void StateStartV2::сompassTest()
{
    float heading = Compass::getInstance().getHeading();
    display->clear();
    display->drawText("Compass", 0, 0, 2);
    display->drawText("Test:", 0, 20, 2);
    char buffer[16];

    CompassIcon(110, 25, *display);

    sprintf(buffer, "ANG:%.1f%s", heading, "'");
    display->drawText(buffer, 0, 44, 2);
}
void StateStartV2::ultrasonicTest()
{
    float dis = MovementModule::getInstance().GetDistance();
    display->clear();

    display->drawText("Ultrasinic", 0, 0, 2);
    display->drawText("Test:", 0, 20, 2);
    char buffer[16];

    sprintf(buffer, "D:%.1f%s", dis, "CM");
    display->drawText(buffer, 0, 44, 2);
}
void StateStartV2::SetNightMod()
{
    Action a = menu3->getAction();
    bool night = display->IsNightMod();

    if (a == NEXT || a == PREV)
    {
        night = !night;
        display->nightMod(night);
    }
    else if (a == OK)
    {
        menu3->setEditMode(false);
        menu3->resetTimer();
    }
    char buf[16];
    sprintf(buf, ">%s<", night ? "true" : "false");
    menu3->setEditModeEmpty(buf);
}
int Vol = 100;
void StateStartV2::SetSoundVolume()
{
    Action a = menu3->getAction();

    if (a == NEXT)
    {
        if(Vol>0)
            Vol-=5;
    }
    else if (a == PREV)
    {
        if(Vol<100)
            Vol+=5;
    }
    else if (a == OK)
    {
        //SoundManager->SetVolume(Val) это не реализовано.. 

        menu3->setEditMode(false);
        menu3->resetTimer();
    }

    char buf[16];
    sprintf(buf, ">%d%s<", Vol,"%");
    menu3->setEditModeEmpty(buf);
}