#pragma once
#include <Arduino.h>
#include "Module/PowerModule/I2C_Master.h"

class MenuClick
{
public:
    enum BeepVolume
    {
        BEEP_OFF,
        BEEP_QUIET,
        BEEP_LOUD
    };

private:
    BeepVolume vol = BEEP_QUIET;

public:
    BeepVolume getVol() { return vol; }
    void setVol(BeepVolume Volume) { vol = Volume; }
    byte getFXMenu()
    {
        switch (vol)
        {
        case BEEP_OFF:
            return FX_NONE;
        case BEEP_QUIET:
            return FX_MENU_CLICK_Q;
        case BEEP_LOUD:
            return FX_MENU_CLICK;
        default:
            return FX_NONE;
        }
    }
};