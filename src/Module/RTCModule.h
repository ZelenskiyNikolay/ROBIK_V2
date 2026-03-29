#pragma once
#include <RTClib.h>

class RTCModule {
private:
    RTC_DS3231 rtc;
    unsigned long lastUpdate;
    float timer = 0;
    float updateInterval = 500;
    DateTime currentTime;

    RTCModule();

public:
    static RTCModule& getInstance();

    void begin();
    void setUpdateInterval(unsigned long intervalMs);
    void update(float dt);
    DateTime getTime();

    RTCModule(RTCModule const&) = delete;
    void operator=(RTCModule const&) = delete;
};