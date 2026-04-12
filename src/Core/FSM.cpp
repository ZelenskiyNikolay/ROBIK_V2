#include "FSM.h"

FSM::FSM(State *initial, DisplayOled *disp)
    : current(initial), display(disp)
{
    current->enter();
}

void FSM::Init(State *initial, DisplayOled *disp)
{
    current = initial;
    display = disp;
}

void FSM::update(float dt)
{
    if (!current)
        return;
    current->update(dt);
}

void FSM::handleEvent(Event e)
{
    switch (e.type)
    {
    case EVENT_CHANGE_STATE:
        changeStateById((StateID)e.data);
        break;

    default:
        break;
    }
}

void FSM::changeState(State *next)
{
    delete current;
    current = next;
    current->enter();
}

void FSM::changeStateById(StateID id)
{
    State *next = nullptr;
    
    current_state = id;

    switch (id)
    {
    case STATE_NORMAL: next = new StateNormal(*display);  break;
    case STATE_START:      next = new StateStart(*display);      break;
    case STATE_USB:      next = new StateUsb(*display);      break;
    case STATE_VOICE_RECORD:      next = new StateVoiceRecord(*display);      break;
    case STATE_CLOCK:      next = new StateClock(*display);      break;
    // case STATE_SLEEPY:      next = new StateSleepy(*display);      break;
    // case STATE_CLOCK:       next = new StateClock(*display);       break;
    //case STATE_SEARCH_BASE: next = new StateSearchBase(*display); break;
    //case STATE_CALIBRATION: next = new StateCalibration(*display); break;
    default:
        next = new StateNormal(*display);
        break;
    }

    if (next)
    {
        delete current; // Чистим память старого состояния
        current = next;
        current->enter();
    }
}