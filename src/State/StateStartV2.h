#pragma once

#include "Core/State.h"
#include "Module/RTCModule.h"

#include "Display/DisplayOled.h"
#include "Display/SpriteOled.h"
#include "Display/UI/Menu.h"
#include "Display/UI/MenuController.h"

#include "Module/BatteryModule.h"
#include "Core/EventBus.h"
#include "Sensors/IRSensor.h"

#include "Sensors/Compass.h"
#include "MOVE/MovementModule.h"

class StateStartV2 : public State
{
public:
    StateStartV2(DisplayOled &disp);
    virtual ~StateStartV2();

    void enter() override;
    void update(float dt) override;
    void Draw(float dt);

private:
    void DrawLabel();
    void SensorBat();
    void ChargeBat();
    void DrawClock(float dt);
    void Draw_D_Test();

    void IrLogic();

    //Методы мено:
    void exitMenu();
    void back();
    void infoBat();
    void сompassTest();
    void ultrasonicTest();

    const float ApdateTimeConst = 30000;
    float timer;
    float _timerPoint;
    bool num_menu = false;
    bool showBattery = false;

    DateTime _time;

    DisplayOled *display;
    SpriteOled sprite;

    Menu *menu1 = nullptr;
    Menu *menu2 = nullptr;
    MenuController *menu = nullptr;
};