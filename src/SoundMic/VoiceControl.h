#pragma once
#include <Arduino.h>
#include "SoundMic/RecordBridge.h"
#include "Sound/SoundManager.h"

#define SAMPLE_COUNT 16000 // 1 секунда при 16кГц
#define MAE_BUF_SIZE 80

class VoiceControl
{
public:
    static VoiceControl &getInstance()
    {
        static VoiceControl instance;
        return instance;
    }

    void begin();
    void Record_Comand();
    void Update();

    int16_t sample_buffer[SAMPLE_COUNT];
    uint16_t MAE[MAE_BUF_SIZE];
private:
    VoiceControl();
    void colect(int16_t *buffer);
    void processBuffer(int16_t *buffer);
    void Count_MAE(int16_t *buffer, size_t size);

    bool _wasRecording = false;
    bool Comand_Ready = false;
    int16_t _threshold = 200;
    int writePoint = 0;

    bool MAE_redy = false;
    uint16_t MAE_count = 0;
};