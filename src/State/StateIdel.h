#pragma once
#include "Arduino.h"
#include "Core/State.h"
#include "Display/DisplayOled.h"
#include "Display/SpriteOled.h"
#include "Core/FSM.h"
#include "Sensors/IRSensor.h"
#include "SoundMic/VoiceControl.h"

#define TIMER_MOV 15000
#define TIMER_SPEAK 20000

class StateIdel : public State
{
public:
    StateIdel(DisplayOled &disp);

    void enter() override;
    void update(float dt) override;
    void Draw(float dt);

private:
    void IrLogic();

    void AddQueue();
    float RND_DIST();
    bool Activ = false;
    float timer1;
    float timer2;

    const float Open_Eyes = 10000;
    const float Close_Eyes = 1000;
    bool IsOpen;
    float timer = 0;

    DisplayOled *display;
    SpriteOled sprite;
};