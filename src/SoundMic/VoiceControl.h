#pragma once
#include <Arduino.h>

#define SAMPLE_COUNT 16000 // 1 секунда при 16кГц

class VoiceControl {
public:
    static VoiceControl &getInstance()
    {
        static VoiceControl instance;
        return instance;
    }
    
    void begin(int pin);

    void collect_and_dump();

    // Записывает 2 сек и возвращает ID совпавшей команды (или -1)
    // int listen(int numCommands);
    // // Обучение: записывает голос и сохраняет в "золотой" массив
    // void learn(int id);

private:
    VoiceControl();
    int _pin;
    int _center =  130;
    int _noise = 5;
    int _gold_profiles[10][50]; // Память на 10 команд
    
    uint16_t sample_buffer[SAMPLE_COUNT];

    void getProfile(int* targetProfile);
};