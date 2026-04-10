#pragma once
#include <Arduino.h>
#include "AudioBridge.h"
#include "SDStream.h"

class SoundManager
{
public:
    static SoundManager &getInstance()
    {
        static SoundManager instance;
        return instance;
    }
    void Play(const uint8_t *data, uint32_t size,bool header = false);
    void Play(const char* filename);
    void update(); // Вызывать в основном loop() на Core 0
    void Stop();
    void Init();
    bool Is_Playing();
private:
    SoundManager();
    int _en_pin;
    const uint8_t *_source = nullptr;
    uint32_t _totalSize = 0;
    uint32_t _cursor = 0;
    const uint32_t HEADER_SIZE = 44; // Пропуск WAV заголовка

    SDStream _sdStream;
    FileStream* _activeStream = nullptr;
    bool _isSD = false;
};