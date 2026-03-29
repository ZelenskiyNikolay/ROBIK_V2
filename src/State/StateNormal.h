#pragma once
#include "Arduino.h"
#include "Core/State.h"
#include "Display/DisplayOled.h"
#include "Display/SpriteOled.h"
// #include "Sound/SoundManager.h"
#include "Core/FSM.h"
#include "Sensors/IRSensor.h"
#include "Sound/SoundManager.h"
//#include <RTClib.h>
// #include "Module/RTCModule.h"
// #include "Settings/GlobalSettings.h"
// #include "Module/BatteryModule.h"
// #include "Sensors/TouchButtons.h"

class StateNormal : public State
{
public:
    StateNormal(DisplayOled &disp);

    void enter() override;
    void update(float dt) override;
    // StateCommand handleEvent(Event e) override;
    void Draw(float dt);

private:
    const float Open_Eyes = 10000;
    const float Close_Eyes = 1000;
    bool IsOpen;
    float timer;
    bool DRAWBAT = false;


    void DrawClock(float dt);
    bool Clock = false;

    bool isDrawingBattery = true;


    void IrLogic();
    bool IsDrawClock = false;
    const float ApdateTimeConst = 1000;

    void LightReaction(float dt);
    int steep=0;
    float timer1;
    bool Light = false;

    void LOWBat();

    void DrawVolumeCount(float dt);
    bool IsDrawVolume = false;
    uint8_t _volume;

    // DateTime _time;

    DisplayOled *display;
    SpriteOled sprite;
    // Sound sound;
    float blinkTimer = 3000;
    bool dance = true;
};