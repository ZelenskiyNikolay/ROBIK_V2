#pragma once
#include "State.h"
#include "Display/DisplayOled.h"
// #include "State/StateCalibration.h"
#include "State/StateNormal.h"
#include "State/StateStart.h" 
#include "State/StateUsb.h"
// #include "State/StateSearchBase.h"

class FSM {
public:
    FSM(State* initial, DisplayOled* disp);
    
    void Init(State *initial, DisplayOled *disp);

    void update(float dt);
    void handleEvent(Event e);
    void changeState(State* next);
    void changeStateById(StateID id);
private:
    State* current;
    DisplayOled* display;
};