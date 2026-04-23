#pragma once
#include "Arduino.h"
#include "Core/State.h"
#include "Display/DisplayOled.h"
#include "Display/SpriteOled.h"
#include "Core/FSM.h"
#include "Sensors/IRSensor.h"
#include "Module/RTCModule.h"
#include "SoundMic/VoiceControl.h"

#define UPDATE_TIME 1000
#define DISPLAY_UP_TIME 500

class StateClock : public State
{
public:
    StateClock(DisplayOled &disp);

    void enter() override;
    void update(float dt) override;
    void Draw(float dt);

private:
    void IrLogic();

    float logic_timer = 0;
    VoiceCmd VoiceComand;
    bool New_Comand = false;

    float timer = 0;
    bool microphone = false;
    
    DateTime _time;

    DisplayOled *display;
    SpriteOled sprite;
};