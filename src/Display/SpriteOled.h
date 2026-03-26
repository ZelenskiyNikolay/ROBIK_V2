#pragma once
#include "DisplayOled.h"
#include "SpriteOled.h"

enum Emotions
{
    NORMAL,
    SLEEPY,
    ANGRY,
    SURPRISED,
    HAPPY,
    SAD,
    LEFT_LOOK,
    RIGHT_LOOK,
    BLINK,
    SHOW_TIME,
    SOOS,
    EYE_STATE_COUNT
};

class SpriteOled
{
public:
    SpriteOled() {}
    SpriteOled(DisplayOled *disp) : display(disp) {}
    void Draw(Emotions emotions);

private:
    DisplayOled *display;
    void drawEyes_Normal();
    void drawEyes_Sleepy();
    void drawEyes_Angry();
    void drawEyes_Surprised();
    void drawEyes_Happy();
    void drawEyes_Sad();
    void drawEyes_Left();
    void drawEyes_Right();
    void drawEyes_Blink();
    void drawEyes_Time();
    void displayTimeOn5110();
};