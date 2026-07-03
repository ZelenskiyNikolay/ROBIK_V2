#pragma once

#include "Core/State.h"
#include "Module/RTCModule.h"

#include "Display/DisplayOled.h"
#include "Display/SpriteOled.h"
#include "Display/UI/Menu.h"
#include "Display/UI/MenuController.h"
// #include "Display/UI/MenuClick.h"
#include "Display/MatriX_Overlay/MatriX.h"

#include "Module/BatteryModule.h"
#include "Core/EventBus.h"
#include "Sensors/IRSensor.h"

#include "Sensors/Compass.h"
#include "MOVE/MovementModule.h"

#include "Sensors/TOFRadar/Radar.h"
#include "Adafruit_VL53L0X.h"

class StateStartV2 : public State
{
public:
    StateStartV2(DisplayOled &disp);

    void enter() override;
    void update(float dt) override;
    void Draw(float dt);

private:
    void DrawLabel();
    void SensorBat();
    void ChargeBat();
    void DrawClock();
    void Draw_D_Test();

    void IrLogic();

    // Методы мено:
    void CreateMainManu();
    void exitMenu();
    void back();
    void infoBat();
    void сompassTest();
    void ultrasonicTest();
    void SetNightMod();
    void SetSoundVolume();
    void ClickSounnd();
    void powerOff();

    void screensaver(float dt);
    void servo(int angle);
    void radar_Test();
    void One_Measure_Test();
    void I2C_Scaner();
    void Tof();

    const float ApdateTimeConst = 30000;
    float timer;
    float _timerPoint;
    bool num_menu = false;
    bool showBattery = false;

    DateTime _time;

    DisplayOled *display;
    SpriteOled sprite;

    MenuController *menu = nullptr;

    double timeout_Screensaver = 30000;
    float timer_Screensaver = timeout_Screensaver;
    bool is_Screensaver_Play = false;
    Matrix matrix;
    Radar radar;
};