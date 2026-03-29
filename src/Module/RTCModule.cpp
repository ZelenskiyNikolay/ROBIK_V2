#include "RTCModule.h"
#include <Wire.h>
#include <RTClib.h>

RTCModule::RTCModule() : lastUpdate(0), updateInterval(1000) {}

RTCModule &RTCModule::getInstance()
{
    static RTCModule instance;
    return instance;
}

void RTCModule::begin()
{
    if (!rtc.begin())
    {
        Serial.println("RTC не найден!");
        return;
    }

    if (rtc.lostPower())
    {
        Serial.println("RTC потерял питание! Время будет сброшено на дату компиляции.");
    }
    else
    {
        Serial.println("RTC работает, время сохранено.");
    }

    currentTime = rtc.now();
    Serial.print("Текущее время: ");
    Serial.println(currentTime.timestamp());
    lastUpdate = millis();
    //Установка часов
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

void RTCModule::setUpdateInterval(unsigned long intervalMs)
{
    updateInterval = intervalMs;
}

void RTCModule::update(float dt)
{
    timer -= dt;
    if (timer < 0)
    {
        timer = updateInterval;
        currentTime = rtc.now();
    }
}

DateTime RTCModule::getTime()
{
    return currentTime;
}