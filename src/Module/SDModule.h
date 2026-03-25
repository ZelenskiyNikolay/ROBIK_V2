#pragma once
#include <Arduino.h>
#include <SPI.h>
#include <SdFat.h>

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
     void ListFiles(const char * dirname);
     // Метод для получения целого числа по ключу
    int getIntConfig(const char* key, int defaultValue);
    
    // Метод для получения float
    float getFloatConfig(const char* key, float defaultValue);
    void  CreateFile(const char* file_name);
private:
    SDModule() {}
    SdFat sd;
    bool CreateConfigFile();
};