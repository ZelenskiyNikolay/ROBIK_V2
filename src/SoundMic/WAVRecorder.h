#pragma once
#include <Arduino.h>
#include "SDStreamWrite.h"
#include "RecordBridge.h"

class WAVRecorder {
public:
    WAVRecorder(SDStreamWrite& stream) : _stream(stream) {}

    bool start(const char* filename, uint32_t sampleRate = 16000) {
        _currentPath = filename;
        _sampleRate = sampleRate;
        _sampleCount = 0;

        // Открываем файл на запись (O_CREAT | O_TRUNC)
        if (!_stream.open(_currentPath, true)) return false;
        
        // Пишем заглушку заголовка
        writeHeader(0); 
        return true;
    }

    void tick() {
        if (recordBridge.readyA) {
            _stream.write(recordBridge.bufferA, RecordBridge::BUF_SIZE * 2);
            _sampleCount += RecordBridge::BUF_SIZE;
            recordBridge.readyA = false;
        }

        if (recordBridge.readyB) {
            _stream.write(recordBridge.bufferB, RecordBridge::BUF_SIZE * 2);
            _sampleCount += RecordBridge::BUF_SIZE;
            recordBridge.readyB = false;
        }
    }

    void stop() {
        if (_sampleCount > 0) {
            // ВАЖНО: Используем seek(0), чтобы не переоткрывать файл через TRUNC
            if (_stream.seek(0)) {
                writeHeader(_sampleCount);
            }
            _stream.close();
            Serial.printf("Saved %lu samples\n", _sampleCount);
        }
    }

private:
    SDStreamWrite& _stream;
    uint32_t _sampleCount = 0;
    uint32_t _sampleRate = 16000;
    const char* _currentPath;

    // СТРОГО: __attribute__((packed)) чтобы заголовок был ровно 44 байта
    struct __attribute__((packed)) WavHeader {
        char chunkID[4];        // "RIFF"
        uint32_t chunkSize;     // 36 + dataSize
        char format[4];         // "WAVE"
        char subChunk1ID[4];    // "fmt "
        uint32_t subChunk1Size; // 16
        uint16_t audioFormat;   // 1 (PCM)
        uint16_t numChannels;   // 1 (Mono)
        uint32_t sampleRate;
        uint32_t byteRate;
        uint16_t blockAlign;
        uint16_t bitsPerSample;
        char subChunk2ID[4];    // "data"
        uint32_t subChunk2Size; // dataSize
    };

    void writeHeader(uint32_t numSamples) {
        WavHeader h;
        uint32_t dataSize = numSamples * 2;

        memcpy(h.chunkID, "RIFF", 4);
        h.chunkSize = 36 + dataSize;
        memcpy(h.format, "WAVE", 4);
        memcpy(h.subChunk1ID, "fmt ", 4);
        h.subChunk1Size = 16;
        h.audioFormat = 1;
        h.numChannels = 1;
        h.sampleRate = _sampleRate;
        h.byteRate = _sampleRate * 2;
        h.blockAlign = 2;
        h.bitsPerSample = 16;
        memcpy(h.subChunk2ID, "data", 4);
        h.subChunk2Size = dataSize;

        _stream.write((uint16_t*)&h, sizeof(WavHeader));
    }
};