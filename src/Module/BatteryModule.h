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
    bool IsInPower() const;
    bool IsCharging() const;
    bool IsBatFull() const;
    int getBatteryPercent();
    void drawBatteryIcon(DisplayOled &display, int x, int y, int percent);
private:
    BatteryModule(){}
    uint8_t pin;
    uint8_t charge_pin;
    bool _is_charge_conect = false;
    bool _is_full = false;
    bool _is_charging = false;
    float voltage = 0.0f;
    unsigned long timer;
    unsigned long timeUpdate = 10000;
    float timer2;
    float timeUpdate2 = 500;
    const float VOLTAGE_DIVIDER = 50.0f;
};