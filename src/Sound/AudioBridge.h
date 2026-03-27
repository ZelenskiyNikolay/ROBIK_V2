#pragma once
#include <Arduino.h>


struct AudioBridge {
    static const uint32_t BUF_SIZE = 2048;
    uint8_t bufferA[BUF_SIZE];
    uint8_t bufferB[BUF_SIZE];
    
    volatile bool readyA = false; // true = буфер А полон, можно играть
    volatile bool readyB = false; // true = буфер B полон, можно играть
    volatile bool isPlaying = false;
    
    volatile int En_Pin=0;

    uint32_t totalSamples = 0;
};

extern AudioBridge audioBridge;