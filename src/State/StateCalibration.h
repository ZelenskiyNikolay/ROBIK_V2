#pragma once
#include "Core/State.h"
#include "Display/DisplayOled.h"
#include "Display/SpriteOled.h"
#include "MOVE/MovementModule.h"
#include "Sensors/Compass.h"

class StateCalibration : public State {
public:
    StateCalibration(DisplayOled& disp);
    
    void enter() override;
    void update(float dt) override;
    void Draw(float dt);

private:
    float distance = 0;

    float timer;
    float timer1;

    int steep_calibration=0;

    void ProcessCalibration(float dt);
    
    DisplayOled* display;
    SpriteOled sprite;
};