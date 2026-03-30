#pragma once
#include <Arduino.h>
#include <SPI.h>
#include <SdFat.h>
#include <Adafruit_TinyUSB.h>

#define SD_CS 22

class SDModule
{
public:
    static SDModule &getInstance()
    {
        static SDModule instance;
        return instance;
    }
     bool begin();
     void ListFiles();

    void  CreateFile(const char* file_name);
    SdFat* getCard() { return &sd; }
private:
    SDModule() {}
    SdFat sd;
};