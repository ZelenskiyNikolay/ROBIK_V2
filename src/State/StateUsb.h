#pragma once
#include "Core/State.h"
#include "Display/DisplayOled.h"
#include "Module/USBModule.h"
#include "Module/SDModule.h"
#include <hardware/watchdog.h>
#include "hardware/structs/usb.h"
#include "Sensors/IRSensor.h"

#define OLED_CS 17

static bool isMounted = false;

class StateUsb : public State {
public:
    StateUsb(DisplayOled &disp);
    void enter() override;
    void update(float dt) override;

private:
    void IrLogic();
    DisplayOled *display;
    USBModule* usb = nullptr;
};