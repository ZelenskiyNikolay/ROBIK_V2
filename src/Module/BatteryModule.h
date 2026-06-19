#pragma once
#include <Arduino.h>
#include "Display/DisplayOled.h"
#include "PowerModule/I2C_Master.h"

#define BAT_CAL 1.04f

class BatteryModule {
public:
    static BatteryModule& getInstance() {
        static BatteryModule instance;
        return instance;
    }

    void begin();
    void update(float dt);
    float getVoltage() const;
    bool IsChargeConect() const;
    int getBatteryPercent();
    void drawBatteryIcon(DisplayOled &display, int x, int y, int percent);
private:
    BatteryModule(){}
    uint8_t pin;
    uint8_t charge_pin;
    bool _is_charge_conect = false;
    float voltage = 0.0f;
    unsigned long timer;
    unsigned long timeUpdate = 10000;
};