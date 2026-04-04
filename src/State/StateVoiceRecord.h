#pragma once
#include "Arduino.h"
#include "Core/State.h"
#include "Display/DisplayOled.h"
#include "Display/SpriteOled.h"
#include "Core/FSM.h"
#include "Sensors/IRSensor.h"
#include "Sound/SoundManager.h"
#include "SoundMic/RecordBridge.h"
#include "SoundMic/WAVRecorder.h"
#include "SoundMic/SDStreamWrite.h" 

#define DISPLAY_UP_TIME 500

class StateVoiceRecord : public State
{
public:
    StateVoiceRecord(DisplayOled &disp);

    void enter() override;
    void update(float dt) override;
    void Draw(float dt);

private:
    float timer;
    float RecTime = 10000;
    bool IsRecording = false;
    void IrLogic();

    DisplayOled *display;
    SpriteOled sprite;
    SDStreamWrite mySDStream;
    WAVRecorder recorder;
};