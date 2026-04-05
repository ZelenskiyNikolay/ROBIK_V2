#pragma once
#include <Arduino.h>
#include "Display/DisplayOled.h"
#include "hardware/adc.h"
#include "hardware/irq.h"

class BatteryModule {
public:
    static BatteryModule& getInstance() {
        static BatteryModule instance;
        return instance;
    }

    void begin(uint8_t pin);
    void update(float dt);
    float getVoltage() const;
    int getBatteryPercent();
    void drawBatteryIcon(DisplayOled &display, int x, int y, int percent);
private:
    BatteryModule(){}
    uint8_t pin;
    float voltage = 0.0f;
    unsigned long timer;
    unsigned long timeUpdate = 19000;
};