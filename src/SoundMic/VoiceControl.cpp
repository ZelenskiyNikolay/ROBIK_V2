#include "VoiceControl.h"

VoiceControl::VoiceControl() {}

void VoiceControl::begin(int pin)
{
    _pin = pin;
    analogReadResolution(12); // Pico ADC
    pinMode(_pin, INPUT);
}

void VoiceControl::collect_and_dump() {
    // 1. Сначала просто читаем в память (максимально быстро)
    // Это займет около 1.5 - 2 секунд для 16000 отсчетов на analogRead
    for (int i = 0; i < SAMPLE_COUNT; i++) {
        sample_buffer[i] = analogRead(_pin); 
    }

    // 2. И только потом выплевываем всё в консоль
    Serial.println("START_DUMP");
    for (int i = 0; i < SAMPLE_COUNT; i++) {
        Serial.println(sample_buffer[i]);
    }
    Serial.println("END_DUMP");
}

void VoiceControl::getProfile(int *targetProfile)
{
    const int window = 640;
    for (int w = 0; w < 50; w++)
    {
        long energy = 0;
        for (int i = 0; i < window; i++)
        {
            int val = (analogRead(_pin) >> 4); // Перевод в 8 бит
            int diff = abs(val - _center);
            if (diff > _noise)
                energy += diff;
        }
        targetProfile[w] = energy;
    }

    // НОРМАЛИЗАЦИЯ: ищем макс. пик и подтягиваем всё под него
    int maxVal = 1;
    for (int i = 0; i < 50; i++)
        if (targetProfile[i] > maxVal)
            maxVal = targetProfile[i];
    for (int i = 0; i < 50; i++)
        targetProfile[i] = (targetProfile[i] * 100) / maxVal;
}

// void VoiceControl::learn(int id)
// {
//     if (id < 10)
//         getProfile(_gold_profiles[id]);
// }

// int VoiceControl::listen(int numCommands)
// {
//     int currentProfile[50];
//     getProfile(currentProfile);

//     int bestMatch = -1;
//     long minError = 1000000;

//     for (int i = 0; i < numCommands; i++)
//     {
//         long error = 0;
//         for (int j = 0; j < 50; j++)
//         {
//             error += abs(currentProfile[j] - _gold_profiles[i][j]);
//         }
//         if (error < minError)
//         {
//             minError = error;
//             bestMatch = i;
//         }
//     }

//     // Если ошибка слишком большая (просто шум), возвращаем -1
//     return (minError > 2500) ? -1 : bestMatch;
// }