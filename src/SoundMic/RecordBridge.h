#pragma once
#include <Arduino.h>


struct RecordBridge {
    static const uint32_t BUF_SIZE = 2048;
    // uint16_t bufferA[BUF_SIZE];
    // uint16_t bufferB[BUF_SIZE];
    int16_t bufferA[BUF_SIZE];
    int16_t bufferB[BUF_SIZE];
    
    volatile bool readyA = false; // true = буфер А полон, можно играть
    volatile bool readyB = false; // true = буфер B полон, можно играть
    volatile bool isRecord = false;

    uint32_t totalSamples = 0;

    volatile uint32_t targetSamples = 0;
};

extern RecordBridge recordBridge;