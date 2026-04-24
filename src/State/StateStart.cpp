#include "StateStart.h"

StateStart::StateStart(DisplayOled &disp)
    : display(&disp), sprite(&disp)
{
}

void StateStart::enter()
{
    display->clear();
    timer = 0;

    display->drawText("  ROBIK", 0, 0, 2);
    display->drawText("              v2.4+", 0, 20, 1);
    display->drawText("press 3 Battary %", 0, 45, 1);
    display->drawText("press 1 to start.", 0, 55, 1);

    num_menu = false;

    MovementModule::getInstance().Init();
}
void StateStart::update(float dt)
{
    MovementModule::getInstance().update(dt);
    BatteryModule &bat = BatteryModule::getInstance();

    if (bat.getBatteryPercent() < 99)
    {
        if (bat.IsChargeConect())
        {
            if (menu != BATARY_CHARGING)
            {
                menu = BATARY_CHARGING;
                num_menu = true;
                timer = 0;
            }
        }
    }
    else
    {
        if (menu == BATARY_CHARGING)
        {
            menu = LABEL;
            num_menu = false;
        }
    }

    IrLogic();

    Draw(dt);
}

void StateStart::IrLogic()
{

    ButtonIR tmp = IRSensor::getInstance().GetSensorState();
    switch (tmp)
    {
    case Button1:
        if (menu == LABEL)
            EventBus::push({EVENT_CHANGE_STATE, STATE_CALIBRATION});
        break;
    case Button2:
        menu = BATARY;
        break;
    case Button3:
        if (menu == LABEL)
            EventBus::push({EVENT_CHANGE_STATE, STATE_USB});
        break;

    case Button4:
        if (menu == LABEL)
            EventBus::push({EVENT_CHANGE_STATE, STATE_VOICE_RECORD});
        break;

    case ButtonStar:
        if (menu != Time)
        {
            menu = Time;
            display->nightMod();
        }
        else
        {
            menu = LABEL;
            display->nightMod(false);
        }
        break;
    case Button0:
        if (menu != DISTANSE_TEST)
        {
            menu = DISTANSE_TEST;
        }
        else
        {
            menu = LABEL;
        }
        break;
    case ButtonHash:
        menu = LABEL;
        break;

    default:
        break;
    }
}
void StateStart::Draw(float dt)
{
    timer -= dt;
    if (timer <= 0)
    {
        timer = 500;
        display->clear();

        if (menu == BATARY)
            SensorBat();
        else if (menu == LABEL)
            DrawLabel();
        else if (menu == BATARY_CHARGING)
            ChargeBat();
        else if (menu == Time)
            DrawClock(dt);
        else if (menu == DISTANSE_TEST)
            Draw_D_Test();
    }
}

void StateStart::Draw_D_Test()
{
    float dis = MovementModule::getInstance().GetDistance();
    float heading = Compass::getInstance().getHeading();
    display->clear();
    char buffer[16];
    sprintf(buffer, "G:D%d, A%d", MovementModule::getInstance().EastDistanse ,
     MovementModule::getInstance().SouthAngle);
    display->drawText(buffer, 0, 0, 1);

    sprintf(buffer, "D:%.1f%s", dis, "CM");
    display->drawText(buffer, 0, 20, 2);
    // Serial.println(buffer);
    sprintf(buffer, "ANG:%.1f%s", heading, "'");
    display->drawText(buffer, 0, 40, 2);
}
void StateStart::DrawLabel()
{
    display->clear();

    display->drawText("  ROBIK", 0, 0, 2);
    display->drawText("              v2.4+", 0, 20, 1);
    display->drawText("press 3 USB MOD", 0, 35, 1);
    display->drawText("press 2 Battary %", 0, 45, 1);
    display->drawText("press 1 to start.", 0, 55, 1);
}
void StateStart::SensorBat()
{
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

void StateStart::ChargeBat()
{
    display->drawText("CHARGING BATTERY:", 0, 0, 1);
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

void StateStart::DrawClock(float dt)
{
    display->clear();

    _time = RTCModule::getInstance().getTime();
    char buffer[9]; // "HH:MM"

    auto second = _time.second();
    if (second % 2 == 0)
        sprintf(buffer, "%02d:%02d", _time.hour(), _time.minute());
    else
        sprintf(buffer, "%02d %02d", _time.hour(), _time.minute());

    display->drawText(buffer, 0, 0, 4);
    sprintf(buffer, "%02d/%02d", _time.day(), _time.month());
    display->drawText(buffer, 0, 32, 4);
}