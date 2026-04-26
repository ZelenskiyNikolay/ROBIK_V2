#pragma once
#include <Arduino.h>
#include "Display/DisplayOled.h"
#include "hardware/adc.h"
#include "hardware/irq.h"

#define BAT_CAL 1.04f

class BatteryModule {
public:
    static BatteryModule& getInstance() {
        static BatteryModule instance;
        return instance;
    }

    void begin(uint8_t pin,uint8_t charge_pin);
    void update(float dt);
    float getVoltage() const;
    bool IsChargeConect() const;
    int getBatteryPercent();
    void drawBatteryIcon(DisplayOled &display, int x, int y, int percent);
private:
    BatteryModule(){}
    float GetFilterVoltage();
    uint8_t pin;
    uint8_t charge_pin;
    bool _is_charge_conect = false;
    float voltage = 0.0f;
    unsigned long timer;
    unsigned long timeUpdate = 10000;
};