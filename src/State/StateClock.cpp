#include "StateClock.h"

StateClock::StateClock(DisplayOled &disp)
    : display(&disp), sprite(&disp) {}

void StateClock::enter()
{
    display->nightMod();
}
void StateClock::update(float dt)
{
    if (VoiceControl::getInstance().Is_New_Comand)
        microphone = false;
    logic_timer -= dt;
    IrLogic();
    Draw(dt);
    VoiceControl::getInstance().Update();
    if (logic_timer < 0)
    {
        logic_timer = UPDATE_TIME;
        if (VoiceControl::getInstance().Is_New_Comand)
        {
            VoiceComand = VoiceControl::getInstance().Get_Voice_Comand();
            New_Comand = true;
        }
    }
    if (New_Comand)
    {
        if (VoiceComand == WAKE_UP)
        {
            display->nightMod(false);
            New_Comand = false;
            EventBus::push({EVENT_CHANGE_STATE, STATE_NORMAL});
        }
        New_Comand = false;
    }
}
void StateClock::IrLogic()
{

    ButtonIR tmp = IRSensor::getInstance().GetSensorState();
    switch (tmp)
    {
    case ButtonOk:
        timer = 0;
        microphone = true;
        VoiceControl::getInstance().Record_Comand();
        break;
    case ButtonHash:
        display->nightMod(false);
        EventBus::push({EVENT_CHANGE_STATE, STATE_NORMAL});
        break;

    default:
        break;
    }
}
void StateClock::Draw(float dt)
{
    timer -= dt;
    if (timer > 0)
        return;

    
    display->clear();

    if (microphone)
    {
        timer = 100;
        sprite.Draw(Emotions::MICROPHONE);
        return;
    }
    timer = DISPLAY_UP_TIME;
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