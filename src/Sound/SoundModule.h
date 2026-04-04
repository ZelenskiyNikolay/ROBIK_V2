#pragma once
#include <Arduino.h>
#include "hardware/pwm.h"
#include "hardware/irq.h"
#include "Sound/AudioBridge.h"
#include "Sound/SoundManager.h"

#define AUDIO_PIN 13
#define AUDIO_EN_PIN 14

class SoundModule
{
public:
    static SoundModule &getInstance()
    {
        static SoundModule instance;
        return instance;
    }
    static void on_pwm_irq();
    void Init();
    void Init(int Audio_pin, int en_pin);
    void handle_interrupt();
private:
    SoundModule();
    int _audio_pin = AUDIO_PIN;
    int _audio_en_pin = AUDIO_EN_PIN;
    const uint32_t HEADER_SIZE = 44;
    volatile uint32_t current_sample = HEADER_SIZE;
    const uint32_t total_samples = 72980; // Твой размер
    uint slice_num;
};